#include "global.h"
#include "sprite.h"
#include "task.h"
#include "event_object_movement.h"
#include "pokemon_icon.h"
#include "random.h"
#include "wild_encounter.h"
#include "event_data.h"
#include "fieldmap.h"
#include "random.h"
#include "roamer.h"
#include "field_player_avatar.h"
#include "battle_setup.h"
#include "overworld.h"
#include "metatile_behavior.h"
#include "event_scripts.h"
#include "script.h"
#include "link.h"
#include "quest_log.h"
#include "constants/maps.h"
#include "constants/abilities.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/pokemon.h"
#include "safari_zone.h"
#include "pokemon.h"

#define MAX_ENCOUNTER_RATE 1600

#define HEADER_NONE 0xFFFF

struct WildEncounterData
{
    u32 rngState;
    u16 prevMetatileBehavior;
    u16 encounterRateBuff;
    u8 stepsSinceLastEncounter;
    u8 abilityEffect;
    u16 leadMonHeldItem;
};

static EWRAM_DATA struct WildEncounterData sWildEncounterData = {};
static EWRAM_DATA bool8 sWildEncountersDisabled = FALSE;

static bool8 UnlockedTanobyOrAreNotInTanoby(void);
static u32 GenerateUnownPersonalityByLetter(u8 letter);
static bool8 IsWildLevelAllowedByRepel(u8 level);
static void ApplyFluteEncounterRateMod(u32 *rate);
static u8 GetFluteEncounterRateModType(void);
static void ApplyCleanseTagEncounterRateMod(u32 *rate);
static bool8 IsLeadMonHoldingCleanseTag(void);
static u16 WildEncounterRandom(void);
static void AddToWildEncounterRateBuff(u8 encouterRate);

#include "data/wild_encounters.h"

static const u8 sUnownLetterSlots[][LAND_WILD_COUNT] = {
  //  A   A   A   A   A   A   A   A   A   A   A   ?
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 27},
  //  C   C   C   D   D   D   H   H   H   U   U   O
    { 2,  2,  2,  3,  3,  3,  7,  7,  7, 20, 20, 14},
  //  N   N   N   N   S   S   S   S   I   I   E   E
    {13, 13, 13, 13, 18, 18, 18, 18,  8,  8,  4,  4},
  //  P   P   L   L   J   J   R   R   R   Q   Q   Q
    {15, 15, 11, 11,  9,  9, 17, 17, 17, 16, 16, 16},
  //  Y   Y   T   T   G   G   G   F   F   F   K   K
    {24, 24, 19, 19,  6,  6,  6,  5,  5,  5, 10, 10},
  //  V   V   V   W   W   W   X   X   M   M   B   B
    {21, 21, 21, 22, 22, 22, 23, 23, 12, 12,  1,  1},
  //  Z   Z   Z   Z   Z   Z   Z   Z   Z   Z   Z   !
    {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26},
};

void DisableWildEncounters(bool8 state)
{
    sWildEncountersDisabled = state;
}

static u8 ChooseWildMonIndex_Land(void)
{
    u8 rand = Random() % ENCOUNTER_CHANCE_LAND_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_0)
        return 0;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_1)
        return 1;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_2)
        return 2;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_3)
        return 3;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_3 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_4)
        return 4;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_4 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_5)
        return 5;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_5 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_6)
        return 6;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_6 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_7)
        return 7;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_7 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_8)
        return 8;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_8 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_9)
        return 9;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_9 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_10)
        return 10;
    else
        return 11;
}

static u8 ChooseWildMonIndex_WaterRock(void)
{
    u8 rand = Random() % ENCOUNTER_CHANCE_WATER_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_0)
        return 0;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_1)
        return 1;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_2)
        return 2;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_3)
        return 3;
    else
        return 4;
}

static u8 ChooseWildMonIndex_Fishing(u8 rod)
{
    u8 wildMonIndex = 0;
    u8 rand = Random() % max(max(ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_TOTAL, ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_TOTAL),
                             ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_TOTAL);

    switch (rod)
    {
    case OLD_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_SLOT_0)
            wildMonIndex = 0;
        else
            wildMonIndex = 1;
        break;
    case GOOD_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2)
            wildMonIndex = 2;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3)
            wildMonIndex = 3;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_4)
            wildMonIndex = 4;
        break;
    case SUPER_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5)
            wildMonIndex = 5;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6)
            wildMonIndex = 6;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7)
            wildMonIndex = 7;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8)
            wildMonIndex = 8;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_9)
            wildMonIndex = 9;
        break;
    }
    return wildMonIndex;
}

static u8 ChooseWildMonLevel(const struct WildPokemon * info)
{
    u8 lo;
    u8 hi;
    u8 mod;
    u8 res;
    if (info->maxLevel >= info->minLevel)
    {
        lo = info->minLevel;
        hi = info->maxLevel;
    }
    else
    {
        lo = info->maxLevel;
        hi = info->minLevel;
    }
    mod = hi - lo + 1;
    res = Random() % mod;
    return lo + res;
}

static u16 GetCurrentMapWildMonHeaderId(void)
{
    u16 i;

    for (i = 0; ; i++)
    {
        const struct WildPokemonHeader * wildHeader = &gWildMonHeaders[i];
        if (wildHeader->mapGroup == MAP_GROUP(MAP_UNDEFINED))
            break;

        if (gWildMonHeaders[i].mapGroup == gSaveBlock1Ptr->location.mapGroup &&
            gWildMonHeaders[i].mapNum == gSaveBlock1Ptr->location.mapNum)
        {
            if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_SIX_ISLAND_ALTERING_CAVE) &&
                gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SIX_ISLAND_ALTERING_CAVE))
            {
                u16 alteringCaveId = VarGet(VAR_ALTERING_CAVE_WILD_SET);
                if (alteringCaveId >= NUM_ALTERING_CAVE_TABLES)
                    alteringCaveId = 0;

                i += alteringCaveId;
            }

            if (!UnlockedTanobyOrAreNotInTanoby())
                break;
            return i;
        }
    }

    return HEADER_NONE;
}

