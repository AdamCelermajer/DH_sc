#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Static skill-asset matrix generator (v21.8, skill verification).

Regenerates, from the ORIGINAL data tables and the staged package:
  coordination/v21.8/SKILL-VERIFICATION-MATRIX.md   (human matrix, one row per skill)
  coordination/v21.8/SKILL-VERIFICATION-MATRIX.json (machine-readable, same rows)

Inputs (all read-only):
  <local-inputs>/skill-tables/skills_pyarray{,names}.bin   SkillTable + per-class skill lists
  <local-inputs>/assets-extra/android/data/pydata/animations_*.bin   AnimTable sequences + clip paths
  <local-inputs>/assets-extra/android/data/pydata/effects_*.bin      FX sets + FX BDAE paths
  <repo>/port/engine-audio/reference/source-bindings-v38/ledger.json  sound ordinal -> wav filename
  <local-inputs>/windows-source-clock-v21.8-staged/assets/           staged BDAE/FX/texture files
  <local-inputs>/windows-source-clock-v21.8-staged/audio-assets/data/sounds/  staged wav files
  runtime_skill_use_profile_v1.hpp in the skills-a worktree (if present) and the
  cast whitelist in this tree's runtime_skill_cast_coordinator_v1.cpp.

Usage (run from anywhere; use -I so no local module shadows stdlib):
  py -3 -I coordination/v21.8/skill_static_check.py [--local-inputs DIR] [--check-only]

