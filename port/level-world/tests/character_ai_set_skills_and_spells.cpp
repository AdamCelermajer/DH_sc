#include "../character_ai_set_skills_and_spells.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace k = dh2::character_ai_set_skills_and_spells;
namespace ctor = dh2::character_ai_skill_script_constructor;
namespace faery = dh2::character_faery_selection;

constexpr std::uintptr_t AI = 0x10001000;
constexpr std::uintptr_t OWNER_A = 0x10002000;
constexpr std::uintptr_t OWNER_B = 0x10003000;
constexpr std::uintptr_t SKILL_VECTOR = 0x10004000;
constexpr std::uintptr_t FAERY_VECTOR = 0x10005000;
constexpr std::uintptr_t SCRIPT_A = 0x10006000;
constexpr std::uintptr_t SCRIPT_B = 0x10006100;
constexpr std::uintptr_t FAERY_A = 0x10006200;
constexpr std::uintptr_t FAERY_B = 0x10006300;

struct Event {
    std::uint32_t operation;
    std::uint32_t list;
    std::uint32_t slot;
    std::uintptr_t receiver;
    std::uintptr_t name;
    std::uintptr_t arguments;
    std::uint32_t integer;
    std::uint32_t number_bits;
    std::uint32_t allocation_bytes;
    std::uint32_t allocation_hint;
    std::string text;
};

struct Fixture {
    std::string scenario;
    std::array<std::uintptr_t, 8> skill_storage{};
    std::array<std::uintptr_t, 8> faery_storage{};
    std::array<std::int32_t, 3> faery_members{{0, 1, 2}};
    std::array<faery::FaeryRow, 3> faery_rows{};
    std::array<std::uintptr_t, 3> full_width_names{{
        static_cast<std::uintptr_t>(0x1234567887654321ull),
        static_cast<std::uintptr_t>(0x1234567887654322ull),
        static_cast<std::uintptr_t>(0x1234567887654323ull)}};
    k::FaeryBinding::FullWidthScriptNames full_width_binding{
        faery_rows.data(), full_width_names.data(), full_width_names.size()};
    faery::FaeryListRow faery_list_row{0, 3, faery_members.data()};
    faery::Tables faery_tables{&faery_list_row, 1, faery_rows.data(), 3};
    faery::Globals faery_globals{&faery_tables, 0};
    faery::Services faery_services{this, get_faery_count, report_faery_assertion};
    ctor::Globals constructor_globals{0};
    ctor::Services constructor_services{this, constructor_invoke, nullptr};
    k::State state{};
    k::Result result{};
    std::array<k::SkillRow, 3> skills{{
        {1, SCRIPT_A}, {0, 0}, {1, SCRIPT_B}
    }};
    std::vector<Event> events;
    std::vector<Event> constructor_events;
    std::vector<std::uintptr_t> faery_query_owners;
    std::uint32_t fail_operation = 0xffffffffu;
    std::uint32_t fail_occurrence = 1;
    std::uint32_t seen_operation = 0;
    std::uint32_t occurrence = 0;
    std::uint32_t next_argument = 0;
    std::uint32_t next_script = 0;
    std::uint32_t constructor_fail = 0;
    std::uint32_t faery_constant_calls = 0;
    std::uint32_t skill_count = 3;
    std::uint32_t faery_count = 3;
    char saved_path[24] = "data/scripts/ai/";
    k::Services services{this, invoke, &constructor_services, &constructor_globals};

