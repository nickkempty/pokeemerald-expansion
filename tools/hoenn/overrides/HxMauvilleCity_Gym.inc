HxMauvilleCity_Gym_OnLoad:
	goto_if_set FLAG_HX_DEFEATED_MAUVILLE_GYM, HxMauvilleCity_Gym_EventScript_DeactivatePuzzle
	switch VAR_HX_MAUVILLE_GYM_STATE
	case 0, HxMauvilleCity_Gym_EventScript_UpdateBarriers
	case 1, HxMauvilleCity_Gym_EventScript_Switch1Pressed
	case 2, HxMauvilleCity_Gym_EventScript_Switch2Pressed
	case 3, HxMauvilleCity_Gym_EventScript_Switch3Pressed
	case 4, HxMauvilleCity_Gym_EventScript_Switch4Pressed
	end

HxMauvilleCity_Gym_EventScript_UpdateBarriers::
	goto_if_set FLAG_HX_MAUVILLE_GYM_BARRIERS_STATE, HxMauvilleCity_Gym_EventScript_SetAltBarriers
	end

HxMauvilleCity_Gym_EventScript_SetAltBarriers::
	setmetatile 3, 11, METATILE_MauvilleGym_RedBeamV1_On, TRUE
	setmetatile 3, 12, METATILE_MauvilleGym_RedBeamV2_On, TRUE
	setmetatile 3, 13, METATILE_MauvilleGym_PoleTop_On, TRUE
	setmetatile 4, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 5, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 4, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 5, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 7, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 8, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 7, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 8, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 4, 13, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 13, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 14, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 14, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 1, 10, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 2, 10, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 1, 11, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 2, 11, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 6, 8, METATILE_MauvilleGym_PoleBottom_On, TRUE
	setmetatile 6, 9, METATILE_MauvilleGym_FloorTile, FALSE
	setmetatile 6, 10, METATILE_MauvilleGym_PoleTop_Off, FALSE
	setmetatile 4, 6, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 6, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 7, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 7, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	end

HxMauvilleCity_Gym_EventScript_Switch1Pressed::
	setvar VAR_0x8004, 0
	special MauvilleGymPressSwitch
	goto HxMauvilleCity_Gym_EventScript_UpdateBarriers
	end

HxMauvilleCity_Gym_EventScript_Switch2Pressed::
	setvar VAR_0x8004, 1
	special MauvilleGymPressSwitch
	goto HxMauvilleCity_Gym_EventScript_UpdateBarriers
	end

HxMauvilleCity_Gym_EventScript_Switch3Pressed::
	setvar VAR_0x8004, 2
	special MauvilleGymPressSwitch
	goto HxMauvilleCity_Gym_EventScript_UpdateBarriers
	end

HxMauvilleCity_Gym_EventScript_Switch4Pressed::
	setvar VAR_0x8004, 3
	special MauvilleGymPressSwitch
	goto HxMauvilleCity_Gym_EventScript_UpdateBarriers
	end

HxMauvilleCity_Gym_EventScript_DeactivatePuzzle::
	special MauvilleGymDeactivatePuzzle
	end

HxMauvilleCity_Gym_EventScript_Switch1::
	lockall
	goto_if_set FLAG_HX_DEFEATED_MAUVILLE_GYM, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_HX_MAUVILLE_GYM_STATE, 1, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_HX_MAUVILLE_GYM_STATE, 1
	setvar VAR_0x8004, 0
	goto HxMauvilleCity_Gym_EventScript_PressFloorSwitch
	end

HxMauvilleCity_Gym_EventScript_Switch2::
	lockall
	goto_if_set FLAG_HX_DEFEATED_MAUVILLE_GYM, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_HX_MAUVILLE_GYM_STATE, 2, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_HX_MAUVILLE_GYM_STATE, 2
	setvar VAR_0x8004, 1
	goto HxMauvilleCity_Gym_EventScript_PressFloorSwitch
	end

HxMauvilleCity_Gym_EventScript_Switch3::
	lockall
	goto_if_set FLAG_HX_DEFEATED_MAUVILLE_GYM, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_HX_MAUVILLE_GYM_STATE, 3, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_HX_MAUVILLE_GYM_STATE, 3
	setvar VAR_0x8004, 2
	goto HxMauvilleCity_Gym_EventScript_PressFloorSwitch
	end

HxMauvilleCity_Gym_EventScript_Switch4::
	lockall
	goto_if_set FLAG_HX_DEFEATED_MAUVILLE_GYM, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_HX_MAUVILLE_GYM_STATE, 4, HxMauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_HX_MAUVILLE_GYM_STATE, 4
	setvar VAR_0x8004, 3
	goto HxMauvilleCity_Gym_EventScript_PressFloorSwitch
	end

HxMauvilleCity_Gym_EventScript_PressFloorSwitch::
	special MauvilleGymSetDefaultBarriers
	special MauvilleGymPressSwitch
	special DrawWholeMapView
	playse SE_UNLOCK
	goto_if_set FLAG_HX_MAUVILLE_GYM_BARRIERS_STATE, HxMauvilleCity_Gym_EventScript_ClearBarriersAltState
	goto_if_unset FLAG_HX_MAUVILLE_GYM_BARRIERS_STATE, HxMauvilleCity_Gym_EventScript_SetBarriersAltState
	releaseall
	end

HxMauvilleCity_Gym_EventScript_SwitchDoNothing::
	releaseall
	end

HxMauvilleCity_Gym_EventScript_SetBarriersAltState::
	setflag FLAG_HX_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

HxMauvilleCity_Gym_EventScript_ClearBarriersAltState::
	clearflag FLAG_HX_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

HxMauvilleCity_Gym_EventScript_Wattson::
	trainerbattle_single TRAINER_HX_WATTSON_1, HxMauvilleCity_Gym_Text_WattsonIntro, HxMauvilleCity_Gym_Text_WattsonDefeat, HxMauvilleCity_Gym_EventScript_WattsonDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_SHOCK_WAVE, HxMauvilleCity_Gym_EventScript_GiveShockWave
	msgbox HxMauvilleCity_Gym_Text_WattsonPostBattle
	release
	end

HxMauvilleCity_Gym_EventScript_WattsonDefeated::
	message HxMauvilleCity_Gym_Text_ReceivedDynamoBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_MAUVILLE_GYM
	setflag FLAG_HX_BADGE03_GET
	special MauvilleGymDeactivatePuzzle
	special DrawWholeMapView
	playse SE_UNLOCK
	msgbox HxMauvilleCity_Gym_Text_ExplainDynamoBadgeTakeThis
	goto HxMauvilleCity_Gym_EventScript_GiveShockWave
	end

HxMauvilleCity_Gym_EventScript_GiveShockWave::
	checkitemspace ITEM_TM34
	goto_if_eq VAR_RESULT, FALSE, HxMauvilleCity_Gym_EventScript_NoRoomForTM
	giveitem_msg HxMauvilleCity_Gym_Text_ReceivedTMShockWave, ITEM_TM34
	setflag FLAG_HX_RECEIVED_TM_SHOCK_WAVE
	setvar VAR_0x8004, 2
	call EventScript_ZumoGymBonusTM
	msgbox HxMauvilleCity_Gym_Text_ExplainShockWave
	release
	end

HxMauvilleCity_Gym_EventScript_NoRoomForTM::
	msgbox HxMauvilleCity_Gym_Text_DontHaveRoomForThis
	release
	end

HxMauvilleCity_Gym_Text_ReceivedTMShockWave::
	.string "{PLAYER} received TM34\n"
	.string "from WATTSON.$"

HxMauvilleCity_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"
