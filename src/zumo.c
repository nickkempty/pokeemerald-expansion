// Zumo Edition systems ported from the pokefirered Ver 4.2 hack (src/field_specials.c there).
#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_specials.h"
#include "fieldmap.h"
#include "item.h"
#include "list_menu.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "random.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/vars.h"
#include "zumo.h"
#include "battle.h"
#include "daycare.h"
#include "pokemon_storage_system.h"
#include "party_menu.h"
#include "constants/moves.h"

u8 gZumoRocketStage;

static const u16 sNineStarters[3][3] = {
    {SPECIES_BULBASAUR,  SPECIES_CHIKORITA, SPECIES_TREECKO},
    {SPECIES_SQUIRTLE,   SPECIES_TOTODILE,  SPECIES_MUDKIP},
    {SPECIES_CHARMANDER, SPECIES_CYNDAQUIL, SPECIES_TORCHIC},
};

static u16 NineStarterSpecies(u16 type, u16 gen)
{
    if (type > 2)
        type = 0;
    if (gen > 2)
        gen = 0;
    return sNineStarters[type][gen];
}

u16 GetStarterSpeciesByTypeAndGen(void)
{
    return NineStarterSpecies(gSpecialVar_0x8004, gSpecialVar_0x8005);
}

u16 GetPlayerStarterSpecies(void)
{
    return NineStarterSpecies(VarGet(VAR_STARTER_MON), VarGet(VAR_PLAYER_STARTER_GEN));
}

u16 PickRandomRivalStarter(void)
{
    // rival variant (VAR_STARTER_MON semantics): 0 -> rival fire, 1 -> rival grass, 2 -> rival water
    static const u8 sVariantForType[3] = {1, 2, 0};
    u16 r = Random() % 9;
    u16 type = r / 3;
    u16 gen = r % 3;
    VarSet(VAR_RIVAL_VARIANT, sVariantForType[type]);
    VarSet(VAR_RIVAL_STARTER_GEN, gen);
    return NineStarterSpecies(type, gen);
}


// Zumo hack: count caught POKeMON from JOHTO/HOENN (national dex 152-386)
u16 CountCaughtMigrantMons(void)
{
    u16 i, count = 0;
    for (i = 152; i <= 386; i++)
    {
        if (GetSetPokedexFlag(i, FLAG_GET_CAUGHT))
            count++;
    }
    return count;
}

// Zumo hack: MEW sighting helpers
#define LOCALID_MEW_SIGHTING 0x60
void SpawnMewSighting(void)
{
    s16 x = gSaveBlock1Ptr->pos.x + MAP_OFFSET - 6;
    s16 y = gSaveBlock1Ptr->pos.y + MAP_OFFSET - 2;
    SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_MEW, MOVEMENT_TYPE_FACE_RIGHT, LOCALID_MEW_SIGHTING, x, y, 3);
}

void CreateMewSightingBattle(void)
{
    u8 badges = Zumo_CountBadges();
    u16 level = 12 + badges * 6;
    if (level > 100)
        level = 100;
    CreateScriptedWildMon(SPECIES_MEW, level, ITEM_NONE);
}

// Zumo hack: mark the starter from PROF. OAK as a selected POKeMON (+50% stats)
void MarkStarterFromOak(void)
{
    u8 i, last = CalculatePlayerPartyCount();
    u8 mark = 1;
    if (last == 0)
        return;
    i = last - 1;
    SetMonData(&gPlayerParty[i], MON_DATA_ZUMO_STARTER, &mark);
    CalculateMonStats(&gPlayerParty[i]);
}

// Zumo hack: POKeDEX data seller - scroll list of unseen POKeMON
static struct ListMenuItem *sDexSellerItems;
static u16 *sDexSellerSpecies;

static void Task_DexSellerInput(u8 taskId);

