#!/usr/bin/env python3
"""v21.8 weapon static verification (Step 2 of the weapon matrix task).

Read-only. Decodes the ORIGINAL pydata tables (ItemTable = loot_table_*, AnimTable /
CharAnimTable = animations_*, CharSounds = sounds_pyarray first table, clip dictionary),
the original melee bindings (clip markers), the original soundpack XML, and the staged
v21.8 package; then checks asset presence and the port's wiring by source probes.

Writes WEAPON-VERIFICATION-MATRIX.md and WEAPON-VERIFICATION-MATRIX.json next to this file.

Usage:
  py -3 weapon_static_check.py --primary C:/Users/adamc/Desktop/workspace/DH_sc \
      --port <worktree>/port/windows-foundation

Paths default to the primary checkout's .local-inputs (absolute, so no junctions are needed).
Stance rules and record layout are transcribed from the port sources cited in PROBES and in
the markdown output. Anything not read from data is labelled INFERENCE in the output.
"""
import argparse
import collections
import json
import os
import re
import struct
import sys
import xml.etree.ElementTree as ET

CLASS_ORDER = ["Warrior", "Mage", "Rogue", "Paladin", "Berserker", "Assassin", "Archer",
               "Necromancer", "Illusionist"]  # ItemTable *EquipValue schema order (base_fields 6..14)
PLAYER_TABLES = [("KnightPlayerBase", 48, "PlayerKnight"), ("MagePlayerBase", 49, "PlayerMage"),
                 ("RoguePlayerBase", 50, "PlayerRogue")]  # actor-profiles-v2.xml animationTable
ATTACK_BASE = 248   # AnimTable sequence of CharAnimTable.Attack (all three player tables)
STATIC_BASE = 243   # CharAnimTable.AttackStatic
STANCE_NAMES = {0: "one-hand (main hand, no dual/shield-less stance rule)", 1: "two-hander",
                2: "dual wield (main + non-shield off-hand)", 3: "staff/wand (ranged 2H)",
                4: "bow (ranged 2H)", 5: "bare-handed (no main hand)"}
TYPE_NAMES = {0: "1H weapon (slot 1)", 2: "2H weapon (slot -4)", 3: "claw/dagger (slot -3)",
              4: "bow (slot -4)", 5: "staff/wand (slot -4)", 6: "shield (slot 2)"}

STEP_F = ["AnchorFX", "Anim", "BlendOut", "Cam", "CamDir", "FX", "MoveGO", "RandomCam", "Redir", "Sound", "Speed", "Swoosh"]
SEQ_F = ["Loop", "Steps", "Type"]
CAM_F = ["CamAnims", "Crit", "Idle", "Shake", "Template"]
STATE_F = ["Attack", "AttackStatic", "Blocking", "DeadlyGreatKB", "Despawn", "DespawnGreatKB", "Died", "Dodging",
           "GreatKnockedBack", "Idle", "IdleFromOOC", "IdleOOC", "IdleSneak", "IdleToOOC", "Injured", "Interact",
           "KnockedBack", "LiftDrop", "LiftIdle", "LiftMove", "Limbus", "MenuIdle", "MenuOnSelect", "PreSpawn",
           "Revived", "Reviving", "Run", "Run180", "RunSneak", "Scared", "Spawn", "Spells", "Stunned", "Template",
           "Walk", "Walk180", "WalkSneak"]

# Port wiring probes: (id, file relative to port root, regex, meaning). Counts and first line are recorded.
PROBES = [
    ("stance_resolver", "level-world/character_stance.cpp", r"candidate=[0-9]", "GetAnimStance: staff>bow>dual>two-hander>bare"),
    ("equipment_queries", "level-world/player_equipment_queries_v1.cpp", r"query_two_effective|main_category==5", "main/off/two-hand/staff/bow flags from ItemTable words"),
    ("locomotion_stance_select", "windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp", r"stance_variants_enabled", "Idle/Walk/Run stance sequence = base + stance"),
    ("attack_bank_planner", "windows-foundation/features/combat/runtime_player_profile_attack_bank_v1.cpp", r"plan_runtime_player_profile_attack_bank_v1", "generic Attack/AttackStatic bank planner (any profile)"),
    ("attack_bank_class_gate", "windows-foundation/main.cpp", r'class_id=="RoguePlayerBase"&&locomotionLibrary', "production attack-bank gate: Rogue only"),
    ("attack_bank_startup_only", "windows-foundation/main.cpp", r"plan_runtime_player_profile_attack_bank_v1\(assets,\*sourceProfile", "bank planned once from startup hands (no rebuild on equip)"),
    ("non_knight_animation_only", "windows-foundation/main.cpp", r'selected->class_id!="KnightPlayerBase"', "non-Knight profiles are animation-only (no outgoing attack)"),
    ("player_ranged_launch", "windows-foundation/main.cpp", r"launchPlayerRangedProjectile", "player ranged projectile launcher (Mage/Rogue bow) in production"),
    ("projectile_executor_cmake", "windows-foundation/CMakeLists.txt", r"runtime_session_projectile_v1", "shared runtime session projectile executor registered in CMake"),
    ("runtime_attack_sound", "windows-foundation/main.cpp", r"RuntimeAttackSound", "features/audio/runtime_attack_sound_v1 (also reads words 5/6): not constructed in main.cpp"),
    ("runtime_swing_fx", "windows-foundation/main.cpp", r"make_shared<f::effects::RuntimeSwingFxObserverV1>", "swing FX observer constructed (passes item words 5/6 to the AnimTable FX path)"),
    ("charsounds_tables", "level-world/character_combat_sound_tables_v2.cpp", r"CharSounds", "CharSounds rows (death/hit/impact lists) loaded"),
    ("swoosh_v4", "level-world/character_animation_swoosh_v4.cpp", r"Swoosh", "AnimStep Swoosh flag handler"),
    ("projectile_render_test", "windows-foundation/CMakeLists.txt", r"runtime_session_projectile_render", "projectile render feature test registered in CMake (no production consumer)"),
    ("combat_main_item_flag", "windows-foundation/main.cpp", r'"--combat-main-item"', "test flag: main-hand ItemTable identifier for startup locomotion/stance"),
    ("combat_off_item_flag", "windows-foundation/main.cpp", r'"--combat-off-item"', "test flag: off-hand ItemTable identifier (absent when no match)"),
    ("test_player_hp_flag", "windows-foundation/main.cpp", r'"--test-player-hp"', "test flag: player HP override for survival in runs"),
]


