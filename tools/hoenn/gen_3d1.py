import json
# Phase 3d-1 (v3.0-alpha7): Route 118 -> Route 119 (+ Weather Institute) -> Fortree City + Gym (Winona)
p='/home/claude/pokefirered/tools/hoenn/maps_phase1.json'
c=json.load(open(p))
NEW=['HxRoute118','HxRoute119','HxRoute119_WeatherInstitute_1F','HxRoute119_WeatherInstitute_2F','HxFortreeCity','HxFortreeCity_Gym']
c['maps']=[m for m in c['maps'] if m['name'] not in NEW]
c['clones']=[k for k in c['clones'] if not k['name'].startswith('HxFortreeCity')]
c['mapsecs']=[x for x in c['mapsecs'] if not any(k in x['id'] for k in ('ROUTE_118','ROUTE_119','FORTREE'))]
c['heal_locations']=[h for h in c['heal_locations'] if 'FORTREE' not in h['id']]
SIGNS={"Common_EventScript_ShowPokemonCenterSign":"EventScript_PokecenterSign","Common_EventScript_ShowPokemartSign":"EventScript_PokemartSign"}
def mk(src,name,id_,tsp,tss,mapsec,music,to,wo,**kw):
    d={"src":src,"name":name,"id":id_,"group":"field","tilesets":[tsp,tss],"mapsec":mapsec,"music":music,"trainer_offset":to,"wild_offset":wo}
    d.update(kw); c['maps'].append(d)
mk('Route118','HxRoute118','MAP_HX_ROUTE118','gTileset_HxMauvilleP','gTileset_HxMauvilleS','MAPSEC_HX_ROUTE_118','MUS_ROUTE3',38,36,
   connections={"MAP_MAUVILLE_CITY":"MAP_HX_MAUVILLE_CITY","MAP_ROUTE119":"MAP_HX_ROUTE119"},warp_targets={},
   drop_scripts=["EventScript_CutTree"]+["Route118_EventScript_"+n for n in ("Wade","Chester","Dalton","Deandre","ItemHyperPotion")],custom_coords=True)
mk('Route119','HxRoute119','MAP_HX_ROUTE119','gTileset_HxFortreeP','gTileset_HxFortreeS','MAPSEC_HX_ROUTE_119','MUS_ROUTE3',39,37,
   connections={"MAP_ROUTE118":"MAP_HX_ROUTE118","MAP_FORTREE_CITY":"MAP_HX_FORTREE_CITY"},
   warp_targets={"0":{"map":"MAP_HX_ROUTE119_WEATHER_INSTITUTE_1F","warp":0}},
   drop_scripts=["EventScript_CutTree"],custom_coords=True)
AQUA1=["Route119_WeatherInstitute_1F_EventScript_Grunt1","Route119_WeatherInstitute_1F_EventScript_Grunt4"]
AQUA2=["Route119_WeatherInstitute_2F_EventScript_Grunt2","Route119_WeatherInstitute_2F_EventScript_Grunt3",
       "Route119_WeatherInstitute_2F_EventScript_Shelly","Route119_WeatherInstitute_2F_EventScript_Grunt5"]
mk('Route119_WeatherInstitute_1F','HxRoute119_WeatherInstitute_1F','MAP_HX_ROUTE119_WEATHER_INSTITUTE_1F','gTileset_HxLabP','gTileset_HxLabS','MAPSEC_HX_ROUTE_119','MUS_SILPH',40,0,
   warp_targets={"0":{"map":"MAP_HX_ROUTE119","warp":0},"1":{"map":"MAP_HX_ROUTE119","warp":0},"2":{"map":"MAP_HX_ROUTE119_WEATHER_INSTITUTE_2F","warp":0}},
   keep_hidden=AQUA1,set_flags={k:"FLAG_HIDE_WI_AQUA" for k in AQUA1},custom_coords=True)
mk('Route119_WeatherInstitute_2F','HxRoute119_WeatherInstitute_2F','MAP_HX_ROUTE119_WEATHER_INSTITUTE_2F','gTileset_HxLabP','gTileset_HxLabS','MAPSEC_HX_ROUTE_119','MUS_SILPH',40,0,
   warp_targets={"0":{"map":"MAP_HX_ROUTE119_WEATHER_INSTITUTE_1F","warp":2}},
   keep_hidden=AQUA2,set_flags={k:"FLAG_HIDE_WI_AQUA" for k in AQUA2},
   extra_trainers=["TRAINER_SHELLY_WEATHER_INSTITUTE"],custom_coords=True)
