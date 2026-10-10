#pragma once

#include "runtime_creation_persistence_v1.hpp"
#include "../flow/menu_flow.hpp"

#include <filesystem>
#include <functional>
#include <memory>

namespace dh::foundation::frontend::creation {

using FrontendGenericStartV1 = std::function<bool(
    const std::shared_ptr<CharacterState>&, int difficulty, std::string&)>;

struct FrontendGenericStartReceiptV1 {
    std::shared_ptr<CharacterState> shared_state;
    std::filesystem::path save_path;
    int selected_slot{-1};
    bool created_in_this_flow{};
    bool start_delivered{};

    bool same_state_as(const std::shared_ptr<CharacterState>& expected) const noexcept;
    bool valid_for(const std::shared_ptr<CharacterState>& expected,
                   int expected_slot) const noexcept;
};

struct RuntimeCreationFlowServicesV1 {
    std::shared_ptr<CharacterState> shared_state;
    RuntimeCreationSourceV1 source;
    // Selects a genuinely empty slot and constructs the exact request/path/id
    // for this new profile. The adapter verifies the request binds the same
    // caller state and the authored name/class.
    std::function<bool(const std::string&, const std::string&, int&,
                       RuntimeCreationRequestV1&, std::string&)> select_new_profile;
    // Explicitly resolves only this selected existing slot. Failure never
    // falls through to profile creation or an alternate file.
    std::function<bool(int, std::filesystem::path&, std::string&)> selected_save_path;
    std::function<bool(int, int, std::string&)> assign_selected_slot;
    FrontendGenericStartV1 start_same_state;
};

// Bridges generic source creation into the authored Confirm -> StartGame flow.
// Confirm builds/persists/reloads and publishes into the existing shared state;
// StartGame assigns that exact slot and invokes the start provider afterward.
class RuntimeCreationFlowAdapterV1 final {
public:
    explicit RuntimeCreationFlowAdapterV1(RuntimeCreationFlowServicesV1);
    flow::Services bind_navigation(flow::Services base = {});
    std::optional<FrontendGenericStartReceiptV1> start_receipt() const;
    const std::shared_ptr<CharacterState>& shared_state() const noexcept { return services_.shared_state; }
    const std::optional<RuntimeCreationResultV1>& creation_result() const noexcept { return creation_result_; }

private:
    RuntimeCreationFlowServicesV1 services_;
    std::optional<RuntimeCreationResultV1> creation_result_;
    std::optional<FrontendGenericStartReceiptV1> active_profile_;
    std::optional<FrontendGenericStartReceiptV1> start_receipt_;
    bool assigned_for_start_{};

    bool create_save(const std::string&, const std::string&, int&, std::string&);
    bool assign_save(int, int, std::string&);
    bool start_game(int, std::string&);
    bool load_selected_existing(const flow::SlotFact&, std::string&);
};

} // namespace dh::foundation::frontend::creation
