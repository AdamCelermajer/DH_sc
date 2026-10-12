# Actual source stage7/state10 integration V38

Implemented new typed source-body adapters, original instruction receipts, a narrow actual-manager API proposal and a narrow lifecycle post-increment proposal. Selected parent graph/core/CMake remain unchanged. The independent manager prototype uses a separate header overlay plus byte-identical copied manager.cpp; it is never linked with another manager implementation.

## Original source order

**State7 (entry3f7204):** writes original globals from actual Leveldc/e0; reads actual procedural bytee8. Fixed levels proceed to counter/load. Procedural levels call GetOnline, read the selected already-published global (offline dc, online e0), construct temporary StreamBuffer, call GenerateRandomLevel. Success assigns the generated stream into actual Level.LoadFileData140; generation miss mutates actual namef8. Destroy temporary stream, set138=500, repeatedly LoadFile(actualname,"Level"). Completion stores130=8, increments13c, then common progress tail. Forty actual ARM cases verify ordering, seed choice, strings, retries and counters. Standard allocator/strlen/mutex/unsigned-division and external source bodies are explicit fixtures.

Source backup behavior is precise: mutate the **first CString character** to'x'; when a dot exists, truncate at first dot and append `_BACKUP.mlx`; no-dot names receive only the first-character mutation. Thus existing LevelPreparation's basename-preserving/always-appending helper remains a source-inspection adaptation outside the identical raw CString domain. Do not substitute it for true state7.

**Root factories:** cached original XML root selector is exactly `Level`. `_LoadFromXML` skips exact gametype `Player`. SWAMP MLX has10 root factory occurrences:1LevelConfig +9Module. LevelConfig receives its authentic early InitPost during factory load, producing real config for states8/9. These root factories are distinct from the195 MGP/MVP constructor occurrences.

**State10 (entry3f70cc):** read actual ObjectManager map1c once into138 before the InitPost loop. It includes reserved null key0 and unpublished/operator[] nodes; count50 is distinct. Original InitPost34552c phase0 creates cursors, phase1 loads actual Module sources (MGP then MVP) and may append modules/insert registry objects. Then phase3 walks actual int-key ordered registry: construct handle, resolve(false), resolve(true), virtual InitPost, resolve(true), TestEnableCondition(false). It clears2c/44/34 in that order before phase4. Phase4 performs case-insensitive RoomZone classification, appends room list24 before RoomZone.InitObjectList, otherwise IsUpdatable/append2c, then reads a8/ac/cc/d0 and appends44 when reached. Completion phase5 returns true.

Source floor.PostLoad is state11. Generic GameObject.InitFinal over actual registry is state17. **Module MGP/MVP load must precede generic InitPost, floor.PostLoad and InitFinal.** Existing RetainedLevelModuleGraph inspection sequence (init modules/floors/finalize before load_next_module_sources) is not this lifecycle order.

## New code

- `stage_loader_v38_root_file.hpp`: typed actual-field state7 driver, explicit globals/generator/stream/root-load services. No second source state or RNG. Native responsiveness uses one provider call per step with retained cursors. Root completion leaves actual130=7; `after_source_increment` performs13c only after dispatcher has produced8.
- `stage_loader_v38_init_post.hpp`: typed InitPost scheduler borrowing the actual manager map, modules and phase7c. It owns only modern scalar cursors and failure guards; no copied registry or output lists. Membership a8/cc transport uses uintptr_t because native pointer identities may be64-bit; only zero/nonzero is used in original predicates.
- `stage_loader_v38_stage10.hpp`: seeds real map1c once, then drives same InitPost scheduler. Concrete retained Module provider calls existing begin_module and receiver.load directly before the facade's geometry gate. Real source state10/phase1 and completed root are required. It preserves original Module selector/shared RNG, actual Level18c/160 context, MGP/MVP ordering and successful reset. Existing module_load_v1 spins synchronously inside one call; this wrapper cannot preempt it.
- `stage-loader-v38-manager-proposed.diff`: add actual uint32 phase7c (original C1 zero34a4f8, also alias34a2dc), true entries_.size() projection1c, and begin/upper_bound/find over existing map including null entries. Original Flush also writes phase7c=0 at3499d4. Normal owner cleanup must preserve that source reset.
- `stage-loader-v38-lifecycle-post-increment.diff`: add reached after_source_increment callbacks; stage7 hook required after130 store and before progress tail. Missing13c service fails while preserving actual state8/completed root prefix, with no completed-stage metric claim or factory replay.

