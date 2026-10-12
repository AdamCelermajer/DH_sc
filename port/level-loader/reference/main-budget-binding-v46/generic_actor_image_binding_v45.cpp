#include "generic_actor_image_binding_v45.hpp"
#include "scene_material_slot_v39.hpp"
#include <zip_asset_pack_v1.hpp>
#include <algorithm>
#include <exception>
namespace dh2::loader {
bool actor_image_owner_v45(const std::shared_ptr<world::CanonicalOpenableGraphV21>& graph,
 MaterialImageServicesV43 images,ActorImageOwnerBorrowV45& out,std::string& e){
 if(!graph){e="Required SAME Container image producer";return false;}
 ActorImageOwnerBorrowV45 b;b.receiver_owner=graph;b.receiver_identity=graph->receiver().base().identity();
 b.published_visual2d8=graph->receiver().base().pointer(0x2d8);b.model_uri290=graph->receiver().base().string(0x290);b.visual=graph->visual();b.images=std::move(images);
 if(!b.published_visual2d8||!b.model_uri290){e="Required SAME Container Visual/model source fields";return false;}
 out=std::move(b);e.clear();return true;
}
bool actor_image_owner_v45(const std::shared_ptr<character::RetainedCharacterActorV1>& actor,
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual,MaterialImageServicesV43 images,ActorImageOwnerBorrowV45& out,std::string& e){
 if(!actor){e="Required SAME Character image producer";return false;}
 ActorImageOwnerBorrowV45 b;b.receiver_owner=actor;b.receiver_identity=actor->canonical(actor).identity;
 b.published_visual2d8=&actor->source_visual();b.model_uri290=actor->source_string(0x290);b.visual=std::move(visual);b.images=std::move(images);
 if(!b.model_uri290){e="Required actual Character model CString290 producer";return false;}
 out=std::move(b);e.clear();return true;
}
namespace {
struct AdmittedImageBytes {
 // Admission must outlive the final byte allocation (reverse destruction).
 std::shared_ptr<void> cpu_admission,filesystem_owner;
 std::shared_ptr<const std::vector<std::uint8_t>> bytes;
};
struct Guard {
 const GenericActorImageServicesV45& s;std::int32_t key;const ActorDrawPrimitiveReadV38& d;
 const ActorImageOwnerBorrowV45& b;const std::uint8_t* bytes{};std::size_t size{};std::string source;
 bool live(std::string& e)const {
  // Check the independently pinned Visual first: release clears source arrays.
  if(!b.visual||!b.visual->ready()){e="Actor image Visual was released/uninitialized";return false;}
  const auto* registered=s.manager->object(key);
  if(!registered||registered->identity!=d.actor.receiver.identity||registered->lease!=d.actor.receiver.lease||registered->shared_handle!=d.actor.receiver.shared_handle||
     !registered->shared_handle||registered->shared_handle->key!=key){e="Actor image SAME canonical handle/receiver is stale";return false;}
  if(!b.receiver_owner||b.receiver_identity!=registered->identity||!b.published_visual2d8||*b.published_visual2d8!=d.actor.visual_identity||
     reinterpret_cast<std::uintptr_t>(b.visual.get())!=d.actor.visual_identity||b.visual->root_identity()!=d.actor.root_identity||b.visual->root_game_object()!=registered->identity){e="Actor image foreign receiver/Visual/root producer";return false;}
  const auto roots=s.roots->roots();if(std::find(roots.begin(),roots.end(),d.actor.root_identity)==roots.end()){e="Actor image SAME root is no longer registered";return false;}
  if(&b.visual->scene()!=&d.actor.scene||&b.visual->bres()!=&d.actor.bres||b.visual->bres().bytes!=bytes||b.visual->bres().size!=size||!bytes||!size){e="Actor image SAME Scene/BRES resource changed";return false;}
  if(!b.model_uri290||b.model_uri290->empty()||*b.model_uri290!=source){e="Actor image actual model CString290 changed/missing";return false;}
  if(d.instance_index>=d.actor.scene.instances.size()||&d.actor.scene.instances[d.instance_index]!=&d.instance||d.mesh.image.bytes!=bytes||d.mesh.image.size!=size){e="Actor image foreign/stale authored instance/BRES";return false;}
  if(!(b.visual->scene_flags()&1u)||d.instance.node_index>=b.visual->node_flags().size()||!(b.visual->node_flags()[d.instance.node_index]&1u)||
     d.instance_index>=b.visual->source_mesh_render_enabled_v40().size()||!b.visual->source_mesh_render_enabled_v40()[d.instance_index]){e="Actor image actual source draw became hidden/disabled";return false;}
  const scene::Material* material=nullptr;if(!resolve_retained_scene_material_slot_v39(d.actor.scene,d.instance,d.primitive_index,material,e))return false;
  if(material!=d.bound_material){e="Actor image did not retain actual source material slot";return false;}
  e.clear();return true;
 }
};
}
bool submit_generic_actor_images_v45(GenericActorVisualInputV37& input,std::int32_t key,
 const GenericActorImageServicesV45& services,
 const std::function<bool(const ActorPrimitiveImagesReadV45&,std::string&)>& consumer,
 ActorImageSubmissionResultV45& out,std::string& e){try{
 if(!services.registry_owner||!services.manager||!services.roots||!services.borrow_resources){e="Required actual image registry/root/resource producer";return false;}
 ActorImageSubmissionResultV45 result;ActorDrawSubmissionResultV38 draws;
 const bool ok=submit_generic_actor_draw_v38(input,key,[&](const ActorDrawPrimitiveReadV38& draw,std::string& e){
  if(!consumer){e="Required synchronous source actor image consumer";return false;}
  // This callback is already inside V37's live SAME producer scope.
  const auto* source_bytes=draw.actor.bres.bytes;const auto source_size=draw.actor.bres.size;
  ActorImageOwnerBorrowV45 owner;if(!services.borrow_resources(draw.actor,owner,e)){if(e.empty())e="Required actual actor image resource borrow";return false;}
  if(!owner.visual||!owner.visual->ready()||!owner.receiver_owner||owner.receiver_identity!=draw.actor.receiver.identity||!owner.model_uri290){e="Required live SAME actor image/model producer";return false;}
  const auto source=owner.model_uri290?*owner.model_uri290:std::string{};
  Guard guard{services,key,draw,owner,source_bytes,source_size,source};if(!guard.live(e))return false;
  std::vector<MaterialImageIdentityV43> identities;
  if(!decode_retained_material_images_v43(draw.actor.scene,*draw.bound_material,draw.actor.bres,source,identities,e))return false;
  std::vector<ActorMaterialImageBindingV45> bindings;bindings.reserve(identities.size());
  for(const auto& id:identities){
   if(!guard.live(e))return false;
   ActorMaterialImageBindingV45 binding;
   if(id.state==MaterialImageStateV43::source_no_image){
    if(!read_material_image_v43(id,{},binding.image,e))return false;++result.source_no_image;
   }else {
    if(!owner.images.owner||!owner.images.canonical_uri||!owner.images.read||!services.admit_before_read){e="Required actual canonical image resolver/read/preallocation admission provider";return false;}
    std::string canonical;if(!owner.images.canonical_uri(id,canonical,e)){if(e.empty())e="Actor image canonical resolver failed";return false;}if(!guard.live(e))return false;
    std::string checked_uri;if(!assets::ZipAssetPackV1::key(canonical,checked_uri,e))return false;
    if(canonical!=checked_uri){e="Actor image resolver must return canonical full cache URI";return false;}
    if(!services.admit_before_read(id,canonical,binding.cpu_admission_pin,e)){if(e.empty())e="Required main actor image admission";return false;}if(!guard.live(e))return false;
    if(!binding.cpu_admission_pin){e="Main actor image admission returned no genuine pin";return false;}
    auto read_services=owner.images;
    // Resolve once. Source resolver may not change between admission and read.
    read_services.canonical_uri=[canonical](const MaterialImageIdentityV43&,std::string& out,std::string& e){out=canonical;e.clear();return true;};
    if(!read_material_image_v43(id,read_services,binding.image,e))return false;
    if(!guard.live(e))return false;
    // Pin the main-supplied admission to the ACTUAL returned allocation. This
    // aliases its vector without another byte copy or a parallel budget owner.
    auto allocation=std::make_shared<AdmittedImageBytes>(AdmittedImageBytes{binding.cpu_admission_pin,binding.image.resource_owner,binding.image.bytes});
    binding.image.bytes=std::shared_ptr<const std::vector<std::uint8_t>>(allocation,allocation->bytes.get());
    ++result.actual_image_reads;
   }
   ++result.sampler_bindings;bindings.push_back(std::move(binding));
  }
  if(!guard.live(e))return false;
  const ActorPrimitiveImagesReadV45 read{draw,bindings};if(!consumer(read,e)){if(e.empty())e="Actor image consumer failed";return false;}
  return guard.live(e);
 },draws,e);
 if(!ok)return false;result.draws=draws;out=result;e.clear();return true;
 }catch(const std::exception& x){e=x.what();return false;}}
}
