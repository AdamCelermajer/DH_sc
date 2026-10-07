#include "source_campaign_batch_compilation_v111.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_noncharacter_virtual_v105.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "native_resource_budget_v38.hpp"
#include <application_services_owner_v5.hpp>
#include <cstring>
#include <canonical_gameobject_graph_v68.hpp>
#include <canonical_character_candidate_v60.hpp>
#include <stdexcept>
#include <admitted_cpu_bytes_v40.hpp>
#include <native_scene_lights_v113.hpp>
#include <android/log.h>
namespace model_renderer {namespace {
#include "native_batch_material_v112.inc"
bool needed(std::string& e,const char* leaf){if(e.empty())e=leaf;return false;}
struct BatchCompileTransportV111 {
 std::weak_ptr<void> world;std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 NativeBatchResourceLoansV111 resources;
 std::int32_t vertices22c{},indices230{};
 std::uintptr_t rendered_node9c{};
 bool driver_ctor_produced{},compiling{};
 bool current(SourceCampaignCandidateBorrowV55& c,std::string& e)const{
  auto w=world.lock();auto l=level.lock();return w&&l&&borrow_source_campaign_candidate_runtime_v61(c,e)&&c.actual_world==w&&c.level==l?true:needed(e,"Retired SAME native batch compile campaign");
 }
 bool driver(std::string& e){if(driver_ctor_produced){e.clear();return true;}SourceCampaignCandidateBorrowV55 c;if(!current(c,e)||!resources.initial_driver_limits)return needed(e,"Actual driver capability9c/a0 loan for CBatchDriver C1");
  std::uint32_t v,i;if(!resources.initial_driver_limits(v,i,e))return false;std::memcpy(&vertices22c,&v,4);std::memcpy(&indices230,&i,4);driver_ctor_produced=true;e.clear();return true;
 }
};
}
bool bind_native_batch_compilation_v110(const SourceCampaignCandidateBorrowV55& actual,NativeBatchResourceLoansV111 resources,dh2::loader::BatchNativeServicesV96& s,std::string& e){
 if(!actual.actual_world||!actual.level||!resources.owner||!resources.assets||!resources.borrow_mesh||!resources.borrow_node)return needed(e,"Actual batch directory/resource loans");
 auto p=std::make_shared<BatchCompileTransportV111>();p->world=actual.actual_world;p->level=actual.level;p->resources=std::move(resources);
 //Per-compiler Scene transport. Source stage21 creates it once; it borrows
 //SAME allocations and carries actual driver/current-node cells, no new Scene.
 if(!s.driver_fields)s.driver_fields=[p](auto& out,auto& e){if(!p->driver(e))return false;out={p,&p->vertices22c,&p->indices230};return true;};
 if(!s.current_rendered_node9c)s.current_rendered_node9c=[p](auto& node,auto& e){SourceCampaignCandidateBorrowV55 c;if(!p->current(c,e)||!p->compiling)return needed(e,"Current rendered9c only inside actual source compile callback");node=p->rendered_node9c;e.clear();return true;};
 if(!s.scene_compile50)s.scene_compile50=[p](const auto& roots,auto& root,bool option,const auto& callback,std::nullptr_t,const std::array<float,3>& point,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<dh2::world::NativeBatchNodeV110> node;std::shared_ptr<dh2::world::NativeBatchMeshV110> mesh;
  if(option||point!=std::array<float,3>{0,0,0}||p->compiling||!p->current(c,e)||!p->driver(e)||!p->resources.borrow_node(root.identity,node,e)||!p->resources.borrow_mesh(root.mesh130,mesh,e)||!node||node->mesh()!=mesh)return needed(e,"Actual compile50 arguments/SAME native node/mesh");
  p->compiling=true;struct Guard{bool& b;~Guard(){b=false;}}guard{p->compiling};dh2::world::NativeBatchCompileServicesV111 services;services.owner=p;services.budget=dh2::android_resources::budget_lease_v39();services.vertices22c=&p->vertices22c;services.indices230=&p->indices230;
  services.current=[p](auto& e){SourceCampaignCandidateBorrowV55 c;return p->current(c,e);};
  services.set_rendered=[p](auto id,auto& e){SourceCampaignCandidateBorrowV55 c;if(!p->current(c,e))return false;p->rendered_node9c=id;e.clear();return true;};
 services.get_rendered=[p](auto& id,auto& e){SourceCampaignCandidateBorrowV55 c;if(!p->current(c,e))return false;id=p->rendered_node9c;e.clear();return true;};
  services.material_pass_v112=[p](const dh2::resources::BresView& image,const dh2::scene::Material& material,dh2::scene::EffectRenderPassV4& out,std::string& e){
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> source;if(!p->current(c,e)||!borrow_source_campaign_condition_world_v70(c,source,e)||!source)return false;
   if(material.effect_file.empty())return selected_batch_material_pass_v112(image,material.effect_uri,material.gles2_technique,out,e);
   if(!source->read_admitted_v81||!source->files_owner)return needed(e,"SAME native APK effect read provider");
   dh2::resources::CpuAdmissionV40 charge;std::vector<std::uint8_t> bytes;bool found{};
   if(!source->read_admitted_v81(material.effect_file,found,bytes,[&](std::uint32_t size,std::string& e){return charge.reserve(dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,size,e);},e)||!found)return needed(e,"Actual external batch effect resource absent");
   if(!charge.commit(e))return false;
   dh2::resources::BresView effect;if(dh2_bres_open(&effect,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return needed(e,"Actual external batch effect BRES invalid");
   return selected_batch_material_pass_v112(effect,material.effect_uri,material.gles2_technique,out,e);
  };
  services.material_values_v113=[p](const dh2::resources::BresView& image,const dh2::scene::Material& material,dh2::world::NativeBatchMaterialValuesV113& out,std::string& e){
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> source;if(!p->current(c,e)||!borrow_source_campaign_condition_world_v70(c,source,e)||!source||!dh2::world::decode_batch_material_values_v113(image,material.id,out,e))return false;
   if(material.effect_file.empty())return dh2::world::decode_batch_effect_values_v113(image,material.effect_uri,material.gles2_technique,out,e);
   if(!source->read_admitted_v81||!source->files_owner)return needed(e,"Actual effect parameter file transport");dh2::resources::CpuAdmissionV40 charge;std::vector<std::uint8_t> bytes;bool found{};
   if(!source->read_admitted_v81(material.effect_file,found,bytes,[&](std::uint32_t n,std::string& e){return charge.reserve(dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,n,e);},e)||!found||!charge.commit(e))return needed(e,"Actual selected effect parameter definitions absent");
   dh2::resources::BresView effect;if(dh2_bres_open(&effect,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return needed(e,"Actual effect parameter BRES invalid");return dh2::world::decode_batch_effect_values_v113(effect,material.effect_uri,material.gles2_technique,out,e);
  };
  const std::weak_ptr<void> world=p->world;
  services.material_lights_v113=[p](const std::shared_ptr<dh2::world::RetainedMeshNodeV91>& mesh,std::uint32_t primitive,
   std::shared_ptr<const dh2::world::NativeBatchMaterialValuesV113> values,std::shared_ptr<dh2::world::NativeMaterialLightsV113>& out,std::string& e){
   SourceCampaignCandidateBorrowV55 c;if(!p->current(c,e)||!c.roots||!mesh||mesh->native_destroyed_v106||!values||!mesh->local_light_v113)return needed(e,"Actual native material/Root.light188 creation transport");
   auto found=mesh->material_lights_v113.find(primitive);
   if(found==mesh->material_lights_v113.end())found=mesh->material_lights_v113.emplace(primitive,std::make_shared<dh2::world::NativeMaterialLightsV113>()).first;
   auto fields=found->second;if(!fields)return needed(e,"Actual SAME CMaterial light parameter fields");
   if(fields->source_creation_v113){out=std::move(fields);e.clear();return true;}
   //CMaterial.initParametersToIdentity5cca58 makes runtime18 NULL. Source
   //40c9a8 constructs EXACT light0..light3 parameter names (not light enums).
   //Retain any already-reached positive ApplySettings store on these SAME
   //cells while installing their previously missing native resource binding.
   for(unsigned i=0;i<4;++i){const auto name=std::string("light")+char('0'+i);
    const dh2::world::NativeBatchMaterialValueV113* value{};const auto local=values->find(name);if(local!=values->end())value=&local->second;else{const auto original=values->effect_defaults.find(name);if(original!=values->effect_defaults.end())value=&original->second;}
    if(!value||fields->source_assignment_v113[i])continue;
    //setMaterialParameter6324c4 runtime18 type mask is 0xa0000: ONLY
    //source17/19 URL strings. Empty and '#' leave actual C1 NULL untouched.
    if((value->source_type!=17&&value->source_type!=19)||value->light_urls.size()!=1)return needed(e,"Actual source light parameter URL/type/count binding");
    const auto& uri=value->light_urls[0];if(uri.empty()||uri=="#")continue;
    std::shared_ptr<dh2::world::NativeLightV113> light;
    if(uri[0]=='#'&&!mesh->local_light_v113(uri.substr(1),light,e))return false;
    //Whole resolveURLs65c874: local root ID first, then actual default
    //Scene GetNode(name,false) and lght subtype. No default light is created.
    if(!light){const auto fragment=uri.find('#');const auto external=fragment==std::string::npos?uri:uri.substr(fragment+1);if(!c.roots->source_find_light_v113(external,light,e))return false;}
    if(light){if(!fields->assign(i,std::move(light),e))return false;}
    else if(__android_log_print(ANDROID_LOG_WARN,"DH2Native","Original material light reference unresolved: %s",uri.c_str())<0)return needed(e,"Actual native unresolved-light diagnostic delivery failed");
   }
   fields->source_creation_v113=std::move(values);out=std::move(fields);e.clear();return true;
  };
  services.gpu.owner=p->resources.owner;
  services.gpu.upload=[world,assets=p->resources.assets](const auto& mesh,auto& e){auto w=world.lock();return w?upload_source_batch_mesh_v111(w,mesh,assets,e):needed(e,"Retired native batch GPU World");};
  services.gpu.release=[world](auto id,auto& e){auto w=world.lock();return w?release_source_batch_mesh_v111(w,id,e):needed(e,"Retired native batch GPU D1 World");};
  services.gpu.invalidate=[world](auto id,auto& e){auto w=world.lock();return w?invalidate_source_batch_mesh_v111(w,id,e):needed(e,"Retired native batch GPU context World");};
  services.game_object_visible=[world](auto id,bool& value,auto& e){auto w=world.lock();SourceCampaignCandidateBorrowV55 c;dh2::world::ObjectUpdateActorV102 receiver;
   if(!w||!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=w||!borrow_source_campaign_object_update_actor_v104(c,id,receiver,e)||!receiver.byte)return false;
   std::uintptr_t character{};if(!receiver.object.as_character||!receiver.object.as_character(receiver.object.context,character,e))return needed(e,"Actual compiled segment Character conversion");bool zonable;
   if(character){if(!source_campaign_character_is_zonable_v104(w,id,zonable,e))return false;}else if(!source_campaign_noncharacter_is_zonable_v105(c,id,zonable,e))return false;
   auto* enabled=receiver.byte(0x80);if(!enabled)return needed(e,"Actual GOBatchSceneNode visibility80");
   if(zonable){auto* zone=receiver.byte(0x2ee);if(!zone)return needed(e,"Actual zonable compiled object2ee");if(*zone){auto* active=receiver.byte(0x2f0);if(!active)return needed(e,"Actual compiled object2f0");if(!*active){value=false;e.clear();return true;}}}
   value=*enabled!=0;e.clear();return true;};
  const bool ok=dh2::world::native_compile_scene_v111(roots,mesh,services,callback,e);
  if(mesh->compiled_v111())mesh->compiled_v111()->root=node;
  return ok&&node->source_compile_setup_v112(mesh->compiled_v111()->solid_batches_v112,e);
 };
 if(!s.quantize_components)s.quantize_components=[p](auto id,bool first,bool second,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<dh2::world::NativeBatchMeshV110> mesh;if(!p->current(c,e)||!p->resources.borrow_mesh(id,mesh,e)||!mesh||!mesh->compiled_v111())return needed(e,"Actual compiled quantization receiver");return mesh->compiled_v111()->quantize(first,second,e);};
 if(!s.flush_mesh_buffers)s.flush_mesh_buffers=[p](auto id,bool first,bool second,bool third,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<dh2::world::NativeBatchMeshV110> mesh;if(!p->current(c,e)||!p->resources.borrow_mesh(id,mesh,e)||!mesh||!mesh->compiled_v111())return needed(e,"Actual compiled FlushMeshBuffers receiver");return mesh->compiled_v111()->flush(mesh,first,second,third,e);};
 const auto prior_retire=s.set_visual_null;
 s.set_visual_null=[p,prior_retire](const dh2::loader::BatchObjectBorrowV96& object,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> source;
  if(!p->current(c,e)||!borrow_source_campaign_condition_world_v70(c,source,e)||!source||!object.visual2d8)return false;
  const auto id=*object.visual2d8;if(!id){e.clear();return true;}
  if(object.archetype48&&(*object.archetype48=="Character"||*object.archetype48=="Player")){
   SourceCampaignCharacterBorrowV62 actor;if(!borrow_source_campaign_character_v62(c.actual_world,object.identity,actor,e)||!actor.character||!actor.character->visual)return false;
   auto visual=actor.character->visual->visual();if(!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=id||(!visual->batch_animation_retained_v112()&&(visual->root_animator_present()||!visual->skinned_meshes().empty())))return needed(e,"SAME actual compiled Character mesh/animator transfer");
   if(!retire_source_campaign_root_geometry_v106(c.actual_world,visual->root_identity(),e))return false;
   return actor.character->visual->source_set_visual_null_batch_v112(e);
  }
  if(object.archetype48&&*object.archetype48=="Module")return prior_retire?prior_retire(object,e):needed(e,"Actual Module visual D1");
  if(source->gameobject_graph_v68){std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
   if(!source->gameobject_graph_v68->borrow_visual(id,visual,e))return false;
   if(visual&&visual->batch_animation_retained_v112()){
    if(!retire_source_campaign_object_geometry_v106(c.actual_world,object.identity,id,e))return false;
    return source->gameobject_graph_v68->destroy_visual_source_v92(object.identity,id,e);
   }
  }
  return prior_retire?prior_retire(object,e):needed(e,"Actual nontransferred source visual deleting destructor");
 };
 e.clear();return true;
}
}
