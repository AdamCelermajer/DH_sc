# v21.8 es-passive report (ES-PASSIVE engine system, with the wiring follow-up)

Branch `v21.8-es-passive`, worktree `.claude/worktrees/es-passive` (subst J:), merged with `windows-foundation` (b087f397) before the follow-up.
Build dir `C:/dh218/es-passive-build`. Full CTest on the final tree: see "Test counts" (213 tests).
Commit: recorded in the final message of this round (the branch head).

Labels: PROVEN = read in the IDA offline export (`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`), its xref/vtable exports, or the original Lua (plain text under `.local-inputs/assets-extra/android/data/scripts/skills/`). INFERENCE = my deduction, not observed.

## Status at a glance

| Item | Status | Evidence |
|---|---|---|
| Generic passive owner wired to live saved skill ranks (stat passives) | WORKED | Real run `C:/dh218/cases/es-passive-a/run.log`: `Timed buff applied frame=60 ... class=Skill_Rogue_Acrobat source=stat passive Acrobat strength=3 ... cells=60:380->1148` (+768 raw = 3 x 256). Unit test on the real SkillTable and session. |
| Stat passive rank 0 removes; reload re-applies | WORKED (unit) | `event_passive_owner_v1_tests` section A |
| Skills-page training drives the same rank path | NOT EXERCISED | The real run sets the saved row with a TEST flag; the Skills page UI path was not driven (no harness clicks). The rank source is the same saved row the page writes. |
| Real hits call the OnTargetHit observer (melee, skill, ranged, hits received) | PARTIAL | Wired at the single applied-hit point (`ActorCombatRuntime::handle_applied_hit`), which covers melee markers, source hits and projectiles. No real run produced an applied player hit (see "Real run"), so no observed dispatch. |
| KillingFrenzy (event passive, attacker side) | PARTIAL | Unit/integration: bus -> owner -> buff, counter 1..5, 1.5 s reset, 5 s at max (`event_passive_owner_v1_tests` B). Not measured on a real hit. CanAttackInMelee gate not implemented. |
| CarrionEater (event passive, kill heal) | PARTIAL | Unit: heal = TempProp1 at rank, clamped to max, only for the player's own kill (`event_passive_owner_v1_tests` C). Not measured on a real kill. |
| Precision, SideStep, Shield-type (knockback/stun/HasBow/defender side) | DID NOT WORK | Not implemented: need the knockback/stun state path, HasBow for passives, and defender-side delivery. |
| Leech_MP (133): consumer | PARTIAL | IDA consumer found; application added (attacker MP/HP regained, clamped). Unit test for the helper; not measured on a real hit. |
| Spell_Damage_Bonus (166-170): consumer | WORKED (already in the port) | IDA `CF_CalcDamage` 0x3b1fb8 lines 130366-130372 read `PROPS_Get(attacker, a6+166)`; port `game-data/combat.cpp:71` reads `prop(attacker, element+166)`. The earlier report's "no consumer" was wrong. |
| Special_Equip_Main_In_Offhand (202): consumer | DID NOT WORK | No literal read in IDA (searched `202`, `, 202)`, `+ 202)`); no port consumer. Not located. |
| HasBow / HasStaff (`Character::_HasBow` 0x3b6f84) | DID NOT WORK for passives | IDA consumer: `GetAnimStance` (0x3a53e0 region: staff -> 3, bow -> byte_4, dual -> 2), not the damage path. The port has `query_bow` (equipment). No Lua-facing predicate was added for passives (Precision not implemented). |

## 1. Original evidence (IDA, Lua)

### 1a. Formula type 6 (done in round 1)
- `CharProperties::_ScaleWithPercentageForBuff` 0x3e2c10 (PROVEN): `dest = (sheet[P1] * (a4 ? base dest : sheet dest)) >> 8`. No added term. The port's extra `+ P1` was removed (`game-data/class_tables.cpp`, `tests/classes.cpp`).

### 1b. Buff combine rule (done in round 1)
- `CharProperties::RecalcProperty` 0x3dfe60 (PROVEN): type & 4 accumulates base, saved, gear and every buff instance where the property is set (`IsPropertySet` = value != default; `AddProperty` sums). type & 2 is set-priority. The port's resolver (`game-data/properties.cpp`) implements the same rules; no change needed.

### 1c. Buff capacity (done in round 1)
- `PROPS_AddBuff` 0x3e232c (PROVEN): capacity 1 for `false`, 128 for `true` or <= 0, N for a number. Full: a same-class instance with strength <= new is replaced; otherwise the new buff is refused. Tie-break beyond capacity 1 is an INFERENCE.

