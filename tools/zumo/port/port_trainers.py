#!/usr/bin/env python3
"""Apply Zumo Ver 4.2 Kanto trainer changes to expansion src/data/trainers_frlg.party; append new Zumo trainers."""
import re, sys, collections
sys.path.insert(0,'/home/claude/zumo-port/tools')
from fr_trainers import load
EXP='/home/claude/rh-hideout/pokeemerald-expansion'
PARTY=EXP+'/src/data/trainers_frlg.party'
base=load('/home/claude/zumo-port/base'); zumo=load('/home/claude/zumo-port/zumo',True)
text=open(PARTY).read()
blocks=re.split(r'(?m)^(?==== )',text)
order=[]; ent={}
for b in blocks:
    m=re.match(r'=== (\w+) ===\n',b)
    if not m: order.append(('raw',b)); continue
    tid=m.group(1); order.append(('t',tid)); ent[tid]=b
def header(b):
    h=b.split('\n\n')[0]
    return dict(re.findall(r'(?m)^([A-Za-z ]+): (.*)$',h))
# learn mappings from base C -> expansion .party
learn=collections.defaultdict(collections.Counter)
for tid,b in ent.items():
    if tid not in base: continue
    h=header(b); c=base[tid]
    learn['class'][(c.get('trainerClass'),h.get('Class'))]+=1
    learn['pic'][(c.get('trainerPic'),h.get('Pic'))]+=1
    mg=c.get('encounterMusic_gender','')
    mus=re.sub(r'F_TRAINER_FEMALE\s*\|\s*','',mg).strip()
    learn['music'][(mus,h.get('Music'))]+=1
    learn['ai'][(c.get('aiFlags'),h.get('AI'))]+=1
def best(kind):
    d={}
    for (a,b),n in learn[kind].most_common():
        if a not in d: d[a]=b
    return d
M={k:best(k) for k in ('class','pic','music','ai')}
def item_name(i): return i
def mons_text(party):
    out=[]
    for p in party:
        sp=p['species']; line=sp
        if p.get('heldItem') and p['heldItem']!='ITEM_NONE': line+=' @ '+p['heldItem']
        iv=int(p.get('iv','0'),0)*31//255
        s=[line,f"Level: {p['lvl']}",f"IVs: {iv} HP / {iv} Atk / {iv} Def / {iv} SpA / {iv} SpD / {iv} Spe"]
        for mv in p.get('moves',[]):
            if mv!='MOVE_NONE': s.append('- '+mv)
        out.append('\n'.join(s))
    return '\n\n'.join(out)
def make_header(tid,c,old=None):
    h=dict(old or {})
    if old is None or c.get('name')!=base.get(tid,{}).get('name'): h['Name']=c.get('name','') or h.get('Name','')
    def setf(field,kind,val,prefix):
        if val is None: return
        if old is not None and tid in base and base[tid].get(field)==c.get(field): return
        v=M[kind].get(val) or val
        if kind=='ai' and v==val:
            v=' / '.join(t.strip()[len('AI_SCRIPT_'):].replace('_',' ').title() for t in val.split('|'))
        h[{'class':'Class','pic':'Pic','music':'Music','ai':'AI'}[kind]]=v
    setf('trainerClass','class',c.get('trainerClass'),'TRAINER_CLASS')
    setf('trainerPic','pic',c.get('trainerPic'),'TRAINER_PIC')
    mg=c.get('encounterMusic_gender','')
    if old is None or tid not in base or base[tid].get('encounterMusic_gender')!=mg:
        mus=re.sub(r'F_TRAINER_FEMALE\s*\|\s*','',mg).strip()
        if mus: h['Music']=M['music'].get(mus) or mus
        h['Gender']='Female' if 'F_TRAINER_FEMALE' in mg else 'Male'
    setf('aiFlags','ai',c.get('aiFlags'),'AI')
    if c.get('doubleBattle'): h['Double Battle']='Yes' if c['doubleBattle']=='TRUE' else 'No'
    if old is None or tid not in base or base[tid].get('items')!=c.get('items'):
        its=[i for i in c.get('items',[]) if i!='ITEM_NONE']
        if its: h['Items']=' / '.join(its)
        else: h.pop('Items',None)
    keys=['Name','Class','Pic','Gender','Music','Items','Double Battle','AI']
    return '\n'.join(f'{k}: {h[k]}' for k in keys if k in h and h[k] is not None)
changed=[t for t in zumo if t in base and zumo[t]!=base[t] and t in ent]
for tid in changed:
    old=header(ent[tid])
    ent[tid]=f'=== {tid} ===\n'+make_header(tid,zumo[tid],old)+'\n\n'+mons_text(zumo[tid]['party'])+'\n\n'
new=[t for t in zumo if t not in base]
out=''.join(ent[x] if k=='t' else x for k,x in order)
if not out.endswith('\n\n'): out+='\n'
for tid in new:
    out+=f'=== {tid} ===\n'+make_header(tid,zumo[tid])+'\n\n'+mons_text(zumo[tid]['party'])+'\n\n'
open(PARTY,'w').write(out)
print('changed',len(changed),'new',len(new))
print({k:len(v) for k,v in M.items()}); print(M['music'])
