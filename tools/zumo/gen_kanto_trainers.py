#!/usr/bin/env python3
"""Zumo Edition: custom Kanto story trainers (ids 1200-1279, flags in SaveBlock1.hxTrainerFlags).
Writes include/constants/zumo_trainers.h, src/data/trainers_zumo.h, src/data/trainer_parties_zumo.h
and applies gym level shifts (from stored vanilla levels, so it is idempotent).
"""
import json, os, re

FR = "/home/claude/pokefirered"
HERE = os.path.dirname(os.path.abspath(__file__))
FIRST_ID = 1200

GRUNT_M = dict(cls="TRAINER_CLASS_TEAM_MAGMA", pic="TRAINER_PIC_MAGMA_GRUNT_M", music="TRAINER_ENCOUNTER_MUSIC_MAGMA", name="GRUNT")
GRUNT_F = dict(cls="TRAINER_CLASS_TEAM_MAGMA", pic="TRAINER_PIC_MAGMA_GRUNT_F", music="F_TRAINER_FEMALE | TRAINER_ENCOUNTER_MUSIC_MAGMA", name="GRUNT")
BOSS_NOX = dict(cls="TRAINER_CLASS_BOSS", pic="TRAINER_PIC_LEADER_GIOVANNI", music="TRAINER_ENCOUNTER_MUSIC_AQUA", name="NOX")
LEADER_MAXIE = dict(cls="TRAINER_CLASS_MAGMA_LEADER", pic="TRAINER_PIC_MAGMA_LEADER_MAXIE", music="TRAINER_ENCOUNTER_MUSIC_MAGMA", name="MAXIE")
ADMIN_M = dict(cls="TRAINER_CLASS_MAGMA_ADMIN", pic="TRAINER_PIC_MAGMA_ADMIN_M", music="TRAINER_ENCOUNTER_MUSIC_MAGMA", name="TABITHA")

# id suffix, base, party [(species, level)] -- default moves
SPEC = [
    ("MTMOON_MAGMA_1", GRUNT_M, [("POOCHYENA", 12), ("NUMEL", 13)]),
    ("MTMOON_MAGMA_2", GRUNT_F, [("ZUBAT", 12), ("POOCHYENA", 12), ("NUMEL", 13)]),
    ("MTMOON_TABITHA", ADMIN_M, [("ZUBAT", 14), ("POOCHYENA", 15), ("NUMEL", 16)]),
    ("ROCKTUNNEL_MAGMA_1", GRUNT_M, [("NUMEL", 23), ("MIGHTYENA", 24)]),
    ("ROCKTUNNEL_MAGMA_2", GRUNT_F, [("GOLBAT", 23), ("KOFFING", 23), ("NUMEL", 24)]),
    ("ROCKTUNNEL_TABITHA", ADMIN_M, [("GOLBAT", 25), ("MIGHTYENA", 25), ("NUMEL", 27)]),
]
# appended after the rival copies so earlier ids stay stable
SPEC2 = [
    ("SSANNE_MAGMA_1", GRUNT_M, [("MIGHTYENA", 31), ("CAMERUPT", 32)]),
    ("SSANNE_MAGMA_2", GRUNT_F, [("GOLBAT", 31), ("WEEZING", 31), ("NUMEL", 32)]),
    ("SSANNE_TABITHA", ADMIN_M, [("CROBAT", 34), ("MIGHTYENA", 34), ("WEEZING", 34), ("CAMERUPT", 36)]),
    ("MANSION_MAXIE", LEADER_MAXIE, [("MIGHTYENA", 43), ("CROBAT", 43), ("WEEZING", 44), ("CAMERUPT", 46)]),
    ("NOX_FINAL", BOSS_NOX, [("DUGTRIO", 78), ("NIDOQUEEN", 79), ("KANGASKHAN", 79), ("NIDOKING", 80), ("TYRANITAR", 81), ("RHYDON", 82)]),
]
# rival copies: (new suffix, vanilla party symbol, level bonus)
RIVAL_COPIES = [
    ("HIDEOUT_RIVAL_SQUIRTLE", "sParty_RivalCeruleanSquirtle", 8),
    ("HIDEOUT_RIVAL_BULBASAUR", "sParty_RivalCeruleanBulbasaur", 8),
    ("HIDEOUT_RIVAL_CHARMANDER", "sParty_RivalCeruleanCharmander", 8),
]
# gym level shifts: trainer constant -> delta (applied to vanilla levels)
GYM_SHIFT = {}
def map_trainers(*prefixes):
    import glob
    out = []
    for pre in prefixes:
        for f in sorted(glob.glob(f"{FR}/data/maps/{pre}*/scripts.inc")):
            for t in re.findall(r"trainerbattle_\w+ (TRAINER_(?!ZK_|HX_)\w+)", open(f).read()):
                if t not in out:
                    out.append(t)
    return out
