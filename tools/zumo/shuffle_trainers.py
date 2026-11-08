#!/usr/bin/env python3
"""Zumo Edition: move Kanto route/dungeon trainers to new standing spots (fixed seed, build time).

Rules
- only plain trainers (trainer_type != NONE, script starts with trainerbattle_*, no local id)
- new tile within RADIUS of the original, same terrain (land/water), same elevation, free of
  objects, warps, signs, coord triggers, ledges and doors, and not touching another object
- the move is kept only if every important tile (warps, map-edge exits, triggers, tiles next to
  items / NPCs / signs) that was reachable from another one before is still reachable afterwards
- original positions are stored in trainer_pos_orig.json so re-running always starts from vanilla
usage: shuffle_trainers.py [--restore]
"""
import json, os, random, re, struct, sys, collections, importlib.util

FR = "/home/claude/pokefirered"
HERE = os.path.dirname(os.path.abspath(__file__))
ORIG = os.path.join(HERE, "trainer_pos_orig.json")
RADIUS = 6
SEED = 0x2E40
spec = importlib.util.spec_from_file_location("hg", "/home/claude/t23/hx_goto.py")
hg = importlib.util.module_from_spec(spec); spec.loader.exec_module(hg)

EXCLUDE = {"PokemonTower_7F", "Route24", "MtMoon_B2F", "Route22", "Route23"}
PATTERN = re.compile(r"^(Route\d+(_North|_South)?|ViridianForest|MtMoon_1F|RockTunnel_1F|RockTunnel_B1F|PokemonTower_[3-6]F)$")


def terrain(beh):
    n = hg.MB.get(beh, "")
    if "WATER" in n or n in ("MB_OCEAN_WATER", "MB_POND_WATER", "MB_SEMI_DEEP_WATER", "MB_DEEP_WATER"):
        return "water"
    return "land"


def load_map(name):
    mj = json.load(open(f"{FR}/data/maps/{name}/map.json"))
    lay = hg.LAYOUTS[mj["layout"]]
    w, h = lay["width"], lay["height"]
    bd = open(os.path.join(FR, lay["blockdata_filepath"]), "rb").read()
    pri = hg.tileset_attrs(lay["primary_tileset"]); sec = hg.tileset_attrs(lay["secondary_tileset"])
    tiles = {}
    for y in range(h):
        for x in range(w):
            v = struct.unpack_from("<H", bd, (y * w + x) * 2)[0]
            mt, col, elev = v & 0x3FF, (v >> 10) & 3, v >> 12
            a = pri[mt] if mt < 640 else (sec[mt - 640] if mt - 640 < len(sec) else 0)
            beh = a & 0x1FF
            tiles[(x, y)] = (col, elev, beh)
    return mj, w, h, tiles


def passable(t):
    col, elev, beh = t
    n = hg.MB.get(beh, "")
    if terrain(beh) == "water" and "WATERFALL" not in n:
        return True
    return col == 0 and n not in hg.BLOCK_BEH


def reach(w, h, tiles, blocked, start):
    seen = {start}
    q = collections.deque([start])
    while q:
        c = q.popleft()
        for d, (dx, dy) in (("U", (0, -1)), ("D", (0, 1)), ("L", (-1, 0)), ("R", (1, 0))):
            n = (c[0] + dx, c[1] + dy)
            if not (0 <= n[0] < w and 0 <= n[1] < h) or n in seen:
                continue
            jn = hg.MB.get(tiles[n][2], "")
            if jn.startswith("MB_JUMP_"):
                if {"EAST": "R", "WEST": "L", "NORTH": "U", "SOUTH": "D"}.get(jn[8:]) == d:
                    n2 = (n[0] + dx, n[1] + dy)
                    if 0 <= n2[0] < w and 0 <= n2[1] < h and n2 not in seen and passable(tiles[n2]) and n2 not in blocked:
                        seen.add(n2); q.append(n2)
                continue
            if passable(tiles[n]) and n not in blocked:
                seen.add(n); q.append(n)
    return seen


def important_tiles(mj, w, h, tiles):
    """(sources, targets): sources = warps, triggers and one tile per map-edge exit run; targets also
    include tiles next to items / NPCs / signs."""
    src = set()
    for wv in mj.get("warp_events", []):
        src.add((wv["x"], wv["y"]))
    for c in mj.get("coord_events", []):
        src.add((c["x"], c["y"]))
    for c in mj.get("connections") or []:
        d = c["direction"]
        edge = [(x, 0) for x in range(w)] if d == "up" else [(x, h - 1) for x in range(w)] if d == "down" else \
               [(0, y) for y in range(h)] if d == "left" else [(w - 1, y) for y in range(h)]
        prev = False
        for t in edge:
            ok = passable(tiles[t])
            if ok and not prev:
                src.add(t)
            prev = ok
    tgt = set(src)
    near = [(o["x"], o["y"]) for o in mj.get("object_events", [])] + [(b["x"], b["y"]) for b in mj.get("bg_events", [])]
    for (x, y) in near:
        for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0)):
            n = (x + dx, y + dy)
            if n in tiles and passable(tiles[n]):
                tgt.add(n)
    src = {t for t in src if t in tiles and passable(tiles[t])}
    return src, tgt


