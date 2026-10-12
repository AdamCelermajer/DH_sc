from pathlib import Path
import json,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/condition-list-v55';rep=b/'reports';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();source=json.loads((r/'original-conditions-v55.json').read_text());profiles=json.loads((r/'condition-constructors-original-v55.json').read_text())['profiles'];v=json.loads((b/'reports/chapter-coverage-v36.json').read_text());swamp=next(x for x in v['levels'] if x['identity']=='SWAMP');rows={x['name']:x for x in source['original_table_rows']};cases=[]
for x in swamp['objects']:
 if x['authored_gametype']=='Character':
  for key in ['activate_cond','deactivate_cond']:
   if key in x['attributes']:
    name=x['attributes'][key];row=rows.get(name);cases.append({'object':x['authored_name'],'property':key,'name':name,'source_uri':x['source_uri'],'direct_child_index':x['direct_child_index'],'condition_sentinel':name in ['','Invalid'],'row_exists':row is not None,'list_op':row['list_op'] if row else None,'stubs':row['stubs'] if row else [],'native_transport_status':'canonical_ctor_available; actual50actor positive probe pending READY762' if row else 'genuine early return; no allocation'})
(rep/'condition-list-v55-swamp-cases.json').write_text(json.dumps({'cache_sha256':v['cache_sha256'],'source_inventory_sha256':sha(b/'reports/chapter-coverage-v36.json'),'authored_property_counts':{'activate':10,'deactivate':2},'sentinels':2,'meaningful':10,'cases':cases},indent=2)+'\n')
md='''# V55 ConditionList / actual constructor transport

The source decoder and list storage are implemented in four new files: condition_list_owner_v55.hpp/.cpp and canonical_condition_receivers_v55.hpp/.cpp. Main retains quest/event/level behavior and the actual Eval provider. No shared or selected files were edited.

| Subtask | Status | Evidence |
|---|---|---|
| Original table decoder/row layout | Done | Whole Arrays4b8dec +CondAnd505970 +Stub/base readers; actual118rows/154stubs/2796bytes |
| Original ConditionList C1/Assign/D1 | Done | Real cache118lists; ctor3poisons; repeatAssign/zero/negative cases |
| Canonical pure condition constructors/D0 | Done | Actual7factories x3poisons and7native receiver cases; no controlled constructor fixture in canonical test |
| Native actual table/list/canonical receiver storage | Done | All118lists/154stubs assign/clear with actual retained receivers; NDK aarch64 syntax |
| Same actor8c/b0 binding | Done prefix | Prior coherent755 native50Character/100cells: real names90empty/Invalid,10positive prefixes before missing factory; upgraded positive probe awaits parent READY |
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

'''
md+='| Object | Property | Condition | Original stub op/argument1/argument2 |\n|---|---|---|---|\n'
for c in cases:md+='| '+c['object']+' | '+c['property']+' | '+c['name']+' | '+('; '.join('/'.join(str(s[k]) for k in ['op','argument1','argument2']) for s in c['stubs']) or 'early return')+' |\n'
md+='\nActual source factory table969258:\n\n| Type | Class | Source allocation | Field8 | Eval entry (main) |\n|---|---|---|---|---|\n'
for p in profiles:md+=f"| {p['op']} | {p['class_name'].split('<')[1].split('>')[0]} | {p['allocation_bytes']} | {p['comparator8'] if p['comparator8'] is not None else 'absent'} | {p['Eval_entry']} |\n"
md+='\nOriginal proof/native receipts remain explicit: original arrays/list +source factory21/D021 pass; canonical native uses actual receivers, and the earlier constructor-control test remains labeled historical fixtures. No Eval, emulator, whole Character InitPost or playable-SWAMP claim. Prior archive test used coherent755; no link against dirty762 was attempted.\n'
(rep/'condition-list-v55-handoff.md').write_text(md);print('WROTE handoff exact SWAMP12case table; actualclasses7; native positive50 pending parentREADY762')
