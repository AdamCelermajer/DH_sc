#include "../character_physics_position.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace policy = dh2::character_physics_position;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

template<class T>
T read(std::ifstream& in) {
    T value{};
    require(bool(in.read(reinterpret_cast<char*>(&value), sizeof(value))),
            "truncated Character physics-position corpus");
    return value;
}

void guards() {
    const std::uint32_t flags = 0xA5A5A5A5u;
    policy::CharacterView view{0x100000001ull, &flags};
    policy::Result result{17, 19};
    require(policy::query(&view, &result) == policy::Status::complete,
            "valid Character query rejected");
    require(result.character_identity == view.identity && result.raw == 0,
            "bit-1 clear or wide identity mismatch");

    const auto saved = result;
    require(policy::query(nullptr, &result) == policy::Status::invalid_argument &&
            std::memcmp(&result, &saved, sizeof(result)) == 0,
            "null owner changed result");
    require(policy::query(&view, nullptr) == policy::Status::invalid_argument,
            "null result accepted");
    auto invalid_owner = view;
    invalid_owner.identity = 0;
    require(policy::query(&invalid_owner, &result) == policy::Status::invalid_argument,
            "zero owner identity accepted");

    alignas(policy::CharacterView) unsigned char view_bytes[sizeof(view) + 1]{};
    auto* misaligned_view = reinterpret_cast<policy::CharacterView*>(view_bytes + 1);
    require(policy::query(misaligned_view, &result) == policy::Status::invalid_argument,
            "misaligned owner accepted");
    alignas(std::uint32_t) unsigned char flag_bytes[sizeof(flags) + 1]{};
    auto* misaligned_flags = reinterpret_cast<const std::uint32_t*>(flag_bytes + 1);
    invalid_owner = {view.identity, misaligned_flags};
    require(policy::query(&invalid_owner, &result) == policy::Status::invalid_argument,
            "misaligned live flags accepted");

    policy::CharacterView alias_view{view.identity, nullptr};
    auto* aliased_result = reinterpret_cast<policy::Result*>(&alias_view);
    require(policy::query(&alias_view, aliased_result) == policy::Status::invalid_argument,
            "overlapping owner and result accepted");

    policy::Result flags_alias_result{};
    policy::CharacterView flags_alias_view{view.identity,
        reinterpret_cast<const std::uint32_t*>(&flags_alias_result)};
    require(policy::query(&flags_alias_view, &flags_alias_result) ==
                policy::Status::invalid_argument,
            "overlapping flags and result accepted");
}
}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc == 2 && std::strcmp(argv[1], "--guards") == 0) {
            guards();
            std::cout << "{\"guard_checks\":8}\n";
            return 0;
        }
        require(argc == 2, "usage: character_physics_position_host corpus.bin | --guards");
        std::ifstream input(argv[1], std::ios::binary);
        require(bool(input), "cannot open original ARM corpus");
        require(read<std::uint32_t>(input) == 0x31504843u,
                "bad Character policy corpus magic");
        const auto count = read<std::uint32_t>(input);
        std::uint32_t queries = 0, enabled = 0;
        for (std::uint32_t i = 0; i < count; ++i) {
            const auto identity = read<std::uint64_t>(input);
            const auto flags = read<std::uint32_t>(input);
            const auto expected = read<std::uint32_t>(input);
            policy::CharacterView view{static_cast<std::uintptr_t>(identity), &flags};
            policy::Result result{};
            require(policy::query(&view, &result) == policy::Status::complete,
                    "valid source Character query failed");
            require(result.character_identity == identity && result.raw == expected,
                    "source Character policy mismatch");
            ++queries;
            enabled += expected != 0;
        }
        char tail{};
        require(!input.read(&tail, 1), "trailing source Character policy bytes");
        std::cout << "{\"validation\":\"PASS\",\"comparisons\":" << queries
                  << ",\"position_from_physics_true\":" << enabled << "}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
