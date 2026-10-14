# RegisterAnim

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `RegisterAnim` | bindFunction | `Character::_RegisterAnim` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132672 |
| `RegisterAnim` | bindMethod | `Character::_RegisterAnim` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132677 |

Address-identified callback: `Character::_RegisterAnim` at `0x003b70d8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`, unified lines 133452-133476.
Recovered declaration: `int __fastcall Character::_RegisterAnim( int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: number tag3 truncated int; unit/scale animation dictionary row index. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:14`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires numeric tag3 animation dictionary row index, truncates int and calls animator ANIM_AddAnimDictToSet. Callee refuses when frozen/disabled byte+56 is set; otherwise lazily initializes unique anim-set ID if-1, then adds only nonnegative row index to AnimSetManager. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c9b14.c:6`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c9b14.c:6`.
**State, timers and events:**
- PROVEN: No Lua return; may lazily create set/add dictionary, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c9b14.c:6`.
**Return:**
- PROVEN: No Lua return; may lazily create set/add dictionary, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c9b14.c:6`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_script_session.cpp:212-226 implements numeric wrapper and supplied same-Character animator register_dictionary; binding at:308-312 covers RegisterAnim/RegisterActorAnim. Source callback path is present for safe signed numeric IDs; resource availability and actual runtime playback are separate.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** character_script_session.cpp:212-226 implements numeric wrapper and supplied same-Character animator register_dictionary; binding at:308-312 covers RegisterAnim/RegisterActorAnim. Source callback path is present for safe signed numeric IDs; resource availability and actual runtime playback are separate.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: animator disabled byte1; RegisterAnim(5) No animation dictionary added. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c9b14.c`:6.
2. **PROVEN:** Given: animator enabled, set ID initialized; RegisterAnim(-1) No dictionary added. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c9b14.c`:6.
3. **PROVEN:** Given: enabled, set ID-1; RegisterAnim(5) Resolves unique anim-set ID, then adds dictionary5. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b70d8.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c9b14.c`:6.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **29** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:289` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "root_troll_intimidate"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"root_troll_intimidate\")","literal_tokens":["\"AnimDict\"","\"root_troll_intimidate\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:167` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "root_troll_intimidate_02"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"root_troll_intimidate_02\")","literal_tokens":["\"AnimDict\"","\"root_troll_intimidate_02\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:96` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:97` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "sandworm_rewind_hole"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"sandworm_rewind_hole\")","literal_tokens":["\"AnimDict\"","\"sandworm_rewind_hole\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:98` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "sandworm_rewind_hole_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"sandworm_rewind_hole_idle\")","literal_tokens":["\"AnimDict\"","\"sandworm_rewind_hole_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:99` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_emerge\")","literal_tokens":["\"AnimDict\"","\"swampking_emerge\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:139` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:140` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_intimidate"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_intimidate\")","literal_tokens":["\"AnimDict\"","\"swampking_intimidate\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:141` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_dive"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_dive\")","literal_tokens":["\"AnimDict\"","\"swampking_dive\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:142` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_emerge\")","literal_tokens":["\"AnimDict\"","\"swampking_emerge\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:143` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall\")","literal_tokens":["\"AnimDict\"","\"swampking_fall\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:144` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_fall_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:145` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall_idle_to_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_fall_idle_to_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:49` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:50` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_intimidate"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_intimidate\")","literal_tokens":["\"AnimDict\"","\"swampking_intimidate\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:51` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_dive"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_dive\")","literal_tokens":["\"AnimDict\"","\"swampking_dive\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:52` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_emerge\")","literal_tokens":["\"AnimDict\"","\"swampking_emerge\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:53` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall\")","literal_tokens":["\"AnimDict\"","\"swampking_fall\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:54` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_fall_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:55` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall_idle_to_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_fall_idle_to_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:231` | act1_level_boss_script_closure | `RegisterAnim(GetPyOID("AnimDict", "swampking_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:232` | act1_level_boss_script_closure | `RegisterAnim(GetPyOID("AnimDict", "swampking_intimidate"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_intimidate\")","literal_tokens":["\"AnimDict\"","\"swampking_intimidate\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:233` | act1_level_boss_script_closure | `RegisterAnim(GetPyOID("AnimDict", "swampking_dive"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_dive\")","literal_tokens":["\"AnimDict\"","\"swampking_dive\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:234` | act1_level_boss_script_closure | `RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_emerge\")","literal_tokens":["\"AnimDict\"","\"swampking_emerge\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:235` | act1_level_boss_script_closure | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall\")","literal_tokens":["\"AnimDict\"","\"swampking_fall\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:236` | act1_level_boss_script_closure | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_fall_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:237` | act1_level_boss_script_closure | `RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"swampking_fall_idle_to_idle\")","literal_tokens":["\"AnimDict\"","\"swampking_fall_idle_to_idle\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:133` | outside scoped roots | `RegisterAnim(GetPyOID("AnimDict", "spell_dh2_prince_drained_life_complete"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"spell_dh2_prince_drained_life_complete\")","literal_tokens":["\"AnimDict\"","\"spell_dh2_prince_drained_life_complete\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:34` | player_skill_lua | `RegisterAnim(GetPyOID("AnimDict", "skill_dh2_prince_rogue_archer_pinning_shot"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"skill_dh2_prince_rogue_archer_pinning_shot\")","literal_tokens":["\"AnimDict\"","\"skill_dh2_prince_rogue_archer_pinning_shot\""]}]` |

## Related natives

`Character::_RegisterAnim`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
