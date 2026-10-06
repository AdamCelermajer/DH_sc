#include "../target_frontal_sort_v41.hpp"
#include <array>
#include <cassert>
#include <iostream>
using namespace dh2::target_search;
struct Fixture {
 std::array<Object48,6> actors{};std::array<Entry16,6> entries{};Entry16 end{};Room16 room{},rooms{};Registry8 registry{&rooms};std::array<Target24,6> heap{};List40 list{};Services16 services{this,call};std::array<bool,6> dead{},hostile{};
 Fixture(){for(unsigned i=0;i<6;++i){actors[i].identity=i+1;actors[i].visible=1;hostile[i]=true;entries[i]={i==5?&end:&entries[i+1],&actors[i]};}end.next=entries.data();room={&rooms,&end};rooms.next=&room;actors[0].character_word1314=100;assert(!dh2_target_list_init(&list,heap.data(),6,actors.data(),1,&services));}
 static int call(void* p,const Request24* q,Response16* r){auto& f=*static_cast<Fixture*>(p);*r={};auto id=q->service==is_enemy?q->other:q->subject;auto i=id?unsigned(id-1):0;
 switch(q->service){case resolve_character:r->word=id?reinterpret_cast<std::uintptr_t>(&f.actors[i]):0;break;case is_character:case is_interactive:r->word=1;break;case is_dead:r->word=f.dead[i];break;case is_enemy:r->word=f.hostile[i];break;case is_player:r->word=id==1;break;default:break;}return 0;}
 unsigned first(float radius=100,float cone=6.2831855f){assert(!dh2_target_search(&list,&registry,radius,cone,&services));Target24 t{};assert(!dh2_target_pop(&list,&t));return unsigned(t.identity);}
};
int main(){unsigned checks=0;std::string error;
 // Heading0 is -Y. Closest side actor precedes the farther frontal actor in registry order.
 Fixture f;f.actors[1].position[0]=1;f.actors[2].position[1]=-20;f.actors[3].position[1]=1;f.actors[4].position[1]=-10;f.actors[5].position[1]=-5;
 f.dead[4]=true;f.hostile[5]=false;
 assert(f.first()==2);++checks; // closest policy is genuinely different
 assert(!dh2::character::target_frontal_sort_v41(f.list,error)&&f.list.sort==2&&f.list.count==0);++checks;
 assert(f.first()==3);++checks; // frontal beats closer side/back/dead/friendly
 assert(f.first(19)==2);++checks; // max range excludes frontal20
 assert(f.first(20)==3);++checks; // exact boundary included
 f.actors[2].visible=0;assert(f.first()==2);++checks;f.actors[2].visible=1;
 f.actors[1].position[0]=0;f.actors[1].position[1]=-2;assert(f.first()==2);++checks; // equal-angle ties preserve source heap policy, not added distance sort
 f.actors[1].position[1]=2;assert(f.first(100,0.1f)==3);++checks; // behind excluded narrow cone
 f.list.count=7;const auto before=f.list;assert(dh2::character::target_frontal_sort_v41(f.list,error)<0&&f.list.count==before.count&&f.list.sort==before.sort);++checks;
 f.list.count=0;f.list.capacity=65537;assert(dh2::character::target_frontal_sort_v41(f.list,error)<0);++checks;
 f.list.capacity=6;f.list.sort=3;assert(dh2::character::target_frontal_sort_v41(f.list,error)<0);++checks;
 std::cout<<"frontal target identity contrasts PASS "<<checks<<"; named eligibility callbacks are fixtures; real native search/heap/sort executed\n";
}
