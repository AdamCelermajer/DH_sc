# Faery and player-skill research from original data (v21.8)

Date: 2026-10-13. Read-only research. No repo files were edited except this report. Nothing was committed.
Scope: work-order item 5 and faery section 4 of `coordination/v21.8/CHARACTER-COVERAGE-MATRIX.md`.

Abbreviations used below:
- **ANDROID** = `.local-inputs/assets-extra/android/data/` (original Android data: `pydata/*.bin`, `scripts/**/*.luac`, `3d/**/*.bdae`, `scene/*.mlx`).
- **PKG** = `.local-inputs/windows-source-clock-v21.7/` (the package asset dirs `assets/`, `audio-assets/`).
- **CAMPAIGN-XML** = `.local-inputs/claude-preview13/h/pkg/assets/original-campaign.xml` (134 of 135 copies under `.local-inputs` are byte-identical, sha256 `3ab20559...`; one copy is empty).
- Binary tables were decoded with throwaway Perl parsers in `/tmp` (outside the repo). Their output was cross-checked against the repo file `port/level-world/reference/character-skills/faery-original-probe.json`, which records what the original `libDungeonHunter2.so` code returns for the same table.

## Corrections to the coverage matrix

1. **Faery models.** The matrix says only Celest is staged. That is true for PKG only. ANDROID stages all five faery models: `3d/characters/faeries/faeries_00_hotty.bdae`, `faeries_01_wetty.bdae`, `faeries_02_celeste.bdae`, `faeries_03_rocky.bdae`, `faeries_04_windy.bdae`.
2. **Rocky, Wetty, Windy spells.** The matrix says these "have no spell". The original scripts define a full cast for all five faeries (`scripts/skills/faerie_<name>.luac`). The gap is in the port, not in the data.
3. **Faery passive.** The matrix says the passive is missing. Original data: `faerie_celest.luac` has an element-resistance passive (`CheckFaeries`). No other faery has one.
4. **Unlock sources for slots 1 to 4.** The matrix says "unknown". Level scripts stage `SetFaeryState` records for slots 1 to 4 in the water, wind, fire and earth temple levels (section 2). The matrix's "later-act scripts not staged" is also wrong: the 41 level `*_pyscripts.bin` files are staged in ANDROID, they are binary, and the records can be read.
5. **Cooldown.** The matrix says a 5000 ms literal (`main.cpp:5391`). The original reads `GetInt("Faery_Cooldown")`, defaults to 5000 when 0, and Mage "Faery Blood" writes that value (`prince_mage_faeryblood.luac`).
6. **Hotty sound.** The open question "fire no-target label maps to thunder_22" is resolved: all four `sfx_mage_staff_elemental_{fire,ice,earth,wind}` labels map to `sfx_mage_staff_elemental_thunder_22.wav` (section 1.4).
7. **Skill Lua.** The matrix says the package has no player skill Lua. True for PKG (only `witch_ranged_00.luac`). ANDROID has 128 scripts in `scripts/skills/`, including all nine skills in item 5.
8. **ChainLightning sound.** The matrix says "no linked cue". The Lua calls `PlaySound("sfx_skill_mage_thunder_braid*")` directly, and all three WAVs are in PKG.

---

## 1. Faeries

### 1.1 Table rows (`ANDROID/pydata/faeries_pyarray.bin`, 699 bytes)

Layout, as confirmed by the probe JSON: a list section (4 lists: `DEFAULT`, `Knight`, `Mage`, `Rogue`) and then 16 records. Each record is four int words, a length-prefixed SpellScript string, and two trailing int words.

Field reading used here (the 4 word positions and the string were matched to the probe JSON values):
- `w2` = faery model-set number (0 = Hotty, 1 = Wetty, 2 = Celest, 3 = Rocky, 4 = Windy), which matches the `faeries_0N_*` file names.
- `w3` = `ModelFile` as a 0-based index into `character_models_dictionary_pyarraynames.bin`. Index 32 is `faeries_00_hotty`, 33 `faeries_01_wetty`, 34 `faeries_02_celeste`, 35 `faeries_03_rocky`, 36 `faeries_04_windy`. Checked against the dictionary list.
- `w8` = element, using the `FaeryTypes` enum in `ANDROID/pydata/faeries_pycst.bin`: Lightning=0, Earth=1, Water=2, Air=3, Fire=4 (COUNT=5, INVALID=-1).
- The two tagged key words (0x130044/0x130042 and similar) are per-row references to text, probably the Name and Description strings. I did not resolve them. `common_text_pyarraynames.bin` has only 9 entries, so they are not indices into that table.

