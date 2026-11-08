#include "global.h"
#include "gflib.h"
#include "berry.h"
#include "daycare.h"
#include "event_data.h"
#include "load_save.h"
#include "money.h"
#include "random.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokedex.h"
#include "script_pokemon_util.h"
#include "data.h"
#include "item_menu_icons.h"
#include "menu.h"
#include "trig.h"
#include "trainer_pokemon_sprites.h"
#include "pokemon_storage_system.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/vars.h"
#include "constants/pokemon.h"

static void CB2_ReturnFromChooseHalfParty(void);
static void CB2_ReturnFromChooseBattleTowerParty(void);

void HealPlayerParty(void)
{
    u8 i, j;
    u8 ppBonuses;
    u8 arg[4];

    // restore HP.
    for(i = 0; i < gPlayerPartyCount; i++)
    {
        u16 maxHP = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
        arg[0] = maxHP;
        arg[1] = maxHP >> 8;
        SetMonData(&gPlayerParty[i], MON_DATA_HP, arg);
        ppBonuses = GetMonData(&gPlayerParty[i], MON_DATA_PP_BONUSES);

        // restore PP.
        for(j = 0; j < MAX_MON_MOVES; j++)
        {
            arg[0] = CalculatePPWithBonus(GetMonData(&gPlayerParty[i], MON_DATA_MOVE1 + j), ppBonuses, j);
            SetMonData(&gPlayerParty[i], MON_DATA_PP1 + j, arg);
        }

        // since status is u32, the four 0 assignments here are probably for safety to prevent undefined data from reaching SetMonData.
        arg[0] = 0;
        arg[1] = 0;
        arg[2] = 0;
        arg[3] = 0;
        SetMonData(&gPlayerParty[i], MON_DATA_STATUS, arg);
    }
}

u8 ScriptGiveMon(u16 species, u8 level, u16 item, u32 unused1, u32 unused2, u8 unused3)
{
    u16 nationalDexNum;
    int sentToPc;
    u8 heldItem[2];
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));

    CreateMon(mon, species, level, 32, 0, 0, OT_ID_PLAYER_ID, 0);
    heldItem[0] = item;
    heldItem[1] = item >> 8;
    SetMonData(mon, MON_DATA_HELD_ITEM, heldItem);
    sentToPc = GiveMonToPlayer(mon);
    nationalDexNum = SpeciesToNationalPokedexNum(species);

    switch(sentToPc)
    {
    case MON_GIVEN_TO_PARTY:
    case MON_GIVEN_TO_PC:
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
        break;
    }

    Free(mon);
    return sentToPc;
}

u8 ScriptGiveEgg(u16 species)
{
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));
    bool8 isEgg;
    bool8 sentToPc;

    CreateEgg(mon, species, TRUE);
    isEgg = TRUE;
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);

    sentToPc = GiveMonToPlayer(mon);
    Free(mon);
    return sentToPc;
}

void HasEnoughMonsForDoubleBattle(void)
{
    switch (GetMonsStateToDoubles())
    {
    case PLAYER_HAS_TWO_USABLE_MONS:
        gSpecialVar_Result = PLAYER_HAS_TWO_USABLE_MONS;
        break;
    case PLAYER_HAS_ONE_MON:
        gSpecialVar_Result = PLAYER_HAS_ONE_MON;
        break;
    case PLAYER_HAS_ONE_USABLE_MON:
        gSpecialVar_Result = PLAYER_HAS_ONE_USABLE_MON;
        break;
    }
}

static bool8 CheckPartyMonHasHeldItem(u16 item)
{
    int i;

    for(i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        if (species != SPECIES_NONE && species != SPECIES_EGG && GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM) == item)
            return TRUE;
    }
    return FALSE;
}

bool8 DoesPartyHaveEnigmaBerry(void)
{
    bool8 hasItem = CheckPartyMonHasHeldItem(ITEM_ENIGMA_BERRY);
    if (hasItem == TRUE)
        GetBerryNameByBerryType(ItemIdToBerryType(ITEM_ENIGMA_BERRY), gStringVar1);

    return hasItem;
}

void CreateScriptedWildMon(u16 species, u8 level, u16 item)
{
    u8 heldItem[2];

    ZeroEnemyPartyMons();
    CreateMon(&gEnemyParty[0], species, level, 32, 0, 0, OT_ID_PLAYER_ID, 0);
    if (item)
    {
        heldItem[0] = item;
        heldItem[1] = item >> 8;
        SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, heldItem);
    }
}

void ScriptSetMonMoveSlot(u8 monIndex, u16 move, u8 slot)
{
    if (monIndex > PARTY_SIZE)
        monIndex = gPlayerPartyCount - 1;

    SetMonMoveSlot(&gPlayerParty[monIndex], move, slot);
}

// Note: When control returns to the event script, gSpecialVar_Result will be
// TRUE if the party selection was successful.
void ChooseHalfPartyForBattle(void)
{
    gMain.savedCallback = CB2_ReturnFromChooseHalfParty;
//    VarSet(VAR_FRONTIER_FACILITY, FACILITY_MULTI_OR_EREADER);
    InitChooseMonsForBattle(CHOOSE_MONS_FOR_CABLE_CLUB_BATTLE);
}

