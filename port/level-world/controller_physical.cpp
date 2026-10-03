#include "controller_physical.hpp"
namespace {
using namespace dh2::navigation;
bool valid_binding(const ControllerPhysicalBinding* binding){
 if(!binding)return true;
 if(!binding->body||!binding->body->state||binding->body->reserved||binding->body->state->flags>0xffffu||!binding->world||!binding->callbacks)return false;
 for(auto byte:binding->world->reserved)if(byte)return false;
 if(!binding->callbacks->commit)return false;
 if(binding->world->first_shape&&(!binding->callbacks->synchronize||!binding->callbacks->destroy_proxy))return false;
 return true;
}
}
extern "C" int dh2_nav_update_path_physical(ControllerPhysicalResult* out,
 const ControllerRequest* request,const ControllerPhysicalBinding* binding){
 if(!out||out->path.reserved||!request||!request->policy||!valid_binding(binding))return 1;
 // UpdatePath's existing scene is also the authoritative body-presence view.
 // Do not silently leave an original physical Stop request unapplied.
 if(request->policy->update_physics&&request->scene){
  const auto& scene=*request->scene;
  if(scene.reserved||(scene.count&&(!scene.keys||!scene.actors)))return 1;
  bool present=false;
  for(unsigned i=0;i<scene.count;++i)if(scene.keys[i]==request->key){
   if(scene.actors[i].physical.present>1)return 1;
   present=scene.actors[i].physical.present;
  }
  if(present!=bool(binding))return 1;
 }
 ControllerPhysicalResult result{};
 const int status=dh2_nav_update_path(&result.path,request);
 if(status)return status;
 if(result.path.physical_stop_requested){
  // Original Stop ignores SetXForm's bool. Even a frozen body or a failed
  // shape synchronization proceeds through the final sleep/force resets.
  dh2::physical::TransformRequest transform{};
  dh2_physical_stop_begin(&transform,binding->body->state,request->controller->position);
  result.transform_result=dh2_body_set_transform(binding->body,&transform,binding->world,binding->callbacks);
  dh2_physical_stop_finish(binding->body->state);
  result.stop_applied=1;
 }
 *out=result;return 0;
}
