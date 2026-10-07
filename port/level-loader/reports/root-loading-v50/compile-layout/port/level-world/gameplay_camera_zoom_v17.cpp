#include "gameplay_camera_zoom_v17.hpp"
#include <cmath>
namespace dh2::camera {
bool GameplayCameraZoomV17::viewport_inverse(std::string& e){if(!services_.backend.configured_backend||!services_.backend.viewport){e="Required actual ZoomHandler current driver viewport";return false;}std::int32_t width,height;if(!services_.backend.viewport(width,height,e))return false;const auto maximum=width<height?height:width;inverse_viewport28_=1.f/static_cast<float>(maximum);return true;}
bool GameplayCameraZoomV17::current(std::shared_ptr<GameplayCameraRuntimeV11>& out,std::string& e)const{out.reset();if(!source_camera_identity20_)return true;out=camera20_.lock();if(!out||!out->level()){e="Lost source ZoomHandler camera20 borrow";return false;}return true;}
bool GameplayCameraZoomV17::receive(void* p,const events::EventBorrowV12& event,events::EventManagerOwnerV12&,std::int32_t& consumed,std::string& e){return static_cast<GameplayCameraZoomV17*>(p)->on_event(event,consumed,e);}
bool GameplayCameraZoomV17::initialize(std::shared_ptr<events::EventManagerOwnerV12> events,std::string& e){
 if(initialized_||attach4_||attach5_){e="ZoomHandler construction already reached registration";return false;}if(!events){e="Required actual Application PostInit heap EventManager14";return false;}application_events14_=events;if(!viewport_inverse(e))return false;
 events::EventReceiverV12 receiver{reinterpret_cast<std::uintptr_t>(this),this,receive,shared_from_this()};bool inserted;
 if(!events->attach(4,receiver,0,inserted,e))return false;attach4_=inserted;
 if(!events->attach(5,receiver,0,inserted,e))return false;attach5_=inserted;initialized_=true;return true;
}
bool GameplayCameraZoomV17::set_camera(std::shared_ptr<GameplayCameraRuntimeV11> camera,std::string& e){camera20_=camera;source_camera_identity20_=camera?reinterpret_cast<std::uintptr_t>(camera->level()):0;enabled24_=0;drag34_=0;return viewport_inverse(e);}
bool GameplayCameraZoomV17::reset_zoom(std::string& e){std::shared_ptr<GameplayCameraRuntimeV11> camera;if(!current(camera,e))return false;if(!camera){e="Required non-NULL source ZoomHandler ResetZoom camera";return false;}camera->level()->fields().automatic_zoom8c=1;camera->level()->fields().requested_zoom88=0;return true;}
bool GameplayCameraZoomV17::on_event(const events::EventBorrowV12& event,std::int32_t& consumed,std::string& e){
 consumed=0;std::shared_ptr<GameplayCameraRuntimeV11> camera;if(!current(camera,e))return false;if(!camera||!camera->level()->fields().mode85)return true;
 if(!event.get_type){e="Required actual camera touch event virtual GetType";return false;}std::int32_t type;if(!event.get_type(event.context,type,e))return false;if(type!=4&&type!=5)return true;
 if(!services_.touch){e="Required actual source touch coordinates/id/pressed payload";return false;}CameraTouchV17 input;if(!services_.touch(event,input,e))return false;
 if(type==4){
  if(!input.pressed)touches8_.erase(input.finger);
  else{
   if(!current(camera,e)||!camera){e="Camera removed during source touch callback";return false;}
   bool accepted=true;if(camera->level()->fields().mode85){if(!services_.map_byte1e4){e="Required original Map singleton byte1e4";return false;}bool inactive;if(!services_.map_byte1e4(inactive,e))return false;accepted=!inactive;if(accepted){if(!services_.map_point_inside){e="Required original Map.IsPointInsideRenderZone";return false;}if(!services_.map_point_inside(input.x,input.y,accepted,e))return false;}}
   if(accepted){if(!services_.touch(event,input,e))return false;auto& record=touches8_[input.finger];record.start_x=input.x;record.start_y=input.y;record.current_x=input.x;record.current_y=input.y;}
  }consumed=touches8_.size()>1?1:0;return true;
 }
 auto& record=touches8_[input.finger];if(record.start_x==0&&record.start_y==0){record.start_x=input.x;record.start_y=input.y;}record.current_x=input.x;record.current_y=input.y;
 if(touches8_.size()>1){
  // Source increments begin(): the first TWO ordered fingers, not first/last.
  const auto first=touches8_.begin(),second=std::next(first);
  auto distance=[](float x,float y){const float xx=x*x,yy=y*y,sum=xx+yy;return static_cast<float>(std::sqrt(static_cast<double>(sum)));};
  const float old_x=static_cast<float>(first->second.start_x)-static_cast<float>(second->second.start_x),old_y=static_cast<float>(first->second.start_y)-static_cast<float>(second->second.start_y);
  const float new_x=static_cast<float>(first->second.current_x)-static_cast<float>(second->second.current_x),new_y=static_cast<float>(first->second.current_y)-static_cast<float>(second->second.current_y);
  const float old_distance=distance(old_x,old_y),new_distance=distance(new_x,new_y);const float change=(new_distance-old_distance)*inverse_viewport28_;
  camera->level()->fields().requested_zoom88=change+camera->level()->fields().requested_zoom88;consumed=1;
 }else if(camera->level()->fields().mode85){const float dx=(static_cast<float>(input.x)-static_cast<float>(record.start_x))*50.f,dy=(-(static_cast<float>(input.y)-static_cast<float>(record.start_y)))*50.f;auto& offset=camera->level()->fields().offset98;const float y=offset[1]-dy,x=offset[0]-dx;offset[1]=y;offset[0]=x;}
 record.start_x=input.x;record.start_y=input.y;return true;
}
bool GameplayCameraZoomV17::on_mouse(const CameraMouseV17& input,std::int32_t& consumed,std::string& e){
 consumed=0;std::shared_ptr<GameplayCameraRuntimeV11> camera;if(!current(camera,e))return false;if(!camera||input.type!=1)return true;auto& fields=camera->level()->fields();
 if(input.subtype==7){const float step=inverse_viewport28_*10.f;const float change=step*input.wheel;fields.requested_zoom88=change+fields.requested_zoom88;consumed=1;return true;}
 if(!enabled24_)return true;
 if(input.subtype==0){if(fields.mode85){if(!services_.map_byte1e4){e="Required original Map singleton mouse gate";return false;}bool inactive;if(!services_.map_byte1e4(inactive,e))return false;if(!inactive){if(!services_.map_point_inside){e="Required original Map mouse render zone";return false;}bool inside;if(!services_.map_point_inside(input.x,input.y,inside,e))return false;}}
  drag34_=1;drag_x2c_=input.x;drag_y30_=input.y;snapshot38_=fields.offset98;consumed=1;return true;
 }
 if(input.subtype==3){drag34_=0;consumed=1;return true;}
 if(input.subtype!=6||!drag34_)return true;
 const float x=(-(static_cast<float>(input.x)-static_cast<float>(drag_x2c_)))*50.f+snapshot38_[0];const float y=(static_cast<float>(input.y)-static_cast<float>(drag_y30_))*50.f+snapshot38_[1];const float z=snapshot38_[2]+0.f;fields.offset98[0]=x;fields.offset98[2]=z;fields.offset98[1]=y;consumed=1;return true;
}
bool GameplayCameraZoomV17::close(std::string& e){auto events=application_events14_.lock();if((attach4_||attach5_)&&!events){e="Required same Application events14 for ZoomHandler destruction";return false;}bool removed;const auto identity=reinterpret_cast<std::uintptr_t>(this);if(attach4_){if(!events->detach(4,identity,removed,e))return false;attach4_=false;}if(attach5_){if(!events->detach(5,identity,removed,e))return false;attach5_=false;}touches8_.clear();initialized_=false;return true;}
}
