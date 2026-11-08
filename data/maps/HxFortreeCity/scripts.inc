HxFortreeCity_MapScripts::
	.byte 0

HxFortreeCity_EventScript_Man::
	msgbox HxFortreeCity_Text_SawGiganticPokemonInSky, MSGBOX_NPC
	end

HxFortreeCity_EventScript_Girl::
	msgbox HxFortreeCity_Text_TreesGrowByDrinkingRainwater, MSGBOX_NPC
	end

HxFortreeCity_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_HX_KECLEON_FLED_FORTREE, HxFortreeCity_EventScript_WomanGymAccessible
	msgbox HxFortreeCity_Text_SomethingBlockingGym, MSGBOX_DEFAULT
	release
	end

HxFortreeCity_EventScript_WomanGymAccessible::
	msgbox HxFortreeCity_Text_ThisTimeIllBeatWinona, MSGBOX_DEFAULT
	release
	end

HxFortreeCity_EventScript_Boy::
	msgbox HxFortreeCity_Text_BugPokemonComeThroughWindow, MSGBOX_NPC
	end

HxFortreeCity_EventScript_OldMan::
	msgbox HxFortreeCity_Text_EveryoneHealthyAndLively, MSGBOX_NPC
	end

HxFortreeCity_EventScript_GameboyKid::
	msgbox HxFortreeCity_Text_PokemonThatEvolveWhenTraded, MSGBOX_NPC
	end

HxFortreeCity_EventScript_CitySign::
	msgbox HxFortreeCity_Text_CitySign, MSGBOX_SIGN
	end

HxFortreeCity_EventScript_GymSign::
	msgbox HxFortreeCity_Text_GymSign, MSGBOX_SIGN
	end

