# UseMana

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

Engine functions to implement next ranking: **#43**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `UseMana` | bindFunction | `Character::_UseMana` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132969 |
| `UseMana` | bindMethod | `Character::_UseMana` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132974 |

Address-identified callback: `Character::_UseMana` at `0x003b7864`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`, unified lines 133809-133832.
Recovered declaration: `void __fastcall Character::_UseMana( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: number tag3 truncated int; unit/scale x256 MP cost. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:15`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires numeric tag3 amount truncated int in x256 MP units. UseMana returns true without deduction for online receiver virtual+84 true, saved GOD_MANA, debug GOD_MANA, or receiver byte5360. Otherwise HasMana gate; if true PROPS_Add property41 by -amount and trace stats, else false/no write. Negative amount is assertion domain (except early online bypass), not supported cost. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdef4.c:11`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdef4.c:11`.
**State, timers and events:**
- PROVEN: Boolean if numeric admitted, otherwise no Lua value; conditional MP deduction, stats trace, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdef4.c:11`.
**Return:**
- PROVEN: Boolean if numeric admitted, otherwise no Lua value; conditional MP deduction, stats trace, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdef4.c:11`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_skill_native_v5.cpp:86-90 uses shared source mana policy and property backend; callback path exists, missing policy provider yields required-service failure rather than invented mana. Supported nonnegative source domain, no runtime claim.
- Source evidence: `port/windows-foundation/features/faery_menu/celest_cast_v1.cpp`:223 — `return fail(error, "Celest UseMana requires synchronized same-world ActorState, CharacterState and MP");`.
- Source evidence: `port/windows-foundation/features/faery_menu/celest_cast_v1.cpp`:231 — `return fail(error, "Celest UseMana PropertyAdd did not debit the source MP property");`.
- Source evidence: `port/windows-foundation/features/faery_menu/celest_cast_v1.cpp`:243 — `if (error.empty()) error = "Same-world Celest UseMana property update failed";`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_cast_session_v1.cpp`:53 — `return fail(error, "Hotty CombatSession preparation requires the complete source target list before UseMana/cooldown");`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_cast_v1.cpp`:422 — `return fail(error, "Hotty UseMana requires synchronized same-world ActorState, CharacterState and MP property");`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_cast_v1.cpp`:430 — `return fail(error, "Hotty UseMana PropertyAdd did not debit the source MP property");`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_cast_v1.cpp`:441 — `if (error.empty()) error = "Same-world Hotty UseMana property update failed";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:502 — `active.receipt.detail = "Celest source UseMana committed before a later OnPre provider failure";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:525 — `return fail(error, "Celest OnPre did not reach its authored UseMana/cooldown prefix");`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:613 — `active.receipt.detail = "Hotty source UseMana committed before a later OnPre provider failure";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:639 — `return fail(error, "Hotty OnPre did not reach its authored UseMana/cooldown prefix");`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:854 — `active.receipt.detail="Authored OnSkillCheck_ HasMana returned false; no departure or UseMana prefix ran";`.

**Port comparison:** character_skill_native_v5.cpp:86-90 uses shared source mana policy and property backend; callback path exists, missing policy provider yields required-service failure rather than invented mana. Supported nonnegative source domain, no runtime claim.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: ordinary offline MP512; UseMana(256) Returns true, MP deducted by256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdef4.c`:11.
2. **PROVEN:** Given: ordinary MP128; UseMana(256) Returns false, no MP deduction. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdef4.c`:11.
3. **PROVEN:** Given: GOD_MANA saved enabled; UseMana(256) Returns true with unchanged MP. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7864.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bdef4.c`:11.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **40** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:232` | outside scoped roots | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:72` | outside scoped roots | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:69` | outside scoped roots | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:72` | outside scoped roots | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:72` | outside scoped roots | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:101` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:74` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:97` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:61` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:127` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:73` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:88` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:65` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:96` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:86` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:89` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:56` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:69` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:73` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:114` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:75` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:167` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:69` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:231` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:132` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:75` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:143` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:149` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:283` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:77` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:112` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:101` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:137` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:67` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:82` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:67` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:75` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:73` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:88` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:70` | player_skill_lua | `UseMana(mana_cost)` | `[{"index":1,"expression":"mana_cost","literal_tokens":[]}]` |

## Related natives

`Character::_UseMana`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
