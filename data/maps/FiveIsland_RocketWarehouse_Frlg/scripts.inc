.equ PLAYER_X_POS, VAR_0x8004
.equ PLAYER_Y_POS, VAR_0x8005

FiveIsland_RocketWarehouse_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, FiveIsland_RocketWarehouse_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, FiveIsland_RocketWarehouse_OnTransition
	.byte 0

FiveIsland_RocketWarehouse_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_FIVE_ISLAND_ROCKET_WAREHOUSE
	setflag FLAG_HIDE_NOX_FINAL
	setflag FLAG_HIDE_FINALE_FRIENDS
	call_if_set FLAG_DEFEATED_NOX_FINAL, FiveIsland_RocketWarehouse_EventScript_ShowFriends
	goto_if_set FLAG_DEFEATED_NOX_FINAL, FiveIsland_RocketWarehouse_EventScript_EndTransition
	call_if_set FLAG_RECOVERED_SAPPHIRE, FiveIsland_RocketWarehouse_EventScript_ShowNox
FiveIsland_RocketWarehouse_EventScript_EndTransition::
	end

FiveIsland_RocketWarehouse_EventScript_ShowNox::
	clearflag FLAG_HIDE_NOX_FINAL
	return

FiveIsland_RocketWarehouse_EventScript_ShowFriends::
	clearflag FLAG_HIDE_FINALE_FRIENDS
	return

FiveIsland_RocketWarehouse_OnLoad::
	call_if_defeated TRAINER_TEAM_ROCKET_ADMIN, FiveIsland_RocketWarehouse_EventScript_SetArrowsForReEntry
	end

FiveIsland_RocketWarehouse_EventScript_SetArrowsForReEntry::
	setmetatile 26, 20, METATILE_SilphCo_Arrow_Up, 0
	setmetatile 27, 20, METATILE_SilphCo_Arrow_Up, 0
	return

FiveIsland_RocketWarehouse_EventScript_Cage::
	lockall
	goto_if_set FLAG_DEFEATED_ROCKETS_IN_WAREHOUSE, FiveIsland_RocketWarehouse_EventScript_CageUnlocked
	msgbox FiveIsland_RocketWarehouse_Text_ManyMonsLockedInPen
	releaseall
	end

FiveIsland_RocketWarehouse_EventScript_CageUnlocked::
	msgbox FiveIsland_RocketWarehouse_Text_PenUnlockedMonsFled
	releaseall
	end

FiveIsland_RocketWarehouse_EventScript_Computer::
	msgbox FiveIsland_RocketWarehouse_Text_ReportWithGraphsAndText, MSGBOX_SIGN
	end

FiveIsland_RocketWarehouse_EventScript_Admin2Trigger::
	lockall
	applymovement LOCALID_WAREHOUSE_ADMIN2, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	setvar VAR_MAP_SCENE_ROCKET_WAREHOUSE, 1
	releaseall
	end

FiveIsland_RocketWarehouse_EventScript_Gideon::
	trainerbattle_single TRAINER_SCIENTIST_GIDEON, FiveIsland_RocketWarehouse_Text_GideonIntro, FiveIsland_RocketWarehouse_Text_GideonDefeat, FiveIsland_RocketWarehouse_EventScript_DefeatedGideon
	goto_if_set FLAG_IS_CHAMPION, FiveIsland_RocketWarehouse_EventScript_MentionGiovannisKid
	msgbox FiveIsland_RocketWarehouse_Text_GetLostLeaveMeBe, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_MentionGiovannisKid::
	famechecker FAMECHECKER_GIOVANNI, 5
	msgbox FiveIsland_RocketWarehouse_Text_GiovannisKidHasRedHair, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_DefeatedGideon::
	msgbox FiveIsland_RocketWarehouse_Text_GideonPostBattle
	setflag FLAG_RECOVERED_SAPPHIRE
	giveitem ITEM_SAPPHIRE
	clearflag FLAG_HIDE_NOX_FINAL
	addobject LOCALID_WAREHOUSE_NOX
	release
	end

