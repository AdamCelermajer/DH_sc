#include "hud_startup_callbacks.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
using Event=std::array<std::uint32_t,7>;
void require(bool value){if(!value)throw std::runtime_error("HUD startup audit mismatch");}
std::uint32_t bits(float value){std::uint32_t out;std::memcpy(&out,&value,4);return out;}
std::uintptr_t identity(unsigned i){return i?UINT64_C(0xa153710000000000)+i*0x1000:0;}
unsigned index(std::uintptr_t value){for(unsigned i=0;i<8;++i)if(value==identity(i))return i;throw std::runtime_error("identity truncated");}
struct Context {
 std::array<std::uint32_t,14> input{};std::vector<Event> calls;
 unsigned answer{},fail_at{},bad_response{},nested_checks{};bool reentry{},entered{};
};
int service(void* opaque,HudStartupState48* state,const HudStartupRequest40* req,HudStartupResponse16* out){
 auto& c=*static_cast<Context*>(opaque);require(!req->reserved);
 const auto op=unsigned(req->operation);
 unsigned key=0;if(req->name){if(!std::strcmp(req->name,"VolumeMusic"))key=1;else if(!std::strcmp(req->name,"VolumeFX"))key=2;else require(false);}
 c.calls.push_back({op,index(req->subject),std::uint32_t(req->argument),key,bits(req->values[0]),bits(req->values[1]),bits(req->values[2])});
 if(c.fail_at==c.calls.size())return 1;
 if(c.bad_response==c.calls.size())out->reserved=1;
 if(op==1&&(c.input[9]&1))state->savegame=identity(5);
 if(op==3)out->value=std::int32_t(c.input[key==1?5:6]);
 if(op==5){out->value=std::int32_t(c.input[7]);if(c.input[9]&2)state->savegame=identity(6);}
 if(op==7){out->value=std::int32_t(c.input[8]);if(c.input[9]&1)state->result=identity(7);}
 if(op==8)c.answer=unsigned(req->argument);
 if(c.reentry&&!c.entered&&op==5){
  c.entered=true;auto original=state->sharp_devices;state->sharp_devices=1;
  HudStartupServices16 services{&c,service};require(dh2_hud_is_multiplayer_enabled(state,&services)==0);
  state->sharp_devices=original;++c.nested_checks;
 }
 return 0;
}
HudStartupState48 state(const Context& c){return {identity(1),identity(2),c.input[4]?identity(3):0,identity(4),c.input[1],c.input[2],c.input[3],0};}
std::uint32_t read(std::ifstream& in){std::uint32_t value;in.read(reinterpret_cast<char*>(&value),4);require(bool(in));return value;}
int main(int argc,char** argv){try{
 require(argc==2);std::ifstream in(argv[1],std::ios::binary);require(read(in)==0x31534348);const auto count=read(in);require(count==1946);
 unsigned calls=0,guards=0,failure_prefixes=0;
 for(unsigned i=0;i<count;++i){
  Context c;for(auto& n:c.input)n=read(in);const auto answer=read(in),events=read(in);
  std::vector<Event> expected(events);for(auto& event:expected)for(auto& word:event)word=read(in);
  auto s=state(c);HudStartupServices16 services{&c,service};int status;
  if(c.input[0]==3){HudDevicePipeline16 device{{c.input[10],c.input[11],c.input[12]},c.input[13]};status=dh2_hud_device_pipeline(&device);require(status==int(answer));}
  else {status=c.input[0]==1?dh2_hud_load_settings(&s,&services):dh2_hud_is_multiplayer_enabled(&s,&services);require(status==0&&c.answer==answer);}
  require(c.calls==expected);calls+=unsigned(c.calls.size());
 }
 require(in.peek()==EOF&&calls==8736);
 Context c;c.input[4]=1;c.input[7]=3;c.input[8]=1;HudStartupServices16 services{&c,service};
 for(unsigned fail=1;fail<=7;++fail)for(unsigned malformed=0;malformed<2;++malformed){
  c.calls.clear();c.fail_at=malformed?0:fail;c.bad_response=malformed?fail:0;auto s=state(c);
  require(dh2_hud_load_settings(&s,&services)==-2&&c.calls.size()==fail);++failure_prefixes;
 }
 c.fail_at=c.bad_response=0;
 for(unsigned guard=0;guard<7;++guard){
  auto s=state(c);auto svc=services;c.calls.clear();
  if(guard==0)s.reserved=1;if(guard==1)s.sharp_devices=256;if(guard==2)s.htc_devices=256;if(guard==3)s.no_igp=256;
  if(guard==4)s.application=0;if(guard==5)s.savegame=0;if(guard==6)svc.invoke=nullptr;
  require(dh2_hud_load_settings(&s,&svc)==-1&&c.calls.empty());++guards;
 }
 c.calls.clear();auto s=state(c);s.result=0;require(dh2_hud_is_multiplayer_enabled(&s,&services)==-1&&c.calls.empty());++guards;
 c.input[8]=2;s=state(c);require(dh2_hud_is_multiplayer_enabled(&s,&services)==-2&&c.calls.size()==1);++guards;
 require(dh2_hud_load_settings(nullptr,&services)==-1&&dh2_hud_device_pipeline(nullptr)==-1);guards+=2;
 HudDevicePipeline16 invalid{{0,256,0},0x78};require(dh2_hud_device_pipeline(&invalid)==-1);++guards;
 c.input[8]=1;c.input[9]=3;c.reentry=true;c.calls.clear();s=state(c);
 require(dh2_hud_load_settings(&s,&services)==0&&c.nested_checks==1&&s.savegame==identity(6));
 require(c.calls.size()==8&&c.calls[6][0]==8&&c.calls[6][1]==4&&c.calls[6][2]==1&&c.calls.back()[1]==6);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_services\":"<<calls<<",\"guards\":"<<guards<<",\"failure_prefixes\":"<<failure_prefixes<<",\"host_service_reentry_checks\":"<<c.nested_checks<<",\"mismatches\":0}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
