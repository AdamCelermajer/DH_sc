# GetRand

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#17**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetRand` | bindFunction and/or Method (see registration ledger) | `GameObject::_Rand` | `GameObject::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038d7ec.c:358 |

Address-identified callback: `GameObject::_Rand` at `0x00393310`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c`, unified lines 109168-109229.
Recovered declaration: `void __fastcall GameObject::_Rand( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: One/two numeric values converted getUInteger; other arities ignored; unit/scale One: [0,bound); two: minimum + Random(maximum-minimum), unsigned subtraction without sorting. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:45`.
- Lua argument 2: Optional numeric maximum for exactly two arguments; unit/scale Exclusive upper bound in ordinary increasing interval. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:45`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Other arities use minimum 0 and bound 100. Random updates seed=(59051*seed+177149)%0xDAF26B only for nonzero bound; debug counter increments even at zero bound. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c:8`.
- PROVEN: Online loads seed from bound receiver word 63 and writes updated seed back; offline uses process seed. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:53`.
**Gates:**
- PROVEN: Exactly one/two arguments require number tag 3, else emit nothing. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:34`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c:1`.
**Return:**
- PROVEN: One integer when accepted; zero outputs for invalid one/two argument types.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Frozen CharacterScriptObjects explicitly fails online seed-owner delivery. Negative numeric conversion wraps signed value to unsigned instead of native getUInteger clamp to zero; nonfinite values are rejected.
- Source evidence: `port/level-world/character_script_objects.cpp`:104.
- Source evidence: `port/level-world/character_script_objects.cpp`:119.

**Concrete behavioral difference:** Frozen CharacterScriptObjects explicitly fails online seed-owner delivery. Negative numeric conversion wraps signed value to unsigned instead of native getUInteger clamp to zero; nonfinite values are rejected.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: GetRand(7,7) Returns 7; seed is unchanged, debug counter increments. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c`:8.
2. **PROVEN:** Given: GetRand("bad") Zero Lua outputs. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393310.c`:27.
3. **PROVEN:** Given: Offline process seed=0; GetRand(0,100) Returns 49; seed=177149. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c`:5.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **68** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:34` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:65` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:98` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:244` | outside scoped roots | `GetRand(1, #t_rndTargetList)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"#t_rndTargetList","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:80` | outside scoped roots | `GetRand(1, 100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:100` | outside scoped roots | `GetRand(1, 100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:8` | outside scoped roots | `GetRand(m_timer_attack_min, m_timer_attack_max)` | `[{"index":1,"expression":"m_timer_attack_min","literal_tokens":[]},{"index":2,"expression":"m_timer_attack_max","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:72` | outside scoped roots | `GetRand(m_timer_attack_min, m_timer_attack_max)` | `[{"index":1,"expression":"m_timer_attack_min","literal_tokens":[]},{"index":2,"expression":"m_timer_attack_max","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:200` | outside scoped roots | `GetRand(1, #t_rndTargetList)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"#t_rndTargetList","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:153` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:190` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:148` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:175` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:187` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:321` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:344` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:187` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:64` | outside scoped roots | `GetRand(3000, 7000)` | `[{"index":1,"expression":"3000","literal_tokens":["3000"]},{"index":2,"expression":"7000","literal_tokens":["7000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:68` | outside scoped roots | `GetRand(3000, 7000)` | `[{"index":1,"expression":"3000","literal_tokens":["3000"]},{"index":2,"expression":"7000","literal_tokens":["7000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:185` | act1_level_boss_script_closure | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:107` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:111` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:139` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:157` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:165` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:353` | outside scoped roots | `GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY)` | `[{"index":1,"expression":"OUTOFRANGE_MIN_DELAY","literal_tokens":[]},{"index":2,"expression":"OUTOFRANGE_MAX_DELAY","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:163` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:182` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:190` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:366` | outside scoped roots | `GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY)` | `[{"index":1,"expression":"OUTOFRANGE_MIN_DELAY","literal_tokens":[]},{"index":2,"expression":"OUTOFRANGE_MAX_DELAY","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:73` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:92` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:100` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:293` | outside scoped roots | `GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY)` | `[{"index":1,"expression":"OUTOFRANGE_MIN_DELAY","literal_tokens":[]},{"index":2,"expression":"OUTOFRANGE_MAX_DELAY","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:201` | outside scoped roots | `GetRand(1, #spots+1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"#spots+1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:249` | act1_level_boss_script_closure | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:272` | act1_level_boss_script_closure | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:290` | act1_level_boss_script_closure | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:298` | act1_level_boss_script_closure | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:350` | act1_level_boss_script_closure | `GetRand(1, numSpawners)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"numSpawners","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:489` | act1_level_boss_script_closure | `GetRand(1,3)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:586` | act1_level_boss_script_closure | `GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY)` | `[{"index":1,"expression":"OUTOFRANGE_MIN_DELAY","literal_tokens":[]},{"index":2,"expression":"OUTOFRANGE_MAX_DELAY","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:153` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:190` | outside scoped roots | `GetRand(1,100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/desert_spider_spawn_container.luac:26` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/lighthouse_spawn_container.luac:24` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/moth_spawn_container.luac:24` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/plant_spawn_container.luac:28` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/plant_spawn_container.luac:29` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/plant_spawn_container.luac:37` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/plant_spawn_container2.luac:28` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/plant_spawn_container2.luac:29` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/plant_spawn_container2.luac:37` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_buff.luac:33` | outside scoped roots | `GetRand(1, #BUFF_STACK)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"#BUFF_STACK","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/zombie_spawn_container.luac:24` | outside scoped roots | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:123` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:36` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:23` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:24` | player_skill_lua | `GetRand(0,FromFixed(attacker:GetProp(GetPyStruct("CharacterProperties", "Slow_Resist_Chance"))))` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"FromFixed(attacker:GetProp(GetPyStruct(\"CharacterProperties\", \"Slow_Resist_Chance\")))","literal_tokens":["\"CharacterProperties\"","\"Slow_Resist_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:101` | player_skill_lua | `GetRand(1, 4)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"4","literal_tokens":["4"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:42` | player_skill_lua | `GetRand(1, 100)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:97` | player_skill_lua | `GetRand(MIN_STUN_DURATION, MAX_STUN_DURATION)` | `[{"index":1,"expression":"MIN_STUN_DURATION","literal_tokens":[]},{"index":2,"expression":"MAX_STUN_DURATION","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:31` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:34` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:99` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:134` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:135` | player_skill_lua | `GetRand(0,100)` | `[{"index":1,"expression":"0","literal_tokens":["0"]},{"index":2,"expression":"100","literal_tokens":["100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:118` | outside scoped roots | `GetRand(1,#g_bomb_spawns)` | `[{"index":1,"expression":"1","literal_tokens":["1"]},{"index":2,"expression":"#g_bomb_spawns","literal_tokens":[]}]` |

## Related natives

`GameObject::_Rand`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
