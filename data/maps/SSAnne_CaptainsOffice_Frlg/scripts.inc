SSAnne_CaptainsOffice_Frlg_MapScripts::
	.byte 0

SSAnne_CaptainsOffice_EventScript_Captain::
	lock
	goto_if_set FLAG_ZK_SSANNE_DONE, SSAnne_CaptainsOffice_EventScript_AlreadyGotCut
	msgbox SSAnne_CaptainsOffice_Text_CaptainIFeelSeasick
	goto_if_unset FLAG_ZK_SSANNE_ORB_GONE, SSAnne_CaptainsOffice_EventScript_ZkOrbStolen
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message SSAnne_CaptainsOffice_Text_RubbedCaptainsBack
	waitmessage
	playfanfare MUS_HEAL
	waitfanfare
	call EventScript_RestorePrevTextColor
	delay 50
	applymovement LOCALID_SS_ANNE_CAPTAIN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SSAnne_CaptainsOffice_Text_ThankYouHaveHMForCut
	giveitem_msg SSAnne_CaptainsOffice_Text_ObtainedHM01FromCaptain, ITEM_POKE_FLUTE, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox SSAnne_CaptainsOffice_Text_ExplainCut
	setflag FLAG_GOT_POKE_FLUTE
	setflag FLAG_ZK_SSANNE_DONE
	setvar VAR_MAP_SCENE_VERMILION_CITY, 1
	release
	end

@ Unused
SSAnne_CaptainsOffice_EventScript_NoRoomForCut::
	msgbox SSAnne_CaptainsOffice_Text_YouHaveNoRoomForThis
	closemessage
	applymovement LOCALID_SS_ANNE_CAPTAIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SSAnne_CaptainsOffice_EventScript_AlreadyGotCut::
	applymovement LOCALID_SS_ANNE_CAPTAIN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SSAnne_CaptainsOffice_Text_SSAnneWillSetSailSoon
	release
	end

SSAnne_CaptainsOffice_EventScript_TrashCan::
	msgbox SSAnne_CaptainsOffice_Text_YuckShouldntHaveLooked, MSGBOX_SIGN
	end

SSAnne_CaptainsOffice_EventScript_Book::
	msgbox SSAnne_CaptainsOffice_Text_HowToConquerSeasickness, MSGBOX_SIGN
	end

SSAnne_CaptainsOffice_EventScript_ZkOrbStolen::
	msgbox SSAnne_CaptainsOffice_Text_ZkOrbStolen
	release
	end

SSAnne_CaptainsOffice_Text_CaptainIFeelSeasick::
	.string "CAPTAIN: Ooargh…\n"
	.string "I feel hideous…\l"
	.string "Urrp! Seasick…$"

SSAnne_CaptainsOffice_Text_RubbedCaptainsBack::
	.string "{PLAYER} rubbed the CAPTAIN's\n"
	.string "back!\p"
	.string "Rub-rub…\n"
	.string "Rub-rub…$"

SSAnne_CaptainsOffice_Text_ThankYouHaveHMForCut::
    .string "CAPTAIN: Whew! Thank you!\n"
    .string "I'm feeling much better now.\p"
    .string "So MAGMA got away with the RED\n"
    .string "ORB… It was a gift for the\l"
    .string "CINNABAR LAB. How shameful!\p"
    .string "Please, take this as thanks.\n"
    .string "It was to be played at our party.$"

SSAnne_CaptainsOffice_Text_ObtainedHM01FromCaptain::
    .string "{PLAYER} obtained a POKé FLUTE\n"
    .string "from the CAPTAIN!$"

SSAnne_CaptainsOffice_Text_ExplainCut::
    .string "Its tune wakes any sleeping\n"
    .string "POKéMON.\p"
    .string "I hear a huge sleeping POKéMON is\n"
    .string "blocking the road near LAVENDER.\p"
    .string "And LT. SURGE was guarding this\n"
    .string "ship. Tell him what happened!$"

SSAnne_CaptainsOffice_Text_SSAnneWillSetSailSoon::
	.string "CAPTAIN: …Whew!\p"
	.string "Now that I'm not sick anymore,\n"
	.string "I guess it's time.\p"
	.string "The S.S. ANNE will set sail soon!\p"
	.string "Farewell, until our return to\n"
	.string "VERMILION CITY!$"

SSAnne_CaptainsOffice_Text_YouHaveNoRoomForThis::
	.string "Oh, no!\n"
	.string "You have no room for this!$"

SSAnne_CaptainsOffice_Text_YuckShouldntHaveLooked::
	.string "Yuck!\n"
	.string "Shouldn't have looked!$"

SSAnne_CaptainsOffice_Text_HowToConquerSeasickness::
	.string "How to Conquer Seasickness…\n"
	.string "The CAPTAIN's reading this!$"


SSAnne_CaptainsOffice_Text_ZkOrbStolen::
    .string "Urrp… Thieves… they opened my\n"
    .string "safe and took the RED ORB…\p"
    .string "They ran… up to the deck…\n"
    .string "Oooargh…$"
