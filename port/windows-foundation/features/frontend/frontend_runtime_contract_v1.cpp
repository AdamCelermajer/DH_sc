#include "frontend_runtime_v1.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"

namespace dh::foundation::frontend {

bool FrontendRuntimeResultV1::gameplay_ready() const noexcept {
    return outcome == FrontendRuntimeOutcomeV1::gameplay_handoff_ready &&
           selected_profile.valid() && source_receipt &&
           source_receipt->selected_slot == selected_slot &&
           source_receipt->selected_profile.same_as(selected_profile) &&
           source_receipt->source_assign_receipt &&
           source_receipt->source_start_receipt &&
           selected_profile.record->init_complete &&
           !selected_profile.record->failed;
}

} // namespace dh::foundation::frontend
