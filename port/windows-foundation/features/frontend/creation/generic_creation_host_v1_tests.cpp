#include "generic_creation_host_v1.hpp"

#include <cassert>
#include <memory>

using namespace dh::foundation::frontend;
using namespace dh::foundation::frontend::creation;
using dh::foundation::CharacterState;

int main() {
    auto state = std::make_shared<CharacterState>();
    RuntimeCreationFlowServicesV1 creation_services;
    creation_services.shared_state = state;
    GenericCreationFrontendHostV1 host({}, std::move(creation_services));
    assert(host.shared_state().get() == state.get());
    assert(!host.shared_state().owner_before(state) && !state.owner_before(host.shared_state()));
    assert(host.runtime_services().generic_creation);

    GenericFrontendLaunchPlanV1 plan{
        state, "profiles/slot-3.sav", 3, true, true};
    assert(plan.same_state_as(state));
    assert(plan.valid_for(state, 3));

    GenericCreationFrontendResultV1 result;
    result.shared_state = state;
    result.frontend.outcome = FrontendRuntimeOutcomeV1::generic_gameplay_started;
    result.frontend.selected_slot = 3;
    result.frontend.generic_start_receipt = FrontendGenericStartReceiptV1{
        state, "profiles/slot-3.sav", 3, true, true};
    result.launch = plan;
    assert(result.gameplay_started_for(state, 3));
    // Generic launch evidence deliberately does not require a native profile
    // loan, InitPost receipt, or native assign/start receipt.
    assert(!result.frontend.gameplay_ready());

    assert(!result.gameplay_started_for(state, 2));
    result.launch->start_delivered = false;
    assert(!result.gameplay_started_for(state, 3));
    result.launch = plan;
    result.launch->save_path.clear();
    assert(!result.gameplay_started_for(state, 3));

    result.launch = plan;
    auto another_state = std::make_shared<CharacterState>();
    assert(!result.gameplay_started_for(another_state, 3));

    // Same address with a distinct control block must not be treated as the
    // caller's state lease. The no-op deleter keeps the test non-owning.
    auto alternate_owner = std::shared_ptr<CharacterState>(state.get(), [](CharacterState*) {});
    assert(alternate_owner.get() == state.get());
    assert(!plan.same_state_as(alternate_owner));
    assert(!result.gameplay_started_for(alternate_owner, 3));

    FrontendRuntimeResultV1 stopped;
    stopped.outcome = FrontendRuntimeOutcomeV1::quit;
    auto stopped_result = host.complete(std::move(stopped));
    assert(stopped_result.shared_state.get() == state.get());
    assert(!stopped_result.launch);
    assert(!stopped_result.gameplay_started_for(state, 3));

    FrontendRuntimeResultV1 started;
    started.outcome = FrontendRuntimeOutcomeV1::generic_gameplay_started;
    started.selected_slot = 3;
    started.generic_start_receipt = FrontendGenericStartReceiptV1{
        state, "profiles/slot-3.sav", 3, true, true};
    auto completed = host.complete(std::move(started));
    assert(completed.shared_state.get() == state.get());
    assert(completed.gameplay_started_for(state, 3));

    result.frontend.generic_start_receipt->start_delivered = false;
    assert(!result.gameplay_started_for(state, 3));
    result.frontend.generic_start_receipt->start_delivered = true;
    result.frontend.outcome = FrontendRuntimeOutcomeV1::quit;
    assert(!result.gameplay_started_for(state, 3));
    return 0;
}
