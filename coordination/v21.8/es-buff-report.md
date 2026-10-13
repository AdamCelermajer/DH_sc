# ES-BUFF report (engine system: buffs and state on other characters)

Agent: es-buff. Branch `v21.8-es-buff`, worktree `.claude/worktrees/es-buff` (subst K:), base `windows-foundation` @ bb604d97.
Build dir `C:/dh218/es-buff-build` (Release, llvm-mingw, CMake/Ninja 3.22.1), script `C:/dh218/es-buff-build.sh`.

## 0. Sources and method

- **IDA (offline export, read-only):** `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`. Line numbers below are in that file.
- **Lua (plain text):** `.local-inputs/assets-extra/android/data/scripts/skills/`, `prince_illusionist_{eclipse,terror,timecontrol}.luac`, `prince_paladin_conversion.luac`, `prince_assassin_contaminate.luac`.
- **Atlas (read-only, primary checkout after the coordinator's move):** `atlas show table:ebc58d6c559c2ad97b75` (Buff_Illusionist_TimeControl) resolves, `atlas find AUTO_DEBUFF_SLOW` returns references. The atlas gives no cell values, so it corroborates names and existence only.
- **Port:** `features/generic_skills/runtime_timed_buff_v1.*`, `playable_actor_world.*`, `combat_session.cpp`, `features/enemy_ai/runtime_enemy_navigation_v1.cpp`, `main.cpp`.

## 1. Original behaviour established (IDA, PROVEN unless marked)

| Native function | Pseudocode line | What it does |
|---|---|---|
| `Character::_CreateBuff` 0x3b86a8 | 134439 | Args: class id (number), duration (number, arg1), capacity (arg2: `false`→1, `true`→0, number→n), strength (arg3), FX index (arg4, number or -1), source string (arg5). Calls `PROPS_AddBuff`. **No Idle/state gate in this function** (see D-ES-1). |
| `CharProperties::PROPS_AddBuff` 0x3e232c | 164163-164400 | Per-class deque of instances (per character). `capacity<=0 → 128`. When the class is full (`size == capacity`): the victim is an instance with **strength < new strength** (the last such in deque order, which is also overwritten to the new strength) or an **equal-strength** instance chosen by timer comparison (the direction of that comparison is uncertain in the decompile, the TMR_TimeLeft outputs are not named); if none qualifies, the call returns nil and the new buff is **not** added. A victim is reused (same BuffInst object, new timer). Not full: a new instance is pushed. A zero duration starts no timer (permanent). |
| `CharProperties::PROPS_DelBuff` 0x3e101c | 163009-163160 | Finds the class decl on the **calling** character's sheet. If the deque holds exactly **one** instance the instance is removed **whatever pointer was passed** (the size test precedes the pointer test). Otherwise, with a pointer, only the instance equal to that pointer is removed; class-only with several instances does nothing. |
| `Character::_RemoveBuff` 0x3b842c | 134319 | `RemoveBuff(class)` → `PROPS_DelBuff(class, nullptr)`. `RemoveBuff(class, x)` with x not a pointer (nil) → no-op. `RemoveBuff(class, ptr)` → `PROPS_DelBuff(class, ptr)`. |
| `Character::_ApplyPropClass` 0x3babdc | 135972 | `ApplyBuff(class, ptr)` → `PROPS_ApplyClassToSheet` + recalc. |
| `Character::_SetIsTargetable` 0x3b7a64 | 133912 | Writes the bool to `Character+1045` only when the argument is a bool. |
| `Character::IsInteractive` 0x3a4870 | 121347-121360 | Friendly non-monster path returns 1; otherwise returns 0 under several gates (`+129`, `+138`, faerie, summoned, `+328 & 0x2000`), else returns `Character+1045`. The byte is therefore a **target-selection** gate. Whether attacks in progress stop on it is not established here. |
| `CharProperties::PROPS_DebuffSlow` 0x3e2a5c | 164537-164600 | Gate: **`Character::IsBoss` only** (no miniboss gate). With duration != 0: class `Debuff_Slow`, FX `AUTO_DEBUFF_SLOW`, `PROPS_AddBuff(class, duration, capacity 1, strength 0, fx, "debuff_slow")`, then recalc of cells 48, 47, 46. |
| `Character::IsBoss` / `IsMiniBoss` 0x3a3158 / 0x3a3144 | 120+ | `CharAI+20` bit 2 / bit 1. |
| `F_ApplyResult` 0x3b10b4, DoT tail | 129868-129883 | For a non-player target with duration `(a1+8)>>8 > 0` and tick `(a1+12) > 0`: `PROPS_AddDot(target+344, duration, tick, element)`. |
| `CharProperties::PROPS_AddDot` 0x3e2720 | 164407 | Element switch names the buff `dot_fire`/`dot_water`/`dot_lightning`/`dot_earth`/`dot_air`, calls `PROPS_AddBuff(..., capacity 1, ...)`, then writes the tick value to property `element + 127`. The per-tick consumer was not located in this pass. |

Lua call shapes (PROVEN, source text): `RemoveBuff(BUFF_ID, buff)` and `ApplyBuff(BUFF_ID, buff)` are global calls on the **script owner** (the MC for Eclipse, Terror, TimeControl). `defender:RemoveBuff(...)` (Conversion) is a method call on the monster. TimeControl gates `not IsPlayer`, `not IsBoss`, `not IsMiniBoss` before `CreateBuff(BUFF_ID, bufftime, false, skill_level, BUFF_FX)` and `ApplyBuff`.

The row `Buff_Illusionist_TimeControl` is **absolute** (`Speed_Modifier_Walk/Rotation/Attack`: p1 = -30, p3 = -3 per level). At level 1 the cells become -33 (factor 0.67) and replace the base value. This is verified by the unit test (section 3).

## 2. Port state before this round and discrepancies

| ID | Claim in plan / phase-1 report | Finding (evidence) |
|---|---|---|
| D-ES-1 | E3 / ES-BUFF (1): "native Idle-state gate in `_CreateBuff` 0x3b86a8 (state==3)". | **Not supported.** The full `_CreateBuff` decompile (lines 134439-134600) has no state read. Treat the gate as unverified or absent. |
| D-ES-2 | ES-PASSIVE (3) and paladin/illusionist N2: buff capacity uses **FIFO eviction**. | **Contradicted by PROPS_AddBuff.** A full class evicts a weaker instance (or an equal one by timer comparison), keeps stronger instances and rejects the new buff when nothing qualifies. There is no FIFO in the decompile. |
| D-ES-3 | ES-BUFF (3) and N11: "only walk (`Speed_Modifier_Walk`) is consumed; rotation and attack consumers not located". | **Stale.** Rotation (cell 47) is read at `combat_session.cpp:839`, and attack (cell 48) at `:887`, `:1484` and `:1971`, live at the next request. Only a real run can confirm monster behaviour. |
| D-ES-4 | ES-BUFF (4) and assassin E1: "`eligible_target` conflates acquire and attack". | **Partly stale.** The split already exists: `eligible_target` (selection, includes `selectionSuppressed_`) vs `damage_eligible` (no suppression), covered by `playable_world_selection_suppression_tests`. Open: attack-continuation sites (`combat_session.cpp:828, 939, 1669`) call `eligible_target`, so a suppressed monster stops attacking. Native attack continuation was not established. |
| D-ES-5 | D13 (illusionist): global `RemoveBuff` on the MC with a monster's handle probably does not remove the monster's buff. | **Confirmed by IDA** (PROPS_DelBuff reads the calling character's map). Refinement: if the MC holds exactly one instance of that class, the call removes **the MC's own** instance whatever handle is passed. Reproduced (section 3). |
| D-ES-6 | PROPS_DebuffSlow gate: the port's `(flags & 4)` IsBoss check. | Matches IDA (IsBoss only, no miniboss gate). Corrected: `Debuff_Slow` now requests the `AUTO_DEBUFF_SLOW` FX as the native does. |
| D-ES-7 | Q16: is `DotTickBorrow` wired to the CombatSession hit path? | **No.** `DotTickBinding` has no consumer outside `features/status_effects/`. The `AUTO_DOT_*` status tests exist but are not on the combat path. |
| D-ES-8 | Plan integration test: "enemy-targeted slow with FX and a DoT". | The DoT part is not implemented (section 4). |
| D-ES-9 | Test baseline "205". | This configure registers **204** tests (`ctest -N`). My change adds no CTest entry. Result: 204/204 passed. |

## 3. Implementation (this round)

Files: `features/generic_skills/runtime_timed_buff_v1.{hpp,cpp}`, `features/generic_skills/runtime_timed_buff_v1_tests.cpp`. Kept inside the existing owner so no CMake change and no conflict in the coordinator or `main.cpp`.

- `TimedBuffSpecV1::suppress_selection` (new): buff-coupled `set_selection_suppressed` (Blindness/Conversion pattern). The owner sets and clears only the suppression it set (`ActorBuffs::selection_owned`), so an external suppression such as a submerged boss survives.
- Handles: `apply_timed_buff_handle` returns the CreateBuff handle. A same-class replacement keeps the handle (native victim reuse). A weaker re-cast returns handle 0 and reports `TimedBuffEventV1::Kind::rejected` (native capacity-1 rule, D-ES-2).
- `remove_timed_buff_by_handle(session, caller, class, TimedBuffRemoveArgV1)`: the native shapes (class only / nil / handle) including the single-instance quirk (D-ES-5).
- `apply_timed_buff_to_targets(session, caster, targets, spec, policy, flags, outcomes)`: the per-target enemy loop. Policy covers `skip_player`, `skip_boss`, `skip_miniboss`, the caster is always skipped. Returns one outcome per target (`applied`, `rejected`, `skipped_*`, `failed`).
- `debuff_slow_spec_from_duration_v1`: `fx_set_name = "AUTO_DEBUFF_SLOW"` (presentation only; the existing FX service resolves the name in `main.cpp`).
- Tests (inside `runtime_timed_buff_v1`, all PASS, run from `C:/dh218/es-buff-build`): monsters 2/3/4 are bound into the real Knight session world. Checks:
  1. Per-target TimeControl: caster skipped, normal monster gets walk = absolute row (0.67 at L1), boss and miniboss untouched, FX requested once per applied target.
  2. RemoveBuff: caller without the class cannot remove the monster's buff with its handle (D13). Caller with one instance removes it whatever handle (quirk). Nil handle no-op. Class-only removes a single instance.
  3. Capacity-1: monster at strength 3 keeps its handle on equal re-cast. A weaker re-cast returns nil, leaves the walk cell unchanged and emits `rejected`.
  4. Selection coupling: hidden monster is not selectable while `damage_eligible` is unchanged. Expiry restores selection. An external suppression on another monster survives the buff's expiry.
  5. Faction: `FactionBuff` flips the monster's sheet and `actor.faction_id` to 8, then restores 2 at expiry (the re-sync goes through `update_combat_properties`).

Full CTest: `ctest` 204/204 passed (`C:/dh218/es-buff-ctest.log`).

Two of my own test assumptions were wrong and were corrected against the evidence, not the code: the TimeControl row is absolute (not base + delta), and the resolver's AIFaction base is `saved[0]` (the copied player base is 11). Neither needed a code change.

## 4. Item results (ES-BUFF 1-5)

**(1) Per-target enemy CreateBuff/ApplyBuff loop, FX per buff, boss gating, native RemoveBuff semantics: PARTIAL.**
- WORKED at engine/unit level: per-target loop (section 3, item 1), capacity-1 rule, handles, RemoveBuff shapes. Evidence: unit test PASS and CTest 204/204.
- DID NOT WORK: the Idle-state gate, because the native function has no such gate (D-ES-1). Nothing to implement.
- NOT DONE: the loop is not wired into a skill profile (the cast coordinator still applies buffs to the caster only, `runtime_skill_cast_coordinator_v1.cpp:1255`). No real-run in the game. The per-target FX is only the owner-level FX call, not checked on screen.

**(2) Buff-driven AIFaction with re-sync, retargeting, faction row 8 relations: PARTIAL.**
- WORKED at unit level: FactionBuff flips `actor.faction_id` to 8 and restores it at expiry (`update_combat_properties` already re-syncs `faction_id`, `playable_actor_world.cpp:~194`). Evidence: unit test item 5.
- NOT VERIFIED: retargeting. `runtime_enemy_controller_v1.cpp:418, 504, 527` re-check `eligible_target` each frame, so a monster should drop a target it no longer hates. Code read only, no run.
- NOT RESOLVED: faction row 8 relations to monster factions (Q4). Not read this round.

**(3) Monster rotation and attack speed modifiers: PARTIAL (the plan's "missing" claim is stale).**
- Consumers exist: `combat_session.cpp:839` (rotation, cell 47), `:887`, `:1484`, `:1971` (attack, cell 48), read live at the next attack request.
- NOT VERIFIED by unit or real run. No new code.

**(4) Targetability split and buff-coupled restore (Blindness, submerged SK): PARTIAL.**
- WORKED: buff-coupled selection suppression with restore and no clobbering (unit test item 4). The split (selection vs damage) pre-existed and is still tested by `playable_world_selection_suppression_tests`.
- OPEN: whether a hidden monster should stop an attack in progress. Native evidence: `IsInteractive` reads `+1045` (selection). Attack-continuation sites use `eligible_target` (`combat_session.cpp:828, 939, 1669`). Changing that needs the attack-side native gate, which was not established this round. Left unchanged so Swamp King behaviour does not move.

**(5) DoT tail (F_ApplyResult → PROPS_AddDot, per-tick damage): DID NOT WORK (not implemented).**
- Evidence: the DoT is a buff named `dot_<element>` with capacity 1 and a tick value in property `element + 127` (`PROPS_AddDot` 0x3e2720; caller `F_ApplyResult` tail, pseudocode lines 129868-129883).
- Blocked on: the per-tick consumer (not located in this pass) and a combat path that applies damage from a buff tick. `DotTickBinding` (`features/status_effects/status_live_binding.hpp`) is not wired (D-ES-7).
- Not claimed WORKED.

## 5. Escalation / process notes

- No step failed twice for the same reason in the product code. Test-authoring mistakes (absolute row, faction base, event queue) were fixed by checking the resolver and the IDA semantics.
- Test inputs: the worktree `.local-inputs` holds junctions to every top-level primary input directory (created with `New-Item -ItemType Junction`, removed with `cmd /c rmdir` only; see the release step below).
- Full CTest previously showed 37 failures only because input directories were missing in the worktree. After junctioning all primary input directories, all 204 tests pass. These failures were environmental, not caused by this change.

## 6. Open items for the lead

- Phase 2b (skill rows) should call `apply_timed_buff_to_targets` from the cast coordinator for TimeControl and Eclipse, and `apply_timed_buff_handle` plus `remove_timed_buff_by_handle` for Terror and Conversion.
- Correct the ES-PASSIVE (3) and paladin/illusionist N2 notes: capacity semantics are not FIFO (D-ES-2).
- Correct E3: `_CreateBuff` has no Idle gate (D-ES-1).
- Native attack-continuation gate for suppressed targets: needs IDA evidence before attack sites change.
- DoT: wire `PROPS_AddDot`-equivalent buffs and a tick consumer on the combat hit path (item 5 is the blocker for Pestilence and Putrid).
