#include "character_loot_item_source_v9.hpp"
namespace dh2::character {
bool CharacterLootItemManagerV8::spawn_source_v9(LootInventorySourceV9& inventory,
 std::uint32_t index,std::uintptr_t source,const float* position,const float* destination,
 std::uintptr_t character,LootItemObjectBorrowV8& out,const LootItemSourceServicesV9& services,std::string& e){
 e.clear();out={};if(running_||!ready_){e="ItemManager unavailable or unsupported reentry";return false;}
 running_=true;struct Guard{bool& value;~Guard(){value=false;}}guard{running_};
 if(!source){e="Required original ItemManager Spawn NULL-source assertion";return false;}
 if(!inventory.owner||!inventory.count||!inventory.item||!inventory.metadata){e="Required same source inventory borrow";return false;}
 if(index>=inventory.count())return true;
 auto* actual=inventory.item(index);if(!actual){e="Required actual temporary inventory item";return false;}
 const auto* row=inventory.metadata(*actual);if(!row){e="Required actual spawn ItemTable row";return false;}
 const auto category=row->record.words[21];if(category<0||std::size_t(category)>=categories_.size())return true;
 auto& c=categories_[std::size_t(category)];const auto current=c.cursor;c.cursor=current+1;if(c.cursor>4)c.cursor=0;
 auto& slot=c.slots[current];if(slot.active&&!despawn(slot,e))return false;if(!slot.object.identity)return true;
 if(!position||!destination){e="Required actual source/destination world position";return false;}
 LootItemRequestV8 q{LootItemOperationV8::position,&slot.object};q.vector=position;q.flag=true;if(!invoke(q,e))return false;
 q.operation=LootItemOperationV8::destination;q.vector=destination;if(!invoke(q,e))return false;
 q.operation=LootItemOperationV8::init_again;q.vector=nullptr;q.index=index;q.character=character;q.audiovisual=&audiovisual_.rows()[std::size_t(category)];
 const auto count=inventory.count();if(!services.invoke){e="Required original InitAgain source-inventory transport";return false;}
 if(!services.invoke(services.context,{q,&inventory},e))return false;
 if(inventory.count()+1!=count){e="Source ItemObject InitAgain did not transfer one actual item";return false;}
 q.operation=LootItemOperationV8::set_visible;q.flag=true;if(!invoke(q,e))return false;
 *slot.object.enabled85=1;slot.active=true;out=slot.object;return true;
}
}
