# v21.8 es-passive report (ES-PASSIVE engine system)

Branch `v21.8-es-passive`, worktree `.claude/worktrees/es-passive` (subst J:), base `windows-foundation` @ bb604d97.
Build dir `C:/dh218/es-passive-build`. Full CTest: **206 / 206 passed** (see "Test counts").
Commit: see the final message (hash recorded at commit time).

Labels: PROVEN = read directly in the IDA offline export (`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`), its xref/vtable exports, or the original Lua. INFERENCE = my deduction, not observed. No real-run (hidden-window) evidence was gathered: the items below are engine and unit/integration level only.

## 1. Open IDA questions (answered before implementation)

### 1a. Formula type 6 (OPEN-6 / Q1) - ANSWERED, port was wrong
- `CharProperties::_ScaleWithPercentageForBuff` 0x3e2c10 (PROVEN, pseudocode ~line 164623): `Property = GetProperty(sheet, P1)`; `v9 = GetProperty(sheet, dest)`; if the buff flag `a4` is set, `v9 = GetProperty(this+2708, dest)`; `SetProperty(sheet, dest, (Property * v9) >> 8)`. There is no added term.
- `_LoadClass` 0x3e2e20 case 6 calls it with `a4`; `PROPS_ApplyClass(temp=true)` and `PROPS_ApplyClassToSheet` pass `a4=1` (PROVEN, lines ~164763, 161362, 161395).
- The port (`port/game-data/class_tables.cpp` case 6) added the raw P1 index as a constant: `add(asr8(mul(...)), a)`. That term is removed.
- Consequence (INFERENCE from the combine rule in 1c): a buff's type-6 cell is `(P1 * base dest) >> 8`, and type-4 properties accumulate it onto the base. Toughness is therefore a percentage bonus on Max_HP, not a shrink. The earlier reading "Max_HP shrinks to 12%" came from reading the buff cell as a replacement.
- Verified: standalone check of `dh2_class_formula` (buff mode 1000, self mode 256). In the passive integration test the L1 Toughness contribution to Max_HP is measured as 5533 raw on a base of 42265 (about +13%, from the authored TempProp1 row).
- Test updates: `game-data/tests/classes.cpp` synthetic case expected 129, now 128 (the old value encoded the `+a`), plus two explicit type-6 assertions. Note: `classes_audit` is not registered as a CTest test in this build, so this file is compile-checked and the formula is run standalone, not through CTest.
- Other type-6 consumers (blade_defect Rating_Defense, OffensiveAura/GuardingAura per the Paladin report) change value with this fix. Their numbers were not re-verified in game.

### 1b. Buff combine rule (OPEN-5) - ANSWERED, port already matches IDA
- `CharProperties::RecalcProperty` 0x3dfe60 (PROVEN, lines ~161950-162200):
  - `IsPropertySet(sheet, p)` = `sheet[p] != CharacterTable default[p]` (0x3df114).
  - `AddProperty(sheet, p, x)` (0x3df140): if set, `sheet[p] += x`; otherwise the value is taken as is. So the total starts at default and sums the non-default contributions.
  - type & 4 (accumulate): `+2708 := default`; add base (+8), saved (+908), gear (+1808) when set; then add EVERY buff instance whose property is set, in class-id ascending order then insertion order.
  - type & 2 (set): the first non-default value in base/saved/gear wins; otherwise the buff group with a set instance wins, last set instance in that group overriding.
- The port's `dh2_property_resolve` (`port/game-data/properties.cpp`) implements exactly these rules (`accumulate` = `out==def ? x : out+x`, which is `AddProperty`; `group` = last set instance in the first group that sets). No code change was needed. The existing test "two buff classes accumulate" covers type 4.

### 1c. Buff capacity (PROPS_AddBuff 0x3e232c) - ANSWERED, now implemented
- CreateBuff 0x3b86a8 argument decode (PROVEN): arg 2 (`capacity`) is a Lua boolean `false` -> 1, boolean `true` -> 0; a number -> that number; `a4 <= 0` -> 128 inside PROPS_AddBuff.
- Admission (PROVEN): a per-class instance deque. If the count is below capacity the instance is appended. If full: an existing instance with `strength < new` is the victim (the loop keeps the last such; the native also overwrites its strength), an equal-strength instance is a candidate chosen by the TMR_TimeLeft comparison (semantics of the out-parameters not resolved, INFERENCE). If no victim, the function returns nullptr (the new buff is refused).
- Port (implemented): `TimedBuffSpecV1::capacity`. Below capacity: append. Full: the weakest same-class instance with `strength <= new` is replaced (ties to the oldest, an INFERENCE beyond capacity 1); otherwise the event `Kind::rejected` is published and nothing changes. Capacity 0 = 128.
- Behaviour change for capacity 1: a stronger existing instance now blocks a weaker re-apply (the native behaviour). Previously every apply replaced. Existing callers (Quickness, Celest, Debuff_Slow) re-apply the same strength, so they are unchanged (`runtime_timed_buff_v1` still passes).

