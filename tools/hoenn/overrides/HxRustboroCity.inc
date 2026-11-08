HxRustboroCity_OnTransition:
	setflag FLAG_HX_HIDE_RUSTBORO_CITY_AQUA_GRUNT
	setflag FLAG_HX_HIDE_RUSTBORO_CITY_DEVON_EMPLOYEE
	end

HxRustboroCity_EventScript_StolenGoodsTrigger0::
	goto HxRustboroCity_EventScript_AquaRaid
	end

HxRustboroCity_EventScript_StolenGoodsTrigger1::
	goto HxRustboroCity_EventScript_AquaRaid
	end

HxRustboroCity_EventScript_StolenGoodsTrigger2::
	goto HxRustboroCity_EventScript_AquaRaid
	end

HxRustboroCity_EventScript_StolenGoodsTrigger3::
	goto HxRustboroCity_EventScript_AquaRaid
	end

HxRustboroCity_EventScript_StolenGoodsTrigger4::
	goto HxRustboroCity_EventScript_AquaRaid
	end

@ The street outside DEVON CORP, right after the player beats ROXANNE: TEAM AQUA steals the BLUE ORB.
HxRustboroCity_EventScript_AquaRaid::
	lockall
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	playse SE_M_EXPLOSION
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 10
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	msgbox HxRustboroCity_Text_AquaBlast, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HX_HIDE_RUSTBORO_CITY_AQUA_GRUNT
	addobject LOCALID_HX_RUSTBORO_AQUA
	clearflag FLAG_HX_HIDE_RUSTBORO_CITY_DEVON_EMPLOYEE
	addobject LOCALID_HX_RUSTBORO_DEVON
	applymovement LOCALID_HX_RUSTBORO_AQUA, HxRustboroCity_Movement_GruntRun
	applymovement LOCALID_HX_RUSTBORO_DEVON, HxRustboroCity_Movement_EmployeeRun
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox HxRustboroCity_Text_EmployeeShout, MSGBOX_DEFAULT
	applymovement LOCALID_HX_RUSTBORO_AQUA, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox HxRustboroCity_Text_GruntIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_HX_GRUNT_WEATHER_INST_1, HxRustboroCity_Text_GruntDefeat
	msgbox HxRustboroCity_Text_GruntEscape, MSGBOX_DEFAULT
	closemessage
	playse SE_WARP_OUT
	removeobject LOCALID_HX_RUSTBORO_AQUA
	setflag FLAG_HX_HIDE_RUSTBORO_CITY_AQUA_GRUNT
	waitse
	applymovement LOCALID_HX_RUSTBORO_DEVON, HxRustboroCity_Movement_EmployeeTurn
	waitmovement 0
	msgbox HxRustboroCity_Text_EmployeeThanks, MSGBOX_DEFAULT
	giveitem ITEM_ULTRA_BALL, 10
	msgbox HxRustboroCity_Text_EmployeeBlueOrb, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_HX_RUSTBORO_DEVON, HxRustboroCity_Movement_EmployeeBack
	waitmovement 0
	removeobject LOCALID_HX_RUSTBORO_DEVON
	setflag FLAG_HX_HIDE_RUSTBORO_CITY_DEVON_EMPLOYEE
	setvar VAR_HX_RUSTBORO_CITY_STATE, 2
	setflag FLAG_HX_AQUA_RAIDED_DEVON
	releaseall
	end

HxRustboroCity_Movement_GruntRun:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	face_right
	step_end

HxRustboroCity_Movement_EmployeeRun:
	delay_8
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	face_right
	step_end

HxRustboroCity_Movement_EmployeeTurn:
	face_player
	step_end

HxRustboroCity_Movement_EmployeeBack:
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

HxRustboroCity_Text_AquaBlast::
	.string "BOOOM!\p"
	.string "That blast came from the direction\n"
	.string "of the DEVON CORP. building!$"

HxRustboroCity_Text_EmployeeShout::
	.string "DEVON EMPLOYEE: Th-thief!\n"
	.string "Somebody stop that man!\p"
	.string "He took the BLUE ORB from the\n"
	.string "research lab!$"

HxRustboroCity_Text_GruntIntro::
	.string "AQUA GRUNT: Out of my way, kid!\p"
	.string "The BLUE ORB belongs to TEAM AQUA\n"
	.string "now. The sea will rise again!\p"
	.string "GROUDON may have raised the land,\n"
	.string "but KYOGRE will take it all back!$"

HxRustboroCity_Text_GruntDefeat::
	.string "AQUA GRUNT: Gwaah!\n"
	.string "I lost?!$"

HxRustboroCity_Text_GruntEscape::
	.string "AQUA GRUNT: Hmph… It doesn't matter!\p"
	.string "The BLUE ORB is already on its way\n"
	.string "to our boss ARCHIE.\p"
	.string "Catch us if you can… out at sea,\n"
	.string "past DEWFORD!$"

HxRustboroCity_Text_EmployeeThanks::
	.string "DEVON EMPLOYEE: Thank you for\n"
	.string "stopping him!\p"
	.string "Please take these as a token of\n"
	.string "our gratitude.$"

HxRustboroCity_Text_EmployeeBlueOrb::
	.string "DEVON EMPLOYEE: But the BLUE ORB is\n"
	.string "gone…\p"
	.string "Our president says the ORBS are\n"
	.string "tied to the great POKéMON of the\l"
	.string "land and the sea.\p"
	.string "If TEAM AQUA awakens KYOGRE too,\n"
	.string "both HOENN and KANTO are in danger!\p"
	.string "Please, follow them to DEWFORD!$"
