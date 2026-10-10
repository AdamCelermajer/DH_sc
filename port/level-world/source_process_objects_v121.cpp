#include "source_process_objects_v121.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
namespace dh2::world {
SourceProcessObjectsV121::SourceProcessObjectsV121(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app):application_(app){
 manager_=std::make_shared<CanonicalObjectManagerV1>(CanonicalObjectManagerServicesV1{this,local,threat,missing,duplicate,network,published});
}
bool SourceProcessObjectsV121::belongs_to(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app)const noexcept{
 const auto actual=application_.lock();return actual&&app&&actual.get()==app.get()&&!actual.owner_before(app)&&!app.owner_before(actual);
}
bool SourceProcessObjectsV121::current(std::string& e)const{
 auto app=application_.lock();
 if(!app||!manager_||app->source_objects_v121().get()!=this){e="Retired/replaced actual process ObjectManager";return false;}
 e.clear();return true;
}
bool SourceProcessObjectsV121::service_current(std::string& e)const{
 if(!current(e))return false;
 if(!services_owner_||!services_current_||!services_current_(e)){if(e.empty())e="Required actual process ObjectManager service receiver";return false;}
 return true;
}
bool SourceProcessObjectsV121::acquire(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,
 std::shared_ptr<SourceProcessObjectsV121>& out,std::string& e){
 out.reset();if(!app){e="Required actual App for ObjectManager.GetInstance";return false;}
 if(app->source_objects_v121()){
  out=app->source_objects_v121();if(!out->belongs_to(app)){e="Different App owns process ObjectManager";return false;}
  return out->current(e);
 }
 auto actual=std::shared_ptr<SourceProcessObjectsV121>(new SourceProcessObjectsV121(app));
 if(!app->publish_source_objects_v121(actual,e))return false;
 out=std::move(actual);e.clear();return true;
}
bool SourceProcessObjectsV121::bind_services(CanonicalObjectManagerServicesV1 services,std::shared_ptr<void> owner,
 std::function<bool(std::string&)> guard,std::string& e){
 if(!current(e)||!owner||!guard)return false;
 const bool same=services_owner_&&services.context==services_.context&&
  !owner.owner_before(services_owner_)&&!services_owner_.owner_before(owner);
 if(!same&&!manager_->source_initialization_admissible_v121()){
  e="Require actual ObjectManager Flush before changing process service receiver";return false;
 }
 if(!same){lifecycle_={};native_class_domain_produced_=false;}
 services_=services;services_owner_=std::move(owner);services_current_=std::move(guard);e.clear();return true;
}
bool SourceProcessObjectsV121::bind_lifecycle(CanonicalObjectLifecycleV1 lifecycle,std::string& e){
 if(!current(e)||!lifecycle.owner){if(e.empty())e="Required actual process class lifecycle owner";return false;}
 lifecycle_=std::move(lifecycle);e.clear();return true;
}
bool SourceProcessObjectsV121::local(void* p,std::uintptr_t& out,std::string& e){
 auto& self=*static_cast<SourceProcessObjectsV121*>(p);
 if(!self.current(e))return false;
 // The process singleton is acquired before a World exists. Its static
 // forwarding callback is already present in services_, but the World-owned
 // receiver is intentionally not bound until canonical gameplay setup. In
 // the process/menu phase, GetObjectByName's local-player query is the actual
 // App PlayerManager alias; do not mistake the installed trampoline for a
 // bound World receiver.
 if(self.services_owner_){
  if(!self.service_current(e)||!self.services_.local_player){if(e.empty())e="Required actual process ObjectManager local-player receiver";return false;}
  return self.services_.local_player(self.services_.context,out,e);
 }
 auto app=self.application_.lock();auto pm=app->source_player_manager_v59();player::PlayerInfoFieldsV1* local{};
 if(!pm||!pm->get_local_player(0,true,local,e)||!local){if(e.empty())e="Required actual process PlayerInfo lookup";return false;}
 out=local->character660;e.clear();return true;
}
bool SourceProcessObjectsV121::threat(void* p,const char* name,std::int32_t room,bool create,target_providers::Handle16& out,std::string& e){
 auto& self=*static_cast<SourceProcessObjectsV121*>(p);
 if(!self.service_current(e)||!self.services_.highest_threat){if(e.empty())e="Required actual HighestThreatPlayer provider";return false;}
 return self.services_.highest_threat(self.services_.context,name,room,create,out,e);
}
bool SourceProcessObjectsV121::missing(void* p,std::string& e){
 auto& self=*static_cast<SourceProcessObjectsV121*>(p);
 if(!self.service_current(e)||!self.services_.missing_name_debug){if(e.empty())e="Required original missing-name Debug provider";return false;}
 return self.services_.missing_name_debug(self.services_.context,e);
}
bool SourceProcessObjectsV121::duplicate(void* p,CanonicalObjectBorrowV1& object,std::string& e){
 auto& self=*static_cast<SourceProcessObjectsV121*>(p);
 if(!self.service_current(e)||!self.services_.destroy_duplicate){if(e.empty())e="Required actual duplicate class D0";return false;}
 return self.services_.destroy_duplicate(self.services_.context,object,e);
}
bool SourceProcessObjectsV121::network(void* p,CanonicalObjectBorrowV1& object,std::string& e){
 auto& self=*static_cast<SourceProcessObjectsV121*>(p);if(!self.current(e))return false;
 // ObjectManager.Add is also used by the process-only menu preview before a
 // gameplay World receiver exists. In that scope the original AssignObject-
 // NetworkId body reads App.GetOnline.byte5 and returns without mutation for
 // offline startup; only an online World receiver can perform the full path.
 if(self.services_owner_){
  if(!self.service_current(e)||!self.services_.assign_network_id){if(e.empty())e="Required actual process AssignObjectNetworkId receiver";return false;}
  return self.services_.assign_network_id(self.services_.context,object,e);
 }
 auto online=self.application_.lock()->get_online_loading_v55();
 if(!online->byte5()){e.clear();return true;} //343274 genuine offline return.
 e="Required whole positive online ObjectManager.AssignObjectNetworkId";return false;
}
bool SourceProcessObjectsV121::published(void* p,std::int32_t key,const CanonicalObjectBorrowV1& object,std::uintptr_t character,std::string& e){
 auto& self=*static_cast<SourceProcessObjectsV121*>(p);if(!self.current(e))return false;
 // The canonical manager has already published its real map/list/character
 // entries before this host observer runs. This observer belongs to an
 // attached World facade; process-only menu preview objects have no World
 // language registry to notify.
 if(!self.services_owner_){e.clear();return true;}
 self.native_class_domain_produced_=true; //Observed actual Add publication.
 if(!self.service_current(e)||!self.services_.published){if(e.empty())e="Required actual process class publication provider";return false;}
 return self.services_.published(self.services_.context,key,object,character,e);
}
bool SourceProcessObjectsV121::flush(const std::function<bool(CanonicalObjectManagerV1&,std::string&)>& quiescent,std::string& e){
 if(!current(e)||!quiescent){if(e.empty())e="Required actual process ObjectManager delivery barrier";return false;}
 //A completed direct campaign Flush has already observed all class/journal
 //unpublication. Retire the old weak-World delegates on that exact receipt,
 //rather than replaying an expired World's host retirement callbacks.
 if(manager_->source_flush_complete_v121()&&manager_->source_initialization_admissible_v121()){
  lifecycle_={};native_class_domain_produced_=false;
 }
 auto self=shared_from_this();auto s=lifecycle_;s.owner=self;s.require_quiescent=quiescent;
 s.native_storage=[self](auto& receiver,auto& out,auto& e){
  if(&receiver!=self->manager_.get()){e="Changed process manager native-storage receiver";return false;}
  return receiver.borrow_native_storage_v108(self->manager_,out,e);
 };
 if(!s.retire_after_unpublication){
  s.retire_after_unpublication=[self](auto& receiver,auto& e){
   if(&receiver!=self->manager_.get()||self->native_class_domain_produced_){
    e="Required positive process class journal retirement after actual Flush";return false;
   }
   //Only THIS real cold C1 has not produced any external class allocation.
   //The executed Flush already cleared its genuine map/lists/counters.
   e.clear();return true;
  };
 }
 if(!manager_->flush_source_v1(s,e))return false;
 native_class_domain_produced_=false;lifecycle_={};e.clear();return true;
}
}
