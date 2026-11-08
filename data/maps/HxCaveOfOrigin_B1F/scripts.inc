HxCaveOfOrigin_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, HxCaveOfOrigin_B1F_OnTransition
	.byte 0

HxCaveOfOrigin_B1F_OnTransition:
	setflag FLAG_HX_GROUDON_RING_HIDE
	goto_if_unset FLAG_HX_NOX_FINAL_DONE, HxCaveOfOrigin_B1F_OnTransitionEnd
	goto_if_set FLAG_HX_GROUDON_GONE, HxCaveOfOrigin_B1F_OnTransitionEnd
	clearflag FLAG_HX_GROUDON_RING_HIDE
HxCaveOfOrigin_B1F_OnTransitionEnd:
	end

HxCaveOfOrigin_B1F_EventScript_NoxTrigger::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_HX_COO_NOX, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox HxCaveOfOrigin_B1F_Text_NoxIntro
	closemessage
	trainerbattle_no_intro TRAINER_ZK_NOX_FINAL, HxCaveOfOrigin_B1F_Text_NoxDefeat
	msgbox HxCaveOfOrigin_B1F_Text_NoxAfter
	closemessage
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 30
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_HX_COO_NOX
	removeobject LOCALID_HX_COO_GROUDON_PIT
	setflag FLAG_HX_NOX_FINAL_DONE
	clearflag FLAG_HX_GROUDON_RING_HIDE
	addobject LOCALID_HX_COO_GROUDON
	setvar VAR_HX_COO_STATE, 1
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox HxCaveOfOrigin_B1F_Text_MCoreBroken
	releaseall
	end

HxCaveOfOrigin_B1F_EventScript_Nox::
	lock
	faceplayer
	msgbox HxCaveOfOrigin_B1F_Text_NoxIntro, MSGBOX_DEFAULT
	release
	end

HxCaveOfOrigin_B1F_EventScript_Groudon::
	lock
	goto_if_unset FLAG_HX_NOX_FINAL_DONE, HxCaveOfOrigin_B1F_EventScript_GroudonControlled
	msgbox HxCaveOfOrigin_B1F_Text_GroudonCalm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HxCaveOfOrigin_B1F_EventScript_GroudonLeave
	playmoncry SPECIES_GROUDON, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_GROUDON, 80
	dowildbattle
	removeobject LOCALID_HX_COO_GROUDON
	setflag FLAG_HX_GROUDON_GONE
	release
	end

HxCaveOfOrigin_B1F_EventScript_GroudonControlled::
	msgbox HxCaveOfOrigin_B1F_Text_GroudonControlled, MSGBOX_DEFAULT
	release
	end

HxCaveOfOrigin_B1F_EventScript_GroudonLeave::
	release
	end

HxCaveOfOrigin_B1F_Text_NoxIntro::
	.string "NOX: So you followed me here, too.\p"
	.string "Look at it. GROUDON, bound to my\n"
	.string "will by M-CORE.\p"
	.string "MAXIE's KYOGRE can't hold it back\n"
	.string "forever. The sea will become land,\l"
	.string "and the land will be MINE!\p"
	.string "TEAM ECLIPSE ends here… or you do!$"

HxCaveOfOrigin_B1F_Text_NoxDefeat::
	.string "M-CORE… is overloading…?$"

HxCaveOfOrigin_B1F_Text_NoxAfter::
	.string "NOX: No… M-CORE is breaking!\p"
	.string "All those years… KANTO, HOENN…\n"
	.string "I lost to a child. Again.\p"
	.string "…But the sea still hides one more\n"
	.string "secret. We will meet again, child,\l"
	.string "far from here!$"

HxCaveOfOrigin_B1F_Text_MCoreBroken::
	.string "M-CORE shattered into pieces!\n"
	.string "NOX vanished in the confusion.\p"
	.string "The rumbling stops… GROUDON is free\n"
	.string "and calm. KYOGRE sinks back into\l"
	.string "the deep sea.\p"
	.string "GROUDON climbed out of the pit and\n"
	.string "is standing right behind you!$"

HxCaveOfOrigin_B1F_Text_GroudonCalm::
	.string "GROUDON is looking at you calmly…\p"
	.string "Will you battle it?$"

HxCaveOfOrigin_B1F_Text_GroudonControlled::
	.string "GROUDON's eyes glow with a cold\n"
	.string "light… It's being controlled!$"

