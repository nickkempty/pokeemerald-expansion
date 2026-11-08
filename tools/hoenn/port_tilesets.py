#!/usr/bin/env python3
"""Port Emerald (primary, secondary) tileset pairs into FireRed's 640/384 layout.

Emerald: primary = 512 tiles / 512 metatiles / 6 palettes, secondary follows at tile 512 / metatile 512.
FireRed: primary slot = 640 tiles / 640 metatiles / 7 palettes, secondary follows at 640.

Trick: tile numbers and metatile ids are continuous (primary then secondary), so we move the first
128 tiles / 128 metatiles / palette 6 of the Emerald secondary into the spare part of the FireRed
primary slot. Every tile number and metatile id then stays exactly the same as in Emerald, so
layouts (map.bin) and metatile data need no remapping at all.
"""
import os, re, struct, sys, shutil, json
from PIL import Image

FR = os.environ.get('FR', '/home/claude/pokefirered')
EM = os.environ.get('EM', '/home/claude/pokeemerald')

EM_PRIM_TILES = 512
EM_PRIM_META = 512
FR_PRIM_TILES = 640
FR_PRIM_META = 640
FR_SEC_TILES = 384
SPLIT = FR_PRIM_TILES - EM_PRIM_TILES  # 128

# ---------------------------------------------------------------- behaviors
def load_fr_behaviors():
    t = open(f'{FR}/include/constants/metatile_behaviors.h').read()
    return {m.group(1): int(m.group(2), 0) for m in re.finditer(r'#define\s+(MB_\w+)\s+(0x[0-9A-Fa-f]+|\d+)', t)}

def load_em_behaviors():
    t = open(f'{EM}/include/constants/metatile_behaviors.h').read()
    body = t[t.index('enum {') + 6:t.index('};')]
    names = []
    for l in body.split('\n'):
        l = l.split('//')[0].strip().rstrip(',')
        if l and not l.startswith('#'):
            names.append(l)
    return names

def load_em_flags():
    t = open(f'{EM}/src/metatile_behavior.c').read()
    flags = {}
    for m in re.finditer(r'\[(MB_\w+)\]\s*=\s*([^,]+),', t):
        f = m.group(2)
        flags[m.group(1)] = (('TILE_FLAG_HAS_ENCOUNTERS' in f), ('TILE_FLAG_SURFABLE' in f))
    return flags

FR_B = load_fr_behaviors()
EM_N = load_em_behaviors()
EM_F = load_em_flags()

# Emerald behaviors that FireRed has no (identically named) constant for.
ALIAS = {
    'MB_ANIMATED_DOOR': 'MB_WARP_DOOR', 'MB_PETALBURG_GYM_DOOR': 'MB_WARP_DOOR', 'MB_NON_ANIMATED_DOOR': 'MB_WARP_DOOR', 'MB_WATER_DOOR': 'MB_WARP_DOOR',
    'MB_INTERIOR_DEEP_WATER': 'MB_DEEP_WATER', 'MB_SOOTOPOLIS_DEEP_WATER': 'MB_DEEP_WATER',
    'MB_UNUSED_SOOTOPOLIS_DEEP_WATER': 'MB_DEEP_WATER', 'MB_UNUSED_SOOTOPOLIS_DEEP_WATER_2': 'MB_DEEP_WATER',
    'MB_NO_SURFACING': 'MB_DEEP_WATER',
    'MB_LONG_GRASS': 'MB_TALL_GRASS', 'MB_LONG_GRASS_SOUTH_EDGE': 'MB_TALL_GRASS', 'MB_ASHGRASS': 'MB_TALL_GRASS',
    'MB_DEEP_SAND': 'MB_SAND',
    'MB_JUMP_NORTHEAST': 'MB_JUMP_NORTH', 'MB_JUMP_NORTHWEST': 'MB_JUMP_NORTH',
    'MB_JUMP_SOUTHEAST': 'MB_JUMP_SOUTH', 'MB_JUMP_SOUTHWEST': 'MB_JUMP_SOUTH',
    'MB_NO_RUNNING': 'MB_RUNNING_DISALLOWED',
    'MB_LAVARIDGE_GYM_1F_WARP': 'MB_LAVARIDGE_1F_WARP',
    'MB_SHOP_SHELF': 'MB_POKEMART_SHELF',
    'MB_POKEMON_CENTER_BOOKSHELF': 'MB_BOOKSHELF', 'MB_PICTURE_BOOK_SHELF': 'MB_BOOKSHELF',
    'MB_TRASH_CAN': 'MB_TRASH_BIN', 'MB_BLUEPRINT': 'MB_BLUEPRINTS',
    'MB_MOSSDEEP_GYM_WARP': 'MB_REGULAR_WARP',
}
TALL_GRASS = {'MB_TALL_GRASS', 'MB_LONG_GRASS', 'MB_ASHGRASS', 'MB_LONG_GRASS_SOUTH_EDGE'}