# alpha10: Silph Co. before gym 4, Sabrina gym 4 (~33), S.S. Anne after Silph, Surge gym 5 (~39)
for t in map_trainers("SilphCo_"):
    GYM_SHIFT[t] = -6 if t.startswith(("TRAINER_RIVAL_", "TRAINER_BOSS_")) else -3
for t in map_trainers("SaffronCity_Gym"):
    GYM_SHIFT[t] = -10
for t in map_trainers("SSAnne_"):
    if not t.startswith("TRAINER_RIVAL_"):
        GYM_SHIFT[t] = 13
for t in map_trainers("VermilionCity_Gym"):
    GYM_SHIFT[t] = 15
# Ver 4.2: postgame Sevii (after the UNION LEAGUE, champion ~89)
for t in map_trainers("OneIsland", "TwoIsland", "ThreeIsland", "FourIsland", "FiveIsland", "SixIsland", "SevenIsland"):
    if t not in GYM_SHIFT:
        GYM_SHIFT[t] = 27 if t == "TRAINER_BOSS_NOX_FINAL" else 30
# alpha13: UNION LEAGUE after 16 badges (Hoenn gyms go up to ~82)
for t in map_trainers("VictoryRoad_"):
    GYM_SHIFT[t] = 35
for t in map_trainers("PokemonLeague_"):
    if t.startswith("TRAINER_ELITE_FOUR_") and not t.endswith("_2"):
        GYM_SHIFT[t] = 28
    elif t.startswith("TRAINER_ELITE_FOUR_"):
        GYM_SHIFT[t] = 20
    elif t.startswith("TRAINER_CHAMPION_FIRST_"):
        GYM_SHIFT[t] = 26
    elif t.startswith("TRAINER_CHAMPION_REMATCH_"):
        GYM_SHIFT[t] = 20
for t in ("TRAINER_LEADER_ERIKA", "TRAINER_LASS_KAY", "TRAINER_LASS_LISA", "TRAINER_PICNICKER_TINA", "TRAINER_BEAUTY_BRIDGET",
          "TRAINER_BEAUTY_TAMIA", "TRAINER_BEAUTY_LORI", "TRAINER_COOLTRAINER_MARY"):
    GYM_SHIFT[t] = -5


def camel(s):
    return "".join(w.capitalize() for w in s.split("_"))


def main():
    consts, entries, parties = [], [], []
    i = FIRST_ID
    i = add_spec(SPEC, i, consts, entries, parties)
    tp = open(f"{FR}/src/data/trainer_parties.h").read()
    tr = open(f"{FR}/src/data/trainers.h").read()
    for suf, src, bonus in RIVAL_COPIES:
        cname = f"TRAINER_ZK_{suf}"
        sym = f"sParty_Zk{camel(suf)}"
        consts.append(f"#define {cname} {i}")
        i += 1
        m = re.search(r"static const struct (\w+) " + src + r"\[\] = \{(.*?)\n\};", tp, re.S)
        body = re.sub(r"\.lvl = (\d+)", lambda mm: f".lvl = {int(mm.group(1)) + bonus}", m.group(2))
        parties.append(f"static const struct {m.group(1)} {sym}[] = {{{body}\n}};\n")
        vanilla = re.search(r"\[TRAINER_RIVAL_CERULEAN_" + suf.split("_")[-1] + r"\] = \{(.*?)\n    \},", tr, re.S).group(1)
        vanilla = re.sub(r"\.party = (\w+)\(\w+\)", lambda mm: f".party = {mm.group(1)}({sym})", vanilla)
        entries.append(f"    [{cname}] = {{{vanilla}\n    }},\n")
    i = add_spec(SPEC2, i, consts, entries, parties)
    finish(consts, entries, parties, tp, tr, i)


