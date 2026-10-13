# v21.8 skills-a: work-order item 5 (Knight/Rogue part) report

Branch `v21.8-skills-a` (worktree `.claude/worktrees/skills-a`, base `windows-foundation` @ 93f4823f). Scope: replace the 4-name whitelist with a data-driven use profile; implement Rogue ViciousStrike, Roundhouse, Quickness; verify Charge/GroundSlam/BashDown/JumpKick in a normal EXE run. Mage skills skipped.

## What changed

- `port/windows-foundation/features/generic_skills/runtime_skill_use_profile_v1.hpp` (new): `kRuntimeSkillUseProfilesV1`, one row per admitted (SkillTable name, script) pair: mana/apply class token, OnPre query (range, cone, sort, LookAt, none for GroundSlam), `sets_cooldown` (only Charge and GroundSlam call SetSkillCooldown in the Lua), Use-time target mode (requery / pre-selected single / pre-list), Use range/cone/sort (GroundSlam range = TempProp1), `rolls_objects` (ViciousStrike pops objects without rolling), Post ClearTarget, Charge shield variant. Each row is transcribed from `.local-inputs/assets-extra/android/data/scripts/skills/prince_*.luac` (plain Lua text despite the extension); the header lists per-row Lua line content.
- `runtime_skill_cast_coordinator_v1.cpp/.hpp`: `RuntimeWarriorSkillBranchV1` (4-branch enum) and the name whitelist are removed. Admission is `find_skill_use_profile_v1(name, script)`; every branch conditional now reads the profile. Behaviour for the four existing skills is unchanged except one intended correction: `SetSkillCooldown` timers start only for the authored sets_cooldown owners (Charge, GroundSlam). BashDown and JumpKick headers say "Cooldown: none/na", so no existing cooldown is lost.
- Dual-wield rows (SkillTable mask bit 0x800000): previously rejected with "Paired-hand ... not supported". Now they follow native `_SkillCombatRoll` (IDA 0x3b9fbc, audit `docs/ida-fidelity-audit-2026-10-11/character3.md` F2; port reference `port/level-world/character_skill_combat_v6.cpp`): one hit with the main hand if `HasMainHandWeapon`, then one with mask|0x4000000 if `HasOffHandWeapon`. The request carries `hand_facts_known`, `has_main_hand_weapon`, `has_off_hand_weapon`; `main.cpp` (hero skill key arm) fills them from `currentLocomotionItems()`. Inference: the audit note and the port kernel agree; I did not re-decompile 0x3b9fbc in this session.
- ViciousStrike and Roundhouse are both dual-wield rows, so both needed this branch.
- `runtime_skill_use_profile_v1_tests.cpp` + CMake `add_test(runtime_skill_use_profile_v1)` (new CTest target): checks six authored (name, script) rows exist in `skills_pyarraynames.bin`, their class tokens exist in `character_classes_pyarraynames.bin`, exact admission (Quickness and mage rows rejected), and the Lua-transcribed numbers.
- `runtime_skill_activation_session_v1_tests.cpp`: the Rogue end-to-end fixture is parameterized (`RogueSkillFixtureSpecV1`) and now also runs ViciousStrike and Roundhouse; checks the dual-wield result count (2 hits per target). Not runnable here (see gaps).
- `run_runtime_skill_activation_session_v1_tests.ps1`: added three link sources (`actor_combat_runtime.cpp`, `retained_sequence_playback.cpp`, `actor_state.cpp`); without them the runner fails to link.

## Per-skill status

Real runs: normal EXE (`dh-foundation.exe` built from this branch), hidden window (`quiet_click_sync.ps1` private desktop), staged v21.8 package as asset root, saves built from the H-report Rogue save and the afterwitch Knight save with the test tool (rank 1 bound to hotbar slot 0, level 9), `--skill-key-frame 150:2` (key 2 = slot 0), `--campaign-skip-frame` to pass the swamp intro. Logs and frames are under `C:/dh218/cases/<case>/` (run.log, f*.png). Lines quoted are from those logs.