--check-only prints the summary and flags without rewriting the .md/.json files.
Exit 0 when the tables decoded and the sanity totals match; 1 otherwise.
Missing assets are FLAGS in the output, not a failure of the script.
"""

import argparse
import json
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.normpath(os.path.join(HERE, "..", ".."))
FALLBACK_PRIMARY = "C:/Users/adamc/Desktop/workspace/DH_sc"

# Tree order matters: a skill is "owned" by the first tree (in this order) that lists it.
# Class row ids are the CharacterTable rows recorded in CHARACTER-COVERAGE-MATRIX.md section 1.
TREES = [
    # (skill-list name in skills_pyarraynames, display name, kind, CharacterTable row)
    ("Knight", "Knight", "base", 263),
    ("KnightBerserker", "Berserker", "subclass of Knight", 264),
    ("KnightPaladin", "Paladin", "subclass of Knight", 265),
    ("Rogue", "Rogue", "base", 325),
    ("RogueArcher", "Archer", "subclass of Rogue", 326),
    ("RogueAssassin", "Assassin", "subclass of Rogue", 327),
    ("Mage", "Mage", "base", 290),
    ("MageIllusionist", "Illusionist", "subclass of Mage", 291),
    ("MageNecromancer", "Necromancer", "subclass of Mage", 292),
]

# Sanity totals from the coverage matrix (35 active / 37 passive / 72 total; 48 subclass-only).
EXPECT_TOTAL = 72
EXPECT_ACTIVE = 35
EXPECT_PASSIVE = 37
EXPECT_SUBCLASS_ONLY = 48

# Candidate links that are NOT proven by a step cue or a clip marker (inference only).
CANDIDATE_SOUNDS = {
    "ChainLightning": "candidate only, not linked: sfx_skill_mage_thunder_braid (+_add_1/_add_2/_loop) present; caller unknown",
    "Figments": "candidate only, not linked: sfx_skill_mage_ilu_nightmare present; no step or marker link",
    "GrapplingHook": "candidate only, not linked: sfx_skill_rogue_ass_throw_knife present; no step or marker link",
}

# Skill row 102 ("SpecializationSkill", no script, no anim) sits in the Knight/Rogue/Mage lists as a
# placeholder; the v21.7 inventory also skipped it. It is not a player skill.
PLACEHOLDER_SKILL_IDS = {102}

PROJECTILE_RE = re.compile(r"proj|arrow|bolt|missile|skull|orb", re.I)


class Reader(object):
    """Little-endian reader for the pyarray binaries (int32 counts, latin1 strings)."""

    def __init__(self, data):
        self.d = data
        self.at = 0

    def i32(self):
        v = struct.unpack_from("<i", self.d, self.at)[0]
        self.at += 4
        return v

    def f32(self):
        v = struct.unpack_from("<f", self.d, self.at)[0]
        self.at += 4
        return v

    def u8(self):
        v = self.d[self.at]
        self.at += 1
        return v

    def txt(self):
        k = self.i32()
        t = self.d[self.at:self.at + k]
        self.at += k
        return t.decode("latin1").rstrip("\0")

    def strs(self):
        return [self.txt() for _ in range(self.i32())]

    def ints(self):
        return [self.i32() for _ in range(self.i32())]


def read_bytes(path):
    with open(path, "rb") as fh:
        return fh.read()


def decode_skill_table(base):
    """Returns (list_names, lists[list_name] -> [skill ids], skills[id] -> dict)."""
    names = Reader(read_bytes(os.path.join(base, "skills_pyarraynames.bin")))
    list_names = names.strs()
    skill_names = names.strs()
    c = Reader(read_bytes(os.path.join(base, "skills_pyarray.bin")))
    n_lists = c.i32()
    raw_lists = [c.ints() for _ in range(n_lists)]
    n_skills = c.i32()
    skills = []
    for i in range(n_skills):
        r = {}
        r["anim_root"] = c.i32()
        r["moving"] = c.u8()
        r["disp"] = c.ints()
        r["element"] = c.i32()
        r["flag6"] = c.u8()
        r["w7"] = c.i32()
        r["level"] = c.i32()
        r["script"] = c.txt()
        r["assignable"] = c.u8()
        r["w12"] = c.i32()
        r["w13"] = c.i32()
        r["icon"] = c.txt()
        r["w16"] = c.i32()
        r["w17"] = c.i32()
        r["type_code"] = c.i32()
        r["name"] = skill_names[i] if i < len(skill_names) else "skill%d" % i
        r["id"] = i
        skills.append(r)
    lists = {}
    for idx, lname in enumerate(list_names):
        if idx < len(raw_lists):
            lists[lname] = raw_lists[idx]
    if c.at != len(c.d):
        raise ValueError("skills_pyarray.bin: %d trailing bytes" % (len(c.d) - c.at))
    return lists, skills


def decode_animations(base):
    """Returns (clip_paths, seq_names, seqs) where seqs[i] = list of step dicts."""
    dv = Reader(read_bytes(os.path.join(base, "animations_dictionary_pyarray.bin"))).strs()
    seq_names = Reader(read_bytes(os.path.join(base, "animations_pyarraynames.bin"))).strs()
    d = Reader(read_bytes(os.path.join(base, "animations_pyarray.bin")))
    n = d.i32()
    seqs = []
    for _ in range(n):
        d.i32()  # loop
        steps = []
        for _ in range(d.i32()):
            st = {}
            st["afx"] = d.u8()
            st["anim"] = d.i32()
            st["blend"] = d.i32()
            st["cam"] = d.i32()
            st["camdir"] = d.u8()
            st["fx"] = d.i32()
            st["move"] = d.u8()
            st["rcam"] = d.ints()
            st["redir"] = d.i32()
            st["snd"] = d.i32()
            st["speed"] = d.f32()
            st["swoosh"] = d.u8()
            steps.append(st)
        d.i32()  # type
        seqs.append(steps)
    # Trailing bytes hold the camera and character sections, which this checker does not use.
    return dv, seq_names, seqs


def decode_effects(base):
    """Returns (fx_paths, fx_sets) where fx_sets[i] = list of step dicts."""
    ev = Reader(read_bytes(os.path.join(base, "effects_dictionary_pyarray.bin"))).strs()
    e = Reader(read_bytes(os.path.join(base, "effects_pyarray.bin")))
    cnt = e.i32()
    sets = []
    for _ in range(cnt):
        e.u8()  # fc
        e.i32()  # loop
        steps = []
        for _ in range(e.i32()):
            s = {}
            s["file"] = e.i32()
            s["fcancel"] = e.u8()
            s["loop"] = e.i32()
            s["oo"] = e.u8()
            s["owa"] = e.u8()
            s["pt"] = e.i32()
            s["pool"] = e.i32()
            s["redir"] = e.i32()
            s["swa"] = e.u8()
            s["si"] = e.u8()
            s["speed"] = e.f32()
            s["sub"] = e.txt()
            steps.append(s)
        e.i32()  # type
        sets.append(steps)
    return ev, sets


def expand_anim(seqs, seq_id, seen=()):
    """Non-redirect steps reachable from an AnimTable sequence (matches the v21.7 inventory)."""
    out = []
    if seq_id < 0 or seq_id >= len(seqs) or seq_id in seen:
        return out
    for st in seqs[seq_id]:
        if st["redir"] == 1:
            out += expand_anim(seqs, st["anim"], seen + (seq_id,))
        else:
            out.append(st)
    return out


def expand_fx(fx_sets, set_id, seen=()):
    """FX file indices reachable from an effect set (redirects followed)."""
    out = []
    if set_id < 0 or set_id >= len(fx_sets) or set_id in seen:
        return out
    for s in fx_sets[set_id]:
        if s["redir"] == 1:
            out += expand_fx(fx_sets, s["file"], seen + (set_id,))
        elif 0 <= s["file"]:
            out.append(s["file"])
    return out


def dedupe(seq):
    seen = set()
    out = []
    for x in seq:
        if x not in seen:
            seen.add(x)
            out.append(x)
    return out


def index_tree(root):
    """Lowercase relative path -> real path, for case-insensitive lookups (data/3D vs data/3d)."""
    idx = {}
    if not os.path.isdir(root):
        return idx
    for dp, _dn, fn in os.walk(root):
        for f in fn:
            full = os.path.join(dp, f)
            rel = os.path.relpath(full, root).replace("\\", "/").lower()
            idx[rel] = full
    return idx


def first_existing(paths):
    for p in paths:
        if p and os.path.exists(p):
            return p
    return None


def find_local_inputs(arg):
    cands = [arg, os.environ.get("DH_LOCAL_INPUTS"), os.path.join(REPO, ".local-inputs"),
             os.path.join(FALLBACK_PRIMARY, ".local-inputs")]
    for c in cands:
        if c and os.path.isdir(os.path.join(c, "skill-tables")):
            return c
    return None


def parse_profile_header(path):
    """SkillUseProfile rows: {skill_name, script, class_token, pre_range, ...}."""
    rows = {}
    if not path or not os.path.exists(path):
        return rows
    text = open(path, encoding="utf-8", errors="replace").read()
    for m in re.finditer(r'\{"(\w+)",\s*"(\w+)",\s*"(\w+)",\s*([\d.]+)f', text):
        rows[m.group(2)] = {"skill_name": m.group(1), "class_token": m.group(3),
                            "pre_range": float(m.group(4))}
    return rows


def build(local_inputs, check_only, out_dir):
    flags = []
    skill_base = os.path.join(local_inputs, "skill-tables")
    pydata = os.path.join(local_inputs, "assets-extra", "android", "data", "pydata")
    staged = os.path.join(local_inputs, "windows-source-clock-v21.8-staged")
    assets_idx = index_tree(os.path.join(staged, "assets"))
    sounds_idx = index_tree(os.path.join(staged, "audio-assets", "data", "sounds"))
    ledger_path = first_existing([
        os.path.join(REPO, "port", "engine-audio", "reference", "source-bindings-v38", "ledger.json"),
        os.path.join(FALLBACK_PRIMARY, "port", "engine-audio", "reference", "source-bindings-v38", "ledger.json")])
    ledger = json.load(open(ledger_path, encoding="utf-8")) if ledger_path else []

    lists, skills = decode_skill_table(skill_base)
    dv, seq_names, seqs = decode_animations(pydata)
    ev, fx_sets = decode_effects(pydata)

    header_a = first_existing([
        os.path.join(REPO, ".claude", "worktrees", "skills-a", "port", "windows-foundation",
                     "features", "generic_skills", "runtime_skill_use_profile_v1.hpp"),
        os.path.join(FALLBACK_PRIMARY, ".claude", "worktrees", "skills-a", "port", "windows-foundation",
                     "features", "generic_skills", "runtime_skill_use_profile_v1.hpp")])
    profiles = parse_profile_header(header_a)
    coord = os.path.join(REPO, "port", "windows-foundation", "features", "generic_skills",
                         "runtime_skill_cast_coordinator_v1.cpp")
    coord_text = open(coord, encoding="utf-8", errors="replace").read() if os.path.exists(coord) else ""

    # Tree membership
    tree_lists = []
    for lname, disp, kind, row in TREES:
        if lname not in lists:
            flags.append("tree list missing from skill table: %s" % lname)
        tree_lists.append((lname, disp, kind, row, lists.get(lname, [])))

    membership = {}
    for order, (lname, disp, kind, row, ids) in enumerate(tree_lists):
        for j in ids:
            if j in PLACEHOLDER_SKILL_IDS:
                continue
            membership.setdefault(j, []).append(order)

    rows = []
    for j in sorted(membership):
        s = skills[j]
        owner_order = membership[j][0]
        owner = tree_lists[owner_order]
        is_active = s["type_code"] == 0
        is_passive = s["type_code"] == -1
        kind = "active" if is_active else ("passive" if is_passive else "unknown(%d)" % s["type_code"])
        shared = [tree_lists[o][1] for o in membership[j]]
        row = {
            "skill_id": j,
            "skill": s["name"],
            "owner_tree": owner[1],
            "owner_kind": owner[2],
            "shared_in": shared,
            "type": kind,
            "level": s["level"],
            "script": s["script"],
            "icon": s["icon"],
            "anim_root": s["anim_root"],
            "moving_flag": s["moving"],
            "assignable": s["assignable"],
            "anim_clips": [],
            "anim_present": None,
            "fx_sets": [],
            "fx_files": [],
            "fx_present": None,
            "sound_cues": [],
            "sound_marker": [],
            "sound_present": None,
            "projectile": "none in AnimTable/FX",
            "cast_mode": "n/a (passive)" if is_passive else "unknown (player Lua not in package)",
            "port_status": "",
            "static_ok": None,
            "runtime_verified": "",
            "notes": [],
        }
        if is_active:
            steps = expand_anim(seqs, s["anim_root"]) if s["anim_root"] >= 0 else []
            clip_bad = []
            fx_ids = []
            snd_ids = []
            for st in steps:
                if st["anim"] >= 0 and st["anim"] < len(dv):
                    row["anim_clips"].append(dv[st["anim"]])
                if st["fx"] >= 0:
                    fx_ids.append(st["fx"])
                if st["snd"] >= 0:
                    snd_ids.append(st["snd"])
            row["anim_clips"] = dedupe(row["anim_clips"])
            # Clip presence (case-insensitive, data/3D -> data/3d)
            clip_ok = []
            for c in row["anim_clips"]:
                key = c.replace("\\", "/").lower()
                if key.startswith("data/") and key in assets_idx:
                    clip_ok.append(True)
                else:
                    clip_ok.append(False)
                    clip_bad.append(c)
            row["anim_present"] = bool(row["anim_clips"]) and not clip_bad
            if not row["anim_clips"]:
                row["anim_present"] = False
                flags.append("%s: no AnimTable clip resolved (root %d)" % (s["name"], s["anim_root"]))
            if clip_bad:
                flags.append("%s: clip(s) missing from package: %s" % (s["name"], ", ".join(clip_bad)))

            # FX
            fx_ids = dedupe(fx_ids)
            row["fx_sets"] = fx_ids
            fx_paths = []
            for fid in fx_ids:
                for fi in expand_fx(fx_sets, fid):
                    if 0 <= fi < len(ev):
                        fx_paths.append(ev[fi])
            fx_paths = dedupe(fx_paths)
            row["fx_paths"] = fx_paths
            row["fx_files"] = [os.path.basename(p) for p in fx_paths]
            fx_missing = [p for p in fx_paths if p.replace("\\", "/").lower() not in assets_idx]
            if not fx_ids:
                row["fx_present"] = None  # no FX in the AnimTable: not an asset gap
                row["notes"].append("no FX set in AnimTable (not counted as missing)")
            else:
                row["fx_present"] = not fx_missing
                if fx_missing:
                    flags.append("%s: FX BDAE missing from package: %s" %
                                 (s["name"], ", ".join(os.path.basename(p) for p in fx_missing)))
            if any(PROJECTILE_RE.search(os.path.basename(p)) for p in fx_paths):
                row["projectile"] = "heuristic: FX name suggests projectile (%s)" % \
                    ", ".join(os.path.basename(p) for p in fx_paths if PROJECTILE_RE.search(os.path.basename(p)))

            # Sounds: step cues (ordinal -> ledger wav) and clip markers (sfx_ strings in BDAE)
            cues = []
            for sid in dedupe(snd_ids):
                if sid < len(ledger):
                    entry = ledger[sid]
                    fn = entry.get("target", {}).get("filename", "")
                    present = fn.lower() in sounds_idx
                    cues.append({"ordinal": sid, "uid": entry.get("uid"), "source": entry.get("source_name"),
                                 "file": fn, "present": present})
                    if not present:
                        flags.append("%s: sound file missing from staged package: %s" % (s["name"], fn))
            row["sound_cues"] = cues
            markers = []
            for c in row["anim_clips"]:
                real = assets_idx.get(c.replace("\\", "/").lower())
                if not real:
                    continue
                data = read_bytes(real)
                for m in set(re.findall(rb"sfx_[A-Za-z0-9_]{3,}", data)):
                    # BDAE strings carry a doubled prefix ("sfx_sfx_skill_..."); the wav is "sfx_skill_...".
                    name = re.sub(r"^(sfx_)+", "sfx_", m.decode("ascii"))
                    fn = name + ".wav"
                    markers.append({"name": name, "file": fn, "present": fn.lower() in sounds_idx})
            seen_m = set()
            row["sound_marker"] = []
            for mk in markers:
                if mk["name"] not in seen_m:
                    seen_m.add(mk["name"])
                    row["sound_marker"].append(mk)
            linked_ok = [c for c in cues if c["present"]]
            marker_ok = [m for m in row["sound_marker"] if m["present"]]
            if linked_ok:
                row["sound_present"] = True
            elif marker_ok:
                row["sound_present"] = True
                row["notes"].append("sound from clip marker only (not a step cue)")
            else:
                row["sound_present"] = False
                if s["name"] in CANDIDATE_SOUNDS:
                    row["notes"].append(CANDIDATE_SOUNDS[s["name"]])
                flags.append("%s: no resolved sound cue (step cues: %d, markers: %d)" %
                             (s["name"], len(cues), len(row["sound_marker"])))

            # Cast mode / port status from the skills-a profile header when present
            prof = profiles.get(s["script"])
            if prof:
                if prof["pre_range"] > 0:
                    row["cast_mode"] = "target (OnPre range search %g) [skills-a profile]" % prof["pre_range"]
                else:
                    row["cast_mode"] = "area/use-time search, no OnPre target query [skills-a profile]"
            status = []
            if s["script"] and ('"%s"' % s["script"]) in coord_text:
                status.append("CAST in this branch (whitelist, runtime_skill_cast_coordinator_v1.cpp)")
            if prof:
                status.append("skills-a profile row (runtime_skill_use_profile_v1.hpp, branch v21.8-skills-a; not merged here)")
            if not status:
                status.append("missing: no cast path in this branch")
            row["port_status"] = "; ".join(status)
            if s["moving"] == 1:
                row["notes"].append("table moving flag=1")

            # Static verdict for an active skill
            static_gaps = []
            if not row["anim_present"]:
                static_gaps.append("anim")
            if row["fx_present"] is False:
                static_gaps.append("fx")
            if not row["sound_present"]:
                static_gaps.append("sound")
            row["static_ok"] = not static_gaps
            row["static_gaps"] = static_gaps
        else:
            row["anim_present"] = None
            row["fx_present"] = None
            row["sound_present"] = None
            if is_passive:
                row["port_status"] = "passive: no gameplay effect in code (rank/text only per matrix)"
                row["static_ok"] = None
                row["static_gaps"] = []
            else:
                row["port_status"] = "unknown table type"
                row["static_ok"] = None
                row["static_gaps"] = []
        rows.append(row)

    # Summaries
    unique_total = len(rows)
    active_total = sum(1 for r in rows if r["type"] == "active")
    passive_total = sum(1 for r in rows if r["type"] == "passive")
    subclass_only = sum(1 for r in rows if r["owner_kind"] != "base" and len(r["shared_in"]) == 1)
    if unique_total != EXPECT_TOTAL or active_total != EXPECT_ACTIVE or passive_total != EXPECT_PASSIVE \
            or subclass_only != EXPECT_SUBCLASS_ONLY:
        flags.append("SANITY MISMATCH vs coverage matrix: total %d (exp %d), active %d (exp %d), "
                     "passive %d (exp %d), subclass-only %d (exp %d)" %
                     (unique_total, EXPECT_TOTAL, active_total, EXPECT_ACTIVE, passive_total,
                      EXPECT_PASSIVE, subclass_only, EXPECT_SUBCLASS_ONLY))
    tree_summary = []
    for lname, disp, kind, row_id, ids in tree_lists:
        trows = [r for r in rows if disp in r["shared_in"]]
        act = [r for r in trows if r["type"] == "active"]
        psv = [r for r in trows if r["type"] == "passive"]
        ok = [r for r in act if r["static_ok"]]
        tree_summary.append({"tree": disp, "kind": kind, "character_row": row_id,
                             "skills": len(trows), "active": len(act), "passive": len(psv),
                             "static_complete_active": len(ok),
                             "missing_assets": [r["skill"] for r in act if not r["static_ok"]]})

    result = {
        "generator": "coordination/v21.8/skill_static_check.py",
        "inputs": {
            "local_inputs": local_inputs,
            "skill_table": os.path.join(skill_base, "skills_pyarray.bin"),
            "animations": os.path.join(pydata, "animations_pyarray.bin"),
            "effects": os.path.join(pydata, "effects_pyarray.bin"),
            "ledger": ledger_path,
            "staged_assets_files": len(assets_idx),
            "staged_sound_files": len(sounds_idx),
            "skills_a_profile_header": header_a,
            "base_cast_whitelist_checked": os.path.relpath(coord, REPO) if os.path.exists(coord) else None,
        },
        "totals": {"unique_skills": unique_total, "active": active_total, "passive": passive_total,
                   "subclass_only": subclass_only},
        "trees": tree_summary,
        "flags": flags,
        "skills": rows,
    }
    if check_only:
        print_summary(result)
        return result

    os.makedirs(out_dir, exist_ok=True)
    with open(os.path.join(out_dir, "SKILL-VERIFICATION-MATRIX.json"), "w", encoding="utf-8") as fh:
        json.dump(result, fh, indent=2)
        fh.write("\n")
    with open(os.path.join(out_dir, "SKILL-VERIFICATION-MATRIX.md"), "w", encoding="utf-8", newline="\n") as fh:
        fh.write(render_markdown(result))
    print_summary(result)
    return result


def print_summary(result):
    t = result["totals"]
    print("unique skills %d (active %d, passive %d, subclass-only %d)" %
          (t["unique_skills"], t["active"], t["passive"], t["subclass_only"]))
    for ts in result["trees"]:
        print("  %-12s %-18s skills %2d active %2d passive %2d static-complete %d" %
              (ts["tree"], ts["kind"], ts["skills"], ts["active"], ts["passive"], ts["static_complete_active"]))
    print("flags: %d" % len(result["flags"]))
    for f in result["flags"]:
        print("  FLAG " + f)


CORRECTIONS_MD = """## Corrections found by this check (versus CHARACTER-COVERAGE-MATRIX.md)

