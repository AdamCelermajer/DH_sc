#pragma once
#include "../../../level-world/canonical_object_manager_v1.hpp"
#include "../../../level-world/openable_container_interaction_v2.hpp"
#include <array>
#include <functional>
#include <map>

namespace dh::foundation::interactions {
enum class Outcome { blocked, completed, failed, unsupported };
// These references must borrow the Character's real +408/+412 fields.
struct CharacterUseBorrow {
    std::uintptr_t owner{};
    const std::uintptr_t* object_of_interest{};
    const std::uintptr_t* ai_current_target{};
    std::uint8_t* use_requested{};
    std::function<bool(bool&,std::string&)> remotely_updated,idle,moving;
    std::function<bool(std::uintptr_t,bool,std::string&)> set_ai_target;
};
Outcome character_use(CharacterUseBorrow&,std::uintptr_t explicit_target,std::string&);
struct ControllerUseBorrow {
    const std::uint8_t* forced{};
    const std::uint8_t* locked{};
    const bool* globally_blocked{};
    std::function<bool(bool&,std::string&)> online;
    std::function<bool(std::uintptr_t,std::string&)> online_prefix;
    std::function<Outcome(std::uintptr_t,std::string&)> controllable;
};
// online_prefix must deliver the WHOLE original merchant / interaction-type /
// network action-5 prefix. Missing online services never silently skip messaging.
Outcome controller_use(const ControllerUseBorrow&,std::uintptr_t,std::string&);
struct RangeBorrow {
    std::uintptr_t owner{},current_target{};
    std::function<bool(std::uintptr_t,std::array<float,3>&,std::string&)> target_position,interaction_spot;
    std::function<bool(std::uintptr_t,bool&,std::string&)> has_interaction_node;
    std::function<bool(float&,std::string&)> melee_radius,interaction_padding;
    std::function<bool(std::uintptr_t,float&,std::string&)> target_radius;
    std::function<bool(std::uintptr_t,std::uintptr_t,int&,std::string&)> interaction_type;
};
bool in_interaction_range(const RangeBorrow&,std::uintptr_t explicit_target,bool&,std::string&);
// Dispatch metadata contains no receiver state. Publication / leases / types
// belong to the existing CanonicalObjectManager; caller supplies its live borrow.
class Router {
public:
    using Handler=std::function<bool(const dh2::world::CanonicalObjectBorrowV1&,std::uintptr_t,std::string&)>;
    bool bind(std::uint32_t source_type,Handler,std::string&);
    Outcome dispatch(const dh2::world::CanonicalObjectBorrowV1&,std::uintptr_t actor,std::string&) const;
private:
    std::map<std::uint32_t,Handler> handlers_;
};
// Delegates key consumption, campaign events, loot, scripts, sounds, animation
// and chest statistic changes to the existing source receiver, preserving order.
Outcome openable(dh2::world::OpenableContainerOwnerV1&,dh2::world::OpenableContainerFieldsV1&,
    const dh2::world::OpenableContainerInteractionServicesV2&,bool disabled,
    std::uintptr_t actor,std::string&);
}

