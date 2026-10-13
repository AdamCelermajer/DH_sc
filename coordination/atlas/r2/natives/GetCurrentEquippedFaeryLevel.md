# GetCurrentEquippedFaeryLevel

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#129**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetCurrentEquippedFaeryLevel` | bindFunction | `Character::_GetCurrentEquippedFaeryLevel` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132586 |
| `GetCurrentEquippedFaeryLevel` | bindMethod | `Character::_GetCurrentEquippedFaeryLevel` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132591 |

Address-identified callback: `Character::_GetCurrentEquippedFaeryLevel` at `0x003b6df8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`, unified lines 133241-133254.
Recovered declaration: `void __fastcall Character::_GetCurrentEquippedFaeryLevel( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbc18.c:5`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Calls SG_GetCurrentFaerieId(receiver,-1) using current save difficulty, then SG_GetFaerieLevel(receiver,id,-1). No save pointer causes current-id0 then level-1; level is persisted save value, not live fairy actor level. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbc18.c:5`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbc18.c:5`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbc18.c:5`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbc18.c:5`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** Catalog lists getter but general CharacterScriptObjects lacks0x3b6df8 (:144-146). Selected companion actor level does not substitute receiver save/difficulty selection.

**Concrete behavioral difference:** Catalog lists getter but general CharacterScriptObjects lacks0x3b6df8 (:144-146). Selected companion actor level does not substitute receiver save/difficulty selection.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: receiver has no save; GetCurrentEquippedFaeryLevel() Returns-1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbc18.c`:5.
2. **PROVEN:** Given: save current difficulty selects fairy ID3 with persisted level5; GetCurrentEquippedFaeryLevel() Returns5. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6df8.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbc18.c`:5.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **5** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:89` | outside scoped roots | `GetCurrentEquippedFaeryLevel()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:109` | outside scoped roots | `GetCurrentEquippedFaeryLevel()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:128` | outside scoped roots | `GetCurrentEquippedFaeryLevel()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:148` | outside scoped roots | `GetCurrentEquippedFaeryLevel()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:167` | outside scoped roots | `GetCurrentEquippedFaeryLevel()` | `[]` |

## Related natives

`Character::_GetCurrentEquippedFaeryLevel`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