def map_behavior(b):
    name = EM_N[b] if b < len(EM_N) else 'MB_NORMAL'
    if name in ALIAS:
        return name, FR_B[ALIAS[name]]
    if name in FR_B and not name.startswith('MB_UNUSED'):
        return name, FR_B[name]
    return name, FR_B['MB_NORMAL']

def convert_attr(a):
    beh = a & 0xFF
    layer = (a >> 12) & 0xF
    name, fb = map_behavior(beh)
    enc, surf = EM_F.get(name, (False, False))
    terrain = 0
    if surf and name != 'MB_WATERFALL':
        terrain = 2
    if name == 'MB_WATERFALL':
        terrain = 3
    if name in TALL_GRASS:
        terrain = 1
    encounter = 0
    if enc:
        encounter = 2 if surf else 1
    return fb | (terrain << 9) | (encounter << 24) | ((layer & 3) << 29)

# ---------------------------------------------------------------- helpers
def read_png_tiles(path):
    im = Image.open(path)
    assert im.mode == 'P' and im.width == 128, (path, im.mode, im.size)
    return im

def pad_image(im, tiles):
    rows = (tiles + 15) // 16
    if im.height >= rows * 8:
        return im
    out = Image.new('P', (128, rows * 8), 0)
    out.putpalette(im.getpalette())
    out.paste(im, (0, 0))
    return out

def sub_image(im, y0, y1):
    out = Image.new('P', (128, y1 - y0), 0)
    out.putpalette(im.getpalette())
    out.paste(im.crop((0, y0, 128, y1)), (0, 0))
    return out

def save_png(im, path):
    # make sure palette has exactly 16 entries so Pillow writes a 4-bit PNG
    pal = im.getpalette()[:48]
    pal += [0] * (48 - len(pal))
    im2 = im.copy()
    im2.putpalette(pal)
    im2.save(path, bits=4, optimize=False)

def read_pal(path):
    lines = open(path).read().split('\n')
    return lines

