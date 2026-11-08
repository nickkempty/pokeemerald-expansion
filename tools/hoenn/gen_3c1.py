import json
p='/home/claude/pokefirered/tools/hoenn/maps_phase1.json'
c=json.load(open(p))
names={m['name'] for m in c['maps']}
NEW=['HxRoute111','HxRoute112','HxFieryPath','HxJaggedPass','HxLavaridgeTown','HxLavaridgeTown_Gym_1F','HxLavaridgeTown_Gym_B1F']
c['maps']=[m for m in c['maps'] if m['name'] not in NEW]
c['clones']=[k for k in c['clones'] if not k['name'].startswith('HxLavaridgeTown')]
c['mapsecs']=[x for x in c['mapsecs'] if 'LAVARIDGE' not in x['id'] and 'ROUTE_111' not in x['id'] and 'ROUTE_112' not in x['id'] and 'FIERY' not in x['id'] and 'JAGGED' not in x['id']]
c['heal_locations']=[h for h in c['heal_locations'] if 'LAVARIDGE' not in h['id']]
EM='/home/claude/pokeemerald/data/maps'
def hxmap(n): return 'MAP_HX_'+n
def mk(src,name,id_,tsp,tss,mapsec,music,to,wo,**kw):
    d={"src":src,"name":name,"id":id_,"group":"field","tilesets":[tsp,tss],"mapsec":mapsec,"music":music,"trainer_offset":to,"wild_offset":wo}
    d.update(kw); c['maps'].append(d)
mk('Route111','HxRoute111','MAP_HX_ROUTE111','gTileset_HxMauvilleP','gTileset_HxMauvilleS','MAPSEC_HX_ROUTE_111','MUS_ROUTE3',35,33,
   connections={"MAP_MAUVILLE_CITY":"MAP_HX_MAUVILLE_CITY","MAP_ROUTE112":"MAP_HX_ROUTE112"},warp_targets={},drop_scripts=["EventScript_CutTree"])
mk('Route112','HxRoute112','MAP_HX_ROUTE112','gTileset_HxLavaridgeP','gTileset_HxLavaridgeS','MAPSEC_HX_ROUTE_112','MUS_ROUTE3',36,34,
   connections={"MAP_ROUTE111":"MAP_HX_ROUTE111","MAP_LAVARIDGE_TOWN":"MAP_HX_LAVARIDGE_TOWN"},
   warp_targets={"0":{"map":"MAP_HX_JAGGED_PASS","warp":2},"1":{"map":"MAP_HX_JAGGED_PASS","warp":3},"2":{"map":"MAP_HX_JAGGED_PASS","warp":0},"3":{"map":"MAP_HX_JAGGED_PASS","warp":1},"4":{"map":"MAP_HX_FIERY_PATH","warp":0},"5":{"map":"MAP_HX_FIERY_PATH","warp":1}},drop_scripts=["EventScript_CutTree"])
mk('FieryPath','HxFieryPath','MAP_HX_FIERY_PATH','gTileset_HxLavaridgeP','gTileset_HxLavaridgeS','MAPSEC_HX_FIERY_PATH','MUS_MT_MOON',37,35,
   warp_targets={"0":{"map":"MAP_HX_ROUTE112","warp":4},"1":{"map":"MAP_HX_ROUTE112","warp":5}},drop_scripts=["EventScript_CutTree"])
mk('JaggedPass','HxJaggedPass','MAP_HX_JAGGED_PASS','gTileset_HxLavaridgeP','gTileset_HxLavaridgeS','MAPSEC_HX_JAGGED_PASS','MUS_MT_MOON',37,35,
   warp_targets={"0":{"map":"MAP_HX_ROUTE112","warp":2},"1":{"map":"MAP_HX_ROUTE112","warp":3},"2":{"map":"MAP_HX_ROUTE112","warp":0},"3":{"map":"MAP_HX_ROUTE112","warp":1}},drop_scripts=["EventScript_CutTree"])
