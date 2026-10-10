// CharacterState schema v4 codec tests (P14 SCHEMA stream): difficulty, menu
// metadata, visited modules and CQPG v2 objective counters in character.save.
#include "character_quest_blob.hpp"
#include "character_state.hpp"
#include "save_store.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
using namespace dh::foundation;
using Bytes = std::vector<char>;
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}
Bytes read_bytes(const std::filesystem::path& p) {
    std::ifstream f(p, std::ios::binary);
    require(static_cast<bool>(f), "cannot read test save");
    return {std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
}
void write_bytes(const std::filesystem::path& p, const Bytes& bytes) {
    std::ofstream f(p, std::ios::binary | std::ios::trunc);
    f.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    require(static_cast<bool>(f), "cannot write test bytes");
}
struct TempDirectory {
    std::filesystem::path path;
    TempDirectory() {
        const auto stamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        path = std::filesystem::temp_directory_path() / ("dhsc-schema-v4-" + std::to_string(stamp));
        require(std::filesystem::create_directory(path), "cannot create test directory");
    }
    ~TempDirectory() { std::error_code error; std::filesystem::remove_all(path, error); }
};

constexpr std::size_t schema_field_at = 12; // magic(8) + file format version(4)
std::vector<std::uint8_t> cqpg_v1(const std::string& id, std::uint32_t rows) {
    std::vector<std::uint8_t> blob{'C', 'Q', 'P', 'G'};
    const auto word = [&](std::uint32_t v) { for (unsigned i = 0; i < 4; ++i) blob.push_back(std::uint8_t(v >> (8 * i))); };
    word(1); word(rows); word(std::uint32_t(id.size()));
    blob.insert(blob.end(), id.begin(), id.end());
    blob.insert(blob.end(), 6, 0); // six unknown buckets
    return blob;
}

CharacterState fixture() {
    auto s = make_default_character("p14-slot-1", "SCHEMA", "KnightPlayerBase");
    s.stats.level = 5; s.stats.health = 50; s.stats.max_health = 100;
    s.experience = 1234; s.gold = 99;
    s.current_difficulty = 1; s.unlocked_difficulty = 2;
    s.menu_metadata.known = true;
    s.menu_metadata.save_time = 1760000000u;
    s.menu_metadata.level_row = {41, 43, -1};
    s.menu_metadata.current_act = {1, 2, 3};
    s.visited_modules = {{"data/scene/001_swamp.mlx", 3, 1}, {"data/scene/001_swamp.mlx", 9, 0},
                         {"data/scene/002_x.mlx", 1, 1}};
    return s;
}
bool same_v4(const CharacterState& a, const CharacterState& b) {
    if (a.visited_modules.size() != b.visited_modules.size()) return false;
    for (std::size_t i = 0; i < a.visited_modules.size(); ++i)
        if (a.visited_modules[i].level_uri != b.visited_modules[i].level_uri ||
            a.visited_modules[i].module_id != b.visited_modules[i].module_id ||
            a.visited_modules[i].visited != b.visited_modules[i].visited) return false;
    return a.schema_version == b.schema_version && a.id == b.id && a.name == b.name &&
           a.stats.level == b.stats.level && a.experience == b.experience && a.gold == b.gold &&
           a.current_difficulty == b.current_difficulty && a.unlocked_difficulty == b.unlocked_difficulty &&
           a.menu_metadata.known == b.menu_metadata.known && a.menu_metadata.save_time == b.menu_metadata.save_time &&
           a.menu_metadata.level_row == b.menu_metadata.level_row &&
           a.menu_metadata.current_act == b.menu_metadata.current_act &&
           a.source_quest_progress_cqpg == b.source_quest_progress_cqpg;
}
// Size of an empty-visited v4 tail: difficulty 4+4, known 1, date 4, rows 12, acts 12, count 4.
constexpr std::size_t v4_fixed_tail = 4 + 4 + 1 + 4 + 12 + 12 + 4;
std::size_t v4_tail_size(const CharacterState& s) {
    std::size_t size = v4_fixed_tail;
    for (const auto& e : s.visited_modules) size += 4 + e.level_uri.size() + 4 + 1;
    return size;
}
Bytes with_schema(Bytes bytes, std::uint32_t schema) {
    bytes[schema_field_at] = char(schema);
    bytes[schema_field_at + 1] = bytes[schema_field_at + 2] = bytes[schema_field_at + 3] = 0;
    return bytes;
}

void expect_load_rejected(const std::filesystem::path& file, const Bytes& bad, const char* label) {
    write_bytes(file, bad);
    auto destination = fixture(); destination.name = "Destination";
    const auto before = destination;
    std::string error;
    require(!load_character(file, destination, error), std::string(label) + " accepted");
    require(!error.empty(), std::string(label) + " has no error detail");
    require(same_v4(destination, before), std::string(label) + " modified the destination");
}
} // namespace

