#include "source_character_owner_factory_npc_context.hpp"

#include "../../../level-world/character_properties_temp_global_v62.hpp"

#include <utility>

namespace dh::foundation::features {
namespace {
using Record = dh2::world::CanonicalCharacterCandidateRecordV60;
using namespace dh2::character;

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool same_input_pointer(const void* existing, const void* actual,
    std::string& error, const char* field) {
    if (existing && existing != actual) {
        error = std::string("NPC script provider supplied a foreign ") + field;
        return false;
    }
    return true;
}

bool validate_record(const std::shared_ptr<Record>& record,
    const SourceCharacterNpcContextProvidersV1& providers,
    RetainedCharacterActorV1*& actor, std::uintptr_t& identity,
    std::string& error) {
    if (!record || record->failed || !record->services.world ||
        !providers.world_lease ||
        providers.world_lease.get() != record->services.world.get()) {
        return fail(error, "Required retained same-world canonical Character record");
    }
    if (!record->actor || !record->actor->object || !record->actor->machine ||
        !record->actor->controller || record->actor->session ||
        !record->fsm_context_v101 || !record->properties || !record->life) {
        return fail(error, "Required same-record NPC actor/controller/FSM before Session construction");
    }

    actor = record->actor.get();
    identity = actor->object->identity;
    auto& object = *actor->object;
    auto& native_fsm = actor->machine->native_fsm();
    if (!identity || actor->source_name().empty() || object.name != actor->source_name() ||
        actor->shared_handle().cached != identity ||
        object.properties != record->properties || object.life != record->life ||
        native_fsm.character != identity || native_fsm.state != &actor->machine->state() ||
        object.binding.state != &object.target || object.target.owner != &object.owner ||
        !object.target.owner || object.target.owner->identity != identity) {
        return fail(error, "NPC input aliases do not identify the same retained Character/target/FSM");
    }

    bool player{};
    if (!record->is_player(player, error)) return false;
    if (player) return fail(error, "NPC script context cannot bind a player Character record");

    if (!providers.context_lease || !providers.host || !providers.level ||
        !providers.objects || !providers.bind_events) {
        return fail(error, "Required actual same-world NPC host/level/object/FSM-event providers");
    }
    if (record->host_context_v70 &&
        record->host_context_v70.get() != providers.context_lease.get()) {
        return fail(error, "NPC host context lease changed after same-record publication");
    }

    ControllerCommandState32 projected{};
    if (!record->services.controller ||
        !record->services.controller(projected, *record, error)) {
        if (error.empty()) error = "Required actual same-Character controller projection";
        return false;
    }
    auto* live = actor->controller->command_state(projected.global_blocked);
    if (!live || live->owner != identity || projected.owner != identity ||
        projected.controller != live->controller ||
        projected.controller != actor->controller->identity()) {
        return fail(error, "NPC controller provider does not borrow the same Character controller");
    }

    error.clear();
    return true;
}
} // namespace

bool bind_source_character_owner_factory_npc_context_v1(
    const std::shared_ptr<Record>& record, CharacterScriptSessionInput& input,
    const SourceCharacterNpcContextProvidersV1& providers,
    SourceCharacterNpcContextBorrowV1& out, std::string& error) {
    out = {};
    RetainedCharacterActorV1* actor{};
    std::uintptr_t identity{};
    if (!validate_record(record, providers, actor, identity, error)) return false;

    auto& object = *actor->object;
    auto& machine = actor->machine->native_fsm();
    if ((input.identity && input.identity != identity) ||
        (input.properties && input.properties != record->properties) ||
        (input.combat && input.combat != record->life) ||
        (input.temporary && input.temporary != character_properties_temp_global_v62()) ||
        !same_input_pointer(input.target, &object.binding, error, "Character target") ||
        !same_input_pointer(input.state_machine, &machine, error, "Character FSM") ||
        !same_input_pointer(input.host, providers.host, error, "NPC host provider") ||
        !same_input_pointer(input.level, providers.level, error, "NPC level provider") ||
        !same_input_pointer(input.objects, providers.objects, error, "NPC object provider")) {
        if (error.empty()) error = "NPC script input attempted a foreign same-Character owner";
        return false;
    }

    // These are direct projections of the record/actor which construct_script
    // will check and retain. Bytecode/cache spans are deliberately untouched.
    input.identity = identity;
    input.name = actor->source_name();
    input.properties = record->properties;
    input.combat = record->life;
    input.temporary = character_properties_temp_global_v62();
    input.position = object.position;
    input.source_is_character = 1;
    input.target = &object.binding;
    input.state_machine = &machine;
    input.host = providers.host;
    input.level = providers.level;
    input.objects = providers.objects;

    // Pin the provider context in the existing canonical record. This owner
    // must not capture `record`; otherwise it would create an ownership cycle.
    if (!record->host_context_v70) record->host_context_v70 = providers.context_lease;
    if (!providers.bind_events(*record, input, error)) {
        if (error.empty()) error = "Required same-record NPC FSM event/timer binding";
        return false;
    }
    if (!input.timer_services || !input.timer_services->expired ||
        input.timer_services->reserved || !input.commands ||
        !input.commands->state || !input.commands->services.invoke ||
        !input.commands_refresh_context || !input.commands_refresh ||
        !input.motion_capabilities.context || !input.motion_capabilities.invoke ||
        !input.animation_registration.context ||
        !input.animation_registration.register_dictionary ||
        !input.gameplay_context || !input.gameplay_binding) {
        return fail(error, "NPC FSM binder did not provide actual same-owner command/motion/animation/gameplay/timer services");
    }

    SourceCharacterNpcContextBorrowV1 result;
    result.record_lease = record;
    result.world_lease = providers.world_lease;
    result.context_lease = record->host_context_v70;
    result.identity = identity;
    result.machine = &machine;
    result.target = &object.binding;
    result.controller = actor->controller.get();
    result.timer_expiry = input.timer_services;
    out = std::move(result);
    error.clear();
    return true;
}

bool validate_source_character_owner_factory_npc_session_v1(
    const SourceCharacterNpcContextBorrowV1& loan, std::string& error) {
    const auto& record = loan.record_lease;
    auto* actor = record ? record->actor.get() : nullptr;
    if (!record || record->failed || !actor || !actor->object ||
        !actor->machine || !actor->controller || !actor->session ||
        !record->fsm_context_v101 || !loan.world_lease ||
        loan.world_lease.get() != record->services.world.get() ||
        loan.context_lease != record->host_context_v70 ||
        !loan.context_lease || loan.identity != actor->object->identity ||
        loan.machine != &actor->machine->native_fsm() ||
        loan.target != &actor->object->binding ||
        loan.controller != actor->controller.get() || !loan.timer_expiry ||
        !loan.timer_expiry->expired || loan.timer_expiry->reserved) {
        return fail(error, "NPC Session lost its retained same-record context/FSM provider leases");
    }

    auto& session = *actor->session;
    const auto& view = session.property_view();
    if (!record->properties || session.properties() != record->properties ||
        actor->object->properties != record->properties ||
        actor->object->life != record->life ||
        session.timers().owner != loan.identity ||
        view.resolved != record->properties->resolved.data() ||
        view.saved != record->properties->saved.data() ||
        view.base != record->properties->base.data() ||
        view.gear != record->properties->gear.data()) {
        return fail(error, "NPC Session/TimerStore/PropertyView do not alias the same canonical Character");
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::features
