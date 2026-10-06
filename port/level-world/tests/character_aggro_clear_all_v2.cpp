#include "../character_aggro_clear_all_v2.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks{};
void check(bool b){++checks;if(!b)throw std::runtime_error("ClearAllAggro native/source mismatch "+std::to_string(checks));}
std::uint32_t word(std::ifstream& f){std::uint32_t v{};f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
int integer(std::ifstream& f){return static_cast<int>(word(f));}
struct SnapshotRow {
 int target{},last{};std::vector<std::array<unsigned,2>> outgoing,incoming;
 bool operator==(const SnapshotRow& o)const{return target==o.target&&last==o.last&&outgoing==o.outgoing&&incoming==o.incoming;}
};
using Snapshot=std::array<SnapshotRow,6>;
Snapshot read(std::ifstream& f){Snapshot rows;for(auto& r:rows){r.target=integer(f);r.last=integer(f);for(auto* v:{&r.outgoing,&r.incoming}){auto n=word(f);check(n<=6);for(unsigned i=0;i<n;++i){const auto a=word(f),b=word(f);v->push_back({a,b});}}}return rows;}
struct Event {unsigned op;int receiver,other;Snapshot snapshot;
 bool operator==(const Event& e)const{return op==e.op&&receiver==e.receiver&&other==e.other&&snapshot==e.snapshot;}};
struct Graph {
 struct Actor {
  std::array<dh2::data::AggroEntry,8> out{},in{};
  dh2::data::AggroTable outgoing{out.data(),0,8},incoming{in.data(),0,8};
  TargetOwner16 owner{};TargetState48 state{};TargetBindings48 binding{};
 };
 std::array<Actor,6> actors;std::vector<Event> trace;bool fail_notify{},reenter{},entered{},add_relation{};
 static std::uintptr_t id(int i){return i<0?0:0x100000001ull+unsigned(i);}
 static int index(std::uintptr_t id){return id?int(id-0x100000001ull):-1;}
 Snapshot snapshot(){Snapshot out;for(unsigned i=0;i<6;++i){auto& a=actors[i];auto& r=out[i];r.target=index(a.state.target);r.last=index(a.state.last_target);for(auto pair:{std::make_pair(&a.outgoing,&r.outgoing),std::make_pair(&a.incoming,&r.incoming)})for(unsigned j=0;j<pair.first->count;++j)pair.second->push_back({unsigned(index(pair.first->entries[j].character)),pair.first->entries[j].threat_bits});}return out;}
 AggroClearActorBorrowV2 borrow(unsigned i){auto& a=actors[i];return{id(int(i)),&a.outgoing,&a.incoming,&a.binding};}
 static bool actor(void* p,std::uintptr_t identity,AggroClearActorBorrowV2& out,std::string& e){auto& g=*static_cast<Graph*>(p);const auto i=index(identity);if(i<0||i>=6){e="missing declared actor";return false;}out=g.borrow(unsigned(i));return true;}
 static int target(void* p,TargetState48* s,const TargetRequest24* q,std::uint32_t* out){auto& g=*static_cast<Graph*>(p);*out=0;if(q->service==target_debug_load){g.trace.push_back({4,index(s->owner->identity),-1,g.snapshot()});return 0;}return q->service==target_debug_query?0:-1;}
 static bool notify(void* p,std::uintptr_t receiver,std::uintptr_t other,const dh2_script_callback_scope*,std::string& e){auto& g=*static_cast<Graph*>(p);g.trace.push_back({2,index(receiver),index(other),g.snapshot()});if(g.fail_notify){e="declared required OnDeAggro failure";return false;}if(g.reenter&&!g.entered){g.entered=true;AggroClearAllResultV2 nested;check(character_aggro_clear_all_v2(nested,g.borrow(0),g.services(),e)&&nested.complete&&nested.notifications==0);}if(g.add_relation&&!g.entered){g.entered=true;dh2::data::AggroChange c{};auto& a=g.actors[0];auto& b=g.actors[5];const dh2::data::AggroRequest q{&a.outgoing,&b.incoming,id(0),id(5),0x41400000,0};check(!dh2_aggro_apply(&c,&q,dh2::data::aggro_set));}return true;}
 AggroClearAllServicesV2 services(){return{this,actor,notify};}
 void initialize(const Snapshot& snap){trace.clear();fail_notify=reenter=entered=add_relation=false;for(unsigned i=0;i<6;++i){auto& a=actors[i];a.owner={id(int(i)),0,0,0};a.state={id(int(i))+0x100,&a.owner,0,id(snap[i].target),id(snap[i].last),0,0,0,0,0};a.binding={&a.state,{this,target},nullptr,{0,0}};for(auto pair:{std::make_pair(&a.outgoing,&snap[i].outgoing),std::make_pair(&a.incoming,&snap[i].incoming)}){pair.first->count=unsigned(pair.second->size());for(unsigned j=0;j<pair.first->count;++j)pair.first->entries[j]={id(int((*pair.second)[j][0])),(*pair.second)[j][1],0};}}}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream f(argv[1],std::ios::binary);check(bool(f)&&word(f)==0x32414341);const auto count=word(f);Graph g;unsigned notifications{},clears{};
 Snapshot populated{};
 for(unsigned c=0;c<count;++c){const auto incoming=word(f),clear=word(f);auto before=read(f),after=read(f);std::vector<Event> trace;const auto n=word(f);for(unsigned i=0;i<n;++i){auto op=word(f);auto receiver=integer(f),other=integer(f);trace.push_back({op,receiver,other,read(f)});}g.initialize(before);AggroClearAllResultV2 result{};std::string error;check(incoming?character_aggro_clear_all_toward_me_v2(result,g.borrow(0),clear,g.services(),error):character_aggro_clear_all_v2(result,g.borrow(0),g.services(),error));if(!(g.snapshot()==after&&g.trace==trace)){std::cerr<<"Case "<<c<<" incoming "<<incoming<<" clear "<<clear<<" finalmatch "<<(g.snapshot()==after)<<" tracematch "<<(g.trace==trace)<<" nativeevents "<<g.trace.size()<<" sourceevents "<<trace.size()<<'\n';for(unsigned j=0;j<6;++j)std::cerr<<"actor "<<j<<" target "<<g.snapshot()[j].target<<'/'<<after[j].target<<" last "<<g.snapshot()[j].last<<'/'<<after[j].last<<'\n';}check(error.empty()&&result.complete&&result.self_map_cleared&&g.snapshot()==after&&g.trace==trace);notifications+=result.notifications;clears+=result.targets_cleared;if(!incoming&&before[0].outgoing.size()>=3)populated=before;}
 check(!populated[0].outgoing.empty());
 std::string error;AggroClearAllResultV2 r{};
 g.initialize(populated);g.fail_notify=true;check(!character_aggro_clear_all_v2(r,g.borrow(0),g.services(),error)&&r.self_map_cleared&&!r.complete&&r.notifications==0&&g.actors[0].outgoing.count==0);check(g.trace.size()==1);
 g.initialize(populated);g.reenter=true;check(character_aggro_clear_all_v2(r,g.borrow(0),g.services(),error)&&g.entered&&r.notifications==populated[0].outgoing.size());
 g.initialize(populated);g.add_relation=true;check(character_aggro_clear_all_v2(r,g.borrow(0),g.services(),error)&&g.actors[0].outgoing.count==1&&g.actors[0].outgoing.entries[0].character==Graph::id(5));
 g.initialize(populated);g.actors[0].outgoing.entries[0].reserved=1;const auto before=g.snapshot();check(!character_aggro_clear_all_v2(r,g.borrow(0),g.services(),error)&&g.trace.empty()&&g.snapshot()==before);
 std::cout<<"{\"status\":\"PASS\",\"source_gold_cases\":"<<count<<",\"checks\":"<<checks<<",\"ordered_notifications\":"<<notifications<<",\"actual_target_sync_branches\":"<<clears<<",\"full_game_death\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
