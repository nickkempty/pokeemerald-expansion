HxDewfordTown_EventScript_Briney::
	lock
	faceplayer
	goto_if_unset FLAG_HX_DEFEATED_DEWFORD_GYM, HxDewfordTown_EventScript_BrineyRoute104
	msgbox HxDewfordTown_Text_BrineySlateport, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HxDewfordTown_EventScript_BrineyRoute104
	closemessage
	fadescreen FADE_TO_BLACK
	warp MAP_HX_SLATEPORT_CITY, 19, 21
	waitstate
	release
	end

HxDewfordTown_EventScript_BrineyRoute104::
	msgbox HxDewfordTown_Text_BrineyReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HxDewfordTown_EventScript_BrineyStay
	closemessage
	fadescreen FADE_TO_BLACK
	warp MAP_HX_ROUTE104, 17, 51
	waitstate
	release
	end

HxDewfordTown_EventScript_BrineyStay::
	msgbox HxDewfordTown_Text_BrineyStay, MSGBOX_DEFAULT
	release
	end

HxDewfordTown_Text_BrineyReturn::
	.string "MR. BRINEY: Want to head back to\n"
	.string "ROUTE 104?$"

HxDewfordTown_Text_BrineyStay::
	.string "MR. BRINEY: I'll be right here.$"

HxDewfordTown_OnTransition:
	setflag FLAG_HX_HIDE_DEWFORD_TOWN_AQUA_GRUNT
	end

HxDewfordTown_EventScript_GymExitTrigger::
	lockall
	clearflag FLAG_HX_HIDE_DEWFORD_TOWN_AQUA_GRUNT
	addobject LOCALID_HX_DEWFORD_AQUA
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_HX_DEWFORD_AQUA, HxDewfordTown_Movement_GruntWalk
	waitmovement 0
	msgbox HxDewfordTown_Text_GruntIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_HX_GRUNT_MUSEUM_1, HxDewfordTown_Text_GruntDefeat
	msgbox HxDewfordTown_Text_GruntEscape, MSGBOX_DEFAULT
	closemessage
	playse SE_WARP_OUT
	removeobject LOCALID_HX_DEWFORD_AQUA
	setflag FLAG_HX_HIDE_DEWFORD_TOWN_AQUA_GRUNT
	waitse
	giveitem ITEM_RARE_CANDY
	setflag FLAG_HX_DEWFORD_AQUA_DONE
	setvar VAR_HX_DEWFORD_TOWN_STATE, 2
	releaseall
	end

HxDewfordTown_Movement_GruntWalk:
	walk_fast_left
	walk_fast_left
	face_left
	step_end

HxDewfordTown_Text_GruntIntro::
	.string "AQUA GRUNT: So you're the kid who\n"
	.string "beat our man in RUSTBORO.\p"
	.string "The BLUE ORB went on ahead with\n"
	.string "the boss. I stayed behind to make\l"
	.string "sure nobody follows!$"

HxDewfordTown_Text_GruntDefeat::
	.string "AQUA GRUNT: Urgh! You're tougher\n"
	.string "than you look!$"

HxDewfordTown_Text_GruntEscape::
	.string "AQUA GRUNT: Listen well. ARCHIE is\n"
	.string "gathering the two ORBS and the\l"
	.string "legends will clash for the whole\l"
	.string "sea. The next stop is LAVARIDGE,\l"
	.string "where MAGMA hides the RED ORB!\p"
	.string "Too late to stop us now!$"

HxDewfordTown_Text_BrineySlateport::
	.string "MR. BRINEY: The way to MAUVILLE and\n"
	.string "the east is quickest from SLATEPORT.\p"
	.string "Shall I sail you there?$"
