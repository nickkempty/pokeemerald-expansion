@ Common scripts for the ported Hoenn maps (tools/hoenn/hx_common.inc)

HxCommon_EventScript_Locked::
	msgbox HxCommon_Text_Locked, MSGBOX_SIGN
	end

HxCommon_Text_Locked::
	.string "The door is locked.\n"
	.string "Nobody seems to be home.$"

@ ---- Alpha travel guide (Pallet Town): temporary shortcut to the Hoenn maps ----
EventScript_HxTravelGuide::
	lock
	faceplayer
	msgbox Text_HxTravelGuideAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_HxTravelGuideSlateport
	msgbox Text_HxTravelGuideAsk2, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_HxTravelGuideCinnabar
	goto EventScript_HxTravelGuideNo
	end

EventScript_HxTravelGuideSlateport::
	call EventScript_HxTravelGuideTeam
	msgbox Text_HxTravelGuideGo
	closemessage
	warp MAP_HX_SLATEPORT_CITY, 19, 21
	waitstate
	end

EventScript_HxTravelGuideCinnabar::
	call EventScript_HxTravelGuideTeam
	setflag FLAG_HX_UNION_ROAD_OPEN
	msgbox Text_HxTravelGuideGo
	closemessage
	warp MAP_CINNABAR_ISLAND, 13, 13
	waitstate
	end

EventScript_HxTravelGuideTeam::
	goto_if_set FLAG_HX_ALPHA_TEAM_GIVEN, EventScript_HxTravelGuideTeamDone
	msgbox Text_HxTravelGuideTeamAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_HxTravelGuideTeamDone
	setflag FLAG_HX_ALPHA_TEAM_GIVEN
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_SYS_POKEDEX_GET
	givemon SPECIES_CHARIZARD, 55
	givemon SPECIES_BLASTOISE, 55
	givemon SPECIES_VENUSAUR, 55
	msgbox Text_HxTravelGuideTeamGiven
EventScript_HxTravelGuideTeamDone::
	return

EventScript_HxTravelGuideNo::
	msgbox Text_HxTravelGuideNo
	release
	end

@ ---- Union Road: Giovanni awakens GROUDON (called from the Viridian Gym after the TM) ----
EventScript_HxUnionRoadAwakening::
	goto_if_set FLAG_HX_UNION_ROAD_OPEN, EventScript_HxUnionRoadAwakeningDone
	msgbox Text_HxAwaken1
	closemessage
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 24
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	msgbox Text_HxAwaken2
	closemessage
	setvar VAR_0x8004, 3
	setvar VAR_0x8005, 3
	setvar VAR_0x8006, 40
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	msgbox Text_HxAwaken3
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_GIOVANNI
	fadescreen FADE_FROM_BLACK
	msgbox Text_HxAwaken4
	setflag FLAG_HX_UNION_ROAD_OPEN
EventScript_HxUnionRoadAwakeningDone::
	return

@ ---- Cinnabar: step onto the new causeway -> Union Road ----
EventScript_HxCinnabarToUnionRoad::
	lockall
	goto_if_unset FLAG_HX_UNION_ROAD_OPEN, EventScript_HxCinnabarToUnionRoadClosed
	warp MAP_HX_UNION_ROAD_1, 16, 4
	waitstate
	end

EventScript_HxCinnabarToUnionRoadClosed::
	releaseall
	end

Text_HxAwaken1::
	.string "NOX: …Hmph. So the EARTHBADGE\n"
	.string "is yours.\p"
	.string "But you are too late, child.\p"
	.string "The M-CORE is already\n"
	.string "resonating with the RED ORB.\p"
	.string "Can you feel it? The ground\n"
	.string "itself is waking!$"

Text_HxAwaken2::
	.string "The whole GYM is shaking!$"

Text_HxAwaken3::
	.string "NOX: GROUDON rises from\n"
	.string "the deep sea!\p"
	.string "The sea floor south of CINNABAR\n"
	.string "turns to land.\p"
	.string "A road to HOENN… and to my\n"
	.string "new empire!\p"
	.string "Farewell. We shall meet again.$"

Text_HxAwaken4::
	.string "…The rumbling slowly fades.\p"
	.string "Something huge happened to\n"
	.string "the sea south of CINNABAR ISLAND!\p"
	.string "You should go and see!$"

Text_HxTravelGuideAsk::
	.string "I'm a guide for the HOENN ALPHA.\p"
	.string "Want me to take you to\n"
	.string "SLATEPORT CITY right now?$"

Text_HxTravelGuideAsk2::
	.string "Then how about the start of\n"
	.string "the UNION ROAD, south of\l"
	.string "CINNABAR ISLAND?$"

Text_HxTravelGuideTeamAsk::
	.string "The trainers over there are tough.\p"
	.string "Want a strong TEST TEAM for\n"
	.string "the trip? (CHARIZARD, BLASTOISE\l"
	.string "and VENUSAUR at Lv. 55)$"

Text_HxTravelGuideTeamGiven::
	.string "Here you go! Take good care\n"
	.string "of them.$"

