HxRusturfTunnel_MapScripts::
	.byte 0

HxRusturfTunnel_EventScript_ItemPokeBall::
	finditem ITEM_POKE_BALL
	end

HxRusturfTunnel_EventScript_ItemMaxEther::
	finditem ITEM_MAX_ETHER
	end

HxRusturfTunnel_EventScript_Mike::
	trainerbattle_single TRAINER_HX_MIKE_2, HxRusturfTunnel_Text_MikeIntro, HxRusturfTunnel_Text_MikeDefeat
	msgbox HxRusturfTunnel_Text_MikePostBattle, MSGBOX_AUTOCLOSE
	end

