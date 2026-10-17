#include "../player_profile_raw_sections_v1.hpp"
#include "../player_profile_create_v1.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
void word(std::vector<std::uint8_t>& out, std::uint32_t n) {
    for (unsigned i = 0; i != 4; ++i) out.push_back(static_cast<std::uint8_t>(n >> (8 * i)));
}
void section(std::vector<std::uint8_t>& out, const char* tag,
             std::initializer_list<std::uint8_t> payload) {
    word(out, static_cast<std::uint32_t>(payload.size()));
    out.insert(out.end(), tag, tag + 4);
    out.insert(out.end(), payload.begin(), payload.end());
}
std::uint32_t read_word(const std::vector<std::uint8_t>& bytes, std::size_t& at) {
    check(bytes.size() - at >= 4, "truncated output word");
    const auto value = std::uint32_t(bytes[at]) | std::uint32_t(bytes[at + 1]) << 8 |
        std::uint32_t(bytes[at + 2]) << 16 | std::uint32_t(bytes[at + 3]) << 24;
    at += 4;
    return value;
}
}

int main() {
    try {
        std::vector<std::uint8_t> source;
        word(source, 4);
        section(source, "ZZZZ", {9});
        section(source, "GEAR", {0xaa});
        section(source, "KEEP", {3, 4});
        section(source, "KEEP", {5}); // source index lookup retains the last duplicate
        PlayerProfileIndexV1 index;
        std::string error;
        check(index.load({source.data(), source.size()}, error), "load source profile");

        const std::uint8_t gear[]{1, 2, 3};
        const std::uint8_t skill[]{7, 8};
        const std::vector<PlayerProfileRawSectionV1> replacements{
            {{{'G', 'E', 'A', 'R'}}, {gear, sizeof(gear)}},
            {{{'S', 'K', 'I', 'L'}}, {skill, sizeof(skill)}}
        };
        std::vector<std::uint8_t> output{0xee};
        check(serialize_player_profile_raw_sections_v1(index.borrow(), replacements,
            output, error), "assemble raw profile");
        check(error.empty(), "success clears error");

        std::size_t at = 0;
        check(read_word(output, at) == 4, "append count");
        const char* expected_tags[]{"GEAR", "KEEP", "SKIL", "ZZZZ"};
        const std::vector<std::vector<std::uint8_t>> expected_payloads{
            {1, 2, 3}, {5}, {7, 8}, {9}
        };
        for (unsigned i = 0; i != 4; ++i) {
            const auto size = read_word(output, at);
            check(size == expected_payloads[i].size(), "section length");
            check(output.size() - at >= 4 + size, "bounded output section");
            check(std::memcmp(output.data() + at, expected_tags[i], 4) == 0,
                  "lexically sorted four-byte tag");
            at += 4;
            check(std::equal(expected_payloads[i].begin(), expected_payloads[i].end(),
                             output.begin() + at), "raw payload replacement/preservation");
            at += size;
        }
        check(at == output.size(), "output fully consumed");
        PlayerProfileIndexV1 rebuilt;
        check(rebuilt.load({output.data(), output.size()}, error),
              "canonical output re-indexes");
        const auto gear_payload = rebuilt.borrow().payload("GEAR");
        check(gear_payload.size == sizeof(gear) &&
              std::memcmp(gear_payload.data, gear, sizeof(gear)) == 0,
              "GEAR body survives profile round trip");

        PlayerSavegameV1 save;
        CharacterTable characters;
        std::int32_t difficulty = 0;
        check(!serialize_player_metadata_profile_v1(index.borrow(), {"GEAR"}, save,
            characters, &difficulty, output, error),
            "metadata serializer keeps seven-tag guard");

        auto invalid = replacements;
        invalid.push_back({{{'G', 'E', 'A', 'R'}}, {gear, sizeof(gear)}});
        const auto before = output;
        check(!serialize_player_profile_raw_sections_v1(index.borrow(), invalid,
            output, error), "duplicate replacement rejected");
        check(output == before, "failure leaves output unchanged");

        invalid = {{{{'B', 'A', '\0', 'D'}}, {gear, sizeof(gear)}}};
        check(!serialize_player_profile_raw_sections_v1(index.borrow(), invalid,
            output, error), "embedded-NUL raw tag rejected");
        check(output == before, "invalid tag leaves output unchanged");
        std::cout << "player_profile_raw_sections_v1: PASS\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "player_profile_raw_sections_v1: " << error.what() << '\n';
        return 1;
    }
}