Text_HxTravelGuideGo::
	.string "Off we go!$"

Text_HxTravelGuideNo::
	.string "Come see me any time.$"

HxCommon_EventScript_Statue::
	msgbox HxCommon_Text_Statue, MSGBOX_SIGN
	end

HxCommon_Text_Statue::
	.string "It's a heavy stone statue.\n"
	.string "It won't budge.$"

	.include "data/maps/HxSlateportCity/scripts.inc"
	.include "data/maps/HxRoute110/scripts.inc"
	.include "data/maps/HxMauvilleCity/scripts.inc"
	.include "data/maps/HxMauvilleCity_Gym/scripts.inc"
	.include "data/maps/HxUnionRoad4/scripts.inc"
	.include "data/maps/HxUnionRoad1/scripts.inc"
	.include "data/maps/HxUnionRoad2/scripts.inc"
	.include "data/maps/HxUnionRoad3/scripts.inc"
	.include "data/maps/HxUnionRoad_Gate/scripts.inc"
	.include "data/maps/HxRoute117/scripts.inc"
	.include "data/maps/HxVerdanturfTown/scripts.inc"
	.include "data/maps/HxRoute116/scripts.inc"
	.include "data/maps/HxRustboroCity/scripts.inc"
	.include "data/maps/HxRustboroCity_Gym/scripts.inc"
	.include "data/maps/HxRusturfTunnel/scripts.inc"
	.include "data/maps/HxRoute104/scripts.inc"
	.include "data/maps/HxRoute104_MrBrineysHouse/scripts.inc"
	.include "data/maps/HxDewfordTown/scripts.inc"
	.include "data/maps/HxDewfordTown_Gym/scripts.inc"
	.include "data/maps/HxRoute111/scripts.inc"
	.include "data/maps/HxRoute112/scripts.inc"
	.include "data/maps/HxFieryPath/scripts.inc"
	.include "data/maps/HxJaggedPass/scripts.inc"
	.include "data/maps/HxLavaridgeTown/scripts.inc"
	.include "data/maps/HxLavaridgeTown_Gym_1F/scripts.inc"
	.include "data/maps/HxLavaridgeTown_Gym_B1F/scripts.inc"
	.include "data/maps/HxPetalburgCity/scripts.inc"
	.include "data/maps/HxPetalburgCity_Gym/scripts.inc"
	.include "data/maps/HxRoute118/scripts.inc"
	.include "data/maps/HxRoute119/scripts.inc"
	.include "data/maps/HxRoute119_WeatherInstitute_1F/scripts.inc"
	.include "data/maps/HxRoute119_WeatherInstitute_2F/scripts.inc"
	.include "data/maps/HxFortreeCity/scripts.inc"
	.include "data/maps/HxFortreeCity_Gym/scripts.inc"
	.include "data/maps/HxRoute120/scripts.inc"
	.include "data/maps/HxRoute121/scripts.inc"
	.include "data/maps/HxLilycoveCity/scripts.inc"
	.include "data/maps/HxRoute124/scripts.inc"
	.include "data/maps/HxMossdeepCity/scripts.inc"
	.include "data/maps/HxMossdeepCity_Gym/scripts.inc"
	.include "data/maps/HxRoute126/scripts.inc"
	.include "data/maps/HxSootopolisCity/scripts.inc"
	.include "data/maps/HxSootopolisCity_Gym_1F/scripts.inc"
	.include "data/maps/HxSootopolisCity_Gym_B1F/scripts.inc"
	.include "data/maps/HxCaveOfOrigin_Entrance/scripts.inc"
	.include "data/maps/HxCaveOfOrigin_1F/scripts.inc"
	.include "data/maps/HxCaveOfOrigin_B1F/scripts.inc"
	.include "data/maps/HxSlateportCity_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxSlateportCity_Mart/scripts.inc"
	.include "data/maps/HxMauvilleCity_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxMauvilleCity_Mart/scripts.inc"
	.include "data/maps/HxUnionRoad2_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxUnionRoad2_Mart/scripts.inc"
	.include "data/maps/HxVerdanturfTown_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxVerdanturfTown_Mart/scripts.inc"
	.include "data/maps/HxRustboroCity_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxRustboroCity_Mart/scripts.inc"
	.include "data/maps/HxDewfordTown_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxLavaridgeTown_Mart/scripts.inc"
	.include "data/maps/HxLavaridgeTown_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxPetalburgCity_Mart/scripts.inc"
	.include "data/maps/HxPetalburgCity_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxFortreeCity_Mart/scripts.inc"
	.include "data/maps/HxFortreeCity_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxLilycoveCity_Mart/scripts.inc"
	.include "data/maps/HxLilycoveCity_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxMossdeepCity_Mart/scripts.inc"
	.include "data/maps/HxMossdeepCity_PokemonCenter_1F/scripts.inc"
	.include "data/maps/HxSootopolisCity_Mart/scripts.inc"
	.include "data/maps/HxSootopolisCity_PokemonCenter_1F/scripts.inc"
