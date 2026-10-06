#pragma once
#include "actor_blended_playback.hpp"
#include "../game-data/animation_bank.hpp"
#include <functional>
#include <memory>

namespace dh2::character {
// Port ownership around the recovered registration/compiler/playback kernels.
// This is not an original Character factory, AI frame or physics producer.
class CharacterAnimationResources final {
 data::AnimationBank metadata_;
 scene::Scene factory_;
 actor::ClipBank clips_;
 CharacterAnimationResources()=default;
public:
 using Reader=std::function<bool(const data::AnimationBankResource&,
                               std::vector<std::uint8_t>&,std::string&)>;
 // Reader supplies the exact resource bytes identified by the packaged bank.
 // Size and SHA256 are checked against the exact packaged resource descriptor.
 // Failure preserves output; resources own all Player bytes and factory nodes.
 static bool load(data::Bytes,const scene::Scene&,const Reader&,
                  std::shared_ptr<const CharacterAnimationResources>&,std::string&);
 CharacterAnimationResources(const CharacterAnimationResources&)=delete;
 CharacterAnimationResources& operator=(const CharacterAnimationResources&)=delete;
 CharacterAnimationResources(CharacterAnimationResources&&)=delete;
 CharacterAnimationResources& operator=(CharacterAnimationResources&&)=delete;
 const data::AnimationBank& metadata()const noexcept{return metadata_;}
 const scene::Scene& factory()const noexcept{return factory_;}
 const actor::ClipBank& clips()const noexcept{return clips_;}
};

// Every Character owns its pose, binding, scheduler, slots, event/root history.
// Immutable CPU resources remain pinned through GL recreation and callbacks.
// The caller owns genuine AI/FSM/event delivery and shared Scene/physics order.
class CharacterAnimationInstance final {
 std::shared_ptr<const CharacterAnimationResources> resources_;
 scene::Scene scene_;
 visual::SceneBinding visual_;
 actor::BlendedPlayback playback_;
 explicit CharacterAnimationInstance(std::shared_ptr<const CharacterAnimationResources>);
public:
 static std::unique_ptr<CharacterAnimationInstance> create(
  std::shared_ptr<const CharacterAnimationResources>,std::string&);
 CharacterAnimationInstance(const CharacterAnimationInstance&)=delete;
 CharacterAnimationInstance& operator=(const CharacterAnimationInstance&)=delete;
 CharacterAnimationInstance(CharacterAnimationInstance&&)=delete;
 CharacterAnimationInstance& operator=(CharacterAnimationInstance&&)=delete;
 const CharacterAnimationResources& resources()const noexcept{return *resources_;}
 scene::Scene& scene()noexcept{return scene_;}
 const scene::Scene& scene()const noexcept{return scene_;}
 visual::SceneBinding& visual()noexcept{return visual_;}
 actor::BlendedPlayback& playback()noexcept{return playback_;}
 bool start(const data::AnimationTables&,int sequence,data::AnimationRandom&,float,std::string&);
 bool scene_phase(std::uint32_t timestamp,std::string&);
 bool animator_phase(const data::AnimationTables&,data::AnimationRandom&,float,std::string&);
};
}