class Rd:
    def __init__(self, buf):
        self.buf = buf
        self.o = 0

    def w(self):
        v = struct.unpack_from("<I", self.buf, self.o)[0]
        self.o += 4
        return v

    def i(self):
        v = struct.unpack_from("<i", self.buf, self.o)[0]
        self.o += 4
        return v

    def f(self):
        v = struct.unpack_from("<f", self.buf, self.o)[0]
        self.o += 4
        return v

    def b(self):
        v = self.buf[self.o]
        self.o += 1
        if v > 1:
            raise ValueError("invalid boolean")
        return v != 0

    def cnt(self):
        return self.w()

    def ints(self):
        return [self.i() for _ in range(self.cnt())]

    def strs(self):
        out = []
        for _ in range(self.cnt()):
            n = self.w()
            out.append(self.buf[self.o:self.o + n].decode("latin-1"))
            self.o += n
        return out


def read(path):
    with open(path, "rb") as handle:
        return handle.read()


def load_items(pydata):
    """Mirror of game-data/items.cpp load_items (records/names/fields)."""
    r = Rd(read(os.path.join(pydata, "loot_table_pyarray.bin")))
    for _ in range(r.cnt()):
        n = r.cnt()
        r.o += n * 4
    n = r.cnt()
    r.o += n * 2
    for _ in range(r.cnt()):
        n = r.cnt()
        r.o += n * 7

    def text(rr):
        n = rr.i()
        s = rr.buf[rr.o:rr.o + n]
        rr.o += n
        return s.decode("latin-1")

    rows = []
    for _ in range(r.cnt()):
        w = [0] * 41
        w[1] = text(r)
        for k in range(3, 7):
            w[k] = r.i()
        w[7] = r.buf[r.o]
        r.o += 1
        for k in range(8, 19):
            w[k] = r.i()
        w[19] = text(r)
        for k in range(21, 41):
            w[k] = r.i()
        rows.append({"w": w, "icon": w[1], "name": w[19]})
    names = Rd(read(os.path.join(pydata, "loot_table_pyarraynames.bin")))
    for _ in range(3):
        names.strs()
    ids = names.strs()
    if len(ids) != len(rows):
        raise SystemExit("ItemTable identifiers/records differ")
    for row, ident in zip(rows, ids):
        row["id"] = ident
    fields = Rd(read(os.path.join(pydata, "loot_table_pystructnames.bin")))
    base = fields.strs()
    schema = fields.strs()
    if len(base) != 15 or len(schema) != 38:
        raise SystemExit("ItemTable schema dimensions differ")
    return rows, schema


def f32(bits):
    return struct.unpack("<f", struct.pack("<i", bits))[0]


