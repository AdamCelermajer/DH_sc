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
    ("session_audio_attack_sound", "windows-foundation/features/audio/runtime_session_audio_v1.cpp", r"make_unique<RuntimeAttackSoundV1>", "RuntimeAttackSoundV1 (AnimStep/Swoosh sound ids) is constructed inside RuntimeSessionAudioV1::bind"),
    ("main_audio_bind", "windows-foundation/main.cpp", r"runtimeAudio->bind\(combatSession", "main.cpp binds the session audio (and so the attack-sound observer) to the CombatSession; requires --audio"),
    ("host_source_row_lookup", "windows-foundation/features/audio/runtime_audio_host_v1.cpp", r"bindings\(\)\.row\(", "host resolves a source id through the 638-row Arrays::Sounds bindings (not the soundpack uid)"),
    ("swoosh_gate_v38", "windows-foundation/features/audio/runtime_attack_sound_v1.cpp", r"audio_animation_swoosh_v38", "Swoosh item-override/fallback gate (_SetAnimStep 0x3ca884..0x3cab20)"),
    ("marker_sfx_prefix", "windows-foundation/features/audio/runtime_combat_audio_v1.cpp", r'rfind\("sfx_",0\)', "authored sfx_ clip event -> named Sounds lookup (CharAI._OnAnimEvent 0x3d4434)"),
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
        rows.append({"death": lists[0], "hit": lists[1], "impact1": lists[2], "impact2": lists[3],
                     "flesh": flesh, "metal": metal})
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


def load_source_bindings(audio_pydata):
    """Arrays::Sounds (the table AnimStep.Sound / Item.SwooshSoundFX / CharSounds lists index).

    Records: u32 count, then count x (i32 uid, i32 event_flag) (Arrays::Sounds::read 0x4b5690 + SoundAutoGen::read
    0x502af4); names: u32 count + length-prefixed strings (readNames 0x4b313c). Source id = row index in this
    638-row stream (port/engine-audio/reference/source-bindings-v38/NOTES.md). NOT a soundpack uid and NOT a
    sounds_pyarray.bin (legacy Sounds_bak) index."""
    rec = read(os.path.join(audio_pydata, "sdd_dungeon_hunter_2_iphone_pyarray.bin"))
    n = struct.unpack_from("<I", rec)[0]
    if len(rec) != 4 + 8 * n:
        raise SystemExit("Sounds records extent differs")
    rows = [struct.unpack_from("<ii", rec, 4 + 8 * i) for i in range(n)]
    names = Rd(read(os.path.join(audio_pydata, "sdd_dungeon_hunter_2_iphone_pyarraynames.bin"))).strs()
    if len(names) != n or len(set(names)) != n:
        raise SystemExit("Sounds names differ")
    return [{"id": i, "name": names[i], "uid": rows[i][0], "event": rows[i][1]} for i in range(n)]


def load_soundpack_full(path):
    """sounds.xml: <sound uid filename label> and <event uid label type value=member sound uids>."""
    root = ET.parse(path).getroot()
    sounds = {int(e.get("uid")): {"label": e.get("label"), "file": e.get("filename")} for e in root.iter("sound")}
    events = {int(e.get("uid")): {"label": e.get("label"), "type": e.get("type"),
                                  "members": [int(v) for v in e.get("value").split(",")]} for e in root.iter("event")}
    return sounds, events


class SoundResolver:
    """source id -> (uid, event?) -> XML sound/event -> exact WAV filename -> presence in the staged audio package.
    No fallback or near-name substitution (original LoadSound 0x3699fc prepends data/sounds/ to the XML filename)."""

    def __init__(self, bindings, sounds, events, wav_dir):
        self.b, self.s, self.e = bindings, sounds, events
        self.wavs = {n.lower() for n in os.listdir(wav_dir)} if os.path.isdir(wav_dir) else set()

    def wav(self, uid):
        s = self.s.get(uid)
        if not s:
            return {"uid": uid, "file": None, "present": False}
        return {"uid": uid, "file": s["file"], "present": s["file"].lower() in self.wavs}

    def resolve(self, sid):
        if sid < 0:
            return {"id": sid, "kind": "none", "name": None, "files": []}
        if sid >= len(self.b):
            return {"id": sid, "kind": "out of range", "name": None, "files": []}
        row = self.b[sid]
        if row["event"] == 1:
            ev = self.e.get(row["uid"])
            return {"id": sid, "kind": "event", "name": row["name"], "uid": row["uid"],
                    "event_label": ev["label"] if ev else None, "event_type": ev["type"] if ev else None,
                    "files": [self.wav(u) for u in ev["members"]] if ev else []}
        return {"id": sid, "kind": "sound", "name": row["name"], "uid": row["uid"], "files": [self.wav(row["uid"])]}

    def text(self, sid):
        r = self.resolve(sid)
        if r["kind"] in ("none", "out of range"):
            return "%d (%s)" % (sid, r["kind"])
        files = ", ".join("%s%s" % (f["file"], "" if f["present"] else " [ABSENT]") for f in r["files"])
        if r["kind"] == "event":
            return "%d %s -> event uid %d (%s of uids %s) -> %s" % (
                sid, r["name"], r["uid"], r["event_type"], "/".join(str(f["uid"]) for f in r["files"]), files)
        return "%d %s -> uid %d -> %s" % (sid, r["name"], r["uid"], files)

    def short(self, sid):
        r = self.resolve(sid)
        if r["kind"] in ("none", "out of range"):
            return "-"
        files = " | ".join("%s%s" % (f["file"], "" if f["present"] else " [ABSENT]") for f in r["files"])
        return "%d %s: %s" % (sid, r["name"], files)


