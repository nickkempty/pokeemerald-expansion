HxMossdeepCity_Gym_MapScripts::
	.byte 0

HxMossdeepCity_Gym_EventScript_TateAndLiza::
	goto_if_set FLAG_HX_DEFEATED_MOSSDEEP_GYM, HxMossdeepCity_Gym_EventScript_TateAndLizaBattle
	goto_if_unset FLAG_HX_BADGE06_GET, HxMossdeepCity_Gym_EventScript_TateAndLizaNotReady
HxMossdeepCity_Gym_EventScript_TateAndLizaBattle::
	trainerbattle_double TRAINER_HX_TATE_AND_LIZA_1, HxMossdeepCity_Gym_Text_TateAndLizaIntro, HxMossdeepCity_Gym_Text_TateAndLizaDefeat, HxMossdeepCity_Gym_Text_TateAndLizaNeedTwoMons, HxMossdeepCity_Gym_EventScript_TateAndLizaDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_CALM_MIND, HxMossdeepCity_Gym_EventScript_GiveCalmMind
	msgbox HxMossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_Gym_EventScript_TateAndLizaDefeated::
	message HxMossdeepCity_Gym_Text_ReceivedMindBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_MOSSDEEP_GYM
	setflag FLAG_HX_BADGE07_GET
	msgbox HxMossdeepCity_Gym_Text_ZkExplainMindBadge, MSGBOX_DEFAULT
	goto HxMossdeepCity_Gym_EventScript_GiveCalmMind
	end

HxMossdeepCity_Gym_EventScript_GiveCalmMind::
	checkitemspace ITEM_TM04
	goto_if_eq VAR_RESULT, FALSE, HxMossdeepCity_Gym_EventScript_NoRoomForTM
	giveitem_msg HxMossdeepCity_Gym_Text_ReceivedTMCalmMind, ITEM_TM04
	setflag FLAG_HX_RECEIVED_TM_CALM_MIND
	setvar VAR_0x8004, 13
	call EventScript_ZumoGymBonusTM
	msgbox HxMossdeepCity_Gym_Text_ExplainCalmMind, MSGBOX_DEFAULT
	msgbox HxMossdeepCity_Gym_Text_ZkVision, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_Gym_EventScript_NoRoomForTM::
	msgbox HxMossdeepCity_Gym_Text_DontHaveRoomForThis, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_Gym_EventScript_TateAndLizaNotReady::
	lock
	faceplayer
	msgbox HxMossdeepCity_Gym_Text_ZkNotReady, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_Gym_Text_ReceivedTMCalmMind::
	.string "{PLAYER} received TM04\n"
	.string "from TATE and LIZA.$"

HxMossdeepCity_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"

HxMossdeepCity_Gym_Text_ZkExplainMindBadge::
	.string "TATE: The MIND BADGE proves you\n"
	.string "LIZA: are one with your POKéMON.\p"
	.string "TATE: You should also take this, too.$"

HxMossdeepCity_Gym_Text_ZkVision::
	.string "TATE: …Wait. LIZA, do you feel it?\n"
	.string "LIZA: A burning mind, heading west…\p"
	.string "TATE: It's GROUDON! NOX is driving it\n"
	.string "LIZA: toward SOOTOPOLIS CITY!\p"
	.string "TATE: And deep under SOOTOPOLIS…\n"
	.string "LIZA: Something huge is waking up.\p"
	.string "TATE: Swim south along ROUTE 126.\n"
	.string "LIZA: There's a cave under the crater!$"

HxMossdeepCity_Gym_Text_ZkNotReady::
	.string "TATE: Hehehe… We knew you'd come.\n"
	.string "LIZA: But you're not ready yet.\p"
	.string "TATE: Bring WINONA's FEATHER BADGE\n"
	.string "LIZA: from FORTREE CITY first!$"

HxMossdeepCity_Gym_EventScript_YellowFloorSwitch::
	lockall
	setvar VAR_0x8004, 0
	goto HxMossdeepCity_Gym_EventScript_RotateFloor
	end

HxMossdeepCity_Gym_EventScript_BlueFloorSwitch::
	lockall
	setvar VAR_0x8004, 1
	goto HxMossdeepCity_Gym_EventScript_RotateFloor
	end

HxMossdeepCity_Gym_EventScript_GreenFloorSwitch::
	lockall
	setvar VAR_0x8004, 2
	goto HxMossdeepCity_Gym_EventScript_RotateFloor
	end

HxMossdeepCity_Gym_EventScript_PurpleFloorSwitch::
	lockall
	setvar VAR_0x8004, 3
	goto HxMossdeepCity_Gym_EventScript_RotateFloor
	end

HxMossdeepCity_Gym_EventScript_RedFloorSwitch::
	lockall
	setvar VAR_0x8004, 4
	goto HxMossdeepCity_Gym_EventScript_RotateFloor
	end

@ rotating tile puzzle (src/rotating_tile_puzzle.c, ported from Emerald)
HxMossdeepCity_Gym_EventScript_RotateFloor::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	copyvar VAR_0x8006, VAR_0x8004
	setvar VAR_0x8004, FALSE
	special RotatingTilePuzzle_Init
	copyvar VAR_0x8004, VAR_0x8006
	special RotatingTilePuzzle_Move
	goto_if_eq VAR_0x8005, 0, HxMossdeepCity_Gym_EventScript_RotateFloorTurn
	waitmovement VAR_0x8005
