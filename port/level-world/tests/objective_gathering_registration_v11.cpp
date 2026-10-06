#include "objective_gathering_registration_v11.hpp"
#include "../game-data/inventory_gathering_ids_v11.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2;
struct Fixture {
 data::InventoryGatheringIdsV11 inventory;
 std::uint8_t enabled{};std::uintptr_t character{123};std::int32_t id{7};bool fail_base{};
 std::vector<int> order;
 static bool base_register(void* raw,std::uintptr_t object,std::string& e){auto& f=*static_cast<Fixture*>(raw);if(object!=456)throw std::runtime_error("same objective");f.order.push_back(1);if(f.fail_base){e="actual base failure fixture";return false;}f.enabled=1;f.id=9;return true;} // Explicit base-lifecycle field mutation fixture.
 static bool base_unregister(void* raw,std::uintptr_t object,std::string&){auto& f=*static_cast<Fixture*>(raw);if(object!=456)throw std::runtime_error("same objective");f.order.push_back(2);return true;}
 static bool add(void* raw,std::uintptr_t character,std::int32_t id,std::string&){auto& f=*static_cast<Fixture*>(raw);if(character!=123||id!=9)throw std::runtime_error("fresh borrowed fields");f.order.push_back(3);f.inventory.register_id(id);return true;}
 static bool remove(void* raw,std::uintptr_t character,std::int32_t id,std::string& e){auto& f=*static_cast<Fixture*>(raw);if(character!=123||id!=9)throw std::runtime_error("fresh borrowed fields");f.order.push_back(4);return f.inventory.unregister_id(id,{},e);}
};
int main(){try{unsigned checks{};auto check=[&](bool yes){++checks;if(!yes)throw std::runtime_error("check "+std::to_string(checks));};Fixture f;character::ObjectiveGatheringBorrowV11 b{456,&f.enabled,&f.character,&f.id};character::ObjectiveGatheringServicesV11 s{&f,Fixture::base_register,Fixture::base_unregister,Fixture::add,Fixture::remove};std::string e;
 check(character::objective_gathering_register_v11(b,s,e)&&f.inventory.contains(9)&&!f.inventory.contains(7));check(f.order==std::vector<int>({1,3}));check(character::objective_gathering_register_v11(b,s,e)&&f.inventory.entries().front().references==2);check(character::objective_gathering_unregister_v11(b,s,e)&&f.inventory.entries().front().references==1);check(character::objective_gathering_unregister_v11(b,s,e)&&f.inventory.entries().empty());
 f.enabled=0;auto count=f.order.size();check(character::objective_gathering_unregister_v11(b,s,e)&&f.order.size()==count+1&&f.order.back()==2);f.fail_base=true;check(!character::objective_gathering_register_v11(b,s,e)&&f.inventory.entries().empty());
 std::cout<<"PASS whole Objective gathering base-before-fresh-field reads, registration/unregistration gates and failure prefix checks="<<checks<<'\n';return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what();return 1;}}
