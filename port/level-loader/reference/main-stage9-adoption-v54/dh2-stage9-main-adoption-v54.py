from pathlib import Path
import hashlib,json,difflib,subprocess
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');main=Path('C:/Users/adamc/Desktop/workspace/DH_sc');r=b/'reference/main-stage9-adoption-v54';r.mkdir(exist_ok=True)
files=['port/level-world/canonical_property_map_v1.cpp','port/level-loader/native_loading_sources_v50.cmake','port/android-native/app/src/main/cpp/actual_device_android_v54.hpp','port/android-native/app/src/main/cpp/actual_device_android_v54.cpp','port/android-native/app/src/main/cpp/actual_device_provider_v54.hpp','port/android-native/app/src/main/cpp/renderer_native_level_c1_v25.inc','port/android-native/app/src/main/cpp/renderer_canonical_world_v4.inc','port/android-native/app/src/main/cpp/renderer_native_gslevel_v27.hpp','port/android-native/app/src/main/cpp/model_renderer.cpp','port/android-native/app/src/main/cpp/CMakeLists.txt']
snapshots={f:(main/f).read_bytes() for f in files};receipt=[{'path':f,'sha256':hashlib.sha256(raw).hexdigest(),'bytes':len(raw)} for f,raw in snapshots.items()]
for f,raw in snapshots.items():
 if not f.endswith('model_renderer.cpp'):(r/('snapshot-'+Path(f).name)).write_bytes(raw)
schema='port/level-world/canonical_property_map_v1.cpp';old=snapshots[schema].decode().replace('\r\n','\n');new=old.replace('kind!="ObjectBase"&&kind!="GameObject"','kind!="LightPoint"&&kind!="ObjectBase"&&kind!="GameObject"',1);needle='#include "canonical_level_config_module_declarations_v1.inc"';assert needle in new;new=new.replace(needle,needle+'\n  #include "canonical_light_point_declarations_v53.inc"',1)
cmake='port/level-loader/native_loading_sources_v50.cmake';cold=snapshots[cmake].decode().replace('\r\n','\n');cnew=cold.replace('visual_transform_v1.cpp module_draw_frame_v1.cpp)','visual_transform_v1.cpp module_draw_frame_v1.cpp\n  canonical_light_point_v53.cpp canonical_light_point_factory_v53.cpp\n  level_lightset_stage9_v53.cpp canonical_lightset_file_v53.cpp)',1);assert cnew!=cold
patch=[]
for f,pre,after in [(schema,old,new),(cmake,cold,cnew)]:patch.extend(difflib.unified_diff(pre.splitlines(True),after.splitlines(True),fromfile='a/'+f,tofile='b/'+f))
for name in ['canonical_light_point_v53.hpp','canonical_light_point_v53.cpp','canonical_light_point_factory_v53.hpp','canonical_light_point_factory_v53.cpp','level_lightset_stage9_v53.hpp','level_lightset_stage9_v53.cpp','canonical_lightset_file_v53.hpp','canonical_lightset_file_v53.cpp']:
 raw=(b/name).read_text();patch.extend(difflib.unified_diff([],raw.splitlines(True),fromfile='/dev/null',tofile='b/port/level-loader/'+name))
