import json
# Phase 3e-2 (v3.0-alpha13): Route 126 -> Sootopolis City (+ Gym 1F/B1F, Juan) + Cave of Origin (NOX final, GROUDON)
p='/home/claude/pokefirered/tools/hoenn/maps_phase1.json'
EM='/home/claude/pokeemerald/data/maps'
c=json.load(open(p))
NEW=['HxRoute126','HxSootopolisCity','HxSootopolisCity_Gym_1F','HxSootopolisCity_Gym_B1F','HxCaveOfOrigin_Entrance','HxCaveOfOrigin_1F','HxCaveOfOrigin_B1F']
c['maps']=[m for m in c['maps'] if m['name'] not in NEW]
c['clones']=[k for k in c['clones'] if not k['name'].startswith('HxSootopolisCity')]
c['mapsecs']=[x for x in c['mapsecs'] if not any(k in x['id'] for k in ('ROUTE_126','SOOTOPOLIS','CAVE_OF_ORIGIN'))]
c['heal_locations']=[h for h in c['heal_locations'] if 'SOOTOPOLIS' not in h['id']]
SIGNS={"Common_EventScript_ShowPokemonCenterSign":"EventScript_PokecenterSign","Common_EventScript_ShowPokemartSign":"EventScript_PokemartSign"}
def mk(src,name,id_,tsp,tss,mapsec,music,to,wo,**kw):
    d={"src":src,"name":name,"id":id_,"group":"field","tilesets":[tsp,tss],"mapsec":mapsec,"music":music,"trainer_offset":to,"wild_offset":wo}
    d.update(kw); c['maps'].append(d)
mk('Route126','HxRoute126','MAP_HX_ROUTE126','gTileset_HxMossdeepP','gTileset_HxMossdeepS','MAPSEC_HX_ROUTE_126','MUS_SURF',40,37,
   connections={"MAP_ROUTE124":"MAP_HX_ROUTE124"},warp_targets={},
   extra_coords=[{"x":x,"y":33,"elevation":0,"var":"VAR_TEMP_1","var_value":"0","script":"HxRoute126_EventScript_UnderwaterCave"} for x in range(41,47)],custom_coords=True)
SOO_KEEP=["SootopolisCity_EventScript_Maxie"]
mk('SootopolisCity','HxSootopolisCity','MAP_HX_SOOTOPOLIS_CITY','gTileset_HxSootopolisP','gTileset_HxSootopolisS','MAPSEC_HX_SOOTOPOLIS_CITY','MUS_CINNABAR',40,37,
   warp_targets={"0":{"map":"MAP_HX_SOOTOPOLIS_CITY_POKEMON_CENTER_1F","warp":0},"1":{"map":"MAP_HX_SOOTOPOLIS_CITY_MART","warp":0},
                 "2":{"map":"MAP_HX_SOOTOPOLIS_CITY_GYM_1F","warp":0},"3":{"map":"MAP_HX_CAVE_OF_ORIGIN_ENTRANCE","warp":0}},
   keep_hidden=SOO_KEEP,set_flags={"SootopolisCity_EventScript_Maxie":"FLAG_SOOTO_MAXIE_GONE"},
   sign_map=SIGNS,drop_scripts=["SootopolisCity_EventScript_CaveOfOriginExpert"],
   extra_objects=[
     {"graphics_id":"OBJ_EVENT_GFX_HX_GROUDON_SIDE","x":28,"y":44,"elevation":1,"script":"HxSootopolisCity_EventScript_Legend","flag":"FLAG_NOX_FINAL_DONE","local_id":"LOCALID_HX_SOOTO_GROUDON"},
     {"graphics_id":"OBJ_EVENT_GFX_HX_KYOGRE_SIDE","x":34,"y":44,"elevation":1,"script":"HxSootopolisCity_EventScript_Legend","flag":"FLAG_NOX_FINAL_DONE","local_id":"LOCALID_HX_SOOTO_KYOGRE"},
     {"graphics_id":"OBJ_EVENT_GFX_HX_SAILOR","x":44,"y":35,"elevation":3,"movement_type":"MOVEMENT_TYPE_FACE_DOWN","script":"HxSootopolisCity_EventScript_Diver"},
     {"graphics_id":"OBJ_EVENT_GFX_HX_MAN_2","x":31,"y":33,"elevation":3,"movement_type":"MOVEMENT_TYPE_FACE_DOWN","script":"HxSootopolisCity_EventScript_GymBlocker","flag":"FLAG_NOX_FINAL_DONE"}],
   map_scripts="\tmap_script MAP_SCRIPT_ON_FRAME_TABLE, HxSootopolisCity_OnFrame\n\t.byte 0\n",
   keep_local_ids={},custom_coords=True)
