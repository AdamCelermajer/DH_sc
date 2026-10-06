#include "../character_world_ai_neutral_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::character::skills;
struct Fixture {
 std::int32_t owner=0,target=1,kind=0;std::uintptr_t resolved=2;unsigned fail{},calls{};
 static int invoke(void* p,const WorldAiRequestV1* q,WorldAiResponseV1* r){auto& f=*static_cast<Fixture*>(p);++f.calls;if(q->service==f.fail)return 1;
  switch(q->service){case world_ai_handle:r->handle={2,0,2};break;case world_ai_object:r->identity=f.resolved;break;case world_ai_kind:r->value=f.kind;break;case world_ai_faction:r->value=q->subject==1?f.owner:f.target;break;default:assert(q->service==world_ai_assert);break;}return 0;
 }
};
int main(){
 dh2::data::AiFactionEntry entry{};entry.id=1;
 WorldAiFactionRowV1 rows[2]{{&entry,1,0},{nullptr,0,0}};std::uintptr_t target=2;std::int32_t assert_mode=0;
 WorldAiRelationshipV1 state{1,&target,rows,2,0,&assert_mode};Fixture f;WorldAiServicesV1 services{&f,Fixture::invoke};WorldAiOutputV1 out{};
 for(int value:{-1,0,1,2}){entry.value=value;assert(!dh2_world_ai_neutral_v1(&out,&state,2,&services)&&out.result==(value==0));}
 entry.id=0;assert(!dh2_world_ai_neutral_v1(&out,&state,2,&services)&&out.result==1);
 f.kind=7;f.calls=0;assert(!dh2_world_ai_neutral_v1(&out,&state,2,&services)&&out.result==1&&f.calls==3);
 f.resolved=0;f.calls=0;assert(!dh2_world_ai_neutral_v1(&out,&state,2,&services)&&out.result==1&&f.calls==2);
 target=0;f.calls=0;assert(!dh2_world_ai_neutral_v1(&out,&state,0,&services)&&out.result==1&&f.calls==0);
 f.kind=0;f.resolved=2;f.target=-1;assert_mode=2;assert(dh2_world_ai_neutral_v1(&out,&state,2,&services)==-3&&out.assert_line==226);
 f.target=1;f.fail=world_ai_object;assert(dh2_world_ai_neutral_v1(&out,&state,2,&services)==-2&&out.phase==world_ai_object);
 std::cout<<"AI_IsNeutral PASS recovered whole control with declared handle/faction/assertion fixture services\n";
}
