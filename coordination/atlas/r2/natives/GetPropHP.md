# GetPropHP

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

Engine functions to implement next ranking: **#33**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetPropHP` | bindFunction and/or Method (see registration ledger) | `Character::_GetPropHP` | `Character::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:650 |

Address-identified callback: `Character::_GetPropHP` at `0x003b6cd8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c`, unified lines 133152-133178.
Recovered declaration: `void __fastcall Character::_GetPropHP( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN:** No Lua arguments are inspected by this audited callback; extra arguments are ignored. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c:1`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Reads cached raw HP property 36 and max HP property 38. For nonzero max raw: hp>>8, max>>8, then (100*rawHP/(max>>8))>>8. Does not clamp percentage. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c:18`.
**Gates:**
- PROVEN: Raw max zero returns three zeros. Nonzero max with integer max zero reaches original division-by-zero behavior; overflow is 32-bit signed native arithmetic. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c:18`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c:1`.
**Return:**
- PROVEN: Three ordered Lua integers: whole HP, whole maximum, percentage.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** Reusable frozen callback preserves ordinary HP/max and exposes source arithmetic failure boundaries; source review does not prove integrated gameplay.
- Source evidence: `port/level-world/character_script_session.cpp`:238.

**Port comparison:** Reusable frozen callback preserves ordinary HP/max and exposes source arithmetic failure boundaries; source review does not prove integrated gameplay.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: GetPropHP() with raw HP=12800 and raw max=25600 Returns 50,100,50. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c`:18.
2. **PROVEN:** Given: GetHP() with max raw zero Returns 0,0,0. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c`:22.
3. **PROVEN:** Given: HP=38400 and max=25600 Returns 150,100,150; no clamp. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6cd8.c`:18.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **16** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:7` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:15` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:378` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:152` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:265` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:273` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:276` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:101` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:112` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:397` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:412` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:334` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:637` | act1_level_boss_script_closure | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:68` | outside scoped roots | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:72` | player_skill_lua | `GetPropHP()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:125` | player_skill_lua | `GetPropHP()` | `[]` |

## Related natives

`Character::_GetPropHP`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
