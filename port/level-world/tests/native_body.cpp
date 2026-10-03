#include "native_body.hpp"
#include "Box2D.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace dh2::physical;
namespace {
void require(bool v,const char* message){if(!v)throw std::runtime_error(message);}
bool near(float a,float b,float tolerance=0.00001f){return std::fabs(a-b)<=tolerance;}
struct Listener:b2ContactListener {unsigned added=0,resolved=0;void Add(const b2ContactPoint*)override{++added;}void Result(const b2ContactResult*)override{++resolved;}};
}
int main(){
 try{
  b2AABB bounds;bounds.lowerBound.Set(-100,-100);bounds.upperBound.Set(100,100);Listener listener;b2World world(bounds,b2Vec2(0,0),true);world.SetContactListener(&listener);
  b2BodyDef def;def.fixedRotation=true;def.isSleeping=true;def.isBullet=true;auto* body=world.CreateBody(&def);b2CircleDef circle;circle.radius=.36f;circle.localPosition.Set(.12f,-.08f);circle.density=1.f;body->CreateShape(&circle);body->SetMassFromShapes();NativeBody native{body,circle.radius,0};NativeBodyObservation observation{};float query[4]{};
  require(dh2_native_body_pin(&native)==0&&body->IsStatic()&&native.pinned==1,"Character initial pin");const auto local=body->GetLocalCenter();require(local.x==circle.localPosition.x&&local.y==circle.localPosition.y,"Pin changed local center");
  float zero[2]{0.f,-0.f};require(dh2_native_body_set_linear(&native,zero)==0&&body->IsSleeping(),"Zero linear request woke sleeping body");float speed[2]{1.5f,-.5f};require(dh2_native_body_set_linear(&native,speed)==0&&!body->IsSleeping(),"Nonzero linear request did not wake");const auto initial=body->GetPosition();world.Step(1.f/60.f,10);require(body->GetPosition().x==initial.x&&body->GetPosition().y==initial.y,"Pinned body moved in real world step");
  require(dh2_native_body_unpin(&native)==0&&body->IsDynamic()&&!body->IsSleeping(),"Move focus unpin failed");world.Step(1.f/60.f,10);require(near(body->GetPosition().x,initial.x+speed[0]/60.f)&&near(body->GetPosition().y,initial.y+speed[1]/60.f),"Real dynamic body did not advance");
  float increments[4]{2.f,-4.f,2.f,-1.f};require(dh2_native_body_add_linear(&native,increments)==0&&body->GetLinearVelocity().x==2.f&&body->GetLinearVelocity().y==-4.5f,"Upper-only velocity cap changed behavior");
  body->ApplyForce(b2Vec2(10,3),body->GetWorldCenter());float stopped_position[2]{200.f,-100.f};require(dh2_native_body_stop(&native,stopped_position)==0&&body->IsSleeping(),"Stop did not sleep");require(dh2_native_body_query(query,&native)==0&&query[0]==200.f&&query[1]==-100.f&&query[3]==36.f,"Native body game-unit query/radius");float angular=0.f;require(dh2_native_body_set_angular(&native,&angular)==0&&!body->IsSleeping(),"Zero angular request did not wake");world.Step(1.f/60.f,10);require(body->GetLinearVelocity().x==0.f&&body->GetLinearVelocity().y==0.f,"Stop failed to clear genuine accumulated force");
  require(dh2_native_body_pin(&native)==0&&dh2_native_body_pin(&native)==0&&native.pinned==1&&body->IsStatic(),"Repeated pin changed lifecycle");require(dh2_native_body_unpin(&native)==0&&dh2_native_body_unpin(&native)==0&&!native.pinned&&body->IsDynamic(),"Repeated unpin changed lifecycle");
  BodyState view{};view.flags=0x58;view.force[0]=13;view.force[1]=-17;view.torque=19;view.sleep_time=23;NativeSubobjectsBridge bridge{&native,&view,nullptr,nullptr,nullptr};require(dh2_native_body_subobject_service(&bridge,dh2::subobjects::physical_update,nullptr)==1&&view.position[0]==body->GetPosition().x&&view.force[0]==13&&view.torque==19&&view.sleep_time==23,"Public native view invented inaccessible fields");float transform_values[3]{0.f,0.f,0.f};require(dh2_native_body_subobject_service(&bridge,dh2::subobjects::apply_body_transform,transform_values)==1&&body->GetPosition().x==0,"Subobjects service did not transform genuine body");
  require(dh2_native_body_subobject_service(&bridge,dh2::subobjects::physical_set_velocity,speed)==1&&body->GetLinearVelocity().x==speed[0],"Subobjects velocity service");require(dh2_native_body_subobject_service(&bridge,dh2::subobjects::physical_wake,nullptr)==1,"Subobjects wake service");
  b2BodyDef wall_definition;wall_definition.position.Set(4,0);auto* wall=world.CreateBody(&wall_definition);b2PolygonDef wall_shape;wall_shape.SetAsBox(.5f,5.f);wall->CreateShape(&wall_shape);float origin[2]{0,0};dh2_native_body_set_position(&native,origin);float toward_wall[2]{3,0};dh2_native_body_set_linear(&native,toward_wall);unsigned steps=0;for(unsigned i=0;i<160;++i){world.Step(1.f/60.f,10);++steps;}
  require(listener.added>0&&listener.resolved>0&&world.GetContactCount()>0,"Real native contacts did not execute");require(body->GetPosition().x<3.1f,"Genuine shape/broadphase/solver failed to block motion");
  require(dh2_native_body_observe(&observation,&native)==0&&observation.dynamic&&observation.bullet&&observation.mass>0&&observation.inertia==0,"Public observation mismatch");
  const auto before=observation;float malformed[2]{std::numeric_limits<float>::infinity(),0};require(dh2_native_body_set_position(&native,malformed)==-1&&dh2_native_body_stop(&native,malformed)==1,"Malformed native position accepted");dh2_native_body_observe(&observation,&native);require(!std::memcmp(&before,&observation,sizeof(before)),"Malformed native input changed body");unsigned rejected=2;native.pinned=2;require(dh2_native_body_pin(&native)==1&&dh2_native_body_unpin(&native)==1,"Malformed lifecycle accepted");native.pinned=0;rejected+=2;
  view.flags=0x10000;const auto malformed_view=view;dh2_native_body_observe(&observation,&native);const auto before_service=observation;require(dh2_native_body_subobject_service(&bridge,dh2::subobjects::physical_set_velocity,speed)==0,"Malformed public view accepted");dh2_native_body_observe(&observation,&native);require(!std::memcmp(&before_service,&observation,sizeof observation)&&!std::memcmp(&malformed_view,&view,sizeof view),"Malformed service changed native body or caller view");++rejected;
  float outside[2]{20000.f,0.f};require(dh2_native_body_stop(&native,outside)==0&&body->IsFrozen()&&body->IsSleeping()&&body->GetLinearVelocity().x==0,"Stop failed after genuine broadphase freeze");
  std::cout<<"{\"genuine_world_steps\":"<<steps+3<<",\"contact_additions\":"<<listener.added<<",\"contact_solver_results\":"<<listener.resolved<<",\"pin_unpin_move_stop_verified\":true,\"native_force_clearing_verified_by_step\":true,\"subobjects_native_services\":4,\"atomic_rejection_checks\":"<<rejected<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