int main() {
    try {
        TempDirectory temp;
        const auto save = temp.path / "character.save";
        const auto scratch = temp.path / "scratch.save";
        std::string error;

        // 1. v4 round trip: values and bytes are exact; re-saving is byte-identical.
        const auto original = fixture();
        require(original.schema_version == 4 && character_schema_version == 4, "schema constant is not 4");
        require(validate_character_state(original).ok(), "fixture must validate");
        require(save_character(save, original, error), "save failed: " + error);
        const auto bytes = read_bytes(save);
        CharacterState loaded;
        require(load_character(save, loaded, error), "load failed: " + error);
        require(same_v4(loaded, original), "v4 round trip differs");
        require(character_current_difficulty(loaded) == 1 && character_unlocked_difficulty(loaded) == 2,
                "difficulty accessors differ");
        require(save_character(scratch, loaded, error) && read_bytes(scratch) == bytes, "v4 re-save is not byte exact");
        require(bytes[schema_field_at] == 4, "schema field is not 4");
        // Documented layout of the appended tail (see report): offsets from the tail start.
        const auto tail = bytes.size() - v4_tail_size(original);
        const auto u32_at = [&](std::size_t at) {
            std::uint32_t v = 0; for (unsigned i = 0; i < 4; ++i) v |= std::uint32_t(std::uint8_t(bytes[at + i])) << (8 * i); return v; };
        require(u32_at(tail) == 1 && u32_at(tail + 4) == 2 && bytes[tail + 8] == 1 &&
                u32_at(tail + 9) == 1760000000u && u32_at(tail + 13) == 41 && u32_at(tail + 17) == 43 &&
                u32_at(tail + 21) == 0xffffffffu && u32_at(tail + 25) == 1 && u32_at(tail + 29) == 2 &&
                u32_at(tail + 33) == 3 && u32_at(tail + 37) == 3, "v4 tail byte layout differs from the documented one");

        // 2. Legacy v1..v3 slots still load: new fields blank/default, file untouched until an explicit save.
        const auto v3_path = temp.path / "legacy-v3.save";
        auto v3 = with_schema(bytes, 3); v3.resize(v3.size() - v4_tail_size(original));
        write_bytes(v3_path, v3);
        CharacterState legacy = fixture();
        require(load_character(v3_path, legacy, error), "schema 3 load failed: " + error);
        require(legacy.schema_version == 4 && legacy.id == original.id && legacy.stats.level == 5 &&
                legacy.current_difficulty == 0 && legacy.unlocked_difficulty == 0 &&
                !legacy.menu_metadata.known && legacy.menu_metadata.save_time == 0 &&
                legacy.menu_metadata.level_row == std::array<std::int32_t, 3>{-1, -1, -1} &&
                legacy.menu_metadata.current_act == std::array<std::int32_t, 3>{1, 1, 1} &&
                legacy.visited_modules.empty(), "schema 3 defaults differ");
        require(read_bytes(v3_path) == v3, "legacy read rewrote the file");
        require(save_character(v3_path, legacy, error) && read_bytes(v3_path).size() == v3.size() + v4_fixed_tail &&
                read_bytes(v3_path)[schema_field_at] == 4, "explicit save did not upgrade a v3 slot to v4");
        // v2 (no quest length either) and v1 representations of a generic character.
        auto plain = make_default_character("legacy-2", "Old", "KnightPlayerBase");
        require(save_character(scratch, plain, error), "plain save failed");
        auto v2 = with_schema(read_bytes(scratch), 2); v2.resize(v2.size() - v4_fixed_tail - 4);
        write_bytes(scratch, v2);
        CharacterState v2_loaded;
        require(load_character(scratch, v2_loaded, error) && v2_loaded.id == "legacy-2" && !v2_loaded.menu_metadata.known &&
                v2_loaded.current_difficulty == 0, "schema 2 load failed: " + error);

        // 3. Truncated / corrupt / trailing data never publishes a partial state.
        for (std::size_t cut = 1; cut <= v4_tail_size(original); ++cut)
            expect_load_rejected(scratch, Bytes(bytes.begin(), bytes.end() - std::ptrdiff_t(cut)), "truncated v4 tail");
        auto trailing = bytes; trailing.push_back('x');
        expect_load_rejected(scratch, trailing, "v4 trailing byte");
        auto bad_known = bytes; bad_known[tail + 8] = 2;
        expect_load_rejected(scratch, bad_known, "invalid known flag");
        auto bad_difficulty = bytes; bad_difficulty[tail] = 3;
        expect_load_rejected(scratch, bad_difficulty, "difficulty 3");
        auto bad_unlocked = bytes; bad_unlocked[tail + 4] = char(0xff); bad_unlocked[tail + 5] = char(0xff);
        bad_unlocked[tail + 6] = char(0xff); bad_unlocked[tail + 7] = char(0xff);
        expect_load_rejected(scratch, bad_unlocked, "unlocked difficulty -1");
        auto bad_row = bytes; bad_row[tail + 13] = char(0xff); bad_row[tail + 14] = 0x3f;
        expect_load_rejected(scratch, bad_row, "level row beyond limit");
        auto bad_count = bytes; bad_count[tail + 37] = char(0xff); bad_count[tail + 38] = char(0xff);
        expect_load_rejected(scratch, bad_count, "visited count beyond limit");
        auto bad_visited = bytes; bad_visited[bytes.size() - 1] = 2; // last visited byte
        expect_load_rejected(scratch, bad_visited, "visited byte 2");

        // 4. Unknown versions reject without touching the destination.
        auto unknown_schema = with_schema(bytes, 5);
        expect_load_rejected(scratch, unknown_schema, "schema 5");
        auto future = original; future.schema_version = 5;
        require(save_character(save, original, error), "reseed failed");
        const auto before_future = read_bytes(save);
        require(!save_character(save, future, error) && read_bytes(save) == before_future,
                "unsupported schema save overwrote the slot");

        // 5. Invalid states are rejected before any byte is written (atomic replace keeps the old file).
        const auto expect_save_rejected = [&](CharacterState bad, const char* label) {
            require(!save_character(save, bad, error), std::string(label) + " saved");
            require(read_bytes(save) == before_future, std::string(label) + " damaged the old slot");
        };
        auto bad = original; bad.current_difficulty = 3; expect_save_rejected(bad, "difficulty 3");
        bad = original; bad.unlocked_difficulty = -1; expect_save_rejected(bad, "unlocked -1");
        bad = original; bad.menu_metadata.level_row[0] = 5000; expect_save_rejected(bad, "row 5000");
        bad = original; bad.menu_metadata.current_act[2] = -1; expect_save_rejected(bad, "act -1");
        bad = make_default_character("p", "n", "c"); bad.menu_metadata.save_time = 7; expect_save_rejected(bad, "unknown metadata with a date");
        bad = original; std::swap(bad.visited_modules[0], bad.visited_modules[1]); expect_save_rejected(bad, "unsorted visited");
        bad = original; bad.visited_modules.push_back(bad.visited_modules.back()); expect_save_rejected(bad, "duplicate visited");
        bad = original; bad.visited_modules[0].visited = 2; expect_save_rejected(bad, "visited byte 2");
        bad = original; bad.visited_modules[0].level_uri.clear(); expect_save_rejected(bad, "empty visited level");
        bad = original; bad.visited_modules.clear();
        require(save_character(scratch, bad, error), "empty visited section must be valid (reserved)");
        CharacterState empty_visited;
        require(load_character(scratch, empty_visited, error) && empty_visited.visited_modules.empty() &&
                empty_visited.menu_metadata.known, "empty visited section round trip failed");

        // 6. CQPG v2 objective counters: Moths 0..8, Lizman 0..5, Witch 0/1.
        const std::string id = "p14-slot-1";
        auto blob = cqpg_v1(id, 64);
        const auto v1_copy = blob;
        std::string e2;
        require(validate_character_quest_blob(id, blob, e2), "v1 envelope must stay valid: " + e2);
        std::vector<QuestObjectiveCounterV2> none;
        require(read_quest_counters(id, blob, none, e2) && none.empty(), "v1 blob must have no counters");
        std::vector<QuestObjectiveCounterV2> counters = {
            {0, 0, 5, 0, 8, true},   // Moths 8/8 done
            {0, 0, 7, 0, 5, false},  // Lizman 5/5 listed as in progress at 5 (example)
            {0, 1, 7, 2, 3, false},
            {1, 2, 63, 63, 2147483647, true}};
        require(write_quest_counters(id, blob, counters, e2), "write counters failed: " + e2);
        require(blob[4] == 2 && blob.size() > v1_copy.size(), "counters must switch the envelope to v2");
        require(std::equal(v1_copy.begin() + 8, v1_copy.end(), blob.begin() + 8), "v1 bucket bytes must be preserved");
        std::vector<QuestObjectiveCounterV2> read_back;
        require(read_quest_counters(id, blob, read_back, e2) && read_back == counters, "counters round trip differs");
        auto witch = counters; witch.push_back({1, 2, 63, 63, 0, false}); // same key as the last counter: duplicate
        require(!write_quest_counters(id, blob, witch, e2), "duplicate/unsorted counter accepted");
        require(read_quest_counters(id, blob, read_back, e2) && read_back == counters, "failed write changed the blob");
        require(!write_quest_counters(id, blob, {{0, 0, 64, 0, 1, false}}, e2), "row beyond the quest table accepted");
        require(!write_quest_counters(id, blob, {{0, 3, 0, 0, 1, false}}, e2), "difficulty 3 counter accepted");
        require(!write_quest_counters(id, blob, {{2, 0, 0, 0, 1, false}}, e2), "collection 2 counter accepted");
        require(!write_quest_counters(id, blob, {{0, 0, 0, 64, 1, false}}, e2), "objective 64 counter accepted");
        require(!write_quest_counters(id, blob, {{0, 0, 0, 0, -1, false}}, e2), "negative counter accepted");
        require(!write_quest_counters(id, blob, {{0, 0, 2, 0, 1, false}, {0, 0, 1, 0, 1, false}}, e2), "unsorted counters accepted");
        std::vector<std::uint8_t> unknown_blob;
        require(!write_quest_counters(id, unknown_blob, counters, e2), "counters fabricated an uninitialized progress blob");
        require(!write_quest_counters("another", blob, counters, e2), "counters accepted for another character");
        // Round trip through character.save, byte exact.
        auto quest_character = original;
        quest_character.source_quest_progress_cqpg = blob;
        require(save_character(save, quest_character, error), "save with CQPG v2 failed: " + error);
        const auto quest_bytes = read_bytes(save);
        CharacterState quest_loaded;
        require(load_character(save, quest_loaded, error) && same_v4(quest_loaded, quest_character) &&
                read_quest_counters(id, quest_loaded.source_quest_progress_cqpg, read_back, e2) && read_back == counters,
                "CQPG v2 character.save round trip differs");
        require(save_character(scratch, quest_loaded, error) && read_bytes(scratch) == quest_bytes, "CQPG v2 re-save not byte exact");
        // Rewriting with no counters returns the canonical v1 bytes.
        auto stripped = blob;
        require(write_quest_counters(id, stripped, {}, e2) && stripped == v1_copy, "no counters must restore the exact v1 envelope");
        // Corrupt counter sections fail at the save layer.
        for (std::size_t cut = 1; cut < blob.size() - v1_copy.size(); ++cut) {
            auto truncated = quest_character; truncated.source_quest_progress_cqpg.resize(blob.size() - cut);
            require(!save_character(save, truncated, error) && read_bytes(save) == quest_bytes,
                    "truncated counter section saved");
        }
        auto trailing_blob = quest_character; trailing_blob.source_quest_progress_cqpg.push_back(0);
        require(!save_character(save, trailing_blob, error) && read_bytes(save) == quest_bytes, "trailing counter byte saved");
        auto v1_trailing = quest_character; v1_trailing.source_quest_progress_cqpg = v1_copy; v1_trailing.source_quest_progress_cqpg.push_back(0);
        require(!save_character(save, v1_trailing, error) && read_bytes(save) == quest_bytes, "v1 trailing byte saved");
        auto v3_blob = quest_character; v3_blob.source_quest_progress_cqpg[4] = 3;
        require(!save_character(save, v3_blob, error) && read_bytes(save) == quest_bytes, "CQPG version 3 saved");
        auto bad_flag = quest_character; bad_flag.source_quest_progress_cqpg.back() = 2;
        require(!save_character(save, bad_flag, error) && read_bytes(save) == quest_bytes, "counter completed flag 2 saved");

        // 7. Pure v1 blobs from other producers remain loadable in a v4 character.
        auto old_quest = original; old_quest.source_quest_progress_cqpg = v1_copy;
        require(save_character(scratch, old_quest, error), "v1 CQPG in v4 character failed: " + error);
        CharacterState old_quest_loaded;
        require(load_character(scratch, old_quest_loaded, error) && same_v4(old_quest_loaded, old_quest), "v1 CQPG round trip differs");

        std::cout << "schema_v4_tests: all checks passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "schema_v4_tests: " << e.what() << '\n';
        return 1;
    }
}
