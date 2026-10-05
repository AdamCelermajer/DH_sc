#include "../character_target_search_v5.hpp"
#include <cassert>
#include <cmath>
#include <vector>
using namespace dh2::target_search;
struct Fixture {
 Object48 owner{},first{},second{};Target24 heap[8]{};List40 list{};
 Room16 sentinel{},room{};Entry16 end{},entries[2]{};Registry8 registry{};
 std::vector<unsigned> trace;bool fail_second=false;
 Fixture(){owner.identity=1;owner.visible=1;owner.has_target_position=1;owner.target_position[1]=-1000;
  first.identity=2;first.visible=1;first.position[1]=-10;second.identity=3;second.visible=1;second.position[1]=-20;
  list={heap,0,8,&owner,&owner,1,0};sentinel.next=&room;room={&sentinel,&end};
  end.next=&entries[0];entries[0]={&entries[1],nullptr};entries[1]={&end,nullptr};registry.rooms=&sentinel;
 }
 static int query(void* p,const Request24* q,Response16* r){auto& self=*static_cast<Fixture*>(p);self.trace.push_back(q->service);*r={};
  switch(q->service){case melee_radius:self.owner.position[0]=1000;return 0;
   case resolve_character:return 0;case is_zonable:return 0;case is_interactive:r->word=1;return 0;
   case interaction_type:r->word=8;return 0;case interaction_radius:return 0;default:return -1;}
 }
 static int resolve(void* p,const Entry16* entry,Object48** out){auto& self=*static_cast<Fixture*>(p);
  const auto which=entry==&self.entries[0]?0u:1u;self.trace.push_back(20+which);
  if(which&&self.fail_second)return -1;*out=which?&self.second:&self.first;return 0;}
};
int main(){Fixture f;const Services16 services{&f,Fixture::query};const SnapshotResolve16V5 resolver{&f,Fixture::resolve};
 const float origin[3]{};
 assert(dh2_target_search_snapshot_v5(&f.list,&f.registry,30,3.141592741f,origin,&services,&resolver)==0);
 assert(f.list.count==2&&f.list.heap[0].identity==2&&std::fabs(f.list.heap[0].distance-10)<0.0001f);
 assert((f.trace==std::vector<unsigned>{9,20,1,7,4,5,6,21,1,7,4,5,6}));
 f.trace.clear();f.fail_second=true;
 assert(dh2_target_search_snapshot_v5(&f.list,&f.registry,30,3.141592741f,origin,&services,&resolver)==2);
 assert(f.list.count==1&&f.list.heap[0].identity==2&&f.trace.back()==21);
 const auto saved=f.list;
 assert(dh2_target_search_snapshot_v5(&f.list,&f.registry,30,3.141592741f,nullptr,&services,&resolver)==1);
 assert(f.list.count==saved.count&&f.list.heap==saved.heap);
}
