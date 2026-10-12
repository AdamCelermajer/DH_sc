# V55 ConditionList / actual constructor transport

The source decoder and list storage are implemented in four new files: condition_list_owner_v55.hpp/.cpp and canonical_condition_receivers_v55.hpp/.cpp. Main retains quest/event/level behavior and the actual Eval provider. No shared or selected files were edited.

| Subtask | Status | Evidence |
|---|---|---|
| Original table decoder/row layout | Done | Whole Arrays/data/names +CondAnd/Stub/base; actual118rows/154stubs/2796data+2789namesbytes; whole ConditionData Init/Clear/recall and case/sentinel proofs |
| Original ConditionList C1/Assign/D1 | Done | Real cache118lists; ctor3poisons; repeatAssign/zero/negative cases |
| Canonical pure condition constructors/D0 | Done | Actual7factories x3poisons and7native receiver cases; no controlled constructor fixture in canonical test |
| Native actual table/list/canonical receiver storage | Done | All118lists/154stubs assign/clear with actual retained receivers; NDK aarch64 syntax |
| Same actor8c/b0 binding | Done | Current coherent762 native50Character/100cells: real90early-return and10positive same-actor canonical init/Assign/D0/Clear cases PASS; main Eval remains required |
| Main adoption / true stage10 execution | Pending | Services graft only at authentic qualified parent38be5c; no C1-phase pre-initialization |
| Condition Eval / gameplay activation | Required main | Missing actual provider fails preserving output; no fake quest state or evaluator |

Exact source structures:

- Arrays::v2Conditions row is Structs::v2CondAnd, source16bytes: vtable0,count4,stubs8,listOp12. Serialized row is u32count, count triples of signed(op,arg1,arg2), signedlistOp. Every stub source stride16 holds vtable0/op4/arg1_8/arg2_12. Original source stub-vtable token96a318 is retained as provenance; no ARM code pointer is executed in modern native code. The native immutable table is the one engine producer; main borrows it, rather than installing another global/sheet.
- ConditionList source allocation12bytes and C1 zeroes count0,array4,stubs8. Modern pointers use native width, with typed logical cells; this is not an ARM image memcpy. Assign writes SAME count/stub cells before allocating count*4 and invokes the source factory table in order. Each canonical instance is stored before its SAME live stub4 is bound. Types0..2 have discriminator8 =0/1/2; types3..6 allocate8 and own NO field8. No uniform12byte object or copied stub state is created.
- D1 invokes actual deleting D0 per current nonnull instance in order, zeroes each array slot after successful D0, frees array and clears array4; source count/stubs are not reset. ConditionData Clear then frees the list receiver and clears SAME actor compiled pointer, preserving tested byte. Failed callbacks keep the source partial prefix and compiled pointer; no successful cleanup/activation is invented.
- Repeated Init overwrites SAME compiled pointer without eagerly freeing the previous list; repeated Assign overwrites array4 without deleting the previous array/conditions. Historical allocations remain pinned through the owning world rather than being secretly cleared. This preserves source observable leak/order semantics; no replay is introduced into stage10. On owning-world teardown ordinary memory ownership releases unreachable transport allocations. Normal original first initialization allocates each authored list once.
- Zero/negative Assign stores signed count and stub pointer and preserves previous array4; D1 then skips instance iteration and only frees its array. Malformed decoder dimensions/counts/truncation/suffix fail without replacing a previously published table. Unsupported op<0 or>=7 stops explicitly before an invalid factory dispatch. Partial unconstructed array slots are never read during a fabricated successful destructor.

Minimal composition for main (candidate source only; parent owns selected/CMake):

```cpp
// At the engine's genuine initialized Arrays producer, exactly once:
ConditionTablesV55::load(records,records_size,names,names_size,tables,error);
auto classes=std::make_shared<CanonicalConditionFactoryV55>(actual_world_lease,actual_eval_services);
auto lists=std::make_shared<ConditionListOwnerV55>(tables,classes->factories());
// Retain tables/classes/lists in that same World/Application owner.
parent_services.conditions=lists->services();
```

Use the SAME V51 qualified parent bridge over actor8c/b0. Its original matching row stores the allocated list pointer in actor+a8/cc BEFORE Assign. Do not initialize conditions in the constructor just to display entities. For evaluation, main can borrow lists->fields(compiled_receiver) and classes->borrow(instance,fields), or classes->evaluate(instance,...), which requires its real callback. The callback receives actual stub4 and optional comparator8 and the exact source Eval entry. Main keeps the table/world pinned and implements source ConditionList Eval AND/early-return loop or its existing genuine body; no successful callback is supplied here.

