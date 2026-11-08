#!/usr/bin/env python3
"""Port Emerald maps into the FireRed tree (Zumo Edition, Hoenn).

usage: port_maps.py [config.json]      (default tools/hoenn/maps_phase1.json)

Re-runnable: map/layout/group entries are replaced, never duplicated.
"""
import json, os, re, shutil, subprocess, sys
from hxlib import *
import port_trainers
import ur_layouts

CFG_PATH = sys.argv[1] if len(sys.argv) > 1 else f"{FR}/tools/hoenn/maps_phase1.json"
CFG = json.load(open(CFG_PATH))
ST = State()

FR_FLAGS_H = read(f"{FR}/include/constants/flags.h")
FR_VARS_H = read(f"{FR}/include/constants/vars.h")
FR_ITEMS_H = read(f"{FR}/include/constants/items.h")
FR_SONGS_H = read(f"{FR}/include/constants/songs.h")
FR_SPECIES_H = read(f"{FR}/include/constants/species.h")
FR_MOVES_H = read(f"{FR}/include/constants/moves.h")
FR_MOVEMENT_H = read(f"{FR}/include/constants/event_object_movement.h")
FR_CHARMAP = read(f"{FR}/charmap.txt")

SPRITE_DB = json.load(open(f"{FR}/tools/hoenn/sprites_db.json"))["sprites"]


def has(sym, text):
    return re.search(r"\b" + re.escape(sym) + r"\b", text) is not None


# ----------------------------------------------------------------------------
# script conversion
# ----------------------------------------------------------------------------
SIMPLE = {"lock", "lockall", "release", "releaseall", "faceplayer", "end", "return", "closemessage",
          "waitmessage", "waitbuttonpress", "waitse", "waitfanfare", "waitmovement"}
LABEL_ARGS_LAST = {"goto", "call"}
COND_PREFIXES = ("goto_if_", "call_if_")
SAFE_SPECIALS = {"DrawWholeMapView", "MauvilleGymPressSwitch", "MauvilleGymSetDefaultBarriers",
                 "MauvilleGymDeactivatePuzzle"}


class Degrade(Exception):
    pass


