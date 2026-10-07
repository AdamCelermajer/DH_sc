#pragma once
#include "source_campaign_object_update_actor_v104.hpp"
#include <canonical_object_lifecycle_v1.hpp>
namespace dh2::loader {struct LevelGameplayServicesV66;}
namespace model_renderer {
// Native selected-receiver leaves. The coordinator itself is the recovered
// ObjectManager.Update body; these are concrete class/lifetime endpoints.
struct SourceObjectUpdateLeavesV105 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uintptr_t,std::string&)> character_frame;
 std::function<bool(std::uintptr_t,std::string&)> room_frame;
 std::function<bool(std::uintptr_t,std::uint32_t,std::string&)> noncharacter_virtual;
 std::function<bool(const dh2::world::ObjectUpdateActorV102&,bool&,std::string&)> noncharacter_remote54;
 std::function<bool(const dh2::world::ObjectUpdateActorV102&,bool&,std::string&)> matching_deferred;
 std::function<bool(const dh2::world::ObjectUpdateActorV102&,std::string&)> fake_remove;
 std::function<bool(const dh2::world::CanonicalObjectBorrowV1&,std::string&)> character_clean;
 dh2::world::CanonicalObjectLifecycleV1 lifecycle;
};
// Assigns only update_objects, preserving physics/scene/camera/audio ordering.
// Does not run a frame, initialize actors or borrow gameplay phase38.
bool compose_source_campaign_object_update_v105(const SourceCampaignCandidateBorrowV55&,
 SourceObjectUpdateLeavesV105,dh2::loader::LevelGameplayServicesV66&,std::string&);
bool source_campaign_object_test_condition_v105(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,bool disable,bool mark,std::string&);
}