### 1d. Receiver of OnCombatResults (partly open)
- PROVEN: `CharAI::OnCombatResults` 0x3d0da4 is at byte 188 of the CharAI vtable (0x966790) and forwards to the AI object's vtable +180. `AISDefault::OnCombatResults` 0x3dc9a8 calls `OnTargetMissed` when `(result[24] & 3) != 0` and flag 0x1000 is set, else `OnTargetHit` when flag 0x800 is set, with args (attacker, target). `AISPlayer` adds 0x400 (OnKill) and does not override `OnCombatResults`.
- NOT located: the caller of 0x3d0da4 (no direct xref; no matching 4-argument virtual call). Receiver order (attacker then target) is the port model `character_combat_results_ai_v1.hpp`, not an IDA fact.

### 1e. Damage-path consumers (follow-up, this round)
- `CF_CalcDamage` 0x3b1fb8 (PROVEN, pseudocode ~130158-130400):
  - lines 130280-130286: `v61 = (damage * PROPS_Get(attacker, 132)) >> 8 / 100`, `v60 = (damage * PROPS_Get(attacker, 133)) >> 8 / 100` (the leech values, written to result words 2 and 3). Lines 130380-130395 do the same for the DOT branch.
  - lines 130366-130372: elemental bonus `PROPS_Get(attacker, a6 + 166)` added to the random damage, scaled by the defender's resistance factor (`PROPS_Get(defender, a6 + 74)`).
