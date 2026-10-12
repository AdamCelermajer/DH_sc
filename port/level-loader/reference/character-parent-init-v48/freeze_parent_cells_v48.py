from pathlib import Path
import hashlib,json,zipfile
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-parent-init-v48';out=r/'actual-actor-cell-candidate';reports=b/'reports';v=b/'vendor/character-rng-integration-v5-loading/port/level-world';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
receipt=json.loads((reports/'character-parent-cells-v48-verified.json').read_text());assert receipt['validation'].startswith('PASS') and not receipt['compiled_condition_allocated']
for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','character_parent_init_fields_v48.hpp']:assert (out/n).read_text()==(v/n).read_text(),n
for p,h in receipt['sources'].items():assert sha(Path(p))==h,p
receipt['selected750_actor_semantic_text_matches']=True;receipt['selected750_actor_hashes']={n:sha(v/n) for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','character_parent_init_fields_v48.hpp']};receipt['selected750_current_build_ready']=False
(reports/'character-parent-cells-v48-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
doc='''# Qualified Character parent InitPost V48: storage proven, shared control pending

The actual Character now has the eight missing inherited scalar cells: seven from ObjectBase C1/C2 (checkpoint28, optimization10c, difficultyec, compileda8/testedac, compiledcc/testedd0) plus GameObject idle sound signed-halfword370. No CanonicalBase, property sheet, visual, condition or save object was allocated around the actor.

| Subtask | Status |
|---|---|
| Original whole ObjectBase C1/C2 and nested ConditionData C1 values | Finished: GO0, six poisoned constructor cases |
| Original idle370 halfword producer | Finished: three poisons, upper bytes preserved |
| Actual actor cell constructor/checked alias transport | Finished: three real factory records; native PASS |
| Same pose/NPC/visual backing and no reset on second borrow | Finished: native PASS |
| Foreign/raw facade/adoption/property guards and alias lifetime | Finished: native PASS |
| Original whole qualified38be5c branch control | Finished: cached rejection1, controlled NULLvisual3, required producers2 |
| Shared generic/Character initialization control kernel | Pending separate V51 extraction |
| Actual ConditionList/renderer/visual/PF providers and teardown | Required main-owned services |
| Whole Character/level activation and rendering | Pending |

The actual-owner test compiles full candidate actor and family together against coherent749 dependencies. Its link map excludes archived actor/family/predicate objects. It proves exact current NPC pose/scale/visual aliases, original eight values, live condition/sound stores, output preservation on rejected borrows, unchanged fields on repeated borrows, record-alias lifetime without cycles, and zero RNG use. It does not call parent initialization, allocate compiled conditions, or preselect model/templates. Parent has adopted the same semantic cells into selected750; its transitive rebuild is pending and no750 archive is used for this proof.

Original38be5c order is Base InitPost (conditions8c thenb0 then optional difficulty), current spawn check, physical2dc clear, near-zero scale normalization, degree/radian+heading stores, qualified SetPosition(true), bounds scaling/absolute update, live Device/LoadVisual/Sync decisions, checkpoint clear, then optional signed-halfword sound lookup. InitFinal remains a separate source method.

The original body runs to return on cached rejection and controlled omitted-visual cases without successful visual/renderer stubs. The omitted cases deliberately supply post-C1 optimization1/device/checkpoint/catalog inputs. Constructor optimization10c is0, so those cases do not establish first-SWAMP visual omission or activation. Normal constructor optimization0 stops honestly at actual LoadVisual; unprovided actual renderer mask stops inside Device. Whole source bodies execute for Base InitPost, empty/Invalid ConditionData, cached spawn, NAME IsPlayer, SetPosition, UpdateAbsoluteAABB and Device. Manager/class/type and arithmetic/catalog domains are explicitly listed in the oracle.

The first SWAMP has50 authored Characters,10 authored activate-condition references and2 deactivate references; no authored Character name begins PlayerCharacter. Inventory preserves module/placement/source IDs and is not live activation proof. Actual PropertyMap borrows remain authoritative.

The concrete existing GameObjectInitializationOwnerV1 and ConditionDataInitV3 kernels require CanonicalGameObjectBaseOwnerV1. Next step is extracting one typed borrowed-storage shared kernel with adapters for actual generic base and actual Character; allocating a second base or permanently copying the engine control is forbidden. Character idle storage is signed16; generic mapping must use a width-preserving store adapter rather than introduce an int32 shadow.

ConditionData nonempty match allocates12bytes, invokes ConditionList C1 at4786f0, stores its receiver at+1c before tail AssignPyData478914. Original row+8 is a v2ConditionStub pointer; row+4 is signed32 count (the current V3 comments mislabel this as a native receiver). AssignPyData stores count and stub pointer, allocates count*4 only if count>0, dispatches each original stub type+4 factory and assigns each stub at constructed condition+4. Main must provide real decoded-stub/table lifetime and actual per-type constructors. Empty/Invalid and genuine table miss are source returns, not successful authored predicates. Clear33e7c8 invokes ConditionList D1 at478eac, CustomFree310440, then clears the SAME compiled pointer; tested is not cleared. NonNULL allocation must not be enabled without that genuine close/failed-candidate ownership path.

Root already has RetainedCharacterFamilyVisualV6 and CharacterVisualAssetOwnerV6 over the actor's actual source_visual()/strings and same scene/root/bounds graph. Reuse them at reached LoadVisual/Sync; actual renderer, mesh/PF, animation and source actions remain required. Device38174c tailcalls3816a8: three live process disable bytes then existing renderer virtual mask&0x78, not a constant.
'''
(reports/'character-parent-init-v48-handoff.md').write_text(doc)
(r/'freeze_parent_cells_v48.py').write_bytes(Path(__file__).read_bytes())
files=[p for p in r.rglob('*') if p.is_file() and '__pycache__' not in p.parts]
for n in ['character-parent-init-v48-dependencies.json','character-parent-init-v48-cell-proposal.json','character-parent-cells-v48-verified.json','character-parent-cells-v48-native.log','character-parent-cells-v48-build.log','character-parent-cells-v48.map','character-parent-init-v48-handoff.md']:files.append(reports/n)
packet=reports/'character-parent-init-v48-cells.zip';manifest={str(p.relative_to(b)).replace('\\','/'):sha(p) for p in files}
with zipfile.ZipFile(packet,'w',zipfile.ZIP_DEFLATED) as z:
 for p in files:z.write(p,str(p.relative_to(b)).replace('\\','/'))
 z.writestr('SHA256SUMS.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(packet) as z:
 for n,h in manifest.items():assert hashlib.sha256(z.read(n)).hexdigest()==h,n
frozen={'schema':'dh2-character-parent-init-v48-frozen','packet':str(packet),'packet_sha256':sha(packet),'members_verified':len(manifest),'original_parent_body_control_proven':True,'actual_owner_cells_native_pass':True,'whole_GameObject_InitPost_wired':False,'compiled_conditions_allocated':False,'selected750_build_ready':False,'files':manifest}
(reports/'character-parent-init-v48-frozen.json').write_text(json.dumps(frozen,indent=2)+'\n');print(json.dumps({k:v for k,v in frozen.items() if k!='files'},indent=2))
