# GetStateTime

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetStateTime` | bindFunction | `Character::_GetStateTime` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132708 |
| `GetStateTime` | bindMethod | `Character::_GetStateTime` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132713 |

Address-identified callback: `Character::_GetStateTime` at `0x003b6d6c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c`, unified lines 133190-133198.
Recovered declaration: `void __fastcall Character::_GetStateTime( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:7`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Pushes receiver dword +1372 (FSM+96 state time) as integer. No elapsed-time calculation or timer creation in callback. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:7`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:7`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:7`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_script_session_v3.cpp FSM callback path exposes stored state_time, not wall-clock subtraction. Units follow state-machine accumulation rather than fixed point; callback source parity only.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:250 — `else if(state_machine&&b.original_callback==0x3b6d6c){function=&dh2_character_native_fsm_script_get_time;context=state_machine;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** character_script_session_v3.cpp FSM callback path exposes stored state_time, not wall-clock subtraction. Units follow state-machine accumulation rather than fixed point; callback source parity only.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: state time field0; GetStateTime() Returns0. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c`:7.
2. **PROVEN:** Given: state time field1500; GetStateTime() Returns1500. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6d6c.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **2** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:129` | outside scoped roots | `GetStateTime()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:166` | act1_level_boss_script_closure | `GetStateTime()` | `[]` |

## Related natives

`Character::_GetStateTime`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
