#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../player_skill_save_writer_v1.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace writer = dh2::data::player_skill_save_writer_v1;
using dh2::data::Bytes;
using dh2::data::PlayerSavegameV1;
using dh2::data::SkillRow;
using dh2::data::SkillTables;
using Raw = std::vector<std::uint8_t>;

namespace {
unsigned checks = 0;
const char* stage = "startup";

void require(bool value) {
    ++checks;
    if (!value)
        throw std::runtime_error(std::string("SKIL writer check ") +
                                 std::to_string(checks) + " at " + stage);
}

void put_word(Raw& bytes, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i)
        bytes.push_back(static_cast<std::uint8_t>(value >> (i * 8)));
}

void put_halfword(Raw& bytes, std::uint16_t value) {
    bytes.push_back(static_cast<std::uint8_t>(value));
    bytes.push_back(static_cast<std::uint8_t>(value >> 8));
}

void put_string(Raw& bytes, const std::string& value) {
    put_word(bytes, static_cast<std::uint32_t>(value.size() + 1));
    bytes.insert(bytes.end(), value.begin(), value.end());
    bytes.push_back(0);
}

struct Sink {
    Raw bytes;
    std::uint32_t calls{};
    std::uint32_t fail_call{};
};

bool write(void* context, Bytes bytes, std::string& error) {
    auto& sink = *static_cast<Sink*>(context);
    ++sink.calls;
    if (sink.fail_call && sink.calls == sink.fail_call) {
        error = "fixture stream failure";
        return false;
    }
    if ((!bytes.data && bytes.size) || bytes.size > 1024) {
        error = "invalid fixture write";
        return false;
    }
    if (bytes.size)
        sink.bytes.insert(sink.bytes.end(), bytes.data,
                          bytes.data + bytes.size);
    return true;
}

writer::Status run(const PlayerSavegameV1& save, const SkillTables& tables,
                   Sink& sink, std::string& error) {
    const writer::WriteServicesV1 stream{&sink, write};
    return writer::write_section_v1(save, tables, stream, error);
}

SkillTables fixture_tables() {
    SkillTables tables;
    SkillRow heal;
    heal.table_name = "Heal";
    tables.skills.push_back(heal);
    SkillRow fire;
    fire.table_name = "Fire";
    tables.skills.push_back(fire);
    return tables;
}

Raw source_section() {
    Raw bytes;
    put_word(bytes, 2);
    put_string(bytes, "Heal");
    put_halfword(bytes, 0xabcd);
    put_string(bytes, "Fire");
    put_halfword(bytes, 0x1234);
    put_word(bytes, 2);  // Slot set 0, deliberately unsorted input.
    put_word(bytes, 3);
    put_word(bytes, 0);
    put_word(bytes, 1);
    put_word(bytes, 1);
    put_word(bytes, 2);  // Slot set 1, also deliberately unsorted.
    put_word(bytes, 7);
    put_word(bytes, std::numeric_limits<std::uint32_t>::max());
    put_word(bytes, std::numeric_limits<std::uint32_t>::max() - 1);  // key -2
    put_word(bytes, 0x12345678);
    return bytes;
}

Raw expected_section() {
    return {
        2, 0, 0, 0,
        5, 0, 0, 0, 'H', 'e', 'a', 'l', 0, 0xcd, 0xab,
        5, 0, 0, 0, 'F', 'i', 'r', 'e', 0, 0x34, 0x12,
        2, 0, 0, 0,
        1, 0, 0, 0, 1, 0, 0, 0,
        3, 0, 0, 0, 0, 0, 0, 0,
        2, 0, 0, 0,
        0xfe, 0xff, 0xff, 0xff, 0x78, 0x56, 0x34, 0x12,
        7, 0, 0, 0, 0xff, 0xff, 0xff, 0xff,
    };
}

void initialize_save(PlayerSavegameV1& save, const SkillTables& tables) {
    save.set_character_only(UINT64_C(0x123456789abcdef0));
    std::string error;
    require(save.initialize_skills_from_character_list({0, 1}, error));
    const auto input = source_section();
    std::size_t consumed = 0;
    require(save.load_skills({input.data(), input.size()}, tables,
                             consumed, error) == 0);
    require(consumed == input.size() && error.empty());
}

void check_exact_bytes_and_order() {
    stage = "source field order, string encoding, and sorted maps";
    const auto tables = fixture_tables();
    PlayerSavegameV1 save;
    initialize_save(save, tables);
    const auto before_skills = save.skills();
    const auto before_slots = save.skill_slots();
    Sink sink;
    std::string error;
    require(run(save, tables, sink, error) == writer::Status::complete);
    require(error.empty() && sink.bytes == expected_section());
    require(sink.calls == 17);
    require(save.skills().size() == before_skills.size() &&
            save.skills()[0].level == before_skills[0].level &&
            save.skills()[1].level == before_skills[1].level &&
            save.skill_slots() == before_slots);
}

void check_source_boundaries_and_prefixes() {
    const auto tables = fixture_tables();
    std::string error;

    stage = "uninitialized source skills assertion";
    PlayerSavegameV1 blank;
    Sink blank_sink;
    require(run(blank, tables, blank_sink, error) ==
            writer::Status::source_assertion_boundary);
    require(blank_sink.bytes.empty() && !error.empty());
    error.clear();

    stage = "missing source SkillTable name dependency after count";
    PlayerSavegameV1 bad_id;
    bad_id.set_character_only(1);
    require(bad_id.initialize_skills_from_character_list({99}, error));
    Sink bad_id_sink;
    require(run(bad_id, tables, bad_id_sink, error) ==
            writer::Status::source_assertion_boundary);
    require(bad_id_sink.bytes == Raw({1, 0, 0, 0}) && !error.empty());
    error.clear();

    stage = "source stream failure keeps accepted prefix";
    PlayerSavegameV1 save;
    initialize_save(save, tables);
    const auto expected = expected_section();
    Sink failed;
    failed.fail_call = 3;  // name bytes fail after the source length word
    require(run(save, tables, failed, error) == writer::Status::failed);
    require(failed.calls == 3 && failed.bytes ==
            Raw(expected.begin(), expected.begin() + 8));
    require(error == "fixture stream failure");
    error.clear();

    stage = "missing stream callback";
    require(writer::write_section_v1(save, tables, {}, error) ==
            writer::Status::invalid_argument);
    require(!error.empty());
}
}  // namespace

int main() {
    try {
        check_exact_bytes_and_order();
        check_source_boundaries_and_prefixes();
        std::cout << "{\"validation\":\"PASS\",\"host_checks\":"
                  << checks << ",\"section\":\"SKIL\"}\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL: " << ex.what() << '\n';
        return 1;
    }
}
