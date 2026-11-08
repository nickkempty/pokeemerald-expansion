HxPetalburgCity_MapScripts::
	.byte 0

HxPetalburgCity_EventScript_Boy::
	msgbox HxPetalburgCity_Text_WaterReflection, MSGBOX_NPC
	end

HxPetalburgCity_EventScript_Gentleman::
	msgbox HxPetalburgCity_Text_FullPartyExplanation, MSGBOX_NPC
	end

HxPetalburgCity_EventScript_ItemMaxRevive::
	finditem ITEM_MAX_REVIVE
	end

HxPetalburgCity_EventScript_ItemEther::
	finditem ITEM_ETHER
	end

HxPetalburgCity_EventScript_GymBoy::
	msgbox HxPetalburgCity_Text_AreYouRookieTrainer, MSGBOX_NPC
	end

HxPetalburgCity_EventScript_GymSign::
	msgbox HxPetalburgCity_Text_GymSign, MSGBOX_SIGN
	end

HxPetalburgCity_EventScript_CitySign::
	msgbox HxPetalburgCity_Text_CitySign, MSGBOX_SIGN
	end

HxPetalburgCity_EventScript_WallyHouseSign::
	msgbox HxPetalburgCity_Text_WallyHouseSign, MSGBOX_SIGN
	end

