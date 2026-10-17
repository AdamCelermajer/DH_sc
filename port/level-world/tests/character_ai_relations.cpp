#include "../character_ai_relations.hpp"

#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace ar = dh2::character_ai_relations;
namespace {
constexpr std::uintptr_t ai = 0x2001000, owner = 0x2004000, owner_b = 0x2008000;
constexpr std::uintptr_t object = 0x200c000, character = 0x2010000;
struct Call { unsigned operation; std::uintptr_t subject; std::uintptr_t other; std::int32_t value; };
struct Fixture {
    ar::State state{ai, owner, object};
    ar::Services services{};
    ar::Result result{};
    std::array<dh2::data::AiFactionEntry, 3> entries{{{1, -1}, {2, 1}, {1, 1}}};
    std::array<dh2::data::AiFactionEntry, 1> alternate{{{1, 1}}};
    std::array<ar::FactionRow, 16> rows{}, new_rows{};
    ar::FactionTable table{rows.data(), rows.size()}, new_table{new_rows.data(), new_rows.size()};
    const ar::FactionTable* current_table = &table;
    std::uintptr_t resolved = character;
    std::uint32_t f4 = 0, owner_player = 0, target_player = 0, interactive = 1;
    std::int32_t interaction_type = 8, target_raw = 1, owner_raw = 0, owner_b_raw = 2, count = 16;
    unsigned target_getters = 0, owner_getters = 0, count_reads = 0;
    int mutation = 0, fail_operation = -1, throw_operation = -1;
    std::vector<Call> calls;
    Fixture();
    int record(unsigned op, std::uintptr_t subject, std::uintptr_t other, std::int32_t value) {
        calls.push_back({op, subject, other, value});
        if (static_cast<int>(op) == throw_operation) throw std::runtime_error("service");
        return static_cast<int>(op) == fail_operation;
    }
    ar::Status run(ar::Relation relation, std::uintptr_t candidate = object) {
        return ar::query(relation, &state, candidate, &services, &result);
    }
};
Fixture::Fixture() {
    rows[0] = {entries.data(), static_cast<std::uint32_t>(entries.size()), static_cast<std::uint32_t>(entries.size())};
    rows[2] = rows[0]; rows[10] = rows[0];
    new_rows[0] = {alternate.data(), 1, 1};
    services.context = this;
    services.resolve_object_handle = [](void* context, ar::State* s, std::uintptr_t candidate, std::uintptr_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(candidate == object);
        *out = f.resolved; const auto status = f.record(0, candidate, 0, f.resolved ? 1 : 0);
        if (f.mutation == 1) s->target_40 = 0;
        return status;
    };
    services.read_object_word_f4 = [](void* context, ar::State*, std::uintptr_t c, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(c == character); *out = f.f4;
        return f.record(1, c, 0, static_cast<std::int32_t>(*out));
    };
    services.get_faction_id = [](void* context, ar::State* s, std::uintptr_t c, std::int32_t* out) {
        auto& f = *static_cast<Fixture*>(context);
        assert(c == character || c == owner || c == owner_b);
        const auto raw = c == character ? f.target_raw : c == owner ? f.owner_raw : f.owner_b_raw;
        *out = raw < 0 || raw >= f.count ? 10 : raw;  // Actual getter fallback.
        const auto status = f.record(2, c, 0, *out);
        if (c == character) {
            ++f.target_getters;
            if (f.mutation == 2 && f.target_getters == 1) s->owner = owner_b;
            if (f.mutation == 6 && f.target_getters == 3) f.rows[0] = {f.alternate.data(), 1, 1};
            if (f.mutation == 9 && f.target_getters == 1) f.target_raw = 2;
        } else {
            ++f.owner_getters;
            if (f.mutation == 3 && f.owner_getters == 1) s->owner = owner_b;
        }
        return status;
    };
    services.faction_count = [](void* context, ar::State* s, std::int32_t* out) {
        auto& f = *static_cast<Fixture*>(context); *out = f.count;
        const auto status = f.record(3, 0, 0, *out);
        if (f.mutation == 4 && ++f.count_reads == 2) s->owner = owner_b;
        return status;
    };
    services.is_player = [](void* context, ar::State* s, std::uintptr_t c, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context);
        assert(c == owner || c == owner_b || c == character);
        *out = c == character ? f.target_player : f.owner_player;
        const auto status = f.record(4, c, 0, static_cast<std::int32_t>(*out));
        if (f.mutation == 7 && c != character) s->owner = owner_b;
        return status;
    };
    services.capture_faction_table = [](void* context, ar::State* s, const ar::FactionTable** out) {
        auto& f = *static_cast<Fixture*>(context); *out = f.current_table;
        const auto status = f.record(5, 0, 0, f.current_table == &f.table ? 0 : 1);
        if (f.mutation == 5) { s->owner = owner_b; f.current_table = &f.new_table; }
        return status;
    };
    services.is_interactive = [](void* context, ar::State* s, std::uintptr_t candidate,
                                 std::uintptr_t o, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(candidate == object); *out = f.interactive;
        const auto status = f.record(6, candidate, o, static_cast<std::int32_t>(*out));
        if (f.mutation == 8) s->owner = owner_b;
        return status;
    };
    services.interaction_type = [](void* context, ar::State*, std::uintptr_t candidate,
                                   std::uintptr_t o, std::int32_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(candidate == object); *out = f.interaction_type;
        return f.record(7, candidate, o, *out);
    };
}
void emit(const Fixture& f, ar::Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status) << ",\"value\":" << f.result.value << ",\"calls\":[";
    for (std::size_t i = 0; i < f.calls.size(); ++i) {
        if (i) std::cout << ',';
        const auto& c = f.calls[i];
        std::cout << '[' << c.operation << ',' << c.subject << ',' << c.other << ',' << c.value << ']';
    }
    std::cout << "]}\n";
}
}  // namespace