FiveIsland_RocketWarehouse_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_47, FiveIsland_RocketWarehouse_Text_Grunt2Intro, FiveIsland_RocketWarehouse_Text_Grunt2Defeat
	msgbox FiveIsland_RocketWarehouse_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_48, FiveIsland_RocketWarehouse_Text_Grunt3Intro, FiveIsland_RocketWarehouse_Text_Grunt3Defeat
	msgbox FiveIsland_RocketWarehouse_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_Admin2::
	trainerbattle_single TRAINER_TEAM_ROCKET_ADMIN_2, FiveIsland_RocketWarehouse_Text_Admin2Intro, FiveIsland_RocketWarehouse_Text_Admin2Defeat, FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin2
	msgbox FiveIsland_RocketWarehouse_Text_Admin2PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin2::
	getplayerxy PLAYER_X_POS, PLAYER_Y_POS
	call_if_le PLAYER_X_POS, 24, FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin2
	msgbox FiveIsland_RocketWarehouse_Text_Admin2PostBattle
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_WAREHOUSE_GRUNT2
	removeobject LOCALID_WAREHOUSE_GRUNT3
	removeobject LOCALID_WAREHOUSE_ADMIN2
	removeobject LOCALID_WAREHOUSE_ADMIN1
	setflag FLAG_HIDE_FIVE_ISLAND_ROCKETS
	fadescreen FADE_FROM_BLACK
	setvar VAR_MAP_SCENE_ROCKET_WAREHOUSE, 1
	setflag FLAG_DEFEATED_ROCKETS_IN_WAREHOUSE
	release
	end

FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin2::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_42, FiveIsland_RocketWarehouse_Text_Grunt1Intro, FiveIsland_RocketWarehouse_Text_Grunt1Defeat
	msgbox FiveIsland_RocketWarehouse_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_Admin1::
	trainerbattle_single TRAINER_TEAM_ROCKET_ADMIN, FiveIsland_RocketWarehouse_Text_Admin1Intro, FiveIsland_RocketWarehouse_Text_Admin1Defeat, FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin1
	msgbox FiveIsland_RocketWarehouse_Text_MadeItSoYouCanComeBackThrough, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin1::
	getplayerxy PLAYER_X_POS, PLAYER_Y_POS
	specialvar VAR_RESULT, GetPlayerFacingDirection
	call_if_eq VAR_RESULT, DIR_NORTH, FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin1
	msgbox FiveIsland_RocketWarehouse_Text_Admin1PostBattle
	closemessage
	call_if_le PLAYER_X_POS, 24, FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitchFar
	call_if_eq PLAYER_X_POS, 25, FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitch
	call_if_eq PLAYER_X_POS, 26, FiveIsland_RocketWarehouse_EventScript_AdminFaceSwitch
	call_if_eq PLAYER_X_POS, 27, FiveIsland_RocketWarehouse_EventScript_AdminFaceSwitch
	playse SE_PIN
	call FiveIsland_RocketWarehouse_EventScript_SetArrowsForReEntry
	special DrawWholeMapView
	waitse
	call_if_le PLAYER_X_POS, 24, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft
	call_if_eq PLAYER_X_POS, 25, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft
	call_if_eq PLAYER_X_POS, 26, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft
	call_if_eq PLAYER_X_POS, 27, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerDown
	msgbox FiveIsland_RocketWarehouse_Text_MadeItSoYouCanComeBackThrough
	release
	end

FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin1::
	goto_if_eq PLAYER_X_POS, 27, Common_EventScript_NopReturn
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitchFar::
	applymovement LOCALID_WAREHOUSE_ADMIN1, FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitchFar
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitch::
	applymovement LOCALID_WAREHOUSE_ADMIN1, FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitch
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminFaceSwitch::
	applymovement LOCALID_WAREHOUSE_ADMIN1, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft::
	applymovement LOCALID_WAREHOUSE_ADMIN1, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerDown::
	applymovement LOCALID_WAREHOUSE_ADMIN1, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitchFar::
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitch::
	walk_right
	walk_in_place_faster_up
	step_end

FiveIsland_RocketWarehouse_EventScript_NoxFinal::
	lock
	faceplayer
	msgbox FiveIsland_RocketWarehouse_Text_NoxFinalIntro
	trainerbattle_no_intro TRAINER_BOSS_NOX_FINAL, FiveIsland_RocketWarehouse_Text_NoxFinalDefeat
	msgbox FiveIsland_RocketWarehouse_Text_NoxFinalAfter
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_DEFEATED_NOX_FINAL
	setflag FLAG_HIDE_NOX_FINAL
	removeobject LOCALID_WAREHOUSE_NOX
	clearflag FLAG_HIDE_FINALE_FRIENDS
	addobject LOCALID_WAREHOUSE_LIA
	addobject LOCALID_WAREHOUSE_ZUMO
	fadescreen FADE_FROM_BLACK
	msgbox FiveIsland_RocketWarehouse_Text_FriendsArrive
	release
	end

