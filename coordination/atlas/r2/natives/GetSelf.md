# GetSelf

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#27**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetSelf` | bindFunction and/or Method (see registration ledger) | `GameObject::_GetSelf` | `GameObject::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7ec.c:31 |

Address-identified callback: `GameObject::_GetSelf` at `0x0038eaf4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eaf4.c`, unified lines 105981-105989.
Recovered declaration: `void __fastcall GameObject::_GetSelf( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN:** No Lua arguments are inspected by this audited callback; extra arguments are ignored. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eaf4.c:1`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Pushes the bound receiver as userdata without mutation or argument inspection. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eaf4.c:7`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eaf4.c:1`.
**Return:**
- PROVEN: One userdata carrying the receiver identity.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** Exact native-address production route scan finds no GetSelf case in CharacterScriptObjects; remaining unsupported method path fails explicitly. Catalog registration alone is not implementation.
- Source evidence: `port/level-world/character_script_objects.cpp`:135.

**Concrete behavioral difference:** Exact native-address production route scan finds no GetSelf case in CharacterScriptObjects; remaining unsupported method path fails explicitly. Catalog registration alone is not implementation.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Receiver A; GetSelf() Returns userdata A. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eaf4.c`:7.
2. **PROVEN:** Given: Receiver B; GetSelf(1,2) Returns userdata B; extra arguments ignored. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eaf4.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **37** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/dummy.luac:28` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/dummy.luac:35` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:385` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:337` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:662` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:386` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:285` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:217` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:218` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:127` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:128` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:220` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:314` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:206` | act1_level_boss_script_closure | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:289` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:318` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:102` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:118` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:40` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:58` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:59` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:93` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:25` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:204` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:42` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:141` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:158` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:169` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:293` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:322` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:127` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:121` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:74` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:91` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:31` | player_skill_lua | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:86` | outside scoped roots | `GetSelf()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:86` | act1_level_boss_script_closure | `GetSelf()` | `[]` |

## Related natives

`GameObject::_GetSelf`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
