#include "character_level_member.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

using namespace dh2::character_level_member;

static IntMember member(std::int32_t value, std::uint64_t revision,
                        std::uint32_t stamp, std::uint8_t changed = 0) {
    IntMember result{};
    result.size_bits = 16;
    result.revision = revision;
    result.field_10 = 0x11111111u;
    result.field_14 = 0x22222222u;
    result.source_stamp = stamp;
    result.changed = changed;
    result.value = value;
    return result;
}

static int host_cases() {
    std::uint32_t cases = 0;
    {
        auto m = member(12, 8, 0xabcdef01u, 0);
        std::uint64_t serial = 19;
        Result r{};
        if (set_value(&m, &serial, 12, &r) != Status::complete ||
            r.member_marked || r.member_value_changed || serial != 19 ||
            m.value != 12 || m.revision != 8 || m.changed != 0 ||
            m.field_10 != 0x11111111u || m.field_14 != 0x22222222u)
            return 1;
        ++cases;
    }
    {
        auto m = member(-1, 8, 0xabcdef01u, 0);
        std::uint64_t serial = 19;
        Result r{};
        if (set_value(&m, &serial, 300, &r) != Status::complete ||
            r.member_marked != 1 || r.member_value_changed != 1 ||
            serial != 20 || m.value != 300 || m.revision != 19 ||
            m.changed != 1 || m.field_10 != 0xabcdef01u ||
            m.field_14 != 0xabcdef01u)
            return 2;
        ++cases;
    }
    {
        auto m = member(1, 0, 0xfedcba98u);
        std::uint64_t serial = std::numeric_limits<std::uint64_t>::max();
        Result r{};
        if (set_value(&m, &serial, 2, &r) != Status::complete ||
            serial != 0 || m.revision != UINT64_MAX || m.value != 2)
            return 3;
        ++cases;
    }
    {
        auto m = member(7, 31, 99);
        std::uint64_t serial = 100;
        State state{&m, &serial};
        Result r{};
        if (set_character_level(&state, 7, 7, &r) != Status::complete ||
            r.temporary_marked || r.member_marked || m.value != 7 ||
            serial != 100)
            return 4;
        ++cases;
    }
    {
        auto m = member(-1, 31, 99);
        std::uint64_t serial = 100;
        State state{&m, &serial};
        Result r{};
        if (set_character_level(&state, 7, 0, &r) != Status::complete ||
            r.temporary_marked != 1 || r.member_marked != 1 ||
            r.member_value_after != 7 || m.revision != 101 || serial != 102 ||
            m.field_10 != 99 || m.field_14 != 99)
            return 5;
        ++cases;
    }
    {
        auto m = member(7, 31, 99);
        std::uint64_t serial = 100;
        State state{&m, &serial};
        Result r{};
        if (set_character_level(&state, 7, 0, &r) != Status::complete ||
            r.temporary_marked != 1 || r.member_marked || serial != 101 ||
            m.revision != 31 || m.value != 7)
            return 6;
        ++cases;
    }
    {
        auto m = member(-1, 31, 99);
        std::uint64_t serial = 100;
        State state{&m, &serial};
        Result r{};
        if (set_character_level(&state, -1, -1, &r) != Status::complete ||
            r.temporary_marked || r.member_marked || serial != 100 ||
            m.value != -1)
            return 7;
        ++cases;
    }
    {
        Result r{};
        if (set_value(nullptr, nullptr, 1, &r) != Status::invalid_argument ||
            set_character_level(nullptr, 1, 0, &r) != Status::invalid_argument)
            return 8;
        ++cases;
    }
    std::printf("{\"validation\":\"PASS\",\"host_cases\":%u}\n", cases);
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) return host_cases();
    if (argc != 7) return 64;
    const std::int32_t initial = static_cast<std::int32_t>(std::strtol(argv[1], nullptr, 0));
    const std::int32_t input = static_cast<std::int32_t>(std::strtol(argv[2], nullptr, 0));
    const std::int32_t residue = static_cast<std::int32_t>(std::strtol(argv[3], nullptr, 0));
    std::uint64_t serial = std::strtoull(argv[4], nullptr, 0);
    const std::uint32_t stamp = static_cast<std::uint32_t>(std::strtoul(argv[5], nullptr, 0));
    const std::uint8_t changed = static_cast<std::uint8_t>(std::strtoul(argv[6], nullptr, 0));
    auto m = member(initial, 0x1122334455667788ull, stamp, changed);
    State state{&m, &serial};
    Result r{};
    if (set_character_level(&state, input, residue, &r) != Status::complete) return 65;
    std::printf("{\"value\":%d,\"serial\":%llu,\"revision\":%llu,\"field10\":%u,\"field14\":%u,\"stamp\":%u,\"changed\":%u,\"temporary_marked\":%u,\"member_marked\":%u,\"member_value_changed\":%u}\n",
                m.value, static_cast<unsigned long long>(serial),
                static_cast<unsigned long long>(m.revision), m.field_10,
                m.field_14, m.source_stamp, static_cast<unsigned>(m.changed),
                r.temporary_marked, r.member_marked, r.member_value_changed);
    return 0;
}
