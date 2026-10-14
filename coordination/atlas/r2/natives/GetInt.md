# GetInt

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

Engine functions to implement next ranking: **#101**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetInt` | bindFunction | `LuaScript::_GetInt` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91771 |

Address-identified callback: `LuaScript::_GetInt` at `0x0037ec14`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`, unified lines 94570-94588.
Recovered declaration: `void __fastcall LuaScript::_GetInt( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: any converted getString; unit/scale hashed script-local key. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:15`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires argument1; converts it using getString, hashes key, reads script-local int map. Missing key is inserted with zero by GetInt 0x37da30. No tag4 guard in wrapper. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037da30.c:39`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037da30.c:39`.
**State, timers and events:**
- PROVEN: One integer if key argument exists; absent key read inserts0. No timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037da30.c:39`.
**Return:**
- PROVEN: One integer if key argument exists; absent key read inserts0. No timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037da30.c:39`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** generic_lua_script_owner_v13.cpp:57-58 uses script-local dh2_script_int_map and binds GetInt. Source callback path proven; hash/conversion helper required for exact nonstring behavior.
- Source evidence: `port/script-runtime/script_int_bindings.cpp`:101 — `return dh2_script_vm_bind_source_values(vm,"GetInt",dh2_script_int_get_callback,const_cast<dh2_script_int_bindings*>(s));`.
- Source evidence: `port/level-world/canonical_character_candidate_v60.cpp`:585 — `if(q.argument1||q.argument0>=r.properties->resolved.size()){e="Required source default-sheet GetInt branch or valid property ID";return false;}`.
- Source evidence: `port/level-world/character_script_owner.cpp`:133 — `std::strcmp(descriptor.name,set?"SetInt":"GetInt"))throw Failure{-1};`.
- Source evidence: `port/level-world/character_script_owner_v2.cpp`:124 — `std::strcmp(descriptor.name,set?"SetInt":"GetInt"))throw Failure{-1};`.
- Source evidence: `port/level-world/character_script_session.cpp`:316 — `if(!std::strcmp(b.name,"GetInt")&&b.original_callback==0x37ec14)return true;`.
- Source evidence: `port/level-world/character_script_session_v2.cpp`:195 — `if(!std::strcmp(b.name,"GetInt")&&b.original_callback==0x37ec14)return true;`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:235 — `if(!std::strcmp(b.name,"GetInt")&&b.original_callback==0x37ec14)return true;`.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:16 — `{"Include",0x37efe4},{"Trace",0x37ee80},{"SetInt",0x37de5c},{"GetInt",0x37ec14},`.
- Source evidence: `port/level-world/openable_container_interaction_v2.cpp`:32 — `std::int32_t count=0;if(!deliver(s.props_get_int,"same CharProperties::GetInt",e,character,218,false,count))return false;`.

**Port comparison:** generic_lua_script_owner_v13.cpp:57-58 uses script-local dh2_script_int_map and binds GetInt. Source callback path proven; hash/conversion helper required for exact nonstring behavior.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: fresh script map; GetInt("count") Returns0 and creates zero-valued hash entry. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037da30.c`:39.
2. **PROVEN:** Given: script map key count=7; GetInt("count") Returns7. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037da30.c`:39.
3. **PROVEN:** Given: GetInt() No Lua return and map unchanged. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ec14.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037da30.c`:39.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **5** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:201` | outside scoped roots | `GetInt("Faery_Cooldown")` | `[{"index":1,"expression":"\"Faery_Cooldown\"","literal_tokens":["\"Faery_Cooldown\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:50` | outside scoped roots | `GetInt("Faery_Cooldown")` | `[{"index":1,"expression":"\"Faery_Cooldown\"","literal_tokens":["\"Faery_Cooldown\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:48` | outside scoped roots | `GetInt("Faery_Cooldown")` | `[{"index":1,"expression":"\"Faery_Cooldown\"","literal_tokens":["\"Faery_Cooldown\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:50` | outside scoped roots | `GetInt("Faery_Cooldown")` | `[{"index":1,"expression":"\"Faery_Cooldown\"","literal_tokens":["\"Faery_Cooldown\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:52` | outside scoped roots | `GetInt("Faery_Cooldown")` | `[{"index":1,"expression":"\"Faery_Cooldown\"","literal_tokens":["\"Faery_Cooldown\""]}]` |

## Related natives

`LuaScript::_GetInt`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