def add_spec(spec, i, consts, entries, parties):
    for suf, base, party in spec:
        cname = f"TRAINER_ZK_{suf}"
        sym = f"sParty_Zk{camel(suf)}"
        consts.append(f"#define {cname} {i}")
        i += 1
        mons = "".join(f"    {{\n        .iv = {60 if base in (ADMIN_M, LEADER_MAXIE, BOSS_NOX) else 30},\n        .lvl = {lv},\n        .species = SPECIES_{sp},\n    }},\n" for sp, lv in party)
        parties.append(f"static const struct TrainerMonNoItemDefaultMoves {sym}[] = {{\n{mons}}};\n")
        entries.append(f"""    [{cname}] = {{
        .trainerClass = {base['cls']},
        .encounterMusic_gender = {base['music']},
        .trainerPic = {base['pic']},
        .trainerName = _("{base['name']}"),
        .items = {{}},
        .doubleBattle = FALSE,
        .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE{' | AI_SCRIPT_TRY_TO_FAINT' if base in (ADMIN_M, LEADER_MAXIE, BOSS_NOX) else ''},
        .party = NO_ITEM_DEFAULT_MOVES({sym}),
    }},
""")
    return i


def finish(consts, entries, parties, tp, tr, i):
    open(f"{FR}/include/constants/zumo_trainers.h", "w").write(
        "#ifndef GUARD_CONSTANTS_ZUMO_TRAINERS_H\n#define GUARD_CONSTANTS_ZUMO_TRAINERS_H\n\n// Zumo Edition Kanto story trainers (generated by tools/zumo/gen_kanto_trainers.py)\n"
        + "\n".join(consts) + "\n\n#endif\n")
    open(f"{FR}/src/data/trainers_zumo.h", "w").write("// generated by tools/zumo/gen_kanto_trainers.py\n" + "".join(entries))
    open(f"{FR}/src/data/trainer_parties_zumo.h", "w").write("// generated by tools/zumo/gen_kanto_trainers.py\n" + "\n".join(parties))

    # ---- gym level shifts (vanilla levels kept in gym_levels_orig.json)
    origp = os.path.join(HERE, "gym_levels_orig.json")
    orig = json.load(open(origp)) if os.path.exists(origp) else {}
    for tconst, delta in GYM_SHIFT.items():
        sym = re.search(r"\[" + tconst + r"\] = \{.*?\.party = \w+\((\w+)\)", tr, re.S).group(1)
        m = re.search(r"(static const struct \w+ " + sym + r"\[\] = \{)(.*?)(\n\};)", tp, re.S)
        lv = [int(x) for x in re.findall(r"\.lvl = (\d+)", m.group(2))]
        if sym not in orig:
            orig[sym] = lv
        it = iter(orig[sym])
        body = re.sub(r"\.lvl = \d+", lambda mm: f".lvl = {max(2, next(it) + delta)}", m.group(2))
        tp = tp[:m.start(2)] + body + tp[m.end(2):]
    open(f"{FR}/src/data/trainer_parties.h", "w").write(tp)
    json.dump(orig, open(origp, "w"), indent=1)
    print(f"{i - FIRST_ID} Zumo trainers, {len(GYM_SHIFT)} gym trainers shifted")


if __name__ == "__main__":
    main()