static bool8 UnlockedTanobyOrAreNotInTanoby(void)
{
    if (FlagGet(FLAG_SYS_UNLOCKED_TANOBY_RUINS))
        return TRUE;
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER))
        return TRUE;
    if (!(gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER)
    ||  gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_LIPTOO_CHAMBER)
    ||  gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_WEEPTH_CHAMBER)
    ||  gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER)
    ||  gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_SCUFIB_CHAMBER)
    ||  gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_RIXY_CHAMBER)
    ||  gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_VIAPOIS_CHAMBER)
    ))
        return TRUE;
    return FALSE;
}

// ---- Zumo: Gen 2 legendary encounters, 2% in every wild encounter area ----
extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];
u16 GetPlayerStarterSpecies(void);

static bool8 Zumo_InFamily(u16 species, u16 root, u8 depth)
{
    u8 i;
    if (species == root)
        return TRUE;
    if (depth == 0)
        return FALSE;
    for (i = 0; i < EVOS_PER_MON; i++)
    {
        u16 t = gEvolutionTable[root][i].targetSpecies;
        if (t != SPECIES_NONE && Zumo_InFamily(species, t, depth - 1))
            return TRUE;
    }
    return FALSE;
}

static u8 Zumo_LegendaryLevel(void)
{
    u8 i, maxLevel = 0, starterLevel = 0;
    u16 starter = GetPlayerStarterSpecies();
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 sp = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        u8 lv;
        if (sp == SPECIES_NONE || sp == SPECIES_EGG)
            continue;
        lv = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        if (lv > maxLevel)
            maxLevel = lv;
        if (starterLevel == 0 && Zumo_InFamily(sp, starter, 3))
            starterLevel = lv;
    }
    if (maxLevel <= 25)
        return 25;
    if (starterLevel == 0)
        starterLevel = GetMonData(&gPlayerParty[0], MON_DATA_LEVEL);
    return starterLevel + 5 > 100 ? 100 : starterLevel + 5;
}

static bool8 Zumo_TryLegendary(u16 *species, u8 *level)
{
    static const u16 sLegends[] = {SPECIES_RAIKOU, SPECIES_ENTEI, SPECIES_SUICUNE, SPECIES_LUGIA, SPECIES_HO_OH};
    if (*species == SPECIES_UNOWN || GetSafariZoneFlag())
        return FALSE;
#ifndef ZUMO_TEST
    if (Random() % 100 >= 2)
        return FALSE;
#endif
    *species = sLegends[Random() % ARRAY_COUNT(sLegends)];
    *level = Zumo_LegendaryLevel();
    return TRUE;
}

static void GenerateWildMon(u16 species, u8 level, u8 slot)
{
    u32 personality;
    s8 chamber;
    Zumo_TryLegendary(&species, &level);
    ZeroEnemyPartyMons();
    if (species != SPECIES_UNOWN)
    {
        CreateMonWithNature(&gEnemyParty[0], species, level, USE_RANDOM_IVS, Random() % NUM_NATURES);
    }
    else
    {
        chamber = gSaveBlock1Ptr->location.mapNum - MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER);
        personality = GenerateUnownPersonalityByLetter(sUnownLetterSlots[chamber][slot]);
        CreateMon(&gEnemyParty[0], species, level, USE_RANDOM_IVS, TRUE, personality, FALSE, 0);
    }
}

static u32 GenerateUnownPersonalityByLetter(u8 letter)
{
    u32 personality;
    do
    {
        personality = (Random() << 16) | Random();
    } while (GetUnownLetterByPersonalityLoByte(personality) != letter);
    return personality;
}

u8 GetUnownLetterByPersonalityLoByte(u32 personality)
{
    return GET_UNOWN_LETTER(personality);
}

enum
{
    WILD_AREA_LAND,
    WILD_AREA_WATER,
    WILD_AREA_ROCKS,
    WILD_AREA_FISHING,
};

#define WILD_CHECK_REPEL    0x1
#define WILD_CHECK_KEEN_EYE 0x2

static bool8 TryGenerateWildMon(const struct WildPokemonInfo * info, u8 area, u8 flags)
{
    u8 slot = 0;
    u8 level;
    switch (area)
    {
    case WILD_AREA_LAND:
        slot = ChooseWildMonIndex_Land();
        break;
    case WILD_AREA_WATER:
        slot = ChooseWildMonIndex_WaterRock();
        break;
    case WILD_AREA_ROCKS:
        slot = ChooseWildMonIndex_WaterRock();
        break;
    }
    level = ChooseWildMonLevel(&info->wildPokemon[slot]);
    if (flags == WILD_CHECK_REPEL && !IsWildLevelAllowedByRepel(level))
    {
        return FALSE;
    }
    GenerateWildMon(info->wildPokemon[slot].species, level, slot);
    return TRUE;
}

static u16 GenerateFishingEncounter(const struct WildPokemonInfo * info, u8 rod)
{
    u8 slot = ChooseWildMonIndex_Fishing(rod);
    u8 level = ChooseWildMonLevel(&info->wildPokemon[slot]);
    GenerateWildMon(info->wildPokemon[slot].species, level, slot);
    return info->wildPokemon[slot].species;
}

static bool8 DoWildEncounterRateDiceRoll(u16 encounterRate)
{
    if (WildEncounterRandom() % MAX_ENCOUNTER_RATE < encounterRate)
        return TRUE;
    return FALSE;
}

static bool8 DoWildEncounterRateTest(u32 encounterRate, bool8 ignoreAbility)
{
    encounterRate *= 16;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        encounterRate = encounterRate * 80 / 100;
    encounterRate += sWildEncounterData.encounterRateBuff * 16 / 200;
    ApplyFluteEncounterRateMod(&encounterRate);
    ApplyCleanseTagEncounterRateMod(&encounterRate);
    if (!ignoreAbility)
    {
        switch (sWildEncounterData.abilityEffect)
        {
        case 1:
            encounterRate /= 2;
            break;
        case 2:
            encounterRate *= 2;
            break;
        }
    }
    if (encounterRate > MAX_ENCOUNTER_RATE)
        encounterRate = MAX_ENCOUNTER_RATE;
    return DoWildEncounterRateDiceRoll(encounterRate);
}

