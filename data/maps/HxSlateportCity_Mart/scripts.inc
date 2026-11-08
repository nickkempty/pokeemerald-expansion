HxSlateportCity_Mart_MapScripts::
	.byte 0

HxSlateportCity_Mart_EventScript_Youngster::
	msgbox HxSlateportCity_Mart_Text_BoughtWeirdFishFromShadyGuy, MSGBOX_NPC
	end

HxSlateportCity_Mart_EventScript_Boy::
	msgbox HxSlateportCity_Mart_Text_GoodThingsIfRaiseMonsDiligently, MSGBOX_NPC
	end

HxSlateportCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	pokemart HxSlateportCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
HxSlateportCity_Mart_Items::
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_EXP_SHARE
	.2byte ITEM_GENDER_SWAP
	.2byte ITEM_NONE
	release
	end
