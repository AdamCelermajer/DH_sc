# mothSlot4:LOCK

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#210**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `LOCK` | bindMethod | `GameObject::_Lock` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105119 |

Address-identified callback: `GameObject::_Lock` at `0x0038d5f8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c`, unified lines 105018-105022.
Recovered declaration: `void __fastcall GameObject::_Lock(int a1, int a2, int a3)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c:1`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Increments receiver byte at +41, an 8-bit nesting counter. Does not push any return, set a Boolean, or acquire a thread lock. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c:1`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c:1`.
**State, timers and events:**
- PROVEN: No Lua return; writes only receiver lock byte, starts no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c:1`.
**Return:**
- PROVEN: No Lua return; writes only receiver lock byte, starts no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c:1`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** Catalog registration alone does not implement byte-counter ownership; no 0x38d5f8 branch in scene Character dispatcher (character_script_objects.cpp:144-146). Original counter nesting must not be replaced by Boolean lock.

**Concrete behavioral difference:** Catalog registration alone does not implement byte-counter ownership; no 0x38d5f8 branch in scene Character dispatcher (character_script_objects.cpp:144-146). Original counter nesting must not be replaced by Boolean lock.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: receiver counter0; receiver:LOCK() Counter becomes1, no Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c`:1.
2. **PROVEN:** Given: receiver counter1; receiver:LOCK(); receiver:LOCK() Counter becomes3, retaining nesting. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d5f8.c`:1.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:130` | outside scoped roots | `mothSlot4:LOCK()` | `[]` |

## Related natives

`GameObject::_Lock`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
