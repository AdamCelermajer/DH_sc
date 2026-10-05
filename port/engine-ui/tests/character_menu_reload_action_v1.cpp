#include "../character_menu_reload_action_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <cstring>
using namespace dh2::ui;
namespace {
void check(bool yes,const char* why){if(!yes)throw std::runtime_error(why);}
CharacterMenuValueV1 numeric(double number){CharacterMenuValueV1 value;value.kind=2;value.number=number;return value;}
struct Input {std::ifstream file;explicit Input(const char* path):file(path,std::ios::binary){check(bool(file),"gold input");}
 template<class T>T read(){T value;file.read(reinterpret_cast<char*>(&value),sizeof(value));check(bool(file),"truncated gold");return value;}};
struct Host {
 std::vector<std::pair<std::uint32_t,std::int32_t>> events;
 bool present{};double value{};unsigned services{};int fail{-1};
 static int reload(void* p,const MenuReloadRequest32V1* request,MenuReloadResponse16V1* response){
  auto& host=*static_cast<Host*>(p);
  check(request->subject==(request->service==reload_spec_prompt_v1?0xabc000000009ULL:0xabc000000001ULL),"same selected actor/menu");
  if(!host.services)host.events.emplace_back(4,0);
  ++host.services;
  if(int(request->service)==host.fail)return -1;
  response->reserved=0;
  if(request->service==reload_saved_level_v1)response->value=0;
  if(request->service==reload_menu_fx_v1)response->identity=0xabc000000009ULL;
  if(request->service==reload_spec_prompt_v1){
   check(request->argument==0&&!std::strcmp(request->path,"_root.menu_CharacterMenu")&&!std::strcmp(request->callback,"IsSpecTime"),"actual specialization callback");
  }
  return 0;
 }
 CharacterMenuReloadActionGraphV1 graph(){
  CharacterMenuReloadActionGraphV1 result;result.owner=std::make_shared<int>(1);
  result.player=[this](auto index,bool remote,auto& actor,auto&){check(!remote,"source remote false");events.emplace_back(3,index);actor=present?0xabc000000001ULL:0;return true;};
  result.player_index=[this](double number,auto& index,auto&){
   check(std::isfinite(number)&&number>=INT32_MIN&&number<=INT32_MAX,"explicit finite EABI fixture domain");
   index=std::int32_t(number);events.emplace_back(2,index);return true;
  };
  result.reload={this,reload};return result;
 }
};
}
int main(int argc,char** argv){try{
 check(argc==2,"reload action gold path");Input input(argv[1]);check(input.read<std::uint32_t>()==0x31415243,"gold magic");
 auto count=input.read<std::uint32_t>();std::uint32_t boundaries=0;Host host;
 CharacterMenuReloadActionV1 owner(host.graph());
 for(unsigned j=0;j<count;++j){
  auto arity=input.read<std::uint32_t>(),kind=input.read<std::uint32_t>();host.value=input.read<double>();host.present=input.read<std::uint32_t>()!=0;
  auto n=input.read<std::uint32_t>();std::vector<std::pair<std::uint32_t,std::int32_t>> expected;
  for(unsigned k=0;k<n;++k){auto op=input.read<std::uint32_t>();auto index=input.read<std::int32_t>();expected.emplace_back(op,index);}boundaries+=n;
  host.events.clear();host.services=0;CharacterMenuCallV1 call;call.arguments.resize(arity);
  for(auto& argument:call.arguments){argument.kind=kind;argument.number=host.value;}
  call.number=[&](const auto& argument,double& number,auto&){check(&argument==&call.arguments.front(),"actual arg0 conversion");host.events.emplace_back(1,0);number=host.value;return true;};
  call.result=numeric(99.5);std::string error;
  check(owner.dispatch("NativeReloadSkills",call,error),error.c_str());
  // Numeric semantic projections already contain ToNumber's unchanged value;
  // actual bridge validates that conversion delivery separately.
  if(arity==1&&kind==2)expected.erase(expected.begin());
  check(host.events==expected,"original wrapper call order");check(call.result.kind==2&&call.result.number==99.5,"source result preserved");
  check(host.services==(host.present?9U:0U),"complete reload coordinator reached");
 }
 unsigned guards=0;std::string error;CharacterMenuCallV1 call;call.result=numeric(77);host.present=true;
 for(auto service:{reload_remove_buffs_v1,reload_saved_skills_v1,reload_spec_prompt_v1}){
  host.events.clear();host.services=0;host.fail=service;
  check(!owner.dispatch("NativeReloadSkills",call,error)&&call.result.number==77,"required failure prefix/result");++guards;
 }
 host.fail=-1;
 auto missing=host.graph();missing.player={};CharacterMenuReloadActionV1 missing_player(missing);check(!missing_player.dispatch("NativeReloadSkills",call,error),"missing player rejects");++guards;
 missing=host.graph();missing.player_index={};CharacterMenuReloadActionV1 missing_index(missing);call.arguments={numeric(0)};check(!missing_index.dispatch("NativeReloadSkills",call,error),"missing EABI rejects");++guards;
 missing=host.graph();CharacterMenuReloadActionV1 missing_AS(missing);call.arguments.front().kind=6;check(!missing_AS.dispatch("NativeReloadSkills",call,error),"missing AS conversion rejects");++guards;
 check(!owner.dispatch("Other",call,error),"unowned callback");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_complete_cases\":"<<count<<",\"ordered_original_boundaries\":"<<boundaries<<",\"required_guards\":"<<guards<<",\"full_reload_coordinator_composed\":true,\"AS_EABI_and_component_services_are_fixtures\":true,\"live_character_menu\":false}\n";
 return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
