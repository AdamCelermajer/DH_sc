# V50 source stage6 GameEvent storage handoff

The original-derived event table decoder, GameEventManager storage owner and stage6 body are source-ready. Native normal and ASAN/UBSAN tests pass against original ARM gold: 66 events, 66 nested objectives, all11 constructor/reset types, 7 retained failure prefixes, 9 stage-body ownership/failure cases. This closes opaque manager/event/objective scalar-storage callbacks; it does not implement functional quest effects.

## Minimal integration hunks

1. Add `game_event_manager_v50.cpp` once to the actual loader source target. Use `game_event_manager_v50.hpp` and lightweight `game_event_level_fields_v50.hpp`.
2. Apply ONLY `reference/game-event-v50/canonical-level-event194-proposed.diff` and add `canonical-level-event194-method-proposed.inc` method to actual coherent context. This adds one strong manager lease to SAME Level, alongside existing pointer `constructor_fields_v3_.field194`. The method demands existing completed-C1 `loading_fields_v26`; it returns pointers to actual state130/raw194/lease, no copied phase/current global.
3. During genuine GameDesign initialization, initialize ONE `GameEventTablesV50` from actual cache entries `data/pydata/v2eventmanager_pyarray.bin` then `...pyarraynames.bin`. Required records-reader-before-name-reader order is original4b8c9c→4b1460. Do not duplicate an already initialized compatible owner: use `GameEventTablesBorrowV50` to borrow it. Existing main names registry/registered_names.inc is not a row table. Extracted fixture bytes are test evidence only; production resolver/cache must supply originals. Main Lua namespace registration remains an explicit separate gap.
4. Borrow actual context `game_event_fields_v50`, actual PlayerManager byte6c9 and existing trace callback. Bind `stage6_game_events_v50(fields,services)` in the external stage6 slot. Required true `player_update` executes378fb4; absent provider fails after original byte store. Stage body never writes state130 or progress. Existing lifecycle dispatcher increments6→7 only after complete.
5. Keep SAME manager/rows/objectives through main stage31 compile and functional behavior, then genuine teardown. No second EventManager/registry. Existing EventManagerOwnerV12 is a different object.

## Exact source order

Level stage6: trace3f72d0..3f7300; actual PM from application40; byte6c9=1 at3f7314; Update378fb4 at3f7318; new12/C1479d08 at3f731c..3f732c; actual Level194 publish3f7330; whole Load479e5c3f7338; dispatcher130++3f733c..3f7344.

Manager C1479d08 invokes vectorC1479cac (begin/end/cap zero). Load479e5c returns on nonempty vector BEFORE reading table globals. First empty load captures actual size9a66e0, resizesNULL slots, captures members9a66e4 before each new32, C14795ac, rereads names9a66e8, stores id/name, Assign479568 then ReInit479538, finally publishes vector slot479f10. All66 rows load; no Level-name filter. Assign publishes exact row1c and stubptr/list count before array allocation. Objective factory47a3e8 constructs true scalar fields then sets SAME stubptr+c/type4. ReInit state0=0 then actual virtual-reset scalar stores. Objective type4 and6 reset completed14 only; other SavedQty types also clear word20. Original C1 does not initialize EventReceiver.byte1c; native representation keeps it unset.

Native `load_step` preserves this intra-load order but is bounded scheduling (one objective C1/reset per step). It is not a byte-for-byte original synchronous timing claim. Failure, bounds and reentry guards are explicit native safety additions. Pending manager and earlier published events remain owned. `GameEventManagerV50` destructor releases native storage only; it must never be described as original functional Unload. Actual `GameEventManager::Unload` is479968, reaches GameEvent::~47957c and objective teardown; main must bind real teardown before registered runtime receivers are released.

## Proof scope

`reference/game-event-v50/original-v50.py` executes actual ARM generated records/name readers, whole GameEventManager C1+Load, all11 Objective factories/virtual-reset bodies with fixture stream/allocation delivery. Gold records/data pointers and constructor fields are original execution results. Native tests compare exact66 decoded rows/names/scripts/nested fields and actual table-row aliases. Stage body tests use EXPLICIT phase6 Level/Player/trace fixtures, not an actual completed C1 or actual PlayerManager.Update body. Actual proposed context member/method compiles against frozen coherent source headers; no mixed-layout archive link was performed. Parent/main must run actual C1194 alias proof after coherent adoption.

### Subgoals and status

| Subgoal | Subtask | Status |
|---|---|---|
| Original evidence | Recover separate manager Load/C1/Assign/ReInit addresses and real tables | Finished |
| Original evidence | Execute all11 Objective C1/reset bodies + original full66 table/manager path | Finished |
| Table producer | Exact records then names decoder with stable sole-owner borrow | Finished |
| Manager storage | Bounded source order, actual row/name aliases, retained failure prefixes | Finished |
| Stage6 integration | Required player prefix, Level194 pointer/lease, bounded Load | Finished in isolated fixture |
| Actual Level lease | Narrow context member/method proposal | Compile checked; awaiting coherent adoption |
| Real source providers | PlayerManager.Update and application table/Lua registration | Main required |
| Functional events | Stage31 compile/register, objective/quest behavior, actual teardown | Main required |
| Chapter1 runtime | Contiguous0..38, actual rendering/gameplay/mobile | Not established by V50 |

Run receipts: `reports/game-event-v50-stage-build.json` storage normal/sanitizer; `reports/game-event-v50-stage-final-build.json` final9 ownership cases normal/sanitizer; `reports/game-event-v50-context-build.json` exact member/method compile-only. Original report `reference/game-event-v50/original-v50.json` records hashes/results. No shared, selected graph, CMake or device mutation.