def swoosh_outcome(step_sound, swoosh, main_snd, main_fx, off_snd, off_fx):
    """Source ids submitted for one AnimStep. Literal port of _SetAnimStep 0x3ca884..0x3cab20 as encoded in
    audio_animation_swoosh_v38.hpp (offhand FX result overwrites the sound-fallback flag).
    Returns (ordered source ids, note)."""
    if not swoosh:
        return ([step_sound] if step_sound >= 0 else []), "ordinary step"
    played = []
    sf = main_snd == -1
    if not sf:
        played.append(main_snd)
    ff = main_fx == -1
    if sf and off_snd != -1:
        played.append(off_snd)
        sf = False
    if ff:
        sf = off_fx == -1
    if sf and step_sound >= 0:
        played.append(step_sound)
    return played, "swoosh step (item override first, step sound when fallback flag set)"


def leaf_steps(seqs, seq_id, path=(), depth=0):
    """Depth-first leaf AnimSteps of a sequence (redirect steps recurse)."""
    out = []
    if not (0 <= seq_id < len(seqs)) or depth > 4:
        return out
    for idx, st in enumerate(seqs[seq_id]["steps"]):
        if st["redir"] == 1:
            out.extend(leaf_steps(seqs, st["anim"], path + ((seq_id, idx),), depth + 1))
        else:
            out.append({"seq": seq_id, "seq_name": seqs[seq_id]["name"], "step": idx, "sound": st["sound"],
                        "swoosh": st["swoosh"], "fx": st["fx"], "clip_index": st["anim"]})
    return out


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
    audio_pydata = os.path.join(audio, "data", "pydata")
    if not os.path.exists(os.path.join(audio_pydata, "sdd_dungeon_hunter_2_iphone_pyarray.bin")):
        audio_pydata = pydata
    melee_xml = os.path.join(assets, "original-melee-bindings.xml")
    proj_extra = os.path.join(li, "runtime-session-projectile-v1", "assets")

    rows, _schema = load_items(pydata)
    seqs, chars, char_names, clip_names, clip_values = load_anim(pydata)
    charsounds = load_charsounds(pydata)
    soundpack = load_soundpack(sound_xml)
    bindings = load_source_bindings(audio_pydata)
    sp_sounds, sp_events = load_soundpack_full(sound_xml)
    resolver = SoundResolver(bindings, sp_sounds, sp_events, os.path.join(audio, "data", "sounds"))
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
                                        "swoosh_fx": set(), "item_swoosh": {}, "eq_first": None, "level": [], "cls_req": set()})
        fam["items"].append(row["id"])
        fam["models"].add(row["name"])
        fam["av"].add(w[21])
        fam["swoosh"].add(w[5])
        fam["swoosh_fx"].add(w[6])
        fam["item_swoosh"][row["id"]] = (w[5], w[6])
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
            r = resolver.resolve(sid)
            swoosh_status.append({"id": sid, "status": "none" if sid < 0 else resolver.short(sid),
                                  "resolved": r})
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
            "item_swoosh": fam["item_swoosh"],
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

    ref = os.path.join(args.port, "engine-audio", "reference")
    with open(os.path.join(ref, "event-families-v38", "census.json"), encoding="utf-8") as handle:
        census_projectiles = json.load(handle)["projectiles"]
    ledger_check = {"rows": len(bindings), "agree": 0, "disagree": []}
    with open(os.path.join(ref, "source-bindings-v38", "ledger.json"), encoding="utf-8") as handle:
        ledger = json.load(handle)
    for led in ledger:
        row = bindings[led["source_id"]]
        if (row["name"], row["uid"], row["event"]) == (led["source_name"], led["uid"], led["event"]):
            ledger_check["agree"] += 1
        else:
            ledger_check["disagree"].append(led["source_id"])
    sound_model = build_sound_model(seqs, chars, char_names, resolver, {f["base"]: f for f in families.values()},
                                    charsounds, census_projectiles, idx_assets, rel_assets, clip_names, clip_values,
                                    collections.Counter(r["w"][5] for r in rows))
    sound_model["ids_resolved"] = {str(i): resolver.resolve(i) for i in sorted(sound_model["ids_used"])}
    sound_model["ids_used"] = sorted(sound_model["ids_used"])
    sound_model["ledger_check"] = ledger_check
    claim_failures = verify_sound_claims(sound_model, resolver)
    sound_model["claim_failures"] = claim_failures

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
        "sound_model": sound_model,
    }
    with open(os.path.join(args.out, "WEAPON-VERIFICATION-MATRIX.json"), "w", encoding="utf-8") as handle:
        json.dump(result, handle, indent=2, default=str)
    write_markdown(result, os.path.join(args.out, "WEAPON-VERIFICATION-MATRIX.md"))
    if claim_failures or ledger_check["disagree"]:
        print("SOUND CLAIM FAILURES:", claim_failures, ledger_check["disagree"])
        sys.exit(2)
    print("families", len(family_out), "attack rows", len(attack), "probes", len(probes))


def scan_markers(path):
    """Authored 'sfx_' animation event names inside a clip (CharAI._OnAnimEvent 0x3d4434 strips 4 bytes and scans Sounds names)."""
    with open(path, "rb") as handle:
        data = handle.read()
    return sorted({m.group().decode("latin-1") for m in re.finditer(rb"sfx_[A-Za-z0-9_]+", data)})


