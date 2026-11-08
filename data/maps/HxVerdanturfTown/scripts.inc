HxVerdanturfTown_MapScripts::
	.byte 0

HxVerdanturfTown_EventScript_Man::
	msgbox HxVerdanturfTown_Text_AirCleanHere, MSGBOX_NPC
	end

HxVerdanturfTown_EventScript_Twin::
	msgbox HxVerdanturfTown_Text_ManDugTunnelForLove, MSGBOX_NPC
	end

HxVerdanturfTown_EventScript_Boy::
	lock
	faceplayer
	goto_if_set FLAG_HX_RUSTURF_TUNNEL_OPENED, HxVerdanturfTown_EventScript_BoyTunnelOpen
	msgbox HxVerdanturfTown_Text_GuyTryingToBustThroughCave, MSGBOX_DEFAULT
	release
	end

HxVerdanturfTown_EventScript_BoyTunnelOpen::
	msgbox HxVerdanturfTown_Text_EasyToGetToRustboroNow, MSGBOX_DEFAULT
	release
	end

HxVerdanturfTown_EventScript_Camper::
	msgbox HxVerdanturfTown_Text_MakeBattleTentDebut, MSGBOX_NPC
	end

HxVerdanturfTown_EventScript_TownSign::
	msgbox HxVerdanturfTown_Text_TownSign, MSGBOX_SIGN
	end

HxVerdanturfTown_EventScript_WandasHouseSign::
	msgbox HxVerdanturfTown_Text_WandasHouse, MSGBOX_SIGN
	end

HxVerdanturfTown_EventScript_BattleTentSign::
	msgbox HxVerdanturfTown_Text_BattleTentSign, MSGBOX_SIGN
	end

HxVerdanturfTown_EventScript_RusturfTunnelSign::
	msgbox HxVerdanturfTown_Text_RusturfTunnelSign, MSGBOX_SIGN
	end

