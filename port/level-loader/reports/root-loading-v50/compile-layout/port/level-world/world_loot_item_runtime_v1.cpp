#include "world_loot_item_runtime_v1.hpp"
namespace dh2::character {
WorldLootItemRuntimeV1::WorldLootItemRuntimeV1(data::LootAudioVisualV8::Borrow audiovisual,WorldLootFactoryServicesV1 services)
 :services_(services),manager_(std::move(audiovisual),{this,spawn,invoke}),award_(manager_,{this,position,random_position,local_player,item,constant,interact}){}
RetainedWorldItemObjectV1* WorldLootItemRuntimeV1::receiver(std::uintptr_t id)const noexcept{auto i=receivers_.find(id);return i==receivers_.end()?nullptr:i->second.get();}
bool WorldLootItemRuntimeV1::spawn(void* p,const char* type,const char* name,bool a,bool b,LootItemObjectBorrowV8& out,std::string& e){
 auto& self=*static_cast<WorldLootItemRuntimeV1*>(p);std::shared_ptr<RetainedWorldItemObjectV1> retained;
 if(!self.services_.spawn||!self.services_.spawn(self.services_.context,type,name,a,b,retained,e)){if(e.empty())e="Required canonical Item template Spawn receiver";return false;}
 if(!retained||retained->base().type_f4()!=3||!retained->base().identity()){e="Canonical Item Spawn must publish the actual type3 receiver";return false;}
 const auto id=retained->base().identity();auto inserted=self.receivers_.emplace(id,retained);
 if(!inserted.second&&inserted.first->second!=retained){e="Canonical Item identity resolves a different retained receiver";return false;}
 out=retained->pool_borrow();return true;
}
bool WorldLootItemRuntimeV1::invoke(void* p,const LootItemRequestV8& r,std::string& e){auto& self=*static_cast<WorldLootItemRuntimeV1*>(p);auto* owner=r.object?self.receiver(r.object->identity):nullptr;if(!owner){e="Required same canonical pooled Item receiver";return false;}return owner->pool_operation(r,e);}
bool WorldLootItemRuntimeV1::position(void* p,std::uintptr_t id,const float*& out,std::string& e){auto& s=*static_cast<WorldLootItemRuntimeV1*>(p);if(!s.services_.drop.position){e="Required source drop position160";return false;}return s.services_.drop.position(s.services_.drop.context,id,out,e);}
bool WorldLootItemRuntimeV1::random_position(void* p,std::uintptr_t a,std::uintptr_t b,float out[3],std::string& e){auto& s=*static_cast<WorldLootItemRuntimeV1*>(p);if(!s.services_.drop.random_drop_position){e="Required actual same-Random source scatter";return false;}return s.services_.drop.random_drop_position(s.services_.drop.context,a,b,out,e);}
bool WorldLootItemRuntimeV1::local_player(void* p,std::int32_t i,bool b,std::uintptr_t& out,std::string& e){auto& s=*static_cast<WorldLootItemRuntimeV1*>(p);if(!s.services_.drop.local_player_character){e="Required source PlayerManager local record660";return false;}return s.services_.drop.local_player_character(s.services_.drop.context,i,b,out,e);}
bool WorldLootItemRuntimeV1::item(void* p,std::uintptr_t id,LootItemPickupBorrowV8& out,std::string& e){auto& s=*static_cast<WorldLootItemRuntimeV1*>(p);auto* receiver=s.receiver(id);if(!receiver){e="Required actual spawned Item receiver";return false;}out.item=receiver->inventory().peek();if(!out.item)return true;out.metadata=receiver->inventory().metadata(*out.item);if(!s.services_.pickup_override58||!s.services_.pickup_override58(s.services_.context,*out.item,out.pickup_override58,e)||!out.pickup_override58){if(e.empty())e="Required same spawned ItemInstance source pickup58";return false;}return true;}
bool WorldLootItemRuntimeV1::constant(void* p,const char* a,const char* b,std::int32_t& out,std::string& e){auto& s=*static_cast<WorldLootItemRuntimeV1*>(p);if(!s.services_.drop.constant){e="Required actual PickUpType constants";return false;}return s.services_.drop.constant(s.services_.drop.context,a,b,out,e);}
bool WorldLootItemRuntimeV1::interact(void* p,std::uintptr_t id,std::uintptr_t character,std::string& e){auto& s=*static_cast<WorldLootItemRuntimeV1*>(p);auto* owner=s.receiver(id);if(!owner){e="Required same ItemObject automatic pickup receiver";return false;}return owner->interact(character,e);}
bool WorldLootItemRuntimeV1::erased(std::uintptr_t id,std::string& e){if(manager_.ready()){e="Flush source ItemManager before canonical pooled receiver removal";return false;}receivers_.erase(id);return true;}
}
