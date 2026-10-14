# Include

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

Engine functions to implement next ranking: **#14**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `Include` | bindFunction | `LuaScript::_Include` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91756 |

Address-identified callback: `LuaScript::_Include` at `0x0037efe4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`, unified lines 94763-94785.
Recovered declaration: `sfc::script::lua::Value *__fastcall LuaScript::_Include( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: string tag4; empty no-load; unit/scale relative script filename. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:17`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Only arg1 string tag4 admitted; calls LuaScript::Load -> LuaManager::AddFile. Nonempty name concatenates script path; .lua receives c unless .luac prefix already, no extension receives .luac. Per-script loaded-file set skips duplicate load; global file cache reused/reset. Missing file or Lua load error returns nativefalse but wrapper does not push that return; loaded set insertion only after successful load. Empty/wrong args no-op. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b574.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b23c.c:45`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b574.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b23c.c:45`.
**State, timers and events:**
- PROVEN: No Lua value; conditional same-VM load/cache/loaded-file-set mutation, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b574.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b23c.c:45`.
**Return:**
- PROVEN: No Lua value; conditional same-VM load/cache/loaded-file-set mutation, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b574.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b23c.c:45`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** generic_lua_script_owner_v13.cpp:79-81 source include and shared LuaScriptCacheOwnerV13 (:7-11) retain path/cache/loaded-set semantics; source callback supported. Independent supplied file readers/assets still determine actual availability; no runtime claim.
- Source evidence: `port/script-runtime/script_runtime.c`:335 — `"%s","unsupported source Include error object");}`.
- Source evidence: `port/script-runtime/script_runtime.c`:367 — `IncludeFrame* frame=include_frame(scope);return frame?frame->error:"invalid Include scope";`.
- Source evidence: `port/script-runtime/script_runtime.c`:376 — `return luaL_error(L,"unsupported source Include execution context");`.
- Source evidence: `port/script-runtime/script_runtime.c`:377 — `if(!lua_checkstack(L,1))return luaL_error(L,"source Include projection stack exhausted");`.
- Source evidence: `port/script-runtime/script_runtime.c`:387 — `if(binding->vm->scope_generation==UINT64_MAX)return luaL_error(L,"source Include generation exhausted");`.
- Source evidence: `port/script-runtime/script_runtime.c`:396 — `if(result){error[sizeof(error)-1]=0;return luaL_error(L,"%s",error[0]?error:"Include provider rejected");}`.
- Source evidence: `port/script-runtime/script_runtime.c`:403 — `lua_pushcclosure(L,include_trampoline,1);lua_setglobal(L,"Include");return 0;`.
- Source evidence: `port/script-runtime/script_runtime.h`:150 — `static_assert(sizeof(dh2_script_include_scope)==16,"Include capability ABI");`.
- Source evidence: `port/level-world/character_script_owner.cpp`:334 — `catch(const std::bad_alloc&){t.message="Native Include ownership allocation failed";return -2;}`.
- Source evidence: `port/level-world/character_script_owner_v2.cpp`:336 — `catch(const std::bad_alloc&){t.message="Native Include ownership allocation failed";return -2;}`.
- Source evidence: `port/level-world/character_script_session.cpp`:125 — `auto& b=*static_cast<PrivateBinding*>(opaque);auto* owner=b.self->owner.get();if(!owner)return fail(error,size,"Closing session Include unsupported");`.
- Source evidence: `port/level-world/character_script_session.cpp`:126 — `auto result=owner->include(b.identity,name,b.self->services,scope);return result?fail(error,size,"Native session Include delivery failed"):0;`.

