# RemoveBuff

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#38**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `RemoveBuff` | bindFunction | `Character::_RemoveBuff` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132879 |
| `RemoveBuff` | bindMethod | `Character::_RemoveBuff` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132884 |

Address-identified callback: `Character::_RemoveBuff` at `0x003b842c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`, unified lines 134320-134374.
Recovered declaration: `unsigned int __fastcall Character::_RemoveBuff( unsigned int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getter not adjacent in decompilation; handler lines 134346, 134357, 134363.

## Recovered behavior, gates, state, timers, events and return

- **PROVEN:** Character::_RemoveBuff 0x3b842c calls PROPS_DelBuff on the bound receiver at Character+1376. The optional pointer selects an instance inside that receiver's class declaration; it never changes the receiver to the pointer's owner. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:614`.
- **PROVEN:** The first argument must be numeric and less than ClassTable size. With one argument deletion uses nullptr; with a second pointer-tag argument the pointer is supplied to PROPS_DelBuff 0x3e101c. Invalid first types/IDs or unsupported second types do not enter deletion. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:42`.
- **PROVEN:** PROPS_DelBuff removes a sole instance regardless of the supplied pointer; for multiple instances it requires a nonnull exact pointer match. Thus receiverA:RemoveBuff(class, receiverB_handle) can delete A's sole instance, but does not delete B's buff. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c:72`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c:108`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

### RemoveBuff receiver and foreign handles

- **PROVEN:** Character::_RemoveBuff 0x3b842c calls PROPS_DelBuff on the bound receiver at Character+1376. The optional pointer selects an instance inside that receiver's class declaration; it never changes the receiver to the pointer's owner. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:614`.
- **PROVEN:** The first argument must be numeric and less than ClassTable size. With one argument deletion uses nullptr; with a second pointer-tag argument the pointer is supplied to PROPS_DelBuff 0x3e101c. Invalid first types/IDs or unsupported second types do not enter deletion. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:42`.
- **PROVEN:** PROPS_DelBuff removes a sole instance regardless of the supplied pointer; for multiple instances it requires a nonnull exact pointer match. Thus receiverA:RemoveBuff(class, receiverB_handle) can delete A's sole instance, but does not delete B's buff. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c:72`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c:108`.


## Current frozen port source

**PARTIAL:** Identifier is present in production source but a complete reusable Lua callback/binding path is not proven.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:939 — `active.receipt.detail = "Source OnPre RemoveBuff failed after the UseMana prefix: " + buff_error;`.

**Concrete behavioral difference:** Original RemoveBuff deletes within its bound receiver and its optional handle only selects among that receiver class instances. The current timed-buff owner erases every instance with the same class name; that is broader than source deletion when multiple instances exist.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: A has one instance of classC; B has its own; call A:RemoveBuff(C, B_handle). A's declaration is deleted; B remains untouched. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:614, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c`:108.
2. **PROVEN:** Given: A has two instances; call A:RemoveBuff(C, B_handle), where handle is not in A. No matching instance is deleted. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:614, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c`:108.
3. **PROVEN:** Given: A has one instance; call A:RemoveBuff(C) with valid numeric class. A's sole instance/declaration is removed and properties recalculated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:614, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c`:108.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **64** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:95` | outside scoped roots | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:121` | act1_level_boss_script_closure | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:202` | act1_level_boss_script_closure | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:123` | act1_level_boss_script_closure | `RemoveBuff(OID_HASTE)` | `[{"index":1,"expression":"OID_HASTE","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:61` | outside scoped roots | `RemoveBuff(oldbuff,fire_buff)` | `[{"index":1,"expression":"oldbuff","literal_tokens":[]},{"index":2,"expression":"fire_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:66` | outside scoped roots | `RemoveBuff(oldbuff, water_buff)` | `[{"index":1,"expression":"oldbuff","literal_tokens":[]},{"index":2,"expression":"water_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:71` | outside scoped roots | `RemoveBuff(oldbuff, wind_buff)` | `[{"index":1,"expression":"oldbuff","literal_tokens":[]},{"index":2,"expression":"wind_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:76` | outside scoped roots | `RemoveBuff(oldbuff, earth_buff)` | `[{"index":1,"expression":"oldbuff","literal_tokens":[]},{"index":2,"expression":"earth_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:81` | outside scoped roots | `RemoveBuff(oldbuff, lightning_buff)` | `[{"index":1,"expression":"oldbuff","literal_tokens":[]},{"index":2,"expression":"lightning_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_bowmastery.luac:29` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:20` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:39` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:61` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:25` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:28` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:35` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:84` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:92` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:26` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:30` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:55` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:70` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:59` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:60` | player_skill_lua | `RemoveBuff(DUMMY_ID, buff2)` | `[{"index":1,"expression":"DUMMY_ID","literal_tokens":[]},{"index":2,"expression":"buff2","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:68` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:69` | player_skill_lua | `RemoveBuff(DUMMY_ID, buff2)` | `[{"index":1,"expression":"DUMMY_ID","literal_tokens":[]},{"index":2,"expression":"buff2","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:31` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:34` | player_skill_lua | `RemoveBuff(BUFF_ID, buff2)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff2","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:46` | player_skill_lua | `RemoveBuff(BUFF_ID, buff_survival)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff_survival","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_toughness.luac:26` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:69` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:30` | player_skill_lua | `RemoveBuff(CLASS_ID, buff[counter])` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff[counter]","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:77` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:48` | player_skill_lua | `RemoveBuff(BUFF_ID, self_buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"self_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:61` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:84` | player_skill_lua | `RemoveBuff(BUFF_ID, SLOT1_BUFF)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"SLOT1_BUFF","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:92` | player_skill_lua | `RemoveBuff(BUFF_ID, SLOT2_BUFF)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"SLOT2_BUFF","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:100` | player_skill_lua | `RemoveBuff(BUFF_ID, SLOT3_BUFF)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"SLOT3_BUFF","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_elemental_mastery.luac:27` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:29` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_magi_shielding.luac:30` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_staffmaster.luac:27` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_leecher.luac:27` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:30` | player_skill_lua | `RemoveBuff(BUFF_ID, buff_wrath)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff_wrath","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:79` | player_skill_lua | `RemoveBuff(data.BUFF_ID, self_buff)` | `[{"index":1,"expression":"data.BUFF_ID","literal_tokens":[]},{"index":2,"expression":"self_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:70` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:94` | player_skill_lua | `RemoveBuff(BUFF_ID, memory_buff[i])` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"memory_buff[i]","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:101` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:65` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:88` | player_skill_lua | `RemoveBuff(BUFF_ID, memory_buff[i])` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"memory_buff[i]","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:94` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:59` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:115` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_acrobat.luac:27` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:42` | player_skill_lua | `RemoveBuff(CLASS_ID, buffbase)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buffbase","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:46` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_combomaster.luac:27` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_findweakness.luac:29` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:78` | player_skill_lua | `RemoveBuff(BUFF_ID, buff)` | `[{"index":1,"expression":"BUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_combat_master.luac:28` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_fast_metabolism.luac:27` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_hardiness.luac:38` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:53` | player_skill_lua | `RemoveBuff(CLASS_ID, self_buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"self_buff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_potion_addict.luac:27` | player_skill_lua | `RemoveBuff(CLASS_ID, buff)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |

## Related natives

`Character::_RemoveBuff`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