- Skill FX BDAEs: the coverage matrix says only BashDown FX164 is staged. The staged package has the
  FX BDAE for every FX-bearing active skill at `assets/data/3d/interface/skill_dh2_prince_*.bdae`
  (the rows below show "present"). Presence is confirmed here; whether the runtime loads and dispatches
  each FX is still unverified (see runtime-verified column).
- Skill sounds: 32 of 35 active skills resolve a cue or clip marker, matching the coverage matrix count.
  Unresolved: ChainLightning, Figments, GrapplingHook. Six archer and rogue skills resolve only through
  `sfx_*` markers inside their clip BDAEs (`sfx_sfx_` doubled prefix in the binary; normalised here).
- Subclass names from the data: Knight -> Berserker, Paladin; Rogue -> Archer, Assassin; Mage -> Illusionist,
  Necromancer. The base class token in the data is "Knight" (script prefix `prince_warrior_*`). There is no
  Elementalist subclass in the data.
- Data spellings kept as-is: skill 126 is `blade_defect` (script `prince_assassin_blade_deflect`); skill 105 is
  `Survival`; `prince_berseeker_*` is the script prefix for the Berserker tree.
- GroundSlam's AnimTable root 350 resolves to `skill_dh2_prince_warrior_crushing_blow.bdae` (the clip is named for
  crushing blow); the FX set 173 uses `skill_dh2_prince_warrior_ground_slam.bdae`.
