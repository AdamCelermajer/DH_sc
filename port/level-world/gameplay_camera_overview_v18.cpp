#include "gameplay_camera_overview_v18.hpp"
#include <cmath>
#include <cstring>
namespace dh2::camera {
namespace {float raw(std::uint32_t bits){float out;std::memcpy(&out,&bits,4);return out;}}
bool GameplayCameraOverviewV18::initialize(std::string& e){
 if(initialized_||node_){e="Overview constructor already reached its camera allocation";return false;}if(!services_.application||!services_.actual_first_factory||!services_.manager){e="Required actual Application/first scene factory/SceneManager for Overview C1";return false;}
 if(!services_.actual_first_factory->create(0x5f6d6163,0,node_,e))return false;
 // Source factory miss is legal CameraBase NULL, but actual default cam_
 // constructor must produce a node in this registered source family.
 if(!node_){e="Registered original cam_ factory returned no camera node";return false;}
 if(!node_->grab(e))return false;retained_camera_=true;
 if(!node_->add_to_root(e))return false;
 if(!node_->set_data(raw(0x3edbf877),raw(0x3fe38e39),raw(0x466a6000),raw(0x46c35000),e)||!node_->set_up({0,1,0},e)||!node_->set_target({0,0,0},e))return false;
 if(!node_->set_rotation({0,raw(0x3f59d4d0),0,raw(0x3f067b80)},e)||!node_->set_position({0,0,raw(0x469c4000)},e))return false;initialized_=true;return true;
}
bool GameplayCameraOverviewV18::activated(std::string& e){
 if(!node_||!initialized_||!services_.local_player0){e="Required constructed Overview and actual local PlayerInfo0";return false;}bool found;PointV2 player;if(!services_.local_player0(found,player,e))return false;x1c_=y20_=z24_=0;base10_=found?player:PointV2{};
 if(!services_.pf_z_bounds){e="Required actual static PFWorld source bounds";return false;}bool world;float low,high;if(!services_.pf_z_bounds(world,low,high,e))return false;float middle=0;if(world){const float span=high-low;const float half=span*.5f;middle=half+low;}const float height=middle+20000.f;
 if(!node_->set_planes(height-5000.f,height+5000.f,e)||!node_->set_target({base10_[0],base10_[1],middle},e)||!node_->set_position({base10_[0],base10_[1],height},e))return false;
 if(!services_.events14){e="Required SAME Application EventManager14 for Overview activation";return false;}
 events::EventReceiverV12 receiver{identity(),this,receive,shared_from_this()};bool inserted;if(!services_.events14->attach(0,receiver,INT32_MAX,inserted,e))return false;event_attached_=true;return true;
}
bool GameplayCameraOverviewV18::deactivated(std::string& e){if(!services_.events14){e="Required SAME Application EventManager14 for Overview deactivation";return false;}bool removed;if(!services_.events14->detach(0,identity(),removed,e))return false;event_attached_=false;return true;}
bool GameplayCameraOverviewV18::receive(void* p,const events::EventBorrowV12& event,events::EventManagerOwnerV12&,std::int32_t& consumed,std::string& e){return static_cast<GameplayCameraOverviewV18*>(p)->on_event(event,consumed,e);}
bool GameplayCameraOverviewV18::on_event(const events::EventBorrowV12& event,std::int32_t& consumed,std::string& e){
 consumed=0;if(!event.get_type){e="Required Overview event GetType";return false;}std::int32_t type;if(!event.get_type(event.context,type,e))return false;if(type!=0)return true;if(!services_.keyboard){e="Required actual source Overview keyboard payload";return false;}CameraKeyboardV18 key;if(!services_.keyboard(event,key,e))return false;
 const auto index=static_cast<std::uint32_t>(key.key)-0x41u;if(index>0x17u)return true;
 if(!key.pressed){if((0xc50019u&(1u<<index))!=0){z24_=0;consumed=1;}return true;}
 switch(key.key){case 'A':x1c_=x1c_-100.f;break;case 'D':x1c_=x1c_+100.f;break;case 'E':z24_=100.f;break;case 'Q':z24_=-100.f;break;case 'S':y20_=y20_-100.f;break;case 'W':y20_=y20_+100.f;break;case 'X':x1c_=y20_=z24_=0;break;default:return true;}consumed=1;return true;
}
bool GameplayCameraOverviewV18::update(std::string& e){
 if(!node_)return true;if(!services_.first_gamepad){e="Required actual InputManager first-connected-gamepad producer";return false;}bool present;CameraGamepadV18 pad;if(!services_.first_gamepad(present,pad,e))return false;
 if(present){const float xx=(pad.x+pad.x)/(pad.x_max-pad.x_min),yy=-((pad.y+pad.y)/(pad.y_max-pad.y_min));const float x2=xx*xx,y2=yy*yy,sum=(x2+y2)+0.f;const float length=static_cast<float>(std::sqrt(static_cast<double>(sum)));PointV2 direction{};
  if(length<.5f){if(!services_.source_vec3_zero){e="Required actual Point3D Vec3ZERO initializer";return false;}direction=*services_.source_vec3_zero;}
  else{const float inverse=1.f/length;direction={xx*inverse,yy*inverse,0};if(length<1.f){const float half=length-.5f,factor=half+half;for(auto& v:direction)v=v*factor;}}
  const float zoom_sum=pad.zoom_min+pad.zoom_max,zoom_plus=zoom_sum+1.f,zoom_threshold=zoom_plus*.5f;
  if(pad.zoom>=zoom_threshold)z24_=direction[1]*-100.f;
  else{x1c_=direction[0]*100.f+x1c_;y20_=direction[1]*100.f+y20_;const float stop_sum=pad.stop_min+pad.stop_max,stop_plus=stop_sum+1.f,stop_threshold=stop_plus*.5f;if(pad.stop>=stop_threshold)z24_=0;}
 }
 if(x1c_+y20_==0.f){if(!services_.local_player0){e="Required same local PlayerInfo0 during Overview update";return false;}bool found;PointV2 p;if(!services_.local_player0(found,p,e))return false;if(found)base10_=p;}
 CameraViewV11 view;if(!node_->view(view,e))return false;const float x=base10_[0]+x1c_,y=base10_[1]+y20_,z=view.eye[2]+z24_;if(!node_->set_position({x,y,z},e))return false;return node_->set_target({x,y,view.target[2]+z24_},e);
}
bool GameplayCameraOverviewV18::close_base(GameplayCameraActiveV13& active,std::string& e){
 // Explicit modern lifetime repair if a caller destroys an active keyboard
 // receiver without source Deactivated: detach before releasing its context.
 if(event_attached_&&!deactivated(e))return false;active.destroy_receiver(identity());if(!node_)return true;if(!node_->drop(e))return false;if(retained_camera_){if(!node_->drop(e))return false;retained_camera_=false;}node_.reset();initialized_=false;return true;
}
CameraBaseBorrowV13 GameplayCameraOverviewV18::base_borrow(){CameraBaseBorrowV13 out;out.receiver=shared_from_this();out.identity=identity();std::weak_ptr<GameplayCameraOverviewV18> weak=shared_from_this();out.activated=[weak](std::string& e){auto self=weak.lock();if(!self){e="Released Overview Activated receiver";return false;}return self->activated(e);};out.deactivated=[weak](std::string& e){auto self=weak.lock();if(!self){e="Released Overview Deactivated receiver";return false;}return self->deactivated(e);};if(node_)out.scene_camera=node_->camera_borrow();return out;}
}