void ShowDexSellerList(void)
{
    struct ListMenuTemplate tmpl;
    struct WindowTemplate win;
    u16 n, count = 0;
    u8 taskId, windowId;

    sDexSellerItems = AllocZeroed(NATIONAL_DEX_COUNT * sizeof(struct ListMenuItem));
    sDexSellerSpecies = AllocZeroed(NATIONAL_DEX_COUNT * sizeof(u16));
    for (n = 1; n <= NATIONAL_DEX_COUNT; n++)
    {
        if (!GetSetPokedexFlag(n, FLAG_GET_SEEN))
        {
            u16 species = NationalPokedexNumToSpecies(n);
            sDexSellerSpecies[count] = species;
            sDexSellerItems[count].name = GetSpeciesName(species);
            sDexSellerItems[count].id = count;
            count++;
        }
    }
    if (count == 0)
    {
        Free(sDexSellerItems);
        Free(sDexSellerSpecies);
        gSpecialVar_Result = 0x7F;
        gSpecialVar_0x8005 = 0;
        return;
    }
    LockPlayerFieldControls();
    SetWindowTemplateFields(&win, 0, 19, 1, 10, 12, 15, 0x038);
    windowId = AddWindow(&win);
    SetStandardWindowBorderStyle(windowId, FALSE);
    tmpl.items = sDexSellerItems;
    tmpl.moveCursorFunc = ListMenuDefaultCursorMoveFunc;
    tmpl.itemPrintFunc = NULL;
    tmpl.totalItems = count;
    tmpl.maxShowed = 6;
    tmpl.windowId = windowId;
    tmpl.header_X = 0;
    tmpl.item_X = 8;
    tmpl.cursor_X = 0;
    tmpl.upText_Y = 0;
    tmpl.cursorPal = 2;
    tmpl.fillValue = 1;
    tmpl.cursorShadowPal = 3;
    tmpl.lettersSpacing = 1;
    tmpl.itemVerticalPadding = 0;
    tmpl.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
    tmpl.fontId = FONT_NORMAL;
    tmpl.cursorKind = 0;
    taskId = CreateTask(Task_DexSellerInput, 8);
    gTasks[taskId].data[0] = ListMenuInit(&tmpl, 0, 0);
    gTasks[taskId].data[1] = windowId;
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void Task_DexSellerInput(u8 taskId)
{
    s32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);
    u8 windowId = gTasks[taskId].data[1];

    if (input == LIST_NOTHING_CHOSEN)
        return;
    PlaySE(SE_SELECT);
    if (input == LIST_CANCEL)
    {
        gSpecialVar_Result = 0x7F;
        gSpecialVar_0x8005 = 0;
    }
    else
    {
        gSpecialVar_Result = 0;
        gSpecialVar_0x8005 = sDexSellerSpecies[input];
    }
    DestroyListMenuTask(gTasks[taskId].data[0], NULL, NULL);
    ClearStdWindowAndFrameToTransparent(windowId, TRUE);
    RemoveWindow(windowId);
    Free(sDexSellerItems);
    Free(sDexSellerSpecies);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

void SetDexSeenFromSeller(void)
{
    u16 n = SpeciesToNationalPokedexNum(gSpecialVar_0x8005);
    GetSetPokedexFlag(n, FLAG_SET_SEEN);
}

// Zumo hack: QUEST menu text (generated)
EWRAM_DATA u8 gZumoQuestText[1000] = {0};
static const u8 sQuestMain0[] = _("NEXT: Go to PROF. OAK's LAB in\nPALLET TOWN and choose your first\lPOKéMON.");
static const u8 sQuestMain1[] = _("NEXT: Pick up OAK's PARCEL at the\nVIRIDIAN CITY POKéMON MART, then\lbring it to PROF. OAK.");
static const u8 sQuestMain2[] = _("NEXT: Go through VIRIDIAN FOREST\nto PEWTER CITY and beat BROCK at\lthe GYM.");
static const u8 sQuestMain3[] = _("NEXT: Cross MT. MOON. TEAM MAGMA\nis digging there! Then beat MISTY\lat the CERULEAN CITY GYM.");
static const u8 sQuestMain4[] = _("NEXT: Visit BILL at the SEA\nCOTTAGE north of CERULEAN CITY.\lHe knows about the strange sea.");
static const u8 sQuestMain5[] = _("NEXT: Follow TEAM MAGMA east to\nROUTE 9 and ROCK TUNNEL, then go\lwest from LAVENDER to CELADON.\pBeat ERIKA at the CELADON GYM.");
static const u8 sQuestMain6[] = _("NEXT: TEAM ECLIPSE hides a lab\nunder the CELADON GAME CORNER.\lFind out what NOX is building!");
static const u8 sQuestMain7[] = _("NEXT: TEAM ECLIPSE holds SILPH CO.\nin SAFFRON. Ride the ELEVATOR:\lCARD KEY 5F, officers 7F, NOX 11F.");
static const u8 sQuestMain7b[] = _("NEXT: SILPH CO. is free! Beat\nSABRINA at the SAFFRON CITY GYM.");
static const u8 sQuestMain8[] = _("NEXT: MAGMA stole the RED ORB on\nthe S.S. ANNE in VERMILION CITY.\lChase the thieves up to the deck!");
static const u8 sQuestMain8b[] = _("NEXT: LT. SURGE is back at the\nVERMILION GYM. Beat him!");
static const u8 sQuestMain9[] = _("NEXT: Go south to FUCHSIA CITY\n(wake SNORLAX with the POKé FLUTE)\land beat KOGA.");
static const u8 sQuestMain11[] = _("NEXT: Surf to CINNABAR ISLAND.\nMAXIE waits in the old mansion with\lthe SECRET KEY. Then beat BLAINE.");
static const u8 sQuestMain12[] = _("NEXT: Return to VIRIDIAN CITY.\nM-CORE PHASE 3 runs under the GYM.\lThe last GYM LEADER is NOX!");
static const u8 sQuestMain13[] = _("NEXT: You have all 16 BADGES! Go\nthrough VICTORY ROAD in KANTO to the\lUNION LEAGUE and become CHAMPION!");
static const u8 sQuestMain14[] = _("NEXT: BILL waits at CINNABAR. Sail\nto the SEVII ISLANDS: NOX fled to\lECLIPSE's last base on FIVE ISLAND!");
static const u8 sQuestMain15[] = _("NEXT: Show the MYSTIC TICKET at\nthe VERMILION CITY harbor and meet\lLUGIA at NAVEL ROCK.");
static const u8 sQuestMain16[] = _("The seas are calm again. You've\nfinished ZUMO EDITION's story! Try\lto complete the POKéDEX!");
static const u8 sQuestMain14old[] = _("NEXT: Sail to the SEVII ISLANDS.\nTEAM ECLIPSE's last plan is hidden\lsomewhere out at sea.");
static const u8 sQuestHoenn0[] = _("NEXT: Walk the UNION ROAD south of\nCINNABAR to SLATEPORT, then go\lnorth to MAUVILLE. Beat WATTSON.");
static const u8 sQuestHoenn1[] = _("NEXT: Go west of MAUVILLE through\nVERDANTURF and RUSTURF TUNNEL to\lRUSTBORO. Beat ROXANNE.");
static const u8 sQuestHoenn2[] = _("NEXT: Visit MR. BRINEY on ROUTE 104\nand sail to DEWFORD. Beat BRAWLY.");
static const u8 sQuestHoenn3[] = _("NEXT: Sail to SLATEPORT. Go north\npast MAUVILLE and MT. CHIMNEY to\lLAVARIDGE. Beat FLANNERY.");
static const u8 sQuestHoenn4[] = _("NEXT: Go back to RUSTBORO and take\nROUTE 104 east to PETALBURG.\lBeat NORMAN.");
static const u8 sQuestHoenn5[] = _("NEXT: Go east of MAUVILLE, then\nnorth on ROUTE 119 past the\lWEATHER INSTITUTE to FORTREE.\pBeat WINONA.");
static const u8 sQuestHoenn6[] = _("NEXT: Go east of FORTREE through\nROUTE 120/121 to LILYCOVE, then SURF\lROUTE 124 to MOSSDEEP. Beat TATE&LIZA.");
static const u8 sQuestHoenn7[] = _("NEXT: SURF south to ROUTE 126 and\nswim through the cave under the\lcrater to SOOTOPOLIS CITY.\pStop NOX in the CAVE OF ORIGIN!");
static const u8 sQuestHoenn8[] = _("NEXT: GROUDON is free! Beat JUAN\nat the SOOTOPOLIS CITY GYM for\lyour 16th BADGE.");
static const u8 sQuestMigrant[] = _("SIDE: Catch POKéMON from JOHTO or\nHOENN and show the AIDE in PEWTER\lCITY (caught: {STR_VAR_1}/3).");
static const u8 sQuestMewtwo[] = _("SIDE: A powerful POKéMON is hiding\ndeep in ROCK TUNNEL B1F.");
static const u8 sQuestMew[] = _("SIDE: A pink POKéMON keeps\nappearing in towns. Only a MASTER\lBALL can catch it!");
static const u8 sQuestBreak[] = _("\p");

void BufferQuestText(void)
{
    const u8 *mainText = NULL;
    u8 *p = gZumoQuestText;
    if (!FlagGet(FLAG_SYS_POKEMON_GET))
        mainText = sQuestMain0;
    else if (!FlagGet(FLAG_SYS_POKEDEX_GET))
        mainText = sQuestMain1;
    else if (!FlagGet(FLAG_BADGE01_GET))
        mainText = sQuestMain2;
    else if (!FlagGet(FLAG_BADGE02_GET))
        mainText = sQuestMain3;
    else if (!FlagGet(FLAG_GOT_SS_TICKET))
        mainText = sQuestMain4;
    else if (!FlagGet(FLAG_BADGE04_GET))
        mainText = sQuestMain5;
    else if (!FlagGet(FLAG_ZK_READ_MCORE_LOG))
        mainText = sQuestMain6;
    else if (VarGet(VAR_MAP_SCENE_SILPH_CO_11F) == 0)
        mainText = sQuestMain7;
    else if (!FlagGet(FLAG_BADGE06_GET))
        mainText = sQuestMain7b;
    else if (!FlagGet(FLAG_ZK_SSANNE_DONE))
        mainText = sQuestMain8;
    else if (!FlagGet(FLAG_BADGE03_GET))
        mainText = sQuestMain8b;
    else if (!FlagGet(FLAG_BADGE05_GET))
        mainText = sQuestMain9;
    else if (!FlagGet(FLAG_BADGE07_GET))
        mainText = sQuestMain11;
    else if (!FlagGet(FLAG_BADGE08_GET))
        mainText = sQuestMain12;
    else if (!FlagGet(FLAG_HX_BADGE03_GET))
        mainText = sQuestHoenn0;
    else if (!FlagGet(FLAG_HX_BADGE01_GET))
        mainText = sQuestHoenn1;
    else if (!FlagGet(FLAG_HX_BADGE02_GET))
        mainText = sQuestHoenn2;
    else if (!FlagGet(FLAG_HX_BADGE04_GET))
        mainText = sQuestHoenn3;
    else if (!FlagGet(FLAG_HX_BADGE05_GET))
        mainText = sQuestHoenn4;
    else if (!FlagGet(FLAG_HX_BADGE06_GET))
        mainText = sQuestHoenn5;
    else if (!FlagGet(FLAG_HX_BADGE07_GET))
        mainText = sQuestHoenn6;
    else if (!FlagGet(FLAG_HX_NOX_FINAL_DONE))
        mainText = sQuestHoenn7;
    else if (!FlagGet(FLAG_HX_BADGE08_GET))
        mainText = sQuestHoenn8;
    else if (!FlagGet(FLAG_DEFEATED_CHAMP))
        mainText = sQuestMain13;
    else if (!FlagGet(FLAG_DEFEATED_NOX_FINAL))
        mainText = sQuestMain14;
    else if (!FlagGet(FLAG_FOUGHT_LUGIA) && !FlagGet(FLAG_LUGIA_FLEW_AWAY))
        mainText = sQuestMain15;
    else
        mainText = sQuestMain16;
    p = StringCopy(p, mainText);
    if (FlagGet(FLAG_SYS_POKEDEX_GET) && !FlagGet(FLAG_MIGRANT_QUEST_DONE))
    {
        u16 n = CountCaughtMigrantMons();
        if (n > 3)
            n = 3;
        ConvertIntToDecimalStringN(gStringVar1, n, STR_CONV_MODE_LEFT_ALIGN, 1);
        p = StringCopy(p, sQuestBreak);
        p = StringExpandPlaceholders(p, sQuestMigrant);
    }
    if (FlagGet(FLAG_ZK_READ_MCORE_LOG) && !FlagGet(FLAG_EARLY_MEWTWO_DONE))
    {
        p = StringCopy(p, sQuestBreak);
        p = StringCopy(p, sQuestMewtwo);
    }
    if (FlagGet(FLAG_SYS_POKEDEX_GET) && !FlagGet(FLAG_MEW_CAUGHT))
    {
        p = StringCopy(p, sQuestBreak);
        p = StringCopy(p, sQuestMew);
    }
}

// Zumo hack: remove the spawned MEW object without touching any flag
void RemoveMewSightingObject(void)
{
    u8 objectEventId;
    if (!TryGetObjectEventIdByLocalIdAndMap(LOCALID_MEW_SIGHTING, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId))
        RemoveObjectEvent(&gObjectEvents[objectEventId]);
}

// ---- Zumo: TEAM ROCKET trio (MUSASHI / KOJIRO / NYARTH) ambush, every 2 badges ----
#define LOCALID_ROCKET_TRIO_A 0x61
#define LOCALID_ROCKET_TRIO_B 0x62
#define LOCALID_ROCKET_TRIO_C 0x63
bool8 MapGridGetCollisionAt(s32, s32);

static bool8 RocketTrioTileFree(s16 x, s16 y)
{
    if (MapGridGetCollisionAt(x, y))
        return FALSE;
    if (GetObjectEventIdByXY(x, y) != OBJECT_EVENTS_COUNT)
        return FALSE;
    return TRUE;
}

void SpawnRocketTrio(void)
{
    static const s8 sDirs[4][2] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};
    static const u8 sFaceTowardPlayer[4] = {MOVEMENT_TYPE_FACE_LEFT, MOVEMENT_TYPE_FACE_RIGHT, MOVEMENT_TYPE_FACE_DOWN, MOVEMENT_TYPE_FACE_UP};
    s16 px = gSaveBlock1Ptr->pos.x + MAP_OFFSET;
    s16 py = gSaveBlock1Ptr->pos.y + MAP_OFFSET;
    u8 d, k, pick = 0;
    for (d = 0; d < 4; d++)
    {
        bool8 ok = TRUE;
        for (k = 2; k <= 4; k++)
            if (!RocketTrioTileFree(px + sDirs[d][0] * k, py + sDirs[d][1] * k))
                ok = FALSE;
        if (ok)
        {
            pick = d;
            break;
        }
    }
    SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_ROCKET_F, sFaceTowardPlayer[pick], LOCALID_ROCKET_TRIO_A, px + sDirs[pick][0] * 2, py + sDirs[pick][1] * 2, 3);
    SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_ROCKET_M, sFaceTowardPlayer[pick], LOCALID_ROCKET_TRIO_B, px + sDirs[pick][0] * 3, py + sDirs[pick][1] * 3, 3);
    SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_MEOWTH, sFaceTowardPlayer[pick], LOCALID_ROCKET_TRIO_C, px + sDirs[pick][0] * 4, py + sDirs[pick][1] * 4, 3);
    // mark this milestone as done right away so it can never repeat
    FlagSet(FLAG_0x8F0 + gZumoRocketStage - 1);
}

