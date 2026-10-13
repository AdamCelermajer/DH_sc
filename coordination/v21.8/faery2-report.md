# v21.8 faery2 report (wave 2, branch v21.8-faery2)

Scope: finish the five faeries (Hotty, Rocky, Wetty, Windy, Celest) on top of the merged faery work (`faery-report.md`). Build dir `C:/dh218/faery2-build`, subst `Y:` = this worktree. Evidence: `coordination/v21.8/faery2-evidence/` (frames and filtered run logs; the full runs are in `C:/dh218/cases/g*`).

Two passes: the Haiku pass (hits, passives, sounds, slow decision rule) and the Sonnet escalation pass described under "Escalation pass" (own body, Wetty slow, Windy push, CTest). The original was investigated first (IDA export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`, `scripts/ai/follower.luac`, `scripts/skills/faerie_*.luac`, authored ClassTable/AnimTable via unit-test dumps), then our code, then the fixes.

## Status per item (final)

| Item | Result |
|---|---|
| 1. Own faery body (follow, visible, idle) for Hotty, Rocky, Wetty, Windy, Celest | WORKED (the Haiku "does not follow / not identifiable" was a test-design misreading, see below) |
| 2. Wetty slow | WORKED (proc rate, hit-captured duration, timed buff owner, observable half speed) |
| 3. Windy push on monsters | WORKED (monster-side SM_SetKnockBackState reusing the B035 state machine; in-world displacement away from the caster) |
| 4. CTest | 198/200 (2 failures belong to the subclass agent; see Tests) |
| Hits/damage, hit FX, pre-cast sound, passive buff (Haiku pass) | WORKED (unchanged) |
| Sound: `sfx_faerie_ice.wav` missing; `sfx_faerie_earth/wind` Lua post-hit names are not catalog labels | unchanged limitation (see Sound) |

## Escalation pass

### Item 1: own faery body, follow, visibility, idle

Original investigation:
- `Level::PlaceFaeryAndFollowers` 0x3f0898 places the faery at the master + unit look vector. `CharAI::SetScriptByName` 0x3ceeb0: AI row `_prim_Faery` (row 20) has script `follower`, so the Lua `scripts/ai/follower.luac` is the live AI (the native `AISFaery` is only for the script name `__faery__`, not used by this row). Events come from `CharAI::_UpdateMaster` 0x3cc5a4 (`IsMyTurn` is always true for a faerie).
- `follower.luac`: `OnMasterOutOfRange -> MoveTo(master)`, any "in range" event (24 close, 25 melee, 23 ranged) `-> Stop()`, `OnMasterOutOfSight -> WarpBehind`. For the faerie `Character::CanRangeAttack` is false (no weapon, no ranged skill), so the melee branch applies: out of range (22) vs melee range (25) = `AI_IsInInteractionRange` 0x3d4f98 (the master is not an enemy): `(distance - ownMeleeRadius - masterRadius) <= AI InteractRadius`.
- `masterRadius` is `vtable+148` of the master = `Character::GetInteractionRadius` 0x3a374c = `CharAI::AI_GetMeleeRadius(master)` (AI row MeleeRadius + weapon reach), NOT the navigation body radius.
- AI row 20: view 2300, interact 250, melee 200; the hero's melee reach is 200. The faery therefore walks only when the hero is farther than 250 + 200 + 200 = 650 units, stops again as soon as it is within 650, so it trails the hero by up to ~650 units (about 450 px on screen). This is the original's rule from the data (inference: not confirmed against footage; no faery-follow video note exists in the repo).

What was wrong in the Haiku verdict: its scripted walk moved the hero 560 units, which never exceeded the (then wrongly smaller-radius) threshold for long, so the faery legitimately moved ~13 units; and it looked for a body shape near the hero. The body IS visible: Celest, Hotty, Rocky, Wetty and Windy are the original models (`faeries_0N_*.bdae`: two 158-vertex wing meshes from `atlas_dh2_game_objects_001.tga` plus an 8-vertex glow quad from `atlas_fx_particles_001.tga`, the elemental aura FX is the embedded `_prim_Faery` actor FX). A small white-winged glow, one per element colour, not a humanoid figure. The robed figure the Haiku pass saw next to the hero is Rene (the Priest follower), a scene NPC.

Code changes (`main.cpp`, `runtime_follower_controller_v1.cpp`):
- `followServices.body_radius` now returns `PlayableActorWorld::melee_reach(master)` (original `GetInteractionRadius`), was the navigation body radius (113.7). Shared follower behaviour change: Rene's approach threshold moves from 563 to 650 as well (original-backed; the follower log line now prints `ownReach` and `masterRadius`).
- Diagnostic only: `Faery body trace` line every 20 frames under `--test-faery` (position, clip, action), `Faery body mesh` material lines, `Actor probe` lines (all session actors, every 4 frames, under `DH_STATE_TRACE`).

Real-run evidence (hidden window, harness `C:/dh218/tools/faery/run-faery.ps1`, frames viewed):
- Follow: `g3-follow` (Celest, hero `--move-run` -x from frame 100): faery stays at (204,557) while the hero is within 650, then `[follower] action=approach distance=654.15`, trace position (-54.6,501) @200, (-399.6,373) @240, (-716,158) @280, (-890,-127) @320, `clip=.../Walk/...` while moving, `clip=.../Idle/...` after `action=stop distance=641` (see `faery2-evidence/follow-trace.txt`). The hero ended at (-1137,-921). Frames `celest-follow-f280.png`, `celest-follow-f360.png` (the faery glow is visible on screen, trailing the hero). The same run for Hotty, Rocky, Wetty and Windy gives the same trace (`g5-*`) and `four-faeries-trailing-hero-f280.png` shows the four coloured faeries (orange, green, cyan, magenta) trailing the hero.
- Idle animation: `five-faeries-idle-animated-f100-f120.png` (hero teleported 574 units away with `--player-to`, faery stationary): the wing meshes change between frame 100 and 120 for all five faeries (the actor's retained `Idle` clip is mirrored onto the equipped body, `mirror_pose`).
- Models differ per faery and load (`Faery body loaded model=...faeries_00_hotty / 01_wetty / 02_celeste / 03_rocky / 04_windy`).

### Item 2: Wetty slow

Original:
- `Character::F_ApplyResult` (the status block at 0x3b01b8, whose chain is guarded by `IsDead`, not `IsPlayer`): when the result byte+25 bit0 is set -> `CharProperties::PROPS_DebuffSlow(defender, attackerProp(189 if mask & 0x10000 else 146) >> 8)`. The bit is rolled by `CF__CalcSlow` (spell mode 2) in the calc function: `chance = SnS_Slow_Target_Chance(188) + attackerLevel/8 - defender Slow_Resist_Chance(144) - defenderLevel/8`; it procs when `chance > Random(100) << 8`; a non-positive property 188 never procs.
- The chance is NOT on the hero: `faerie_wetty.luac` OnSkill_ does `SetProp(SnS_Level)`, `ApplyPropClass(Spell_Water_1 + skill_level)` and then `SpellCombatRoll(target)`. The authored ClassTable rows are (dumped by the new test `faery_status_proc_v1`): `Spell_Water_1` slow 15 % / 5000 ms, `Spell_Water_2` 25 % / 5000 ms, no other Water row carries stun/fear/hurt/push. Only the Air rows carry a push chance (80 / 105). So the slow is a 15 % proc chance (minus the level terms), correctly implemented; four rolls without a proc were just chance (0.85^4 = 52 %).

Bug found and fixed: the Debuff_Slow hook read the duration (property 189) from the hero's LIVE sheet, where the temporary Spell_Water class is not applied, so a proc would have been silently dropped (duration 0). Now `DamageEvent::source_slow_duration_raw` carries property 189/146 of the sheet the hit was rolled with (`CombatSession::apply_source_result`), and `debuff_slow_spec_from_duration_v1` turns it into the `Debuff_Slow` timed buff (owner `RuntimeTimedBuffOwnerV1`).

Second gap fixed: the monster walk rate was fixed in the navigation config, so the slow changed the sheet but not the speed. `RuntimeEnemyNavigationV1` now re-binds the actor's walk locomotion at `walk_rate * live/base` of `Speed_Modifier_Walk` (cell 46) whenever it changes (`original_speed_modifier_ratio`); a monster whose base modifier is not 1.0 keeps its old rate until a buff changes the cell.

Test aid (as requested): `--test-rng-at SEED@FRAME` resets the persisted combat RNG stream to `{SEED,0}` at that frame (`PlayableActorWorld::set_random_state_for_test`). Seed search (first cast, two lizards, 64 rolls): slow at seeds 11 (2nd target), 13, 20, 31 (1st target); the same procs also occur without the aid (`g9-slow-0`, 4th roll of a normal run).

Real-run evidence (`faery2-evidence/wetty-slow-evidence.txt`, scenario `g15`: identical runs except the seed; slow = seed 13, control = seed 3):
- `Damage frame=195 ... marker=SpellCombatRoll outcomes=0x100`, `Debuff slow applied target=...768 durationMs=5000`, `Timed buff applied ... class=Debuff_Slow ... cells=46:0->-12800,47:0->-12800,48:0->-12800` (the authored row is -50 % walk/rotation/attack), `[nav] walk rate rebound ... modifierRatio=0.5`.
- Observable speed: both lizards walk toward the hero from frame 156; identical until the slow. The control lizard covers (615,728) -> (570,718) in 24 frames (stops at f228); the slowed one needs 48 frames (f204 -> f252) for the same distance, the probe shows `walkRate=0.5` for the slowed actor only.
- Unit: `faery_status_proc_v1` (authored Spell_* rows: Water-only slow 15/25 % 5000 ms, Air-only push 80/105, no hurt/stun/fear), `runtime_timed_buff_v1` (hit-captured duration rule, Debuff_Slow row = -12800 on cells 46/47/48, ratio 0.5, ratio edge cases).
- Not done: the original `AUTO_DEBUFF_SLOW` FX (AnimatedEffectTable name, not an EffectsTables set, so the timed buff owner's FX service does not play it).

### Item 3: Windy push on monsters

Original (all verified in IDA):
- `F_ApplyResult` chain (0x3b01b8, guarded by `IsDead`): when result byte+24 bit7 (push, rolled by `CF__CalcPush`, Air_1 80 %, Air_2 105 %) is set: `CharStateMachine::SM_SetKnockBackState(great = mask & 0x100000, attacker, direct = mask & 0x18000000)`; the faery mask `0x1005554a` has bit 0x10000000 (direct `_SetState(10, 50011)`) and not 0x100000 (normal variant).
- `SM_SetKnockBackState` 0x3c5ea0: no-op for a Boss; otherwise it reads the defender's own `CharAnimTable` row, +72 (KnockedBack) or +36 (GreatKnockedBack), stanced by the AnimStancedAnim bits 0x400/0x800 (`GetAnimStance`), then state 10.
- `CSKnockedBack` 0x3c4a48/0x3c48e0/0x3c5ab0: OnFocus sets the animation, turns the actor to face the attacker (`LookAt`), switches the physics filter and unpins; there is no impulse, the displacement is the knockback clip's own root motion (MoveGO); state 10 ends with the clip and returns to Idle. The Lizardman row (CharAnimTable 34) has authored roots 378 (normal) / 375 (great), the Moth rows 449/446, Lizardman Type2 89/86.
- The player-side consumer (B035, mudlord2/3) already implements exactly this state machine; it was player-only by two gates, not by the original.

Implementation (reuse of B035, no new state machine):
- `capture_session_push_admission_v1`: the admission is now `alive && state accepts event 50011` (was additionally `is_player`).
- `CombatSession::play_actor_source_sequence`: the KnockedBack10 gate no longer requires `traits->is_player`.
- `main.cpp`: a push bank per combat monster profile (`monsterPushBanks`, built from the defender's own CharAnimTable row, stance 0, clips appended to that profile's Session visual before initialize, log `[push] monster <profile> CharAnimTable=.. bank normal=.. great=.. pushClips=..`); the push sink looks the bank up by the target's population profile, and a profile without a bank is a logged no-op.

Real-run evidence (`faery2-evidence/windy-knockback-evidence.txt`, `windy-monster-knockback-f272-f360.png`, `windy-monster-knockback-f284.png`): `[push] KnockBack monster target=...769 profile=Swamp_LizadMan_Type1 ... great=0 direct=1 outcomes=0x80 mask=0x1005554a`; the pushed lizard goes to `action=6` (knocked_back) at frame 276 and is displaced by the clip root motion from (358.7,474.3) to (462.0,406.3) (about 125 units, directly away from the caster at (203.6,557.5)), stays in the state until frame ~348, then returns to its AI (`action=2`, attacking); the second, unpushed lizard (outcome 0x0) is unaffected; the frames show the lizard thrown back inside the whirlwind FX.
- Unit: `b035_session_push_effect_v1` now also asserts that a monster defender (pre-hit Idle) is admitted (`!target_was_player`); `faery_status_proc_v1` asserts the Lizardman and Knight CharAnimTable rows carry both knockback roots.
- Not verified: the great (0x100000) variant on a monster (bank exists, no skill with that mask was run); Boss defenders (source no-op, covered by the existing `ai->flags & 4` gate in `play_session_push_animation_v1`); the monster hurt animation concurrently with a push (needs a spell that rolls both).

## Hits, passives, sound (Haiku pass, unchanged)

| Faery | Target hits + damage | Hit FX (frame 274) | Pre-cast sound | Post-hit sound | Passive buff |
|---|---|---|---|---|---|
| Celest | WORKED | WORKED (lightning on both lizards) | WORKED (dispatched) `sfx_spell_lightning_small` | none in Lua | WORKED (apply) |
| Hotty | WORKED | WORKED (fire burst, floaters) | WORKED `sfx_spell_fire_small` | none in Lua | WORKED |
| Rocky | WORKED | WORKED (earth dust, spikes) | WORKED `RockyEarthSpell` | `sfx_faerie_earth`: label absent (silent by native rule) | WORKED |
| Wetty | WORKED | WORKED (water burst) | WORKED `sfx_spell_water_small` | `sfx_faerie_ice`: FILE ABSENT, not invented | WORKED |
| Windy | WORKED | WORKED (whirlwind ring) | WORKED `WindyWhirlwindSpell` | `sfx_faerie_wind`: label absent (silent by native rule) | WORKED |

- Hits: `Damage ... SpellCombatRoll` for every target; one `SpellCombatRoll` per Character (Celest fix from the earlier pass).
- Strike receipt naming fixed earlier (`runtime_skill_cast_coordinator_v1.cpp` names the selected faery).
- Passives: original `faerie_celest.luac CheckFaeries` -> `CreateBuff(class, 0, false, 1)` + `ApplyBuff`; duration 0 is permanent (`PROPS_AddBuff` starts a timer only for a nonzero duration). Port: `TimedBuffSpecV1.permanent`, applied on equip (`Faery passive applied ...`, `Timed buff applied ... durationMs=0`). Not measured: the damage-reduction effect of the resistance, the `AW_` variant for level 1 (`--test-faery` leaves level 0).
- Sound: dispatch is logged (`status=dispatched`), audibility is not captured by the harness. `sfx_faerie_ice.wav` exists in no input; not substituted. `sfx_faerie_earth`/`sfx_faerie_wind` are file names, not catalog labels; the original native 2D path returns silently for a name absent from `Arrays::Sounds` (inference from `LuaScript::_PlaySound` 0x37e578).

## Tests

- New/changed: `faery_status_proc_v1` (new), `runtime_timed_buff_v1` (hit-captured slow rule, Debuff_Slow row, speed ratio), `b035_session_push_effect_v1` (monster admission).
- Full CTest on the final build (`C:/dh218/faery2-ctest-v3.txt`, then the six non-passing tests re-run after adding the missing `.local-inputs` junctions): **198/200 pass**. The first full run showed 6 failures; four (`equipment_visual`, `source_session_program`, `session_skill_binding`, `combat_session_source_departure`) were only missing junctions (`windows-equipment-assets`, `actors`, dictionary inputs) and pass with the junctions. The remaining two are another agent's work and were not touched:
  - `original_melee_bindings` FAILS "Original binding table counts changed": the shared `.local-inputs/windows-melee-bindings/original-melee-bindings.xml` was rewritten by the subclass agent (adds subclass actors). Not modified here.
  - `runtime_skill_activation_session_v1` FAILS "Skill request CharacterState does not identify this retained session actor ... state-class=RoguePlayerBase_Assassin": the Assassin/subclass work in progress (the same failure appears on the untouched baseline of this worktree).

## Files changed (this pass)

- `port/windows-foundation/main.cpp`: follower master radius (melee reach), faery body/actor diagnostics, `--test-rng-at`, slow hook uses the hit-captured duration, monster push banks + monster branch of the push sink.
- `features/companions/runtime_follower_controller_v1.cpp`: log fields.
- `features/enemy_ai/runtime_enemy_navigation_v1.{hpp,cpp}`: live walk-rate re-bind. `original_actor_properties.{hpp,cpp}`: `original_speed_modifier_ratio`.
- `combat_system.hpp`, `combat_session.cpp`: `DamageEvent::source_slow_duration_raw`; KnockedBack10 gate no longer player-only. `playable_actor_world.{hpp,cpp}`: `set_random_state_for_test`.
- `features/generic_skills/runtime_timed_buff_v1.{hpp,cpp,_tests.cpp}`, `features/combat/b035_session_push_effect_v1{.cpp,_tests.cpp}`, `features/faery_menu/tests/faery_status_proc_v1_tests.cpp` (+ `CMakeLists.txt`).
- `coordination/v21.8/faery2-report.md`, `faery2-evidence/`.

## Remaining gaps

1. Faery follow distance (650 units) is the data-driven result of the original rules; no original footage was available to compare (inference).
2. Wetty slow FX (`AUTO_DEBUFF_SLOW`) and the great-knockback variant on a monster are not exercised; resistance passives' damage effect not measured; `sfx_faerie_ice.wav` is absent from every input.
3. The monster hurt reaction concurrent with a push, Boss defender knockback (source no-op) and the stun/fear/push of other skills reuse the same path but were not run here.
4. Audibility of dispatched cues is not captured.