| Row | Name (pyarraynames) | SpellScript (Lua) | ModelFile idx -> model | Element (w8) | Set no. (w2) | List slot |
|---|---|---|---|---|---|---|
| 1 | Celest | `faerie_celest` | 34 -> `faeries_02_celeste` | 0 Lightning | 2 | 0 |
| 13 | Rocky | `faerie_rocky` | 35 -> `faeries_03_rocky` | 1 Earth | 3 | 1 |
| 14 | Wetty | `faerie_wetty` | 33 -> `faeries_01_wetty` | 2 Water | 1 | 2 |
| 15 | Windy | `faerie_windy` | 36 -> `faeries_04_windy` | 3 Air | 4 | 3 |
| 7 | Hotty | `faerie_hotty` | 32 -> `faeries_00_hotty` | 4 Fire | 0 | 4 |

- Lists: Knight, Mage and Rogue all use `[1,13,14,15,7]` (Celest, Rocky, Wetty, Windy, Hotty). `DEFAULT` = `[2,4,5,6,3]` (the Fake rows).
- Rows 2 to 6 are `Fake_Celest`, `Fake_Hotty`, `Fake_Rocky`, `Fake_Wetty`, `Fake_Windy`. They have an empty SpellScript and are placeholders.
- Rows 8 to 12 are `Mage_Celest`, `Mage_Hotty`, `Mage_Rocky`, `Mage_Wetty`, `Mage_Windy`, with scripts `faerie_*_mage`. They are in no list, and no `*_mage` Lua exists in ANDROID (checked the directory).

### 1.2 Per-faery data

Shared by all five ability scripts (`ANDROID/scripts/skills/faerie_*.luac`):
- Mana: `MANA_BASE = {10, 20}` indexed by skill level (clamped to 0..1), plus `0.2 x player level` (`MANA_PLVL`), plus 0 per skill level (`MANA_SLVL`).
- Range: `{600, 800}` by skill level.
- Cooldown: `GetInt("Faery_Cooldown")`. If 0, use 5000 ms. Set by the Mage passive `prince_mage_faeryblood.luac` (`SetInt("Faery_Cooldown", cooldown_time)`).
- Use: `SetSpellCooldown(cooldown)` and `UseMana(mana_cost)` in pre-skill. Targets are enemies and attackable objects, sorted NoSort, found with `TargetListSearch(range)`.
- Gating: `not HasSpellCooldown() and HasMana(mana_cost)`.
- Hit: `SpellCombatRoll` per target. Objects are hit twice to break them.

**Celest** (`faerie_celest.luac`; slot 0; Lightning; model `faeries_02_celeste`)
- Ability: lightning strike on multiple targets. Class `Spell_Lightning_1` (`ApplyPropClass(CLASS_ID + skill_level)`).
- Cast FX (pre-cast, not conditional on targets): `Celest_Level_1_Player` (AnimatedEffect name). Hit FX: `Celest_Level_1_Main`.
- Sounds: `sfx_spell_lightning_small` for 1 to 2 targets, `_medium` for 3 to 5, `_big` for 6 or more (played in pre-skill). With no targets: `StaticBallKilled`.
- Passive (`CheckFaeries`, run from `OnSkillUpdate_`): reads `GetEquippedFaeryElement()` and, for the equipped element, creates and applies one resistance buff with duration 0. Buff class names (all present in `character_classes_pyarraynames.bin`): Lightning `Celeste_LightningResistance`, Earth `Primula_EarthResistance`, Water `Maria_WaterResistance`, Air `Sylph_WindResistance`, Fire `Avalon_FireResistance`. When the faery level is 1, the `AW_` variant is used (for example `AW_Celeste_LightningResistance`).
- Only this script has a passive. Its element branch covers all five elements.

