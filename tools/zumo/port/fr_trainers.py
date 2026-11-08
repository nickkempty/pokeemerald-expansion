#!/usr/bin/env python3
"""Parse pokefirered trainers.h + trainer_parties.h (+ zumo extras) into dicts."""
import re
def parse_parties(text):
    parties={}
    for m in re.finditer(r'static const struct (\w+) (sParty_\w+)\[\]\s*=\s*\{(.*?)\n\};',text,re.S):
        mons=[]
        for b in re.finditer(r'\{([^{}]*(?:\{[^{}]*\}[^{}]*)*)\}',m.group(3)):
            body=b.group(1); d={}
            for k in ('iv','lvl','species','heldItem'):
                x=re.search(r'\.'+k+r'\s*=\s*([^,\n]+)',body)
                if x: d[k]=x.group(1).strip()
            mv=re.search(r'\.moves\s*=\s*\{([^}]*)\}',body)
            if mv: d['moves']=[t.strip() for t in mv.group(1).split(',') if t.strip()]
            if 'species' in d: mons.append(d)
        parties[m.group(2)]=mons
    return parties
def parse_trainers(text):
    tr={}
    for m in re.finditer(r'\[(TRAINER_\w+)\]\s*=\s*\{(.*?)\n    \},',text,re.S):
        body=m.group(2); d={}
        for k in ('trainerClass','encounterMusic_gender','trainerPic','doubleBattle','aiFlags'):
            x=re.search(r'\.'+k+r'\s*=\s*([^,\n]+)',body)
            if x: d[k]=x.group(1).strip()
        x=re.search(r'\.trainerName\s*=\s*_\("([^"]*)"\)',body); d['name']=x.group(1) if x else ''
        x=re.search(r'\.items\s*=\s*\{([^}]*)\}',body); d['items']=[t.strip() for t in x.group(1).split(',') if t.strip()] if x else []
        x=re.search(r'\.party\s*=\s*\w+\((sParty_\w+)\)',body) or re.search(r'(sParty_\w+)',body)
        d['partyLabel']=x.group(1) if x else None
        tr[m.group(1)]=d
    return tr
def load(root, extra=False):
    t=open(root+'/src/data/trainers.h').read(); p=open(root+'/src/data/trainer_parties.h').read()
    if extra:
        import os
        for f in ('trainers_zumo.h',):
            if os.path.exists(root+'/src/data/'+f): t+=open(root+'/src/data/'+f).read()
        for f in ('trainer_parties_zumo.h',):
            if os.path.exists(root+'/src/data/'+f): p+=open(root+'/src/data/'+f).read()
    parties=parse_parties(p); tr=parse_trainers(t)
    for k,d in tr.items(): d['party']=parties.get(d['partyLabel'],[])
    return tr
if __name__=='__main__':
    b=load('/home/claude/zumo-port/base'); z=load('/home/claude/zumo-port/zumo',True)
    ch=[k for k in z if k in b and z[k]!=b[k]]; new=[k for k in z if k not in b]
    print(len(b),len(z),'changed',len(ch),'new',new[:30])
    import json; print(json.dumps(z['TRAINER_LEADER_BROCK'],indent=0)[:600]); print(b['TRAINER_LEADER_BROCK']['party'])
