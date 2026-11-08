HxRoute104_MapScripts::
	.byte 0

HxRoute104_EventScript_BugCatcher::
	msgbox HxRoute104_Text_WhatsItLikeAtBottomOfSea, MSGBOX_SIGN
	end

HxRoute104_EventScript_Girl1::
	msgbox HxRoute104_Text_BrineyLivesInSeasideCottage, MSGBOX_NPC
	end

HxRoute104_EventScript_Haley::
	trainerbattle_single TRAINER_HX_HALEY_1, HxRoute104_Text_HaleyIntro, HxRoute104_Text_HaleyDefeat
	msgbox HxRoute104_Text_HaleyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_Boy1::
	msgbox HxRoute104_Text_ThrowBallAtWeakenedPokemon, MSGBOX_NPC
	end

HxRoute104_EventScript_Woman::
	msgbox HxRoute104_Text_OnlyThrowBallAtWildPokemon, MSGBOX_NPC
	end

HxRoute104_EventScript_Girl2::
	msgbox HxRoute104_Text_ImNotATrainer, MSGBOX_NPC
	end

HxRoute104_EventScript_Ivan::
	trainerbattle_single TRAINER_HX_IVAN, HxRoute104_Text_IvanIntro, HxRoute104_Text_IvanDefeat
	msgbox HxRoute104_Text_IvanPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_ItemPPUp::
	finditem ITEM_PP_UP
	end

HxRoute104_EventScript_Gina::
	trainerbattle_double TRAINER_HX_GINA_AND_MIA_1, HxRoute104_Text_GinaIntro, HxRoute104_Text_GinaDefeat, HxRoute104_Text_GinaNotEnoughMons
	msgbox HxRoute104_Text_GinaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_Mia::
	trainerbattle_double TRAINER_HX_GINA_AND_MIA_1, HxRoute104_Text_MiaIntro, HxRoute104_Text_MiaDefeat, HxRoute104_Text_MiaNotEnoughMons
	msgbox HxRoute104_Text_MiaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_Cindy::
	trainerbattle_single TRAINER_HX_CINDY_1, HxRoute104_Text_CindyIntro, HxRoute104_Text_CindyDefeat
	msgbox HxRoute104_Text_CindyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_ItemPokeBall::
	finditem ITEM_POKE_BALL
	end

HxRoute104_EventScript_ItemXAccuracy::
	finditem ITEM_X_ACCURACY
	end

HxRoute104_EventScript_ItemPotion::
	finditem ITEM_POTION
	end

HxRoute104_EventScript_Boy2::
	msgbox HxRoute104_Text_TMsAreOneTimeUse, MSGBOX_NPC
	end

HxRoute104_EventScript_Darian::
	trainerbattle_single TRAINER_HX_DARIAN, HxRoute104_Text_DarianIntro, HxRoute104_Text_DarianDefeat
	msgbox HxRoute104_Text_DarianPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_BrineysCottageSign::
	msgbox HxRoute104_Text_MrBrineysCottage, MSGBOX_SIGN
	end

HxRoute104_EventScript_RouteSignPetalburg::
	msgbox HxRoute104_Text_RouteSignPetalburg, MSGBOX_SIGN
	end

HxRoute104_EventScript_RouteSignRustboro::
	msgbox HxRoute104_Text_RouteSignRustboro, MSGBOX_SIGN
	end

HxRoute104_EventScript_FlowerShopSign::
	msgbox HxRoute104_Text_PrettyPetalFlowShop, MSGBOX_SIGN
	end

HxRoute104_EventScript_TrainerTipsDoubleBattles::
	msgbox HxRoute104_Text_TrainerTipsDoubleBattles, MSGBOX_SIGN
	end

