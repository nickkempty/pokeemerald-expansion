HxDewfordTown_Gym_EventScript_Brawly::
	trainerbattle_single TRAINER_HX_BRAWLY_1, HxDewfordTown_Gym_Text_BrawlyIntro, HxDewfordTown_Gym_Text_BrawlyDefeat, HxDewfordTown_Gym_EventScript_BrawlyDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_BULK_UP, HxDewfordTown_Gym_EventScript_GiveBulkUp
	msgbox HxDewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

HxDewfordTown_Gym_EventScript_BrawlyDefeated::
	message HxDewfordTown_Gym_Text_ReceivedKnuckleBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_DEWFORD_GYM
	setflag FLAG_HX_BADGE02_GET
	setvar VAR_HX_DEWFORD_TOWN_STATE, 1
	msgbox HxDewfordTown_Gym_Text_KnuckleBadgeInfo, MSGBOX_DEFAULT
	goto HxDewfordTown_Gym_EventScript_GiveBulkUp
	end

HxDewfordTown_Gym_EventScript_GiveBulkUp::
	checkitemspace ITEM_TM08
	goto_if_eq VAR_RESULT, FALSE, HxDewfordTown_Gym_EventScript_NoRoomForTM
	giveitem_msg HxDewfordTown_Gym_Text_ReceivedTMBulkUp, ITEM_TM08
	setflag FLAG_HX_RECEIVED_TM_BULK_UP
	setvar VAR_0x8004, 9
	call EventScript_ZumoGymBonusTM
	msgbox HxDewfordTown_Gym_Text_ExplainBulkUp, MSGBOX_DEFAULT
	release
	end

HxDewfordTown_Gym_EventScript_NoRoomForTM::
	msgbox HxDewfordTown_Gym_Text_DontHaveRoomForThis, MSGBOX_DEFAULT
	release
	end

HxDewfordTown_Gym_Text_KnuckleBadgeInfo::
	.string "The KNUCKLE BADGE is proof that you\n"
	.string "can stand your ground against the\l"
	.string "roughest waves.\p"
	.string "Please take this with you, too.$"

HxDewfordTown_Gym_Text_ReceivedTMBulkUp::
	.string "{PLAYER} received TM08\n"
	.string "from BRAWLY.$"

HxDewfordTown_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"