def convert_pair(prim, sec, frname):
    """prim/sec: names under EM data/tilesets/{primary,secondary}; frname: base name for FR dirs."""
    pd = f'{EM}/data/tilesets/primary/{prim}'
    sd = f'{EM}/data/tilesets/secondary/{sec}'
    fp = f'{FR}/data/tilesets/primary/{frname}'
    fs = f'{FR}/data/tilesets/secondary/{frname}'
    for d in (fp, fs):
        os.makedirs(d + '/palettes', exist_ok=True)

    # ---- tiles
    pim = pad_image(read_png_tiles(pd + '/tiles.png'), EM_PRIM_TILES)
    assert pim.height == 32 * 8, (prim, pim.height)
    sim = read_png_tiles(sd + '/tiles.png')
    s_tiles = (sim.height // 8) * 16
    assert s_tiles <= 512, (sec, s_tiles)
    sim = pad_image(sim, 512)
    first = sub_image(sim, 0, SPLIT // 16 * 8)               # first 128 tiles
    rest = sub_image(sim, SPLIT // 16 * 8, sim.height)       # remaining tiles
    rest = pad_image(rest, FR_SEC_TILES)
    combined = Image.new('P', (128, pim.height + first.height), 0)
    combined.putpalette(pim.getpalette())
    combined.paste(pim, (0, 0))
    combined.paste(first, (0, pim.height))
    assert combined.height == FR_PRIM_TILES // 16 * 8
    # the pixel indices are what matter, palette in PNG is irrelevant for gbagfx
    save_png(combined, fp + '/tiles.png')
    save_png(rest, fs + '/tiles.png')

    # ---- palettes: primary slots 0-5 from Emerald primary, slot 6 from Emerald secondary slot 6
    for i in range(16):
        n = f'{i:02d}.pal'
        if i < 6:
            src = f'{pd}/palettes/{n}'
        elif i == 6:
            src = f'{sd}/palettes/{n}'
        else:
            src = f'{pd}/palettes/{n}'
        shutil.copy(src, f'{fp}/palettes/{n}')
        # secondary: slots 7-12 are the Emerald secondary palettes; others are never loaded
        shutil.copy(f'{sd}/palettes/{n}' if i >= 6 else f'{pd}/palettes/{n}', f'{fs}/palettes/{n}')

    # ---- metatiles (16 bytes each, same format in both games)
    pm = open(pd + '/metatiles.bin', 'rb').read()
    pm = pm + b'\0' * (EM_PRIM_META * 16 - len(pm))
    sm = open(sd + '/metatiles.bin', 'rb').read()
    n_meta = len(sm) // 16
    assert n_meta <= 512, (sec, n_meta)
    first_m = sm[:SPLIT * 16]
    first_m += b'\0' * (SPLIT * 16 - len(first_m))
    rest_m = sm[SPLIT * 16:]
    if not rest_m:
        rest_m = b'\0' * 16
    open(fp + '/metatiles.bin', 'wb').write(pm + first_m)
    open(fs + '/metatiles.bin', 'wb').write(rest_m)

    # ---- attributes 16 -> 32 bit
    def attrs(path, count):
        d = open(path, 'rb').read()
        v = list(struct.unpack('<%dH' % (len(d) // 2), d))
        v += [0] * (count - len(v))
        return v
    pa = attrs(pd + '/metatile_attributes.bin', EM_PRIM_META)
    sa_raw = open(sd + '/metatile_attributes.bin', 'rb').read()
    sa = list(struct.unpack('<%dH' % (len(sa_raw) // 2), sa_raw))
    sa_first = (sa[:SPLIT] + [0] * SPLIT)[:SPLIT]
    sa_rest = sa[SPLIT:] or [0]
    pa_all = [convert_attr(a) for a in pa + sa_first]
    sa_all = [convert_attr(a) for a in sa_rest]
    open(fp + '/metatile_attributes.bin', 'wb').write(struct.pack('<%dI' % len(pa_all), *pa_all))
    open(fs + '/metatile_attributes.bin', 'wb').write(struct.pack('<%dI' % len(sa_all), *sa_all))
    return dict(name=frname, prim=prim, sec=sec, tiles=s_tiles, metatiles=n_meta)

def camel(s):
    return ''.join(w.capitalize() for w in s.split('_'))

def write_headers(pairs):
    out_dir = f'{FR}/src/data/tilesets'
    g, m, h = [], [], []
    for p in pairs:
        n = p['name']; C = camel(n)
        for kind, K, sec_flag in (('primary', 'P', 'FALSE'), ('secondary', 'S', 'TRUE')):
            base = f'data/tilesets/{kind}/{n}'
            sym = f'{C}{K}'
            g.append(f'const u32 gTilesetTiles_{sym}[] = INCBIN_U32("{base}/tiles.4bpp.lz");\n')
            g.append(f'const u16 gTilesetPalettes_{sym}[][16] =\n{{\n' +
                     ''.join(f'\tINCBIN_U16("{base}/palettes/{i:02d}.gbapal"),\n' for i in range(16)) + '};\n\n')
            m.append(f'const u16 gMetatiles_{sym}[] = INCBIN_U16("{base}/metatiles.bin");\n')
            m.append(f'const u32 gMetatileAttributes_{sym}[] = INCBIN_U32("{base}/metatile_attributes.bin");\n')
            h.append(f'const struct Tileset gTileset_{sym} =\n{{\n    .isCompressed = TRUE,\n    .isSecondary = {sec_flag},\n'
                     f'    .tiles = gTilesetTiles_{sym},\n    .palettes = gTilesetPalettes_{sym},\n'
                     f'    .metatiles = gMetatiles_{sym},\n    .callback = NULL,\n    .metatileAttributes = gMetatileAttributes_{sym},\n}};\n\n')
    open(f'{out_dir}/hoenn_graphics.h', 'w').write(''.join(g))
    open(f'{out_dir}/hoenn_metatiles.h', 'w').write(''.join(m))
    open(f'{out_dir}/hoenn_headers.h', 'w').write(''.join(h))

if __name__ == '__main__':
    pairs_in = json.load(open(sys.argv[1]))   # [{"prim":"general","sec":"slateport","name":"hx_slateport"}, ...]
    done = []
    for p in pairs_in:
        r = convert_pair(p['prim'], p['sec'], p['name'])
        print(r)
        done.append(r)
    write_headers(done)