def connectivity(w, h, tiles, blocked, imp):
    src, tgt = imp
    res = {}
    for a in src:
        if a in blocked:
            continue
        r = reach(w, h, tiles, blocked, a)
        res[a] = frozenset(t for t in tgt if t in r)
    return res


def script_ok(mapname, script):
    txt = open(f"{FR}/data/maps/{mapname}/scripts.inc").read() + "\n\n" + open(f"{FR}/data/scripts/trainers.inc").read()
    m = re.search(re.escape(script) + r"::\n(.*?)(?:\n\n|\Z)", txt, re.S)
    if not m:
        return False
    first = [l.strip() for l in m.group(1).split("\n") if l.strip()]
    return bool(first) and first[0].startswith(("trainerbattle_single", "trainerbattle_double"))


def main():
    names = sorted(n for n in os.listdir(f"{FR}/data/maps") if PATTERN.match(n) and n not in EXCLUDE)
    orig = json.load(open(ORIG)) if os.path.exists(ORIG) else {}
    restore = "--restore" in sys.argv
    moved = total = 0
    for name in names:
        p = f"{FR}/data/maps/{name}/map.json"
        mj = json.load(open(p))
        objs = mj.get("object_events", [])
        if name not in orig:
            orig[name] = [[o["x"], o["y"], o.get("movement_type")] for o in objs]
        for o, (ox, oy, mt) in zip(objs, orig[name]):
            o["x"], o["y"], o["movement_type"] = ox, oy, mt
        if restore:
            json.dump(mj, open(p, "w"), indent=2); open(p, "a").write("\n")
            continue
        _, w, h, tiles = load_map(name)
        rng = random.Random(f"{SEED}-{name}")
        imp = important_tiles(mj, w, h, tiles)
        warps = {(wv["x"], wv["y"]) for wv in mj.get("warp_events", [])}
        coords = {(c["x"], c["y"]) for c in mj.get("coord_events", [])}
        signs = {(b["x"], b["y"]) for b in mj.get("bg_events", [])}
        occupied = {(o["x"], o["y"]) for o in objs}
        base = connectivity(w, h, tiles, occupied, imp)
        for o in objs:
            if o.get("trainer_type", "TRAINER_TYPE_NONE") in ("TRAINER_TYPE_NONE", "0") or o.get("local_id"):
                continue
            if not script_ok(name, o["script"]):
                continue
            total += 1
            here = (o["x"], o["y"])
            col0, elev0, beh0 = tiles[here]
            cands = []
            for y in range(max(0, here[1] - RADIUS), min(h, here[1] + RADIUS + 1)):
                for x in range(max(0, here[0] - RADIUS), min(w, here[0] + RADIUS + 1)):
                    t = (x, y)
                    if t == here or abs(x - here[0]) + abs(y - here[1]) > RADIUS or abs(x - here[0]) + abs(y - here[1]) < 2:
                        continue
                    col, elev, beh = tiles[t]
                    if not passable(tiles[t]) or terrain(beh) != terrain(beh0) or elev != elev0:
                        continue
                    n = hg.MB.get(beh, "")
                    if "GRASS" in n or "DOOR" in n or "WARP" in n or "JUMP" in n or "LADDER" in n:
                        continue
                    if t in warps or t in coords or t in signs or t in imp[1]:
                        continue
                    if any((x + dx, y + dy) in occupied for dx in (-1, 0, 1) for dy in (-1, 0, 1) if (x + dx, y + dy) != here):
                        continue
                    if x in (0, w - 1) or y in (0, h - 1):
                        continue
                    # open ground only: no standing in 1-2 tile gaps or corridors
                    if sum(1 for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0)) if passable(tiles[(x + dx, y + dy)])) < 4:
                        continue
                    cands.append(t)
            rng.shuffle(cands)
            for t in cands[:25]:
                trial = (occupied - {here}) | {t}
                new = connectivity(w, h, tiles, trial, imp)
                if all(base.get(a, frozenset()) <= new.get(a, frozenset()) for a in base):
                    occupied = trial
                    o["x"], o["y"] = t
                    moved += 1
                    if o.get("movement_type", "").startswith("MOVEMENT_TYPE_FACE_"):
                        best, bl = None, -1
                        for dname, (dx, dy) in (("UP", (0, -1)), ("DOWN", (0, 1)), ("LEFT", (-1, 0)), ("RIGHT", (1, 0))):
                            k, (cx, cy) = 0, t
                            while k < 5:
                                cx, cy = cx + dx, cy + dy
                                if (cx, cy) not in tiles or not passable(tiles[(cx, cy)]) or (cx, cy) in occupied:
                                    break
                                k += 1
                            if k > bl or (k == bl and o["movement_type"].endswith(dname)):
                                best, bl = dname, k
                        o["movement_type"] = "MOVEMENT_TYPE_FACE_" + best
                    break
        json.dump(mj, open(p, "w"), indent=2); open(p, "a").write("\n")
        print(name, "done")
    json.dump(orig, open(ORIG, "w"))
    print(f"moved {moved}/{total} trainers")


if __name__ == "__main__":
    main()
