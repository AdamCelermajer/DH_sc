# GetState

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetState` | bindFunction | `Character::_GetState` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132699 |
| `GetState` | bindMethod | `Character::_GetState` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132704 |

Address-identified callback: `Character::_GetState` at `0x003b6d78`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c`, unified lines 133200-133211.
Recovered declaration: `void __fastcall Character::_GetState( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c01ac.c:5`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Calls SM_GetState on receiver FSM +1276. SM_GetState reads current state pointer at FSM+32: returns pointed state ID, or -1 if absent. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c01ac.c:5`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c01ac.c:5`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c01ac.c:5`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c01ac.c:5`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_script_session_v3.cpp:249 binds FSM getter; source state-machine adapter preserves -1/current ID. Callback path proven, no integrated runtime verification.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:249 — `else if(state_machine&&b.original_callback==0x3b6d78){function=&dh2_character_native_fsm_script_get_state;context=state_machine;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** character_script_session_v3.cpp:249 binds FSM getter; source state-machine adapter preserves -1/current ID. Callback path proven, no integrated runtime verification.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: FSM current state absent; GetState() Returns-1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c01ac.c`:5.
2. **PROVEN:** Given: FSM current state ID7; GetState() Returns7. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d78.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c01ac.c`:5.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **42** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:54` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:134` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:143` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:161` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:50` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:83` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:447` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:75` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:95` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:272` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:78` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:61` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:85` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:394` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/follower.luac:32` | act1_level_boss_script_closure | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:135` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:175` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:436` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:113` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:145` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:322` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:345` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:44` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:60` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:76` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:109` | act1_level_boss_script_closure | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:151` | act1_level_boss_script_closure | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:238` | act1_level_boss_script_closure | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:67` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:91` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:271` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:76` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:251` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:119` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:29` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:211` | act1_level_boss_script_closure | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:370` | act1_level_boss_script_closure | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:664` | act1_level_boss_script_closure | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:135` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:175` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:365` | outside scoped roots | `GetState()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:181` | act1_level_boss_script_closure | `GetState()` | `[]` |

## Related natives

`Character::_GetState`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
