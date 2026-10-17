#include "player_enemy_kill_credit_v1.hpp"

#include <array>
#include <exception>

namespace dh2::player_enemy_kill_credit_v1 {
namespace {
namespace ch = character;

constexpr std::uint32_t virtual_bit = 1u << ch::ai_event_virtual;
constexpr std::uint32_t state_event_bit = 1u << ch::ai_event_state_event;
constexpr std::uint32_t ais_virtual_bit = 1u << ch::ai_event_ais_virtual;

struct EventBridge {
    const Bindings* bindings = nullptr;
    ch::AIEventState64* state = nullptr;
    std::uintptr_t victim = 0;
    std::uintptr_t char_ai_on_kill = 0;
    std::uintptr_t ais_player_on_kill = 0;
};

std::int32_t dispatch(void* raw, ch::AIEventState64* state,
                      const ch::AIEventRequest40* request,
                      std::uint32_t* value) {
    auto& bridge = *static_cast<EventBridge*>(raw);
    const auto& b = *bridge.bindings;
    if (!state || state != bridge.state || !state->owner || !request || !value ||
        state->ai != b.char_ai || state->active != b.active_ais ||
        state->owner->owner != b.character) return 1;

    if (request->service == ch::ai_event_state_event) {
        if (request->operation || request->event != 4 ||
            request->subject != state->owner->state_machine || request->callee ||
            request->payload != bridge.victim || !(b.backend.available & state_event_bit)) return 1;
        return b.backend.invoke(b.backend.context, state, request, value);
    }

    if (request->service != ch::ai_event_virtual || request->operation != 0xb0 ||
        request->event != 4 || request->subject != b.char_ai ||
        request->callee != bridge.char_ai_on_kill || request->payload != bridge.victim) return 1;

    // CharAI::OnKill reads CharAI+0x1c and returns immediately when it is
    // null. The reached RaiseEvent is still complete, but no AIS callback is
    // invoked and no second VM is invented.
    if (!b.active_ais) return 0;
    if (!(b.backend.available & ais_virtual_bit)) return 1;

    // CharAI::OnKill loads CharAI+0x1c and dispatches AIS+0xb0 with the same
    // killed Character pointer. AISDefault::OnKill is an empty source body;
    // AISPlayer::OnKill performs the source flags gate and Lua call in backend.
    const ch::AIEventRequest40 ais_request{
        ch::ai_event_ais_virtual, 0xb0, 4, 0, b.active_ais,
        bridge.ais_player_on_kill, bridge.victim
    };
    return b.backend.invoke(b.backend.context, state, &ais_request, value);
}

bool valid(const Bindings& b) {
    if (!b.character || !b.char_ai || !b.controller ||
        !b.state_machine || !b.property_owner || b.forced > 255 || b.locked > 255 ||
        b.paused > 255 || b.global_blocked > 255 || !b.victim_outgoing ||
        !b.properties || !b.current_target ||
        !b.backend.invoke || b.backend.reserved || (b.backend.available & ~63u) ||
        dh2_property_validate(b.properties)) return false;

    const auto& aggro = *b.victim_outgoing;
    if (aggro.count > aggro.capacity || (aggro.count && !aggro.entries)) return false;
    if (b.properties->types[23] != -1 && !(std::uint32_t(b.properties->types[23]) & (8u | 32u))) return false;
    if (b.properties->types[24] != -1 && !(std::uint32_t(b.properties->types[24]) & (8u | 32u))) return false;

    const bool reaches_state_machine = !b.forced && (b.global_blocked || b.locked);
    if (reaches_state_machine) return (b.backend.available & state_event_bit) != 0;
    return !b.active_ais || (b.backend.available & ais_virtual_bit) != 0;
}
}

Runtime::Runtime(Bindings bindings) : bindings_(bindings) {}

Status Runtime::after_loot_attempt(std::uintptr_t victim, std::uintptr_t killer,
                                   std::uint32_t kill_force, Result* output,
                                   std::string& error) {
    if (busy_) return Status::busy;
    if (consumed_) return Status::consumed;
    if (!output || !victim) return Status::invalid_argument;

    // Character::Kill performs DropLoot first, then skips the aggro/event4 and
    // property-credit loop for a null killer or nonzero force argument.
    if (!killer || kill_force) {
        consumed_ = true;
        return Status::ineligible_kill;
    }
    if (!valid(bindings_)) return Status::invalid_argument;

    const auto& aggro = *bindings_.victim_outgoing;
    if (aggro.count != 1 || aggro.entries[0].character != bindings_.character)
        return Status::ineligible_aggro;

    busy_ = true;
    consumed_ = true;
    struct BusyReset { bool& value; ~BusyReset() { value = false; } } reset{busy_};
    *output = {};
    error.clear();

    try {
        std::array<std::uintptr_t, 51> char_ai_virtuals{};
        std::array<std::uintptr_t, 51> ais_virtuals{};
        char_ai_virtuals[0xb0 / 4] = char_ai_on_kill_identity;
        ais_virtuals[0xb0 / 4] = ais_player_on_kill_identity;

        ch::AIEventOwner48 owner{
            bindings_.character, bindings_.controller, bindings_.state_machine,
            bindings_.property_owner, bindings_.forced, bindings_.locked, 0, 0
        };
        ch::AIEventState64 state{
            bindings_.char_ai, &owner, char_ai_virtuals.data(), bindings_.active_ais,
            ais_virtuals.data(), bindings_.paused, 0, bindings_.global_blocked, 0, 0, 0
        };
        EventBridge bridge{&bindings_, &state, victim,
                           char_ai_on_kill_identity, ais_player_on_kill_identity};
        const ch::AIEventServices24 event_services{
            &bridge, dispatch, virtual_bit | state_event_bit, 0
        };
        const ch::AIEventPayload24 payload{victim, 0, 0, 0};
        output->event4_reached = 1;
        output->event4_status = dh2_character_ai_event(
            &output->dispatch, &state, 4, &payload, &event_services);
        output->event4_completed = output->event4_status == 0;

        // Character::Kill ignores RaiseEvent's return, rereads the recipient's
        // target, writes the matching field directly, and always reaches both
        // AddInt calls. A renderer selection is only a projection of that
        // canonical source write and cannot block later source effects.
        if (*bindings_.current_target == victim) {
            *bindings_.current_target = 0;
            output->target_cleared = 1;
            if (bindings_.clear_matching_target) {
                output->target_projection_attempted = 1;
                output->target_projection_status = bindings_.clear_matching_target(
                    bindings_.target_context, victim);
            }
        }

        // PROPS_AddInt(Character+0x560, property, 1): source API receives a
        // fixed-point delta of 256 and stores through the live PropertyView.
        output->property23_status = dh2_property_add(bindings_.properties, 23, 256);
        output->property23_added = output->property23_status == 0;
        output->property24_status = dh2_property_add(bindings_.properties, 24, 256);
        output->property24_added = output->property24_status == 0;
        if (output->property23_status || output->property24_status) {
            error = "Character::Kill property AddInt provider failed";
            return Status::failed;
        }
        return Status::complete;
    } catch (const std::exception& exception) {
        error = exception.what();
    } catch (...) {
        error = "Character::Kill credit provider exception";
    }
    return Status::failed;
}

} // namespace dh2::player_enemy_kill_credit_v1
