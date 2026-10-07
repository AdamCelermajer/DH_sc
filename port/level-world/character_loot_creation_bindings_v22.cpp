#include "character_loot_creation_bindings_v22.hpp"
#include <cstring>
namespace dh2::character {
bool CharacterLootCreationBindingsV22::debug(bool load,const char* key,std::int32_t& out,std::string& e){
 if(!backend_.provider_lease){e="Required retained loot Debug/file backend";return false;}
 if(load){const int status=dh2_character_debug_load(&debug_,&files_);if(status!=1){e="Required source loot Debug.Load status "+std::to_string(status);return false;}return true;}
 std::uint32_t bits{};const int status=dh2_character_debug_get(&bits,&debug_,key,&files_);
 if(status!=1){e="Required source loot Debug.Get status "+std::to_string(status);return false;}std::memcpy(&out,&bits,4);return true;
}
bool CharacterLootCreationBindingsV22::entry(void* p,const data::LootEntryRequestV8& q,std::int32_t& out,std::string& e){
 auto& s=*static_cast<CharacterLootCreationBindingsV22*>(p);using O=data::LootEntryOperationV8;
 if(!s.backend_.provider_lease){e="Required retained loot entry providers";return false;}
 switch(q.operation){case O::debug_load:return s.debug(true,q.key,out,e);case O::debug_query:return s.debug(false,q.key,out,e);
 case O::warrior_count:return s.players_.class_count(263,out,e);case O::mage_count:return s.players_.class_count(290,out,e);case O::rogue_count:return s.players_.class_count(325,out,e);
 case O::assertion:if(s.backend_.assertion)return s.backend_.assertion(s.backend_.context,q,out,e);e="Required source loot assertion continuation";return false;
 }e="Required source loot entry operation";return false;
}
bool CharacterLootCreationBindingsV22::power(void* p,const data::LootPowerRequestV7& q,std::int32_t& out,std::string& e){
 auto& s=*static_cast<CharacterLootCreationBindingsV22*>(p);using O=data::LootPowerOperationV7;
 if(q.operation==O::debug_load)return s.debug(true,q.key,out,e);if(q.operation==O::debug_query)return s.debug(false,q.key,out,e);
 if(q.operation==O::add_power&&q.item&&s.backend_.provider_lease)return s.presentation_.add_power(*q.item,q.power,q.difficulty,s.text_,e);
 e="Required SAME source loot ItemInstance/Power presentation";return false;
}
bool CharacterLootCreationBindingsV22::query(void* p,const data::LootCreationQueryV8& q,data::LootCreationResponseV8& out,std::string& e){auto& s=*static_cast<CharacterLootCreationBindingsV22*>(p);if(!s.backend_.provider_lease||!s.backend_.query){e="Required actual loot GetPlayersCount/current-Level producer";return false;}return s.backend_.query(s.backend_.context,q,out,e);}
bool CharacterLootCreationBindingsV22::notifications(void* p,data::LootTemporaryInventoryV8& inventory,data::ItemInstanceV1& item,std::string& e){auto& s=*static_cast<CharacterLootCreationBindingsV22*>(p);if(!s.backend_.provider_lease||!s.backend_.notifications){e="Required source NULL-character full notifications";return false;}return s.backend_.notifications(s.backend_.context,inventory,item,e);}
WorldItemLootCreationServicesV10 CharacterLootCreationBindingsV22::services()noexcept{WorldItemLootCreationServicesV10 s;s.source.entry={this,entry};s.source.power={this,power};s.source.text=text_;s.source.context=this;s.source.query=query;s.notifications_context=this;s.full_notifications=notifications;return s;}
}