def load_anim(pydata):
    k = Rd(read(os.path.join(pydata, "animations_pyarraynames.bin")))
    seq_names, cam_names, char_names = k.strs(), k.strs(), k.strs()
    lay = Rd(read(os.path.join(pydata, "animations_pystructnames.bin")))
    if [lay.strs(), lay.strs(), lay.strs(), lay.strs()] != [STEP_F, SEQ_F, CAM_F, STATE_F]:
        raise SystemExit("AnimTable schema differs")
    d = Rd(read(os.path.join(pydata, "animations_pyarray.bin")))
    if d.cnt() != len(seq_names):
        raise SystemExit("AnimTable sequence dimensions differ")
    seqs = []
    for _ in seq_names:
        loop = d.i()
        steps = []
        for _ in range(d.cnt()):
            st = {"anchor": d.b(), "anim": d.i(), "blend": d.i(), "cam": d.i(), "camdir": d.b(), "fx": d.i(),
                  "movego": d.b()}
            st["randomcam"] = d.ints()
            st["redir"] = d.i()
            st["sound"] = d.i()
            st["speed"] = d.f()
            st["swoosh"] = d.b()
            steps.append(st)
        seqs.append({"name": None, "loop": loop, "steps": steps, "type": d.i()})
    if d.cnt() != len(cam_names):
        raise SystemExit("camera dimensions differ")
    for _ in cam_names:
        d.ints(); d.i(); d.i(); d.i(); d.i()
    if d.cnt() != len(char_names):
        raise SystemExit("character dimensions differ")
    chars = []
    for _ in char_names:
        row = {}
        for idx, name in enumerate(STATE_F):
            row[name] = d.ints() if idx in (15, 31) else [d.i()]
        chars.append(row)
    if d.o != len(d.buf):
        raise SystemExit("AnimTable suffix differs")
    for idx, name in enumerate(seq_names):
        seqs[idx]["name"] = name
    dn = Rd(read(os.path.join(pydata, "animations_dictionary_pyarraynames.bin"))).strs()
    dv = Rd(read(os.path.join(pydata, "animations_dictionary_pyarray.bin"))).strs()
    return seqs, chars, char_names, dn, dv


def load_charsounds(pydata):
    raw = read(os.path.join(pydata, "sounds_pyarray.bin"))
    r = Rd(raw)
    rows = []
    for _ in range(r.cnt()):
        lists = []
        for _ in range(4):
            n = r.cnt()
            lists.append([r.i() for _ in range(n)])
        flesh, metal = raw[r.o], raw[r.o + 1]
        r.o += 2
        rows.append({"death": lists[0], "hit": lists[1], "impact1": lists[2], "impact2": lists[3]})
    names = Rd(read(os.path.join(pydata, "sounds_pyarraynames.bin"))).strs()
    for idx, row in enumerate(rows):
        row["name"] = names[idx] if idx < len(names) else "?"
    return rows


def load_soundpack(path):
    out = {}
    if not os.path.exists(path):
        return out
    root = ET.parse(path).getroot()
    for sound in root.iter("sound"):
        out[int(sound.get("uid"))] = {"label": sound.get("label"), "file": sound.get("filename"),
                                      "bank": sound.get("bank")}
    return out


def load_melee(path):
    actors, clips = {}, {}
    if not os.path.exists(path):
        return actors, clips
    root = ET.parse(path).getroot()
    for actor in root.iter("actor"):
        if actor.get("id") not in [p[0] for p in PLAYER_TABLES]:
            continue
        states = {}
        for state in actor.iter("state"):
            seqs = []
            for seq in state.iter("sequence"):
                seqs.append({"id": int(seq.get("id")), "name": seq.get("name")})
            states[state.get("name")] = seqs
        actors[actor.get("id")] = states
    for clip in root.iter("clip"):
        markers = [m.get("name") + "@" + m.get("timeMs") for m in clip.iter("marker")]
        clips[clip.get("uri").lower()] = {"start": clip.get("startMs"), "end": clip.get("endMs"), "markers": markers}
    return actors, clips


def index_files(roots):
    """Case-insensitive basename index across several roots (flattened staged layout)."""
    index = collections.defaultdict(list)
    relative = {}
    for root in roots:
        if not root or not os.path.isdir(root):
            continue
        for dirpath, _dirs, files in os.walk(root):
            for name in files:
                full = os.path.join(dirpath, name)
                index[name.lower()].append(full)
                relative[os.path.relpath(full, root).replace("\\", "/").lower()] = full
    return index, relative


def find_asset(uri, index, relative):
    norm = uri.replace("\\", "/").lower()
    if norm.startswith("data/"):
        norm = norm[5:]
    if norm in relative:
        return relative[norm]
    base = os.path.basename(norm)
    hits = index.get(base, [])
    return hits[0] if hits else None


def decode_sequence(seqs, chars, char_names, clip_names, clip_values, seq_id, depth=0, seen=None):
    seen = seen or set()
    if seq_id in seen or depth > 3 or not (0 <= seq_id < len(seqs)):
        return {"id": seq_id, "error": "out of range or cycle"}
    seen = seen | {seq_id}
    seq = seqs[seq_id]
    phases = []
    for step in seq["steps"]:
        entry = {"redir": step["redir"], "speed": round(step["speed"], 3), "sound_row": step["sound"],
                 "fx": step["fx"], "swoosh": step["swoosh"], "movego": step["movego"]}
        if step["redir"] == 1:
            entry["sub"] = decode_sequence(seqs, chars, char_names, clip_names, clip_values, step["anim"],
                                           depth + 1, seen)
        else:
            idx = step["anim"]
            entry["clip"] = clip_values[idx] if 0 <= idx < len(clip_values) else None
            entry["clip_alias"] = clip_names[idx] if 0 <= idx < len(clip_names) else None
        phases.append(entry)
    return {"id": seq_id, "name": seq["name"], "loop": seq["loop"], "type": seq["type"], "phases": phases}


def flatten_clips(node, out):
    if "clip" in node and node["clip"]:
        out.append(node["clip"])
    for ph in node.get("phases", []):
        if "sub" in ph:
            flatten_clips(ph["sub"], out)
        elif ph.get("clip"):
            out.append(ph["clip"])
    return out