    explicit Fixture(std::string name) : scenario(std::move(name)) {
        for (std::uint32_t i = 0; i < faery_rows.size(); ++i) {
            faery_rows[i].words[5] = (i == 0 || i == 2) ? 1u : 0u;
            faery_rows[i].words[6] = i == 0 ? FAERY_A : FAERY_B;
            faery_rows[i].words[8] = i;
        }
        state.ai = AI;
        state.owner = OWNER_A;
        state.assert_level = 0;
        state.skills = {SKILL_VECTOR, nullptr, nullptr, nullptr};
        state.faeries = {FAERY_VECTOR, nullptr, nullptr, nullptr};
        state.faery_binding = {{OWNER_A, 0}, &faery_globals, &faery_services};

        if (scenario == "vectors_nonempty") {
            skill_storage[0] = 0x11110001;
            faery_storage[0] = 0x22220001;
            state.skills.begin = skill_storage.data();
            state.skills.end = skill_storage.data() + 1;
            state.skills.capacity = skill_storage.data() + skill_storage.size();
            state.faeries.begin = faery_storage.data();
            state.faeries.end = faery_storage.data() + 1;
            state.faeries.capacity = faery_storage.data() + faery_storage.size();
        } else if (scenario == "skill_only_empty") {
            faery_storage[0] = 0x22220001;
            state.faeries.begin = faery_storage.data();
            state.faeries.end = faery_storage.data() + 1;
            state.faeries.capacity = faery_storage.data() + faery_storage.size();
        } else if (scenario == "faery_only_empty") {
            skill_storage[0] = 0x11110001;
            state.skills.begin = skill_storage.data();
            state.skills.end = skill_storage.data() + 1;
            state.skills.capacity = skill_storage.data() + skill_storage.size();
        }
        if (scenario == "empty_lists") {
            skill_count = 0;
            faery_count = 0;
            faery_list_row.list_size = 0;
            faery_tables.faery_count = 0;
        }
        if (scenario == "all_gated_null") {
            for (auto& row : skills) row = {0, 0};
            for (auto& row : faery_rows) row.words[5] = 0;
        }
        if (scenario == "fullwidth_faery_name") {
            for (auto& row : skills) row = {0, 0};
            for (auto& row : faery_rows) row.words[5] = 0;
            faery_rows[0].words[5] = 1;
            state.faery_binding.full_width_script_names = &full_width_binding;
        }
        if (scenario == "load_fail") {
            // All named Load calls report false; _commons' result is ignored.
        }
        if (scenario == "fail_call") {
            fail_operation = static_cast<std::uint32_t>(k::Operation::call_script);
            fail_occurrence = 1;
        }
        if (scenario == "fail_child") constructor_fail = 1;
    }

    static const char* event_text(const k::Request& request) {
        return request.text ? request.text : "";
    }

    static std::int32_t get_faery_count(void* opaque, faery::Character* character,
                                        const faery::Request*, faery::Response* response) {
        auto& self = *static_cast<Fixture*>(opaque);
        ++self.faery_constant_calls;
        self.faery_query_owners.push_back(character->identity);
        response->word = static_cast<std::int32_t>(self.faery_list_row.list_size);
        return 0;
    }

    static std::int32_t report_faery_assertion(void*, faery::Character*,
                                                const faery::Request*) { return 0; }

    static std::int32_t constructor_invoke(void* opaque, ctor::State*,
                                           const ctor::Request* request) {
        auto& self = *static_cast<Fixture*>(opaque);
        Event event{};
        event.operation = 0x100u + static_cast<std::uint32_t>(request->operation);
        event.receiver = request->arguments_identity;
        event.name = reinterpret_cast<std::uintptr_t>(request->text);
        event.integer = request->integer;
        // Source script names are stable opaque identities in this fixture;
        // the child constructor forwards them without dereferencing them.
        event.text.clear();
        self.constructor_events.push_back(std::move(event));
        if (self.constructor_fail && request->operation == ctor::Operation::arguments_push_integer)
            return 1;
        return 0;
    }

    static std::int32_t invoke(void* opaque, k::State* state, const k::Request* request,
                               k::Response* response) {
        auto& self = *static_cast<Fixture*>(opaque);
        Event event{};
        event.operation = static_cast<std::uint32_t>(request->operation);
        event.list = static_cast<std::uint32_t>(request->list);
        event.slot = request->slot;
        event.receiver = request->receiver;
        event.name = request->script_name;
        event.arguments = request->arguments;
        event.integer = request->integer;
        event.number_bits = request->number_bits;
        event.allocation_bytes = request->allocation_bytes;
        event.allocation_hint = request->allocation_hint;
        event.text = event_text(*request);
        self.events.push_back(event);
        const auto op = event.operation;
        if (self.fail_operation == op) {
            if (++self.occurrence == self.fail_occurrence) return 9;
        }
        switch (request->operation) {
        case k::Operation::debug_load:
        case k::Operation::debug_get_switch:
        case k::Operation::set_script_path:
        case k::Operation::release_script_path:
        case k::Operation::release_skill_script_allocation:
        case k::Operation::arguments_push_string:
        case k::Operation::arguments_push_integer:
        case k::Operation::arguments_set_string:
        case k::Operation::arguments_set_number:
        case k::Operation::arguments_destroy:
        case k::Operation::load_script:
        case k::Operation::call_script:
        case k::Operation::init_vcb:
            break;
        case k::Operation::capture_script_path:
            response->identity = 0x9900;
            response->path = self.saved_path;
            response->path_size = std::strlen(self.saved_path);
            break;
        case k::Operation::get_skill_list:
            response->count = self.skill_count;
            break;
        case k::Operation::get_skill:
            if (request->slot >= self.skills.size()) return 2;
            response->skill = self.skills[request->slot];
            if (self.scenario == "mutate_owner" && request->slot == 0)
                state->owner = OWNER_B;
            break;
        case k::Operation::get_faery_list:
            response->count = self.faery_count;
            break;
        case k::Operation::get_faery_list_id:
            response->word = 0;
            if (self.scenario == "mutate_faery_owner" && request->slot == 0)
                state->owner = OWNER_B;
            break;
        case k::Operation::reserve: {
            auto& vector = request->list == k::List::skill ? state->skills : state->faeries;
            auto& storage = request->list == k::List::skill ? self.skill_storage : self.faery_storage;
            if (vector.end != vector.begin) return 3;
            if (request->integer) {
                vector.begin = storage.data();
                vector.end = storage.data();
                vector.capacity = storage.data() + storage.size();
            }
            break;
        }
        case k::Operation::arguments_construct:
            response->identity = 0xa000u + (++self.next_argument * 0x100u);
            break;
        case k::Operation::allocate_skill_script:
            if (request->allocation_bytes != 0x1cu || request->allocation_hint != 0)
                return 4;
            response->identity = 0xb000u + (++self.next_script * 0x100u);
            break;
        case k::Operation::append_skill_script: {
            auto& vector = request->list == k::List::skill ? state->skills : state->faeries;
            const auto before = static_cast<std::size_t>(vector.end - vector.begin);
            if (!vector.begin || vector.end == vector.capacity) return 5;
            vector.begin[before] = request->script_name;
            ++vector.end;
            break;
        }
        }
        if (request->operation == k::Operation::load_script) {
            const bool common = std::strcmp(event.text.c_str(), "_commons") == 0;
            response->loaded = !common && self.scenario != "load_fail" ? 1u : 0u;
        }
        return 0;
    }