decl=(b/'reference/level-lightset-v53/canonical_light_point_declarations_v53.inc.proposed').read_text();patch.extend(difflib.unified_diff([],decl.splitlines(True),fromfile='/dev/null',tofile='b/port/level-world/canonical_light_point_declarations_v53.inc'))
(r/'current-root-stage9-source-v54.patch').write_text(''.join(patch),newline='\n')
(r/'main_stage9_lightset_binding_v54.hpp').write_text(r'''#pragma once
#include <level_source_loading_v43.hpp>
#include <canonical_lightset_file_v53.hpp>
#include <canonical_light_point_factory_v53.hpp>
#include <canonical_level_module_bindings_v2.hpp>
namespace model_renderer {
struct MainStage9ActualInputsV54 {
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level;
 // Alias only the actual source manager inside its real retained owner facet.
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager;
 std::shared_ptr<dh2::world::CanonicalLevelModuleBindingsV2> modules;
 std::shared_ptr<void> actual_device_owner;
 dh2::loader::CanonicalLightsetFileInputsV53 files;
 // Main's weak-World/current renderer manager+candidate+application guard.
 std::function<bool(std::string&)> validate_scope;
};
class MainStage9LightsetBindingV54 final {
 MainStage9ActualInputsV54 input_;std::unique_ptr<dh2::loader::Stage9BodyV53> body_;
 std::shared_ptr<dh2::loader::CanonicalLightsetFileV53> file_;
 std::uintptr_t config_id_{};std::int32_t config_key_{};
 bool current_config(std::shared_ptr<dh2::world::CanonicalLevelConfigV1>&,std::string&);
 bool validate(std::string&);
public:
 explicit MainStage9LightsetBindingV54(MainStage9ActualInputsV54 in):input_(std::move(in)){}
 dh2::loader::LifecycleStepV36 step(std::string&);
 static bool bind(MainStage9ActualInputsV54,dh2::loader::SourceLoadingInputsV43&,
  std::shared_ptr<MainStage9LightsetBindingV54>&,std::string&);
 const auto& actual_file()const noexcept{return file_;}
};
}
''')
(r/'main_stage9_lightset_binding_v54.cpp.proposed').write_text(r'''#include "main_stage9_lightset_binding_v54.hpp"
#include "actual_device_android_v54.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include <retained_level_module_graph_v1.hpp>
namespace model_renderer {
bool MainStage9LightsetBindingV54::current_config(std::shared_ptr<dh2::world::CanonicalLevelConfigV1>& out,std::string& e){
 auto fields=input_.level->config_fields();if(!fields.config38||!*fields.config38){e="Required stage8 actual published Level.config38";return false;}
 const auto* config=input_.modules->level_config(*fields.config38);
 if(!config){e="Published config38 is not SAME module-factory config receiver";return false;}
 std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* actor{};
 bool found=input_.manager->source_ordered_begin_v38(key,actor);
 for(;found;found=input_.manager->source_ordered_next_v38(key,key,actor)){
  if(actor&&actor->identity==config->identity()){
   // No const_cast: actual registry context is the same originally allocated
   // mutable receiver; fields are only read by stage9. Alias its genuine lease.
   if(!actor->lease||!actor->type_f4||*actor->type_f4!=4||actor->context!=config||!actor->shared_handle||actor->shared_handle->key!=key){e="Required genuine published GO4 config lease/key/context";return false;}
   auto* receiver=static_cast<dh2::world::CanonicalLevelConfigV1*>(actor->context);
   if(body_&&(config_id_!=actor->identity||config_key_!=key)){e="Stage9 config registration changed";return false;}
   out=std::shared_ptr<dh2::world::CanonicalLevelConfigV1>(actor->lease,receiver);config_id_=actor->identity;config_key_=key;e.clear();return true;
  }
 }
 e="Published config38 missing from actual source manager registry";return false;
}
bool MainStage9LightsetBindingV54::validate(std::string& e){
 if(!input_.validate_scope||!input_.validate_scope(e))return false;
 dh2::loader::CanonicalCurrentLevelBorrowV1 actual;
 if(!borrow_current_native_level_v27(actual,e)||actual.level()!=input_.level){e="Stage9 requires SAME published GSLevel.s_level candidate";return false;}
 std::int32_t ignored{};if(!borrow_actual_device_driver_type_v55(ignored,e))return false;
 if(body_){std::shared_ptr<dh2::world::CanonicalLevelConfigV1> config;if(!current_config(config,e))return false;}
 e.clear();return true;
}
dh2::loader::LifecycleStepV36 MainStage9LightsetBindingV54::step(std::string& e){
 using namespace dh2::loader;
 if(!validate(e))return LifecycleStepV36::failed;
 if(!body_){
  std::shared_ptr<dh2::world::CanonicalLevelConfigV1> config;if(!current_config(config,e))return LifecycleStepV36::failed;
  Stage9ConfigBorrowV53 fields;if(!borrow_stage9_config_v53(input_.level,config,fields,e))return LifecycleStepV36::failed;
  auto file_input=input_.files;file_input.validate_current=[this](std::string& error){return validate(error);};
  file_=std::make_shared<CanonicalLightsetFileV53>(std::move(file_input));
  Stage9ServicesV53 services;services.actual_device_owner=input_.actual_device_owner;
  // Original Device.IsFixedPipeline381668 -> driver's virtual5c -> type&7.
  services.is_fixed_pipeline=[](bool& out,std::string& error){std::int32_t type{};if(!borrow_actual_device_driver_type_v55(type,error))return false;out=(static_cast<std::uint32_t>(type)&7u)!=0;error.clear();return true;};
  services.actual_file_loader_owner=file_;services.load_module_file_step=[file=file_](const std::string& name,const char* tag,std::string& error){return file->step(name,tag,error);};
  services.validate_current=[this](std::string& error){return validate(error);};
  body_=std::make_unique<Stage9BodyV53>(std::move(fields),std::move(services));
 }
 return body_->step(e);
}
bool MainStage9LightsetBindingV54::bind(MainStage9ActualInputsV54 actual,dh2::loader::SourceLoadingInputsV43& loading,std::shared_ptr<MainStage9LightsetBindingV54>& out,std::string& e){
 if(!actual.level||!actual.manager||!actual.modules||!actual.actual_device_owner||!actual.validate_scope||!loading.preparation||loading.preparation->level()!=actual.level||loading.manager!=actual.manager||&loading.preparation->manager()!=actual.manager.get()||actual.files.level!=actual.level||actual.files.manager!=actual.manager.get()||!actual.files.candidate_owner||!actual.files.classes.context||!actual.files.classes.construct||!actual.files.actual_filesystem_owner||!actual.files.resolve_filename){e="Require SAME authentic stage9 Level/manager/module factory/source files/device owners";return false;}
 if(loading.external.stage_body[9]){e="Stage9 already has authoritative provider";return false;}
 auto next=std::make_shared<MainStage9LightsetBindingV54>(std::move(actual));
 loading.external.stage_body[9]=[next](std::string& error){return next->step(error);};out=std::move(next);e.clear();return true;
}
}
''')
# Original fixed-pipeline source for exact current driver adapter.
tools=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
asm=subprocess.check_output([str(tools/'llvm-objdump.exe'),'-d','--demangle','--start-address=0x381668','--stop-address=0x3816a8',str(main/'.local-inputs/libDungeonHunter2.so')],text=True);(r/'device-fixed-pipeline-original.asm').write_text(asm)
(r/'current-root-source-receipt-v54.json').write_text(json.dumps({'read_only_main':True,'stage9_packet_immutable':'level-lightset-stage9-v53-6f40d806a11189d0.zip','source_files':receipt},indent=2)+'\n')
print(json.dumps({'schema_patch_lines':len(new.splitlines())-len(old.splitlines()),'cmake_added_existing_owner_source_TUs':4,'new_packet_source_files':8,'binding_proposal':'main_stage9_lightset_binding_v54.cpp.proposed','read_only_root':True},indent=2))
