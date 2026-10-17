#include "../native_trophy_runtime.hpp"

#include <array>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

namespace {
using dh2::data::Bytes;
using dh2::data::TrophyUnlockStatusV1;
using dh2::native::trophies::OwnerV1;

void require(bool ok, const char* message) {
    if (!ok) { std::cerr << message << '\n'; std::exit(1); }
}

void word(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i)
        out.push_back(static_cast<std::uint8_t>(value >> (i * 8)));
}

std::vector<std::uint8_t> strings(std::initializer_list<const char*> values) {
    std::vector<std::uint8_t> out;
    word(out, static_cast<std::uint32_t>(values.size()));
    for (const auto* value : values) {
        std::size_t length = 0;
        while (value[length]) ++length;
        word(out, static_cast<std::uint32_t>(length));
        out.insert(out.end(), value, value + length);
    }
    return out;
}

std::vector<std::uint8_t> record(std::int32_t value) {
    std::vector<std::uint8_t> out;
    word(out, 1);
    for (std::int32_t field : {value, 2, 3, 4, 5, 6, 7})
        word(out, static_cast<std::uint32_t>(field));
    return out;
}

void test_single_runtime_owner() {
    auto records = record(101);
    auto names = strings({"source_row"});
    auto fields = strings({"Desc", "GLIndex", "GLLive", "Grade",
                           "Label", "Name", "Type"});
    auto replacement_records = record(909);
    auto replacement_names = strings({"replacement"});
    OwnerV1 runtime;
    std::string error;
    require(runtime.initialize({records.data(), records.size()},
                               {names.data(), names.size()},
                               {fields.data(), fields.size()}, error),
            "native TrophyManager runtime failed to initialize");
    auto* first = runtime.manager();
    require(first && first->initialized() && first->size() == 1 &&
            first->find_id_by_name("source_row") == 0,
            "canonical native runtime did not own the source row");
    require(first->unlock(0) == TrophyUnlockStatusV1::completed,
            "canonical TrophyManager failed its source unlock transition");
    require(runtime.initialize({replacement_records.data(), replacement_records.size()},
                               {replacement_names.data(), replacement_names.size()},
                               {fields.data(), fields.size()}, error) &&
            runtime.manager() == first && first->is_unlocked(0) &&
            first->find_id_by_name("source_row") == 0 &&
            first->find_id_by_name("replacement") == -1,
            "re-entry replaced the app-wide TrophyManager or lost memory-only state");
    require(OwnerV1::persistence_available &&
            !dh2::data::TrophyManagerOwnerV1::source_persistence_available,
            "Android runtime sidecar persistence or payload-owner boundary differs");
}

void test_source_savegame_sidecar() {
    auto records = record(101);
    auto names = strings({"source_row"});
    auto fields = strings({"Desc", "GLIndex", "GLLive", "Grade",
                           "Label", "Name", "Type"});
    const auto root = std::filesystem::temp_directory_path() /
        ("dh2_trophy_sidecar_" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    std::string error;
    OwnerV1 runtime;
    require(runtime.initialize({records.data(), records.size()},
                               {names.data(), names.size()},
                               {fields.data(), fields.size()}, error),
            "sidecar runtime failed to initialize");
    require(runtime.load_savegame(root.string(), error) &&
            !runtime.manager()->is_unlocked(0),
            "missing source achievements.savegame did not default locked");
    require(runtime.manager()->unlock(0) == TrophyUnlockStatusV1::completed &&
            runtime.save_savegame(root.string(), error),
            "source unlock failed to persist achievements.savegame");
    const auto sidecar = root / "achievements.savegame";
    require(std::filesystem::file_size(sidecar) == 16,
            "source TrophyManager sidecar is not the 128-bit payload size");
    {
        std::ifstream input(sidecar, std::ios::binary);
        std::array<std::uint8_t, 16> payload{};
        input.read(reinterpret_cast<char*>(payload.data()), payload.size());
        require(input.gcount() == 16 && payload[0] == 1 && payload[1] == 0,
                "source bit zero was not encoded into the sidecar");
    }
    OwnerV1 restored;
    require(restored.initialize({records.data(), records.size()},
                                {names.data(), names.size()},
                                {fields.data(), fields.size()}, error) &&
            restored.load_savegame(root.string(), error) &&
            restored.manager()->is_unlocked(0),
            "source achievements.savegame did not restore to the canonical manager");
    {
        std::ofstream malformed(sidecar, std::ios::binary | std::ios::trunc);
        const std::array<std::uint8_t, 15> short_payload{};
        malformed.write(reinterpret_cast<const char*>(short_payload.data()),
                         short_payload.size());
    }
    OwnerV1 rejected;
    require(rejected.initialize({records.data(), records.size()},
                                {names.data(), names.size()},
                                {fields.data(), fields.size()}, error) &&
            !rejected.load_savegame(root.string(), error) &&
            !rejected.manager()->is_unlocked(0),
            "malformed sidecar partially mutated source TrophyManager state");
    std::filesystem::remove_all(root);
}

void test_source_cache(const char* records_path, const char* names_path,
                       const char* fields_path) {
    const auto read = [](const char* path) {
        FILE* file = std::fopen(path, "rb");
        require(file != nullptr, "could not open original TrophyTable corpus");
        std::vector<std::uint8_t> bytes;
        int ch = 0;
        while ((ch = std::fgetc(file)) != EOF) bytes.push_back(static_cast<std::uint8_t>(ch));
        std::fclose(file);
        return bytes;
    };
    const auto records = read(records_path);
    const auto names = read(names_path);
    const auto fields = read(fields_path);
    OwnerV1 runtime;
    std::string error;
    require(runtime.initialize({records.data(), records.size()},
                               {names.data(), names.size()},
                               {fields.data(), fields.size()}, error),
            "native TrophyManager rejected original cache table");
    auto* manager = runtime.manager();
    require(manager && manager->size() == 69 &&
            manager->find_id_by_name("epic_withskills") == 36 &&
            manager->find_id_by_name("use_100_potions") == 51 &&
            manager->find_id_by_name("gear_10kgold") == 24 &&
            manager->find_id_by_name("gear_100kgold") == 25 &&
            manager->find_id_by_name("gear_1mgold") == 26 &&
            manager->find_id_by_name("gear_transmute") >= 0,
            "native TrophyManager table IDs differ from source rows");
}
}

int main(int argc, char** argv) {
    test_single_runtime_owner();
    test_source_savegame_sidecar();
    if (argc != 1) {
        require(argc == 4, "source-cache mode requires three TrophyTable paths");
        test_source_cache(argv[1], argv[2], argv[3]);
    }
    std::cout << "Native TrophyManager runtime v1 audit passed\n";
}