static void CB2_ReturnFromChooseHalfParty(void)
{
    switch (gSelectedOrderFromParty[0])
    {
    case 0:
        gSpecialVar_Result = FALSE;
        break;
    default:
        gSpecialVar_Result = TRUE;
        break;
    }

    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void ChooseBattleTowerPlayerParty(void)
{
    gMain.savedCallback = CB2_ReturnFromChooseBattleTowerParty;
    InitChooseMonsForBattle(CHOOSE_MONS_FOR_BATTLE_TOWER);
}

static void CB2_ReturnFromChooseBattleTowerParty(void)
{
    switch (gSelectedOrderFromParty[0])
    {
    case 0:
        LoadPlayerParty();
        gSpecialVar_Result = FALSE;
        break;
    default:
        ReducePlayerPartyToThree();
        gSpecialVar_Result = TRUE;
        break;
    }

    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void ReducePlayerPartyToThree(void)
{
    struct Pokemon * party = AllocZeroed(3 * sizeof(struct Pokemon));
    int i;

    // copy the selected pokemon according to the order.
    for (i = 0; i < 3; i++)
        if (gSelectedOrderFromParty[i]) // as long as the order keeps going (did the player select 1 mon? 2? 3?), do not stop
            party[i] = gPlayerParty[gSelectedOrderFromParty[i] - 1]; // index is 0 based, not literal

    CpuFill32(0, gPlayerParty, sizeof gPlayerParty);

    // overwrite the first 3 with the order copied to.
    for (i = 0; i < 3; i++)
        gPlayerParty[i] = party[i];

    CalculatePlayerPartyCount();
    Free(party);
}

// ---- Zumo: POKéMON Center gacha clerk ----
// One pull per GYM badge is free; each further pull costs 5,000 more than the last.
// The counter resets whenever the player earns a new badge.
// "x3" opens the next three pulls at once and guarantees one strong POKéMON.
#define ZUMO_GACHA_PULLS VAR_0x40F5
#define ZUMO_GACHA_BADGES VAR_0x40F6
#define ZUMO_GACHA_STEP 5000
#define ZG_MAX 3

struct ZumoGachaRes
{
    u8 count;
    u8 anyStrong;
    u8 sentToPc;
    u16 species[ZG_MAX];
    u32 pid[ZG_MAX];
    u32 otId;
    u8 strong[ZG_MAX];
    u8 tier[ZG_MAX];
    u16 ivSum[ZG_MAX];
};

static struct ZumoGachaRes sZG;

static const u16 sHxBadgeFlags[] = {
    FLAG_HX_BADGE01_GET, FLAG_HX_BADGE02_GET, FLAG_HX_BADGE03_GET, FLAG_HX_BADGE04_GET,
    FLAG_HX_BADGE05_GET, FLAG_HX_BADGE06_GET, FLAG_HX_BADGE07_GET, FLAG_HX_BADGE08_GET,
};

// counts the 8 KANTO badges plus the 8 HOENN badges (0-16)
u8 Zumo_CountBadges(void)
{
    u8 i, n = 0;

    for (i = 0; i < NUM_BADGES; i++)
    {
        if (FlagGet(FLAG_BADGE01_GET + i))
            n++;
    }
    for (i = 0; i < ARRAY_COUNT(sHxBadgeFlags); i++)
    {
        if (FlagGet(sHxBadgeFlags[i]))
            n++;
    }
    return n;
}

static bool8 Zumo_GachaIsLegendary(u16 dex)
{
    return (dex == 144 || dex == 145 || dex == 146 || dex == 150 || dex == 151
         || (dex >= 243 && dex <= 245) || (dex >= 249 && dex <= 251)
         || (dex >= 377 && dex <= 386));
}

static u16 Zumo_GachaBaseStatTotal(u16 species)
{
    const struct SpeciesInfo *b = &gSpeciesInfo[species];

    return b->baseHP + b->baseAttack + b->baseDefense + b->baseSpeed + b->baseSpAttack + b->baseSpDefense;
}

static u16 Zumo_GachaPickSpecies(bool8 strongOnly)
{
    u16 dex, species;

    while (TRUE)
    {
        dex = 1 + Random() % 386;
        if (Zumo_GachaIsLegendary(dex))
            continue;
        species = NationalPokedexNumToSpecies(dex);
        if (strongOnly && Zumo_GachaBaseStatTotal(species) < 500)
            continue;
        return species;
    }
}

static u16 Zumo_FreeMonSlots(void)
{
    u16 n = PARTY_SIZE - gPlayerPartyCount;
    u8 b, i;

    for (b = 0; b < TOTAL_BOXES_COUNT; b++)
        for (i = 0; i < IN_BOX_COUNT; i++)
            if (GetBoxMonDataAt(b, i, MON_DATA_SPECIES) == SPECIES_NONE)
                n++;
    return n;
}

static u8 Zumo_GachaLevel(void)
{
    u8 level = 5 + Zumo_CountBadges() * 5;

    return level > 50 ? 50 : level;
}

// creates one gacha POKéMON and gives it to the player; returns 0 party, 1 PC, 2 no room
static u8 Zumo_GachaGive(u8 slot, u16 species, bool8 strong)
{
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));
    u8 result, i;
    u16 sum;

    CreateMon(mon, species, Zumo_GachaLevel(), 32, 0, 0, OT_ID_PLAYER_ID, 0);
    if (strong)
    {
        u8 iv[6];

        do
        {
            sum = 0;
            for (i = 0; i < 6; i++)
            {
                iv[i] = 20 + Random() % 12;
                sum += iv[i];
            }
        } while (sum < 150);
        for (i = 0; i < 6; i++)
            SetMonData(mon, MON_DATA_HP_IV + i, &iv[i]);
        CalculateMonStats(mon);
    }
    sum = 0;
    for (i = 0; i < 6; i++)
        sum += GetMonData(mon, MON_DATA_HP_IV + i);
    sZG.species[slot] = species;
    sZG.pid[slot] = GetMonData(mon, MON_DATA_PERSONALITY);
    sZG.otId = GetMonData(mon, MON_DATA_OT_ID);
    sZG.strong[slot] = strong;
    sZG.ivSum[slot] = sum;
    {
        u16 bst = Zumo_GachaBaseStatTotal(species);
        u8 tier = (bst < 300) ? 1 : (bst < 400) ? 2 : (bst < 480) ? 3 : (bst < 550) ? 4 : 5;

        if (sum >= 165 && tier < 5)
            tier++;
        sZG.tier[slot] = tier;
    }
    result = GiveMonToPlayer(mon);
    if (result <= 1)
    {
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
    }
    Free(mon);
    return result;
}