- Skill row 102 ("SpecializationSkill", no script, no anim) is a placeholder in the base class lists and is
  excluded from the counts.
- Cast-whitelist and skills-a profile status are per-script string checks, not a parse of the C++ logic.
"""

STEP3_MD = """## Step 3: test-only cast-any-skill design (DESIGN ONLY, not implemented)

Findings from the code on this branch:

- Existing test flags in `port/windows-foundation/main.cpp`: `--test-player-hp N[@FRAME]` (I024),
  `--test-xp FRAME:AMOUNT` (B157), `--test-skill-points N` and `--test-stat-points N` (B096/B103, applied at
  drawn==60 through the profile projection), `--skill-key-frame FRAME:KEY` (press hotbar key 1..5 at FRAME),
  `--skills-page-frame`, `--capture-at`. There is no grant-skill, class/subclass or level flag.
- Cast rejection is NOT a level or specialization gate. `features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`
  (the `source_supported_skill` check before the Use arm, about line 706) returns `phase=rejected` for any
  script outside the four whitelisted scripts (lines 124-141: BashDown, GroundSlam, Charge, JumpKick).
  A grant, a forced level 12+ or a forced subclass therefore cannot make the other 31 active skills castable.
  Each needs a per-skill use profile (skills-a `runtime_skill_use_profile_v1.hpp` has 6 rows) and a general Use arm.
