# Remove

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#88**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `Remove` | bindFunction | `TriggerTrap::_Remove` | `_ZN11TriggerTrap14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:116531 |
| `Remove` | bindMethod | `TriggerTrap::_Remove` | `_ZN11TriggerTrap14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:116536 |

Address-identified callback: `TriggerTrap::_Remove` at `0x0039dd18`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`, unified lines 116490-116508.
Recovered declaration: `int __fastcall TriggerTrap::_Remove(int a1, int a2, int a3)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:17`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Bound TriggerTrap sets byte+1024 removal requested. If visual subobject+728 absent or visual script Call("remove",0) returns0, invokes receiver virtual+64(0) then ObjectBase::Delete. If script handles remove (nonzero), leaves object alive for visual/script lifecycle. No Lua value. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:17`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:17`.
**State, timers and events:**
- PROVEN: No Lua result; removal flag plus conditional visual call/virtual cleanup/delete; no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:17`.
**Return:**
- PROVEN: No Lua result; removal flag plus conditional visual call/virtual cleanup/delete; no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c:17`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** General scene Character dispatcher does not bind TriggerTrap callback; selected object removal services may retire directly, missing the visual-script veto/deferred lifecycle branch.
- Source evidence: `port/windows-foundation/features/bosses/runtime_timer_trap_v1.cpp`:96 — `if (!std::regex_search(text, m, std::regex("event\\s*==\\s*\"(\\w+)\"[\\s\\S]*?g_counter\\s*>=\\s*COUNTER_DELAY[\\s\\S]*?Remove\\(\\)")))`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_timer_trap_v1.cpp`:97 — `return fail(error, "trap script has no counted Remove()");`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_timer_trap_v1.cpp`:173 — `log_.push_back("TimerTrap_" + std::to_string(t.id) + " Remove() after " + std::to_string(t.activations) + " activations");`.
- Source evidence: `port/windows-foundation/features/frontend/flow/menu_flow.cpp`:91 — `if(top()!="menu_MainMenu"||erase_confirmation_)return fail(error,"Remove confirmation is unavailable");`.
- Source evidence: `port/windows-foundation/features/frontend/flow/menu_flow.cpp`:92 — `if(!selected_slot_.in_use||selected_slot_.id<0||selected_slot_.save_path.empty())return fail(error,"Remove requires the exact occupied selected profile");`.
- Source evidence: `port/level-world/canonical_object_manager_v1.cpp`:166 — `if(!resolve(a))return fail();if(a.identity){if(!a.type_f4){e="Required SAME type_f4 for source Remove";return fail();}if(*a.type_f4==5)remove_all(modules_,a.identity);}`.
- Source evidence: `port/level-world/canonical_object_manager_v1.cpp`:183 — `if(!game){e="Original Remove reaches NULL GameObject2fc read after count50 decrement; unsupported safe native domain";return fail();}`.
- Source evidence: `port/level-world/canonical_object_manager_v1.cpp`:190 — `}catch(const std::exception& ex){e=ex.what();return fail();}catch(...){e="ObjectManager Remove native leaf threw; prefix retained";return fail();}`.
- Source evidence: `port/level-world/canonical_object_update_v102.cpp`:41 — `if(!s.remove||!s.remove(a,fake,e))return required(fake?"FakeRemove":"Remove");i=f.deletion3c->erase(i);continue;`.
- Source evidence: `port/level-world/physical_world.cpp`:164 — `void NativeWorld::Remove(const b2ContactPoint* p){dispatch(ContactEvent::remove,p);}`.
- Source evidence: `port/level-world/physical_world.hpp`:48 — `void Remove(const b2ContactPoint*) override;`.

**Concrete behavioral difference:** General scene Character dispatcher does not bind TriggerTrap callback; selected object removal services may retire directly, missing the visual-script veto/deferred lifecycle branch.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: trap no visual subobject; Remove() Removal byte1, virtual cleanup then object delete. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:17.
2. **PROVEN:** Given: trap visual remove call returns1; Remove() Removal byte1; no immediate cleanup/delete. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:17.
3. **PROVEN:** Given: trap visual remove call returns0; Remove() Cleanup/delete occurs. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:8, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039dd18.c`:17.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/objects/swampking_trap.luac:24` | outside scoped roots | `Remove()` | `[]` |

## Related natives

`TriggerTrap::_Remove`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
