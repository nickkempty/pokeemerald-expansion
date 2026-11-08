.equ LOCALID_MEW_SIGHTING, 0x60

EventScript_MewSighting::
	lockall
	special SpawnMewSighting
	playmoncry SPECIES_MEW, CRY_MODE_NORMAL
	applymovement LOCALID_MEW_SIGHTING, Movement_MewSightingEnter
	waitmovement 0
	waitmoncry
	msgbox Text_MewSightingAppeared, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_MewSightingBattle
	msgbox Text_MewSightingLeft
	closemessage
	applymovement LOCALID_MEW_SIGHTING, Movement_MewSightingLeave
	waitmovement 0
	special RemoveMewSightingObject
	releaseall
	end

EventScript_MewSightingBattle::
	closemessage
	special CreateMewSightingBattle
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	special RemoveMewSightingObject
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_MewSightingCaught
	msgbox Text_MewSightingEscaped
	releaseall
	end

EventScript_MewSightingCaught::
	setflag FLAG_MEW_CAUGHT
	releaseall
	end

Movement_MewSightingEnter::
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	jump_in_place_right
	step_end

Movement_MewSightingLeave::
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	step_end

Text_MewSightingAppeared::
    .string "A small pink POKéMON is floating in\n"
    .string "the air, giggling at you!\p"
    .string "Do you want to try to catch it?$"

Text_MewSightingLeft::
    .string "The pink POKéMON flew off with the\n"
    .string "wind…$"

Text_MewSightingEscaped::
    .string "The pink POKéMON vanished into the\n"
    .string "sky! It might show up again in\l"
    .string "another town…$"

EventScript_ZumoMoveReminder::
	lock
	faceplayer
	msgbox Text_ZumoMoveReminderIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ZumoMoveReminderBye
EventScript_ZumoMoveReminderChoose::
	msgbox Text_ZumoMoveReminderWhich
	special ChooseMonForMoveRelearner
	waitstate
	goto_if_ge VAR_0x8004, PARTY_SIZE, EventScript_ZumoMoveReminderBye
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, EventScript_ZumoMoveReminderEgg
	goto_if_eq VAR_0x8005, 0, EventScript_ZumoMoveReminderNone
	msgbox Text_ZumoMoveReminderPickMove
	special TeachMoveRelearnerMove
	waitstate
	goto_if_eq VAR_0x8004, 0, EventScript_ZumoMoveReminderChoose
	goto EventScript_ZumoMoveReminderBye
	end

EventScript_ZumoMoveReminderEgg::
	msgbox Text_ZumoMoveReminderEgg
	goto EventScript_ZumoMoveReminderChoose
	end

EventScript_ZumoMoveReminderNone::
	msgbox Text_ZumoMoveReminderNone
	goto EventScript_ZumoMoveReminderChoose
	end

EventScript_ZumoMoveReminderBye::
	msgbox Text_ZumoMoveReminderBye
	release
	end

EventScript_ZumoDexSeller::
	lock
	faceplayer
	showmoneybox 0, 0
	msgbox Text_ZumoDexSellerIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ZumoDexSellerBye
EventScript_ZumoDexSellerPick::
	message Text_ZumoDexSellerWhich
	waitmessage
	special ShowDexSellerList
	waitstate
	goto_if_eq VAR_RESULT, 0x7F, EventScript_ZumoDexSellerBye
	checkmoney 3000
	goto_if_eq VAR_RESULT, FALSE, EventScript_ZumoDexSellerNoMoney
	removemoney 3000
	updatemoneybox
	special SetDexSeenFromSeller
	bufferspeciesname STR_VAR_1, VAR_0x8005
	playse SE_SHOP
	msgbox Text_ZumoDexSellerDone
	goto EventScript_ZumoDexSellerBye
	end

EventScript_ZumoDexSellerNoMoney::
	msgbox Text_ZumoDexSellerNoMoney
	goto EventScript_ZumoDexSellerBye
	end

EventScript_ZumoDexSellerBye::
	hidemoneybox
	msgbox Text_ZumoDexSellerBye
	release
	end

Text_ZumoMoveReminderIntro::
    .string "I'm the MOVE REMINDER! I can help\n"
    .string "your POKéMON remember moves it\l"
    .string "forgot. It's free!\p"
    .string "Shall I help?$"

