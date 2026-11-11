#ifndef GUARD_ZUMO_H
#define GUARD_ZUMO_H

// Zumo Edition shared helpers
u8 Zumo_CountBadges(void);
u16 CountCaughtMigrantMons(void);
extern u8 gZumoRocketStage;
extern u8 gZumoQuestText[];
struct Pokemon;
bool8 Zumo_TryChangeGender(struct Pokemon *mon);
void Zumo_DaycareGainBattleExp(u32 exp);
bool32 Zumo_MonCanUseFieldMove(struct Pokemon *mon, u32 move);
u32 Zumo_RewardPercent(void);

#endif // GUARD_ZUMO_H
