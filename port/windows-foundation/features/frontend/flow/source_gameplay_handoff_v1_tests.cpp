#include "source_gameplay_handoff_v1.hpp"
#include "../../../../level-world/canonical_character_candidate_v60.hpp"
#include "../../../../level-world/canonical_character_save_v86.hpp"
#include "../../../../level-world/canonical_character_spawn_select_v87.hpp"
#include "../../../../level-world/character_ai_groups_v87.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::frontend;
using namespace dh::foundation::frontend::flow;

namespace {
void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

FrontendRuntimeResultV1 ready_result(const FrontendProfileLoanV1& profile,
                                     int slot,
                                     const std::shared_ptr<const void>& assign,
                                     const std::shared_ptr<const void>& start) {
    FrontendRuntimeResultV1 result;
    result.outcome = FrontendRuntimeOutcomeV1::gameplay_handoff_ready;
    result.selected_slot = slot;
    result.selected_profile = profile;
    result.source_receipt = FrontendSourceStartReceiptV1{profile, slot, assign, start};
    return result;
}

std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> test_record() {
    // This uninitialized, process-lifetime record is only a readiness-flag
    // fixture. A no-op deleter avoids pretending the test ran native teardown.
    static auto* raw = new dh2::world::CanonicalCharacterCandidateRecordV60();
    static std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> lease(
        raw, [](dh2::world::CanonicalCharacterCandidateRecordV60*) {});
    return lease;
}

struct Fixture {
    FrontendProfileLoanV1 profile;
    std::shared_ptr<int> menu_token{std::make_shared<int>(1)};
    std::shared_ptr<int> gameplay_token{std::make_shared<int>(2)};
    std::shared_ptr<const void> assign{std::make_shared<int>(3)};
    std::shared_ptr<const void> start{std::make_shared<int>(4)};
    platform_input::InputOwnerEpoch epoch;
    platform_input::SemanticInput gameplay_input;
    SourceGameplayHandoffBindingsV1 bindings;
    std::vector<std::string> order;
    unsigned menu_calls{}, gameplay_calls{};

    Fixture() {
        profile.record = test_record();
        profile.record->init_complete = true;
        bindings.runtime_result = ready_result(profile, 2, assign, start);
        bindings.expected_profile = profile;
        bindings.expected_slot = 2;
        bindings.input_epoch = &epoch;
        bindings.gameplay_input = &gameplay_input;
        bindings.old_menu.lease = menu_token;
        bindings.old_menu.cancel_release = [this](const std::shared_ptr<void>& owner,
                                                   std::string&) {
            ++menu_calls;
            check(input::same_input_owner_v1(owner, menu_token), "Menu release got a different owner");
            order.push_back("menu");
            return true;
        };
        bindings.old_gameplay.borrow.receiver_lease = gameplay_token;
        bindings.old_gameplay.borrow.epoch = epoch.value();
        bindings.old_gameplay.borrow.actor_id = 77;
        bindings.old_gameplay.borrow.character = 0x1234;
        bindings.old_gameplay.borrow.controller = 0x5678;
        bindings.old_gameplay.expected_actor = 77;
        bindings.old_gameplay.release = [this](const platform_input::SourceOwnerBorrow& owner,
                                               const platform_input::Frame& frame,
                                               std::string& error) {
            ++gameplay_calls;
            check(input::same_input_owner_v1(owner.receiver_lease, gameplay_token),
                  "Gameplay release got a different owner");
            check(epoch.validate(owner, 77, error), "Old gameplay owner expired before release");
            order.push_back("gameplay");
            return frame.attack.released && !frame.attack.held;
        };
    }
};
}

