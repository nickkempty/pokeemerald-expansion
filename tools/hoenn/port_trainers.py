"""Trainer import: Emerald trainer + party -> FireRed trainer slot (TRAINER_HX_*)."""
import json, os, re, collections
from hxlib import *

EM_TRAINERS = read(f"{EM}/src/data/trainers.h")
EM_PARTIES = read(f"{EM}/src/data/trainer_parties.h")

ENTRY_RE = re.compile(r"\[(TRAINER_\w+)\]\s*=\s*\{(.*?)\n    \},", re.S)
PARTY_RE = re.compile(r"static const struct (TrainerMon\w+) (sParty_\w+)\[\] = \{(.*?)\n\};", re.S)
EM_ENTRIES = {m.group(1): m.group(2) for m in ENTRY_RE.finditer(EM_TRAINERS)}
EM_PARTY = {m.group(2): (m.group(1), m.group(3)) for m in PARTY_RE.finditer(EM_PARTIES)}

FR_CONSTS = read(f"{FR}/include/constants/trainers.h")
FR_OPP = read(f"{FR}/include/constants/opponents.h")
FR_ITEMS = read(f"{FR}/include/constants/items.h")
FR_SPECIES = read(f"{FR}/include/constants/species.h")
FR_MOVES = read(f"{FR}/include/constants/moves.h")


def fr_has(sym, text):
    return re.search(r"#define " + re.escape(sym) + r"\b", text) is not None


def free_slots():
    """TRAINER_* constants of FR that nothing references (RS leftovers etc.) + ids above 748."""
    txt = FR_OPP
    names = [(n, int(i)) for n, i in re.findall(r"#define (TRAINER_\w+)\s+(\d+)\b", txt)]
    tok = collections.Counter()
    for root in ["data", "src", "include"]:
        for dp, _, fs in os.walk(os.path.join(FR, root)):
            for f in fs:
                p = os.path.join(dp, f)
                if not f.endswith((".json", ".inc", ".c", ".h", ".s", ".txt")):
                    continue
                if p.endswith(("opponents.h", "data/trainers.h", "trainer_parties.h", "trainers_hx.h",
                               "hx_trainers.h", "trainer_parties_hx.h")):
                    continue
                for m in re.findall(r"TRAINER_[A-Z0-9_]+", read(p)):
                    tok[m] += 1
    skip = {"TRAINER_NONE", "TRAINER_PKMN_PROF_PROF_OAK", "TRAINER_PLAYER_BRENDAN", "TRAINER_PLAYER_MAY",
            "TRAINER_PLAYER_RED", "TRAINER_PLAYER_LEAF"}
    out = [(n, i) for n, i in names if tok[n] == 0 and n not in skip]
    return out


def map_pic(sym):
    suf = sym[len("TRAINER_PIC_"):]
    for cand in ("TRAINER_PIC_RS_" + suf, sym):
        if fr_has(cand, FR_CONSTS):
            return cand
    return None


def map_class(sym):
    suf = sym[len("TRAINER_CLASS_"):]
    for cand in ("TRAINER_CLASS_RS_" + suf, sym):
        if fr_has(cand, FR_CONSTS):
            return cand
    return None


PREFERRED = {
    "TRAINER_ROXANNE_1": "TRAINER_LEADER_ROXANNE",
    "TRAINER_BRAWLY_1": "TRAINER_LEADER_BRAWLY",
    "TRAINER_WATTSON_1": "TRAINER_LEADER_WATTSON",
    "TRAINER_FLANNERY_1": "TRAINER_LEADER_FLANNERY",
    "TRAINER_NORMAN_1": "TRAINER_LEADER_NORMAN",
    "TRAINER_WINONA_1": "TRAINER_LEADER_WINONA",
    "TRAINER_TATE_AND_LIZA_1": "TRAINER_LEADER_TATE_LIZA",
}


def hx_name(em):
    return "TRAINER_HX_" + em[len("TRAINER_"):]


