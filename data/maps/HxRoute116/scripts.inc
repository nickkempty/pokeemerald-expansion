HxRoute116_MapScripts::
	.byte 0

HxRoute116_EventScript_Joey::
	trainerbattle_single TRAINER_HX_JOEY, HxRoute116_Text_JoeyIntro, HxRoute116_Text_JoeyDefeat
	msgbox HxRoute116_Text_JoeyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_Jose::
	trainerbattle_single TRAINER_HX_JOSE, HxRoute116_Text_JoseIntro, HxRoute116_Text_JoseDefeat
	msgbox HxRoute116_Text_JosePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_ItemEther::
	finditem ITEM_ETHER
	end

HxRoute116_EventScript_ItemRepel::
	finditem ITEM_REPEL
	end

HxRoute116_EventScript_Clark::
	trainerbattle_single TRAINER_HX_CLARK, HxRoute116_Text_ClarkIntro, HxRoute116_Text_ClarkDefeat
	msgbox HxRoute116_Text_ClarkPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_ItemHPUp::
	finditem ITEM_HP_UP
	end

HxRoute116_EventScript_Janice::
	trainerbattle_single TRAINER_HX_JANICE, HxRoute116_Text_JaniceIntro, HxRoute116_Text_JaniceDefeat
	msgbox HxRoute116_Text_JanicePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_Karen::
	trainerbattle_single TRAINER_HX_KAREN_1, HxRoute116_Text_KarenIntro, HxRoute116_Text_KarenDefeat
	msgbox HxRoute116_Text_KarenPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_Jerry::
	trainerbattle_single TRAINER_HX_JERRY_1, HxRoute116_Text_JerryIntro, HxRoute116_Text_JerryDefeat
	msgbox HxRoute116_Text_JerryPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_ItemXSpecial::
	finditem ITEM_X_SPECIAL
	end

HxRoute116_EventScript_Sarah::
	trainerbattle_single TRAINER_HX_SARAH, HxRoute116_Text_SarahIntro, HxRoute116_Text_SarahDefeat
	msgbox HxRoute116_Text_SarahPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_Dawson::
	trainerbattle_single TRAINER_HX_DAWSON, HxRoute116_Text_DawsonIntro, HxRoute116_Text_DawsonDefeat
	msgbox HxRoute116_Text_DawsonPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_ItemPotion::
	finditem ITEM_POTION
	end

HxRoute116_EventScript_Johnson::
	trainerbattle_single TRAINER_HX_JOHNSON, HxRoute116_Text_JohnsonIntro, HxRoute116_Text_JohnsonDefeat
	msgbox HxRoute116_Text_JohnsonPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_Devan::
	trainerbattle_single TRAINER_HX_DEVAN, HxRoute116_Text_DevanIntro, HxRoute116_Text_DevanDefeat
	msgbox HxRoute116_Text_DevanPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute116_EventScript_RouteSignRustboro::
	msgbox HxRoute116_Text_RouteSignRustboro, MSGBOX_SIGN
	end

HxRoute116_EventScript_RusturfTunnelSign::
	msgbox HxRoute116_Text_RusturfTunnelSign, MSGBOX_SIGN
	end

HxRoute116_EventScript_TunnelersRestHouseSign::
	msgbox HxRoute116_Text_TunnelersRestHouse, MSGBOX_SIGN
	end

HxRoute116_EventScript_TrainerTipsBToStopEvolution::
	msgbox HxRoute116_Text_TrainerTipsBToStopEvolution, MSGBOX_SIGN
	end

HxRoute116_EventScript_TrainerTipsBagHasPockets::
	msgbox HxRoute116_Text_TrainerTipsBagHasPockets, MSGBOX_SIGN
	end

