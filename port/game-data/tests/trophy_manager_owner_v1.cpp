#include "../trophy_manager_owner_v1.hpp"

#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using dh2::data::Bytes;
using dh2::data::TrophyManagerOwnerV1;
using dh2::data::TrophyTableV1;
using dh2::data::TrophyUnlockEventV1;
using dh2::data::TrophyUnlockStatusV1;

void require(bool value, const char* message) {
    if (!value) {
        std::cerr << message << '\n';
        std::exit(1);
    }
}

void word(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (unsigned n = 0; n < 4; ++n)
        out.push_back(static_cast<std::uint8_t>(value >> (8 * n)));
}

void string(std::vector<std::uint8_t>& out, const char* value) {
    std::size_t size = 0;
    while (value[size]) ++size;
    word(out, static_cast<std::uint32_t>(size));
    out.insert(out.end(), value, value + size);
}

std::vector<std::uint8_t> strings(std::initializer_list<const char*> values) {
    std::vector<std::uint8_t> out;
    word(out, static_cast<std::uint32_t>(values.size()));
    for (const auto* value : values) string(out, value);
    return out;
}

std::vector<std::uint8_t> records() {
    std::vector<std::uint8_t> out;
    word(out, 2);
    const std::int32_t rows[][7] = {
        {100, 2, 1, 3, 4, 500, 6},
        {200, 7, 0, 8, 9, 600, 10}
    };
    for (const auto& row : rows)
        for (const auto value : row)
            word(out, static_cast<std::uint32_t>(value));
    return out;
}

TrophyTableV1 make_table() {
    auto data = records();
    auto names = strings({"kill_100", "potion_100"});
    auto fields = strings({"Desc", "GLIndex", "GLLive", "Grade",
                           "Label", "Name", "Type"});
    TrophyTableV1 table;
    std::string error;
    require(table.load({data.data(), data.size()}, {names.data(), names.size()},
                       {fields.data(), fields.size()}, error), "valid TrophyTable rejected");
    require(error.empty() && table.rows().size() == 2, "TrophyTable dimensions differ");
    return table;
}

void test_table_loader() {
    auto data = records();
    auto names = strings({"kill_100", "potion_100"});
    auto fields = strings({"Desc", "GLIndex", "GLLive", "Grade",
                           "Label", "Name", "Type"});
    TrophyTableV1 table;
    std::string error;
    require(table.load({data.data(), data.size()}, {names.data(), names.size()},
                       {fields.data(), fields.size()}, error), "valid TrophyTable rejected");
    require(table.rows()[0].table_name == "kill_100" && table.rows()[1].table_name == "potion_100",
            "parallel TrophyTable names changed");
    const auto& row = table.rows()[0];
    require(row.desc == 100 && row.gl_index == 2 && row.gl_live == 1 &&
            row.grade == 3 && row.label == 4 && row.name == 500 && row.type == 6,
            "source field order changed");

    const auto old_size = table.rows().size();
    auto bad_schema = strings({"Desc", "GLIndex", "GLLive", "Grade",
                               "Name", "Label", "Type"});
    require(!table.load({data.data(), data.size()}, {names.data(), names.size()},
                        {bad_schema.data(), bad_schema.size()}, error),
            "wrong source schema accepted");
    require(table.rows().size() == old_size && table.rows()[0].desc == 100,
            "failed table load modified the active snapshot");

    auto bad_names = strings({"kill_100"});
    require(!table.load({data.data(), data.size()}, {bad_names.data(), bad_names.size()},
                        {fields.data(), fields.size()}, error), "name count mismatch accepted");
    std::vector<std::uint8_t> truncated = data;
    truncated.pop_back();
    require(!table.load({truncated.data(), truncated.size()}, {names.data(), names.size()},
                        {fields.data(), fields.size()}, error), "truncated row accepted");

    std::vector<std::uint8_t> oversized_records;
    word(oversized_records, 129);
    for (unsigned i = 0; i < 129u * 7u; ++i) word(oversized_records, i);
    std::vector<std::uint8_t> oversized_names;
    word(oversized_names, 129);
    for (unsigned i = 0; i < 129; ++i) {
        const auto name = std::string("trophy_") + std::to_string(i);
        word(oversized_names, static_cast<std::uint32_t>(name.size()));
        oversized_names.insert(oversized_names.end(), name.begin(), name.end());
    }
    require(!table.load({oversized_records.data(), oversized_records.size()},
                        {oversized_names.data(), oversized_names.size()},
                        {fields.data(), fields.size()}, error),
            "TrophyTable beyond source bitset capacity accepted");
}

