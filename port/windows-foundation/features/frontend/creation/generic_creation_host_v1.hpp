#pragma once

#include "runtime_creation_flow_adapter_v1.hpp"
#include "../frontend_runtime_v1.hpp"

#include <optional>

namespace dh::foundation::frontend::creation {

// Host-facing evidence for a generic source-backed start. This contract is
// intentionally separate from FrontendProfileLoanV1 and native action
// receipts: it names the same portable CharacterState owner and the selected
// save identity consumed by the generic frontend flow.
struct GenericFrontendLaunchPlanV1 {
    std::shared_ptr<CharacterState> shared_state;
    std::filesystem::path save_path;
    int selected_slot{-1};
    bool created_in_this_flow{};
    bool start_delivered{};

    bool same_state_as(const std::shared_ptr<CharacterState>& expected) const noexcept;
    bool valid_for(const std::shared_ptr<CharacterState>& expected,
                   int expected_slot) const noexcept;
};

struct GenericCreationFrontendResultV1 {
    FrontendRuntimeResultV1 frontend;
    // Always preserves the caller-supplied owner, including failure/quit
    // outcomes. The helper never replaces or allocates a second state owner.
    std::shared_ptr<CharacterState> shared_state;
    std::optional<GenericFrontendLaunchPlanV1> launch;

    bool gameplay_started_for(const std::shared_ptr<CharacterState>& expected,
                              int expected_slot) const noexcept;
};

// Keeps the generic adapter alive while the caller invokes the existing
// run_frontend_v1 with runtime_services(). Root supplies the real source
// data, slot/path, assignment, and same-state start providers. `complete`
// packages successful output only after the injected StartGame provider
// returns success and the runtime emits its generic start receipt.
class GenericCreationFrontendHostV1 final {
public:
    GenericCreationFrontendHostV1(FrontendRuntimeServicesV1 base_services,
                                  RuntimeCreationFlowServicesV1 creation_services);

    GenericCreationFrontendHostV1(const GenericCreationFrontendHostV1&) = delete;
    GenericCreationFrontendHostV1& operator=(const GenericCreationFrontendHostV1&) = delete;

    const FrontendRuntimeServicesV1& runtime_services() const noexcept {
        return runtime_services_;
    }
    const std::shared_ptr<CharacterState>& shared_state() const noexcept {
        return shared_state_;
    }
    GenericCreationFrontendResultV1 complete(FrontendRuntimeResultV1) const;

private:
    std::shared_ptr<CharacterState> shared_state_;
    std::shared_ptr<RuntimeCreationFlowAdapterV1> adapter_;
    FrontendRuntimeServicesV1 runtime_services_;
};

} // namespace dh::foundation::frontend::creation
