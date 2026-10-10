#include "character_same_scene_animator_v6.hpp"
namespace dh2::character {
CharacterSameSceneAnimatorV6::CharacterSameSceneAnimatorV6(std::shared_ptr<world::RetainedGameObjectVisualV1> visual,CharacterSameSceneAnimatorServicesV6 services):visual_(std::move(visual)),services_(std::move(services)){}
bool CharacterSameSceneAnimatorV6::source_add_animation_dict_v116(std::int32_t id,std::string& e){
 if(source_dictionary_disabled38_v116_){e.clear();return true;}
 if(set_id_==-1){
  if(!services_.unique_set_id_v116||!services_.unique_set_id_v116(set_id_,e))return false;
 }
 if(id<0){e.clear();return true;}
 if(!services_.same_set_manager){e="Required SAME source AnimationSet manager before AddAnim";return false;}
 if(!set_)set_=services_.same_set_manager->find(set_id_);
 if(!set_){
  if(set_id_<0){e.clear();return true;} //original AddAnim signed set-key exit
  set_=services_.same_set_manager->create(set_id_);if(!set_){e="Actual AnimationSet AddAnim insertion reentry";return false;}
 }
 if(set_->failed){e=set_->error;return false;}
 //Use the actual existing dictionary, clip bank and registration cells. The
 //native cache refresh preserves scheduler/timeline state and adds no tick.
 if(!registration({NpcAnimationRegistrationOperationV6::add_animation,set_id_,id},e))return false;
 if(!services_.add_animation_trace_v116){e="Required original AnimSetManager AddAnim Debug provider";return false;}
 if(!services_.add_animation_trace_v116(e))return false;
 set_->registration.refresh_indices();
 set_->ready=true; //completed actual CreateAnimSet/LoadAnimation prefix only
 if(!ready_){e.clear();return true;} //no render/controller exists before attachment
 return playback_.source_rebind_render_v109(set_->clips,set_->registration,visual_->scene(),*binding_,e);
}
bool CharacterSameSceneAnimatorV6::bind_visual_v116(std::shared_ptr<world::RetainedGameObjectVisualV1> visual,
 std::function<bool(std::function<bool(std::string&)>,std::string&)> attach,
 std::function<bool(std::string&)> changed,std::string& e){
 if(attempted_||visual_||!visual||!attach||!changed){e="Required first SAME source CharAnimator render attachment";return false;}
 visual_=std::move(visual);services_.attach=std::move(attach);services_.pose_changed=std::move(changed);e.clear();return true;
}
bool CharacterSameSceneAnimatorV6::load(std::int32_t id,const animation::Player*& player,std::uintptr_t& identity,std::string& error){
 if(id<0||!services_.animation_dictionary||std::size_t(id)>=services_.animation_dictionary->values.size()){error="Required original animation dictionary resource "+std::to_string(id);return false;}
 auto existing=set_->clips.find(id);if(existing!=set_->clips.end()){player=&existing->second;identity=reinterpret_cast<std::uintptr_t>(player);return true;}
 const auto& path=services_.animation_dictionary->values[id];
 for(const auto& entry:set_->paths)if(entry.second==path&&entry.first!=id){error="Required shared CCDB alias animation dictionary binding";return false;}
 std::vector<std::uint8_t> bytes;if(!services_.read||!services_.read(path,bytes,error))return false;
 animation::Player candidate;const scene::Scene no_render_binding;
 if(!candidate.load(bytes.data(),bytes.size(),visual_?visual_->scene():no_render_binding,error,animation::MissingTargets::ignore))return false;
 auto result=set_->clips.emplace(id,std::move(candidate));set_->paths.emplace(id,path);
 if(!visual_)set_->deferred_bindings_v116.insert(id);
 player=&result.first->second;identity=reinterpret_cast<std::uintptr_t>(player);return true;
}
bool CharacterSameSceneAnimatorV6::registration(const NpcAnimationRegistrationRequestV6& q,std::string& error){
 using O=NpcAnimationRegistrationOperationV6;
 if(q.operation!=O::add_template&&q.operation!=O::add_animation){if(!services_.registration.invoke){error="Required source animation Debug/Sound/FX preload";return false;}return services_.registration.invoke(q,error);}
 // Original manager LoadAnimation ignores negative dictionary IDs.
 if(q.resource<0)return true;
 const animation::Player* player=nullptr;std::uintptr_t identity=0;if(!load(q.resource,player,identity,error))return false;
 if(!set_->registration.append(q.resource,identity,player,error))return false;
 if(q.operation==O::add_template)return set_->registration.set_default(identity,player,error);
 return true;
}
bool CharacterSameSceneAnimatorV6::initialize(const data::AnimationTables& tables,std::int32_t table,std::int32_t skill_list,const std::vector<std::int32_t>& skills,std::string& error){
 error.clear();if(attempted_){error=failed_?"Failed source CharAnimator prefix cannot replay":"Source CharAnimator already constructed";return false;}attempted_=true;
 if(!visual_||!visual_->ready()||!visual_->root_identity()||!services_.world||!services_.same_set_manager||!services_.attach||!services_.pose_changed){failed_=true;error="Required SAME retained Character visual/animator services";return false;}
 const auto selected_set=static_cast<std::int32_t>((static_cast<std::uint32_t>(table)<<8)|static_cast<std::uint32_t>(skill_list));
 if(set_&&set_id_!=selected_set){failed_=true;error="Changed SAME source Character unique AnimationSet after early dictionary registration";return false;}
 set_id_=selected_set;
 set_=services_.same_set_manager->find(set_id_);
 if(!set_){
  set_=services_.same_set_manager->create(set_id_);if(!set_){failed_=true;error="Source AnimationSet insertion reentry";return false;}
  NpcAnimationRegistrationServicesV6 callbacks;callbacks.sound_manager_present=services_.registration.sound_manager_present;callbacks.invoke=[this](const auto& q,auto& e){return registration(q,e);};
  callbacks.current_sound_manager=services_.registration.current_sound_manager;
  const bool registered=services_.register_set_v62?
   services_.register_set_v62(tables,table,set_id_,skills,std::move(callbacks),error):
   character_npc_register_animation_set_v6(tables,table,set_id_,skills,std::move(callbacks),error);
  if(!registered){failed_=true;set_->failed=true;set_->error=error;failure_=error;return false;}
  if(!set_->registration.default_player()){failed_=true;set_->failed=true;error="Required original registered template/default library";set_->error=error;return false;}
  set_->registration.refresh_indices();set_->ready=true;
 }else if(!set_->ready||set_->failed){failed_=true;error="Retained failed/in-progress source AnimationSet prefix: "+set_->error;return false;}
 if(!set_->deferred_bindings_v116.empty()){
  //Bind resource tracks to the first actual render graph in-place. Player
  //addresses/registration identities and bytes stay on the original bank;
  //there was no rendered playback or clock before this source attachment.
  for(auto at=set_->deferred_bindings_v116.begin();at!=set_->deferred_bindings_v116.end();){
   auto found=set_->clips.find(*at);if(found==set_->clips.end()){failed_=true;error="Lost actual deferred animation library";return false;}
   auto& player=found->second;
   if(!player.bind_scene_v116(visual_->scene(),error,animation::MissingTargets::ignore)){failed_=true;set_->failed=true;set_->error=error;return false;}
   at=set_->deferred_bindings_v116.erase(at);
  }
 }
 binding_=&visual_->binding().character_binding_v6();
 if(!binding_->bind(visual_->scene(),error)||!playback_.compile_dynamic(set_->clips,set_->registration,visual_->scene(),*binding_,error)){failed_=true;failure_=error;return false;}
 std::weak_ptr<int> lifetime=callback_lifetime_;
 if(!services_.attach([this,lifetime](std::string& e){if(lifetime.expired()){e="Character animator released before SAME root detachment";return false;}return detach(e);},error)){failed_=true;failure_=error;return false;}
 attached_=true;ready_=true;
 // The original scene delivery and actor animator delivery are separate.
 // Register only the rendered-scene path here; animator_phase remains at
 // the Character update point after the genuine physical Step.
 if(!visual_->bind_character_scene_phase_v69(lifetime,[this,lifetime](std::uint32_t stamp,std::string& e){
   if(lifetime.expired()){e="Character scene animator owner expired";return false;}
   return scene_phase(stamp,e);
  },error)){failed_=true;failure_=error;return false;}
 return true;
}
bool CharacterSameSceneAnimatorV6::start(const data::AnimationTables& tables,std::int32_t sequence,data::AnimationRandom& random,float speed,std::string& error){
 if(!ready_){error="Required ready SAME scene CharAnimator";return false;}if(!playback_.observer.invoke||!playback_.selection_fx_v2.invoke){error="Required SAME Character event and authored-step FX services";return false;}if(!playback_.start(tables,sequence,random,set_->clips,*binding_,visual_->scene(),speed,error))return false;return services_.pose_changed(error);
}
bool CharacterSameSceneAnimatorV6::scene_phase(std::uint32_t stamp,std::string& error){if(!ready_){error="Required active SAME scene CharAnimator";return false;}if(!playback_.scene_phase(stamp,set_->clips,*binding_,visual_->scene(),error))return false;return services_.pose_changed(error);}
bool CharacterSameSceneAnimatorV6::animator_phase(const data::AnimationTables& tables,data::AnimationRandom& random,float speed,std::string& error){if(!ready_){error="Required active SAME scene CharAnimator";return false;}return playback_.animator_phase(tables,random,set_->clips,*binding_,visual_->scene(),speed,playback_.completion.extra_ms,error);}
bool CharacterSameSceneAnimatorV6::detach(std::string& error){error.clear();if(!attached_)return true;
 if(visual_&&visual_->batch_animation_retained_v112()){attached_=false;return true;} //Root reference moves; SAME Character4f8 still owns playback.
 // removeAnimators598658 invokes onUnbind(+2c), then drops the root loan.
 // AnimatorBlender.onUnbind366c8c clears render targets; it does not call
 // CharAnimator.ANIM_StopClip or deliver an animation-end event. Preserve the
 // Character-owned sequence while disabling this detached render endpoint.
 attached_=false;ready_=false;return true;}
bool CharacterSameSceneAnimatorV6::source_add_set_to_render_v109(std::shared_ptr<world::RetainedGameObjectVisualV1> visual,std::string& e){
 if(!visual){e.clear();return true;} // Original genuine NULL VisualObject exit.
 if(attached_||!set_||!set_->ready||set_->failed||!visual->ready()||!visual->root_identity()){
  e="Required SAME retained animation set after actual previous render detachment";return false;
 }
 visual_=std::move(visual);binding_=&visual_->binding().character_binding_v6();ready_=false;
 if(!binding_->bind(visual_->scene(),e)||!playback_.source_rebind_render_v109(set_->clips,set_->registration,visual_->scene(),*binding_,e))return false;
 const auto weak=std::weak_ptr<int>(callback_lifetime_);auto same=visual_;
 services_.attach=[same](auto remove,auto& e){return same->attach_character_animator_v6(std::move(remove),e);};
 services_.pose_changed=[same](auto& e){return same->character_pose_changed_v6(e);};
 if(!services_.attach([this,weak](auto& e){if(weak.expired()){e="Released SAME Character animator";return false;}return detach(e);},e))return false;
 attached_=true;
 if(!same->bind_character_scene_phase_v69(weak,[this,weak](auto stamp,auto& e){if(weak.expired()){e="Released SAME faery animator";return false;}return scene_phase(stamp,e);},e))return false;
 ready_=true;e.clear();return true;
}
bool CharacterSameSceneAnimatorV6::source_dec_anim_set_users_v107(std::string& error){
 //GetAnimationSet borrows the same registered dynamic set. Its temporary
 //reference grab/drop balances; this source24 counter is a distinct field.
 if(!visual_){error.clear();return true;}
 if(!visual_->root_identity()&&!visual_->batch_animation_retained_v112()){error="Required actual VisualObject or acquired compiled controller for GetAnimationSet";return false;}
 if(!set_){error="Required actual CharAnimator GetAnimationSet result";return false;}
 --set_->source_users24_v107;error.clear();return true;
}
bool CharacterSameSceneAnimatorV6::source_update_user_state_v107(std::uint32_t flags,bool& enabled,std::string& e){
 enabled=(flags&0x200u)!=0;
 if(!enabled){if(source_animating5c_v107_&&!source_dec_anim_set_users_v107(e))return false;source_animating5c_v107_=0;return true;}
 if(!source_animating5c_v107_){
  if(visual_){if(!set_){e="Required actual source GetAnimationSet for IncUsers";return false;}++set_->source_users24_v107;}
 }
 source_animating5c_v107_=1;e.clear();return true;
}
}
