# SetSkillCooldownTimerId__

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#142**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SetSkillCooldownTimerId__` | bindFunction | `Character::_SetSkillCooldownTimerId` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:133059 |
| `SetSkillCooldownTimerId__` | bindMethod | `Character::_SetSkillCooldownTimerId` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:133064 |

Address-identified callback: `Character::_SetSkillCooldownTimerId` at `0x003b97e0`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`, unified lines 135234-135298.
Recovered declaration: `void __fastcall Character::_SetSkillCooldownTimerId( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: skill index getNumber; unit/scale receiver skill slot. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:22`.
- Lua argument 2: number tag3/nil tag0; unit/scale timer ID;-1 invalid. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:41`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires >=2 args skill index and timer ID number/nil. Numeric first arg bypasses bounds guard; nonnumeric is converted/bounds-checked. Nonnull script at receiver skill slot gets script+24 unsigned timer ID; nil sets-1. Does not create or stop timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:26`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:45`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:26`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:45`.
**State, timers and events:**
- PROVEN: No Lua value; script cooldown field only; no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:26`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:45`.
**Return:**
- PROVEN: No Lua value; script cooldown field only; no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:26`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c:45`.

## Lua wrapper/helper traces

Proven source chain: `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:102` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:98` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:74` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:74` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:74` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:66` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:144` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:76` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:89` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:71` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `SetSkillCooldownTimerId__`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** Original helper starts a timer separately, then stores ID; generic scene callback0x3b97e0 absent (:144-146). Port cooldown schedules do not prove arbitrary Lua handle setter.

**Concrete behavioral difference:** Original helper starts a timer separately, then stores ID; generic scene callback0x3b97e0 absent (:144-146). Port cooldown schedules do not prove arbitrary Lua handle setter.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: valid skill script0; SetSkillCooldownTimerId__(0,12) Skill0 cooldown ID12. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:26, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:45.
2. **PROVEN:** Given: valid script0; SetSkillCooldownTimerId__(0,nil) Skill0 cooldown ID-1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:26, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:45.
3. **PROVEN:** Given: SetSkillCooldownTimerId__(0,true) No assignment; second tag neither number nor nil. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:26, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b97e0.c`:45.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **2** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:199` | autoloaded_lua_common_helpers | `SetSkillCooldownTimerId__(skill.ID, nil)` | `[{"index":1,"expression":"skill.ID","literal_tokens":[]},{"index":2,"expression":"nil","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:209` | autoloaded_lua_common_helpers | `SetSkillCooldownTimerId__(SKILL_ID, timerId)` | `[{"index":1,"expression":"SKILL_ID","literal_tokens":[]},{"index":2,"expression":"timerId","literal_tokens":[]}]` |

## Related natives

`Character::_SetSkillCooldownTimerId`, `SetSkillCooldownTimerId__`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
