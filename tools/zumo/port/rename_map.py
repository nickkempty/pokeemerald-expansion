#!/usr/bin/env python3
"""Learn token renames pokefirered (base) -> pokeemerald-expansion FRLG, from scripts/text of all shared maps."""
import difflib, glob, os, re, json, collections
BASE='/home/claude/zumo-port/base'; EXP='/home/claude/rh-hideout/pokeemerald-expansion'
tok=re.compile(r'[A-Za-z_][A-Za-z0-9_]*')
pairs=collections.Counter(); keep=collections.Counter()
def read(p): return open(p,errors='ignore').read() if os.path.exists(p) else ''
for d in sorted(glob.glob(EXP+'/data/maps/*_Frlg')):
    name=os.path.basename(d)[:-5]
    b=BASE+'/data/maps/'+name
    if not os.path.isdir(b): continue
    bl=(read(b+'/scripts.inc')+'\n'+read(b+'/text.inc')).splitlines()
    el=read(d+'/scripts.inc').splitlines()
    sm=difflib.SequenceMatcher(None,bl,el,autojunk=False)
    for op,i1,i2,j1,j2 in sm.get_opcodes():
        if op=='equal':
            for l in bl[i1:i2]:
                for t in tok.findall(l): keep[t]+=1
        if op=='replace' and i2-i1==j2-j1:
            for x,y in zip(bl[i1:i2],el[j1:j2]):
                tx,ty=tok.findall(x),tok.findall(y)
                if len(tx)==len(ty):
                    for a,c in zip(tx,ty):
                        if a!=c: pairs[(a,c)]+=1
# also map.json gfx ids, by aligned object events
for d in sorted(glob.glob(EXP+'/data/maps/*_Frlg')):
    name=os.path.basename(d)[:-5]; b=BASE+'/data/maps/'+name+'/map.json'
    if not os.path.exists(b): continue
    bj=json.load(open(b)); ej=json.load(open(d+'/map.json'))
    for k in ('object_events','coord_events','bg_events','warp_events'):
        for x,y in zip(bj.get(k) or [], ej.get(k) or []):
            for f in set(x)&set(y):
                if isinstance(x[f],str) and isinstance(y[f],str) and x[f]!=y[f]:
                    pairs[(x[f],y[f])]+=1
defined=set()
for f in glob.glob(EXP+'/include/**/*.h',recursive=True)+glob.glob(EXP+'/asm/**/*.inc',recursive=True):
    t=open(f,errors='ignore').read()
    defined.update(re.findall(r'#define\s+(\w+)',t)); defined.update(re.findall(r'\.macro\s+(\w+)',t))
    defined.update(re.findall(r'^\s*(\w+)\s*[,=]',t,re.M))
for f in glob.glob(EXP+'/data/**/*.inc',recursive=True)+glob.glob(EXP+'/data/*.s'):
    defined.update(re.findall(r'^(\w+)::?',open(f,errors='ignore').read(),re.M))
best={}
for (a,c),n in pairs.most_common():
    if a in defined and not a.startswith(('OBJ_EVENT_GFX_','MUS_','MAP_')): continue      # still exists in expansion -> not a rename
    if a in best: continue
    if keep[a] > n: continue      # token mostly unchanged elsewhere -> context-specific edit, not a rename
    if not re.match(r'^[A-Z_]|^[A-Z]|^\w+_(EventScript|Text|Movement)', a) and keep[a] > 0: continue
    best[a]=(c,n)
out={a:c for a,(c,n) in best.items()}
json.dump(out,open('/home/claude/zumo-port/rename.json','w'),indent=0,sort_keys=True)
print(len(out))

