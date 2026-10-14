# player:RemoveBuff

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#220**.

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

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:173` | outside scoped roots | `player:RemoveBuff(DEBUFF_ID)` | `[{"index":1,"expression":"DEBUFF_ID","literal_tokens":[]}]` |

## Related natives

`Character::_RemoveBuff`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