- Consequence: a `--test-grant-skill` flag is NOT a tiny safe change and was NOT added. Adding one now would
  look like it works for the 31 skills while every cast is still rejected.

Proposed flags, to implement after the profile table is merged into this branch:

1. `--test-grant-skill NAME[:RANK[:KEY]]` (TEST ONLY, same frame hook as the B103 grant): sets the saved skill
   row rank to RANK (default 1) through the training-commit path (main.cpp near the "Source skill training row"
   log, about line 4486) and puts the skill on hotbar KEY (1..5). Refuses passives for KEY, unknown names and
   duplicate keys. Never in startup.args.
2. `--test-class-subclass ROW`: overrides the saved CharacterTable row (263..327) for tree membership only.
   It does not implement the level-12 specialization flow, which the port does not have.
3. `--test-level N`: raises the saved SnS_Level for display checks only. Cast gating does not read it today.
4. Unit test (CTest): parse each flag, reject bad input (unknown skill, passive KEY, KEY 0 or 6, duplicate key),
   and check that the grant writes the expected saved row for one whitelisted and one profile-only skill.

Real-run verification: `skill_cast_runner_template.ps1` casts each active skill in a hidden-window run
(`-Reps N`, three hotbar presses per run at `-FirstCastFrame` spaced by `-CastSpacingFrames`) and records:
accepted (`Source skill key=... phase=P skill=...`, accepted when P != 0), FX (`Source player step FX ... dispatched=1`),
and sound (`Container sound ordinal=... status=submitted` or `Skill container sound:`). It writes `cast-runs.json`,
`summary.csv` and frame captures under `C:\\dh218\\skillcast\\<stamp>\\<skill>\\rep<k>\\`. Today it reports every
non-whitelisted skill as BLOCKED without launching. Dry run: `-DryRun`.

