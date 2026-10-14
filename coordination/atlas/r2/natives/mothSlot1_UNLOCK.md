# mothSlot1:UNLOCK

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#148**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `UNLOCK` | bindMethod | `GameObject::_Unlock` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105123 |

Address-identified callback: `GameObject::_Unlock` at `0x0038d7a8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c`, unified lines 105098-105103.
Recovered declaration: `void __fastcall GameObject::_Unlock(int a1, int a2, int a3)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:3`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Decrements receiver byte +41 only when it is nonzero. Underflow is prevented; repeated unlock at zero is a no-op. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:3`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:3`.
**State, timers and events:**
- PROVEN: No Lua return; writes only receiver lock byte when nonzero, starts no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:3`.
**Return:**
- PROVEN: No Lua return; writes only receiver lock byte when nonzero, starts no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c:3`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** No 0x38d7a8 branch in scene Character dispatcher (character_script_objects.cpp:144-146); no general receiver lock-byte callback path proven despite catalog. Original counted decrement cannot be a Boolean clear.
- Source evidence: `port/windows-foundation/features/frontend/art/original_art_data.cpp`:27 — `static const std::pair<const char*,const char*> labels[]{{"MENU_UNIQUE_CHARS","\n” !\"#'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ^abcdefghijklmnopqrstuvwxyz ©®"},{"MENU_MAIN_MENU","Main Menu"},{"MENU_NEW_GAME","New Game"},{"MENU_CO`.
- Source evidence: `port/windows-foundation/features/frontend/art/original_art_data.cpp`:27 — `static const std::pair<const char*,const char*> labels[]{{"MENU_UNIQUE_CHARS","\n” !\"#'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ^abcdefghijklmnopqrstuvwxyz ©®"},{"MENU_MAIN_MENU","Main Menu"},{"MENU_NEW_GAME","New Game"},{"MENU_CO`.

**Concrete behavioral difference:** No 0x38d7a8 branch in scene Character dispatcher (character_script_objects.cpp:144-146); no general receiver lock-byte callback path proven despite catalog. Original counted decrement cannot be a Boolean clear.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: receiver counter0; receiver:UNLOCK() Counter remains0; no Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c`:3.
2. **PROVEN:** Given: receiver counter2; receiver:UNLOCK() Counter becomes1; receiver remains nested-locked. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7a8.c`:3.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **2** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:85` | outside scoped roots | `mothSlot1:UNLOCK()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:145` | outside scoped roots | `mothSlot1:UNLOCK()` | `[]` |

## Related natives

`GameObject::_Unlock`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