def count_phases(node):
    n = 0
    for ph in node.get("phases", []):
        if "sub" in ph:
            n += 1 + count_phases(ph["sub"])
    return n


def run_probes(port_root):
    results = []
    for pid, rel, pattern, meaning in PROBES:
        path = os.path.join(port_root, rel)
        entry = {"id": pid, "file": rel.replace("\\", "/"), "pattern": pattern, "meaning": meaning,
                 "hits": 0, "first_line": None, "file_present": os.path.exists(path)}
        if os.path.exists(path):
            hits = []
            with open(path, encoding="utf-8", errors="replace") as handle:
                for num, line in enumerate(handle, 1):
                    if re.search(pattern, line):
                        hits.append(num)
            entry["hits"] = len(hits)
            entry["first_line"] = hits[0] if hits else None
        results.append(entry)
    return results


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--primary", default="C:/Users/adamc/Desktop/workspace/DH_sc")
    ap.add_argument("--port", required=True, help="worktree 'port' directory (contains windows-foundation/, level-world/)")
    ap.add_argument("--out", default=os.path.dirname(os.path.abspath(__file__)))
    args = ap.parse_args()
    li = os.path.join(args.primary, ".local-inputs")
    pydata = os.path.join(li, "assets-extra", "android", "data", "pydata")
    android_data = os.path.join(li, "assets-extra", "android", "data")
    staged = os.path.join(li, "windows-source-clock-v21.8-staged")
    assets = os.path.join(staged, "assets")
    audio = os.path.join(staged, "audio-assets")
    sound_xml = os.path.join(li, "sounds-original-v4.xml")
    melee_xml = os.path.join(assets, "original-melee-bindings.xml")
    proj_extra = os.path.join(li, "runtime-session-projectile-v1", "assets")

    rows, _schema = load_items(pydata)
    seqs, chars, char_names, clip_names, clip_values = load_anim(pydata)
    charsounds = load_charsounds(pydata)
    soundpack = load_soundpack(sound_xml)
    melee_actors, melee_clips = load_melee(melee_xml)
    idx_assets, rel_assets = index_files([assets, android_data])
    idx_staged, rel_staged = index_files([assets])
    idx_audio, rel_audio = index_files([audio])
    idx_proj, rel_proj = index_files([os.path.join(assets, "data", "3d", "projectiles"),
                                      os.path.join(proj_extra, "data", "3d", "projectiles")])
    proj_names = Rd(read(os.path.join(pydata, "projectiles_pyarraynames.bin"))).strs()

    # Weapon families: group by (type, slot, category, family base name).
    families = collections.OrderedDict()
    for row in rows:
        w = row["w"]
        if w[22] not in (0, 2, 3, 4, 5, 6):
            continue
        base = re.sub(r"_?(Hard|VeryHard)$", "", row["id"])
        base = re.sub(r"\d+$", "", base)
        key = (base, w[22], w[26], w[37])
        fam = families.setdefault(key, {"base": base, "type": w[22], "slot": w[26], "category": w[37],
                                        "items": [], "models": set(), "av": set(), "swoosh": set(),
                                        "swoosh_fx": set(), "eq_first": None, "level": [], "cls_req": set()})
        fam["items"].append(row["id"])
        fam["models"].add(row["name"])
        fam["av"].add(w[21])
        fam["swoosh"].add(w[5])
        fam["swoosh_fx"].add(w[6])
        fam["cls_req"].add(w[34])
        fam["level"].append(w[30])  # StrengthRequirement (LevelRequirement=w29 is 0 for all weapons)
        if fam["eq_first"] is None:
            fam["eq_first"] = [round(f32(w[8 + k]), 2) for k in range(9)]

    # Stance per family (INFERENCE from the query kernel; see PROBES).
    family_out = []
    for key, fam in families.items():
        t = fam["type"]
        if t == 5 and fam["category"] == 5:
            stance = [3]
        elif t == 4:
            stance = [4]
        elif t == 2:
            stance = [1]
        elif t == 3:
            stance = [2]  # dual only when two such items equip (claw/dagger main and off)
        elif t == 6:
            stance = [0]  # shield is never a dual stance
        else:
            stance = [0]
        models_present = []
        models_missing = []
        models_staged_missing = []
        for model in sorted(fam["models"]):
            # Item _Hard/_VeryHard rows carry matching _hard/_veryhard model names.
            fname = model.lower() + ".bdae"
            hit = find_asset(fname, idx_assets, rel_assets)
            (models_present if hit else models_missing).append(model)
            if not find_asset(fname, idx_staged, rel_staged):
                models_staged_missing.append(model)
        swoosh_ids = sorted(fam["swoosh"])
        swoosh_status = []
        for sid in swoosh_ids:
            if sid < 0:
                swoosh_status.append({"id": sid, "status": "none"})
            elif sid in soundpack:
                s = soundpack[sid]
                fpath = find_asset(s["file"], idx_audio, rel_audio)
                swoosh_status.append({"id": sid, "status": "soundpack uid -> %s (%s)" % (s["label"], s["file"]),
                                      "file_present": bool(fpath), "uid_namespace_check": "INFERENCE: label is %s" % (
                                          "music" if "Music" in s["label"] else "sfx")})
            else:
                swoosh_status.append({"id": sid, "status": "no soundpack uid"})
        family_out.append({
            "family": fam["base"], "type": t, "type_name": TYPE_NAMES.get(t, str(t)),
            "slot": fam["slot"], "category_param37": fam["category"],
            "item_count": len(fam["items"]), "item_examples": fam["items"][:3] + fam["items"][-1:],
            "stance": stance, "stance_name": [STANCE_NAMES[s] for s in stance],
            "model_names": sorted(fam["models"]),
            "models_present_in_staged_or_original": len(models_present),
            "models_missing": len(models_missing),
            "models_missing_from_staged_package": models_staged_missing,
            "missing_examples": models_missing[:3],
            "audiovisual_ids": sorted(fam["av"]),
            "swoosh_sound_ids": swoosh_status,
            "swoosh_fx_ids": sorted(fam["swoosh_fx"]),
            "class_equip_values_first_tier": dict(zip(CLASS_ORDER, fam["eq_first"])) if fam["eq_first"] else None,
            "strength_requirement_range": [min(fam["level"]), max(fam["level"])] if fam["level"] else None,
        })

    # Attack sequence per stance for the three player AnimTables.
    attack = []
    for actor_id, table, anim_name in PLAYER_TABLES:
        cidx = char_names.index(anim_name)
        row = chars[cidx]
        if row["Attack"][0] != ATTACK_BASE or row["AttackStatic"][0] != STATIC_BASE:
            attack.append({"actor": actor_id, "warning": "unexpected Attack/AttackStatic base"})
        melee_states = melee_actors.get(actor_id, {})
        for stance in range(6):
            for state, base in (("Attack", ATTACK_BASE), ("AttackStatic", STATIC_BASE)):
                sid = base + stance
                if sid >= len(seqs):
                    continue
                node = decode_sequence(seqs, chars, char_names, clip_names, clip_values, sid)
                clips = flatten_clips(node, [])
                clip_info = []
                for uri in clips:
                    key = uri.replace("\\", "/").lower()
                    mel = melee_clips.get(key) or melee_clips.get("data/" + key.lstrip("/")) or {}
                    present = find_asset(uri, idx_assets, rel_assets) is not None
                    clip_info.append({"uri": uri, "present": present, "markers": mel.get("markers", None)})
                sound_rows = sorted({ph.get("sound_row") for ph in node_iter(node)
                                     if ph.get("sound_row") is not None})
                attack.append({
                    "actor": actor_id, "table": table, "stance": stance, "state": state,
                    "sequence_id": sid, "sequence_name": seqs[sid]["name"],
                    "stance_offset_rule": "AnimTable id = base + stance (runtime_player_combo_chain_v1.cpp:172-177; "
                                          "applied only when stanced_list bit set)",
                    "melee_bindings_has_sequence": any(s["id"] == sid for s in melee_states.get(state, [])),
                    "combo_phases": count_phases(node),
                    "clip_count": len(clips),
                    "clips_present": sum(1 for c in clip_info if c["present"]),
                    "clips_with_markers_in_melee_xml": sum(1 for c in clip_info if c["markers"]),
                    "marker_names": sorted({m.split("@")[0] for c in clip_info for m in (c["markers"] or [])}),
                    "charsounds_rows": sound_rows,
                    "clips": clip_info[:6],
                })

    # Sounds: CharSounds rows referenced by attack steps.
    sound_rows = []
    for idx, row in enumerate(charsounds):
        lists = {}
        for kind in ("death", "hit", "impact1", "impact2"):
            resolved = []
            for sid in row[kind]:
                s = soundpack.get(sid)
                resolved.append({"id": sid, "uid_label": s["label"] if s else None,  # presence only, not a binding
                                 "file_present": bool(s and find_asset(s["file"], idx_audio, rel_audio))})
            lists[kind] = resolved
        sound_rows.append({"row": idx, "name": row["name"], "lists": lists})

    # Projectiles.
    proj_rows = []
    wanted = ["FireWandProjectile", "EarthWandProjectile", "WaterWandProjectile", "AirWandProjectile",
              "LightningWandProjectile", "BowProjectile", "ArrowDefault", "ArrowMagic", "ArrowPinShot"]
    for name in wanted:
        if name in proj_names:
            proj_rows.append({"name": name, "row": proj_names.index(name), "name_in_table": True})
        else:
            proj_rows.append({"name": name, "row": None, "name_in_table": False})
    staged_proj = sorted(os.path.basename(p) for paths in idx_proj.values() for p in paths)

    probes = run_probes(args.port)

    result = {
        "inputs": {"pydata": pydata, "staged_assets": assets, "soundpack": sound_xml,
                   "melee_bindings": melee_xml, "port": args.port},
        "item_table_records": len(rows),
        "weapon_families": family_out,
        "attack_sequences": attack,
        "charsounds_rows": sound_rows,
        "projectile_rows": proj_rows,
        "staged_projectile_models": staged_proj,
        "port_probes": probes,
    }
    with open(os.path.join(args.out, "WEAPON-VERIFICATION-MATRIX.json"), "w", encoding="utf-8") as handle:
        json.dump(result, handle, indent=2, default=str)
    write_markdown(result, os.path.join(args.out, "WEAPON-VERIFICATION-MATRIX.md"))
    print("families", len(family_out), "attack rows", len(attack), "probes", len(probes))


