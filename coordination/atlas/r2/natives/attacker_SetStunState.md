# attacker:SetStunState

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#123**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SetStunState` | bindFunction | `Character::_SetStunState` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132726 |
| `SetStunState` | bindMethod | `Character::_SetStunState` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132731 |

Address-identified callback: `Character::_SetStunState` at `0x003b9580`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9580.c`, unified lines 135110-135170.
Recovered declaration: `int __fastcall Character::_SetStunState( int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
No explicit operator[] index was extracted. This does not prove a zero-argument callback.

## Recovered behavior, gates, state, timers, events and return

- **PROVEN:** SM_SetStunState 0x3c5ffc rejects bosses first. It obtains GetCharAnimTableId 0x3a3228 (invalid authored ID falls back to17), then validates the returned row against CharAnimTable size. It reads the Stunned field at byte140 of the160-byte native row, not an arbitrary hardcoded animation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a3228.c:7`, `port/level-world/reference/character-native-effects/NOTES.md:24`.
- **PROVEN:** Only when pending bit2 is absent does it start timer43 for the supplied unsigned integer milliseconds; reapplying while pending does not restart that timer. The timer response is ignored by this source caller, then pending bit2 is set. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:17`.
- **PROVEN:** It queries AnimStancedAnim.SL__LIST_IPHONE mask0x200. If present, GetAnimStance is added to the base Stunned sequence; otherwise modifier0. The selected sequence is stored at machine+40. Force enters state9 through _SetState; otherwise event50012 is raised. Requested mode adds flag0x800 after that transition/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:34`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

### CharAnimTable stun lookup and admission

- **PROVEN:** SM_SetStunState 0x3c5ffc rejects bosses first. It obtains GetCharAnimTableId 0x3a3228 (invalid authored ID falls back to17), then validates the returned row against CharAnimTable size. It reads the Stunned field at byte140 of the160-byte native row, not an arbitrary hardcoded animation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a3228.c:7`, `port/level-world/reference/character-native-effects/NOTES.md:24`.
- **PROVEN:** Only when pending bit2 is absent does it start timer43 for the supplied unsigned integer milliseconds; reapplying while pending does not restart that timer. The timer response is ignored by this source caller, then pending bit2 is set. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:17`.
- **PROVEN:** It queries AnimStancedAnim.SL__LIST_IPHONE mask0x200. If present, GetAnimStance is added to the base Stunned sequence; otherwise modifier0. The selected sequence is stored at machine+40. Force enters state9 through _SetState; otherwise event50012 is raised. Requested mode adds flag0x800 after that transition/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:34`.


## Current frozen port source

**MISSING:** No exact production-source identifier hit or curated callback/binding proof was found.

**Concrete behavioral difference:** Original SM_SetStunState performs boss/animation-row/timer/state-event gates. The frozen port routes selected effects through native-effect/FSM services; this does not establish general Lua callback parity or exact source admission for every actor.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Boss actor, force=true, duration3000. No stun timer, override or state transition is performed. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a3228.c`:7, `port/level-world/reference/character-native-effects/NOTES.md`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:34.
2. **PROVEN:** Given: Nonboss valid row, Stunned sequence50, stance7, stanced mask0x200, pending bit clear. Override57; timer43 starts once with3000ms; normal mode raises50012. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a3228.c`:7, `port/level-world/reference/character-native-effects/NOTES.md`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:34.
3. **PROVEN:** Given: Same actor receives another stun while pending bit2 is already set. Existing stun timer is retained; animation override/event selection still executes. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a3228.c`:7, `port/level-world/reference/character-native-effects/NOTES.md`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:34.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **2** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:34` | player_skill_lua | `attacker:SetStunState(DURATION,true)` | `[{"index":1,"expression":"DURATION","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:113` | player_skill_lua | `attacker:SetStunState(3000, true)` | `[{"index":1,"expression":"3000","literal_tokens":["3000"]},{"index":2,"expression":"true","literal_tokens":[]}]` |

## Related natives

`Character::_SetStunState`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
