#include "character_target_search.hpp"
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
using namespace dh2::target_search;
static std::map<std::pair<unsigned,unsigned>,unsigned> libm;
static unsigned libm_calls=0;
static float fl(unsigned w){float f;std::memcpy(&f,&w,4);return f;}
static unsigned bits(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
static float imported(unsigned op,float input){++libm_calls;auto p=libm.find({op,bits(input)});if(p!=libm.end())return fl(p->second);if(std::isnan(input))return std::nanf("");std::fprintf(stderr,"Unknown libm %u/%08x\n",op,bits(input));std::abort();}
extern "C" float sinf(float x) noexcept{return imported(1,x);}
extern "C" float cosf(float x) noexcept{return imported(2,x);}
extern "C" float acosf(float x) noexcept{return imported(3,x);}
extern "C" void sincosf(float x,float* s,float* c) noexcept{*s=imported(1,x);*c=imported(2,x);}
extern "C" float __wrap_sinf(float x){return imported(1,x);}
extern "C" float __wrap_cosf(float x){return imported(2,x);}
extern "C" float __wrap_acosf(float x){return imported(3,x);}
extern "C" void __wrap_sincosf(float x,float* s,float* c){*s=imported(1,x);*c=imported(2,x);}
struct Case {std::array<unsigned,12> h;std::vector<std::array<unsigned,20>> objects;std::vector<std::vector<unsigned>> rooms;std::vector<std::array<unsigned,5>> outputs;std::vector<std::array<unsigned,2>> trace;std::vector<unsigned> visible;};
static void need(bool p,const char* message){if(!p){std::fprintf(stderr,"%s\n",message);std::abort();}}
struct Reader {std::vector<unsigned char> data;size_t at=0;unsigned word(){need(at+4<=data.size(),"truncated corpus");unsigned v;std::memcpy(&v,data.data()+at,4);at+=4;return v;}template<size_t N>std::array<unsigned,N> array(){std::array<unsigned,N> a{};for(auto&v:a)v=word();return a;}};
struct Fixture {
 Case* test;std::vector<Object48> objects;std::vector<Room16> rooms;std::vector<Entry16> heads;std::vector<std::vector<Entry16>> entries;Room16 sentinel{};Registry8 registry{&sentinel};List40 list{};std::array<Target24,128> storage{};Services16 services{this,&invoke};std::vector<std::array<unsigned,2>> trace;bool triggered=false;unsigned nested=0;int provider=0;
 explicit Fixture(Case& c):test(&c),objects(c.objects.size()),rooms(c.rooms.size()),heads(c.rooms.size()),entries(c.rooms.size()) {
  for(size_t i=0;i<objects.size();++i){auto&w=c.objects[i];auto&o=objects[i];o={};o.identity=i+1;for(int j=0;j<3;++j){o.position[j]=fl(w[j]);o.target_position[j]=fl(w[16+j]);}o.character_word1310=static_cast<int>(w[3]);o.character_word1314=static_cast<int>(w[4]);o.visible=w[5];o.zoned=w[6];o.in_zone=w[7];o.has_target_position=w[19];o.rotation=i?0:fl(c.h[3]);}
  sentinel.next=rooms.empty()?&sentinel:rooms.data();
  for(size_t i=0;i<rooms.size();++i){rooms[i]={i+1<rooms.size()?&rooms[i+1]:&sentinel,&heads[i]};entries[i].resize(c.rooms[i].size());heads[i].next=entries[i].empty()?&heads[i]:entries[i].data();for(size_t j=0;j<entries[i].size();++j){auto index=c.rooms[i][j];entries[i][j]={j+1<entries[i].size()?&entries[i][j+1]:&heads[i],index==0xffffffff?nullptr:&objects.at(index)};}}
 }
 static int invoke(void* context,const Request24* q,Response16* out) {
  auto&f=*static_cast<Fixture*>(context);if(f.provider)return 1;auto idx=static_cast<unsigned>((q->service==is_enemy?q->other:q->subject)-1);f.trace.push_back({q->service,idx});*out={};const auto&w=f.test->objects[idx==0xffffffff?0:idx];
  if(q->service==is_interactive||q->service==interaction_type)need(q->other==1,"owner argument mismatch");
  if(q->service==is_enemy)need(q->subject==1,"enemy owner mismatch");
  switch(q->service){case resolve_character:out->word=idx!=0xffffffff&&w[8]?reinterpret_cast<std::uintptr_t>(&f.objects[idx]):0;break;case is_player:out->word=w[9];break;case is_dead:out->word=w[10];break;case is_interactive:out->word=w[11];break;case interaction_type:out->word=w[12];break;case interaction_radius:out->number=fl(w[13]);break;case is_zonable:out->word=w[14];break;case is_enemy:out->word=w[15];break;case melee_radius:out->number=fl(f.test->h[4]);break;case is_character:out->word=w[8];break;default:std::abort();}
  if(q->service==is_interactive&&f.test->h[7]==idx&&!f.triggered){f.triggered=true;auto j=f.test->h[8];f.objects[j].visible=0;f.trace.push_back({11,j});}
  if(q->service==is_interactive&&f.test->h[9]==idx&&!f.triggered){f.triggered=true;f.trace.push_back({12,idx});++f.nested;need(dh2_target_search(&f.list,&f.registry,fl(f.test->h[5]),fl(f.test->h[6]),&f.services)==0,"nested search failed");f.trace.push_back({13,idx});}
  return 0;
 }
};
static bool equal(unsigned a,unsigned b){return a==b||(std::isnan(fl(a))&&std::isnan(fl(b)));}
int main(int argc,char**argv){need(argc==2,"usage: target_search_audit corpus");Reader r;std::ifstream file(argv[1],std::ios::binary);r.data={std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};need(r.word()==0x31525354,"bad corpus");auto count=r.word();std::vector<Case> cases(count);
 for(auto&c:cases){c.h=r.array<12>();for(unsigned i=0;i<c.h[0];++i)c.objects.push_back(r.array<20>());for(unsigned i=0;i<c.h[1];++i){unsigned n=r.word();std::vector<unsigned>room(n);for(auto&v:room)v=r.word();c.rooms.push_back(room);}for(unsigned i=0;i<c.h[10];++i)c.outputs.push_back(r.array<5>());for(unsigned i=0;i<c.h[11];++i)c.trace.push_back(r.array<2>());for(unsigned i=0;i<c.h[0];++i)c.visible.push_back(r.word());}
 unsigned math=r.word();for(unsigned i=0;i<math;++i){auto row=r.array<3>();libm[{row[0],row[1]}]=row[2];}need(r.at==r.data.size(),"trailing bytes");unsigned callbacks=0,outputs=0,nested=0,guards=0,provider=0;
 for(auto&c:cases){Fixture f(c);need(dh2_target_list_init(&f.list,f.storage.data(),128,&f.objects[0],c.h[2],&f.services)==0,"init failed");f.trace.clear();need(dh2_target_search(&f.list,&f.registry,fl(c.h[5]),fl(c.h[6]),&f.services)==0,"search failed");need(f.trace==c.trace,"callback order mismatch");need(f.list.count==c.outputs.size(),"count mismatch");for(auto&e:c.outputs){Target24 out{};need(dh2_target_pop(&f.list,&out)==0,"pop failed");need(out.identity==std::uintptr_t(e[0])+1&&equal(bits(out.distance),e[1])&&equal(bits(out.angle),e[2])&&out.flags==e[3]&&out.reserved==e[4],"target mismatch");++outputs;}for(size_t i=0;i<c.visible.size();++i)need(f.objects[i].visible==c.visible[i],"mutation mismatch");callbacks+=f.trace.size();nested+=f.nested;
  auto saved=f.list;auto bad=[&](int result){need(result==1&&std::memcmp(&saved,&f.list,sizeof(saved))==0,"atomic guard failed");++guards;};bad(dh2_target_list_init(&f.list,f.storage.data(),0,&f.objects[0],0,&f.services));bad(dh2_target_list_init(&f.list,f.storage.data(),128,&f.objects[0],3,&f.services));bad(dh2_target_search(&f.list,nullptr,0,0,&f.services));bad(dh2_target_search(&f.list,&f.registry,0,0,nullptr));bad(dh2_target_pop(&f.list,f.storage.data()));bad(dh2_target_pop(&f.list,reinterpret_cast<Target24*>(&f.list)));bad(dh2_target_list_init(&f.list,reinterpret_cast<Target24*>(&f.list),1,&f.objects[0],0,&f.services));Target24 out{};need(dh2_target_pop(&f.list,&out)==2,"empty pop failed");
  f.provider=1;need(dh2_target_search(&f.list,&f.registry,0,0,&f.services)==2,"provider failure failed");++provider;
  f.provider=0;f.trace.clear();Entry16 end{},bad_entry{};bad_entry={&end,reinterpret_cast<Object48*>(reinterpret_cast<std::uintptr_t>(&f.objects[0])+1)};end.next=&bad_entry;Room16 room_end{},room{};room_end.next=&room;room={&room_end,&end};Registry8 malformed{&room_end};need(dh2_target_search(&f.list,&malformed,0,0,&f.services)==2,"unaligned borrowed object accepted");need(f.trace.size()==(f.list.reference_character?1u:0u),"malformed object reached resolve callback");++provider;
  f.trace.clear();room.objects=reinterpret_cast<Entry16*>(reinterpret_cast<std::uintptr_t>(&end)+1);need(dh2_target_search(&f.list,&malformed,0,0,&f.services)==2,"unaligned room head accepted");++provider;
 }
 need(libm_calls>0,"libm import fixture was bypassed");
 std::printf("{\"validation\":\"PASS\",\"comparisons\":%u,\"ordered_callbacks\":%u,\"accepted_records\":%u,\"synchronous_same_list_reentries\":%u,\"atomic_rejection_checks\":%u,\"provider_failure_checks\":%u,\"libm_fixture_records\":%u,\"libm_fixture_calls\":%u,\"mismatches\":0}\n",count,callbacks,outputs,nested,guards,provider,math,libm_calls);
}
