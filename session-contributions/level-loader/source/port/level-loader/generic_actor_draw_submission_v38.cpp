#include "generic_actor_draw_submission_v38.hpp"
#include "scene_material_slot_v39.hpp"
#include "render_eligibility_v40.hpp"
namespace dh2::loader {
bool submit_generic_actor_draw_v38(GenericActorVisualInputV37& input,std::int32_t key,
 const std::function<bool(const ActorDrawPrimitiveReadV38&,std::string&)>& consumer,
 ActorDrawSubmissionResultV38& out,std::string& e){
 ActorDrawSubmissionResultV38 result;ActorVisualConsumeResultV37 visual_result;
 const bool ok=input.consume(key,[&](const ActorVisualReadV37& actor,std::string& e){
  if(!consumer){e="Required synchronous actor draw consumer";return false;}
  for(std::size_t i=0;i<actor.scene.instances.size();++i){
   const auto& instance=actor.scene.instances[i];
   if(instance.node_index>=actor.node_flags.size()){e="Actor draw node visibility index is absent";return false;}
   if(i>=actor.source_mesh_render_enabled.size()){e="Required SAME actor mesh render field";return false;}
   if(!(actor.root_flags&1u)||!(actor.node_flags[instance.node_index]&1u)||!source_rigid_mesh_render_gate_v40(actor.source_mesh_render_enabled[i])){++result.hidden_instances;continue;}
   assets::Mesh mesh{};
   if(dh2_mesh_open(&mesh,&actor.bres,static_cast<std::int32_t>(instance.geometry))!=assets::Error::ok){e="Actor draw actual mesh payload rejected";return false;}
   const world::RetainedGameObjectVisualV1::SkinnedMesh* skin=nullptr;
   for(const auto& candidate:actor.skinned_meshes)if(candidate.instance==i){if(skin){e="Actor draw skin instance is ambiguous";return false;}skin=&candidate;}
   if(instance.controller>=0&&!skin){e="Required actual retained actor skin producer";return false;}
   if(instance.controller<0&&skin){e="Actor draw rigid instance has a foreign skin producer";return false;}
   if(skin&&(skin->skin.geometry!=instance.geometry||skin->positions.size()!=mesh.vertices||skin->palette.size()!=skin->skin.nodes.size())){
    e="Actor draw actual skin geometry/vertex/palette domain differs";return false;
   }
   for(auto material:instance.materials)if(material>=actor.scene.materials.size()){e="Actor draw material binding is out of range";return false;}
   ++result.visible_instances;
   for(std::uint32_t p=0;p<mesh.primitives;++p){
    assets::Primitive primitive{};
    if(dh2_mesh_primitive(&mesh,static_cast<std::int32_t>(p),&primitive)!=assets::Error::ok){e="Actor draw primitive payload rejected";return false;}
    const scene::Material* material=nullptr;
    if(!resolve_retained_scene_material_slot_v39(actor.scene,instance,p,material,e))return false;
    const auto binding=ActorMaterialBindingV38::original_assigned_slot;
    const ActorDrawPrimitiveReadV38 draw{actor,instance,static_cast<std::uint32_t>(i),p,mesh,primitive,
      skin?ActorPositionSpaceV38::retained_skin_world:ActorPositionSpaceV38::raw_attribute_local,
      skin,instance.world,binding,material};
    if(!consumer(draw,e))return false;
    ++result.primitives;if(skin)++result.skinned_primitives;else ++result.static_primitives;
   }
  }
  return true;
 },visual_result,e);
 if(!ok)return false;
 result.visual=visual_result.disposition;out=result;e.clear();return true;
}
}
