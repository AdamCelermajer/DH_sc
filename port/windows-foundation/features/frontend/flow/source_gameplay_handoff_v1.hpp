#pragma once

#include "../frontend_runtime_v1.hpp"
#include "../input/input_epoch_frame_v1.hpp"
#include "../../platform_input/source_owner_contract.hpp"

namespace dh::foundation::frontend::flow {

struct SourceMenuInputOwnerV1 {
    std::shared_ptr<void> lease;
    // Must synchronously cancel captures and deliver the source menu's release
    // edges to this same owner before it can be unpublished.
    std::function<bool(const std::shared_ptr<void>&, std::string&)> cancel_release;
};

struct SourceGameplayInputOwnerV1 {
    platform_input::SourceOwnerBorrow borrow;
    std::uint64_t expected_actor{};
    // Delivers release/EndSkill to the exact old gameplay owner.
    std::function<bool(const platform_input::SourceOwnerBorrow&,
                       const platform_input::Frame&, std::string&)> release;
};

struct SourceGameplayHandoffBindingsV1 {
    frontend::FrontendRuntimeResultV1 runtime_result;
    frontend::FrontendProfileLoanV1 expected_profile;
    int expected_slot{-1};
    platform_input::InputOwnerEpoch* input_epoch{};
    platform_input::SemanticInput* gameplay_input{};
    SourceMenuInputOwnerV1 old_menu;
    SourceGameplayInputOwnerV1 old_gameplay;
};

struct SourceGameplayHandoffResultV1 {
    frontend::FrontendProfileLoanV1 selected_profile;
    std::uint64_t input_epoch{};
};

// One-shot, ordered publication. The runtime result must be an authored quit
// result carrying the same fully initialized canonical record. This seam does
// not create or retain a second Character, Save, profile, or controller.
class SourceGameplayHandoffV1 final {
public:
    bool publish(const SourceGameplayHandoffBindingsV1&,
                 SourceGameplayHandoffResultV1&, std::string& error);

private:
    bool menu_released_{};
    std::shared_ptr<void> released_menu_owner_;
    bool published_{};
};

} // namespace dh::foundation::frontend::flow