// VAR_RESULT: 0 = no badge yet, 1 = next spin is free, 2 = next spin costs money
// STR_VAR_1 = price of x1, STR_VAR_2 = price of x3
void Zumo_GachaStatus(void)
{
    u8 badges = Zumo_CountBadges();
    u32 p, price;

    if (VarGet(ZUMO_GACHA_BADGES) != badges)
    {
        VarSet(ZUMO_GACHA_BADGES, badges);
        VarSet(ZUMO_GACHA_PULLS, 0);
    }
    if (badges == 0)
    {
        gSpecialVar_Result = 0;
        return;
    }
    p = VarGet(ZUMO_GACHA_PULLS);
    price = p * ZUMO_GACHA_STEP;
    ConvertIntToDecimalStringN(gStringVar1, price, STR_CONV_MODE_LEFT_ALIGN, 7);
    ConvertIntToDecimalStringN(gStringVar2, (3 * p + 3) * ZUMO_GACHA_STEP, STR_CONV_MODE_LEFT_ALIGN, 7);
    gSpecialVar_Result = (price == 0) ? 1 : 2;
}

// VAR_RESULT: 0 = given to party, 1 = sent to PC, 2 = no room (nothing charged), 3 = not enough money
// VAR_TEMP_1 = species received
void Zumo_GachaRoll(void)
{
    u32 price = (u32)VarGet(ZUMO_GACHA_PULLS) * ZUMO_GACHA_STEP;
    u16 species;
    u8 result;

    if (price != 0 && !IsEnoughMoney(&gSaveBlock1Ptr->money, price))
    {
        gSpecialVar_Result = 3;
        return;
    }
    if (Zumo_FreeMonSlots() < 1)
    {
        gSpecialVar_Result = 2;
        return;
    }
    species = Zumo_GachaPickSpecies(FALSE);
    sZG.count = 1;
    sZG.anyStrong = FALSE;
    result = Zumo_GachaGive(1, species, FALSE);
    sZG.sentToPc = result;
    if (price != 0)
        RemoveMoney(&gSaveBlock1Ptr->money, price);
    VarSet(ZUMO_GACHA_PULLS, VarGet(ZUMO_GACHA_PULLS) + 1);
    VarSet(VAR_TEMP_1, species);
    VarSet(VAR_0x40F7, result);
    gSpecialVar_Result = result;
}

// x3: VAR_RESULT 0 = ok (all in party), 1 = at least one sent to PC, 2 = no room, 3 = not enough money
void Zumo_GachaRoll3(void)
{
    u32 p = VarGet(ZUMO_GACHA_PULLS);
    u32 price = (3 * p + 3) * ZUMO_GACHA_STEP;
    u8 i, r, anyPc = 0;

    if (!IsEnoughMoney(&gSaveBlock1Ptr->money, price))
    {
        gSpecialVar_Result = 3;
        return;
    }
    if (Zumo_FreeMonSlots() < 3)
    {
        gSpecialVar_Result = 2;
        return;
    }
    sZG.count = 3;
    sZG.anyStrong = TRUE;
    // the strong one always sits in the middle
    for (i = 0; i < 3; i++)
    {
        r = Zumo_GachaGive(i, Zumo_GachaPickSpecies(i == 1), i == 1);
        if (r == 1)
            anyPc = 1;
    }
    RemoveMoney(&gSaveBlock1Ptr->money, price);
    VarSet(ZUMO_GACHA_PULLS, p + 3);
    sZG.sentToPc = anyPc;
    VarSet(VAR_0x40F7, anyPc);
    gSpecialVar_Result = anyPc;
}

