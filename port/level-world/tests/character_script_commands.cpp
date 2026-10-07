#include "../character_script_commands.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
unsigned checks=0,guards=0;void check(bool ok){++checks;if(!ok)throw std::runtime_error("Script command mismatch");}
unsigned word(std::istream& s){unsigned v;s.read(reinterpret_cast<char*>(&v),4);check(bool(s));return v;}
float number(unsigned v){float f;std::memcpy(&f,&v,4);return f;}
unsigned bits(float f){unsigned v;std::memcpy(&v,&f,4);return v;}
constexpr std::uintptr_t identities[]={0,0x100000001ull,0x100000002ull,0x100000003ull,0x100000004ull};
using Row=std::array<unsigned,6>;
struct Fixture {
 std::vector<Row> calls;std::vector<unsigned> conversions;std::vector<dh2_script_value> values;
 std::vector<std::array<unsigned,4>> descriptors;
 std::array<std::array<float,3>,4> points;
 int failure=-1;ScriptCommandState48 state{};ScriptCommandServices24 services{this,invoke,convert};
 static unsigned id(std::uintptr_t identity){for(unsigned i=0;i<5;++i)if(identity==identities[i])return i;throw std::runtime_error("Bad identity");}
 static int invoke(void* p,ScriptCommandState48* s,const ScriptCommandRequest40* r,const float** out){auto& f=*static_cast<Fixture*>(p);check(s==&f.state&&!r->reserved&&!r->reserved1);f.calls.push_back({r->service,id(r->subject),id(r->target),bits(r->point[0]),bits(r->point[1]),bits(r->point[2])});
  if(int(r->service)==f.failure)return 1;
  if(r->service==script_target_position)*out=f.points[r->subject==identities[1]?0:1].data();
  if(r->service==script_look_vector)*out=f.points[2].data();return 0;
 }
 static int convert(void* p,const dh2_script_value* a,float* out){auto& f=*static_cast<Fixture*>(p);auto index=a-f.values.data();check(index>=0&&std::size_t(index)<f.values.size());f.conversions.push_back(index);auto v=f.descriptors[index];
  if(v[0]==1)*out=float(v[2]);else if(v[0]==3||v[0]==4)*out=number(v[1]);
  // The ARM32 fixture stores identities as its actual synthetic source
  // addresses. Native IDs are opaque64; the explicit numeric service maps
  // them to that original uint32 identity instead of truncating pointers.
  else if(v[0]==2||v[0]==7)*out=v[3]?float(0x02000000u+0x30000u+(v[3]==4?0x10000u:0)):0;
  else *out=0;return 0;
 }
};
bool same_word(unsigned a,unsigned b){return a==b||((a&0x7fffffff)>0x7f800000&&(b&0x7fffffff)>0x7f800000);}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream s(argv[1],std::ios::binary);check(word(s)==0x314d4353);auto count=word(s);unsigned calls=0,conversions=0;
 for(unsigned n=0;n<count;++n){auto op=word(s),arity=word(s),target=word(s),path=word(s);Fixture f;f.values.resize(arity);f.descriptors.resize(arity);
  for(unsigned i=0;i<arity;++i){auto& d=f.descriptors[i];for(auto& v:d)v=word(s);auto& a=f.values[i];a={};a.type=d[0];a.number=number(d[1]);a.boolean=d[2];a.identity=identities[d[3]];}
  s.read(reinterpret_cast<char*>(f.points.data()),48);check(bool(s));f.state={identities[1],identities[2],target?identities[3]:0,f.points[0].data(),f.points[3].data(),path,0};
  unsigned returned=99;dh2_script_value out{};check(dh2_character_script_command(&f.state,op,f.values.data(),arity,&f.services,&out,1,&returned)==1);
  auto expected_calls=word(s);check(f.calls.size()==expected_calls);for(unsigned i=0;i<expected_calls;++i)for(unsigned j=0;j<6;++j){auto expected=word(s);check(j<3?f.calls[i][j]==expected:same_word(f.calls[i][j],expected));}calls+=expected_calls;
  auto expected_conversions=word(s);check(f.conversions.size()==expected_conversions);for(auto v:f.conversions)check(v==word(s));conversions+=expected_conversions;
  auto expected_returns=word(s);check(returned==expected_returns);if(returned)check(out.type==DH2_SCRIPT_BOOLEAN&&out.boolean==word(s));
 }check(s.peek()==EOF);
 Fixture f;f.state={identities[1],identities[2],identities[3],f.points[0].data(),f.points[3].data(),0,0};unsigned returned=7;dh2_script_value out{};
 check(dh2_character_script_command(nullptr,0,nullptr,0,&f.services,&out,1,&returned)==-1&&returned==7);++guards;
 check(dh2_character_script_command(&f.state,6,nullptr,0,&f.services,&out,1,&returned)==-1&&returned==7);++guards;
 check(dh2_character_script_command(&f.state,5,nullptr,0,&f.services,nullptr,0,&returned)==-1&&returned==7);++guards;
 check(dh2_character_script_command(&f.state,1,nullptr,0,&f.services,&out,1,&returned)==-3&&f.calls.empty());++guards;
 check(dh2_character_script_command(&f.state,0,nullptr,0,nullptr,&out,1,&returned)==-2&&f.calls.empty());++guards;
 f.failure=script_controller_attack;check(dh2_character_script_command(&f.state,3,nullptr,0,&f.services,&out,1,&returned)==-2&&f.calls.size()==1);++guards;
 f.failure=script_target_position;f.calls.clear();dh2_script_value object{};object.type=2;object.identity=identities[4];check(dh2_character_script_command(&f.state,4,&object,1,&f.services,&out,1,&returned)==-2&&f.calls.size()==1);++guards;
 object.reserved=1;f.calls.clear();check(dh2_character_script_command(&f.state,3,&object,1,&f.services,&out,1,&returned)==-1&&f.calls.empty());++guards;
 Dl_info module{};check(dladdr(reinterpret_cast<void*>(dh2_character_script_command),&module));
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"checks\":"<<checks<<",\"ordered_requests\":"<<calls<<",\"number_conversions\":"<<conversions<<",\"guards\":"<<guards<<",\"module_library\":\""<<module.dli_fname<<"\",\"full_controller_path_combat\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
