#include "../application_spawn_random_owner_v4.hpp"
#include <cassert>
#include <iostream>
int main(){using namespace dh2;auto lease=std::make_shared<int>(1);world::ApplicationSpawnRandomOwnerV4 random;std::string e;
 assert(random.channel(0).seed==0&&random.channel(1).seed==0&&random.channel(0).calls==0&&random.channel(1).calls==0);
 unsigned visible=0,marks=0;world::CanonicalSpawnApplicationServicesV4 s;s.application_lease=lease;s.random=&random;
 s.online_byte5=[](bool& value,std::string&){value=false;return true;};s.handle_as_player=[](bool& value,std::string&){value=false;return true;};
 s.set_visible_false=[&](std::string&){++visible;return true;};s.mark_for_deletion=[&](std::string&){++marks;return true;};
 for(unsigned i=0;i<9;++i){actor::RuntimeState runtime{};world::CanonicalGameObjectBaseOwnerV1 base(100+i,11,lease,runtime);std::int32_t roll,prob;
  assert(world::canonical_check_spawn_probability_v4(base,s,roll,prob,e)&&roll==-2&&prob==100);
  assert(world::canonical_check_spawn_probability_v4(base,s,roll,prob,e)&&random.channel(0).calls==i+1);
 }assert(random.channel(1).calls==0&&visible==0&&marks==0);
 {actor::RuntimeState runtime{};world::CanonicalGameObjectBaseOwnerV1 base(200,7,lease,runtime);*base.integer(0x274)=0;s.mark_for_deletion={};std::int32_t roll,prob;
  assert(!world::canonical_check_spawn_probability_v4(base,s,roll,prob,e));assert(*base.byte(0x82)==0&&*base.byte(0x81)==1&&*base.integer(0x270)>=0&&visible==1);assert(e.find("MarkForDeletion")!=std::string::npos);
 }
 {actor::RuntimeState runtime{};world::CanonicalGameObjectBaseOwnerV1 base(201,7,lease,runtime);*base.integer(0x108)=1;*base.pointer(0xfc)=0xffffffffu;s.online_byte5=[](bool& value,std::string&){value=true;return true;};std::int32_t roll,prob;
  assert(world::canonical_check_spawn_probability_v4(base,s,roll,prob,e)&&random.channel(1).calls==1&&random.channel(0).calls==10);
  std::int32_t owner;assert(base.source_online_owner_fc_v4(owner,e)&&owner==-1);
 }
 {actor::RuntimeState runtime{};world::CanonicalGameObjectBaseOwnerV1 base(202,0,lease,runtime);s.handle_as_player=[](bool& value,std::string&){value=true;return true;};s.online_byte5={};std::int32_t roll,prob;assert(world::canonical_check_spawn_probability_v4(base,s,roll,prob,e)&&roll==-2);assert(random.channel(0).calls==10&&random.channel(1).calls==1);}
 std::cout<<"Application spawn globals native PASS nine shared rolls/cache + channel1 signedfc + delete failureprefix + player bypass; online/Handle/visibility/manager callbacks explicit fixtures\n";
}
