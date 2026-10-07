#include "character_menu_mutations_v4.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {unsigned checks{};void audit(bool value,const char* expression){++checks;if(!value)throw std::runtime_error("menu mutation check "+std::to_string(checks)+": "+expression);}
#define check(value) audit((value),#value)
std::vector<std::uint8_t> file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
struct Fixture {character::CharacterMenuMutationServicesV4 services;bool player{true},local{true},reduce{},fail{};unsigned predicates{};std::vector<std::string> awards;data::OwnedInventoryServicesV4 effects;
 static bool effect(void* raw,data::FreshInventoryOwnedV4& inv,const data::OwnedInventoryRequestV4& q,data::OwnedInventoryResponseV4&,std::string& e){auto& s=*static_cast<Fixture*>(raw);check(q.operation==data::OwnedInventoryOperationV4::gold_notifications);return character::character_menu_gold_notifications_v4(inv,s.services,e);}
};}
int main(int argc,char** argv){try{
 check(argc==2);std::string e;data::LootTablesV2 tables;auto records=file(std::string(argv[1])+"/loot_table_pyarray.bin"),names=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),schema=file(std::string(argv[1])+"/loot_table_pystructnames.bin");check(tables.load(bytes(records),bytes(names),bytes(schema),e));
 auto owner=std::make_shared<int>(1);auto props=std::make_shared<data::PropertyState>();data::LootRandom8V2 random{1,0};
 data::FreshInventoryOwnedV4 inv(0x1234,tables.borrow(),random,12,props);Fixture f;f.effects={&f,Fixture::effect,nullptr};f.services.owner=owner;
 f.services.is_player=[&](auto id,bool& out,auto&){check(id==inv.character());++f.predicates;out=f.player;return true;};
 f.services.is_local_player=[&](auto id,bool& out,auto&){check(id==inv.character());++f.predicates;out=f.local;return true;};
 f.services.achievement=[&](auto id,const char* key,auto& error){check(id==inv.character());f.awards.emplace_back(key);if(f.fail){error="actual fixture trophy delivery failure";return false;}if(f.reduce){f.reduce=false;return inv.set_gold(5,f.effects,error);}return true;};
 for(auto value:{0,9999,10000,99999,100000,999999,1000000}){
  f.awards.clear();e.clear();check(inv.set_gold(value,f.effects,e)&&inv.gold()==value);
  check(f.awards.size()==unsigned(value>9999)+unsigned(value>99999)+unsigned(value>999999));
  if(value>9999)check(f.awards[0]=="gear_10kgold");if(value>99999)check(f.awards[1]=="gear_100kgold");if(value>999999)check(f.awards[2]=="gear_1mgold");
 }
 f.awards.clear();f.reduce=true;e.clear();check(!inv.set_gold(1000000,f.effects,e)&&inv.gold()==1000000&&e=="Unsupported destructive inventory callback reentry");
 f.awards.clear();f.reduce=true;e.clear();check(character::character_menu_gold_notifications_v4(inv,f.services,e)&&inv.gold()==5&&f.awards==std::vector<std::string>{"gear_10kgold"}); // Direct owner boundary permits fixture callback mutation; fresh reads preserved.
 f.awards.clear();f.player=false;check(inv.set_gold(1000000,f.effects,e)&&f.awards.empty());f.player=true;f.local=false;check(inv.set_gold(1000000,f.effects,e)&&f.awards.empty());f.local=true;
 f.fail=true;e.clear();check(!inv.set_gold(10000,f.effects,e)&&inv.gold()==10000&&e=="actual fixture trophy delivery failure");f.fail=false;
 auto temporary=data::FreshInventoryOwnedV4::create_drop_temporary_v4(tables.borrow(),random,props);check(temporary&&temporary->character()==0&&temporary->potion_capacity_v4()==-1&&temporary->items().empty()&&temporary->gold()==0&&temporary->properties()==props&&&temporary->table()==&inv.table());character::CharacterMenuMutationServicesV4 absent;check(character::character_menu_gold_notifications_v4(*temporary,absent,e));
 e.clear();check(!character::character_menu_gold_notifications_v4(inv,absent,e));
 auto saved=std::make_shared<data::PlayerSavegameV1>();auto profile=std::make_shared<data::PlayerSaveLoadOwnerV1>(saved);unsigned calls{};
 data::PlayerSaveWriteServicesV1 write;write.owner=owner;write.invoke=[&](const auto& q,auto&,auto& error){++calls;check(q.save==saved.get()&&q.authority==profile.get());error="actual campaign writer missing";return false;};
 data::PlayerSaveWriteOwnerV1 writer(profile,write);check(writer.save(e)&&!calls&&writer.reached_phase()==0);
 check(profile->publish_profile({0x5678,owner},e));check(!writer.save(e)&&calls==1&&writer.reached_phase()==1&&e=="actual campaign writer missing");
 check(profile->publish_profile({},e));check(writer.save(e)&&calls==1&&writer.reached_phase()==0);
 std::cout<<"PASS real cache inventory gold store + all source thresholds/keys/fresh callback reads/failure prefix, NULL-character leaf; SAME Save null-profile no-write guard and present-profile required writer; predicates/achievement endpoints declared fixtures checks="<<checks<<"\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