### 1d. Call site for the player's OnCombatResults (OPEN-4 / Q14) - PARTIAL
- PROVEN: `CharAI::OnCombatResults(Character,Character,void*)` 0x3d0da4 is the CharAI vtable (`_ZTV6CharAI`, 0x966790) entry at byte offset 188; it forwards to the AI object's vtable +180 (`AISDefault`/`AISExternal` `OnCombatResults`). `AISDefault::OnCombatResults` 0x3dc9a8 calls the Lua `OnTargetMissed` when `(result[24] & 3) != 0` and flag 0x1000 is set, else `OnTargetHit` when flag 0x800 is set, with args (attacker, target). `AISDefault::InitVCB` 0x3dc7d8 sets those flags from the VFTable. `AISPlayer::InitVCB` 0x3dd884 adds 0x400 (OnKill); `AISPlayer` does not override `OnCombatResults`, so the player inherits the default (PROVEN).
- NOT located: the caller of 0x3d0da4. It has no direct xref (`xrefs.jsonl`), and no 4-argument virtual call at vtable +188 appears in the pseudocode export. The defender-side delivery therefore rests on the port model (`character_combat_results_ai_v1.hpp`: "attacker dispatch precedes target dispatch") and on the Lua authoring (Shield, SideStep, Putrid, Mirror all check `defender == self`). Status: receiver order is a port model, not an IDA fact.
- `_commons.luac` `StealAIEvent` (PROVEN): listeners share one VFTable proxy per event name, run in registration order, each with its own skill id; the comment says some events are not propagated to Lua for players.

## 2. Implementation

| Piece | Files | Status |
|---|---|---|
| OnTargetHit / OnTargetMissed bus with attacker-then-target fan-out and per-receiver flag selection | `features/generic_skills/combat_result_event_bus_v1.{hpp,cpp}` | WORKED (unit + integration test) |
| CombatSession observer, fed after a source hit is applied | `combat_session.{hpp,cpp}` (`set_combat_result_observer`, call in `apply_source_result`) | PARTIAL: wired, not exercised by a real hit (no hit fixture in the test) |
| Generic stat passive owner (OnSkillUpdate_ pattern: remove previous, create permanent, strength = rank) | `features/generic_skills/passive_skill_owner_v1.{hpp,cpp}` | PARTIAL: engine works (Toughness test); NOT wired to live SkillTable ranks in `main.cpp` (no rank source wired; the faery passive stays hard-coded) |
| Buff capacity admission | `runtime_timed_buff_v1.{hpp,cpp}` | WORKED (capacity tests) |
| Formula type 6 fix | `port/game-data/class_tables.cpp`, `tests/classes.cpp` | WORKED (standalone formula check + passive test); other type-6 consumers not re-verified |
| Buff combine rule per property | verified, no change | WORKED (IDA equivalence, existing test) |
| Consumers of buff properties in the damage path (Leech_MP 133, Spell_Damage_Bonus 166-170, Special_Equip_Main_In_Offhand 202, HasBow/HasStaff) | none | DID NOT WORK (not attempted in this round) |

## 3. Integration tests (`features/generic_skills/passive_skill_owner_v1_tests.cpp`, CTest `passive_skill_owner_v1`)

- Stat passive (Toughness, `Skill_Berserk_Toughness`) through the generic passive owner on a real CombatSession (KnightPlayerBase fixture): L1 adds exactly its class contribution to Max_HP (type-4 accumulate); a rank change replaces it (L2 = base + L2 contribution, no stacking); the same rank is a no-op; rank 0 restores the base.
- Event passive (KillingFrenzy-style, `Skill_Berserk_KillingFrenzy`): the bus delivers a player hit once, a monster hit on the player is ignored by the attacker check, a miss is not an OnTargetHit; unlimited-capacity instances stack on the speed cell; the stack is removed by RemoveBuff.
- Capacity on the same class: unlimited accumulates; capacity 2 evicts the weakest; capacity 1 rejects a weaker newcomer (event published); capacity 1 replaces an equal-strength instance.
- Bus unit rules: both receivers served attacker first; miss selects OnTargetMissed only; unsubscribe by key; unknown callback refused.

## 4. Test counts and environment

- Full CTest on the branch: **206 / 206 passed** (run log `C:/dh218/es-passive-ctest-run2.log`). The first run before my change listed 204 tests, not the 205 in the brief; I did not reconcile that difference.
- The first baseline run failed ~30 tests (missing `.local-inputs` junctions in the worktree). Those are environmental, fixed by junctioning every `.local-inputs` directory the tests reference (39 junctions, all removable with `cmd /c rmdir`, none recursive). Failures were not code failures.
- `classes_audit` (game-data) is built only in a separate project and is not a CTest target.

## 5. Remaining gaps (for the lead)

1. Live wiring: no SkillTable rank source feeds `RuntimePassiveSkillOwnerV1` in `main.cpp`. The owner is ready; the rank-change trigger and the player's rank source still need wiring.
2. Hit coverage: `apply_source_result` is the only dispatch point. Weapon and melee hits that do not go through it do not reach the bus. Not verified in a real hit.
3. The OnTargetHit defender-side receiver is a port model (see 1d). IDA caller of `CharAI::OnCombatResults` remains open.
4. Capacity tie-break beyond capacity 1 (equal strength, multi-instance) is an inference; the TMR_TimeLeft semantics are unresolved.
5. Damage-path consumers (Leech_MP, Spell_Damage_Bonus, offhand equip flag, HasBow) are not implemented. Their status is DID NOT WORK in this round.
6. No real-run evidence exists for any of these items; the claims above are unit/integration level only.

## Files touched

- `port/game-data/class_tables.cpp`, `port/game-data/tests/classes.cpp`
- `port/windows-foundation/CMakeLists.txt`, `combat_session.hpp`, `combat_session.cpp`
- `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.{hpp,cpp}`
- New: `features/generic_skills/combat_result_event_bus_v1.{hpp,cpp}`, `passive_skill_owner_v1.{hpp,cpp}`, `passive_skill_owner_v1_tests.cpp`
- New: `coordination/v21.8/es-passive-report.md` (this file)