void GetRocketTrioStage(void)
{
    gSpecialVar_Result = gZumoRocketStage;
}

void RemoveRocketTrio(void)
{
    u8 objectEventId, i;
    for (i = 0; i < 3; i++)
        if (!TryGetObjectEventIdByLocalIdAndMap(LOCALID_ROCKET_TRIO_A + i, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId))
            RemoveObjectEvent(&gObjectEvents[objectEventId]);
}

// ---- Zumo Edition: ported Emerald Mauville gym puzzle ----

// Zumo Edition: raise the UNION ROAD causeway south of Cinnabar Island.
// Writes whole metatile entries (id | collision << 10 | elevation << 12) because
// setmetatile keeps the old elevation, and the sea tiles sit at elevation 1.
#define HX_ENT(id, col, elev) ((id) | ((col) << 10) | ((elev) << 12))

void HxRaiseCinnabarCauseway(void)
{
    s32 y, x;

    for (y = 15; y <= 19; y++)
    {
        if (y == 15)
        {
            MapGridSetMetatileEntryAt(11 + MAP_OFFSET, y + MAP_OFFSET, HX_ENT(289, 1, 3));
            MapGridSetMetatileEntryAt(16 + MAP_OFFSET, y + MAP_OFFSET, HX_ENT(290, 1, 3));
        }
        else
        {
            MapGridSetMetatileEntryAt(10 + MAP_OFFSET, y + MAP_OFFSET, HX_ENT(299, 0, 1));
            MapGridSetMetatileEntryAt(17 + MAP_OFFSET, y + MAP_OFFSET, HX_ENT(299, 0, 1));
            MapGridSetMetatileEntryAt(11 + MAP_OFFSET, y + MAP_OFFSET, HX_ENT(300, 1, 3));
            MapGridSetMetatileEntryAt(16 + MAP_OFFSET, y + MAP_OFFSET, HX_ENT(298, 1, 3));
        }
        for (x = 12; x <= 15; x++)
            MapGridSetMetatileEntryAt(x + MAP_OFFSET, y + MAP_OFFSET, HX_ENT(217, 0, 3));
    }
}

