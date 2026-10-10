#include "save_store.hpp"
#include "character_quest_blob.hpp"

#include <atomic>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace dh::foundation {
namespace {
constexpr std::size_t max_file_size = 16 * 1024 * 1024;
constexpr std::uint32_t max_entries = static_cast<std::uint32_t>(character_collection_limit);
constexpr std::uint32_t max_string_size = static_cast<std::uint32_t>(character_text_limit);
constexpr unsigned char magic[] = {'D', 'H', 'S', 'A', 'V', 'E', 0, 1};

std::uint32_t unsigned_bits(std::int32_t value) {
    std::uint32_t bits; std::memcpy(&bits, &value, sizeof(bits)); return bits;
}
std::int32_t signed_bits(std::uint32_t bits) {
    std::int32_t value; std::memcpy(&value, &bits, sizeof(value)); return value;
}

struct Writer {
    std::vector<unsigned char> bytes;
    void integer(std::uint64_t value, unsigned width) {
        for (unsigned i = 0; i < width; ++i) bytes.push_back(static_cast<unsigned char>(value >> (i * 8)));
        check_size();
    }
    void check_size() const {
        if (bytes.size() > max_file_size) throw std::runtime_error("Save exceeds the 16 MiB limit");
    }
    void string(const std::string& value) {
        if (value.size() > max_string_size) throw std::runtime_error("Save string exceeds the character text limit");
        integer(value.size(), 4);
        bytes.insert(bytes.end(), value.begin(), value.end());
        check_size();
    }
    void real(float value) {
        static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559,
                      "Save format requires IEEE-754 32-bit floats");
        std::uint32_t bits;
        std::memcpy(&bits, &value, sizeof(bits));
        integer(bits, 4);
    }
    void count(std::size_t value) {
        if (value > max_entries) throw std::runtime_error("Save collection exceeds the entry limit");
        integer(value, 4);
    }
};

struct Reader {
    const std::vector<unsigned char>& bytes;
    std::size_t position = 0;
    std::uint64_t integer(unsigned width) {
        if (width > bytes.size() - position) throw std::runtime_error("Truncated save");
        std::uint64_t value = 0;
        for (unsigned i = 0; i < width; ++i) value |= std::uint64_t(bytes[position++]) << (i * 8);
        return value;
    }
    std::uint32_t u32() { return static_cast<std::uint32_t>(integer(4)); }
    std::string string() {
        const auto size = u32();
        if (size > max_string_size || size > bytes.size() - position)
            throw std::runtime_error("Invalid save string length");
        std::string value(reinterpret_cast<const char*>(bytes.data() + position), size);
        position += size;
        return value;
    }
    float real() {
        const auto bits = u32();
        float value;
        std::memcpy(&value, &bits, sizeof(value));
        return value;
    }
    std::uint32_t count() {
        const auto value = u32();
        if (value > max_entries) throw std::runtime_error("Invalid save collection length");
        return value;
    }
    bool flag() {
        const auto value = integer(1);
        if (value > 1) throw std::runtime_error("Invalid save boolean");
        return value != 0;
    }
};

void require_valid(const CharacterState& state) {
    if (state.schema_version != character_schema_version) throw std::runtime_error("Unsupported character schema version");
    const auto result = validate_character_state(state);
    if (!result.ok()) throw std::runtime_error("Invalid character state: " + result.errors.front());
}