SWAMP acceptance matrix (exact names/case/IDs; Invalid rows return before lookup):

| Object | Property | Condition | Original stub op/argument1/argument2 |
|---|---|---|---|
| _prim_NPC_PriestGood | activate_cond | RENE_FOLLOW | 1/8/13 |
| _prim_ActorTroll | activate_cond | GameStartOnly | 1/50/6 |
| _prim_ActorPrisoner | activate_cond | GameStartOnly | 1/50/6 |
| _prim_MothTemplate_13 | activate_cond | Invalid | early return |
| _prim_MothTemplate_13 | deactivate_cond | Invalid | early return |
| _prim_NPC_05 | deactivate_cond | IsBefore_Swamp_KillWitch | 1/52/13; 1/50/13 |
| _prim_NPC_LizBloodPOST | activate_cond | IsAfter_Swamp_LizBlood | 0/51/13 |
| _prim_Monster_LizardFan01 | activate_cond | IsBefore_Swamp_Escape | 1/50/13 |
| _prim_Monster_MisterSnack | activate_cond | IsBefore_Swamp_Escape | 1/50/13 |
| _prim_Monster_LizardFan03 | activate_cond | IsBefore_Swamp_Escape | 1/50/13 |
| _prim_Boss_SwampKing_01 | activate_cond | IsBefore_Swamp_Escape | 1/50/13 |
| _prim_NPC_Eddie | activate_cond | EDWARD_FOLLOW | 2/59/6; 1/24/6 |

Actual source factory table969258:

| Type | Class | Source allocation | Field8 | Eval entry (main) |
|---|---|---|---|---|
| 0 | Condition_IsQuestInState | 12 | 0 | 0x478fe8 |
| 1 | Condition_IsQuestStateLower | 12 | 1 | 0x478fe8 |
| 2 | Condition_IsQuestStateHigher | 12 | 2 | 0x478fe8 |
| 3 | Condition_IsPlayerInLevel | 8 | absent | 0x478d78 |
| 4 | Condition_IsLevelInState | 8 | absent | 0x478638 |
| 5 | Condition_IsPlayerAtLevel | 8 | absent | 0x478640 |
| 6 | Condition_IsEventInState | 8 | absent | 0x478db4 |

Original proof/native receipts remain explicit: original arrays/list +source factory21/D021 pass; canonical native uses actual receivers, and the earlier constructor-control test remains labeled historical fixtures. No Eval, emulator, whole Character InitPost or playable-SWAMP claim. Historical required-factory prefix used coherent755; updated positive same-actor test links only after parent READY762. Source headers/libraries remain coherent; no dirty graph was linked.

Receiver lifetime: constructor factories/receivers hold weak actual authority; checked live field borrows temporarily pin it. World→lists→factory→World cycles are prevented and verified. The optional main Eval owner is weakly borrowed; main callback must avoid capturing its owning World strongly. Live stub/discriminator cells are on the same canonical receiver, with no sidecar state.

Reproducible positive selected proof command (only after parent coherent READY): `python reference/condition-list-v55/run_condition_swamp_positive_v55.py --coherent-ready`. It compiles unique ConditionList/canonical receiver/positive50 test sources, includes the same frozen V51 headers and current selected V5 headers, and links the coherent receiver-transport-v5-host archive group plus libdh2_script_runtime.so and zlib. Exact code/source hashes and link-map digest are in condition-swamp-positive-v55-verified.json; runner records the full command construction and excludes legacy V4 graph archives. The copied factory setup inc uses sourcechecked design.bin and compares the original GameDesign streams with the real cache. Character input is the four cache-authored name/gametype/activate_cond/deactivate_cond properties; it does not claim full placed actor behavior or all other properties.

Authentic callback seam: after the existing NpcInitPost owner reaches request.source_entry38be5c, lend `parent_services.conditions=lists->services()` and invoke existing `character_parent_init_post_v51(record,request,response,parent_services,error)`. Its reached-phase guard and SAME actor/NPC backing checks remain intact. Root must retain tables/classes/lists in its real owner and Clear actor8c/b0 before releasing that owner. This proof directly tests component transport, not the whole ordered source Init or rendered/playable SWAMP.
