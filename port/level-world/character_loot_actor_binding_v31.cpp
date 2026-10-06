#include "character_loot_actor_binding_v31.hpp"
namespace dh2::character {
bool CharacterLootActorBindingV31::fields(std::uintptr_t id,CharacterLootActorFieldsV31& out,skills::WorldTargetActorBorrowV1& same,std::string& e){
 out={};same={};if(!id||!services_.provider_lease||!services_.borrow||!services_.borrow(services_.context,id,out,e)){if(e.empty())e="Required actual canonical loot receiver capability";return false;}
 if(!out.receiver_lease||out.canonical.identity!=id||!out.canonical.type_f4||!out.canonical.shared_handle||world_.actor(id,&same)||same.identity!=id){e="Required SAME registered canonical loot identity/handle";return false;}
 target_providers::Handle16* handle{};target_providers::Registry24* registry{};
 if(world_.handle_borrow(id,&handle,&registry)||handle!=out.canonical.shared_handle){e="Loot must borrow SAME canonical shared Handle";return false;}
 if(*out.canonical.type_f4==0&&(!same.character||!out.properties||out.properties->resolved!=same.character->resolved)){e="Loot must borrow SAME Character resolved sheet";return false;}
 return true;
}
bool CharacterLootActorBindingV31::drop_actor(std::uintptr_t id,CharacterLootLiveBorrowV22& out,std::string& e){
 out={};CharacterLootActorFieldsV31 b;skills::WorldTargetActorBorrowV1 same;if(!fields(id,b,same,e))return false;
 out.actor.identity=id;out.actor.type_index=static_cast<std::int32_t>(*b.canonical.type_f4);out.source_type_f4=b.canonical.type_f4;out.position160=same.position;out.receiver_lease=std::move(b.receiver_lease);
 if(*b.canonical.type_f4==0){out.actor.properties=b.properties;out.table101c=b.properties->resolved+9;out.actor.ai=data::ai_props(ai_,b.properties->resolved[1]);if(!out.actor.ai){e="Required actual Character GetCharAI row for DropLoot";return false;}}
 return true;
}
bool CharacterLootActorBindingV31::is_player(std::uintptr_t id,bool& out,std::string& e){
 auto s=world_.targets().query_services();target_providers::Request24 q{target_providers::virtual_player,0,id,0};std::uintptr_t result{};
 if(!s.invoke||s.invoke(s.context,&q,&result)){e=world_.targets().error();if(e.empty())e="Required source IsPlayer virtual for loot";return false;}out=result!=0;return true;
}
bool CharacterLootActorBindingV31::character_cast(std::uintptr_t id,std::uintptr_t& out,std::string& e){
 out=0;if(!id)return true;target_providers::Handle16 h{};std::uintptr_t live{};if(world_.get_handle(id,&h)||world_.resolve(&h,&live,false)){e=world_.error();return false;}if(!live)return true;
 CharacterLootActorFieldsV31 b;skills::WorldTargetActorBorrowV1 same;if(!fields(live,b,same,e))return false;
 if(!b.canonical.as_character){e="Required canonical virtual AsCharacter for pickup";return false;}return b.canonical.as_character(b.canonical.context,out,e);
}
bool CharacterLootActorBindingV31::pickup_actor(std::uintptr_t id,LootPickupActorV23& out,std::string& e){
 out={};CharacterLootActorFieldsV31 b;skills::WorldTargetActorBorrowV1 same;if(!fields(id,b,same,e))return false;
 if(*b.canonical.type_f4!=0||!b.object_of_interest14a4){e="Required SAME pickup Character OOI field14a4";return false;}
 bool player{};if(!is_player(id,player,e))return false;
 if(player&&(!b.gear||!b.gear->ready()||!b.gear->inventory()||b.gear->inventory()->character()!=id||!b.gear->property_view()||b.gear->property_view()->resolved!=b.properties->resolved)){e="Required SAME actual pickup Gear authority";return false;}
 out={id,std::move(b.receiver_lease),b.properties,b.object_of_interest14a4,b.gear,player};return true;
}
}