static u8 GetAbilityEncounterRateModType(void)
{
    sWildEncounterData.abilityEffect = 0;
    if (!GetMonData(&gPlayerParty[0], MON_DATA_SANITY_IS_EGG))
    {
        u8 ability = GetMonAbility(&gPlayerParty[0]);
        if (ability == ABILITY_STENCH)
            sWildEncounterData.abilityEffect = 1;
        else if (ability == ABILITY_ILLUMINATE)
            sWildEncounterData.abilityEffect = 2;
    }
    return sWildEncounterData.abilityEffect;
}

static bool8 DoGlobalWildEncounterDiceRoll(void)
{
    if ((Random() % 100) >= 60)
        return FALSE;
    return TRUE;
}

bool8 StandardWildEncounter(u32 currMetatileAttrs, u16 previousMetatileBehavior)
{
    u16 headerId;
    struct Roamer * roamer;

    if (sWildEncountersDisabled == TRUE)
        return FALSE;

    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId != HEADER_NONE)
    {
        if (ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) == TILE_ENCOUNTER_LAND)
        {
            if (gWildMonHeaders[headerId].landMonsInfo == NULL)
                return FALSE;
            else if (previousMetatileBehavior != ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR) && !DoGlobalWildEncounterDiceRoll())
                return FALSE;
#ifndef ZUMO_TEST
            if (DoWildEncounterRateTest(gWildMonHeaders[headerId].landMonsInfo->encounterRate, FALSE) != TRUE)
#else
            if (0)
#endif
            {
                AddToWildEncounterRateBuff(gWildMonHeaders[headerId].landMonsInfo->encounterRate);
                return FALSE;
            }

            else if (TryStartRoamerEncounter() == TRUE)
            {
                roamer = &gSaveBlock1Ptr->roamer;
                if (!IsWildLevelAllowedByRepel(roamer->level))
                {
                    return FALSE;
                }

                StartRoamerBattle();
                return TRUE;
            }
            else
            {

                // try a regular wild land encounter
                if (TryGenerateWildMon(gWildMonHeaders[headerId].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_REPEL) == TRUE)
                {
                    StartWildBattle();
                    return TRUE;
                }
                else
                {
                    AddToWildEncounterRateBuff(gWildMonHeaders[headerId].landMonsInfo->encounterRate);
                }
            }
        }
        else if (ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) == TILE_ENCOUNTER_WATER
                 || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) && MetatileBehavior_IsBridge(ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR)) == TRUE))
        {
            if (gWildMonHeaders[headerId].waterMonsInfo == NULL)
                return FALSE;
            else if (previousMetatileBehavior != ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR) && !DoGlobalWildEncounterDiceRoll())
                return FALSE;
            else if (DoWildEncounterRateTest(gWildMonHeaders[headerId].waterMonsInfo->encounterRate, FALSE) != TRUE)
            {
                AddToWildEncounterRateBuff(gWildMonHeaders[headerId].waterMonsInfo->encounterRate);
                return FALSE;
            }

            if (TryStartRoamerEncounter() == TRUE)
            {
                roamer = &gSaveBlock1Ptr->roamer;
                if (!IsWildLevelAllowedByRepel(roamer->level))
                {
                    return FALSE;
                }

                StartRoamerBattle();
                return TRUE;
            }
            else // try a regular surfing encounter
            {
                if (TryGenerateWildMon(gWildMonHeaders[headerId].waterMonsInfo, WILD_AREA_WATER, WILD_CHECK_REPEL) == TRUE)
                {
                    StartWildBattle();
                    return TRUE;
                }
                else
                {
                    AddToWildEncounterRateBuff(gWildMonHeaders[headerId].waterMonsInfo->encounterRate);
                }
            }
        }
    }

    return FALSE;
}

void RockSmashWildEncounter(void)
{
    u16 headerIdx = GetCurrentMapWildMonHeaderId();
    if (headerIdx == HEADER_NONE)
        gSpecialVar_Result = FALSE;
    else if (gWildMonHeaders[headerIdx].rockSmashMonsInfo == NULL)
        gSpecialVar_Result = FALSE;
    else if (DoWildEncounterRateTest(gWildMonHeaders[headerIdx].rockSmashMonsInfo->encounterRate, TRUE) != TRUE)
        gSpecialVar_Result = FALSE;
    else if (TryGenerateWildMon(gWildMonHeaders[headerIdx].rockSmashMonsInfo, WILD_AREA_ROCKS, WILD_CHECK_REPEL) == TRUE)
    {
        StartWildBattle();
        gSpecialVar_Result = TRUE;
    }
    else
        gSpecialVar_Result = FALSE;
}

bool8 SweetScentWildEncounter(void)
{
    s16 x, y;
    u16 headerId;

    PlayerGetDestCoords(&x, &y);
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId != HEADER_NONE)
    {
        if (MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) == TILE_ENCOUNTER_LAND)
        {
            if (TryStartRoamerEncounter() == TRUE)
            {
                StartRoamerBattle();
                return TRUE;
            }

            if (gWildMonHeaders[headerId].landMonsInfo == NULL)
                return FALSE;

            TryGenerateWildMon(gWildMonHeaders[headerId].landMonsInfo, WILD_AREA_LAND, 0);

            StartWildBattle();
            return TRUE;
        }
        else if (MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) == TILE_ENCOUNTER_WATER)
        {
            if (TryStartRoamerEncounter() == TRUE)
            {
                StartRoamerBattle();
                return TRUE;
            }

            if (gWildMonHeaders[headerId].waterMonsInfo == NULL)
                return FALSE;

            TryGenerateWildMon(gWildMonHeaders[headerId].waterMonsInfo, WILD_AREA_WATER, 0);
            StartWildBattle();
            return TRUE;
        }
    }

    return FALSE;
}