**Hotty** (`faerie_hotty.luac`; slot 4; Fire; model `faeries_00_hotty`)
- Ability: fire strike on multiple targets. Class `Spell_Fire_1` (`SetTempProps` in `OnSkillInfo_`, `ApplyPropClass` on hit).
- Cast FX (on target): `Hotty_Level_1_Player_Pre`. Hit FX: `Hotty_Level_1_Target`.
- Sounds: `sfx_spell_fire_small` / `_medium` / `_big` by target count (pre-skill). With no targets: `sfx_mage_staff_elemental_fire`.
- Cooldown: the COOLDOWN_BASE/PLVL/SLVL constants are declared, but the formula is commented out. Effective value is `GetInt("Faery_Cooldown")`, else 5000.
- No passive.

**Rocky** (`faerie_rocky.luac`; slot 1; Earth; model `faeries_03_rocky`)
- Ability: ground and rock strikes on multiple targets. Class `Spell_Earth_1`.
- Cast FX: `Rocky_Level_1_Player_Pre`. Hit FX: `Rocky_Level_1_Target`.
- Sounds: `RockyEarthSpell` when there are targets (pre-skill). Without targets: `sfx_mage_staff_elemental_earth`. After the hit: `PlaySound("sfx_faerie_earth")`.
- No passive.

**Wetty** (`faerie_wetty.luac`; slot 2; Water; model `faeries_01_wetty`)
- Ability: water strike on multiple targets. Class `Spell_Water_1`.
- Cast FX: `Wetty_Level_1_Player_Pre`. Hit FX: `Wetty_Level_1_Main`.
- Sounds: `sfx_spell_water_small` / `_medium` / `_big` by target count. Without targets: `sfx_mage_staff_elemental_ice`. After the hit: `PlaySound("sfx_faerie_ice")`.
- No passive.

**Windy** (`faerie_windy.luac`; slot 3; Air; model `faeries_04_windy`)
- Ability: wind on multiple targets. Class `Spell_Air_1`.
- Cast FX: `Windy_Level_1_Player_Pre`. Hit FX: `Windy_Level_1_Main`.
- Sounds: `WindyWhirlwindSpell` with targets. Without targets: `sfx_mage_staff_elemental_wind`. After the hit: `PlaySound("sfx_faerie_wind")`.
- No passive.

### 1.3 Animation names

Faery body clips (only the Celest set is staged as separate files, in ANDROID and PKG under `3d/characters/faeries/animations/`): `faeries_celeste_idle`, `_idle_02`, `_walk`, `_loop`, `_twist`, `_back_flip`, `_crazy`, `_alarm`, `_spell_pre_cast`, `_spell_cast`, `_despawn`, `_dying`.

Shared faery animation names (`animations_pyarraynames.bin`): `Faeries_Idle`, `Faeries_Walk`, `Faeries_Attack1H`, `Faeries_Template`. Hotty, Rocky, Wetty and Windy have no separate body clip files.

Player cast clips (`prince_spell_*`), staged in ANDROID `3d/characters/prince/animations/` and PKG `assets/animations/`:
- Hotty: `prince_spell_pre_hotty`, `prince_spell_hotty`
- Rocky: `prince_spell_pre_rocky`, `prince_spell_rocky`
- Wetty: `prince_spell_pre_wetty`, `prince_spell_wetty`
- Windy: `prince_spell_pre_windy`, `prince_spell_windy`
- Celest and general channeling: `prince_spell_pre_channeling`, `prince_spell_channeling`, `prince_spell_post_channeling`, plus `prince_spell_self` and `prince_spell_pre_self`.

The Lua scripts do not name these clips. Which clip each faery actually plays is set in the AnimTable, which I did not decode. The matrix says Celest uses Spells[0] sequence 348 and Hotty sequence 351. I did not check those numbers.

### 1.4 Faery sounds (catalog mapping)

Source: PKG `audio-assets/data/sounds/sounds.xml` maps labels to files. "PKG file" means the WAV is in `audio-assets/data/sounds/`.

