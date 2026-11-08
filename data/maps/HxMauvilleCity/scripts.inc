HxMauvilleCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, HxMauvilleCity_OnTransition
	.byte 0

HxMauvilleCity_OnTransition::
	setflag FLAG_HX_VISITED_MAUVILLE_CITY
	clearflag FLAG_HX_MAUVILLE_GYM_BARRIERS_STATE
	setvar VAR_HX_MAUVILLE_GYM_STATE, 0
	end

HxMauvilleCity_EventScript_Boy::
	msgbox HxMauvilleCity_Text_NurseHurtMonBackToHealth, MSGBOX_NPC
	end

HxMauvilleCity_EventScript_RichBoy::
	msgbox HxMauvilleCity_Text_PokemonCanJumpYouOnBike, MSGBOX_NPC
	end

HxMauvilleCity_EventScript_Maniac::
	msgbox HxMauvilleCity_Text_AllSortsOfPeopleComeThrough, MSGBOX_NPC
	end

HxMauvilleCity_EventScript_Woman::
	msgbox HxMauvilleCity_Text_RydelVeryGenerous, MSGBOX_NPC
	end

HxMauvilleCity_EventScript_SchoolKidM::
	lock
	faceplayer
	goto_if_set FLAG_HX_TV_EXPLAINED, HxMauvilleCity_EventScript_TVExplained
	msgbox HxMauvilleCity_Text_ExplainTV, MSGBOX_DEFAULT
	setflag FLAG_HX_TV_EXPLAINED
	release
	end

HxMauvilleCity_EventScript_TVExplained::
	msgbox HxMauvilleCity_Text_BeenCheckingOutTV, MSGBOX_DEFAULT
	release
	end

HxMauvilleCity_EventScript_ItemXSpeed::
	finditem ITEM_X_SPEED
	end

HxMauvilleCity_EventScript_GymSign::
	msgbox HxMauvilleCity_Text_GymSign, MSGBOX_SIGN
	end

HxMauvilleCity_EventScript_CitySign::
	msgbox HxMauvilleCity_Text_CitySign, MSGBOX_SIGN
	end

HxMauvilleCity_EventScript_BikeShopSign::
	msgbox HxMauvilleCity_Text_BikeShopSign, MSGBOX_SIGN
	end

HxMauvilleCity_EventScript_GameCornerSign::
	msgbox HxMauvilleCity_Text_GameCornerSign, MSGBOX_SIGN
	end

