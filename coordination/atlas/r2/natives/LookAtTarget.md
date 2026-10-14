# LookAtTarget

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#47**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `LookAtTarget` | bindFunction | `Character::_LookAtTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132487 |
| `LookAtTarget` | bindMethod | `Character::_LookAtTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132492 |

Address-identified callback: `Character::_LookAtTarget` at `0x003b5684`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c`, unified lines 132238-132246.
Recovered declaration: `float __fastcall Character::_LookAtTarget( Character *this, const sfc::script::lua::Arguments *a2, GameObject **a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393d48.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393cec.c:9`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: No Lua args inspected; calls GameObject::LookAt(receiver,current target+1032) directly, bypassing controller lock/block gate. Null target no-op; nonnull uses its GetTargetPosition then LookTowards(targetPosition-ownerPosition). Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393d48.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393cec.c:9`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393d48.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393cec.c:9`.
**State, timers and events:**
- PROVEN: No Lua value; direct orientation update when target nonnull, no wrapper timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393d48.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393cec.c:9`.
**Return:**
- PROVEN: No Lua value; direct orientation update when target nonnull, no wrapper timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393d48.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393cec.c:9`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** No0x3b5684 branch in character_skill_native_v5 supported list (:12); existing LookAt goes through controller and cannot substitute direct target-facing semantics.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:173 — `next.close_range_looks_at_target = contains(close, "LookAtTarget()");`.

**Concrete behavioral difference:** No0x3b5684 branch in character_skill_native_v5 supported list (:12); existing LookAt goes through controller and cannot substitute direct target-facing semantics.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: current target null; LookAtTarget() No orientation operation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393d48.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393cec.c`:9.
2. **PROVEN:** Given: current target nonnull, controller locked; LookAtTarget() Direct GameObject look operation still invoked; wrapper does not inspect controller lock. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5684.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393d48.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00393cec.c`:9.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **6** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:79` | outside scoped roots | `LookAtTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:122` | outside scoped roots | `LookAtTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:32` | outside scoped roots | `LookAtTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:214` | act1_level_boss_script_closure | `LookAtTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:191` | act1_level_boss_script_closure | `LookAtTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:158` | outside scoped roots | `LookAtTarget()` | `[]` |

## Related natives

`Character::_LookAtTarget`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