WEAPON_GROUPS = [
    # label, family base name, stance used for Attack/AttackStatic
    ("1H sword (Longsword)", "Longsword", 0),
    ("1H axe (BattleAxe)", "BattleAxe", 0),
    ("1H mace (Warhammer)", "Warhammer", 0),
    ("Dagger alone (main hand only)", "Dagger", 0),
    ("Claw alone (main hand only)", "Claw", 0),
    ("Dagger dual (main + non-shield off-hand)", "Dagger", 2),
    ("Claw dual (main + non-shield off-hand)", "Claw", 2),
    ("2H sword (TwoHandsSword)", "TwoHandsSword", 1),
    ("2H axe (TwoHandsAxe)", "TwoHandsAxe", 1),
    ("2H mace (TwoHandsMace)", "TwoHandsMace", 1),
    ("Staff", "Staff", 3),
    ("Wand", "Wand", 3),
    ("Bow", "Bow", 4),
]


def build_sound_model(seqs, chars, char_names, resolver, fam_by_base, charsounds, census_projectiles,
                      idx_assets, rel_assets, clip_names, clip_values, item_word5_counts):
    model = {"weapons": [], "ids_used": set()}
    note_ids = model["ids_used"]
    knight = chars[char_names.index("PlayerKnight")]
    off_snd, off_fx = -1, -1  # off-hand candidates (Shield, Dagger, Claw) all have word5 = word6 = -1 (checked below)
    off_checks = {b: sorted(set(v for v in fam_by_base[b]["item_swoosh"].values()))
                  for b in ("Shield", "Dagger", "Claw") if b in fam_by_base}
    model["offhand_item_swoosh_pairs"] = off_checks
    for label, base, stance in WEAPON_GROUPS:
        fam = fam_by_base[base]
        pairs = sorted(set(fam["item_swoosh"].values()))
        entry = {"label": label, "family": base, "stance": stance, "item_swoosh_pairs": pairs, "states": {}}
        for state, seq_base in (("Attack", ATTACK_BASE), ("AttackStatic", STATIC_BASE)):
            seq_id = seq_base + stance
            leaves = leaf_steps(seqs, seq_id)
            rows = []
            for leaf in leaves:
                variants = []
                for (w5, w6) in pairs:
                    ids, note = swoosh_outcome(leaf["sound"], leaf["swoosh"], w5, w6, off_snd, off_fx)
                    variants.append({"item_word5": w5, "item_word6": w6, "ids": ids})
                    note_ids.update(ids)
                rows.append({"seq": leaf["seq"], "seq_name": leaf["seq_name"], "step": leaf["step"],
                             "step_sound": leaf["sound"], "swoosh": leaf["swoosh"], "variants": variants})
            entry["states"][state] = {"sequence_id": seq_id, "sequence_name": seqs[seq_id]["name"], "leaves": rows}
        # authored sfx_ markers in the clips of Attack / AttackStatic
        markers = {}
        for state, seq_base in (("Attack", ATTACK_BASE), ("AttackStatic", STATIC_BASE)):
            node_clips = []
            for leaf in leaf_steps(seqs, seq_base + stance):
                idx = leaf["clip_index"]
                if 0 <= idx < len(clip_values):
                    node_clips.append(clip_values[idx])
            for uri in sorted(set(node_clips)):
                path = find_asset(uri, idx_assets, rel_assets)
                if not path:
                    continue
                for m in scan_markers(path):
                    markers.setdefault(m, []).append(os.path.basename(path))
        entry["sfx_markers"] = {}
        for m, clips in markers.items():
            name = m[4:]
            sid = next((r["id"] for r in resolver.b if r["name"] == name), -1)
            entry["sfx_markers"][m] = {"clips": sorted(set(clips)), "source_name": name, "source_id": sid}
            if sid >= 0:
                note_ids.add(sid)
        model["weapons"].append(entry)
    # Blocking state of the player table, and bare-hand stance 5.
    block_ids = knight["Blocking"]
    model["block"] = {"sequence_ids": block_ids, "leaves": [
        [dict(l) for l in leaf_steps(seqs, sid)] for sid in block_ids if sid >= 0]}
    model["bare_hand"] = {
        "Attack+5": {"id": ATTACK_BASE + 5, "name": seqs[ATTACK_BASE + 5]["name"]},
        "AttackStatic+5": {"id": STATIC_BASE + 5, "name": seqs[STATIC_BASE + 5]["name"]},
        "UnarmedAttack_sequences": [i for i, q in enumerate(seqs) if q["name"] and q["name"].startswith("UnarmedAttack")],
    }
    unarmed_ids = set(model["bare_hand"]["UnarmedAttack_sequences"])
    model["bare_hand"]["player_tables_referencing_unarmed"] = [
        n for n in ("PlayerKnight", "PlayerMage", "PlayerRogue")
        if any(l["seq"] in unarmed_ids for k in ("Attack", "AttackStatic") for q in chars[char_names.index(n)][k]
               for l in leaf_steps(seqs, q))]
    model["bare_hand"]["all_char_tables_referencing_unarmed"] = [
        char_names[ci] for ci, row in enumerate(chars)
        if any(l["seq"] in unarmed_ids for k in ("Attack", "AttackStatic") for q in row[k] for l in leaf_steps(seqs, q))]
    # CharSounds melee hit/impact/death.
    cs = []
    for idx, row in enumerate(charsounds):
        cs.append({"row": idx, "name": row["name"], "flesh_flag": row["flesh"], "metal_flag": row["metal"],
                   "lists": {k: row[k] for k in ("death", "hit", "impact1", "impact2")}})
        for k in ("death", "hit", "impact1", "impact2"):
            note_ids.update(row[k])
    model["charsounds"] = cs
    # Projectile impact sounds for weapon projectiles (census.json rows; fields +0x0c, +0x18, +0x30).
    wanted = ["BowProjectile", "ArrowDefault", "ArrowMagic", "ArrowPinShot", "FireWandProjectile",
              "EarthWandProjectile", "WaterWandProjectile", "AirWandProjectile", "LightningWandProjectile"]
    model["projectiles"] = []
    for name in wanted:
        row = next((p for p in census_projectiles if p["name"] == name), None)
        if row:
            ids = [row["impact1_sound"], row["impact_default_sound"], row["impact2_3_sound"]]
            note_ids.update(i for i in ids if i >= 0)
            model["projectiles"].append({"name": name, "row": row["id"], "impact_type1": ids[0],
                                         "impact_default": ids[1], "impact_type2_3": ids[2]})
    # Where each weapon-relevant source id is referenced at all (AnimTable leaf steps, item word5, CharSounds, projectiles).
    step_counts = collections.Counter(st["sound"] for q in seqs for st in q["steps"] if st["redir"] != 1)
    usage = {}
    for sid in (232, 233, 248, 249, 289, 290, 293, 294, 491, 598, 599, 600, 601, 291, 292, 478, 479, 480, 481, 482):
        usage[str(sid)] = {
            "name": resolver.b[sid]["name"],
            "anim_steps_any_sequence": step_counts.get(sid, 0),
            "item_word5_any_item": item_word5_counts.get(sid, 0),
            "charsounds_lists": sum(row[k].count(sid) for row in charsounds for k in ("death", "hit", "impact1", "impact2")),
            "projectile_fields": sum(1 for p in census_projectiles
                                     for k in ("impact1_sound", "impact_default_sound", "impact2_3_sound") if p[k] == sid),
        }
    model["id_usage"] = usage
    return model


