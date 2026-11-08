import json, re
p='/home/claude/pokefirered/tools/hoenn/maps_phase1.json'
c=json.load(open(p))
NEW=['HxPetalburgCity','HxPetalburgCity_Gym']
c['maps']=[m for m in c['maps'] if m['name'] not in NEW]
c['clones']=[k for k in c['clones'] if not k['name'].startswith('HxPetalburgCity')]
c['mapsecs']=[x for x in c['mapsecs'] if 'PETALBURG' not in x['id']]
c['heal_locations']=[h for h in c['heal_locations'] if 'PETALBURG' not in h['id']]
EM='/home/claude/pokeemerald/data/maps'
def mk(src,name,id_,tsp,tss,mapsec,music,to,wo,**kw):
    d={"src":src,"name":name,"id":id_,"group":"field","tilesets":[tsp,tss],"mapsec":mapsec,"music":music,"trainer_offset":to,"wild_offset":wo}
    d.update(kw); c['maps'].append(d)
mk('PetalburgCity','HxPetalburgCity','MAP_HX_PETALBURG_CITY','gTileset_HxPetalburgP','gTileset_HxPetalburgS','MAPSEC_HX_PETALBURG_CITY','MUS_VERMILLION',38,36,
   connections={"MAP_ROUTE104":"MAP_HX_ROUTE104"},
   warp_targets={"2":{"map":"MAP_HX_PETALBURG_CITY_GYM","warp":0},"3":{"map":"MAP_HX_PETALBURG_CITY_POKEMON_CENTER_1F","warp":0},"5":{"map":"MAP_HX_PETALBURG_CITY_MART","warp":0}},
   sign_map={"Common_EventScript_ShowPokemonCenterSign":"EventScript_PokecenterSign","Common_EventScript_ShowPokemartSign":"EventScript_PokemartSign"},
   drop_scripts=["EventScript_CutTree"],custom_coords=True)
# gym
d=json.load(open(f'{EM}/PetalburgCity_Gym/map.json'))
wt={}
for i,w in enumerate(d['warp_events']):
    dm=w['dest_map']
    wt[str(i)]={"map":"MAP_HX_PETALBURG_CITY","warp":0} if dm=='MAP_PETALBURG_CITY' else {"map":"MAP_HX_"+dm[4:],"warp":int(w['dest_warp_id'])}
sc=open('/home/claude/pokeemerald/data/maps/PetalburgCity_Gym/scripts.inc').read()
doors={}
for m in re.finditer(r'setmetatile (\d+), (\d+), METATILE_PetalburgGym_RoomEntrance_(Left|Right), FALSE',sc):
    doors[(int(m.group(1)),int(m.group(2)))]=528 if m.group(3)=="Left" else 529
ts=[{"x":x,"y":y,"meta":v} for (x,y),v in sorted(doors.items())]
DOORS=[(1,104),(7,104),(1,78),(7,78),(1,91),(7,91),(7,39),(1,52),(7,52),(1,65),(7,13),(1,26)]
for x,y in DOORS:
    ts.append({"x":x,"y":y,"meta":540,"col":3})
    ts.append({"x":x,"y":y+1,"meta":548,"col":3})
sign_scripts=[b['script'] for b in d['bg_events'] if b['script'].endswith('Door')]
mk('PetalburgCity_Gym','HxPetalburgCity_Gym','MAP_HX_PETALBURG_CITY_GYM','gTileset_HxPetalburgGymP','gTileset_HxPetalburgGymS','MAPSEC_HX_PETALBURG_CITY','MUS_GYM',40,36,
   warp_targets=wt,drop_scripts=["EventScript_CutTree"]+sign_scripts,extra_trainers=["TRAINER_NORMAN_1"],tile_set=ts,keep_hidden=["PetalburgCity_Gym_EventScript_Norman","PetalburgCity_Gym_EventScript_GymGuide"])
for m in c['maps']:
    if m['name']=='HxRoute104': m['connections']["MAP_PETALBURG_CITY"]="MAP_HX_PETALBURG_CITY"
c['mapsecs'].append({"id":"MAPSEC_HX_PETALBURG_CITY","name":"PETALBURG CITY"})
c['heal_locations'].append({"id":"HEAL_LOCATION_HX_PETALBURG_CITY","map":"MAP_HX_PETALBURG_CITY","x":20,"y":17,"respawn_map":"MAP_HX_PETALBURG_CITY_POKEMON_CENTER_1F","respawn_npc":"LOCALID_HX_PETALBURG_NURSE"})
rb=[k for k in c['clones'] if k['name']=='HxRustboroCity_Mart'][0]
dc=[k for k in c['clones'] if k['name']=='HxDewfordTown_PokemonCenter_1F'][0]
mart=dict(rb); mart.update({"name":"HxPetalburgCity_Mart","id":"MAP_HX_PETALBURG_CITY_MART","mapsec":"MAPSEC_HX_PETALBURG_CITY","parent":"MAP_HX_PETALBURG_CITY","parent_warp":2,"items":rb['items']+['ITEM_MAX_POTION']})
cen=dict(dc); cen.update({"name":"HxPetalburgCity_PokemonCenter_1F","id":"MAP_HX_PETALBURG_CITY_POKEMON_CENTER_1F","mapsec":"MAPSEC_HX_PETALBURG_CITY","parent":"MAP_HX_PETALBURG_CITY","parent_warp":1,"heal":"HEAL_LOCATION_HX_PETALBURG_CITY","nurse_localid":"LOCALID_HX_PETALBURG_NURSE"})
c['clones']+=[mart,cen]
json.dump(c,open(p,'w'),indent=1,ensure_ascii=False)
t=json.load(open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json'))
for e in ({"prim":"general","sec":"petalburg","name":"hx_petalburg"},{"prim":"building","sec":"petalburg_gym","name":"hx_petalburg_gym"}):
    if e not in t: t.append(e)
json.dump(t,open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json','w'),indent=1)
print(len(ts),'door tiles',sign_scripts)