int main(int argc, char** argv) {
    if (argc == 12 || argc == 14) {
        Fixture f;
        const auto relation = static_cast<ar::Relation>(std::stoul(argv[1]));
        const auto candidate = std::stoi(argv[2]) ? object : 0;
        if (!std::stoi(argv[3])) f.state.target_40 = 0;
        if (!std::stoi(argv[4])) f.resolved = 0;
        f.f4 = std::stoul(argv[5]); f.owner_player = std::stoul(argv[6]); f.target_player = std::stoul(argv[7]);
        f.interactive = std::stoul(argv[8]); f.interaction_type = std::stoi(argv[9]);
        f.entries[0].value = std::stoi(argv[10]); f.mutation = std::stoi(argv[11]);
        if (argc == 14) { f.owner_raw = std::stoi(argv[12]); f.target_raw = std::stoi(argv[13]); }
        const auto status = f.run(relation, candidate); emit(f, status); return 0;
    }
    assert(argc == 1);
    unsigned cases = 0;
    auto check = [&](Fixture& f, ar::Relation r, bool value, std::uintptr_t candidate = object) {
        assert(f.run(r, candidate) == ar::Status::complete && f.result.value == value); ++cases;
    };
    for (auto r : {ar::Relation::enemy, ar::Relation::friend_, ar::Relation::neutral}) {
        for (int value : {-1, 0, 1}) {
            Fixture f; f.entries[0].value = value;
            check(f, r, r == ar::Relation::enemy ? value < 0 : r == ar::Relation::friend_ ? value > 0 : value == 0);
            assert(f.result.faction_getter_calls == 6 && f.result.faction_count_reads == 2);
        }
        { Fixture f; f.rows[0].count = 0; check(f, r, r == ar::Relation::neutral); }
        { Fixture f; f.target_raw = 3; check(f, r, r == ar::Relation::neutral); }
        {
            Fixture f; f.state.target_40 = 0; f.services = {};
            check(f, r, r == ar::Relation::neutral, 0); assert(f.calls.empty());
        }
        { Fixture f; check(f, r, r == ar::Relation::enemy, 0); assert(f.result.candidate == object); }
        { Fixture f; f.mutation = 1; check(f, r, r == ar::Relation::enemy); assert(f.result.candidate == object); }
        for (int mode : {2, 3, 4, 5, 6, 9}) {
            Fixture f; f.mutation = mode;
            const bool positive = mode == 6 || mode == 9;
            check(f, r, r == ar::Relation::enemy ? !positive : r == ar::Relation::friend_ ? positive : false);
            if (mode == 5) assert(f.calls[f.calls.size() - 2].subject == owner);
        }
    }
    for (auto r : {ar::Relation::friend_, ar::Relation::neutral}) {
        Fixture f; f.resolved = 0; f.services.get_faction_id = nullptr;
        check(f, r, r == ar::Relation::neutral); assert(f.result.word_f4_reads == 0);
        Fixture g; g.f4 = 7; check(g, r, r == ar::Relation::neutral); assert(g.result.faction_getter_calls == 0);
    }
    for (int type : {7, 8, 9}) {
        Fixture f; f.resolved = 0; f.interaction_type = type;
        check(f, ar::Relation::enemy, type == 8); assert(f.result.faction_getter_calls == 0);
    }
    {
        Fixture f; f.f4 = 1; f.mutation = 8;
        check(f, ar::Relation::enemy, true);
        assert(f.calls[2].other == owner && f.calls[3].other == owner_b && f.calls[3].subject == object);
    }
    {
        Fixture f; f.resolved = 0; f.interactive = 0; f.services.interaction_type = nullptr;
        check(f, ar::Relation::enemy, false); assert(!f.result.interaction_type_queries);
    }
    {
        Fixture f; f.owner_player = 7; f.target_player = 9; f.services.capture_faction_table = nullptr;
        check(f, ar::Relation::enemy, false); assert(f.result.player_queries == 2 && !f.result.table_captures);
    }
    { Fixture f; f.owner_player = 0; f.target_player = 1; check(f, ar::Relation::enemy, true); assert(f.result.player_queries == 1); }
    { Fixture f; f.owner_player = f.target_player = 1; check(f, ar::Relation::friend_, false); assert(!f.result.player_queries); }
    { Fixture f; f.mutation = 7; check(f, ar::Relation::enemy, true); assert(f.calls[f.calls.size() - 2].subject == owner_b); }
    { Fixture f; f.owner_raw = -1; check(f, ar::Relation::enemy, true); assert(f.calls[5].value == 10); }
    {
        Fixture f; f.fail_operation = 5; f.mutation = 5;
        assert(f.run(ar::Relation::enemy) == ar::Status::service_failed && f.state.owner == owner_b && f.current_table == &f.new_table); ++cases;
    }
    {
        Fixture f; f.throw_operation = 2;
        assert(f.run(ar::Relation::enemy) == ar::Status::service_failed && f.result.faction_getter_calls == 1); ++cases;
    }
    {
        Fixture f; f.services.resolve_object_handle = nullptr;
        assert(f.run(ar::Relation::enemy) == ar::Status::service_unavailable && f.calls.empty()); ++cases;
    }
    {
        Fixture f; f.count = 0;
        assert(f.run(ar::Relation::enemy) == ar::Status::invalid_source_fact && f.result.faction_getter_calls == 2); ++cases;
    }
    {
        Fixture f; f.rows[0].count = 4;
        assert(f.run(ar::Relation::enemy) == ar::Status::invalid_source_fact); ++cases;
    }
    {
        Fixture f; f.result.value = 123;
        assert(ar::query(ar::Relation::enemy, &f.state, object, &f.services,
                         reinterpret_cast<ar::Result*>(&f.state)) == ar::Status::invalid_argument);
        assert(ar::query(static_cast<ar::Relation>(3), &f.state, object, &f.services, &f.result) == ar::Status::invalid_argument);
        assert(ar::query(ar::Relation::enemy, reinterpret_cast<ar::State*>(reinterpret_cast<char*>(&f.state) + 1),
                         object, &f.services, &f.result) == ar::Status::invalid_argument);
        assert(f.result.value == 123 && f.calls.empty()); ++cases;
    }
    std::cout << "{\"ai_relation_cases\":" << cases
              << ",\"enemy_faction_leaf_reused\":true,\"all_three_source_predicates\":true,"
                 "\"null_target_and_noncharacter_branches\":true,\"fresh_owner_faction_and_table_reads\":true,"
                 "\"row_read_after_target_getter\":true,\"partial_failure_and_alias_guards\":true,"
                 "\"mismatches\":0,\"native_wired\":false}\n";
}
