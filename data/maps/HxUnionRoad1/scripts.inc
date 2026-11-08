HxUnionRoad1_MapScripts::
	.byte 0

HxUnionRoad1_EventScript_Sign::
	msgbox HxUnionRoad1_Text_Sign, MSGBOX_SIGN
	end

HxUnionRoad1_Text_Sign::
	.string "UNION ROAD\n"
	.string "CINNABAR ISLAND - SLATEPORT CITY\p"
	.string "The sea floor rose overnight.\n"
	.string "Stay on the sand, and keep\l"
	.string "away from the deep water!$"

HxUnionRoad1_EventScript_ToCinnabar::
	warp MAP_CINNABAR_ISLAND, 13, 18
	waitstate
	end

HxRoute108_EventScript_Cory::
	trainerbattle_single TRAINER_HX_CORY_1, HxRoute108_Text_CoryIntro, HxRoute108_Text_CoryDefeated
	msgbox HxRoute108_Text_CoryPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute118_EventScript_Wade::
	trainerbattle_single TRAINER_HX_WADE, HxRoute118_Text_WadeIntro, HxRoute118_Text_WadeDefeat
	msgbox HxRoute118_Text_WadePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute106_EventScript_Elliot::
	trainerbattle_single TRAINER_HX_ELLIOT_1, HxRoute106_Text_ElliotIntro, HxRoute106_Text_ElliotDefeated
	msgbox HxRoute106_Text_ElliotPostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute118_EventScript_Deandre::
	trainerbattle_single TRAINER_HX_DEANDRE, HxRoute118_Text_DeandreIntro, HxRoute118_Text_DeandreDefeat
	msgbox HxRoute118_Text_DeandrePostBattle, MSGBOX_AUTOCLOSE
	end

HxRoute108_EventScript_ItemStarPiece::
	finditem ITEM_STAR_PIECE
	end

HxRoute106_EventScript_ItemProtein::
	finditem ITEM_PROTEIN
	end

