# Retained monster VM update and state callbacks

## Exact original caller facts

`AISExternal::OnUpdate` at `0x3dce64/64` first executes the maintained AISDefault update. It then tests live `+0xb8` bit 0 and calls `LuaScript::Call("OnUpdate")` only when that bit is set, followed by independent `CallStateUpdate` and `CallStateConditions`. Existing `ais_external_update` owns that full orchestration and counter prefix. This extension supplies its actual Session callback dependencies, not a second implementation of the whole AI frame.

`LuaScript::Call(char const*)` at `0x37c514/112` constructs ReturnValues, calls the no-Arguments overload `0x37c494`, then destroys ReturnValues. It passes **zero explicit Lua arguments**. Thus the original `_commons.luac` declaration `OnUpdate(timestamp)` receives nil. The comment about time does not produce a timestamp. No elapsed time or fake argument is added.

`CharAIScript::CallStateUpdate` at `0x3d8eb4/20` reads live `AIS+0xb4`, returns if null, otherwise loads the owned name at state `+0x14` and tail-calls the same LuaScript Call. `CallStateConditions` at `0x3d8ea0/20` separately repeats the pointer read and takes name `+0x2c`. Their original ARM instructions run in the differential gate. An update callback may replace/clear the current state or mutate names; the next condition call reads the new projection. Both use the source VFTable alias resolution with one lookup, then the real same-VM discarded-return path. Returned table `_this` lookups may mutate the alias map before the next callback and are preserved.

## Genuine null-state ownership

The constructor evidence is explicit: CharAIScript C1 `0x3d8f44/108` stores zero at `+0xb4` at `0x3d8f88`; C2 `0x3d8fb0/108` stores it at `0x3d8ff4`. The AISExternal constructors call that base constructor and do not subsequently write `+0xb4`. The maintained constructor projection already exposes `current_state_b4`. The unchanged plain monster and common files neither call `RegisterAIState` nor `ChangeAIState`.

`CharAIScript::_ChangeState` at `0x3d9710/128` resolves a retained registry entry, executes old-state Post, stores its callback record at `+0xb4` (`0x3d9780`), then executes new-state Init. State registry construction, this transition body, and its live ownership are dependency evidence here, not reconstructed by this extension. The native owner must derive `CurrentState.active` from its actual constructor/registry producer and retain old/new state entries/name storage. A null branch must not be inferred from missing methods or a development state ID.

## Session API and failures

`Event::update` invokes the current `OnUpdate` alias with zero arguments. The plain monster has no OnUpdate VFTable entry; fallback calls the actual original common function if the external update gate requests it. This Session entry does not itself set the source override flag or bypass its gate.

`call_state_update` and `call_state_conditions` reuse `ais_state_callbacks::invoke` with its retained logical State/Table projections. Public `CurrentState` and `StateCallbacks` are aliases of those existing types; fields are `State.ais`, `State.active`, and `Table.identity/update/conditions/init/post`. Neither structure overlays original ARM memory. The reused source kernel captures the current pointer and selected name once. A real null pointer skips Lua and leaves callback counters unchanged; a nonnull record dispatches its selected name. An invalid/null control, zero AIS identity, misaligned record, or null name is rejected explicitly. The original helpers have no null-name guard; the native null-name rejection is an invalid-source boundary, not a recovered source guard. An empty/missing function or an invoked missing native provider fails through the real VM, without fallback or a successful no-op.

Session operations are synchronous on one owning thread. CurrentState, state entries, old/new names, service context, actor objects, and the VM remain alive until return. No same-session reentry or destruction is allowed. The existing dispatch policy faults the Session after a Lua callback error, preserves effects, and requires owner teardown/replacement rather than replay. It is a bounded port failure policy, not a claim that original void LuaScript Call propagates an error identically. Independent state calls reload the projection; no combined method caches it across update/conditions.

## Exact resolved-path loading

`load_resolved(exact_path, Source, LoadResult*, error)` composes the frozen `lua_script_load_once::State` with this same VM pointer and a real `dh2_script_vm_load` service. It begins **after** original LuaManager directory/extension resolution. The VM owns the separate cache projection; a successful VM replacement gets a fresh path set.

A hit returns success before touching supplied bytes/loading. A miss executes retained bytes using the exact key as chunk name, records only normal zero status, and retains prior Lua/provider effects on error. Syntax/runtime/provider load errors preserve real VM error text and remain retryable; unlike dispatch errors, a failed load does not fault an otherwise ready Session. Exact keys are not normalized or case-folded. Source callback errors are never converted to successful loads.

Legacy common/external stage methods retain their existing semantics and labels. Those labels alone do not prove the original fully resolved cache key, so they are not silently inserted into this new set. Parent source path resolution may explicitly use `load_resolved` for correctly resolved additional/skill files on the retained VM. This is not a full LuaManager/AddFile/file-cache reconstruction. An internal authored identity transport chunk remains separate from original file-cache accounting.

## Verification and scope

The dedicated test includes the unchanged legacy fixture with its main renamed and reruns all32 prior cases, then runs real float32 Lua over unchanged original common/monster bytes. Additional clearly authored fixture suffixes exercise zero arity, fresh VFTable alias effects from discarded returns, independent state mutations, missing functions/providers, exceptions, reentry, real exact-key cache hits/misses, retry after partial effects, and cache replacement with the VM. The runner hashes that included fixture and every compiled source/header.

Six existing host runners that compile this Session now link and hash the reused `lua_script_load_once` and `ais_state_callbacks` implementations/headers: external Session, Ghost ActorSession, native Ghost owner, monster initialization, positive initialization/debug persistence, and Character level runtime. No shared legacy C++ fixture, CMake, or native renderer is changed by this extension.

The original differential runs both complete20B state helpers with Lua Call as a named mutation boundary; it supplies no new original Lua Call body credit. Both helper addresses are already maintained in `ais_state_callbacks` and reused here; this extension adds zero new complete original caller bodies. Count each original function only once in the project ledger. No new whole AISExternal OnUpdate, Lua callback source body, state registry/transition, filesystem/path resolver, or whole Lua VM reconstruction is claimed. Production CMake/native integration belongs to the parent.