def verify_sound_claims(model, resolver):
    """Machine check of the hand-written sound cells in STATUS_ROWS / section 4. Returns a list of failures."""
    bad = []

    def ids(label, state):
        w = next(x for x in model["weapons"] if x["label"] == label)
        return [i for leaf in w["states"][state]["leaves"] for v in leaf["variants"] for i in v["ids"]]

    def files(sid):
        return [f["file"] for f in resolver.resolve(sid)["files"]]

    def expect(name, got, want):
        if got != want:
            bad.append("%s: got %r want %r" % (name, got, want))

    for label in ("1H sword (Longsword)", "1H axe (BattleAxe)", "1H mace (Warhammer)", "Dagger alone (main hand only)"):
        expect(label + " Attack", ids(label, "Attack"), [232, 232, 233])
        expect(label + " AttackStatic", ids(label, "AttackStatic"), [289, 289, 289])
    for label in ("Dagger dual (main + non-shield off-hand)", "Claw dual (main + non-shield off-hand)"):
        expect(label + " Attack", ids(label, "Attack"), [598, 598, 599, 599])
        expect(label + " AttackStatic", ids(label, "AttackStatic"), [293] * 4)
    for label in ("2H sword (TwoHandsSword)", "2H axe (TwoHandsAxe)", "2H mace (TwoHandsMace)"):
        expect(label + " Attack", ids(label, "Attack"), [232, 233, 233])
        expect(label + " AttackStatic", ids(label, "AttackStatic"), [289] * 3)
    for label in ("Staff", "Wand"):
        expect(label + " Attack", ids(label, "Attack"), [478, 479, 480, 481, 482])
        for sid in range(478, 483):
            expect("%s id %d file" % (label, sid), files(sid), ["sfx_mage_staff_elemental_thunder_22.wav"])
    expect("232 files", files(232), ["sfx_swoosh_sword_1_22.wav"])
    expect("233 files", files(233), ["sfx_swoosh_sword_2_22.wav"])
    expect("289 files", files(289), ["sfx_swoosh_sword_1_22.wav", "sfx_swoosh_sword_2_22.wav"])
    expect("293 files", files(293), ["sfx_swoosh_dagger_1_22.wav", "sfx_swoosh_dagger_2_22.wav"])
    expect("491 files", files(491), ["sfx_mc_shoot_arrow_22.wav"])
    bow = next(x for x in model["weapons"] if x["family"] == "Bow")
    expect("bow marker ids", sorted(v["source_id"] for v in bow["sfx_markers"].values()), [491])
    for w in model["weapons"]:
        if w["family"] != "Bow":
            expect(w["label"] + " sfx markers", w["sfx_markers"], {})
    for pr in model["projectiles"]:
        if "Wand" in pr["name"]:
            expect(pr["name"] + " impact fields", [pr["impact_type1"], pr["impact_default"], pr["impact_type2_3"]], [-1, -1, -1])
    for sid in ("248", "290", "294", "600", "601"):
        u = model["id_usage"][sid]
        expect("unreferenced id " + sid, [u["anim_steps_any_sequence"], u["item_word5_any_item"], u["charsounds_lists"], u["projectile_fields"]], [0, 0, 0, 0])
    for sid in range(478, 483):
        if not resolver.resolve(sid)["files"][0]["present"]:
            bad.append("staff WAV absent for %d" % sid)
    return bad


def node_iter(node):
    for ph in node.get("phases", []):
        yield ph
        if "sub" in ph:
            yield from node_iter(ph["sub"])


