# target:IsDoor

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#121**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `IsDoor` | bindFunction | `GameObject::_IsDoor` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105182 |
| `IsDoor` | bindMethod | `GameObject::_IsDoor` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105187 |

Address-identified callback: `GameObject::_IsDoor` at `0x0038e9d8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c`, unified lines 105880-105888.
Recovered declaration: `void __fastcall GameObject::_IsDoor( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:7`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Pushes receiver uint32 object kind at +244 == 2. Does not dispatch a virtual or inspect model/name. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:7`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:7`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:7`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Catalog records IsDoor but scene Character dispatcher has no callback branch and returns unreconstructed-method failure at character_script_objects.cpp:144-146; original always returns the kind comparison.

**Concrete behavioral difference:** Catalog records IsDoor but scene Character dispatcher has no callback branch and returns unreconstructed-method failure at character_script_objects.cpp:144-146; original always returns the kind comparison.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: receiver kind2; receiver:IsDoor() True. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c`:7.
2. **PROVEN:** Given: receiver kind0; receiver:IsDoor() False. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9d8.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **3** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:121` | outside scoped roots | `target:IsDoor()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:139` | outside scoped roots | `target:IsDoor()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:178` | player_skill_lua | `target:IsDoor()` | `[]` |

## Related natives

`GameObject::_IsDoor`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
