HxUnionRoad4_MapScripts::
	.byte 0

HxUnionRoad4_EventScript_Sign::
	msgbox HxUnionRoad4_Text_Sign, MSGBOX_SIGN
	end

HxUnionRoad4_Text_Sign::
	.string "SLATEPORT CITY - straight ahead\n"
	.string "Welcome to HOENN!$"

HxRoute109_EventScript_Huey::
	trainerbattle_single TRAINER_HX_HUEY, HxRoute109_Text_HueyIntro, HxRoute109_Text_HueyDefeated
	msgbox HxRoute109_Text_HueyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Edmond::
	trainerbattle_single TRAINER_HX_EDMOND, HxRoute109_Text_EdmondIntro, HxRoute109_Text_EdmondDefeated
	msgbox HxRoute109_Text_EdmondPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Ricky::
	trainerbattle_single TRAINER_HX_RICKY_1, HxRoute109_Text_RickyIntro, HxRoute109_Text_RickyDefeated
	msgbox HxRoute109_Text_RickyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Lola::
	trainerbattle_single TRAINER_HX_LOLA_1, HxRoute109_Text_LolaIntro, HxRoute109_Text_LolaDefeated
	msgbox HxRoute109_Text_LolaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Hailey::
	trainerbattle_single TRAINER_HX_HAILEY, HxRoute109_Text_HaileyIntro, HxRoute109_Text_HaileyDefeated
	msgbox HxRoute109_Text_HaileyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Chandler::
	trainerbattle_single TRAINER_HX_CHANDLER, HxRoute109_Text_ChandlerIntro, HxRoute109_Text_ChandlerDefeated
	msgbox HxRoute109_Text_ChandlerPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_SeashoreHouseGirl::
	msgbox HxRoute109_Text_ChillAtMyPapasSpot, MSGBOX_NPC
	end

HxRoute109_EventScript_OldMan::
	msgbox HxRoute109_Text_ZigzagoonPicksUpLitter, MSGBOX_NPC
	end

HxRoute109_EventScript_Zigzagoon::
	msgbox HxRoute109_Text_ZigzagoonCry, MSGBOX_NPC
	end

HxRoute109_EventScript_SandCastleBoy::
	msgbox HxRoute109_Text_SandCastleTakingLongTime, MSGBOX_NPC
	end

HxRoute109_EventScript_SoftSandGirl::
	msgbox HxRoute109_Text_WereGoingToMakeBigCastle, MSGBOX_NPC
	end

HxRoute109_EventScript_ItemPotion::
	finditem ITEM_POTION
	end

HxRoute109_EventScript_Gwen::
	trainerbattle_single TRAINER_HX_GWEN, HxRoute109_Text_GwenIntro, HxRoute109_Text_GwenDefeated
	msgbox HxRoute109_Text_GwenPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Austina::
	trainerbattle_single TRAINER_HX_AUSTINA, HxRoute109_Text_AustinaIntro, HxRoute109_Text_AustinaDefeated
	msgbox HxRoute109_Text_AustinaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Carter::
	trainerbattle_single TRAINER_HX_CARTER, HxRoute109_Text_CarterIntro, HxRoute109_Text_CarterDefeated
	msgbox HxRoute109_Text_CarterPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_Elijah::
	trainerbattle_single TRAINER_HX_ELIJAH, HxRoute109_Text_ElijahIntro, HxRoute109_Text_ElijahDefeated
	msgbox HxRoute109_Text_ElijahPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute109_EventScript_ItemPPUp::
	finditem ITEM_PP_UP
	end

HxRoute109_EventScript_SeashoreHouseSign::
	msgbox HxRoute109_Text_SeashoreHouseSign, MSGBOX_SIGN
	end

HxRoute109_EventScript_TrainerTipsSign::
	msgbox HxRoute109_Text_TrainerTipsSign, MSGBOX_SIGN
	end

