.equ NUM_DOOR_GRUNTS_DEFEATED, VAR_TEMP_1

RocketHideout_B4F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, RocketHideout_B4F_OnLoad
	.byte 0

RocketHideout_B4F_OnLoad::
	setvar NUM_DOOR_GRUNTS_DEFEATED, 0
	call_if_defeated TRAINER_TEAM_ROCKET_GRUNT_16, RocketHideout_B4F_EventScript_CountGruntDefeated
	call_if_defeated TRAINER_TEAM_ROCKET_GRUNT_17, RocketHideout_B4F_EventScript_CountGruntDefeated
	call_if_ne NUM_DOOR_GRUNTS_DEFEATED, 2, RocketHideout_B4F_EventScript_SetBarrier
	end

RocketHideout_B4F_EventScript_CountGruntDefeated::
	addvar NUM_DOOR_GRUNTS_DEFEATED, 1
	return

@ Zumo: ZUMO guards NOX's M-CORE lab (he still believes NOX)
RocketHideout_B4F_EventScript_Giovanni::
	lock
	faceplayer
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	msgbox RocketHideout_B4F_Text_ZumoIntro
	goto_if_eq VAR_RIVAL_VARIANT, 2, RocketHideout_B4F_EventScript_ZumoSquirtle
	goto_if_eq VAR_RIVAL_VARIANT, 1, RocketHideout_B4F_EventScript_ZumoBulbasaur
	trainerbattle_no_intro TRAINER_ZK_HIDEOUT_RIVAL_CHARMANDER, RocketHideout_B4F_Text_ZumoDefeat
	goto RocketHideout_B4F_EventScript_ZumoAfter

RocketHideout_B4F_EventScript_ZumoSquirtle::
	trainerbattle_no_intro TRAINER_ZK_HIDEOUT_RIVAL_SQUIRTLE, RocketHideout_B4F_Text_ZumoDefeat
	goto RocketHideout_B4F_EventScript_ZumoAfter

RocketHideout_B4F_EventScript_ZumoBulbasaur::
	trainerbattle_no_intro TRAINER_ZK_HIDEOUT_RIVAL_BULBASAUR, RocketHideout_B4F_Text_ZumoDefeat
	goto RocketHideout_B4F_EventScript_ZumoAfter

RocketHideout_B4F_EventScript_ZumoAfter::
	msgbox RocketHideout_B4F_Text_ZumoAfter
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_HIDEOUT_GIOVANNI
	setflag FLAG_ZK_HIDEOUT_ZUMO_GONE
	addobject LOCALID_SILPH_SCOPE
	clearflag FLAG_HIDE_SILPH_SCOPE
	setflag FLAG_HIDE_CELADON_ROCKETS
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message RocketHideout_B4F_Text_FoundReport
	waitmessage
	waitfanfare
	msgbox RocketHideout_B4F_Text_Report
	setflag FLAG_ZK_READ_MCORE_LOG
	release
	end

RocketHideout_B4F_EventScript_SilphScope::
	lock
	faceplayer
	removeobject LOCALID_SILPH_SCOPE
	giveitem ITEM_SILPH_SCOPE
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	release
	end

RocketHideout_B4F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_18, RocketHideout_B4F_Text_Grunt1Intro, RocketHideout_B4F_Text_Grunt1Defeat, RocketHideout_B4F_EventScript_DefeatedGrunt1
	msgbox RocketHideout_B4F_Text_Grunt1PostBattle
	release
	end

RocketHideout_B4F_EventScript_DefeatedGrunt1::
	msgbox RocketHideout_B4F_Text_Grunt1PostBattle
	addobject LOCALID_LIFT_KEY
	clearflag FLAG_HIDE_LIFT_KEY
	release
	end

RocketHideout_B4F_EventScript_LiftKey::
	lock
	faceplayer
	setflag FLAG_CAN_USE_ROCKET_HIDEOUT_LIFT
	removeobject LOCALID_LIFT_KEY
	giveitem ITEM_LIFT_KEY
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	release
	end

RocketHideout_B4F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_16, RocketHideout_B4F_Text_Grunt2Intro, RocketHideout_B4F_Text_Grunt2Defeat, RocketHideout_B4F_EventScript_DefeatedGrunt2
	msgbox RocketHideout_B4F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B4F_EventScript_DefeatedGrunt2::
	setvar NUM_DOOR_GRUNTS_DEFEATED, 0
	call_if_defeated TRAINER_TEAM_ROCKET_GRUNT_16, RocketHideout_B4F_EventScript_CountGruntDefeated
	call_if_defeated TRAINER_TEAM_ROCKET_GRUNT_17, RocketHideout_B4F_EventScript_CountGruntDefeated
	call_if_eq NUM_DOOR_GRUNTS_DEFEATED, 2, RocketHideout_B4F_EventScript_RemoveBarrier
	call_if_eq NUM_DOOR_GRUNTS_DEFEATED, 2, RocketHideout_B4F_EventScript_DrawMapForBarrierRemoval
	release
	end