FiveIsland_RocketWarehouse_EventScript_LiaFinale::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_MYSTIC_TICKET, FiveIsland_RocketWarehouse_EventScript_LiaAfterTicket
	msgbox FiveIsland_RocketWarehouse_Text_LiaFinale
	giveitem ITEM_MYSTIC_TICKET
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	setflag FLAG_RECEIVED_MYSTIC_TICKET
	setflag FLAG_ENABLE_SHIP_NAVEL_ROCK
	msgbox FiveIsland_RocketWarehouse_Text_LiaTicketExplain
	release
	end

FiveIsland_RocketWarehouse_EventScript_LiaAfterTicket::
	msgbox FiveIsland_RocketWarehouse_Text_LiaTicketExplain
	release
	end

FiveIsland_RocketWarehouse_EventScript_ZumoFinale::
	lock
	faceplayer
	msgbox FiveIsland_RocketWarehouse_Text_ZumoFinale
	release
	end

FiveIsland_RocketWarehouse_Text_ManyMonsLockedInPen::
	.string "There are many POKéMON locked\n"
	.string "away in the pen…$"

FiveIsland_RocketWarehouse_Text_PenUnlockedMonsFled::
	.string "The pen door is unlocked.\n"
	.string "The POKéMON appear to have fled.$"

FiveIsland_RocketWarehouse_Text_Grunt2Intro::
    .string "Did you come here knowing it's\n"
    .string "TEAM ECLIPSE's WAREHOUSE?$"

FiveIsland_RocketWarehouse_Text_Grunt2Defeat::
	.string "What do you think you're doing?!$"

FiveIsland_RocketWarehouse_Text_Grunt2PostBattle::
    .string "TEAM ECLIPSE broke up?\n"
    .string "What planet are you from?$"

FiveIsland_RocketWarehouse_Text_Grunt1Intro::
	.string "Oh, now your POKéMON look strong.\n"
	.string "You're willing to sell them?\p"
	.string "Oh?\n"
	.string "You wanted to battle?$"

FiveIsland_RocketWarehouse_Text_Grunt1Defeat::
	.string "Oh, but…\n"
	.string "Too much!$"

FiveIsland_RocketWarehouse_Text_Grunt1PostBattle::
	.string "We can give you a great price.\n"
	.string "Sell us your POKéMON!$"

FiveIsland_RocketWarehouse_Text_Grunt3Intro::
    .string "I got word about you from the\n"
    .string "others.\p"
    .string "But there's no way a kid like you'd\n"
    .string "know our BOSS NOX!$"

FiveIsland_RocketWarehouse_Text_Grunt3Defeat::
	.string "Don't…you…dare…laugh…$"

FiveIsland_RocketWarehouse_Text_Grunt3PostBattle::
	.string "Don't mess in the doings of adults,\n"
	.string "you jumped-up pip-squeak!$"

FiveIsland_RocketWarehouse_Text_Admin1Intro::
    .string "This warehouse is TEAM ECLIPSE's\n"
    .string "last base! The boss is waiting for\l"
    .string "the final piece!$"

FiveIsland_RocketWarehouse_Text_Admin1Defeat::
	.string "I…\n"
	.string "I'm shattered…$"

FiveIsland_RocketWarehouse_Text_Admin1PostBattle::
	.string "You're doing things according to\n"
	.string "your beliefs and morals.\p"
	.string "I understand now…$"

FiveIsland_RocketWarehouse_Text_MadeItSoYouCanComeBackThrough::
	.string "I've made it so you can come back\n"
	.string "in through there.\p"
	.string "Why don't you go get rested and\n"
	.string "refreshed for what lies ahead?\p"
	.string "The ADMIN after me outranks me\n"
	.string "thoroughly as a TRAINER.\p"
	.string "You'd better steel yourself for one\n"
	.string "harsh challenge.$"

FiveIsland_RocketWarehouse_Text_Admin2Intro::
    .string "M-CORE failed, but NOX has one more\n"
    .string "plan. The SAPPHIRE's power\l"
    .string "will make the WAVE CALLER reach\l"
    .string "the bottom of the sea!$"

FiveIsland_RocketWarehouse_Text_Admin2Defeat::
    .string "Urgh…\n"
    .string "You were too strong…\p"
    .string "…\n"
    .string "Th-that's NOX's BADGE!\p"
    .string "So it's true?\n"
    .string "TEAM ECLIPSE really has disbanded?$"

