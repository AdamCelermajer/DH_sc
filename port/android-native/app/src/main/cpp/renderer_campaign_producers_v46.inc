// Audio-only closure factories; caller retains genuine source receiver/World.
std::function<bool(int,std::string&)> bind_positioned_audio_v46(dh2::audio::AudioCategoryV46 category,
 std::shared_ptr<void>world,std::uintptr_t subject,std::function<bool(std::array<float,3>&,std::string&)>position){
 return [category,weak=std::weak_ptr<void>(world),subject,position=std::move(position)](int id,std::string&error){
  auto world=weak.lock();if(!world){error="Released actual source sound World";return false;}
  dh2::audio::AudioApplicationBorrowV42 captured;
  if(!borrow_actual_application_audio_v42(captured,error))return false;
  if(!position){error="Required actual source audio position getter";return false;}
  std::array<float,3>point{};if(!position(point,error))return false;
  return submit_campaign_audio_v46(category,captured,world,dh2::audio::audio_world_request_v38(captured.identity(),subject,id,point),error);
 };
}
std::function<bool(std::string&)> bind_container_play_v46(std::shared_ptr<void>world,std::uintptr_t subject,
 std::function<bool(int&,std::string&)>get_sound,std::function<bool(std::array<float,3>&,std::string&)>position){
 return [weak=std::weak_ptr<void>(world),subject,get_sound=std::move(get_sound),position=std::move(position)](std::string&error){
  auto world=weak.lock();if(!world){error="Released actual source Container World";return false;}
  dh2::audio::AudioApplicationBorrowV42 captured;
  if(!borrow_actual_application_audio_v42(captured,error))return false;
  if(!get_sound||!position){error="Required actual Container GetSound/raw-position services";return false;}
  int id{};if(!get_sound(id,error))return false;
  std::array<float,3>point{};if(!position(point,error))return false;
  return submit_campaign_audio_v46(dh2::audio::AudioCategoryV46::container,captured,world,
   dh2::audio::audio_world_request_v38(captured.identity(),subject,id,point),error);
 };
}
namespace {
struct TargetAudioRelayV46 {
 dh2::character::TargetEventServices40 original;dh2::audio::AudioApplicationBorrowV42 manager;
 std::shared_ptr<void>world;std::string&error;
};
int target_audio_relay_v46(void*raw,dh2::character::TargetEventState32*state,
 const dh2::character::TargetEventRequest64*q,dh2::character::TargetEventResponse24*out){
 auto&relay=*static_cast<TargetAudioRelayV46*>(raw);
 if(!q)return -1;
 if(q->service!=dh2::character::target_event_play_sound)
  return relay.original.invoke?relay.original.invoke(relay.original.context,state,q,out):-1;
 dh2::character::CombatSoundPlayV1 play;play.manager=relay.manager.identity();play.target=q->subject;play.sound_id=q->sound;
 std::copy_n(q->position,3,play.position.begin());play.source_bool=q->flag!=0;play.source_integer=q->integer;
 play.source_float0=q->parameters[0];play.source_float1=q->parameters[1];
 return submit_campaign_audio_v46(dh2::audio::AudioCategoryV46::monster,relay.manager,relay.world,play,relay.error)?0:-1;
}
}
int dispatch_target_audio_v46(dh2::character::TargetEventState32*state,unsigned event,
 dh2::character::TargetEventServices40 services,std::shared_ptr<void>world,std::string&error){
 TargetAudioRelayV46 relay{services,{},std::move(world),error};
 if(!borrow_actual_application_audio_v42(relay.manager,error))return 2;
 services.context=&relay;services.invoke=target_audio_relay_v46;services.sound_manager=relay.manager.identity();
 return dh2_audio_target_event_v38(state,event,&services);
}
