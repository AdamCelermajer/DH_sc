#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::data::v2_event_table_v1 {

struct Bytes { const std::uint8_t* data = nullptr; std::uint32_t size = 0; };

enum class ObjectiveType : std::int32_t {
    kill_x_enemies = 0,
    clear_enemies = 1,
    trigger_plate = 2,
    destroy_game_object = 3,
    move_in_zone = 4,
    talk_to_npc = 5,
    automatic = 6,
    open_game_object = 7,
    trigger_on = 8,
    picked_up_liftable = 9,
    kill_enemy_template = 10,
    clear_enemy_template = 11,
    gather_loot = 12,
    invalid = 13,
};

struct Trigger {
    std::int32_t type = 0;
    std::int32_t op1 = 0;
    std::int32_t op2 = 0;
    std::string text1;
    std::string text2;
    std::int32_t value1 = 0;
    std::int32_t value2 = 0;
    std::int32_t value3 = 0;
};

struct Event {
    std::string name;
    std::int32_t level = 0;
    std::string script;
    std::vector<Trigger> triggers;
};

struct EventStates {
    std::int32_t active = 0;
    std::int32_t completed = 0;
    std::int32_t count = 0;
    std::int32_t inactive = 0;
};

struct Table {
    std::vector<Event> rows;
    EventStates states;
    std::uint32_t packed_bytes_consumed = 0;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    malformed,
    unsupported,
};

// Parses the recovered v2Event stream, its parallel row-name table, and the
// v2EventState constants. The binary struct-name file is empty in the source
// cache; field layout is verified against the recovered v2Event reader.
bool load(Bytes packed, Bytes names, Bytes constants, Table& out, std::string& error);
const Event* find(const Table& table, const char* name) noexcept;

} // namespace dh2::data::v2_event_table_v1