Text_ZumoMoveReminderWhich::
    .string "Which POKéMON should remember a\n"
    .string "move?$"

Text_ZumoMoveReminderPickMove::
    .string "Which move should it remember?$"

Text_ZumoMoveReminderEgg::
    .string "Hmm… An EGG can't remember any\n"
    .string "moves!$"

Text_ZumoMoveReminderNone::
    .string "This POKéMON has no forgotten moves\n"
    .string "to remember right now.$"

Text_ZumoMoveReminderBye::
    .string "Come back any time!$"

Text_ZumoDexSellerIntro::
    .string "I sell POKéDEX data! For ¥3,000,\n"
    .string "I'll fill in the entry of one\l"
    .string "POKéMON you've never seen.\p"
    .string "Want to buy some data?$"

Text_ZumoDexSellerWhich::
    .string "Which POKéMON's data do you want?$"

Text_ZumoDexSellerDone::
    .string "Done! The data on {STR_VAR_1} was\n"
    .string "added to your POKéDEX!$"

Text_ZumoDexSellerNoMoney::
    .string "Sorry, you don't have enough\n"
    .string "money.$"

Text_ZumoDexSellerBye::
    .string "Thank you! Please come again!$"

EventScript_QuestMenu::
	lockall
	special BufferQuestText
	msgbox gZumoQuestText
	releaseall
	end

EventScript_ZumoTestWarp::
	warp MAP_ROUTE1, 12, 33
	waitstate
	end

@ ---- Gym bonus TM: choose 1 of 3 type TMs after each gym win (v2.3) ----
EventScript_ZumoGymBonusTM::
	msgbox Text_ZumoGymBonusIntro
EventScript_ZumoGymBonusLoop::
	goto_if_eq VAR_0x8004, 0, EventScript_ZumoGymBonus_0
	goto_if_eq VAR_0x8004, 1, EventScript_ZumoGymBonus_1
	goto_if_eq VAR_0x8004, 2, EventScript_ZumoGymBonus_2
	goto_if_eq VAR_0x8004, 3, EventScript_ZumoGymBonus_3
	goto_if_eq VAR_0x8004, 4, EventScript_ZumoGymBonus_4
	goto_if_eq VAR_0x8004, 5, EventScript_ZumoGymBonus_5
	goto_if_eq VAR_0x8004, 6, EventScript_ZumoGymBonus_6
	goto_if_eq VAR_0x8004, 7, EventScript_ZumoGymBonus_7
	goto_if_eq VAR_0x8004, 8, EventScript_ZumoGymBonus_8
	goto_if_eq VAR_0x8004, 9, EventScript_ZumoGymBonus_9
	goto_if_eq VAR_0x8004, 10, EventScript_ZumoGymBonus_10
	goto_if_eq VAR_0x8004, 11, EventScript_ZumoGymBonus_11
	goto_if_eq VAR_0x8004, 12, EventScript_ZumoGymBonus_12
	goto_if_eq VAR_0x8004, 13, EventScript_ZumoGymBonus_13
	goto_if_eq VAR_0x8004, 14, EventScript_ZumoGymBonus_14
	return