| Skill | Status | Evidence |
|---|---|---|
| Knight BashDown | WORKED | Real run `bashdown-1`: `Source skill key=2 slot=0 frame=150 ... phase=1 skill=BashDown`; `Source player step FX ... sequence=347 ... dispatched=1 sets=164`; `Audio step source ... sequence=347 ... sound=190 status=1` (1 = dispatched); frame f162 shows the sword strike pose (seed overlay "Quest Completed" covers part of the frame). |
| Knight GroundSlam | WORKED (cast, animation, FX, sound submission) | `groundslam-1`: phase=1 skill=GroundSlam; FX `sequence=350 ... sets=173 dispatched=1`; sound `sequence=350 ... sound=185 status=1`; f170 shows the overhead strike pose. Hit application not logged. |
| Knight Charge | WORKED (cast, animation, FX, sound submission) | `charge-1`: phase=1 skill=Charge MP=21.5; FX `sequence=349 ... sets=172 dispatched=1`; sound `sequence=349 ... sound=48 status=1`; f170 shows the charge burst and sword pose. Hit application not logged (matrix "no hit seen" still open). |
| Rogue JumpKick | WORKED (cast, animation, FX, sound submission) | `jumpkick-1`: phase=1 skill=JumpKick MP=28.25 (from 40.25); FX `sequence=521 ... sets=160 dispatched=1`; sound `sequence=521 ... sound=125 status=1`; f162 shows the kick pose. Hit application not logged. |
| Rogue ViciousStrike | PARTIAL | `vicious-4`: phase=1 skill=ViciousStrike MP=24.25 (from 40.25); FX `sequence=528 ... sets=163 dispatched=1`; sound `sequence=528 ... sound=225 status=1`; f156/f170 show the dual-dagger spin (distinct from idle). Not verified: target damage (no hit line logged; the unit fixture that asserts two hits per target cannot run here), the stun side effect (the Lua applies `_Stun` class, the port does not reproduce stun tails). |
| Rogue Roundhouse | PARTIAL | `roundhouse-2`: phase=1 skill=Roundhouse MP=20.25 (from 40.25); FX `sequence=525 ... sets=162 dispatched=1`; no sound by design (Lua sound line commented out; log `sound=-1 status=1`); f162 shows the kick/spin pose. Not verified: hits (same as ViciousStrike). |
| Rogue Quickness | DID NOT WORK | `quickness-1`: `Source skill key=2 slot=0 ... phase=0 skill=Quickness ... diagnostic=No complete generic same-session Use arm is implemented for this authored source skill script`. Quickness is a timed buff (CreateBuff/ApplyBuff of `Skill_Rogue_Quickness_buff`, duration SnS_Duration or 4000+1000*level, FX `spell_dh2_prince_rogue_quickness`), and no generic timed-buff owner for the player sheet exists in the port. Left out of the table on purpose (unit test asserts it stays out). |

Mana numbers are the observed MP after the key press (the prefix is committed before the Use); I did not cross-check them against the class projection.

## Tests

- CTest (full run on the final commit, `C:/dh218/skills-a-build`): 186/188 passing. The 2 failures are `private_save_transport_win32` and `source_level_config`. Both fail on the baseline too, and both need inputs absent from the primary `.local-inputs` (`publication/checkpoint/...` assets and `windows-camera-assets`). Baseline at 93f4823f, before the worktree junctions for `windows-equipment-assets`, `windows-melee-bindings`, `actors` and `windows-toolchain` were added: 172/187, with 15 failures. Most of those were the same missing-input class; `frame_pacer` was a timing flake. New test `runtime_skill_use_profile_v1` passes.
- Run hygiene: `ctest` started through PowerShell `Start-Process` stalled repeatedly with no test child running. The same suite started from bash completed normally, so use bash for ctest in this environment.

