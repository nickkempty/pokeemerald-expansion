#ifndef GUARD_ZUMO_H
#define GUARD_ZUMO_H

// Zumo Edition shared helpers
u8 Zumo_CountBadges(void);
u16 CountCaughtMigrantMons(void);
extern u8 gZumoRocketStage;
extern u8 gZumoQuestText[];
struct Pokemon;
bool8 Zumo_TryChangeGender(struct Pokemon *mon);

#endif // GUARD_ZUMO_H
