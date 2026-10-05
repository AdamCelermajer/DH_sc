#include "character_world_ai_queue_v1.hpp"
#include <cassert>
#include <vector>
using namespace dh2::character;
struct Fixture {std::vector<WorldAIQueueQueryV1> calls;int mode{};};
static bool query(void* p,std::uintptr_t ai,WorldAIQueueQueryV1 q,std::int32_t& v,std::string& e){
 auto& f=*static_cast<Fixture*>(p);f.calls.push_back(q);v=0;
 if(f.mode==1 && q==WorldAIQueueQueryV1::Byte80){e="actual byte80 unavailable";return false;}
 if(f.mode==2 && q==WorldAIQueueQueryV1::Forced)v=1;
 if(f.mode==2 && q==WorldAIQueueQueryV1::RemotelyUpdated)v=1;
 if(f.mode==3 && q==WorldAIQueueQueryV1::Dead)v=1;
 if(f.mode==4 && q==WorldAIQueueQueryV1::Byte80)v=1;
 if(f.mode==5 && q==WorldAIQueueQueryV1::Follower)v=1;
 (void)ai;return true;
}
int main(){
 Fixture f;WorldAIQueueServicesV1 s{&f,query};CharacterWorldAIQueueV1 q;bool turn;
 assert(!q.is_my_turn(1,s,turn));assert(q.add(1));assert(q.add(2));assert(q.add(3));assert(!q.add(1));
 assert(q.is_my_turn(1,{},turn)&&turn);assert(q.is_my_turn(2,s,turn)&&!turn);
 f.mode=1;f.calls.clear();assert(!q.advance(10,s));assert(q.countdown()==180&&q.actors().front()==2);
 assert(f.calls.size()==6&&f.calls.back()==WorldAIQueueQueryV1::Byte80);
 f.calls.clear();assert(q.advance(200,{}));assert(q.countdown()==-20&&f.calls.empty());
 f.mode=2;assert(q.advance(10,s));assert(q.actors().front()==3&&q.countdown()==180);
 assert(f.calls.size()==3);assert(f.calls[1]==WorldAIQueueQueryV1::Dead);
 assert(q.advance(180,s));f.mode=3;f.calls.clear();assert(q.advance(0,s));assert(q.actors().front()==3);
 assert(f.calls.size()==8);assert(q.advance(180,s));f.mode=4;f.calls.clear();assert(q.advance(0,s));
 assert(q.actors().front()==1&&f.calls.back()==WorldAIQueueQueryV1::Zoned);
 f.mode=5;assert(q.is_my_turn(1,s,turn)&&turn);assert(q.remove(1));assert(!q.remove(1));
 assert(!q.is_my_turn(1,s,turn));
}
