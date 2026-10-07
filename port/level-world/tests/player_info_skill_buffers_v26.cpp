#include "../player_info_skill_buffers_v26.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace dh2::player;
int main(){try{
 unsigned checks=0;auto check=[&](bool x){++checks;if(!x)throw std::runtime_error("PlayerInfo buffer check "+std::to_string(checks));};
 MatchingLocalSelectionOwnerV4 matching;PlayerNetworkLocalOwnerV4 network(matching);
 PlayerInfoFieldsV1 record,other;auto lease=std::make_shared<int>(42);std::string error;
 PlayerInfoSkillBuffersV26 owner;std::array<std::int8_t,3> slots{7,8,9};
 std::array<std::int8_t,30> levels;levels.fill(17);std::int32_t copied=-1;
 check(!owner.get_slots(record,slots.data(),slots.size(),copied,error));
 check(copied==0&&slots[0]==7);
 check(!owner.construct(record,network,error));
 check(network.construct(record,lease,error));
 const auto actual=network.borrow(record);check(actual&&actual->receiver==&record);
 check(owner.construct(record,network,error));
 check(owner.receiver()==&record&&network.borrow(record)==actual);
 check(!owner.construct(record,network,error));
 check(owner.get_slots(record,slots.data(),slots.size(),copied,error));
 check(copied==3&&std::all_of(slots.begin(),slots.end(),[](auto x){return x==-1;}));
 check(owner.get_levels(record,levels.data(),levels.size(),copied,error));
 check(copied==30&&std::all_of(levels.begin(),levels.end(),[](auto x){return x==-1;}));
 slots.fill(8);check(!owner.get_slots(record,slots.data(),2,copied,error));
 check(copied==0&&slots[0]==8);check(!owner.get_slots(other,slots.data(),3,copied,error));
 check(copied==0&&slots[0]==8);check(!owner.get_levels(record,nullptr,30,copied,error));
 // Existing source Character publication never replays ctor/Reset defaults.
 record.character660=0x123456;check(owner.get_slots(record,slots.data(),3,copied,error));
 check(record.character660==0x123456&&network.borrow(record)==actual);
 std::cout<<"{\"status\":\"PASS\",\"checks\":"<<checks<<",\"source_final_Reset\":true,\"net_serialization_claimed\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
