HxRoute126_EventScript_UnderwaterCave::
	lockall
	msgbox HxRoute126_Text_UnderwaterCave, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HxRoute126_EventScript_UnderwaterCaveNo
	closemessage
	playse SE_M_DIVE
	warpteleport MAP_HX_SOOTOPOLIS_CITY, 43, 33
	waitstate
	releaseall
	end

HxRoute126_EventScript_UnderwaterCaveNo::
	releaseall
	end

HxRoute126_Text_UnderwaterCave::
	.string "There's a dark underwater cave under\n"
	.string "the crater rim.\p"
	.string "It leads inside to SOOTOPOLIS CITY.\n"
	.string "Swim through it?$"
