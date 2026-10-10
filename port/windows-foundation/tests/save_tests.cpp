#include "character_state.hpp"
#include "save_store.hpp"

#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace dh::foundation;
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

bool equal(const CharacterState& a, const CharacterState& b) {
    if (a.schema_version != b.schema_version || a.id != b.id || a.name != b.name ||
        a.class_id != b.class_id || a.experience != b.experience || a.gold != b.gold ||
        a.source_quest_progress_cqpg != b.source_quest_progress_cqpg ||
        a.stats.level != b.stats.level || a.stats.health != b.stats.health ||
        a.stats.max_health != b.stats.max_health || a.stats.resource != b.stats.resource ||
        a.stats.max_resource != b.stats.max_resource || a.stats.strength != b.stats.strength ||
        a.stats.dexterity != b.stats.dexterity || a.stats.intelligence != b.stats.intelligence ||
        a.stats.endurance != b.stats.endurance || a.stats.energy != b.stats.energy ||
        a.inventory.size() != b.inventory.size() || a.equipment.size() != b.equipment.size() ||
        a.skills.size() != b.skills.size() || a.unlocks != b.unlocks ||
        a.source_endurance_energy_known != b.source_endurance_energy_known ||
        a.source_points_known != b.source_points_known ||
        a.source_stat_points != b.source_stat_points || a.source_skill_points != b.source_skill_points ||
        a.source_skill_slots_known != b.source_skill_slots_known ||
        a.source_faery_state_known != b.source_faery_state_known ||
        a.skill_slots.size() != b.skill_slots.size() || a.source_faery_list_id != b.source_faery_list_id) return false;
    for (std::size_t i = 0; i < a.inventory.size(); ++i)
        if (a.inventory[i].instance_id != b.inventory[i].instance_id ||
            a.inventory[i].definition_id != b.inventory[i].definition_id ||
            a.inventory[i].quantity != b.inventory[i].quantity) return false;
    for (std::size_t i = 0; i < a.equipment.size(); ++i)
        if (a.equipment[i].slot != b.equipment[i].slot ||
            a.equipment[i].item_instance_id != b.equipment[i].item_instance_id ||
            a.equipment[i].equipment_set != b.equipment[i].equipment_set ||
            a.equipment[i].source_slot != b.equipment[i].source_slot) return false;
    for (std::size_t i = 0; i < a.skills.size(); ++i)
        if (a.skills[i].id != b.skills[i].id || a.skills[i].rank != b.skills[i].rank) return false;
    for (std::size_t i = 0; i < a.skill_slots.size(); ++i)
        if (a.skill_slots[i].equipment_set != b.skill_slots[i].equipment_set ||
            a.skill_slots[i].slot != b.skill_slots[i].slot ||
            a.skill_slots[i].saved_skill_row != b.skill_slots[i].saved_skill_row) return false;
    for (std::size_t i = 0; i < a.faery_by_difficulty.size(); ++i) {
        if (a.faery_by_difficulty[i].current_faery != b.faery_by_difficulty[i].current_faery) return false;
        for (std::size_t j = 0; j < a.faery_by_difficulty[i].faeries.size(); ++j)
            if (a.faery_by_difficulty[i].faeries[j].state != b.faery_by_difficulty[i].faeries[j].state ||
                a.faery_by_difficulty[i].faeries[j].level != b.faery_by_difficulty[i].faeries[j].level) return false;
    }
    return true;
}

CharacterState fixture() {
    auto s = make_default_character("act1-player-17", "Prince \"Test\"", "rogue");
    s.stats = {17, 83.25f, 170.5f, 31.125f, 99.75f, 13.5f, 22.25f, 9.125f};
    s.stats.endurance = 17.75f; s.stats.energy = 4.5f; s.source_endurance_energy_known = true;
    s.source_stat_points=9;s.source_skill_points=1;s.source_points_known=true;
    s.experience = 0x100000002ULL;
    s.gold = 0x200000003ULL;
    s.inventory = {{"sword-instance", "act1.sword", 1}, {"potion-instance", "potion.health", 23}};
    s.equipment = {{"main_hand", "sword-instance", 0, 1}};
    s.skills = {{"dash", 0}, {"strike", 7}, {"strike", 0}};
    s.skill_slots = {{0, 0, 0}, {1, 0, 2}}; s.source_skill_slots_known = true;
    s.unlocks = {"act1.opening_seen", "act1.gate_open"};
    s.source_faery_list_id = 3; s.source_faery_state_known = true;
    s.faery_by_difficulty[0].current_faery = 2; s.faery_by_difficulty[0].faeries[2] = {1, 23};
    return s;
}

