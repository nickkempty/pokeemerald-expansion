SSAnne_Deck_MapScripts::
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
