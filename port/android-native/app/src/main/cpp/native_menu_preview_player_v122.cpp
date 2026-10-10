#include "renderer_menu_preview_domain_v121.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "canonical_spawn_owner_v1.hpp"
#include "source_process_objects_v121.hpp"
#include "native_menu_preview_profile_v122.hpp"
#include "native_menu_preview_player_v122.hpp"
#include "native_menu_preview_lifecycle_v123.hpp"
#include <algorithm>
#include <map>
namespace model_renderer {namespace {
std::map<std::uintptr_t,std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>> preview_records_v122;
class PreviewPlayerV122:public std::enable_shared_from_this<PreviewPlayerV122> {
public:
 MenuPreviewProcessDomainV121 domain;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60> factory;
 std::unique_ptr<dh2::world::CanonicalSpawnAttemptV1> spawn;
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
 std::shared_ptr<NativeMenuPreviewLifecycleV123> lifecycle;
 dh2::world::CanonicalClassReceiverV1 receiver;
 bool load4_attempted{},load4_complete{};
 std::int32_t selected_slot{-1};bool fresh_profile{};
 static bool construct(void* raw,const dh2::world::CanonicalFactoryEntryV1& entry,dh2::world::CanonicalClassReceiverV1& out,std::string& e){
  auto& self=*static_cast<PreviewPlayerV122*>(raw);
  auto name=std::make_shared<const std::string>("PlayerCharacter_0");dh2::world::CanonicalSourceObjectRequestV1 request;
  request.source_lease=name;request.native_spawn_name_v68=name->c_str();request.element=UINT32_MAX;
  if(!self.factory->construct(entry,request,out,e))return false;
  self.receiver=out;self.record=self.factory->find(out.object.identity);
  if(self.record){
   self.record->menu_preview_selected_slot_v122=self.selected_slot;
   self.record->menu_preview_fresh_v122=self.fresh_profile;
   preview_records_v122[out.object.identity]=self.record;
   if(!register_native_menu_preview_lifecycle_character_v123(self.lifecycle,self.factory,self.record,e))return false;
  }return true;
 }
 bool current(std::string& e){auto app=application.lock();if(!app||!app->source_objects_v121()||app->source_objects_v121()->manager()!=domain.objects||!record||!record->actor){e="Retired actual preview Character/process manager";return false;}return true;}
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual(std::string& e){
  if(!current(e))return {};auto out=record->visual?record->visual->visual():nullptr;
  if(!out||record->actor->source_visual()!=reinterpret_cast<std::uintptr_t>(out.get())){e="Required SAME preview Character Visual2d8";return {};}
  return out;
 }
};
}
bool borrow_native_menu_preview_character_v122(const std::shared_ptr<void>& domain,std::uintptr_t id,
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& out,bool& matched,std::string& e){
 out.reset();matched=false;auto at=preview_records_v122.find(id);if(at==preview_records_v122.end())return true;
 auto r=at->second.lock();if(!r||!r->actor||!r->actor->object){preview_records_v122.erase(at);return true;}
 if(!domain||r->services.world.get()!=domain.get()||r->services.world.owner_before(domain)||domain.owner_before(r->services.world))return true;
 matched=true;if(r->actor->object->identity!=id){e="Actual preview Character identity mismatch";return false;}out=std::move(r);e.clear();return true;
}
void forget_native_menu_preview_character_v123(const std::shared_ptr<void>& domain,std::uintptr_t id,
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& r){
 const auto at=preview_records_v122.find(id);if(at==preview_records_v122.end())return;
 if(at->second.lock()==r&&r&&r->services.world==domain)preview_records_v122.erase(at);
}
bool create_native_menu_preview_player_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 AAssetManager* assets,const std::string& directory,std::int32_t slot,bool fresh,MenuPreviewCharacterV121& out,std::string& e){
 if(out.owner||!app||!assets||slot<0||slot>=4){e="Invalid/replayed actual menu CreatePlayer request";return false;}
 auto self=std::make_shared<PreviewPlayerV122>();out.owner=self;self->application=app;
 self->selected_slot=slot;self->fresh_profile=fresh;
 if(!borrow_native_menu_preview_domain_v121(app,assets,directory,self->domain,e))return false;
 auto& d=self->domain;
 if(!d.owner||!d.objects||!d.properties||!d.character_services||!app->source_objects_v121()||app->source_objects_v121()->manager()!=d.objects){e="Required SAME process Character factory/property/resource owners";return false;}
 dh2::world::CanonicalCharacterCandidateServicesV60 services;
 if(!d.character_services(slot,fresh,services,e))return false;
 if(services.world!=d.owner||services.canonical_objects!=d.objects||services.physical_world!=d.physical||services.design!=d.design.get()){e="Menu Character providers addressed a different process resource domain";return false;}
 self->factory=std::make_shared<dh2::world::CanonicalCharacterCandidateFactoryV60>(std::move(services));
 if(!acquire_native_menu_preview_lifecycle_v123(app,d,self->lifecycle,e))return false;
 dh2::world::CanonicalSpawnServicesV1 spawn;spawn.context=self.get();spawn.construct=PreviewPlayerV122::construct;
 spawn.resolve=[](void* raw,auto& h,bool refresh,const auto*& object,auto& e){auto& p=*static_cast<PreviewPlayerV122*>(raw);return p.domain.objects->resolve_handle_v4(h,refresh,object,[](std::string& error){error="Original preview Spawn reached NULL Handle assertion";return false;},e);};
 spawn.virtual38=[](void*,const auto&,bool& value,auto& e){value=true;e.clear();return true;}; // Character virtual38
 spawn.append_pending=[](void* raw,const auto& object,auto& e){return static_cast<PreviewPlayerV122*>(raw)->domain.objects->append_pending(object,e);};
 spawn.receiver=[](void* raw,const auto& object,const dh2::world::CanonicalClassReceiverV1*& out,auto& e){auto& p=*static_cast<PreviewPlayerV122*>(raw);if(p.receiver.object.identity!=object.identity){e="Preview requires actual Flush before duplicate PlayerCharacter_0";return false;}out=&p.receiver;return true;};
 self->spawn=std::make_unique<dh2::world::CanonicalSpawnAttemptV1>(*d.objects,*d.properties,spawn);
 // Whole shipping CreatePlayer3acea4: deferred source Spawn, then its own
 // Save/InitPost/InitFinal dispatch; never an extra starting-kit actor.
 if(!self->spawn->spawn("Character","PlayerCharacter_0",true,true,e))return false;
 if(!self->record){e.clear();return true;} // genuine delivered NULL allocation
 auto& r=*self->record;out.identity=r.actor->object->identity;
 bool player{};if(!r.is_player(player,e))return false;
 if(player&&!r.initialize_player_save(e))return false;
 if(r.save)r.save->set_slot(fresh?-1:slot); // whole SG_SetSlot source NULL guard
 if(player&&!prepare_native_menu_preview_profile_v122(app,d,r,r.preview_profile_v122,e))return false;
 if(!r.initialize(e)||!r.initialize_final_v70(e))return false;
 if(!r.actor->machine){e="Required actual preview Character FSM";return false;}
 r.actor->machine->state().idle_suppressed=0;
 if(r.actor->machine->transition(3,-1,0)<0){e="Actual preview SetIdleState failed";return false;}
 auto visual=self->visual(e);if(!visual)return false;out.visual_root=visual->root_identity();
 out.sg_load=[self](std::uint32_t mask,auto& e){if(!self->current(e)||!self->record->load){if(e.empty())e="Required SAME preview SaveLoad";return false;}
  if(mask==4&&self->load4_attempted&&!self->load4_complete){e="Failed preview SG_Load4 prefix cannot retry";return false;}
  if(mask==4)self->load4_attempted=true;
  if(!self->record->load->load(static_cast<std::int32_t>(mask),e))return false;
  if(mask==4)self->load4_complete=true;return true;};
 out.attach_visual=[self](auto& e){auto v=self->visual(e);if(!v)return false;if(!self->domain.scene){e="Required SAME preview SceneManager";return false;}dh2::world::GameObjectSceneRootBorrowV1 root;return self->domain.scene->borrow_registered_root_v110(v->root_identity(),root,e);};
 out.visual_position=[self](const auto& p,auto& e){auto v=self->visual(e);return v&&v->source_root_set_position_v112(p.data(),e);};
 out.visual_rotation=[self](const auto& q,auto& e){auto v=self->visual(e);if(!v)return false;std::copy(q.begin(),q.end(),v->binding().root.quaternion);v->binding().root.flags|=2u;return v->binding().update_world(v->scene(),e);};
 out.visual_visible=[self](bool value,auto& e){auto v=self->visual(e);return v&&v->source_set_visible_recur_v91(value,e);};
 e.clear();return true;
}
}
