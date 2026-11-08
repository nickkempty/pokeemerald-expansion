HxLilycoveCity_MapScripts::
	.byte 0

HxLilycoveCity_EventScript_Sailor2::
	lock
	faceplayer
	goto_if_set FLAG_HX_TEAM_AQUA_ESCAPED_IN_SUBMARINE, HxLilycoveCity_EventScript_Sailor2AquaGone
	msgbox HxLilycoveCity_Text_TeamAquaBeenTrainingWailmer, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_Sailor2AquaGone::
	msgbox HxLilycoveCity_Text_SomeonePuntedTeamAquaOut, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_HX_BADGE07_GET, HxLilycoveCity_EventScript_GirlAquaGone
	msgbox HxLilycoveCity_Text_StrangeCaveInCove, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_GirlAquaGone::
	msgbox HxLilycoveCity_Text_GoingToMoveDeleterForHMs, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_Man3::
	msgbox HxLilycoveCity_Text_ContestHallInTown, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_RichBoy::
	msgbox HxLilycoveCity_Text_ImFromKanto, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_Man2::
	msgbox HxLilycoveCity_Text_ImArtDealer, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_Woman2::
	lock
	faceplayer
	goto_if_set FLAG_HX_TEAM_AQUA_ESCAPED_IN_SUBMARINE, HxLilycoveCity_EventScript_Woman2AquaGone
	msgbox HxLilycoveCity_Text_SomeoneStoleMyPokemon, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_Woman2AquaGone::
	msgbox HxLilycoveCity_Text_MissingPokemonCameBack, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_ExpertM1::
	msgbox HxLilycoveCity_Text_SeaRemainsForeverYoung, MSGBOX_SIGN
	end

HxLilycoveCity_EventScript_ExpertF::
	msgbox HxLilycoveCity_Text_SixtyYearsAgoHusbandProposed, MSGBOX_SIGN
	end

HxLilycoveCity_EventScript_ExpertM2::
	lock
	faceplayer
	goto_if_set FLAG_HX_BADGE07_GET, HxLilycoveCity_EventScript_ExpertM2AquaGone
	msgbox HxLilycoveCity_Text_TeamAquaRenovatedCavern, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_ExpertM2AquaGone::
	msgbox HxLilycoveCity_Text_TeamAquaLotGoneForGood, MSGBOX_DEFAULT
	release
	end

HxLilycoveCity_EventScript_ItemMaxRepel::
	finditem ITEM_MAX_REPEL
	end

HxLilycoveCity_EventScript_SchoolKidM::
	lock
	faceplayer
	msgbox HxLilycoveCity_Text_DoYouKnowAboutBerryBlender, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, HxLilycoveCity_EventScript_KnowAboutBerryBlender
	call_if_eq VAR_RESULT, NO, HxLilycoveCity_EventScript_DontKnowAboutBerryBlender
	release
	end

HxLilycoveCity_EventScript_KnowAboutBerryBlender::
	msgbox HxLilycoveCity_Text_FasterSpinBetterPokeblocks, MSGBOX_DEFAULT
	return

HxLilycoveCity_EventScript_DontKnowAboutBerryBlender::
	msgbox HxLilycoveCity_Text_ExplainBerryBlender, MSGBOX_DEFAULT
	return

HxLilycoveCity_EventScript_Woman1::
	msgbox HxLilycoveCity_Text_HoneymoonVowToSeeRarePokemon, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_Man1::
	msgbox HxLilycoveCity_Text_JustArrivedAndSawRarePokemon, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_Sailor1::
	msgbox HxLilycoveCity_Text_HeardTowerCalledSkyPillar, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_FatMan::
	msgbox HxLilycoveCity_Text_SawTallTowerOnRoute131, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_MuseumSign::
	msgbox HxLilycoveCity_Text_MuseumSignPlayersExhibit, MSGBOX_NPC
	end

HxLilycoveCity_EventScript_DepartmentStoreSign::
	msgbox HxLilycoveCity_Text_DepartmentStoreSign, MSGBOX_SIGN
	end

HxLilycoveCity_EventScript_CitySign::
	msgbox HxLilycoveCity_Text_CitySign, MSGBOX_SIGN
	end

HxLilycoveCity_EventScript_ContestHallSign::
	msgbox HxLilycoveCity_Text_ContestHallSign, MSGBOX_SIGN
	end

HxLilycoveCity_EventScript_MotelSign::
	msgbox HxLilycoveCity_Text_MotelSign, MSGBOX_SIGN
	end

HxLilycoveCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_HX_SYS_GAME_CLEAR, HxLilycoveCity_EventScript_HarborSignFerryReady
	msgbox HxLilycoveCity_Text_HarborSignUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

HxLilycoveCity_EventScript_HarborSignFerryReady::
	msgbox HxLilycoveCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

HxLilycoveCity_EventScript_TrainerFanClubSign::
	msgbox HxLilycoveCity_Text_TrainerFanClubSign, MSGBOX_SIGN
	end

HxLilycoveCity_EventScript_MoveDeletersHouseSign::
	msgbox HxLilycoveCity_Text_MoveDeletersHouseSign, MSGBOX_SIGN
	end

