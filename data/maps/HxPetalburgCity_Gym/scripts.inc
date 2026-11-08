HxPetalburgCity_Gym_MapScripts::
	.byte 0

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

HxPetalburgCity_Gym_EventScript_Mary::
	trainerbattle_single TRAINER_HX_MARY, HxPetalburgCity_Gym_Text_MaryIntro, HxPetalburgCity_Gym_Text_MaryDefeat
	msgbox HxPetalburgCity_Gym_Text_MaryPostBattle, MSGBOX_AUTOCLOSE
	end

HxPetalburgCity_Gym_EventScript_Randall::
	trainerbattle_single TRAINER_HX_RANDALL, HxPetalburgCity_Gym_Text_RandallIntro, HxPetalburgCity_Gym_Text_RandallDefeat
	msgbox HxPetalburgCity_Gym_Text_RandallPostBattle, MSGBOX_AUTOCLOSE
	end

HxPetalburgCity_Gym_EventScript_Parker::
	trainerbattle_single TRAINER_HX_PARKER, HxPetalburgCity_Gym_Text_ParkerIntro, HxPetalburgCity_Gym_Text_ParkerDefeat
	msgbox HxPetalburgCity_Gym_Text_ParkerPostBattle, MSGBOX_AUTOCLOSE
	end

HxPetalburgCity_Gym_EventScript_Alexia::
	trainerbattle_single TRAINER_HX_ALEXIA, HxPetalburgCity_Gym_Text_AlexiaIntro, HxPetalburgCity_Gym_Text_AlexiaDefeat
	msgbox HxPetalburgCity_Gym_Text_AlexiaPostBattle, MSGBOX_AUTOCLOSE
	end

HxPetalburgCity_Gym_EventScript_George::
	trainerbattle_single TRAINER_HX_GEORGE, HxPetalburgCity_Gym_Text_GeorgeIntro, HxPetalburgCity_Gym_Text_GeorgeDefeat
	msgbox HxPetalburgCity_Gym_Text_GeorgePostBattle, MSGBOX_AUTOCLOSE
	end

HxPetalburgCity_Gym_EventScript_Jody::
	trainerbattle_single TRAINER_HX_JODY, HxPetalburgCity_Gym_Text_JodyIntro, HxPetalburgCity_Gym_Text_JodyDefeat
	msgbox HxPetalburgCity_Gym_Text_JodyPostBattle, MSGBOX_AUTOCLOSE
	end

HxPetalburgCity_Gym_EventScript_Berke::
	trainerbattle_single TRAINER_HX_BERKE, HxPetalburgCity_Gym_Text_BerkeIntro, HxPetalburgCity_Gym_Text_BerkeDefeat
	msgbox HxPetalburgCity_Gym_Text_BerkePostBattle, MSGBOX_AUTOCLOSE
	end

HxPetalburgCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_HX_DEFEATED_PETALBURG_GYM, HxPetalburgCity_Gym_EventScript_GymGuidePostVictory
	msgbox HxPetalburgCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

HxPetalburgCity_Gym_EventScript_GymGuidePostVictory::
	msgbox HxPetalburgCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

HxPetalburgCity_Gym_EventScript_SpeedRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_AccuracyRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_ConfusionRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_LeftDefenseRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_RightDefenseRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_RecoveryRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_LeftStrengthRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_RightStrengthRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_LeftOHKORoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_RightOHKORoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_LeftGymLeadersRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_RightGymLeadersRoomDoor::
	msgbox HxPetalburgCity_Gym_Text_DoorAppearsLocked, MSGBOX_NPC
	end

HxPetalburgCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE05_GET, HxPetalburgCity_Gym_EventScript_GymStatueCertified
	goto HxPetalburgCity_Gym_EventScript_GymStatue
	end

HxPetalburgCity_Gym_EventScript_GymStatueCertified::
	msgbox HxPetalburgCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

HxPetalburgCity_Gym_EventScript_GymStatue::
	msgbox HxPetalburgCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

HxPetalburgCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_HX_BADGE05_GET, HxPetalburgCity_Gym_EventScript_GymStatueCertified
	goto HxPetalburgCity_Gym_EventScript_GymStatue
	end

