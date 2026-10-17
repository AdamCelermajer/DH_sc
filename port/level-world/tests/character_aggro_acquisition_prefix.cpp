#include "../character_aggro_acquisition_prefix.hpp"

#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace ap = dh2::character_aggro_acquisition_prefix;
namespace {
constexpr std::uintptr_t ai = 0x10010000, owner = 0x10014000, owner_b = 0x10018000;
constexpr std::uint32_t ten = 0x41200000, twenty = 0x41a00000;
constexpr std::uint32_t fifty = 0x42480000, sixty = 0x42700000;
struct Call { unsigned op; std::uintptr_t subject; std::uint32_t value; };
struct Fixture {
    ap::State state{ai, owner, {0, 0}};
    dh2_random_state random{{123, 0x76543210}, {4, 0x01234567}};
    ap::Result result{};
    std::array<ap::AiPropsRow, 16> rows{}, new_rows{};
    ap::AiPropsTable table{rows.data(), rows.size()}, new_table{new_rows.data(), new_rows.size()};
    const ap::AiPropsTable* current_table = &table;
    // p1,p2,preFaerie,NPC,Monster,Remote,target408,postFaerie,target418,
    // rawAIId,Awaiting,HasAggro,spawn radius word,turn,debug,countdown,elapsed,mutation.
    std::array<std::uint32_t, 18> words{{0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0}};
    unsigned players = 0, faeries = 0, deltas = 0;
    int fail = -1, throwing = -1;
    std::vector<Call> calls;
    ap::Services services{this, invoke, capture};
    Fixture() {
        for (auto& row : rows) row = {ten, twenty};
        for (auto& row : new_rows) row = {0x41f00000, 0x42200000};
    }
    static std::int32_t invoke(void* context, ap::State* state, ap::Query query,
                               std::uintptr_t subject, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context);
        const auto op = static_cast<unsigned>(query);
        const auto mutation = f.words[17];
        unsigned index = 0;
        switch (query) {
            case ap::Query::is_player: index = f.players++ ? 1 : 0; break;
            case ap::Query::is_faerie: index = f.faeries++ ? 7 : (f.players == 2 && !f.words[1] ? 2 : 7); break;
            case ap::Query::is_npc: index = 3; break;
            case ap::Query::is_monster: index = 4; break;
            case ap::Query::is_remotely_updated: index = 5; break;
            case ap::Query::target_408_present: index = 6; break;
            case ap::Query::target_418_present: index = 8; break;
            case ap::Query::get_char_ai_id: index = 9; break;
            case ap::Query::state_awaiting_to_spawn: index = 10; break;
            case ap::Query::has_aggro: index = 11; break;
            case ap::Query::spawn_radius_143c: index = 12; break;
            case ap::Query::is_my_turn: index = 13; break;
            case ap::Query::disable_optimization: index = 14; break;
            case ap::Query::frame_delta: *out = f.deltas++ ? 19 : 16; break;
        }
        if (query != ap::Query::frame_delta) *out = f.words[index];
        if (query == ap::Query::get_char_ai_id && (*out & 0x80000000 || *out >= 16)) *out = 8;
        f.calls.push_back({op, subject, *out});
        if ((mutation == 1 && query == ap::Query::is_player && f.players == 1) ||
            (mutation == 2 && query == ap::Query::is_player && f.players == 2) ||
            (mutation == 3 && query == ap::Query::is_faerie && f.faeries == 1) ||
            (mutation == 4 && query == ap::Query::is_npc) ||
            (mutation == 5 && query == ap::Query::is_monster) ||
            (mutation == 6 && query == ap::Query::is_remotely_updated) ||
            (mutation == 7 && query == ap::Query::target_408_present) ||
            (mutation == 8 && query == ap::Query::is_faerie && f.faeries == 2) ||
            (mutation == 9 && query == ap::Query::target_418_present) ||
            (mutation == 15 && query == ap::Query::is_my_turn)) state->owner = owner_b;
        if (mutation == 11 && query == ap::Query::get_char_ai_id) f.rows[*out].aggro_radius_3c = fifty;
        if (mutation == 12 && query == ap::Query::state_awaiting_to_spawn) {
            f.rows[0] = {fifty, sixty}; state->owner = owner_b;
        }
        if (mutation == 13 && query == ap::Query::has_aggro) {
            f.rows[0].view_radius_40 = sixty; state->owner = owner_b;
        }
        if (mutation == 14 && query == ap::Query::spawn_radius_143c) state->owner = owner_b;
        if (static_cast<int>(op) == f.throwing) throw std::runtime_error("provider");
        return static_cast<int>(op) == f.fail;
    }
    static std::int32_t capture(void* context, ap::State* state, const ap::AiPropsTable** out) {
        auto& f = *static_cast<Fixture*>(context); *out = f.current_table;
        f.calls.push_back({14, 0, f.current_table == &f.table ? 0u : 1u});
        if (f.words[17] == 10) { state->owner = owner_b; f.current_table = &f.new_table; }
        if (f.throwing == 14) throw std::runtime_error("table");
        return f.fail == 14;
    }
    ap::Status run(dh2_random_state* rng = nullptr) {
        state.timing = {words[15], words[16]};
        return ap::prepare(&state, rng ? rng : &random, &services, &result);
    }
};
void emit(const Fixture& f, ap::Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"decision\":" << static_cast<unsigned>(f.result.decision)
              << ",\"radius\":" << f.result.radius_word << ",\"list_owner\":" << f.result.list_owner
              << ",\"owner\":" << f.state.owner << ",\"countdown\":" << f.state.timing.countdown_ms
              << ",\"elapsed\":" << f.state.timing.elapsed_not_turn_ms
              << ",\"seed\":" << f.random.seeds[0] << ",\"counter\":" << f.random.counters[0]
              << ",\"sync_seed\":" << f.random.seeds[1] << ",\"sync_counter\":" << f.random.counters[1]
              << ",\"calls\":[";
    for (std::size_t i = 0; i < f.calls.size(); ++i) {
        if (i) std::cout << ',';
        const auto& c = f.calls[i]; std::cout << '[' << c.op << ',' << c.subject << ',' << c.value << ']';
    }
    std::cout << "]}\n";
}
}  // namespace

