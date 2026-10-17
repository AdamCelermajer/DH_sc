#include "../destructible_container_interaction_v1.hpp"

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace kernel = dh2::level_world::destructible_container_interaction_v1;

struct Context {
    std::vector<int> calls;
    kernel::events::Event event{};
    std::int32_t frame{-1};
    std::int32_t state{-1};
    bool level{true};
};

bool current_level(void* raw, bool& present, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.calls.push_back(1);
    present = context.level;
    return true;
}
bool destroy_constant(void* raw, std::int32_t& value, std::string&) {
    static_cast<Context*>(raw)->calls.push_back(2);
    value = 77;
    return true;
}
bool raise_event(void* raw, kernel::events::Event& event, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.calls.push_back(3);
    context.event = event;
    return true;
}
bool set_frame(void* raw, std::int32_t frame, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.calls.push_back(4);
    context.frame = frame;
    return true;
}
bool hit_sound(void* raw, std::int32_t sound, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.calls.push_back(5);
    assert(sound == 325);
    return true;
}
bool set_opener(void* raw, std::uintptr_t attacker, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.calls.push_back(6);
    assert(attacker == 0x1234);
    return true;
}
bool set_state(void* raw, std::int32_t state, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.calls.push_back(7);
    context.state = state;
    return true;
}
bool remove_physical(void* raw, std::string&) {
    static_cast<Context*>(raw)->calls.push_back(8);
    return true;
}
bool activate(void* raw, std::string&) {
    static_cast<Context*>(raw)->calls.push_back(9);
    return true;
}
bool do_open(void* raw, std::string&) {
    static_cast<Context*>(raw)->calls.push_back(10);
    return true;
}
bool interact_sound(void* raw, std::int32_t sound, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.calls.push_back(11);
    assert(sound == 325);
    return true;
}

int main() {
    Context context;
    kernel::State state{};
    state.identity = 0x5678;
    state.attacker = 0x1234;
    state.network_id = 91;
    state.data_id = 30;
    state.initial_hits = 1;
    state.remaining_hits = 1;
    state.interact_sound = 325;
    state.container_state = 2;
    state.visual_present = true;

    const kernel::Services services{
        &context, current_level, destroy_constant, raise_event, set_frame,
        hit_sound, set_opener, set_state, remove_physical, activate,
        do_open, interact_sound};
    kernel::Result result{};
    std::string error;

    assert(kernel::interact(state, services, result, error) == kernel::Status::complete);
    assert(result.hit_counter_decremented && !result.destroy_event_raised &&
           !result.opened && state.remaining_hits == 0);
    assert((context.calls == std::vector<int>{4, 5}));
    assert(context.frame == 1);

    context.calls.clear();
    state.visual_present = false;
    assert(kernel::interact(state, services, result, error) == kernel::Status::complete);
    assert(result.destroy_event_raised && result.opened);
    assert(context.event.objective_type == 77 && context.event.character == 0x1234 &&
           context.event.network_id == 91 && context.event.flag0 == 0 &&
           context.event.flag1 == 0 && context.event.subject_id == -1 &&
           context.event.source_data_id == 30 &&
           context.event.object_identity == 0);
    assert((context.calls == std::vector<int>{1, 2, 3, 6, 7, 8, 10, 11}));
    assert(context.state == 4 && state.container_state == 4);

    // The derived event precedes Container::Interact's state 3/4 guard.
    context.calls.clear();
    assert(kernel::interact(state, services, result, error) == kernel::Status::complete);
    assert(result.destroy_event_raised && result.already_open && !result.opened);
    assert((context.calls == std::vector<int>{1, 2, 3}));

    // The animation's derived `opened` event raises a second event without an
    // attacker. It carries ObjectBase+0x64 and the virtual DataID separately.
    context.calls.clear();
    context.event = {};
    const kernel::OpenedEventServices opened_services{
        &context, current_level, destroy_constant, raise_event};
    assert(kernel::raise_opened_event(12, 30, opened_services, error) ==
           kernel::Status::complete);
    assert((context.calls == std::vector<int>{1, 2, 3}));
    assert(context.event.objective_type == 77 && context.event.character == 0 &&
           context.event.network_id == 12 && context.event.flag0 == 0 &&
           context.event.flag1 == 0 && context.event.subject_id == -1 &&
           context.event.source_data_id == 30 &&
           context.event.object_identity == 0);

    context.calls.clear();
    context.level = false;
    assert(kernel::raise_opened_event(12, 30, opened_services, error) ==
           kernel::Status::current_level_missing);
    assert((context.calls == std::vector<int>{1}));
    context.level = true;
    return 0;
}
