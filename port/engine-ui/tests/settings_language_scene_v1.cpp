#include "../settings_language_scene_v1.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh2::ui;
namespace {
void check(bool b,const char* s){if(!b)throw std::runtime_error(s);}
struct Ctx {
 unsigned types[3],object_types[3];std::uint8_t flags[3];bool mutate{},reject{},replace_tree{},change_next{};SettingsLanguageScene24V1* scene;SettingsObjectNode32V1* end;SettingsCharacterNode16V1* sentinel;SettingsCharacterNode16V1* first;
 std::vector<std::pair<unsigned,unsigned>> calls;
 static int service(void* p,const SettingsSceneRequest16V1* r,unsigned* result){auto& x=*static_cast<Ctx*>(p);auto op=static_cast<unsigned>(r->operation);auto id=static_cast<unsigned>(r->identity);check(!r->reserved,"reserved request");x.calls.push_back({op,id});*result=op==1?x.types[id-1]==1:op==2?x.types[id-1]==7:op==4?1:0;if(op==5&&x.mutate)x.object_types[id-11]=14;if(op==3){if(x.reject)return 1;if(x.replace_tree)x.scene->object_first=x.end;if(x.change_next)x.first->next=x.sentinel;}return 0;}
};
}
int main(int argc,char** argv){try{
 check(argc==2,"usage language-scene-fixtures.bin");std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> b(std::istreambuf_iterator<char>(f),{});check(b.size()>=8&&!std::memcmp(b.data(),"LSV1",4),"gold header");std::size_t at=4;auto word=[&](){check(b.size()-at>=4,"gold word");unsigned v;std::memcpy(&v,b.data()+at,4);at+=4;return v;};auto count=word();unsigned additional=0,guards=0;
 for(unsigned n=0;n<count;++n){Ctx x{};for(auto& t:x.types)t=word();for(auto& t:x.object_types)t=word();x.mutate=word();unsigned expected[3];for(auto& v:expected)v=word();auto length=word();std::vector<std::pair<unsigned,unsigned>> wanted;for(unsigned i=0;i<length;++i){auto op=word(),id=word();wanted.push_back({op,id});}
  SettingsCharacterNode16V1 sentinel{},chars[3];SettingsObjectNode32V1 end{},nodes[3];SettingsSceneObject24V1 objects[3];SettingsLanguageScene24V1 scene{&sentinel,&end,nodes};x.scene=&scene;x.end=&end;x.sentinel=&sentinel;x.first=chars;sentinel.next=chars;end.parent=nodes;end.left=nodes;end.right=nodes+2;
  for(unsigned i=0;i<3;++i){chars[i]={i<2?chars+i+1:&sentinel,i+1};x.flags[i]=1;objects[i]={i+11,&x.object_types[i],&x.flags[i]};nodes[i]={i?nodes+i-1:&end,nullptr,i<2?nodes+i+1:nullptr,objects+i};}
  SettingsSceneServices16V1 services{&x,Ctx::service};check(!dh2_settings_v1_refresh_language_scene(&scene,&services)&&x.calls==wanted,"ordered source graph gold");for(unsigned i=0;i<3;++i)check(x.flags[i]==expected[i],"source invalidation gold");
  if(!n){
   x.calls.clear();x.types[0]=1;x.reject=true;check(dh2_settings_v1_refresh_language_scene(&scene,&services)==-2&&x.calls==std::vector<std::pair<unsigned,unsigned>>{{1,1},{3,1}},"missing inventory must reject");++additional;x.reject=false;
   x.calls.clear();x.replace_tree=true;check(!dh2_settings_v1_refresh_language_scene(&scene,&services),"live object graph replacement");for(auto p:x.calls)check(p.first<4,"tree re-fetch after callbacks");++additional;x.replace_tree=false;scene.object_first=nodes;
   x.calls.clear();x.change_next=true;check(!dh2_settings_v1_refresh_language_scene(&scene,&services),"live next replacement");unsigned characters=0;for(auto p:x.calls)if(p.second<10){++characters;check(p.second==1,"next replacement retains later character");}check(characters==2,"source current character prefix");++additional;x.change_next=false;chars[0].next=chars+1;
   auto copy=scene;copy.characters=nullptr;check(dh2_settings_v1_refresh_language_scene(&copy,&services)==-1,"null sentinel");++guards;auto bad=services;bad.invoke=nullptr;check(dh2_settings_v1_refresh_language_scene(&scene,&bad)==-1,"missing callback");++guards;
   chars[0].next=chars;check(dh2_settings_v1_refresh_language_scene(&scene,&services)==-1,"source graph cycle bounded");++guards;
  }
 }
 check(at==b.size(),"gold trailing bytes");std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"required_failure_and_live_mutation_checks\":"<<additional<<",\"guards\":"<<guards<<",\"mismatches\":0}"<<std::endl;
}catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