bool8 DoesCurrentMapHaveFishingMons(void)
{
    u16 headerIdx = GetCurrentMapWildMonHeaderId();
    if (headerIdx == HEADER_NONE)
        return FALSE;
    if (gWildMonHeaders[headerIdx].fishingMonsInfo == NULL)
        return FALSE;
    return TRUE;
}

void FishingWildEncounter(u8 rod)
{
    GenerateFishingEncounter(gWildMonHeaders[GetCurrentMapWildMonHeaderId()].fishingMonsInfo, rod);
    IncrementGameStat(GAME_STAT_FISHING_CAPTURES);
    StartWildBattle();
}

u16 GetLocalWildMon(bool8 *isWaterMon)
{
    u16 headerId;
    const struct WildPokemonInfo * landMonsInfo;
    const struct WildPokemonInfo * waterMonsInfo;

    *isWaterMon = FALSE;
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
        return SPECIES_NONE;
    landMonsInfo = gWildMonHeaders[headerId].landMonsInfo;
    waterMonsInfo = gWildMonHeaders[headerId].waterMonsInfo;
    // Neither
    if (landMonsInfo == NULL && waterMonsInfo == NULL)
        return SPECIES_NONE;
        // Land Pokemon
    else if (landMonsInfo != NULL && waterMonsInfo == NULL)
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
        // Water Pokemon
    else if (landMonsInfo == NULL && waterMonsInfo != NULL)
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock()].species;
    }
    // Either land or water Pokemon
    if ((Random() % 100) < 80)
    {
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
    }
    else
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock()].species;
    }
}

u16 GetLocalWaterMon(void)
{
    u16 headerId = GetCurrentMapWildMonHeaderId();

    if (headerId != HEADER_NONE)
    {
        const struct WildPokemonInfo * waterMonsInfo = gWildMonHeaders[headerId].waterMonsInfo;

        if (waterMonsInfo)
            return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock()].species;
    }
    return SPECIES_NONE;
}

bool8 UpdateRepelCounter(void)
{
    u16 steps;

    if (InUnionRoom() == TRUE)
        return FALSE;

    if (gQuestLogState == QL_STATE_PLAYBACK)
        return FALSE;

    steps = VarGet(VAR_REPEL_STEP_COUNT);

    if (steps != 0)
    {
        steps--;
        VarSet(VAR_REPEL_STEP_COUNT, steps);
        if (steps == 0)
        {
            ScriptContext_SetupScript(EventScript_RepelWoreOff);
            return TRUE;
        }
    }
    return FALSE;
}

static bool8 IsWildLevelAllowedByRepel(u8 wildLevel)
{
    u8 i;

    if (!VarGet(VAR_REPEL_STEP_COUNT))
        return TRUE;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_HP) && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
        {
            u8 ourLevel = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);

            if (wildLevel < ourLevel)
                return FALSE;
            else
                return TRUE;
        }
    }

    return FALSE;
}

static void ApplyFluteEncounterRateMod(u32 *encounterRate)
{
    switch (GetFluteEncounterRateModType())
    {
    case 1:
        *encounterRate += *encounterRate / 2;
        break;
    case 2:
        *encounterRate = *encounterRate / 2;
        break;
    }
}

static u8 GetFluteEncounterRateModType(void)
{
    if (FlagGet(FLAG_SYS_WHITE_FLUTE_ACTIVE) == TRUE)
        return 1;
    else if (FlagGet(FLAG_SYS_BLACK_FLUTE_ACTIVE) == TRUE)
        return 2;
    else
        return 0;
}

static void ApplyCleanseTagEncounterRateMod(u32 *encounterRate)
{
    if (IsLeadMonHoldingCleanseTag())
        *encounterRate = *encounterRate * 2 / 3;
}

static bool8 IsLeadMonHoldingCleanseTag(void)
{
    if (sWildEncounterData.leadMonHeldItem == ITEM_CLEANSE_TAG)
        return TRUE;
    else
        return FALSE;
}

void SeedWildEncounterRng(u16 seed)
{
    sWildEncounterData.rngState = seed;
    ResetEncounterRateModifiers();
}

static u16 WildEncounterRandom(void)
{
    sWildEncounterData.rngState = ISO_RANDOMIZE2(sWildEncounterData.rngState);
    return sWildEncounterData.rngState >> 16;
}

static u8 GetMapBaseEncounterCooldown(u8 encounterType)
{
    u16 headerIdx = GetCurrentMapWildMonHeaderId();
    if (headerIdx == HEADER_NONE)
        return 0xFF;
    if (encounterType == TILE_ENCOUNTER_LAND)
    {
        if (gWildMonHeaders[headerIdx].landMonsInfo == NULL)
            return 0xFF;
        if (gWildMonHeaders[headerIdx].landMonsInfo->encounterRate >= 80)
            return 0;
        if (gWildMonHeaders[headerIdx].landMonsInfo->encounterRate < 10)
            return 8;
        return 8 - (gWildMonHeaders[headerIdx].landMonsInfo->encounterRate / 10);
    }
    if (encounterType == TILE_ENCOUNTER_WATER)
    {
        if (gWildMonHeaders[headerIdx].waterMonsInfo == NULL)
            return 0xFF;
        if (gWildMonHeaders[headerIdx].waterMonsInfo->encounterRate >= 80)
            return 0;
        if (gWildMonHeaders[headerIdx].waterMonsInfo->encounterRate < 10)
            return 8;
        return 8 - (gWildMonHeaders[headerIdx].waterMonsInfo->encounterRate / 10);
    }
    return 0xFF;
}

void ResetEncounterRateModifiers(void)
{
    sWildEncounterData.encounterRateBuff = 0;
    sWildEncounterData.stepsSinceLastEncounter = 0;
}