| Lua call | Catalog label -> file | PKG file? |
|---|---|---|
| `sfx_spell_lightning_small/medium/big` (Celest) | -> `sfx_faerie_lightning_small/medium/big.wav` | yes |
| `StaticBallKilled` (Celest, no target) | -> `sfx_static_ball_killed.wav` | yes |
| `CelestLightningSpell` (not called by Lua) | -> `sfx_faerie_lightning_medium.wav` | yes |
| `sfx_spell_fire_small/medium/big` (Hotty) | -> `sfx_faerie_fire_small/medium/big.wav` | yes |
| `HottyFireSpell` (not called by Lua) | no catalog row (name exists in `sounds_pyarraynames.bin` only) | n/a |
| `sfx_spell_water_small/medium/big` (Wetty) | -> `sfx_faerie_water_small/medium/big.wav` | yes |
| `WettyWaterSpell` (Wetty post-hit, by label) | -> `sfx_faerie_ice.wav` | **no** |
| `RockyEarthSpell` (Rocky) | -> `sfx_faerie_earth.wav` | yes |
| `WindyWhirlwindSpell` (Windy) | -> `sfx_faerie_wind.wav` | yes |
| `sfx_mage_staff_elemental_{fire,ice,earth,wind}` (no-target, all four) | -> `sfx_mage_staff_elemental_thunder_22.wav` (same file for all four) | yes |
| `PlaySound("sfx_faerie_earth")`, `("sfx_faerie_wind")`, `("sfx_faerie_ice")` (direct post-hit calls) | no catalog label with these exact names; the labels are `RockyEarthSpell`, `WindyWhirlwindSpell`, `WettyWaterSpell` | direct-name resolution not verified; `sfx_faerie_ice` file missing |

Consequences: the Wetty post-hit cue points at a missing file (`sfx_faerie_ice.wav`). The Hotty no-target label resolves to thunder_22.

### 1.5 Effects (AnimatedEffect names, `effects_pyarraynames.bin`)

Present: `Celest_Level_1_Player`, `Celest_Level_1_Main`, `Hotty_Level_1_Player_Pre`, `Hotty_Level_1_Target`, `Rocky_Level_1_Player_Pre`, `Rocky_Level_1_Target`, `Wetty_Level_1_Player_Pre`, `Wetty_Level_1_Main`, `Windy_Level_1_Player_Pre`, `Windy_Level_1_Main`.

Name-to-file: the AnimatedEffect table row to dictionary index link is not decoded. These candidates come from the element stem in the effects dictionary (`effects_dictionary_pyarray.bin`, paths and names have the same 284 indices):
- `skill_dh2_faery_{lightning,earth,water,wind,fire}` (dict indices 74-78) -> `3d/interface/skill_dh2_faery_*.bdae`. All five are in ANDROID. PKG has only `skill_dh2_faery_lightning.bdae`.
- `spell_dh2_faery_{lightning,earth,water,wind,fire}` (dict indices 197-203) -> `3d/interface/spell_dh2_faery_*.bdae`. All five are in ANDROID. PKG has only `spell_dh2_faery_lightning.bdae`.
- `faeries_celeste_faery_dust` (dict 55) -> `3d/interface/faeries_celeste_faery_dust.bdae` (ANDROID).

Treat these element-to-FX links as inference, not verified.

---

## 2. Unlock and level sources

### 2.1 Verified in the campaign export (CAMPAIGN-XML)

- The export has 70 script tags (ids 1 to 54). It is partial: it has the swamp scripts, not the temples. It is not the full campaign.
- `Swamp_Intro` is script id 30, lines 1962 to 3048. Command index 156 (line 3005) is `Script_SetFaeryState` (kind 27) with slot 0 and state 1.
- `FaeryStates` in `faeries_pycst.bin` is Locked=0, Unlocked=1, so this unlocks slot 0, which is Celest (the list order is the slot order).
- `Swamp_Intro` is in level `001_swamp` (`001_swamp_pyscriptnames.bin` lists it). The export has no act field. The matrix calls this Act 1.
- The export contains no `Script_IncFaeryLevel` (kind 28).

### 2.2 Record scan of the binary level scripts (`ANDROID/pydata/scripts/*_pyscripts.bin`)

The binary files are not decoded in full. I searched for the 12-byte record `[kind][slot][state]` with kind 27 (`SetFaeryState`), slot 0 to 4 and state 0 or 1, and for kind 28 (`IncFaeryLevel`). The scan returns the known Swamp_Intro record at `001_swamp_pyscripts.bin` offset 8724, which validates the pattern. The enclosing script names are not decoded for these hits.

Byte offsets for each hit (state 1 in every case):

