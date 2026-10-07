#include "character_relationships.hpp"
#include <vector>
#include <array>
#include <fstream>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <cstring>
namespace rel=dh2::relationships;namespace tp=dh2::target_providers;
using Trace=std::array<unsigned,3>;
static void need(bool b,const char*m){if(!b){std::fprintf(stderr,"%s\n",m);std::abort();}}
struct Reader{std::vector<unsigned char>d;size_t at=0;unsigned word(){need(at+4<=d.size(),"truncated corpus");unsigned v;std::memcpy(&v,d.data()+at,4);at+=4;return v;}template<size_t N>std::array<unsigned,N> array(){std::array<unsigned,N>v{};for(auto&x:v)x=word();return v;}};
static const char* names[]={"PlayerCharacter","Other","PlayerCharacterPrince","xPlayerCharacter","Player","","PlayerCharacte","playerCharacter"};
struct Fixture{
 std::array<unsigned,18>row;std::vector<int>types;tp::Types16 typeheader{};std::vector<std::vector<dh2::data::AiFactionEntry>> factions;std::vector<rel::FactionRow16>faction_rows;rel::Factions16 table{};
 std::array<rel::Object48,4>objects{};std::array<dh2::character::CombatProperties896,4>props{};std::array<tp::Handle16,4>handles{};std::array<rel::State16,4>states{};std::array<tp::Record16,16>records{};tp::Registry24 registry{records.data(),1,16,9,0};rel::Services16 services{this,&invoke},provider{this,&provide};std::vector<Trace>trace;bool triggered=false;int failure=0;
 Fixture(const std::array<unsigned,18>&r,const std::vector<int>&t,const std::vector<std::vector<dh2::data::AiFactionEntry>>&f):row(r),types(t),factions(f){
  typeheader={types.data(),static_cast<unsigned>(types.size()),0};if(row[14])factions[0]={{15,1000},{1,static_cast<int>(row[15])},{1,row[15]<0x80000000?-1:1}};
  faction_rows.resize(factions.size());for(size_t i=0;i<factions.size();++i)faction_rows[i]={factions[i].data(),static_cast<unsigned>(factions[i].size()),0};table={faction_rows.data(),static_cast<unsigned>(faction_rows.size()),0};
  for(unsigned i=0;i<4;++i){props[i].words[0]=static_cast<int>(i==0?row[1]:i==3?10:row[2]);props[i].words[1]=static_cast<int>(i==0?row[3]:i==3?44:row[4]);objects[i]={{i+1,(row[13]&&row[7]&&(i==1||i==2))?nullptr:&props[i],names[(i==0?row[5]:row[6])-1],0x2000,static_cast<unsigned char>((i==1||i==2)?row[16]:0),0,1,static_cast<unsigned char>((i==1||i==2)?row[17]:1)},&handles[i],static_cast<int>((i==1||i==2)?row[7]:0),0};handles[i]={static_cast<int>(i==1?row[9]:7),3,i==1?(row[8]?reinterpret_cast<std::uintptr_t>(&objects[row[8]-1]):0):reinterpret_cast<std::uintptr_t>(&objects[i])};states[i]={&objects[i],row[11]?&objects[1]:nullptr};}records[0]={7,0,reinterpret_cast<std::uintptr_t>(&objects[2])};
 }
 unsigned index(std::uintptr_t id){need(id>=1&&id<=4,"bad service identity");return static_cast<unsigned>(id-1);}
 void observe(unsigned obj){if(triggered||!row[12]||(row[12]==4&&obj==0))return;triggered=true;if(row[12]==1)states[0].owner=&objects[3];else if(row[12]==2||row[12]==4)props[obj].words[0]=row[12]==2?7:10;else{trace.push_back({7,0,0});int out;need(rel::dh2_character_relationship(&out,2,&states[0],&objects[1],&registry,&table,&services)==0,"nested friend failed");trace.push_back({8,0,0});}}
 static int invoke(void*p,const rel::Request24*q,std::uintptr_t*out){auto&f=*static_cast<Fixture*>(p);if(f.failure)return 1;auto i=f.index(q->subject);f.trace.push_back({q->service,static_cast<unsigned>(q->subject),static_cast<unsigned>(q->other)});int value=0;
  if(q->service==rel::virtual_player){f.observe(i);need(tp::dh2_character_target_query(&value,tp::is_player,&f.objects[i].character,nullptr,&f.typeheader,&f.provider)==0,"player backend failed");}
  else if(f.row[13]&&i==1){need(tp::dh2_gameobject_target_query(&value,q->service==rel::virtual_interactive?1:2,0)==0,"base backend failed");}
  else {auto j=f.index(q->other);need(tp::dh2_character_target_query(&value,q->service==rel::virtual_interactive?tp::is_interactive:tp::interaction_type,&f.objects[i].character,&f.objects[j].character,&f.typeheader,&f.provider)==0,"interaction backend failed");}
  *out=static_cast<unsigned>(value);return 0;
 }
 static int provide(void*p,const tp::Request24*q,std::uintptr_t*out){auto&f=*static_cast<Fixture*>(p);auto i=f.index(q->subject);int value=0;
  if(q->service==tp::virtual_dead){f.trace.push_back({11,static_cast<unsigned>(q->subject),0});value=f.objects[i].character.dead1449;}
  else if(q->service==tp::virtual_player){f.trace.push_back({1,static_cast<unsigned>(q->subject),0});f.observe(i);need(tp::dh2_character_target_query(&value,tp::is_player,&f.objects[i].character,nullptr,&f.typeheader,&f.provider)==0,"nested player failed");}
  else {need(q->service==tp::ai_friend||q->service==tp::ai_enemy,"unknown provider service");f.trace.push_back({q->service==tp::ai_friend?13u:14u,static_cast<unsigned>(q->subject),static_cast<unsigned>(q->other)});need(rel::dh2_character_relationship(&value,q->service==tp::ai_friend?2:1,&f.states[i],&f.objects[f.index(q->other)],&f.registry,&f.table,&f.services)==0,"recursive relationship failed");}
  *out=static_cast<unsigned>(value);return 0;
 }
};
int main(int argc,char**argv){need(argc==2,"usage: relationships_audit corpus");Reader r;std::ifstream file(argv[1],std::ios::binary);r.d={std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};need(r.word()==0x314c4552,"bad corpus");auto nt=r.word(),nf=r.word(),nc=r.word();std::vector<int>types(nt);for(auto&x:types)x=static_cast<int>(r.word());std::vector<std::vector<dh2::data::AiFactionEntry>>factions(nf);for(auto&f:factions){f.resize(r.word());for(auto&e:f){e.id=static_cast<int>(r.word());e.value=static_cast<int>(r.word());}}unsigned callbacks=0,guards=0,failures=0,nested=0;
 for(unsigned i=0;i<nc;++i){auto row=r.array<18>();auto result=r.word();auto frames=r.array<4>();auto owner=r.word();auto words=r.array<4>();auto count=r.word();auto ntrace=r.word();std::vector<std::array<unsigned,2>>entries(count);for(auto&e:entries)e=r.array<2>();std::vector<Trace>trace(ntrace);for(auto&t:trace)t=r.array<3>();Fixture f(row,types,factions);int out=0;need(rel::dh2_character_relationship(&out,row[0],&f.states[0],row[10]?&f.objects[1]:nullptr,&f.registry,&f.table,&f.services)==0,"relationship rejected");if(static_cast<unsigned>(out)!=result||f.trace!=trace)std::fprintf(stderr,"case %u result %u/%u traces %zu/%zu\n",i,static_cast<unsigned>(out),result,f.trace.size(),trace.size());need(static_cast<unsigned>(out)==result&&f.trace==trace&&f.registry.count==count&&f.states[0].owner==&f.objects[owner-1],"relationship result/order mismatch");for(unsigned j=0;j<4;++j)need(f.handles[j].frame==frames[j]&&static_cast<unsigned>(f.props[j].words[0])==words[j],"source state effects mismatch");for(unsigned j=0;j<count;++j)need(f.records[j].key==static_cast<int>(entries[j][0])&&f.records[j].object==(entries[j][1]?reinterpret_cast<std::uintptr_t>(&f.objects[entries[j][1]-1]):0),"ordered map effects mismatch");callbacks+=f.trace.size();for(auto&t:trace)nested+=t[0]==7;
  out=0x12345678;auto h=f.handles;auto bad=[&](int status){need(status==1&&out==0x12345678&&std::memcmp(h.data(),f.handles.data(),sizeof(h))==0,"atomic rejection effects");++guards;};bad(rel::dh2_character_relationship(&out,0,&f.states[0],&f.objects[1],&f.registry,&f.table,&f.services));bad(rel::dh2_character_relationship(&out,row[0],nullptr,&f.objects[1],&f.registry,&f.table,&f.services));bad(rel::dh2_character_relationship(&out,row[0],&f.states[0],reinterpret_cast<rel::Object48*>(reinterpret_cast<std::uintptr_t>(&f.objects[1])+1),&f.registry,&f.table,&f.services));auto saved=f.table;f.table.count=10;bad(rel::dh2_character_relationship(&out,row[0],&f.states[0],&f.objects[1],&f.registry,&f.table,&f.services));f.table=saved;need(rel::dh2_character_relationship(const_cast<int*>(f.states[0].owner->character.properties->words),row[0],&f.states[0],&f.objects[1],&f.registry,&f.table,&f.services)==1,"property alias accepted");++guards;
  f.failure=1;f.handles[1]={7,3,reinterpret_cast<std::uintptr_t>(&f.objects[1])};f.objects[1].object_type=2;need(rel::dh2_character_relationship(&out,1,&f.states[0],&f.objects[1],&f.registry,&f.table,&f.services)==2&&out==0x12345678&&f.handles[1].frame==f.registry.frame,"provider failure order");++failures;
  f.failure=0;f.handles[1]={12345,3,0};f.registry.capacity=f.registry.count;need(rel::dh2_character_relationship(&out,1,&f.states[0],&f.objects[1],&f.registry,&f.table,&f.services)==2&&out==0x12345678&&f.handles[1].frame==f.registry.frame,"capacity failure order");++failures;
  f.handles[1]={7,3,reinterpret_cast<std::uintptr_t>(&f.objects[1])+1};need(rel::dh2_character_relationship(&out,1,&f.states[0],&f.objects[1],&f.registry,&f.table,&f.services)==2&&out==0x12345678,"unaligned resolved provider accepted");++failures;
 }
 need(r.at==r.d.size(),"trailing corpus");std::printf("{\"validation\":\"PASS\",\"comparisons\":%u,\"ordered_callbacks\":%u,\"nested_relationships\":%u,\"atomic_rejection_checks\":%u,\"provider_failure_checks\":%u,\"mismatches\":0}\n",nc,callbacks,nested,guards,failures);
}
