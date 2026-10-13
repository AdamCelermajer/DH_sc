# GetEquippedFaeryElement

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#116**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetEquippedFaeryElement` | bindFunction and/or Method (see registration ledger) | `Character::_GetEquippedFaeryElement` | `Character::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:330 |

Address-identified callback: `Character::_GetEquippedFaeryElement` at `0x003b6dc4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6dc4.c`, unified lines 133226-133239.
Recovered declaration: `void __fastcall Character::_GetEquippedFaeryElement( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN:** No Lua arguments are inspected by this audited callback; extra arguments are ignored. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6dc4.c:1`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Gets selected faerie ID for current global difficulty (-1 selector), maps through character faerie list, and returns authored faerie row Element at +8. No-save pointer gives selected index 0, not a fabricated none-element fallback. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6dc4.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6dc4.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c:9`.
**Gates:**
- PROVEN: Faerie lookup asserts type bounds, list count==FaeryTypes.COUNT, and row Type==selected ID. Invalid data remains an assertion/unsafe index boundary, not a fallback element. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003aeac0.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003aeac0.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003aeac0.c:42`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6dc4.c:1`.
**Return:**
- PROVEN: One integer authored element ID; ignores Lua arguments.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Reusable callback requests actual saved selection and table element via required services. It fails absent service/data instead of native unsafe indexing; complete live provider wiring is not proven by this source audit.
- Source evidence: `port/level-world/character_faery_element_v3.cpp`:7.
- Source evidence: `port/level-world/character_faery_element_v3.cpp`:25.

**Concrete behavioral difference:** Reusable callback requests actual saved selection and table element via required services. It fails absent service/data instead of native unsafe indexing; complete live provider wiring is not proven by this source audit.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Selected faerie type2; its mapped row Element=3; GetEquippedFaeryElement() Returns 3. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6dc4.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003aeac0.c`:41.
2. **PROVEN:** Given: Selected faerie type0; mapped row Element=1 Returns 1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6dc4.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003aeac0.c`:41.
3. **PROVEN:** Given: Different difficulty slots select type0/type2 Current global difficulty determines selected slot; returns that row element. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bb98c.c`:9.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **6** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:88` | outside scoped roots | `GetEquippedFaeryElement()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:108` | outside scoped roots | `GetEquippedFaeryElement()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:127` | outside scoped roots | `GetEquippedFaeryElement()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:147` | outside scoped roots | `GetEquippedFaeryElement()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:166` | outside scoped roots | `GetEquippedFaeryElement()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:44` | player_skill_lua | `GetEquippedFaeryElement()` | `[]` |

## Related natives

`Character::_GetEquippedFaeryElement`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
