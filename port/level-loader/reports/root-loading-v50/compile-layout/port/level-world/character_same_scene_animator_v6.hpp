#pragma once
#include "actor_blended_playback.hpp"
#include "character_npc_animation_set_v6.hpp"
#include "retained_gameobject_visual_v1.hpp"
namespace dh2::character {
struct CharacterAnimationSetRecordV6 {
 actor::ClipBank clips;animation::RegistrationSet registration;
 std::map<std::int32_t,std::string> paths;bool ready{},failed{};std::string error;
};
// One actual native animation-set directory per World. Exists uses the same
// source integer set key, preserving first publication and failed prefixes.
class CharacterAnimationSetCacheV6 {
 std::map<std::int32_t,std::shared_ptr<CharacterAnimationSetRecordV6>> records_;
public:
 std::shared_ptr<CharacterAnimationSetRecordV6> find(std::int32_t id)const{auto i=records_.find(id);return i==records_.end()?nullptr:i->second;}
 std::shared_ptr<CharacterAnimationSetRecordV6> create(std::int32_t id){auto r=std::make_shared<CharacterAnimationSetRecordV6>();auto p=records_.emplace(id,r);return p.second?r:nullptr;}
 std::size_t size()const noexcept{return records_.size();}
};
struct CharacterSameSceneAnimatorServicesV6 {
 std::shared_ptr<void> world;
 std::shared_ptr<CharacterAnimationSetCacheV6> same_set_manager;
 const data::Dictionary* animation_dictionary{};
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)> read;
 NpcAnimationRegistrationServicesV6 registration;
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
 actor::BlendedPlayback& playback()noexcept{return playback_;}
 visual::SceneBinding& binding()noexcept{return *binding_;}
 const actor::ClipBank& clips()const noexcept{return set_->clips;}
 const animation::RegistrationSet& registrations()const noexcept{return set_->registration;}
 std::int32_t set_id()const noexcept{return set_id_;}
 bool ready()const noexcept{return ready_;}
};
}