void write_u32(std::vector<char>& bytes, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i) bytes.push_back(static_cast<char>(value >> (8 * i)));
}
void write_u64(std::vector<char>& bytes, std::uint64_t value) {
    for (unsigned i = 0; i < 8; ++i) bytes.push_back(static_cast<char>(value >> (8 * i)));
}
void write_text(std::vector<char>& bytes, const std::string& value) {
    write_u32(bytes, static_cast<std::uint32_t>(value.size()));
    bytes.insert(bytes.end(), value.begin(), value.end());
}
void write_real(std::vector<char>& bytes, float value) {
    std::uint32_t bits; std::memcpy(&bits, &value, sizeof(bits)); write_u32(bytes, bits);
}
std::vector<char> legacy_v1_bytes(const CharacterState& state) {
    std::vector<char> bytes{'D','H','S','A','V','E',0,1};
    write_u32(bytes, 1); write_u32(bytes, 1);
    write_text(bytes, state.id); write_text(bytes, state.name); write_text(bytes, state.class_id);
    write_u32(bytes, state.stats.level);
    write_real(bytes, state.stats.health); write_real(bytes, state.stats.max_health);
    write_real(bytes, state.stats.resource); write_real(bytes, state.stats.max_resource);
    write_real(bytes, state.stats.strength); write_real(bytes, state.stats.dexterity); write_real(bytes, state.stats.intelligence);
    write_u64(bytes, state.experience); write_u64(bytes, state.gold);
    write_u32(bytes, static_cast<std::uint32_t>(state.inventory.size()));
    for (const auto& item : state.inventory) {
        write_text(bytes, item.instance_id); write_text(bytes, item.definition_id); write_u32(bytes, item.quantity);
    }
    write_u32(bytes, static_cast<std::uint32_t>(state.equipment.size()));
    for (const auto& item : state.equipment) { write_text(bytes, item.slot); write_text(bytes, item.item_instance_id); }
    write_u32(bytes, static_cast<std::uint32_t>(state.skills.size()));
    for (const auto& skill : state.skills) { write_text(bytes, skill.id); write_u32(bytes, skill.rank); }
    write_u32(bytes, static_cast<std::uint32_t>(state.unlocks.size()));
    for (const auto& unlock : state.unlocks) write_text(bytes, unlock);
    return bytes;
}

std::vector<char> read_bytes(const std::filesystem::path& p) {
    std::ifstream f(p, std::ios::binary);
    require(static_cast<bool>(f), "cannot read test save");
    return {std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
}
void write_bytes(const std::filesystem::path& p, const std::vector<char>& bytes) {
    std::ofstream f(p, std::ios::binary | std::ios::trunc);
    f.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    require(static_cast<bool>(f), "cannot write test corruption");
}

struct TempDirectory {
    std::filesystem::path path;
    TempDirectory() {
        const auto stamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        path = std::filesystem::temp_directory_path() / ("dhsc-save-tests-" + std::to_string(stamp));
        require(std::filesystem::create_directory(path), "cannot create test directory");
    }
    ~TempDirectory() { std::error_code error; std::filesystem::remove_all(path, error); }
};
}

