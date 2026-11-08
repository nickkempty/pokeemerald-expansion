import json
# Phase 3e-1 (v3.0-alpha12): Route 120 -> Route 121 -> Lilycove City -> Route 124 -> Mossdeep City + Gym (Tate & Liza)
p='/home/claude/pokefirered/tools/hoenn/maps_phase1.json'
EM='/home/claude/pokeemerald/data/maps'
c=json.load(open(p))
NEW=['HxRoute120','HxRoute121','HxLilycoveCity','HxRoute124','HxMossdeepCity','HxMossdeepCity_Gym']
c['maps']=[m for m in c['maps'] if m['name'] not in NEW]
c['clones']=[k for k in c['clones'] if not k['name'].startswith(('HxLilycoveCity','HxMossdeepCity'))]
c['mapsecs']=[x for x in c['mapsecs'] if not any(k in x['id'] for k in ('ROUTE_120','ROUTE_121','ROUTE_124','LILYCOVE','MOSSDEEP'))]
c['heal_locations']=[h for h in c['heal_locations'] if not any(k in h['id'] for k in ('LILYCOVE','MOSSDEEP'))]
SIGNS={"Common_EventScript_ShowPokemonCenterSign":"EventScript_PokecenterSign","Common_EventScript_ShowPokemartSign":"EventScript_PokemartSign"}
def mk(src,name,id_,tsp,tss,mapsec,music,to,wo,**kw):
    d={"src":src,"name":name,"id":id_,"group":"field","tilesets":[tsp,tss],"mapsec":mapsec,"music":music,"trainer_offset":to,"wild_offset":wo}
    d.update(kw); c['maps'].append(d)
mk('Route120','HxRoute120','MAP_HX_ROUTE120','gTileset_HxFortreeP','gTileset_HxFortreeS','MAPSEC_HX_ROUTE_120','MUS_ROUTE3',40,37,
   connections={"MAP_FORTREE_CITY":"MAP_HX_FORTREE_CITY","MAP_ROUTE121":"MAP_HX_ROUTE121"},warp_targets={},
   drop_scripts=["EventScript_CutTree","Route120_EventScript_BerryBeauty"],custom_coords=True)
mk('Route121','HxRoute121','MAP_HX_ROUTE121','gTileset_HxLilycoveP','gTileset_HxLilycoveS','MAPSEC_HX_ROUTE_121','MUS_ROUTE3',40,37,
   connections={"MAP_ROUTE120":"MAP_HX_ROUTE120","MAP_LILYCOVE_CITY":"MAP_HX_LILYCOVE_CITY"},warp_targets={},
   drop_scripts=["EventScript_CutTree"],custom_coords=True)
mk('LilycoveCity','HxLilycoveCity','MAP_HX_LILYCOVE_CITY','gTileset_HxLilycoveP','gTileset_HxLilycoveS','MAPSEC_HX_LILYCOVE_CITY','MUS_VERMILLION',40,37,
   connections={"MAP_ROUTE121":"MAP_HX_ROUTE121","MAP_ROUTE124":"MAP_HX_ROUTE124"},
   warp_targets={"0":{"map":"MAP_HX_LILYCOVE_CITY_MART","warp":0},"2":{"map":"MAP_HX_LILYCOVE_CITY_POKEMON_CENTER_1F","warp":0}},
   sign_map=SIGNS,drop_scripts=["EventScript_CutTree","LilycoveCity_EventScript_BerryGentleman"],custom_coords=True)
mk('Route124','HxRoute124','MAP_HX_ROUTE124','gTileset_HxMossdeepP','gTileset_HxMossdeepS','MAPSEC_HX_ROUTE_124','MUS_SURF',40,37,
   connections={"MAP_LILYCOVE_CITY":"MAP_HX_LILYCOVE_CITY","MAP_MOSSDEEP_CITY":"MAP_HX_MOSSDEEP_CITY"},warp_targets={},custom_coords=True)
mk('MossdeepCity','HxMossdeepCity','MAP_HX_MOSSDEEP_CITY','gTileset_HxMossdeepP','gTileset_HxMossdeepS','MAPSEC_HX_MOSSDEEP_CITY','MUS_CINNABAR',40,37,
   connections={"MAP_ROUTE124":"MAP_HX_ROUTE124"},
   warp_targets={"1":{"map":"MAP_HX_MOSSDEEP_CITY_GYM","warp":0},"2":{"map":"MAP_HX_MOSSDEEP_CITY_POKEMON_CENTER_1F","warp":0},"4":{"map":"MAP_HX_MOSSDEEP_CITY_MART","warp":0}},
   sign_map=SIGNS,drop_scripts=["EventScript_CutTree","MossdeepCity_EventScript_DynamicPunchTutor"],custom_coords=True)
