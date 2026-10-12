#include "hud_initialization_v1.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::ui;
namespace {
void need(bool x,const char*s){if(!x)throw std::runtime_error(s);}
std::uint32_t hash(const char*s){if(!s||!*s)return 0;std::uint32_t h=2166136261u;for(;*s;++s)h=(h^static_cast<unsigned char>(*s))*16777619u;return h;}
std::int32_t signed_word(std::uint32_t u){std::int32_t s;std::memcpy(&s,&u,4);return s;}
struct Fixture {
 std::vector<std::uint32_t> v;std::vector<std::uint8_t> events;std::map<std::string,std::string> strings;std::array<std::vector<std::pair<std::int32_t,double>>,2> args;
 HudInitSkill96 skill{};std::array<std::int32_t,3> props{};unsigned calls{},fail_at{};std::uint8_t override{};std::uint32_t language=77;int actor,object,call;bool options{};
 std::uintptr_t actor_id(){return reinterpret_cast<std::uintptr_t>(&actor);}std::uintptr_t object_id(){return reinterpret_cast<std::uintptr_t>(&object);}std::uintptr_t call_id(){return reinterpret_cast<std::uintptr_t>(&call);}
 unsigned identity(std::uintptr_t p){return p==actor_id()?1:p==object_id()?2:p==call_id()?5:p==reinterpret_cast<std::uintptr_t>(&args[0])?3:p==reinterpret_cast<std::uintptr_t>(&args[1])?4:p==reinterpret_cast<std::uintptr_t>(&skill)?6:0;}
 const char* string(std::string s){auto i=strings.emplace(s,s).first;return i->second.c_str();}
 void event(const HudInitRequest64&q){std::uint32_t w[9]={static_cast<unsigned>(q.operation),q.index,static_cast<std::uint32_t>(q.value),static_cast<std::uint32_t>(q.other),q.type,identity(q.subject),identity(q.object),hash(q.text),hash(q.name)};auto*p=reinterpret_cast<const std::uint8_t*>(w);events.insert(events.end(),p,p+36);p=reinterpret_cast<const std::uint8_t*>(&q.number);events.insert(events.end(),p,p+8);}
 void setup(std::vector<std::uint32_t> values,bool opt){v=std::move(values);options=opt;events.clear();strings.clear();calls=0;language=77;args={};if(opt){override=v[7];return;}for(unsigned i=0;i<3;++i)props[i]=signed_word(v[27+i]);skill={props.data(),v[26],0,signed_word(v[13]),signed_word(v[22]),signed_word(v[21]),signed_word(v[20]),signed_word(v[23]),static_cast<std::uint8_t>(v[24]),static_cast<std::uint8_t>(v[25]),0,"authored_icon",123,{0,0,0,0,0}};}
 static int dispatch(void*p,const HudInitRequest64*q,HudInitResponse32*r){auto&f=*static_cast<Fixture*>(p);if(++f.calls==f.fail_at)return 0;auto op=static_cast<unsigned>(q->operation);auto i=q->index;auto&v=f.v;
  if(op>5&&op!=29&&op!=30)f.event(*q);
  if(f.options){switch(op){case 30:r->text=f.string(std::array<const char*,4>{"HUDStyle","Language","language","missing"}[v[1]]);break;case 4:r->identity=v[3]&&v[2]==5?f.object_id():0;break;case 31:r->value=signed_word(v[4]);break;case 32:r->value=signed_word(v[5]);break;case 33:r->value=signed_word(v[6]);break;case 34:r->value=f.override;break;case 35:f.language=static_cast<std::uint32_t>(q->value);break;case 36:r->value=signed_word(v[9]);break;case 18:r->text=q->value<0?nullptr:f.string("Option["+std::to_string(q->value)+"]");break;case 24:if(i==11&&v[8])f.override=1-f.override;break;case 26:case 28:break;default:throw std::runtime_error("Unknown option fixture");}return 1;}
  switch(op){case 1:r->value=v[2+i];r->identity=r->value==5?f.object_id():0;break;case 2:r->number=signed_word(v[6+i]);break;case 3:r->value=(i==3?v[9]:v[6+i])!=0;break;case 4:case 5:r->identity=v[11]&&v[2+i]==5?f.object_id():0;break;case 29:r->value=v[2+i]==2;break;case 6:r->identity=v[10]?f.actor_id():0;break;case 7:r->value=signed_word(v[q->other?33:30+i]);break;case 8:r->value=123;break;case 9:r->identity=reinterpret_cast<std::uintptr_t>(&f.skill);break;case 10:r->value=signed_word(v[12]);if(v[34]&1){++v[13];f.skill.required_level=signed_word(v[13]);}break;case 11:r->value=signed_word(v[14]);break;case 12:r->value=signed_word(v[15]);break;case 13:r->value=v[16];break;case 14:r->value=signed_word(v[17]);break;case 15:r->value=signed_word(v[19]);break;case 16:r->value=signed_word(v[18]);break;
   case 17:{if(!std::strcmp(q->text,"CharacterDesign"))r->value=5;else{const char*keys[]={"GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL","GAMEPLAYMENUS_NEEDS_SKILL_POINTS","GAMEPLAYMENUS_MAX_SKILL_LEVEL","GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING"};unsigned n=0;while(n<4&&std::strcmp(q->name,keys[n]))++n;need(n<4,"Unknown constant fixture");r->value=1000+n;}break;}
   case 18:r->text=f.string("String["+std::to_string(q->value)+"]");break;case 19:r->identity=reinterpret_cast<std::uintptr_t>(&f.args[i]);break;case 20:f.args[f.identity(q->subject)-3].push_back({q->value,q->number});break;case 21:r->fraction=.25f;if(v[34]&4){v[27]^=0x1234abcdu;f.props[0]=signed_word(v[27]);}break;case 22:r->value=signed_word(v[27+i%3]);break;
   case 23:{std::string s=q->text;s+='|';for(auto a:f.args[f.identity(q->subject)-3]){if(s.back()!='|')s+=',';char number[64];std::snprintf(number,sizeof number,"%.6g",a.second);s+=std::to_string(a.first)+":"+number;}r->text=f.string(s);break;}
   case 24:if((v[34]&2)&&i==0){v[21]=0xffffffffu;v[25]=0;f.skill.description_text=-1;f.skill.assignable=0;}break;case 25:case 26:case 27:case 28:break;default:throw std::runtime_error("Unknown initialization fixture");
  }return 1;
 }
 int run(){HudInitInput16 input{call_id(),options?(v[0]==3?2u:0u):v[1],4};HudInitServices16 s{this,dispatch};return dh2_ui_hud_initialization_v1(&input,v[0],&s);}
};
std::uint32_t read32(std::istream&f){std::uint32_t v;f.read(reinterpret_cast<char*>(&v),4);need(bool(f),"Truncated gold");return v;}
}
int main(int argc,char**argv){try{if(argc!=3)return 2;unsigned comparisons=0,ordered=0,prefixes=0,guards=0;Fixture f;
 for(unsigned file=0;file<2;++file){std::ifstream gold(argv[file+1],std::ios::binary);need(read32(gold)==(file?0x314f4948u:0x314e4948u),"Wrong gold");auto count=read32(gold);for(unsigned n=0;n<count;++n){auto words=read32(gold),events=read32(gold),language=file?read32(gold):0;std::vector<std::uint32_t>v(words);gold.read(reinterpret_cast<char*>(v.data()),words*4);std::vector<std::uint8_t> expected(events*44);gold.read(reinterpret_cast<char*>(expected.data()),expected.size());need(bool(gold),"Gold record truncated");f.fail_at=0;f.setup(v,file);need(f.run()==0,"Native source invocation failed");need(f.events==expected,"Ordered original service mismatch");if(file)need(f.language==language,"Original language publication mismatch");auto full_calls=f.calls;++comparisons;ordered+=events;
  // Every reached required provider may fail; the earlier delivered effects
  // must remain and no subsequent provider may execute.
  for(unsigned k=1;k<=full_calls;++k){f.setup(v,file);f.fail_at=k;auto rc=f.run();need(rc==-2,"Required failure swallowed");need(f.calls==k,"Failure prefix continued");need(f.events.size()<=expected.size()&&std::equal(f.events.begin(),f.events.end(),expected.begin()),"Failed prefix differs");++prefixes;}
 }}
 HudInitInput16 input{f.call_id(),2,4};HudInitServices16 services{&f,Fixture::dispatch};need(dh2_ui_hud_initialization_v1(nullptr,0,&services)==-1,"Null input accepted");++guards;need(dh2_ui_hud_initialization_v1(&input,5,&services)==-1,"Unknown entry accepted");++guards;input.available_arguments=1;need(dh2_ui_hud_initialization_v1(&input,3,&services)==-1,"Missing unguarded option arguments accepted");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<comparisons<<",\"ordered_services\":"<<ordered<<",\"required_failure_prefixes\":"<<prefixes<<",\"native_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
