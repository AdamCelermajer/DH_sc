# v21.8 item 8: faery ability registry, five faeries (faery agent)

Branch `v21.8-faery`, worktree `.claude/worktrees/faery` (subst `F:`). Build dir `C:/dh218/faery-build`. Research input: `coordination/v21.8/FAERY-SKILL-RESEARCH.md`.

## Verification sources used

- **Lua bytecode strings** (`.local-inputs/assets-extra/android/data/scripts/skills/faerie_{celest,hotty,rocky,wetty,windy}.luac`): constants, PlayFX/PlaySound/ApplyPropClass calls and the full CheckFaeries buff table. Re-verified the research claims here. Corrections below.
- **IDA export, not live IDA.** The REA provider is unavailable on this host: no IDA MCP registration, Ghidra not installed, Hopper unsupported on win32. I used the offline export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/` (`names.jsonl`, `pseudocode/`), which is not a live database.
- **Data tables:** `sounds.xml` (audio-assets) and `sounds_pyarraynames.bin`.

## Research claims re-checked

- Confirmed: Hotty, Rocky, Wetty and Windy share one skeleton (MANA_BASE {10,20}, MANA_PLVL 0.2, RANGE {600,800}, `GetInt("Faery_Cooldown")` default 5000, SpellCombatRoll, Hit FX per target). Class rows are Spell_Fire_1 / Spell_Earth_1 / Spell_Water_1 / Spell_Air_1.
- Confirmed: CheckFaeries buff per element, with the `AW_` variant at level 1. Fire Avalon_FireResistance, Water Maria_WaterResistance, Air Sylph_WindResistance, Earth Primula_EarthResistance, Lightning Celeste_LightningResistance.
- Confirmed (native, `LuaScript::_PlaySound` at `0037e578`): a 2D PlaySound whose name is not in `Arrays::Sounds` returns without playing. The 3D path (`GameObject::_PlaySound3D` at `0039212c`) does a strcmp scan of the same table and then calls Play3D with id -1.
- Confirmed: `sfx_faerie_earth`, `sfx_faerie_wind` and `sfx_faerie_ice` are absent as catalog labels (`sounds.xml` has 0 rows for each). They are file names only. The Lua post-hit `PlaySound("sfx_faerie_*")` therefore cannot resolve by label, and the original plays nothing for it (inference from the native branch plus the catalog). The port logs `uid=-1 ... label absent from the sounds table` for the same reason.
- Confirmed: `sfx_faerie_ice.wav` is absent from the package and the v21.8 staged assets. Not invented.
- Correction: the v21.8 staged package holds faeries_00/01/03/04 `.bdae` only. faeries_02_celeste.bdae is in `original-cache` (the Celest NPC attaches it in the run log), not in the staged top-level.
- Correction: the research's cooldown line is right for the default. The Mage Faery Blood override (`Faery_Cooldown`) is not wired; the port hard-codes 5000 ms (`hotty_cast_v1.cpp:434`, HUD `main.cpp` ~5560).
- Not verified: the faery_table row word meanings beyond what the research states; the passive's trigger (whether `OnSkillUpdate_` runs when a non-Celest faery is equipped).

## Changes

- `features/faery_menu/faery_ability_registry_v1.{hpp,cpp}` (new): registry keyed by SpellScript (five entries: element, model, spell class, FX names, pre/hit/no-target sound labels, passive flag). Also `faery_ability_for_list_slot_v1`, `faery_slot_from_test_argument_v1`, `faery_strike_pre_sound_labels_v1`, `faery_strike_hit_sound_labels_v1`, `faery_passive_buff_class_v1`.
- Hotty-shaped path generalized to any registry strike Faery: `hotty_cast_v1.{hpp,cpp}` (row resolve by registry, spell class from the registry, `HottyPreparedCastV1::ability`), `hotty_character_cast_v1.cpp` (validator and `hit.source_id` by registry), `hotty_effects_v1.{hpp,cpp}` and `hotty_effects_tables_v1.cpp` (FX names from the registry, `resolve_faery_effect_ids_v1`, slot-4 check removed).
- `hotty_effects_v1.cpp` and `hotty_effects_tables_v1.cpp` are now compiled (previously dead code, so Hotty FX was never dispatched).
- `runtime_skill_cast_coordinator_v1.{hpp,cpp}`: `resolve_current_faery_script` dispatches from the registry; registry-keyed pre labels; post-hit cue (`RuntimeSkillFaeryPreSoundV1::hit_phase`); receipt names the registry script.
- `main.cpp`: `--test-faery=<Celest|Rocky|Wetty|Windy|Hotty|0..4>` (also `--test-faery <value>`) equips and unlocks the slot in memory after the save loads. Per-faery FX dispatchers bound at effects bind. Key-4 gate uses the registry, not the slot number. Hit-cue log line.
- `CMakeLists.txt`: registry source, `faery_ability_registry_v1` CTest, and `hotty_effects_v1.cpp` / `hotty_effects_tables_v1.cpp` in `foundation_runtime_skill_cast`.
- The Hotty runners (`run_hotty_cast_v1_tests.ps1`, `run_hotty_session_cast_v1_tests.ps1`) add the registry source.

## Per-item status

1. **Registry keyed by table script:** WORKED
   - Unit: `faery_ability_registry_v1` CTest passes (lookup, element/model/class, FX names, pre/hit labels, passive buffs, test-arg parsing).
   - Real run: each faery's key-4 cast selects its own sequence: Hotty 351, Windy 354, Rocky 355, Wetty 356, Celest 348.
2. **Celest keeps working:** WORKED. Real run `f8v3-celest`: cast at frame 425, `do_spell completed its ordered two-roll result loop` at frame 448, lightning and StaticBallKilled cues dispatched. Full CTest green.
3. **Hotty (FX, mana, cooldown):** PARTIAL.
   - FX: now bound and dispatched (`Source Faery FX bound script=faerie_hotty PlayerSet=38 TargetSet=39`). Before this change there was no Hotty FX dispatch; the Player_Pre FX diagnostic is gone.
   - Real run `f8v3-hotty`: cast, pre-cast sound `sfx_mage_staff_elemental_fire` dispatched (no targets), do_spell Use at frame 414.
   - Not verified: Player_Pre and per-target FX need targets > 0, and no run reached a target. Mana and cooldown: the formula equals the Lua for rank 0/1 and the 5000 ms cooldown matches the default, but the MP readout (18.5 after cast in every run) is not conclusive.
4. **Rocky, Wetty, Windy (full spells):** PARTIAL.
   - Real runs `f8v3-rocky/wetty/windy`: key-4 cast accepted, the registry sequence selected, pre-cast sound dispatched (`sfx_mage_staff_elemental_earth/ice/wind`), do_spell Use completes (phase 2), receipt names the correct script.
   - Not verified: hits, target FX, and target pre-cast tiers (no target in range in any run).
   - Not implemented: their hit damage on real targets is unverified.
5. **Passive resistance buffs:** DID NOT WORK (not implemented). The element to buff mapping is in the registry (`faery_passive_buff_class_v1`) and unit-tested. Applying the ClassTable buff to the live sheet and reversing it on change needs a sheet-edit/remove path that does not exist here.
6. **Model, animation, idle-follow visuals of the player's own faery:** DID NOT WORK (not implemented). The run shows only the Celest NPC attached. Models 00/01/03/04 are staged; Hotty/Rocky/Wetty/Windy have no body clip files (research section 1.3).
7. **Sound cues:** PARTIAL.
   - Pre-cast 3D cues dispatch for all five (Celest lightning/StaticBallKilled; Hotty fire; Rocky earth; Wetty ice; Windy wind).
   - Post-hit cues `sfx_faerie_earth/ice/wind` are requested and resolve to `uid=-1 status=failed label absent`. Matches the native analysis above (silent in the original).
   - `sfx_faerie_ice.wav` is absent, so Wetty has no hit cue. Reported, not substituted.
8. **`--test-faery` test flag:** WORKED. Parser unit-tested. Five real runs (`f8v3-*`) show `Test Faery equipped slot=N`.

## Unlock sources (per research, not re-verified in the save data)

- **Act 1:** Celest only. `Script_SetFaeryState` slot 0 is in `001_swamp` Swamp_Intro (cmd 156), per `CAMPAIGN-XML`.
- **Later than Act 1 (by inference from level numbers; act field absent in data):**
  - Rocky (slot 1): temple levels 013, 023, 029, 031.
  - Wetty (slot 2): 023, 029, 031.
  - Windy (slot 3): 029, 031.
  - Hotty (slot 4): 031.
  - The record scan also shows Celest at many later levels (011-037). Only the swamp hit is confirmed in the campaign export.

## Test runs and evidence

- Harness: `C:/dh217/tools/run-case.ps1`, seed `slot1`, real key 4 (`-52` = VK 0x34), menu clicks `3:1135:226`, `6:1130:259`, cast at about 16 s, 900 frames, hidden window.
- Frames and logs copied to `coordination/v21.8/faery-evidence/f8v3-{celest,hotty,rocky,wetty,windy}-{final.png,run.log}`.
- Seed `bossroom4` fails Continue with `binding incompatible: player/targetable role changed`, both with this build and with the prior v21.7 `after-n.exe`. Pre-existing; not caused by this change.

## Test counts and limits

- CTest: 187/187 baseline in this worktree after adding junctions; final **188/188** (new `faery_ability_registry_v1`).
- `run_hotty_cast_v1_tests.ps1`: passes (compile plus fixture).
- `run_hotty_session_cast_v1_tests.ps1`: **not run.** It uses `[IO.Path]::GetRelativePath` (PowerShell 7 only); pwsh is not installed here.
- `faery_slot_has_spell_v1` in `character_state_faery_v1.hpp` is now unused by main; left in place.
- Junctions added inside `F:\.local-inputs` (ignored): the list in `C:/dh217/junctions.txt` and `junctions-extra.txt`, plus `windows-source-clock-v21.8-staged`, `windows-source-clock-v21.7`, `loot-fx-v32-assets`. Remove with `cmd /c rmdir` before any worktree delete. `F:` subst is still mapped; remove with `subst F: /D`.

## Gaps to close next

1. Target hits: walk the hero into range of a monster (no placement option exists beyond `--player-to ZONE@FRAME`) to verify Player_Pre and target FX, the pre-cast tier sounds, and damage for all strike Faeries.
2. Passive: a reversible class-buff apply on the live CombatProperties sheet keyed by equipped element and level.
3. Own-faery visual: model placement and idle-follow from the staged 00/01/03/04 models, with the Celest 02 model from `original-cache`.
4. Mage Faery Blood `Faery_Cooldown` override (not wired).

---

# Sonnet escalation pass (2026-10-13): targets, own-faery visual, handoff

Status: WIP commit; full CTest NOT re-run after the last edits. Evidence: coordination/v21.8/faery-evidence/v2-* (frames, log excerpts) plus runner scripts run-faery.ps1, batch1.ps1, crop.ps1 there.

## Item 1: spells with targets (real runs, 2 Swamp_LizadMan_Type1 spawned next to the hero)
Harness: run-faery.ps1 + batch1.ps1 use the v21.8 STAGED package (it holds the faery FX BDAEs; the v21.7 package caused the earlier "FX resource not found"). Recipe: batch1.ps1 -Tag t -Exe C:/dh218/faery-v7.exe -Names Hotty -Key "3:1135:226:0" -Frames 360 -Caps 255,262,268,274,280 -More "--fixed-step|.016|--skill-key-frame|250:4" (cast at frame 250, hit near 275). Spawns: --spawn-test ROW@X,Y,Z@FRAME (max 2 per row); slot1 seed = witch cave arrival (203.6,557.5,100.4), spawns at 450,557,100 and 330,500,100 work.
Original facts (plain-text Lua + IDA export):
- Target list: Enemy characters + AttackableOnly objects, NoSort, range 600/800; port query matches for characters; real runs targets=2 for all five.
- GetTargetListTop 4th value (gameobject) is character != null (ObjectSearcher::TargetList::Search 0x4a2f34). Characters take the if(gameobject) branch: ONE SpellCombatRoll, target FX only when damage > 0; the two-roll branch is for breakable objects.
- BUG FIXED (Celest): the port rolled twice per Character (double damage 25.4+28.2 vs 22.7/27.6). celest_source_use_v1.cpp now rolls once, Main FX only on positive damage. hotty_session_cast_v1_tests.cpp expectations updated but that PowerShell runner could NOT be run (GetRelativePath needs PS7): unverified.
- Damage is class independent in the original: F_SpellAttack ORs mask 0x1005554A (bit 27 clear) so CF__CalcDamage type 2 uses properties 79/80 (weapon damage), not SnS 174/175. Equal numbers across Hotty/Rocky/Wetty/Celest in the same RNG state are correct. Misses are real rolls (outcomes=0x1).
- Added outcomes/srcmask to the Damage frame= log line (main.cpp).
- Status effects: Spell_Air has SnS_Push_Target_Chance (183), Spell_Water has SnS_Slow_Target_Chance (188) + SnS_Slow_Duration (189); Earth/Fire/Lightning none. The formula already yields them in DamageEvent.source_outcomes (0x80 push, 0x100 slow, 0x40 stun, 0x20 scare; a Windy run showed 0x80). Applying them is not wired for any attacker. Integration point: consume receipt.source_outcomes after apply_source_result in apply_hotty_character_targets_v1; slow needs the timed-buff owner of branch v21.8-skills-a (character_slow_reaction_v1), knockback needs an enemy KnockedBack owner (b035_session_push_effect_v1 only admits the player as target).
Per faery (run d1): Hotty 2 targets, 22.7/27.6, fire FX seen (run h2), sfx_spell_fire_small, no post-hit sound in Lua. Rocky 2 targets, rock-spike FX seen, RockyEarthSpell, post-hit sfx_faerie_earth label absent (silent). Wetty 2 targets, water FX seen, sfx_spell_water_small, sfx_faerie_ice absent (silent), slow not applied. Windy 2 targets, whirlwind FX seen, WindyWhirlwindSpell, sfx_faerie_wind absent (silent), push not applied. Celest 2 targets, one roll each after the fix, sfx_spell_lightning_small.
Sound caveat: Android sounds_pyarraynames.bin has only CamelCase names; sfx_spell_* labels exist only in the package sounds.xml; the port resolves by XML label (earlier P15 decision), unchanged.
Not done: breakable-object targets (faery query is Character only; the Warrior skill path has an object arm in main.cpp objectTargets that could be reused).

## Item 2: own faery visual (implemented for all five models)
Original (IDA): Character::GetCharModelName 0x3a54d4 (Faerie-type AI row Type 3 shows the MASTER current faery ModelFile), Character::ChangeFaery 0x3ae99c (SetVisualObject), Level::PlaceFaeryAndFollowers 0x3f0898 (position = master target position + UNIT look vector; at level load, after player placement and from NativeHUDSetActiveFaery). Follow logic is the AI row script follower (follower.luac, existing B077 controller); native AISFaery is not used by this row. No faery-side cast hook found in follower.luac or the spell Lua, so no cast pose was implemented.
Implementation: features/companions/runtime_faery_body_v1.{hpp,cpp} (model uri, placement, private body visual per model, pose mirror via sample_local_pose/apply_local_pose of the actor retained clip), ctest runtime_faery_body_v1 (all five models load and sample Idle/Walk of the Celest clip bank; passes). main.cpp: per-frame block after the population visual update (equipped faery from the registry, placement when the equipped slot changes, pose mirror) and shownVisual for draw, embedded FX, billboards. Session visuals are never reloaded.
Evidence (runs f3-*): log lines Faery body loaded / Faery placed slot=N for all five; frame v2-f3-Rocky-f420.png + crop420 show the Rocky body (rocks, green glow) trailing the hero; the faery follows and stops about 559 units behind (interact 250 + melee radius). Frames of the other four exist in C:/dh218/cases/f3-*/f420.png, not reviewed.
Caveats: placed on the hero at level start (as the original), skipped while a cutscene or scripted clip owns the actor; menu equip switch is detected by polling the equipped slot and was not exercised in a real run; swamp intro interplay unchecked.

## Item 3: passive buffs (not implemented, as instructed)
Registry mapping ready: faery_passive_buff_class_v1(element, faery_level) (Lightning Celeste_LightningResistance, Earth Primula_EarthResistance, Water Maria_WaterResistance, Air Sylph_WindResistance, Fire Avalon_FireResistance; AW_ variant at faery level 1). Original: faerie_celest.luac CheckFaeries runs from the CELEST script OnSkillUpdate_ only: CreateBuff+ApplyBuff on equipped element or level change, RemoveBuff of the previous. Integration point: the per-frame faery block in main.cpp already tracks the equipped slot; call runtime_timed_buff_v1 (branch v21.8-skills-a) there.

## Known state / next steps
- Full CTest not run on the final tree (last full 188/188 before this pass); runtime_faery_body_v1 and follower_decision_v1 pass; hotty_session_cast_v1_tests edits unverified.
- Build: bash C:/dh218/faery-build.sh (needs subst F: to the worktree). ctest from bash with llvm-mingw and cmake 3.22.1 on PATH: cd C:/dh218/faery-build && ctest -j6. Exe: C:/dh218/faery-build/dh-foundation.exe; pass forward-slash paths to batch1.ps1.
- Next: full ctest and fix fallout; review f3 frames of the other four faeries and a real menu equip switch; object targets; status application once timed-buff and enemy knockback owners exist; passive hookup; cast pose only if an original trigger is found.
