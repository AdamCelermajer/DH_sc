#include "actor_movement.hpp"
#include "asset_catalog.hpp"
#include "collision_scene.hpp"
#include "gameplay_camera.hpp"
#include "original_character.hpp"
#include <cmath>
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
bool near(float a,float b,float tolerance=.05f){return std::abs(a-b)<tolerance;}
CollisionScene plane(){CollisionScene s;s.triangles={{{-100000,-100000,0},{100000,-100000,0},{100000,100000,0},true},
 {{-100000,-100000,0},{100000,100000,0},{-100000,100000,0},true}};return s;}
Vec3 scaled(Vec3 p,Vec3 scale){return {p.x*scale.x,p.y*scale.y,p.z*scale.z};}
}
int main(int argc,char** argv){try{
 check(argc==2,"expected repository root or original asset root");
 std::filesystem::path root=argv[1];const auto repoAssets=root/"port/android-native/app/src/main/assets";
 if(std::filesystem::is_directory(repoAssets))root=repoAssets;
 AssetCatalog assets(root);CharacterVisual visual;CharacterVisualConfig config;std::string error;
 config.model_path="models/prince_modular.bdae";config.template_clip_path="animations/prince_template_anim.bdae";
 config.clips={{"walk","animations/prince_walk_1hand.bdae"},{"idle","animations/prince_idle_shield.bdae"}};
 config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;
 config.motion_node_id="auto";config.consume_root_motion=true;
 check(visual.load(assets,config,error),error);check(visual.select("walk",true,error),error);
 check(std::string(visual.source_motion_root_name(false))=="root_camera","original auto root must prefer root_camera over Bip01");
 check(std::string(visual.root_motion_node_id()).find("root_camera")!=std::string::npos,"root extraction must use actual source auto choice");
 std::int32_t start=0,end=0;check(visual.animation_range("walk",start,end,error),error);
 check(end-start==800,"genuine Prince walk source duration changed");
 check(visual.update(.8,error),error);const auto rawCycle=visual.take_root_motion();
 check(std::hypot(rawCycle.x,rawCycle.y)>1,"genuine walk clip must contain displacement");
 const auto duplicate=visual.take_root_motion();check(duplicate.x==0&&duplicate.y==0&&duplicate.z==0,"root delta cannot be consumed twice");
 // Source property columns 12/13/14 at 100 percent convert to actor XY .009*100,
 // Z .01*100. This is the source transform scale, not a fabricated walk speed.
 const Vec3 sourceScale{.009f*100,.009f*100,.01f*100};
 ActorMovementConfig movement;movement.bodyRadius=10;movement.bodyHeight=100;movement.maxStepUp=10;
 movement.maxStepDown=10;movement.maxSlopeDegrees=30;movement.turnSpeedRadians=10;movement.forwardAxis={0,-1,0};
 ActorMovement actor(movement);actor.setPosition({0,0,0});const auto collision=plane();CameraMovementBasis basis;
 ActorMovement stationary(movement);stationary.setPosition({0,0,5});
 check(stationary.steer({},basis,.016),"stationary steering rejected");
 check(!stationary.apply_root_motion({100,100,0},collision),"idle drift became horizontal movement");
 check(stationary.state().grounded&&near(stationary.state().position.z,0)&&stationary.state().position.x==0&&stationary.state().position.y==0,"idle did not synchronize center floor");
 InputActions input;input.move2D={1,0};
 check(visual.select("idle",true,error),error);check(visual.select("walk",true,error),error);
 for(int i=0;i<2;++i){check(actor.steer(input,basis,.4),"steering rejected");check(visual.update(.4,error),error);
  check(actor.apply_root_motion(scaled(visual.take_root_motion(),sourceScale),collision),"genuine walk root rejected");
  const auto once=visual.take_root_motion();check(once.x==0&&once.y==0&&once.z==0,"split update duplicate consume");}
 const auto feet=actor.state().position;
 const float expectedDistance=std::hypot(rawCycle.x*sourceScale.x,rawCycle.y*sourceScale.y);
 check(near(std::hypot(feet.x,feet.y),expectedDistance),"400+400 world displacement differs from source cycle times actor scale");
 check(feet.x>0&&near(feet.y,0)&&near(feet.z,0),"source -Y root turns into requested camera +X ground movement");
 check(visual.select("idle",true,error),error);check(actor.steer({},basis,.4),"idle steering rejected");
 Vec3 idleMin{},idleMax{};check(visual.indexed_bounds(idleMin,idleMax,error),error);
 check(visual.update(.4,error),error);actor.apply_root_motion(scaled(visual.take_root_motion(),sourceScale),collision);
 check(near(actor.state().position.x,feet.x)&&near(actor.state().position.y,feet.y),"idle clip cannot move feet");
 CharacterVisual raw;config.consume_root_motion=false;check(raw.load(assets,config,error),error);check(raw.select("idle",true,error),error);
 check(raw.update(.4,error),error);Vec3 rawMin{},rawMax{},consumedMin{},consumedMax{};
 check(raw.indexed_bounds(rawMin,rawMax,error),error);check(visual.indexed_bounds(consumedMin,consumedMax,error),error);
 check(near(rawMin.z,consumedMin.z,.001f)&&near(rawMax.z,consumedMax.z,.001f),"source auto consumption cannot drop idle skeleton by pelvis height");
 // The optional staged full level is a genuine asset regression separate from
 // the synthetic plane. Spawn is Swamp entrypointID=0 from its authored MGP.
 const auto swampAssets=std::filesystem::path(argv[1])/".local-inputs/windows-foundation-assets";
 if(std::filesystem::is_directory(swampAssets)){
  AssetCatalog swamp(swampAssets);CollisionScene realFloor;
  check(load_collision_level(swamp,"data/scene/001_swamp.mlx",realFloor,error),error);
  const Vec3 spawn{1090.75f,-212.202f,258};FloorHit hit;
  check(realFloor.floor(spawn.x,spawn.y,spawn.z,hit,10,10),"authored Swamp entry spawn center must have floor");
  auto actualMovement=movement;
  actualMovement.bodyRadius=.5f*std::max((consumedMax.x-consumedMin.x)*sourceScale.x,(consumedMax.y-consumedMin.y)*sourceScale.y);
  actualMovement.bodyHeight=(consumedMax.z-consumedMin.z)*sourceScale.z;
  ActorMovement swampActor(actualMovement);swampActor.setPosition(spawn);
  check(visual.select("walk",true,error),error);check(visual.update(.1,error),error);
  check(swampActor.step_root_motion(input,basis,realFloor,scaled(visual.take_root_motion(),sourceScale),1),"original walk cannot be rejected at valid authored Swamp center");
  check(swampActor.state().grounded&&swampActor.state().position.x>spawn.x,"authored Swamp actor advances and remains grounded");
  std::cout<<"Swamp authored spawn movement passed; position="<<swampActor.state().position.x<<','<<swampActor.state().position.y<<','<<swampActor.state().position.z<<'\n';
 }
 ActorMovement autonomous(movement);autonomous.setPosition({0,0,0});
 const auto autonomousFloor=plane();
 check(!autonomous.apply_root_motion({10,0,0},autonomousFloor),"Idle manual input unexpectedly admitted displacement");
 check(autonomous.apply_authored_root_motion({10,0,0},autonomousFloor)&&near(autonomous.state().position.x,10),
       "Authored AI/combat displacement was suppressed by absent keyboard input");
 check(!autonomous.apply_root_motion({10,0,0},autonomousFloor)&&near(autonomous.state().position.x,10),
       "Authored displacement changed the persistent manual input multiplier");
 std::cout<<"actor original root motion passed; raw cycle="<<rawCycle.x<<','<<rawCycle.y<<','<<rawCycle.z
          <<" XY scale="<<sourceScale.x<<" Z scale="<<sourceScale.z<<" world distance="<<std::hypot(feet.x,feet.y)<<'\n';return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