d=json.load(open(f'{EM}/SootopolisCity_Gym_1F/map.json'))
mk('SootopolisCity_Gym_1F','HxSootopolisCity_Gym_1F','MAP_HX_SOOTOPOLIS_CITY_GYM_1F','gTileset_HxSootopolisGymP','gTileset_HxSootopolisGymS','MAPSEC_HX_SOOTOPOLIS_CITY','MUS_GYM',36,0,
   warp_targets={"0":{"map":"MAP_HX_SOOTOPOLIS_CITY","warp":2},"1":{"map":"MAP_HX_SOOTOPOLIS_CITY","warp":2},"2":{"map":"MAP_HX_SOOTOPOLIS_CITY_GYM_B1F","warp":0}},
   map_scripts="\tmap_script MAP_SCRIPT_ON_FRAME_TABLE, HxSootopolisCity_Gym_1F_OnFrame\n\tmap_script MAP_SCRIPT_ON_RESUME, HxSootopolisCity_Gym_1F_OnResume\n\tmap_script MAP_SCRIPT_ON_LOAD, HxSootopolisCity_Gym_1F_OnLoad\n\tmap_script MAP_SCRIPT_ON_TRANSITION, HxSootopolisCity_Gym_1F_OnTransition\n\t.byte 0\n",
   extra_trainers=["TRAINER_JUAN_1"],custom_coords=True)
mk('SootopolisCity_Gym_B1F','HxSootopolisCity_Gym_B1F','MAP_HX_SOOTOPOLIS_CITY_GYM_B1F','gTileset_HxSootopolisGymP','gTileset_HxSootopolisGymS','MAPSEC_HX_SOOTOPOLIS_CITY','MUS_GYM',36,0,
   warp_targets={"0":{"map":"MAP_HX_SOOTOPOLIS_CITY_GYM_1F","warp":2}},custom_coords=True)
mk('CaveOfOrigin_Entrance','HxCaveOfOrigin_Entrance','MAP_HX_CAVE_OF_ORIGIN_ENTRANCE','gTileset_HxCaveP','gTileset_HxCaveS','MAPSEC_HX_CAVE_OF_ORIGIN','MUS_MT_MOON',40,37,
   warp_targets={"0":{"map":"MAP_HX_SOOTOPOLIS_CITY","warp":3},"1":{"map":"MAP_HX_CAVE_OF_ORIGIN_1F","warp":0}},custom_coords=True)
mk('CaveOfOrigin_1F','HxCaveOfOrigin_1F','MAP_HX_CAVE_OF_ORIGIN_1F','gTileset_HxCaveP','gTileset_HxCaveS','MAPSEC_HX_CAVE_OF_ORIGIN','MUS_MT_MOON',40,37,
   warp_targets={"0":{"map":"MAP_HX_CAVE_OF_ORIGIN_ENTRANCE","warp":1},"1":{"map":"MAP_HX_CAVE_OF_ORIGIN_B1F","warp":0}},custom_coords=True)