mk('LavaridgeTown','HxLavaridgeTown','MAP_HX_LAVARIDGE_TOWN','gTileset_HxLavaridgeP','gTileset_HxLavaridgeS','MAPSEC_HX_LAVARIDGE_TOWN','MUS_VERMILLION',36,34,
   connections={"MAP_ROUTE112":"MAP_HX_ROUTE112"},
   warp_targets={"1":{"map":"MAP_HX_LAVARIDGE_TOWN_GYM_1F","warp":0},"2":{"map":"MAP_HX_LAVARIDGE_TOWN_MART","warp":0},"3":{"map":"MAP_HX_LAVARIDGE_TOWN_POKEMON_CENTER_1F","warp":0}},
   sign_map={"Common_EventScript_ShowPokemonCenterSign":"EventScript_PokecenterSign","Common_EventScript_ShowPokemartSign":"EventScript_PokemartSign"},
   drop_scripts=["EventScript_CutTree"],
   map_scripts="\tmap_script MAP_SCRIPT_ON_TRANSITION, HxLavaridgeTown_OnTransition\n\t.byte 0\n",
   extra_objects=[{"graphics_id":"OBJ_EVENT_GFX_HX_MAXIE","x":9,"y":17,"local_id":"LOCALID_HX_LAVARIDGE_MAXIE","movement_type":"MOVEMENT_TYPE_FACE_LEFT","script":"0x0","flag":"FLAG_HIDE_LAVARIDGE_TOWN_MAXIE"},
                {"graphics_id":"OBJ_EVENT_GFX_GIOVANNI","x":10,"y":17,"local_id":"LOCALID_HX_LAVARIDGE_NOX","movement_type":"MOVEMENT_TYPE_FACE_LEFT","script":"0x0","flag":"FLAG_HIDE_LAVARIDGE_TOWN_NOX"}],
   extra_coords=[{"x":5,"y":16,"var":"VAR_LAVARIDGE_TOWN_STATE","var_value":"1","script":"HxLavaridgeTown_EventScript_GymExitTrigger"}],
   custom_coords=True)
def gym(name,src,dests):
    d=json.load(open(f'{EM}/{src}/map.json'))
    wt={}
    for i,w in enumerate(d['warp_events']):
        dm=w['dest_map']
        if dm=='MAP_LAVARIDGE_TOWN': wt[str(i)]={"map":"MAP_HX_LAVARIDGE_TOWN","warp":0}
        else: wt[str(i)]={"map":"MAP_HX_"+dm[4:],"warp":int(w['dest_warp_id'])}
    mk(src,name,'MAP_HX_'+src.replace('LavaridgeTown','LAVARIDGE_TOWN').upper(),'gTileset_HxLavaridgeGymP','gTileset_HxLavaridgeGymS','MAPSEC_HX_LAVARIDGE_TOWN','MUS_GYM',38,30,warp_targets=wt,drop_scripts=["EventScript_CutTree"],**dests)
gym('HxLavaridgeTown_Gym_1F','LavaridgeTown_Gym_1F',{"extra_trainers":["TRAINER_FLANNERY_1"]})
gym('HxLavaridgeTown_Gym_B1F','LavaridgeTown_Gym_B1F',{"extra_blocks_from":["LavaridgeTown_Gym_1F"]})
for m in c['maps']:
    if m['name']=='HxMauvilleCity': m['connections']["MAP_ROUTE111"]="MAP_HX_ROUTE111"
c['mapsecs']+= [{"id":"MAPSEC_HX_ROUTE_111","name":"ROUTE 111"},{"id":"MAPSEC_HX_ROUTE_112","name":"ROUTE 112"},{"id":"MAPSEC_HX_FIERY_PATH","name":"FIERY PATH"},{"id":"MAPSEC_HX_JAGGED_PASS","name":"JAGGED PASS"},{"id":"MAPSEC_HX_LAVARIDGE_TOWN","name":"LAVARIDGE TOWN"}]
c['heal_locations'].append({"id":"HEAL_LOCATION_HX_LAVARIDGE_TOWN","map":"MAP_HX_LAVARIDGE_TOWN","x":9,"y":7,"respawn_map":"MAP_HX_LAVARIDGE_TOWN_POKEMON_CENTER_1F","respawn_npc":"LOCALID_HX_LAVARIDGE_NURSE"})
rb=[k for k in c['clones'] if k['name']=='HxRustboroCity_Mart'][0]
dc=[k for k in c['clones'] if k['name']=='HxDewfordTown_PokemonCenter_1F'][0]
mart=dict(rb); mart.update({"name":"HxLavaridgeTown_Mart","id":"MAP_HX_LAVARIDGE_TOWN_MART","mapsec":"MAPSEC_HX_LAVARIDGE_TOWN","parent":"MAP_HX_LAVARIDGE_TOWN","parent_warp":1})
cen=dict(dc); cen.update({"name":"HxLavaridgeTown_PokemonCenter_1F","id":"MAP_HX_LAVARIDGE_TOWN_POKEMON_CENTER_1F","mapsec":"MAPSEC_HX_LAVARIDGE_TOWN","parent":"MAP_HX_LAVARIDGE_TOWN","parent_warp":2,"heal":"HEAL_LOCATION_HX_LAVARIDGE_TOWN","nurse_localid":"LOCALID_HX_LAVARIDGE_NURSE"})
c['clones']+=[mart,cen]
json.dump(c,open(p,'w'),indent=1,ensure_ascii=False)
t=json.load(open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json'))
for e in ({"prim":"general","sec":"lavaridge","name":"hx_lavaridge"},{"prim":"building","sec":"lavaridge_gym","name":"hx_lavaridge_gym"}):
    if e not in t: t.append(e)
json.dump(t,open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json','w'),indent=1)
