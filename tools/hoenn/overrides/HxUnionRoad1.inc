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