static bool8 HandleWildEncounterCooldown(u32 currMetatileAttrs)
{
    u8 encounterType = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);
    u32 minSteps;
    u32 encRate;
    if (encounterType == TILE_ENCOUNTER_NONE)
        return FALSE;
    minSteps = GetMapBaseEncounterCooldown(encounterType);
    if (minSteps == 0xFF)
        return FALSE;
    minSteps *= 256;
    encRate = 5 * 256;
    switch (GetFluteEncounterRateModType())
    {
    case 1:
        minSteps -= minSteps / 2;
        encRate += encRate / 2;
        break;
    case 2:
        minSteps *= 2;
        encRate /= 2;
        break;
    }
    sWildEncounterData.leadMonHeldItem = GetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM);
    if (IsLeadMonHoldingCleanseTag() == TRUE)
    {
        minSteps += minSteps / 3;
        encRate -= encRate / 3;
    }
    switch (GetAbilityEncounterRateModType())
    {
    case 1:
        minSteps *= 2;
        encRate /= 2;
        break;
    case 2:
        minSteps /= 2;
        encRate *= 2;
        break;
    }
    minSteps /= 256;
    encRate /= 256;
    if (sWildEncounterData.stepsSinceLastEncounter >= minSteps)
        return TRUE;
    sWildEncounterData.stepsSinceLastEncounter++;
    if ((Random() % 100) < encRate)
        return TRUE;
    return FALSE;
}

bool8 VisibleMons_OnPlayerStep(void);
bool8 VisibleMons_SuppressRandomGrass(u32 metatileAttrs);

bool8 TryStandardWildEncounter(u32 currMetatileAttrs)
{
    if (VisibleMons_OnPlayerStep() == TRUE)
        return TRUE;
    if (VisibleMons_SuppressRandomGrass(currMetatileAttrs))
    {
        sWildEncounterData.prevMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return FALSE;
    }
    if (!HandleWildEncounterCooldown(currMetatileAttrs))
    {
        sWildEncounterData.prevMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return FALSE;
    }
    else if (StandardWildEncounter(currMetatileAttrs, sWildEncounterData.prevMetatileBehavior) == TRUE)
    {
        sWildEncounterData.encounterRateBuff = 0;
        sWildEncounterData.stepsSinceLastEncounter = 0;
        sWildEncounterData.prevMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return TRUE;
    }
    else
    {
        sWildEncounterData.prevMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return FALSE;
    }
}

static void AddToWildEncounterRateBuff(u8 encounterRate)
{
    if (VarGet(VAR_REPEL_STEP_COUNT) == 0)
        sWildEncounterData.encounterRateBuff += encounterRate;
    else
        sWildEncounterData.encounterRateBuff = 0;
}

// ---------------------------------------------------------------------------
// Zumo hack: visible wild POKeMON in tall grass.
// Icons wander on grass tiles; touching one starts the battle. No random
// encounters on tall grass while the map has visible POKeMON enabled.
// ---------------------------------------------------------------------------
#define VMON_MAX 5
#define VMON_NONE 0xFF

struct VisibleMon
{
    u16 species;
    u8 level;
    u8 active;
    s16 x, y;      // map coords incl. MAP_OFFSET
    s8 dx, dy;     // current move direction
    u8 progress;   // 0..16 pixels moved toward (x+dx, y+dy)
    u8 timer;
    u8 spriteId;
    u8 kind;       // TILE_ENCOUNTER_LAND or TILE_ENCOUNTER_WATER
};

static EWRAM_DATA struct VisibleMon sVMons[VMON_MAX] = {0};
static EWRAM_DATA bool8 sVMonMapActive = FALSE;
static EWRAM_DATA u16 sVMonSteps = 0;
static EWRAM_DATA u8 sVMonTarget = 0;
static EWRAM_DATA u8 sVMonKinds = 0; // bit 0 = land POKeMON shown, bit 1 = water POKeMON shown

static void Task_VisibleMons(u8 taskId);

// Tiles where the map has random encounters of this kind: grass/cave floor (land) or water
static bool8 VMon_IsSpot(s16 x, s16 y, u8 kind)
{
    if (x < MAP_OFFSET || y < MAP_OFFSET
     || x >= gMapHeader.mapLayout->width + MAP_OFFSET || y >= gMapHeader.mapLayout->height + MAP_OFFSET)
        return FALSE;
    if (MapGridGetCollisionAt(x, y))
        return FALSE;
    return MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) == kind;
}

static void VMon_PlayerPos(s16 *x, s16 *y)
{
    struct ObjectEvent *p = &gObjectEvents[gPlayerAvatar.objectEventId];
    *x = p->currentCoords.x;
    *y = p->currentCoords.y;
}

static bool8 VMon_TileTaken(s16 x, s16 y, u8 except)
{
    u8 i;
    s16 px, py;
    VMon_PlayerPos(&px, &py);
    if (x == px && y == py)
        return TRUE;
    if (GetObjectEventIdByXY(x, y) != OBJECT_EVENTS_COUNT)
        return TRUE;
    for (i = 0; i < VMON_MAX; i++)
    {
        if (i == except || !sVMons[i].active)
            continue;
        if ((sVMons[i].x == x && sVMons[i].y == y)
         || (sVMons[i].progress && sVMons[i].x + sVMons[i].dx == x && sVMons[i].y + sVMons[i].dy == y))
            return TRUE;
    }
    return FALSE;
}

static const struct WildPokemonInfo *VMon_Info(u8 kind)
{
    u16 headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
        return NULL;
    return kind == TILE_ENCOUNTER_WATER ? gWildMonHeaders[headerId].waterMonsInfo : gWildMonHeaders[headerId].landMonsInfo;
}

static bool8 VMon_PickMon(u8 kind, u16 *species, u8 *level)
{
    u8 slot;
    const struct WildPokemonInfo *info = VMon_Info(kind);
    if (info == NULL)
        return FALSE;
    slot = kind == TILE_ENCOUNTER_WATER ? ChooseWildMonIndex_WaterRock() : ChooseWildMonIndex_Land();
    *species = info->wildPokemon[slot].species;
    *level = ChooseWildMonLevel(&info->wildPokemon[slot]);
    return TRUE;
}

