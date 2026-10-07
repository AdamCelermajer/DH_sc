#pragma once
#include "retained_character_actor_v1.hpp"
#include "retained_character_visual_connection_v5.hpp"
#include "character_visual_asset_owner_v6.hpp"
#include "character_same_scene_animator_v6.hpp"
#include "character_live_body_projection_v6.hpp"
namespace dh2::character {
struct CharacterAnimationSelectionV6 {std::int32_t table{},skill_list{};std::vector<std::int32_t> skill_animations;};
struct CharacterFamilyVisualServicesV6 {
 std::shared_ptr<void> world;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> scene_manager;
 std::shared_ptr<CharacterAnimationSetCacheV6> animation_manager;
 world::RetainedGameObjectVisualServicesV1 visual;
 std::function<bool(std::string&)> update_pf;
 const data::Dictionary* animation_dictionary{};
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)> animation_read;
 NpcAnimationRegistrationServicesV6 registration;
 std::function<bool(std::string&)> add_animation_trace_v116;
 std::function<bool(const data::AnimationTables&,std::int32_t,std::int32_t,
  const std::vector<std::int32_t>&,NpcAnimationRegistrationServicesV6,std::string&)> register_set_v62;
 // Actual GetCharAnimTable/GetCharSkillList producer; no forced empty list.
 std::function<bool(RetainedCharacterActorV1&,CharacterAnimationSelectionV6&,std::string&)> animation_selection;
 actor::BlendedEventObserver events;
 actor::BlendedSelectionFxServicesV2 step_fx;
};
// Concrete family ownership capsule used at original inherited LoadVisual and
// Character SetAnimationSet calls. It borrows the same canonical actor only.
// Full inherited GameObject/Condition/Revive/AI initialization is not bypassed.
class RetainedCharacterFamilyVisualV6 {
 std::shared_ptr<RetainedCharacterActorV1> actor_;
 CharacterFamilyVisualServicesV6 services_;
 std::shared_ptr<world::RetainedCharacterVisualConnectionV5> connection_;
 std::unique_ptr<world::CharacterVisualAssetOwnerV6> assets_;
 std::unique_ptr<CharacterSameSceneAnimatorV6> animator_;
 bool bind_attempted_{},animation_attempted_{};std::string error_;
 bool bind(std::string&);
public:
 RetainedCharacterFamilyVisualV6(std::shared_ptr<RetainedCharacterActorV1>,CharacterFamilyVisualServicesV6);
 bool load_visual(std::string&);
 bool source_set_visual_v109(const char*,const char*,bool,std::string&);
 bool source_add_animation_set_v109(std::string&);
 bool sync_visual(std::uintptr_t,std::string&);
 bool sync_visual_position_v7(std::uintptr_t,std::string&);
 bool set_animation_set(const data::AnimationTables&,std::string&);
 bool body_projection(const physical::NpcBodyRequest&,physical::NpcBodyProjection&,std::string&);
 bool body_projection_v62(const physical::NpcBodyRequest&,physical::NpcBodyProjection&,std::string&);
 bool close(std::string&);
 bool source_set_visual_null_batch_v112(std::string&);
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual()const{return connection_?connection_->attached():nullptr;}
 std::shared_ptr<world::RetainedGameObjectVisualV1> animation_visual_v112()const{
  auto source=visual();if(source)return source;
  if(!animator_||!animator_->ready())return {};
  auto retained=animator_->animation_visual_v112();return retained&&retained->batch_animation_retained_v112()?retained:nullptr;
 }
 CharacterSameSceneAnimatorV6* animator()const noexcept{return animator_.get();}
};
}
