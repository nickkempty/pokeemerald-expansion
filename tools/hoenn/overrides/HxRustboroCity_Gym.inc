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
