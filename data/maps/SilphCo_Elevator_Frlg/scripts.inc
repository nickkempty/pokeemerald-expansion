SilphCo_Elevator_Frlg_MapScripts::
	.byte 0

SilphCo_Elevator_EventScript_FloorSelect::
	lockall
	setvar VAR_0x8004, 1
	call_if_unset FLAG_TEMP_2, EventScript_GetElevatorFloor
	copyvar VAR_0x8005, VAR_ELEVATOR_FLOOR
	special DrawElevatorCurrentFloorWindow
	message gText_WantWhichFloor
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_SILPHCO_FLOORS
	specialvar VAR_RESULT, InitElevatorFloorSelectMenuPos
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, SilphCo_Elevator_EventScript_To11F
	case 1, SilphCo_Elevator_EventScript_To10F
	case 2, SilphCo_Elevator_EventScript_To9F
	case 3, SilphCo_Elevator_EventScript_To8F
	case 4, SilphCo_Elevator_EventScript_To7F
	case 5, SilphCo_Elevator_EventScript_To6F
	case 6, SilphCo_Elevator_EventScript_To5F
	case 7, SilphCo_Elevator_EventScript_To4F
	case 8, SilphCo_Elevator_EventScript_To3F
	case 9, SilphCo_Elevator_EventScript_To2F
	case 10, SilphCo_Elevator_EventScript_To1F
	case 11, SilphCo_Elevator_EventScript_ExitFloorSelect
	case 127, SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To1F::
	setvar VAR_0x8006, 4
	setdynamicwarp MAP_SILPH_CO_1F, 255, 22, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 4, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 4
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To2F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_Elevator_EventScript_FloorLocked
	setvar VAR_0x8006, 5
	setdynamicwarp MAP_SILPH_CO_2F, 255, 22, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 5, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 5
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To3F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_Elevator_EventScript_FloorLocked
	setvar VAR_0x8006, 6
	setdynamicwarp MAP_SILPH_CO_3F, 255, 22, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 6, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 6
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To4F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_Elevator_EventScript_FloorLocked
	setvar VAR_0x8006, 7
	setdynamicwarp MAP_SILPH_CO_4F, 255, 22, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 7, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 7
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To5F::
	setvar VAR_0x8006, 8
	setdynamicwarp MAP_SILPH_CO_5F, 255, 22, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 8, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 8
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To6F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_Elevator_EventScript_FloorLocked
	setvar VAR_0x8006, 9
	setdynamicwarp MAP_SILPH_CO_6F, 255, 20, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 9, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 9
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To7F::
	setvar VAR_0x8006, 10
	setdynamicwarp MAP_SILPH_CO_7F, 255, 23, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 10, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 10
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To8F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_Elevator_EventScript_FloorLocked
	setvar VAR_0x8006, 11
	setdynamicwarp MAP_SILPH_CO_8F, 255, 22, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 11, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 11
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To9F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_Elevator_EventScript_FloorLocked
	setvar VAR_0x8006, 12
	setdynamicwarp MAP_SILPH_CO_9F, 255, 24, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 12, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 12
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To10F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_Elevator_EventScript_FloorLocked
	setvar VAR_0x8006, 13
	setdynamicwarp MAP_SILPH_CO_10F, 255, 13, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 13, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 13
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_To11F::
	goto_if_lt VAR_MAP_SCENE_SILPH_CO_7F, 1, SilphCo_Elevator_EventScript_Floor11Locked
	setvar VAR_0x8006, 14
	setdynamicwarp MAP_SILPH_CO_11F, 255, 13, 3
	goto_if_eq VAR_ELEVATOR_FLOOR, 14, SilphCo_Elevator_EventScript_ExitFloorSelect
	call SilphCo_Elevator_EventScript_MoveElevator
	setvar VAR_ELEVATOR_FLOOR, 14
	goto SilphCo_Elevator_EventScript_ExitFloorSelect
	end

SilphCo_Elevator_EventScript_ExitFloorSelect::
	special CloseElevatorCurrentFloorWindow
	releaseall
	end

SilphCo_Elevator_EventScript_MoveElevator::
	special CloseElevatorCurrentFloorWindow
	closemessage
	waitse
	special AnimateElevator
	waitstate
	setflag FLAG_TEMP_2
	return

SilphCo_Elevator_EventScript_FloorLocked::
	special CloseElevatorCurrentFloorWindow
	msgbox SilphCo_Elevator_Text_FloorLocked
	releaseall
	end

SilphCo_Elevator_EventScript_Floor11Locked::
	special CloseElevatorCurrentFloorWindow
	msgbox SilphCo_Elevator_Text_Floor11Locked
	releaseall
	end

SilphCo_Elevator_Text_FloorLocked::
	.string "…The button won't light up.\n"
	.string "TEAM ECLIPSE locked this floor.\p"
	.string "Only 5F and 7F seem to be open.$"

SilphCo_Elevator_Text_Floor11Locked::
	.string "11F - ACCESS DENIED\n"
	.string "ECLIPSE OFFICER CODE REQUIRED.\p"
	.string "Someone on 7F might know a way up…$"