RocketHideout_B4F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_17, RocketHideout_B4F_Text_Grunt3Intro, RocketHideout_B4F_Text_Grunt3Defeat, RocketHideout_B4F_EventScript_DefeatedGrunt3
	msgbox RocketHideout_B4F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B4F_EventScript_DefeatedGrunt3::
	setvar NUM_DOOR_GRUNTS_DEFEATED, 0
	call_if_defeated TRAINER_TEAM_ROCKET_GRUNT_16, RocketHideout_B4F_EventScript_CountGruntDefeated
	call_if_defeated TRAINER_TEAM_ROCKET_GRUNT_17, RocketHideout_B4F_EventScript_CountGruntDefeated
	call_if_eq NUM_DOOR_GRUNTS_DEFEATED, 2, RocketHideout_B4F_EventScript_RemoveBarrier
	call_if_eq NUM_DOOR_GRUNTS_DEFEATED, 2, RocketHideout_B4F_EventScript_DrawMapForBarrierRemoval
	release
	end

RocketHideout_B4F_EventScript_DrawMapForBarrierRemoval::
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	return

RocketHideout_B4F_EventScript_SetBarrier::
	setmetatile 17, 12, METATILE_SilphCo_HideoutBarrier_TopLeft_Floor, 1
	setmetatile 18, 12, METATILE_SilphCo_HideoutBarrier_TopRight_Floor, 1
	setmetatile 17, 13, METATILE_SilphCo_HideoutBarrier_BottomLeft, 1
	setmetatile 18, 13, METATILE_SilphCo_HideoutBarrier_BottomRight, 1
	setmetatile 17, 14, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 18, 14, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	return

RocketHideout_B4F_EventScript_RemoveBarrier::
	setmetatile 17, 12, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 18, 12, METATILE_SilphCo_HideoutFloor, 0
	setmetatile 17, 13, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 18, 13, METATILE_SilphCo_HideoutFloor, 0
	setmetatile 17, 14, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 18, 14, METATILE_SilphCo_HideoutFloor, 0
	return

RocketHideout_B4F_Text_GiovanniIntro::
    .string "So, you're the kid who keeps\n"
    .string "getting in my way.\p"
    .string "I am NOX, leader of TEAM ECLIPSE.\n"
    .string "Look around. Every cage here holds\l"
    .string "a POKéMON the storm brought to\n"
    .string "KANTO.\p"
    .string "The sea took everything from me\n"
    .string "once. Soon, with my WAVE CALLER,\l"
    .string "the sea will obey me instead!$"

RocketHideout_B4F_Text_GiovanniDefeat::
    .string "Hmph… The current turned against\n"
    .string "me.$"

RocketHideout_B4F_Text_GiovanniPostBattle::
    .string "Keep this hideout and its cages.\n"
    .string "I'm done here.\p"
    .string "The WAVE CALLER only needs one\n"
    .string "more piece of technology, and\l"
    .string "SILPH CO. has it. We'll meet again,\n"
    .string "kid.$"

RocketHideout_B4F_Text_Grunt2Intro::
    .string "The boss is busy! Get lost, kid!$"

RocketHideout_B4F_Text_Grunt2Defeat::
	.string "Burned again!$"

RocketHideout_B4F_Text_Grunt2PostBattle::
    .string "Those POKéMON from JOHTO and HOENN\n"
    .string "fetch a high price… Ugh, I lost.$"

RocketHideout_B4F_Text_Grunt3Intro::
    .string "The LIFT KEY? You'll never get it!$"

RocketHideout_B4F_Text_Grunt3Defeat::
	.string "Ayaya!$"

RocketHideout_B4F_Text_Grunt3PostBattle::
    .string "Fine, take the key. You still can't\n"
    .string "stop NOX!$"

RocketHideout_B4F_Text_Grunt1Intro::
    .string "You can't free these caged\n"
    .string "POKéMON! They belong to TEAM\l"
    .string "ECLIPSE now!$"

RocketHideout_B4F_Text_Grunt1Defeat::
	.string "No!$"

RocketHideout_B4F_Text_Grunt1PostBattle::
    .string "The boss lost his ship and crew in a\n"
    .string "storm long ago. He says the sea\l"
    .string "owes him.$"


RocketHideout_B4F_Text_ZumoIntro::
    .string "{RIVAL}: {PLAYER}?!\n"
    .string "What are you doing down here?\p"
    .string "NOX told me everything. A huge\n"
    .string "storm is coming from the sea.\p"
    .string "Only his M-CORE can protect all\n"
    .string "the POKéMON that washed up in\l"
    .string "KANTO. So I'm helping him!\p"
    .string "Smell ya later… after I beat you!$"

RocketHideout_B4F_Text_ZumoDefeat::
    .string "Tch… Why do you always win?$"

RocketHideout_B4F_Text_ZumoAfter::
    .string "{RIVAL}: …Fine.\p"
    .string "You think NOX is lying? Then read\n"
    .string "this. I found it on his desk.\p"
    .string "I'm going to ask him myself!$"

RocketHideout_B4F_Text_FoundReport::
    .string "{PLAYER} picked up NOX's\n"
    .string "lab report!$"

RocketHideout_B4F_Text_Report::
    .string "M-CORE PROJECT - REPORT 7\p"
    .string "The M-CORE is complete. Its psychic\n"
    .string "waves come from SUBJECT M2.\p"
    .string "SUBJECT M2 broke out of its cage\n"
    .string "and fled toward ROCK TUNNEL.\p"
    .string "TEAM MAGMA will bring the RED ORB\n"
    .string "by sea to VERMILION on the\l"
    .string "S.S. ANNE.\p"
    .string "PHASE 2: amplify the M-CORE at\n"
    .string "SILPH CO. in SAFFRON.\p"
    .string "PHASE 3: wake the giant sleeping\n"
    .string "under the sea south of CINNABAR.\p"
    .string "- NOX$"