class Converter:
    def __init__(self, blocks, prefix="Hx", trainer_offset=0):
        self.blocks = blocks
        self.prefix = prefix
        self.trainer_offset = trainer_offset
        self.out_scripts = collections.OrderedDict()   # new label -> list of lines
        self.out_texts = collections.OrderedDict()
        self.done = set()

    def new(self, label):
        return self.prefix + label

    def conv_flag(self, tok):
        if tok.startswith("FLAG_"):
            return ST.flag(tok)
        return tok

    def conv_var(self, tok):
        if tok.startswith("VAR_"):
            if tok in ("VAR_RESULT", "VAR_FACING") or re.match(r"VAR_0x80\w\w", tok):
                return tok
            return ST.var(tok)
        return tok

    def conv_arg(self, tok):
        if tok in self.blocks:
            return self.new(tok)
        if tok.startswith("FLAG_"):
            return self.conv_flag(tok)
        if tok.startswith("VAR_"):
            return self.conv_var(tok)
        if tok.startswith("TRAINER_") and tok in port_trainers.EM_ENTRIES:
            hx, _ = port_trainers.allocate(ST, tok)
            ST.trainers[tok]["offset"] = self.trainer_offset
            return hx
        if tok.startswith("ITEM_"):
            if has(tok, FR_ITEMS_H):
                return tok
            raise Degrade("item " + tok)
        if tok.startswith(("SE_", "MUS_")):
            if has(tok, FR_SONGS_H):
                return tok
            raise Degrade("song " + tok)
        return tok

    def convert_text(self, label):
        new = self.new(label)
        if new in self.out_texts:
            return
        lines = []
        for l in self.blocks[label]:
            if not l.strip():
                continue
            s = l.strip()
            for tok in re.findall(r"\{([A-Z_0-9 ]+)\}", s):
                if "{" + tok + "}" not in FR_CHARMAP and tok.split()[0] not in FR_CHARMAP:
                    pass
            lines.append("    " + s)
        self.out_texts[new] = lines

    def convert_script(self, label):
        new = self.new(label)
        if new in self.out_scripts or new in self.done:
            return
        self.done.add(new)
        lines = []
        pending = []
        for raw in self.blocks[label]:
            cmd, args = split_cmd(raw)
            if cmd is None:
                continue
            if cmd.startswith("."):
                raise Degrade("directive " + cmd)
            if cmd in SIMPLE and not args:
                lines.append("\t" + cmd)
            elif cmd == "waitmovement":
                lines.append("\twaitmovement 0")
            elif cmd == "delay":
                lines.append("\tdelay " + args[0])
            elif cmd in ("msgbox", "message"):
                if args[0] not in self.blocks:
                    raise Degrade("msgbox target " + args[0])
                pending.append(("text", args[0]))
                typ = ""
                if len(args) > 1:
                    if args[1] not in ("MSGBOX_NPC", "MSGBOX_SIGN", "MSGBOX_DEFAULT", "MSGBOX_YESNO", "MSGBOX_AUTOCLOSE"):
                        raise Degrade("msgbox type " + args[1])
                    typ = ", " + args[1]
                lines.append(f"\t{cmd} {self.new(args[0])}{typ}")
            elif cmd in LABEL_ARGS_LAST:
                if args[0] not in self.blocks:
                    raise Degrade("goto target " + args[0])
                pending.append(("script", args[0]))
                lines.append(f"\t{cmd} {self.new(args[0])}")
            elif cmd.startswith(COND_PREFIXES):
                a = [self.conv_arg(x) for x in args]
                if args[-1] not in self.blocks:
                    raise Degrade("cond target " + args[-1])
                pending.append(("script", args[-1]))
                lines.append(f"\t{cmd} " + ", ".join(a))
            elif cmd in ("setflag", "clearflag"):
                lines.append(f"\t{cmd} {self.conv_flag(args[0])}")
            elif cmd == "setvar":
                lines.append(f"\tsetvar {self.conv_var(args[0])}, {args[1]}")
            elif cmd == "addvar":
                lines.append(f"\taddvar {self.conv_var(args[0])}, {args[1]}")
            elif cmd == "playse":
                lines.append(f"\tplayse {self.conv_arg(args[0])}")
            elif cmd == "playfanfare":
                lines.append(f"\tplayfanfare {self.conv_arg(args[0])}")
            elif cmd in ("trainerbattle_single", "trainerbattle_double"):
                a = []
                for i, x in enumerate(args):
                    if x in self.blocks:
                        pending.append(("script" if i == 3 and cmd == "trainerbattle_single" else "text", x))
                    if x == "NO_MUSIC":
                        a.append("NO_MUSIC")
                    else:
                        a.append(self.conv_arg(x))
                lines.append(f"\t{cmd} " + ", ".join(a))
            elif cmd == "finditem":
                lines.append(f"\tfinditem {self.conv_arg(args[0])}" + (f", {args[1]}" if len(args) > 1 else ""))
            elif cmd == "giveitem":
                lines.append(f"\tgiveitem {self.conv_arg(args[0])}" + (f", {args[1]}" if len(args) > 1 else ""))
            elif cmd == "special" and args[0] in SAFE_SPECIALS:
                lines.append(f"\tspecial {args[0]}")
            else:
                raise Degrade("command " + cmd)
        self.out_scripts[new] = lines
        for kind, target in pending:
            if kind == "text" and is_text_block(self.blocks[target]):
                self.convert_text(target)
            elif kind == "text":
                raise Degrade("text target is not text: " + target)
            else:
                self.convert_script(target)

    def first_text(self, label, seen=None):
        seen = seen if seen is not None else set()
        if label in seen or label not in self.blocks:
            return None
        seen.add(label)
        for raw in self.blocks[label]:
            cmd, args = split_cmd(raw)
            if cmd == "msgbox" and args and args[0] in self.blocks and is_text_block(self.blocks[args[0]]):
                return args[0]
            if cmd in ("goto", "call") and args:
                t = self.first_text(args[0], seen)
                if t:
                    return t
            if cmd and cmd.startswith(COND_PREFIXES):
                t = self.first_text(args[-1], seen)
                if t:
                    return t
        return None

    def trainer_fastpath(self, label):
        """Trainer scripts with rematch / match-call logic are reduced to a plain trainer battle."""
        cmds = [split_cmd(r) for r in self.blocks[label]]
        cmds = [c for c in cmds if c[0]]
        if cmds and cmds[0][0] == "trainerbattle" and cmds[0][1] and cmds[0][1][0] == "TRAINER_BATTLE_CONTINUE_SCRIPT" \
                and len(cmds[0][1]) >= 5:
            # gym trainers: trainerbattle CONTINUE_SCRIPT, TRAINER, LOCALID, intro, defeat, checkscript
            ca = cmds[0][1]
            cmds[0] = ("trainerbattle_single", [ca[1], ca[3], ca[4]])
        if not cmds or cmds[0][0] not in ("trainerbattle_single", "trainerbattle_double"):
            return False
        cmd, args = cmds[0]
        trainer = args[0]
        if trainer not in port_trainers.EM_ENTRIES:
            return False
        intro, defeat = args[1], args[2]
        extra = args[3] if cmd == "trainerbattle_double" else None
        # does the plain form already hold (no extra logic after the battle)?
        simple = len(cmds) == 3 and cmds[1][0] == "msgbox" and cmds[2][0] == "end" and \
            (len(args) == 3 or (cmd == "trainerbattle_double" and len(args) == 4))
        post = None
        for cand in (intro.replace("Intro", "PostBattle"), intro.replace("Intro", "PostBattle1")):
            if cand in self.blocks and is_text_block(self.blocks[cand]):
                post = cand
                break
        if simple:
            post = cmds[1][1][0]
        if post is None:
            return False
        hx, _ = port_trainers.allocate(ST, trainer)
        ST.trainers[trainer]["offset"] = self.trainer_offset
        for t in (intro, defeat, post) + ((extra,) if extra else ()):
            self.convert_text(t)
        a = [hx, self.new(intro), self.new(defeat)] + ([self.new(extra)] if extra else [])
        self.out_scripts[self.new(label)] = [f"\t{cmd} " + ", ".join(a),
                                              f"\tmsgbox {self.new(post)}, MSGBOX_AUTOCLOSE", "\tend"]
        self.done.add(self.new(label))
        return True

    def add_root(self, label):
        """Convert an Emerald root script; on failure degrade to a plain dialogue.  Returns kind."""
        if label not in self.blocks:
            return None
        if self.trainer_fastpath(label):
            return "full"
        snapshot = (dict(self.out_scripts), dict(self.out_texts), set(self.done))
        try:
            self.convert_script(label)
            return "full"
        except Degrade as e:
            self.out_scripts, self.out_texts, self.done = (collections.OrderedDict(snapshot[0]),
                                                           collections.OrderedDict(snapshot[1]), snapshot[2])
            t = self.first_text(label)
            if t is None:
                return None
            self.convert_text(t)
            self.out_scripts[self.new(label)] = [f"\tmsgbox {self.new(t)}, MSGBOX_NPC", "\tend"]
            return "degraded:" + str(e)