static bool8 VMon_SpawnOne(u8 i, u8 minDist, u8 kind)
{
    u16 tries;
    s16 px, py;
    u16 w = gMapHeader.mapLayout->width, h = gMapHeader.mapLayout->height;
    VMon_PlayerPos(&px, &py);
    for (tries = 0; tries < 300; tries++)
    {
        s16 x = MAP_OFFSET + (Random() % w);
        s16 y = MAP_OFFSET + (Random() % h);
        if (!VMon_IsSpot(x, y, kind) || VMon_TileTaken(x, y, i))
            continue;
        if ((x > px ? x - px : px - x) + (y > py ? y - py : py - y) < minDist)
            continue;
        if (!VMon_PickMon(kind, &sVMons[i].species, &sVMons[i].level))
            return FALSE;
        sVMons[i].kind = kind;
        sVMons[i].x = x;
        sVMons[i].y = y;
        sVMons[i].dx = sVMons[i].dy = 0;
        sVMons[i].progress = 0;
        sVMons[i].timer = 30 + (Random() % 90);
        sVMons[i].spriteId = VMON_NONE;
        sVMons[i].active = TRUE;
        return TRUE;
    }
    return FALSE;
}

static void VMon_CreateSprite(u8 i)
{
    u8 spriteId;
    LoadMonIconPalette(sVMons[i].species);
    spriteId = CreateMonIcon(sVMons[i].species, SpriteCB_MonIcon, -64, -64, 2, 0, FALSE);
    if (spriteId == MAX_SPRITES)
    {
        sVMons[i].spriteId = VMON_NONE;
        return;
    }
    gSprites[spriteId].coordOffsetEnabled = TRUE;
    gSprites[spriteId].oam.priority = 2;
    sVMons[i].spriteId = spriteId;
}

static void VMon_UpdateSprite(u8 i)
{
    s16 sx, sy;
    struct Sprite *spr;
    if (sVMons[i].spriteId == VMON_NONE)
        return;
    spr = &gSprites[sVMons[i].spriteId];
    SetSpritePosToMapCoords(sVMons[i].x, sVMons[i].y, &sx, &sy);
    sx += 8 + sVMons[i].dx * sVMons[i].progress;
    sy += 2 + sVMons[i].dy * sVMons[i].progress;
    spr->x = sx;
    spr->y = sy;
    spr->invisible = (sx + gSpriteCoordOffsetX < -24 || sx + gSpriteCoordOffsetX > 264
                   || sy + gSpriteCoordOffsetY < -24 || sy + gSpriteCoordOffsetY > 184);
}

// Only destroy sprites that really are our icons (never the player or NPCs)
static void VMon_DestroyOne(u8 i)
{
    u8 id = sVMons[i].spriteId;
    if (id < MAX_SPRITES && gSprites[id].inUse && gSprites[id].callback == SpriteCB_MonIcon && sVMonMapActive)
        DestroyMonIcon(&gSprites[id]);
    sVMons[i].spriteId = VMON_NONE;
}

void VisibleMons_DestroySprites(void)
{
    u8 i;
    for (i = 0; i < VMON_MAX; i++)
        VMon_DestroyOne(i);
}

static void VisibleMons_StartTask(void)
{
    if (!FuncIsActiveTask(Task_VisibleMons))
        CreateTask(Task_VisibleMons, 80);
}

// Fresh map: roll new POKeMON. destroyOld = sprites from the previous map still exist.
void VisibleMons_OnNewMap(bool8 destroyOld)
{
    u8 i, n = 0, nLand = 0, nWater = 0;
    u16 grass = 0, water = 0, t;
    u16 w = gMapHeader.mapLayout->width, h = gMapHeader.mapLayout->height;

    if (destroyOld)
        VisibleMons_DestroySprites();
#ifdef ZUMO_VMON_OFF
    sVMonMapActive = FALSE; return;
#endif
    for (i = 0; i < VMON_MAX; i++)
    {
        sVMons[i].active = FALSE;
        sVMons[i].spriteId = VMON_NONE;
    }
    sVMonMapActive = FALSE;
    sVMonKinds = 0;
    sVMonSteps = 0;
    if (GetCurrentMapWildMonHeaderId() == HEADER_NONE)
        return;
    // estimate the amount of land-encounter tiles (grass, cave floor) and water on this map
    for (t = 0; t < 400; t++)
    {
        s16 sx = MAP_OFFSET + (Random() % w), sy = MAP_OFFSET + (Random() % h);
        if (VMon_IsSpot(sx, sy, TILE_ENCOUNTER_LAND))
            grass++;
        else if (VMon_IsSpot(sx, sy, TILE_ENCOUNTER_WATER))
            water++;
    }
    if (VMon_Info(TILE_ENCOUNTER_LAND) != NULL && grass)
        nLand = grass >= 60 ? 4 : grass >= 25 ? 3 : grass >= 8 ? 2 : 1;
    if (VMon_Info(TILE_ENCOUNTER_WATER) != NULL && water)
        nWater = water >= 60 ? 2 : 1;
    if (nLand == 0 && nWater == 0)
        return;
    if (nWater == 0 && nLand)
        nLand++;
    else if (nLand == 0)
        nWater = water >= 60 ? 4 : water >= 25 ? 3 : 2;
    if (nLand + nWater > VMON_MAX)
        nLand = VMON_MAX - nWater;
    sVMonMapActive = TRUE;
    sVMonKinds = (nLand ? 1 : 0) | (nWater ? 2 : 0);
    sVMonTarget = nLand + nWater;
    for (i = 0; i < sVMonTarget; i++)
    {
        sVMons[i].kind = i < nLand ? TILE_ENCOUNTER_LAND : TILE_ENCOUNTER_WATER;
        if (VMon_SpawnOne(i, 3, sVMons[i].kind))
            n++;
    }
    for (i = 0; i < VMON_MAX; i++)
        if (sVMons[i].active)
            VMon_CreateSprite(i);
    VisibleMons_StartTask();
}

