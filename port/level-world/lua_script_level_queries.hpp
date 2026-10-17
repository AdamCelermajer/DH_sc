#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::lua_script_level_queries {

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    service_unavailable = 2,
    service_failed = 3,
    service_exception = 4,
};

enum class OutputPath : std::uint32_t {
    none = 0,
    host_player_level = 1,
    current_level_difficulty = 2,
    sentinel_level = 3,
    selected_level_range = 4,
};

// Only the source fields consumed by Arguments front-value inspection are
// projected. type is the original sfc::script::lua::Value type tag; number is
// read later through the Value::getNumber provider if type==3.
struct Argument {
    std::uintptr_t identity;
    std::uint32_t type;
    std::uint32_t reserved;
};

// Borrowed live view of the source Arguments vector. The wrapper reads count
// and front only after it has called GetCurrentLevel and captured Level+0x3c.
struct Arguments {
    Argument* values;
    std::uint32_t count;
    std::uint32_t reserved;
};

struct Services {
    void* context;
    // HostPlayerLevel: global Application, its +0x40 PlayerManager, then the
    // exact GetHostingPlayer call. These are separate so call order and the
    // manager receiver remain visible to providers/tests.
    std::int32_t (*get_application)(void*, std::uintptr_t* application);
    std::int32_t (*get_application_player_manager)(void*, std::uintptr_t application,
                                                    std::uintptr_t* manager);
    std::int32_t (*get_hosting_player)(void*, std::uintptr_t manager,
                                       std::uintptr_t* player);
    std::int32_t (*read_player_word)(void*, std::uintptr_t player,
                                     std::uint32_t byte_offset, std::int32_t* value);

    // Application::GetCurrentLevel is a separate global-level getter; it does
    // not read Application+0x40. Field reads use the captured Level identity.
    std::int32_t (*get_current_level)(void*, std::uintptr_t* level);
    std::int32_t (*read_level_word)(void*, std::uintptr_t level,
                                    std::uint32_t byte_offset, std::int32_t* value);

    // GetCurrentLevelRange's numeric front path calls Value::getNumber, then
    // the original __aeabi_f2iz conversion. Non-number/default paths call
    // neither provider.
    std::int32_t (*value_get_number)(void*, std::uintptr_t value, float* number);
    std::int32_t (*float_to_signed_int)(void*, float number, std::int32_t* integer);

    // The current LevelTable object is captured once after argument
    // conversion. Each read_level_table_word call must resolve its current
    // backing-row pointer, because source reloads that pointer after the first
    // ReturnValues push. The callback receives the captured table identity,
    // ARM32-wrapped row byte offset, and exact source word offset.
    std::int32_t (*get_level_table)(void*, std::uintptr_t* table);
    std::int32_t (*read_level_table_word)(void*, std::uintptr_t table,
                                          std::uint32_t row_byte_offset,
                                          std::uint32_t word_offset,
                                          std::int32_t* value);
    std::int32_t (*push_integer)(void*, std::int32_t value);
};

struct Result {
    OutputPath output_path;
    std::uint32_t service_calls;
    std::uint32_t values_pushed;
    std::uint32_t row_byte_offset;
    std::int32_t level_oid;
    std::int32_t selected_difficulty;
    std::int32_t values[2];
};

Status get_host_player_level(const Services* services, Result* result);
Status get_host_player_difficulty(const Services* services, Result* result);
Status get_current_level_range(const Arguments* arguments,
                               const Services* services, Result* result);

static_assert(sizeof(Argument) == 16, "argument view layout");
static_assert(sizeof(Arguments) == 16, "arguments view layout");
static_assert(sizeof(Result) == 32, "query result layout");

}  // namespace dh2::lua_script_level_queries
