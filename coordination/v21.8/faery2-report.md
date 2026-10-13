# v21.8 faery2 report (wave 2, branch v21.8-faery2)

Scope: finish the five faeries (Hotty, Rocky, Wetty, Windy, Celest) on top of the merged faery work (`faery-report.md`). Build dir `C:/dh218/faery2-build`, subst `Y:` = this worktree. Evidence: `coordination/v21.8/faery2-evidence/` (frames and filtered run logs), real runs in `C:/dh218/cases/f2a-*`, `f2b-Wetty`, `f2c-Rocky`, `f2m-Celest`.

Runs: hidden-window `C:/dh218/tools/faery/batch1.ps1` (the same harness as `run-faery.ps1`, staged v21.8 package), `--test-faery <name>`, two spawned `Swamp_LizadMan_Type1` at (450,557,100) and (330,500,100), `--fixed-step .016`, key 4 cast at frame 250. Frames were looked at directly.

## Status per faery

| Faery | Target hits + damage | Status effect | Hit FX (frame 274) | Pre-cast sound | Post-hit sound | Passive buff | Own body |
|---|---|---|---|---|---|---|---|
| Celest | WORKED | none in original | WORKED (white lightning strike on both lizards) | WORKED (dispatched) `sfx_spell_lightning_small` | none in Lua | WORKED (apply) | DID NOT WORK (follow, visibility) |
| Hotty | WORKED | none in original | WORKED (fire burst, "27"/"22" floaters) | WORKED (dispatched) `sfx_spell_fire_small` | none in Lua | WORKED (apply) | DID NOT WORK (same) |
| Rocky | WORKED | none in original | WORKED (earth dust + rock spikes) | WORKED (dispatched) `RockyEarthSpell` | `sfx_faerie_earth`: label absent (silent by native rule) | WORKED (apply) | DID NOT WORK (same) |
| Wetty | WORKED | slow: PARTIAL (implemented, not seen in a real run) | WORKED (blue water burst) | WORKED (dispatched) `sfx_spell_water_small` | `sfx_faerie_ice`: FILE ABSENT, not invented | WORKED (apply) | DID NOT WORK (same) |
| Windy | WORKED | push: DID NOT WORK (outcome 0x80 seen, no monster knockback state) | WORKED (whirlwind ring) | WORKED (dispatched) `WindyWhirlwindSpell` | `sfx_faerie_wind`: label absent (silent by native rule) | WORKED (apply) | DID NOT WORK (same) |

