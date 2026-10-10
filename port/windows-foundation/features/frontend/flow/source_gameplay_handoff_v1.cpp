#include "source_gameplay_handoff_v1.hpp"

namespace dh::foundation::frontend::flow {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool SourceGameplayHandoffV1::publish(const SourceGameplayHandoffBindingsV1& b,
                                     SourceGameplayHandoffResultV1& result,
                                     std::string& error) {
    error.clear();
    if (published_) return fail(error, "Source gameplay handoff already published");
    if (b.runtime_result.outcome != frontend::FrontendRuntimeOutcomeV1::gameplay_handoff_ready ||
        !b.runtime_result.error.empty())
        return fail(error, "Frontend result is not a successful gameplay handoff");
    if (!b.expected_profile.valid() || b.expected_slot < 0 ||
        b.runtime_result.selected_slot != b.expected_slot ||
        !b.runtime_result.selected_profile.same_as(b.expected_profile))
        return fail(error, "Frontend handoff does not carry the expected canonical profile loan");
    if (!b.runtime_result.gameplay_ready())
        return fail(error, "Native start receipt slot/profile/assignment/start lease mismatch");
    if (!b.input_epoch || !b.gameplay_input || !b.old_menu.lease ||
        !b.old_menu.cancel_release || !b.old_gameplay.expected_actor ||
        !b.old_gameplay.release)
        return fail(error, "Required old menu/gameplay input owners and release callbacks");

    if (!b.input_epoch->validate(b.old_gameplay.borrow,
                                 b.old_gameplay.expected_actor, error))
        return false;

    if (menu_released_) {
        if (!input::same_input_owner_v1(released_menu_owner_, b.old_menu.lease))
            return fail(error, "Menu input owner changed after its release prefix");
    } else {
        // Retain the exact lease before invoking external code. If later
        // gameplay release fails, retry skips this completed menu prefix.
        released_menu_owner_ = b.old_menu.lease;
        if (!b.old_menu.cancel_release(released_menu_owner_, error)) {
            if (error.empty()) error = "Old menu input cancellation/release failed";
            return false;
        }
        menu_released_ = true;
    }

    const auto old_epoch = b.input_epoch->value();
    const auto old_owner = b.old_gameplay.borrow;
    const auto old_actor = b.old_gameplay.expected_actor;
    const auto release = b.old_gameplay.release;
    if (!b.input_epoch->before_replace(
            *b.gameplay_input,
            [&](const platform_input::Frame& frame, std::string& release_error) {
                if (b.input_epoch->value() != old_epoch ||
                    !b.input_epoch->validate(old_owner, old_actor, release_error))
                    return fail(release_error, "Old gameplay input frame became stale before release");
                return release(old_owner, frame, release_error);
            }, error))
        return false;

    result.selected_profile = b.runtime_result.selected_profile;
    result.input_epoch = b.input_epoch->value();
    published_ = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::frontend::flow
