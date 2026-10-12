#include "../character_npc_initpost_owner_v1.hpp"
#include <cassert>
#include <array>
#include <iostream>
#include <vector>
using namespace dh2::character;
struct Fixture {
 std::uint8_t called{};std::int32_t probability=100,room=4,offset=2;
 std::string waypoint,model;std::int16_t properties_id=-1;
 dh2::data::PropertyState sheets{};dh2::data::PropertyView properties{};
 std::uint32_t delayed{},adopted_delayed{};bool adopted{},adopt_on_ai{},adopt_on_load{};std::uintptr_t fx{},visual=9;
 float scale[3]{},fade_in{},fade_out{},position[3]{1,2,3},initial[3]{},rotation[3]{4,5,6},initial_rotation[3]{};
 dh2::data::AiProps ai{};std::vector<NpcInitPostRequestV1> calls;
 std::size_t fail_call{};bool player{},condition=true;std::int32_t sample=50;
 Fixture(){properties.base=sheets.base.data();properties.resolved=sheets.resolved.data();sheets.base[12]=sheets.base[13]=sheets.base[14]=100;sheets.resolved[210]=7;sheets.resolved[211]=9;ai.self_fx=3;}
 static std::uint32_t* live_delay(void* p){auto& f=*static_cast<Fixture*>(p);return f.adopted?&f.adopted_delayed:&f.delayed;}
 NpcInitPostBorrowV1 borrow(){return {41,&called,&probability,&waypoint,&room,&properties_id,&properties,&model,scale,&delayed,&offset,&fx,&visual,&fade_in,&fade_out,position,initial,rotation,initial_rotation,this,live_delay};}
 static bool service(void* p,const NpcInitPostRequestV1& q,NpcInitPostResponseV1& r,std::string& e){auto& f=*static_cast<Fixture*>(p);assert(q.subject==41);f.calls.push_back(q);if(f.fail_call==f.calls.size()){e="explicit required fixture service failure";return false;}
  if((q.source_entry==0x3a2fec&&f.adopt_on_ai)||(q.source_entry==0x3cf1f0&&f.adopt_on_load))f.adopted=true;
  switch(q.source_entry){case 0x38bd64:r.value=f.sample;break;case 0x38ab60:r.value=f.condition;break;case 0x3a54d4:r.text="actualModelResource";break;case 0x3a2fec:r.ai=&f.ai;break;case 0x3a49f0:r.value=f.player;break;case 0x495430:assert(q.argument0==5);r.identity=99;break;case 0x3a58f4:std::copy(f.position,f.position+3,f.initial);break;case 0x34aca0:r.identity=71;break;case 0x33fdc0:case 0x33fee4:r.identity=72;break;default:break;}return true;
 }
};
int main(){unsigned checks=0;
 Fixture full;CharacterNpcInitPostOwnerV1 owner(full.borrow(),{&full,Fixture::service});assert(owner.initialize());++checks;
 assert(full.called==1&&full.fx==99&&full.model=="actualModelResource"&&full.fade_in==7&&full.fade_out==9);++checks;
 assert(full.initial_rotation[0]==4&&full.initial_rotation[2]==6&&full.initial[2]==3);++checks;
 const std::vector<std::uint32_t> expected={0x38bd64,0x337888,0x337a88,0x3b3d38,0x3df2a4,0x3e0810,0x3a54d4,0x337888,0x337a88,0x38be5c,0x38ab60,0x3bc4d0,0x3a2fec,0x3a49f0,0x3a49f0,0x3cf1f0,0x495430,0x3b4738,0x337888,0x337a88,0x3c9f4c,0x337888,0x337a88,0x3b3b00,0x337888,0x337a88,0x3a49f0,0x3a58f4,0x393db4,0x470a54,0x3a59ac,0x3b3a70,0x3ce7c0,0x3d37d0,0x337888,0x337a88};
 assert(full.calls.size()==expected.size());for(unsigned n=0;n<expected.size();++n){assert(full.calls[n].source_entry==expected[n]);++checks;}
 const auto count=full.calls.size();assert(owner.initialize()&&full.calls.size()==count);++checks;
 for(unsigned n=1;n<=count;++n){Fixture f;f.fail_call=n;CharacterNpcInitPostOwnerV1 x(f.borrow(),{&f,Fixture::service});assert(!x.initialize()&&f.called==1&&f.calls.size()==n);assert(!x.initialize()&&f.calls.size()==n);checks+=2;}
 Fixture skipped;skipped.sample=100;CharacterNpcInitPostOwnerV1 skip(skipped.borrow(),{&skipped,Fixture::service});assert(skip.initialize()&&skipped.calls.size()==1&&skipped.called==1);++checks;
 Fixture rejected;rejected.condition=false;CharacterNpcInitPostOwnerV1 reject(rejected.borrow(),{&rejected,Fixture::service});assert(reject.initialize());assert(rejected.calls[rejected.calls.size()-2].source_entry==0xffffff40&&rejected.calls.back().source_entry==0x33ddb4);++checks;
 Fixture delay;delay.ai.delayed_load=1;CharacterNpcInitPostOwnerV1 d(delay.borrow(),{&delay,Fixture::service});assert(d.initialize()&&delay.delayed==1);for(auto q:delay.calls)assert(q.source_entry!=0x3cf1f0&&q.source_entry!=0x3ce7c0);++checks;
 Fixture waypoint;waypoint.waypoint="ActualWaypoint";waypoint.properties_id=17;CharacterNpcInitPostOwnerV1 w(waypoint.borrow(),{&waypoint,Fixture::service});assert(w.initialize());assert(waypoint.calls[3].source_entry==0x34aca0&&waypoint.calls[6].source_entry==0x405540);++checks;
 Fixture player;player.player=true;CharacterNpcInitPostOwnerV1 p(player.borrow(),{&player,Fixture::service});assert(!p.initialize()&&p.error().find("player")!=std::string::npos);++checks;
 Fixture late;late.adopt_on_load=true;late.adopted_delayed=1;CharacterNpcInitPostOwnerV1 l(late.borrow(),{&late,Fixture::service});assert(l.initialize());for(auto q:late.calls)assert(q.source_entry!=0x3ce7c0);assert(late.delayed==0);++checks;
 Fixture earlier;earlier.adopt_on_ai=true;earlier.ai.delayed_load=1;CharacterNpcInitPostOwnerV1 a(earlier.borrow(),{&earlier,Fixture::service});assert(a.initialize()&&earlier.adopted_delayed==1&&earlier.delayed==0);++checks;
 std::cout<<"{\"checks\":"<<checks<<",\"positive_service_calls\":"<<count<<",\"scope\":\"source NPC coordinator with explicit helper fixtures; no full helper backend claim\"}\n";
}