#undef HX_ENT

// Zumo Edition: flip a POKeMON's gender (GENDER SWAP item). Nature and shininess are kept by
// UpdateMonPersonality. Returns FALSE for single-gender / genderless species and eggs.
bool8 Zumo_TryChangeGender(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES);
    u32 pid = GetMonData(mon, MON_DATA_PERSONALITY);
    u8 oldGender, ratio;
    u32 b;

    if (species == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;
    ratio = gSpeciesInfo[species].genderRatio;
    if (ratio == MON_MALE || ratio == MON_FEMALE || ratio == MON_GENDERLESS)
        return FALSE;
    oldGender = GetGenderFromSpeciesAndPersonality(species, pid);
    for (b = 0; b < 256; b++)
    {
        u32 newPid = (pid & 0xFFFFFF00) | b;
        if (GetGenderFromSpeciesAndPersonality(species, newPid) != oldGender)
        {
            UpdateMonPersonality(&mon->box, newPid);
            CalculateMonStats(mon);
            return TRUE;
        }
    }
    return FALSE;
}

// ---- Zumo Edition: NATURE changer in every POKeMON Center (5,000 per try) ----
// gSpecialVar_0x8004 = party slot. Picks a random nature different from the current one.
// VAR_RESULT: 0 = changed (STR_VAR_1 nickname, STR_VAR_2 nature), 1 = egg/invalid
void Zumo_RandomizeNature(void)
{
    struct Pokemon *mon;
    u32 oldNature, newNature;

    if (gSpecialVar_0x8004 >= PARTY_SIZE)
    {
        gSpecialVar_Result = 1;
        return;
    }
    mon = &gPlayerParty[gSpecialVar_0x8004];
    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
    {
        gSpecialVar_Result = 1;
        return;
    }
    oldNature = GetMonData(mon, MON_DATA_HIDDEN_NATURE);
    do
    {
        newNature = Random() % NUM_NATURES;
    } while (newNature == oldNature);
    SetMonData(mon, MON_DATA_HIDDEN_NATURE, &newNature);
    CalculateMonStats(mon);
    GetMonNickname(mon, gStringVar1);
    StringCopy(gStringVar2, gNaturesInfo[newNature].name);
    gSpecialVar_Result = 0;
}

