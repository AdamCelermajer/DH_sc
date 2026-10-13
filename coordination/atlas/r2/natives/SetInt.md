# SetInt

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

Engine functions to implement next ranking: **#103**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SetInt` | bindFunction | `LuaScript::_SetInt` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91766 |

Address-identified callback: `LuaScript::_SetInt` at `0x0037de5c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`, unified lines 93999-94031.
Recovered declaration: `sfc::script::lua::Value *__fastcall LuaScript::_SetInt( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: any getString; unit/scale hashed key. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:20`.
- Lua argument 2: any getNumber truncated integer; unit/scale plain integer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:30`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires at least two arguments; converts first with getString and second with getNumber then truncates to int. SetInt 0x37d990 hashes key and inserts/replaces script-local int value; no Lua tag checks. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037d990.c:53`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037d990.c:53`.
**State, timers and events:**
- PROVEN: No Lua result; map insert/replace, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037d990.c:53`.
**Return:**
- PROVEN: No Lua result; map insert/replace, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037d990.c:53`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** generic_lua_script_owner_v13.cpp:57 and :89 bind script-local integer callbacks and conversion providers; reusable callback path proven, no runtime claim.
- Source evidence: `port/script-runtime/script_int_bindings.cpp`:100 — `int status=dh2_script_vm_bind_source_values(vm,"SetInt",dh2_script_int_set_callback,const_cast<dh2_script_int_bindings*>(s));if(status)return status;`.
- Source evidence: `port/level-world/character_script_owner.cpp`:133 — `std::strcmp(descriptor.name,set?"SetInt":"GetInt"))throw Failure{-1};`.
- Source evidence: `port/level-world/character_script_owner_v2.cpp`:124 — `std::strcmp(descriptor.name,set?"SetInt":"GetInt"))throw Failure{-1};`.
- Source evidence: `port/level-world/character_script_session.cpp`:315 — `if(!std::strcmp(b.name,"SetInt")&&b.original_callback==0x37de5c)return true; // Installed by genuine native-integer ScriptOwner before this delivery.`.
- Source evidence: `port/level-world/character_script_session_v2.cpp`:194 — `if(!std::strcmp(b.name,"SetInt")&&b.original_callback==0x37de5c)return true; // Installed by genuine native-integer ScriptOwnerV2 before this delivery.`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:234 — `if(!std::strcmp(b.name,"SetInt")&&b.original_callback==0x37de5c)return true; // Installed by genuine native-integer ScriptOwnerV2 before this delivery.`.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:16 — `{"Include",0x37efe4},{"Trace",0x37ee80},{"SetInt",0x37de5c},{"GetInt",0x37ec14},`.

**Port comparison:** generic_lua_script_owner_v13.cpp:57 and :89 bind script-local integer callbacks and conversion providers; reusable callback path proven, no runtime claim.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: SetInt("count",7.75) then GetInt("count") Stored/read integer7. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037d990.c`:53.
2. **PROVEN:** Given: map count=5; SetInt("count") Map count remains5, no Lua value. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037d990.c`:53.
3. **PROVEN:** Given: SetInt("count",2); SetInt("count",9) Same hash entry replaced with9. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037de5c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037d990.c`:53.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **18** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:386` | outside scoped roots | `SetInt("num_monsters", g_monster_alives)` | `[{"index":1,"expression":"\"num_monsters\"","literal_tokens":["\"num_monsters\""]},{"index":2,"expression":"g_monster_alives","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:538` | outside scoped roots | `SetInt("wave_time",\t\t\tg_wave_time)` | `[{"index":1,"expression":"\"wave_time\"","literal_tokens":["\"wave_time\""]},{"index":2,"expression":"g_wave_time","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:539` | outside scoped roots | `SetInt("wave_total_time",\tg_wave_time)` | `[{"index":1,"expression":"\"wave_total_time\"","literal_tokens":["\"wave_total_time\""]},{"index":2,"expression":"g_wave_time","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:540` | outside scoped roots | `SetInt("wave_number",\t\tg_current_wave)` | `[{"index":1,"expression":"\"wave_number\"","literal_tokens":["\"wave_number\""]},{"index":2,"expression":"g_current_wave","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:541` | outside scoped roots | `SetInt("num_waves",\t\t\t#waves)` | `[{"index":1,"expression":"\"num_waves\"","literal_tokens":["\"num_waves\""]},{"index":2,"expression":"#waves","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:547` | outside scoped roots | `SetInt("wave_number", 0)` | `[{"index":1,"expression":"\"wave_number\"","literal_tokens":["\"wave_number\""]},{"index":2,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:72` | outside scoped roots | `SetInt("num_players", g_players_total)` | `[{"index":1,"expression":"\"num_players\"","literal_tokens":["\"num_players\""]},{"index":2,"expression":"g_players_total","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:111` | outside scoped roots | `SetInt("num_players", g_players_total)` | `[{"index":1,"expression":"\"num_players\"","literal_tokens":["\"num_players\""]},{"index":2,"expression":"g_players_total","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:177` | outside scoped roots | `SetInt("num_monsters", g_monster_alives)` | `[{"index":1,"expression":"\"num_monsters\"","literal_tokens":["\"num_monsters\""]},{"index":2,"expression":"g_monster_alives","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:314` | outside scoped roots | `SetInt("wave_time", 0)` | `[{"index":1,"expression":"\"wave_time\"","literal_tokens":["\"wave_time\""]},{"index":2,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:316` | outside scoped roots | `SetInt("wave_time", g_wave_time)` | `[{"index":1,"expression":"\"wave_time\"","literal_tokens":["\"wave_time\""]},{"index":2,"expression":"g_wave_time","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:398` | outside scoped roots | `SetInt("wave_time",\t\t\tg_wave_time)` | `[{"index":1,"expression":"\"wave_time\"","literal_tokens":["\"wave_time\""]},{"index":2,"expression":"g_wave_time","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:399` | outside scoped roots | `SetInt("wave_total_time",\tg_wave_time)` | `[{"index":1,"expression":"\"wave_total_time\"","literal_tokens":["\"wave_total_time\""]},{"index":2,"expression":"g_wave_time","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:400` | outside scoped roots | `SetInt("wave_number",\t\tg_current_wave)` | `[{"index":1,"expression":"\"wave_number\"","literal_tokens":["\"wave_number\""]},{"index":2,"expression":"g_current_wave","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:401` | outside scoped roots | `SetInt("num_waves",\t\t\t#waves)` | `[{"index":1,"expression":"\"num_waves\"","literal_tokens":["\"num_waves\""]},{"index":2,"expression":"#waves","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:408` | outside scoped roots | `SetInt("wave_number", 0)` | `[{"index":1,"expression":"\"wave_number\"","literal_tokens":["\"wave_number\""]},{"index":2,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:474` | outside scoped roots | `SetInt("game_state", g_game_state)` | `[{"index":1,"expression":"\"game_state\"","literal_tokens":["\"game_state\""]},{"index":2,"expression":"g_game_state","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:33` | player_skill_lua | `SetInt("Faery_Cooldown", cooldown_time)` | `[{"index":1,"expression":"\"Faery_Cooldown\"","literal_tokens":["\"Faery_Cooldown\""]},{"index":2,"expression":"cooldown_time","literal_tokens":[]}]` |

## Related natives

`LuaScript::_SetInt`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