void test_manager() {
    const auto table = make_table();
    TrophyManagerOwnerV1 owner;
    std::string error;
    require(owner.initialize(table, error) && owner.initialized(), "manager initialization failed");
    require(owner.size() == 2 && owner.get(0) && owner.get(1), "source trophy rows not projected");
    require(owner.find_id_by_name("kill_100") == 0 &&
            owner.find_id_by_name("potion_100") == 1 &&
            owner.find_id_by_name("KILL_100") == -1 &&
            owner.find_id_by_name("missing") == -1,
            "TrophyTable name lookup did not preserve exact source matching");
    require(owner.get(0)->id == 0 && owner.get(0)->name_text_id == 500 &&
            owner.get(0)->desc_text_id == 100 && owner.get(0)->type == 6 &&
            owner.get(0)->grade == 3 && owner.get(0)->label == 4 &&
            owner.get(0)->gl_live == 1 && owner.get(0)->gl_index == 2,
            "TrophyData fields differ from source InitTrophies projection");
    require(!owner.get(-1) && !owner.get(2) && !owner.is_unlocked(-1) &&
            !owner.is_unlocking(2), "invalid trophy IDs were not safely rejected");
    require(owner.begin_unlock(-1) == TrophyUnlockStatusV1::invalid_id &&
            owner.unlock(2) == TrophyUnlockStatusV1::invalid_id,
            "invalid IDs entered the unlock flow");

    require(owner.begin_unlock(0) == TrophyUnlockStatusV1::started && owner.is_unlocking(0),
            "source pending state not retained");
    require(owner.begin_unlock(0) == TrophyUnlockStatusV1::already_unlocking &&
            owner.unlock(0) == TrophyUnlockStatusV1::already_unlocking,
            "duplicate pending unlock was not suppressed");
    TrophyUnlockEventV1 event{};
    require(owner.complete_unlock(0, &event) == TrophyUnlockStatusV1::completed,
            "pending unlock did not complete");
    require(!owner.is_unlocking(0) && owner.is_unlocked(0) && owner.get(0)->unlocked,
            "callback did not clear pending and set unlocked state");
    require(event.id == 0 && event.name_text_id == 500 && event.desc_text_id == 100 &&
            event.type == 6 && event.grade == 3 && event.label == 4 &&
            event.gl_live == 1 && event.gl_index == 2,
            "unlock event did not preserve table-derived presentation values");
    require(owner.unlock(0) == TrophyUnlockStatusV1::already_unlocked,
            "repeat unlock changed an unlocked row");

    require(owner.complete_unlock(1) == TrophyUnlockStatusV1::not_unlocking &&
            !owner.is_unlocked(1),
            "completion without pending state was accepted");

    const auto integrations = TrophyManagerOwnerV1::integrations();
    require(!integrations.game_center && !integrations.gl_live && !integrations.google_play,
            "unimplemented achievement integrations were advertised");
    require(!TrophyManagerOwnerV1::source_persistence_available,
            "sidecar achievement persistence was advertised without an owner seam");

    std::array<std::uint8_t, 16> payload{};
    require(owner.encode_savegame_bits(payload, error), "source save-bit encoding failed");
    require(payload[0] == 0x01 && payload[1] == 0 && payload[15] == 0,
            "source save bits were not encoded by row index");

    TrophyManagerOwnerV1 restored;
    require(restored.initialize(table, error), "restore manager initialization failed");
    require(restored.load_savegame_bits({payload.data(), payload.size()}, error) &&
            restored.is_unlocked(0) && !restored.is_unlocked(1),
            "source save-bit round trip did not restore unlocked rows");

    TrophyManagerOwnerV1 invalid_restore;
    require(invalid_restore.initialize(table, error),
            "invalid-payload manager initialization failed");
    require(invalid_restore.unlock(1) == TrophyUnlockStatusV1::completed,
            "test could not establish preexisting unlocked state");
    require(!invalid_restore.load_savegame_bits(
                {payload.data(), payload.size() - 1}, error) &&
            invalid_restore.is_unlocked(1) && !invalid_restore.is_unlocked(0),
            "truncated save payload changed existing unlocked state");
    std::array<std::uint8_t, 17> oversized_payload{};
    require(!invalid_restore.load_savegame_bits(
                {oversized_payload.data(), oversized_payload.size()}, error) &&
            invalid_restore.is_unlocked(1) && !invalid_restore.is_unlocked(0),
            "oversized save payload changed existing unlocked state");
}

