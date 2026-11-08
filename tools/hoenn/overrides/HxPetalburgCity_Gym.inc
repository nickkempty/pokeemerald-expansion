HxPetalburgCity_Gym_EventScript_Norman::
	lock
	faceplayer
	goto_if_set FLAG_HX_DEFEATED_PETALBURG_GYM, HxPetalburgCity_Gym_EventScript_NormanDone
	goto_if_unset FLAG_HX_BADGE04_GET, HxPetalburgCity_Gym_EventScript_NormanNotReady
	trainerbattle_single TRAINER_HX_NORMAN_1, HxPetalburgCity_Gym_Text_NormanIntroZumo, HxPetalburgCity_Gym_Text_NormanDefeat, HxPetalburgCity_Gym_EventScript_NormanDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_FACADE, HxPetalburgCity_Gym_EventScript_GiveFacade
	release
	end

HxPetalburgCity_Gym_EventScript_NormanNotReady::
	msgbox HxPetalburgCity_Gym_Text_NormanNotReady, MSGBOX_DEFAULT
	release
	end

HxPetalburgCity_Gym_EventScript_NormanDone::
	goto_if_unset FLAG_HX_RECEIVED_TM_FACADE, HxPetalburgCity_Gym_EventScript_GiveFacade
	msgbox HxPetalburgCity_Gym_Text_DadGoingToKeepTraining, MSGBOX_DEFAULT
	release
	end

HxPetalburgCity_Gym_EventScript_NormanDefeated::
	message HxPetalburgCity_Gym_Text_ReceivedBalanceBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_PETALBURG_GYM
	setflag FLAG_HX_BADGE05_GET
	msgbox HxPetalburgCity_Gym_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	goto HxPetalburgCity_Gym_EventScript_GiveFacade
	end

HxPetalburgCity_Gym_EventScript_GiveFacade::
	checkitemspace ITEM_TM42
	goto_if_eq VAR_RESULT, FALSE, HxPetalburgCity_Gym_EventScript_NoRoomForTM
	giveitem_msg HxPetalburgCity_Gym_Text_ReceivedTMFacade, ITEM_TM42
	setflag FLAG_HX_RECEIVED_TM_FACADE
	setvar VAR_0x8004, 11
	call EventScript_ZumoGymBonusTM
	msgbox HxPetalburgCity_Gym_Text_ExplainFacade, MSGBOX_DEFAULT
	release
	end

HxPetalburgCity_Gym_EventScript_NoRoomForTM::
	msgbox HxPetalburgCity_Gym_Text_DontHaveRoomForThis, MSGBOX_DEFAULT
	release
	end

HxPetalburgCity_Gym_Text_NormanNotReady::
	.string "DAD: {PLAYER}, you're not ready yet.\p"
	.string "Beat the LAVARIDGE GYM first.\n"
	.string "Then come and face me!$"

HxPetalburgCity_Gym_Text_NormanIntroZumo::
	.string "DAD: Hm…\n"
	.string "So, you really did collect GYM\n"
	.string "BADGES from KANTO all the way here.\p"
	.string "{PLAYER}…\p"
	.string "I'm so happy that I can have\n"
	.string "a real battle with my own child.\p"
	.string "But, a battle is a battle!\p"
	.string "I will do everything in my power as\n"
	.string "a GYM LEADER to win.\p"
	.string "{PLAYER}, you'd better give it your\n"
	.string "best shot, too!$"

HxPetalburgCity_Gym_Text_ReceivedTMFacade::
	.string "{PLAYER} received TM42\n"
	.string "from DAD.$"

HxPetalburgCity_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"