    k::Status run() { return k::prepare(&state, &services, &result); }
};

void dump(const Fixture& fixture, k::Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"skill_slots\":" << fixture.result.skill_slots
              << ",\"faery_slots\":" << fixture.result.faery_slots
              << ",\"script_allocations\":" << fixture.result.script_allocations
              << ",\"null_appends\":" << fixture.result.null_appends
              << ",\"declarations\":" << fixture.result.declarations
              << ",\"init_vcb_called\":" << fixture.result.init_vcb_called
              << ",\"faery_constant_calls\":" << fixture.faery_constant_calls
              << ",\"events\":[";
    for (std::size_t i = 0; i < fixture.events.size(); ++i) {
        if (i) std::cout << ',';
        const auto& event = fixture.events[i];
        std::cout << '[' << event.operation << ',' << event.list << ',' << event.slot
                  << ',' << event.receiver << ',' << event.name << ',' << event.arguments
                  << ',' << event.integer << ',' << event.number_bits << ','
                  << event.allocation_bytes << ',' << event.allocation_hint << ",\""
                  << event.text << "\"]";
    }
    std::cout << "],\"skill_values\":[";
    const auto skill_count = fixture.state.skills.begin && fixture.state.skills.end
        ? static_cast<std::size_t>(fixture.state.skills.end - fixture.state.skills.begin) : 0;
    for (std::size_t i = 0; i < skill_count; ++i) {
        if (i) std::cout << ',';
        std::cout << fixture.state.skills.begin[i];
    }
    std::cout << "],\"faery_values\":[";
    const auto faery_count = fixture.state.faeries.begin && fixture.state.faeries.end
        ? static_cast<std::size_t>(fixture.state.faeries.end - fixture.state.faeries.begin) : 0;
    for (std::size_t i = 0; i < faery_count; ++i) {
        if (i) std::cout << ',';
        std::cout << fixture.state.faeries.begin[i];
    }
    std::cout << "]}\n";
}

