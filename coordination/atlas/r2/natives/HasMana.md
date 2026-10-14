# HasMana

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#42**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `HasMana` | bindFunction and/or Method (see registration ledger) | `Character::_HasMana` | `Character::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:695 |

Address-identified callback: `Character::_HasMana` at `0x003b78ec`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b78ec.c`, unified lines 133834-133857.
Recovered declaration: `void __fastcall Character::_HasMana( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Lua number tag 3 -> signed integer truncation; unit/scale Raw 1/256 mana amount; no multiplication by 256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b78ec.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b78ec.c:19`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Offline or online with null authority virtual result: compares amount <= cached MP property 41. Online nonnull authority result returns true without MP test. Does not debit mana. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bd40c.c:6`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bd40c.c:29`.
**Gates:**
- PROVEN: Missing/non-number argument emits no result. Negative amount asserts; nonfatal assertion modes continue the compare. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b78ec.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bd40c.c:17`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b78ec.c:1`.
**Return:**
- PROVEN: One Lua boolean for valid numeric argument; zero values otherwise.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Frozen ManaRun::has rejects negative cost with service failure, whereas native nonfatal assertion modes still compare it. Ordinary nonnegative compare and online authority bypass are implemented.
- Source evidence: `port/level-world/character_skill_mana_v5.cpp`:19.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:87.

**Concrete behavioral difference:** Frozen ManaRun::has rejects negative cost with service failure, whereas native nonfatal assertion modes still compare it. Ordinary nonnegative compare and online authority bypass are implemented.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Offline MP=1280; HasMana(1024) true; MP remains 1280. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bd40c.c`:6.
2. **PROVEN:** Given: Offline MP=1280; HasMana(1536) false; MP unchanged. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bd40c.c`:6.
3. **PROVEN:** Given: Online nonnull authority; MP=0; HasMana(256) true. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bd40c.c`:29.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **40** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:219` | outside scoped roots | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:59` | outside scoped roots | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:57` | outside scoped roots | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:59` | outside scoped roots | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:61` | outside scoped roots | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:88` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:58` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:83` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:47` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:110` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:52` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:76` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:50` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:84` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:52` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:61` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:45` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:55` | player_skill_lua | `HasMana(mana_cost, false)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:68` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:215` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:58` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:151` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:45` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:216` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:114` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:53` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:138` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:131` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:278` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:52` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:83` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:78` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:127` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:49` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:72` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:49` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:57` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:51` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:73` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:64` | player_skill_lua | `HasMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |

## Related natives

`Character::_HasMana`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
