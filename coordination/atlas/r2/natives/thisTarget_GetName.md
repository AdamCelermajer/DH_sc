# thisTarget:GetName

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#234**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetName` | bindFunction | `GameObject::_GetName` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105136 |
| `GetName` | bindMethod | `GameObject::_GetName` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105141 |

Address-identified callback: `GameObject::_GetName` at `0x0038ebf0`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c`, unified lines 106037-106045.
Recovered declaration: `void __fastcall GameObject::_GetName( GameObject *this, const sfc::script::lua::Arguments *a2, const char **a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:7`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Pushes the bound receiver name pointer at +68 as a Lua string. Does not look up an object by name or consult current target. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:7`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:7`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:7`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Container-script callback returns its container-context name and rejects extra arguments (container_open_script_v1.cpp:153-166); original ignores arguments and reads any bound GameObject name. Scene Character dispatch does not implement 0x38ebf0 (character_script_objects.cpp:144-146).
- Source evidence: `port/windows-foundation/features/containers/container_open_script_v1.cpp`:158 — `set_error(context, error, error_capacity, "GetName expects no arguments");`.
- Source evidence: `port/windows-foundation/features/containers/container_open_script_v1.cpp`:211 — `{"Trace", lua_trace}, {"GetName", lua_get_name},`.

**Concrete behavioral difference:** Container-script callback returns its container-context name and rejects extra arguments (container_open_script_v1.cpp:153-166); original ignores arguments and reads any bound GameObject name. Scene Character dispatch does not implement 0x38ebf0 (character_script_objects.cpp:144-146).

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: receiver name="MothSlot1"; receiver:GetName() Returns exactly "MothSlot1". Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c`:7.
2. **PROVEN:** Given: receiver name=""; receiver:GetName() Returns the empty string. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebf0.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:86` | player_skill_lua | `thisTarget:GetName()` | `[]` |

## Related natives

`GameObject::_GetName`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
