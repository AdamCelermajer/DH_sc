# HasBow

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#97**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `HasBow` | bindFunction | `Character::_HasBow` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:133014 |
| `HasBow` | bindMethod | `Character::_HasBow` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:133019 |

Address-identified callback: `Character::_HasBow` at `0x003b6f84`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`, unified lines 133360-133371.
Recovered declaration: `void __fastcall Character::_HasBow( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c:5`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Calls inventory HasBow at receiver+892; selected main-hand equip set slot1 must exist and its item weapon-type field +148 equal4. Missing main hand returns false. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c:5`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c:5`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c:5`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c:5`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** Skill-native supported callback list omits0x3b6f84 (character_skill_native_v5.cpp:12); inventory stance code detects bow but does not bind HasBow. Original supports direct arbitrary Lua getter.

**Concrete behavioral difference:** Skill-native supported callback list omits0x3b6f84 (character_skill_native_v5.cpp:12); inventory stance code detects bow but does not bind HasBow. Original supports direct arbitrary Lua getter.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: main-hand slot missing; HasBow() False. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c`:5.
2. **PROVEN:** Given: equipped main-hand item weapon type4; HasBow() True. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c`:5.
3. **PROVEN:** Given: main-hand weapon type5; HasBow() False. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f84.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00400080.c`:5.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **8** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:83` | player_skill_lua | `HasBow()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:55` | player_skill_lua | `HasBow()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:80` | player_skill_lua | `HasBow()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:44` | player_skill_lua | `HasBow()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:19` | player_skill_lua | `HasBow()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:30` | player_skill_lua | `HasBow()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:64` | player_skill_lua | `HasBow()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:107` | player_skill_lua | `HasBow()` | `[]` |

## Related natives

`Character::_HasBow`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
