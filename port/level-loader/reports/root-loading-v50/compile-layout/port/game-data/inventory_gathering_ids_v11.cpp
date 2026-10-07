#include "inventory_gathering_ids_v11.hpp"
namespace dh2::data {
bool InventoryGatheringIdsV11::contains(std::int32_t id)const noexcept{for(const auto& entry:entries_)if(entry.id==id)return true;return false;}
void InventoryGatheringIdsV11::register_id(std::int32_t id){
 for(auto& entry:entries_)if(entry.id==id){entry.references=static_cast<std::uint8_t>(unsigned(entry.references)+1u);return;}
 entries_.push_back({id,1});
}
bool InventoryGatheringIdsV11::unregister_id(std::int32_t id,const InventoryGatheringAssertServicesV11& s,std::string& e){
 for(auto i=entries_.begin();i!=entries_.end();++i)if(i->id==id){i->references=static_cast<std::uint8_t>(unsigned(i->references)-1u);if(!i->references)entries_.erase(i);return true;}
 if(!s.missing_id){e="Required actual inventory gathering missing-ID assertion policy";return false;}
 return s.missing_id(s.context,id,e);
}
}