// ---- Zumo Edition: POKeMON Center day care ----
// One POKeMON can be left at any POKeMON Center and picked up at any other one.
// Every time the party earns battle EXP, the day care POKeMON gets half of it.
STATIC_ASSERT(sizeof(struct BoxPokemon) <= sizeof(((struct SaveBlock3 *)0)->zumoDaycareMon), ZumoDaycareMonFits);
#define sDaycareMon (*(struct BoxPokemon *)gSaveBlock3Ptr->zumoDaycareMon)

void Zumo_DaycareGainBattleExp(u32 exp)
{
    u32 total;

    if (!GetBoxMonData(&sDaycareMon, MON_DATA_SPECIES))
        return;
    total = gSaveBlock3Ptr->zumoDaycareExp + exp / 2;
    if (total > 2000000)
        total = 2000000;
    gSaveBlock3Ptr->zumoDaycareExp = total;
}

// VAR_RESULT = TRUE if a POKeMON is at the day care; buffers its nickname in STR_VAR_1
void Zumo_DaycareHasMon(void)
{
    gSpecialVar_Result = GetBoxMonData(&sDaycareMon, MON_DATA_SPECIES) != SPECIES_NONE;
    if (gSpecialVar_Result)
        GetBoxMonNickname(&sDaycareMon, gStringVar1);
}

