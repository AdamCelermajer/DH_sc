# BitAnd

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `BitAnd` | bindFunction | `LuaScript::_BitAnd` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91826 |

Address-identified callback: `LuaScript::_BitAnd` at `0x0037e9ec`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`, unified lines 94462-94550.
Recovered declaration: `void __fastcall LuaScript::_BitAnd( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1..N: number tag3 truncated int32; unit/scale bitmask. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:38`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires at least two arguments and every argument tag3 numeric. Truncates each number to signed integer, folds bitwise AND, and pushes integer. Any non-number or fewer than two suppresses result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:51`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:61`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:62`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:51`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:61`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:62`.
**State, timers and events:**
- PROVEN: One integer iff at least2 all numeric; otherwise no Lua values. No state/timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:51`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:61`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:62`.
**Return:**
- PROVEN: One integer iff at least2 all numeric; otherwise no Lua values. No state/timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:51`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:61`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c:62`.

## Lua wrapper/helper traces

Proven source chain: `IsBoss` -> `BitAnd`.
Proven source chain: `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:91` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:102` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:87` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:88` `IsMiniBoss(target)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:98` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:99` `IsMiniBoss(target)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:100` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:100` `IsMiniBoss(target)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:50` `IsMiniBoss(defender)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:50` `IsBoss(defender)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:95` `IsMiniBoss(target)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:95` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:29` `IsMiniBoss(target)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:29` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:93` `IsMiniBoss(target)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:93` `IsBoss(target)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:82` `IsBoss(victim_data.ptr)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:83` `IsMiniBoss(victim_data.ptr)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:95` `IsBoss(victim_data.ptr)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:96` `IsMiniBoss(victim_data.ptr)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:84` `IsMiniBoss(defender)`; chain `IsMiniBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:84` `IsBoss(defender)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:107` `IsBoss(defender)`; chain `IsBoss` -> `BitAnd`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:110` `IsMiniBoss(defender)`; chain `IsMiniBoss` -> `BitAnd`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** generic_lua_script_owner_v13.cpp:65 and :95 install dh2_script_scalar_bit_and; source scalar callback path proven for supported numeric domain, no integrated runtime claim.
- Source evidence: `port/script-runtime/script_scalar_bindings.c`:109 — `CALLBACK(bit_and,BIT_AND)`.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:19 — `{"Rand",0x37df30},{"RandF",0x37e2d8},{"BitNot",0x37f814},{"BitAnd",0x37e9ec},{"BitOr",0x37e814},{"BitXOr",0x37f750},`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:271 — `const std::uint32_t addresses[]={0x37ebc4,0x37ee84,0x37e1a4,0x37e068,0x37f814,0x37e9ec,0x37e814,0x37f750};`.

**Port comparison:** generic_lua_script_owner_v13.cpp:65 and :95 install dh2_script_scalar_bit_and; source scalar callback path proven for supported numeric domain, no integrated runtime claim.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: BitAnd(7,3,1) Returns1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:51, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:54, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:61, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:62.
2. **PROVEN:** Given: BitAnd(7) No Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:51, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:54, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:61, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:62.
3. **PROVEN:** Given: BitAnd(7,"3") No Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:51, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:54, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:61, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e9ec.c`:62.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **2** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:260` | autoloaded_lua_common_helpers | `BitAnd(flag, GetPyCst("AIFlags", "Boss"))` | `[{"index":1,"expression":"flag","literal_tokens":[]},{"index":2,"expression":"GetPyCst(\"AIFlags\", \"Boss\")","literal_tokens":["\"AIFlags\"","\"Boss\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:268` | autoloaded_lua_common_helpers | `BitAnd(flag, GetPyCst("AIFlags", "MiniBoss"))` | `[{"index":1,"expression":"flag","literal_tokens":[]},{"index":2,"expression":"GetPyCst(\"AIFlags\", \"MiniBoss\")","literal_tokens":["\"AIFlags\"","\"MiniBoss\""]}]` |

## Related natives

`BitAnd`, `LuaScript::_BitAnd`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