mk('FortreeCity','HxFortreeCity','MAP_HX_FORTREE_CITY','gTileset_HxFortreeP','gTileset_HxFortreeS','MAPSEC_HX_FORTREE_CITY','MUS_CELADON',40,37,
   connections={"MAP_ROUTE119":"MAP_HX_ROUTE119"},
   warp_targets={"0":{"map":"MAP_HX_FORTREE_CITY_POKEMON_CENTER_1F","warp":0},"2":{"map":"MAP_HX_FORTREE_CITY_GYM","warp":0},"3":{"map":"MAP_HX_FORTREE_CITY_MART","warp":0}},
   sign_map=SIGNS,drop_scripts=["EventScript_CutTree"],custom_coords=True)
mk('FortreeCity_Gym','HxFortreeCity_Gym','MAP_HX_FORTREE_CITY_GYM','gTileset_HxFortreeGymP','gTileset_HxFortreeGymS','MAPSEC_HX_FORTREE_CITY','MUS_GYM',41,0,
   warp_targets={"0":{"map":"MAP_HX_FORTREE_CITY","warp":1},"1":{"map":"MAP_HX_FORTREE_CITY","warp":1}},
   map_scripts="\tmap_script MAP_SCRIPT_ON_TRANSITION, HxFortreeCity_Gym_OnTransition\n\tmap_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, HxFortreeCity_Gym_OnWarp\n\t.byte 0\n",
   extra_trainers=["TRAINER_WINONA_1"],custom_coords=True)
for m in c['maps']:
    if m['name']=='HxMauvilleCity': m['connections']["MAP_ROUTE118"]="MAP_HX_ROUTE118"
c['mapsecs']+=[{"id":"MAPSEC_HX_ROUTE_118","name":"ROUTE 118"},{"id":"MAPSEC_HX_ROUTE_119","name":"ROUTE 119"},{"id":"MAPSEC_HX_FORTREE_CITY","name":"FORTREE CITY"}]
c['heal_locations'].append({"id":"HEAL_LOCATION_HX_FORTREE_CITY","map":"MAP_HX_FORTREE_CITY","x":5,"y":7,"respawn_map":"MAP_HX_FORTREE_CITY_POKEMON_CENTER_1F","respawn_npc":"LOCALID_HX_FORTREE_NURSE"})
rb=[k for k in c['clones'] if k['name']=='HxRustboroCity_Mart'][0]
dc=[k for k in c['clones'] if k['name']=='HxDewfordTown_PokemonCenter_1F'][0]
mart=dict(rb); mart.update({"name":"HxFortreeCity_Mart","id":"MAP_HX_FORTREE_CITY_MART","mapsec":"MAPSEC_HX_FORTREE_CITY","parent":"MAP_HX_FORTREE_CITY","parent_warp":2,"items":rb['items']+['ITEM_MAX_POTION','ITEM_FULL_RESTORE']})
cen=dict(dc); cen.update({"name":"HxFortreeCity_PokemonCenter_1F","id":"MAP_HX_FORTREE_CITY_POKEMON_CENTER_1F","mapsec":"MAPSEC_HX_FORTREE_CITY","parent":"MAP_HX_FORTREE_CITY","parent_warp":0,"heal":"HEAL_LOCATION_HX_FORTREE_CITY","nurse_localid":"LOCALID_HX_FORTREE_NURSE"})
c['clones']+=[mart,cen]
json.dump(c,open(p,'w'),indent=1,ensure_ascii=False)
t=json.load(open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json'))
for e in ({"prim":"general","sec":"fortree","name":"hx_fortree"},{"prim":"building","sec":"fortree_gym","name":"hx_fortree_gym"},{"prim":"building","sec":"lab","name":"hx_lab"}):
    if e not in t: t.append(e)
json.dump(t,open('/home/claude/pokefirered/tools/hoenn/tilesets_phase1.json','w'),indent=1)
print('ok')