// Back from a battle or a menu on the same map: rebuild sprites from saved state.
void VisibleMons_OnReturnToField(void)
{
    u8 i;
    if (!sVMonMapActive)
        return;
    for (i = 0; i < VMON_MAX; i++)
    {
        sVMons[i].spriteId = VMON_NONE;
        if (sVMons[i].active)
            VMon_CreateSprite(i);
    }
    VisibleMons_StartTask();
}

bool8 VisibleMons_SuppressRandomGrass(u32 metatileAttrs)
{
    u32 type = ExtractMetatileAttribute(metatileAttrs, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);
    if (!sVMonMapActive)
        return FALSE;
    if (type == TILE_ENCOUNTER_LAND)
        return (sVMonKinds & 1) != 0;
    if (type == TILE_ENCOUNTER_WATER)
        return (sVMonKinds & 2) != 0;
    return FALSE;
}

static void Task_VisibleMons(u8 taskId)
{
    u8 i;
    if (!sVMonMapActive)
    {
        DestroyTask(taskId);
        return;
    }
    for (i = 0; i < VMON_MAX; i++)
    {
        struct VisibleMon *m = &sVMons[i];
        if (!m->active)
            continue;
        if (m->progress)
        {
            if (++m->progress >= 16)
            {
                m->x += m->dx;
                m->y += m->dy;
                m->dx = m->dy = 0;
                m->progress = 0;
                m->timer = 40 + (Random() % 100);
            }
        }
        else if (!ArePlayerFieldControlsLocked() && m->timer-- == 0)
        {
            static const s8 sDirs[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
            u8 d = Random() % 4;
            s16 nx = m->x + sDirs[d][0], ny = m->y + sDirs[d][1];
            if (VMon_IsSpot(nx, ny, m->kind) && !VMon_TileTaken(nx, ny, i))
            {
                m->dx = sDirs[d][0];
                m->dy = sDirs[d][1];
                m->progress = 1;
            }
            else
            {
                m->timer = 20 + (Random() % 40);
            }
        }
        VMon_UpdateSprite(i);
    }
}

// Called every time the player finishes a step. Returns TRUE if a battle started.
bool8 VisibleMons_OnPlayerStep(void)
{
    u8 i;
    s16 px, py;
    if (!sVMonMapActive)
        return FALSE;
    VMon_PlayerPos(&px, &py);
    for (i = 0; i < VMON_MAX; i++)
    {
        struct VisibleMon *m = &sVMons[i];
        if (!m->active)
            continue;
        if ((m->x == px && m->y == py)
         || (m->progress && m->x + m->dx == px && m->y + m->dy == py))
        {
            VMon_DestroyOne(i);
            m->active = FALSE;
            GenerateWildMon(m->species, m->level, 0);
            StartWildBattle();
            return TRUE;
        }
    }
    // slowly respawn POKeMON that were battled
    if (++sVMonSteps >= 25)
    {
        sVMonSteps = 0;
        for (i = 0; i < sVMonTarget && i < VMON_MAX; i++)
        {
            if (!sVMons[i].active)
            {
                if (VMon_SpawnOne(i, 8, sVMons[i].kind))
                    VMon_CreateSprite(i);
                break;
            }
        }
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
// Zumo hack: a party POKeMON (icon sprite) follows the player.
// Chosen from the party menu (FOLLOW / STAY). Stored as the PID in two vars.
// ---------------------------------------------------------------------------
#define FOLLOWER_QUEUE 6
#define FOLLOWER_NONE 0xFF

static u8 sFollowerSprite; // set to FOLLOWER_NONE by Follower_OnNewMap before first use
static u16 sFollowerSpecies;
static s16 sFolH1X, sFolH1Y;  // tile the player stood on one step ago (incl. MAP_OFFSET)
static s16 sFolH2X, sFolH2Y;  // tile before that
static s16 sFolLastX, sFolLastY;
static s16 sFolCX, sFolCY;    // player sprite pixel position minus tile*16 when standing still
static u8 sFolAnim;
static u8 sFolFlip;

static void Task_Follower(u8 taskId);

static u32 Follower_GetPid(void)
{
    return (u32)VarGet(VAR_0x40F0) | ((u32)VarGet(VAR_0x40F4) << 16);
}

static u8 Follower_FindSlot(void)
{
    u8 i;
    u32 pid = Follower_GetPid();
    if (pid == 0)
        return FOLLOWER_NONE;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE)
            continue;
        if (GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY) == pid
         && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            return i;
    }
    return FOLLOWER_NONE;
}

bool8 Follower_IsSlot(u8 slot)
{
    return Follower_FindSlot() == slot;
}

static void Follower_DestroySprite(void)
{
    u8 id = sFollowerSprite;
    if (id < MAX_SPRITES && gSprites[id].inUse && gSprites[id].callback == SpriteCB_MonIcon)
        DestroyMonIcon(&gSprites[id]);
    sFollowerSprite = FOLLOWER_NONE;
}

static void Follower_SnapToPlayer(void)
{
    s16 px, py;
    s16 bx = 0, by = 0;
    u8 dir;
    struct ObjectEvent *pobj = &gObjectEvents[gPlayerAvatar.objectEventId];
    VMon_PlayerPos(&px, &py);
    sFolLastX = px;
    sFolLastY = py;
    // start on the tile behind the player (if it is free), else under him
    dir = GetOppositeDirection(pobj->facingDirection);
    bx = px;
    by = py;
    MoveCoords(dir, &bx, &by);
    if (dir == DIR_NONE || MapGridGetCollisionAt(bx, by) || GetObjectEventIdByXY(bx, by) != OBJECT_EVENTS_COUNT)
    {
        bx = px;
        by = py;
    }
    sFolH1X = sFolH2X = bx;
    sFolH1Y = sFolH2Y = by;
    sFolAnim = 0;
    sFolFlip = (pobj->facingDirection == DIR_EAST);
}

static void Follower_CreateSprite(void)
{
    u8 slot = Follower_FindSlot();
    u8 spriteId;
    if (slot == FOLLOWER_NONE)
        return;
    sFollowerSpecies = GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES);
    LoadMonIconPalette(sFollowerSpecies);
    spriteId = CreateMonIcon(sFollowerSpecies, SpriteCB_MonIcon, -64, -64, 2, GetMonData(&gPlayerParty[slot], MON_DATA_PERSONALITY), FALSE);
    if (spriteId == MAX_SPRITES)
        return;
    gSprites[spriteId].coordOffsetEnabled = TRUE;
    sFollowerSprite = spriteId;
}

