# Source target event pipeline host composition

This audit executes the existing complete source router and prefixes into the actual `CharacterScriptSession` and original cache `monster.luac` callbacks. It changes no production, renderer, central CMake, runtime or frozen proof files. It is a host integration audit of individually source-proved components, **not** a new whole-chain original-instruction differential or live monster AI claim.

## Executed chain

The test calls `dh2_character_ai_event` with Character events `9..0x11`. Its virtual service composes event9 with `dh2_character_enemy_spotted`; events `0xa..0x11` use `dh2_character_target_event`. Those prefixes reload the selected active endpoint and deliver through `CharacterScriptSession::dispatch_target`. The session supplies the genuine alias/availability lookup and source-object Value bridge into original monster Lua. Existing router event9 then reloads owner and invokes its FSM service; permitted target-notification events terminate after their prefixes. Blocked/locked routing sends the original event to the FSM without invoking prefixes; forced bypasses both gates.

Revived `0xb` uses the selected original inherited empty `AISDefault::OnTargetRevived` body, whose raw vtable/function proof is frozen in `reference/character-target-event-route/endpoints-original-probe.json`. The audit does not invent a Lua `OnTargetRevived` registration. All other endpoint numbers use the genuine AISExternal target enum mapping.

The test loads native `CharacterGameDesign` from the real-cache GDO1 input and selects the genuine `Crypt_Skeleton` row, applies native property/class recalculation, then executes actual commons/monster top-level and Init. The retained `CharacterScriptObjects` owner supplies real target bindings, property/life/position storage, object GetID/GetTarget methods, source table representation, actual setter/dead/sight queries and finalizer lifetimes. The two identities exceed 32 bits. A retained Lua table stays callable after the caller releases the enemy's `shared_ptr`; the registry retains its storage through VM close.

Debug queries execute the genuine native DebugSwitches empty-map/missing-file loader and query insertion. Its filesystem open is an explicit not-found fixture. AI sound words come from the real decoded AI rows. The threat word comes from the frozen CES1 corpus whose actual original DesignSettings table/row reader proved `EnemySpottedAggro=10.0`; it is not a fabricated combat constant. Native ownership of the complete DesignSettings table remains outside this composition.

## Observed script behavior and boundaries

- Real `monster_OnEnemySpotted` checks HasTarget, commits SetTarget through the actual setter, then HeadTo passes through source Lua/controller/Character/PathTo kernels. The FindPath provider returns its explicit failed result; no actual scene movement is claimed. A second EnemySpotted with an existing target leaves HeadTo uncalled.
- Real `monster_OnTargetDied` executes Stop before ClearTarget. The continued byte is cleared by the prefix, and current/last target are cleared by the actual native target wrapper.
- Real OutSight first receives the source prefix's ClearAggro service and changed-byte reset, then its Lua Stop. Its unsupported source global ClearAggro fails before Lua ClearTarget. The audit preserves and checks that actual error prefix. It subsequently installs a **test-only scoped ClearAggro fixture closure**, explicitly not a reconstructed wrapper, and verifies that the unchanged original Lua suffix then reaches genuine ClearTarget.
- InSight uses the real row sound ID and source Play3D argument order before its Lua endpoint. Play3D itself is a sound service fixture.
- OutRange reads actual native FSM state plus genuine AIStates constants and HasPath, then reaches MoveTo. Its controller/path bodies again stop at the declared failed FindPath fixture.
- Ranged and Melee use the actual Skeleton script/property branch and reach Attack. The attack command provider is explicit; no target search/combat action is manufactured.
- CloseRange reaches the actual unsupported CreateBuff global, retaining the already delivered prefix and current target. No fake buff or skill provider is installed.

GroupInfo, spawn/limbus/combat queries, aggro map operations, controller/remote/Stop/event3f/FSM bodies, FindPath and sound are explicit synchronous fixtures. Host cached level/current row/difficulty are fixtures over decoded tables. They are required missing owners, not accepted default live services. The raw numeric conversion fixture is not reached by these object-only authored callbacks. Retained native objects are a lifetime owner, not a reconstruction of the original ObjectManager handle map/discovery/deletion.

## Reentry and failure ownership

A HeadTo service fixture uses only the transient `commands.scope` capability to reenter the existing full router with InSight. The nested prefix executes before Step/FSM ownership is claimed and reaches the same privately owned VM through `dispatch_target(...,scope)`. A generic VM query and a generic unscoped recursive endpoint call both remain rejected. After the nested call, the outer capability is restored and valid; target and command scopes are null after every completed/error path.

An injected HeadTo provider failure preserves the real prior SetTarget mutation and stops the outer dispatcher before its event9 FSM suffix. Error effects are not rolled back. VM-close finalization performs actual GetTarget/GetID and ClearTarget while retained scene objects and all providers remain alive. Borrowed providers and sessions must not be destroyed or retargeted during callbacks.

## Proof and reproduction

`tests/character_target_pipeline.cpp` and `tests/character_target_pipeline_host.py` pass **1,298 checks / 91 source events / 59 AIS endpoint deliveries**, including all **72 forced/blocked/locked event9..17 combinations**, four source early state gates, active-null, one nested source chain, two busy guards and finalizer target clearing. There are 40 explicit FSM deliveries, seven sound fixture calls, eight failed FindPath queries and six explicitly labelled ClearAggro fixture suffixes. Sanitizers report zero findings.

From repository root:

```powershell
python port/level-world/tests/character_target_pipeline_host.py --build /home/adampalace/dh2-world-build
```

The runner compiles only the new test and a separate frozen EnemySpotted shim, because that prefix was not yet in the current central DSO. All other kernels run from actual current central sanitized world/data/runtime DSOs. It verifies `dladdr`, captures the full `ldd` dependency list, and hashes source/interfaces/input/proofs and binaries before and after. A moved dependency aborts the audit. Its report is `reports/character-target-pipeline-host-audit.json`; commands and exact hashes are recorded there. It does not infer central compiler inputs from current source hashes; central build provenance belongs to the parent binder.

Original source claims remain bounded by the separate router, EnemySpotted, TargetEvent and selected Revived endpoint instruction proofs bound in the report. No new APK/ARM64-device, whole-frame, physics, navigation, whole AI or gameplay parity is asserted here. The next source dependency for a complete OutSight suffix is the actual Lua ClearAggro wrapper and `AI_ClearAggro` ownership body, not weakening this audit's unsupported-prefix check.