EventScript_ZumoGymBonus_0::
	setvar VAR_0x8005, ITEM_TM37
	setvar VAR_0x8006, MOVE_SANDSTORM
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM31
	setvar VAR_0x8006, MOVE_BRICK_BREAK
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM28
	setvar VAR_0x8006, MOVE_DIG
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_1::
	setvar VAR_0x8005, ITEM_TM18
	setvar VAR_0x8006, MOVE_RAIN_DANCE
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM13
	setvar VAR_0x8006, MOVE_ICE_BEAM
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM14
	setvar VAR_0x8006, MOVE_BLIZZARD
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_2::
	setvar VAR_0x8005, ITEM_TM24
	setvar VAR_0x8006, MOVE_THUNDERBOLT
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM25
	setvar VAR_0x8006, MOVE_THUNDER
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM16
	setvar VAR_0x8006, MOVE_LIGHT_SCREEN
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_3::
	setvar VAR_0x8005, ITEM_TM09
	setvar VAR_0x8006, MOVE_BULLET_SEED
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM22
	setvar VAR_0x8006, MOVE_SOLAR_BEAM
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM11
	setvar VAR_0x8006, MOVE_SUNNY_DAY
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_4::
	setvar VAR_0x8005, ITEM_TM36
	setvar VAR_0x8006, MOVE_SLUDGE_BOMB
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM32
	setvar VAR_0x8006, MOVE_DOUBLE_TEAM
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM46
	setvar VAR_0x8006, MOVE_THIEF
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_5::
	setvar VAR_0x8005, ITEM_TM29
	setvar VAR_0x8006, MOVE_PSYCHIC
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM30
	setvar VAR_0x8006, MOVE_SHADOW_BALL
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM33
	setvar VAR_0x8006, MOVE_REFLECT
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_6::
	setvar VAR_0x8005, ITEM_TM35
	setvar VAR_0x8006, MOVE_FLAMETHROWER
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM50
	setvar VAR_0x8006, MOVE_OVERHEAT
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM11
	setvar VAR_0x8006, MOVE_SUNNY_DAY
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_7::
	setvar VAR_0x8005, ITEM_TM28
	setvar VAR_0x8006, MOVE_DIG
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM23
	setvar VAR_0x8006, MOVE_IRON_TAIL
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM27
	setvar VAR_0x8006, MOVE_RETURN
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_8::
	setvar VAR_0x8005, ITEM_TM26
	setvar VAR_0x8006, MOVE_EARTHQUAKE
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM23
	setvar VAR_0x8006, MOVE_IRON_TAIL
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM08
	setvar VAR_0x8006, MOVE_BULK_UP
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_9::
	setvar VAR_0x8005, ITEM_TM31
	setvar VAR_0x8006, MOVE_BRICK_BREAK
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM01
	setvar VAR_0x8006, MOVE_FOCUS_PUNCH
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM21
	setvar VAR_0x8006, MOVE_FRUSTRATION
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_10::
	setvar VAR_0x8005, ITEM_TM11
	setvar VAR_0x8006, MOVE_SUNNY_DAY
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM38
	setvar VAR_0x8006, MOVE_FIRE_BLAST
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM43
	setvar VAR_0x8006, MOVE_SECRET_POWER
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_11::
	setvar VAR_0x8005, ITEM_TM44
	setvar VAR_0x8006, MOVE_REST
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM17
	setvar VAR_0x8006, MOVE_PROTECT
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM40
	setvar VAR_0x8006, MOVE_AERIAL_ACE
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_12::
	setvar VAR_0x8005, ITEM_TM47
	setvar VAR_0x8006, MOVE_STEEL_WING
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM15
	setvar VAR_0x8006, MOVE_HYPER_BEAM
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM10
	setvar VAR_0x8006, MOVE_HIDDEN_POWER
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_13::
	setvar VAR_0x8005, ITEM_TM29
	setvar VAR_0x8006, MOVE_PSYCHIC
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM30
	setvar VAR_0x8006, MOVE_SHADOW_BALL
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM32
	setvar VAR_0x8006, MOVE_DOUBLE_TEAM
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoGymBonus_14::
	setvar VAR_0x8005, ITEM_TM03
	setvar VAR_0x8006, MOVE_WATER_PULSE
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM07
	setvar VAR_0x8006, MOVE_HAIL
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	setvar VAR_0x8005, ITEM_TM44
	setvar VAR_0x8006, MOVE_REST
	call EventScript_ZumoOfferTM
	goto_if_eq VAR_RESULT, YES, EventScript_ZumoGymBonusGive
	goto EventScript_ZumoGymBonusLoop

EventScript_ZumoOfferTM::
	bufferitemname STR_VAR_1, VAR_0x8005
	buffermovename STR_VAR_2, VAR_0x8006
	msgbox Text_ZumoGymBonusOffer, MSGBOX_YESNO
	return

EventScript_ZumoGymBonusGive::
	checkitemspace VAR_0x8005
	goto_if_eq VAR_RESULT, FALSE, EventScript_ZumoGymBonusNoRoom
	giveitem VAR_0x8005
	return

EventScript_ZumoGymBonusNoRoom::
	msgbox Text_ZumoGymBonusNoRoom
	return

Text_ZumoGymBonusIntro::
    .string "As a special prize for your win,\n"
    .string "you may pick one more TM!\p"
    .string "Choose carefully…$"

Text_ZumoGymBonusOffer::
    .string "{STR_VAR_1}: {STR_VAR_2}\n"
    .string "Take this one?$"

