HxRoute126_MapScripts::
	.byte 0

HxRoute126_EventScript_UnderwaterCave::
	lockall
	msgbox HxRoute126_Text_UnderwaterCave, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HxRoute126_EventScript_UnderwaterCaveNo
	closemessage
	playse SE_M_DIVE
	warpteleport MAP_HX_SOOTOPOLIS_CITY, 43, 33
	waitstate
	releaseall
	end

HxRoute126_EventScript_UnderwaterCaveNo::
	releaseall
	end

HxRoute126_Text_UnderwaterCave::
	.string "There's a dark underwater cave under\n"
	.string "the crater rim.\p"
	.string "It leads inside to SOOTOPOLIS CITY.\n"
	.string "Swim through it?$"

HxRoute126_EventScript_Barry::
	trainerbattle_single TRAINER_HX_BARRY, HxRoute126_Text_BarryIntro, HxRoute126_Text_BarryDefeat
	msgbox HxRoute126_Text_BarryPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute126_EventScript_Dean::
	trainerbattle_single TRAINER_HX_DEAN, HxRoute126_Text_DeanIntro, HxRoute126_Text_DeanDefeat
	msgbox HxRoute126_Text_DeanPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute126_EventScript_Nikki::
	trainerbattle_single TRAINER_HX_NIKKI, HxRoute126_Text_NikkiIntro, HxRoute126_Text_NikkiDefeat
	msgbox HxRoute126_Text_NikkiPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute126_EventScript_Brenda::
	trainerbattle_single TRAINER_HX_BRENDA, HxRoute126_Text_BrendaIntro, HxRoute126_Text_BrendaDefeat
	msgbox HxRoute126_Text_BrendaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute126_EventScript_ItemGreenShard::
	finditem ITEM_GREEN_SHARD
	end

HxRoute126_EventScript_Sienna::
	trainerbattle_single TRAINER_HX_SIENNA, HxRoute126_Text_SiennaIntro, HxRoute126_Text_SiennaDefeat
	msgbox HxRoute126_Text_SiennaPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute126_EventScript_Pablo::
	trainerbattle_single TRAINER_HX_PABLO_1, HxRoute126_Text_PabloIntro, HxRoute126_Text_PabloDefeat
	msgbox HxRoute126_Text_PabloPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute126_EventScript_Isobel::
	trainerbattle_single TRAINER_HX_ISOBEL, HxRoute126_Text_IsobelIntro, HxRoute126_Text_IsobelDefeat
	msgbox HxRoute126_Text_IsobelPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute126_EventScript_Leonardo::
	trainerbattle_single TRAINER_HX_LEONARDO, HxRoute126_Text_LeonardoIntro, HxRoute126_Text_LeonardoDefeat
	msgbox HxRoute126_Text_LeonardoPostBattle, MSGBOX_AUTOCLOSE
	end

