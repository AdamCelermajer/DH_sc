#include "world_item_live_owner_v5.hpp"
#include <stdexcept>
namespace dh2::character {
world::CanonicalItemFactoryServicesV2 WorldItemLiveOwnerV5::factory_services(){
 auto s=services_.factory;s.item.context=this;s.item.invoke=operation;
 s.item.visual_present=present;s.item.sound_position1a8=sound_position;s.item.full_notifications=full_notifications;return s;
}
WorldLootFactoryServicesV1 WorldItemLiveOwnerV5::pool_services(){
 WorldLootFactoryServicesV1 s;s.context=this;s.spawn=spawn;
 s.pickup_override58=world::CanonicalItemFactoryV2::pickup_override58;s.drop=services_.drop;return s;
}
WorldItemLiveOwnerV5::WorldItemLiveOwnerV5(world::CanonicalObjectManagerV1& manager,
 world::CanonicalPropertyMapV1& map,physical::NativeWorld& physics,
 const navigation::CollisionWorld* geometry,navigation::ObstacleRegistry* obstacles,
 data::LootTablesV2::Borrow tables,data::LootAudioVisualV8::Borrow av,WorldItemLiveServicesV5 s)
 :services_(std::move(s)),physics_(physics),geometry_(geometry),obstacles_(obstacles),
 factory_(manager,map,services_.world,std::move(tables),av,factory_services()),pool_(av,pool_services()){
 if(!services_.world||!services_.roots)throw std::invalid_argument("Required SAME canonical World/Scene root owners for Item live graph");
}
bool WorldItemLiveOwnerV5::ensure(std::uintptr_t id,std::string& e){
 if(detached_){e="Item live physical graph is detached";return false;}
 if(auto i=records_.find(id);i!=records_.end()){
  if(i->second.graph)return true;e="Item graph retains a failed construction prefix; retry unavailable";return false;
 }
 auto item=factory_.find(id);if(!item){e="Required SAME canonical Item receiver before graph construction";return false;}
 auto [it,inserted]=records_.try_emplace(id);(void)inserted;auto& r=it->second;r.item=item;
 WorldItemGraphServicesV3 s;
 if(!services_.graph_services||!services_.graph_services(services_.context,*item,s,e)){
  if(e.empty())e="Required actual Item graph production services";return false;
 }
 r.pf=std::make_unique<WorldItemPfConnectionV4>(*item,services_.world,geometry_,obstacles_);
 s.visual=world::world_item_scene_services_v3(std::move(s.visual),services_.roots,
  [this,id](std::uintptr_t root){auto* g=graph(id);return g?g->visual().constructing_root_v3(root):nullptr;});
 s.visual.update_pf=[this,id](std::string& error){return records_.at(id).pf->update(error);};
 s.physical.update_pf=[this,id](std::string& error){return records_.at(id).pf->update(error);};
 s.initialization.update_pf_object=[this,id](std::string& error){return records_.at(id).pf->update(error);};
 r.graph=std::make_unique<WorldItemGraphV3>(item,physics_,std::move(s));r.pf->physical(&r.graph->physical());
 auto interaction_services=services_.interaction;interaction_services.context=this;interaction_services.invoke=interaction;
 item->bind_interaction(interaction_services);return true;
}
bool WorldItemLiveOwnerV5::operation(void* raw,const WorldItemRequestV1& q,std::int32_t& out,std::string& e){
 auto& self=*static_cast<WorldItemLiveOwnerV5*>(raw);if(!self.ensure(q.object,e))return false;
 bool handled{};if(!self.records_.at(q.object).graph->route(q,out,handled,e))return false;if(handled)return true;
 if(!self.services_.outer_item){e="Required actual Item frame/tooltip/Character receiver "+std::to_string(unsigned(q.operation));return false;}
 return self.services_.outer_item(self.services_.context,q,out,e);
}
bool WorldItemLiveOwnerV5::present(void* raw,std::uintptr_t id,bool& out,std::string& e){auto& self=*static_cast<WorldItemLiveOwnerV5*>(raw);if(!self.ensure(id,e))return false;return self.records_.at(id).graph->visual().present(out,e);}
bool WorldItemLiveOwnerV5::sound_position(void* raw,std::uintptr_t id,const float*& out,std::string& e){auto& self=*static_cast<WorldItemLiveOwnerV5*>(raw);auto item=self.factory_.find(id);if(!item||(out=item->base().vector3(0x1a8))==nullptr){e="Required SAME Item SetDestination1a8 authority";return false;}return true;}
bool WorldItemLiveOwnerV5::full_notifications(void* raw,data::LootTemporaryInventoryV8& inventory,data::ItemInstanceV1& item,std::string& e){auto& self=*static_cast<WorldItemLiveOwnerV5*>(raw);const auto& s=self.services_.factory.item;if(!s.full_notifications){e="Required actual NULL-character full-inventory notifications";return false;}return s.full_notifications(s.context,inventory,item,e);}
bool WorldItemLiveOwnerV5::spawn(void* raw,const char* type,const char* name,bool deferred,bool network,std::shared_ptr<RetainedWorldItemObjectV1>& out,std::string& e){return static_cast<WorldItemLiveOwnerV5*>(raw)->factory_.spawn(type,name,deferred,network,out,e);}
bool WorldItemLiveOwnerV5::interaction(void* raw,const LootInteractRequestV8& q,LootInteractResponseV8& out,std::string& e){
 auto& self=*static_cast<WorldItemLiveOwnerV5*>(raw);
 if(!self.factory_.find(q.object)){e="Required canonical Item interaction identity";return false;}
 if(q.operation==LootInteractOperationV8::despawn)return self.pool_.manager().despawn(q.object,e);
 if(!self.services_.interaction.invoke){e="Required actual Item pickup/quest/presentation receiver "+std::to_string(std::uint32_t(q.operation));return false;}
 return self.services_.interaction.invoke(self.services_.interaction.context,q,out,e);
}
WorldItemGraphV3* WorldItemLiveOwnerV5::graph(std::uintptr_t id)const noexcept{auto i=records_.find(id);return i==records_.end()?nullptr:i->second.graph.get();}
bool WorldItemLiveOwnerV5::precache(std::string& e){return pool_.precache(e);}
bool WorldItemLiveOwnerV5::init_final_v23(std::uintptr_t id,bool& eligible,std::string& e){if(!ensure(id,e))return false;return records_.at(id).graph->visual().init_final_v23(eligible,e);}
bool WorldItemLiveOwnerV5::update_pf_v23(std::uintptr_t id,std::string& e){auto found=records_.find(id);if(detached_||found==records_.end()||!found->second.pf){e="Required SAME assigned Item PF owner";return false;}return found->second.pf->update(e);}
bool WorldItemLiveOwnerV5::interact(std::uintptr_t id,std::uintptr_t character,std::string& e){auto item=factory_.find(id);if(!item||!graph(id)){e="Required initialized SAME pooled Item interaction";return false;}return item->interact(character,e);}
bool WorldItemLiveOwnerV5::update(std::uintptr_t id,std::uint32_t dt,std::uintptr_t ooi,std::string& e){auto item=factory_.find(id);if(detached_||!item||!graph(id)){e="Required attached SAME Item frame owner";return false;}return item->update(dt,ooi,e);}
bool WorldItemLiveOwnerV5::sample_visuals(std::uint32_t ms,std::string& e){for(auto& [id,r]:records_){(void)id;if(!r.graph){e="Required constructed Item visual prefix";return false;}if(!r.graph->visual().update_v3(ms,[this](std::string&){services_.roots->notify_visibility_changed_v3();return true;},e))return false;}return true;}
bool WorldItemLiveOwnerV5::detach_physics(std::string& e){
 for(auto& [id,r]:records_){(void)id;if(!r.graph){e="Cannot detach incomplete Item graph prefix";return false;}if(!r.graph->physical().detach(e))return false;r.pf->suspend();}
 detached_=true;geometry_=nullptr;obstacles_=nullptr;return true;
}
void WorldItemLiveOwnerV5::rebind(const navigation::CollisionWorld* geometry,navigation::ObstacleRegistry* obstacles)noexcept{geometry_=geometry;obstacles_=obstacles;for(auto& [id,r]:records_){(void)id;if(r.pf)r.pf->rebind(geometry,obstacles);}detached_=false;}
bool WorldItemLiveOwnerV5::erased(std::uintptr_t id,std::string& e){
 if(pool_.manager().ready()){e="Flush SAME145 ItemManager before canonical receiver removal";return false;}
 auto actual=factory_.find(id);
 if(actual&&actual->fields().tooltip3c8){
  if(!services_.destroy_tooltip_v104||!services_.destroy_tooltip_v104(services_.context,id,e)){if(e.empty())e="Required ItemObject D1 owned tooltip release";return false;}
  actual->fields().tooltip3c8=0;
 }
 auto i=records_.find(id);if(i!=records_.end()){
  if(i->second.graph&&(!i->second.graph->physical().detach(e)||!i->second.graph->visual().release(e)||!i->second.graph->clear_conditions_v75(e)))return false;
  records_.erase(i);
 }
 if(!pool_.erased(id,e))return false;factory_.erased(id);return true;
}
}