HxMossdeepCity_Gym_EventScript_RotateFloorTurn::
	special RotatingTilePuzzle_Turn
	goto_if_eq VAR_0x8005, 0, HxMossdeepCity_Gym_EventScript_RotateFloorDone
	waitmovement VAR_0x8005
HxMossdeepCity_Gym_EventScript_RotateFloorDone::
	special RotatingTilePuzzle_Free
	releaseall
	end

HxMossdeepCity_Gym_EventScript_WarpToEntrance::
	lockall
	warpteleport MAP_HX_MOSSDEEP_CITY_GYM, 7, 30
	waitstate
	releaseall
	end

HxMossdeepCity_Gym_EventScript_Preston::
	trainerbattle_single TRAINER_HX_PRESTON, HxMossdeepCity_Gym_Text_PrestonIntro, HxMossdeepCity_Gym_Text_PrestonDefeat
	msgbox HxMossdeepCity_Gym_Text_PrestonPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Blake::
	trainerbattle_single TRAINER_HX_BLAKE, HxMossdeepCity_Gym_Text_BlakeIntro, HxMossdeepCity_Gym_Text_BlakeDefeat
	msgbox HxMossdeepCity_Gym_Text_BlakePostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Maura::
	trainerbattle_single TRAINER_HX_MAURA, HxMossdeepCity_Gym_Text_MauraIntro, HxMossdeepCity_Gym_Text_MauraDefeat
	msgbox HxMossdeepCity_Gym_Text_MauraPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Samantha::
	trainerbattle_single TRAINER_HX_SAMANTHA, HxMossdeepCity_Gym_Text_SamanthaIntro, HxMossdeepCity_Gym_Text_SamanthaDefeat
	msgbox HxMossdeepCity_Gym_Text_SamanthaPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Virgil::
	trainerbattle_single TRAINER_HX_VIRGIL, HxMossdeepCity_Gym_Text_VirgilIntro, HxMossdeepCity_Gym_Text_VirgilDefeat
	msgbox HxMossdeepCity_Gym_Text_VirgilPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Hannah::
	trainerbattle_single TRAINER_HX_HANNAH, HxMossdeepCity_Gym_Text_HannahIntro, HxMossdeepCity_Gym_Text_HannahDefeat
	msgbox HxMossdeepCity_Gym_Text_HannahPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_HX_DEFEATED_MOSSDEEP_GYM, HxMossdeepCity_Gym_EventScript_GymGuidePostVictory
	msgbox HxMossdeepCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_Gym_EventScript_GymGuidePostVictory::
	msgbox HxMossdeepCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_Gym_EventScript_Nate::
	trainerbattle_single TRAINER_HX_NATE, HxMossdeepCity_Gym_Text_NateIntro, HxMossdeepCity_Gym_Text_NateDefeat
	msgbox HxMossdeepCity_Gym_Text_NatePostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Sylvia::
	trainerbattle_single TRAINER_HX_SYLVIA, HxMossdeepCity_Gym_Text_SylviaIntro, HxMossdeepCity_Gym_Text_SylviaDefeat
	msgbox HxMossdeepCity_Gym_Text_SylviaPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Clifford::
	trainerbattle_single TRAINER_HX_CLIFFORD, HxMossdeepCity_Gym_Text_CliffordIntro, HxMossdeepCity_Gym_Text_CliffordDefeat
	msgbox HxMossdeepCity_Gym_Text_CliffordPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Macey::
	trainerbattle_single TRAINER_HX_MACEY, HxMossdeepCity_Gym_Text_MaceyIntro, HxMossdeepCity_Gym_Text_MaceyDefeat
	msgbox HxMossdeepCity_Gym_Text_MaceyPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Kathleen::
	trainerbattle_single TRAINER_HX_KATHLEEN, HxMossdeepCity_Gym_Text_KathleenIntro, HxMossdeepCity_Gym_Text_KathleenDefeat
	msgbox HxMossdeepCity_Gym_Text_KathleenPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_Nicholas::
	trainerbattle_single TRAINER_HX_NICHOLAS, HxMossdeepCity_Gym_Text_NicholasIntro, HxMossdeepCity_Gym_Text_NicholasDefeat
	msgbox HxMossdeepCity_Gym_Text_NicholasPostBattle, MSGBOX_AUTOCLOSE
	end

HxMossdeepCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE07_GET, HxMossdeepCity_Gym_EventScript_GymStatueCertified
	goto HxMossdeepCity_Gym_EventScript_GymStatue
	end

HxMossdeepCity_Gym_EventScript_GymStatueCertified::
	msgbox HxMossdeepCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

HxMossdeepCity_Gym_EventScript_GymStatue::
	msgbox HxMossdeepCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

HxMossdeepCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE07_GET, HxMossdeepCity_Gym_EventScript_GymStatueCertified
	goto HxMossdeepCity_Gym_EventScript_GymStatue
	end

