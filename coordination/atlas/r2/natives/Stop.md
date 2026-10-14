# Stop

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#19**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `Stop` | bindFunction and/or Method (see registration ledger) | `Character::_Stop` | `Character::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:33 |

Address-identified callback: `Character::_Stop` at `0x003b56b4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56b4.c`, unified lines 132262-132266.
Recovered declaration: `int __fastcall Character::_Stop(Character *this, const sfc::script::lua::Arguments *a2, v2Controller **a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN:** No Lua arguments are inspected by this audited callback; extra arguments are ignored. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56b4.c:1`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Calls bound Character controller at byte +888. Controller forwards owner virtual slot +52 only when forced byte +9 is nonzero, or global blocked=false and local byte +8=false. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56b4.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040559c.c:3`.
- PROVEN: Recovered controller handoff traces Character Ctrl_Stop: query remote-update first; local owner calls GameObject Stop then synchronously raises event 0x3f with null payload. Evidence: `port/level-world/reference/character-controller-commands/NOTES.md:18`.
**Gates:**
- PROVEN: Forced bypass wins over both block gates. Remote-update suppresses local stop/event after controller dispatch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040559c.c:3`, `port/level-world/reference/character-controller-commands/NOTES.md:18`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56b4.c:1`.
**Return:**
- PROVEN: No Lua outputs; callback C integer is not a ReturnValues push.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Source controller gates are reconstructed; session routes to required command services. Full physical Stop/event delivery depends on supplied live services and is not established by this contract audit.
- Source evidence: `port/level-world/character_script_session.cpp`:168.
- Source evidence: `port/level-world/reference/character-controller-commands/NOTES.md`:34.

**Concrete behavioral difference:** Source controller gates are reconstructed; session routes to required command services. Full physical Stop/event delivery depends on supplied live services and is not established by this contract audit.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Stop() with forced=1 and global blocked=1 Controller forwards exactly once; local action still depends on remote-update. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040559c.c`:3.
2. **PROVEN:** Given: Stop() with forced=0 and global blocked=1 No owner command, stop operation or event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040559c.c`:3.
3. **PROVEN:** Given: Stop() with forced=0, global blocked=0, local lock=1 No owner command. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040559c.c`:3.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **59** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:68` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:79` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:90` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:132` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:159` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:51` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:120` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:466` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:512` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:528` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:289` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:335` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:351` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:39` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:82` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:413` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:449` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:461` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:477` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/follower.luac:43` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/follower.luac:50` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/follower.luac:57` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:457` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:497` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:511` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:529` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:85` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:100` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:131` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:145` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:69` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:399` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:80` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:82` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:91` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:126` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:168` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:182` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:125` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:239` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:248` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:51` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:289` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:325` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:339` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:355` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:412` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:365` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:676` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:386` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:426` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:440` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:458` | outside scoped roots | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:55` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:158` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:170` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:190` | act1_level_boss_script_closure | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:127` | player_skill_lua | `Stop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:121` | player_skill_lua | `Stop()` | `[]` |

## Related natives

`Character::_Stop`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
