HxUnionRoad3_MapScripts::
	.byte 0

HxUnionRoad3_EventScript_Sign::
	msgbox HxUnionRoad3_Text_Sign, MSGBOX_SIGN
	end

HxUnionRoad3_Text_Sign::
	.string "UNION ROAD CHECKPOINT\n"
	.string "Travelers to SLATEPORT CITY,\l"
	.string "please use the gate house.$"

HxRoute118_EventScript_Chester::
	trainerbattle_single TRAINER_HX_CHESTER, HxRoute118_Text_ChesterIntro, HxRoute118_Text_ChesterDefeat
	msgbox HxRoute118_Text_ChesterPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute118_EventScript_Dalton::
	trainerbattle_single TRAINER_HX_DALTON_1, HxRoute118_Text_DaltonIntro, HxRoute118_Text_DaltonDefeat
	msgbox HxRoute118_Text_DaltonPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_Winston::
	trainerbattle_single TRAINER_HX_WINSTON_1, HxRoute104_Text_WinstonIntro, HxRoute104_Text_WinstonDefeat
	msgbox HxRoute104_Text_WinstonPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute104_EventScript_Billy::
	trainerbattle_single TRAINER_HX_BILLY, HxRoute104_Text_BillyIntro, HxRoute104_Text_BillyDefeat
	msgbox HxRoute104_Text_BillyPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute106_EventScript_Ned::
	trainerbattle_single TRAINER_HX_NED, HxRoute106_Text_NedIntro, HxRoute106_Text_NedDefeated
	msgbox HxRoute106_Text_NedPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute118_EventScript_ItemHyperPotion::
	finditem ITEM_HYPER_POTION
	end

HxRoute105_EventScript_ItemIron::
	finditem ITEM_IRON
	end

