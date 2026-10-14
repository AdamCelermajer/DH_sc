# object:CreateBuff

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#212**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `CreateBuff` | bindFunction | `Character::_CreateBuff` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132870 |
| `CreateBuff` | bindMethod | `Character::_CreateBuff` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132875 |

Address-identified callback: `Character::_CreateBuff` at `0x003b86a8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b86a8.c`, unified lines 134439-134603.
Recovered declaration: `void __fastcall Character::_CreateBuff( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getter not adjacent in decompilation; handler lines 134474.

## Recovered behavior, gates, state, timers, events and return

- **PROVEN:** PROPS_AddBuff 0x3e232c maps capacity<=0 to128. Replacement/reuse scans run only when current count equals requested capacity. Lower unsigned strength entries qualify and their strength is immediately rewritten; equal-strength permanent entries qualify; timed ties compare elapsed time through TMR_TimeLeft. This is not a general FIFO eviction policy. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:85`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:112`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003db344.c:14`, `port/level-world/reference/character-buffs/NOTES.md:33`.
- **PROVEN:** At full capacity, if no entry qualifies, allocation is refused and nullptr returned. A qualifying entry is reused after stopping its prior timer; nonzero duration starts a one-shot event54 timer, zero duration is permanent. ResetSheet initializes defaults before SnS_Level receives strength<<8. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:178`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:200`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:238`.
- **PROVEN:** Combination depends on each property type from AAA_TYPES_DONT_DELETE, not a universal buff-sum rule. Type&4 accumulates; type&2 takes base then saved then gear, then first ascending declaration with a set value (last set instance in that group); type&1 favors the highest descending declaration group, then gear/saved/base. Type&32 uses base+saved; type&16 uses base. Defaults identify unset values. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:181`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:300`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003deed8.c:7`, `port/game-data/properties.cpp:15`.
- **PROVEN:** The native buff kernel and property resolver are recovered, but Windows RuntimeTimedBuffOwnerV1 currently replaces every matching class instance on application and exposes no arbitrary native capacity selection. Its source is a partial production adaptation of the original buff-map contract. Evidence: `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.cpp:162`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

### Buff capacity, reuse and per-property combination

- **PROVEN:** PROPS_AddBuff 0x3e232c maps capacity<=0 to128. Replacement/reuse scans run only when current count equals requested capacity. Lower unsigned strength entries qualify and their strength is immediately rewritten; equal-strength permanent entries qualify; timed ties compare elapsed time through TMR_TimeLeft. This is not a general FIFO eviction policy. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:85`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:112`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003db344.c:14`, `port/level-world/reference/character-buffs/NOTES.md:33`.
- **PROVEN:** At full capacity, if no entry qualifies, allocation is refused and nullptr returned. A qualifying entry is reused after stopping its prior timer; nonzero duration starts a one-shot event54 timer, zero duration is permanent. ResetSheet initializes defaults before SnS_Level receives strength<<8. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:178`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:200`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:238`.
- **PROVEN:** Combination depends on each property type from AAA_TYPES_DONT_DELETE, not a universal buff-sum rule. Type&4 accumulates; type&2 takes base then saved then gear, then first ascending declaration with a set value (last set instance in that group); type&1 favors the highest descending declaration group, then gear/saved/base. Type&32 uses base+saved; type&16 uses base. Defaults identify unset values. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:181`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:300`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003deed8.c:7`, `port/game-data/properties.cpp:15`.
- **PROVEN:** The native buff kernel and property resolver are recovered, but Windows RuntimeTimedBuffOwnerV1 currently replaces every matching class instance on application and exposes no arbitrary native capacity selection. Its source is a partial production adaptation of the original buff-map contract. Evidence: `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.cpp:162`.


## Current frozen port source

**PARTIAL:** Identifier is present in production source but a complete reusable Lua callback/binding path is not proven.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1257 — `active.receipt.detail = "Source CreateBuff/ApplyBuff failed after the committed OnPre prefix: " + buff_error;`.

**Concrete behavioral difference:** Original Character::_CreateBuff creates/reset sheets and schedules a timer for nonzero duration; no Idle admission test appears in this body. Frozen production source exposes selected timed-buff owner paths, not the full original arbitrary-capacity Lua constructor path.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Capacity1; existing strength5; request strength4. No eligible reusable instance; AddBuff returns null instead of evicting the stronger instance. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:85, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:112, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003db344.c`:14, `port/level-world/reference/character-buffs/NOTES.md`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:178, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:200, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:238, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:181, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:300, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003deed8.c`:7, `port/game-data/properties.cpp`:15, `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.cpp`:162.
2. **PROVEN:** Given: Capacity1; one permanent strength5; request strength5 duration1000. Same instance is reused, old timer is stopped, defaults reset, then a new event54 timer starts. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:85, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:112, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003db344.c`:14, `port/level-world/reference/character-buffs/NOTES.md`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:178, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:200, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:238, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:181, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:300, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003deed8.c`:7, `port/game-data/properties.cpp`:15, `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.cpp`:162.
3. **PROVEN:** Given: Type&4 property default0, base100*256, two set buff values10*256 and20*256. Resolved value130*256; for a nonaccumulating type this expectation is not applicable. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:85, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:112, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003db344.c`:14, `port/level-world/reference/character-buffs/NOTES.md`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:178, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:200, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c`:238, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:181, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c`:300, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003deed8.c`:7, `port/game-data/properties.cpp`:15, `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.cpp`:162.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:44` | outside scoped roots | `object:CreateBuff(DEBUFF_ID, DEBUFF_DELAY, false, 0, VIP_BUFF_FX)` | `[{"index":1,"expression":"DEBUFF_ID","literal_tokens":[]},{"index":2,"expression":"DEBUFF_DELAY","literal_tokens":[]},{"index":3,"expression":"false","literal_tokens":[]},{"index":4,"expression":"0","literal_tokens":["0"]},{"index":5,"expression":"VIP_BUFF_FX","literal_tokens":[]}]` |

## Related natives

`Character::_CreateBuff`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
