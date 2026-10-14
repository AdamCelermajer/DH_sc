# HasMaster

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#57**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `HasMaster` | bindFunction | `Character::_HasMaster` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132505 |
| `HasMaster` | bindMethod | `Character::_HasMaster` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132510 |

Address-identified callback: `Character::_HasMaster` at `0x003b6f3c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c`, unified lines 133317-133330.
Recovered declaration: `void __fastcall Character::_HasMaster( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:12`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Pushes Boolean receiver master pointer at +1048 is nonzero, with no master liveness check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:12`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:12`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:12`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c:12`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** Companion source_master owners contain master state but CharacterScriptObjects lacks 0x3b6f3c (:144-146); no general receiver Boolean getter dispatch proved.
- Source evidence: `port/windows-foundation/features/companions/live_companion_scripts.cpp`:116 — `const Binding bindings[]={{"HasMaster",Operation::has_master,true,false},`.
- Source evidence: `port/windows-foundation/features/companions/source_owner_scripts.cpp`:57 — `static constexpr Source source[]={{"HasMaster",0x3b6f3c},{"HasTarget",0x3b6f50},{"GetTarget",0x3b6c7c},`.

**Concrete behavioral difference:** Companion source_master owners contain master state but CharacterScriptObjects lacks 0x3b6f3c (:144-146); no general receiver Boolean getter dispatch proved.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: master null; HasMaster() False. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c`:12.
2. **PROVEN:** Given: master nonnull but dead; HasMaster() True; no dead gate is read. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f3c.c`:12.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **9** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:13` | outside scoped roots | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/follower.luac:5` | act1_level_boss_script_closure | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:16` | outside scoped roots | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:23` | outside scoped roots | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:137` | act1_level_boss_script_closure | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:160` | act1_level_boss_script_closure | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:436` | outside scoped roots | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:455` | outside scoped roots | `HasMaster()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:461` | outside scoped roots | `HasMaster()` | `[]` |

## Related natives

`Character::_HasMaster`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
