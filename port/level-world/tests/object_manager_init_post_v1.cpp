#include "../object_manager_init_post_v1.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace dh2::object_manager_init_post_v1;
using dh2::object_manager_runtime_owner_v1::GameObject;
using dh2::object_manager_runtime_owner_v1::Owner;

namespace {

struct Fixture {
    std::vector<std::string> events;
    Owner* retire_owner{};
    Owner* canonical_owner{};
};

std::string object_tag(const GameObject& object) {
    return std::to_string(object.identity);
}

bool load_module(void* context, Address identity) {
    static_cast<Fixture*>(context)->events.push_back(
        "module:" + std::to_string(identity));
    return true;
}

bool object_init_post(void* context, GameObject& object) {
    static_cast<Fixture*>(context)->events.push_back(
        "init:" + object_tag(object));
    return true;
}

bool test_enable_condition(void* context, GameObject& object,
                           bool include_enabled) {
    static_cast<Fixture*>(context)->events.push_back(
        std::string("test-enable:") + (include_enabled ? "true:" : "false:") +
        object_tag(object));
    return !include_enabled;
}

bool clear_post_init_lists(void* context) {
    static_cast<Fixture*>(context)->events.push_back("clear:2c,44,34");
    return true;
}

bool is_room_zone(void* context, const GameObject& object, bool* result) {
    static_cast<Fixture*>(context)->events.push_back(
        "is-room-zone:" + object_tag(object));
    *result = object.identity == 0xB;
    return true;
}

bool append_room_zone(void* context, GameObject& object) {
    static_cast<Fixture*>(context)->events.push_back(
        "append-room-zone:" + object_tag(object));
    return true;
}

bool room_zone_init_object_list(void* context, GameObject& object) {
    static_cast<Fixture*>(context)->events.push_back(
        "room-zone-init-list:" + object_tag(object));
    return true;
}

bool has_additional_init_list(void* context, const GameObject& object,
                              bool* result) {
    static_cast<Fixture*>(context)->events.push_back(
        "v38:" + object_tag(object));
    *result = object.identity == 0xA;
    return true;
}

bool append_additional_init_object(void* context, GameObject& object) {
    auto& fixture=*static_cast<Fixture*>(context);
    fixture.events.push_back(
        "append-2c:" + object_tag(object));
    if(fixture.canonical_owner) {
        bool appended=false;
        return fixture.canonical_owner->append_init_post_update_object(
            object.identity,true,&appended)==
               dh2::object_manager_runtime_owner_v1::Status::ok&&appended;
    }
    return true;
}

bool active_list_condition_gate(void* context, const GameObject& object,
                                bool* result) {
    static_cast<Fixture*>(context)->events.push_back(
        "condition-gate-ac-a8:" + object_tag(object));
    *result = object.identity != 0xC;
    return true;
}

bool read_active_list_flags(void* context, const GameObject& object,
                            bool* flag_d0, bool* flag_cc) {
    static_cast<Fixture*>(context)->events.push_back(
        "flags-d0-cc:" + object_tag(object));
    *flag_d0 = object.identity == 0xB || object.identity == 0xD;
    *flag_cc = object.identity != 0xB && object.identity != 0xD;
    return true;
}

bool append_active_object(void* context, GameObject& object) {
    auto& fixture=*static_cast<Fixture*>(context);
    fixture.events.push_back(
        "append-44:" + object_tag(object));
    if(fixture.canonical_owner) {
        bool appended=false;
        return fixture.canonical_owner->append_init_post_condition_object(
            object.identity,&appended)==
               dh2::object_manager_runtime_owner_v1::Status::ok&&appended;
    }
    return true;
}

bool record_update_condition(void* context, GameObject* object) noexcept {
    auto* events=static_cast<std::vector<std::string>*>(context);
    if(!events||!object||!object->identity)return false;
    events->push_back("condition:"+object_tag(*object));
    return true;
}

bool record_update_object(void* context, GameObject* object) noexcept {
    auto* events=static_cast<std::vector<std::string>*>(context);
    if(!events||!object||!object->identity)return false;
    events->push_back("update:"+object_tag(*object));
    return true;
}

Services services(Fixture& fixture) {
    return {&fixture, load_module, object_init_post, test_enable_condition,
            clear_post_init_lists, is_room_zone, append_room_zone,
            room_zone_init_object_list, has_additional_init_list,
            append_additional_init_object, active_list_condition_gate,
            read_active_list_flags, append_active_object};
}

GameObject object(Address identity) {
    GameObject result{};
    result.identity = identity;
    return result;
}

void module_and_object_phases_are_resumable_and_ordered() {
    Owner owner;
    GameObject* ignored = nullptr;
    assert(owner.add_object(40, object(0xD), &ignored) ==
           dh2::object_manager_runtime_owner_v1::Status::ok);
    assert(owner.add_object(30, object(0xC), &ignored) ==
           dh2::object_manager_runtime_owner_v1::Status::ok);
    assert(owner.add_object(7, object(0xA), &ignored) ==
           dh2::object_manager_runtime_owner_v1::Status::ok);
    assert(owner.add_object(3, object(0xB), &ignored) ==
           dh2::object_manager_runtime_owner_v1::Status::ok);

    const ModuleRef modules[]{{0x100}, {0x200}};
    State state{};
    assert(begin(&state, &owner, modules, 2) == Status::ok);
    Fixture fixture;
    fixture.canonical_owner=&owner;
    const auto provider = services(fixture);

    Status status = Status::progress;
    for (unsigned step_index = 0; step_index < 64 && status != Status::complete;
         ++step_index) {
        status = step(&state, &provider);
        assert(status == Status::progress || status == Status::complete);
    }
    assert(status == Status::complete);
    assert(state.phase == Phase::done);
    assert(state.module_load_calls == 2);
    assert(state.object_init_calls == 4);
    assert(state.post_init_object_calls == 4);
    for (const auto handle : {3, 7, 30, 40}) {
        const auto* row = owner.find_by_source_handle(handle);
        assert(row && row->object_init_post_ready == 1);
    }
    GameObject* active_update=nullptr;
    assert(owner.update_object_count()==1&&
           owner.update_object_at(0,&active_update)&&active_update&&
           active_update->identity==0xA);
    assert(owner.init_post_condition_object_count()==4);
    // B is the explicit source short-circuit case: its first gate is true,
    // while its unread second pair would be d0=true, cc=false.
    const Address expected_condition_order[]{0xB,0xA,0xC,0xD};
    for(std::size_t index=0;index<4;++index) {
        Address identity=0;
        assert(owner.init_post_condition_object_at(index,&identity));
        assert(identity==expected_condition_order[index]);
    }
    std::vector<Address> visited_conditions;
    std::size_t visited_condition_count=0;
    const auto record_condition_visit=[](void* raw,GameObject* row) noexcept {
        auto* visited=static_cast<std::vector<Address>*>(raw);
        if(!visited||!row||!row->identity)return false;
        visited->push_back(row->identity);
        return true;
    };
    assert(owner.visit_condition_objects(&visited_conditions,
        record_condition_visit,&visited_condition_count)==
        dh2::object_manager_runtime_owner_v1::Status::ok);
    const std::vector<Address> expected_condition_visits{0xB,0xA,0xC,0xD};
    assert(visited_condition_count==4&&
           visited_conditions==expected_condition_visits);

    // ObjectManager::Update drains spawn staging before the +0x44 condition
    // list, then visits +0x2c update objects. Keep a focused ordering fixture
    // for that phase boundary, using the same retained owner lists.
    std::vector<std::string> update_events;
    std::size_t condition_phase_count=0,update_phase_count=0;
    assert(owner.visit_condition_objects(&update_events,
        record_update_condition,&condition_phase_count)==
        dh2::object_manager_runtime_owner_v1::Status::ok);
    assert(owner.visit_update_objects(&update_events,record_update_object,
        &update_phase_count)==dh2::object_manager_runtime_owner_v1::Status::ok);
    const std::vector<std::string> expected_update_events{
        "condition:11","condition:10","condition:12","condition:13",
        "update:10"};
    assert(condition_phase_count==4&&update_phase_count==1&&
           update_events==expected_update_events);

    const std::vector<std::string> expected{
        "module:256", "module:512",
        "init:11", "test-enable:false:11",
        "init:10", "test-enable:false:10",
        "init:12", "test-enable:false:12",
        "init:13", "test-enable:false:13",
        "clear:2c,44,34",
        "is-room-zone:11", "append-room-zone:11", "room-zone-init-list:11",
        "condition-gate-ac-a8:11", "append-44:11",
        "is-room-zone:10", "v38:10", "append-2c:10",
        "condition-gate-ac-a8:10", "append-44:10",
        "is-room-zone:12", "v38:12", "condition-gate-ac-a8:12",
        "flags-d0-cc:12", "append-44:12",
        "is-room-zone:13", "v38:13", "condition-gate-ac-a8:13",
        "append-44:13"};
    assert(fixture.events == expected);
}

void missing_module_provider_does_not_advance_cursor() {
    Owner owner;
    const ModuleRef modules[]{{0x1234}};
    State state{};
    assert(begin(&state, &owner, modules, 1) == Status::ok);

    assert(step(&state, nullptr) == Status::unsupported_provider);
    assert(state.phase == Phase::load_modules && state.next_module == 0);
    assert(begin(&state, &owner, modules, 1) == Status::invalid_state);
    assert(state.phase == Phase::load_modules && state.next_module == 0);

    Fixture fixture;
    auto provider = services(fixture);
    provider.object_init_post = nullptr;
    assert(step(&state, &provider) == Status::progress);
    assert(state.next_module == 1 && state.module_load_calls == 1);
}

void begin_rejects_invalid_borrowed_module_view() {
    Owner owner;
    State state{};
    const ModuleRef invalid[]{{0}};
    assert(begin(&state, &owner, invalid, 1) == Status::invalid_argument);
    assert(state.phase == Phase::unstarted);
    assert(begin(&state, &owner, nullptr, 1) == Status::invalid_argument);
}

void failed_test_enable_does_not_publish_completion_receipt() {
    Owner owner;
    GameObject* row = nullptr;
    assert(owner.add_object(3, object(0xB), &row) ==
           dh2::object_manager_runtime_owner_v1::Status::ok);
    row->object_init_post_ready = 1; // stale receipt from a prior lifecycle

    const ModuleRef modules[]{{0x100}};
    State state{};
    assert(begin(&state, &owner, modules, 1) == Status::ok);
    Fixture fixture;
    auto provider = services(fixture);
    provider.test_enable_condition = [](void*, GameObject&, bool) { return false; };

    Status status = Status::progress;
    for (unsigned i = 0; i < 8 && status == Status::progress; ++i)
        status = step(&state, &provider);
    assert(status == Status::provider_failed);
    assert(state.phase == Phase::failed);
    assert(state.failed_phase == Phase::object_init_post);
    assert(state.failed_provider == ProviderBoundary::test_enable_condition);
    assert(state.failed_identity == 0xB);
    assert(state.failed_source_handle == 3);
    assert(std::string(provider_boundary_name(state.failed_provider)) ==
           "test_enable_condition");
    assert(std::string(phase_name(state.failed_phase)) == "object_init_post");
    assert(row->object_init_post_ready == 0);
}

void init_post_clear_empties_all_three_canonical_lists() {
    Owner owner;
    GameObject* row = nullptr;
    assert(owner.add_object(3, object(0xB), &row) ==
           dh2::object_manager_runtime_owner_v1::Status::ok);
    bool changed = false;
    assert(owner.append_init_post_update_object(row->identity, true, &changed) ==
           dh2::object_manager_runtime_owner_v1::Status::ok && changed);
    assert(owner.stage_spawn_update_object(row->identity, true, &changed) ==
           dh2::object_manager_runtime_owner_v1::Status::ok && changed);
    assert(owner.append_init_post_condition_object(row->identity,&changed)==
           dh2::object_manager_runtime_owner_v1::Status::ok&&changed);
    assert(owner.update_object_count() == 1 &&
           owner.staged_update_object_count() == 1&&
           owner.init_post_condition_object_count()==1);
    owner.clear_init_post_lists();
    assert(owner.update_object_count() == 0 &&
           owner.staged_update_object_count() == 0&&
           owner.init_post_condition_object_count()==0);
}

void removed_row_is_re_resolved_before_test_enable() {
    Owner owner;
    GameObject* row = nullptr;
    assert(owner.add_object(3, object(0xB), &row) ==
           dh2::object_manager_runtime_owner_v1::Status::ok);
    State state{};
    assert(begin(&state, &owner, nullptr, 0) == Status::ok);
    Fixture fixture;
    fixture.retire_owner = &owner;
    auto provider = services(fixture);
    provider.object_init_post = [](void* raw, GameObject& current) {
        auto& fixture = *static_cast<Fixture*>(raw);
        fixture.events.push_back("init:" + object_tag(current));
        bool removed = false;
        return fixture.retire_owner->remove_object(current.source_handle,
                                                    &removed) ==
                   dh2::object_manager_runtime_owner_v1::Status::ok && removed;
    };
    assert(step(&state, &provider) == Status::progress); // empty module list
    assert(step(&state, &provider) == Status::progress); // callback retires row
    assert(step(&state, &provider) == Status::progress); // list clear
    assert(state.phase == Phase::build_post_init_lists);
    assert(state.object_init_calls == 0);
    const std::vector<std::string> expected{
        "init:11", "clear:2c,44,34"};
    assert(fixture.events == expected);
}

} // namespace

int main() {
    module_and_object_phases_are_resumable_and_ordered();
    missing_module_provider_does_not_advance_cursor();
    begin_rejects_invalid_borrowed_module_view();
    failed_test_enable_does_not_publish_completion_receipt();
    init_post_clear_empties_all_three_canonical_lists();
    removed_row_is_re_resolved_before_test_enable();
    std::cout << "ObjectManager InitPost v1 host checks passed: resumable source order, +0x2c/+0x44 active lists, OR short-circuit\n";
}
