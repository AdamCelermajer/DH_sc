#include "canonical_module_graph_v3.hpp"
namespace dh2::world {
bool CanonicalModuleGraphV3::missing(const char* p,std::string& e)const{e=std::string("Required same canonical Module graph ")+p;return false;}
std::shared_ptr<RetainedModuleVisualV3> CanonicalModuleGraphV3::visual(std::uintptr_t id)const{
 for(const auto& entry:entries_){auto p=entry.second->visuals.find(id);if(p!=entry.second->visuals.end())return p->second;}return {};
}
bool CanonicalModuleGraphV3::bind_generated_room_services_v91(
 decltype(CanonicalModuleGraphServicesV3::spawn_zone) spawn,
 decltype(CanonicalModuleGraphServicesV3::zone_init) init,std::string& e){
 if(!entries_.empty()||services_.spawn_zone||services_.zone_init||!spawn||!init){e="Generated rooms require fresh SAME Module graph before C1";return false;}
 services_.spawn_zone=std::move(spawn);services_.zone_init=std::move(init);e.clear();return true;
}
bool CanonicalModuleGraphV3::module_receiver_v91(std::uintptr_t id,std::shared_ptr<void>& pin,CanonicalModuleV1*& actual,std::string& e){
 for(auto& pair:entries_){auto record=pair.second->record.lock();if(record&&record->receiver&&record->receiver->base().identity()==id){pin=record;actual=record->receiver.get();e.clear();return true;}}
 e="Required SAME live Module receiver record";return false;
}
bool CanonicalModuleGraphV3::module_visited_cell_v91(std::uintptr_t id,std::shared_ptr<void>& pin,std::uint8_t*& byte,std::string& e){
 CanonicalModuleV1* actual{};if(!module_receiver_v91(id,pin,actual,e))return false;byte=&actual->source_visited3fc_v91();return true;
}
bool CanonicalModuleGraphV3::bind_condition_services_v75(const ConditionDataInitServicesV3& same,std::string& e){
 if(!entries_.empty()||!same.owner||!same.conditions||!same.construct_condition||!same.initialize_condition||!same.destroy_condition){
  e="Required complete SAME condition services before Module construction";return false;
 }
 const auto& prior=services_.conditions;
 if(prior.owner){
  if(prior.owner.get()!=same.owner.get()||prior.owner.owner_before(same.owner)||same.owner.owner_before(prior.owner)||prior.conditions!=same.conditions){
   e="Module graph cannot replace its actual condition arena";return false;
  }
  e.clear();return true;
 }
 services_.conditions=same;e.clear();return true;
}
bool CanonicalModuleGraphV3::bind(const std::shared_ptr<CanonicalModuleRecordV2>& record,GameObjectInitializationServicesV1& init,ModuleInitServicesV1& module,std::string& error){
 error.clear();if(!record||entries_.count(record.get())||!services_.candidate||!services_.rooms)return missing("fresh record/world owners",error);
 auto entry=std::make_shared<Entry>();entry->record=record;entries_[record.get()]=entry;
 std::weak_ptr<CanonicalModuleGraphV3> weak=shared_from_this();std::weak_ptr<Entry> weak_entry=entry;
 auto actor=[weak_entry](std::string& e)->CanonicalGameObjectBaseOwnerV1*{auto entry=weak_entry.lock();auto record=entry?entry->record.lock():nullptr;if(!record||!record->receiver){e="Released canonical Module receiver";return nullptr;}return &record->receiver->base();};
 auto lookup=[weak_entry](std::uintptr_t id,std::string& e){auto entry=weak_entry.lock();if(entry){auto p=entry->visuals.find(id);if(p!=entry->visuals.end())return p->second;}e="Required SAME retained Module visual receiver";return std::shared_ptr<RetainedModuleVisualV3>{};};
 init.condition_init=[weak,actor](std::uint32_t offset,std::string& e){auto graph=weak.lock();auto* base=actor(e);if(!graph||!base)return false;return condition_data_init_v3(*base,offset,graph->services_.conditions,e);};
 init.update_pf_object=[weak,actor](std::string& e){auto graph=weak.lock();auto* base=actor(e);if(!graph||!base)return false;if(!graph->services_.update_pf)return graph->missing("actual UpdatePFObject",e);return graph->services_.update_pf(*base,e);};
 init.set_position=[weak,actor,lookup](const float* p,bool destination,std::string& e){auto graph=weak.lock();auto* base=actor(e);if(!graph||!base)return false;auto position=graph->services_.position;position.visual_sync=[lookup](std::uintptr_t id,std::string& e){auto visual=lookup(id,e);return visual&&visual->sync(e);};return game_object_set_position_v2(*base,p,destination,position,e);};
 init.visual_sync=[lookup](std::uintptr_t id,std::string& e){auto visual=lookup(id,e);return visual&&visual->sync(e);};
 init.visual_root=[lookup](std::uintptr_t id,std::uintptr_t& root,std::string& e){auto visual=lookup(id,e);if(!visual)return false;root=visual->root_identity();return true;};
 init.visual_set_light_set=[lookup](std::uintptr_t id,std::int32_t light,std::string& e){auto visual=lookup(id,e);if(!visual)return false;visual->set_light_set(light);return true;};
 init.node_from_name=[weak_entry](std::uintptr_t root,const char* name,std::uintptr_t& node,std::string& e){auto entry=weak_entry.lock();if(entry)for(auto& pair:entry->visuals)if(pair.second->root_identity()==root)return pair.second->node_from_name(name,node,e);e="Required SAME Module root name-search receiver";return false;};
 init.load_visual=[weak,weak_entry,actor](std::string& e){
  auto graph=weak.lock();auto entry=weak_entry.lock();auto* base=actor(e);if(!graph||!entry||!base)return false;
  if(!entry->assets){GameObjectVisualAssetServicesV1 asset;asset.owner=graph->services_.candidate;
   asset.construct=[weak,weak_entry](std::uintptr_t parent,const std::string& model,const std::string& xref,std::uintptr_t& out,std::string& e){
    auto graph=weak.lock();auto entry=weak_entry.lock();auto record=entry?entry->record.lock():nullptr;if(!graph||!record||!record->receiver)return false;
    auto& base=record->receiver->base();if(base.identity()!=parent)return graph->missing("constructor parent identity",e);
    if(!graph->services_.read_asset)return graph->missing("actual declared BRES transport",e);
    std::shared_ptr<const std::vector<std::uint8_t>> bytes;bool found=false;if(!graph->services_.read_asset(model,bytes,found,e))return false;
    auto services=graph->services_.visual;services.update_pf=[weak,weak_entry](std::string& e){auto graph=weak.lock();auto entry=weak_entry.lock();auto record=entry?entry->record.lock():nullptr;if(!graph||!record||!record->receiver)return false;if(!graph->services_.update_pf)return graph->missing("actual UpdatePFObject",e);return graph->services_.update_pf(record->receiver->base(),e);};
    auto visual=std::make_shared<RetainedModuleVisualV3>(base,std::move(services));entry->visuals[visual->identity()]=visual;
    if(found){bool selected=false;if(!visual->initialize(bytes,xref.c_str(),selected,e))return false;}else if(!visual->source_asset_miss(e))return false;
    out=visual->identity();return true;
   };
   asset.root=[weak_entry](std::uintptr_t id,std::uintptr_t& root,std::string& e){auto entry=weak_entry.lock();if(entry){auto p=entry->visuals.find(id);if(p!=entry->visuals.end()){root=p->second->root_identity();return true;}}e="Required actual Module visual root";return false;};
   asset.destroy=[weak_entry](std::uintptr_t id,std::string& e){auto entry=weak_entry.lock();if(entry){auto p=entry->visuals.find(id);if(p!=entry->visuals.end()){if(!p->second->release(e))return false;entry->visuals.erase(p);return true;}}e="Required actual Module visual destructor";return false;};
   asset.set_root_game_object=[weak_entry](std::uintptr_t root,std::uintptr_t parent,std::string& e){auto entry=weak_entry.lock();if(entry)for(auto& p:entry->visuals)if(p.second->root_identity()==root)return p.second->set_root_game_object(parent,e);e="Required same root+204 receiver";return false;};
   entry->assets=std::make_unique<GameObjectVisualAssetOwnerV1>(*base,std::move(asset));
  }return entry->assets->load_visual(e);
 };
 module.owner=services_.candidate;
 module.visual_apply_mesh_box=[lookup](std::uintptr_t id,std::string& e){auto visual=lookup(id,e);return visual&&visual->apply_mesh_box(e);};
 module.visual_physical=[lookup](std::uintptr_t id,bool& physical,std::string& e){auto visual=lookup(id,e);if(!visual)return false;physical=visual->physical();return true;};
 module.load_floor=[weak,lookup,actor](std::uintptr_t id,std::int32_t room,const std::string& name,bool solid,const float* bounds,std::string& e){
  auto graph=weak.lock();auto visual=lookup(id,e);auto* base=actor(e);if(!graph||!visual||!base)return false;if(bounds!=base->absolute_aabb12c())return graph->missing("same owner AABB borrow",e);
  std::shared_ptr<ModulePFRoomV3> result;if(!graph->services_.rooms->load(visual->bres(),visual->resource_lease(),visual->scene(),room,name,result,e))return false;
  return !result||graph->services_.rooms->extend_owner_bounds(result,bounds,solid,e);
 };
 module.root_world_bounds=[lookup](std::uintptr_t id,std::array<float,6>& box,std::string& e){auto visual=lookup(id,e);return visual&&visual->root_bounds(box,e);};
 module.spawn_room_zone=services_.spawn_zone;module.zone_init=services_.zone_init;
 return true;
}
bool CanonicalModuleGraphV3::release(const CanonicalModuleRecordV2* key,std::string& e){
 auto p=entries_.find(key);if(p==entries_.end()){e="Required retained Module graph release";return false;}
 auto entry=p->second;auto record=entry->record.lock();
 for(auto i=entry->visuals.begin();i!=entry->visuals.end();){if(!i->second->release(e))return false;i=entry->visuals.erase(i);}
 if(record&&record->receiver){auto& base=record->receiver->base();*base.pointer(0x2d8)=0;
  if(!condition_data_clear_v3(base,0x8c,services_.conditions,e)||!condition_data_clear_v3(base,0xb0,services_.conditions,e))return false;}
 entries_.erase(p);return true;
}
}

