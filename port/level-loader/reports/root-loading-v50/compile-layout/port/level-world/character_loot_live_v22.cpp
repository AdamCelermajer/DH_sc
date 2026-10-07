#include "character_loot_live_v22.hpp"
namespace dh2::character {
CharacterLootLiveV22::CharacterLootLiveV22(skills::CharacterWorldRuntimeV1& world,player::PlayerManagerOwnerV1& players,
 data::LootTablesV2::Borrow tables,data::LootPowerResourcesV7::Borrow powers,data::ItemPowerTablesV5::Borrow definitions,
 data::LootRandom8V2& rng,CharacterLootLiveServicesV22 s)
 :world_(world),players_(players),random_(rng),services_(std::move(s)),
 creation_(tables,std::move(powers),std::move(definitions),rng,services_.creation),
 drop_(world,std::move(tables),{this,actor,player,create,drop}),kill_(drop_,{this,table}){}
bool CharacterLootLiveV22::borrow(std::uintptr_t id,CharacterLootLiveBorrowV22& out,std::string& e){
 out={};if(!services_.provider_lease||!services_.actor){e="Required retained live loot actor provider";return false;}
 if(!services_.actor(services_.context,id,out,e))return false;
 skills::WorldTargetActorBorrowV1 same;
 if(world_.actor(id,&same)||!out.receiver_lease||out.actor.identity!=id||!same.search){e="Required SAME registered canonical loot actor";return false;}
 if(!out.source_type_f4||std::uint32_t(out.actor.type_index)!=*out.source_type_f4||(out.actor.type_index==0)!=bool(same.character)){e="Loot actor type differs from SAME canonical receiver";return false;}
 if(out.actor.type_index==0&&(!out.actor.properties||out.actor.properties->resolved!=same.character->resolved)){e="Loot actor properties differ from SAME Character";return false;}
 if(out.position160!=same.position){e="Loot position must borrow SAME source position160";return false;}
 return true;
}
bool CharacterLootLiveV22::actor(void* p,std::uintptr_t id,skills::WorldLootActorBorrowV8& out,std::string& e){CharacterLootLiveBorrowV22 b;if(!static_cast<CharacterLootLiveV22*>(p)->borrow(id,b,e))return false;out=b.actor;return true;}
bool CharacterLootLiveV22::player(void* p,std::uintptr_t id,bool& out,std::string& e){auto& s=*static_cast<CharacterLootLiveV22*>(p);if(!s.services_.is_player){e="Required loot Character IsPlayer source virtual";return false;}return s.services_.is_player(s.services_.context,id,out,e);}
bool CharacterLootLiveV22::create(void* p,data::LootTemporaryInventoryV8& inv,std::int32_t t,std::int32_t v,std::int32_t b,std::int32_t f,std::string& e){return static_cast<CharacterLootLiveV22*>(p)->creation_.add(inv,t,v,b,f,e);}
bool CharacterLootLiveV22::drop(void* p,data::LootTemporaryInventoryV8& inv,std::uintptr_t a,std::uintptr_t b,std::int32_t index,std::string& e){if(index!=-1){e="Required source explicit participant DropLoot branch";return false;}auto& self=*static_cast<CharacterLootLiveV22*>(p);if(!self.items_){e="Required SAME canonical Item145 pool binding";return false;}return self.items_->drop(inv,a,b,e);}
bool CharacterLootLiveV22::table(void* p,std::uintptr_t id,const std::int32_t*& out,std::string& e){CharacterLootLiveBorrowV22 b;if(!static_cast<CharacterLootLiveV22*>(p)->borrow(id,b,e))return false;if(!b.table101c||!b.actor.properties||b.table101c!=b.actor.properties->resolved+9){e="Required SAME initialized Character table101c/resolved9";return false;}out=b.table101c;return true;}
bool CharacterLootLiveV22::position(void* p,std::uintptr_t id,const float*& out,std::string& e){CharacterLootLiveBorrowV22 b;if(!static_cast<CharacterLootLiveV22*>(p)->borrow(id,b,e))return false;if(!b.position160){e="Required source loot position160";return false;}out=b.position160;return true;}
bool CharacterLootLiveV22::scatter(void* p,std::uintptr_t a,std::uintptr_t b,float out[3],std::string& e){auto& s=*static_cast<CharacterLootLiveV22*>(p);const float* source{};const float* killer{};if(!position(p,a,source,e))return false;if(b&&!position(p,b,killer,e))return false;return character_loot_scatter_connected_v9(s.random_,source,killer,out,e);}
bool CharacterLootLiveV22::local(void* p,std::int32_t index,bool dummy,std::uintptr_t& out,std::string& e){auto& s=*static_cast<CharacterLootLiveV22*>(p);player::PlayerInfoFieldsV1* record{};if(!s.players_.get_local_player(index,dummy,record,e))return false;if(!record){e="Required source local PlayerInfo record";return false;}out=record->character660;return true;}
bool CharacterLootLiveV22::constant(void* p,const char* group,const char* name,std::int32_t& out,std::string& e){auto& s=*static_cast<CharacterLootLiveV22*>(p);const auto& source=s.services_.drop;if(!source.constant){e="Required actual PickUpType script constant";return false;}return source.constant(source.context,group,name,out,e);}
bool CharacterLootLiveV22::bind_pool(WorldLootItemRuntimeV1& pool,std::string& e){if(items_||running_||drop_.pending_inventory()){e="Live loot pool publication must occur once before destructive delivery";return false;}items_=&pool;return true;}
bool CharacterLootLiveV22::precache(std::string& e){if(!items_){e="Required SAME canonical Item145 pool binding";return false;}return items_->precache(e);}
bool CharacterLootLiveV22::route(KillActor56& actor,const KillRequest56& q,KillResponse16& out,bool& handled,std::string& e){
 handled=false;if(q.service!=kill_drop_loot)return true;handled=true;
 if(running_){e="Unsupported destructive live DropLoot reentry";return false;}
 auto previous=failures_.find(actor.identity);if(previous!=failures_.end()){e=previous->second;return false;}
 running_=true;struct Guard{bool& value;~Guard(){value=false;}} guard{running_};
 bool source_handled{};if(!kill_.route(actor,q,out,source_handled,e)){if(e.empty())e="Required ordered live DropLoot continuation";failures_.emplace(actor.identity,e);return false;}
 if(!source_handled){e="Ordered DropLoot source adapter did not handle request";failures_.emplace(actor.identity,e);return false;}return true;
}
}
