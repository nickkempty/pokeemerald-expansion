PewterCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PewterCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_PEWTER_CITY
	end

PewterCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

PewterCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox PewterCity_PokemonCenter_1F_Text_TeamRocketMtMoonImOnPhone, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_Jigglypuff::
	lock
	faceplayer
	fadeoutbgm 0
	playbgm MUS_RG_JIGGLYPUFF, 0
	message PewterCity_PokemonCenter_1F_Text_Jigglypuff
	waitmessage
	delay 455
	playbgm MUS_POKE_CENTER, 1
	release
	end

PewterCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox PewterCity_PokemonCenter_1F_Text_WhenJiggylypuffSingsMonsGetDrowsy, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_GBAKid1::
	lock
	msgbox PewterCity_PokemonCenter_1F_Text_TradingMyClefairyForPikachu
	release
	end

PewterCity_PokemonCenter_1F_EventScript_GBAKid2::
	msgbox PewterCity_PokemonCenter_1F_Text_TradingPikachuWithKid, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_MigrantAide::
	lock
	faceplayer
	goto_if_set FLAG_MIGRANT_QUEST_DONE, PewterCity_PokemonCenter_1F_EventScript_MigrantAideDone
	specialvar VAR_RESULT, CountCaughtMigrantMons
	goto_if_ge VAR_RESULT, 3, PewterCity_PokemonCenter_1F_EventScript_MigrantAideReward
	buffernumberstring STR_VAR_1, VAR_RESULT
	msgbox PewterCity_PokemonCenter_1F_Text_MigrantAideNotYet
	release
	end

PewterCity_PokemonCenter_1F_EventScript_MigrantAideReward::
	msgbox PewterCity_PokemonCenter_1F_Text_MigrantAideGreat
	giveitem ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	setflag FLAG_MIGRANT_QUEST_DONE
	msgbox PewterCity_PokemonCenter_1F_Text_MigrantAideAfter
	release
	end

PewterCity_PokemonCenter_1F_EventScript_MigrantAideDone::
	msgbox PewterCity_PokemonCenter_1F_Text_MigrantAideAfter
	release
	end

PewterCity_PokemonCenter_1F_Text_TeamRocketMtMoonImOnPhone::
    .string "What!?\p"
    .string "TEAM ECLIPSE is at MT. MOON?\n"
    .string "Huh?\p"
    .string "I'm on the phone!\n"
    .string "Scram!$"

PewterCity_PokemonCenter_1F_Text_Jigglypuff::
    .string "PURIN: Puu pupuu!$"

PewterCity_PokemonCenter_1F_Text_WhenJiggylypuffSingsMonsGetDrowsy::
    .string "Yawn!\p"
    .string "When PURIN sings, POKéMON get\n"
    .string "drowsy…\p"
    .string "…Me, too…\n"
    .string "Snore…$"

PewterCity_PokemonCenter_1F_Text_TradingMyClefairyForPikachu::
    .string "I really want a PIKACHU, so I'm\n"
    .string "trading my PIPPI for one.$"

PewterCity_PokemonCenter_1F_Text_TradingPikachuWithKid::
	.string "I'm trading POKéMON with that kid\n"
	.string "there.\p"
	.string "I had two PIKACHU, so I figured\n"
	.string "I might as well trade one.$"


PewterCity_PokemonCenter_1F_Text_MigrantAideNotYet::
    .string "I'm PROF. OAK's AIDE. I'm studying\n"
    .string "the POKéMON that drifted here from\l"
    .string "JOHTO and HOENN.\p"
    .string "You've caught {STR_VAR_1} of them\n"
    .string "so far. Come back when you've\l"
    .string "caught at least three!$"

PewterCity_PokemonCenter_1F_Text_MigrantAideGreat::
    .string "Oh! You've already caught three\n"
    .string "POKéMON from beyond the sea! PROF.\l"
    .string "OAK will be thrilled.\p"
    .string "Please take this. It will help you\n"
    .string "raise all your new friends!$"

PewterCity_PokemonCenter_1F_Text_MigrantAideAfter::
    .string "The EXP. SHARE lets a POKéMON gain\n"
    .string "EXP. without battling.\p"
    .string "Watch out for TEAM ECLIPSE. They\n"
    .string "want those migrant POKéMON too.$"
