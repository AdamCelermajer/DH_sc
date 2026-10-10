#include "runtime_creation_flow_adapter_v1.hpp"

#include "../../../save_store.hpp"

#include <utility>

namespace dh::foundation::frontend::creation {
namespace {
bool same_owner(const std::shared_ptr<CharacterState>& a,
                const std::shared_ptr<CharacterState>& b) noexcept {
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool FrontendGenericStartReceiptV1::same_state_as(
    const std::shared_ptr<CharacterState>& expected) const noexcept {
    return same_owner(shared_state, expected);
}

bool FrontendGenericStartReceiptV1::valid_for(
    const std::shared_ptr<CharacterState>& expected, int expected_slot) const noexcept {
    return start_delivered && selected_slot >= 0 && selected_slot == expected_slot &&
           !save_path.empty() && same_state_as(expected);
}

RuntimeCreationFlowAdapterV1::RuntimeCreationFlowAdapterV1(RuntimeCreationFlowServicesV1 services)
    : services_(std::move(services)) {}

flow::Services RuntimeCreationFlowAdapterV1::bind_navigation(flow::Services base) {
    auto prior_inspect=std::move(base.inspect_slot);
    base.inspect_slot=[this,prior=std::move(prior_inspect)](int slot,flow::SlotFact& fact,std::string& error){
        if(prior)return prior(slot,fact,error);
        if(active_profile_&&active_profile_->selected_slot==slot&&!active_profile_->save_path.empty()){
            fact={slot,true,active_profile_->save_path};error.clear();return true;
        }
        error="Exact selected profile inspection provider unavailable";return false;
    };
    auto prior_profile_change=std::move(base.selected_profile_changed);
    base.selected_profile_changed=[this,prior=std::move(prior_profile_change)](const flow::SlotFact& fact){
        creation_result_.reset();active_profile_.reset();start_receipt_.reset();assigned_for_start_=false;
        if(prior)prior(fact);
    };
    base.create_save = [this](const std::string& name, const std::string& cls,
                              int& slot, std::string& error) {
        return create_save(name, cls, slot, error);
    };
    base.assign_save = [this](int slot, int player, std::string& error) {
        return assign_save(slot, player, error);
    };
    base.start_game = [this](int difficulty, std::string& error) {
        return start_game(difficulty, error);
    };
    base.load_selected_profile=[this](const flow::SlotFact& fact,std::string& error){
        return load_selected_existing(fact,error);
    };
    return base;
}

std::optional<FrontendGenericStartReceiptV1> RuntimeCreationFlowAdapterV1::start_receipt() const {
    return start_receipt_;
}

bool RuntimeCreationFlowAdapterV1::create_save(
    const std::string& name, const std::string& cls, int& slot, std::string& error) {
    // A new empty-slot flow starts with a clean result. Navigator retains its
    // saved prefix for retries, so this method is called once per new profile.
    creation_result_.reset();active_profile_.reset();start_receipt_.reset();assigned_for_start_=false;
    if (!services_.shared_state || !services_.select_new_profile)
        return fail(error, "Generic source profile and empty-slot selection providers are required");
    RuntimeCreationRequestV1 request;
    int selected = -1;
    if (!services_.select_new_profile(name, cls, selected, request, error)) {
        if (error.empty()) error = "Generic empty-slot/request provider failed";
        return false;
    }
    if (selected < 0 || !same_owner(request.shared_state, services_.shared_state) ||
        request.player_name != name || request.class_token != cls || request.save_path.empty())
        return fail(error, "Generic creation request did not bind the selected slot, authored name/class, path and caller state");

    RuntimeCreationServicesV1 creation_services;
    creation_services.source = services_.source;
    creation_result_ = RuntimeCreationPersistenceV1::create_reload(request, creation_services);
    if (creation_result_->status != RuntimeCreationStatusV1::prepared_for_start) {
        error = creation_result_->error.empty() ? "Generic source profile prepare/save/reload failed"
                                                : creation_result_->error;
        return false;
    }
    active_profile_ = FrontendGenericStartReceiptV1{
        services_.shared_state, request.save_path, selected, true, false};
    assigned_for_start_ = false;
    slot = selected;
    error.clear();
    return true;
}

bool RuntimeCreationFlowAdapterV1::load_selected_existing(const flow::SlotFact& selected, std::string& error) {
    const int slot=selected.id;
    if (!services_.shared_state || slot<0 || !selected.in_use)
        return fail(error, "Explicit occupied selected-existing-save fact is unavailable");
    std::filesystem::path path=selected.save_path;
    if(services_.selected_save_path){
        std::filesystem::path resolved;
        if(!services_.selected_save_path(slot,resolved,error)){
            if(error.empty())error="Selected-existing-save path resolver failed";
            return false;
        }
        if(resolved.empty())return fail(error,"Selected-existing-save resolver returned no exact file");
        if(!path.empty()&&path!=resolved)return fail(error,"Selected slot path disagrees with the exact path resolved for that slot");
        path=std::move(resolved);
    }
    if (path.empty()) return fail(error, "Selected-existing-save provider returned no explicit file");

    CharacterState loaded;
    if (!load_character(path, loaded, error)) {
        if (error.empty()) error = "Selected existing profile could not be loaded";
        return false;
    }
    const auto validation = validate_character_state(loaded);
    if (!validation.ok())
        return fail(error, validation.errors.empty() ? "Selected existing profile is invalid"
                                                     : validation.errors.front().c_str());
    const auto exact_pointer = services_.shared_state.get();
    const auto exact_owner = services_.shared_state;
    *services_.shared_state = std::move(loaded);
    if (services_.shared_state.get() != exact_pointer || !same_owner(services_.shared_state, exact_owner))
        return fail(error, "Selected profile load changed the caller CharacterState identity/owner");
    active_profile_ = FrontendGenericStartReceiptV1{
        services_.shared_state, std::move(path), slot, false, false};
    return true;
}

bool RuntimeCreationFlowAdapterV1::assign_save(int slot, int player, std::string& error) {
    assigned_for_start_ = false;
    if (slot < 0 || player != 0)
        return fail(error, "Generic frontend supports only the selected local player slot");
    if (!active_profile_ || active_profile_->selected_slot != slot) {
        return fail(error,"Selected existing profile must be loaded from its exact SlotFact before assignment");
    }
    if (!services_.assign_selected_slot)
        return fail(error, "Same-source selected-slot assignment provider is unavailable");
    if (!services_.assign_selected_slot(slot, player, error)) {
        if (error.empty()) error = "Selected-slot assignment provider failed";
        return false;
    }
    assigned_for_start_ = true;
    error.clear();
    return true;
}

bool RuntimeCreationFlowAdapterV1::start_game(int difficulty, std::string& error) {
    if (difficulty < 0 || difficulty > 2)
        return fail(error, "Generic StartGame difficulty is outside the three original modes");
    if (!assigned_for_start_ || !active_profile_)
        return fail(error, "StartGame requires the exact selected slot assignment in this flow");
    if (!services_.start_same_state)
        return fail(error, "Same-state generic start provider is unavailable");
    if (!same_owner(active_profile_->shared_state, services_.shared_state))
        return fail(error, "Generic profile no longer refers to the caller's exact shared CharacterState");
    const auto exact_pointer = services_.shared_state.get();
    const auto exact_owner = services_.shared_state;
    if (!services_.start_same_state(services_.shared_state, difficulty, error)) {
        if (error.empty()) error = "Same-state generic start provider failed";
        return false;
    }
    if (services_.shared_state.get() != exact_pointer || !same_owner(services_.shared_state, exact_owner))
        return fail(error, "Generic start replaced the caller's CharacterState pointer or owner");
    active_profile_->start_delivered = true;
    start_receipt_ = active_profile_;
    if (creation_result_) {
        creation_result_->status = RuntimeCreationStatusV1::start_provider_returned_success;
        creation_result_->start_provider_succeeded = true;
    }
    assigned_for_start_ = false;
    error.clear();
    return true;
}

} // namespace dh::foundation::frontend::creation