int main() {
    try {
        std::string error;
        Fixture fixture;
        fixture.gameplay_input.key(0x20, true);
        (void)fixture.gameplay_input.take_frame();

        SourceGameplayHandoffV1 handoff;
        SourceGameplayHandoffResultV1 result;
        check(handoff.publish(fixture.bindings, result, error), error.c_str());
        check(fixture.order == std::vector<std::string>({"menu", "gameplay"}),
              "Menu/gameplay release order changed");
        check(result.selected_profile.same_as(fixture.profile),
              "Handoff copied or changed the canonical profile loan");
        check(result.input_epoch == 2 && fixture.epoch.value() == 2,
              "Input epoch did not advance after both old owners released");
        check(!fixture.epoch.validate(fixture.bindings.old_gameplay.borrow, 77, error),
              "Old gameplay owner remained valid after publication");

        input::InputEpochFrameV1<platform_input::Frame> queued;
        queued.epoch = 1;
        queued.owner_lease = fixture.gameplay_token;
        check(!input::validate_input_epoch_frame_v1(
                  queued, result.input_epoch, fixture.gameplay_token, error),
              "Stale queued frame crossed the input epoch");
        queued.epoch = result.input_epoch;
        check(input::validate_input_epoch_frame_v1(
                  queued, result.input_epoch, fixture.gameplay_token, error),
              "Current queued frame rejected");
        auto alias_owner = std::shared_ptr<void>(std::make_shared<int>(9),
                                                 fixture.gameplay_token.get());
        queued.owner_lease = alias_owner;
        check(!input::validate_input_epoch_frame_v1(
                  queued, result.input_epoch, fixture.gameplay_token, error),
              "Same address with a different shared-control-block owner was accepted");

        Fixture retry;
        retry.gameplay_input.key(0x20, true);
        (void)retry.gameplay_input.take_frame();
        unsigned attempts{};
        retry.bindings.old_gameplay.release = [&](const platform_input::SourceOwnerBorrow&,
                                                   const platform_input::Frame& frame,
                                                   std::string&) {
            ++attempts;
            retry.order.push_back("gameplay");
            return attempts > 1 && frame.attack.released;
        };
        SourceGameplayHandoffV1 retry_handoff;
        check(!retry_handoff.publish(retry.bindings, result, error) && retry.epoch.value() == 1,
              "Failed gameplay release published a new epoch");
        check(retry.menu_calls == 1, "Old menu prefix was not delivered once");
        check(retry_handoff.publish(retry.bindings, result, error), error.c_str());
        check(retry.menu_calls == 1 && retry.epoch.value() == 2,
              "Retry replayed menu release or failed to publish epoch");

        Fixture incomplete;
        incomplete.profile.record->init_complete = false;
        incomplete.bindings.runtime_result = ready_result(
            incomplete.profile, 2, incomplete.assign, incomplete.start);
        SourceGameplayHandoffV1 blocked;
        check(!blocked.publish(incomplete.bindings, result, error) &&
                  incomplete.menu_calls == 0 && incomplete.epoch.value() == 1,
              "InitPost-incomplete profile reached input release/publication");
        incomplete.profile.record->init_complete = true;

        Fixture wrong_slot;
        wrong_slot.bindings.runtime_result = ready_result(
            wrong_slot.profile, 3, wrong_slot.assign, wrong_slot.start);
        SourceGameplayHandoffV1 slot_blocked;
        check(!slot_blocked.publish(wrong_slot.bindings, result, error) &&
                  wrong_slot.menu_calls == 0,
              "Mismatched native assignment/start slot was accepted");

        Fixture quit;
        quit.bindings.runtime_result.outcome = FrontendRuntimeOutcomeV1::quit;
        SourceGameplayHandoffV1 quit_blocked;
        check(!quit_blocked.publish(quit.bindings, result, error) && quit.menu_calls == 0,
              "Quit/back was confused with gameplay handoff");

        Fixture foreign_profile;
        auto foreign_record = std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(
            foreign_profile.profile.record.get(),
            [](dh2::world::CanonicalCharacterCandidateRecordV60*) {});
        foreign_profile.bindings.expected_profile.record = std::move(foreign_record);
        SourceGameplayHandoffV1 foreign_blocked;
        check(!foreign_blocked.publish(foreign_profile.bindings, result, error) &&
                  foreign_profile.menu_calls == 0,
              "Same record address with a different profile lease was accepted");

        std::cout << "PASS source gameplay handoff: same initialized canonical loan, matching source start receipt, release-before-epoch, stale-frame rejection, retry-safe prefix\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
