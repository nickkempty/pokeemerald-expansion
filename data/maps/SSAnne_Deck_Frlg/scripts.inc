SSAnne_Deck_Frlg_MapScripts::
	.byte 0

SSAnne_Deck_EventScript_Youngster::
	msgbox SSAnne_Deck_Text_ShipDepartingSoon, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_BaldingMan::
	msgbox SSAnne_Deck_Text_FeelSeasick, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_Sailor::
	msgbox SSAnne_Deck_Text_ScrubbingDecksHardWork, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_Edmond::
	trainerbattle_single TRAINER_SAILOR_EDMOND, SSAnne_Deck_Text_EdmondIntro, SSAnne_Deck_Text_EdmondDefeat
	msgbox SSAnne_Deck_Text_EdmondPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_Deck_EventScript_Trevor::
	trainerbattle_single TRAINER_SAILOR_TREVOR, SSAnne_Deck_Text_TrevorIntro, SSAnne_Deck_Text_TrevorDefeat
	msgbox SSAnne_Deck_Text_TrevorPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_Deck_EventScript_Lia::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM01, SSAnne_Deck_EventScript_LiaAfter
	msgbox SSAnne_Deck_Text_LiaIntro
	release
	end

SSAnne_Deck_EventScript_LiaAfter::
	msgbox SSAnne_Deck_Text_LiaAfter
	release
	end

SSAnne_Deck_EventScript_ZkTabitha::
SSAnne_Deck_EventScript_ZkMaxie::
SSAnne_Deck_EventScript_ZkTrigger::
	lockall
	goto_if_set FLAG_ZK_SSANNE_ORB_GONE, SSAnne_Deck_EventScript_ZkEnd
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_ZK_SSANNE_TABITHA, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox SSAnne_Deck_Text_ZkTabithaIntro
	closemessage
	trainerbattle_no_intro TRAINER_ZK_SSANNE_TABITHA, SSAnne_Deck_Text_ZkTabithaDefeat
	applymovement LOCALID_ZK_SSANNE_MAXIE, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SSAnne_Deck_Text_ZkMaxieSpeech
	closemessage
	fadescreen FADE_TO_BLACK
	playse SE_FLEE
	removeobject LOCALID_ZK_SSANNE_TABITHA
	removeobject LOCALID_ZK_SSANNE_MAXIE
	setflag FLAG_ZK_SSANNE_ORB_GONE
	waitse
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox SSAnne_Deck_Text_ZkAfterMaxie
	setvar VAR_ZK_SSANNE_DECK, 1
SSAnne_Deck_EventScript_ZkEnd::
	releaseall
	end

SSAnne_Deck_Text_ShipDepartingSoon::
	.string "The party's over.\n"
	.string "The ship will be departing soon.$"

SSAnne_Deck_Text_ScrubbingDecksHardWork::
	.string "Whew!\n"
	.string "Scrubbing decks is hard work!$"

SSAnne_Deck_Text_FeelSeasick::
	.string "Urf… I feel ill…\p"
	.string "I got seasick, so I stepped out to\n"
	.string "get some air…$"

SSAnne_Deck_Text_EdmondIntro::
	.string "Hey, matey!\p"
	.string "Let's do a little jig!$"

SSAnne_Deck_Text_EdmondDefeat::
	.string "You're impressive!$"

SSAnne_Deck_Text_EdmondPostBattle::
	.string "How many kinds of POKéMON do you\n"
	.string "think there are in this big world?$"

SSAnne_Deck_Text_TrevorIntro::
	.string "Ahoy, there!\n"
	.string "Are you seasick?$"

SSAnne_Deck_Text_TrevorDefeat::
	.string "I was just careless!$"

SSAnne_Deck_Text_TrevorPostBattle::
	.string "My pa said there are 100 kinds of\n"
	.string "POKéMON. I think there are more.$"


SSAnne_Deck_Text_LiaIntro::
    .string "Hi! I'm LIA, a sea researcher.\n"
    .string "PROF. BIRCH sent me from HOENN.\p"
    .string "Since the big storm, the currents\n"
    .string "around KANTO keep changing.\l"
    .string "They're carrying POKéMON from\n"
    .string "JOHTO and HOENN here.\p"
    .string "I think something powerful is\n"
    .string "stirring deep under the sea. And\l"
    .string "TEAM ECLIPSE seems to know about\n"
    .string "it, too.\p"
    .string "Your friend {RIVAL} already went\n"
    .string "after the grunts on board. Be\l"
    .string "careful!$"

SSAnne_Deck_Text_LiaAfter::
    .string "The currents are calm for now, but\n"
    .string "they won't stay that way.\p"
    .string "If you find out what TEAM ECLIPSE\n"
    .string "is planning, please tell me. I'll be\l"
    .string "watching the sea!$"

SSAnne_Deck_Text_ZkTabithaIntro::
    .string "TABITHA: Hehehe! You again?\n"
    .string "Third time's the charm, kid!\p"
    .string "Boss, go ahead. I'll keep this\n"
    .string "pest busy!$"

SSAnne_Deck_Text_ZkTabithaDefeat::
    .string "Hehe… Doesn't matter.\n"
    .string "The boss already has it.$"

SSAnne_Deck_Text_ZkMaxieSpeech::
    .string "MAXIE: So you are the child who\n"
    .string "keeps troubling my team.\p"
    .string "Behold. The RED ORB, taken from\n"
    .string "the S.S. ANNE's safe.\p"
    .string "NOX thinks M-CORE will make\n"
    .string "GROUDON obey ECLIPSE…\p"
    .string "But it is TEAM MAGMA that will\n"
    .string "expand the land!\p"
    .string "We meet again at CINNABAR, child.\n"
    .string "Come, TABITHA!$"

SSAnne_Deck_Text_ZkAfterMaxie::
    .string "MAXIE and TABITHA escaped on a\n"
    .string "speedboat with the RED ORB!\p"
    .string "…Better tell the CAPTAIN.$"
