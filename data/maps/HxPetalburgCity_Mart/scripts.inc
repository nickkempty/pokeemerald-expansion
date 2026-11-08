HxPetalburgCity_Mart_MapScripts::
	.byte 0

HxPetalburgCity_Mart_EventScript_Youngster::
	msgbox HxPetalburgCity_Mart_Text_BoughtWeirdFishFromShadyGuy, MSGBOX_NPC
	end

HxPetalburgCity_Mart_EventScript_Boy::
	msgbox HxPetalburgCity_Mart_Text_GoodThingsIfRaiseMonsDiligently, MSGBOX_NPC
	end

HxPetalburgCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	pokemart HxPetalburgCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
HxPetalburgCity_Mart_Items::
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_EXP_SHARE
	.2byte ITEM_GENDER_SWAP
	.2byte ITEM_MAX_POTION
	.2byte ITEM_NONE
	release
	end