def node_iter(node):
    for ph in node.get("phases", []):
        yield ph
        if "sub" in ph:
            yield from node_iter(ph["sub"])


# Hand-verified per-weapon status (Step 1). Evidence is file:line in the worktree or the named data table.
# Re-check any line when the port changes; the probes in section 6 are the mechanical part.
STATUS_ROWS = [
    ("1H sword (Longsword01-16)", "0", "248 Human_Attack_00_1H (3 phases, 6 clips) / 243 AttackStatic; attack_mainhand@334 and @900 in melee XML",
     "none (word5=-1, word6=-1); AnimStep sound ids unresolved", "none",
     "none (no projectile)", "Knight keeps the default combat profile (main.cpp:1119 excludes only non-Knight classes from animation-only); 1H sequence used by default, no stance bank rebuild",
     "Other classes animation-only (main.cpp:1119). Stance banks are built only for Rogue (main.cpp:1750), once at startup (main.cpp:1770); no rebuild on equip. Swing sound id unresolved."),
    ("1H axe (BattleAxe01-16)", "0", "same sequence family as sword (248/243)", "none (word5=-1)", "none", "none",
     "same as sword; axe swing sound unknown (CHARACTER-COVERAGE-MATRIX)", "same gaps as sword"),
    ("1H mace (Warhammer01-16)", "0", "same sequence family as sword (248/243)", "none (word5=-1)", "none", "none",
     "same as sword", "same gaps as sword"),
    ("Shield (offhand, Shield01-16)", "0 (not a dual stance)", "Attack+5 = 253 Human_BLOCK_00_1H (prince_block.bdae); not mapped to the Blocking state here",
     "none (word5=-1)", "none", "none", "not verified: block input unknown (CHARACTER-COVERAGE-MATRIX)", "block handling unverified; model present"),
    ("Dagger / Claw (dual, type 3, slot -3)", "2 when main+offhand", "250 Human_Attack_02_Dual (4 phases, 12 clips prince_dual_*) / 245 AttackStatic; markers not in staged melee XML",
     "none (word5=-1); dagger swoosh labels exist in soundpack (sfx_swoosh_dagger_1/2) but no item reference", "none", "none",
     "Rogue startup gear only: plan_runtime_player_profile_attack_bank_v1 call at main.cpp:1770; later gear swaps not rebuilt (matrix)",
     "no off-hand flag for tests (--combat-off-item absent); dual untestable by flags; bank not rebuilt on equip"),
    ("Two-hander (TwoHandsSword/Axe/Mace01-16)", "1", "249 Human_Attack_01_2H (3 phases, 6 clips prince_2hand_*) / 244 AttackStatic; not in melee XML (no markers)",
     "none (word5=-1)", "none", "none",
     "locomotion stance resolves (B102); attack bank not built for non-Rogue; 2H bank never rebuilt on equip", "2H attack unverified/likely missing; damage markers unknown"),
    ("Staff (Quarterstaff01-16)", "3", "251 Human_Attack_03_Staff (1 clip skill_dh2_prince_mage_wand_attack.bdae); not in melee XML; do_skill marker per mage report only",
     "word5 = 478-482 -> soundpack uids that are AMBIENT MUSIC tracks (namespace mismatch: UNRESOLVED)", "none (word6=-1)",
     "do_skill at state 5 -> ProjectileTable row 20 FireWandProjectile (elemental_bolt_fire.bdae; source: mage report, not re-derived here)",
     "planner supports stance 3 (generic). Production: Mage attack only on the v21.8-mage branch (not in this worktree); no player launcher in windows-foundation (probe player_ranged_launch = 0)",
     "bolt model missing from staged package; launch refused without --projectile-source-assets (mage report)"),
    ("Wand (Wand01-16)", "3", "same clip/sequence as staff (251)", "word5 = 478-482 (music namespace, UNRESOLVED)", "none",
     "element row not determined: FireWand row 20 vs Earth 15 / Water 38 / Air 2 / Lightning 23 (which one a wand uses is UNVERIFIED)",
     "same as staff", "same as staff; element choice unverified"),
    ("Bow (Bow01-16, MC_LWeapon_Bow)", "4", "252 Human_Attack_05_Bow (3 clips prince_ranged_attack*.bdae); no melee XML entry; no markers",
     "none (word5=-1)", "none", "BowProjectile row 10 by name only; which row the bow clip fires is UNVERIFIED",
     "stance resolves (tests); bow attack bank not wired; no launcher", "no bow attack, no bow projectile in production (matrix)"),
    ("Unarmed (no main hand)", "5", "Attack+5 = 253 (Human_BLOCK_00_1H); AttackStatic+5 = 248 (alias of Attack+0): the table has no bare-hand attack",
     "n/a", "n/a", "n/a", "n/a", "data fact: bare stance has no attack sequence"),
]

