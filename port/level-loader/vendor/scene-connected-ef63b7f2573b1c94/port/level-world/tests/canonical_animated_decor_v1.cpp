#include "../canonical_animated_decor_v1.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
struct Fixture {
 std::shared_ptr<int> pin=std::make_shared<int>(1);dh2::actor::RuntimeState runtime{};
 std::unique_ptr<CanonicalAnimatedDecorV1> actor;
 CanonicalPropertyMapV1 map{{nullptr,&canonical_vec3_origin_v1(),nullptr}};
 std::vector<std::string> calls;std::function<bool(std::string&)> completion;
 bool physical{},found{true},accepted{true},high{true};int roll{};std::uintptr_t bodies{};std::string error;
 Fixture(){
  GameObjectInitializationServicesV1 init;init.owner=pin;
  init.condition_init=[this](std::uint32_t o,std::string&){calls.push_back(o==0x8c?"condition8c":"conditionb0");return true;};
  init.check_spawn_probability=[this](std::int32_t& value,std::string&){value=roll;calls.push_back("spawn");return true;};
  init.set_position=[this](const float* p,bool update,std::string&){assert(p==runtime.subobjects.position&&update);calls.push_back("position");return true;};
  init.device_high_performance=[this](bool& value,std::string&){value=high;calls.push_back("device");return true;};
  init.load_visual=[this](std::string&){*actor->base().pointer(0x2d8)=901;calls.push_back("load");return true;};
  init.visual_sync=[this](std::uintptr_t p,std::string&){assert(p==901);calls.push_back("base_sync");return true;};
  AnimatedDecorServicesV1 s;s.owner=pin;
  s.visual_sync=[this](std::uintptr_t p,std::string&){assert(p==901);calls.push_back("sync");return true;};
  s.visual_physical28=[this](std::uintptr_t p,bool& out,std::string&){assert(p==901);out=physical;calls.push_back("physical28");return true;};
  s.construct_podecor=[this](CanonicalGameObjectBaseOwnerV1& base,std::uintptr_t& body,std::string&){assert(&base==&actor->base());body=1000+(++bodies);calls.push_back("body");return true;};
  s.set_physical=[this](std::uintptr_t body,bool preserve,std::string&){assert(!preserve);*actor->base().pointer(0x2dc)=body;calls.push_back("setbody");return true;};
  s.animation_count=[this](std::uintptr_t p,std::int32_t& count,std::string&){assert(p==901);count=4;calls.push_back("count");return true;};
  s.animation_exists=[this](std::uintptr_t p,const std::string& name,bool& out,std::string&){assert(p==901);out=found;calls.push_back("exists:"+name);return true;};
  s.play_index=[this](std::uintptr_t p,std::int32_t index,bool loop,bool& out,std::string&){assert(p==901);out=accepted;calls.push_back("index:"+std::to_string(index)+":"+std::to_string(loop));return true;};
  s.play_name=[this](std::uintptr_t p,const std::string& name,bool loop,bool& out,std::string&){assert(p==901&&loop);out=accepted;calls.push_back("name:"+name);return true;};
  s.random=[this](std::int32_t maximum,std::int32_t& index,std::string&){assert(maximum==3);index=2;calls.push_back("random");return true;};
  s.install_random_completion=[this](std::uintptr_t p,std::function<bool(std::string&)> cb,std::string&){assert(p==901);completion=std::move(cb);calls.push_back("callback");return true;};
  s.random_play_assertion=[this](std::string&){calls.push_back("assertion");return true;};
  s.update=[this](std::string&){calls.push_back("update");return true;};
  s.destroy_base=[this](std::string&){completion={};calls.push_back("destroy");return true;};
  actor=std::make_unique<CanonicalAnimatedDecorV1>(pin,runtime,init,s);
  actor->base().class_name20()="AnimatedDecor";auto property=actor->properties();assert(map.init_properties(property,error)&&map.load_defaults(property,error));
 }
 void animation(const char* name){auto p=actor->properties();assert(map.set_property(p,"startanim",name,error));}
 bool called(const char* name){for(auto& c:calls)if(c==name)return true;return false;}
};
int main(){
 {Fixture f;assert(f.actor->base().type_f4()==20&&*f.actor->base().byte(0x84)==1&&f.actor->solid()==1&&f.actor->load_floor()==0);assert(f.actor->init_post(f.error));assert(f.actor->start_animation()=="idle"&&f.called("name:idle")&&!f.completion&&f.called("update"));assert(f.actor->destroy(f.error)&&f.actor->start_animation().empty());}
 {Fixture f;f.physical=true;f.animation("RANDOMALL");assert(f.actor->init_post(f.error)&&f.bodies==2&&f.called("index:2:0")&&f.completion);f.calls.clear();f.accepted=false;assert(f.completion(f.error));assert(f.calls==std::vector<std::string>({"count","random","index:2:0","assertion"}));assert(f.actor->destroy(f.error)&&!f.completion);}
 {Fixture f;f.animation("missing");f.found=false;assert(f.actor->init_post(f.error)&&f.called("index:0:1")&&!f.completion);}
 {Fixture f;f.animation("activate");f.accepted=false;assert(f.actor->init_post(f.error)&&f.called("name:activate")&&f.called("index:0:1"));}
 {Fixture f;f.high=false;assert(f.actor->init_post(f.error)&&!f.called("load")&&!f.called("update")&&f.actor->start_animation().empty());}
 {Fixture f;f.roll=100;assert(f.actor->init_post(f.error)&&!f.called("position")&&!f.called("update"));}
 {auto pin=std::make_shared<int>(1);dh2::actor::RuntimeState runtime{};GameObjectInitializationServicesV1 init;CanonicalAnimatedDecorV1 a(pin,runtime,init,{});std::string e;assert(!a.init_post(e)&&*a.base().byte(0x10c)==1);assert(!a.destroy(e)&&e.find("GameObject destructor")!=std::string::npos);}
 std::cout<<"AnimatedDecor PASS actual base/property storage + explicit visual/timeline/physical fixtures; no production scene claim\n";
}