# ----------------------------------------------------------------------------
# helpers for map data
# ----------------------------------------------------------------------------
TRAINER_TEXT = parse_blocks(f"{EM}/data/text/trainers.inc")


def camel(name):
    return "".join(p.capitalize() for p in name.lower().split("_"))


def load_json(p):
    return json.load(open(p))


def dump_json(p, d):
    with open(p, "w") as f:
        json.dump(d, f, indent=2)
        f.write("\n")


def upd_list(lst, key, value, newitem):
    for i, it in enumerate(lst):
        if it.get(key) == value:
            lst[i] = newitem
            return
    lst.append(newitem)


def load_text_block(path):
    return read(path)



def build_custom(m):
    """Synthetic 'Emerald map' for maps that are authored here (Union Road etc.)."""
    blocks = collections.OrderedDict()
    objs = []
    for b in m.get("borrow", []):
        sd = f"{EM}/data/maps/{b['src']}"
        sj = load_json(f"{sd}/map.json")
        for fn in ("scripts.inc", "text.inc"):
            if os.path.exists(f"{sd}/{fn}"):
                for k, v in parse_blocks(f"{sd}/{fn}").items():
                    blocks.setdefault(k, v)
        o = next(o for o in sj["object_events"] if o["script"] == b["script"])
        oo = dict(o)
        oo["x"], oo["y"] = b["x"], b["y"]
        oo["elevation"] = b.get("elevation", 0)
        if "movement_type" in b:
            oo["movement_type"] = b["movement_type"]
        if "sight" in b:
            oo["trainer_sight_or_berry_tree_id"] = str(b["sight"])
        objs.append(oo)
    for o in m.get("objects", []):
        oo = {"elevation": 0, "movement_type": "MOVEMENT_TYPE_FACE_DOWN", "movement_range_x": 0, "movement_range_y": 0,
              "trainer_type": "TRAINER_TYPE_NONE", "trainer_sight_or_berry_tree_id": "0", "flag": "0"}
        oo.update(o)
        objs.append(oo)
    ej = {"id": m["id"], "map_type": m.get("map_type", "MAP_TYPE_ROUTE"), "weather": m.get("weather", "WEATHER_SUNNY"),
          "allow_cycling": m.get("allow_cycling", False), "allow_running": True, "show_map_name": True,
          "battle_scene": "MAP_BATTLE_SCENE_NORMAL",
          "connections": [], "object_events": objs,
          "warp_events": [dict(w, elevation=w.get("elevation", 0)) for w in m.get("warps", [])],
          "bg_events": m.get("signs", []), "coord_events": m.get("coords", [])}
    m["warp_targets"] = {str(i): {"map": w["dest_map"], "warp": w["dest_warp_id"]} for i, w in enumerate(m.get("warps", []))}
    if m.get("gen"):
        g = ur_layouts.GENS[m["gen"]]()
        layout = {"width": g["W"], "height": g["H"]}
        mb = ur_layouts.to_bytes(g["grid"])
        border = ur_layouts.BORDER
    else:
        layout, mb, border = None, None, None
    return ej, blocks, layout, mb, border