static void Follower_StartTask(void)
{
    if (!FuncIsActiveTask(Task_Follower))
        CreateTask(Task_Follower, 81);
}

void Follower_Refresh(void)
{
    Follower_DestroySprite();
    if (Follower_FindSlot() == FOLLOWER_NONE)
        return;
    Follower_SnapToPlayer();
    Follower_CreateSprite();
    Follower_StartTask();
}

void Follower_SetFromPartySlot(u8 slot)
{
    u32 pid = GetMonData(&gPlayerParty[slot], MON_DATA_PERSONALITY);
    VarSet(VAR_0x40F0, pid & 0xFFFF);
    VarSet(VAR_0x40F4, pid >> 16);
    Follower_Refresh();
}

void Follower_Clear(void)
{
    VarSet(VAR_0x40F0, 0);
    VarSet(VAR_0x40F4, 0);
    Follower_DestroySprite();
}

// new map: sprites of the old map were (or will be) wiped
void Follower_OnNewMap(bool8 destroyOld)
{
    if (destroyOld)
        Follower_DestroySprite();
    sFollowerSprite = FOLLOWER_NONE;
    if (Follower_FindSlot() == FOLLOWER_NONE)
        return;
    Follower_SnapToPlayer();
    Follower_CreateSprite();
    Follower_StartTask();
}

void Follower_OnReturnToField(void)
{
    sFollowerSprite = FOLLOWER_NONE;
    if (Follower_FindSlot() == FOLLOWER_NONE)
        return;
    Follower_CreateSprite();
    Follower_StartTask();
}

static void Task_Follower(u8 taskId)
{
    s16 px, py, rx, ry, steps, fx, fy, dirX, dirY, mag, bx, by;
    struct Sprite *spr, *pspr;
    struct ObjectEvent *pobj;
    u8 slot;

    if (sFollowerSprite == FOLLOWER_NONE || Follower_FindSlot() == FOLLOWER_NONE)
    {
        DestroyTask(taskId);
        return;
    }
    slot = Follower_FindSlot();
    if (GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES) != sFollowerSpecies)
    {
        // evolved: rebuild with the new icon
        Follower_DestroySprite();
        Follower_CreateSprite();
        if (sFollowerSprite == FOLLOWER_NONE)
        {
            DestroyTask(taskId);
            return;
        }
    }
    spr = &gSprites[sFollowerSprite];
    pobj = &gObjectEvents[gPlayerAvatar.objectEventId];
    pspr = &gSprites[pobj->spriteId];
    VMon_PlayerPos(&px, &py);
    // far away (warp, fly, ...): snap back to the player
    if (abs(px - sFolLastX) > 3 || abs(py - sFolLastY) > 3)
        Follower_SnapToPlayer();
    // while the player stands still, learn where his sprite sits relative to his tile
    SetSpritePosToMapCoords(px, py, &bx, &by);
    if (gPlayerAvatar.tileTransitionState == T_NOT_MOVING)
    {
        sFolCX = pspr->x - bx;
        sFolCY = pspr->y - by;
    }
    // player entered a new tile: shift the trail
    if (px != sFolLastX || py != sFolLastY)
    {
        sFolH2X = sFolH1X;
        sFolH2Y = sFolH1Y;
        sFolH1X = sFolLastX;
        sFolH1Y = sFolLastY;
        sFolLastX = px;
        sFolLastY = py;
    }
    // how far the player's sprite still has to travel to reach his tile
    rx = pspr->x - (bx + sFolCX);
    ry = pspr->y - (by + sFolCY);
    mag = abs(rx) + abs(ry);
    steps = abs(px - sFolH1X) + abs(py - sFolH1Y);
    if (steps < 1)
        steps = 1;
    // follower covers (H2 -> H1) in lock-step with the player's (H1 -> H0)
    dirX = sFolH1X - sFolH2X;
    dirY = sFolH1Y - sFolH2Y;
    SetSpritePosToMapCoords(sFolH1X, sFolH1Y, &fx, &fy);
    if (dirX != 0 || dirY != 0)
    {
        s16 nx = dirX > 0 ? 1 : (dirX < 0 ? -1 : 0);
        s16 ny = dirY > 0 ? 1 : (dirY < 0 ? -1 : 0);
        s16 back = mag * (abs(dirX) + abs(dirY)) / steps;
        // never drift farther than the trail allows
        fx -= nx * back;
        fy -= ny * back;
    }
    // face the way the player faces (icons only have a side view: flip left/right)
    if (pobj->facingDirection == DIR_EAST)
        sFolFlip = 1;
    else if (pobj->facingDirection == DIR_WEST)
        sFolFlip = 0;
    if (mag != 0 || gPlayerAvatar.tileTransitionState != T_NOT_MOVING)
        sFolAnim++;
    else
        sFolAnim = 0;
    spr->x = fx + 8;
    spr->y = fy + 2;
    spr->x2 = 0;
    spr->y2 = (sFolAnim & 8) ? -2 : 0;
    spr->hFlip = sFolFlip;
    spr->oam.matrixNum = (spr->oam.matrixNum & ~8) | (sFolFlip ? 8 : 0);
    spr->oam.priority = pspr->oam.priority;
    spr->subpriority = pspr->subpriority + ((fy > by + ry) ? -1 : 1);
    spr->invisible = (gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_SURFING | PLAYER_AVATAR_FLAG_UNDERWATER))
                  || pspr->invisible
                  || (spr->x + gSpriteCoordOffsetX < -24 || spr->x + gSpriteCoordOffsetX > 264
                   || spr->y + gSpriteCoordOffsetY < -24 || spr->y + gSpriteCoordOffsetY > 184);
}