FiveIsland_RocketWarehouse_Text_Admin2PostBattle::
    .string "We will abandon this WAREHOUSE…\p"
    .string "But don't think this is over.\n"
    .string "I won't let this be the end.\p"
    .string "I will find NOX.\n"
    .string "And I will resurrect TEAM ECLIPSE!\l"
    .string "I will…\p"
    .string "Until then, farewell!$"

FiveIsland_RocketWarehouse_Text_ReportWithGraphsAndText::
	.string "A report with graphs and text is on\n"
	.string "the PC monitor.\p"
	.string "“The Influence of Magnetic Waves\n"
	.string "on POKéMON Evolution”…\p"
	.string "… … … … … …\p"
	.string "It's too difficult to understand…$"

FiveIsland_RocketWarehouse_Text_GideonIntro::
    .string "Heh heh. This SAPPHIRE is the last\n"
    .string "part of the WAVE CALLER. You can't\l"
    .string "have it!$"

FiveIsland_RocketWarehouse_Text_GideonDefeat::
	.string "Gah! Darn!\n"
	.string "Darn! Darn!$"

FiveIsland_RocketWarehouse_Text_GideonPostBattle::
    .string "Fine, take the gem… But NOX is still\n"
    .string "here. Go and see for yourself!$"

FiveIsland_RocketWarehouse_Text_GetLostLeaveMeBe::
	.string "What do you want now?\n"
	.string "Go on, get lost, leave me be.$"

FiveIsland_RocketWarehouse_Text_GiovannisKidHasRedHair::
    .string "What, you again?\p"
    .string "You… You're not NOX's kid,\n"
    .string "are you?\p"
    .string "No, that can't be right!\n"
    .string "NOX's kid has red hair.\p"
    .string "You don't have any reasons for\n"
    .string "bugging me. Leave me alone!$"


FiveIsland_RocketWarehouse_Text_NoxFinalIntro::
    .string "NOX: So it ends where it began. On\n"
    .string "the sea.\p"
    .string "M-CORE is gone, and GROUDON slipped\n"
    .string "from my grasp. The WAVE CALLER was\l"
    .string "my last secret.\p"
    .string "Without the SAPPHIRE, the WAVE\n"
    .string "CALLER can't reach the guardian of\l"
    .string "the sea. You've ruined years of my\n"
    .string "work.\p"
    .string "Then I'll take my last stand with\n"
    .string "everything I have left. Come,\l"
    .string "{PLAYER}!$"

FiveIsland_RocketWarehouse_Text_NoxFinalDefeat::
    .string "…Even the sea itself seems to be on\n"
    .string "your side.$"

FiveIsland_RocketWarehouse_Text_NoxFinalAfter::
    .string "NOX: When my crew was lost, I\n"
    .string "wanted the sea to answer to me.\p"
    .string "But you and your POKéMON… you\n"
    .string "trust each other the way my crew\l"
    .string "once trusted me.\p"
    .string "TEAM ECLIPSE is finished. I'll shut\n"
    .string "the WAVE CALLER down myself.\l"
    .string "Farewell, {PLAYER}.$"

FiveIsland_RocketWarehouse_Text_FriendsArrive::
    .string "{RIVAL} and LIA came running in!$"

FiveIsland_RocketWarehouse_Text_LiaFinale::
    .string "LIA: {PLAYER}! The currents are\n"
    .string "calming down already. You did it!\p"
    .string "But LUGIA is still restless after\n"
    .string "the WAVE CALLER's signals. Someone\l"
    .string "has to go and meet it.\p"
    .string "Here, take this. It's a ticket for\n"
    .string "the ship to NAVEL ROCK, where\l"
    .string "LUGIA rests.$"

FiveIsland_RocketWarehouse_Text_LiaTicketExplain::
    .string "Show the MYSTIC TICKET at the\n"
    .string "VERMILION CITY harbor. The\l"
    .string "SEAGALLOP will take you to NAVEL\n"
    .string "ROCK.\p"
    .string "Whether you catch LUGIA or let it\n"
    .string "rest, I know you'll do what's\l"
    .string "right.$"

FiveIsland_RocketWarehouse_Text_ZumoFinale::
    .string "{RIVAL}: Man, I missed the big\n"
    .string "fight! You beat NOX all by\l"
    .string "yourself!\p"
    .string "After this, I'm going to sail to\n"
    .string "HOENN with LIA. There are so many\l"
    .string "POKéMON I've never met!\p"
    .string "Let's battle again someday, okay?\n"
    .string "Smell ya later, {PLAYER}!$"
