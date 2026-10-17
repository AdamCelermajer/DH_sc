#include "../aggro.hpp"
#include "../../level-world/character_ai_initialization.hpp"

#include <stdexcept>

namespace {
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
void publish(dh2::data::AggroOwner& owner,dh2::character_ai_initialization::State& ai){
 ai.tree_7c.count=owner.out_count;ai.tree_94.count=owner.in_count;
}
}

int main(){
 using namespace dh2::data;
 AggroOwner attacker,target;attacker.initialize(2);target.initialize(2);
 dh2::character_ai_initialization::State attacker_ai{},target_ai{};
 auto out=attacker.outgoing_table();auto in=target.incoming_table();
 AggroRequest request{&out,&in,0x101,0x202,0x3f800000,0};AggroChange change{};
 require(dh2_aggro_apply(&change,&request,aggro_set)==0&&change.inserted==1&&
         change.requests==aggro_notify_target,"source relation insertion failed");
 attacker.out_count=out.count;target.in_count=in.count;publish(attacker,attacker_ai);publish(target,target_ai);
 require(attacker.outgoing_table().entries==attacker.outgoing.data()&&
         target.incoming_table().entries==target.incoming.data()&&
         attacker_ai.tree_7c.count==attacker.outgoing_table().count&&
         target_ai.tree_94.count==target.incoming_table().count&&
         attacker.outgoing[0].character==0x202&&target.incoming[0].character==0x101,
         "combat and CharAI did not share the retained relation stores");
 out=attacker.outgoing_table();in=target.incoming_table();
 request={&out,&in,0x101,0x202,0,0};
 require(dh2_aggro_apply(&change,&request,aggro_clear)==0&&change.removed==1&&
         change.requests==aggro_notify_target_cleared,"source relation removal failed");
 attacker.out_count=out.count;target.in_count=in.count;publish(attacker,attacker_ai);publish(target,target_ai);
 require(attacker_ai.tree_7c.count==0&&target_ai.tree_94.count==0&&
         attacker.outgoing_table().count==0&&target.incoming_table().count==0,
         "CharAI count projections diverged after canonical relation removal");
 return 0;
}
