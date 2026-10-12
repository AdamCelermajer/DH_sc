#include "../character_revive_owner_v1.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::character;
struct Fixture{std::array<std::uint32_t,27> gold{};unsigned app_queries{},fail_at{};std::vector<std::uint32_t> trace;std::array<std::uint32_t,3> position{};
 static int service(void* p,const CharacterReviveRequestV1* q,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);assert(q&&q->subject==41&&q->reserved==0);f.trace.push_back(q->entry);if(f.fail_at==f.trace.size())return -7;
  switch(q->entry){case 0x3a4d5c:assert(q->argument0==3&&q->payload==0);break;case 0x7fd794:*out=f.gold[7+(f.app_queries++?1:0)];break;case 0x3a49f0:*out=f.gold[6];break;case 0x525508:{auto* v=reinterpret_cast<float*>(q->payload);assert(v&&v[0]==1&&v[1]==2&&v[2]==3);v[2]=9;break;}case 0x393db4:assert(q->argument0==1);std::memcpy(f.position.data(),reinterpret_cast<void*>(q->payload),12);break;default:break;}return 0;
 }
};
int main(int argc,char** argv){assert(argc==2);std::ifstream file(argv[1],std::ios::binary);std::uint32_t count{};file.read(reinterpret_cast<char*>(&count),4);assert(count==288);unsigned checks=0;
 for(unsigned n=0;n<count;++n){Fixture f;file.read(reinterpret_cast<char*>(f.gold.data()),108);assert(file);dh2::data::CombatActorState life;life.dead=f.gold[0];life.low_health_armed=f.gold[1];std::uint8_t remote=f.gold[2];std::int32_t network=f.gold[3];std::uint32_t aux=f.gold[4];const float spawn[3]{1,2,3};CharacterReviveBorrowV1 borrow{41,&life,&remote,&network,&aux,spawn};CharacterReviveServicesV1 service{&f,Fixture::service};CharacterReviveResultV1 result;
  assert(dh2_character_revive_v1(&result,&borrow,12345,f.gold[5],&service)==1&&result.completed==1);++checks;
  const std::array<std::uint32_t,5> actual{life.dead,life.low_health_armed,remote,static_cast<std::uint32_t>(network),aux};for(unsigned i=0;i<5;++i){assert(actual[i]==f.gold[9+i]);++checks;}
  assert(f.trace.size()==f.gold[14]&&result.calls==f.trace.size());++checks;for(unsigned i=0;i<f.trace.size();++i){assert(f.trace[i]==f.gold[15+i]);++checks;}
  for(unsigned i=0;i<3;++i){assert(f.position[i]==f.gold[24+i]);++checks;}
  for(unsigned fail=1;fail<=f.gold[14];++fail){Fixture prefix;prefix.gold=f.gold;prefix.fail_at=fail;life.dead=f.gold[0];life.low_health_armed=f.gold[1];remote=f.gold[2];network=f.gold[3];aux=f.gold[4];service.context=&prefix;assert(dh2_character_revive_v1(&result,&borrow,0,f.gold[5],&service)==-2&&!result.completed&&result.calls==fail&&prefix.trace.size()==fail);++checks;}
 }
 std::cout<<"{\"original_cases\":"<<count<<",\"checks\":"<<checks<<",\"scope\":\"whole source Revive orchestration/stores with explicit original helper boundaries\"}\n";
}
