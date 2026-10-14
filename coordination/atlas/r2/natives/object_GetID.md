# object:GetID

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetID` | bindFunction | `GameObject::_GetID` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105127 |
| `GetID` | bindMethod | `GameObject::_GetID` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105132 |

Address-identified callback: `GameObject::_GetID` at `0x0038ebe4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c`, unified lines 106027-106035.
Recovered declaration: `void __fastcall GameObject::_GetID( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:7`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Pushes the bound receiver pointer as a pointer identity. No integer conversion, target substitution, or property lookup. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:7`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:7`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:7`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** Pointer identity conversion and binding are present in gameobject_lua_representation.cpp:19-22 and character_script_objects.cpp:75. Exact identity result is represented as DH2_SCRIPT_IDENTITY; this is source parity evidence, without runtime verification.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:247 — `else if((target||state_machine)&&b.original_callback==0x38ebe4){function=&get_id;context=this;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:155 — `auto& t=*static_cast<Impl*>(opaque);if(!returned||!out||!capacity)return fail(error,size,"Malformed GetID output");*returned=0;`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:156 — `return dh2::gameobject_lua::dh2_gameobject_lua_get_id(out,returned,t.identity)?fail(error,size,"Native GetID delivery failed"):0;`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** Pointer identity conversion and binding are present in gameobject_lua_representation.cpp:19-22 and character_script_objects.cpp:75. Exact identity result is represented as DH2_SCRIPT_IDENTITY; this is source parity evidence, without runtime verification.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: receiver pointer P; receiver:GetID() Returns pointer identity P. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c`:7.
2. **PROVEN:** Given: distinct receivers P and Q; compare P:GetID() and Q:GetID() Returns distinct pointer identities. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ebe4.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **7** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:315` | outside scoped roots | `object:GetID()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:171` | outside scoped roots | `object:GetID()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:188` | outside scoped roots | `object:GetID()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:42` | player_skill_lua | `object:GetID()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:79` | outside scoped roots | `object:GetID()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:118` | outside scoped roots | `object:GetID()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:153` | outside scoped roots | `object:GetID()` | `[]` |

## Related natives

`GameObject::_GetID`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
