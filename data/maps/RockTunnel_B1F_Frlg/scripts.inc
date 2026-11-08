RockTunnel_B1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RockTunnel_B1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, RockTunnel_B1F_OnResume
	.byte 0

RockTunnel_B1F_OnTransition::
	setflag FLAG_HIDE_EARLY_MEWTWO
	goto_if_set FLAG_EARLY_MEWTWO_DONE, RockTunnel_B1F_EventScript_EndTransition
	goto_if_unset FLAG_ZK_READ_MCORE_LOG, RockTunnel_B1F_EventScript_EndTransition
	clearflag FLAG_HIDE_EARLY_MEWTWO
RockTunnel_B1F_EventScript_EndTransition::
	end

RockTunnel_B1F_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, RockTunnel_B1F_EventScript_TryRemoveMewtwo
	end

RockTunnel_B1F_EventScript_TryRemoveMewtwo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

RockTunnel_B1F_EventScript_Mewtwo::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MEWTWO, CRY_MODE_ENCOUNTER
	message RockTunnel_B1F_Text_Mewtwo
	waitmessage
	waitmoncry
	delay 20
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setwildbattle SPECIES_MEWTWO, 25
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RockTunnel_B1F_EventScript_CaughtMewtwo
	msgbox RockTunnel_B1F_Text_MewtwoStillHere
	release
	end

RockTunnel_B1F_EventScript_CaughtMewtwo::
	setflag FLAG_EARLY_MEWTWO_DONE
	setflag FLAG_FOUGHT_MEWTWO
	release
	end

RockTunnel_B1F_EventScript_Sofia::
	trainerbattle_single TRAINER_PICNICKER_SOFIA, RockTunnel_B1F_Text_SofiaIntro, RockTunnel_B1F_Text_SofiaDefeat
	msgbox RockTunnel_B1F_Text_SofiaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Martha::
	trainerbattle_single TRAINER_PICNICKER_MARTHA, RockTunnel_B1F_Text_MarthaIntro, RockTunnel_B1F_Text_MarthaDefeat
	msgbox RockTunnel_B1F_Text_MarthaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Dudley::
	trainerbattle_single TRAINER_HIKER_DUDLEY, RockTunnel_B1F_Text_DudleyIntro, RockTunnel_B1F_Text_DudleyDefeat
	msgbox RockTunnel_B1F_Text_DudleyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Allen::
	trainerbattle_single TRAINER_HIKER_ALLEN, RockTunnel_B1F_Text_AllenIntro, RockTunnel_B1F_Text_AllenDefeat
	msgbox RockTunnel_B1F_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Eric::
	trainerbattle_single TRAINER_HIKER_ERIC, RockTunnel_B1F_Text_EricIntro, RockTunnel_B1F_Text_EricDefeat
	msgbox RockTunnel_B1F_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Cooper::
	trainerbattle_single TRAINER_POKEMANIAC_COOPER, RockTunnel_B1F_Text_CooperIntro, RockTunnel_B1F_Text_CooperDefeat
	msgbox RockTunnel_B1F_Text_CooperPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Steve::
	trainerbattle_single TRAINER_POKEMANIAC_STEVE, RockTunnel_B1F_Text_SteveIntro, RockTunnel_B1F_Text_SteveDefeat
	msgbox RockTunnel_B1F_Text_StevePostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Winston::
	trainerbattle_single TRAINER_POKEMANIAC_WINSTON, RockTunnel_B1F_Text_WinstonIntro, RockTunnel_B1F_Text_WinstonDefeat
	msgbox RockTunnel_B1F_Text_WinstonPostBattle, MSGBOX_AUTOCLOSE
	end

@ Zumo: TEAM MAGMA drilling squad
RockTunnel_B1F_EventScript_MagmaGrunt1::
	trainerbattle_single TRAINER_ZK_ROCKTUNNEL_MAGMA_1, RockTunnel_B1F_Text_MagmaGrunt1Intro, RockTunnel_B1F_Text_MagmaGrunt1Defeat
	msgbox RockTunnel_B1F_Text_MagmaGrunt1After, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_MagmaGrunt2::
	trainerbattle_single TRAINER_ZK_ROCKTUNNEL_MAGMA_2, RockTunnel_B1F_Text_MagmaGrunt2Intro, RockTunnel_B1F_Text_MagmaGrunt2Defeat
	msgbox RockTunnel_B1F_Text_MagmaGrunt2After, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Tabitha::
	trainerbattle_single TRAINER_ZK_ROCKTUNNEL_TABITHA, RockTunnel_B1F_Text_TabithaIntro, RockTunnel_B1F_Text_TabithaDefeat, RockTunnel_B1F_EventScript_TabithaDefeated
	end

RockTunnel_B1F_EventScript_TabithaDefeated::
	msgbox RockTunnel_B1F_Text_TabithaAfter
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_ZK_ROCKTUNNEL_MAGMA_GONE
	getplayerxy VAR_0x8004, VAR_0x8005
	warpsilent MAP_ROCK_TUNNEL_B1F, VAR_0x8004, VAR_0x8005
	waitstate
	end

RockTunnel_B1F_Text_SofiaIntro::
	.string "Do you know how you can avoid\n"
	.string "getting lost in the mountains?\p"
	.string "You can bend twigs as trail\n"
	.string "markers.$"

