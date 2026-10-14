# attacker:ApplyPropClass

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#179**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ApplyPropClass` | bindFunction | `Character::_ApplyPropClass` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132852 |
| `ApplyPropClass` | bindMethod | `Character::_ApplyPropClass` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132857 |

Address-identified callback: `Character::_ApplyPropClass` at `0x003babdc`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`, unified lines 135973-136048.
Recovered declaration: `unsigned int __fastcall Character::_ApplyPropClass( unsigned int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getter not adjacent in decompilation; handler lines 136003, 136016, 136039.

## Recovered behavior, gates, state, timers, events and return

- **PROVEN:** Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:641`.
- **PROVEN:** A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:72`.
- **PROVEN:** The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:9`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

### ApplyBuff binding and pointer branch

- **PROVEN:** Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:641`.
- **PROVEN:** A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:72`.
- **PROVEN:** The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:9`.


## Current frozen port source

**PARTIAL:** Source-backed class evaluation is wired for skill owners; generic script-owner/class-table admission remains context-dependent.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:331 — `!contains(delayed, "TargetListSearch(AOE_RANGE, 360") || !contains(delayed, "ApplyPropClass(AOE_CLASS_ID)"))`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:273 — `next.delayed_combat_roll = contains(delayed, "ApplyPropClass(CLASS_ID)") && contains(delayed, "SkillCombatRoll(target)") &&`.
- Source evidence: `port/windows-foundation/features/faery_menu/celest_cast_v1.cpp`:143 — `if (error.empty()) error = "Original Celest ApplyPropClass projection failed";`.
- Source evidence: `port/windows-foundation/features/faery_menu/celest_source_use_v1.cpp`:134 — `if (error.empty()) error = "Celest source ApplyPropClass failed before SpellCombatRoll";`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_cast_v1.cpp`:178 — `if (error.empty()) error = "Source Hotty ApplyPropClass projection failed";`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_character_cast_v1.cpp`:116 — `if (error.empty()) error = "Hotty source ApplyPropClass projection failed before SpellCombatRoll";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_mage_skill_owner_v1.cpp`:191 — `return fail(error, "Mage skill Use requires its profile and ApplyPropClass formula sheet");`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1216 — `active.receipt.detail = "Source ApplyPropClass failed after committed OnPre prefix";`.
- Source evidence: `port/level-world/character_skill_properties_v1.cpp`:53 — `return dh2_script_vm_bind_source_values(vm,"ClearProps",skill_clear_properties_v1,b)||dh2_script_vm_bind_source_values(vm,"SetProp",skill_set_property_v1,b)||dh2_script_vm_bind_source_values(vm,"ApplyPropClass",skill_apply_class_v1,b)?-1:0;`.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: ApplyBuff(validClass, liveBuffSheetPointer). Class rows execute on that sheet in buff mode, then receiver properties recalculate. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:641, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:9.
2. **PROVEN:** Given: ApplyBuff(validClass, nullPointer). Pointer branch performs no class application or recalculation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:641, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:9.
3. **PROVEN:** Given: ApplyPropClass(validClass, true). Boolean branch applies the class through PROPS_ApplyClass, not the pointer-sheet buff branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:641, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:9.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:20` | player_skill_lua | `attacker:ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |

## Related natives

`Character::_ApplyPropClass`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