## Evidence per AGENTS.md (investigate before implementing)

- Visual reference: NOT compared against the original footage in this session. The coverage matrix and the Lua headers were used instead. Real-run frames are the only visual evidence (see above), and they show the animation pose and the FX dispatch, not an original-video match.
- Logic reference: the Lua sources (`prince_rogue_vicious_strike`, `prince_rogue_roundhouse`, `prince_rogue_quickness` and the four whitelisted scripts) for targeting, mana, cooldown, Use, Post and sound. The dual-wield branch comes from the IDA audit note (`character3.md` F2, IDA 0x3b9fbc), cross-checked against the port's Android kernel `character_skill_combat_v6.cpp`. I did not re-decompile 0x3b9fbc in this session.
- Uncertainties: the hit side (damage on the targets) of ViciousStrike and Roundhouse was not logged in the EXE runs; the stun effect is not reproduced; mana numbers are the observed MP after the key press and were not cross-checked against the class projection; Quickness's buff row (`Skill_Rogue_Quickness_buff`) effect was not decoded.
- Verification: unit (CTest table test, syntax-checked activation fixture), real EXE (animation, FX set, sound-cue status) as listed above.
- `runtime_skill_activation_session_v1_tests` (PowerShell runner) compiles and links with the runner fix, but the binary crashes (access violation, rc 0xC0000005) or exits with "unsupported character schema version" / "Asset root is not an existing directory" depending on the launch environment. The same crash occurs at HEAD (checked with HEAD copies of the three coordinator/test files), so this is a pre-existing failure and not caused by this change. The new ViciousStrike and Roundhouse fixture rows are therefore unit-unverified.

## Gaps and next steps