| Slot | Faery | Level file (offset) |
|---|---|---|
| 0 | Celest | 001_swamp (8724), 004_root_den (638), 009_abbey (8107), 010_elemental_lightning_room (386), 011_earth_temple_01 (364), 012_earth_temple_02 (563), 013_earth_temple_03 (612, 5961), 014_elemental_earth_room (386), 016_red_desert_cave_01 (174), 017_red_desert_cave_02 (432), 018_red_desert_cave_03 (174), 022_swamp2 (799), 022_swamp2_troll_cave (443), 023_watertemple_01 (156, 1595, 4203), 024_elemental_water_room (70), 025_icy_hub (1433), 029_wind_temple (819), 030_elemental_air_room (386), 031_firetemple (2673), 032_elemental_fire_room (386), 037_machine_rooms_01 (3399) |
| 1 | Rocky | 013_earth_temple_03 (624, 5973), 023_watertemple_01 (1607, 4215), 029_wind_temple (795), 031_firetemple (2649) |
| 2 | Wetty | 023_watertemple_01 (1619, 4227), 029_wind_temple (807), 031_firetemple (2661) |
| 3 | Windy | 029_wind_temple (783), 031_firetemple (2637) |
| 4 | Hotty | 031_firetemple (2685) |

- The slots 1 to 4 hits come in runs with a 12-byte stride. For example, the fire temple run is slots 3, 1, 2, 0, 4 at offsets 2637, 2649, 2661, 2673, 2685.
- The temple levels match the story-NPC strings in the same files: `Hiding Sylph` (029 wind temple, Windy), `Hiding two cultists and Avalon` (031 fire temple, Hotty), `_prim_Maria` (023 water temple, Wetty), `Unlocking Primula (again)` (013 earth temple 03, Rocky). This is consistent with the unlock records, but it is not proof.
- Slot 0 also appears in the five elemental trial rooms (010, 014, 024, 030, 032). I did not decode what the trials do.
- Kind 28 (`IncFaeryLevel`): no hit for slots 0 to 4 in any level. Faery level-ups are not granted by any staged level script under this pattern.

### 2.3 Not found

- The act number for each temple level: not in the data (`levels_pyarray` has no act field; the level names use sequence codes a01 to a32).
- The enclosing script name for each binary hit, except Swamp_Intro.
- Faery level-up grants (`IncFaeryLevel`): none found.
- Quests: `v2quests_pyarraynames.bin` lists `WindTemp_FindFaery`, `FireTemp_FindHotty`, `Elemental_*_Trial` and `MachineRooms01Scripts.FaeryReplace`. The quest-to-script links are not decoded. The quest reward types do not include faery state.

---

## 3. Player skills (item 5 plus Charge and GroundSlam)

All Lua is in ANDROID `scripts/skills/<file>.luac`. Class names are from `character_classes_pyarraynames.bin`, and all of them exist. "Dict" means the effects dictionary index; the path is `3d/interface/...` unless stated.

