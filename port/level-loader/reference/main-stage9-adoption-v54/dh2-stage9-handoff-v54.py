from pathlib import Path
import json,hashlib,zipfile,shutil
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/main-stage9-adoption-v54';main=Path('C:/Users/adamc/Desktop/workspace/DH_sc')
(r/'renderer-stage9-hook-v54.inc.proposed').write_text(r'''// At genuine NativeGS/source-loading assembly, BEFORE SourceLoadingV43::create,
// in model_renderer.cpp anonymous scope where these existing owners are visible.
// No equivalent SourceLoadingV43 runtime assembly site exists in current Main
// yet; renderer_native_level_c1_v25.inc is C1/GS transport only. This fragment
// attaches to that forthcoming source candidate assembly, never preview load().

MainStage9ActualInputsV54 actual;
actual.level=world->native_level_c1_v25->candidate();
actual.manager=std::shared_ptr<dh2::world::CanonicalObjectManagerV1>(
 world->canonical_world,&world->canonical_world->manager);
// source_modules is the SAME CanonicalLevelModuleBindingsV2 whose actual GO4
// publication produced Level.config38. Do not allocate one solely for stage9.
actual.modules=source_modules;
actual.actual_device_owner=dh2::android_resources::budget_lease_v39();
actual.files=existing_actual_root_file_inputs; // same candidate/manager/classes
actual.files.level=actual.level;actual.files.manager=actual.manager.get();
actual.files.actual_filesystem_owner=existing_v52_fs_owner;
actual.files.resolve_filename=existing_v52_root_resolver;
actual.validate_scope=[weak=std::weak_ptr<WorldScriptContext>(world),
 weak_manager=std::weak_ptr<dh2::world::CanonicalObjectManagerV1>(actual.manager),
 weak_level=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(actual.level)](std::string& error){
 auto current=weak.lock();auto manager=weak_manager.lock();auto level=weak_level.lock();
 if(!current||!manager||!level||live_script_world!=current||!equipment_platform||
 equipment_platform->world!=current||!current->canonical_world||
 &current->canonical_world->manager!=manager.get()||!current->native_level_c1_v25||
 current->native_level_c1_v25->candidate()!=level){error="Required actual current World/source manager/Level C1 scope";return false;}
 error.clear();return true;
};
std::shared_ptr<MainStage9LightsetBindingV54> stage9;
if(!MainStage9LightsetBindingV54::bind(std::move(actual),source_loading_inputs,stage9,error))return false;
// The SourceLoading callback owns stage9. Keep its file journal for the same
// candidate teardown; it uses weak binding validation, so externally held file
// views reject a destroyed stage9 owner. Do not also strongly pin World from a
// World-owned binding (cycle): canonical facet contains raw World; guards weak.

// Existing CanonicalLevelClassDispatchInputsV1.remaining before unknown class:
if(entry.name&&std::string(entry.name)=="LightPoint"&&entry.original_address==0x34115c)
 return same_candidate_light_factory->construct(entry,source,receiver,error);
return existing_remaining(entry,source,receiver,error);
// actual light factory input.world = SAME source candidate/provider facet;
// provider uses weak record for same receiver callbacks. Preserve actual
// RendererCanonicalWorldV4.assign_network (offline genuine early return),
// language_added and canonical_vec3_origin_v1. Bind the source LoadTemplate
// assertion9 diagnostic: existing properties ctor currently passes nullptr.
// No dummy node/GL actor; native light callbacks remain required until Main's
// authentic retained lighting engine supplies them.
''')
text='''# Main stage 9 adoption V54

`current-root-stage9-source-v54.patch` adds the frozen V53 eight hpp/cpp files under `port/level-loader`, the10-property schema fragment under `port/level-world`, one class-whitelist/include edit, and four source TUs to **existing** `native_loading_sources_v50.cmake`. That file is already included by `port/level-world/CMakeLists.txt:689`; it deduplicates real source paths into `dh2_level_world`. Native target already links that owner. Do not `add_subdirectory(port/level-loader)` for production: its standalone floor/scene libraries would duplicate the runtime authorities. Do not compile a second PropertyMap/light-set Names/driver implementation.

The patch’s exact preimage hashes are in current-root-source-receipt-v54.json. Main was read only. Final readback found only model_renderer.cpp moved during audit; schema/CMake/device/C1/current canonical owner preimages stayed identical. Patch adds no diff to moving model_renderer.cpp. V53 ZIP SHA remains ccfe3cceee83fd704e81d82987c39e362ec9ce48dd45cc13b6bdb7616b34f580.

The additional typed `main_stage9_lightset_binding_v54.hpp/.cpp.proposed` is the immediate renderer consumer. Copy to current `port/android-native/app/src/main/cpp` and add its cpp to existing dh2_native source list (or include implementation once from model_renderer; choose ONE). The binding passed strict syntax-only `-Wall -Wextra -Werror` against coherent762 headers/current Main APIs. No762 library linked, no Android build/device run. It:

- Verifies preparation’s SAME Level and canonical manager and a sole empty `external.stage_body[9]` slot.
- Defers config borrowing until first state9 call, after actual stage8 publication; looks up the actual Module factory GO4 receiver and identical manager lease/context/key, then borrows its genuine strings. Never writes config38 or casts away const.
- Calls existing `model_renderer::borrow_current_native_level_v27(CanonicalCurrentLevelBorrowV1&,string&)` and matches current GSLevel.s_level.
- Uses existing live `borrow_actual_device_driver_type_v55(int32_t&,string&)`; original Device.IsFixedPipeline381668 is `(type &7)!=0`. Current ActualDeviceProvider validates live EGL/context/display/thread/budget generation and actual GLES2 driver returns8, so the predicate is source-derived. No hardcoded false/device flags.
- Requires its device owner to match the real `dh2::android_resources::budget_lease_v39()` control block. A dummy owner or lost context fails.
- Reuses actual V52 filename resolution + CanonicalCachedFile/manager/class dispatcher/Level160/18c. Assigned stream140 stays an explicit original-stream producer requirement.
- Uses weak callbacks to its owning binding; retained external file journals fail if the binding is destroyed, avoiding raw-this lifetime hazards.

Current root hooks/owners are exact:

| Main owner/API | Actual current place | Binding |
|---|---|---|
| WorldScriptContext.native_level_c1_v25/native_gslevel_v27 | model_renderer.cpp | SAME C1 candidate, never new Level |
| RendererCanonicalWorldV4.manager/properties | renderer_canonical_world_v4.inc:7–13 | Alias this canonical facet; source registry must be the genuine fresh C1 producer before preview/player adoption |
| assign_network / language_added | same include:19–44 | Reuse actual offline/online fact and sole language registry |
| canonical_vec3_origin_v1 | same properties ctor | Genuine ZERO default |
| GSLevel.s_level borrow | model_renderer.cpp exported borrow_current_native_level_v27 | Current Level identity validation |
| actual Device type/current generation | actual_device_android_v54.cpp:34 / ActualDeviceProviderV54 | Existing real renderer-type borrow + original scalar mask |
| mounted cache owner | RendererLevelApplicationV25.files->cache | Existing V52 actual filesystem/mount callback; save files_directory is unrelated |
| actual config factory | CanonicalLevelModuleBindingsV2.level_config(identity) | SAME registered GO4 lease/context/string storage |

**Runtime assembly still open:** current Main imports SourceLoadingV43/nativeGSFrame sources through V50 CMake, but has no SourceLoadingV43 runtime assembly in renderer yet; renderer_native_level_c1_v25.inc explicitly retains C1/GS and logs Level.Init pending. `renderer-stage9-hook-v54.inc.proposed` shows the exact weak World/current manager/Level guard and stage9 attachment at the forthcoming genuine source assembly. `source_modules`, existing actual root-file inputs, same class dispatcher and V52 owner/resolver must be the ones that assembly supplies; they are deliberately not fabricated preview aliases. Existing canonical properties constructor currently passes null LoadTemplate assertion callback; source `_templateName` still requires its authentic assertion9 diagnostic binding.

Real native LightSceneNode/CLight/SceneManager root/automatic list/type, functional Player attachment and full light-set material/filter/driver owners are absent from current Main source examined. V53’s required services remain unbound until Main supplies that one engine owner. This adoption adds the genuine GO19 source receiver; it does not introduce alternate native light engine implementations or claim source registration is GPU lighting.

Apply-check the source patch against current hashes, compile existing owner + native consumer once, then run real source stage9/10 checks. Full lighting/emulator proof remains a separate Main step.
'''
(r/'HANDOFF.md').write_text(text)
for p in Path('C:/Users/adamc/AppData/Local/Temp').glob('dh2-stage9-*v54.py'):shutil.copy2(p,r/p.name)
items=[{'path':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size} for p in r.iterdir() if p.is_file()];digest=hashlib.sha256(json.dumps(items,sort_keys=True).encode()).hexdigest()[:16];z=b/'reports'/('main-stage9-adoption-v54-'+digest+'.zip');assert not z.exists()
with zipfile.ZipFile(z,'w',zipfile.ZIP_DEFLATED) as out:
 for item in items:out.write(r/item['path'],item['path'])
 out.writestr('manifest.json',json.dumps({'version':54,'files':items},indent=2)+'\n')
with zipfile.ZipFile(z) as check:
 for item in items:assert hashlib.sha256(check.read(item['path'])).hexdigest()==item['sha256']
print(json.dumps({'zip':str(z),'sha256':hashlib.sha256(z.read_bytes()).hexdigest(),'payloads':len(items)},indent=2))
