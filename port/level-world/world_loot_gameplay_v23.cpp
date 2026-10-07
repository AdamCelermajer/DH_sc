#include "world_loot_gameplay_v23.hpp"
#include <cstring>
namespace dh2::character {
WorldLootGameplayV23::WorldLootGameplayV23(skills::CharacterWorldRuntimeV1& w,player::PlayerManagerOwnerV1& p,
 world::CanonicalObjectManagerV1& m,world::CanonicalPropertyMapV1& fields,physical::NativeWorld& physics,
 const navigation::CollisionWorld* geometry,navigation::ObstacleRegistry* obstacles,data::LootPowerResourcesV7::Borrow powers,
 data::LootAudioVisualV8::Borrow av,WorldLootGameplayServicesV23 services)
 :world_(w),players_(p),manager_(m),properties_(fields),physics_(physics),geometry_(geometry),obstacles_(obstacles),
 powers_(std::move(powers)),audiovisual_(std::move(av)),services_(std::move(services)),vox_({this,sound}),vox_identity_(services_.pickup.vox_identity){}
bool WorldLootGameplayV23::current(LootCurrentLevelV23& out,std::string& e){out={};
 if(!services_.current_level||!services_.current_level(services_.context,out,e)){if(e.empty())e="Required actual loot current Level";return false;}
 if(out.identity&&(!out.receiver_lease||!out.difficulty118||!out.phase130)){e="Current loot Level must borrow SAME constructor/load fields";return false;}return true;
}
bool WorldLootGameplayV23::query(void* p,const data::LootCreationQueryV8& q,data::LootCreationResponseV8& out,std::string& e){auto& self=*static_cast<WorldLootGameplayV23*>(p);out={};
 if(q.operation==data::LootCreationOperationV8::player_count){out.value=*self.players_.character_count_field();return true;} // wholeGame.GetPlayersCount, sourcePM6c4
 LootCurrentLevelV23 level;if(!self.current(level,e))return false;out.present=level.identity!=0;if(out.present)out.value=*level.difficulty118;return true;
}
bool WorldLootGameplayV23::notifications(void* p,data::LootTemporaryInventoryV8& inventory,data::ItemInstanceV1& item,std::string& e){auto& self=*static_cast<WorldLootGameplayV23*>(p);const auto& source=self.services_.creation;
 if(!source.notifications){e="Required actual NULL-character inventory full notifications";return false;}return source.notifications(source.context,inventory,item,e);
}
bool WorldLootGameplayV23::graph(void* p,RetainedWorldItemObjectV1& item,WorldItemGraphServicesV3& out,std::string& e){auto& self=*static_cast<WorldLootGameplayV23*>(p);const auto& source=self.services_.items;
 if(!source.graph_services||!source.graph_services(source.context,item,out,e)){if(e.empty())e="Required actual Item cache/device/condition/body services";return false;}
 out.vox=&self.vox_;out.vox_identity=self.vox_identity_;
 const auto id=item.base().identity();auto [it,inserted]=self.destruction_.emplace(id,Destruction{&self,out.destruction_context,out.before_item_destroy});
 if(!inserted){e="Item graph services must bind the SAME receiver once";return false;}
 out.destruction_context=&it->second;out.before_item_destroy=[](void* raw,data::ItemInstanceV1& instance,std::string& error){auto& d=*static_cast<Destruction*>(raw);if(d.before&&!d.before(d.context,instance,error))return false;return d.self->forget_presentation(instance,error);};
 const auto peer_owner=out.physical.peer_owner;
 out.physical.peer_owner=[&self,peer_owner](void* peer,std::uintptr_t& owner,std::string& error){navigation::PhysicalContact contact{};bool handled{};if(!self.borrow_contact(peer,contact,owner,handled,error))return false;if(handled)return true;if(!peer_owner){error="Required actual actor PhysicalObject+8 owner projection";return false;}return peer_owner(peer,owner,error);};
 const auto peer_visible=out.physical.peer_visible;
 out.physical.peer_visible=[&self,peer_visible](std::uintptr_t owner,std::uint8_t& visible,std::string& error){auto item=self.items_?self.items_->factory().find(owner):nullptr;if(item){const auto* actual=item->base().byte(0x80);if(!actual){error="Required SAME Item visible80";return false;}visible=*actual;return true;}if(!peer_visible){error="Required actual peer actor visible80";return false;}return peer_visible(owner,visible,error);};
 out.physical.resolve=[&self,&item](std::uintptr_t input,std::uintptr_t& identity,std::uint32_t& type,std::string& error){if(input!=item.base().identity()){error="POItem GetHandle must query SAME receiver";return false;}const world::CanonicalObjectBorrowV1* actual{};auto& handle=item.base().shared_handle();if(!self.manager_.resolve_handle_v4(handle,false,actual,[](std::string& e){e="Required source NULL Item handle assertion";return false;},error))return false;identity=actual?actual->identity:0;type=0;if(actual){if(!actual->type_f4){error="Required SAME canonical Item type_f4";return false;}type=*actual->type_f4;}return true;};
 const auto as_character=out.physical.as_character;
 out.physical.as_character=[&self,as_character](std::uintptr_t object,std::uintptr_t& character,std::string& error){auto item=self.items_?self.items_->factory().find(object):nullptr;if(item){auto same=item->canonical(item);if(!same.as_character){error="Required canonical Item source AsCharacter";return false;}return same.as_character(same.context,character,error);}if(!as_character){error="Required actual peer Handle AsCharacter";return false;}return as_character(object,character,error);};
 out.initialization.init_pf_object=[&self,&item](bool source_static,const float* position,float radius,std::uintptr_t user,std::string& error){
  if(!self.geometry_||!position||user!=item.base().identity()){error="Required SAME Item PF InitObject floor/position/user";return false;}
  navigation::ObjectInitRequest request{self.geometry_,&item.runtime().object,user,{position[0],position[1],position[2]},radius,std::uint32_t(source_static),0};
  if(dh2_nav_init_object(&request)){error="Actual Item source PF InitObject rejected";return false;}return true;
 };
 out.initialization.set_visible=[&self,id](bool value,std::string& error){auto* graph=self.items_?self.items_->graph(id):nullptr;if(!graph){error="Required SAME Item visibility graph";return false;}return graph->source_visibility_v23(value,error);};
 if(!out.initialization.light_set_id){if(!self.services_.light_names){e="Required actual source LightSet name facet";return false;}out.initialization.light_set_id=[&self](const std::string& name,std::int32_t& value,std::string&){value=self.services_.light_names->get_id(name);return true;};}
 self.item_ids_.push_back(id);return true;
}
bool WorldLootGameplayV23::interaction(void* p,const LootInteractRequestV8& q,LootInteractResponseV8& out,std::string& e){auto& self=*static_cast<WorldLootGameplayV23*>(p);if(!self.pickup_){e="Required retained pickup owner";return false;}return self.pickup_->route(q,out,e);}
int WorldLootGameplayV23::sound(void* p,const sound::VoxPlay3DRequestV2& q,sound::VoxPlay3DResponseV2& out){auto& self=*static_cast<WorldLootGameplayV23*>(p);using O=sound::VoxPlay3DOperationV2;
 if(!q.play||q.play->manager!=self.vox_identity_)return -1;
 if(q.operation==O::disabled){std::uint32_t bits{};if(!self.services_.debug||dh2_character_debug_load(self.services_.debug,&self.services_.debug_files)!=1||dh2_character_debug_get(&bits,self.services_.debug,"IsDisablingSounds",&self.services_.debug_files)!=1){self.error_="Required actual Item sound Debug";return -1;}out.value=bits!=0;return 0;}
 if(q.operation==O::current_level){LootCurrentLevelV23 level;if(!self.current(level,self.error_))return -1;out.identity=level.identity;if(level.identity)std::memcpy(&out.value,level.phase130,sizeof out.value);return 0;}
 if(!self.services_.sound.invoke){self.error_="Required positive loot Vox source service "+std::to_string(unsigned(q.operation));return -1;}return self.services_.sound.invoke(self.services_.sound.context,q,out);
}
bool WorldLootGameplayV23::outer(void* p,const WorldItemRequestV1& q,std::int32_t& out,std::string& e){auto& self=*static_cast<WorldLootGameplayV23*>(p);if(!self.items_){e="Required SAME retained Item live graph";return false;}
 if(q.operation==WorldItemOperationV1::collision_interact_type){
  auto item=self.items_->factory().find(q.object);if(!item){e="Required SAME canonical Item InteractionType receiver";return false;}
  // Whole GetInteractionType3ebeb4: actual IsInteractive virtual first,
  // optional SAME inventory.GetItem(0), then unconditional return -1. The
  // loaded item's pickup metadata is NOT this interaction-type return value.
  bool interactive{};if(!item->is_interactive(interactive,e))return false;
  if(interactive&&!item->inventory().peek()){e="Interactive Item lost source GetItem(0) backing";return false;}
  out=-1;return true;
 }
 if(q.operation==WorldItemOperationV1::game_update||q.operation==WorldItemOperationV1::stop||q.operation==WorldItemOperationV1::is_at_destination){
  auto* graph=self.items_->graph(q.object);if(!graph){e="Required SAME Item frame graph";return false;}
  auto it=self.frames_.find(q.object);if(it==self.frames_.end()){
   WorldItemFrameServicesV5 s;if(!self.services_.frame_services||!self.services_.frame_services(self.services_.context,*graph,s,e)){if(e.empty())e="Required actual Item camera/PF/path/frame facts";return false;}
   s.begin_update=[&self](RetainedWorldItemObjectV1&,std::string& error){return world::gameobject_update_begin_v23(self.services_.update_prefix,error);};
   s.end_update=[](RetainedWorldItemObjectV1&,std::string& error){return world::gameobject_update_end_v23(error);};
   s.collision_interact=[&self,id=q.object](std::uintptr_t user,std::string& error){auto actual=self.items_->factory().find(id);if(!actual){error="Required SAME source collision Item receiver";return false;}return actual->interact_from_update_v11(user,error);};
   it=self.frames_.emplace(q.object,std::make_unique<WorldItemFrameV5>(*graph,std::move(s))).first;
  }else if(q.operation==WorldItemOperationV1::game_update){
   //Borrow current path/workspace/scene policy on every actual Update. The
   //retained Item frame is not an authority for stale floor or path capacity.
   WorldItemFrameServicesV5 s;if(!self.services_.frame_services||!self.services_.frame_services(self.services_.context,*graph,s,e))return false;
   s.begin_update=[&self](RetainedWorldItemObjectV1&,std::string& error){return world::gameobject_update_begin_v23(self.services_.update_prefix,error);};
   s.end_update=[](RetainedWorldItemObjectV1&,std::string& error){return world::gameobject_update_end_v23(error);};
   s.collision_interact=[&self,id=q.object](std::uintptr_t user,std::string& error){auto actual=self.items_->factory().find(id);if(!actual){error="Required SAME source collision Item receiver";return false;}return actual->interact_from_update_v11(user,error);};
   it->second->rebind(std::move(s));
  }
  it->second->absolute_time(self.absolute_ms_);bool handled{};if(!it->second->route(q,out,handled,e))return false;if(!handled){e="Original Item frame route not handled";return false;}return true;
 }
 if(!self.services_.items.outer_item){e="Required positive outer Item source operation "+std::to_string(unsigned(q.operation));return false;}return self.services_.items.outer_item(self.services_.items.context,q,out,e);
}
bool WorldLootGameplayV23::initialize(std::string& e){
 if(!factory_prepared_&&!prepare_source_factory_v88(e))return false;
 return precache_source_pool_v88(e);
}
bool WorldLootGameplayV23::prepare_source_factory_v88(std::string& e){if(attempted_){e="Loot factory preparation retains reached prefix; retry unavailable";return false;}attempted_=true;
 if(!services_.application_lease||(!services_.factory_source_v88&&(!services_.gear||!services_.gear->ready()))||!services_.debug||!services_.presentation||!powers_||!audiovisual_||!services_.items.world||!services_.items.roots||!geometry_||!obstacles_||!vox_identity_){e="Required actual retained World/Arrays/Text/Item/Debug loot graph";return false;}
 data::LootTablesV2::Borrow tables;data::ItemPowerTablesV5::Borrow definitions;data::ItemTextServicesV5 text;data::LootRandom8V2* rng{};
 const bool bound_sources=services_.factory_source_v88?services_.factory_source_v88(services_.context,tables,definitions,text,rng,e):services_.gear->loot_sources_v8(tables,definitions,text,rng,e);
 if(!bound_sources||!tables||!definitions||!text.metadata||!text.invoke||!rng)return false;
 auto backend=services_.creation;backend.context=this;backend.query=query;
 // Preserve contexts for reached assertion/full-notification callbacks.
 backend.assertion=[](void* raw,const data::LootEntryRequestV8& q,std::int32_t& value,std::string& error){auto& s=static_cast<WorldLootGameplayV23*>(raw)->services_.creation;if(!s.assertion){error="Required actual source loot assertion";return false;}return s.assertion(s.context,q,value,error);};
 backend.notifications=notifications;
 creation_=std::make_unique<CharacterLootCreationBindingsV22>(players_,*services_.debug,services_.debug_files,*services_.presentation,text,std::move(backend));
 auto source=services_.loot;source.creation=creation_->services();loot_=std::make_unique<CharacterLootLiveV22>(world_,players_,tables,powers_,definitions,*rng,std::move(source));
 auto pickup=std::move(services_.pickup);pickup.vox=&vox_;pickup_=std::make_unique<WorldLootPickupV23>(players_,std::move(pickup));
 auto items=services_.items;items.context=this;items.graph_services=graph;items.outer_item=outer;items.drop=loot_->drop_services();items.interaction={this,interaction,creation_->services().source.entry};
 items.factory.item.inventory_debug=creation_->services().source.entry;items.factory.item.context=this;items.factory.item.full_notifications=notifications;
 items_=std::make_unique<WorldItemLiveOwnerV5>(manager_,properties_,physics_,geometry_,obstacles_,tables,audiovisual_,std::move(items));
 if(!loot_->bind_pool(items_->pool(),e)||!pickup_->bind_items(*items_,e))return false;
 factory_prepared_=true;e.clear();return true;
}
bool WorldLootGameplayV23::precache_source_pool_v88(std::string& e){
 if(!factory_prepared_||!items_||precache_attempted_){e="Required SAME prepared Item factory and single stage29 PreCache";return false;}
 precache_attempted_=true;
 if(!items_->precache(e))return false;ready_=true;e.clear();return true;
}
bool WorldLootGameplayV23::route(KillActor56& actor,const KillRequest56& q,KillResponse16& out,bool& handled,std::string& e){handled=q.service==kill_drop_loot;if(!handled)return true;if(!ready_||!loot_){e="Required initialized actual145 loot graph";return false;}return loot_->route(actor,q,out,handled,e);}
bool WorldLootGameplayV23::initialize_final(std::string& e){if(!ready_||frame_failed_||final_attempted_){e="Required fresh ready Item pool before source InitFinal; no replay";return false;}final_attempted_=true;for(auto id:item_ids_){bool eligible{};if(!items_->init_final_v23(id,eligible,e)){frame_failed_=true;return false;}}return true;}
bool WorldLootGameplayV23::update_item_source_v104(std::uintptr_t id,std::uint32_t ms,std::uint32_t dt,std::string& e){
 if(!factory_prepared_||!items_||frame_failed_){e="Item visitor requires SAME prepared factory/retained successful frame prefix";return false;}
 auto item=items_->factory().find(id);if(!item){e="ObjectManager Item visitor has a foreign source receiver";return false;}
 absolute_ms_=ms;std::uintptr_t ooi{};
 if(item->fields().tooltip3c8&&(!services_.tooltip_ooi||!services_.tooltip_ooi(services_.context,item->fields().tooltip_character3c4,ooi,e))){frame_failed_=true;return false;}
 if(!items_->update_source_item_v104(id,dt,ooi,e)){frame_failed_=true;return false;}return true;
}
bool WorldLootGameplayV23::update(std::uint32_t ms,std::uint32_t dt,std::string& e){if(!ready_||frame_failed_){e="Loot frame retains incomplete/failed source prefix";return false;}absolute_ms_=ms;
 for(auto id:item_ids_){auto item=items_->factory().find(id);if(!item||!item->base().lifecycle().updating85)continue;
  // Renderer transport visits real Item receivers selected by source85.
  // This does not consume or replace ObjectManager's pending-start queue.
  if(!update_item_source_v104(id,ms,dt,e))return false;
 }return true;
}
bool WorldLootGameplayV23::draw_scenes(std::vector<LootDrawSceneV23>& out,std::string& e){out.clear();if(!ready_||frame_failed_){e="Required ready loot presentation graph";return false;}
 for(auto id:item_ids_){auto item=items_->factory().find(id);if(!item)continue;auto* graph=items_->graph(id);if(!graph){e="Required actual retained Item draw graph";return false;}auto visual=graph->visual().visual();if(!visual||!visual->ready()){e="Required actual Item source visual";return false;}if((visual->scene_flags()&1u)!=0)out.push_back({id,std::move(visual)});}return true;
}
bool WorldLootGameplayV23::borrow_contact(void* peer,navigation::PhysicalContact& out,std::uintptr_t& owner,bool& handled,std::string& e){handled=false;owner=0;if(!items_)return true;for(auto id:item_ids_){auto* graph=items_->graph(id);if(graph&&peer==&graph->physical()){handled=true;owner=id;return world_item_physical_contact_v4(*graph,out,e);}}return true;}
bool WorldLootGameplayV23::source_visuals_v104(std::vector<std::shared_ptr<world::RetainedGameObjectVisualV1>>& out,std::string& e){
 out.clear();if(!ready_||frame_failed_||!items_){e="Required actual stage29 Item visual graph";return false;}
 for(auto id:item_ids_){auto item=items_->factory().find(id);auto* graph=items_->graph(id);if(!item||!graph){e="Retired actual Item visual graph before source scene capture";return false;}
  const auto* field=item->base().pointer(0x2d8);if(!field){e="Required SAME source Item visual2d8";return false;}if(!*field)continue;
  auto visual=graph->visual().visual();if(!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=*field||!visual->ready()){e="Required SAME initialized Item visual receiver";return false;}out.push_back(std::move(visual));
 }e.clear();return true;
}
bool WorldLootGameplayV23::interact(std::uintptr_t item,std::uintptr_t character,std::string& e){if(!ready_||frame_failed_){e="Required ready actual loot graph for pickup";return false;}return items_->interact(item,character,e);}
bool WorldLootGameplayV23::detach_physics(std::string& e){if(!items_){e="Required retained actual Item physical graph";return false;}if(!items_->detach_physics(e))return false;frames_.clear();geometry_=nullptr;obstacles_=nullptr;return true;}
bool WorldLootGameplayV23::release(std::string& e){
 if(!items_){e="Required SAME ItemManager release graph";return false;}ready_=false;factory_prepared_=false;
 if(!items_->pool().manager().flush(e))return false;
 for(auto id:item_ids_)if(!items_->erased(id,e))return false;
 frames_.clear();item_ids_.clear();destruction_.clear();return true;
}
void WorldLootGameplayV23::rebind(const navigation::CollisionWorld* geometry,navigation::ObstacleRegistry* obstacles,std::string& e){if(!items_||!geometry||!obstacles){e="Required actual replacement Item floor/obstacles";return;}geometry_=geometry;obstacles_=obstacles;items_->rebind(geometry,obstacles);frames_.clear();e.clear();}
bool WorldLootGameplayV23::forget_presentation(data::ItemInstanceV1& item,std::string& e)noexcept{if(!services_.presentation){e="Required same loot ItemPower lifetime owner";return false;}return services_.presentation->forget(item,e);}
}