TOOLING_ROWS = [
    ("--combat-main-item <ItemTable identifier>", "main.cpp:658; read at main.cpp:1465 only while equipmentStateInitialized is false",
     "sets the startup main-hand for the locomotion stance and for the Rogue attack bank (main.cpp:1755). Works for any main-hand family name, e.g. Staff01, Bow01, TwoHandsSword01."),
    ("--combat-off-item", "absent (probe combat_off_item_flag = 0)",
     "no CLI way to set an off-hand, so dual (stance 2) and shield states cannot be equipped by flag."),
    ("--test-player-hp <n>", "main.cpp:638", "keeps the hero alive for long attack sequences."),
    ("--save <file>", "existing option (run-mage.ps1 uses it)", "menu-start with a saved profile; the profile's own gear is used unless the equipment-state flag path applies."),
    ("C:/dh218/tools/run-mage.ps1", "outside the repo (mage worktree tooling)", "menu start, clicks, frame captures; base for the batch template."),
    ("C:/dh217/tools/run-case.ps1", "outside the repo (v21.7 harness)", "hidden-window real runs with real keys."),
    ("coordination/v21.8/weapon_run_template.ps1", "added in this branch", "batch runner TEMPLATE: loops weapon identifiers, captures logs, greps attack-bank and dispatch lines. NOT EXECUTED (no build or run in this session)."),
]