Text_ZumoGymBonusNoRoom::
    .string "Your BAG is full…\n"
    .string "The bonus TM is lost!$"

@ ---- TEAM ROCKET trio ambush (every 2 badges) ----
EventScript_ZumoRocketTrio::
	lockall
	special SpawnRocketTrio
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Text_ZumoRocketIntro
	special GetRocketTrioStage
	goto_if_eq VAR_RESULT, 1, EventScript_ZumoRocketBattle1
	goto_if_eq VAR_RESULT, 2, EventScript_ZumoRocketBattle2
	goto_if_eq VAR_RESULT, 3, EventScript_ZumoRocketBattle3
	goto EventScript_ZumoRocketBattle4

EventScript_ZumoRocketBattle1::
	trainerbattle_double TRAINER_ZUMO_ROCKET_1, Text_ZumoRocketBattleIntro, Text_ZumoRocketDefeat, Text_ZumoRocketNeedTwo, EventScript_ZumoRocketTrioDefeated
	goto EventScript_ZumoRocketTrioLeave
EventScript_ZumoRocketBattle2::
	trainerbattle_double TRAINER_ZUMO_ROCKET_2, Text_ZumoRocketBattleIntro, Text_ZumoRocketDefeat, Text_ZumoRocketNeedTwo, EventScript_ZumoRocketTrioDefeated
	goto EventScript_ZumoRocketTrioLeave
EventScript_ZumoRocketBattle3::
	trainerbattle_double TRAINER_ZUMO_ROCKET_3, Text_ZumoRocketBattleIntro, Text_ZumoRocketDefeat, Text_ZumoRocketNeedTwo, EventScript_ZumoRocketTrioDefeated
	goto EventScript_ZumoRocketTrioLeave
EventScript_ZumoRocketBattle4::
	trainerbattle_double TRAINER_ZUMO_ROCKET_4, Text_ZumoRocketBattleIntro, Text_ZumoRocketDefeat, Text_ZumoRocketNeedTwo, EventScript_ZumoRocketTrioDefeated
	goto EventScript_ZumoRocketTrioLeave

EventScript_ZumoRocketTrioLeave::
	special RemoveRocketTrio
	releaseall
	end

EventScript_ZumoRocketTrioDefeated::
	msgbox Text_ZumoRocketAfter
	special RemoveRocketTrio
	giveitem ITEM_NUGGET
	releaseall
	end

Text_ZumoRocketIntro::
    .string "MUSASHI: Hold it right there, kid!\p"
    .string "KOJIRO: We've been tailing you\n"
    .string "since the last town.\p"
    .string "NYARTH: Hand over all your rare\n"
    .string "POKéMON, nya!$"

Text_ZumoRocketBattleIntro::
    .string "MUSASHI: The new and improved\n"
    .string "TEAM ROCKET is on the job!\p"
    .string "KOJIRO: Let's make this quick!$"

Text_ZumoRocketDefeat::
    .string "MUSASHI: What?! Beaten AGAIN?!$"

Text_ZumoRocketNeedTwo::
    .string "MUSASHI: Hmph! Come back with\n"
    .string "two POKéMON if you want a real\l"
    .string "battle!$"

Text_ZumoRocketAfter::
    .string "KOJIRO: Why does this always\n"
    .string "happen to us…\p"
    .string "NYARTH: Looks like we're blasting\n"
    .string "off agaaain, nya!\p"
    .string "MUSASHI: Fine! Take this and leave\n"
    .string "us alone!$"

@ ---- POKéMON Center gacha clerk (ORB BURST scene) ----
EventScript_ZumoGachaClerk::
	lock
	faceplayer
	special Zumo_GachaStatus
	goto_if_eq VAR_RESULT, 0, EventScript_ZumoGachaNoBadge
	goto_if_eq VAR_RESULT, 1, EventScript_ZumoGachaMenuFree
	message Text_ZumoGachaMenuPaid
	waitmessage
	goto EventScript_ZumoGachaMenu
	end

EventScript_ZumoGachaMenuFree::
	message Text_ZumoGachaMenuFree
	waitmessage
	goto EventScript_ZumoGachaMenu
	end

EventScript_ZumoGachaMenu::
	multichoice 0, 0, MULTICHOICE_ZUMO_GACHA, FALSE
	switch VAR_RESULT
	case 0, EventScript_ZumoGachaSpin1
	case 1, EventScript_ZumoGachaSpin3
	goto EventScript_ZumoGachaDecline
	end

