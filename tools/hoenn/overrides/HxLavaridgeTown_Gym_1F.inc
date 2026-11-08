HxLavaridgeTown_Gym_1F_EventScript_Flannery::
	trainerbattle_single TRAINER_HX_FLANNERY_1, HxLavaridgeTown_Gym_1F_Text_FlanneryIntro, HxLavaridgeTown_Gym_1F_Text_FlanneryDefeat, HxLavaridgeTown_Gym_1F_EventScript_FlanneryDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_OVERHEAT, HxLavaridgeTown_Gym_1F_EventScript_GiveOverheat
	msgbox HxLavaridgeTown_Gym_1F_Text_FlanneryPostBattle, MSGBOX_DEFAULT
	release
	end

HxLavaridgeTown_Gym_1F_EventScript_FlanneryDefeated::
	message HxLavaridgeTown_Gym_1F_Text_ReceivedHeatBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_LAVARIDGE_GYM
	setflag FLAG_HX_BADGE04_GET
	setvar VAR_HX_LAVARIDGE_TOWN_STATE, 1
	msgbox HxLavaridgeTown_Gym_1F_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	goto HxLavaridgeTown_Gym_1F_EventScript_GiveOverheat
	end

HxLavaridgeTown_Gym_1F_EventScript_GiveOverheat::
	checkitemspace ITEM_TM50
	goto_if_eq VAR_RESULT, FALSE, HxLavaridgeTown_Gym_1F_EventScript_NoRoomForTM
	giveitem_msg HxLavaridgeTown_Gym_1F_Text_ReceivedTMOverheat, ITEM_TM50
	setflag FLAG_HX_RECEIVED_TM_OVERHEAT
	setvar VAR_0x8004, 10
	call EventScript_ZumoGymBonusTM
	msgbox HxLavaridgeTown_Gym_1F_Text_ExplainOverheat, MSGBOX_DEFAULT
	release
	end

HxLavaridgeTown_Gym_1F_EventScript_NoRoomForTM::
	msgbox HxLavaridgeTown_Gym_1F_Text_DontHaveRoomForThis, MSGBOX_DEFAULT
	release
	end

HxLavaridgeTown_Gym_1F_Text_ReceivedTMOverheat::
	.string "{PLAYER} received TM50\n"
	.string "from FLANNERY.$"

HxLavaridgeTown_Gym_1F_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"
