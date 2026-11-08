HxRoute119_WeatherInstitute_2F_EventScript_Shelly::
	trainerbattle_single TRAINER_HX_SHELLY_WEATHER_INSTITUTE, HxRoute119_WeatherInstitute_2F_Text_ShellyIntro, HxRoute119_WeatherInstitute_2F_Text_ShellyDefeat, HxRoute119_WeatherInstitute_2F_EventScript_ShellyDefeated
	msgbox HxRoute119_WeatherInstitute_2F_Text_ShellyAfterZumo, MSGBOX_AUTOCLOSE
	end

HxRoute119_WeatherInstitute_2F_EventScript_ShellyDefeated::
	msgbox HxRoute119_WeatherInstitute_2F_Text_ShellyAfterZumo, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_HX_HIDE_WI_AQUA
	playse SE_EXIT
	waitse
	warpsilent MAP_HX_ROUTE119_WEATHER_INSTITUTE_2F, 5, 6
	waitstate
	end

HxRoute119_WeatherInstitute_2F_EventScript_WeatherScientist::
	lock
	faceplayer
	goto_if_unset FLAG_HX_HIDE_WI_AQUA, HxRoute119_WeatherInstitute_2F_EventScript_ScientistScared
	goto_if_set FLAG_HX_RECEIVED_CASTFORM, HxRoute119_WeatherInstitute_2F_EventScript_ScientistAfter
	msgbox HxRoute119_WeatherInstitute_2F_Text_ScientistThanks, MSGBOX_DEFAULT
	setvar VAR_TEMP_1, SPECIES_CASTFORM
	givemon SPECIES_CASTFORM, 45, ITEM_MYSTIC_WATER
	goto_if_eq VAR_RESULT, 2, HxRoute119_WeatherInstitute_2F_EventScript_NoRoom
	setflag FLAG_HX_RECEIVED_CASTFORM
	playfanfare MUS_LEVEL_UP
	message HxRoute119_WeatherInstitute_2F_Text_ReceivedCastform
	waitmessage
	waitfanfare
	msgbox HxRoute119_WeatherInstitute_2F_Text_ScientistAfter, MSGBOX_DEFAULT
	release
	end

HxRoute119_WeatherInstitute_2F_EventScript_ScientistScared::
	msgbox HxRoute119_WeatherInstitute_2F_Text_ScientistScared, MSGBOX_DEFAULT
	release
	end

HxRoute119_WeatherInstitute_2F_EventScript_ScientistAfter::
	msgbox HxRoute119_WeatherInstitute_2F_Text_ScientistAfter, MSGBOX_DEFAULT
	release
	end

HxRoute119_WeatherInstitute_2F_EventScript_NoRoom::
	msgbox HxRoute119_WeatherInstitute_2F_Text_NoRoom, MSGBOX_DEFAULT
	release
	end

HxRoute119_WeatherInstitute_2F_Text_ShellyAfterZumo::
	.string "SHELLY: Ahahahaha!\n"
	.string "Fine, you win this round!\p"
	.string "But the data we need is already\n"
	.string "on its way to our boss ARCHIE.\p"
	.string "With the BLUE ORB and this\n"
	.string "weather data, KYOGRE will wake\l"
	.string "and swallow that silly ROAD whole!\p"
	.string "TEAM AQUA, fall back!$"

HxRoute119_WeatherInstitute_2F_Text_ScientistScared::
	.string "Th-those TEAM AQUA people took\n"
	.string "over the whole institute!$"

HxRoute119_WeatherInstitute_2F_Text_ScientistThanks::
	.string "Thank you! You saved us all!\p"
	.string "TEAM AQUA stole our records of\n"
	.string "the storms over the land bridge.\p"
	.string "Please, take this POKéMON as\n"
	.string "thanks. It was born from our\l"
	.string "weather research.$"

HxRoute119_WeatherInstitute_2F_Text_ReceivedCastform::
	.string "{PLAYER} received POWALEN!$"

HxRoute119_WeatherInstitute_2F_Text_ScientistAfter::
	.string "The storms over the sea are not\n"
	.string "natural. Something under the\l"
	.string "ocean is pushing back against\l"
	.string "the land GROUDON raised.\p"
	.string "Old records speak of a third\n"
	.string "POKéMON that lives high above\l"
	.string "SKY PILLAR and calms the land\l"
	.string "and the sea.\p"
	.string "The FORTREE GYM LEADER knows the\n"
	.string "skies better than anyone. Ask her!$"

HxRoute119_WeatherInstitute_2F_Text_NoRoom::
	.string "Oh, you have no room for another\n"
	.string "POKéMON… Come back later!$"