// gSpecialVar_0x8004 = party slot. VAR_RESULT: 0 ok, 1 egg/invalid, 2 it's your last usable POKeMON
void Zumo_DaycareDeposit(void)
{
    struct Pokemon *mon;
    u32 i, others = 0;

    if (gSpecialVar_0x8004 >= PARTY_SIZE)
    {
        gSpecialVar_Result = 1;
        return;
    }
    mon = &gPlayerParty[gSpecialVar_0x8004];
    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
    {
        gSpecialVar_Result = 1;
        return;
    }
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (i != gSpecialVar_0x8004 && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) != SPECIES_NONE
         && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG) && GetMonData(&gPlayerParty[i], MON_DATA_HP) != 0)
            others++;
    }
    if (others == 0)
    {
        gSpecialVar_Result = 2;
        return;
    }
    GetMonNickname(mon, gStringVar1);
    sDaycareMon = mon->box;
    gSaveBlock3Ptr->zumoDaycareExp = 0;
    ZeroMonData(mon);
    CompactPartySlots();
    CalculatePlayerPartyCount();
    UpdateFollowingPokemon();
    gSpecialVar_Result = 0;
}

static u32 Zumo_DaycareNewExp(struct Pokemon *mon)
{
    u32 exp = GetMonData(mon, MON_DATA_EXP) + gSaveBlock3Ptr->zumoDaycareExp;
    u32 maxExp = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][MAX_LEVEL];

    return exp > maxExp ? maxExp : exp;
}

