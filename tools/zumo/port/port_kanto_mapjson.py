#!/usr/bin/env python3
"""Apply Zumo Ver 4.2 changes in Kanto map.json files onto expansion *_Frlg/map.json."""
import json, os, re, subprocess, glob
P='/home/claude/zumo-port'; BASE=P+'/base'; ZUMO=P+'/zumo'; EXP='/home/claude/rh-hideout/pokeemerald-expansion'
ren=json.load(open(P+'/rename.json'))
consts=set()
for f in glob.glob(EXP+'/include/constants/*.h'):
    t=open(f,errors='ignore').read()
    consts.update(re.findall(r'#define\s+(\w+)',t)); consts.update(re.findall(r'^\s*(\w+)\s*[,=]',t,re.M))
def rn(t):
    if t in ren: return ren[t]
    if t.startswith('OBJ_EVENT_GFX_') and t+'_FRLG' in consts: return t+'_FRLG'
    if t.startswith('MUS_') and t not in consts and 'MUS_RG_'+t[4:] in consts: return 'MUS_RG_'+t[4:]
    if t.startswith('MAP_HX_'): return 'MAP_'+t[7:]
    return t
def conv(v):
    if isinstance(v,str): return re.sub(r'[A-Za-z_][A-Za-z0-9_]*', lambda m: rn(m.group(0)), v)
    if isinstance(v,dict): return {k:conv(x) for k,x in v.items()}
    if isinstance(v,list): return [conv(x) for x in v]
    return v
def key(e): return json.dumps({k:v for k,v in e.items() if k!='local_id'},sort_keys=True)
changed=subprocess.run(['git','-C',ZUMO,'diff','--name-only','ce6406d','zumo','--','data/maps'],capture_output=True,text=True).stdout.split()
names=sorted({p.split('/')[2] for p in changed if p.endswith('map.json') and not p.split('/')[2].startswith('Hx')})
for name in names:
    ep=f'{EXP}/data/maps/{name}_Frlg/map.json'
    if not os.path.exists(ep): print('MISSING',name); continue
    b=json.load(open(f'{BASE}/data/maps/{name}/map.json')); z=json.load(open(f'{ZUMO}/data/maps/{name}/map.json')); e=json.load(open(ep))
    out=dict(e); notes=[]
    for k in z:
        if k in ('object_events','warp_events','coord_events','bg_events'): continue
        if z.get(k)!=b.get(k):
            out[k]=conv(z[k]); notes.append(k)
    for k in ('object_events','warp_events','coord_events','bg_events'):
        bl=b.get(k) or []; zl=z.get(k) or []; el=e.get(k) or []
        if bl==zl: continue
        bmap={key(x):i for i,x in enumerate(bl)}
        new=[]
        for i,ze in enumerate(zl):
            if key(ze) in bmap and bmap[key(ze)]<len(el):
                new.append(el[bmap[key(ze)]])          # unchanged event: keep expansion's version
            else:
                ce=conv(ze)
                if i<len(bl) and i<len(el) and 'local_id' in el[i] and bl[i].get('x')==ze.get('x') and bl[i].get('y')==ze.get('y'):
                    ce={'local_id':el[i]['local_id'],**ce}
                new.append(ce)
        out[k]=new; notes.append(f'{k}:{len(el)}->{len(new)}')
    json.dump(out,open(ep,'w'),indent=2); open(ep,'a').write('\n')
    print(name, ' '.join(notes))
