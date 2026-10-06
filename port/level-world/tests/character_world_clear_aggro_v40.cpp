#include "../character_world_clear_aggro_v40.hpp"
#include "../character_target_update.hpp"
#include <array>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>
#include <iostream>
using namespace dh2;
using namespace dh2::character;
struct Pair {
 struct A {TargetOwner16 owner{};TargetState48 target{};TargetBindings48 binding{};std::array<data::AggroEntry,8> out{},in{};data::AggroTable outgoing{out.data(),0,8},incoming{in.data(),0,8};std::uintptr_t controller{};};
 std::array<A,4> actors;std::shared_ptr<void> lease=std::make_shared<int>(1);std::vector<unsigned> trace;unsigned mutation{},stop{},borrows{};bool fail_notify{},fail_stop{},fail_set{},controller_after_set{};
 Pair(){for(unsigned i=0;i<4;++i){auto& a=actors[i];a.owner.identity=i+1;a.target.owner=&a.owner;a.target.identity=100+i;a.binding.state=&a.target;a.binding.services={this,target_service};a.controller=100+i;}}
 static int target_service(void* raw,TargetState48* state,const TargetRequest24* q,std::uint32_t* out){auto& p=*static_cast<Pair*>(raw);*out=0;
  if(q->service==target_debug_load)return 0;
  if(q->service==target_debug_query){if(state==&p.actors[1].target){p.trace.push_back(2);if(p.controller_after_set)p.actors[1].controller=777;if(p.fail_set)return -1;}return 0;}
  if(q->service==target_owner_ai_id||q->service==target_virtual_dead||q->service==target_in_sight)return 0;
  return -1;
 }
 static bool borrow(void* raw,std::uintptr_t id,WorldClearAggroActorV40& out,std::string& e){auto& p=*static_cast<Pair*>(raw);++p.borrows;if(id<1||id>4){e="missing registered actor";return false;}auto& a=p.actors[id-1];out={id,&a.outgoing,&a.incoming,&a.binding,a.controller,p.lease};return true;}
 static bool notify(void* raw,std::uintptr_t receiver,std::uintptr_t owner,const dh2_script_callback_scope*,std::string& e){auto& p=*static_cast<Pair*>(raw);assert(receiver==2&&owner==1);p.trace.push_back(1);assert(p.actors[1].incoming.count==0);
  if(p.mutation==1)p.actors[1].target.target=0;
  if(p.mutation==2)p.actors[0].owner.identity=3;
  if(p.mutation==3)p.actors[1].controller=103;
  if(p.fail_notify){e="required OnDeAggro fixture";return false;}return true;
 }
 static bool stopped(void* raw,std::uintptr_t actor,std::uintptr_t controller,const dh2_script_callback_scope*,std::string& e){auto& p=*static_cast<Pair*>(raw);assert(actor==2&&p.actors[1].target.target==0);p.stop=unsigned(controller);p.trace.push_back(3);if(p.fail_stop){e="required Cmd_Stop fixture";return false;}return true;}
 void seed(bool forward,bool reverse,bool targets){
  auto add=[&](unsigned b,float value){std::uint32_t bits;std::memcpy(&bits,&value,4);data::AggroChange c{};data::AggroRequest q{&actors[0].outgoing,&actors[b].incoming,1,b+1,bits,0};assert(!dh2_aggro_apply(&c,&q,data::aggro_set));};add(2,2);add(3,2);if(forward||reverse)add(1,10);
  if(!forward&&reverse){auto& t=actors[0].outgoing;for(unsigned i=0;i<t.count;++i)if(t.entries[i].character==2){for(unsigned j=i+1;j<t.count;++j)t.entries[j-1]=t.entries[j];--t.count;break;}}
  if(forward&&!reverse)actors[1].incoming.count=0;
  actors[1].target.target=targets?1:0;
 }
 CharacterWorldClearAggroV40 owner(){return CharacterWorldClearAggroV40({this,borrow,notify,stopped});}
};
int main(int argc,char** argv){unsigned checks=0;
 // Contrast native fixtures exercise the same original reentry branches and
 // compare original gold callback order/map ownership below when supplied.
 for(bool identity:{false,true})for(bool forward:{false,true})for(bool reverse:{false,true})for(bool targets:{false,true})for(unsigned mutation=0;mutation<4;++mutation){Pair p;p.seed(forward,reverse,targets);p.mutation=mutation;auto o=p.owner();assert(!o.clear(1,identity?2:0));
  std::vector<unsigned> expected;if(identity&&forward)expected.push_back(1);const bool clear=identity&&targets&&!(forward&&(mutation==1||mutation==2));if(clear){expected.push_back(2);expected.push_back(3);}assert(p.trace==expected);
  assert(p.actors[0].outgoing.count==unsigned(2+(forward&&!identity)));assert(p.actors[1].incoming.count==unsigned(reverse&&!(forward&&identity)));
  if(clear)assert(p.stop==unsigned(identity&&forward&&mutation==3?103:101));++checks;
 }
 Pair reentry;reentry.seed(true,true,true);reentry.controller_after_set=true;auto r=reentry.owner();assert(!r.clear(1,2)&&reentry.stop==777&&reentry.borrows==3);++checks;
 Pair notify_failed;notify_failed.seed(true,true,true);notify_failed.fail_notify=true;auto n=notify_failed.owner();assert(n.clear(1,2)==-2&&notify_failed.actors[0].outgoing.count==2&&!notify_failed.actors[1].incoming.count&&notify_failed.actors[1].target.target==1&&notify_failed.trace==std::vector<unsigned>{1});++checks;
 Pair set_failed;set_failed.seed(true,true,true);set_failed.fail_set=true;auto s=set_failed.owner();assert(s.clear(1,2)==-2&&set_failed.actors[1].target.target==1&&set_failed.trace==std::vector<unsigned>({1,2})&&!set_failed.actors[1].binding.scope);++checks;
 Pair stop_failed;stop_failed.seed(true,true,true);stop_failed.fail_stop=true;auto t=stop_failed.owner();assert(t.clear(1,2)==-2&&stop_failed.actors[1].target.target==0&&!stop_failed.actors[1].binding.scope);++checks;
 // Early null-target return must not touch a malformed receiver/provider.
 CharacterWorldClearAggroV40 empty({});assert(!empty.clear(0,0));++checks;
 if(argc==2){std::ifstream f(argv[1],std::ios::binary);unsigned count{};f.read(reinterpret_cast<char*>(&count),4);assert(f&&count<=1000);for(unsigned i=0;i<count;++i){std::array<unsigned,9> row{};f.read(reinterpret_cast<char*>(row.data()),36);assert(f);Pair p;p.seed(row[1],row[2],row[3]);p.mutation=row[4];auto o=p.owner();assert(!o.clear(1,row[0]?2:0));unsigned mask=0;for(auto op:p.trace)mask|=1u<<op;assert(mask==row[5]&&p.actors[0].outgoing.count==row[6]&&p.actors[1].incoming.count==row[7]&&(!p.stop||p.stop-100==row[8]));++checks;}assert(f.peek()==std::char_traits<char>::eof());}
 std::cout<<"world ClearAggro V40 PASS "<<checks<<" checks: reciprocal source maps/target setter/reentrant retarget & controller/failure prefixes; actual notification/Stop endpoints remain fixtures\n";
}
