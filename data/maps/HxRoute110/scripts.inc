HxRoute110_MapScripts::
	.byte 0

HxRoute110_EventScript_Boy2::
	msgbox HxRoute110_Text_WhichShouldIChoose, MSGBOX_NPC
	end

HxRoute110_EventScript_CyclingGuy2::
	msgbox HxRoute110_Text_BikeTechniques, MSGBOX_NPC
	end

HxRoute110_EventScript_OldWoman::
	msgbox HxRoute110_Text_WalkOnTheLowRoad, MSGBOX_NPC
	end

HxRoute110_EventScript_CyclingGuy1::
	msgbox HxRoute110_Text_YouGotBikeFromRydel, MSGBOX_NPC
	end

HxRoute110_EventScript_OldMan::
	msgbox HxRoute110_Text_TwoRoads, MSGBOX_NPC
	end

HxRoute110_EventScript_CyclingGirl1::
	msgbox HxRoute110_Text_HairStreamsBehindMe, MSGBOX_NPC
	end

HxRoute110_EventScript_Boy1::
	msgbox HxRoute110_Text_RideBikeAtFullSpeed, MSGBOX_NPC
	end

HxRoute110_EventScript_Jasmine::
	trainerbattle_single TRAINER_HX_JASMINE, HxRoute110_Text_JasmineIntro, HxRoute110_Text_JasmineDefeated
	msgbox HxRoute110_Text_JasminePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Anthony::
	trainerbattle_single TRAINER_HX_ANTHONY, HxRoute110_Text_AnthonyIntro, HxRoute110_Text_AnthonyDefeated
	msgbox HxRoute110_Text_AnthonyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Abigail::
	trainerbattle_single TRAINER_HX_ABIGAIL_1, HxRoute110_Text_AbigailIntro, HxRoute110_Text_AbigailDefeated
	msgbox HxRoute110_Text_AbigailPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Benjamin::
	trainerbattle_single TRAINER_HX_BENJAMIN_1, HxRoute110_Text_BenjaminIntro, HxRoute110_Text_BenjaminDefeated
	msgbox HxRoute110_Text_BenjaminPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Edward::
	trainerbattle_single TRAINER_HX_EDWARD, HxRoute110_Text_EdwardIntro, HxRoute110_Text_EdwardDefeated
	msgbox HxRoute110_Text_EdwardPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Jaclyn::
	trainerbattle_single TRAINER_HX_JACLYN, HxRoute110_Text_JaclynIntro, HxRoute110_Text_JaclynDefeated
	msgbox HxRoute110_Text_JaclynPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Edwin::
	trainerbattle_single TRAINER_HX_EDWIN_1, HxRoute110_Text_EdwinIntro, HxRoute110_Text_EdwinDefeated
	msgbox HxRoute110_Text_EdwinPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Dale::
	trainerbattle_single TRAINER_HX_DALE, HxRoute110_Text_DaleIntro, HxRoute110_Text_DaleDefeated
	msgbox HxRoute110_Text_DalePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_ItemDireHit::
	finditem ITEM_DIRE_HIT
	end

HxRoute110_EventScript_ItemRareCandy::
	finditem ITEM_RARE_CANDY
	end

HxRoute110_EventScript_ChallengeGuy::
	msgbox HxRoute110_Text_AcroBikesDoNotQualify, MSGBOX_NPC
	end

HxRoute110_EventScript_Jacob::
	trainerbattle_single TRAINER_HX_JACOB, HxRoute110_Text_JacobIntro, HxRoute110_Text_JacobDefeated
	msgbox HxRoute110_Text_JacobPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Timmy::
	trainerbattle_single TRAINER_HX_TIMMY, HxRoute110_Text_TimmyIntro, HxRoute110_Text_TimmyDefeated
	msgbox HxRoute110_Text_TimmyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Isabel::
	trainerbattle_single TRAINER_HX_ISABEL_1, HxRoute110_Text_IsabelIntro, HxRoute110_Text_IsabelDefeated
	msgbox HxRoute110_Text_IsabelPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Kaleb::
	trainerbattle_single TRAINER_HX_KALEB, HxRoute110_Text_KalebIntro, HxRoute110_Text_KalebDefeated
	msgbox HxRoute110_Text_KalebPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Alyssa::
	trainerbattle_single TRAINER_HX_ALYSSA, HxRoute110_Text_AlyssaIntro, HxRoute110_Text_AlyssaDefeated
	msgbox HxRoute110_Text_AlyssaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_Joseph::
	trainerbattle_single TRAINER_HX_JOSEPH, HxRoute110_Text_JosephIntro, HxRoute110_Text_JosephDefeated
	msgbox HxRoute110_Text_JosephPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute110_EventScript_ItemElixir::
	finditem ITEM_ELIXIR
	end

HxRoute110_EventScript_VandalizedSign::
	msgbox HxRoute110_Text_AquaWasHere, MSGBOX_SIGN
	end

HxRoute110_EventScript_SeasideParkingSign::
	msgbox HxRoute110_Text_SeasideParkingSign, MSGBOX_SIGN
	end

HxRoute110_EventScript_CyclingRoadSign::
	msgbox HxRoute110_Text_CyclingRoadSign, MSGBOX_SIGN
	end

HxRoute110_EventScript_SlateportCitySign::
	msgbox HxRoute110_Text_SlateportCitySign, MSGBOX_SIGN
	end

HxRoute110_EventScript_Route103Sign::
	msgbox HxRoute110_Text_Route103Sign, MSGBOX_SIGN
	end

HxRoute110_EventScript_MauvilleCitySign::
	msgbox HxRoute110_Text_MauvilleCitySign, MSGBOX_SIGN
	end

HxRoute110_EventScript_TrainerTipsPrlzSleep::
	msgbox HxRoute110_Text_TrainerTipsPrlzSleep, MSGBOX_SIGN
	end

HxRoute110_EventScript_TrainerTipsRegisterItems::
	msgbox HxRoute110_Text_TrainerTipsRegisterItems, MSGBOX_SIGN
	end

HxRoute110_EventScript_TrickHouseSign::
	msgbox HxRoute110_Text_TrickHouseSign, MSGBOX_SIGN
	end

HxRoute110_EventScript_CyclingRoadResultsSign::
	msgbox HxRoute110_Text_ThereIsNoRecord, MSGBOX_NPC
	end

