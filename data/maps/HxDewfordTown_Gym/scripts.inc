HxDewfordTown_Gym_MapScripts::
	.byte 0

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

HxDewfordTown_Gym_EventScript_Takao::
	trainerbattle_single TRAINER_HX_TAKAO, HxDewfordTown_Gym_Text_TakaoIntro, HxDewfordTown_Gym_Text_TakaoDefeat
	msgbox HxDewfordTown_Gym_Text_TakaoPostBattle, MSGBOX_AUTOCLOSE
	end

HxDewfordTown_Gym_EventScript_Jocelyn::
	trainerbattle_single TRAINER_HX_JOCELYN, HxDewfordTown_Gym_Text_JocelynIntro, HxDewfordTown_Gym_Text_JocelynDefeat
	msgbox HxDewfordTown_Gym_Text_JocelynPostBattle, MSGBOX_AUTOCLOSE
	end

HxDewfordTown_Gym_EventScript_Laura::
	trainerbattle_single TRAINER_HX_LAURA, HxDewfordTown_Gym_Text_LauraIntro, HxDewfordTown_Gym_Text_LauraDefeat
	msgbox HxDewfordTown_Gym_Text_LauraPostBattle, MSGBOX_AUTOCLOSE
	end

HxDewfordTown_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_HX_DEFEATED_DEWFORD_GYM, HxDewfordTown_Gym_EventScript_GymGuidePostVictory
	msgbox HxDewfordTown_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

HxDewfordTown_Gym_EventScript_GymGuidePostVictory::
	msgbox HxDewfordTown_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

HxDewfordTown_Gym_EventScript_Cristian::
	trainerbattle_single TRAINER_HX_CRISTIAN, HxDewfordTown_Gym_Text_CristianIntro, HxDewfordTown_Gym_Text_CristianDefeat
	msgbox HxDewfordTown_Gym_Text_CristianPostBattle, MSGBOX_AUTOCLOSE
	end

HxDewfordTown_Gym_EventScript_Lilith::
	trainerbattle_single TRAINER_HX_LILITH, HxDewfordTown_Gym_Text_LilithIntro, HxDewfordTown_Gym_Text_LilithDefeat
	msgbox HxDewfordTown_Gym_Text_LilithPostBattle, MSGBOX_AUTOCLOSE
	end

HxDewfordTown_Gym_EventScript_Brenden::
	trainerbattle_single TRAINER_HX_BRENDEN, HxDewfordTown_Gym_Text_BrendenIntro, HxDewfordTown_Gym_Text_BrendenDefeat
	msgbox HxDewfordTown_Gym_Text_BrendenPostBattle, MSGBOX_AUTOCLOSE
	end

HxDewfordTown_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE02_GET, HxDewfordTown_Gym_EventScript_GymStatueCertified
	goto HxDewfordTown_Gym_EventScript_GymStatue
	end

HxDewfordTown_Gym_EventScript_GymStatueCertified::
	msgbox HxDewfordTown_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

HxDewfordTown_Gym_EventScript_GymStatue::
	msgbox HxDewfordTown_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

HxDewfordTown_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE02_GET, HxDewfordTown_Gym_EventScript_GymStatueCertified
	goto HxDewfordTown_Gym_EventScript_GymStatue
	end