Limits: no per-skill sound line exists in the code, so the sound column is the closest generic line
(`Container sound ordinal`). Cooldowns are not in the matrix, so `-CastSpacingFrames` must be set by hand.
A `status=submitted` line only shows the audio request was accepted, not that anything was heard.

Runtime-verified column: blank for every row. No skill was launched in this session.
"""

def md_cell(text):
    return str(text).replace("|", "/").replace("\n", " ")


def render_markdown(result):
    lines = []
    lines.append("# Skill verification matrix (v21.8)")
    lines.append("")
    lines.append("Generated by `coordination/v21.8/skill_static_check.py` (do not hand-edit; rerun the script).")
    lines.append("Static columns come from the original tables and the staged package. The runtime-verified")
    lines.append("column is blank until a hidden-window cast run confirms it (see the Step 3 section below).")
    lines.append("")
    lines.append("## Sources and method")
    lines.append("")
    lines.append("- Skill table: `.local-inputs/skill-tables/skills_pyarray.bin` (per-class skill lists + 103 skill rows).")
    lines.append("- AnimTable: `assets-extra/android/data/pydata/animations_pyarray.bin` (byte-identical to `.local-inputs/actors/`).")
    lines.append("- FX sets: `assets-extra/android/data/pydata/effects_pyarray.bin` (byte-identical to `.local-inputs/loot-fx-v32-assets/`).")
    lines.append("- Sound ordinal -> wav: `port/engine-audio/reference/source-bindings-v38/ledger.json` (`target.filename`).")
    lines.append("- Presence: staged package `.local-inputs/windows-source-clock-v21.8-staged/assets` (case-insensitive) and `audio-assets/data/sounds`.")
    lines.append("- Clip markers: `sfx_*` strings found inside each staged clip BDAE (a second sound source, reported separately).")
    lines.append("- Tree ownership: the first class/subclass list (order: Knight, Berserker, Paladin, Rogue, Archer, Assassin, Mage, Illusionist, Necromancer) that contains the skill; base skills shared by a subclass are listed in `shared in`.")
    lines.append("- Cast mode: only known for the six skills that have a skills-a profile row (OnPre query present or absent). Everything else is unknown because the player skill Lua is not in the package (`assets/data/scripts/skills` has only `witch_ranged_00.luac`).")
    lines.append("- Projectile: heuristic only (FX file name contains proj/arrow/bolt/missile/skull/orb). The projectile table is not decoded.")
    lines.append("")
    lines.append(CORRECTIONS_MD.rstrip("\n"))
    lines.append("")
    lines.append(STEP3_MD.rstrip("\n"))
    lines.append("")
    t = result["totals"]
    lines.append("## Totals")
    lines.append("")
    lines.append("- Unique skills: %d (active %d, passive %d); subclass-only skills: %d." %
                 (t["unique_skills"], t["active"], t["passive"], t["subclass_only"]))
    lines.append("")
    lines.append("| Tree | Kind | CharacterTable row | Skills | Active | Passive | Active with complete static assets | Active with missing asset |")
    lines.append("|---|---|---|---|---|---|---|---|")
    for ts in result["trees"]:
        lines.append("| %s | %s | %d | %d | %d | %d | %d | %s |" %
                     (ts["tree"], ts["kind"], ts["character_row"], ts["skills"], ts["active"], ts["passive"],
                      ts["static_complete_active"], ", ".join(ts["missing_assets"]) or "none"))
    lines.append("")
    lines.append("## Flags (missing or unresolved static assets)")
    lines.append("")
    if result["flags"]:
        for f in result["flags"]:
            lines.append("- " + f)
    else:
        lines.append("- none")
    lines.append("")
    lines.append("## Matrix (one row per skill)")
    lines.append("")
    lines.append("anim = AnimTable clips present; FX = FX BDAE present (`none in table` = no FX set, not a gap); "
                 "sound = a step cue or clip marker resolves to a staged wav; static-ok = all applicable checks pass "
                 "(passives: n/a).")
    lines.append("")
    lines.append("| id | skill | tree (owner) | shared in | type | lvl | script | anim root | anim clip(s) | anim | FX set(s) | FX BDAE | sound cue(s) (ordinal, uid, wav) | sound | projectile | cast mode | port status | static-ok | runtime-verified | notes |")
    lines.append("|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|")
    for r in result["skills"]:
        clips = ", ".join(os.path.basename(c) for c in r["anim_clips"]) or "-"
        anim = "n/a" if r["type"] != "active" else ("present" if r["anim_present"] else "MISSING")
        fx_sets = ", ".join(str(x) for x in r["fx_sets"]) or "none in table"
        fx_files = ", ".join(p.replace("\\", "/").lower().split("data/3d/", 1)[-1] for p in r.get("fx_paths", [])) or "-"
        fx = "n/a" if r["type"] != "active" else (
            "none in table" if r["fx_present"] is None else ("present" if r["fx_present"] else "MISSING"))
        cues = []
        for c in r["sound_cues"]:
            cues.append("%d/uid%s %s %s" % (c["ordinal"], c["uid"], c["file"], "ok" if c["present"] else "MISSING"))
        for m in r["sound_marker"]:
            cues.append("marker %s %s" % (m["file"], "ok" if m["present"] else "MISSING"))
        snd_cell = "; ".join(cues) or "none linked"
        snd = "n/a" if r["type"] != "active" else ("present" if r["sound_present"] else "MISSING")
        static = "n/a" if r["type"] != "active" else ("yes" if r["static_ok"] else "NO (" + ", ".join(r.get("static_gaps", [])) + ")")
        notes = "; ".join(r["notes"])
        lines.append("| %s | %s | %s (%s) | %s | %s | %d | %s | %d | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | | %s |" % (
            r["skill_id"], md_cell(r["skill"]), md_cell(r["owner_tree"]), r["owner_kind"],
            md_cell(", ".join(r["shared_in"])), r["type"], r["level"], md_cell(r["script"]), r["anim_root"],
            md_cell(clips), anim, md_cell(fx_sets), md_cell(fx_files), md_cell(snd_cell), snd,
            md_cell(r["projectile"]), md_cell(r["cast_mode"]), md_cell(r["port_status"]), static,
            md_cell(notes)))
    lines.append("")
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--local-inputs", default=None, help="path to .local-inputs (auto-detected if omitted)")
    ap.add_argument("--out-dir", default=HERE, help="where to write the .md/.json (default: this folder)")
    ap.add_argument("--check-only", action="store_true", help="print summary and flags, write nothing")
    args = ap.parse_args()
    li = find_local_inputs(args.local_inputs)
    if not li:
        print("ERROR: .local-inputs with skill-tables not found; pass --local-inputs", file=sys.stderr)
        return 1
    try:
        result = build(li, args.check_only, args.out_dir)
    except (OSError, ValueError, KeyError, struct.error) as exc:
        print("ERROR: %s" % exc, file=sys.stderr)
        return 1
    sane = result["totals"]["unique_skills"] == EXPECT_TOTAL
    return 0 if sane else 1


if __name__ == "__main__":
    sys.exit(main())