def write_markdown(result, path):
    lines = []
    add = lines.append
    add("# v21.8 weapon verification matrix (static)")
    add("")
    add("Generated by `coordination/v21.8/weapon_static_check.py` from the original pydata tables, the original")
    add("melee bindings, the original soundpack XML, the staged v21.8 package and the port sources. Re-run to refresh.")
    add("Every value below is read from data unless marked INFERENCE. Sound uid/namespace resolution is NOT verified.")
    add("")
    add("## 1. Weapon families (ItemTable, %d records)" % result["item_table_records"])
    add("")
    add("| family | type (word22) | slot (word26) | cat (word37) | items | stance | models present/missing | AV id | swoosh sound id (word5) | swoosh fx (word6) | strength req range |")
    add("|---|---|---|---|---|---|---|---|---|---|---|")
    for fam in result["weapon_families"]:
        sw = "; ".join(("%s = %s" % (s["id"], s["status"])) if s["id"] >= 0 else "none" for s in fam["swoosh_sound_ids"])
        add("| %s | %s %s | %s | %s | %d | %s | %d / %d | %s | %s | %s | %s |" % (
            fam["family"], fam["type"], fam["type_name"], fam["slot"], fam["category_param37"], fam["item_count"],
            ",".join(str(s) for s in fam["stance"]), fam["models_present_in_staged_or_original"],
            fam["models_missing"], ",".join(str(x) for x in fam["audiovisual_ids"]), sw,
            ",".join(str(x) for x in fam["swoosh_fx_ids"]), fam["strength_requirement_range"]))
    add("")
    add("Staged-package weapon models missing (must be 0 for the port to load them): %d" % sum(
        len(f["models_missing_from_staged_package"]) for f in result["weapon_families"]))
    add("")
    add("Models: MC_RWeapon_* (right hand) and MC_LWeapon_* (left hand: bow, shield). Item variants `_Hard` and")
    add("`_VeryHard` are separate ItemTable rows (3 per tier). Model presence is checked by flattened basename")
    add("(staged `assets/models/`, original `3d/characters/prince/weapons/`).")
    add("")
    add("## 2. Stance and per-class equip values")
    add("")
    add("Stance is computed by the port's GetAnimStance rules (character_stance.cpp:6-11: staff > bow > dual > two-hander >")
    add("bare) fed by player_equipment_queries_v1.cpp:13-14 (two-hand = slot -4, dual = any non-shield off-hand item).")
    add("Class order of equip values = ItemTable schema (Warrior, Mage, Rogue, Paladin, Berserker, Assassin, Archer,")
    add("Necromancer, Illusionist), first tier shown. No port code reads the EquipValue fields (grep: none), so")
    add("class usability is only the raw data (INFERENCE: value > 0 means usable).")
    add("")
    add("| family | class equip values (first tier) |")
    add("|---|---|")
    for fam in result["weapon_families"]:
        vals = fam["class_equip_values_first_tier"]
        add("| %s | %s |" % (fam["family"], ", ".join("%s=%s" % (k, v) for k, v in vals.items()) if vals else "-"))
    add("")
    add("## 3. Attack animation sets (AnimTable, player tables 48/49/50)")
    add("")
    add("Attack = sequence 248 + stance, AttackStatic = 243 + stance (rule runtime_player_combo_chain_v1.cpp:172-177).")
    add("Markers come from the staged original-melee-bindings.xml clip entries (empty = clip not listed there).")
    add("")
    add("| actor (table) | stance | state | seq | name | combo phases | clips present | melee XML seq | marker names | AnimStep sound ids (raw, UNRESOLVED) |")
    add("|---|---|---|---|---|---|---|---|---|---|")
    for item in result["attack_sequences"]:
        if "warning" in item:
            add("| %s | WARNING %s |" % (item["actor"], item["warning"]))
            continue
        add("| %s (%d) | %d | %s | %d | %s | %d | %d/%d | %s | %s | %s |" % (
            item["actor"], item["table"], item["stance"], item["state"], item["sequence_id"],
            item["sequence_name"] or "-", item["combo_phases"], item["clips_present"], item["clip_count"],
            "yes" if item["melee_bindings_has_sequence"] else "no",
            ",".join(item["marker_names"]) or "-", ",".join(str(x) for x in item["charsounds_rows"]) or "-"))
    add("")
    add("## 4. Sounds and FX")
    add("")
    add("- AnimStep.Sound values on player attack steps (232, 233, 289 ...) are NOT CharSounds row indices (that table has 3")
    add("  rows). They are UNRESOLVED here. sounds_pyarraynames.bin holds five string arrays (sizes 3, 5, 27, 8, 183); the")
    add("  binding from AnimStep.Sound to a sound file is not verified. Treat every sound name claim here as unverified.")
    add("- Item SwooshSoundFX (word 5) / SwooshFX (word 6) reach the attack path through features/effects/runtime_swing_fx_observer_v1.cpp:52-55,")
    add("  constructed at main.cpp:4199 (WIRED). features/audio/runtime_attack_sound_v1.cpp:34 reads the same words but is NOT constructed in main.cpp.")
    add("")
    add("CharSounds rows (sounds_pyarray first table) with their raw id lists; `uid exists` = the id is a soundpack uid.")
    add("")
    add("| CharSounds row | name | death | hit | impact1 | impact2 |")
    add("|---|---|---|---|---|---|")
    for row in result["charsounds_rows"]:
        cells = []
        for kind in ("death", "hit", "impact1", "impact2"):
            cells.append(", ".join("%d (uid exists: %s)" % (x["id"], "yes" if x["uid_label"] else "no")
                                   for x in row["lists"][kind]) or "-")
        add("| %d | %s | %s |" % (row["row"], row["name"], " | ".join(cells)))
    add("")
    add("Soundpack uid namespace: 578 sounds (sounds-original-v4.xml). Weapon swoosh ids 478-482 (staves/wands) resolve to")
    add("ambient music tracks (FireTowerAmbientMusic, ...). That is a namespace mismatch, so the swoosh ids are UNRESOLVED.")
    add("")
    add("## 5. Projectiles (ProjectileTable, projectiles_pyarraynames order = row id)")
    add("")
    add("| name | row | in table |")
    add("|---|---|---|")
    for p in result["projectile_rows"]:
        add("| %s | %s | %s |" % (p["name"], p["row"], p["name_in_table"]))
    add("")
    add("Staged projectile models: %s" % (", ".join(result["staged_projectile_models"]) or "none"))
    add("(FireWandProjectile = elemental_bolt_fire.bdae per the v21.8-mage report; not in the staged package.)")
    add("")
    add("## 6. Port wiring probes (static grep of the worktree)")
    add("")
    add("| probe | file | hits | first line | meaning |")
    add("|---|---|---|---|---|")
    for p in result["port_probes"]:
        add("| %s | %s | %d | %s | %s |" % (p["id"], p["file"], p["hits"], p["first_line"] or "-", p["meaning"]))
    add("")
    add("## 7. Per-weapon status (hand-verified against the tables and sources above)")
    add("")
    add("| weapon | stance | attack sequences (AnimTable) | swoosh / hit sound | FX | projectile | port wiring | gaps |")
    add("|---|---|---|---|---|---|---|---|")
    for row in STATUS_ROWS:
        add("| " + " | ".join(row) + " |")
    add("")
    add("## 8. Test tooling for equipping any weapon and running attacks")
    add("")
    add("| flag / tool | where | use |")
    add("|---|---|---|")
    for row in TOOLING_ROWS:
        add("| " + " | ".join(row) + " |")
    add("")
    add("## 9. Step verdicts")
    add("")
    add("- Step 1 (enumerate weapon types, stances, attack sets, sounds, FX, projectiles, port status): WORKED for the item data,")
    add("  the attack sequences and the port wiring. DID NOT WORK to a sound file: AnimStep.Sound and CharSounds ids and the weapon")
    add("  swoosh ids are unresolved (the swoosh ids 478-482 land on music tracks).")
    add("- Step 2 (static completeness per weapon type, script): WORKED. 12 families, 576 weapon model names, 36 attack rows, 17 probes.")
    add("  Sound files per weapon: not verified. Projectile model elemental_bolt_fire.bdae missing from the staged package.")
    add("- Step 3 (test-only way to equip any weapon and attack in real runs): PARTIAL. --combat-main-item already sets any main-hand")
    add("  family. No off-hand flag, and the attack bank is only built for Rogue, so non-Rogue attacks cannot be exercised by flags.")
    add("  Batch template written, NOT executed (no build or hidden-window run in this session).")
    add("")
    with open(path, "w", encoding="utf-8") as handle:
        handle.write("\n".join(lines) + "\n")



if __name__ == "__main__":
    sys.exit(main())
