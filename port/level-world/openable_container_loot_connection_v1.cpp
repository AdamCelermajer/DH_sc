#include "openable_container_loot_connection_v1.hpp"
namespace dh2::world {
bool OpenableContainerLootConnectionV1::character(std::uintptr_t id,character::skills::WorldLootActorBorrowV8& out,std::string& e){
 out={};if(!id)return true;
 if(!services_.character){e="Required canonical ObjectManager character loot resolution";return false;}
 if(!services_.character(services_.context,id,out,e))return false;
 if(out.identity&&out.type_index!=0){e="Loot character projection must apply original type0 filter";return false;}return true;
}
bool OpenableContainerLootConnectionV1::query(void* c,const data::LootCreationQueryV8& q,data::LootCreationResponseV8& r,std::string& e){
 auto& s=*static_cast<OpenableContainerLootConnectionV1*>(c);auto& original=s.services_.creation;
 if(!original.query){e="Required actual AddLoot player-count/current-level producer";return false;}
 return original.query(original.context,q,r,e);
}
bool OpenableContainerLootConnectionV1::create(void* c,std::int32_t id,std::unique_ptr<data::ItemInstanceV1>& item,std::string& e){
 auto& s=*static_cast<OpenableContainerLootConnectionV1*>(c);
 return s.pending_->create(id,item,s.services_.creation.text,e);
}
bool OpenableContainerLootConnectionV1::store(void* c,std::unique_ptr<data::ItemInstanceV1>& item,std::string& e){
 auto& s=*static_cast<OpenableContainerLootConnectionV1*>(c);
 return s.pending_->store(item,s.services_.creation.entry,s.services_.notifications_context,s.services_.notifications,e);
}
bool OpenableContainerLootConnectionV1::drop_table(std::int32_t table,std::uintptr_t chest,std::uintptr_t opener,
 std::int32_t powers,bool flag,std::string& e){
 if(flag){e="Only original chest DropLootTable source flag=false is bound";return false;}
 if(running_||pending_){e="Reentry or unconsumed source chest loot prefix";return false;}
 running_=true;struct Reset{bool& v;~Reset(){v=false;}}reset{running_};
 character::skills::WorldLootActorBorrowV8 credited,source;
 if(!character(opener,credited,e)||!character(chest,source,e))return false;
 bool eligible=false;
 if(source.identity){if(!source.ai){e="Required source Character AI for boss/miniboss loot gate";return false;}eligible=(source.ai->flags&6)!=0;}
 if(!eligible){if(!credited.identity)return true;
  if(!services_.is_player){e="Required original opener IsPlayer virtual";return false;}
  if(!services_.is_player(services_.context,credited.identity,eligible,e))return false;
  if(!eligible)eligible=credited.identity==source.identity;
 }
 if(!eligible)return true;
 if(!tables_){e="Required actual chest loot tables";return false;}
 pending_=std::make_unique<data::LootTemporaryInventoryV8>(tables_);
 character::skills::WorldLootActorBorrowV8 bonus;
 if(!character(opener,bonus,e))return false;
 std::int32_t value=0,power=0;
 if(bonus.identity&&(!bonus.properties||dh2_property_resolve(bonus.properties,195,&value)||dh2_property_resolve(bonus.properties,196,&power))){e="Required same opener properties195/196";return false;}
 auto services=services_.creation;services.context=this;services.query=query;services.create=create;services.store=store;
 if(!creation_.add(table,value,power,powers,false,false,services,e))return false;
 if(!award_.drop(*pending_,chest,opener,e))return false;
 if(!pending_->items().empty()){e="Chest world drop retained actual unspawned items";return false;}
 pending_.reset();return true;
}
}
