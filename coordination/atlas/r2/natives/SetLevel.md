# SetLevel

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SetLevel` | bindFunction | `Character::_SetLevel` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132825 |
| `SetLevel` | bindMethod | `Character::_SetLevel` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132830 |

Address-identified callback: `Character::_SetLevel` at `0x003b73a4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`, unified lines 133581-133616.
Recovered declaration: `int __fastcall Character::_SetLevel( int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: number tag3 truncated int; unit/scale x256 level; only upper cap. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:20`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires numeric tag3 raw x256 level. Truncates int, caps at MaxLevelDVeryHard<<8 without lower clamp, stores base property level at receiver+1464. RecalcProperties(force1), RegenHP(-1) then RegenMP(-1) fill to current maxima after recalculation. Does not just update display/save level. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdca4.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdbb8.c:10`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdca4.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdbb8.c:10`.
**State, timers and events:**
- PROVEN: No Lua value; base level/recalculated props/HP/MP full restoration, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdca4.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdbb8.c:10`.
**Return:**
- PROVEN: No Lua value; base level/recalculated props/HP/MP full restoration, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdca4.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdbb8.c:10`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_script_session_v3.cpp:266 installs source SetLevel property/vitals callback for retained receiver. Selected source callback parity supported; class stats/assets and runtime invocation remain separate.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** character_script_session_v3.cpp:266 installs source SetLevel property/vitals callback for retained receiver. Selected source callback parity supported; class stats/assets and runtime invocation remain separate.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: SetLevel("5") No level/recalc/vitals change. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdca4.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdbb8.c`:10.
2. **PROVEN:** Given: MaxLevelDVeryHard=100; SetLevel(30000) Level base25600, then recalc and HP/MP fill. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdca4.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdbb8.c`:10.
3. **PROVEN:** Given: SetLevel(1280) below cap Stores raw1280 (level5), recalculates and fills HP/MP. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b73a4.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdca4.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdbb8.c`:10.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **16** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:356` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:204` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:327` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:360` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:53` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:273` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:47` | act1_level_boss_script_closure | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:54` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:60` | act1_level_boss_script_closure | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:208` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:320` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:347` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:253` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:539` | act1_level_boss_script_closure | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:289` | outside scoped roots | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:126` | act1_level_boss_script_closure | `SetLevel(ToFixed(m_lvl))` | `[{"index":1,"expression":"ToFixed(m_lvl)","literal_tokens":[]}]` |

## Related natives

`Character::_SetLevel`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
