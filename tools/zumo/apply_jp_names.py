#!/usr/bin/env python3
"""Zumo: show Pokemon species names as Japanese anime names (Gen 1-3). Run from repo root."""
import glob, re
names = {}
for line in open('tools/zumo/jp_names.txt', encoding='utf-8'):
    line = line.strip()
    if line and '=' in line:
        en, jp = line.split('=', 1)
        assert len(jp) <= 12, jp
        names[en] = jp
used = set()
for f in sorted(glob.glob('src/data/pokemon/species_info/gen_*_families.h')):
    s = open(f, encoding='utf-8').read()
    def rep(m):
        en = m.group(1)
        if en in names:
            used.add(en)
            return f'.speciesName = _("{names[en]}")'
        return m.group(0)
    s2 = re.sub(r'\.speciesName = _\("([^"]+)"\)', rep, s)
    if s2 != s:
        open(f, 'w', encoding='utf-8').write(s2)
missing = set(names) - used
print(f'renamed {len(used)} species names; not found: {sorted(missing)}')