// TRUE if the POKéMON from the last gacha spin went to the PC
void Zumo_GachaWasPC(void)
{
    gSpecialVar_Result = (VarGet(VAR_0x40F7) == 1);
}

// ---------------------------------------------------------------------------
// ORB BURST scene: capsules open one at a time, the effect scales with the
// POKéMON's strength tier (1 = weak ... 5 = top class)
// ---------------------------------------------------------------------------
static const u32 sZumoGachaBg_Tiles[] = INCBIN_U32("graphics/zumo_gacha/bg.4bpp");
static const u16 sZumoGachaBg_Map[] = INCBIN_U16("graphics/zumo_gacha/bg_map.bin");
static const u16 sZumoGachaBg_Pal[] = INCBIN_U16("graphics/zumo_gacha/bg.gbapal");

// BG1 palette banks 1..5 = tier colours (1 colour, 2 light colour, 3 white)
static const u16 sZumoGachaBeam_Pal[5][16] = {
    { RGB(0, 0, 0), RGB(18, 26, 31), RGB(24, 29, 31), RGB(31, 31, 31) },
    { RGB(0, 0, 0), RGB(13, 29, 15), RGB(21, 31, 22), RGB(31, 31, 31) },
    { RGB(0, 0, 0), RGB(25, 15, 31), RGB(29, 22, 31), RGB(31, 31, 31) },
    { RGB(0, 0, 0), RGB(31, 27, 10), RGB(31, 30, 20), RGB(31, 31, 31) },
    { RGB(0, 0, 0), RGB(31, 27, 10), RGB(31, 30, 20), RGB(31, 31, 31) },
};
static const u16 sZumoGachaRainbow[6] = {
    RGB(31, 8, 8), RGB(31, 20, 6), RGB(31, 30, 8), RGB(10, 30, 12), RGB(10, 26, 31), RGB(24, 12, 31)
};
static const u16 sZumoGachaFlash[6] = {
    RGB(31, 31, 31), RGB(31, 31, 31), RGB(31, 31, 31), RGB(31, 26, 8), RGB(31, 31, 31), RGB(31, 31, 31)
};
// 1 white, 2 blue border, 3 dark text, 4 red text, 5 gold, 6 banner purple, 7 shadow
static const u16 sZumoGachaText_Pal[16] = {
    RGB(0, 0, 0), RGB(31, 31, 31), RGB(8, 13, 23), RGB(7, 7, 8),
    RGB(26, 6, 3), RGB(31, 27, 8), RGB(7, 4, 14), RGB(20, 20, 21),
    0, 0, 0, 0, 0, 0, 0, 0
};

static const struct BgTemplate sZumoGachaBgTemplates[] = {
    { .bg = 0, .charBaseIndex = 0, .mapBaseIndex = 27, .screenSize = 0, .paletteMode = 0, .priority = 0, .baseTile = 0 },
    { .bg = 1, .charBaseIndex = 1, .mapBaseIndex = 28, .screenSize = 0, .paletteMode = 0, .priority = 1, .baseTile = 0 },
    { .bg = 2, .charBaseIndex = 2, .mapBaseIndex = 29, .screenSize = 0, .paletteMode = 0, .priority = 2, .baseTile = 0 },
};

enum { ZGW_MSG, ZGW_PLATE0, ZGW_PLATE1, ZGW_PLATE2, ZGW_BANNER };
static const struct WindowTemplate sZumoGachaWindowTemplates[] = {
    [ZGW_MSG]    = { .bg = 0, .tilemapLeft = 1,  .tilemapTop = 15, .width = 28, .height = 5, .paletteNum = 15, .baseBlock = 0x001 },
    [ZGW_PLATE0] = { .bg = 0, .tilemapLeft = 2,  .tilemapTop = 11, .width = 8,  .height = 4, .paletteNum = 15, .baseBlock = 0x08D },
    [ZGW_PLATE1] = { .bg = 0, .tilemapLeft = 11, .tilemapTop = 11, .width = 8,  .height = 4, .paletteNum = 15, .baseBlock = 0x0AD },
    [ZGW_PLATE2] = { .bg = 0, .tilemapLeft = 20, .tilemapTop = 11, .width = 8,  .height = 4, .paletteNum = 15, .baseBlock = 0x0CD },
    [ZGW_BANNER] = { .bg = 0, .tilemapLeft = 9,  .tilemapTop = 0,  .width = 12, .height = 2, .paletteNum = 15, .baseBlock = 0x0ED },
    DUMMY_WIN_TEMPLATE
};