mk('CaveOfOrigin_B1F','HxCaveOfOrigin_B1F','MAP_HX_CAVE_OF_ORIGIN_B1F','gTileset_HxCaveP','gTileset_HxCaveS','MAPSEC_HX_CAVE_OF_ORIGIN','MUS_ENCOUNTER_GYM_LEADER',40,37,
   warp_targets={"0":{"map":"MAP_HX_CAVE_OF_ORIGIN_1F","warp":1}},
   extra_objects=[
     {"graphics_id":"OBJ_EVENT_GFX_GIOVANNI","x":9,"y":11,"elevation":3,"movement_type":"MOVEMENT_TYPE_FACE_DOWN","script":"HxCaveOfOrigin_B1F_EventScript_Nox","flag":"FLAG_NOX_FINAL_DONE","local_id":"LOCALID_HX_COO_NOX"},
     {"graphics_id":"OBJ_EVENT_GFX_HX_GROUDON_FRONT","x":9,"y":9,"elevation":2,"movement_type":"MOVEMENT_TYPE_FACE_DOWN","script":"HxCaveOfOrigin_B1F_EventScript_Groudon","flag":"FLAG_NOX_FINAL_DONE","local_id":"LOCALID_HX_COO_GROUDON_PIT"},
     {"graphics_id":"OBJ_EVENT_GFX_HX_GROUDON_FRONT","x":9,"y":14,"elevation":3,"movement_type":"MOVEMENT_TYPE_FACE_UP","script":"HxCaveOfOrigin_B1F_EventScript_Groudon","flag":"FLAG_GROUDON_RING_HIDE","local_id":"LOCALID_HX_COO_GROUDON"}],
   map_scripts="\tmap_script MAP_SCRIPT_ON_TRANSITION, HxCaveOfOrigin_B1F_OnTransition\n\t.byte 0\n",
   extra_coords=[{"x":x,"y":13,"elevation":0,"var":"VAR_COO_STATE","var_value":"0","script":"HxCaveOfOrigin_B1F_EventScript_NoxTrigger"} for x in (8,9,10)],
   custom_coords=True)
for m in c['maps']:
    if m['name']=='HxRoute124': m['connections']["MAP_ROUTE126"]="MAP_HX_ROUTE126"
c['mapsecs']+=[{"id":"MAPSEC_HX_ROUTE_126","name":"ROUTE 126"},{"id":"MAPSEC_HX_SOOTOPOLIS_CITY","name":"SOOTOPOLIS CITY"},{"id":"MAPSEC_HX_CAVE_OF_ORIGIN","name":"CAVE OF ORIGIN"}]
c['heal_locations'].append({"id":"HEAL_LOCATION_HX_SOOTOPOLIS_CITY","map":"MAP_HX_SOOTOPOLIS_CITY","x":43,"y":32,
        "respawn_map":"MAP_HX_SOOTOPOLIS_CITY_POKEMON_CENTER_1F","respawn_npc":"LOCALID_HX_SOOTOPOLIS_NURSE"})
rb=[k for k in c['clones'] if k['name']=='HxMossdeepCity_Mart'][0]
dc=[k for k in c['clones'] if k['name']=='HxMossdeepCity_PokemonCenter_1F'][0]
mart=dict(rb); mart.update({"name":"HxSootopolisCity_Mart","id":"MAP_HX_SOOTOPOLIS_CITY_MART","mapsec":"MAPSEC_HX_SOOTOPOLIS_CITY","parent":"MAP_HX_SOOTOPOLIS_CITY","parent_warp":1})
cen=dict(dc); cen.update({"name":"HxSootopolisCity_PokemonCenter_1F","id":"MAP_HX_SOOTOPOLIS_CITY_POKEMON_CENTER_1F","mapsec":"MAPSEC_HX_SOOTOPOLIS_CITY","parent":"MAP_HX_SOOTOPOLIS_CITY","parent_warp":0,
    "heal":"HEAL_LOCATION_HX_SOOTOPOLIS_CITY","nurse_localid":"LOCALID_HX_SOOTOPOLIS_NURSE"})
c['clones']+=[mart,cen]
c.setdefault('extra_flags',[])
for f in ("FLAG_NOX_FINAL_DONE","FLAG_GROUDON_GONE","FLAG_GROUDON_RING_HIDE","FLAG_SOOTO_SCENE_DONE","FLAG_HIDE_SOOTO_MAXIE"):
    if f not in c['extra_flags']: c['extra_flags'].append(f)
json.dump(c,open(p,'w'),indent=1,ensure_ascii=False)
t=json.load(open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json'))
for e in ({"prim":"general","sec":"sootopolis","name":"hx_sootopolis"},{"prim":"building","sec":"sootopolis_gym","name":"hx_sootopolis_gym"},{"prim":"general","sec":"cave","name":"hx_cave"}):
    if e not in t: t.append(e)
json.dump(t,open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json','w'),indent=1)
print('ok')