# Hand-verified per-weapon status (Step 1). Evidence is file:line in the worktree or the named data table.
# Re-check any line when the port changes; the probes in section 6 are the mechanical part.
STATUS_ROWS = [
    ("1H sword (Longsword01-16)", "0", "248 Human_Attack_00_1H (3 phases, 6 clips) / 243 AttackStatic; attack_mainhand@334 and @900 in melee XML",
     "swing via step fallback (no item override): combo 232,232,233 -> sfx_swoosh_sword_1_22.wav x2 then sfx_swoosh_sword_2_22.wav; static 289 -> random sword_1/sword_2. Hit: weapon independent (4.2)", "none",
     "none (no projectile)", "Knight keeps the default combat profile (main.cpp:1119 excludes only non-Knight classes from animation-only); 1H sequence used by default, no stance bank rebuild",
     "Other classes animation-only (main.cpp:1119). Stance banks are built only for Rogue (main.cpp:1750), once at startup (main.cpp:1770); no rebuild on equip. Sound path is wired only with --audio (runtime_session_audio_v1.cpp:190, main.cpp:4220)."),
    ("1H axe (BattleAxe01-16)", "0", "same sequence family as sword (248/243)", "same files as the 1H sword (no axe-specific cue exists: word5=-1)", "none", "none",
     "same as sword", "same gaps as sword"),
    ("1H mace (Warhammer01-16)", "0", "same sequence family as sword (248/243)", "same files as the 1H sword (no mace-specific cue exists: word5=-1)", "none", "none",
     "same as sword", "same gaps as sword"),
    ("Shield (offhand, Shield01-16)", "0 (not a dual stance)", "Attack+5 = 253 Human_BLOCK_00_1H (prince_block.bdae); not mapped to the Blocking state here",
     "no sound in the original: Blocking seq 253 step sound -1, word5=-1, env_character_block (248) has no producer", "none", "none", "not verified: block input unknown (CHARACTER-COVERAGE-MATRIX)", "block handling unverified; model present; no block sound to implement"),
    ("Dagger / Claw (dual, type 3, slot -3)", "2 when main+offhand", "250 Human_Attack_02_Dual (4 phases, 12 clips prince_dual_*) / 245 AttackStatic; markers not in staged melee XML",
     "dual: combo 598,598,599,599 -> sfx_swoosh_dagger_1_22.wav x2, sfx_swoosh_dagger_2_22.wav x2; static 293 -> random dagger_1/dagger_2 (all present). Alone (stance 0): sword files as the 1H sword", "none", "none",
     "Rogue startup gear only: plan_runtime_player_profile_attack_bank_v1 call at main.cpp:1770; later gear swaps not rebuilt (matrix)",
     "no off-hand flag for tests (--combat-off-item absent); dual untestable by flags; bank not rebuilt on equip"),
    ("Two-hander (TwoHandsSword/Axe/Mace01-16)", "1", "249 Human_Attack_01_2H (3 phases, 6 clips prince_2hand_*) / 244 AttackStatic; not in melee XML (no markers)",
     "combo 232,233,233 -> sword_1, sword_2, sword_2 (_22.wav); static 289 -> random sword_1/sword_2 (same files as 1H; env_weapon_2h 290 is unused)", "none", "none",
     "locomotion stance resolves (B102); attack bank not built for non-Rogue; 2H bank never rebuilt on equip", "2H attack unverified/likely missing; damage markers unknown"),
    ("Staff (Quarterstaff01-16)", "3", "251 Human_Attack_03_Staff (1 clip skill_dh2_prince_mage_wand_attack.bdae); not in melee XML; do_skill marker per mage report only",
     "item word5 478-482 (earth/fire/ice/lightning/wind) -> uid 31/29/30/33/32 -> sfx_mage_staff_elemental_thunder_22.wav (present; the same clip for all five); step sound -1. Bolt impact: silent (ProjectileTable impact fields -1)", "none (word6=-1)",
     "do_skill at state 5 -> ProjectileTable row 20 FireWandProjectile (elemental_bolt_fire.bdae; source: mage report, not re-derived here)",
     "planner supports stance 3 (generic). Production: Mage attack only on the v21.8-mage branch (not in this worktree); no player launcher in windows-foundation (probe player_ranged_launch = 0)",
     "bolt model missing from staged package; launch refused without --projectile-source-assets (mage report)"),
    ("Wand (Wand01-16)", "3", "same clip/sequence as staff (251)", "same as staff: word5 478-482 -> sfx_mage_staff_elemental_thunder_22.wav (per-item element id)", "none",
     "element row not determined: FireWand row 20 vs Earth 15 / Water 38 / Air 2 / Lightning 23 (which one a wand uses is UNVERIFIED)",
     "same as staff", "same as staff; element choice unverified"),
    ("Bow (Bow01-16, MC_LWeapon_Bow)", "4", "252 Human_Attack_05_Bow (3 clips prince_ranged_attack*.bdae); no melee XML entry; no markers",
     "launch: clip event sfx_sfx_mc_shoot_arrow (id 491) -> sfx_mc_shoot_arrow_22.wav (present); impact: arrow_hit 291 -> sfx_impact_arrow_flesh_1/2_22.wav, arrow_miss 292 -> sfx_impact_arrow_object_1/2_22.wav; AnimSteps silent", "none", "BowProjectile row 10 by name only; which row the bow clip fires is UNVERIFIED",
     "stance resolves (tests); bow attack bank not wired; no launcher", "no bow attack, no bow projectile in production (matrix)"),
    ("Unarmed (no main hand)", "5", "Attack+5 = 253 (Human_BLOCK_00_1H); AttackStatic+5 = 248 (alias of Attack+0): the table has no bare-hand attack",
     "no sound: no bare-hand attack sequence in the player tables", "n/a", "n/a", "n/a", "data fact: bare stance has no attack sequence"),
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



def write_sound_section(result, add):
    m = result["sound_model"]
    res = {int(k): v for k, v in m["ids_resolved"].items()}

    def short(sid):
        r = res.get(sid)
        if r is None or r["kind"] in ("none", "out of range"):
            return "-"
        files = " / ".join("%s%s" % (f["file"], "" if f["present"] else " [ABSENT]") for f in r["files"])
        if r["kind"] == "event":
            return "%d %s -> event %d (random: %s)" % (sid, r["name"], r["uid"], files)
        return "%d %s -> %s" % (sid, r["name"], files)

    lc = m["ledger_check"]
    add("## 4. Sounds: how a sound id resolves to a WAV (verified rule)")
    add("")
    add("**Rule.** Every sound id in the weapon paths (AnimStep.Sound, ItemTable word5 SwooshSoundFX, the CharSounds")
    add("death/hit/impact lists, ProjectileTable impact fields, and the name of an authored `sfx_` clip event) is a")
    add("**source id = row index in the 638-row `Arrays::Sounds` stream** `sdd_dungeon_hunter_2_iphone_pyarray.bin` (+ `...names.bin`).")
    add("It is not a soundpack uid, not a CharSounds row index and not an index into the legacy 183-row `sounds_pyarray.bin`")
    add("array. Each row is `(uid, isEvent)`:")
    add("")
    add("1. `isEvent = 0`: `uid` is a `<sound uid=...>` in `sounds.xml`; the file is `data/sounds/<filename>` exactly as spelled")
    add("   (no `_22`/`_44` or near-name substitution).")
    add("2. `isEvent = 1`: `uid` is an `<event uid=...>` in `sounds.xml` (type `random`); one member sound uid is chosen with the")
    add("   shared gameplay Random, then rule 1 applies.")
    add("3. id `< 0`: no sound (`Play3D` returns at `cmp sl,#0; blt`).")
    add("")
    add("Original evidence (libDungeonHunter2.so, sha256 36498eb8...): `VoxSoundManager::Play3D` 0x36b5d8 computes `row = Sounds + 12*id`,")
    add("reads uid `[row+4]` and flag `[row+8]`; flag 1 branches to `PlayEvent` 0x36b420, else `PlaySoundPackSound` 0x36a7c0 (disassembly:")
    add("`port/engine-audio/reference/event-families-v38/0036b5d8.asm`, `0036b420.asm`). Callers: `CharAnimator::_SetAnimStep` 0x3ca79c")
    add("passes `step+0x2c` (AnimStep.Sound) to Play3D at 0x3ca8e0; `_PlayItemSwooshSFX` 0x3c94b8 passes `Item+0x14` (word5) at 0x3c953c.")
    add("The stream/name/uid/event binding is the one proven by `port/engine-audio/reference/source-bindings-v38/` (Unicorn-executed original")
    add("readers); this script re-parses the stream and cross-checks all %d rows against that `ledger.json`: %d agree, %d differ." % (
        lc["rows"], lc["agree"], len(lc["disagree"])))
    add("Live IDA/REA was not available in this session (rea reports no configured provider for ELF/ARM); the evidence above is the")
    add("committed original-instruction disassembly and proofs for the same binary hash, re-read, not a fresh decompile.")
    add("")
    add("**Namespace correction.** Earlier versions of this matrix looked the ids up as soundpack uids and concluded that")
    add("478-482 were ambient music (soundpack uid 478-482 are music rows). That was the wrong table. Source ids 478-482 are")
    add("`sfx_mage_staff_elemental_{earth,fire,ice,lightning,wind}` -> uid 31,29,30,33,32 -> all `sfx_mage_staff_elemental_thunder_22.wav`")
    add("(the authored XML points all five at the same clip; the element-specific WAVs `sfx_mage_staff_elemental_{earth,fire,ice,wind}.wav`")
    add("exist in the package but no XML row references them).")
    add("")
    add("**Swing selection (AnimStep with the Swoosh flag), `_SetAnimStep` 0x3ca884-0x3cab20 / `audio_animation_swoosh_v38`:** without the Swoosh")
    add("flag the step sound plays. With the flag, the main-hand item word5 plays first (if not -1), then the off-hand item word5")
    add("(only if the main one was -1); the step's own Sound is then ALSO played when the sound-fallback flag is set. The flag is")
    add("re-assigned by the off-hand FX query when the main item has no FX (word6 = -1 on every weapon), so for all weapon items the step")
    add("sound always plays, and a staff/wand plays its element sound plus the step sound (the staff step sound is -1, so just the element sound).")
    add("Port: `runtime_attack_sound_v1.cpp` reproduces this literally; `runtime_audio_host_v1.cpp:151,166` resolves the id with")
    add("`bindings().row(id)`; the event/uid selection is `audio_gameplay_runtime_v42.cpp:15-30`.")
    add("")
    add("### 4.1 Per-weapon sound table (computed: AnimTable x ItemTable word5/word6 x Arrays::Sounds x sounds.xml x staged WAVs)")
    add("")
    add("Cells show `source id name -> exact WAV` ([ABSENT] = file not in the staged `audio-assets/data/sounds`). Event rows list the random")
    add("candidates. Off-hand items (Shield, Dagger, Claw) all have word5 = word6 = -1 (checked: %s), so they never add a sound." % (
        m["offhand_item_swoosh_pairs"],))
    add("")
    add("| weapon | stance | swing: Attack (each combo phase) | swing: AttackStatic | item swoosh override (word5) | authored sfx_ clip events |")
    add("|---|---|---|---|---|---|")
    fam_items = {f["family"]: len(f["item_swoosh"]) for f in result["weapon_families"]}
    for w in m["weapons"]:
        cells = []
        for state in ("Attack", "AttackStatic"):
            st = w["states"][state]
            lines = []
            seen = set()
            for leaf in st["leaves"]:
                for v in leaf["variants"]:
                    for sid in v["ids"]:
                        key = (leaf["seq"], sid, v["item_word5"])
                        if key in seen:
                            continue
                        seen.add(key)
                        tag = (" [word5 %d]" % v["item_word5"]) if v["item_word5"] >= 0 else ""
                        lines.append("%s step%d%s: %s" % (leaf["seq_name"], leaf["step"], tag, short(sid)))
            cells.append("<br>".join(lines) if lines else "none")
        pairs = [tuple(p) for p in w["item_swoosh_pairs"]]
        if pairs == [(-1, -1)]:
            item = "none (word5 = -1, word6 = -1 on all %d items)" % fam_items[w["family"]]
        else:
            item = "<br>".join("%s (word6 %d)" % (short(p[0]), p[1]) for p in pairs)
        mk = "<br>".join("%s in %s -> %s" % (k, ", ".join(v["clips"]), short(v["source_id"]))
                         for k, v in w["sfx_markers"].items()) or "none"
        add("| %s | %d | %s | %s | %s | %s |" % (w["label"], w["stance"], cells[0], cells[1], item, mk))
    add("")
    add("Staff/Wand element split is per item (word5, JSON `item_swoosh`): 478 earth, 479 fire, 480 ice, 481 lightning, 482 wind. Every item with a")
    add("non-negative word5 is a staff or wand (96 rows).")
    add("")
    add("### 4.2 Hit, impact, death (melee): weapon independent")
    add("")
    add("Melee hit sounds do not depend on the weapon. `F_ApplyCombatSound` 0x3afee0 (port `character_combat_sound_v1.cpp`): the TARGET's")
    add("death list if the target died, else the TARGET's hit list if damage > 0; then, if the attacker is a Character and damage > 0, the")
    add("ATTACKER's `impact1` list when the target row's flesh flag is set, else the attacker's `impact2` list when the target's metal flag is")
    add("set. One entry per list is chosen with the shared Random. The ids are source ids (same rule).")
    add("")
    add("| CharSounds row | name | flesh / metal flags | death | hit | impact1 (target flesh) | impact2 (target metal) |")
    add("|---|---|---|---|---|---|---|")
    for row in m["charsounds"]:
        lst = row["lists"]

        def cell(k):
            return "<br>".join(short(i) for i in lst[k]) or "-"
        add("| %d | %s | %d / %d | %s | %s | %s | %s |" % (row["row"], row["name"], row["flesh_flag"], row["metal_flag"],
                                                        cell("death"), cell("hit"), cell("impact1"), cell("impact2")))
    add("")
    add("Only row 1 (ArmoredHuman) has lists; the player's attacker row is ArmoredHuman per the recorded live Swamp trace (impact1 120/121")
    add("selected, `coordination/v21.3/B122-report.md`). Monsters have their own hit/death cues via their Injured/Died AnimSteps.")
    add("")
    add("### 4.3 Block, miss, unarmed, ranged")
    add("")
    blk = m["block"]
    add("- **Block.** Player Blocking state = sequence %s (%s); its step sound is %s. `env_character_block` (id 248 -> event 1 -> "
        "`sfx_impact_block_1_22.wav` / `sfx_impact_block_2_22.wav`, both present) is referenced by no AnimStep, no item, no CharSounds list, "
        "no projectile field and no clip event (table 4.4). **No block sound is produced by any data path found; do not invent one.** "
        "(A call from compiled code with an immediate id is not excluded by this static scan; the 53-candidate direct-branch census "
        "`event-families-v38/census.json` lists no block producer.)" % (
            blk["sequence_ids"], ", ".join(l[0]["seq_name"] for l in blk["leaves"] if l),
            ", ".join(str(l[0]["sound"]) for l in blk["leaves"] if l)))
    add("- **Melee miss.** There is no separate miss cue: the only sound for a swing that hits nothing is the swing cue of 4.1. "
        "`F_ApplyCombatSound` runs only on a combat result.")
    bh = m["bare_hand"]
    add("- **Unarmed.** The player tables have no bare-hand attack: stance 5 Attack+5 = sequence %d %s, AttackStatic+5 = %d %s. "
        "`UnarmedAttack01/02` (sequences %s; UnarmedAttack01 step sound 232 WeaponSwoosh1 with the Swoosh flag) are referenced by no CharAnimTable "
        "Attack/AttackStatic (player tables referencing: %s; all tables: %s). A bare-hand player swing sound cannot be derived from the data." % (
            bh["Attack+5"]["id"], bh["Attack+5"]["name"], bh["AttackStatic+5"]["id"], bh["AttackStatic+5"]["name"],
            bh["UnarmedAttack_sequences"], bh["player_tables_referencing_unarmed"] or "none",
            bh["all_char_tables_referencing_unarmed"] or "none"))
    add("- **Bow launch.** The three bow clips (`prince_ranged_attack`, `_02`, `_03`) each carry the authored event `sfx_sfx_mc_shoot_arrow`; "
        "`CharAI._OnAnimEvent` 0x3d4434 strips `sfx_` and scans the Sounds names -> `sfx_mc_shoot_arrow` (id 491) -> uid 28 -> "
        "`sfx_mc_shoot_arrow_22.wav`. The bow AnimSteps themselves have sound -1. Port path: `runtime_combat_audio_v1.cpp` "
        "(`rfind(\"sfx_\",0)`), reached only when the bow clip's retained events are delivered (the bow attack is not wired in production, sections 6/7).")
    add("- **Staff/Wand launch.** The wand-attack clip has no `sfx_` event. The cast cue is the item word5 override above (element id -> the one "
        "thunder WAV). The spawned bolt is a ProjectileTable row; its impact sounds are next.")
    add("")
    add("| projectile (row) | impact type1 field | default field | type 2/3 field | meaning |")
    add("|---|---|---|---|---|")
    for pr in m["projectiles"]:
        silent = pr["impact_type1"] < 0 and pr["impact_default"] < 0 and pr["impact_type2_3"] < 0
        mean = "silent (all three fields -1)" if silent else "default: %s; type 2/3: %s" % (
            short(pr["impact_default"]), short(pr["impact_type2_3"]))
        add("| %s (%d) | %d | %d | %d | %s |" % (pr["name"], pr["row"], pr["impact_type1"], pr["impact_default"],
                                                 pr["impact_type2_3"], mean))
    add("")
    add("Arrow impact: default field -> `env_weapon_arrow_hit` (flesh impact WAVs); type 2/3 field -> `env_weapon_arrow_miss` (object impact "
        "WAVs). Which projectile a given attack fires is outside this table.")
    add("")
    add("### 4.4 Where each weapon cue id is referenced anywhere in the original data")
    add("")
    add("| id | name | AnimTable steps (any sequence) | any item word5 | CharSounds lists | projectile fields |")
    add("|---|---|---|---|---|---|")
    for k, v in m["id_usage"].items():
        add("| %s | %s | %d | %d | %d | %d |" % (k, v["name"], v["anim_steps_any_sequence"], v["item_word5_any_item"],
                                                  v["charsounds_lists"], v["projectile_fields"]))
    add("")
    add("### 4.5 Weapon cues with no sound in the original data (do not invent)")
    add("")
    add("- Block: id 248 `env_character_block` is unreferenced (above). Shield has no sound of its own (word5 = -1, no clip event).")
    add("- `env_weapon_2h` (290) is unreferenced: two-handers use 232/233 for combos and 289 `env_weapon_1h` for AttackStatic, the same files as one-handers.")
    add("- `env_weapon_staff` (294) and `sfx_swoosh_staff_1/2` (600/601; WAVs present) are unreferenced: staff/wand use the item word5 override (thunder clip).")
    add("- Staff/Wand projectile impact: all five wand projectile rows have impact fields -1 (silent).")
    add("- Bare-handed player swing: no sequence (above). Melee miss: no cue.")
    absent = sorted({f["file"] for r in res.values() for f in r["files"] if not f["present"] and f["file"]})
    add("- Cues whose WAV is not in the package (exact filename, no substitution): %s." % (", ".join(absent) or "none"))
    add("")
    add("Everything else on the weapon paths resolves to a present WAV: every swing (sword/axe/mace/dagger/claw/2H/dual/staff/wand), "
        "bow launch, arrow impacts.")
    add("")


def write_markdown(result, path):
    lines = []
    add = lines.append
    add("# v21.8 weapon verification matrix (static)")
    add("")
    add("Generated by `coordination/v21.8/weapon_static_check.py` from the original pydata tables, the original")
    add("melee bindings, the original soundpack XML, the staged v21.8 package and the port sources. Re-run to refresh.")
    add("Every value below is read from data unless marked INFERENCE. Sound ids are resolved with the verified rule in section 4.")
    add("")
    add("## 1. Weapon families (ItemTable, %d records)" % result["item_table_records"])
    add("")
    add("| family | type (word22) | slot (word26) | cat (word37) | items | stance | models present/missing | AV id | swoosh sound id (word5) | swoosh fx (word6) | strength req range |")
    add("|---|---|---|---|---|---|---|---|---|---|---|")
    for fam in result["weapon_families"]:
        sw = "; ".join(s["status"] if s["id"] >= 0 else "none" for s in fam["swoosh_sound_ids"])
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
    add("| actor (table) | stance | state | seq | name | combo phases | clips present | melee XML seq | marker names | AnimStep sound ids (source ids, see section 4) |")
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
    write_sound_section(result, add)
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
    add("- Step 1 (enumerate weapon types, stances, attack sets, sounds, FX, projectiles, port status): WORKED, including sound files.")
    add("  Rule: every sound id is a row of the 638-row Arrays::Sounds stream (uid, isEvent) -> sounds.xml sound/event -> data/sounds WAV (section 4).")
    add("- Step 2 (static completeness per weapon type, script): WORKED. 12 families, 576 weapon model names, 36 attack rows, 21 probes,")
    add("  a per-weapon resolved sound table (4.1) checked against the staged WAVs. Projectile model elemental_bolt_fire.bdae missing from the staged package.")
    add("- Sound resolution in the port is correct for weapons (host resolves the Arrays::Sounds row; Swoosh gate is the literal original). It is")
    add("  wired in production only when the game runs with --audio. The remaining weapon-sound gaps are data facts (4.5) and missing WAV filenames, not resolver bugs.")
    add("- Step 3 (test-only way to equip any weapon and attack in real runs): PARTIAL. --combat-main-item already sets any main-hand")
    add("  family. No off-hand flag, and the attack bank is only built for Rogue, so non-Rogue attacks cannot be exercised by flags.")
    add("  Batch template written, NOT executed (no build or hidden-window run in this session).")
    add("")
    with open(path, "w", encoding="utf-8") as handle:
        handle.write("\n".join(lines) + "\n")



if __name__ == "__main__":
    sys.exit(main())
