HxSootopolisCity_OnFrame:
	map_script_2 VAR_HX_SOOTO_STATE, 0, HxSootopolisCity_EventScript_ArrivalScene
	.2byte 0

HxSootopolisCity_EventScript_ArrivalScene::
	lockall
	setvar VAR_HX_SOOTO_STATE, 1
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 30
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	msgbox HxSootopolisCity_Text_ZkArrival1
	closemessage
	setvar VAR_0x8004, 3
	setvar VAR_0x8005, 3
	setvar VAR_0x8006, 30
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	msgbox HxSootopolisCity_Text_ZkArrival2
	releaseall
	end

HxSootopolisCity_EventScript_Maxie::
	lock
	faceplayer
	goto_if_set FLAG_HX_NOX_FINAL_DONE, HxSootopolisCity_EventScript_MaxieAfter
	msgbox HxSootopolisCity_Text_ZkMaxie, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_EventScript_MaxieAfter::
	msgbox HxSootopolisCity_Text_ZkMaxieAfter, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_EventScript_Legend::
	msgbox HxSootopolisCity_Text_ZkLegend, MSGBOX_SIGN
	end

HxSootopolisCity_EventScript_GymBlocker::
	msgbox HxSootopolisCity_Text_ZkGymBlocker, MSGBOX_NPC
	end

HxSootopolisCity_EventScript_Diver::
	lock
	faceplayer
	msgbox HxSootopolisCity_Text_ZkDiverAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HxSootopolisCity_EventScript_DiverNo
	msgbox HxSootopolisCity_Text_ZkDiverGo
	closemessage
	warp MAP_HX_MOSSDEEP_CITY, 28, 17
	waitstate
	release
	end

HxSootopolisCity_EventScript_DiverNo::
	msgbox HxSootopolisCity_Text_ZkDiverNo
	release
	end

HxSootopolisCity_Text_ZkArrival1::
	.string "The ground is shaking!\n"
	.string "Something huge clashes in the lake!$"

HxSootopolisCity_Text_ZkArrival2::
	.string "GROUDON, under M-CORE's control,\n"
	.string "is fighting another giant POKéMON…\p"
	.string "That's KYOGRE!\n"
	.string "Someone is standing by the GYM…$"

HxSootopolisCity_Text_ZkMaxie::
	.string "MAXIE: You came, child.\p"
	.string "I used the BLUE ORB to wake KYOGRE.\n"
	.string "It is the only thing that can hold\l"
	.string "GROUDON back.\p"
	.string "But it won't last. NOX controls\n"
	.string "GROUDON from deep inside the CAVE\l"
	.string "OF ORIGIN, north of the city.\p"
	.string "Break M-CORE, and GROUDON will be\n"
	.string "free. Go! Quickly!$"

HxSootopolisCity_Text_ZkMaxieAfter::
	.string "MAXIE: KYOGRE has returned to the\n"
	.string "sea, and GROUDON sleeps.\p"
	.string "I wanted to expand the land…\n"
	.string "but not like this.\p"
	.string "TEAM MAGMA owes you, child.$"

HxSootopolisCity_Text_ZkLegend::
	.string "GROUDON and KYOGRE are locked in\n"
	.string "battle! Getting closer is too\l"
	.string "dangerous!$"

HxSootopolisCity_Text_ZkGymBlocker::
	.string "JUAN went to the CAVE OF ORIGIN\n"
	.string "to stop the giants, but he hasn't\l"
	.string "come back…\p"
	.string "The GYM is closed until this is\n"
	.string "over!$"

HxSootopolisCity_Text_ZkDiverAsk::
	.string "I know the underwater cave under\n"
	.string "the crater rim.\p"
	.string "Want me to take you back out to\n"
	.string "MOSSDEEP CITY by boat?$"

HxSootopolisCity_Text_ZkDiverGo::
	.string "Hang on tight!$"

HxSootopolisCity_Text_ZkDiverNo::
	.string "Just talk to me when you're ready.$"
