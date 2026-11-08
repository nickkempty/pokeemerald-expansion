#!/usr/bin/env python3
"""Layout generators for the Union Road (land bridge risen from the sea) maps.

Terrain is painted from a boolean land mask with the Emerald beach / shore metatiles
(gTileset_General + gTileset_Slateport -> ported as gTileset_HxSlateportP/S).
Metatile ids, collision and elevation values were taken from Emerald's Route 109.
"""
import os, random, struct

FR = os.environ.get("FR", "/home/claude/pokefirered")
EM = os.environ.get("EM", "/home/claude/pokeemerald")


def raw(mid, col=0, elev=0):
    return mid | (col << 10) | (elev << 12)


WATER = raw(368, 0, 1)
FILL = raw(292, 0, 0)
GRASS = raw(21, 0, 3)          # MB_TALL_GRASS, encounter tile
EDGE = dict(N=raw(284, 0, 3), S=raw(300, 0, 1), W=raw(291, 0, 1), E=raw(293, 0, 1),
            NW=raw(283, 0, 1), NE=raw(285, 0, 1), SW=raw(299, 0, 1), SE=raw(301, 0, 1),
            cNW=raw(317, 0, 3), cNE=raw(318, 0, 3), cSW=raw(325, 0, 1), cSE=raw(326, 0, 1))
TERRAIN_IDS = {v & 0x3FF for v in EDGE.values()} | {292}
WATER_DECOR_IDS = {336, 337, 344, 345, 370, 372, 373, 374, 378, 379, 380, 381, 382, 386, 388}
DECOR_A = [[raw(336, 1), raw(337, 1)], [raw(344, 1), raw(345, 1)]]     # swirl rock
DECOR_B = [[raw(373, 1), raw(374, 1)], [raw(381, 1), raw(382, 1)]]     # boulder
BORDER = struct.pack("<4H", 0x170, 0x170, 0x170, 0x170)


def new_mask(W, H):
    return [[False] * W for _ in range(H)]


def spans(W, H, rows):
    """rows: list of (y0, y1, x0, x1) inclusive -> land mask"""
    m = new_mask(W, H)
    for y0, y1, x0, x1 in rows:
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                m[y][x] = True
    return m


def land(m, W, H, x, y):
    if x < 0 or x >= W:
        return False
    y = max(0, min(H - 1, y))      # vertical connections: the road continues past the map edge
    return m[y][x]


def paint(m, W, H):
    out = [[WATER] * W for _ in range(H)]
    for y in range(H):
        for x in range(W):
            if not m[y][x]:
                continue
            n, s = land(m, W, H, x, y - 1), land(m, W, H, x, y + 1)
            w, e = land(m, W, H, x - 1, y), land(m, W, H, x + 1, y)
            if not n and not w:
                r = EDGE["NW"]
            elif not n and not e:
                r = EDGE["NE"]
            elif not s and not w:
                r = EDGE["SW"]
            elif not s and not e:
                r = EDGE["SE"]
            elif not n:
                r = EDGE["N"]
            elif not s:
                r = EDGE["S"]
            elif not w:
                r = EDGE["W"]
            elif not e:
                r = EDGE["E"]
            else:
                if not land(m, W, H, x - 1, y - 1):
                    r = EDGE["cNW"]
                elif not land(m, W, H, x + 1, y - 1):
                    r = EDGE["cNE"]
                elif not land(m, W, H, x - 1, y + 1):
                    r = EDGE["cSW"]
                elif not land(m, W, H, x + 1, y + 1):
                    r = EDGE["cSE"]
                else:
                    r = FILL
            out[y][x] = r
    return out


def dist_to_land(m, W, H, x, y, maxd=3):
    best = 99
    for dy in range(-maxd, maxd + 1):
        for dx in range(-maxd, maxd + 1):
            xx, yy = x + dx, y + dy
            if 0 <= xx < W and 0 <= yy < H and m[yy][xx]:
                best = min(best, max(abs(dx), abs(dy)))
    return best


