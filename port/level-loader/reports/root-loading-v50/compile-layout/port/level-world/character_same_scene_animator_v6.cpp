#include "character_same_scene_animator_v6.hpp"
namespace dh2::character {
CharacterSameSceneAnimatorV6::CharacterSameSceneAnimatorV6(std::shared_ptr<world::RetainedGameObjectVisualV1> visual,CharacterSameSceneAnimatorServicesV6 services):visual_(std::move(visual)),services_(std::move(services)){}
bool CharacterSameSceneAnimatorV6::load(std::int32_t id,const animation::Player*& player,std::uintptr_t& identity,std::string& error){
 if(id<0||!services_.animation_dictionary||std::size_t(id)>=services_.animation_dictionary->values.size()){error="Required original animation dictionary resource "+std::to_string(id);return false;}
 auto existing=set_->clips.find(id);if(existing!=set_->clips.end()){player=&existing->second;identity=reinterpret_cast<std::uintptr_t>(player);return true;}
 const auto& path=services_.animation_dictionary->values[id];
 for(const auto& entry:set_->paths)if(entry.second==path&&entry.first!=id){error="Required shared CCDB alias animation dictionary binding";return false;}
 std::vector<std::uint8_t> bytes;if(!services_.read||!services_.read(path,bytes,error))return false;
 animation::Player candidate;if(!candidate.load(bytes.data(),bytes.size(),visual_->scene(),error,animation::MissingTargets::ignore))return false;
 auto result=set_->clips.emplace(id,std::move(candidate));set_->paths.emplace(id,path);player=&result.first->second;identity=reinterpret_cast<std::uintptr_t>(player);return true;
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
 set_id_=static_cast<std::int32_t>((static_cast<std::uint32_t>(table)<<8)|static_cast<std::uint32_t>(skill_list));
 set_=services_.same_set_manager->find(set_id_);
 if(!set_){
  set_=services_.same_set_manager->create(set_id_);if(!set_){failed_=true;error="Source AnimationSet insertion reentry";return false;}
  NpcAnimationRegistrationServicesV6 callbacks;callbacks.sound_manager_present=services_.registration.sound_manager_present;callbacks.invoke=[this](const auto& q,auto& e){return registration(q,e);};
  if(!character_npc_register_animation_set_v6(tables,table,set_id_,skills,std::move(callbacks),error)){failed_=true;set_->failed=true;set_->error=error;failure_=error;return false;}
  if(!set_->registration.default_player()){failed_=true;set_->failed=true;error="Required original registered template/default library";set_->error=error;return false;}
  set_->registration.refresh_indices();set_->ready=true;
 }else if(!set_->ready||set_->failed){failed_=true;error="Retained failed/in-progress source AnimationSet prefix: "+set_->error;return false;}
 binding_=&visual_->binding().character_binding_v6();
 if(!binding_->bind(visual_->scene(),error)||!playback_.compile_dynamic(set_->clips,set_->registration,visual_->scene(),*binding_,error)){failed_=true;failure_=error;return false;}
 std::weak_ptr<int> lifetime=callback_lifetime_;
 if(!services_.attach([this,lifetime](std::string& e){if(lifetime.expired()){e="Character animator released before SAME root detachment";return false;}return detach(e);},error)){failed_=true;failure_=error;return false;}
 attached_=true;ready_=true;return true;
}
bool CharacterSameSceneAnimatorV6::start(const data::AnimationTables& tables,std::int32_t sequence,data::AnimationRandom& random,float speed,std::string& error){
 if(!ready_){error="Required ready SAME scene CharAnimator";return false;}if(!playback_.observer.invoke||!playback_.selection_fx_v2.invoke){error="Required SAME Character event and authored-step FX services";return false;}if(!playback_.start(tables,sequence,random,set_->clips,*binding_,visual_->scene(),speed,error))return false;return services_.pose_changed(error);
}
bool CharacterSameSceneAnimatorV6::scene_phase(std::uint32_t stamp,std::string& error){if(!ready_){error="Required active SAME scene CharAnimator";return false;}if(!playback_.scene_phase(stamp,set_->clips,*binding_,visual_->scene(),error))return false;return services_.pose_changed(error);}
bool CharacterSameSceneAnimatorV6::animator_phase(const data::AnimationTables& tables,data::AnimationRandom& random,float speed,std::string& error){if(!ready_){error="Required active SAME scene CharAnimator";return false;}return playback_.animator_phase(tables,random,set_->clips,*binding_,visual_->scene(),speed,playback_.completion.extra_ms,error);}
bool CharacterSameSceneAnimatorV6::detach(std::string& error){error.clear();if(!attached_)return true;if(!playback_.stop_immediate_v1(true,error))return false;attached_=false;ready_=false;return true;}
}
