#ifndef GUARD_CONSTANTS_ZUMO_EXTRA_H
#define GUARD_CONSTANTS_ZUMO_EXTRA_H
// Zumo Edition (ported from the pokefirered Ver 4.2 hack): flags/vars that lived in vanilla unused slots

#define FLAG_MIGRANT_QUEST_DONE         0x4A7
#define FLAG_MEW_CAUGHT                 0x4A8
#define FLAG_EARLY_MEWTWO_DONE          0x4A9
#define FLAG_HIDE_EARLY_MEWTWO          0x4AA
#define FLAG_HIDE_NOX_FINAL             0x4AB
#define FLAG_DEFEATED_NOX_FINAL         0x4AC
#define FLAG_HIDE_FINALE_FRIENDS        0x4AD

#define VAR_PLAYER_STARTER_GEN          0x4029 // 0: Gen1, 1: Gen2, 2: Gen3
#define VAR_RIVAL_VARIANT               0x402A // same meaning as VAR_STARTER_MON, picks rival team variant
#define VAR_RIVAL_STARTER_GEN           0x402B // 0: Gen1, 1: Gen2, 2: Gen3
#define VAR_ZK_SSANNE_DECK              0x403C // S.S. ANNE deck Magma scene (was VAR_0x402C in Ver 4.2)

#include "constants/hx_flags.h"
#include "constants/zumo_flags.h"

#endif // GUARD_CONSTANTS_ZUMO_EXTRA_H