def sprinkle(grid, m, W, H, rng, prob=0.5, y_from=0, y_to=None, keep_out=()):
    """2x2 water decor one tile off the shore (like the originals around Route 109)."""
    y_to = H - 1 if y_to is None else y_to
    used = set()
    for y in range(y_from, y_to):
        for x in range(0, W - 1):
            cells = [(x, y), (x + 1, y), (x, y + 1), (x + 1, y + 1)]
            if any(c in used for c in cells):
                continue
            if any(grid[cy][cx] != WATER for cx, cy in cells):
                continue
            if any((cx, cy) in keep_out for cx, cy in cells):
                continue
            d = min(dist_to_land(m, W, H, cx, cy) for cx, cy in cells)
            if d != 2:
                continue
            if rng.random() > prob * 0.12:
                continue
            blk = DECOR_A if rng.random() < 0.5 else DECOR_B
            for j in range(2):
                for i in range(2):
                    grid[y + j][x + i] = blk[j][i]
            for cx, cy in cells:
                used.add((cx, cy))
                used.add((cx + 1, cy)); used.add((cx - 1, cy)); used.add((cx, cy + 1)); used.add((cx, cy - 1))


def rect_fill(grid, x0, y0, x1, y1, val):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            grid[y][x] = val


def grass_patch(grid, x0, y0, x1, y1):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            if (grid[y][x] & 0x3FF) == 292:
                grid[y][x] = GRASS


def stamp(grid, src_layout, src_w, sx, sy, w, h, dx, dy):
    for j in range(h):
        for i in range(w):
            v = struct.unpack_from("<H", src_layout, ((sy + j) * src_w + sx + i) * 2)[0]
            grid[dy + j][dx + i] = v


# Emerald blocks loungers/parasols only through an elevation mismatch (3 vs 4), which a player standing
# on an elevation-0 sand tile bypasses. Give them a real collision bit so they are always solid.
SOLID_IDS = {737, 745, 753, 740, 741, 748, 749, 756, 757}


def to_bytes(grid):
    def fix(v):
        return v | (1 << 10) if (v & 0x3FF) in SOLID_IDS else v
    return b"".join(struct.pack("<H", fix(v)) for row in grid for v in row)


_slate = None


def slateport():
    global _slate
    if _slate is None:
        _slate = open(f"{FR}/data/layouts/HxSlateportCity/map.bin", "rb").read()
    return _slate


def stamp_center(grid, dx, dy):       # 4x4 Pokemon Center, door at (dx+1, dy+3)
    stamp(grid, slateport(), 40, 18, 16, 4, 4, dx, dy)


def stamp_mart(grid, dx, dy):         # 4x4 Poke Mart, door at (dx+1, dy+3)
    stamp(grid, slateport(), 40, 12, 23, 4, 4, dx, dy)


def stamp_house(grid, dx, dy):        # 4x4 purple-roof house, door at (dx+1, dy+3)
    stamp(grid, slateport(), 40, 20, 41, 4, 4, dx, dy)


def parasol(grid, x, y):              # 2x3 parasol with shadow (decoration, from Route 109)
    for j, row in enumerate(([740, 741], [748, 749], [756, 757])):
        for i, mid in enumerate(row):
            grid[y + j][x + i] = raw(mid, 0, 3)


# ---------------------------------------------------------------------------
# the maps
# ---------------------------------------------------------------------------
def gen_ur1():
    W, H = 32, 40
    m = spans(W, H, [(3, 8, 10, 21), (9, 11, 8, 23), (12, 18, 5, 26), (19, 21, 8, 23),
                     (22, 27, 11, 20), (28, 30, 8, 23), (31, 36, 6, 25), (37, 39, 10, 21)])
    g = paint(m, W, H)
    grass_patch(g, 8, 13, 12, 16)
    grass_patch(g, 19, 14, 23, 17)
    grass_patch(g, 9, 32, 13, 35)
    grass_patch(g, 18, 32, 22, 34)
    sprinkle(g, m, W, H, random.Random(11))
    return dict(W=W, H=H, grid=g, mask=m)


