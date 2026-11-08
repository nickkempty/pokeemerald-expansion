HxRustboroCity_Gym_MapScripts::
	.byte 0

HxRustboroCity_Gym_EventScript_Roxanne::
	trainerbattle_single TRAINER_HX_ROXANNE_1, HxRustboroCity_Gym_Text_RoxanneIntro, HxRustboroCity_Gym_Text_RoxanneDefeat, HxRustboroCity_Gym_EventScript_RoxanneDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_ROCK_TOMB, HxRustboroCity_Gym_EventScript_GiveRockTomb
	msgbox HxRustboroCity_Gym_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

HxRustboroCity_Gym_EventScript_RoxanneDefeated::
	message HxRustboroCity_Gym_Text_ReceivedStoneBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_RUSTBORO_GYM
	setflag FLAG_HX_BADGE01_GET
	setvar VAR_HX_RUSTBORO_CITY_STATE, 1
	msgbox HxRustboroCity_Gym_Text_StoneBadgeInfo, MSGBOX_DEFAULT
	goto HxRustboroCity_Gym_EventScript_GiveRockTomb
	end

HxRustboroCity_Gym_EventScript_GiveRockTomb::
	checkitemspace ITEM_TM39
	goto_if_eq VAR_RESULT, FALSE, HxRustboroCity_Gym_EventScript_NoRoomForTM
	giveitem_msg HxRustboroCity_Gym_Text_ReceivedTMRockTomb, ITEM_TM39
	setflag FLAG_HX_RECEIVED_TM_ROCK_TOMB
	setvar VAR_0x8004, 8
	call EventScript_ZumoGymBonusTM
	msgbox HxRustboroCity_Gym_Text_ExplainRockTomb, MSGBOX_DEFAULT
	release
	end

HxRustboroCity_Gym_EventScript_NoRoomForTM::
	msgbox HxRustboroCity_Gym_Text_DontHaveRoomForThis, MSGBOX_DEFAULT
	release
	end

HxRustboroCity_Gym_Text_StoneBadgeInfo::
	.string "The STONE BADGE is the mark of a\n"
	.string "TRAINER who has proven that raw power\l"
	.string "can stand up to careful planning.\p"
	.string "Please take this with you, too.$"

HxRustboroCity_Gym_Text_ReceivedTMRockTomb::
	.string "{PLAYER} received TM39\n"
	.string "from ROXANNE.$"

HxRustboroCity_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"

HxRustboroCity_Gym_EventScript_Josh::
	trainerbattle_single TRAINER_HX_JOSH, HxRustboroCity_Gym_Text_JoshIntro, HxRustboroCity_Gym_Text_JoshDefeat
	msgbox HxRustboroCity_Gym_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

HxRustboroCity_Gym_EventScript_Tommy::
	trainerbattle_single TRAINER_HX_TOMMY, HxRustboroCity_Gym_Text_TommyIntro, HxRustboroCity_Gym_Text_TommyDefeat
	msgbox HxRustboroCity_Gym_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRustboroCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_HX_DEFEATED_RUSTBORO_GYM, HxRustboroCity_Gym_EventScript_GymGuidePostVictory
	msgbox HxRustboroCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

HxRustboroCity_Gym_EventScript_GymGuidePostVictory::
	msgbox HxRustboroCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

HxRustboroCity_Gym_EventScript_Marc::
	trainerbattle_single TRAINER_HX_MARC, HxRustboroCity_Gym_Text_MarcIntro, HxRustboroCity_Gym_Text_MarcDefeat
	msgbox HxRustboroCity_Gym_Text_MarcPostBattle, MSGBOX_AUTOCLOSE
	end

HxRustboroCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE01_GET, HxRustboroCity_Gym_EventScript_GymStatueCertified
	goto HxRustboroCity_Gym_EventScript_GymStatue
	end

HxRustboroCity_Gym_EventScript_GymStatueCertified::
	msgbox HxRustboroCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

HxRustboroCity_Gym_EventScript_GymStatue::
	msgbox HxRustboroCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

HxRustboroCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE01_GET, HxRustboroCity_Gym_EventScript_GymStatueCertified
	goto HxRustboroCity_Gym_EventScript_GymStatue
	end

