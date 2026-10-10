#include "native_menu_preview_lifecycle_v123.hpp"
#include "source_process_objects_v121.hpp"
#include "character_clean_v123.hpp"
#include "character_update_pointers_v105.hpp"
#include "character_ai_groups_v87.hpp"
#include <map>
#include <set>
namespace model_renderer {
namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
using Factory=dh2::world::CanonicalCharacterCandidateFactoryV60;
using Object=dh2::world::CanonicalObjectBorrowV1;
bool same_owner(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
}
class NativeMenuPreviewLifecycleV123:public std::enable_shared_from_this<NativeMenuPreviewLifecycleV123> {
 struct Loan {std::shared_ptr<Factory> factory;std::shared_ptr<Record> record;Object object;std::shared_ptr<dh2::character::ScriptCharacterObject> script;};
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 MenuPreviewProcessDomainV121 domain_;
 std::map<std::uintptr_t,Loan> receivers_;
 std::map<std::uintptr_t,Object> orphans_;
 std::set<std::uintptr_t> closed_;
 bool current(std::string& e)const{
  auto app=application_.lock();auto process=app?app->source_objects_v121():nullptr;
  if(!process||process->manager()!=domain_.objects||!process->belongs_to(app)||!domain_.owner||!domain_.scene||!domain_.physical){e="Retired/replaced same-App preview lifecycle domain";return false;}
  if(domain_.scene->source_delivery_busy_v106()||!domain_.physical->cleanup_delivery_idle_v106()){e="Preview Character lifecycle overlaps scene/physical delivery";return false;}
  e.clear();return true;
 }
 bool borrow(const Object& object,std::shared_ptr<Record>& r,std::string& e)const{
  if(!current(e))return false;const auto at=receivers_.find(object.identity);
  if(at==receivers_.end()||!same_owner(object.lease,at->second.object.lease)||object.context!=at->second.object.context||object.shared_handle!=at->second.object.shared_handle){e="Unknown/different retained preview lifecycle receiver";return false;}
  r=at->second.record;
  if(!r||!r->actor||!r->actor->object||r->actor->object->identity!=object.identity||r->services.world!=domain_.owner||r->services.canonical_objects!=domain_.objects||at->second.factory->find(object.identity)!=r){e="Preview lifecycle lost SAME native Character/factory storage";return false;}
  return true;
 }
 bool disabled(std::uintptr_t id,const std::uint8_t*& out,std::string& e)const{
  out=nullptr;const auto at=receivers_.find(id);
  if(at==receivers_.end()){e="AI pointer references a receiver outside actual preview lifecycle directory";return false;}
  std::shared_ptr<Record> r;if(!borrow(at->second.object,r,e))return false;
  out=r->actor->source_bool_field(0x81);if(!out){e="Unproduced SAME preview ObjectBase.disabled81";return false;}return true;
 }
 bool detach_fx(std::uintptr_t id,std::string& e){
  // Each Character's actual resource owner can have a real FX instance pool.
  // All retained pools detach this receiver before any native allocation dies.
  for(const auto& at:receivers_){
   if(!detach_native_menu_preview_fx_anchors_v123(*at.second.record,at.first,id,e))return false;
  }return true;
 }
 bool d0(const Object& object,std::string& e){
  std::shared_ptr<Record> r;if(!borrow(object,r,e))return false;
  if(closed_.count(object.identity)){e="Repeated actual preview Character D0 delivery";return false;}
  // Positive attachment/auxiliary allocations require their genuine producer.
  // The preview's real fresh Character C1 owns NULL cells; never erase a
  // positive owner merely to let this scene transition continue.
  if(r->actor->position_fields_v7().attached2e0){e="Required actual process preview attached2e0 release owner";return false;}
  auto aux=r->actor->source_auxiliary14ec_v107();
  if(!aux||(*aux&&!r->services.destroy_character_aux14ec_v107)){e="Required actual process preview auxiliary14ec release owner";return false;}
  dh2::world::CharacterCleanServicesV123 clean;
  clean.retire_draws=[this,id=object.identity](auto& error){return retire_native_menu_character_draws_v123(domain_.owner,id,error);};
  clean.detach_fx_anchors=[this,id=object.identity](auto& error){return detach_fx(id,error);};
  clean.borrow_fx_owner=[r](auto& error){return borrow_native_menu_preview_fx_owner_v123(*r,error);};
  if(!dh2::world::character_clean_v123(*r,clean,e))return false;
  auto target=r->actor->source_target_list304_v111();if(!target||!target->destroy(e)){if(e.empty())e="Required SAME preview TargetList304 D1";return false;}
  if(!r->close_after_unpublication(e))return false;
  closed_.insert(object.identity);e.clear();return true;
 }
 bool retire(dh2::world::CanonicalObjectManagerV1& manager,std::string& e){
  if(!current(e)||domain_.objects.get()!=&manager)return false;
  for(auto id:closed_){
   const Object* object{};std::int32_t key{};bool found=manager.source_ordered_begin_v38(key,object);
   while(found){if(object&&object->identity==id){e="Preview Character map remains published after D0";return false;}found=manager.source_ordered_next_v38(key,key,object);}
   const auto at=receivers_.find(id);if(at==receivers_.end()){e="Closed preview receiver journal missing at retirement";return false;}
   const auto& r=at->second.record;
   if(r->target_registered_v62){if(!r->services.world_targets||r->services.world_targets->remove(id)){e="Required actual preview target registration at retirement";return false;}r->target_registered_v62=false;}
   if(!retire_native_menu_preview_script_receiver_v123(*r,id,at->second.script,e))return false;
   if(!at->second.factory->retire_closed_v111(id,e))return false;
   forget_native_menu_preview_character_v123(domain_.owner,id,r);
   orphans_.erase(id);receivers_.erase(at);
  }
  closed_.clear();e.clear();return true;
 }
public:
 NativeMenuPreviewLifecycleV123(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,const MenuPreviewProcessDomainV121& domain):application_(app),domain_(domain){domain_.character_services={};}
 bool belongs_to(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,const MenuPreviewProcessDomainV121& d)const{return application_.lock()==app&&same_owner(domain_.owner,d.owner)&&domain_.objects==d.objects&&domain_.scene==d.scene&&domain_.physical==d.physical;}
 bool register_character(const std::shared_ptr<Factory>& factory,const std::shared_ptr<Record>& r,std::string& e){
  if(!current(e)||!factory||!r||!r->actor||!r->actor->object||r->services.world!=domain_.owner||r->services.canonical_objects!=domain_.objects){if(e.empty())e="Required actual preview factory Character before Add";return false;}
  const auto id=r->actor->object->identity;if(factory->find(id)!=r||receivers_.count(id)){e="Invalid/repeated actual preview lifecycle registration";return false;}
  receivers_.emplace(id,Loan{factory,r,r->actor->canonical(r),r->actor->object});e.clear();return true;
 }
 bool bind(std::string& e){
  if(!current(e))return false;auto self=shared_from_this();dh2::world::CanonicalObjectLifecycleV1 s;s.owner=self;
  s.require_quiescent=[self](auto& manager,auto& error){return self->domain_.objects.get()==&manager&&self->current(error);};
  s.game_object=[self](const auto& object,bool& game,auto& fields,auto& error){
   std::shared_ptr<Record> r;if(!self->borrow(object,r,error)||!object.as_character)return false;
   std::uintptr_t character{};if(!object.as_character(object.context,character,error)||character!=object.identity){error="Required actual preview GameObject conversion";return false;}
   dh2::world::GameObjectInitializationFieldsV62 same;if(!r->actor->inherited_initialization_fields_v62(r,same,error))return false;
   fields={r,object.identity,same.pointer(0x2f4),same.byte(0x2f8),same.byte(0x2fc)};game=true;return true;
  };
  s.room_remove=[](const auto&,auto,auto& error){error="Required actual positive preview Room.DelObject receiver";return false;};
  s.flush_target_list=[self](const auto& object,auto& error){std::shared_ptr<Record> r;if(!self->borrow(object,r,error))return false;auto target=r->actor->source_target_list304_v111();if(!target){error="Required actual preview TargetList304 constructor";return false;}return target->flush_backup_results_v111(error);};
  s.object_delete=[self](const auto& object,auto& error){std::shared_ptr<Record> r;if(!self->borrow(object,r,error))return false;r->actor->source_delete_v111();return true;};
  s.ai_update_pointers=[self](const auto& object,auto& error){std::shared_ptr<Record> r;if(!self->borrow(object,r,error))return false;
   auto& a=*r->actor;auto ai=a.source_ai_pointers_v105();auto& kill=a.kill_fields_v42();
   if(!ai||!kill.produced||!a.object){error="Required SAME produced preview AI/Character pointer cells";return false;}
   return dh2::character::character_update_pointers_v105(a.object->target,*ai,a.source_ooi14a4,kill.killer144c,[self](auto id,auto& disabled,auto& e){return self->disabled(id,disabled,e);},error);
  };
  s.ai_remove_from_group=[self](const auto& object,auto& error){std::shared_ptr<Record> r;return self->borrow(object,r,error)&&dh2::character::source_character_remove_from_group_v87(*r,error);};
  s.online_byte5=[self](bool& online,auto& error){if(!self->current(error))return false;auto actual=self->application_.lock()->get_online_loading_v55();if(!actual){error="Required SAME preview App.GetOnline5";return false;}online=actual->byte5()!=0;return true;};
  s.character_virtual28=[self](const auto& object,bool& player,auto& error){std::shared_ptr<Record> r;return self->borrow(object,r,error)&&r->is_player(player,error);};
  s.network_id108=[self](const auto& object,std::uint16_t& id,auto& error){std::shared_ptr<Record> r;if(!self->borrow(object,r,error))return false;dh2::world::GameObjectInitializationFieldsV62 fields;if(!r->actor->inherited_initialization_fields_v62(r,fields,error))return false;const auto actual=fields.integer(0x108);if(!actual){error="Required actual preview network halfword108";return false;}id=static_cast<std::uint16_t>(*actual);return true;};
  s.class_d0=[self](const auto& object,auto& error){return self->d0(object,error);};
  s.orphan_admit=[self](const auto& object,auto& error){std::shared_ptr<Record> r;if(!self->borrow(object,r,error))return false;if(!self->orphans_.emplace(object.identity,object).second){error="Repeated actual preview orphan admission";return false;}return true;};
  s.native_receiver=[self](auto id,auto& out,auto& error){if(!self->current(error))return false;const auto at=self->orphans_.find(id);if(at==self->orphans_.end()){error="Unknown actual retained preview orphan receiver";return false;}out=at->second;return true;};
  s.retire_after_unpublication=[self](auto& manager,auto& error){return self->retire(manager,error);};
  return application_.lock()->source_objects_v121()->bind_lifecycle(std::move(s),e);
 }
};
namespace {std::map<std::uintptr_t,std::weak_ptr<NativeMenuPreviewLifecycleV123>> preview_lifecycles_v123;}
bool acquire_native_menu_preview_lifecycle_v123(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,const MenuPreviewProcessDomainV121& domain,std::shared_ptr<NativeMenuPreviewLifecycleV123>& out,std::string& e){
 out.reset();if(!app||!app->source_objects_v121()||domain.objects!=app->source_objects_v121()->manager()){e="Required SAME process ObjectManager before preview lifecycle C1";return false;}
 auto& slot=preview_lifecycles_v123[app->identity()];auto owner=slot.lock();
 if(owner&&!owner->belongs_to(app,domain)){e="Preview lifecycle domain replaced before source retirement";return false;}
 if(!owner){
  if(!domain.objects->source_initialization_admissible_v121()){e="Require completed actual ObjectManager Flush before producing a different preview lifecycle receiver";return false;}
  owner=std::make_shared<NativeMenuPreviewLifecycleV123>(app,domain);slot=owner;
 }
 if(!owner->bind(e))return false;out=std::move(owner);e.clear();return true;
}
bool register_native_menu_preview_lifecycle_character_v123(const std::shared_ptr<NativeMenuPreviewLifecycleV123>& owner,const std::shared_ptr<Factory>& factory,const std::shared_ptr<Record>& record,std::string& e){
 if(!owner){e="Required actual preview lifecycle owner before Character publication";return false;}return owner->register_character(factory,record,e);
}
}