static const u8 sZGText_Opening1[] = _("Opening the capsule...");
static const u8 sZGText_OpeningN[] = _("Opening capsule {STR_VAR_2} of 3...");
static const u8 sZGText_Reveal1[] = _("Oh... {STR_VAR_1}.");
static const u8 sZGText_Reveal2[] = _("{STR_VAR_1} appeared!");
static const u8 sZGText_Reveal3[] = _("Nice! It's {STR_VAR_1}!");
static const u8 sZGText_Reveal4[] = _("WOW! A golden light!\nIt's {STR_VAR_1}!");
static const u8 sZGText_Reveal5[] = _("INCREDIBLE!!! A rainbow light!\n{STR_VAR_1} has appeared!!!");
static const u8 *const sZGRevealText[5] = { sZGText_Reveal1, sZGText_Reveal2, sZGText_Reveal3, sZGText_Reveal4, sZGText_Reveal5 };
static const u8 sZGText_Got1[] = _("{PLAYER} got {STR_VAR_1}!");
static const u8 sZGText_Got3[] = _("{PLAYER} got {STR_VAR_1}, {STR_VAR_2}\nand {STR_VAR_3}!");
static const u8 sZGText_Strong[] = _("{STR_VAR_1} has outstanding stats!\n(IV {STR_VAR_2}/186, strong species)");
static const u8 sZGText_Banner3[] = _("GREAT!");
static const u8 sZGText_Banner4[] = _("EXCELLENT!");
static const u8 sZGText_Banner5[] = _("AMAZING!!");
static const u8 *const sZGBannerText[3] = { sZGText_Banner3, sZGText_Banner4, sZGText_Banner5 };
static const u8 sZGText_IV[] = _("IV ");
static const u8 sZGText_Of[] = _("/186");

static const u8 sZGColor_Dark[3] = {1, 3, 7};
static const u8 sZGColor_Red[3] = {1, 4, 7};
static const u8 sZGColor_Banner[3] = {6, 5, 6};

static const u16 sZGBallItems[3] = {ITEM_POKE_BALL, ITEM_GREAT_BALL, ITEM_ULTRA_BALL};
static const u8 sZGSlotX[3] = {48, 120, 192};
static const u8 sZGOrder3[3] = {0, 2, 1};   // the strong one (middle) is opened last
static const u8 sZGOrder1[1] = {0};
#define ZG_CY 62
#define ZG_START 50
#define ZG_REV 230
#define ZG_BURST_T 100   // local frame of the burst inside one reveal

struct ZumoGachaScene
{
    u16 frame;
    u8 ball[ZG_MAX];
    u8 mon[ZG_MAX];
    u8 matrix[ZG_MAX];
    u16 *bg1Buf;
    u16 *bg2Buf;
    u8 n;
    u8 first;     // first slot used (1 when only one capsule)
    u8 state;
    u8 order[ZG_MAX];
    u8 bannerTier;
};
static struct ZumoGachaScene sZS;

static void ZG_NullVBlankHBlank(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
}