EventScript_ZumoGachaNoBadge::
	msgbox Text_ZumoGachaNoBadge
	release
	end

EventScript_ZumoGachaDecline::
	msgbox Text_ZumoGachaDecline
	release
	end

EventScript_ZumoGachaSpin1::
	special Zumo_GachaRoll
	goto_if_eq VAR_RESULT, 3, EventScript_ZumoGachaNoMoney
	goto_if_eq VAR_RESULT, 2, EventScript_ZumoGachaNoRoom
	closemessage
	fadescreen FADE_TO_BLACK
	special Zumo_GachaScene
	waitstate
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ZumoGachaDone
	special Zumo_GachaWasPC
	goto_if_eq VAR_RESULT, TRUE, EventScript_ZumoGachaNamePC
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto EventScript_ZumoGachaDone
	end

EventScript_ZumoGachaNamePC::
	call Common_EventScript_NameReceivedBoxMon
	call Common_EventScript_TransferredToPC
	release
	end

EventScript_ZumoGachaDone::
	special Zumo_GachaWasPC
	goto_if_eq VAR_RESULT, TRUE, EventScript_ZumoGachaSentPC
	release
	end

EventScript_ZumoGachaSentPC::
	call Common_EventScript_TransferredToPC
	release
	end

EventScript_ZumoGachaSpin3::
	special Zumo_GachaRoll3
	goto_if_eq VAR_RESULT, 3, EventScript_ZumoGachaNoMoney
	goto_if_eq VAR_RESULT, 2, EventScript_ZumoGachaNoRoom3
	closemessage
	fadescreen FADE_TO_BLACK
	special Zumo_GachaScene
	waitstate
	special Zumo_GachaWasPC
	goto_if_eq VAR_RESULT, TRUE, EventScript_ZumoGachaSomePC
	release
	end

EventScript_ZumoGachaSomePC::
	msgbox Text_ZumoGachaSomePC
	release
	end

EventScript_ZumoGachaNoMoney::
	msgbox Text_ZumoGachaNoMoney
	release
	end

EventScript_ZumoGachaNoRoom::
	goto Common_EventScript_NoMoreRoomForPokemon
	end

EventScript_ZumoGachaNoRoom3::
	msgbox Text_ZumoGachaNoRoom3
	release
	end

Text_ZumoGachaNoBadge::
    .string "Welcome to the POKéMON GACHA!\p"
    .string "Beat a GYM and you get one FREE\n"
    .string "spin for that BADGE.\p"
    .string "Come back after you earn one!$"

Text_ZumoGachaMenuFree::
    .string "Congratulations on your BADGE!\n"
    .string "Your first spin is on the house.\p"
    .string "Or open 3 at once for ¥{STR_VAR_2}\n"
    .string "with 1 STRONG one guaranteed!$"

Text_ZumoGachaMenuPaid::
    .string "Welcome back to the GACHA!\p"
    .string "x1: ¥{STR_VAR_1}   x3: ¥{STR_VAR_2}\n"
    .string "x3 guarantees 1 STRONG one!$"

Text_ZumoGachaDecline::
    .string "No problem! Come back any time.$"

Text_ZumoGachaNoMoney::
    .string "Oh no, you don't have enough\n"
    .string "money for that…$"

Text_ZumoGachaNoRoom3::
    .string "You need room for 3 POKéMON\n"
    .string "(party and PC) to open 3 at once.$"

Text_ZumoGachaSomePC::
    .string "Some of the POKéMON were sent to\n"
    .string "the PC.$"

@ ---- NATURE changer (every POKeMON Center): random new nature for 5,000 ----
EventScript_ZumoNatureChanger::
	lock
	faceplayer
	showmoneybox 0, 0
	msgbox Text_ZumoNatureIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ZumoNatureBye
	checkmoney 5000
	goto_if_eq VAR_RESULT, FALSE, EventScript_ZumoNatureNoMoney
	msgbox Text_ZumoNatureWhich
	special ChoosePartyMon
	waitstate
	goto_if_ge VAR_0x8004, PARTY_SIZE, EventScript_ZumoNatureBye
	special Zumo_RandomizeNature
	goto_if_ne VAR_RESULT, 0, EventScript_ZumoNatureCant
	removemoney 5000
	updatemoneybox
	playse SE_USE_ITEM
	msgbox Text_ZumoNatureDone
	hidemoneybox
	release
	end

