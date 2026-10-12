from pathlib import Path
import json,hashlib,zipfile,shutil
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-lightset-v53';reports=b/'reports'
names=['canonical_light_point_v53.hpp','canonical_light_point_v53.cpp','canonical_light_point_factory_v53.hpp','canonical_light_point_factory_v53.cpp','level_lightset_stage9_v53.hpp','level_lightset_stage9_v53.cpp','canonical_lightset_file_v53.hpp','canonical_lightset_file_v53.cpp']
for script in Path('C:/Users/adamc/AppData/Local/Temp').glob('dh2-*v53.py'):
 if any(x in script.name for x in ['light','stage9']):shutil.copy2(script,r/script.name)
v=b/'vendor/character-rng-integration-v5-loading/port';source_paths=[v/'level-world/canonical_property_map_v1.cpp',v/'level-world/canonical_object_factory_v1.cpp',v/'level-world/light_set_name_owner_v3.hpp',v/'level-world/light_set_name_owner_v3.cpp',v/'level-world/canonical_level_config_module_v1.hpp',v/'level-loader/canonical_level_context_v1.hpp',v/'level-loader/level_constructor_v3.hpp']
receipt=[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in source_paths]
report={'version':53,'source_original_sha256':'36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80','source_status':'new isolated adoption candidate; selected/shared source untouched','files':names,'actual_source_behavior':{'stage9':'Device.IsFixedPipeline chooses exactly fixed_light_set2d0 OR light_set2b8, no sequential/fallback route; empty chosen filename skips; tagModule','original_outer_poll_proof':8,'original_GO19_C1_and_declarations':True,'light_specific_descriptors':10,'original_normalization_bit_exact_cases':4,'original_SyncData_field_radius_cases':7,'LoadScene_reference_literal':'','SyncData_debug_switch':'TestMultiPlayerLight','native_Swamp_ingestion':'actual supplied Module XML/native canonical parser+factory+PropertyMap+manager,oneGO19','registered_name':'_prim_PlayerLight_S (existing actual source factory special rule)','GO19_inherits':'ObjectBase, not GameObject','deferred_InitPost':'required actual CLightSceneNode(true); no fake renderer success'},'bindings':{'source_filename':'reuse V52 authentic resolver; no fixed worlds/data aliases','file_parser_storage':'reuse same actual CanonicalCachedFileV1/manager/candidate/class dispatch, pins original XML and exact factory prefix','config':'borrow same published Level38 and actual CanonicalLevelConfigV1 strings','names':'borrow existing LightSetNameOwnerV3 only in main full light manager facet; no copied names tree','node_storage':'Main real retained native node/CLight identity; typed owner/pointers/validation; scalar writes recovered','assigned_Level140_stream':'resource-open adapter explicitly rejects; real assigned-stream continuation still required','network':'missing AssignObjectNetworkId proved; positive source proof uses explicit host network endpoint fixture','activation':False,'GPU':False,'functional_attachment':False},'tests':{'standalone':'ASAN/UBSAN original8branch/poll+4normalization+7SyncData; current actual PropertyMap with isolated schema proposal; stage9 stale config/prefix replay checks','actual_graph':'READY755 read-only source cache/registry proof; initial missing-network guard; actual source light InitPost stops at native-node provider','schema_modification':'test-only independent CPP copy; selected actual PropertyMap not edited','canonical_file_adapter':'syntax-only coherent selected header graph','no_device_or_shared_mutation':True},'source_receipt':receipt,'remaining':['Parent adoption/common CMake/manifest and integrated source lifecycle stage9 check','Authentic Main Device.IsFixedPipeline/current scope binding','Main actual CLightSceneNode/SceneManager root+automatic list+CLight type storage/current owner','Main functional LightPoint.RefreshAttachment and full light-set material/filter/driver services','Assigned stream140 producer if reached; no fabricated file count/progress or runtime ready']}
report_path=reports/'level-lightset-stage9-v53.json';report_path.write_text(json.dumps(report,indent=2)+'\n')
handoff='''# Stage 9 light-set / GO19 source adoption V53

Stage 9 is original **Module ingestion**. `Device::IsFixedPipeline381668` selects one filename: fixed pipeline uses `LevelConfig.fixed_light_set2d0`; programmable uses `light_set2b8`. Both call `Level.LoadFile3f3b40(...,"Module")` repeatedly until complete. The original executable oracle proves alternatives, never fixed-then-regular. This supersedes the initial static-reading message that said sequential loading.

The supplied Swamp Module declares one ObjectBase-derived LightPoint, GO_ID19. The actual current755 canonical source walk registers it as `_prim_PlayerLight_S` (existing genuine factory special-name rule), radius3.5, automatic1, attachedPlayer/offset200, ambient255. Missing network admission is separately tested; the positive host source test uses an explicitly labeled network endpoint fixture. It does not establish network/runtime behavior.

New factory34115c adapter allocates that SAME retained receiver, returns the existing CanonicalClassReceiverV1 transport, and retains original declaration/World owners. No alternate registry or detached XML actor list. The proposed10 light-specific properties must be added to the existing class PropertyMap whitelist/schema. LightPoint does NOT inherit GameObject declaration/Visual fields. `_templateName` remains the existing PropertyMap field. Source ZERO remains a genuine borrowed default. Radius/automatic are absent at C1 until descriptor defaults produce them. Byte87 uses the same documented modern deterministic room-local safety correction as other ObjectBase receivers (source C1 indeterminate87).

Source InitPost prefix clears1b0/1b1, normalizes all colors/255 and linear/quadratic attenuation/1000,/1e6. Native float bytes match original ARM. SyncData writes actual borrowed CLight color alpha1, attenuation, and radius ONLY >.01, with `TestMultiPlayerLight` source debug skip for nonempty dae. Scene.LoadScene's reference literal is **empty**, not a guessed Light name. Actual retained scene-node/native-light construction, scene-root attachment, automatic registry, native type0 and functional RefreshAttachment are explicit Main services. The host's SAME registered receiver stops exactly at required `CLightSceneNode(true)`; stage9 source ingestion completion is not stage10 or renderer completion.

The new file adapter reuses actual CanonicalCachedFileV1/Zip/XML/factory/manager and source Level160/18c fields; it calls V52 real filename resolution. Initial assigned stream140 is explicitly outside this cache-open adapter and rejected. No extra file13c/progress updates are guessed. Caller supplies actual source parse-result/release continuations and cleanup. Check actual Level/config/device/application ownership each poll. See stage9-binding-v53.inc.proposed for exact owner/stage/factory binding. Release actual manager/transport before factory.erased and file journal discard.

| Subtask | Status |
|---|---|
| Original stage9 mode/file/tag/poll body | Done, original8cases |
| Genuine GO19 C1 + factory +10properties | Done, candidate/native source proof |
| Scalar InitPost + CLight SyncData | Done, original/native bit comparisons |
| Same canonical Module XML/registry ingestion | Done, actual SWAMP/read-only755 proof |
| Stage9 lifecycle application adoption | Pending parent/Main integration |
| Real native light/root/automatic list/type | Required Main provider |
| Functional player attachment + shader/light-set driver | Required Main provider |
| Live renderer/GPU/whole Act1 ready | Open; no claim |

No selected/common/shared source or frozen older packet changed; no emulator/device. Initial host failures were probe API-name/header/schema-insertion mistakes and incorrect expected raw player-light name; final expectations use exact current canonical API/source rule. Source proof never weakened owner/no-fake/GPU boundaries.
'''
(reports/'level-lightset-stage9-v53-handoff.md').write_text(handoff)
paths=[b/n for n in names]+[report_path,reports/'level-lightset-stage9-v53-handoff.md']
paths += [p for p in r.iterdir() if p.is_file() and not p.name.startswith(('lightset-native-v53','lightset-actual-source-v53'))]
items=[{'path':p.relative_to(b).as_posix(),'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size} for p in paths];digest=hashlib.sha256(json.dumps(items,sort_keys=True).encode()).hexdigest()[:16]
target=reports/('level-lightset-stage9-v53-'+digest+'.zip');assert not target.exists()
with zipfile.ZipFile(target,'w',zipfile.ZIP_DEFLATED) as z:
 for p in paths:z.write(p,p.relative_to(b).as_posix())
 z.writestr('manifest.json',json.dumps({'version':53,'files':items},indent=2)+'\n')
with zipfile.ZipFile(target) as z:
 for item in items:assert hashlib.sha256(z.read(item['path'])).hexdigest()==item['sha256']
print(json.dumps({'zip':str(target),'sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'payloads':len(items),'bytes':target.stat().st_size},indent=2))