std::vector<std::uint8_t> read_file(const char* path) {
    std::ifstream input(path, std::ios::binary);
    require(bool(input), "could not open TrophyTable corpus file");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void test_source_corpus(const char* record_path, const char* names_path,
                        const char* fields_path) {
    auto records = read_file(record_path);
    auto names = read_file(names_path);
    auto fields = read_file(fields_path);
    TrophyTableV1 table;
    std::string error;
    require(table.load({records.data(), records.size()}, {names.data(), names.size()},
                       {fields.data(), fields.size()}, error),
            "original TrophyTable cache was rejected");
    require(table.rows().size() == 69 && table.rows().front().table_name == "epic_lvl10" &&
            table.rows().back().table_name == "quest_died_100_times",
            "original TrophyTable row count or endpoints differ");
    TrophyManagerOwnerV1 owner;
    require(owner.initialize(table, error) && owner.size() == table.rows().size(),
            "original TrophyTable did not initialize TrophyData entries");
    require(owner.find_id_by_name("use_100_potions") >= 0 &&
            owner.find_id_by_name("missing") == -1,
            "original TrophyTable name lookup failed");
    TrophyUnlockEventV1 event{};
    require(owner.unlock(0, &event) == TrophyUnlockStatusV1::completed &&
            event.name_text_id == table.rows()[0].name &&
            event.desc_text_id == table.rows()[0].desc && owner.is_unlocked(0),
            "original TrophyTable fields did not reach unlock presentation");
    require(owner.unlock(31) == TrophyUnlockStatusV1::completed &&
            owner.unlock(32) == TrophyUnlockStatusV1::completed &&
            owner.unlock(68) == TrophyUnlockStatusV1::completed,
            "original TrophyTable boundary rows failed to unlock");
    std::array<std::uint8_t, 16> payload{};
    require(owner.encode_savegame_bits(payload, error), "original save bits failed encoding");
    require(payload[0] == 0x01 && payload[3] == 0x80 &&
            payload[4] == 0x01 && payload[8] == 0x10,
            "original save bits do not use little-endian row-index bit ordering");

    TrophyManagerOwnerV1 restored;
    require(restored.initialize(table, error) &&
            restored.load_savegame_bits({payload.data(), payload.size()}, error) &&
            restored.is_unlocked(0) && restored.is_unlocked(31) &&
            restored.is_unlocked(32) && restored.is_unlocked(68),
            "original save bitset failed a full row-boundary round trip");
}
} // namespace

int main(int argc, char** argv) {
    test_table_loader();
    test_manager();
    if (argc != 1) {
        require(argc == 4, "source-corpus mode needs records, names and fields paths");
        test_source_corpus(argv[1], argv[2], argv[3]);
    }
    std::cout << "TrophyManager owner v1 audit passed\n";
}