EventScript_ZumoNatureCant::
	msgbox Text_ZumoNatureCant
	goto EventScript_ZumoNatureBye

EventScript_ZumoNatureNoMoney::
	msgbox Text_ZumoNatureNoMoney
EventScript_ZumoNatureBye::
	msgbox Text_ZumoNatureBye
	hidemoneybox
	release
	end

Text_ZumoNatureIntro::
	.string "I'm a POKéMON MINDSET COACH!\p"
	.string "For ¥5,000 I'll give one of your\n"
	.string "POKéMON a brand-new random NATURE.\l"
	.string "Want to try?$"

Text_ZumoNatureWhich::
	.string "Which POKéMON should I coach?$"

Text_ZumoNatureDone::
	.string "Done! {STR_VAR_1}'s NATURE\n"
	.string "is now {STR_VAR_2}!$"

Text_ZumoNatureCant::
	.string "Hmm, I can't coach that one.$"

Text_ZumoNatureNoMoney::
	.string "Sorry, it costs ¥5,000.$"

Text_ZumoNatureBye::
	.string "Come back any time!$"

@ ---- Day care (every POKeMON Center): 1 POKeMON, gets half of the party's battle EXP ----
EventScript_ZumoDaycare::
	lock
	faceplayer
	special Zumo_DaycareHasMon
	goto_if_eq VAR_RESULT, TRUE, EventScript_ZumoDaycareHasMon
	msgbox Text_ZumoDaycareIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ZumoDaycareBye
	msgbox Text_ZumoDaycareWhich
	special ChoosePartyMon
	waitstate
	goto_if_ge VAR_0x8004, PARTY_SIZE, EventScript_ZumoDaycareBye
	special Zumo_DaycareDeposit
	goto_if_eq VAR_RESULT, 1, EventScript_ZumoDaycareCant
	goto_if_eq VAR_RESULT, 2, EventScript_ZumoDaycareLast
	msgbox Text_ZumoDaycareDeposited
	release
	end

EventScript_ZumoDaycareHasMon::
	special Zumo_DaycareBufferInfo
	msgbox Text_ZumoDaycareStatus, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ZumoDaycareBye
	special Zumo_DaycareWithdraw
	goto_if_eq VAR_RESULT, 1, EventScript_ZumoDaycareFull
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	msgbox Text_ZumoDaycareWithdrawn
	waitfanfare
	release
	end

EventScript_ZumoDaycareCant::
	msgbox Text_ZumoDaycareCant
	goto EventScript_ZumoDaycareBye

EventScript_ZumoDaycareLast::
	msgbox Text_ZumoDaycareLast
	goto EventScript_ZumoDaycareBye

EventScript_ZumoDaycareFull::
	msgbox Text_ZumoDaycareFull
EventScript_ZumoDaycareBye::
	msgbox Text_ZumoDaycareBye
	release
	end

Text_ZumoDaycareIntro::
	.string "I run the POKéMON CENTER DAY CARE!\p"
	.string "Leave one POKéMON with me. Whenever\n"
	.string "your team earns EXP in battle, it\l"
	.string "gets half of it too!\p"
	.string "You can pick it up at any POKéMON\n"
	.string "CENTER. Leave one with me?$"

Text_ZumoDaycareWhich::
	.string "Which POKéMON will you leave?$"

Text_ZumoDaycareDeposited::
	.string "I'll take good care of {STR_VAR_1}!\n"
	.string "Go win some battles!$"

Text_ZumoDaycareStatus::
	.string "Your {STR_VAR_1} has grown by\n"
	.string "{STR_VAR_2} level(s) while it was here.\p"
	.string "Do you want to take it back?$"

Text_ZumoDaycareWithdrawn::
	.string "{STR_VAR_1} came back to your team!$"

Text_ZumoDaycareCant::
	.string "Sorry, I can't take that one.$"

Text_ZumoDaycareLast::
	.string "That's your last POKéMON that can\n"
	.string "battle! Keep it with you.$"

Text_ZumoDaycareFull::
	.string "Your team is full. Make some room\n"
	.string "first.$"

Text_ZumoDaycareBye::
	.string "Come see me any time!$"