static void VBlankCB_ZumoGacha(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void ZG_Msg(const u8 *str)
{
    StringExpandPlaceholders(gStringVar4, str);
    FillWindowPixelBuffer(ZGW_MSG, PIXEL_FILL(1));
    FillWindowPixelRect(ZGW_MSG, PIXEL_FILL(2), 0, 0, 224, 2);
    FillWindowPixelRect(ZGW_MSG, PIXEL_FILL(2), 0, 38, 224, 2);
    FillWindowPixelRect(ZGW_MSG, PIXEL_FILL(2), 0, 0, 2, 40);
    FillWindowPixelRect(ZGW_MSG, PIXEL_FILL(2), 222, 0, 2, 40);
    AddTextPrinterParameterized4(ZGW_MSG, FONT_NORMAL, 8, 3, 0, 0, sZGColor_Dark, TEXT_SKIP_DRAW, gStringVar4);
    PutWindowTilemap(ZGW_MSG);
    CopyWindowToVram(ZGW_MSG, COPYWIN_FULL);
}

static void ZG_DrawPlate(u8 slot)
{
    u8 w = ZGW_PLATE0 + slot;
    u8 buf[24], num[6];

    FillWindowPixelBuffer(w, PIXEL_FILL(1));
    FillWindowPixelRect(w, PIXEL_FILL(2), 0, 0, 64, 2);
    FillWindowPixelRect(w, PIXEL_FILL(2), 0, 30, 64, 2);
    FillWindowPixelRect(w, PIXEL_FILL(2), 0, 0, 2, 32);
    FillWindowPixelRect(w, PIXEL_FILL(2), 62, 0, 2, 32);
    AddTextPrinterParameterized4(w, FONT_NORMAL, 4, 0, 0, 0, sZGColor_Dark, TEXT_SKIP_DRAW, gSpeciesNames[sZG.species[slot]]);
    StringCopy(buf, sZGText_IV);
    ConvertIntToDecimalStringN(num, sZG.ivSum[slot], STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(buf, num);
    StringAppend(buf, sZGText_Of);
    AddTextPrinterParameterized4(w, FONT_NORMAL, 4, 14, 0, 0, sZG.tier[slot] >= 4 ? sZGColor_Red : sZGColor_Dark, TEXT_SKIP_DRAW, buf);
    PutWindowTilemap(w);
    CopyWindowToVram(w, COPYWIN_FULL);
}

static void ZG_DrawBanner(u8 tier)
{
    FillWindowPixelBuffer(ZGW_BANNER, PIXEL_FILL(6));
    AddTextPrinterParameterized4(ZGW_BANNER, FONT_NORMAL, 10, 0, 0, 0, sZGColor_Banner, TEXT_SKIP_DRAW, sZGBannerText[tier - 3]);
    PutWindowTilemap(ZGW_BANNER);
    CopyWindowToVram(ZGW_BANNER, COPYWIN_FULL);
}

// halo circle tiles (64x64 px): tiles 8..71 of the BG1 char block, colour index 1
static void ZG_MakeHaloTiles(void)
{
    u8 *dst = (u8 *)(BG_VRAM + 0x4000);
    u8 tx, ty, px, py;

    CpuFill32(0, dst, 32 * 4);
    CpuFill32(0x11111111, dst + 32 * 1, 32);
    for (ty = 0; ty < 8; ty++)
    {
        for (tx = 0; tx < 8; tx++)
        {
            u8 *t = dst + (8 + ty * 8 + tx) * 32;

            for (py = 0; py < 8; py++)
            {
                for (px = 0; px < 8; px += 2)
                {
                    s16 x0 = tx * 8 + px - 32, y = ty * 8 + py - 32, x1 = x0 + 1;
                    u8 lo = (x0 * x0 + y * y < 30 * 30) ? 1 : 0;
                    u8 hi = (x1 * x1 + y * y < 30 * 30) ? 1 : 0;

                    t[py * 4 + px / 2] = lo | (hi << 4);
                }
            }
        }
    }
}

#define ZGM(tile, bank) ((tile) | ((bank) << 12))

static void ZG_PutBeam(u8 cx, u8 w, u16 entry)
{
    u8 y, c;
    s16 x0 = (s16)cx - w / 2;

    for (y = 0; y < ZG_CY / 8 + 1; y++)
        for (c = 0; c < w; c++)
            if (x0 + c >= 0 && x0 + c < 30)
                sZS.bg1Buf[y * 32 + x0 + c] = entry;
}

// rebuilds the whole BG1 layer (beams + halos) for every reveal that has started
static bool8 ZG_ComposeBg1(u16 f)
{
    u8 k, x, y;
    bool8 beams = FALSE;

    CpuFill16(0, sZS.bg1Buf, 32 * 32 * 2);
    for (k = 0; k < sZS.n; k++)
    {
        s16 t = (s16)f - (ZG_START + k * ZG_REV);
        u8 i = sZS.order[k];
        u8 slot = sZS.first + i;
        u8 tier = sZG.tier[slot];
        u8 cx = sZGSlotX[slot] / 8;
        u16 e = ZGM(1, tier);

        if (t < 60)
            continue;
        if (t < ZG_BURST_T)
        {
            // teaser: a thin beam in the tier colour leaks out of the capsule
            ZG_PutBeam(cx, 1, e);
            beams = TRUE;
            continue;
        }
        t -= ZG_BURST_T;
        if (t >= 14 && tier >= 3)
        {
            beams = TRUE;
            for (y = 0; y < 8; y++)
                for (x = 0; x < 8; x++)
                    sZS.bg1Buf[(ZG_CY / 8 - 4 + y) * 32 + (cx - 4 + x)] = ZGM(8 + y * 8 + x, tier);
        }
        if (t < 80)
        {
            u8 w = (t < 12) ? 3 : (t < 30) ? 2 : 1;

            beams = TRUE;
            ZG_PutBeam(cx, w, e);
            if (tier >= 3 && t < 50)
            {
                ZG_PutBeam(cx - 4, 1, e);
                ZG_PutBeam(cx + 4, 1, e);
            }
            if (tier >= 5 && t < 60)
            {
                ZG_PutBeam(cx - 7, 1, e);
                ZG_PutBeam(cx + 7, 1, e);
                ZG_PutBeam(cx - 2, 1, e);
                ZG_PutBeam(cx + 2, 1, e);
            }
        }
    }
    CopyBgTilemapBufferToVram(1);
    return beams;
}

static void ZG_SetScale(u8 i, s16 scale)
{
    s16 m = (256 * 256) / scale;

    SetOamMatrix(sZS.matrix[i], m, 0, 0, m);
}

static void ZG_Flash(u8 which)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, sZumoGachaFlash[which]);
}

// the burst of reveal k
static void ZG_Burst(u8 k)
{
    u8 i = sZS.order[k];
    u8 slot = sZS.first + i;
    u16 id;
    struct Sprite *spr;
    u8 tier = sZG.tier[slot];

    gSprites[sZS.ball[i]].invisible = TRUE;
    id = CreateMonPicSprite_HandleDeoxys(sZG.species[slot], sZG.otId, sZG.pid[slot], TRUE, sZGSlotX[slot], ZG_CY + 2, 12 + i, 0xFFFF);
    sZS.mon[i] = id;
    spr = &gSprites[id];
    spr->oam.priority = 1;
    spr->invisible = TRUE;
    spr->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
    spr->oam.matrixNum = sZS.matrix[i];
    CalcCenterToCornerVec(spr, spr->oam.shape, spr->oam.size, ST_OAM_AFFINE_DOUBLE);
    ZG_SetScale(i, 40);
    if (tier >= 2)
        ZG_Flash(tier >= 4 ? 3 : 0);
    PlaySE(SE_BALL_OPEN);
}

