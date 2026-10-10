#include "character_loot_item_manager_v8.hpp"
#include <cstdio>
namespace dh2::character {
bool CharacterLootItemManagerV8::invoke(const LootItemRequestV8& q,std::string& e){if(!services_.invoke){e="Required original world ItemObject lifecycle provider at "+std::to_string(std::uint32_t(q.operation));return false;}return services_.invoke(services_.context,q,e);}
bool CharacterLootItemManagerV8::despawn(Slot& slot,std::string& e){
 auto& o=slot.object;if(!o.identity)return true;
 if(!o.category3ac||!o.enabled85){e="Required same retained ItemObject category/enabled fields";return false;}
 auto category=*o.category3ac;if(category<0||std::size_t(category)>=categories_.size())return true;
 Slot* actual{};for(auto& s:categories_[std::size_t(category)].slots)if(s.object.identity==o.identity){actual=&s;break;}if(!actual)return true;
 actual->active=false;LootItemRequestV8 q{LootItemOperationV8::set_visible,&o};q.flag=false;if(!invoke(q,e))return false;
 q.operation=LootItemOperationV8::remove_all;q.flag=true;if(!invoke(q,e))return false;
 q.operation=LootItemOperationV8::physical;q.flag=false;if(!invoke(q,e))return false;
 *o.enabled85=0;return true;
}
bool CharacterLootItemManagerV8::precache(std::string& e){
 e.clear();if(running_||attempted_){e="Unsupported ItemManager PreCache repeat/reentry";return false;}running_=true;attempted_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 if(!audiovisual_){e="Required actual loot audiovisual cache";return false;}
 const auto& rows=audiovisual_.rows();categories_.reserve(rows.size());
 for(std::size_t category=0;category<rows.size();++category){categories_.emplace_back();auto& c=categories_.back();
  for(unsigned n=0;n<5;++n){char name[64];std::snprintf(name,sizeof(name),"ItemObject_%02u_%02u",unsigned(category),n);
   if(!services_.spawn_object){e="Required original ObjectManager Item template spawn";return false;}
   auto& o=c.slots[n].object;if(!services_.spawn_object(services_.context,"Item",name,false,true,o,e))return false;
   if(o.identity){if(o.type_index!=3||!o.category3ac||!o.enabled85){e="Required actual spawned source ItemObject type3/field ownership";return false;}
    LootItemRequestV8 q{LootItemOperationV8::init_once,&o,&rows[category]};q.index=unsigned(category);if(!invoke(q,e))return false;if(*o.category3ac!=std::int16_t(category)){e="Source ItemObject InitOnce did not store actual category";return false;}
   }
  }
  for(auto& slot:c.slots)if(!despawn(slot,e))return false;
 }ready_=true;return true;
}
bool CharacterLootItemManagerV8::spawn(data::LootTemporaryInventoryV8& inventory,std::uint32_t index,std::uintptr_t source,const float* position,const float* destination,std::uintptr_t character,LootItemObjectBorrowV8& out,std::string& e){
 e.clear();out={};if(running_||!ready_){e="ItemManager unavailable or unsupported reentry";return false;}running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 if(!source){e="Required original ItemManager Spawn NULL-source assertion";return false;}
 if(index>=inventory.items().size())return true;
 auto* item=inventory.items()[index]->item.get();
 if(!item){e="Required actual temporary inventory item";return false;}
 // Metadata comes from the same immutable inventory cache, never the pool.
 const auto* row=inventory.metadata(*item);
 if(!row){e="Required actual spawn ItemTable row";return false;}
 auto category=row->record.words[21];if(category<0||std::size_t(category)>=categories_.size())return true;
 auto& c=categories_[std::size_t(category)];auto current=c.cursor;c.cursor=current+1;if(c.cursor>4)c.cursor=0;
 auto& slot=c.slots[current];if(slot.active&&!despawn(slot,e))return false;if(!slot.object.identity)return true;
 if(!position||!destination){e="Required actual source/destination world position";return false;}
 LootItemRequestV8 q{LootItemOperationV8::position,&slot.object};q.vector=position;q.flag=true;if(!invoke(q,e))return false;
 q.operation=LootItemOperationV8::destination;q.vector=destination;if(!invoke(q,e))return false;
 q.operation=LootItemOperationV8::init_again;q.vector=nullptr;q.inventory=&inventory;q.index=index;q.character=character;q.audiovisual=&audiovisual_.rows()[std::size_t(category)];
 const auto count=inventory.items().size();if(!invoke(q,e))return false;if(inventory.items().size()+1!=count){e="Source ItemObject InitAgain did not transfer one actual loot item";return false;}
 q.operation=LootItemOperationV8::set_visible;q.flag=true;q.inventory=nullptr;if(!invoke(q,e))return false;
 *slot.object.enabled85=1;slot.active=true;out=slot.object;return true;
}
bool CharacterLootItemManagerV8::despawn(std::uintptr_t id,std::string& e){e.clear();if(running_){e="Unsupported ItemManager DeSpawn reentry";return false;}running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};for(auto& c:categories_)for(auto& s:c.slots)if(s.object.identity==id)return despawn(s,e);return true;}
bool CharacterLootItemManagerV8::flush(std::string& e){e.clear();if(running_){e="Unsupported ItemManager Flush reentry";return false;}categories_.clear();ready_=false;attempted_=false;return true;}
}
