#include "live_enemy_binding.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;
void check(bool v,const char* e){if(!v)throw std::runtime_error(e);}
int main(){try{
 ActorState actor;actor.id=7;
 dh2::data::PropertyState properties{};dh2::data::PropertyView view{};view.resolved=properties.resolved.data();
 dh2::character::TargetOwner16 owner{7,0,0,0};dh2::character::TargetState48 target{};target.owner=&owner;
 dh2::character::State state{};std::uint32_t clock=177;int controller{},path{},rng{};
 LiveEnemyBorrow b{&actor,&view,&target,&state,&controller,&path,&clock,&rng,std::make_shared<int>(0)};
 unsigned calls=0,borrows=0;std::string error;bool fail=false,wrong=false,unsynchronized=false;
 LiveEnemyProviders providers;
 providers.borrow=[&](ActorId id,LiveEnemyBorrow& out,std::string&){check(id==7,"identity changed");++borrows;out=b;return true;};
 providers.selected=[&](const LiveEnemyBorrow& loan,SelectedMonsterCallback& out,std::string&){
  check(loan.actor==&actor&&loan.state==&state&&loan.properties==&view&&loan.scene_clock==&clock&&loan.random_channel0==&rng,"copied source owners");
  out={7,44,wrong?dh2::character::script_default:dh2::character::script_monster,true,
   [&](std::uint32_t event,ActorId payload,std::string& e){++calls;check(event==9&&payload==8,"event changed");target.target=8;if(!unsynchronized)actor.target_id=8;if(fail){e="required native DoSkill owner";return false;}return true;}};
  return true;
 };
 check(dispatch_live_monster_target(providers,7,9,8,error)&&calls==1,"selected callback not delivered");
 wrong=true;check(!dispatch_live_monster_target(providers,7,9,8,error)&&calls==1,"default actor enabled");wrong=false;
 const auto before=borrows;check(!dispatch_live_monster_target(providers,7,11,8,error)&&borrows==before,"unregistered event admitted");
 b.random_channel0=nullptr;check(!dispatch_live_monster_target(providers,7,9,8,error)&&calls==1,"missing original RNG accepted");b.random_channel0=&rng;
 actor.target_id=target.target=0;fail=true;
 check(!dispatch_live_monster_target(providers,7,9,8,error)&&target.target==8&&actor.target_id==8&&error=="required native DoSkill owner","failed source prefix rolled back");fail=false;
 actor.target_id=target.target=0;unsynchronized=true;
 check(!dispatch_live_monster_target(providers,7,9,8,error)&&target.target==8,"private target accepted");
 std::cout<<"PASS same retained owner, selected AIS, unregistered event, missing RNG, failure prefix and canonical target publication\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
