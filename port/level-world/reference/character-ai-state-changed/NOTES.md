# Source state-change and animation-end delivery

The new helpers recover the complete CharAI active-AIS relays and the selected inherited default bodies. The External animation-end path executes its zero-argument Lua callback rather than suppressing it.

## Source and selected tables

`original-functions.json` captures seven routines from original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

* CharAI::OnStateChanged 0x3d0bec, 36 bytes, SHA256 `6b77a2b9a1797ffad40e31858b3ad939c8e8f12347408a908ac79ee44eeae330`: load active +1c once; return if null; capture its vtable and call +20 with r1/r2 unchanged. There are no owner, pause, controller or availability tests here.
* AISDefault::OnStateChanged 0x3dbe8c is exactly ARM `bx lr`, bytes `1eff2fe1`.
* CharAI::OnEndOfAnim 0x3d0ce8, 36 bytes: same active gate, then virtual +98, no semantic arguments.
* AISDefault::OnEndOfAnim 0x3dbeec is exactly `bx lr`.
* AISExternal::OnEndOfAnim 0x3dccd0, 36 bytes including its literal word: call the Default empty body, then tail-call LuaScript::Call 0x37c514 with the literal `OnEndOfAnim`. It does not read +b8 availability flags and has no arguments.
* LuaScript::Call(const char*) 0x37c514 constructs ReturnValues, calls the overload at 0x37c494, and destroys ReturnValues. The overload freshly invokes GetVirtualFunction 0x37c314, then private Instance::call 0x31ab14; its returned values are projected/discarded. The source diagnostic call counter is incremented at +37c500.

`vtables.json` reads actual static table bytes and retains the first 51 source slot keys used by AIEventState64; the whole 216-byte table SHA is bound, including its additional entry outside this projection.

| Selected class | Static table | +20 | +98 |
|---|---|---|---|
| AISDefault | 0x966970 | Default 3dbe8c | Default 3dbeec |
| AISExternal | 0x966a48 | Default 3dbe8c | External 3dccd0 |
| AISPlayerIPhone | 0x966cd0 | Default 3dbe8c | Default 3dbeec |

External +98 is **not** inherited empty behavior. The actual `_commons.luac` defines an empty Lua `OnEndOfAnim`; invoking that real function still matters because an alias or subsequent script definition can replace it. Other nonempty subclasses remain provider boundaries.

## Native ABI and composition

`dh2_character_ai_state_changed(AIEventState64*, int32 new, int32 previous, services)` captures active and the projected +20 function key before delivery. The exact Default function key is executed as its genuine native empty body. An unknown nonzero key requires `ai_event_ais_virtual` availability and an invoke provider. It never silently succeeds for an unavailable nonempty callee.

Its request uses service `ai_event_ais_virtual`, operation 0x20, event metadata 0x1d, captured subject/callee, payload `uint32(new)` and argument `uint32(previous)`. Both signed source words retain their raw bits, zero-extended only to fit the request's native64 payload field. Callback result words are ignored as in the original void helper. Synchronous delivery/reentry is allowed; callbacks may mutate the live state, but must retain the captured receiver/table/services through the call.

`dh2_character_ai_end_anim` similarly captures active/+98. It handles Default 3dbeec locally and requires delivery for External/unknown bodies. Its request operation is 0x98, with event/payload/argument zero: this helper has no Character-event argument. The outer dispatcher retains whether event22 or23 caused it.

Both relays return 0 complete, 1 malformed native projection, 2 unavailable provider, or 3 provider failure. Null active returns before requiring an AIS table/provider. Native alignment/reserved/AI-identity validation is an explicit contract; original crash-domain parity is not claimed. Provider failure preserves already-delivered callback effects. The caller must project coherent active identity and the corresponding 51-slot table; this stage does not allocate/publish AIS objects.

`dh2_character_ais_external_end_anim(TargetScript16*,TargetServices16*)` is the exact External endpoint: zero-argument `OnEndOfAnim`, no flag gate. A source script-call provider is required; failure is observable.

`character_ais_external_end_anim(ScriptOwner&,captured_identity,scope=nullptr)` in the separate VM file resolves the retained captured session with `find`, requires External kind, and uses that session's private VM and alias map. It does not re-read active. Alias resolution and Lua global lookup are fresh for every call. An optional scoped capability must belong to that same VM and be currently valid; generic same-VM reentry remains prohibited. The helper returns existing private-runtime diagnostics without reclassifying errors as source success. The owner, session, VM and alias backing must survive callbacks/finalizers.

Integration for a +98 request whose callee is 0x3dccd0: call the VM helper with `request.subject`, retaining the original ScriptOwner and any current same-VM callback scope. For an actual Default key, use the relay's local empty path. Do not invent a Default result for other function keys.

## Proof and limits

`character-ai-state-changed-arm64-differential.json` records 1578 actual-original/O2 ARM64 comparisons and 752 ordered callback observations, zero mismatches. The oracle executes complete CharAI relays, the actual default bodies and the actual External wrapper. It checks all three selected static +20 identities, nine raw argument words including signed extremes, active/null states, captured identity/table changes, nested reentry to default and nonempty providers, full OnEnd relay through External, and 258 availability masks. Nonempty arbitrary subclass virtuals and LuaScript::Call are explicit synchronous services; Call/allocator/private-Lua instruction parity is not claimed by this new oracle. Complete Call bodies are captured for source inspection.

`character-ai-state-changed-host-audit.json` replays the original gold with 25641 checks, 1578 cases, 754 callbacks (including provider-failure guards), 163 nested invocations and 13 guards. A second audit records 737 checks over real retained ScriptOwner/private Lua and actual 13535-byte commons source. It executes the authored empty endpoint, changes aliases and Lua globals, observes discarded-return table projection, preserves error-side effects and stack depth, tests expired capabilities/unknown identities/nonExternal sessions, and performs two scoped nested deliveries after active was cleared. Callback registration, cache delivery, owner classification/budget and timer context use the pre-existing explicitly labelled host fixture; the gameplay namespace is not fabricated.

Both sanitizer executables pass AddressSanitizer, UndefinedBehaviorSanitizer and LeakSanitizer with zero findings. New helpers are compiled in isolated executables. Existing world/runtime DSOs are dependencies for genuine owned ScriptOwner/VM execution; their bytes are hashed before/after. This does not claim the new helpers already execute inside the main DSO, an APK or a complete FSM/live scene.

The source diagnostic global Lua call counter is not produced here. Lua aliases/private ReturnValues behavior reuse existing native source-runtime machinery and its separate proofs under `port/script-runtime/reference/function-alias` and `callback-scope`. Whole Application registration/AIS lifecycle and nonempty other subclasses remain separate owners/providers.

Reproduce from repository root using the configured Python/Unicorn environment:

```powershell
& port/level-world/tools/build_character_ai_state_changed_oracle.ps1
python port/level-world/tests/character_ai_state_changed_differential.py
python port/level-world/tests/character_ai_state_changed_host.py --with-vm
```

Future central targets: `tests/character_ai_state_changed.cpp` takes `reference/character-ai-state-changed/state-changed-fixtures.bin`; `tests/character_ai_state_changed_vm.cpp` takes actual `ai-commons-source.luac`. Link the core and VM new CPPs plus existing world/runtime dependencies. The VM audit includes the frozen old owner fixture without editing it. No existing source, CMake, runtime, session or renderer files were changed.
