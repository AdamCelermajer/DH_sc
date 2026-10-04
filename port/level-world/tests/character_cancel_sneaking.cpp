#include "../character_cancel_sneaking.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character::sneaking;
using Words=std::vector<std::uint32_t>;
unsigned checks=0,cases=0,calls=0,guards=0,actual=0;
void check_at(bool v,int line){++checks;if(!v)throw std::runtime_error("CancelSneaking host comparison failed at line "+std::to_string(line));}
#define check(value) check_at((value),__LINE__)
std::uint32_t word(std::istream& f){std::uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
std::int32_t signed_word(std::uint32_t v){std::int32_t out;std::memcpy(&out,&v,4);return out;}
struct Fixture {
 std::array<Character48,2> characters{};AI24 ai{};Tables32 table{};std::array<std::array<std::int32_t,224>,2> props{};
 std::array<Skill76,6> skills{};std::array<List16,4> lists{};std::array<std::array<std::int32_t,5>,4> ids{};
 std::array<std::uintptr_t,6> scripts{};Services16 services{this,invoke};Words input,trace;int failing=-1;
 Words snapshot(){return {characters[0].changed415,static_cast<std::uint32_t>(props[0][198]),ai.owner==&characters[0]?0u:1u};}
 static int invoke(void* p,const Request24* r,std::uint32_t* out){
  auto& f=*static_cast<Fixture*>(p);auto op=r->operation;check(!r->reserved&&op<4);auto receiver=op<2?r->receiver-0x100000001:r->receiver?r->receiver-0x200000000:0xffffffffu;
  Words row{op,static_cast<std::uint32_t>(receiver),r->index,r->argument};auto snap=f.snapshot();row.insert(row.end(),snap.begin(),snap.end());f.trace.insert(f.trace.end(),row.begin(),row.end());++calls;
  if(static_cast<int>(op)==f.failing)return 1;
  auto mode=f.input[7];if(op==1&&(mode==1||mode==2))f.props[0][198]=mode==1?0:1;
  if(op==1&&mode==6)f.ai.owner=&f.characters[1];
  if(op==2&&(mode==3||mode==5))f.scripts[r->index]=mode==5?0:0x200000007;
  if(op==2&&mode==4)f.ai.owner=&f.characters[1];
  *out=op==0?f.input[0]:op==2?f.input[6]:0;return 0;
 }
 void reset(const Words& v){
  input=v;trace.clear();failing=-1;services={this,invoke};for(auto& p:props)p.fill(0);
  for(unsigned i=0;i<2;++i){props[i][198]=signed_word(v[1]);props[i][28]=i?3:signed_word(v[2]);characters[i]={0x100000001ULL+i,{props[i].data(),224,0},&table,&ai,static_cast<std::uint8_t>(v[8]),{}};}
  for(unsigned i=0;i<6;++i){for(auto& w:skills[i].words)w=v[11];skills[i].words[7]=(v[11]&~0x2000000u)|((v[3]&(1u<<i))?0x2000000u:0);skills[i].words[18]=v[5];scripts[i]=(v[4]&(1u<<i))?0x200000001+i:0;}
  ids={{{0,0,0,0,0},{0,0,0,0,0},{2,4,1,0,0},{3,0,5,4,2}}};unsigned counts[]={0,1,3,5};for(unsigned i=0;i<4;++i)lists[i]={ids[i].data(),counts[i],0};table={lists.data(),4,0,skills.data(),6,0};ai={&characters[0],scripts.data(),6,0};
 }
 unsigned run(){return input[9]?dh2_character_cancel_skill(&ai,input[10],&services):dh2_character_cancel_sneaking(&characters[0],&services);}
};
// Test-only serialized traversal derived from complete Skill/SkillList read
// instructions. Owned strings/array contents are skipped; null projected pointer
// fields are never consumed by the cancel coordinator. This is not a loader API.
struct ActualTables {
 std::vector<std::vector<std::int32_t>> ids;std::vector<List16> lists;std::vector<Skill76> rows;Tables32 table{};
 explicit ActualTables(const char* file){std::ifstream f(file,std::ios::binary);check(bool(f));auto count=word(f);check(count==36);ids.resize(count);lists.resize(count);
  for(unsigned i=0;i<count;++i){auto n=word(f);check(n<=32);for(unsigned j=0;j<n;++j)ids[i].push_back(signed_word(word(f)));lists[i]={ids[i].data(),n,0};}
  count=word(f);check(count==127);rows.resize(count);
  auto byte=[&](){auto x=f.get();check(x!=EOF);return static_cast<std::uint32_t>(x);};
  auto text=[&](){auto n=word(f);check(n<65536);f.ignore(n);check(bool(f));return n;};
  for(auto& row:rows){auto* r=row.words;r[1]=word(f);r[2]=byte();r[3]=word(f);check(r[3]<65536);for(unsigned i=0;i<r[3];++i)word(f);r[5]=word(f);r[6]=byte();r[7]=word(f);r[8]=word(f);r[9]=text();r[11]=byte();r[12]=word(f);r[13]=word(f);r[14]=text();r[16]=word(f);r[17]=word(f);r[18]=word(f);check(!(r[7]&0x2000000));}
  check(f.peek()==EOF);for(const auto& l:ids)for(auto id:l)check(id>=0&&static_cast<unsigned>(id)<rows.size());table={lists.data(),36,0,rows.data(),127,0};
 }
};
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream gold(argv[1],std::ios::binary);check(word(gold)==0x314b5343);auto n=word(gold);check(n==976);Fixture f;
 for(unsigned i=0;i<n;++i){Words input(12),expected(3);for(auto& x:input)x=word(gold);for(auto& x:expected)x=word(gold);auto count=word(gold);Words trace(count*7);for(auto& x:trace)x=word(gold);f.reset(input);check(!f.run());check(f.snapshot()==expected);check(f.trace==trace);++cases;}check(gold.peek()==EOF);
 ActualTables tables(argv[2]);for(int id=-1;id<37;++id)for(unsigned player=0;player<2;++player)for(unsigned sneak=0;sneak<2;++sneak){f.reset({player,sneak,static_cast<std::uint32_t>(id),0,0,0,0,0,0xa5,0,0,0});f.table=tables.table;check(!f.run());check(f.characters[0].changed415==(player?1:0xa5));check(f.trace.size()==(player?14:7));++actual;}
 auto valid=[&](){f.reset({1,1,3,8,63,1,1,0,0xa5,0,0,0});};
 for(int op=0;op<4;++op){valid();f.failing=op;check(f.run()==2);check(f.trace.size()==static_cast<unsigned>((op+1)*7));check(f.characters[0].changed415==(op<2?0xa5:1));++guards;}
 valid();check(dh2_character_cancel_sneaking(nullptr,&f.services)==1);++guards;
 alignas(Character48) unsigned char bad[sizeof(Character48)+1]{};check(dh2_character_cancel_sneaking(reinterpret_cast<Character48*>(bad+1),&f.services)==1);++guards;
 valid();f.characters[0].resolved.words=nullptr;check(f.run()==1&&f.characters[0].changed415==1);++guards;
 valid();f.characters[0].resolved.count=198;check(f.run()==1);++guards;
 valid();f.table.list_count=3;f.props[0][28]=-1;check(f.run()==1);++guards;
 valid();f.ids[3][0]=-1;check(f.run()==1);++guards;
 valid();f.table.skill_count=3;check(f.run()==1);++guards;
 valid();f.ai.count=0;check(f.run()==1);++guards;
 valid();f.ai.owner=nullptr;check(f.run()==1);++guards;
 valid();f.scripts[0]=0;f.services.invoke=nullptr;check(dh2_character_cancel_skill(&f.ai,0,&f.services)==0);++guards;
 valid();f.props[0][198]=0;f.characters[0].tables=nullptr;f.input[0]=0;check(!f.run()&&f.trace.size()==7);++guards;
 // Live callback mutations were replayed above; no global pending guard is
 // introduced. Whole nested skill-script VM execution remains outside this test.
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"cases\":"<<cases<<",\"ordered_calls\":"<<calls<<",\"actual_cache_cases\":"<<actual<<",\"actual_lists\":36,\"actual_skills\":127,\"native_guards\":"<<guards<<",\"full_DelBuff\":false,\"full_skill_VM\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