# gym: internal warp pads keep their ids
d=json.load(open(f'{EM}/MossdeepCity_Gym/map.json'))
wt={}
for i,w in enumerate(d['warp_events']):
    wt[str(i)]={"map":"MAP_HX_MOSSDEEP_CITY","warp":1} if w['dest_map']=='MAP_MOSSDEEP_CITY' else {"map":"MAP_HX_MOSSDEEP_CITY_GYM","warp":int(w['dest_warp_id'])}
statues=[{"graphics_id":"OBJ_EVENT_GFX_HX_TRICK_HOUSE_STATUE","x":o['x'],"y":o['y'],"elevation":o['elevation'],
          "script":"HxCommon_EventScript_Statue"} for o in d['object_events'] if o['graphics_id']=='OBJ_EVENT_GFX_TRICK_HOUSE_STATUE']
mk('MossdeepCity_Gym','HxMossdeepCity_Gym','MAP_HX_MOSSDEEP_CITY_GYM','gTileset_HxMossdeepGymP','gTileset_HxMossdeepGymS','MAPSEC_HX_MOSSDEEP_CITY','MUS_GYM',33,0,
   warp_targets=wt,extra_objects=statues,extra_trainers=["TRAINER_TATE_AND_LIZA_1"],custom_coords=True)
for m in c['maps']:
    if m['name']=='HxFortreeCity': m['connections']["MAP_ROUTE120"]="MAP_HX_ROUTE120"
c['mapsecs']+=[{"id":"MAPSEC_HX_ROUTE_120","name":"ROUTE 120"},{"id":"MAPSEC_HX_ROUTE_121","name":"ROUTE 121"},
               {"id":"MAPSEC_HX_LILYCOVE_CITY","name":"LILYCOVE CITY"},{"id":"MAPSEC_HX_ROUTE_124","name":"ROUTE 124"},
               {"id":"MAPSEC_HX_MOSSDEEP_CITY","name":"MOSSDEEP CITY"}]
for city,(x,y) in (("LILYCOVE",(24,15)),("MOSSDEEP",(28,17))):
    c['heal_locations'].append({"id":f"HEAL_LOCATION_HX_{city}_CITY","map":f"MAP_HX_{city}_CITY","x":x,"y":y,
        "respawn_map":f"MAP_HX_{city}_CITY_POKEMON_CENTER_1F","respawn_npc":f"LOCALID_HX_{city}_NURSE"})
rb=[k for k in c['clones'] if k['name']=='HxFortreeCity_Mart'][0]
dc=[k for k in c['clones'] if k['name']=='HxFortreeCity_PokemonCenter_1F'][0]
for city,Name,mw,cw in (("LILYCOVE","Lilycove",0,2),("MOSSDEEP","Mossdeep",4,2)):
    mart=dict(rb); mart.update({"name":f"Hx{Name}City_Mart","id":f"MAP_HX_{city}_CITY_MART","mapsec":f"MAPSEC_HX_{city}_CITY","parent":f"MAP_HX_{city}_CITY","parent_warp":mw,
        "items":rb['items']+(['ITEM_MAX_REPEL'] if city=='LILYCOVE' else ['ITEM_MAX_REPEL','ITEM_MAX_REVIVE'])})
    cen=dict(dc); cen.update({"name":f"Hx{Name}City_PokemonCenter_1F","id":f"MAP_HX_{city}_CITY_POKEMON_CENTER_1F","mapsec":f"MAPSEC_HX_{city}_CITY","parent":f"MAP_HX_{city}_CITY","parent_warp":cw,
        "heal":f"HEAL_LOCATION_HX_{city}_CITY","nurse_localid":f"LOCALID_HX_{city}_NURSE"})
    c['clones']+=[mart,cen]
json.dump(c,open(p,'w'),indent=1,ensure_ascii=False)
t=json.load(open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json'))
for e in ({"prim":"general","sec":"lilycove","name":"hx_lilycove"},{"prim":"general","sec":"mossdeep","name":"hx_mossdeep"},{"prim":"building","sec":"mossdeep_gym","name":"hx_mossdeep_gym"}):
    if e not in t: t.append(e)
json.dump(t,open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json','w'),indent=1)
print('ok', len(statues))