namespace dh2::world {
bool CanonicalModuleGraphV3::source_sync_visibility_v94(std::uintptr_t id,std::string& e){
 if(!id)return missing("positive typed Module SyncVisibility identity",e);
 for(const auto& item:entries_){auto entry=item.second;auto record=entry?entry->record.lock():nullptr;
  if(!record||!record->receiver||record->receiver->base().identity()!=id)continue;
  auto& base=record->receiver->base();const auto slot=base.pointer(0x2d8);if(!slot)return missing("SAME Module visual2d8 cell",e);
  if(!*slot){e.clear();return true;} // Original GameObject.SyncVisibility NULL visual branch.
  auto found=entry->visuals.find(*slot);if(found==entry->visuals.end()||!found->second||found->second->identity()!=*slot)return missing("SAME Module-owned VisualObject2d8",e);
  return found->second->sync_visibility_v94(e); //record remains pinned through all reached native stores.
 }
 return missing("live canonical Module record for SyncVisibility",e);
}
}

namespace dh2::world {
bool CanonicalModuleGraphV3::source_set_visual_null_v110(std::uintptr_t id,std::uintptr_t expected,std::string& e){
 for(const auto& item:entries_){auto entry=item.second;auto record=entry?entry->record.lock():nullptr;
  if(!record||!record->receiver||record->receiver->base().identity()!=id)continue;
  auto slot=record->receiver->base().pointer(0x2d8);
  if(!slot||*slot!=expected||!expected||!entry->assets)return missing("SAME batching Module Visual2d8 assignment",e);
  //Qualified SetVisualObject(NULL) only: conditions, class, physics and graph
  //membership remain owned by the live Module. Whole release() is different.
  return entry->assets->set_visual(std::uintptr_t{},e);
 }
 return missing("actual Module visual-assignment owner",e);
}
}