def gen_ur2():
    W, H = 32, 36
    m = spans(W, H, [(0, 4, 10, 21), (5, 7, 7, 24), (8, 29, 2, 29), (30, 32, 7, 24), (33, 35, 10, 21)])
    g = paint(m, W, H)
    stamp_center(g, 6, 11)       # door (7,14)
    stamp_mart(g, 22, 11)        # door (23,14)
    for px, py in [(12, 10), (17, 12), (4, 19), (13, 21), (24, 21), (20, 25), (7, 25)]:
        parasol(g, px, py)
    sprinkle(g, m, W, H, random.Random(12))
    return dict(W=W, H=H, grid=g, mask=m)


def gen_ur3():
    W, H = 32, 40
    m = spans(W, H, [(0, 9, 10, 21), (10, 11, 8, 21), (12, 21, 6, 17), (22, 23, 6, 21),
                     (24, 31, 14, 25), (32, 33, 10, 25), (34, 38, 8, 23)])
    g = paint(m, W, H)
    stamp_house(g, 14, 33)       # gate house, door (15,36)
    grass_patch(g, 11, 4, 15, 7)
    grass_patch(g, 8, 14, 11, 18)
    grass_patch(g, 16, 26, 22, 29)
    sprinkle(g, m, W, H, random.Random(13))
    return dict(W=W, H=H, grid=g, mask=m)


def _orig_layout109():
    return open(f"{EM}/data/layouts/Route109/map.bin", "rb").read()


def gen_ur4():
    """Route 109's beach in front of Slateport, plus a sand causeway that replaces the sea to the south."""
    W0, H0 = 40, 63
    W, H = 40, 62
    src = _orig_layout109()
    orig = [[struct.unpack_from("<H", src, (y * W0 + x) * 2)[0] for x in range(W0)] for y in range(H0)]
    KEEP_Y = 30
    m = new_mask(W, H)
    for y in range(KEEP_Y + 1):
        for x in range(W):
            mid = orig[y][x] & 0x3FF
            if mid != 368 and mid not in WATER_DECOR_IDS:
                m[y][x] = True
    for y0, y1, x0, x1 in [(22, 47, 15, 24), (48, 50, 12, 27), (51, 58, 9, 30)]:
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                m[y][x] = True
    g = paint(m, W, H)
    # put back everything that is not plain terrain in the part of Route 109 we keep (parasols, house, rocks...)
    for y in range(KEEP_Y + 1):
        for x in range(W):
            mid = orig[y][x] & 0x3FF
            if mid in TERRAIN_IDS or mid == 368:
                continue
            if mid in WATER_DECOR_IDS and m[y][x]:
                continue
            g[y][x] = orig[y][x]
    grass_patch(g, 17, 32, 22, 35)
    grass_patch(g, 16, 41, 21, 44)
    stamp_house(g, 17, 52)       # gate house, door (18,55)
    sprinkle(g, m, W, H, random.Random(14), y_from=KEEP_Y + 1)
    return dict(W=W, H=H, grid=g, mask=m)


GENS = {"ur1": gen_ur1, "ur2": gen_ur2, "ur3": gen_ur3, "ur4": gen_ur4}


def self_test():
    """re-paint Route 109's own land mask and compare the terrain tiles with the original"""
    W, H = 40, 63
    src = _orig_layout109()
    orig = [[struct.unpack_from("<H", src, (y * W + x) * 2)[0] for x in range(W)] for y in range(H)]
    m = new_mask(W, H)
    for y in range(H):
        for x in range(W):
            mid = orig[y][x] & 0x3FF
            m[y][x] = mid != 368 and mid not in WATER_DECOR_IDS
    g = paint(m, W, H)
    bad = []
    for y in range(H):
        for x in range(W):
            mid = orig[y][x] & 0x3FF
            if mid in TERRAIN_IDS and g[y][x] != orig[y][x] and (g[y][x] & 0x3FF) != 292 or \
               mid in TERRAIN_IDS and g[y][x] != orig[y][x] and mid != 292:
                bad.append((x, y, mid, g[y][x] & 0x3FF))
    print("mismatches:", len(bad))
    for b in bad[:40]:
        print(b)


if __name__ == "__main__":
    self_test()
