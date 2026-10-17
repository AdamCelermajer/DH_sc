#include "../character_init_post_player_v1.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace init = dh2::character_init_post_player_v1;
namespace {
void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
std::uint32_t read_word(std::istream& input) {
    std::array<unsigned char, 4> bytes{};
    input.read(reinterpret_cast<char*>(bytes.data()), 4);
    check(bool(input), "truncated original ARM fixture");
    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
        (std::uint32_t(bytes[2]) << 16) | (std::uint32_t(bytes[3]) << 24);
}
std::int32_t signed_word(std::uint32_t bits) {
    std::int32_t value{}; std::memcpy(&value, &bits, sizeof(value)); return value;
}
constexpr std::uintptr_t CHARACTER = 0x100000001ull;
constexpr std::uintptr_t PROPERTIES = 0x200000001ull;
constexpr std::uintptr_t SAVE = 0x300000001ull;

struct Fixture {
    struct Trace { std::uint32_t operation, callsite, subject, ordinal; std::int32_t argument; };
    std::int16_t property_id{};
    std::uintptr_t save{};
    std::uint32_t level{}, difficulty{}, local[2]{};
    std::int16_t equipment_property_id{};
    std::vector<Trace> trace;
    init::Runtime* runtime{};
    bool fail_equipment{}, reenter{};
    unsigned source_errors{};

    static std::uint32_t subject_kind(const init::Request& q) {
        if (q.subject == CHARACTER) return 1;
        if (q.subject == PROPERTIES) return 2;
        if (q.subject == SAVE) return 3;
        return 0;
    }
    static int invoke(void* raw, const init::Request& q, init::Reply& reply, std::string& error) {
        auto& f = *static_cast<Fixture*>(raw);
        f.trace.push_back({std::uint32_t(q.operation), q.source_callsite, subject_kind(q),
                           q.query_ordinal, q.argument});
        if (f.reenter) {
            f.reenter = false;
            init::Result nested{}; std::string nested_error;
            check(f.runtime->initialize(&nested, nested_error) == init::Status::busy,
                  "reentrant InitPost did not stop at its owner boundary");
        }
        if (f.fail_equipment && q.operation == init::Operation::init_equipment) {
            error = "fixture blocked the equipment continuation"; return 1;
        }
        switch (q.operation) {
        case init::Operation::load_save_mask:
            check(q.argument == 4 && q.subject == CHARACTER, "SG_Load(4) contract changed"); break;
        case init::Operation::current_level:
            reply.identity = f.level ? 0x400000001ull : 0;
            reply.word = f.difficulty; break;
        case init::Operation::set_difficulty:
            check(q.subject == CHARACTER && q.argument == signed_word(f.difficulty),
                  "current Level difficulty was not forwarded"); break;
        case init::Operation::is_local_player:
            check(q.query_ordinal == 1 || q.query_ordinal == 2, "fresh locality ordinal missing");
            reply.word = f.local[q.query_ordinal - 1]; break;
        case init::Operation::init_equipment:
            check(q.subject == CHARACTER, "equipment Character identity changed");
            f.property_id = f.equipment_property_id; break;
        case init::Operation::reset_gear_properties:
        case init::Operation::load_base_properties:
        case init::Operation::load_gear_properties:
        case init::Operation::recalc_properties:
            check(q.subject == PROPERTIES, "property owner identity changed"); break;
        case init::Operation::quest_sync:
            check(q.subject == SAVE, "quest sync did not freshly read Character+14e8"); break;
        case init::Operation::init_skill_slots:
            check(q.subject == CHARACTER, "skills Character identity changed"); break;
        }
        return 0;
    }
    init::Bindings bindings() {
        return {CHARACTER, PROPERTIES, &property_id, &save, {this, invoke}};
    }
};

void source_cases(const char* path) {
    std::ifstream input(path, std::ios::binary);
    check(bool(input), "original ARM cases file is required");
    check(read_word(input) == 0x31504943u, "original ARM cases magic differs"); // CIP1
    const auto count = read_word(input);
    unsigned cases = 0;
    for (unsigned n = 0; n < count; ++n) {
        std::array<std::uint32_t, 7> in{};
        for (auto& value : in) value = read_word(input);
        const auto expected_count = read_word(input);
        std::vector<Fixture::Trace> expected(expected_count);
        for (auto& row : expected) {
            row.operation = read_word(input); row.callsite = read_word(input);
            row.subject = read_word(input); row.ordinal = read_word(input);
            row.argument = signed_word(read_word(input));
        }
        Fixture fixture;
        fixture.level = in[0]; fixture.difficulty = in[1];
        fixture.local[0] = in[2]; fixture.local[1] = in[3];
        fixture.save = in[4] ? SAVE : 0;
        fixture.property_id = static_cast<std::int16_t>(signed_word(in[5]));
        fixture.equipment_property_id = static_cast<std::int16_t>(signed_word(in[6]));
        init::Runtime runtime(fixture.bindings()); fixture.runtime = &runtime;
        init::Result result{}; std::string error;
        check(runtime.initialize(&result, error) == init::Status::complete, "host InitPost adapter failed");
        check(fixture.trace.size() == expected.size(), "host/original ARM operation count differs");
        for (std::size_t i = 0; i < expected.size(); ++i) {
            const auto& a = fixture.trace[i]; const auto& b = expected[i];
            check(a.operation == b.operation && a.callsite == b.callsite && a.subject == b.subject &&
                  a.ordinal == b.ordinal && a.argument == b.argument,
                  "host/original ARM call order, argument or owner differs");
        }
        check(result.stage == init::Stage::covered_cutoff && result.save_load_calls == 1,
              "adapter did not stop at the pinned cutoff");
        ++cases;
    }
    check(input.peek() == std::char_traits<char>::eof(), "unconsumed original ARM fixture bytes");

    Fixture failed; failed.local[0] = 1; failed.fail_equipment = true;
    init::Runtime failed_runtime(failed.bindings()); failed.runtime = &failed_runtime;
    init::Result result{}; std::string error;
    check(failed_runtime.initialize(&result, error) == init::Status::failed &&
          result.stage == init::Stage::init_equipment && !error.empty() && failed.trace.size() == 4,
          "provider failure lost the reached InitPost prefix");

    Fixture reentry; reentry.reenter = true;
    init::Runtime reentry_runtime(reentry.bindings()); reentry.runtime = &reentry_runtime;
    error.clear();
    check(reentry_runtime.initialize(&result, error) == init::Status::complete,
          "outer InitPost adapter did not complete after rejected reentry");
    std::cout << "character InitPost player block: " << cases << " ARM differential cases, failure-prefix and reentry passed\n";
}
}

int main(int argc, char** argv) {
    try { check(argc == 2, "usage: character_init_post_player_v1_audit original-cases.bin"); source_cases(argv[1]); }
    catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
    return 0;
}
