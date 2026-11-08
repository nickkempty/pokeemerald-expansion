HxSootopolisCity_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, HxSootopolisCity_Gym_1F_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, HxSootopolisCity_Gym_1F_OnResume
	map_script MAP_SCRIPT_ON_LOAD, HxSootopolisCity_Gym_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, HxSootopolisCity_Gym_1F_OnTransition
	.byte 0

HxSootopolisCity_Gym_1F_OnTransition:
	setvar VAR_ICE_STEP_COUNT, 1
	end

HxSootopolisCity_Gym_1F_OnResume:
	setstepcallback STEP_CB_SOOTOPOLIS_ICE
	end

HxSootopolisCity_Gym_1F_OnLoad:
	call HxSootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special SetSootopolisGymCrackedIceMetatiles
	end

@ ice puzzle (Emerald): stairs appear after enough ice has been cracked (step callback in src/field_tasks.c)
HxSootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles::
	goto_if_lt VAR_ICE_STEP_COUNT, 8, HxSootopolisCity_Gym_1F_EventScript_StopCheckingStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 28, HxSootopolisCity_Gym_1F_EventScript_OpenFirstStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 67, HxSootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs
	setmetatile 8, 4, 0x207, FALSE
	setmetatile 8, 5, 0x207, FALSE
HxSootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs::
	setmetatile 8, 10, 0x207, FALSE
	setmetatile 8, 11, 0x207, FALSE
HxSootopolisCity_Gym_1F_EventScript_OpenFirstStairs::
	setmetatile 8, 15, 0x207, FALSE
	setmetatile 8, 16, 0x207, FALSE
HxSootopolisCity_Gym_1F_EventScript_StopCheckingStairs::
	return

HxSootopolisCity_Gym_1F_OnFrame:
	map_script_2 VAR_ICE_STEP_COUNT, 8, HxSootopolisCity_Gym_1F_EventScript_UnlockStairs
	map_script_2 VAR_ICE_STEP_COUNT, 28, HxSootopolisCity_Gym_1F_EventScript_UnlockStairs
	map_script_2 VAR_ICE_STEP_COUNT, 67, HxSootopolisCity_Gym_1F_EventScript_UnlockStairs
	map_script_2 VAR_ICE_STEP_COUNT, 0, HxSootopolisCity_Gym_1F_EventScript_FallThroughIce
	.2byte 0

HxSootopolisCity_Gym_1F_EventScript_UnlockStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call HxSootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

HxSootopolisCity_Gym_1F_EventScript_FallThroughIce::
	lockall
	delay 20
	applymovement LOCALID_PLAYER, HxSootopolisCity_Gym_1F_Movement_FallThroughIce
	waitmovement 0
	playse SE_FALL
	delay 60
	warphole MAP_HX_SOOTOPOLIS_CITY_GYM_B1F
	waitstate
	end

HxSootopolisCity_Gym_1F_Movement_FallThroughIce:
	set_invisible
	step_end

HxSootopolisCity_Gym_1F_EventScript_Juan::
	goto_if_set FLAG_HX_DEFEATED_SOOTOPOLIS_GYM, HxSootopolisCity_Gym_1F_EventScript_JuanBattle
	goto_if_unset FLAG_HX_BADGE07_GET, HxSootopolisCity_Gym_1F_EventScript_JuanNotReady
HxSootopolisCity_Gym_1F_EventScript_JuanBattle::
	trainerbattle_single TRAINER_HX_JUAN_1, HxSootopolisCity_Gym_1F_Text_ZkJuanIntro, HxSootopolisCity_Gym_1F_Text_ZkJuanDefeat, HxSootopolisCity_Gym_1F_EventScript_JuanDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_WATER_PULSE, HxSootopolisCity_Gym_1F_EventScript_GiveWaterPulse
	msgbox HxSootopolisCity_Gym_1F_Text_ZkJuanPostBattle, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_Gym_1F_EventScript_JuanDefeated::
	message HxSootopolisCity_Gym_1F_Text_ReceivedRainBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_SOOTOPOLIS_GYM
	setflag FLAG_HX_BADGE08_GET
	msgbox HxSootopolisCity_Gym_1F_Text_ZkExplainRainBadge, MSGBOX_DEFAULT
	goto HxSootopolisCity_Gym_1F_EventScript_GiveWaterPulse
	end