int main(int argc, char** argv) {
    if (argc == 2) {
        Fixture fixture(argv[1]);
        dump(fixture, fixture.run());
        return 0;
    }
    unsigned cases = 0;
    {
        Fixture f("baseline");
        assert(f.run() == k::Status::complete);
        assert(f.result.skill_slots == 3 && f.result.faery_slots == 3);
        assert(f.result.script_allocations == 4 && f.result.null_appends == 2);
        assert(f.result.declarations == 8 && f.result.init_vcb_called == 1);
        assert(f.faery_constant_calls == 6);
        assert(f.events.front().operation == static_cast<std::uint32_t>(k::Operation::debug_load));
        assert(f.events.back().operation == static_cast<std::uint32_t>(k::Operation::release_script_path));
        std::vector<std::uint32_t> declared_slots;
        for (const auto& event : f.events) {
            if (event.operation == static_cast<std::uint32_t>(k::Operation::arguments_set_number))
                declared_slots.push_back(event.number_bits);
        }
        assert((declared_slots == std::vector<std::uint32_t>{0x00000000u, 0x40000000u,
                                                             0xbf800000u, 0xbf800000u}));
        std::vector<std::uint32_t> constructor_slots;
        for (const auto& event : f.constructor_events) {
            if (event.operation == 0x100u + static_cast<std::uint32_t>(ctor::Operation::arguments_push_integer))
                constructor_slots.push_back(event.integer);
        }
        assert((constructor_slots == std::vector<std::uint32_t>{0u, 2u,
                                                               0xffffffffu, 0xffffffffu}));
        ++cases;
    }
    {
        Fixture f("vectors_nonempty");
        assert(f.run() == k::Status::complete);
        for (const auto& event : f.events)
            assert(event.operation != static_cast<std::uint32_t>(k::Operation::get_skill_list) &&
                   event.operation != static_cast<std::uint32_t>(k::Operation::get_faery_list) &&
                   event.operation != static_cast<std::uint32_t>(k::Operation::reserve) &&
                   event.operation != static_cast<std::uint32_t>(k::Operation::arguments_construct));
        assert(f.result.init_vcb_called == 1);
        ++cases;
    }
    {
        Fixture f("skill_only_empty");
        assert(f.run() == k::Status::complete && f.result.skill_slots == 3 && f.result.faery_slots == 0);
        ++cases;
    }
    {
        Fixture f("faery_only_empty");
        assert(f.run() == k::Status::complete && f.result.skill_slots == 0 && f.result.faery_slots == 3);
        ++cases;
    }
    {
        Fixture f("empty_lists");
        assert(f.run() == k::Status::complete && f.result.skill_slots == 0 && f.result.faery_slots == 0);
        assert(f.result.declarations == 0 && f.result.null_appends == 0);
        ++cases;
    }
    {
        Fixture f("all_gated_null");
        assert(f.run() == k::Status::complete && f.result.null_appends == 6);
        assert(f.result.declarations == 0 && f.result.script_allocations == 0);
        ++cases;
    }
    {
        Fixture f("load_fail");
        assert(f.run() == k::Status::complete && f.result.script_allocations == 0);
        assert(f.result.null_appends == 6 && f.result.declarations == 8);
        ++cases;
    }
    {
        Fixture f("mutate_owner");
        assert(f.run() == k::Status::complete);
        auto it = std::find_if(f.events.begin(), f.events.end(), [](const Event& e) {
            return e.operation == static_cast<std::uint32_t>(k::Operation::get_skill) && e.slot == 1;
        });
        assert(it != f.events.end() && it->receiver == OWNER_B);
        ++cases;
    }
    {
        Fixture f("mutate_faery_owner");
        assert(f.run() == k::Status::complete);
        assert(f.faery_query_owners.size() == 6);
        assert(f.faery_query_owners[0] == OWNER_A && f.faery_query_owners[1] == OWNER_A);
        assert(f.faery_query_owners[2] == OWNER_B && f.faery_query_owners[3] == OWNER_B);
        ++cases;
    }
    {
        Fixture f("fail_call");
        assert(f.run() == k::Status::service_failed);
        assert(std::any_of(f.events.begin(), f.events.end(), [](const Event& e) {
            return e.operation == static_cast<std::uint32_t>(k::Operation::arguments_destroy);
        }));
        assert(f.events.back().operation == static_cast<std::uint32_t>(k::Operation::release_script_path));
        ++cases;
    }
    {
        Fixture f("fail_child");
        assert(f.run() == k::Status::child_kernel_failed);
        assert(std::any_of(f.events.begin(), f.events.end(), [](const Event& e) {
            return e.operation == static_cast<std::uint32_t>(k::Operation::release_skill_script_allocation);
        }));
        ++cases;
    }
    {
        Fixture f("baseline");
        f.state.skills = {0, nullptr, nullptr, nullptr};
        assert(f.run() == k::Status::invalid_argument && f.events.empty());
        ++cases;
    }
    {
        Fixture f("baseline");
        f.services.invoke = nullptr;
        assert(f.run() == k::Status::service_unavailable && f.events.empty());
        ++cases;
    }
    {
        Fixture f("fullwidth_faery_name");
        assert(f.run() == k::Status::complete);
        const auto expected = f.full_width_names[0];
        const auto loaded = std::find_if(f.events.begin(), f.events.end(), [](const Event& event) {
            return event.operation == static_cast<std::uint32_t>(k::Operation::load_script) &&
                   event.list == static_cast<std::uint32_t>(k::List::faery) && event.name != 0;
        });
        assert(loaded != f.events.end() && loaded->name == expected);
        const auto called = std::find_if(f.events.begin(), f.events.end(), [](const Event& event) {
            return event.operation == static_cast<std::uint32_t>(k::Operation::call_script) &&
                   event.list == static_cast<std::uint32_t>(k::List::faery);
        });
        assert(called != f.events.end() && called->name == expected);
        ++cases;
    }
    std::cout << "{\"validation\":\"PASS\",\"host_cases\":" << cases
              << ",\"native_wired\":false,\"missing_service_success\":false}\n";
}