std::vector<unsigned char> encode(const CharacterState& state) {
    require_valid(state);
    Writer w;
    w.bytes.insert(w.bytes.end(), std::begin(magic), std::end(magic));
    w.integer(1, 4); // File format version, separate from character schema.
    w.integer(state.schema_version, 4);
    w.string(state.id); w.string(state.name); w.string(state.class_id);
    w.integer(state.stats.level, 4);
    w.real(state.stats.health); w.real(state.stats.max_health);
    w.real(state.stats.resource); w.real(state.stats.max_resource);
    w.real(state.stats.strength); w.real(state.stats.dexterity); w.real(state.stats.intelligence);
    w.real(state.stats.endurance); w.real(state.stats.energy); w.integer(state.source_endurance_energy_known ? 1 : 0, 1);
    w.integer(state.source_stat_points, 4); w.integer(state.source_skill_points, 4);
    w.integer(state.source_points_known ? 1 : 0, 1);
    w.integer(state.experience, 8); w.integer(state.gold, 8);
    w.count(state.inventory.size());
    for (const auto& item : state.inventory) {
        w.string(item.instance_id); w.string(item.definition_id); w.integer(item.quantity, 4);
    }
    w.count(state.equipment.size());
    for (const auto& item : state.equipment) {
        w.string(item.slot); w.string(item.item_instance_id);
        w.integer(unsigned_bits(item.equipment_set), 4);
        w.integer(unsigned_bits(item.source_slot), 4);
    }
    w.count(state.skills.size());
    for (const auto& skill : state.skills) { w.string(skill.id); w.integer(skill.rank, 4); }
    w.count(state.unlocks.size());
    for (const auto& unlock : state.unlocks) w.string(unlock);
    w.integer(state.source_skill_slots_known ? 1 : 0, 1);
    w.count(state.skill_slots.size());
    for (const auto& slot : state.skill_slots) {
        w.integer(slot.equipment_set, 4); w.integer(slot.slot, 4); w.integer(slot.saved_skill_row, 4);
    }
    w.integer(unsigned_bits(state.source_faery_list_id), 4);
    w.integer(state.source_faery_state_known ? 1 : 0, 1);
    for (const auto& difficulty : state.faery_by_difficulty) {
        w.integer(static_cast<std::uint32_t>(difficulty.current_faery), 4);
        for (const auto& faery : difficulty.faeries) {
            w.integer(faery.state, 1); w.integer(faery.level, 2);
        }
    }
    w.integer(state.source_quest_progress_cqpg.size(),4);
    w.bytes.insert(w.bytes.end(),state.source_quest_progress_cqpg.begin(),state.source_quest_progress_cqpg.end());
    // Schema v4 tail (append-only): difficulty, per-slot menu metadata, visited modules.
    w.integer(unsigned_bits(state.current_difficulty),4);
    w.integer(unsigned_bits(state.unlocked_difficulty),4);
    w.integer(state.menu_metadata.known?1:0,1);
    w.integer(state.menu_metadata.save_time,4);
    for(const auto row:state.menu_metadata.level_row)w.integer(unsigned_bits(row),4);
    for(const auto act:state.menu_metadata.current_act)w.integer(unsigned_bits(act),4);
    w.integer(state.visited_modules.size(),4);
    for(const auto& entry:state.visited_modules){
        w.string(entry.level_uri);w.integer(entry.module_id,4);w.integer(entry.visited,1);
    }
    w.check_size();
    return std::move(w.bytes);
}

CharacterState decode(const std::vector<unsigned char>& bytes) {
    Reader r{bytes};
    for (const auto expected : magic)
        if (r.integer(1) != expected) throw std::runtime_error("Not a DH foundation save");
    if (r.u32() != 1) throw std::runtime_error("Unsupported save format version");
    CharacterState state;
    const auto serialized_schema = r.u32();
    if (serialized_schema < 1 || serialized_schema > character_schema_version)
        throw std::runtime_error("Unsupported character schema version");
    state.schema_version = character_schema_version;
    state.id = r.string(); state.name = r.string(); state.class_id = r.string();
    state.stats.level = r.u32();
    state.stats.health = r.real(); state.stats.max_health = r.real();
    state.stats.resource = r.real(); state.stats.max_resource = r.real();
    state.stats.strength = r.real(); state.stats.dexterity = r.real(); state.stats.intelligence = r.real();
    if (serialized_schema >= 2) {
        state.stats.endurance = r.real(); state.stats.energy = r.real();
        state.source_endurance_energy_known = r.flag();
        state.source_stat_points = r.u32(); state.source_skill_points = r.u32();
        state.source_points_known = r.flag();
    }
    state.experience = r.integer(8); state.gold = r.integer(8);
    for (auto count = r.count(); count > 0; --count) {
        InventoryItem item;
        item.instance_id = r.string(); item.definition_id = r.string(); item.quantity = r.u32();
        state.inventory.push_back(std::move(item));
    }
    for (auto count = r.count(); count > 0; --count) {
        EquipmentBinding item;
        item.slot = r.string(); item.item_instance_id = r.string();
        if (serialized_schema >= 2) {
            item.equipment_set = signed_bits(r.u32());
            item.source_slot = signed_bits(r.u32());
        }
        state.equipment.push_back(std::move(item));
    }
    for (auto count = r.count(); count > 0; --count) {
        SkillProgress skill;
        skill.id = r.string(); skill.rank = r.u32();
        state.skills.push_back(std::move(skill));
    }
    for (auto count = r.count(); count > 0; --count) state.unlocks.push_back(r.string());
    if (serialized_schema >= 2) {
        state.source_skill_slots_known = r.flag();
        for (auto count = r.count(); count > 0; --count)
            state.skill_slots.push_back({r.u32(), r.u32(), r.u32()});
        state.source_faery_list_id = signed_bits(r.u32());
        state.source_faery_state_known = r.flag();
        for (auto& difficulty : state.faery_by_difficulty) {
            difficulty.current_faery = signed_bits(r.u32());
            for (auto& faery : difficulty.faeries) {
                faery.state = static_cast<std::uint8_t>(r.integer(1));
                faery.level = static_cast<std::uint16_t>(r.integer(2));
            }
        }
    }
    if(serialized_schema>=3){
        const auto size=r.u32();
        if(size>character_quest_blob_limit||size>bytes.size()-r.position)throw std::runtime_error("Invalid source quest progress length");
        state.source_quest_progress_cqpg.assign(bytes.begin()+r.position,bytes.begin()+r.position+size);r.position+=size;
    }
    if(serialized_schema>=4){
        state.current_difficulty=signed_bits(r.u32());
        state.unlocked_difficulty=signed_bits(r.u32());
        state.menu_metadata.known=r.flag();
        state.menu_metadata.save_time=r.u32();
        for(auto& row:state.menu_metadata.level_row)row=signed_bits(r.u32());
        for(auto& act:state.menu_metadata.current_act)act=signed_bits(r.u32());
        const auto visited=r.u32();
        if(visited>character_visited_limit)throw std::runtime_error("Invalid save visited-module length");
        for(auto count=visited;count>0;--count){
            CharacterVisitedModule entry;
            entry.level_uri=r.string();entry.module_id=r.u32();
            entry.visited=static_cast<std::uint8_t>(r.integer(1));
            state.visited_modules.push_back(std::move(entry));
        }
    }
    if (r.position != bytes.size()) throw std::runtime_error("Unexpected trailing save data");
    require_valid(state);
    return state;
}

