HxMossdeepCity_MapScripts::
	.byte 0

HxMossdeepCity_EventScript_Sailor::
	lock
	faceplayer
	goto_if_set FLAG_HX_RECEIVED_HM_DIVE, HxMossdeepCity_EventScript_SailorMagmaGone
	msgbox HxMossdeepCity_Text_MossdeepTargetedByMagma, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_EventScript_SailorMagmaGone::
	msgbox HxMossdeepCity_Text_FeelReliefOnLand, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_EventScript_ExpertM::
	msgbox HxMossdeepCity_Text_LifeNeedsSeaToLive, MSGBOX_NPC
	end

HxMossdeepCity_EventScript_PokefanF::
	lock
	faceplayer
	goto_if_set FLAG_HX_RECEIVED_HM_DIVE, HxMossdeepCity_EventScript_PokefanFMagmaGone
	msgbox HxMossdeepCity_Text_SpaceCenterReceivedLetter, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_EventScript_PokefanFMagmaGone::
	msgbox HxMossdeepCity_Text_SpaceCenterLaunchingRockets, MSGBOX_DEFAULT
	release
	end

HxMossdeepCity_EventScript_NinjaBoy::
	msgbox HxMossdeepCity_Text_WailmerWatching, MSGBOX_NPC
	end

HxMossdeepCity_EventScript_Girl::
	msgbox HxMossdeepCity_Text_NiceIfWorldCoveredByFlowers, MSGBOX_NPC
	end

HxMossdeepCity_EventScript_ItemNetBall::
	finditem ITEM_NET_BALL
	end

HxMossdeepCity_EventScript_Man::
	msgbox HxMossdeepCity_Text_SurfExhilarating, MSGBOX_NPC
	end

HxMossdeepCity_EventScript_Woman::
	msgbox HxMossdeepCity_Text_SpecialSpaceCenterRock, MSGBOX_NPC
	end

HxMossdeepCity_EventScript_KingsRockBoy::
	msgbox HxMossdeepCity_Text_StevensHouseOverThere, MSGBOX_NPC
	end

HxMossdeepCity_EventScript_BlackBelt::
	msgbox HxMossdeepCity_Text_SootopolisNewGymLeader, MSGBOX_NPC
	end

HxMossdeepCity_EventScript_CitySign::
	msgbox HxMossdeepCity_Text_CitySign, MSGBOX_SIGN
	end

HxMossdeepCity_EventScript_GymSign::
	msgbox HxMossdeepCity_Text_GymSign, MSGBOX_SIGN
	end

HxMossdeepCity_EventScript_SpaceCenterSign::
	msgbox HxMossdeepCity_Text_SpaceCenterSign, MSGBOX_SIGN
	end

HxMossdeepCity_EventScript_WhiteRock::
	msgbox HxMossdeepCity_Text_ItsAWhiteRock, MSGBOX_SIGN
	end

