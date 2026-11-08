HxRoute112_MapScripts::
	.byte 0

HxRoute112_EventScript_Brice::
	trainerbattle_single TRAINER_HX_BRICE, HxRoute112_Text_BriceIntro, HxRoute112_Text_BriceDefeat
	msgbox HxRoute112_Text_BricePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute112_EventScript_Larry::
	trainerbattle_single TRAINER_HX_LARRY, HxRoute112_Text_LarryIntro, HxRoute112_Text_LarryDefeat
	msgbox HxRoute112_Text_LarryPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute112_EventScript_Carol::
	trainerbattle_single TRAINER_HX_CAROL, HxRoute112_Text_CarolIntro, HxRoute112_Text_CarolDefeat
	msgbox HxRoute112_Text_CarolPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute112_EventScript_Trent::
	trainerbattle_single TRAINER_HX_TRENT_1, HxRoute112_Text_TrentIntro, HxRoute112_Text_TrentDefeat
	msgbox HxRoute112_Text_TrentPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute112_EventScript_Hiker::
	msgbox HxRoute112_Text_NotEasyToGetBackToLavaridge, MSGBOX_NPC
	end

HxRoute112_EventScript_ItemNugget::
	finditem ITEM_NUGGET
	end

HxRoute112_EventScript_Bryant::
	trainerbattle_single TRAINER_HX_BRYANT, HxRoute112_Text_BryantIntro, HxRoute112_Text_BryantDefeat
	msgbox HxRoute112_Text_BryantPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute112_EventScript_Shayla::
	trainerbattle_single TRAINER_HX_SHAYLA, HxRoute112_Text_ShaylaIntro, HxRoute112_Text_ShaylaDefeat
	msgbox HxRoute112_Text_ShaylaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute112_EventScript_MtChimneySign::
	msgbox HxRoute112_Text_MtChimneySign, MSGBOX_SIGN
	end

HxRoute112_EventScript_MtChimneyCableCarSign::
	msgbox HxRoute112_Text_MtChimneyCableCarSign, MSGBOX_SIGN
	end

HxRoute112_EventScript_RouteSignLavaridge::
	msgbox HxRoute112_Text_RouteSignLavaridge, MSGBOX_SIGN
	end