**Port comparison:** generic_lua_script_owner_v13.cpp:79-81 source include and shared LuaScriptCacheOwnerV13 (:7-11) retain path/cache/loaded-set semantics; source callback supported. Independent supplied file readers/assets still determine actual availability; no runtime claim.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: script path data/scripts/ai/; Include("_commons") with available file Loads data/scripts/ai/_commons.luac into same VM once. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b574.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b23c.c`:45.
2. **PROVEN:** Given: same successful Include("_commons") twice Second call does not execute file again. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b574.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b23c.c`:45.
3. **PROVEN:** Given: Include(5) No load or Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037efe4.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b574.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b23c.c`:45.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **49** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassin1.luac:1` | outside scoped roots | `Include("Assassin1_settings")` | `[{"index":1,"expression":"\"Assassin1_settings\"","literal_tokens":["\"Assassin1_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassin1.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassin2.luac:1` | outside scoped roots | `Include("Assassin2_settings")` | `[{"index":1,"expression":"\"Assassin2_settings\"","literal_tokens":["\"Assassin2_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassin2.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassin3.luac:1` | outside scoped roots | `Include("Assassin3_settings")` | `[{"index":1,"expression":"\"Assassin3_settings\"","literal_tokens":["\"Assassin3_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassin3.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon.luac:1` | outside scoped roots | `Include("AssassinDemon_settings")` | `[{"index":1,"expression":"\"AssassinDemon_settings\"","literal_tokens":["\"AssassinDemon_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/balrog.luac:1` | outside scoped roots | `Include("Balrog_settings")` | `[{"index":1,"expression":"\"Balrog_settings\"","literal_tokens":["\"Balrog_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/balrog.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward.luac:1` | outside scoped roots | `Include("monster")` | `[{"index":1,"expression":"\"monster\"","literal_tokens":["\"monster\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward.luac:2` | outside scoped roots | `Include("coward_core")` | `[{"index":1,"expression":"\"coward_core\"","literal_tokens":["\"coward_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1.luac:1` | outside scoped roots | `Include("DarkQueen1_settings")` | `[{"index":1,"expression":"\"DarkQueen1_settings\"","literal_tokens":["\"DarkQueen1_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1.luac:2` | outside scoped roots | `Include("DarkQueen1_core")` | `[{"index":1,"expression":"\"DarkQueen1_core\"","literal_tokens":["\"DarkQueen1_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2.luac:1` | outside scoped roots | `Include("DarkQueen2_settings")` | `[{"index":1,"expression":"\"DarkQueen2_settings\"","literal_tokens":["\"DarkQueen2_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/dw_statue.luac:1` | outside scoped roots | `Include("DW_Statue_settings")` | `[{"index":1,"expression":"\"DW_Statue_settings\"","literal_tokens":["\"DW_Statue_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/dw_statue.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss.luac:1` | outside scoped roots | `Include("EdwardBoss_settings")` | `[{"index":1,"expression":"\"EdwardBoss_settings\"","literal_tokens":["\"EdwardBoss_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss.luac:2` | outside scoped roots | `Include("EdwardBoss_core")` | `[{"index":1,"expression":"\"EdwardBoss_core\"","literal_tokens":["\"EdwardBoss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/emissary.luac:1` | outside scoped roots | `Include("Emissary_settings")` | `[{"index":1,"expression":"\"Emissary_settings\"","literal_tokens":["\"Emissary_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/emissary.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite.luac:1` | outside scoped roots | `Include("Flaminite_settings")` | `[{"index":1,"expression":"\"Flaminite_settings\"","literal_tokens":["\"Flaminite_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite.luac:2` | outside scoped roots | `Include("flaminite_core")` | `[{"index":1,"expression":"\"flaminite_core\"","literal_tokens":["\"flaminite_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed.luac:1` | outside scoped roots | `Include("FlaminiteED_settings")` | `[{"index":1,"expression":"\"FlaminiteED_settings\"","literal_tokens":["\"FlaminiteED_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant.luac:2` | outside scoped roots | `Include("largeplant_core")` | `[{"index":1,"expression":"\"largeplant_core\"","literal_tokens":["\"largeplant_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:1` | outside scoped roots | `Include("machineroom/waves_easy")` | `[{"index":1,"expression":"\"machineroom/waves_easy\"","literal_tokens":["\"machineroom/waves_easy\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort.luac:1` | outside scoped roots | `Include("machineroom/vip_core")` | `[{"index":1,"expression":"\"machineroom/vip_core\"","literal_tokens":["\"machineroom/vip_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort.luac:2` | outside scoped roots | `Include("machineroom/referee")` | `[{"index":1,"expression":"\"machineroom/referee\"","literal_tokens":["\"machineroom/referee\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk.luac:1` | outside scoped roots | `Include("monster")` | `[{"index":1,"expression":"\"monster\"","literal_tokens":["\"monster\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk.luac:2` | outside scoped roots | `Include("madruk_core")` | `[{"index":1,"expression":"\"madruk_core\"","literal_tokens":["\"madruk_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso.luac:1` | outside scoped roots | `Include("monster")` | `[{"index":1,"expression":"\"monster\"","literal_tokens":["\"monster\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso.luac:2` | outside scoped roots | `Include("madruk_torso_core")` | `[{"index":1,"expression":"\"madruk_torso_core\"","literal_tokens":["\"madruk_torso_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac:1` | outside scoped roots | `Include("monster")` | `[{"index":1,"expression":"\"monster\"","literal_tokens":["\"monster\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac:2` | outside scoped roots | `Include("miniboss_state")` | `[{"index":1,"expression":"\"miniboss_state\"","literal_tokens":["\"miniboss_state\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon.luac:1` | outside scoped roots | `Include("monster")` | `[{"index":1,"expression":"\"monster\"","literal_tokens":["\"monster\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon.luac:2` | outside scoped roots | `Include("mothcocoon_core")` | `[{"index":1,"expression":"\"mothcocoon_core\"","literal_tokens":["\"mothcocoon_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:1` | outside scoped roots | `Include("monster")` | `[{"index":1,"expression":"\"monster\"","literal_tokens":["\"monster\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mybossname.luac:1` | outside scoped roots | `Include("myBossName_settings")` | `[{"index":1,"expression":"\"myBossName_settings\"","literal_tokens":["\"myBossName_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mybossname.luac:2` | outside scoped roots | `Include("generic_boss_core")` | `[{"index":1,"expression":"\"generic_boss_core\"","literal_tokens":["\"generic_boss_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll.luac:1` | outside scoped roots | `Include("RootTroll_settings")` | `[{"index":1,"expression":"\"RootTroll_settings\"","literal_tokens":["\"RootTroll_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll.luac:2` | outside scoped roots | `Include("roottroll_core")` | `[{"index":1,"expression":"\"roottroll_core\"","literal_tokens":["\"roottroll_core\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:9` | outside scoped roots | `Include("sandworm_core.lua")` | `[{"index":1,"expression":"\"sandworm_core.lua\"","literal_tokens":["\"sandworm_core.lua\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:9` | outside scoped roots | `Include("sandworm_core.lua")` | `[{"index":1,"expression":"\"sandworm_core.lua\"","literal_tokens":["\"sandworm_core.lua\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:9` | outside scoped roots | `Include("sandworm_core.lua")` | `[{"index":1,"expression":"\"sandworm_core.lua\"","literal_tokens":["\"sandworm_core.lua\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:1` | outside scoped roots | `Include("soundtest/waves_sdd")` | `[{"index":1,"expression":"\"soundtest/waves_sdd\"","literal_tokens":["\"soundtest/waves_sdd\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon.luac:1` | outside scoped roots | `Include("windDragon_settings")` | `[{"index":1,"expression":"\"windDragon_settings\"","literal_tokens":["\"windDragon_settings\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon.luac:2` | outside scoped roots | `Include("windDragon_core")` | `[{"index":1,"expression":"\"windDragon_core\"","literal_tokens":["\"windDragon_core\""]}]` |

## Related natives

`LuaScript::_Include`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