HxSootopolisCity_Gym_1F_EventScript_GiveWaterPulse::
	checkitemspace ITEM_TM03
	goto_if_eq VAR_RESULT, FALSE, HxSootopolisCity_Gym_1F_EventScript_NoRoomForTM
	giveitem_msg HxSootopolisCity_Gym_1F_Text_ReceivedTMWaterPulse, ITEM_TM03
	setflag FLAG_HX_RECEIVED_TM_WATER_PULSE
	setvar VAR_0x8004, 14
	call EventScript_ZumoGymBonusTM
	msgbox HxSootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	msgbox HxSootopolisCity_Gym_1F_Text_ZkJuanPostBattle, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_Gym_1F_EventScript_NoRoomForTM::
	msgbox HxSootopolisCity_Gym_1F_Text_DontHaveRoomForThis, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_Gym_1F_EventScript_JuanNotReady::
	lock
	faceplayer
	msgbox HxSootopolisCity_Gym_1F_Text_ZkJuanNotReady, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_Gym_1F_Text_ReceivedTMWaterPulse::
	.string "{PLAYER} received TM03\n"
	.string "from JUAN.$"

HxSootopolisCity_Gym_1F_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"

HxSootopolisCity_Gym_1F_Text_ZkJuanDefeat::
	.string "Ahahaha, excellent!\n"
	.string "Very well, you are the winner.\p"
	.string "Please accept the RAIN BADGE!$"

HxSootopolisCity_Gym_1F_Text_ZkExplainRainBadge::
	.string "JUAN: With the RAIN BADGE, you now\n"
	.string "hold all 16 BADGES of KANTO and\l"
	.string "HOENN.\p"
	.string "And, so that you never forget the\n"
	.string "battle we shared, take this…$"

HxSootopolisCity_Gym_1F_Text_ZkJuanPostBattle::
	.string "JUAN: You calmed GROUDON and KYOGRE,\n"
	.string "and now you hold every BADGE.\p"
	.string "The UNION LEAGUE awaits you at\n"
	.string "INDIGO PLATEAU, beyond VICTORY ROAD\l"
	.string "in KANTO. Go, and shine!$"

HxSootopolisCity_Gym_1F_Text_ZkJuanNotReady::
	.string "JUAN: Ah, a challenger…\p"
	.string "But you do not yet carry the MIND\n"
	.string "BADGE of MOSSDEEP CITY.\p"
	.string "Return when TATE and LIZA have\n"
	.string "acknowledged you.$"

HxSootopolisCity_Gym_1F_Text_ZkJuanIntro::
	.string "JUAN: So, you are the one who\n"
	.string "freed GROUDON from M-CORE.\p"
	.string "SOOTOPOLIS owes you its life.\n"
	.string "But a GYM LEADER must still test\l"
	.string "every challenger.\p"
	.string "Please, bear witness to a grand\n"
	.string "illusion of water sculpted by my\l"
	.string "POKéMON and myself!$"

HxSootopolisCity_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_HX_DEFEATED_SOOTOPOLIS_GYM, HxSootopolisCity_Gym_1F_EventScript_GymGuidePostVictory
	msgbox HxSootopolisCity_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox HxSootopolisCity_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

HxSootopolisCity_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE08_GET, HxSootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto HxSootopolisCity_Gym_1F_EventScript_GymStatue
	end

HxSootopolisCity_Gym_1F_EventScript_GymStatueCertified::
	msgbox HxSootopolisCity_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

HxSootopolisCity_Gym_1F_EventScript_GymStatue::
	msgbox HxSootopolisCity_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

HxSootopolisCity_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE08_GET, HxSootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto HxSootopolisCity_Gym_1F_EventScript_GymStatue
	end

