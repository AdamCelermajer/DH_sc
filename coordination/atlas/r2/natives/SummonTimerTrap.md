# SummonTimerTrap

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#62**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SummonTimerTrap` | bindFunction | `GameObject::_SummonTimerTrap` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105463 |
| `SummonTimerTrap` | bindMethod | `GameObject::_SummonTimerTrap` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105468 |

Address-identified callback: `GameObject::_SummonTimerTrap` at `0x00390c50`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`, unified lines 107829-107909.
Recovered declaration: `void __fastcall GameObject::_SummonTimerTrap( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1,2: number tag3 -> unsigned valid table indices; unit/scale TimerTraps, GameObjectDamager rows. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:30`.
- Lua argument 3..5: optional userdata orall3 numeric; unit/scale raw position. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:62`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires valid numeric TimerTraps row and numeric GameObjectDamager row. TimerTrap::Create(owner,row,damage) spawns unique TimerTrap_%04u object, requires objectkind16, fills owner/trap/damager fields, defaults receiver point, virtual initializes trap, adds owner room. Optional userdata arg3 copies point; >=5 all-numeric3..5 raw point. Wrapper pushes userdata; Create failure null is pushed only when no optional positioning dereferences null; positioning failure domain not guessed. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:49`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039d724.c:39`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:49`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039d724.c:39`.
**State, timers and events:**
- PROVEN: Trap userdata (null creation maypushnull) with trap/owner/room initialization; timers belong trap lifecycle, not wrapper. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:49`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039d724.c:39`.
**Return:**
- PROVEN: Trap userdata (null creation maypushnull) with trap/owner/room initialization; timers belong trap lifecycle, not wrapper. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:49`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039d724.c:39`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected trap services exist, but no generic scene0x390c50 callback (:144-146). Source uses reusable TimerTrap class, unique names and owner/damage rows, not delayed one-shot damage alone.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:330 — `if (!contains(delayed, "SkillCombatRoll(target)") || !contains(delayed, "SummonTimerTrap(TIMERTRAP_ID") ||`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:1071 — `log("SummonTimerTrap " + skill.script.timer_trap + " not created: " + presentation);`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:1074 — `log("SummonTimerTrap " + skill.script.timer_trap + " has no TimerTrap owner in this build");`.

**Concrete behavioral difference:** Selected trap services exist, but no generic scene0x390c50 callback (:144-146). Source uses reusable TimerTrap class, unique names and owner/damage rows, not delayed one-shot damage alone.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: invalid trap row; SummonTimerTrap(TimerTraps size,0) No trap creation or Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:43, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:49, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:50, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039d724.c`:39.
2. **PROVEN:** Given: valid rows, successful TimerTrap; SummonTimerTrap(T,D,10,20,30) New kind16 trap position(10,20,30), owner bound receiver, trap rowT/damagerD; returns userdata. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:43, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:49, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:50, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039d724.c`:39.
3. **PROVEN:** Given: valid rows successful trap; SummonTimerTrap(T,D) Trap starts at owner raw position; registered in owner room if exists. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:43, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:49, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390c50.c`:50, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039d724.c`:39.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **4** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:84` | outside scoped roots | `SummonTimerTrap(TIMERTRAP_ID, TIMERTRAP_DAM_ID, x, y, z)` | `[{"index":1,"expression":"TIMERTRAP_ID","literal_tokens":[]},{"index":2,"expression":"TIMERTRAP_DAM_ID","literal_tokens":[]},{"index":3,"expression":"x","literal_tokens":[]},{"index":4,"expression":"y","literal_tokens":[]},{"index":5,"expression":"z","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:90` | outside scoped roots | `SummonTimerTrap(TIMERTRAP_ID, TIMERTRAP_DAM_ID, x, y, z)` | `[{"index":1,"expression":"TIMERTRAP_ID","literal_tokens":[]},{"index":2,"expression":"TIMERTRAP_DAM_ID","literal_tokens":[]},{"index":3,"expression":"x","literal_tokens":[]},{"index":4,"expression":"y","literal_tokens":[]},{"index":5,"expression":"z","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:84` | act1_level_boss_script_closure | `SummonTimerTrap(TIMERTRAP_ID, TIMERTRAP_DAM_ID, x, y, z)` | `[{"index":1,"expression":"TIMERTRAP_ID","literal_tokens":[]},{"index":2,"expression":"TIMERTRAP_DAM_ID","literal_tokens":[]},{"index":3,"expression":"x","literal_tokens":[]},{"index":4,"expression":"y","literal_tokens":[]},{"index":5,"expression":"z","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:90` | act1_level_boss_script_closure | `SummonTimerTrap(TIMERTRAP_ID, TIMERTRAP_DAM_ID, x, y, z)` | `[{"index":1,"expression":"TIMERTRAP_ID","literal_tokens":[]},{"index":2,"expression":"TIMERTRAP_DAM_ID","literal_tokens":[]},{"index":3,"expression":"x","literal_tokens":[]},{"index":4,"expression":"y","literal_tokens":[]},{"index":5,"expression":"z","literal_tokens":[]}]` |

## Related natives

`GameObject::_SummonTimerTrap`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
