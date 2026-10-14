# LookAt

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#10**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `LookAt` | bindFunction | `Character::_LookAt` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132289 |
| `LookAt` | bindMethod | `Character::_LookAt` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132294 |

Address-identified callback: `Character::_LookAt` at `0x003b8ed8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`, unified lines 134751-134777.
Recovered declaration: `sfc::script::lua::Value *__fastcall Character::_LookAt( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: pointer tag2/userdata tag7; unit/scale target identity. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:21`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Arg1 pointer tag2/userdata tag7 dispatches Cmd_LookAt(receiver controller,target). Controller permits when override byte9 true, or global blocked false and local locked byte8 false; otherwise suppresses virtual look command. Wrapper missing/wrong type does nothing. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/004052bc.c:3`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/004052bc.c:3`.
**State, timers and events:**
- PROVEN: No Lua result; conditional controller look command, no wrapper timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/004052bc.c:3`.
**Return:**
- PROVEN: No Lua result; conditional controller look command, no wrapper timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/004052bc.c:3`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** character_skill_native_v5.cpp:115-118 admits pointer/object types and forwards world_.look_at; preserves wrapper but relies supplied controller service for lock/override behavior; does not itself prove those gates for every actor.
- Source evidence: `port/windows-foundation/main.cpp`:6353 — `if(dh2_character_look_at_point(&look,point)){e="Injured Blur source LookAt failed";return false;}`.
- Source evidence: `port/windows-foundation/main.cpp`:6399 — `if(dh2_nav_look_towards(&heading,direction)){e="KnockBack Focus source LookAt failed";return false;}`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:578 — `return fail(error, "Original GameObject LookAt(Point) kernel rejected the target");`.
- Source evidence: `port/windows-foundation/features/combat/b035_native_test_fixture_v1.hpp`:94 — `check(event.actor==1&&event.source_other_actor==2&&cells.at(1).locked,"Source LookAt/CancelSneaking ordering/attacker");`.
- Source evidence: `port/windows-foundation/features/combat/b035_native_test_fixture_v1.hpp`:97 — `float heading=actor->transform.rotation[2];check(dh2_nav_look_towards(&heading,direction)==0,"Source LookAt heading");`.
- Source evidence: `port/windows-foundation/features/combo_chain/source_combo_consumers.cpp`:69 — `if(dh2_character_look_at_point(&look,request->position)){bridge.failure="Original GameObject LookAt(Point) rejected backing";return -1;}`.
- Source evidence: `port/windows-foundation/features/combo_chain/source_combo_consumers.cpp`:72 — `bridge.failure="Unexpected original LookAt controllable service";return -1;`.
- Source evidence: `port/windows-foundation/features/combo_chain/source_combo_consumers.cpp`:77 — `error=bridge.failure.empty()?"Original controller/Character LookAt failed":std::move(bridge.failure);return false;`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:269 — `contains(pre, "LookAt(target)");`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:548 — `return fail(error, "Original GameObject LookAt(Point) kernel rejected the target");`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1226 — `active.receipt.detail = "GroundSlam source LookAt failed after OnPre";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_target_query_v1.cpp`:400 — `return fail(error, "BashDown LookAt requires a selected character target from its source query");`.

**Concrete behavioral difference:** character_skill_native_v5.cpp:115-118 admits pointer/object types and forwards world_.look_at; preserves wrapper but relies supplied controller service for lock/override behavior; does not itself prove those gates for every actor.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: LookAt("target") No command; string type rejected. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/004052bc.c`:3.
2. **PROVEN:** Given: LookAt(T) admitted userdata, controller locked1/override0 No virtual look command. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/004052bc.c`:3.
3. **PROVEN:** Given: LookAt(T), override1, globally blocked1 Dispatches virtual controller target LookAt despite blocked flag. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8ed8.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/004052bc.c`:3.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **106** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:183` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:248` | outside scoped roots | `LookAt(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:272` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:296` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:317` | outside scoped roots | `LookAt(closestTarget)` | `[{"index":1,"expression":"closestTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:501` | outside scoped roots | `LookAt( target )` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:156` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:324` | outside scoped roots | `LookAt( target )` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:139` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:204` | outside scoped roots | `LookAt(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:228` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:252` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:273` | outside scoped roots | `LookAt(closestTarget)` | `[{"index":1,"expression":"closestTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:242` | outside scoped roots | `LookAt(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:266` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:290` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:311` | outside scoped roots | `LookAt(closestTarget)` | `[{"index":1,"expression":"closestTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:156` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:62` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:94` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:19` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:197` | act1_level_boss_script_closure | `LookAt(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:41` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:36` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:81` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:46` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_00.luac:46` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_01.luac:47` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_ranged_00.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_ranged_00.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:47` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:76` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:47` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:35` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:78` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:47` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:52` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:43` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:42` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:46` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:151` | outside scoped roots | `LookAt( player )` | `[{"index":1,"expression":"player","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_arrow_default.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_arrow_default.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:53` | outside scoped roots | `LookAt(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_fireball.luac:43` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_fireball.luac:49` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_poisonball.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_poisonball.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:99` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:71` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:94` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:111` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:119` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:127` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:58` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:123` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:139` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:69` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:62` | player_skill_lua | `LookAt( (GetTargetListTop()) )` | `[{"index":1,"expression":"(GetTargetListTop())","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:66` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:83` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:66` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:237` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:71` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:163` | player_skill_lua | `LookAt(GetTargetListTop())` | `[{"index":1,"expression":"GetTargetListTop()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:63` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:228` | player_skill_lua | `LookAt( (GetTargetListTop()) )` | `[{"index":1,"expression":"(GetTargetListTop())","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:69` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:145` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:63` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:62` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:71` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:67` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:85` | player_skill_lua | `LookAt(GetTargetListTop())` | `[{"index":1,"expression":"GetTargetListTop()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:90` | player_skill_lua | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:47` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack_noskill.luac:46` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_charge.luac:46` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:44` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:47` | outside scoped roots | `LookAt(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:61` | outside scoped roots | `LookAt(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:69` | outside scoped roots | `LookAt(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:36` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:50` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:36` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:50` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:42` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:44` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:47` | act1_level_boss_script_closure | `LookAt(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:61` | act1_level_boss_script_closure | `LookAt(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:69` | act1_level_boss_script_closure | `LookAt(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:46` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:45` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:51` | act1_level_boss_script_closure | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:36` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:36` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:50` | outside scoped roots | `LookAt(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |

## Related natives

`Character::_LookAt`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
