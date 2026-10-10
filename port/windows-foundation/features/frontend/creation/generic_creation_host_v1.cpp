#include "generic_creation_host_v1.hpp"

#include <utility>

namespace dh::foundation::frontend::creation {
namespace {
bool same_owner(const std::shared_ptr<CharacterState>& a,
                const std::shared_ptr<CharacterState>& b) noexcept {
    return a && b && a.get() == b.get() &&
           !a.owner_before(b) && !b.owner_before(a);
}

GenericCreationFrontendResultV1 failed_result(
    std::shared_ptr<CharacterState> state, const char* reason) {
    GenericCreationFrontendResultV1 result;
    result.shared_state = std::move(state);
    result.frontend.outcome = FrontendRuntimeOutcomeV1::host_failed;
    result.frontend.error = reason;
    return result;
}
} // namespace

GenericCreationFrontendHostV1::GenericCreationFrontendHostV1(
    FrontendRuntimeServicesV1 base_services,
    RuntimeCreationFlowServicesV1 creation_services)
    : shared_state_(creation_services.shared_state),
      adapter_(std::make_shared<RuntimeCreationFlowAdapterV1>(
          std::move(creation_services))),
      runtime_services_(std::move(base_services)) {
    runtime_services_.generic_creation = adapter_;
}

bool GenericFrontendLaunchPlanV1::same_state_as(
    const std::shared_ptr<CharacterState>& expected) const noexcept {
    return same_owner(shared_state, expected);
}

bool GenericFrontendLaunchPlanV1::valid_for(
    const std::shared_ptr<CharacterState>& expected, int expected_slot) const noexcept {
    return start_delivered && selected_slot >= 0 && selected_slot == expected_slot &&
           !save_path.empty() && same_state_as(expected);
}

bool GenericCreationFrontendResultV1::gameplay_started_for(
    const std::shared_ptr<CharacterState>& expected, int expected_slot) const noexcept {
    return frontend.generic_gameplay_ready() && launch &&
           launch->valid_for(expected, expected_slot) &&
           same_owner(shared_state, expected) &&
           same_owner(launch->shared_state, shared_state);
}

GenericCreationFrontendResultV1 GenericCreationFrontendHostV1::complete(
    FrontendRuntimeResultV1 frontend) const {
    GenericCreationFrontendResultV1 result;
    result.shared_state = shared_state_;
    result.frontend = std::move(frontend);
    if (!shared_state_)
        return failed_result({}, "Generic frontend requires the caller's existing CharacterState owner");

    if (result.frontend.outcome != FrontendRuntimeOutcomeV1::generic_gameplay_started ||
        !result.frontend.generic_start_receipt)
        return result;

    const auto& receipt = *result.frontend.generic_start_receipt;
    if (!receipt.valid_for(shared_state_, result.frontend.selected_slot)) {
        result.frontend.outcome = FrontendRuntimeOutcomeV1::source_operation_failed;
        result.frontend.error =
            "Generic frontend returned a start receipt for a different state owner or selected slot";
        result.frontend.generic_start_receipt.reset();
        return result;
    }

    result.launch = GenericFrontendLaunchPlanV1{
        receipt.shared_state,
        receipt.save_path,
        receipt.selected_slot,
        receipt.created_in_this_flow,
        receipt.start_delivered};
    if (!result.gameplay_started_for(shared_state_, result.frontend.selected_slot)) {
        result.frontend.outcome = FrontendRuntimeOutcomeV1::source_operation_failed;
        result.frontend.error = "Generic frontend launch plan failed state/slot identity validation";
        result.launch.reset();
    }
    return result;
}

} // namespace dh::foundation::frontend::creation