// buffers STR_VAR_1 nickname, STR_VAR_2 levels gained so far
void Zumo_DaycareBufferInfo(void)
{
    struct Pokemon mon;
    u32 oldLevel, newLevel, exp;

    BoxMonToMon(&sDaycareMon, &mon);
    GetMonNickname(&mon, gStringVar1);
    oldLevel = GetMonData(&mon, MON_DATA_LEVEL);
    exp = Zumo_DaycareNewExp(&mon);
    SetMonData(&mon, MON_DATA_EXP, &exp);
    newLevel = GetLevelFromMonExp(&mon);
    ConvertIntToDecimalStringN(gStringVar2, newLevel - oldLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
    gSpecialVar_Result = newLevel - oldLevel;
}

// VAR_RESULT: 0 ok (back in the party), 1 party is full
void Zumo_DaycareWithdraw(void)
{
    struct Pokemon mon;
    u32 i, exp;
    enum Move learnedMove;
    bool8 firstMove;

    if (gPlayerPartyCount >= PARTY_SIZE)
    {
        gSpecialVar_Result = 1;
        return;
    }
    BoxMonToMon(&sDaycareMon, &mon);
    exp = Zumo_DaycareNewExp(&mon);
    SetMonData(&mon, MON_DATA_EXP, &exp);
    for (i = 0; i < MAX_LEVEL; i++)
    {
        if (!TryIncrementMonLevel(&mon))
            break;
        firstMove = TRUE;
        while ((learnedMove = MonTryLearningNewMove(&mon, firstMove)) != MOVE_NONE)
        {
            firstMove = FALSE;
            if (learnedMove == MON_HAS_MAX_MOVES)
                DeleteFirstMoveAndGiveMoveToMon(&mon, gMoveToLearn);
        }
    }
    CalculateMonStats(&mon);
    GetMonNickname(&mon, gStringVar1);
    gPlayerParty[gPlayerPartyCount] = mon;
    ZeroBoxMonData(&sDaycareMon);
    gSaveBlock3Ptr->zumoDaycareExp = 0;
    CalculatePlayerPartyCount();
    UpdateFollowingPokemon();
    gSpecialVar_Result = 0;
}
#undef sDaycareMon
