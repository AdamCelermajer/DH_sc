# object:ApplyBuff

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#211**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ApplyBuff` | bindFunction | `Character::_ApplyPropClass` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132906 |
| `ApplyBuff` | bindMethod | `Character::_ApplyPropClass` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132911 |

Address-identified callback: `Character::_ApplyPropClass` at `0x003babdc`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`, unified lines 135973-136048.
Recovered declaration: `unsigned int __fastcall Character::_ApplyPropClass( unsigned int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getter not adjacent in decompilation; handler lines 136003, 136016, 136039.

## Recovered behavior, gates, state, timers, events and return

- **PROVEN:** Type5 dispatches to _ScaleWithPercentage 0x3e2c78: destination S[d] becomes arithmetic_shift_right_8(low32(S[a]*S[d])). Type6 dispatches to _ScaleWithPercentageForBuff 0x3e2c10: scale S[a] multiplies S[d], or owner resolved[d] when the caller enables buff mode. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c78.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c10.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:62`.
- **PROVEN:** Type6 then falls through to _AddValue 0x3e2bdc, adding raw Param1 (the scale property index). This unusual addition is actual original behavior: assembly calls 0x3e2c10 at0x3e2ef4 and 0x3e2bdc at0x3e2f04. Do not remove it as an assumed bug. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:64`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2bdc.c:6`, `port/game-data/reference/classes/reference/original-functions.asm:183`.
- **PROVEN:** For valid property indices the frozen dh2_class_formula cases5/6 match these formulas, including low32 multiplication, arithmetic right shift and the raw Param1 addition. Its buff argument denotes the owner sheet for the type6 destination operand; reversing multiplication order is equivalent. Evidence: `port/game-data/class_tables.cpp:23`, `port/game-data/class_tables.cpp:24`.
- **PROVEN:** A resulting buff-sheet Max_HP delta is not the final actor Max_HP. RecalcProperty type&4 accumulates the base, saved, gear and set buff-sheet values. A smaller buff delta alone does not prove Toughness shrinks final HP. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:91`, `port/game-data/properties.cpp:16`.
- **PROVEN:** Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:641`.
- **PROVEN:** A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:72`.
- **PROVEN:** The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:9`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

### Class formula types 5 and 6

- **PROVEN:** Type5 dispatches to _ScaleWithPercentage 0x3e2c78: destination S[d] becomes arithmetic_shift_right_8(low32(S[a]*S[d])). Type6 dispatches to _ScaleWithPercentageForBuff 0x3e2c10: scale S[a] multiplies S[d], or owner resolved[d] when the caller enables buff mode. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c78.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c10.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:62`.
- **PROVEN:** Type6 then falls through to _AddValue 0x3e2bdc, adding raw Param1 (the scale property index). This unusual addition is actual original behavior: assembly calls 0x3e2c10 at0x3e2ef4 and 0x3e2bdc at0x3e2f04. Do not remove it as an assumed bug. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:64`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2bdc.c:6`, `port/game-data/reference/classes/reference/original-functions.asm:183`.
- **PROVEN:** For valid property indices the frozen dh2_class_formula cases5/6 match these formulas, including low32 multiplication, arithmetic right shift and the raw Param1 addition. Its buff argument denotes the owner sheet for the type6 destination operand; reversing multiplication order is equivalent. Evidence: `port/game-data/class_tables.cpp:23`, `port/game-data/class_tables.cpp:24`.
- **PROVEN:** A resulting buff-sheet Max_HP delta is not the final actor Max_HP. RecalcProperty type&4 accumulates the base, saved, gear and set buff-sheet values. A smaller buff delta alone does not prove Toughness shrinks final HP. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:91`, `port/game-data/properties.cpp:16`.

### ApplyBuff binding and pointer branch

- **PROVEN:** Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:641`.
- **PROVEN:** A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:72`.
- **PROVEN:** The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:9`.


## Current frozen port source

**PARTIAL:** Identifier is present in production source but a complete reusable Lua callback/binding path is not proven.
- Source evidence: `port/windows-foundation/features/companions/catch_up_buff_v1.cpp`:89 — `for (std::size_t at = s.find("ApplyBuff"); at != std::string::npos; at = s.find("ApplyBuff", at + 1)) {`.
- Source evidence: `port/windows-foundation/features/companions/catch_up_buff_v1.cpp`:89 — `for (std::size_t at = s.find("ApplyBuff"); at != std::string::npos; at = s.find("ApplyBuff", at + 1)) {`.
- Source evidence: `port/windows-foundation/features/companions/catch_up_buff_v1.cpp`:91 — `std::size_t p = skip_space(s, at + std::string_view("ApplyBuff").size());`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1257 — `active.receipt.detail = "Source CreateBuff/ApplyBuff failed after the committed OnPre prefix: " + buff_error;`.

**Concrete behavioral difference:** Original ApplyBuff is registered as both global function and object method and uses the caller-supplied pointer/null/bool branches before recalculating the bound receiver. Frozen production code found here is a selected skill-owner path; it does not prove a general VM binding. The current timed-buff owner replaces matching class instances, which is not identical to the source per-class instance/capacity contract.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Type5, destination raw25600, scale raw384. Raw result38400. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c78.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c10.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:62, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:64, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2bdc.c`:6, `port/game-data/reference/classes/reference/original-functions.asm`:183, `port/game-data/class_tables.cpp`:23, `port/game-data/class_tables.cpp`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:91, `port/game-data/properties.cpp`:16.
2. **PROVEN:** Given: Type6, scale property190 contains384; owner destination raw25600; buff mode enabled. Raw buff destination38590:38400+190. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c78.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c10.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:62, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:64, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2bdc.c`:6, `port/game-data/reference/classes/reference/original-functions.asm`:183, `port/game-data/class_tables.cpp`:23, `port/game-data/class_tables.cpp`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:91, `port/game-data/properties.cpp`:16.
3. **PROVEN:** Given: Type6, same scale but owner destination raw51200. Raw buff destination76990:76800+190. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c78.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c10.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:62, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:64, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2bdc.c`:6, `port/game-data/reference/classes/reference/original-functions.asm`:183, `port/game-data/class_tables.cpp`:23, `port/game-data/class_tables.cpp`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:91, `port/game-data/properties.cpp`:16.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:45` | outside scoped roots | `object:ApplyBuff(DEBUFF_ID, buff)` | `[{"index":1,"expression":"DEBUFF_ID","literal_tokens":[]},{"index":2,"expression":"buff","literal_tokens":[]}]` |

## Related natives

`Character::_ApplyPropClass`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
