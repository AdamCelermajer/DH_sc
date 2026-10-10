#include "native_monster_endpoint.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;using namespace dh::foundation::enemy_ai;
void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
int main(){try{
 ActorState actor;actor.id=7;dh2::data::PropertyState properties{};dh2::data::PropertyView pv{};pv.resolved=properties.resolved.data();
 dh2::character::TargetOwner16 owner{7,0,0,0};dh2::character::TargetState48 target{};target.owner=&owner;
 dh2::character::State state{};int controller{},changed_controller{},path{},rng{};std::uint32_t clock{};std::uint8_t heading{};
 NativeMonsterReceiver receiver{{&actor,&pv,&target,&state,&controller,&path,&clock,&rng,std::make_shared<int>(0)},&heading};
 NativeMonsterServices s;std::vector<std::string> trace;std::string error;bool fail_move=false;
 s.receiver=[&](NativeMonsterReceiver& r,std::string&){trace.push_back("reload");r=receiver;return true;};
 s.set_target=[&](const NativeMonsterReceiver& r,ActorId id,std::uint32_t mode,std::string&){check(mode==0,"mode changed");trace.push_back("set");r.live.target->target=id;r.live.actor->target_id=id;return true;};
 s.controller_stop=[&](void* c,std::string&){check(c==&controller,"controller changed");trace.push_back("stop");return true;};
 s.target_position=[&](ActorId id,std::array<float,3>& xyz,std::string&){check(id==8,"target changed");trace.push_back("position");xyz={1,2,3};receiver.live.controller=&changed_controller;return true;};
 s.controller_move_point=[&](void* c,const std::array<float,3>& xyz,std::string&){check(c==&controller&&xyz[2]==3,"captured controller or position lost");trace.push_back("move_point");return !fail_move;};
 s.clear_all_aggro=[&](const NativeMonsterReceiver&,std::string&){trace.push_back("clear_aggro");return true;};
 s.inherited=[&](std::uint32_t address,const NativeMonsterReceiver&,std::string&){check(address==0x3dc284,"inherited vtable wrong");trace.push_back("default_melee");return true;};
 check(native_monster_target_event(9,8,s,error)&&heading==1&&trace==std::vector<std::string>{"reload","set","reload"},"enemy spotted ordering");
 trace.clear();check(native_monster_target_event(10,0,s,error)&&trace==std::vector<std::string>{"reload","stop","reload","set","reload","clear_aggro"},"died ordering");
 actor.target_id=target.target=8;trace.clear();check(native_monster_target_event(12,0,s,error)&&trace==std::vector<std::string>{"reload","position","move_point","reload","set","reload","clear_aggro"},"out sight ordering");
 receiver.live.controller=&controller;actor.target_id=target.target=8;trace.clear();fail_move=true;
 check(!native_monster_target_event(12,0,s,error)&&target.target==8&&trace==std::vector<std::string>{"reload","position","move_point"},"failed reached move prefix");
 trace.clear();check(native_monster_target_event(13,0,s,error)&&trace.empty(),"native empty body synthesized work");
 check(native_monster_target_event(17,0,s,error)&&trace==std::vector<std::string>{"reload","default_melee"},"inherited body substituted");
 std::cout<<"PASS native AISMonster ordered overrides, controller capture/reloads, failed reached prefix, empty body and genuine inherited endpoint\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