RockTunnel_B1F_Text_SofiaDefeat::
	.string "Ohhh!\n"
	.string "I did my best!$"

RockTunnel_B1F_Text_SofiaPostBattle::
	.string "I want to go home!$"

RockTunnel_B1F_Text_DudleyIntro::
	.string "Hahaha!\n"
	.string "Can you beat my power?$"

RockTunnel_B1F_Text_DudleyDefeat::
	.string "Oops!\n"
	.string "Outmuscled!$"

RockTunnel_B1F_Text_DudleyPostBattle::
	.string "I go for power because I hate\n"
	.string "thinking.$"

RockTunnel_B1F_Text_CooperIntro::
	.string "You have a POKéDEX?\n"
	.string "I want one, too.$"

RockTunnel_B1F_Text_CooperDefeat::
	.string "Shoot!\n"
	.string "I am so jealous!$"

RockTunnel_B1F_Text_CooperPostBattle::
	.string "When you finish your POKéDEX,\n"
	.string "can I have it?$"

RockTunnel_B1F_Text_SteveIntro::
	.string "Um… Do you know about POKéMON\n"
	.string "cosplay?$"

RockTunnel_B1F_Text_SteveDefeat::
	.string "Well, that's that.$"

RockTunnel_B1F_Text_StevePostBattle::
    .string "POKéMON cosplay is dressing up as\n"
    .string "POKéMON for fun.\p"
    .string "PIPPI is a favorite.$"

RockTunnel_B1F_Text_AllenIntro::
	.string "My POKéMON techniques will leave\n"
	.string "you crying!$"

RockTunnel_B1F_Text_AllenDefeat::
	.string "I give!\n"
	.string "You're a better technician!$"

RockTunnel_B1F_Text_AllenPostBattle::
	.string "In mountains, you'll often find\n"
	.string "ROCK-type POKéMON.$"

RockTunnel_B1F_Text_MarthaIntro::
	.string "I don't often come here, but I will\n"
	.string "battle you.$"

RockTunnel_B1F_Text_MarthaDefeat::
	.string "Oh!\n"
	.string "I lost!$"

RockTunnel_B1F_Text_MarthaPostBattle::
	.string "I like tiny POKéMON.\n"
	.string "Big ones are too scary!$"

RockTunnel_B1F_Text_EricIntro::
	.string "Hit me with your best shot!$"

RockTunnel_B1F_Text_EricDefeat::
	.string "Fired away!$"

RockTunnel_B1F_Text_EricPostBattle::
	.string "I'll raise my POKéMON to beat\n"
	.string "yours, kid.$"

RockTunnel_B1F_Text_WinstonIntro::
	.string "I draw illustrations of POKéMON\n"
	.string "when I'm home.$"

RockTunnel_B1F_Text_WinstonDefeat::
	.string "Whew…\n"
	.string "I'm exhausted…$"

RockTunnel_B1F_Text_WinstonPostBattle::
	.string "I'm an artist, not a fighter.\n"
	.string "I'll go home to draw.$"


RockTunnel_B1F_Text_Mewtwo::
    .string "A powerful POKéMON with a TEAM\n"
    .string "ECLIPSE tag on its arm is resting\l"
    .string "here.\p"
    .string "It glares at you!$"

RockTunnel_B1F_Text_MewtwoStillHere::
    .string "The powerful POKéMON is still deep\n"
    .string "inside the tunnel. Rest up and try\l"
    .string "again!$"

RockTunnel_B1F_Text_MagmaGrunt1Intro::
    .string "Hey! This tunnel is TEAM MAGMA's\n"
    .string "drilling site now!$"

RockTunnel_B1F_Text_MagmaGrunt1Defeat::
    .string "My drill… I mean, my POKéMON!$"

RockTunnel_B1F_Text_MagmaGrunt1After::
    .string "The rock here hums when the RED\n"
    .string "light under CINNABAR flashes.\l"
    .string "Creepy, huh?$"

RockTunnel_B1F_Text_MagmaGrunt2Intro::
    .string "ADMIN TABITHA said nobody gets\n"
    .string "past us!$"

RockTunnel_B1F_Text_MagmaGrunt2Defeat::
    .string "Ugh! Too hot to handle!$"

RockTunnel_B1F_Text_MagmaGrunt2After::
    .string "We're mapping every fault line in\n"
    .string "KANTO for the boss.$"

RockTunnel_B1F_Text_TabithaIntro::
    .string "TABITHA: You again?!\p"
    .string "The bedrock under KANTO answers\n"
    .string "the call of GROUDON. We only need\l"
    .string "to measure the echo!\p"
    .string "I won't lose twice!$"

RockTunnel_B1F_Text_TabithaDefeat::
    .string "Grr… I lost twice?!$"

RockTunnel_B1F_Text_TabithaAfter::
    .string "TABITHA: Fine, we have our data.\p"
    .string "But be careful, kid. Something\n"
    .string "escaped from NOX's lab under\l"
    .string "CELADON's GAME CORNER.\p"
    .string "A POKéMON made by humans…\n"
    .string "They say it hides deep in this\l"
    .string "very tunnel.\p"
    .string "TEAM MAGMA, move out!$"
