#pragma once
#include "actor_blended_playback.hpp"
#include "character_npc_animation_set_v6.hpp"
#include "retained_gameobject_visual_v1.hpp"
#include <set>
namespace dh2::character {
struct CharacterAnimationSetRecordV6 {
 actor::ClipBank clips;animation::RegistrationSet registration;
 std::uint32_t source_users24_v107{}; //CDynamicAnimationSet C1 36491c.
 std::map<std::int32_t,std::string> paths;bool ready{},failed{};std::string error;
 std::set<std::int32_t> deferred_bindings_v116; //only resource tracks loaded before a render graph
};
// One actual native animation-set directory per World. Exists uses the same
// source integer set key, preserving first publication and failed prefixes.
class CharacterAnimationSetCacheV6 {
 std::map<std::int32_t,std::shared_ptr<CharacterAnimationSetRecordV6>> records_;
public:
 std::shared_ptr<CharacterAnimationSetRecordV6> find(std::int32_t id)const{auto i=records_.find(id);return i==records_.end()?nullptr:i->second;}
 std::shared_ptr<CharacterAnimationSetRecordV6> create(std::int32_t id){auto r=std::make_shared<CharacterAnimationSetRecordV6>();auto p=records_.emplace(id,r);return p.second?r:nullptr;}
 std::size_t size()const noexcept{return records_.size();}
 bool source_flush_v121(std::string& e){
  for(const auto& entry:records_)if(entry.second&&entry.second->source_users24_v107){
   e="Animation-set Flush requires actual Character user release";return false;
  }
  records_.clear();e.clear();return true;
 }
};
struct CharacterSameSceneAnimatorServicesV6 {
 std::shared_ptr<void> world;
 std::shared_ptr<CharacterAnimationSetCacheV6> same_set_manager;
 const data::Dictionary* animation_dictionary{};
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)> read;
 NpcAnimationRegistrationServicesV6 registration;
 std::function<bool(std::string&)> add_animation_trace_v116;
 std::function<bool(std::int32_t&,std::string&)> unique_set_id_v116;
 std::function<bool(const data::AnimationTables&,std::int32_t,std::int32_t,
  const std::vector<std::int32_t>&,NpcAnimationRegistrationServicesV6,std::string&)> register_set_v62;
 // SAME root animator attachment/removal, not a guessed scene flag.
 std::function<bool(std::function<bool(std::string&)>,std::string&)> attach;
 std::function<bool(std::string&)> pose_changed;
};
// Per-Character CharAnimator over the existing VisualObject scene. Source
// pose, skin, node handles and renderer consume that SAME Scene, never a copy.
class CharacterSameSceneAnimatorV6 {
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual_;
 CharacterSameSceneAnimatorServicesV6 services_;
 std::shared_ptr<CharacterAnimationSetRecordV6> set_;
 visual::SceneBinding* binding_{};actor::BlendedPlayback playback_;
 std::int32_t set_id_{-1};bool attempted_{},ready_{},failed_{},attached_{};
 std::uint8_t source_animating5c_v107_{}; //CharAnimator C1, SAME Character4f8.
 std::uint8_t source_dictionary_disabled38_v116_{}; //CharAnimator C1 3c8ff4 produces0.
 std::string failure_;
 std::shared_ptr<int> callback_lifetime_{std::make_shared<int>(0)};
 bool registration(const NpcAnimationRegistrationRequestV6&,std::string&);
 bool load(std::int32_t,const animation::Player*&,std::uintptr_t&,std::string&);
public:
 CharacterSameSceneAnimatorV6(std::shared_ptr<world::RetainedGameObjectVisualV1>,CharacterSameSceneAnimatorServicesV6);
 ~CharacterSameSceneAnimatorV6(){callback_lifetime_.reset();}
 CharacterSameSceneAnimatorV6(const CharacterSameSceneAnimatorV6&)=delete;
 bool initialize(const data::AnimationTables&,std::int32_t table,std::int32_t skill_list,const std::vector<std::int32_t>& skills,std::string&);
 bool start(const data::AnimationTables&,std::int32_t,data::AnimationRandom&,float,std::string&);
 bool scene_phase(std::uint32_t,std::string&);
 bool animator_phase(const data::AnimationTables&,data::AnimationRandom&,float,std::string&);
 bool detach(std::string&);
 bool source_add_set_to_render_v109(std::shared_ptr<world::RetainedGameObjectVisualV1>,std::string&);
 bool source_add_animation_dict_v116(std::int32_t,std::string&);
 bool bind_visual_v116(std::shared_ptr<world::RetainedGameObjectVisualV1>,
  std::function<bool(std::function<bool(std::string&)>,std::string&)>,
  std::function<bool(std::string&)>,std::string&);
 bool source_dec_anim_set_users_v107(std::string&);
 bool source_update_user_state_v107(std::uint32_t actual_flags520,bool&,std::string&);
 bool source_has_active_users_v107()const noexcept{return source_animating5c_v107_!=0;}
 actor::BlendedPlayback& playback()noexcept{return playback_;}
 visual::SceneBinding& binding()noexcept{return *binding_;}
 const actor::ClipBank& clips()const noexcept{return set_->clips;}
 const animation::RegistrationSet& registrations()const noexcept{return set_->registration;}
 std::int32_t set_id()const noexcept{return set_id_;}
 bool ready()const noexcept{return ready_;}
 const std::shared_ptr<world::RetainedGameObjectVisualV1>& animation_visual_v112()const noexcept{return visual_;}
};
}
