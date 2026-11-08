HxRoute118_MapScripts::
	.byte 0

HxRoute118_EventScript_Perry::
	trainerbattle_single TRAINER_HX_PERRY, HxRoute118_Text_PerryIntro, HxRoute118_Text_PerryDefeat
	msgbox HxRoute118_Text_PerryPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute118_EventScript_Girl::
	msgbox HxRoute118_Text_CanCrossRiversWithSurf, MSGBOX_NPC
	end

HxRoute118_EventScript_GoodRodFisherman::
	lock
	faceplayer
	goto_if_set FLAG_HX_RECEIVED_GOOD_ROD, HxRoute118_EventScript_ReceivedGoodRod
	msgbox HxRoute118_Text_YouAgreeGoodRodIsGood, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, HxRoute118_EventScript_ReceiveGoodRod
	goto_if_eq VAR_RESULT, NO, HxRoute118_EventScript_DeclineGoodRod
	end

HxRoute118_EventScript_ReceivedGoodRod::
	msgbox HxRoute118_Text_TryCatchingMonWithGoodRod, MSGBOX_DEFAULT
	release
	end

HxRoute118_EventScript_ReceiveGoodRod::
	msgbox HxRoute118_Text_IdenticalMindsTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_GOOD_ROD
	setflag FLAG_HX_RECEIVED_GOOD_ROD
	msgbox HxRoute118_Text_TryYourLuckFishing, MSGBOX_DEFAULT
	release
	end

HxRoute118_EventScript_DeclineGoodRod::
	msgbox HxRoute118_Text_DontYouLikeToFish, MSGBOX_DEFAULT
	release
	end

HxRoute118_EventScript_Rose::
	trainerbattle_single TRAINER_HX_ROSE_1, HxRoute118_Text_RoseIntro, HxRoute118_Text_RoseDefeat
	msgbox HxRoute118_Text_RosePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute118_EventScript_Barny::
	trainerbattle_single TRAINER_HX_BARNY, HxRoute118_Text_BarnyIntro, HxRoute118_Text_BarnyDefeat
	msgbox HxRoute118_Text_BarnyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute118_EventScript_RouteSignMauville::
	msgbox HxRoute118_Text_RouteSignMauville, MSGBOX_SIGN
	end

HxRoute118_EventScript_RouteSign119::
	msgbox HxRoute118_Text_RouteSign119, MSGBOX_SIGN
	end