Per-item summary (the task's items):

1. Target hits, damage, status: hits WORKED for all five (two targets each, `Damage ... SpellCombatRoll removed=22.7148 / 27.5586 outcomes=0x0`, identical across faeries because damage is class-independent in the original, see faery-report). Wetty slow: PARTIAL. Windy push: DID NOT WORK.
2. Passive buffs on `RuntimeTimedBuffOwnerV1`: WORKED for apply on equip (five runs). Damage-reduction effect of the resistance not measured. Level 1 (`AW_` variant) not exercised: `--test-faery` leaves the faery level at 0.
3. Own faery body, model, follow, idle: PARTIAL at best. Body loads and is placed as the original does; the follower AI runs; but the body does not visibly follow and is never identifiable in frames. Idle animation not verified. DID NOT WORK for follow/visibility (details below).
4. Sound per spell: pre-cast cues dispatched for all five (`status=dispatched`, logs only, no audio capture, so audibility is not verified). Missing file: `sfx_faerie_ice.wav` (0 matches under `.local-inputs`). Not invented; the Wetty post-hit cue stays silent.

## Evidence

### Target hits (item 1)
- Logs: each faery `Damage frame=275 ... target=...768 removed=22.7148` and `target=...769 removed=27.5586`, `srcmask=0x1005554a` (see `*-key-lines.txt`).
- Frames: `faery2-evidence/{celest,hotty,rocky,wetty,windy}-f274-hit.png` (viewed; FX on the lizard targets in all five; the "27"/"22" damage floaters are visible in the Hotty frame, not in the others at that frame).
- Celest fix from the earlier pass still holds: one `SpellCombatRoll` per Character (`celest_source_use_v1.cpp`).

### Strike receipt naming (bug fixed in this pass)
- The strike arm of `runtime_skill_cast_coordinator_v1.cpp` hard-coded `skill_name = "faerie_hotty"` and "Hotty OnPre completed" for all four strike faeries. Fixed: the receipt now takes the selected faery's registry entry (`find_faery_ability_by_script_v1(row.script)`) for `skill_name`, `source_script` and the detail string.
- Verified in a real Rocky run: `Source Faery key=4 ... slot=1 sequence=355 ... diagnostic=Rocky OnPre completed`.

### Passive buffs (item 2)
- Original, `scripts/skills/faerie_celest.luac` (strings): `CheckFaeries()` runs from `OnSkillUpdate_`. On equipped element or faery level change it calls `RemoveBuff(old)`, then `CreateBuff(id, 0, false, 1)` and `ApplyBuff(id)`. Element to class: Fire `Avalon_FireResistance`, Water `Maria_WaterResistance`, Air `Sylph_WindResistance`, Earth `Primula_EarthResistance`, Lightning `Celeste_LightningResistance`; level 1 uses the `AW_` variant.
- Original, IDA `Character::_CreateBuff` 0x3b86a8 forwards `(classId, duration=arg1, capacity=!arg2, strength=arg3, fx=arg4)` to `CharProperties::PROPS_AddBuff` 0x3e232c. That function starts an expiry timer only when the duration is nonzero (`if (a3) TMR_Start`). Duration 0 is therefore permanent until `RemoveBuff`. Capacity from `false` is 1.
- Port: `TimedBuffSpecV1.permanent` (new). A zero duration without `permanent` is still rejected, so skill behaviour is unchanged. `faery_passive_buff_class_v1` gives the class. `main.cpp` (faery block, per frame) removes the old class and applies the new one on change, with `strength=1`, no FX.
- Real runs: `Faery passive applied class=<row> element=N level=0 frame=0`, then `Timed buff applied frame=1 ... durationMs=0 cells=<idx>:192->7872` (Celest cell 76, Wetty 75, Hotty 74, Rocky 77), and `activeBuffs=1` in the per-frame probe.
- Unit: `runtime_timed_buff_v1` checks a permanent buff survives `advance(600 s)`, is removed by `RemoveBuff`, and restores the sheet. PASS.
- Not verified: the damage-reduction effect (no before/after damage comparison); the `AW_` variant; whether the original applies the passive only when the Celest script is loaded (inference: any equipped faery, because `CheckFaeries` reads the equipped element, not the Celest slot).

### Slow and push (item 1, status effects)
- Original, IDA `Character::F_ApplyResult` 0x3b10b4, the byte+25 bit0 branch: `PROPS_DebuffSlow(defender, value>>8)` where value = attacker property 189 (SnS_Slow_Duration) if the skill mask has 0x10000, else 146. Skill mask `0x1005554a` used by the faery spells includes 0x10000, so the Water faery uses 189.
- Original, IDA `CharProperties::PROPS_DebuffSlow` 0x3e2a5c: does nothing for a boss (`Character::IsBoss`, `ai_props flags & 4`) or a zero duration. Otherwise `PROPS_AddBuff(ClassTable "Debuff_Slow", duration, capacity 1, strength 0, fx AUTO_DEBUFF_SLOW, "debuff_slow")` then `ApplyClassToSheet` and recalc of properties 48, 47, 46. The class row exists in the authored ClassTable.
- Port: `debuff_slow_spec_v1` (`runtime_timed_buff_v1.cpp`, unit-tested: property 189 vs 146, push outcome does not slow, boss does not slow, zero duration does not slow). Hooked in the Damage event loop of `main.cpp` (`0x100` outcome on a non-dead target, attacker = event attacker, applied through `RuntimeTimedBuffOwnerV1`).
- Real run for Wetty (2 casts, 4 Water rolls, `f2b-Wetty`): all four `outcomes=0x0`. The slow path was not exercised, so Wetty slow stays PARTIAL.
- Not done: the AUTO_DEBUFF_SLOW FX (it is an AnimatedEffectTable name, not an EffectsTables set name, so the owner's FX service does not play it); the speed change on monsters (movement rebinding for monsters does not exist; the buff sheet change is only visible in the sheet, and the monster's walk rate is not confirmed to read cell 46).
- Push (Windy): IDA `F_ApplyResult` calls `CharStateMachine::SM_SetKnockBackState` for a defender with byte+24 bit7 (`0x80`). The port logs `outcomes=0x80` on the second Windy target (`f2a-Windy/run.log`), but has no monster KnockedBack state or animation bank. The mudlord2 work applies this only to the hero (B035 path) and records "no monster bank". DID NOT WORK; needs a monster knockback bank first.

### Own faery body (item 3)
- Original, IDA `Level::PlaceFaeryAndFollowers` 0x3f0898: `pos = GameObject::GetLookAtVec(player) + GetTargetPosition(player)`, then `SetPosition` and `ForceUpdatePosition`. The port uses `hero + (cos heading, sin heading, 0)`, which matches (`runtime_faery_body_v1.cpp`).
- Original, `scripts/ai/follower.luac`: `OnMasterOutOfRange -> MoveTo(master)`, `OnMasterOutOfSight -> WarpBehind`, `OnFriendSpotted -> SetMaster`, melee/close/ranged master range -> `Stop()`.
- Real runs: `Faery body loaded model=... faeries_0N_*.bdae` for all five; `Embedded FX actor _prim_Faery attached`; the follower decisions print (`[follower] actor=8886960962655148639 script=follower action=approach distance=563.848 ... action=stop distance=560.952`).
- The walk run `f2m-Celest` (scripted walk from frame 300, `--move-axis 1,0,0 --move-from-frame 300 --move-frames 460`): hero (probe) moved (203.611,557.532) -> (749.631,737.245), about 560 units. The faery actor (`Actor final ... _prim_Faery`) ended at (216.958,561.383), about 13 units from its placement. The follow decision fires but the actor does not travel. The faery is not identifiable in frames 300 to 470 (`faery2-evidence/celest-walk-f420.png`). The red-robed figure beside the hero in those frames is most likely a scene NPC (inference from its fixed scene position in the stationary frames; not confirmed).
- Correction to `faery-report.md`: its crop `v2-f3-Rocky-crop420.png` shows the Rocky spell burst, not the trailing body. The "trailing body" claim is not supported by that evidence.
- Idle clip: not verified (only the Celest clip bank has separate body clips; Hotty, Rocky, Wetty, Windy have none, per the research).

### Sound (item 4)
- Pre-cast labels resolve through `sounds.xml` to files present in the package (`sfx_faerie_{lightning,fire,water}_{small,medium,big}`, `sfx_faerie_earth`, `sfx_faerie_wind`, `sfx_static_ball_killed`, `sfx_mage_staff_elemental_thunder_22`). Dispatch is logged; audibility not captured.
- `sfx_faerie_ice.wav`: absent (0 matches under `.local-inputs`, the Wetty post-hit cue is the only loss). Not substituted.
- `sfx_faerie_earth.wav` and `sfx_faerie_wind.wav` exist, but the Lua post-hit calls `PlaySound("sfx_faerie_earth")` / `("sfx_faerie_wind")` use file names, not catalog labels. The original native 2D path returns without playing for a name absent from `Arrays::Sounds` (IDA `LuaScript::_PlaySound` 0x37e578, per the earlier pass). The port logs `label absent` for the same reason, so silence matches the original (inference from the native branch).

## Tests

- New/changed unit checks: `runtime_timed_buff_v1` (permanent buff, rejected zero duration without permanent, negative permanent rejected, debuff-slow decision: 5 cases). PASS.
- Targeted subset PASS: `faery_ability_registry_v1`, `faery_cast_sound_v1`, `runtime_faery_body_v1`, `runtime_timed_buff_v1`, `runtime_skill_use_profile*` (4 tests).
- Full CTest on the final build (`ctest-final2.txt`): **197/199**.
  - `original_melee_bindings` FAILS with "Original binding table counts changed". Its input `windows-melee-bindings/original-melee-bindings.xml` was rewritten in the shared primary at 16:23:52 by another agent's export work (`.local-inputs/windows-melee-bindings`, junctioned into this worktree). It passed in this worktree's baseline (`ctest-baseline.txt`, test 41, about 16:23). This branch's diff does not touch melee bindings.
  - `runtime_skill_activation_session_v1` FAILS at HEAD in this worktree too (baseline, built before any edit here): "Skill request CharacterState does not identify this retained session actor ... RoguePlayerBase_Assassin". Pre-existing; not from this branch. (The subclass fixtures are the known SKIP-without-binding gap in MASTER-HANDOFF.)
  - Two earlier baseline failures (`runtime_session_projectile_v1`, `runtime_session_projectile_render_v1`) were environment-only (missing junctions `runtime-session-projectile-v1` and `windows-source-clock-v21.8-staged`); fixed by junctions in this worktree.
- Baseline run on HEAD before the projectile junctions were added: 3 failures (the two projectile tests above, plus `runtime_skill_activation_session_v1`). After adding the junctions, only `runtime_skill_activation_session_v1` remains of those. Not a green 199 in this worktree.

## Files changed (this branch)

- `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.{hpp,cpp}`: `TimedBuffSpecV1::permanent`; zero-duration rule; `debuff_slow_spec_v1`.
- `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1_tests.cpp`: permanent and slow-decision checks; negative/zero-without-permanent rejections.
- `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`: strike receipt named by the selected faery's registry entry.
- `port/windows-foundation/main.cpp`: `ensureTimedBuffOwner` (owner creation moved out of the key handler, same parameters); faery passive block (per frame); Debuff_Slow hook in the Damage loop.
- `coordination/v21.8/faery2-report.md` (this file) and `coordination/v21.8/faery2-evidence/` (5 hit frames, 2 walk frames, key log lines).

## Gaps and next steps (owner in brackets)

1. Windy push on monsters: needs a monster KnockedBack state and clip bank (original `SM_SetKnockBackState`, CharAnimTable KnockedBack). [combat/AI owner]
2. Wetty slow: no real-run 0x100 yet. Next: a run with more Water rolls (more targets) and a check of cell 46 on the target; implement the AUTO_DEBUFF_SLOW FX (AnimatedEffectTable lookup, not the EffectsTables set path). [status owner]
3. Own faery follow: the follow decision runs but the actor does not move. Next: trace the `MoveTo` path for the faery actor (its motion/locomotion), and confirm the faery is visible at the walk-away distance. [companion owner]
4. Own faery idle: no clip for Hotty/Rocky/Wetty/Windy in the staged data; Celest only. [asset owner]
5. Passive: confirm faery level semantics (level 1 `AW_` variant) and the damage effect of the resistance. [passive owner]
6. Sound: `sfx_faerie_ice.wav` is missing from every input; no substitute. Audibility of dispatched cues is not captured by the harness. [audio owner]
7. Shared input: `windows-melee-bindings/original-melee-bindings.xml` changed at 16:23:52 (other agent). Re-baseline `original_melee_bindings` after that agent settles.