int main() {
    try {
        TempDirectory temp;
        const auto save = temp.path / "character.save";
        const auto corrupt = temp.path / "corrupt.save";
        const auto original = fixture();
        std::string error;
        require(validate_character_state(original).ok(), "fixture must be valid");
        require(save_character(save, original, error), "first save failed: " + error);
        auto loaded = make_default_character("sentinel", "Sentinel", "mage");
        require(load_character(save, loaded, error), "load failed: " + error);
        require(equal(loaded, original), "roundtrip lost character fields");
        const auto bytes = read_bytes(save);
        require(bytes.size() > 16, "save unexpectedly short");
        // Older schema2 representation is unchanged except for the appended
        // schema3 payload length. Loading it must preserve unknown progress.
        auto old_v2=bytes;old_v2[12]=2;old_v2.resize(old_v2.size()-4);
        write_bytes(corrupt,old_v2);auto migrated_v2=loaded;
        require(load_character(corrupt,migrated_v2,error)&&migrated_v2.source_quest_progress_cqpg.empty(),"schema2 quest migration differs");
        require(read_bytes(corrupt)==old_v2,"schema2 read changed file");
        auto quest_character=original;
        auto& quest=quest_character.source_quest_progress_cqpg;
        quest={'C','Q','P','G'};
        const auto quest_word=[&](std::uint32_t value){for(unsigned i=0;i<4;++i)quest.push_back(static_cast<std::uint8_t>(value>>(8*i)));};
        quest_word(1);quest_word(64);quest_word(static_cast<std::uint32_t>(original.id.size()));
        quest.insert(quest.end(),original.id.begin(),original.id.end());
        quest.insert(quest.end(),6,0); // Source codec's six unknown buckets.
        require(save_character(corrupt,quest_character,error),"quest payload save failed: "+error);
        auto quest_loaded=loaded;require(load_character(corrupt,quest_loaded,error)&&equal(quest_character,quest_loaded),"quest payload roundtrip failed");
        const auto quest_file=read_bytes(corrupt);quest_character.id="wrong-owner";
        require(!save_character(corrupt,quest_character,error)&&read_bytes(corrupt)==quest_file,"wrong quest owner overwrote save");
        auto broken_quest_file=quest_file;broken_quest_file.pop_back();write_bytes(corrupt,broken_quest_file);
        const auto quest_before=quest_loaded;
        require(!load_character(corrupt,quest_loaded,error)&&equal(quest_loaded,quest_before),"truncated quest payload replaced destination");

        // A real schema-v1 byte stream is promoted only in memory. Semantic
        // values absent from that stream remain unknown/default until an
        // explicit caller save writes schema v2.
        const auto legacy_path = temp.path / "legacy-v1.save";
        auto legacy = original;
        legacy.skills = {{"dash", 3}, {"strike", 7}};
        legacy.equipment = {{"main_hand", "sword-instance"}};
        const auto legacy_bytes = legacy_v1_bytes(legacy);
        write_bytes(legacy_path, legacy_bytes);
        CharacterState migrated;
        require(load_character(legacy_path, migrated, error), "legacy v1 load failed: " + error);
        require(migrated.schema_version == character_schema_version && migrated.id == legacy.id &&
                migrated.name == legacy.name && migrated.stats.intelligence == legacy.stats.intelligence &&
                migrated.stats.endurance == 0 && migrated.stats.energy == 0 &&
                !migrated.source_endurance_energy_known && !migrated.source_points_known &&
                migrated.source_stat_points==0 && migrated.source_skill_points==0 && !migrated.source_skill_slots_known &&
                !migrated.source_faery_state_known && migrated.skill_slots.empty() &&
                migrated.source_faery_list_id == -1,
                "legacy v1 values/new-field unknown defaults differ");
        require(read_bytes(legacy_path) == legacy_bytes, "legacy read migrated the file without an explicit save");
        require(save_character(legacy_path, migrated, error), "explicit v2 rewrite failed: " + error);
        require(read_bytes(legacy_path) != legacy_bytes, "explicit save did not write the v2 representation");
        CharacterState migrated_again;
        require(load_character(legacy_path, migrated_again, error) && equal(migrated, migrated_again),
                "explicit v2 migration roundtrip differs: " + error);

        auto reject = [&](const std::vector<char>& bad, const char* label) {
            write_bytes(corrupt, bad);
            auto destination = fixture();
            destination.name = "Existing character";
            const auto before = destination;
            error.clear();
            require(!load_character(corrupt, destination, error), std::string(label) + " accepted");
            require(!error.empty(), std::string(label) + " has no error detail");
            require(equal(destination, before), std::string(label) + " modified destination");
        };
        auto malformed = bytes;
        malformed[0] ^= 0x20;
        reject(malformed, "malformed magic");
        reject({}, "empty save");
        reject(std::vector<char>(bytes.begin(), bytes.begin() + 15), "truncated header");
        reject(std::vector<char>(bytes.begin(), bytes.end() - 1), "truncated payload");
        auto unknown_version = bytes;
        unknown_version[8] = 99;
        unknown_version[9] = unknown_version[10] = unknown_version[11] = 0;
        reject(unknown_version, "unknown file version");
        auto unknown_schema = bytes;
        unknown_schema[12] = 99;
        unknown_schema[13] = unknown_schema[14] = unknown_schema[15] = 0;
        reject(unknown_schema, "unknown character schema");
        auto invalid_length = bytes;
        for (std::size_t i = 16; i < 20; ++i) invalid_length[i] = static_cast<char>(0xff);
        reject(invalid_length, "unbounded string length");
        auto trailing = bytes;
        trailing.push_back('x');
        reject(trailing, "trailing bytes");

        auto missing_destination = original;
        require(!load_character(temp.path / "missing.save", missing_destination, error),
                "missing save accepted");
        require(equal(missing_destination, original), "missing save modified destination");

        auto invalid = original;
        invalid.equipment[0].item_instance_id = "missing-item";
        require(!save_character(save, invalid, error), "invalid character save accepted");
        require(read_bytes(save) == bytes, "failed save destroyed previous file");
        invalid = original; invalid.equipment[0].source_slot = -1;
        require(!save_character(save, invalid, error) && read_bytes(save) == bytes,
                "partial source equipment identity damaged an existing save");
        invalid = original; invalid.skill_slots[0].saved_skill_row = 99;
        require(!save_character(save, invalid, error) && read_bytes(save) == bytes,
                "dangling saved-skill row damaged an existing save");
        invalid = original; invalid.faery_by_difficulty[0].current_faery = 5;
        require(!save_character(save, invalid, error) && read_bytes(save) == bytes,
                "out-of-range source faery selection damaged an existing save");

        auto replacement = original;
        replacement.name = "Replacement";
        replacement.experience += 19;
        replacement.inventory[1].quantity = 3;
        replacement.unlocks.push_back("act1.boss_defeated");
        require(save_character(save, replacement, error), "replacement save failed: " + error);
        require(load_character(save, loaded, error), "replacement load failed: " + error);
        require(equal(loaded, replacement), "replacement retained old data");

        // Empty collections are valid and must clear prior loaded collections.
        const auto defaults = make_default_character();
        require(save_character(save, defaults, error), "default save failed: " + error);
        require(load_character(save, loaded, error), "default load failed: " + error);
        require(equal(loaded, defaults), "empty collections did not replace prior data");
        std::cout << "save_tests: all checks passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "save_tests: " << e.what() << '\n';
        return 1;
    }
}
