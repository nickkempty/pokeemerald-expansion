#!/usr/bin/env python3
"""3-way merge Zumo (pokefirered) Kanto map scripts+texts into expansion *_Frlg/scripts.inc.
base = pret pokefirered at ce6406d, theirs = Zumo Ver 4.2, ours = expansion. Renames learned by rename_map.py.
Writes merged files in place (expansion tree) and reports conflicts."""
import json, os, re, subprocess, sys, glob, tempfile
P='/home/claude/zumo-port'; BASE=P+'/base'; ZUMO=P+'/zumo'; EXP='/home/claude/rh-hideout/pokeemerald-expansion'
ren=json.load(open(P+'/rename.json'))
consts=set()
for f in glob.glob(EXP+'/include/constants/*.h'):
    consts.update(re.findall(r'#define\s+(\w+)',open(f,errors='ignore').read()))
    consts.update(re.findall(r'^\s*(\w+)\s*[,=]',open(f,errors='ignore').read(),re.M))
def rn(t):
    if t in ren: return ren[t]
    if t.startswith('OBJ_EVENT_GFX_') and t+'_FRLG' in consts: return t+'_FRLG'
    if t.startswith('MUS_') and t not in consts and 'MUS_RG_'+t[4:] in consts: return 'MUS_RG_'+t[4:]
    return t
def conv(s):
    out=[]
    for line in s.split('\n'):
        if '.string' in line:   # never rename inside text strings
            out.append(line); continue
        out.append(re.sub(r'[A-Za-z_][A-Za-z0-9_]*', lambda m: rn(m.group(0)), line))
    return '\n'.join(out)
def rd(p): return open(p,errors='ignore').read() if os.path.exists(p) else ''
def combined(root,name,mapname_new):
    s=rd(f'{root}/data/maps/{name}/scripts.inc'); t=rd(f'{root}/data/maps/{name}/text.inc')
    c=s.rstrip('\n')+'\n\n'+t if t else s
    c=c.replace(f'{name}_MapScripts::', f'{mapname_new}_MapScripts::')
    return conv(c)
changed=subprocess.run(['git','-C',P+'/zumo','diff','--name-only','ce6406d','HEAD','--','data/maps'],capture_output=True,text=True).stdout.split()
maps=sorted({p.split('/')[2] for p in changed if (p.endswith('scripts.inc') or p.endswith('text.inc')) and not p.split('/')[2].startswith('Hx')})
report=[]
for name in maps:
    ed=f'{EXP}/data/maps/{name}_Frlg'
    if not os.path.isdir(ed): report.append((name,'NO_EXP_MAP',0)); continue
    ours=ed+'/scripts.inc'
    with tempfile.TemporaryDirectory() as td:
        b=td+'/base'; z=td+'/zumo'
        open(b,'w').write(combined(BASE,name,name+'_Frlg')); open(z,'w').write(combined(ZUMO,name,name+'_Frlg'))
        r=subprocess.run(['git','merge-file','--theirs','-L','expansion','-L','pokefirered','-L','zumo',ours,b,z],capture_output=True,text=True)
        report.append((name,'ok' if r.returncode==0 else 'CONFLICT',r.returncode))
for n,s,c in report: print(f'{s:10} {c:3} {n}')
print('maps',len(maps),'conflicted',sum(1 for r in report if r[1]=='CONFLICT'),'missing',sum(1 for r in report if r[1]=='NO_EXP_MAP'))