| Skill | Lua file | Class rows | FX / effects referenced in Lua | Projectile | Sound called in Lua | Header values (mana / cast / cooldown) | BDAE staging |
|---|---|---|---|---|---|---|---|
| ViciousStrike | `prince_rogue_vicious_strike.luac` | `Skill_Rogue_ViciousStrike` (+ `_Stun`, `_LowHit`, `_HighHit`) | none | none | none | 15 (+1) / 1 s / n/a. Search 300, range 160, angle 180; stun class | anim `skill_dh2_prince_rogue_vicious_strike` (ANDROID and PKG) |
| Quickness | `prince_rogue_quickness.luac` | `Skill_Rogue_Quickness`, `Skill_Rogue_Quickness_buff` | buff FX `spell_dh2_prince_rogue_quickness` (dict 239) | none | none | 15 (+1) / n/a / n/a; duration 4 s (+1 s per level). Fallback 4000 + 1000 x level | `spell_dh2_prince_rogue_quickness.bdae` ANDROID only; `skill_dh2_prince_rogue_quickness` ANDROID and PKG |
| Roundhouse | `prince_rogue_roundhouse.luac` | `Skill_Rogue_Roundhouse` | bloodsplat FX and `sfx_skill_rogue_desert_pinwheel` are both commented out, so no live FX or sound | none | none (commented) | 15 (+1) / 1 s / n/a; range 220 | `skill_dh2_prince_rogue_roundhouse` ANDROID and PKG |
| ColdRay | `prince_mage_coldray.luac` | `Skill_Mage_ColdRay` | on hit `elem_dmg_02_water` (AnimatedEffect name; not resolved to a dictionary file) | none | none | range 650, angle 90, freeze 1000 ms (no cast/cooldown in header) | `skill_dh2_prince_mage_cold_ray` (dict 122) ANDROID and PKG |
| StoneEscape | `prince_mage_stone_escape.luac` | `Skill_Mage_StoneEscape` | `spell_dh2_prince_mage_stone_escape_magefx` (dict 237) at start and end | projectile `TeleportTool`; summons CharacterTable `Dummy` as a statue | none | 10 (+2) / 1.5 s / 3 s; move 650; explosion delay 2200 ms; cooldown from SnS_Cooldown, else 5200 - 200 x level | `spell_dh2_prince_mage_stone_escape_magefx` ANDROID and PKG; dictionary also has `skill_dh2_prince_mage_stone_escape_idle` (134) and `_death` (133), not called by the Lua; `_death` is ANDROID only |
| Cyclone | `prince_mage_cyclone.luac` | `Skill_Cyclone`, `Skill_Cyclone_Proximity` | none in Lua | projectile `Tornado` (repeated spawn; victims checked against `AIStates.Stunned`) | none | no header mana/cast values | anim `skill_dh2_prince_mage_cyclone` (dict 123) ANDROID and PKG; projectile BDAE `projectile_dh2_prince_mage_cyclone.bdae` ANDROID only (name-matched, not table-linked) |
| ChainLightning | `prince_mage_chain_lightning.luac` | `Skill_Chain_Lightning` | `spell_dh2_hurt_simplified_chain_lightning` (dict 219) | projectile `SimplifiedChainLightning` per jump; `SimplifiedChainLightning_base` if no target | `sfx_skill_mage_thunder_braid` (cast, no target), `sfx_skill_mage_thunder_braid_add_1` and `_add_2` (on jumps). All three WAVs in PKG | range 1000; 5 jumps; 15% penalty per jump | projectile `projectile_dh2_simplified_chain_lightning.bdae` ANDROID only (name-matched) |
| Charge | `prince_warrior_charge.luac` | `Skill_Warrior_Charge` (+ `_ShieldBonus`) | none | none | none | 7 (+1) / 0.5 s / 1 s (SnS_Cooldown); distance 200 (+20 per level); shield bonus | anim `skill_dh2_prince_warrior_charge` (dict 151) ANDROID and PKG |
| GroundSlam | `prince_warrior_ground_slam.luac` | `Skill_Warrior_GroundSlam` | none | none | none | 10 (+1) / 1 s / 2 s; shockwave radius 300 (+5%) | anim `skill_dh2_prince_warrior_ground_slam` (dict 152) ANDROID and PKG |

Related: `prince_mage_faeryblood.luac` (class `Skill_Mage_FaeryBlood`) sets `Faery_Cooldown` (section 1.2). The skill-cast numbers (damage, exact mana) are in the class rows, which I did not decode. The values above are the header comments and the Lua constants.

Staging, package side: the Lua files are not in PKG (`assets/data/scripts/skills` has only `witch_ranged_00.luac`). The following items are missing from PKG but present in ANDROID: `spell_dh2_prince_rogue_quickness.bdae`, `skill_dh2_prince_mage_stone_escape_death.bdae`, `projectile_dh2_prince_mage_cyclone.bdae`, `projectile_dh2_simplified_chain_lightning.bdae`, `projectile_dh2_chain_lightning.bdae`, `skill_dh2_faery_water.bdae`, `spell_dh2_faery_water.bdae`.

---

## 4. Limits and open items

- The AnimatedEffect table row to dictionary index link is not decoded, so the Celest, Hotty, Rocky, Wetty and Windy `*_Level_1_*` effects are not mapped to BDAE files. Element stems are candidates only.
- Projectile names (`Tornado`, `TeleportTool`, `SimplifiedChainLightning*`) are not mapped to projectile BDAEs. The matches in section 3 are by name only.
- Numeric class values (damage, exact mana and cooldown per level) are not decoded from the class table.
- Hits in the binary level scripts are a record pattern, validated against one XML instance. Each hit's enclosing script and act are unknown.
- The matrix's faery and skill statuses (for example "missing" and "no spell") describe the port. This report describes the original data only.
- Faery text names and descriptions (the two tagged key words per row) are not resolved.
