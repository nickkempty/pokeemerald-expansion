HxFortreeCity_Gym_OnTransition:
	special RotatingGate_InitPuzzle
	end

HxFortreeCity_Gym_OnWarp:
	map_script_2 VAR_TEMP_4, 0, HxFortreeCity_Gym_EventScript_InitRotatingGates
	.2byte 0

@ the gate puzzle keeps its gate orientations in VAR_TEMP_0 - VAR_TEMP_3
HxFortreeCity_Gym_EventScript_InitRotatingGates::
	special RotatingGate_InitPuzzleAndGraphics
	setvar VAR_TEMP_4, 1
	end

HxFortreeCity_Gym_EventScript_Winona::
	goto_if_set FLAG_HX_DEFEATED_FORTREE_GYM, HxFortreeCity_Gym_EventScript_WinonaBattle
	goto_if_unset FLAG_HX_BADGE05_GET, HxFortreeCity_Gym_EventScript_WinonaNotReady
HxFortreeCity_Gym_EventScript_WinonaBattle::
	trainerbattle_single TRAINER_HX_WINONA_1, HxFortreeCity_Gym_Text_WinonaIntro, HxFortreeCity_Gym_Text_WinonaDefeat, HxFortreeCity_Gym_EventScript_WinonaDefeated, NO_MUSIC
	goto_if_unset FLAG_HX_RECEIVED_TM_AERIAL_ACE, HxFortreeCity_Gym_EventScript_GiveAerialAce
	msgbox HxFortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

HxFortreeCity_Gym_EventScript_WinonaDefeated::
	message HxFortreeCity_Gym_Text_ReceivedFeatherBadge
	waitmessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	setflag FLAG_HX_DEFEATED_FORTREE_GYM
	setflag FLAG_HX_BADGE06_GET
	msgbox HxFortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	goto HxFortreeCity_Gym_EventScript_GiveAerialAce
	end

HxFortreeCity_Gym_EventScript_GiveAerialAce::
	checkitemspace ITEM_TM40
	goto_if_eq VAR_RESULT, FALSE, HxFortreeCity_Gym_EventScript_NoRoomForTM
	giveitem_msg HxFortreeCity_Gym_Text_ReceivedTMAerialAce, ITEM_TM40
	setflag FLAG_HX_RECEIVED_TM_AERIAL_ACE
	setvar VAR_0x8004, 12
	call EventScript_ZumoGymBonusTM
	msgbox HxFortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	msgbox HxFortreeCity_Gym_Text_WinonaSkyPillar, MSGBOX_DEFAULT
	release
	end

HxFortreeCity_Gym_EventScript_NoRoomForTM::
	msgbox HxFortreeCity_Gym_Text_DontHaveRoomForThis, MSGBOX_DEFAULT
	release
	end

HxFortreeCity_Gym_Text_ReceivedTMAerialAce::
	.string "{PLAYER} received TM40\n"
	.string "from WINONA.$"

HxFortreeCity_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.\n"
	.string "Make some space in your BAG first!$"

HxFortreeCity_Gym_Text_WinonaSkyPillar::
	.string "WINONA: One more thing.\p"
	.string "The skies above HOENN have been\n"
	.string "restless ever since the land\l"
	.string "bridge rose from the sea.\p"
	.string "My bird POKéMON sense a great\n"
	.string "presence high above SKY PILLAR.\p"
	.string "If GROUDON and KYOGRE ever clash,\n"
	.string "only the one who rules the sky\l"
	.string "could stand between them.$"

HxFortreeCity_Gym_EventScript_WinonaNotReady::
	lock
	faceplayer
	msgbox HxFortreeCity_Gym_Text_WinonaNotReady, MSGBOX_DEFAULT
	release
	end

HxFortreeCity_Gym_Text_WinonaNotReady::
	.string "WINONA: You came all this way\n"
	.string "through my gates… Impressive.\p"
	.string "But your wings are not ready yet.\n"
	.string "Earn the BALANCE BADGE from NORMAN\l"
	.string "in PETALBURG CITY, then return.$"