- `CharProperties::PROPS_GetMagicDamageBonus` 0x3df718 (PROVEN): `sheet[166 + element of the equipped faery]`.
- `F_ApplyResult` 0x3b10b4 (PROVEN, ~129876-129890): words 2 and 3 are passed to `PROPS_AddDot` (defender) as amount/duration in one branch. The mapping of words 2/3 to "leech MP" is therefore ambiguous in IDA (INFERENCE: the leech is applied through the port's own model, `game-data/combat_application.cpp`). Flagged as an open question.
- `Character::_HasBow` 0x3b6f84 -> `ItemInventory::HasBow` (not in the export); callers: `GetAnimStance` (0x3a53e0 region) only.

## 2. Implementation (this round)

- `features/generic_skills/passive_skill_owner_v1.{hpp,cpp}`: `stat_passive_class_for_script_v1` (table of the 15 non-Mage stat passives whose original script is the plain `CreateBuff(CLASS_ID, 0, false, slvl)` pattern; class names taken from `GetPyOID("ClassTable", ...)` in each script), `sync_stat_passive_ranks_v1` (saved rows -> ranks). Mage passives and Leadership (group buff) are excluded.
- `features/generic_skills/event_passive_owner_v1.{hpp,cpp}` (new): `RuntimeEventPassiveOwnerV1` on the OnTargetHit bus (attacker side only; the listener checks its own role). KillingFrenzy: the Lua counter (1..5), the 1500 ms reset that removes the buff but keeps the counter, the 5000 ms timer at 5. CarrionEater: TempProp1 at the rank healed onto the player, clamped to max, also written to the HP property. `sync_event_passive_ranks_v1`.
- `features/generic_skills/combat_result_event_bus_v1.{hpp,cpp}`: event gains `target_died`.
- `actor_combat_runtime.{hpp,cpp}`: `set_combat_result_observer`, called at the top of `handle_applied_hit` for every applied hit.
- `combat_session.{hpp,cpp}`: the observer forwarder is installed on the runtime at session creation; `set_combat_result_observer(attacker, target, outcomes, target_died)`; `has_combat_result_observer`. (The earlier call in `apply_source_result` was removed to avoid double dispatch.)
- Leech application: `apply_attacker_leech_v1` (`actor_state.hpp`, inline, clamped, non-positive ignored). Called in `CombatSystem::consume_marker` (melee markers) and `CombatSession::apply_source_result` (source hits). `PlayableActorWorld::resolve_damage_with_outcomes` and its interface gained defaulted `hp_leech_raw`/`mp_leech_raw` outputs; the test fake in `tests/actor_combat_runtime_tests.cpp` was updated to the new signature.
- `main.cpp`: per-frame wiring before `combatSession->update`: rebuild the owners when the Session changes; install the observer (queues hits); sync stat ranks from `state.skills` with `menuSourceOwner.skill_owner->borrow()`; sync event ranks. After the update: dispatch the queued hits to the bus, then advance the Killing Frenzy timer. Buff event names gained `rejected`. The faery passive path is unchanged (it still works as before).
- TEST ONLY flags (documented in `main.cpp`):
  - `--test-skill-rank NAME=RANK`: sets an EXISTING saved row at frame 60. A row that is not in the saved SkillList makes the HUD fail with `Source-known saved skill rows do not match any original SkillList order`, so the flag never adds rows. It prints the saved row names when a name is missing.
  - `--test-event-passive-rank killing_frenzy=N|carrion_eater=N`: sets the event passive's rank without a saved row (needed because no available save has a Berserker or Necromancer SkillList).
  - Observer log `Passive hit observed frame=... attacker=... target=... outcomes=... died=...` (prints only hits that involve the player).

## 3. Real run

Cases (hidden window, `quiet_click_sync.ps1`, staged v21.8 package, Rogue save from `atk-buffed` (RoguePlayerBase: JumpKick, ViciousStrike, Quickness, Roundhouse, Ambidextrous, Acrobat, ComboMaster, FindWeakness, SpecializationSkill)):

- `C:/dh218/cases/es-passive-a/run.log`: `--test-skill-rank Acrobat=3`, `--test-event-passive-rank killing_frenzy=1`, scripted `--attack-start-frame 200 --attack-frames 1`. Stat passive measured:
  - `Test skill rank Acrobat=3 frame=60`
  - `Timed buff applied frame=60 ... class=Skill_Rogue_Acrobat source=stat passive Acrobat strength=3 durationMs=0 cells=60:380->1148` (Rating_Dodge, cell 60).
  - The attack at frame 216 (`Source animation ... name=attack_offhand ... synchronousHit=1`) produced no applied hit and no observer line.
- `C:/dh218/cases/es-passive-b/run.log`: same flags, player placed at (-6850,-250,259) beside `LizTemplate_03` (combat monster, HP 47.5). Attacks at 216 and 221, no applied player hit, no observer line, no buff.
- Frames: `C:/dh218/cases/es-passive-b/f80.png`, `f250.png` (viewed: HUD and the Knight's sword swing; no damage number visible).
- Why no hit: the log states `Selected class RoguePlayerBase: source movement/HUD/menu and incoming damage; outgoing attacks await their own source pipeline`. Outgoing player damage is not wired in this build for this fixture. This is the second failed attempt at a real event-passive hit for the same reason, so per the escalation rule this step is stopped and handed to Sonnet.
- Not measured: KillingFrenzy and CarrionEater on a real hit.
- Harness note: a foreign saved row (Toughness, Hardiness) breaks the HUD (`saved rows do not match any original SkillList order`); the Rogue save's rows are listed in the log.

## 4. Test counts

- New tests: `event_passive_owner_v1` (sections A-D: live stat sync from SkillTable/CharacterState, Killing Frenzy cycle, Carrion Eater heal and clamp, leech helper). `passive_skill_owner_v1` and `runtime_timed_buff_v1` still pass.
- Full CTest on the final tree: **213 / 213 passed** (`C:/dh218/es-passive-ctest-run3.log`).

## 5. Gaps and notes for the lead

1. Outgoing player damage does not reach monsters in the real-run fixture (see section 3). Until it does, no event passive can be measured live. Escalate to Sonnet.
2. Hit observer covers applied hits only. Misses (`outcomes & 3`) do not reach OnTargetMissed listeners; no current passive needs them.
3. The defender-side receiver order is a port model (IDA caller of `CharAI::OnCombatResults` not located).
4. KillingFrenzy's CanAttackInMelee gate is not implemented (every player hit counts).
5. Leech: IDA `F_ApplyResult` uses words 2/3 in a DOT call; the port applies them as attacker regain. The mapping is unconfirmed (see 1e).
6. Buff events print `actor=-1` for apply events (the event's actor field is not filled); cosmetic.
7. Skills-page training was not driven; the rank path is the saved row the page writes.
8. Mage passives and Leadership are not in the stat table (other owners).

## Files touched (this round)

- `port/game-data/class_tables.cpp`, `port/game-data/tests/classes.cpp` (round 1)
- `port/windows-foundation/main.cpp`, `CMakeLists.txt`, `combat_session.{hpp,cpp}`, `actor_combat_runtime.{hpp,cpp}`, `actor_state.hpp`, `combat_system.{hpp,cpp}`, `playable_actor_world.{hpp,cpp}`, `tests/actor_combat_runtime_tests.cpp`
- `features/generic_skills/runtime_timed_buff_v1.{hpp,cpp}` (round 1)
- `features/generic_skills/combat_result_event_bus_v1.{hpp,cpp}`, `passive_skill_owner_v1.{hpp,cpp}`, `event_passive_owner_v1.{hpp,cpp}`, and the tests `passive_skill_owner_v1_tests.cpp`, `event_passive_owner_v1_tests.cpp`