## Verification and scope

ASAN/UBSAN independent targets pass:

- Original InitPost16 fixtures:182 calls and269 events, exact call phases/order/room/transient/module lists, including dynamic modules, inserted registry nodes, negative keys and mutated membership.
- Native stage7 output matches40 actual original ARM cases for filename, event order and final130/13c/138 fields. Generator, file, class and stream services remain explicit fixture observers.
- Actual CanonicalObjectManager prototype8 checks: phase zero, true map1c, reserved null0, negative null insertion, unpublished node, and map1c=3 while count50=0.
- Actual original ObjectManager constructor field stores execute across2aliases/3poisons (6 cases); Flush's phase reset executes3poison slices. Full Flush is an observer during C1, so pre-Flush map1c0 is explicitly separate from completed reserved-null map1c1.
- Existing original scalar parity remains38 dispatcher +195 progress cases. Proposed post-increment lifecycle passes13 safety cases including actual130->13c->progress order and failure/no-replay.
- State10 actual-manager counter remains its initial map1c despite later insertion. Retained Module bridge shape checks show load before geometry, no replay and explicit missing dynamic Module failure. Real Module effects were not executed in that shape probe.

## Parent adoption and required producers

1. Review/apply the narrow manager header diff to a fresh coherent private graph. The prototype's copied manager.cpp is a test dependency, not a second authority or source change.
2. Review/apply lifecycle post-increment diff. Existing standalone stage7 body fixtures need an explicitly labeled counter callback; production must update actual file13c after actual130=8, never fabricate a completed Level.
3. Borrow actual completed Level C1 via existing counter/Stage7 helpers. Supply actual dc/e0 global cells; no timer or synthetic seed. Bind fixed root-file callback to the existing retained original CachedFile/Root walk, resolving actual C1 name rather than blindly substituting an immutable inspection definition. Procedural generator/faithful stream serialization and actual AssignSteam140 still need real bindings; derived inspection XML is not accepted as original serialization proof.
4. Bind source stage10 Module service using concrete retained provider or corresponding narrow preparation method. All actual manager modules must map to retained canonical Module records. Dynamic module identities absent from current root-only records fail explicitly until actual record lookup is supplied.
5. Bind authentic handle constructor (including null map nodes), same-map resolve, class InitPost, condition evaluation, class type getter, IsUpdatable, RoomZone.InitObjectList and actual a8/ac/cc/d0 storage. Actual manager list24/2c/44 owners and clear34 must be supplied by main; scheduler does not invent list vectors. They are currently explicit dependencies.
6. Source state11 must follow complete source InitPost before real floor.PostLoad. State17 must perform generic GameObject.InitFinal in actual registry order. Historical preparation initialized/finalized flags should reflect real class calls, not be set to fabricate preparation completion. New source bridge journals do not yet update the facade's legacy status counters.
7. Incomplete abort remains the separate required teardown service from V36. Do not invoke normal Unload/player save during incomplete loading.

Whole Level.Init, gameplay readiness, current-GS publication, world0/audio publication and live emulator rendering remain unverified. No emulator launch, shared graph edit or persistent save write occurred.

## Reproduce

```text
python reference/level-init-v36/stage-loader-v38-stage7-proof.py
python reference/level-init-v36/stage-loader-v38-manager-ctor-proof.py
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/cmake-stage-loader-v38 -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-loader-v38 -G Ninja
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-loader-v38 -j 1
python reference/level-init-v36/stage-loader-v38-compare.py
wsl.exe -d Ubuntu -- /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-loader-v38/dh2_stage_v38_bridge
wsl.exe -d Ubuntu -- /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-loader-v38/dh2_stage_v38_post /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/level-init-v36/lifecycle_v36_original_gold.bin
```

Python source oracles require the existing local Unicorn runtime/test helpers and original ELF. Trace comparison consumes the supplied original ObjectManager fixture receipt; it does not compare against a second native implementation.
