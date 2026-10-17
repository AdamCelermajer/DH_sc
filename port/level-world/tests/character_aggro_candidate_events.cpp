#include "../character_aggro_candidate_events.hpp"

#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace ce = dh2::character_aggro_candidate_events;
namespace ts = dh2::character::aggro_search;
namespace {
constexpr std::uintptr_t ai = 0x2001000, owner = 0x2004000, owner_b = 0x2008000;
constexpr std::uintptr_t identity40 = 0x2020000, new40 = 0x2024000;
constexpr std::array<std::uintptr_t, 3> ids{0x200c000, 0x2010000, 0x2014000};
struct Call {
    unsigned operation;
    std::uintptr_t owner, candidate;
    unsigned count;
};
struct Fixture {
    ts::GameObject owner_object{};
    ts::Character owner_character{};
    std::array<ts::GameObject, 3> objects{};
    std::array<ts::Character, 3> characters{};
    std::array<ts::TargetInfo, 8> heap{};
    ts::TargetList list{};
    ce::State state{ai, owner, identity40};
    ce::Services services{};
    ce::Result result{};
    std::array<unsigned, 3> masks{1, 2, 4};
    std::vector<Call> calls;
    int mutation = 0, fail_operation = -1, throw_operation = -1;
    std::uintptr_t fail_candidate = 0;
    bool nested_once = false;
    Fixture();
    unsigned index(std::uintptr_t value) const {
        for (unsigned i = 0; i < ids.size(); ++i) if (ids[i] == value) return i;
        throw std::runtime_error("Character resolution metadata used as source identity");
    }
    void add(unsigned count) {
        assert(count <= 3);
        list.count = count;
        for (unsigned i = 0; i < count; ++i)
            heap[i] = {ids[i], ids[i] + 0x10000000, static_cast<float>(i + 1), 0, 1, 0};
    }
    ce::Status run() { return ce::consume(&state, &list, &services, &result); }
    bool fails(unsigned operation, std::uintptr_t candidate) const {
        return static_cast<int>(operation) == fail_operation &&
               (!fail_candidate || fail_candidate == candidate);
    }
};
Fixture::Fixture() {
    owner_object.identity = owner;
    owner_object.forward[0] = 1;
    owner_object.visible = 1;
    owner_character = {owner, &owner_object, 0, 10};
    for (unsigned i = 0; i < ids.size(); ++i) {
        objects[i].identity = ids[i];
        objects[i].position[0] = static_cast<float>(i + 1);
        objects[i].visible = 1;
        characters[i] = {ids[i] + 0x10000000, &objects[i], 0, 0};
    }
    assert(ts::dh2_aggro_target_list_init(&list, heap.data(), heap.size(), &owner_character) == 0);
    services.context = this;
    services.classify = [](void* context, ce::State* state, ce::Relation relation,
                           std::uintptr_t current_owner, std::uintptr_t candidate,
                           std::uint32_t* output) {
        auto& f = *static_cast<Fixture*>(context);
        const unsigned operation = static_cast<unsigned>(relation);
        const auto idx = f.index(candidate);
        assert(f.list.count && f.list.heap[0].object_identity == candidate);
        f.calls.push_back({operation, current_owner, candidate, f.list.count});
        if (f.throw_operation == static_cast<int>(operation)) throw std::runtime_error("classify");
        *output = f.masks[idx] & (1u << operation) ? 7 : 0;  // Raw source truthiness.
        if (f.mutation == 1 && relation == ce::Relation::enemy) state->owner = owner_b;
        if (f.mutation == 5 && !f.nested_once) {
            f.nested_once = true;
            Fixture child; child.add(1); child.masks[0] = 2; child.mutation = 2;
            assert(ce::consume(state, &child.list, &child.services, &child.result) == ce::Status::complete);
            assert(child.list.count == 0 && child.result.friend_events == 1 && child.result.source_event_12 == 1);
            assert(f.list.count && f.list.heap[0].object_identity == candidate);
        }
        return f.fails(operation, candidate) ? 1 : 0;
    };
    services.raise_event = [](void* context, ce::State* state, std::uintptr_t current_owner,
                             std::uint32_t event, std::uintptr_t payload) {
        auto& f = *static_cast<Fixture*>(context);
        assert(event == 7 || event == 8 || event == 9 || event == 12);
        if (event != 12) assert(f.list.count && f.list.heap[0].object_identity == payload);
        else assert(f.list.count == 0 && payload == state->source_identity_40);
        f.calls.push_back({event, current_owner, payload, f.list.count});
        if (f.mutation == 2 && event != 12) { state->owner = owner_b; state->source_identity_40 = new40; }
        if (f.mutation == 3 && event == 9) state->source_identity_40 = new40;
        if (f.mutation == 4) f.services = {};
        if (f.throw_operation == static_cast<int>(event)) throw std::runtime_error("raise");
        return f.fails(event, payload) ? 1 : 0;
    };
}
std::vector<unsigned> events(const Fixture& f) {
    std::vector<unsigned> output;
    for (const auto& call : f.calls) if (call.operation >= 7) output.push_back(call.operation);
    return output;
}
int search_service(void* context, const ts::Request* request, ts::Response* response) {
    auto& f = *static_cast<Fixture*>(context);
    *response = {};
    if (request->operation == ts::ai_melee_radius) return 0;
    const auto i = f.index(request->subject);
    switch (request->operation) {
        case ts::resolve_character: response->word = reinterpret_cast<std::uintptr_t>(&f.characters[i]); return 0;
        case ts::is_zonable: return 0;
        case ts::is_interactive: response->word = 1; return 0;
        case ts::interaction_radius: return 0;
        default: return 1;
    }
}
void emit(const Fixture& f, ce::Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status) << ",\"remaining\":" << f.list.count
              << ",\"consumed\":" << f.result.candidates_consumed << ",\"calls\":[";
    for (std::size_t i = 0; i < f.calls.size(); ++i) {
        if (i) std::cout << ',';
        const auto& c = f.calls[i];
        std::cout << '[' << c.operation << ',' << c.owner << ',' << c.candidate << ',' << c.count << ']';
    }
    std::cout << "]}\n";
}
}  // namespace