def main():
    maps = CFG["maps"]
    clones = CFG.get("clones", [])
    # --- emerald layouts index
    em_layouts = {l["id"]: l for l in load_json(f"{EM}/data/layouts/layouts.json")["layouts"]}
    fr_layouts = load_json(f"{FR}/data/layouts/layouts.json")
    mgroups = load_json(f"{FR}/data/maps/map_groups.json")

    # make sure our groups exist
    for g in ("gMapGroup_HxField", "gMapGroup_HxIndoor"):
        if g not in mgroups:
            mgroups[g] = []
            mgroups["group_order"].append(g)
    mgroups["gMapGroup_HxField"] = [m["name"] for m in maps if m.get("group", "field") == "field"]
    mgroups["gMapGroup_HxIndoor"] = [m["name"] for m in maps if m.get("group") == "indoor"] + \
                                    [c["name"] for c in clones]
    for m in maps + clones:
        if m["name"] not in mgroups["connections_include_order"]:
            mgroups["connections_include_order"].append(m["name"])

    scripts_inc = []
    text_inc = []
    wild_new = []
    em_wild = load_json(f"{EM}/src/data/wild_encounters.json")["wild_encounter_groups"][0]["encounters"]
    all_gfx_missing = set()
    report = []

    for m in maps:
        name = m["name"]
        if m.get("custom"):
            ej, blocks, layout, mb_bytes, border_bytes = build_custom(m)
        else:
            src = m["src"]
            ej = load_json(f"{EM}/data/maps/{src}/map.json")
            blocks = parse_blocks(f"{EM}/data/maps/{src}/scripts.inc")
            if os.path.exists(f"{EM}/data/maps/{src}/text.inc"):
                for k, v in parse_blocks(f"{EM}/data/maps/{src}/text.inc").items():
                    blocks.setdefault(k, v)
        for xs in m.get("extra_blocks_from", []):
            for k, v in parse_blocks(f"{EM}/data/maps/{xs}/scripts.inc").items():
                blocks.setdefault(k, v)
        for k, v in parse_blocks(f"{EM}/data/scripts/item_ball_scripts.inc").items():
            blocks.setdefault(k, v)
        for k, v in TRAINER_TEXT.items():
            blocks.setdefault(k, v)
        conv = Converter(blocks, trainer_offset=m.get("trainer_offset", 32))
        for et in m.get("extra_trainers", []):
            port_trainers.allocate(ST, et)
            ST.trainers[et]["offset"] = m.get("trainer_offset", 32)
        if not m.get("custom"):
            layout = em_layouts[ej["layout"]]
            mb_bytes = open(f"{EM}/{layout['blockdata_filepath']}", "rb").read()
            border_bytes = open(f"{EM}/{layout['border_filepath']}", "rb").read()
        # ---- layout ----
        tp, ts = m["tilesets"]
        ldir = f"{FR}/data/layouts/{name}"
        if layout is not None:
            os.makedirs(ldir, exist_ok=True)
            mb = bytearray(mb_bytes)
            open(f"{ldir}/border.bin", "wb").write(border_bytes)
            W, H = layout["width"], layout["height"]
        else:
            mb = bytearray()
            W = H = 0
        # ---- warps ----
        new_warps = []
        warp_new_index = {}
        locked = []
        for i, w in enumerate(ej["warp_events"]):
            dm = w["dest_map"]
            tgt = m.get("warp_targets", {}).get(str(i))
            if tgt:
                nw = dict(w)
                nw["dest_map"] = tgt["map"]
                nw["dest_warp_id"] = str(tgt.get("warp", 0))
                warp_new_index[i] = len(new_warps)
                new_warps.append(nw)
            else:
                locked.append(w)
        _unl = {tuple(u) for u in m.get("unlock_warps", [])}
        locked = [w for w in locked if (w["x"], w["y"]) not in _unl]
        for w in locked:
            off = (w["y"] * W + w["x"]) * 2
            v = mb[off] | (mb[off + 1] << 8)
            v = (v & ~0x0C00) | (1 << 10)       # impassable
            mb[off] = v & 0xFF
            mb[off + 1] = v >> 8
        for tp_ in m.get("tile_patch", []):      # copy a tile entry (metatile+collision+elevation) onto a rectangle
            fx, fy = tp_["from"]
            for yy in range(tp_["y0"], tp_["y1"] + 1):
                for xx in range(tp_["x0"], tp_["x1"] + 1):
                    sx, sy = (fx if not tp_.get("keep_x") else xx), (fy if not tp_.get("keep_y") else yy)
                    so = (sy * W + sx) * 2
                    do = (yy * W + xx) * 2
                    mb[do], mb[do + 1] = mb[so], mb[so + 1]
        for ts_ in m.get("tile_set", []):        # force a metatile (FR id) on a tile, keep elevation, clear collision
            do = (ts_["y"] * W + ts_["x"]) * 2
            v = mb[do] | mb[do + 1] << 8
            v = (v & 0xF000) | ((ts_.get("col", 0) & 3) << 10) | (ts_["meta"] & 0x3FF)
            mb[do], mb[do + 1] = v & 0xFF, v >> 8
        if layout is not None:
            open(f"{ldir}/map.bin", "wb").write(bytes(mb))
            lay = {
                "id": "LAYOUT_" + m["id"][4:],
                "name": f"{name}_Layout",
                "width": W, "height": H, "border_width": 2, "border_height": 2,
                "primary_tileset": tp, "secondary_tileset": ts,
                "border_filepath": f"data/layouts/{name}/border.bin",
                "blockdata_filepath": f"data/layouts/{name}/map.bin",
            }
            upd_list(fr_layouts["layouts"], "id", lay["id"], lay)
        else:
            lay = {"id": m["layout_existing"]}

        # ---- connections ----
        conns = []
        if m.get("custom"):
            conns = [dict(c) for c in m.get("conns", [])]
        for c in ej.get("connections") or []:
            tgt = m.get("connections", {}).get(c["map"])
            if tgt:
                cc = dict(c)
                cc["map"] = tgt
                conns.append(cc)

        # ---- script overrides ----
        ov_path = f"{FR}/tools/hoenn/overrides/{name}.inc"
        ov_text = read(ov_path) if os.path.exists(ov_path) else ""
        ov_labels = set(re.findall(r"^(\w+)::", ov_text, re.M))
        for tok in set(re.findall(r"\bTRAINER_HX_(\w+)", ov_text)):
            conv.conv_arg("TRAINER_" + tok)
        # texts referenced by the override are pulled in from the Emerald map
        for tok in set(re.findall(r"\bHx(\w+)", ov_text)):
            if tok in blocks and is_text_block(blocks[tok]):
                conv.convert_text(tok)

        # ---- objects ----
        objs = []
        for o in ej["object_events"]:
            gfx = o["graphics_id"]
            if o["script"] in ("0x0", "0") or gfx.startswith("OBJ_EVENT_GFX_VAR"):
                continue
            hide = o["flag"].startswith("FLAG_HIDE_")
            keep = o["script"] in m.get("keep_hidden", [])
            if hide and not keep:
                continue
            if o["script"] in m.get("drop_scripts", []):
                continue
            if gfx in ("OBJ_EVENT_GFX_BERRY_TREE", "OBJ_EVENT_GFX_PROF_BIRCH", "OBJ_EVENT_GFX_SCOTT",
                       "OBJ_EVENT_GFX_WALLY"):
                continue
            hxgfx = "OBJ_EVENT_GFX_HX_" + gfx[len("OBJ_EVENT_GFX_"):]
            if o.get("fr_gfx"):
                hxgfx = gfx
            elif gfx not in SPRITE_DB:
                all_gfx_missing.add(gfx)
                continue
            final = conv.new(o["script"])
            if final in ov_labels:
                kind = "override"
            else:
                kind = conv.add_root(o["script"])
            if kind is None:
                report.append(f"{name}: dropped {gfx} {o['script']} (no dialogue)")
                continue
            if kind.startswith("degraded"):
                report.append(f"{name}: degraded {o['script']}: {kind[9:]}")
            oo = dict(o)
            oo.pop("fr_gfx", None)
            oo["graphics_id"] = hxgfx
            oo["script"] = final
            oo["type"] = "object"
            if o["flag"].startswith("FLAG_ITEM_"):
                oo["flag"] = ST.flag(o["flag"])
            elif o["flag"] not in ("0", "FLAG_HIDE_MAUVILLE_GYM_WATTSON") and not keep:
                oo["flag"] = ST.flag(o["flag"])
            else:
                oo["flag"] = "0"
            if o["script"] in m.get("set_flags", {}):
                oo["flag"] = ST.flag(m["set_flags"][o["script"]])
            if o["script"] in m.get("move_objects", {}):
                oo["x"], oo["y"] = m["move_objects"][o["script"]]
            if o["movement_type"].replace("MOVEMENT_TYPE_", "MOVEMENT_TYPE_") not in FR_MOVEMENT_H:
                oo["movement_type"] = "MOVEMENT_TYPE_FACE_DOWN"
            # remove Emerald-only keys
            oo.pop("local_id", None)
            if o.get("local_id") and o["local_id"] in m.get("keep_local_ids", {}):
                oo["local_id"] = m["keep_local_ids"][o["local_id"]]
            objs.append(oo)

        # ---- extra objects (hand-placed, work for non-custom maps too) ----
        for eo in m.get("extra_objects", []):
            oo = {"elevation": 0, "movement_type": "MOVEMENT_TYPE_FACE_DOWN", "movement_range_x": 0, "movement_range_y": 0,
                  "trainer_type": "TRAINER_TYPE_NONE", "trainer_sight_or_berry_tree_id": "0", "flag": "0", "type": "object"}
            oo.update(eo)
            if oo["flag"] != "0":
                oo["flag"] = ST.flag(oo["flag"])
            objs.append(oo)

        # ---- bg events ----
        bgs = []
        for b in ej.get("bg_events", []):
            if b["type"] == "sign":
                sc = b["script"]
                mapped = m.get("sign_map", {}).get(sc)
                if mapped:
                    bb = dict(b); bb["script"] = mapped; bgs.append(bb); continue
                final = conv.new(sc)
                if final in ov_labels:
                    kind = "override"
                else:
                    kind = conv.add_root(sc)
                if kind is None:
                    continue
                bb = dict(b)
                bb["script"] = final
                bgs.append(bb)
            elif b["type"] == "hidden_item":
                try:
                    flag = ST.hidden_flag(b["flag"])
                except SystemExit:
                    report.append(f"{name}: no hidden-item flag left for {b['item']}")
                    continue
                if not has(b["item"], FR_ITEMS_H):
                    continue
                bb = dict(b)
                bb["flag"] = flag
                bb["quantity"] = b.get("quantity", 1)
                bb["underfoot"] = b.get("underfoot", False)
                bb.pop("player_facing_dir", None)
                bgs.append(bb)
        for w in locked:
            bgs.append({"type": "sign", "x": w["x"], "y": w["y"], "elevation": w.get("elevation", 0),
                        "player_facing_dir": "BG_EVENT_PLAYER_FACING_ANY", "script": "HxCommon_EventScript_Locked"})

        # ---- coord events (only those with converted scripts) ----
        coords = []
        for c in ej.get("coord_events", []):
            if "script" not in c:
                continue
            final = conv.new(c["script"])
            if final in ov_labels:
                cc = dict(c)
                cc["script"] = final
                if cc.get("var"):
                    cc["var"] = ST.var(cc["var"])
                coords.append(cc)

        for c in m.get("extra_coords", []):
            cc = {"type": "trigger", "elevation": 0, "var": "0", "var_value": "0"}
            cc.update(c)
            if cc["var"] != "0":
                cc["var"] = ST.var(cc["var"])
            coords.append(cc)

        # ---- wild encounters ----
        for e in em_wild:
            if e.get("map") == (m.get("wild_from") or ej["id"]):
                ne = json.loads(json.dumps(e))
                ne["map"] = m["id"]
                ne["base_label"] = "gHx" + (name[2:] if m.get("custom") else e["base_label"][1:])
                off = m.get("wild_offset", 30)
                for key in ("land_mons", "water_mons", "rock_smash_mons", "fishing_mons"):
                    if key in ne:
                        for mon in ne[key]["mons"]:
                            mon["min_level"] = max(2, min(100, mon["min_level"] + off))
                            mon["max_level"] = max(2, min(100, mon["max_level"] + off))
                for key in ("land_mons", "water_mons", "rock_smash_mons", "fishing_mons"):
                    if key in m.get("wild_replace", {}):
                        ne[key] = m["wild_replace"][key]
                    if key in m.get("wild_drop", []):
                        ne.pop(key, None)
                wild_new.append(ne)

        # ---- trainer level offsets ----
        for em_t, info in ST.trainers.items():
            pass

        # ---- map.json ----
        out = {
            "id": m["id"], "name": name, "layout": lay["id"],
            "music": m["music"], "region_map_section": m["mapsec"],
            "requires_flash": False, "weather": ej.get("weather", "WEATHER_NONE"),
            "map_type": ej["map_type"],
            "allow_cycling": ej.get("allow_cycling", False), "allow_escaping": False,
            "allow_running": ej.get("allow_running", True), "show_map_name": ej.get("show_map_name", True),
            "floor_number": 0, "battle_scene": ej.get("battle_scene", "MAP_BATTLE_SCENE_NORMAL"),
        }
        if conns:
            out["connections"] = conns
        out["object_events"] = objs
        out["warp_events"] = new_warps
        out["coord_events"] = coords
        out["bg_events"] = bgs
        os.makedirs(f"{FR}/data/maps/{name}", exist_ok=True)
        dump_json(f"{FR}/data/maps/{name}/map.json", out)

        # ---- scripts / text ----
        s = [f"{name}_MapScripts::\n"]
        s.append(m.get("map_scripts", "\t.byte 0\n") + "\n")
        if ov_text:
            s.append(ov_text.rstrip() + "\n\n")
        for lab, lines in conv.out_scripts.items():
            s.append(f"{lab}::\n" + "\n".join(lines) + "\n\n")
        write(f"{FR}/data/maps/{name}/scripts.inc", "".join(s))
        t = []
        for lab, lines in conv.out_texts.items():
            t.append(f"{lab}::\n" + "\n".join(lines) + "\n\n")
        write(f"{FR}/data/maps/{name}/text.inc", "".join(t))
        scripts_inc.append(f'\t.include "data/maps/{name}/scripts.inc"\n')
        text_inc.append(f'\t.include "data/maps/{name}/text.inc"\n')

    # ---------------- clones (Pokemon Center / Mart) ----------------
    for c in clones:
        name = c["name"]
        srcj = load_json(f"{FR}/data/maps/{c['frsrc']}/map.json")
        out = json.loads(json.dumps(srcj))
        out["id"] = c["id"]
        out["name"] = name
        out["region_map_section"] = c["mapsec"]
        _pw = c["parent_warp"]
        _pn = next((mm["name"] for mm in maps if mm["id"] == c["parent"]), None)
        if _pn:
            _pj = load_json(f"{FR}/data/maps/{_pn}/map.json")
            for _i, _w in enumerate(_pj["warp_events"]):
                if _w["dest_map"] == c["id"]:
                    _pw = _i
                    break
        for w in out["warp_events"]:
            if w["dest_map"] == c["frsrc_parent"]:
                w["dest_map"] = c["parent"]
                w["dest_warp_id"] = str(_pw)
            elif c.get("kind") == "center" and w["dest_map"].endswith("POKEMON_CENTER_2F"):
                w["dest_map"] = c["id"].replace("1F", "2F") if c.get("with_2f") else w["dest_map"]
        for o in out["object_events"]:
            if "local_id" in o:
                o["local_id"] = o["local_id"]
        scr = read(f"{FR}/data/maps/{c['frsrc']}/scripts.inc")
        txt = read(f"{FR}/data/maps/{c['frsrc']}/text.inc")
        old = c["frsrc"]
        scr = scr.replace(old + "_", name + "_")
        txt = txt.replace(old + "_", name + "_")
        for o in out["object_events"]:
            o["script"] = o["script"].replace(old + "_", name + "_")
        if c.get("kind") == "center":
            scr = scr.replace(c["frheal"], c["heal"])
            # drop stairs warp to 2F (not ported)
            out["warp_events"] = [w for w in out["warp_events"] if not w["dest_map"].endswith("POKEMON_CENTER_2F")]
            for o in out["object_events"]:
                if o.get("local_id"):
                    o["local_id"] = c["nurse_localid"]
                    break
        if c.get("kind") == "mart":
            scr = re.sub(r"(\w+_Items)::\n(?:\t\.2byte \w+\n)+", lambda mm: mm.group(1) + "::\n" + "".join(f"\t.2byte {i}\n" for i in c["items"]) + "\t.2byte ITEM_NONE\n", scr)
        os.makedirs(f"{FR}/data/maps/{name}", exist_ok=True)
        dump_json(f"{FR}/data/maps/{name}/map.json", out)
        write(f"{FR}/data/maps/{name}/scripts.inc", scr)
        write(f"{FR}/data/maps/{name}/text.inc", txt)
        scripts_inc.append(f'\t.include "data/maps/{name}/scripts.inc"\n')
        text_inc.append(f'\t.include "data/maps/{name}/text.inc"\n')

    # ---------------- common hx script file ----------------
    common = read(f"{FR}/tools/hoenn/hx_common.inc")
    write(f"{FR}/data/hx_scripts.inc", common + "\n" + "".join(scripts_inc))
    write(f"{FR}/data/hx_text.inc", "".join(text_inc))

    # ---------------- write global files ----------------
    dump_json(f"{FR}/data/layouts/layouts.json", fr_layouts)
    dump_json(f"{FR}/data/maps/map_groups.json", mgroups)

    # wild encounters
    wp = f"{FR}/src/data/wild_encounters.json"
    wj = load_json(wp)
    grp = wj["wild_encounter_groups"][0]
    grp["encounters"] = [e for e in grp["encounters"] if not e["map"].startswith("MAP_HX_")] + wild_new
    dump_json(wp, wj)

    # mapsecs
    rp = f"{FR}/src/data/region_map/region_map_sections.json"
    rj = load_json(rp)
    secs = rj["map_sections"]
    for sec in CFG["mapsecs"]:
        ent = {"id": sec["id"], "name": sec["name"], "x": 0, "y": 0, "width": 0, "height": 0}
        upd_list(secs, "id", sec["id"], ent)
    # keep MAPSEC_SPECIAL_AREA last among the vanilla ones -- ours go after it
    dump_json(rp, rj)

    # heal locations
    hp = f"{FR}/src/data/heal_locations.json"
    hj = load_json(hp)
    for h in CFG["heal_locations"]:
        upd_list(hj["heal_locations"], "id", h["id"], h)
    dump_json(hp, hj)

    for m2 in maps + clones:
        for fn in ("scripts.inc", "text.inc"):
            txt = read(f"{FR}/data/maps/{m2['name']}/{fn}")
            for x in set(re.findall(r"\bFLAG_HX_(\w+)", txt)):
                ST.flag("FLAG_" + x)
            for x in set(re.findall(r"\bVAR_HX_(\w+)", txt)):
                ST.var("VAR_" + x)
    for f in CFG.get("extra_flags", []):
        ST.flag(f)
    ST.write_headers()
    port_trainers.write_trainers(ST)
    ST.save()

    # event_scripts.s includes
    ep = f"{FR}/data/event_scripts.s"
    es = read(ep)
    if 'data/hx_scripts.inc' not in es:
        es = es.replace('\t.include "data/maps/PalletTown/scripts.inc"', '\t.include "data/hx_scripts.inc"\n\t.include "data/maps/PalletTown/scripts.inc"', 1)
        es = es.replace('\t.include "data/maps/PalletTown/text.inc"', '\t.include "data/hx_text.inc"\n\t.include "data/maps/PalletTown/text.inc"', 1)
        write(ep, es)

    if all_gfx_missing:
        print("missing sprites:", sorted(all_gfx_missing))
    for r in report:
        print(r)


if __name__ == "__main__":
    main()
