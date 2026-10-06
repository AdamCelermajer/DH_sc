#pragma once
#include "gameplay_camera_scene_v3.hpp"
#include "../engine-animation/animation_registration.hpp"
#include "visual_timeline.hpp"
#include "../game-data/animation_tables.hpp"
#include <map>
#include <functional>
namespace dh2::camera {
struct CameraAnimationResourceV10 {
 std::vector<std::uint8_t> bytes;resources::BresView bres{};animation::Player player;
 std::vector<std::array<std::int32_t,2>> ranges;
};
struct CameraAnimationSetV10 {
 animation::RegistrationSet registration;
 std::map<std::int32_t,std::shared_ptr<CameraAnimationResourceV10>> by_id;
 bool skip_load3c{},ready{},failed{};std::string failure;
};
struct CameraAnimationManagerServicesV10 {
 std::shared_ptr<void> application_lease;
 const data::Dictionary* dictionary{};
 std::function<bool(std::uint8_t&,std::string&)> actual_lg_devices;
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)> read;
 // Original AddAnim's Debug.load/GetSwitch after actual LoadAnimation;
 // return is ignored by source, but callback delivery must be genuine.
 std::function<bool(std::string&)> debug_after_add;
};
// Source singleton manager's camera domain. Retain once in Application and
// share this same directory with future Character registration successors;
// do not construct a new manager per World/GL restore or clone live actor banks.
class GameplayCameraAnimationManagerV10 {
 CameraAnimationManagerServicesV10 services_;
 std::int32_t current1c_{-1};
 std::map<std::int32_t,std::shared_ptr<CameraAnimationSetV10>> sets_;
 std::map<std::string,std::shared_ptr<CameraAnimationResourceV10>> resources_;
 bool load(CameraAnimationSetV10&,std::int32_t,const scene::Scene&,std::shared_ptr<CameraAnimationResourceV10>&,std::string&);
public:
 explicit GameplayCameraAnimationManagerV10(CameraAnimationManagerServicesV10 s):services_(std::move(s)){}
 bool create(std::int32_t&,std::string&);
 bool register_camera(std::int32_t,const data::CameraAnimationSet&,const scene::Scene&,std::string&);
 std::shared_ptr<CameraAnimationSetV10> find(std::int32_t)const;
 std::int32_t current_source_key()const noexcept{return current1c_;}
 // Original475664: erase registered sets then counter1c=-1. Canonical
 // resource directory models the separate scene asset cache and survives.
 void source_flush_v17()noexcept{sets_.clear();current1c_=-1;}
};
// Actual single AnimatorSet over the same camera scene; no Character scheduler,
// AI/FSM, synthetic event22 or second pose scene. Clip indexc is an animation
// segment index, not a repeat count (source animator virtual30=setAnimationClip).
class GameplayCameraAnimatorV10 {
 std::shared_ptr<GameplayCameraSceneV3> scene_;
 std::shared_ptr<CameraAnimationSetV10> set_;
 animation::TransformSet compiled_;
 std::vector<std::array<float,4>> values_;std::vector<std::uint8_t> initialized_;
 std::vector<std::int32_t> cursors_;
 std::int32_t engine_index_{};std::uint32_t root_timestamp_{};
 timeline::Completion completion_{};
 std::function<void()> completion_callback_;
 bool ready_{};
 bool pose(std::string&);
public:
 timeline::State timeline{};
 std::int32_t clip_indexc{};std::uint8_t byte10{1};
 GameplayCameraAnimatorV10(std::shared_ptr<GameplayCameraSceneV3>,std::shared_ptr<CameraAnimationSetV10>);
 bool initialize(std::string&);
 void set_source_completion(std::function<void()> callback){completion_callback_=std::move(callback);}
 bool play(std::int32_t resource,bool loop,bool& played,std::string&);
 bool scene_phase(std::uint32_t,std::string&);
 // Caller must first unregister this actual root animator; this invalidates
 // native callback borrows without fabricating a final animation event.
 void release_after_unregistration()noexcept{completion_callback_={};ready_=false;}
 bool ready()const noexcept{return ready_;}
 const std::shared_ptr<GameplayCameraSceneV3>& scene_borrow()const noexcept{return scene_;}
};
}