int main(int argc, char** argv) {
    if (argc == 19) {
        Fixture f;
        for (unsigned i = 0; i < f.words.size(); ++i) f.words[i] = std::uint32_t(std::strtoull(argv[i + 1], nullptr, 0));
        const auto status = f.run(); emit(f, status); return 0;
    }
    assert(argc == 1);
    unsigned cases = 0;
    auto ready = [&](Fixture& f, std::uint32_t radius) {
        assert(f.run() == ap::Status::complete && f.result.decision == ap::Decision::ready_normal_acquisition);
        assert(f.result.radius_word == radius); ++cases;
    };
    { Fixture f; ready(f, twenty); assert(f.result.timing.debug_queries == 1); }
    { Fixture f; f.words[11] = 7; ready(f, ten); }
    { Fixture f; f.words[0] = 7; f.services.capture_ai_props = nullptr;
      assert(ap::prepare(&f.state, nullptr, &f.services, &f.result) == ap::Status::complete &&
             f.result.decision == ap::Decision::skip_player && f.calls.size() == 1); ++cases; }
    { Fixture f; f.words[1] = 9; ready(f, twenty); assert(f.result.timing.turn_queries == 0); }
    { Fixture f; f.words[2] = 1; ready(f, twenty); assert(f.result.timing.turn_queries == 0); }
    { Fixture f; f.words[3] = 1; assert(f.run() == ap::Status::complete && f.result.decision == ap::Decision::skip_npc); ++cases; }
    { Fixture f; f.words[6] = 1; assert(f.run() == ap::Status::unsupported_branch &&
       f.result.decision == ap::Decision::monster_target_branch && !f.result.table_captures); ++cases; }
    { Fixture f; f.words[4] = 0; f.words[6] = 1; ready(f, twenty); }
    { Fixture f; f.words[5] = 1; f.words[6] = 1; ready(f, twenty); }
    { Fixture f; f.words[7] = f.words[8] = 1;
      assert(f.run() == ap::Status::complete && f.result.decision == ap::Decision::skip_faerie_target); ++cases; }
    { Fixture f; f.words[13] = 0; f.words[14] = 0;
      assert(f.run() == ap::Status::complete && f.result.decision == ap::Decision::waiting_turn &&
             f.state.timing.elapsed_not_turn_ms == 19); ++cases; }
    { Fixture f; f.words[14] = 0; f.words[15] = 17;
      assert(f.run() == ap::Status::complete && f.result.decision == ap::Decision::waiting_delay &&
             f.state.timing.countdown_ms == 1); ++cases; }
    { Fixture f; f.words[14] = 0; ready(f, twenty); assert(f.result.timing.ordinary_random_draws == 1); }
    for (auto word : {0u, 0x80000000u, 0xbf800000u, 0x7fc00000u, 0xff800000u}) {
        Fixture f; f.words[10] = 1; f.words[12] = word; ready(f, twenty);
    }
    for (auto word : {0x3f800000u, 0x7f800000u}) {
        Fixture f; f.words[10] = 1; f.words[12] = word; ready(f, word);
        assert(f.calls.back().op == unsigned(ap::Query::spawn_radius_143c));
    }
    for (unsigned mode = 1; mode <= 15; ++mode) {
        Fixture f; f.words[17] = mode;
        if (mode == 9) f.words[7] = 1;
        if (mode == 14) { f.words[10] = 1; f.words[12] = 0x3f800000; }
        ready(f, mode == 12 || mode == 13 ? sixty : mode == 14 ? 0x3f800000 : twenty);
        if (mode == 10) assert(f.current_table == &f.new_table && f.result.radius_word == twenty);
        if (mode == 14) assert(f.result.list_owner == owner && f.state.owner == owner_b);
    }
    { Fixture f; f.words[17] = 12; f.words[11] = 1; ready(f, ten); }
    { Fixture f; f.words[17] = 11; f.words[11] = 1; ready(f, fifty); }
    { Fixture f; f.words[9] = 0xffffffff; ready(f, twenty); }
    { Fixture f; f.words[9] = 99; ready(f, twenty); }
    { Fixture f; f.fail = 14; f.words[17] = 10;
      assert(f.run() == ap::Status::service_failed && f.state.owner == owner_b && f.current_table == &f.new_table); ++cases; }
    { Fixture f; f.throwing = unsigned(ap::Query::is_npc);
      assert(f.run() == ap::Status::service_failed && f.result.timing.debug_queries == 1); ++cases; }
    { Fixture f; f.fail = unsigned(ap::Query::disable_optimization); f.words[16] = 41;
      assert(f.run() == ap::Status::service_failed && f.state.timing.elapsed_not_turn_ms == 0); ++cases; }
    { Fixture f; f.services.capture_ai_props = nullptr;
      assert(f.run() == ap::Status::service_unavailable); ++cases; }
    { Fixture f; f.table.capacity = 0;
      assert(f.run() == ap::Status::invalid_source_fact); ++cases; }
    { Fixture f; f.result.radius_word = 123;
      assert(ap::prepare(&f.state, &f.random, &f.services, reinterpret_cast<ap::Result*>(&f.state)) == ap::Status::invalid_argument);
      assert(ap::prepare(reinterpret_cast<ap::State*>(reinterpret_cast<char*>(&f.state) + 1),
                         &f.random, &f.services, &f.result) == ap::Status::invalid_argument);
      assert(f.calls.empty() && f.result.radius_word == 123); ++cases; }
    std::cout << "{\"validation\":\"PASS\",\"acquisition_prefix_cases\":" << cases
              << ",\"delay_kernel_reused\":true,\"fresh_owner_and_table_phases\":true,"
                 "\"float_override_guards\":true,\"unsupported_retarget_explicit\":true,"
                 "\"native_wired\":false,\"mismatches\":0}\n";
}
