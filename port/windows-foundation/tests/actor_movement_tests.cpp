#include "actor_movement.hpp"
#include "collision_scene.hpp"
#include "gameplay_camera.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <array>
#include <cstring>
using namespace dh::foundation;
namespace {
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
bool near(float a,float b){return std::abs(a-b)<0.0002f;}
// Synthetic test dimensions/speeds; these are not claims about original tuning.
ActorMovementConfig config(){ActorMovementConfig c;c.walkSpeed=4;c.runSpeed=8;c.turnSpeedRadians=1;
 c.bodyRadius=.2f;c.bodyHeight=2;c.maxStepUp=.5f;c.maxStepDown=.5f;c.maxSlopeDegrees=30;return c;}
void floorRect(CollisionScene& s,float x0,float x1,float y0,float y1,float z0=0,float z1=0){
 const Vec3 a{x0,y0,z0},b{x1,y0,z1},c{x1,y1,z1},d{x0,y1,z0};
 s.triangles.push_back({a,b,c,true});s.triangles.push_back({a,c,d,true});
}
CollisionScene flat(){CollisionScene s;floorRect(s,-100,100,-100,100);return s;}
InputActions move(float x,float y,bool run=false){InputActions i;i.move2D={x,y};i.run=run;return i;}
void run(){
 const CameraMovementBasis basis;const auto plane=flat();
 ActorMovement a(config()),b(config());a.setPosition({0,0,0});b.setPosition({0,0,0});
 check(a.step(move(1,0),basis,plane,1),"flat actor moves");
 check(near(a.state().position.x,4)&&near(a.state().position.z,0),"walk speed in units per second");
 check(a.state().grounded&&a.state().action==ActorAction::Walk&&a.state().animationName=="walk","walking state follows actual movement");
 for(int i=0;i<10;++i)b.step(move(1,0),basis,plane,.1);
 check(near(a.state().position.x,b.state().position.x),"flat displacement dt partition invariant");
 a.setPosition({0,0,0});a.step(move(1,1),basis,plane,1);
 check(near(std::hypot(a.state().position.x,a.state().position.y),4),"diagonal input cannot increase speed");
 a.setPosition({0,0,0});a.step(move(.5f,0),basis,plane,1);
 check(near(a.state().position.x,2),"analog input preserves partial speed");
 a.setPosition({0,0,0});a.step(move(0,1,true),basis,plane,.5);
 check(near(a.state().position.y,4)&&a.state().action==ActorAction::Run,"run speed and state");
 check(near(a.state().facingRadians,.5f),"turn speed caps facing change");
 a.step(move(0,1),basis,plane,2);check(near(a.state().facingRadians,1.5707963f),"facing reaches requested heading");
 InputActions attack=move(1,0);attack.attack=true;const auto before=a.state().position;
 check(!a.step(attack,basis,plane,.1)&&near(a.state().position.x,before.x)&&near(a.state().position.y,before.y),"attack request prevents locomotion");
 check(a.state().action==ActorAction::Attack,"attack action selected");
 a.step({},basis,plane,.1);check(a.state().action==ActorAction::Idle,"released intent returns idle");
 for(double dt : {-1.0,0.0,std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()})
  check(!a.step(move(1,0),basis,plane,dt),"invalid delta cannot move actor");
 CollisionScene ramp;floorRect(ramp,-10,10,-10,10,-2.5f,2.5f);
 a.setPosition({0,0,0});check(a.step(move(1,0),basis,ramp,1),"walkable ramp accepted");
 check(near(a.state().position.x,4)&&near(a.state().position.z,1),"feet follow ramp floor height");
 CollisionScene steep;floorRect(steep,-10,10,-10,10,-10,10);
 a.setPosition({0,0,0});check(!a.step(move(1,0),basis,steep,1),"excess slope rejected");
 CollisionScene lowStep;floorRect(lowStep,-10,0,-10,10);floorRect(lowStep,0,10,-10,10,.4f,.4f);
 a.setPosition({-2,0,0});check(a.step(move(1,0),basis,lowStep,1),"allowed step climb");
 check(a.state().position.x>1.9f&&near(a.state().position.z,.4f),"step reaches upper floor");
 a.setPosition({2,0,.4f});a.step(move(-1,0),basis,lowStep,1);
 check(a.state().position.x<-1.9f&&near(a.state().position.z,0),"allowed step descent");
 CollisionScene highStep;floorRect(highStep,-10,0,-10,10);floorRect(highStep,0,10,-10,10,1,1);
 a.setPosition({-2,0,0});a.step(move(1,0),basis,highStep,1);
 check(a.state().position.x<=.0001f&&near(a.state().position.z,0),"excess step blocked when center lacks admissible height");
 a.setPosition({2,0,1});a.step(move(-1,0),basis,highStep,1);
 check(a.state().position.x>=-.0001f&&near(a.state().position.z,1),"excess drop blocked at supported center");
 CollisionScene edge;floorRect(edge,-10,0,-10,10);a.setPosition({-2,0,0});a.step(move(1,0),basis,edge,1);
 check(a.state().position.x<=.0001f,"center cannot walk beyond floor edge");
 a.setPosition({-.05f,0,0});check(a.step(move(0,1),basis,edge,.25),"supported center near edge can move parallel despite body radius");
 check(near(a.state().position.x,-.05f)&&near(a.state().position.y,1),"body radius does not reject valid floor center");
 CollisionScene wall=plane;wall.triangles.push_back({{0,-20,0},{0,20,0},{0,20,10},false});
 wall.triangles.push_back({{0,-20,0},{0,20,10},{0,-20,10},false});
 a.setPosition({-2,0,0});a.step(move(1,1),basis,wall,1);
 check(a.state().position.x<=-.19f&&a.state().position.y>2,"body sweeps prevent wall tunneling and allow sliding");
 // A second differently configured actor shares the same world/basis without class branches.
 auto slower=config();slower.walkSpeed=2;ActorMovement npc(slower);npc.setPosition({0,0,0});npc.step(move(0,1),basis,plane,1);
 check(near(npc.state().position.y,2),"actor-specific data drives shared movement");
}
void rootTests(){
 const CameraMovementBasis basis;const auto plane=flat();
 auto c=config();c.walkSpeed=0;c.runSpeed=0;c.forwardAxis={0,-1,0};c.turnSpeedRadians=10;
 ActorMovement full(c),split(c);full.setPosition({0,0,0});split.setPosition({0,0,0});
 check(full.step_root_motion(move(1,0),basis,plane,{0,-4,99},1),"root motion needs no constant speed");
 check(near(full.state().position.x,4)&&near(full.state().position.y,0)&&near(full.state().position.z,0),"authored -Y forward rotates to +X; grounded Z ignores root Z");
 check(near(full.state().facingRadians,1.5707963f),"authored forward axis determines facing rotation");
 for(int i=0;i<4;++i)split.step_root_motion(move(1,0),basis,plane,{0,-1,0},.25);
 check(near(split.state().position.x,full.state().position.x)&&near(split.state().position.y,full.state().position.y),"split root deltas preserve straight path");
 full.setPosition({0,0,0});full.steer(move(1,0),basis,1);const float facing=full.state().facingRadians;
 full.apply_root_motion({0,-1,0},plane);full.apply_root_motion({0,-1,0},plane);
 check(near(full.state().facingRadians,facing)&&near(full.state().position.x,2),"applying roots does not steer twice");
 full.setPosition({0,0,0});full.steer({},basis,1);
 check(!full.apply_root_motion({0,-4,0},plane)&&near(full.state().position.y,0),"idle root drift discarded");
 InputActions attack;attack.attack=true;full.steer(attack,basis,1);
 check(full.apply_root_motion({0,-1,0},plane)&&near(full.state().position.y,-1),"stationary attack accepts authored lunge");
 full.setPosition({0,0,0});full.step_root_motion(move(.5f,0,true),basis,plane,{0,-4,0},1);
 check(near(full.state().position.x,2)&&full.state().action==ActorAction::Run,"root run clip independent speed with analog scale");
 const auto prior=full.state();
 check(!full.apply_root_motion({std::numeric_limits<float>::quiet_NaN(),0,0},plane),"nonfinite root rejected");
 check(near(full.state().position.x,prior.position.x)&&near(full.state().facingRadians,prior.facingRadians)&&full.state().action==prior.action,"invalid apply is atomic");
 check(!full.step_root_motion(move(0,-1),basis,plane,{0,std::numeric_limits<float>::infinity(),0},1),"nonfinite combined root rejected");
 check(near(full.state().position.x,prior.position.x)&&near(full.state().facingRadians,prior.facingRadians)&&full.state().action==prior.action,"invalid combined root is atomic");
 CollisionScene edge;floorRect(edge,-10,0,-10,10);full.setPosition({-2,0,0});
 full.step_root_motion(move(1,0),basis,edge,{0,-4,0},1);check(full.state().position.x<=.0001f,"root displacement respects center floor edge");
 CollisionScene wall=plane;wall.triangles.push_back({{0,-20,0},{0,20,0},{0,20,10},false});wall.triangles.push_back({{0,-20,0},{0,20,10},{0,-20,10},false});
 full.setPosition({-2,0,0});full.step_root_motion(move(1,0),basis,wall,{0,-4,0},1);
 check(full.state().position.x<=-.19f,"root displacement cannot tunnel through wall");
 CollisionScene ramp;floorRect(ramp,-10,10,-10,10,-2.5f,2.5f);full.setPosition({0,0,0});
 full.step_root_motion(move(1,0),basis,ramp,{0,-4,0},1);check(near(full.state().position.z,1),"root displacement follows ramp height");
}
void headingTests(){
 const CameraMovementBasis basis;ActorMovement actor(config());actor.setPosition({0,0,0});
 check(actor.steer(move(0,1),basis,.1),"valid steering publishes source heading");
 check(actor.state().desiredHeading.active&&near(actor.state().desiredHeading.direction.x,0)&&near(actor.state().desiredHeading.direction.y,1),"desired heading records requested direction");
 check(near(actor.state().facingRadians,.1f)&&near(actor.state().desiredHeading.sourceAngleRadians,-3.14159265f),"source minus-Y desired angle differs from bounded visual plus-X facing");
 actor.setFacingRadians(-.5f);
 check(near(actor.state().desiredHeading.direction.y,1)&&near(actor.state().desiredHeading.sourceAngleRadians,-3.14159265f),"visual facing setter does not replace desired command heading");
 const float facing=actor.state().facingRadians;
 check(actor.setDesiredHeading({.5f,0,99})&&actor.state().desiredHeading.active&&near(actor.state().desiredHeading.direction.x,.5f)&&near(actor.state().desiredHeading.direction.z,0),"source retains analog magnitude and drops Z");
 check(near(actor.state().facingRadians,facing),"source heading setter does not turn visual");
 check(actor.setDesiredHeading({3,4,0})&&near(actor.state().desiredHeading.direction.x,.6f)&&near(actor.state().desiredHeading.direction.y,.8f),"source normalizes only above unit magnitude");
 const float priorAngle=actor.state().desiredHeading.sourceAngleRadians;
 check(actor.setDesiredHeading({0,-1,0},false)&&near(actor.state().desiredHeading.sourceAngleRadians,priorAngle),"rotate false preserves source angle");
 check(actor.setDesiredHeading({.005f,0,1})&&!actor.state().desiredHeading.active&&near(actor.state().desiredHeading.direction.x,.005f),"original heading active threshold differs from nonzero movement input");
 check(actor.setDesiredHeading({.02f,0,0})&&actor.state().desiredHeading.active,"input above original threshold activates heading");
 InputActions attack;attack.attack=true;check(actor.steer(attack,basis,.1)&&near(actor.state().desiredHeading.direction.x,.02f),"stationary attack preserves explicit heading until caller changes it");
 check(actor.steer({},basis,.1)&&!actor.state().desiredHeading.active&&near(actor.state().desiredHeading.direction.x,0),"released locomotion explicitly clears desired heading");
 const auto angleAfterStop=actor.state().desiredHeading.sourceAngleRadians;
 actor.stopDesiredHeading();check(near(actor.state().desiredHeading.sourceAngleRadians,angleAfterStop),"stop retains source heading angle");
 const auto previous=actor.state();
 check(!actor.setDesiredHeading({std::numeric_limits<float>::infinity(),0,0}),"nonfinite heading rejected");
 check(!actor.setDesiredHeading({std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),0}),"overflowing squared heading rejected");
 check(actor.state().desiredHeading.active==previous.desiredHeading.active&&near(actor.state().desiredHeading.sourceAngleRadians,previous.desiredHeading.sourceAngleRadians)&&near(actor.state().facingRadians,previous.facingRadians),"invalid heading setter preserves state");
 CollisionScene empty;actor.setPosition({0,0,0});
 check(!actor.step(move(1,0),basis,empty,.1)&&actor.state().desiredHeading.active&&near(actor.state().desiredHeading.direction.x,1),"unsupported or blocked movement still records desired heading");
 actor.setPosition({0,0,0});check(!actor.state().desiredHeading.active&&near(actor.state().desiredHeading.direction.x,0),"new placement resets heading seam");
}
void sourceAdmissionTests(){
 const CameraMovementBasis basis;auto c=config();c.forwardAxis={0,-1,0};c.turnSpeedRadians=10;
 ActorMovement player(c);player.setPosition({10,20,30});check(player.steer(move(.5f,0),basis,1),"source admission initial steer");const float facing=player.state().facingRadians;
 unsigned calls=0;ActorMovement::SourceMotionAdmission clamped=[&](Vec3 from,Vec3 world,ActorMovement::SourceMotionAdmissionResult& result,std::string&){
  ++calls;check(near(from.x,10)&&near(from.y,20)&&near(from.z,30),"source solver received different current position");check(near(world.x,4)&&near(world.y,0)&&near(world.z,0),"source solver lost shared facing/analog/vertical-root policy");
  result={{12.5f,20,400},true};return true;
 };
 bool moved=false;std::string error;check(player.apply_root_motion({0,-8,99},clamped,moved,error)&&moved&&calls==1,"authoritative clamped source solve failed");
 check(near(player.state().position.x,12.5f)&&near(player.state().position.z,400)&&player.state().grounded&&near(player.state().facingRadians,facing),"source result was replaced by preview step/height solve or turned again");
 const auto admitted=player.state();ActorMovement::SourceMotionAdmission blocked=[&](Vec3 from,Vec3,ActorMovement::SourceMotionAdmissionResult& result,std::string&){++calls;result={from,true};return true;};moved=true;
 check(player.apply_root_motion({0,-8,0},blocked,moved,error)&&!moved&&near(player.state().position.x,admitted.position.x),"blocked source result was mistaken for failure or axis-slid");
 auto failing=[](Vec3,Vec3,ActorMovement::SourceMotionAdmissionResult& result,std::string&e){result={{999,999,999},false};e="original floor unavailable";return false;};moved=true;
 check(!player.apply_root_motion({0,-8,0},failing,moved,error)&&!moved&&error=="original floor unavailable"&&near(player.state().position.z,400)&&player.state().grounded,"source failure was hidden or changed live state");
 check(!player.apply_root_motion({0,-8,0},ActorMovement::SourceMotionAdmission{},moved,error)&&!moved,"missing source callback used preview fallback");
 auto malformed=[](Vec3,Vec3,ActorMovement::SourceMotionAdmissionResult&r,std::string&){r={{std::numeric_limits<float>::infinity(),0,0},false};return true;};
 check(!player.apply_root_motion({0,-8,0},malformed,moved,error)&&near(player.state().position.x,12.5f)&&player.state().grounded,"malformed source output committed");
 auto throwing=[](Vec3,Vec3,ActorMovement::SourceMotionAdmissionResult&,std::string&)->bool{throw std::runtime_error("source exception");};check(!player.apply_root_motion({0,-8,0},throwing,moved,error)&&error=="source exception","source exception not propagated");
 const auto prior=player.state();check(!player.step_root_motion(move(-1,0),basis,failing,{0,-4,0},1,moved,error)&&near(player.state().facingRadians,prior.facingRadians)&&player.state().animationName==prior.animationName,"failed combined source solve changed steering state");
 ActorMovement npc(c);npc.setPosition({-2,3,4});InputActions attack;attack.attack=true;check(npc.steer(attack,basis,1),"NPC source attack steer");
 auto free=[](Vec3 p,Vec3 d,ActorMovement::SourceMotionAdmissionResult&r,std::string&){r={{p.x+d.x,p.y+d.y,p.z},false};return true;};check(npc.apply_root_motion({1,-2,999},free,moved,error)&&moved&&near(npc.state().position.x,-1)&&near(npc.state().position.y,1)&&!npc.state().grounded,"NPC attack source admission did not share transform/status policy");
 check(npc.steer({},basis,1),"NPC source idle steer");auto idle=[&](Vec3 p,Vec3 d,ActorMovement::SourceMotionAdmissionResult&r,std::string&){check(near(d.x,0)&&near(d.y,0),"idle passed root drift into source solver");r={{p.x,p.y,p.z+1},true};return true;};
 check(npc.apply_root_motion({100,100,100},idle,moved,error)&&moved&&npc.state().grounded&&near(npc.state().position.z,5),"zero-delta original grounding adjustment was dropped");
}
void sourceRotationTests(){
 const CameraMovementBasis basis;ActorMovement actor(config());actor.setPosition({0,0,0},0);
 check(actor.steer_source_intent(move(0,1),basis,.1),"source intent phase failed");check(near(actor.state().facingRadians,0)&&actor.state().desiredHeading.active&&near(actor.state().desiredHeading.direction.y,1),"source intent prematurely applied preview bounded turn");
 Vec3 sampledDelta{};auto admit=[&](Vec3 from,Vec3 delta,ActorMovement::SourceMotionAdmissionResult& out,std::string&){sampledDelta=delta;out={{from.x+delta.x,from.y+delta.y,from.z},true};return true;};bool moved=false;std::string error;
 check(actor.apply_root_motion({1,0,0},admit,moved,error)&&near(sampledDelta.x,1)&&near(sampledDelta.y,0),"early root motion did not use prior facing");const auto earlyPosition=actor.state().position;
 float euler[3]{.25f,-.5f,0};const float heading=actor.state().desiredHeading.sourceAngleRadians;std::uint32_t turn=1;ActorMovement::SourceRotationBorrow borrow{euler,&heading,&turn};std::array<std::int32_t,224> sheet{};std::uint32_t flags=0x20;unsigned calls=0;
 auto sync=[&](const float* same,std::string&){++calls;check(same==euler&&near(same[0],.25f)&&near(same[1],-.5f)&&same[2]==heading,"SyncRotation did not observe SAME final borrowed Euler XYZ");return true;};
 check(actor.apply_source_character_rotation_late(borrow,&flags,sheet.data(),UINT32_MAX,true,sync,error)&&calls==1&&turn==1&&euler[2]==heading,"negative source sentinel/unsigned dt did not snap while retaining turn flag");check(near(actor.state().position.x,earlyPosition.x)&&near(actor.state().position.y,earlyPosition.y),"late rotation retroactively transformed early displacement");
 check(actor.apply_root_motion({1,0,0},admit,moved,error)&&near(sampledDelta.x,-1)&&near(sampledDelta.y,0),"next root phase did not consume final source facing");
 // Genuine fixed-percent property47 and actual flags decide speed/sync. Compare
 // adapter's borrowed writes with the established original coordinator kernel.
 sheet[47]=6400;flags=0x10;float target=2;float actual[3]{.3f,.4f,.2f};std::uint32_t actualTurn=0;dh2::actor::RotationState expected{{actual[0],actual[1],actual[2]},target,actualTurn,0};std::uint32_t wantSync=999;
 dh2::actor::RotationPolicy policy{1.25f,31,1,0};check(dh2_actor_update_rotation(&expected,&policy,&wantSync)==0,"original reference rotation");
 check(actor.apply_source_character_rotation_late({actual,&target,&actualTurn},&flags,sheet.data(),31,true,{},error),"suppressed source sync required a callback");check(std::memcmp(actual,expected.rotation,12)==0&&actualTurn==expected.turn_positive&&calls==1,"source property47 or visual policy bytes differ");
 float unwrapped=8.25f;flags=0x20;check(actor.apply_source_character_rotation_late({actual,&unwrapped,&actualTurn},&flags,sheet.data(),0,false,{},error)&&actual[2]==unwrapped&&actor.state().facingRadians==unwrapped,"source Z was normalized or visual absence rejected");
 const auto previous=actor.state();const auto oldAngle=actual[2];flags=0;
 check(!actor.apply_source_character_rotation_late({actual,&target,&actualTurn},&flags,sheet.data(),20,true,{},error)&&actual[2]==oldAngle&&actor.state().facingRadians==previous.facingRadians,"missing required SyncRotation mutated source prefix");
 check(!actor.apply_source_character_rotation_late({actual,&target,&actualTurn},nullptr,sheet.data(),20,true,sync,error)&&actual[2]==oldAngle,"missing real flags were fabricated");
 std::uint32_t malformed=2;check(!actor.apply_source_rotation_late({actual,&target,&malformed},policy,{},error)&&actual[2]==oldAngle,"malformed actual turn flag mutated borrowed state");
 float failingHeading=-.75f;flags=0x20;auto failedSync=[](const float*,std::string&e){e="source SyncRotation unavailable";return false;};check(!actor.apply_source_character_rotation_late({actual,&failingHeading,&actualTurn},&flags,sheet.data(),16,true,failedSync,error)&&actual[2]==failingHeading&&actor.state().facingRadians==failingHeading&&error=="source SyncRotation unavailable","failed source tail discarded completed actual Euler prefix");
}
void sourcePositionSyncTests(){
 const CameraMovementBasis basis;ActorMovement actor(config());actor.setPosition({0,0,0});
 check(actor.steer_source_intent(move(.5f,0),basis,.016),"source sync initial analog steer");
 const auto intent=actor.state();
 actor.sync_source_position({10,20,30},.25f);
 check(actor.state().action==intent.action&&actor.state().animationName==intent.animationName&&
       actor.state().desiredHeading.active==intent.desiredHeading.active&&
       actor.state().desiredHeading.direction.x==intent.desiredHeading.direction.x&&
       actor.state().desiredHeading.sourceAngleRadians==intent.desiredHeading.sourceAngleRadians,
       "source position import erased admitted locomotion intent");
 Vec3 delta{};ActorMovement::SourceMotionAdmission admit=[&](Vec3 from,Vec3 sampled,auto& out,std::string&){
  delta=sampled;out={{from.x+sampled.x,from.y+sampled.y,from.z},true};return true;
 };bool moved=false;std::string error;
 check(actor.apply_root_motion({2,0,0},admit,moved,error)&&moved&&near(std::hypot(delta.x,delta.y),1),
       "source position import zeroed half-analog authored root motion");
 actor.sync_source_position(actor.state().position,8.25f);
 check(actor.state().facingRadians==8.25f&&actor.state().desiredHeading.active,
       "source position publication changed exact yaw or cleared heading");
 const auto beforeInvalid=actor.state();bool rejected=false;
 try{actor.sync_source_position({std::numeric_limits<float>::infinity(),0,0},0);}catch(const std::invalid_argument&){rejected=true;}
 check(rejected&&actor.state().position.x==beforeInvalid.position.x&&actor.state().facingRadians==beforeInvalid.facingRadians,
       "invalid source position sync changed admitted prefix");
 actor.setPosition({10,20,30});
 check(actor.state().action==ActorAction::Idle&&!actor.state().desiredHeading.active,
       "startup/restore placement stopped clearing old intent");
 check(actor.apply_root_motion({2,0,0},admit,moved,error)&&!moved&&near(delta.x,0)&&near(delta.y,0),
       "startup/restore placement retained stale analog movement");
}
}
int main(){try{run();rootTests();headingTests();sourceAdmissionTests();sourceRotationTests();sourcePositionSyncTests();std::cout<<"actor movement tests passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