bool replace_file(const std::filesystem::path& source, const std::filesystem::path& destination,
                  std::string& error) {
#ifdef _WIN32
    if (MoveFileExW(source.c_str(), destination.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        return true;
    error = "Cannot replace save file (Windows error " + std::to_string(GetLastError()) + ")";
    return false;
#else
    std::error_code ec;
    std::filesystem::rename(source, destination, ec);
    if (!ec) return true;
    error = "Cannot replace save file: " + ec.message();
    return false;
#endif
}
} // namespace

bool save_character(const std::filesystem::path& path, const CharacterState& state, std::string& error) {
    error.clear();
    std::filesystem::path temporary;
    bool temporary_created = false;
    try {
        if (path.empty() || path.filename().empty()) throw std::runtime_error("Save filename is empty");
        const auto bytes = encode(state);
        static std::atomic<std::uint64_t> sequence{0};
        temporary = path;
        temporary += ".tmp." + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count())
                   + "." + std::to_string(sequence.fetch_add(1));
#ifdef _WIN32
        const HANDLE handle = CreateFileW(temporary.c_str(), GENERIC_WRITE, 0, nullptr,
                                          CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (handle == INVALID_HANDLE_VALUE) throw std::runtime_error("Cannot create save temporary file");
        temporary_created = true;
        DWORD written = 0;
        const bool wrote = WriteFile(handle, bytes.data(), static_cast<DWORD>(bytes.size()), &written, nullptr)
                           && written == bytes.size();
        const bool flushed = wrote && FlushFileBuffers(handle);
        const bool closed = CloseHandle(handle);
        if (!wrote || !flushed || !closed) throw std::runtime_error("Cannot write save temporary file");
#else
        if (std::filesystem::exists(temporary)) throw std::runtime_error("Save temporary filename collision");
        std::ofstream stream(temporary, std::ios::binary | std::ios::trunc);
        if (!stream) throw std::runtime_error("Cannot create save temporary file");
        temporary_created = true;
        stream.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        stream.flush();
        if (!stream) throw std::runtime_error("Cannot write save temporary file");
        stream.close();
        if (!stream) throw std::runtime_error("Cannot close save temporary file");
#endif
        if (replace_file(temporary, path, error)) return true;
    } catch (const std::exception& exception) {
        error = exception.what();
    }
    if (temporary_created) {
        std::error_code ignored;
        std::filesystem::remove(temporary, ignored);
    }
    return false;
}

bool load_character(const std::filesystem::path& path, CharacterState& state, std::string& error) {
    error.clear();
    try {
        std::ifstream stream(path, std::ios::binary | std::ios::ate);
        if (!stream) throw std::runtime_error("Cannot open save file");
        const auto length = stream.tellg();
        if (length < 0 || length > static_cast<std::streamoff>(max_file_size))
            throw std::runtime_error("Invalid save file size");
        std::vector<unsigned char> bytes(static_cast<std::size_t>(length));
        stream.seekg(0);
        if (!bytes.empty()) stream.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        if (!stream) throw std::runtime_error("Cannot read save file");
        auto candidate = decode(bytes);
        state = std::move(candidate);
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}
} // namespace dh::foundation