static void ZG_Cleanup(void)
{
    u8 i;

    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    ChangeBgX(2, 0, BG_COORD_SET);
    ChangeBgY(2, 0, BG_COORD_SET);
    for (i = 0; i < sZS.n; i++)
    {
        if (sZS.mon[i] != 0xFF)
            FreeAndDestroyMonPicSprite(sZS.mon[i]);
        DestroySprite(&gSprites[sZS.ball[i]]);
        FreeSpriteTilesByTag(0x2D10 + i);
        FreeSpritePaletteByTag(0x2D10 + i);
        FreeOamMatrix(sZS.matrix[i]);
    }
    FreeAllWindowBuffers();
    Free(sZS.bg1Buf);
    Free(sZS.bg2Buf);
    SetVBlankCallback(NULL);
    SetMainCallback2(gMain.savedCallback);
}

static void CB2_ZumoGachaMain(void)
{
    u16 f = sZS.frame;
    u8 k, i, lastK = sZS.n - 1;
    bool8 beams;
    s16 shakeX = 0, shakeY = 0;
    u16 endT = ZG_START + lastK * ZG_REV + ZG_BURST_T;   // burst of the last reveal

    if (sZS.state == 2)
    {
        if (!gPaletteFade.active)
            ZG_Cleanup();
        RunTextPrinters();
        AnimateSprites();
        BuildOamBuffer();
        UpdatePaletteFade();
        return;
    }

    // ---- capsules that are still closed: idle bob / shake ----
    for (k = 0; k < sZS.n; k++)
    {
        s16 t = (s16)f - (ZG_START + k * ZG_REV);
        struct Sprite *spr = &gSprites[sZS.ball[sZS.order[k]]];

        if (t >= ZG_BURST_T)
            continue;
        if (t < 0)
        {
            spr->x2 = 0;
            spr->y2 = Sin((f * 4 + k * 60) & 0xFF, 2);
        }
        else
        {
            s16 amp = 1 + (t * 6) / ZG_BURST_T;

            spr->x2 = Sin((f * 29 + k * 40) & 0xFF, amp);
            spr->y2 = 0;
            if (t == 20 || t == 45 || t == 70 || t == 90)
                PlaySE(SE_BALL_CLICK);
        }
    }

    for (k = 0; k < sZS.n; k++)
    {
        s16 t = (s16)f - (ZG_START + k * ZG_REV);
        u8 i2 = sZS.order[k];
        u8 slot = sZS.first + i2;
        u8 tier = sZG.tier[slot];

        if (t == 0)
        {
            if (sZS.n == 1)
                ZG_Msg(sZGText_Opening1);
            else
            {
                ConvertIntToDecimalStringN(gStringVar2, k + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
                ZG_Msg(sZGText_OpeningN);
            }
        }
        if (t == ZG_BURST_T)
            ZG_Burst(k);
        if (t > ZG_BURST_T)
        {
            s16 u = t - ZG_BURST_T;

            // secondary flashes and shaking for the top tiers
            if (tier >= 4 && u == 20)
                ZG_Flash(tier >= 5 ? 3 : 0);
            if (tier >= 5 && u == 40)
                ZG_Flash(0);
            if (tier >= 5 && u < 70)
            {
                shakeX = Sin((f * 47) & 0xFF, 3);
                shakeY = Sin((f * 61 + 40) & 0xFF, 2);
            }
            else if (tier >= 4 && u < 40)
            {
                shakeX = Sin((f * 47) & 0xFF, 1);
                shakeY = Sin((f * 61 + 40) & 0xFF, 1);
            }
            // pop in
            if (u >= 8 && u < 44)
            {
                s16 peak = 270 + 22 * tier;
                s16 sc;

                if (u < 24)
                    sc = 40 + ((peak - 40) * (u - 8)) / 16;
                else
                    sc = peak - ((peak - 256) * (u - 24)) / 20;
                gSprites[sZS.mon[i2]].invisible = FALSE;
                ZG_SetScale(i2, sc);
            }
            else if (u == 44)
                ZG_SetScale(i2, 256);
            if (u == 8)
            {
                if (tier >= 5)
                    PlayFanfare(MUS_LEVEL_UP);
                else if (tier >= 3)
                    PlaySE(SE_SHINY);
                else
                    PlaySE(SE_EXP_MAX);
            }
            if (u == 54)
            {
                ZG_DrawPlate(slot);
                if (tier >= 3 && tier > sZS.bannerTier)
                {
                    sZS.bannerTier = tier;
                    ZG_DrawBanner(tier);
                }
                StringCopy(gStringVar1, gSpeciesNames[sZG.species[slot]]);
                ZG_Msg(sZGRevealText[tier - 1]);
            }
        }
    }

    // rainbow tier: cycle the colour of palette bank 5
    {
        u16 c = sZumoGachaRainbow[(f / 3) % 6];

        LoadPalette(&c, BG_PLTT_ID(5) + 1, 2);
    }
    beams = ZG_ComposeBg1(f);
    {
        s16 eva = beams ? 13 : 9 + Sin((f * 3) & 0xFF, 2);

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(eva, 16 - eva));
    }
    ChangeBgX(1, shakeX << 8, BG_COORD_SET);
    ChangeBgY(1, shakeY << 8, BG_COORD_SET);
    ChangeBgX(2, shakeX << 8, BG_COORD_SET);
    ChangeBgY(2, shakeY << 8, BG_COORD_SET);

    // ---- wrap-up after the last reveal ----
    if (f == endT + 120)
    {
        if (sZS.n == 1)
        {
            StringCopy(gStringVar1, gSpeciesNames[sZG.species[sZS.first]]);
            ZG_Msg(sZGText_Got1);
        }
        else
        {
            StringCopy(gStringVar1, gSpeciesNames[sZG.species[0]]);
            StringCopy(gStringVar2, gSpeciesNames[sZG.species[1]]);
            StringCopy(gStringVar3, gSpeciesNames[sZG.species[2]]);
            ZG_Msg(sZGText_Got3);
        }
    }
    if (f == endT + 300 && sZG.anyStrong)
    {
        StringCopy(gStringVar1, gSpeciesNames[sZG.species[1]]);
        ConvertIntToDecimalStringN(gStringVar2, sZG.ivSum[1], STR_CONV_MODE_LEFT_ALIGN, 3);
        ZG_Msg(sZGText_Strong);
    }
    if (f >= endT + 130 && (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON)))
    {
        sZS.state = 2;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    }
    sZS.frame++;
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void CB2_ZumoGachaInit(void)
{
    switch (gMain.state)
    {
    case 0:
        ZG_NullVBlankHBlank();
        gMain.state++;
        break;
    case 1:
        DmaFillLarge16(3, 0, (void *)VRAM, VRAM_SIZE, 0x1000);
        DmaFill32Defvars(3, 0, (void *)OAM, OAM_SIZE);
        DmaFill16Defvars(3, 0, (void *)PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(FALSE);
        InitBgsFromTemplates(0, sZumoGachaBgTemplates, NELEMS(sZumoGachaBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        InitWindows(sZumoGachaWindowTemplates);
        DeactivateAllTextPrinters();
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        FreeAllSpritePalettes();
        ResetAllPicSprites();
        gMain.state++;
        break;
    case 2:
    {
        u8 i, x, y;

        sZS.frame = 0;
        sZS.state = 0;
        sZS.bannerTier = 0;
        sZS.n = sZG.count;
        sZS.first = (sZS.n == 1) ? 1 : 0;
        for (i = 0; i < sZS.n; i++)
            sZS.order[i] = (sZS.n == 1) ? sZGOrder1[i] : sZGOrder3[i];
        // background art
        CpuCopy32(sZumoGachaBg_Tiles, (void *)(BG_VRAM + 0x8000), sizeof(sZumoGachaBg_Tiles));
        sZS.bg2Buf = AllocZeroed(32 * 32 * 2);
        for (y = 0; y < 20; y++)
            for (x = 0; x < 30; x++)
                sZS.bg2Buf[y * 32 + x] = sZumoGachaBg_Map[y * 30 + x];
        SetBgTilemapBuffer(2, sZS.bg2Buf);
        sZS.bg1Buf = AllocZeroed(32 * 32 * 2);
        SetBgTilemapBuffer(1, sZS.bg1Buf);
        ZG_MakeHaloTiles();
        LoadPalette(sZumoGachaBg_Pal, BG_PLTT_ID(0), 32);
        LoadPalette(sZumoGachaBeam_Pal, BG_PLTT_ID(1), sizeof(sZumoGachaBeam_Pal));
        LoadPalette(sZumoGachaText_Pal, BG_PLTT_ID(15), 32);
        // capsule icons, scaled to ~1.5x
        for (i = 0; i < sZS.n; i++)
        {
            u8 slot = sZS.first + i;
            u8 id = AddItemIconObject(0x2D10 + i, 0x2D10 + i, sZGBallItems[Random() % 3]);
            struct Sprite *spr = &gSprites[id];

            sZS.ball[i] = id;
            sZS.mon[i] = 0xFF;
            sZS.matrix[i] = AllocOamMatrix();
            SetOamMatrix(sZS.matrix[i], 171, 0, 0, 171);
            spr->x = sZGSlotX[slot];
            spr->y = ZG_CY;
            spr->oam.priority = 1;
            spr->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
            spr->oam.matrixNum = sZS.matrix[i];
            CalcCenterToCornerVec(spr, spr->oam.shape, spr->oam.size, ST_OAM_AFFINE_DOUBLE);
        }
        ZG_Msg(sZGText_Opening1);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        gMain.state++;
        break;
    }
    case 3:
        if (IsDma3ManagerBusyWithBgCopy())
            break;
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BD);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(13, 3));
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_ZumoGacha);
        SetMainCallback2(CB2_ZumoGachaMain);
        break;
    }
}

// opens the gacha scene for the POKéMON in sZG (the script calls "waitstate" afterwards)
void Zumo_GachaScene(void)
{
    gMain.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
    gMain.state = 0;
    SetMainCallback2(CB2_ZumoGachaInit);
}
