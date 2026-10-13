# PlayActorAnim

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#162**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `PlayActorAnim` | bindFunction | `Character::_PlayAnim` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132681 |
| `PlayActorAnim` | bindMethod | `Character::_PlayAnim` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132686 |

Address-identified callback: `Character::_PlayAnim` at `0x003ba388`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`, unified lines 135690-135822.
Recovered declaration: `unsigned int __fastcall Character::_PlayAnim( unsigned int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: number tag3 -> unsigned valid AnimDict ID; unit/scale animation dictionary row. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:42`.
- Lua argument 2: optional numeric secondary clip; unit/scale dictionary row; absent-1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:110`.
- Lua argument 3: optional Boolean; unit/scale anim state flag defaultfalse. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:121`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires numeric dictionary ID within AnimDict size. Static SLOT advances cyclically over5 reusable AnimTable pairs: old0..3 selects one-shot IDs16..19 and secondary21..24; fifth resets to pair15/20. First table must2 steps, second1 (assertions). First clip ID=arg1; optional numeric arg2 secondary clip, otherwise secondary-1. Optional Boolean arg3 passed to SM_SetAnimState; defaultfalse. FSM stores anim ID/flags and direct _SetState14 (no event ID), independent controller gates. Global temporary table slots shared across receivers. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:53`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c1aa8.c:3`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:53`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c1aa8.c:3`.
**State, timers and events:**
- PROVEN: No Lua value; global table slot/clip fields and direct FSM14 update, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:53`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c1aa8.c:3`.
**Return:**
- PROVEN: No Lua value; global table slot/clip fields and direct FSM14 update, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:53`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c1aa8.c:3`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected actor animation player uses recovered animator, but general scene0x3ba388 callback missing (:144-146). Preserve global slot cycling, optional second clip and forced anim state; one asset play API lacks these semantics.
- Source evidence: `port/windows-foundation/main.cpp`:3436 — `std::cout<<"[campaign] PlayActorAnim: actor "<<id<<" has no live instance; clip "<<clip<<" is a no-op\n";`.
- Source evidence: `port/windows-foundation/features/campaign_host/campaign_host.cpp`:615 — `std::cout << "[campaign] PlayActorAnim actor=" << id << " clip=" << signed_field(c, 8) << " duration_ms=" << duration`.
- Source evidence: `port/windows-foundation/features/cinematics/runtime_troll_return_v1.cpp`:96 — `throw std::runtime_error("TrollReturn PlayActorAnim record lacks source animation/wait/receiver fields");`.
- Source evidence: `port/windows-foundation/features/cinematics/runtime_troll_return_v1.cpp`:101 — `if(*actorName!=next.actor_name)throw std::runtime_error("TrollReturn PlayActorAnim receiver changes within authored chain");`.
- Source evidence: `port/windows-foundation/features/companions/source_companion_animation_bank_v1.cpp`:77 — `throw std::runtime_error("PlayActorAnim receiver missing from loaded campaign source");`.
- Source evidence: `port/windows-foundation/features/companions/source_companion_animation_bank_v1.cpp`:90 — `item.unsupported_reason = "PlayActorAnim fields are incomplete or invalid";`.
- Source evidence: `port/windows-foundation/features/companions/source_companion_animation_bank_v1.cpp`:141 — `throw std::runtime_error("Requested script has no PlayActorAnim records for supplied source actor owners");`.
- Source evidence: `port/windows-foundation/features/companions/source_companion_campaign_animation_v1.cpp`:63 — `throw std::runtime_error("PlayActorAnim receiver field is missing from loaded campaign script");`.
- Source evidence: `port/windows-foundation/features/companions/source_companion_campaign_animation_v1.cpp`:75 — `need.unsupported_reason = "PlayActorAnim Anim/Next/Slot/Wait fields are incomplete or invalid";`.
- Source evidence: `port/windows-foundation/features/companions/source_companion_campaign_animation_v1.cpp`:133 — `throw std::runtime_error("Requested campaign script has no PlayActorAnim records for supplied companion actors");`.

**Concrete behavioral difference:** Selected actor animation player uses recovered animator, but general scene0x3ba388 callback missing (:144-146). Preserve global slot cycling, optional second clip and forced anim state; one asset play API lacks these semantics.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: PlayAnim(AnimDict size) No slot/state mutation; index equals size. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:38, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:53, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:55, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c1aa8.c`:3.
2. **PROVEN:** Given: valid E, first call at SLOT0; PlayAnim(E) Uses AnimTable16/21, first clipE/second-1; SetAnimState14 Booleanfalse. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:38, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:53, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:55, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c1aa8.c`:3.
3. **PROVEN:** Given: valid E; PlayAnim(E,F,true) Primary clipE, secondaryF, FSM anim flagtrue; direct anim state14. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:38, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:53, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003ba388.c`:55, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c1aa8.c`:3.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:137` | outside scoped roots | `PlayActorAnim(GetPyOID("AnimDict", "moth_cocoon_open"))` | `[{"index":1,"expression":"GetPyOID(\"AnimDict\", \"moth_cocoon_open\")","literal_tokens":["\"AnimDict\"","\"moth_cocoon_open\""]}]` |

## Related natives

`Character::_PlayAnim`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