def allocate(state, em):
    """Return (hx constant, fr slot constant)."""
    if em in state.trainers:
        return hx_name(em), state.trainers[em]["slot"]
    if em not in EM_ENTRIES:
        raise KeyError(em)
    used = {t["slot"] for t in state.trainers.values()}
    slot = PREFERRED.get(em)
    if slot is None:
        for n, i in free_slots():
            if n not in used and n not in PREFERRED.values():
                slot = n
                break
    if slot is None:
        # beyond FR's table: numeric ids from 748 up (>= 768 live in SaveBlock1.hxTrainerFlags, max id 1279)
        nums = [t["slot"] for t in state.trainers.values() if isinstance(t["slot"], int)]
        slot = max(nums + [747]) + 1
        if slot > 1199:  # 1200-1279 reserved for tools/zumo Kanto trainers
            raise SystemExit("out of trainer slots")
    state.trainers[em] = {"slot": slot, "offset": 0}
    return hx_name(em), slot


def convert_level(lvl, offset):
    return max(1, min(100, int(lvl) + offset))


def party_text(party_sym, offset, new_sym):
    kind, body = EM_PARTY[party_sym]
    def fix_lvl(m):
        return ".lvl = %d" % convert_level(m.group(1), offset)
    body = re.sub(r"\.lvl\s*=\s*(\d+)", fix_lvl, body)
    return f"static const struct {kind} {new_sym}[] = {{{body}\n}};\n\n"


def write_trainers(state):
    slots = []
    parties = []
    entries = []
    consts = []
    removed = set()
    for em, info in sorted(state.trainers.items(), key=lambda kv: (isinstance(kv[1]["slot"], int), str(kv[1]["slot"]).zfill(6))):
        body = EM_ENTRIES[em]
        slot = info["slot"]
        removed.add(slot)
        pm = re.search(r"\.party\s*=\s*(\w+)\((sParty_\w+)\)", body)
        new_party = "sParty_Hx" + pm.group(2)[len("sParty_"):]
        parties.append(party_text(pm.group(2), info["offset"], new_party))
        body = re.sub(r"\.party\s*=\s*\w+\(sParty_\w+\)", f".party = {pm.group(1)}({new_party})", body)
        body = re.sub(r"\.trainerPic\s*=\s*(\w+)", lambda m: ".trainerPic = " + (map_pic(m.group(1)) or "TRAINER_PIC_RS_YOUNGSTER"), body)
        body = re.sub(r"\.trainerClass\s*=\s*(\w+)", lambda m: ".trainerClass = " + (map_class(m.group(1)) or "TRAINER_CLASS_RS_YOUNGSTER"), body)
        body = re.sub(r"\n\s*\n", "\n", body)
        entries.append(f"    [{slot}] = {{{body}\n    }},\n")
        consts.append(f"#define {hx_name(em)} {slot}\n")
    write(f"{FR}/src/data/trainers_hx.h", "// Auto-generated by tools/hoenn/port_trainers.py\n" + "".join(entries))
    write(f"{FR}/src/data/trainer_parties_hx.h", "// Auto-generated by tools/hoenn/port_trainers.py\n" + "".join(parties))
    write(f"{FR}/include/constants/hx_trainers.h",
          "// Auto-generated by tools/hoenn/port_trainers.py\n#ifndef GUARD_CONSTANTS_HX_TRAINERS_H\n#define GUARD_CONSTANTS_HX_TRAINERS_H\n\n"
          + "".join(consts) + "\n#endif\n")
    # remove the now-replaced placeholder entries from trainers.h
    p = f"{FR}/src/data/trainers.h"
    s = read(p)
    for slot in removed:
        if isinstance(slot, int):
            continue
        pat = re.compile(r"    \[" + slot + r"\] = \{.*?\n    \},\n", re.S)
        s = pat.sub("", s, count=1)
    if '#include "trainers_hx.h"' not in s:
        idx = s.rstrip().rfind("};")
        s = s[:idx] + '#include "trainers_hx.h"\n' + s[idx:]
    write(p, s)
    # parties include
    p = f"{FR}/src/data/trainer_parties.h"
    s = read(p)
    if '#include "trainer_parties_hx.h"' not in s:
        s = s.rstrip("\n") + '\n\n#include "trainer_parties_hx.h"\n'
    write(p, s)