int main(int argc, char** argv) {
    if (argc == 7) {
        Fixture f; f.add(static_cast<unsigned>(std::stoul(argv[1])));
        for (unsigned i = 0; i < 3; ++i) f.masks[i] = static_cast<unsigned>(std::stoul(argv[i + 2]));
        f.state.source_identity_40 = std::stoul(argv[5]);
        f.mutation = std::stoi(argv[6]);
        const auto status = f.run(); emit(f, status); return 0;
    }
    assert(argc == 1);
    unsigned cases = 0;
    auto complete = [&](Fixture& f) {
        assert(f.run() == ce::Status::complete && f.list.count == 0); ++cases;
    };
    for (unsigned mask : {1, 2, 4, 0, 7, 6}) {
        Fixture f; f.add(1); f.masks[0] = mask; complete(f);
        const auto e = events(f);
        if (mask & 1) assert(e == std::vector<unsigned>{9} && f.result.relationship_queries == 1);
        else if (mask & 2) assert((e == std::vector<unsigned>{7, 12}) && f.result.relationship_queries == 2);
        else if (mask & 4) assert((e == std::vector<unsigned>{8, 12}) && f.result.relationship_queries == 3);
        else assert((e == std::vector<unsigned>{12}) && f.result.relationship_queries == 3);
        assert(f.result.candidates_consumed == 1);
    }
    {
        Fixture f; f.add(3); f.masks = {1, 1, 1}; complete(f);
        assert((events(f) == std::vector<unsigned>{9, 9, 9}));
        assert(f.result.enemy_events == 3 && !f.result.source_event_12 && f.result.candidates_consumed == 3);
    }
    {
        Fixture f;
        ts::ObjectEntry sentinel{}, a{}, b{}, c{};
        sentinel = {&c, nullptr}; c = {&b, &f.objects[2]};
        b = {&a, &f.objects[1]}; a = {&sentinel, &f.objects[0]};
        ts::Room end{}, room{&end, &sentinel}; end = {&room, nullptr};
        const ts::RoomRegistry registry{&end};
        const ts::Services source{&f, search_service};
        assert(ts::dh2_aggro_target_search(&f.list, &registry, 10, 6.2831854820251465f, &source) == 0);
        assert(f.list.count == 3); complete(f);
        assert((events(f) == std::vector<unsigned>{9, 7, 8}));
        assert(f.calls[0].candidate == ids[0] && f.calls[2].candidate == ids[1]);
    }
    { Fixture f; f.services.classify = nullptr; complete(f); assert((events(f) == std::vector<unsigned>{12})); }
    {
        Fixture f; f.state.source_identity_40 = 0; f.services = {};
        complete(f); assert(f.calls.empty() && !f.result.source_event_12);
    }
    {
        Fixture f; f.add(1); f.masks[0] = 4; f.mutation = 1; complete(f);
        assert(f.calls[0].owner == owner && f.calls[1].owner == owner_b && f.calls[2].owner == owner_b);
        assert(f.calls[3].owner == owner_b && f.calls[3].operation == 8);
    }
    { Fixture f; f.add(1); f.mutation = 1; complete(f); assert(f.calls[1].owner == owner_b); }
    {
        Fixture f; f.add(1); f.masks[0] = 2; f.mutation = 2; complete(f);
        assert(f.calls.back().operation == 12 && f.calls.back().candidate == new40 && f.calls.back().owner == owner_b);
    }
    { Fixture f; f.add(3); f.mutation = 4; complete(f); assert(f.result.candidates_consumed == 3); }
    {
        Fixture f; f.add(1); f.mutation = 5; complete(f);
        assert(f.calls[1].operation == 9 && f.calls[1].owner == owner_b && f.nested_once);
    }
    {
        Fixture f; f.add(2); f.fail_operation = 0; f.fail_candidate = ids[1];
        assert(f.run() == ce::Status::service_failed && f.list.count == 1 && f.heap[0].object_identity == ids[1]);
        assert(f.result.candidates_consumed == 1 && f.result.enemy_events == 1); ++cases;
    }
    {
        Fixture f; f.add(1); f.fail_operation = 9; f.mutation = 3;
        assert(f.run() == ce::Status::service_failed && f.list.count == 1 && f.state.source_identity_40 == new40);
        assert(!f.result.candidates_consumed && !f.result.enemy_events); ++cases;
    }
    {
        Fixture f; f.add(1); f.throw_operation = 9; f.mutation = 3;
        assert(f.run() == ce::Status::service_failed && f.list.count == 1 && f.state.source_identity_40 == new40);
        assert(!f.result.candidates_consumed); ++cases;
    }
    {
        Fixture f; f.add(1); f.throw_operation = 0;
        assert(f.run() == ce::Status::service_failed && f.list.count == 1 && f.result.relationship_queries == 1); ++cases;
    }
    {
        Fixture f; f.add(1); f.masks[0] = 2; f.fail_operation = 12;
        assert(f.run() == ce::Status::service_failed && f.list.count == 0 && f.result.candidates_consumed == 1);
        assert(f.result.friend_events == 1 && !f.result.source_event_12); ++cases;
    }
    {
        Fixture f; f.add(1); f.services.classify = nullptr;
        assert(f.run() == ce::Status::service_unavailable && f.list.count == 1 && f.calls.empty()); ++cases;
    }
    {
        Fixture f; f.add(1); f.services.raise_event = nullptr;
        assert(f.run() == ce::Status::service_unavailable && f.list.count == 1 && f.calls.size() == 1); ++cases;
    }
    auto malformed = [&](auto mutate) {
        Fixture f; f.add(1); f.result.candidates_consumed = 123; mutate(f);
        assert(f.run() == ce::Status::invalid_argument && f.calls.empty() && f.result.candidates_consumed == 123); ++cases;
    };
    malformed([](Fixture& f) { f.list.capacity = 0; });
    malformed([](Fixture& f) { f.list.capacity = 65537; });
    malformed([](Fixture& f) { f.list.count = f.list.capacity + 1; });
    malformed([](Fixture& f) { f.list.owner = nullptr; });
    malformed([](Fixture& f) { f.owner_character.object = nullptr; });
    malformed([](Fixture& f) { f.heap[0].flags = 0; });
    malformed([](Fixture& f) { f.heap[0].object_identity = 0; });
    malformed([](Fixture& f) { f.list.heap = reinterpret_cast<ts::TargetInfo*>(reinterpret_cast<char*>(f.heap.data()) + 1); });
    malformed([](Fixture& f) { f.list.owner = reinterpret_cast<ts::Character*>(&f.result); });
    malformed([](Fixture& f) { f.owner_character.object = reinterpret_cast<ts::GameObject*>(&f.result); });
    {
        Fixture f; f.result.candidates_consumed = 123;
        assert(ce::consume(reinterpret_cast<ce::State*>(reinterpret_cast<char*>(&f.state) + 1),
                           &f.list, &f.services, &f.result) == ce::Status::invalid_argument);
        assert(ce::consume(&f.state, &f.list, &f.services, reinterpret_cast<ce::Result*>(&f.state)) == ce::Status::invalid_argument);
        assert(f.calls.empty() && f.result.candidates_consumed == 123); ++cases;
    }
    std::cout << "{\"aggro_candidate_event_cases\":" << cases
              << ",\"source_object_identity_preserved\":true,\"nearest_search_heap_reused\":true,"
                 "\"all_candidates_no_first_enemy_break\":true,\"enemy_friend_neutral_precedence\":true,"
                 "\"event_precedes_pop\":true,\"fresh_owner_and_source40\":true,"
                 "\"independent_list_reentry\":true,\"partial_failures_and_atomic_guards\":true,"
                 "\"mismatches\":0,\"native_wired\":false}\n";
}
