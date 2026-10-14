# SetTarget

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SetTarget` | bindFunction | `Character::_SetTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132451 |
| `SetTarget` | bindMethod | `Character::_SetTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132456 |

Address-identified callback: `Character::_SetTarget` at `0x003b8f38`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`, unified lines 134779-134805.
Recovered declaration: `sfc::script::lua::Value *__fastcall Character::_SetTarget( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: pointer tag2/userdata tag7; unit/scale target identity. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:21`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires arg1 pointer tag2/userdata tag7; invokes AI_SetTarget(mode0). Writes candidate pointer, clears receiver uint16 hit count if target differs, publishes target. Null returns without alive/sight recompute. Nonnull reloads owner AI ID, resets changed byte if last-target differs, synchronizes last-target, caches dead XOR1 and strict sight query. Debug tracing queries occur before publication. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.
**State, timers and events:**
- PROVEN: No Lua value; candidate/target/hit-count/cache changes as admitted, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.
**Return:**
- PROVEN: No Lua value; candidate/target/hit-count/cache changes as admitted, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_target_bindings.cpp:29-58 and :77-85 reproduce candidate/publication/cache ordering and type gate with source services; source callback path proven, no runtime verification.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:242 — `if(target&&(b.original_callback==0x3b8f38||b.original_callback==0x3b5690)){`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:243 — `if(dh2_script_vm_bind_source_scoped(c.vm,b.name,b.original_callback==0x3b8f38?&set_target:&clear_target,this))throw std::runtime_error("Scoped target registration failed");`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** character_target_bindings.cpp:29-58 and :77-85 reproduce candidate/publication/cache ordering and type gate with source services; source callback path proven, no runtime verification.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: current target T, hit count5; SetTarget(null pointer) Candidate/target null, hit count0; cached alive/sight retained. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c`:25.
2. **PROVEN:** Given: current target T, hit count5; SetTarget(T) Hit count remains5; alive/sight recomputed. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c`:25.
3. **PROVEN:** Given: current target T; SetTarget("T") No pointer/flag change, no Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f38.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c`:25.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **38** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:28` | outside scoped roots | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:180` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:246` | outside scoped roots | `SetTarget(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:270` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:294` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:315` | outside scoped roots | `SetTarget(closestTarget)` | `[{"index":1,"expression":"closestTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:154` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:30` | outside scoped roots | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:136` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:202` | outside scoped roots | `SetTarget(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:226` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:250` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:271` | outside scoped roots | `SetTarget(closestTarget)` | `[{"index":1,"expression":"closestTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:240` | outside scoped roots | `SetTarget(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:264` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:288` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:309` | outside scoped roots | `SetTarget(closestTarget)` | `[{"index":1,"expression":"closestTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:77` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:190` | outside scoped roots | `SetTarget(attacker)` | `[{"index":1,"expression":"attacker","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:301` | outside scoped roots | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:222` | outside scoped roots | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:67` | act1_level_boss_script_closure | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:230` | act1_level_boss_script_closure | `SetTarget(attacker)` | `[{"index":1,"expression":"attacker","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:70` | act1_level_boss_script_closure | `SetTarget(t)` | `[{"index":1,"expression":"t","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:154` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:60` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:345` | outside scoped roots | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:92` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:358` | outside scoped roots | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:17` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:285` | outside scoped roots | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:195` | act1_level_boss_script_closure | `SetTarget(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:575` | act1_level_boss_script_closure | `SetTarget(enemy)` | `[{"index":1,"expression":"enemy","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:240` | outside scoped roots | `SetTarget(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:39` | act1_level_boss_script_closure | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:210` | act1_level_boss_script_closure | `SetTarget(attacker)` | `[{"index":1,"expression":"attacker","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:82` | outside scoped roots | `SetTarget(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:40` | player_skill_lua | `SetTarget(defender)` | `[{"index":1,"expression":"defender","literal_tokens":[]}]` |

## Related natives

`Character::_SetTarget`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
