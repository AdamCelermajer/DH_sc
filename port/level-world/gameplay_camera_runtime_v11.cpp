#include "gameplay_camera_runtime_v11.hpp"
#include <cmath>
#include <cstring>
namespace dh2::camera {
namespace {float source_float(std::uint32_t bits){float out;std::memcpy(&out,&bits,4);return out;}}
bool GameplayCameraRuntimeV11::load(const CameraLoadV11& input,std::string& e){
 if(scene_||loaded_||closed_){e="Required fresh source CameraLevel load lifecycle";return false;}
 input_=input;
 if(!services_.world_lease||!services_.tables||!services_.manager||!services_.read){e="Required same World/camera tables/Application AnimSetManager/resource owner";return false;}
 // Source Level::_LoadCamera finds the first exact member name. A missing row
 // would be an unsafe original -1 dereference, reported before that access.
 const auto& names=services_.tables->camera_names;
 for(std::size_t i=0;i<names.size();++i)if(names[i]==input.animation_set){row_=static_cast<std::int32_t>(i);break;}
 if(row_<0||std::size_t(row_)>=services_.tables->cameras.size()){e="Required actual LevelConfig CamAnimSet member";return false;}
 scene_=std::make_shared<GameplayCameraSceneV3>();std::vector<std::uint8_t> bytes;
 if(!services_.read(input.file,bytes,e)||!scene_->load(std::move(bytes),e))return false;
 // Visual constructor publishes its real root before node selection and
 // AnimationSet registration. A failed attachment still requires discard.
 if(!services_.attach_graph){e="Required actual camera visual SceneManager root attachment";return false;}
 attach_attempted_=true;if(!services_.attach_graph(scene_,e))return false;
 if(!scene_->select(input.node,camera_,e))return false;
 if(services_.retain_selected_v19&&!services_.retain_selected_v19(scene_,camera_,e))return false;
 const auto& selected=scene_->cameras().at(camera_);const auto& graph=scene_->graph().graph;
 // CalculateDefaultTargetDistance virtual+a0 is getPosition, not absolute
 // position: use camera instance's authored parent and target node locals.
 const auto& parent=graph.at(selected.node);const auto& target=graph.at(selected.target_node);
 const float x=target.translation[0]-parent.translation[0],y=target.translation[1]-parent.translation[1],z=target.translation[2]-parent.translation[2];
 const float xx=x*x,yy=y*y,zz=z*z;const float distance=std::sqrt((xx+yy)+zz);
 if(!services_.manager->create(set_id_,e)||!services_.manager->register_camera(set_id_,services_.tables->cameras[row_],scene_->graph(),e))return false;
 animator_=std::make_shared<GameplayCameraAnimatorV10>(scene_,services_.manager->find(set_id_));if(!animator_->initialize(e))return false;
 // Source controller attaches its same AnimatorSet to root in its C1. The
 // borrowed production service must perform real root membership/onBind.
 if(!services_.attach_animator){e="Required same SceneManager camera animator attachment";return false;}
 if(!services_.attach_animator(scene_,animator_,e))return false;attached_=true;
 auto s=services_.actor_services;s.target.scene_lease=scene_;s.target.root_present=true;s.camera_present=true;
 s.target.root_position=[this](PointV2& out,std::string& error){return scene_->root_position(out.data(),error);};
 s.target.root_set_position=[this](const PointV2& p,std::string& error){return scene_->set_root_position(p.data(),error);};
 s.camera_set_position=[this](const PointV2& p,std::string& error){return scene_->set_camera_instance_position(camera_,p.data(),error);};
 s.camera_set_clip_start=[this](float value,std::string& error){
  if(!scene_||closed_){error="Required same retained CameraLevel near-plane owner";return false;}
  view_.near_plane=value;error.clear();return true;
 };
 s.camera_set_clip_end=[this](float value,std::string& error){
  if(!scene_||closed_){error="Required same retained CameraLevel far-plane owner";return false;}
  view_.far_plane=value;error.clear();return true;
 };
 s.active_is_this=[this](bool& active,std::string& error){if(!services_.is_active){error="Required process CameraBase active identity";return false;}return services_.is_active(reinterpret_cast<std::uintptr_t>(this),active,error);};
 s.zoom_bounds=[this](bool mode,float& lower,float& upper,std::string& error){return source_zoom_bounds_v5(services_.design,mode,lower,upper,error);};
 s.controller_present=true;s.controller_clip_indexc=&animator_->clip_indexc;s.controller_byte10=&animator_->byte10;
 s.controller_play=[this](std::int32_t id,bool& played,std::string& error){return animator_->play(id,false,played,error);};
 level_=std::make_unique<GameplayCameraLevelV4>(std::move(s));level_->fields().default_distance94=distance;
 animator_->set_source_completion([this](){level_->source_animation_callback();});
 // Exact Level.SetData follows registration. Its fixed aspect supersedes
 // onChangedSceneManager's constructor viewport-derived aspect.
 view_.fov=source_float(0x3edbf877);view_.aspect=source_float(0x3fd578e9);
 view_.near_plane=static_cast<float>(input.near_plane);view_.far_plane=static_cast<float>(input.far_plane);
 const float projection[4]{view_.fov,view_.aspect,view_.near_plane,view_.far_plane};dh2_camera_perspective_v8(&view_.projection,projection);
 loaded_=true;return true;
}
bool GameplayCameraRuntimeV11::activate(std::string& e){if(!loaded_||closed_){e="Required loaded camera activation";return false;}if(!services_.activate){e="Required original CameraBase/SceneManager activation";return false;}return services_.activate(reinterpret_cast<std::uintptr_t>(this),scene_,camera_,e);}
bool GameplayCameraRuntimeV11::play_idle(std::string& e){if(!loaded_||closed_){e="Required loaded CameraLevel idle";return false;}return level_->play_animation(services_.tables->cameras[row_].idle,0,false,e);}
bool GameplayCameraRuntimeV11::set_target(std::uintptr_t actor,std::int32_t ms,std::string& e){if(!loaded_||closed_){e="Required loaded CameraLevel target";return false;}return level_->target().set_target(actor,ms,e);}
bool GameplayCameraRuntimeV11::source_clip_defaults_v120(std::int32_t& near_plane,std::int32_t& far_plane,std::string& e)const{
 if(!loaded_||closed_||!level_||!scene_){e="Required same loaded CameraLevel and retained LevelConfig clip fields";return false;}
 near_plane=input_.near_plane;far_plane=input_.far_plane;e.clear();return true;
}
bool GameplayCameraRuntimeV11::set_clip_start(float value,std::string& e){
 if(!loaded_||closed_||!level_){e="Required same loaded CameraLevel near-plane receiver";return false;}
 return level_->set_clip_start(value,e);
}
bool GameplayCameraRuntimeV11::set_clip_end(float value,std::string& e){
 if(!loaded_||closed_||!level_){e="Required same loaded CameraLevel far-plane receiver";return false;}
 return level_->set_clip_end(value,e);
}
bool GameplayCameraRuntimeV11::source_map_fields_v1(std::uint8_t byte133,std::uint32_t word136,float float140,std::string& e){
 if(!loaded_||closed_||!scene_||!level_){e="Required same loaded Map CameraLevel fields133/136/140";return false;}
 // CameraLevel::Update reads +133 as map-mode, while HandleZoom reads the
 // float words at +136/+140 as requested and automatic zoom. Keep these in
 // this runtime's existing source-shaped LevelState, not in shadow fields.
 level_->fields().mode85=byte133!=0;
 level_->fields().automatic_zoom8c=float140;
 level_->fields().requested_zoom88=source_float(word136);
 e.clear();return true;
}
bool GameplayCameraRuntimeV11::source_set_data_v1(float horizontal_fov_or_mag,float aspect,float near_plane,float far_plane,bool unused,std::string& e){
 (void)unused; // CameraBase::SetData's bool parameter is not read in its body.
 if(!loaded_||closed_||!scene_||!level_){e="Required same loaded CameraBase SetData receiver";return false;}
 // Original CameraBase::SetData invokes virtual +316 then +312, before the
 // CameraLevel near/far virtuals at +304/+308; +276 is its source up vector.
 // The loaded source camera record distinguishes horizontal FOV from
 // magnification by camera kind; this owner's view field stores that exact
 // +316 scalar, while +312 is the separately-authored aspect.
 view_.fov=horizontal_fov_or_mag;view_.aspect=aspect;
 if(!level_->set_clip_start(near_plane,e)||!level_->set_clip_end(far_plane,e))return false;
 return source_set_camera_vector_v1({0.0f,0.0f,1.0f},e);
}
bool GameplayCameraRuntimeV11::source_set_camera_vector_v1(const PointV2& vector,std::string& e){
 if(!loaded_||closed_||!scene_||!level_){e="Required same loaded CameraBase virtual +276 vector receiver";return false;}
 view_.up=vector;e.clear();return true;
}
bool GameplayCameraRuntimeV11::update(std::string& e){if(!loaded_||closed_){e="Required loaded CameraLevel update";return false;}return level_->update(e);}
bool GameplayCameraRuntimeV11::scene_phase(std::uint32_t timestamp,std::string& e){if(!loaded_||closed_){e="Required retained camera SceneManager animation phase";return false;}return animator_->scene_phase(timestamp,e);}
bool GameplayCameraRuntimeV11::source_update_absolute_v67(std::string& e){
 if(!loaded_||closed_||!scene_){e="Required SAME loaded camera absolute-position owner";return false;}
 return scene_->update_selected_absolute_v67(camera_,e);
}
bool GameplayCameraRuntimeV11::source_positions_v67(PointV2& camera,PointV2& parent,std::string& e){
 if(!loaded_||closed_||!scene_){e="Required SAME actual camera/parent position cache";return false;}
 PointV2 target;
 return scene_->eye_and_target(camera_,camera.data(),target.data(),e)&&scene_->camera_parent_position_v67(camera_,parent.data(),e);
}
bool GameplayCameraRuntimeV11::view(CameraViewV11& out,std::string& e){if(!loaded_||closed_){e="Required loaded same-scene camera view";return false;}if(!scene_->eye_and_target(camera_,view_.eye.data(),view_.target.data(),e))return false;dh2_camera_look_at_v8(&view_.view,view_.eye.data(),view_.target.data(),view_.up.data());const float projection[4]{view_.fov,view_.aspect,view_.near_plane,view_.far_plane};dh2_camera_perspective_v8(&view_.projection,projection);out=view_;return true;}
bool GameplayCameraRuntimeV11::source_node_vectors_v67(PointV2&look,PointV2&up,std::string&e)const{
 if(!loaded_||closed_||!scene_||camera_>=scene_->cameras().size()){e="Required same selected source camera node8";return false;}
 const auto& selected=scene_->cameras()[camera_];const auto& graph=scene_->graph().graph;
 if(selected.node>=graph.size()){e="Required retained source camera parent transform";return false;}
 const auto& absolute=graph[selected.node].world;
 for(unsigned i=0;i<3;++i){look[i]=absolute[4+i];up[i]=absolute[8+i];}
 e.clear();return true;
}
bool GameplayCameraRuntimeV11::close(std::string& e){
 if(closed_)return true;
 // CameraLevel D1 destroys Visual/controller first, then CameraTarget D2
 // reaches CameraBase D2, which clears only its own process-active slot.
 if(attach_attempted_){if(!services_.detach){e="Required same camera animator/root unregistration";return false;}if(!services_.detach(scene_,animator_,e))return false;attach_attempted_=false;attached_=false;}
 if(animator_)animator_->release_after_unregistration();
 if(loaded_){if(!services_.release_active){e="Required CameraBase destruction active-slot release";return false;}if(!services_.release_active(reinterpret_cast<std::uintptr_t>(this),e))return false;}
 level_.reset();animator_.reset();scene_.reset();loaded_=false;closed_=true;return true;
}
}
