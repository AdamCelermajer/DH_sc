#include "world_item_loot_creation_v10.hpp"
namespace dh2::character {
bool WorldItemLootCreationV10::create(void* raw,std::int32_t id,std::unique_ptr<data::ItemInstanceV1>& out,std::string& e){auto& self=*static_cast<WorldItemLootCreationV10*>(raw);if(!self.current_){e="Required SAME pending loot inventory for creation";return false;}return self.current_->create(id,out,self.services_.source.text,e);}
bool WorldItemLootCreationV10::store(void* raw,std::unique_ptr<data::ItemInstanceV1>& item,std::string& e){auto& self=*static_cast<WorldItemLootCreationV10*>(raw);if(!self.current_){e="Required SAME pending loot inventory for storage";return false;}return self.current_->store(item,self.services_.source.entry,self.services_.notifications_context,self.services_.full_notifications,e);}
bool WorldItemLootCreationV10::query(void* raw,const data::LootCreationQueryV8& q,data::LootCreationResponseV8& out,std::string& e){auto& self=*static_cast<WorldItemLootCreationV10*>(raw);const auto& s=self.services_.source;if(!s.query){e="Required actual Loot PlayerManager/current-Level query";return false;}return s.query(s.context,q,out,e);}
bool WorldItemLootCreationV10::add(data::LootTemporaryInventoryV8& destination,std::int32_t table,std::int32_t value,std::int32_t power,std::int32_t fixed,std::string& e){
 if(current_){e="Unsupported live loot inventory reentry";return false;}current_=&destination;struct Guard{data::LootTemporaryInventoryV8*& p;~Guard(){p=nullptr;}}guard{current_};
 auto s=services_.source;s.context=this;s.create=create;s.store=store;s.query=query;
 return source_.add(table,value,power,fixed,false,false,s,e);
}
}
