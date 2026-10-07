#include "audio_level_gameplay_v67.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
namespace dh2::audio {
bool AudioLevelGameplayV67::producer(std::string&error)const{
 if(std::this_thread::get_id()==producer_)return true;
 error="Required SAME owning Level audio producer thread";return false;
}
bool AudioLevelGameplayV67::current(std::string&error){
 if(!producer(error))return false;
 if(released_v101_||!bridge_){error="Released actual Level audio providers";return false;}
 const auto&s=bridge_->services_on_producer();std::shared_ptr<loader::CanonicalLevelContextV1>current;
 if(!s.current_level||!s.current_level(current,error))return false;
 if(current!=level_||!bridge_->manager().manager->runtime_on_producer()){error="Required SAME active Level/Application audio producer";return false;}
 error.clear();return true;
}
std::shared_ptr<AudioLevelGameplayV67> AudioLevelGameplayV67::compose(AudioCampaignServicesV46 s,
 AudioLevelBindingsV67 b,loader::LevelGameplayServicesV66&out,std::string&error){
 if(!s.selected_level||!b.camera||!b.roots||b.camera->roots()!=b.roots||
    !b.config||!b.application_byte_b4||!b.source_music_volume||!b.plain_command||
    !b.driver||!b.local_character0||!b.target_position||!b.look_at||!b.visual_up){
  error="Required actual SAME Level camera/scene/PM/settings/plain Play leaves";return {};
 }
 auto owner=std::make_shared<AudioLevelGameplayV67>();owner->level_=s.selected_level;owner->bindings_=std::move(b);
 if(!s.exact_legacy_listener_stream||!audio_listener_rows_v38(s.exact_legacy_listener_stream->data(),s.exact_legacy_listener_stream->size(),owner->rows_,error))return {};
 const std::weak_ptr<AudioLevelGameplayV67>weak=owner;
 s.camera_owner=owner->bindings_.camera;
 s.actual_update_listener=[weak](const auto&row,float*p,float*f,float*u,std::string&e){
  const auto live=weak.lock();if(!live){e="Required retained actual Level listener owner";return false;}
  return live->listener_vectors(row,p,f,u,e);
 };
 owner->bridge_=AudioCampaignBridgeV46::publish(std::move(s),error);if(!owner->bridge_)return {};
 out.start_level_sound=[owner](std::string&e){return owner->start_level_sound(e);};
 out.update_listener=[owner](std::string&e){return owner->update_listener(e);};
 out.set_ambient=[owner](std::string&e){return owner->set_ambient(e);};
 error.clear();return owner;
}
bool AudioLevelGameplayV67::fade(int milliseconds,std::uint32_t&frames,std::string&error){
 if(milliseconds==0){frames=0;return true;}
 if(milliseconds<0){error="Required nonnegative actual source fade";return false;}
 AudioDeviceClockV40 clock;auto*runtime=bridge_->manager().manager->runtime_on_producer();
 if(!runtime||!runtime->clock().snapshot(clock)||!clock.ready||!clock.rate){error="Required actual output rate for reached source fade";return false;}
 const auto count=std::uint64_t(milliseconds)*clock.rate/1000;
 if(count>UINT32_MAX){error="Source fade exceeds transport capacity";return false;}
 frames=std::uint32_t(count);return true;
}
bool AudioLevelGameplayV67::play(int ordinal,bool enabled,int milliseconds,int state,bool bypass_online,std::string&error){
 // Exact Play36b80c prefix; no current-Level/loading/listener Play3D gates.
 const auto&s=bridge_->services_on_producer();bool disabled{};
 if(!s.actual_debug("IsDisablingSounds",disabled,error))return false;
 if(disabled||ordinal<0){error.clear();return true;}
 if(!bypass_online){bool online{},muted{};
  if(!s.actual_online(online,error))return false;
  if(online){if(!s.actual_network_muted||!s.actual_network_muted(muted,error)){if(error.empty())error="Required actual network mute byte";return false;}
   if(muted){error.clear();return true;}}
 }
 auto manager=bridge_->manager().manager;
 if(manager->disabled()){if(!bindings_.platform_play){error="Required source platform Play type2";return false;}return bindings_.platform_play(ordinal,enabled,2,error);}
 auto*runtime=manager->runtime_on_producer();if(!runtime){error="Required SAME plain Play runtime";return false;}
 return runtime->submit_plain_source(ordinal,AudioAuthoredEventScopeV46::current(),
  [this,enabled,state,milliseconds](const auto&sound,const auto&group,AudioCommandV34&command,std::string&e){
   if(!bindings_.plain_command(enabled,state,sound,group,command,e))return false;
   return fade(milliseconds,command.fade_frames,e);
  },error);
}
bool AudioLevelGameplayV67::play_plain_v115(int ordinal,bool enabled,int milliseconds,int group,bool bypass_online,std::string& error){
 if(!producer(error)||released_v101_||!bridge_){if(error.empty())error="Released SAME plain Play provider";return false;}
 return play(ordinal,enabled,milliseconds,group,bypass_online,error);
}
bool AudioLevelGameplayV67::stop_music(int milliseconds,std::string&error){
 auto manager=bridge_->manager().manager;auto&fields=manager->music_fields_on_producer();
 if(fields.current_music_24==-1){error.clear();return true;}
 const auto old=fields.current_music_24;
 if(!manager->stop_sound_v106(old,milliseconds,bindings_.platform_stop,error))return false;
 fields.current_music_24=-1;fields.field_28=old;error.clear();return true;
}
bool AudioLevelGameplayV67::stop_music_v117(int milliseconds,std::string& error){
 if(!producer(error)||released_v101_||!bridge_){if(error.empty())error="Released SAME StopMusic provider";return false;}
 return stop_music(milliseconds,error);
}
bool AudioLevelGameplayV67::play_music(int music,bool enabled,bool stop,int milliseconds,std::string&error){
 if(!producer(error))return false;
 if(released_v101_||!bridge_){error="Released actual Level music providers";return false;}
 const auto&s=bridge_->services_on_producer();float volume{};bool disabled{},app_enabled{};
 if(!bindings_.source_music_volume(volume,error))return false;
 if(!std::isfinite(volume)){error="Required actual GetSoundVolume(2)";return false;}
 if(volume<.5f){error.clear();return true;}
 if(!s.actual_debug("IsDisablingSounds",disabled,error))return false;
 if(disabled){error.clear();return true;}
 if(!bindings_.application_byte_b4(app_enabled,error))return false;
 if(!app_enabled){error.clear();return true;}
 auto manager=bridge_->manager().manager;auto&fields=manager->music_fields_on_producer();
 if(music==-1){if(!stop){error.clear();return true;}fields.current_music_24=-1;return stop_music(milliseconds,error);}
 if(music==fields.current_music_24&&!manager->disabled()){
  bool playing{};auto*runtime=manager->runtime_on_producer();
  if(!runtime||!runtime->source_ordinal_playing(music,playing,error))return false;
  if(playing){std::uint32_t frames{};if(!fade(50,frames,error))return false;return runtime->resume_source_ordinal(music,frames,error);}
 }
 fields.field_28=fields.current_music_24;
 if(fields.current_music_24!=music&&!stop_music(milliseconds,error))return false;
 fields.current_music_24=music;fields.enabled_30=std::uint8_t(enabled);
 if(manager->disabled()){if(!bindings_.platform_play){error="Required actual platform PlayMusic type1";return false;}return bindings_.platform_play(music,enabled,1,error);}
 return play(music,enabled,milliseconds,2,true,error);
}
bool AudioLevelGameplayV67::set_in_safe_zone_music(bool safe,std::string&error){
 if(!current(error))return false;
 auto manager=bridge_->manager().manager;auto&fields=manager->music_fields_on_producer();auto source=level_->config_fields();
 if(manager->disabled()){fields.level_music_32=std::uint8_t(safe);return play_music(*source.music11c,true,false,2000,error);}
 if(safe){if(*source.safezone120<0){error="Source safezone music assertion: actual Level120 is negative";return false;}
  fields.level_music_32=1;if(fields.ambient_31)return play_music(*source.safezone120,true,false,2000,error);
  error.clear();return true;}
 fields.level_music_32=0;return play_music(*source.music11c,true,false,2000,error);
}
bool AudioLevelGameplayV67::start_level_sound(std::string&error){
 if(!current(error))return false;
 const auto source=level_->constructor_borrow_v3();auto&fields=bridge_->manager().manager->music_fields_on_producer();
 if(!source.fields->byte144){
  if(fields.level_music_32){if(!set_in_safe_zone_music(false,error))return false;}
  else if(!play_music(*source.music11c,true,false,2000,error))return false;
  if(!play(*source.ambient124,false,0,0,false,error))return false;
  fields.ambient_31=1;
 }
 source.fields->byte144=1;error.clear();return true;
}
bool AudioLevelGameplayV67::set_ambient(std::string&error){
 if(!current(error))return false;
 const auto source=level_->config_fields();const auto*config=bindings_.config(*source.config38);
 const auto*color=config?config->vector(0x1cc):nullptr;
 if(!color){error="Required SAME LevelConfig1cc ambient RGB";return false;}
 for(float c:*color)if(!std::isfinite(c)){error="Nonfinite authored ambient RGB";return false;}
 bindings_.roots->set_ambient_v67({(*color)[0],(*color)[1],(*color)[2],1.f});error.clear();return true;
}
namespace {
std::int32_t source_f2iz(float x){return std::isnan(x)?0:x>=2147483648.f?INT32_MAX:x<=-2147483648.f?INT32_MIN:std::int32_t(x);}
bool world_pixel(const camera::GameplayCameraPickingV20&p,float x,float y,camera::PointV2&out,std::string&e){
 camera::CameraRayV20 ray;if(!p.ray({source_f2iz(x),source_f2iz(y)},ray,e))return false;
 camera::PointV2 direction;float square{};for(unsigned i=0;i<3;++i){direction[i]=ray.end[i]-ray.start[i];square+=direction[i]*direction[i];}
 const auto length=std::sqrt(square);if(!std::isfinite(length)||length==0){e="Required actual nonzero source picking ray";return false;}
 for(auto&v:direction)v/=length;
 if(direction[2]==0){e="Original listener picking ray parallel to z0";return false;}
 const auto t=ray.start[2]/direction[2];for(unsigned i=0;i<3;++i)out[i]=ray.start[i]-t*direction[i];return true;
}
}
bool AudioLevelGameplayV67::update_listener(std::string&error){
 if(!current(error))return false;
 auto&cached=bridge_->manager().manager->listener_row_on_producer();
 if(!cached){const auto index=level_->constructor_fields_v3().phase_e4;if(index>=rows_.size()){error="Required actual first Level.e4 Listener row";return false;}cached=rows_[index];}
 const auto&row=*cached;camera::PointV2 position{},front{},up{},camera_front{},camera_up{};
 const auto session=bindings_.camera->world();const auto runtime=session&&session->camera?session->camera->level():nullptr;
 const bool camera_present=runtime&&runtime->loaded()&&runtime->scene();
 camera::CameraViewV11 view;
 if(camera_present&&(!bindings_.camera->view(view,error)||!runtime->source_node_vectors_v67(camera_front,camera_up,error)))return false;
 auto character=[this](std::uintptr_t&out,std::string&e){return bindings_.local_character0(out,e);};
 auto picking=[this,&view](AudioListenerDriverV67&driver,camera::GameplayCameraPickingV20&out,std::string&e){
  if(!bindings_.driver(driver,e))return false;driver.picking.camera=view;return out.bind(driver.picking,e);
 };
 if(row.anchor==0){if(camera_present)position=view.eye;}
 else if(row.anchor==1||row.anchor==2){std::uintptr_t actor{};if(!character(actor,error))return false;
  if(actor){if(!character(actor,error)||!actor||!bindings_.target_position(actor,position,error))return false;
   if(row.anchor==1){AudioListenerDriverV67 driver;camera::GameplayCameraPickingV20 projection;std::array<std::int32_t,2>pixel;
    if(!picking(driver,projection,error)||!projection.screen_pixels(position,pixel,error)||!world_pixel(projection,float(pixel[0]),float(driver.viewport_bottom),position,error))return false;}}
 }else if(row.anchor==3){AudioListenerDriverV67 driver;camera::GameplayCameraPickingV20 projection;
  if(!picking(driver,projection,error)||!world_pixel(projection,float(driver.screen_width/2),float(driver.screen_height/2),position,error))return false;}
 if(row.orientation==0||row.orientation==2){if(camera_present){front=camera_front;if(row.orientation==2)front[2]=0;}}
 else if(row.orientation==1){std::uintptr_t actor{};if(!character(actor,error))return false;
  if(actor&&(!character(actor,error)||!actor||!bindings_.look_at(actor,front,error)))return false;}
 if(row.up_vector==0){if(camera_present)up=camera_up;}
 else if(row.up_vector==1){std::uintptr_t actor{};if(!character(actor,error))return false;
  if(actor&&(!character(actor,error)||!actor||!bindings_.visual_up(actor,up,error)))return false;}
 // Original missing node8 skips SetListenerPos, preserving its prior fields.
 if(camera_present){VoxListenerAuthorityV38 next;
  if(!audio_listener_update_v38(row,position.data(),front.data(),up.data(),next,error))return false;
  bridge_->manager().manager->listener_authority_on_producer()=next;}
 error.clear();return true;
}
bool AudioLevelGameplayV67::listener_vectors(const AudioListenerRowV38&,float*p,float*f,float*u,std::string&error){
 if(!current(error))return false;
 const auto&listener=bridge_->manager().manager->listener_authority_on_producer();
 if(!listener){error="Required actual reached SetListenerPos on SAME camera node";return false;}
 std::copy_n(listener->listener.position,3,p);std::copy_n(listener->listener.front,3,f);std::copy_n(listener->listener.up,3,u);error.clear();return true;
}
bool AudioLevelGameplayV67::borrow_aggro_level_v101(character::PlayerAggroLevelBorrowV1&out,std::string&error){
 if(!current(error))return false;
 const auto fields=level_->config_fields();out={level_->identity(),fields.config38,nullptr,fields.music11c};
 if(!*fields.config38){error.clear();return true;} // caller owns original NULL-config assertion
 const auto*config=bindings_.config(*fields.config38);
 if(!config||config->identity()!=*fields.config38){error="Required actual registered LevelConfig38 for music producer";return false;}
 const auto*enabled=config->byte(0x1c8);if(!enabled){error="Required actual canonical LevelConfig music byte1c8";return false;}
 out={level_->identity(),fields.config38,enabled,fields.music11c};error.clear();return true;
}
bool AudioLevelGameplayV67::release_source_providers_v101(const std::shared_ptr<void>&world,std::string&error){
 if(!producer(error))return false;
 if(released_v101_){error.clear();return true;}
 if(!world||!bridge_||bridge_->world()!=world){error="Foreign Level audio provider release";return false;}
 if(!dh2::android_audio::unpublish_actual_playback_v101(bridge_,error))return false;
 released_v101_=true;bridge_.reset();bindings_={};level_.reset();rows_.clear();error.clear();return true;
}
bool AudioLevelGameplayV67::stop_sound_v106(int sound,int fade,std::string&error){
 return stop_3d_v112(sound,fade,nullptr,0.f,error);
}
bool AudioLevelGameplayV67::stop_3d_v112(int sound,int fade,const float* center,float radius,std::string&error){
 if(sound<0){error.clear();return true;}
 if(!producer(error)||released_v101_||!bridge_){if(error.empty())error="Released same-world per-source Stop provider";return false;}
 // D2 runs after current-Level globals may have been cleared: no current()
 // borrower or gameplay selector is added to the source Stop operation.
 return bridge_->manager().manager->stop_3d_v112(sound,fade,center,radius,bindings_.platform_stop,error);
}
bool AudioLevelGameplayV67::detach_captured_providers_v102(const std::shared_ptr<AudioCampaignBridgeV46>&expected,std::string&error){
 if(!producer(error))return false;
 if(released_v101_||!bridge_){error.clear();return true;}
 if(!expected||bridge_.get()!=expected.get()||bridge_.owner_before(expected)||expected.owner_before(bridge_)){
  error="Required exact captured Level audio provider identity";return false;
 }
 // Registry removal is performed separately against the captured V46 lease.
 // This only releases this old V67 owner and never borrows current Level/GS.
 released_v101_=true;bridge_.reset();bindings_={};level_.reset();rows_.clear();error.clear();return true;
}
}
