#include "world_loot_pickup_v23.hpp"
namespace dh2::character {
bool WorldLootPickupV23::actor(std::uintptr_t id,LootPickupActorV23& out,std::string& e){
 out={};if(!services_.provider_lease||!services_.actor||!services_.actor(services_.context,id,out,e)){if(e.empty())e="Required canonical pickup Character receiver";return false;}
 if(out.identity!=id||!out.receiver_lease||!out.properties||!out.object_of_interest14a4){e="Required SAME pickup Character fields/properties/OOI";return false;}
 if(out.source_is_player&&(!out.gear||!out.gear->ready()||!out.gear->inventory()||out.gear->inventory()->character()!=id||!out.gear->property_view()||out.gear->property_view()->resolved!=out.properties->resolved)){e="Pickup Gear must borrow SAME canonical player properties";return false;}return true;
}
bool WorldLootPickupV23::gear(void* p,std::uintptr_t id,player::PlayerEquipmentRenderOwnerV1*& out,std::string& e){LootPickupActorV23 b;if(!static_cast<WorldLootPickupV23*>(p)->actor(id,b,e))return false;if(!b.source_is_player||!b.gear){e="Required real player pickup Gear";return false;}out=b.gear;return true;}
bool WorldLootPickupV23::quest_player(void* p,std::uintptr_t id,bool& out,std::string& e){LootPickupActorV23 b;if(!static_cast<WorldLootPickupV23*>(p)->actor(id,b,e))return false;out=b.source_is_player;return true;}
bool WorldLootPickupV23::gathering(void* p,std::uintptr_t id,std::int32_t item,bool& out,std::string& e){player::PlayerEquipmentRenderOwnerV1* player{};if(!gear(p,id,player,e))return false;auto* ids=player->gathering_ids_v11();if(!ids){e="Required SAME source inventory gathering list30";return false;}out=ids->contains(item);return true;}
bool WorldLootPickupV23::after(void* p,std::uintptr_t id,std::int32_t item,std::string& e){auto& self=*static_cast<WorldLootPickupV23*>(p);auto q=self.services_.quests;
 // Preserve external context for reached GS/constant/raise calls while using
 // canonical Gear for IsPlayer and list30. No absent-GS fallback is invented.
 struct Context{WorldLootPickupV23* self;LootPickupQuestServicesV10 source;};Context c{&self,q};
 q.context=&c;q.is_player=[](void* raw,std::uintptr_t a,bool& b,std::string& error){return quest_player(static_cast<Context*>(raw)->self,a,b,error);};
 q.registered_gathering_id=[](void* raw,std::uintptr_t a,std::int32_t i,bool& b,std::string& error){return gathering(static_cast<Context*>(raw)->self,a,i,b,error);};
 q.current_game_state=[](void* raw,std::uintptr_t& gs,std::string& error){auto& s=static_cast<Context*>(raw)->source;if(!s.current_game_state){error="Required pickup quest current GS";return false;}return s.current_game_state(s.context,gs,error);};
 q.constant=[](void* raw,const char* a,const char* b,std::int32_t& value,std::string& error){auto& s=static_cast<Context*>(raw)->source;if(!s.constant){error="Required pickup quest literal constant";return false;}return s.constant(s.context,a,b,value,error);};
 q.raise_async=[](void* raw,std::uintptr_t gs,const LootPickupQuestEventV10& event,std::string& error){auto& s=static_cast<Context*>(raw)->source;if(!s.raise_async){error="Required synchronous original pickup quest RaiseAsync";return false;}return s.raise_async(s.context,gs,event,error);};
 return loot_pickup_quest_tail_v10(id,item,q,e);
}
bool WorldLootPickupV23::bind_items(WorldItemLiveOwnerV5& actual,std::string& e){if(items_){e="Pickup Item graph publication must occur once";return false;}items_=&actual;return true;}
bool WorldLootPickupV23::route(const LootInteractRequestV8& q,LootInteractResponseV8& out,std::string& e){
 out={};if(!services_.provider_lease||!items_){e="Required retained canonical Item pickup graph";return false;}
 auto item=items_->factory().find(q.object);if(!item||q.inventory!=&item->inventory()){e="Pickup request must use SAME canonical Item embedded inventory";return false;}
 using O=LootInteractOperationV8;bool handled{};if(!inventory_.route(q,out,handled,e))return false;if(handled)return true;
 switch(q.operation){
 case O::character_cast:{std::uintptr_t id{};if(!services_.character_cast||!services_.character_cast(services_.context,q.character,id,e)){if(e.empty())e="Required actual Item Interact ObjectHandle Character cast";return false;}if(!id)return true;LootPickupActorV23 b;if(!actor(id,b,e))return false;out.character={id,b.properties,b.object_of_interest14a4};return true;}
 case O::is_player:{LootPickupActorV23 b;if(!actor(q.character,b,e))return false;out.value=b.source_is_player;return true;}
 case O::player_id:{player::PlayerInfoFieldsV1* p{};if(!players_.get_by_character(q.character,q.flag,p,e)||!p){if(e.empty())e="Required same pickup PlayerInfo";return false;}out.value=p->friendly678;return true;} // source3ed1f0/3ed640, NOT internal670
 case O::local_player_character:{player::PlayerInfoFieldsV1* p{};if(!players_.get_local_player(q.argument,q.flag,p,e)||!p){if(e.empty())e="Required pickup local PlayerInfo660";return false;}out.identity=p->character660;return true;}
 case O::saved_option:if(!services_.settings||!q.key){e="Required SAME actual saved pickup option";return false;}out.value=services_.settings->saved_option(q.key);return true;
 case O::show_text:if(!services_.enqueue_status){e="Required SAME Application StatusMsg4 queue/movie owner";return false;}return services_.enqueue_status(services_.context,q.text,static_cast<std::uint32_t>(q.argument),e);
 case O::font_color:{if(!q.item){e="Required actual pickup item color";return false;}std::uint32_t color{};if(!item_color_lookup_v2(*q.item,services_.colors,color,e))return false;out.value=static_cast<std::int32_t>(color);return true;}
 case O::pickup_sound:{if(!services_.vox){e="Required actual pickup Vox owner";return false;}const float* p=item->base().vector3(0x160);return world_item_drop_sound_v2(*services_.vox,services_.vox_identity,q.object,static_cast<std::int16_t>(q.argument),p,e);} // source3ed558..598 uses rawItem position160, not cached1a8
 case O::hide_glow:if(!item->fields().glow3cc)return true;break; // whole HideGlow3ebccc NULL branch
 case O::increment_stat:return true; // original IncrementStat3790ec -> IncreaseStat3790e0 literal bx lr
 default:break;
 }
 if(!services_.remaining){e="Required positive pickup source service "+std::to_string(std::uint32_t(q.operation));return false;}
 return services_.remaining(services_.context,q,out,e);
}
}
