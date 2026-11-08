SilphCo_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SilphCo_1F_OnTransition
	.byte 0

SilphCo_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SILPH_CO_1F
	end

SilphCo_1F_EventScript_Receptionist::
	msgbox SilphCo_1F_Text_WelcomePresidentInBoardroom, MSGBOX_NPC
	end

SilphCo_1F_EventScript_FloorSign::
	msgbox SilphCo_1F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_1F_EventScript_StairsGuard::
	msgbox SilphCo_1F_Text_StairsSealed, MSGBOX_NPC
	end

SilphCo_1F_Text_WelcomePresidentInBoardroom::
	.string "Welcome.\p"
	.string "The PRESIDENT is in the boardroom\n"
	.string "on 11F.$"

SilphCo_1F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "1F$"


SilphCo_1F_Text_StairsSealed::
    .string "The stairs are sealed off!\n"
    .string "TEAM ECLIPSE business only.\p"
    .string "Our officers meet on 7F, and the\n"
    .string "CARD KEY is safe on 5F. Heh!\p"
    .string "…Wait, why did I say that?$"