1. Quickness needs a generic timed-buff owner (apply class-row props to the player's sheet for the buff duration, remove on expiry, FX). Not started.
2. Stun/knockback/DOT tails of ViciousStrike and Roundhouse results are not reproduced by the port (documented in the header).
3. Hit application for ViciousStrike and Roundhouse is unverified in the EXE (no damage log line; needs a seed with enemies in front, or a damage log). The one-hand dual-wield path (only main or only off hand) has no unit test.
4. The activation session runner needs its crash/schema failure fixed before its fixtures count as unit evidence.
5. Environment: the worktree `.local-inputs` junctions to the primary were removed at the end of this run (`cmd /c rmdir`); recreate them the same way before any build or test.

---

# Escalation round (Sonnet 5.5): Quickness timed buff, hit application, stun/knockback tails

Same branch `v21.8-skills-a`, same worktree (subst `S:`). Items: (1) Rogue Quickness with a reusable timed-buff owner, (2) verify/prove ViciousStrike and Roundhouse target damage incl. per-hand hits and the one-hand paths, (3) stun/knockback tails.

## Evidence base and its limits

- IDA through the rea MCP was NOT available in this session: `list_documents` returns `provider_unavailable` (IDA: `REA_IDA_MCP_CONFIG` not set; Ghidra: `GHIDRA_INSTALL_DIR` not set and the ELF target is not PE; Hopper: unsupported on win32). So no live decompile was done. The original behaviour was taken from the repo's recorded, ARM64-differential-tested IDA reconstruction of the native buff owner (`port/level-world/character_buffs.cpp`, `character_skill_buff_bindings_v3.cpp`, `character_property_bindings.cpp`, `character_skill_properties_v1.cpp`, `reference/character-skill-gameplay-v3/NOTES.md`, which cite 0x3b86a8 CreateBuff, 0x3b842c RemoveBuff, 0x3babdc ApplyPropClass/ApplyBuff, 0x3b9d8c GetProp, 0x495430 GrabAnimFX, 0x494978 DropAnimatedFX) and from the original Lua `prince_rogue_quickness.luac` plus the authored ClassTable/EffectsTables rows decoded with small probes. Everything below marked (inference) is mine.
- No original gameplay-video comparison of Quickness was possible: no Quickness footage note exists in `coordination/` or `docs/` (matrix: Rogue Quickness "missing", no video line). Visual evidence is real-run frames only.

## Original model recovered (Quickness)

- Lua: `OnSkillUpdate_` caches `bufftime = FromFixed(GetProp(SnS_Duration, true))` (the `true` flag reads the temporary sheet that `SetTempProps(Skill_Rogue_Quickness, slvl)` fills, see `dh2_character_get_prop`), falling back to `TIME_BASE 4000 + TIME_INC 1000 * level` only when 0. `OnPreSkill_`: `RemoveBuff(BUFF_ID, buff)` then `UseMana`. `OnSkill_`: `SetProp(SnS_Level)`, `SetBuffProp(SnS_Level)`, `CreateBuff(BUFF_ID, bufftime, false, slvl, BUFF_FX)`, `ApplyBuff(BUFF_ID, buff)`, and a Lua-side `StartSkillTimerCB(bufftime)` that only clears the Lua local. `OnPostSkill_` = `ClearTarget`.
- Data (ClassTable via probe): `Skill_Rogue_Quickness` projects `SnS_Duration = 3000 + 1000 * level` ms (4 s at level 1, as the Lua header says); `Skill_Rogue_Quickness_buff` is exactly three formulas, `Speed_Modifier_Walk`, `Speed_Modifier_Rotation`, `Speed_Modifier_Attack` each `5120 + 384 * SnS_Level` raw (fixed point /256, percent): +21.5 % at level 1, +1.5 % per level. The Lua header's "Dodge increased" has NO matching class row (no Rating_Dodge formula): not implemented, nothing to implement from data.
- Native buff owner: instance = free default sheet with `SnS_Level = strength<<8` filed under its class id (ascending map, insertion order); `ApplyBuff` evaluates the class into the instance sheet with linear sources read from the owner's resolved sheet; recalc resolves each property with the native type rule over base, saved, gear, then the buff groups (type-4 cells accumulate); expiry/removal frees the sheet, drops the FX and recalcs. First-instance FX is loaded by `GrabAnimFX` (retained looping marker, `buff_fx_load` -> `grab_marker_v28`) and released by `DropAnimatedFX` (`drop`). The EffectsTables set is `spell_dh2_prince_rogue_quickness` (set 234, one step, file 239 = `data/3D/interface/spell_dh2_prince_rogue_quickness.bdae`, loop 0, speed 1.1).
- Sound: the Lua has no PlaySound; the only cue is the AnimTable step cue of the cast animation (sequence 524, sound 552), already submitted by the existing step-sound path.

## What was built

- `features/generic_skills/runtime_timed_buff_v1.{hpp,cpp}` (new, ~430 lines): `RuntimeTimedBuffOwnerV1`, a small data-driven timed-buff / temporary-property owner. Input is only `TimedBuffSpecV1 {class_name, strength, duration_ms, fx_set_name}`; the property effect comes entirely from the authored ClassTable row, so passives and Faery buffs can reuse `apply_timed_buff` / `remove_timed_buff` later. Core `resolve_timed_buffs_v1` re-resolves the cells a buff touches with the native `dh2_property_resolve` rule and ordered buff groups over the live `base/saved/gear` of the actor sheet, and publishes through `PlayableActorWorld::update_combat_properties`. `advance(dt)` (called once per frame after the Session update, outside it) ages buffs, expires them, drops the buffs of dead/removed actors, and re-asserts the overlay if an external writer (equipment rebuild, stat change) rebuilt the sheet without it (a copy-modify writer such as the mana debit is left intact). `take_sheet_dirty` tells the host to re-derive rate-bound state; `take_events` feeds the log. FX is a std::function service pair so the owner has no renderer dependency; presentation failure never blocks the gameplay effect.
- `TimedBuffSinkV1` (abstract) is all the cast coordinator sees, so every test runner that links the coordinator alone still links.
- Use profile: `RuntimeSkillBuffProfileV1`, new `buff` pointer on `RuntimeSkillUseProfileV1`; the Quickness row (7th row) has no OnPre query, no cooldown, Post ClearTarget, class `Skill_Rogue_Quickness` for mana/projection, buff class `Skill_Rogue_Quickness_buff`, FX set name, TIME_BASE/TIME_INC fallback. Coordinator: a buff skill is rejected before any commit when no owner is bound; Focus/Pre removes the previous same-class buff; Use applies the buff with `duration = projected SnS_Duration` (fallback TIME_BASE + TIME_INC*level when 0) and `strength = rank`; the receipt carries `timed_buff_class` and `timed_buff_duration_ms`.
- `main.cpp`: lazily builds the owner on the first skill key (FX = `grab_marker_v28` on the player's manager identity, drop = `drop`), advances it after `combatSession->update`, logs `Timed buff applied/expired/...` events with the changed sheet cells, rebinds the walk/run locomotion rates from the buffed `Speed_Modifier_Walk` (attack and rotation speed are already read live per attack/turn), plus DH_STATE_TRACE-only probes (player position every 4 frames, action edges).

## Item 2: hit application, found and fixed on the way

- The hit path itself already worked (per-hand `F_SkillAttack` hits via `apply_source_result`, written in the earlier round). Real-run proof: see the log excerpt below.
- Found: the coordinator applied Use once per cast. ViciousStrike's authored clip has THREE `do_skill` markers (frames 157, 183, 202 in the real run; the Lua header describes stab, spin, scissor), and the original Event handler runs `Use` (OnSkill_: ApplyPropClass(stun class), TargetListSearch, SkillCombatRoll) per marker. Now every distinct retained `do_skill` marker of a generic skill is another Use volley (accumulating `applied_results`; hit event indices continue); an exact duplicate callback of the same marker (same wall timestamp/slot/lag), completed casts and Faery `do_spell` casts stay single-shot. BashDown/GroundSlam/Charge/JumpKick/Roundhouse/Quickness have one marker, so they are unchanged.
- Also found: the earlier-round activation session runner could not be used as unit evidence because it linked the stale prebuilt libs in `.local-inputs/windows-foundation-build` (AssetCatalog layout mismatch gave the access violation). Built against this branch's libs it runs. It is now a CTest target `runtime_skill_activation_session_v1` (coordinator compiled from source with the Warrior-only switch). It uses the v21.8 staged package when present (preview-9 lacks the Vicious/Roundhouse/Quickness clips), raises the fixture level to each row's requirement, and checks per-hand masks.

## Per-item status (this round)

| Item | Status | Evidence |
|---|---|---|
| 1. Rogue Quickness + timed-buff owner | WORKED | Unit: `runtime_timed_buff_v1` (CTest, real Knight Session) and the Rogue end-to-end cast in `runtime_skill_activation_session_v1`. Real runs `quick-3`/`quick-4`/`quick-5`/`atk-*`. Details below. |
| 2. ViciousStrike / Roundhouse target damage, per-hand, one-hand | WORKED | Real runs `vicious-6`, `round-6`: `Source skill hit` lines with damage on the target and per-hand masks; unit: both-hand, main-only and off-hand-only fixtures. |
| 3. Stun / knockback tails | DID NOT WORK (not expressible by the Session; documented) | The stun roll is computed and now visible in the log (`outcomes=0x40`), but the Session has no Stunned/KnockedBack monster state. |

### Item 1 evidence

Real EXE: normal `dh-foundation.exe` built from this branch, hidden window (`quiet_click_sync` private desktop), staged v21.8 package, Rogue save with Quickness rank 1 on hotbar slot 0, real key press (`--skill-key-frame 150:2`). Logs and frames: `C:/dh218/cases/{quick-3,quick-4,quick-5,atk-plain,atk-buffed}/`.

- Cast and buff (quick-4): `Source skill key=2 ... phase=1 skill=Quickness MP=20.25`; `Timed buff applied frame=186 ... class=Skill_Rogue_Quickness_buff strength=1 durationMs=4000 cells=46:0->5504,47:0->5504,48:0->5504`; `Timed buff speeds frame=186 walkRate=1.215 rotationRate=1.215 attackRate=1.215 locomotionRebound=1`.
- Expiry: `Timed buff expired frame=435 ... cells=46:5504->0,...` = 249 fixed-step frames x 16 ms = 3.98 s after the apply, then `walkRate=1 ... attackRate=1`.
- Move speed (quick-3, same scripted `--move-segment` run, DH_STATE_TRACE position probe every 4 frames): steady-state displacement per 4 frames is 55.6-56.9 units unbuffed (frames 116-132) and 65.2-69.9 buffed (frames 304-324, mean 67.5): about +20 % against the expected +21.5 % (the buffed segments include turn/accel ramps).
- Attack speed (`atk-plain` vs `atk-buffed`, one scripted attack press at frame 200): the swing lasts 29 frames unbuffed (action 2 at frame 200, idle at 229) and 24 frames buffed (200 to 224): ratio 1.208 against 1.215.
- FX (quick-4 frames f300 and f400 vs f455): the purple rune FX `spell_dh2_prince_rogue_quickness` is on and follows the player at f300/f400 (buff active) and is gone at f455 (after expiry). The cast-animation FX set 161 plays separately at the cast point (`Source player step FX ... sequence=524 ... dispatched=1 sets=161`). The buff FX is loaded as a GrabAnimFX looping marker; my first version used `play_set` (an auto-returning one-shot), which vanished before the buff ended (f300 of that earlier run `quick-3` shows no aura), so it was changed.
- Sound: step cue only: `Audio step source ... sequence=524 step=0 sound=552 status=1` (status 1 = dispatched). Not heard by ear (headless, as in the rest of the matrix).
- Re-cast (quick-5, second key at frame 230 while active): `Timed buff removed frame=230 ... cells=46:5504->0` (Lua OnPre RemoveBuff), `applied frame=266`, `expired frame=515`; never two instances.
- Unit: `runtime_timed_buff_v1` asserts the class numbers against an independent `apply_class`, +21.5 % at level 1, no unrelated cell changes, copy-modify write survival, re-assertion after an external rebuild, expiry after 400 ms steps with restore, FX dropped on expiry/replace/removal, replace-not-stack, two classes accumulating on the same cells with independent clocks (`Shrine_Quickness_buff`), explicit removal, FX failure tolerance, rejections (unknown class, zero duration, foreign actor), dead actor drops its buffs. `runtime_skill_activation_session_v1` runs the real Rogue slot-0 Quickness cast through the retained `do_skill` marker: one buff, no hit results, duration 4000 ms, +5504 raw on cells 46/47/48, mana spent, target cleared, a duplicate marker does not restart or stack it, expiry restores.

Not done / inferences for item 1: (inference) bufftime = projected class `SnS_Duration` (what `GetProp(SnS_Duration, true)` reads after `SetTempProps`); if the temp sheet were empty at cast time the Lua would use 4000 + 1000*level (5 s at level 1), 1 s longer. The `SetProp/SetBuffProp(SnS_Level)` writes to the owner sheet are applied only to a local copy (the cell is a runtime-only type 8). The "Dodge" part of the Lua header has no class row. The native multi-instance FX enable (`buff_fx_object`/`buff_fx_enable`) is not used: a same-class buff is always replaced here, as the Lua does. Walk-rate re-derivation relies on rebinding the locomotion clips (`bindSourcePlayerLocomotion`); a swing already in flight keeps its rate until the next attack request.

### Item 2 evidence

Real EXE (`vicious-6`, `round-6`): a Swamp lizard spawned 100 units in front of the Rogue with the existing `--spawn-test Swamp_LizadMan_Type1@990,-212,258@30` aid, real key 2 at frame 150. New log line `Source skill hit` per applied result (abridged):

```
Roundhouse    frame=183 index=0 requested=8.79297 removed=8.79297 targetHP=53.707/62.5  mask=0x8b35535 offHand=0 outcomes=0x40
Roundhouse    frame=183 index=1 requested=0       removed=0       targetHP=53.707/62.5  mask=0xcb35535 offHand=1 outcomes=0x1  (off hand missed)
ViciousStrike frame=157 index=0 miss (0x1) / index=1 removed=5.11328 targetHP=57.3867/62.5 offHand=1 outcomes=0x40
ViciousStrike frame=183 index=2 miss       / index=3 removed=5.20703 targetHP=52.1797/62.5
ViciousStrike frame=202 index=4 miss       / index=5 removed=4.44922 targetHP=47.7305/62.5
```

Floating combat text matched (`anim_sct_normaldamage...`, `Miss`). The off-hand hit carries mask bit 0x04000000 (native `_SkillCombatRoll`). The three ViciousStrike volleys are the three authored `do_skill` markers.

Unit (CTest `runtime_skill_activation_session_v1`, full Session), one `PASS Rogue <skill> main=<m> off=<o> markers=<n> results=<r>` line per run: ViciousStrike both hands `markers=3 results=12` (3 volleys x 2 targets x 2 hands), main only `results=6`, off only `results=6`; Roundhouse both hands `results=4`, main only `2`, off only `2`. Each result checks the hand mask (main = row mask, off = `|0x4000000`), target, marker and source id, that HP was removed somewhere in the first volley, and that a duplicate marker callback does not re-roll or consume RNG.

### Item 3: stun / knockback tails

- The Lua clearly applies them: ViciousStrike `ApplyPropClass(Skill_Rogue_ViciousStrike_Stun)` (`SnS_Stun_Target_Chance` 80 + 5/level percent, `SnS_Stun_Duration` 2000 + 200/level ms), the Roundhouse class has stun 95 % + 5/level and 2000 + 500/level ms, JumpKick has `SnS_Push_Target_Chance`. The port already evaluates these class cells into the attacker formula sheet, so the result roll is made (`outcomes` bit 6 = 0x40 was set on 2 of 6 ViciousStrike hits and 1 of 2 Roundhouse hits in the real runs).
- The host consumes only the `hurt` outcome (0x10, Injured reaction in `ActorCombatRuntime::start_injure_reaction`). There is no Stunned or monster KnockedBack state, AI suspension or duration timer in `CombatSession` or the AI owners (state 10 exists only for the hero push path, B035). Implementing a stun would invent that state machine (animation choice, AI/pathing hold, timer) without the original CSStunned data, so it is documented, not faked. The `Source skill hit` log line prints the raw outcome bits with a legend so the missing consumer is visible.

## Tests and build

- New CTest targets: `runtime_timed_buff_v1`, `runtime_skill_activation_session_v1` (this one makes the earlier unrunnable ViciousStrike/Roundhouse fixtures real unit evidence). `runtime_skill_use_profile_v1` now expects 7 rows (Quickness admitted with its buff arm).
- The earlier round listed `private_save_transport_win32` and `source_level_config` as failing for missing inputs; with worktree junctions to the primary `.local-inputs/publication` and `windows-camera-assets` both pass.
- Run hygiene: ctest was started from bash (not PowerShell Start-Process). A full run under machine contention takes about 50 minutes.

- Final state: full CTest 190/190 on commit 5a20eb30 (build dir C:/dh218/skills-a-build, with the worktree junctions above). Tested exe: C:/dh218/skills-a2.exe (copy of skills-a-build/dh-foundation.exe built from this branch).
- Cleanup: worktree .local-inputs junctions and the S: subst were removed at the end (junctions with cmd /c rmdir); recreate them from C:/dh218/skills-a-junctions.txt before building.
