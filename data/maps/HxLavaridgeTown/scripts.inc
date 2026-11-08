HxLavaridgeTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, HxLavaridgeTown_OnTransition
	.byte 0

HxLavaridgeTown_OnTransition:
	setflag FLAG_HX_HIDE_LAVARIDGE_TOWN_MAXIE
	setflag FLAG_HX_HIDE_LAVARIDGE_TOWN_NOX
	end

HxLavaridgeTown_EventScript_GymExitTrigger::
	lockall
	clearflag FLAG_HX_HIDE_LAVARIDGE_TOWN_MAXIE
	clearflag FLAG_HX_HIDE_LAVARIDGE_TOWN_NOX
	addobject LOCALID_HX_LAVARIDGE_MAXIE
	addobject LOCALID_HX_LAVARIDGE_NOX
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_HX_LAVARIDGE_MAXIE, HxLavaridgeTown_Movement_MaxieWalk
	applymovement LOCALID_HX_LAVARIDGE_NOX, HxLavaridgeTown_Movement_NoxWalk
	waitmovement 0
	msgbox HxLavaridgeTown_Text_MaxieIntro, MSGBOX_DEFAULT
	msgbox HxLavaridgeTown_Text_NoxLine, MSGBOX_DEFAULT
	msgbox HxLavaridgeTown_Text_MaxieSeal, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_HX_LAVARIDGE_MAXIE, HxLavaridgeTown_Movement_LeaveRight
	applymovement LOCALID_HX_LAVARIDGE_NOX, HxLavaridgeTown_Movement_LeaveRight
	waitmovement 0
	removeobject LOCALID_HX_LAVARIDGE_MAXIE
	removeobject LOCALID_HX_LAVARIDGE_NOX
	setflag FLAG_HX_HIDE_LAVARIDGE_TOWN_MAXIE
	setflag FLAG_HX_HIDE_LAVARIDGE_TOWN_NOX
	msgbox HxLavaridgeTown_Text_Aftermath, MSGBOX_DEFAULT
	giveitem ITEM_RARE_CANDY
	setvar VAR_HX_LAVARIDGE_TOWN_STATE, 2
	releaseall
	end

HxLavaridgeTown_Movement_MaxieWalk:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	face_left
	step_end

HxLavaridgeTown_Movement_NoxWalk:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	face_left
	step_end

HxLavaridgeTown_Movement_LeaveRight:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

HxLavaridgeTown_Text_MaxieIntro::
	.string "MAXIE: So this is the kid who has\n"
	.string "been tearing through the gyms.\p"
	.string "I am MAXIE, leader of TEAM MAGMA.\n"
	.string "I want the land to grow, and the\l"
	.string "land has grown. Thanks to HIM.$"

HxLavaridgeTown_Text_NoxLine::
	.string "NOX: The M-CORE gave GROUDON its\n"
	.string "strength. MAXIE's MAGMA keeps the\l"
	.string "ROAD from being swallowed by the sea.\p"
	.string "You have talent, kid. Don't waste it\n"
	.string "by getting in our way.$"

HxLavaridgeTown_Text_MaxieSeal::
	.string "MAXIE: We go to MT. CHIMNEY. The\n"
	.string "RED ORB will answer to the M-CORE.\p"
	.string "Stay out of it, or the sea will\n"
	.string "have the two of us together!$"

HxLavaridgeTown_Text_Aftermath::
	.string "...They are gone. NOX and MAXIE are\n"
	.string "working together!\p"
	.string "Their words made it clear: the\n"
	.string "M-CORE is the real danger here.\p"
	.string "Something fell from MAXIE's coat\n"
	.string "as he left. Better take it.$"

HxLavaridgeTown_EventScript_ExpertF::
	msgbox HxLavaridgeTown_Text_OhYouLikeHotSprings, MSGBOX_NPC
	end

HxLavaridgeTown_EventScript_ExpertM::
	msgbox HxLavaridgeTown_Text_HotSpringsNeverRunDry, MSGBOX_NPC
	end

HxLavaridgeTown_EventScript_OldMan::
	msgbox HxLavaridgeTown_Text_PokemonNippedBackside, MSGBOX_SIGN
	end

HxLavaridgeTown_EventScript_Twin::
	msgbox HxLavaridgeTown_Text_BatheInHotSpringsEveryDay, MSGBOX_NPC
	end

HxLavaridgeTown_EventScript_HotSpringsOldWoman1::
	msgbox HxLavaridgeTown_Text_IfPokemonInHotSprings, MSGBOX_NPC
	end

HxLavaridgeTown_EventScript_HotSpringsOldWoman2::
	msgbox HxLavaridgeTown_Text_HotSpringsClaims, MSGBOX_NPC
	end

HxLavaridgeTown_EventScript_EggWoman::
	msgbox HxLavaridgeTown_Text_EverySoOftenEggFoundAtDayCare, MSGBOX_NPC
	end

HxLavaridgeTown_EventScript_HerbShopSign::
	msgbox HxLavaridgeTown_Text_HerbShopSign, MSGBOX_SIGN
	end

HxLavaridgeTown_EventScript_GymSign::
	msgbox HxLavaridgeTown_Text_GymSign, MSGBOX_SIGN
	end

HxLavaridgeTown_EventScript_TownSign::
	msgbox HxLavaridgeTown_Text_TownSign, MSGBOX_SIGN
	end

