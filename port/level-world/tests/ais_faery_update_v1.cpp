#include "../ais_faery_update_v1.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>

namespace update = dh2::ais_faery_update_v1;

struct Call { update::Operation operation; std::uintptr_t subject; int a; int b; };
struct Fixture {
    std::vector<Call> calls;
    std::vector<std::int32_t> current_ids;
    std::size_t next_id{};
    bool fail_visual{};
};

int invoke(void* raw, const update::Request& request, std::int32_t* output) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.calls.push_back({request.operation, request.subject,
                             request.argument0, request.argument1});
    if (!output) return 1;
    *output = 0;
    if (request.operation == update::Operation::current_faery_id) {
        if (fixture.next_id >= fixture.current_ids.size()) return 1;
        *output = fixture.current_ids[fixture.next_id++];
    }
    return request.operation == update::Operation::set_modular_skin &&
        fixture.fail_visual ? 1 : 0;
}

int main() {
    update::Character character{0x20, 0, 0};
    update::State state{0x10, &character, -1};
    Fixture fixture;
    const update::Services services{&fixture, &invoke};
    update::Result result{};

    assert(update::update(&state, &services, &result) == update::Status::complete);
    assert(fixture.calls.size() == 1 &&
           fixture.calls[0].operation == update::Operation::default_on_update);
    fixture.calls.clear();

    character.master = 0x30;
    assert(update::update(&state, &services, &result) == update::Status::complete);
    assert(fixture.calls.size() == 1 &&
           fixture.calls[0].operation == update::Operation::default_on_update);
    fixture.calls.clear();

    character.visual = 0x40;
    fixture.current_ids = {2, 2};
    fixture.next_id = 0;
    assert(update::update(&state, &services, &result) == update::Status::complete);
    assert(state.last_faery_id == 2 && result.visual_changed &&
           fixture.calls.size() == 4 && result.callbacks == 4);
    assert(fixture.calls[0].operation == update::Operation::default_on_update);
    assert(fixture.calls[1].operation == update::Operation::current_faery_id &&
           fixture.calls[1].subject == character.master && fixture.calls[1].a == -1);
    assert(fixture.calls[2].operation == update::Operation::current_faery_id);
    assert(fixture.calls[3].operation == update::Operation::set_modular_skin &&
           fixture.calls[3].subject == character.visual && fixture.calls[3].a == 0 &&
           fixture.calls[3].b == 2);
    fixture.calls.clear();

    fixture.current_ids = {2};
    fixture.next_id = 0;
    assert(update::update(&state, &services, &result) == update::Status::complete);
    assert(fixture.calls.size() == 2 && !result.visual_changed &&
           result.current_faery_id == 2);
    fixture.calls.clear();

    fixture.current_ids = {3, 4};
    fixture.next_id = 0;
    fixture.fail_visual = true;
    assert(update::update(&state, &services, &result) == update::Status::failed);
    assert(state.last_faery_id == 4 && result.current_faery_id == 4 &&
           !result.visual_changed && fixture.calls.size() == 4);

    std::puts("{\"ais_faery_source_order\":true,\"changed_skin_applied_once\":true,\"changed_value_reread\":true,\"source_store_retained_on_visual_failure\":true}");
    return 0;
}
