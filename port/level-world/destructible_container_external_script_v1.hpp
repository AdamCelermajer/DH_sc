#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

extern "C" {
#include "../adam-script-runtime/script_runtime.h"
}

namespace dh2::destructible_container_external_script_v1 {

struct Source {
    const void* bytes{};
    std::size_t size{};
    const char* path{};
};

struct SummonResult {
    bool returned_object{};
    std::uintptr_t identity{};
};

// Callbacks borrow the canonical GameObject/Level context for this one
// ExternalScript. Summon receives the source Lua values unchanged; the caller
// owns Character::CreateNPC and ObjectManager/RoomZone enrollment.
struct Services {
    void* context{};
    bool (*get_random)(void*, std::uint32_t bound, std::int32_t& value,
                       std::string& error){};
    bool (*get_py_oid)(void*, const char* category, const char* member,
                       std::int32_t& value, std::string& error){};
    bool (*summon)(void*, std::uintptr_t caller,
                   const dh2_script_value* arguments, std::uint32_t count,
                   SummonResult&, std::string& error){};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    allocation_failed,
    service_unavailable,
    source_load_failed,
    callback_failed,
    busy,
};

class Owner final {
public:
    Owner();
    ~Owner();
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    // Mirrors LuaScript::BindFunction's base/math/table/string opening order,
    // then loads the one exact external script into this per-GameObject VM.
    Status initialize(Source, std::uintptr_t caller, const Services&,
                      std::string& error,
                      std::size_t memory_limit = 2U * 1024U * 1024U);
    // Source LuaScript::Call("OnOpen") is zero-argument when opener is null,
    // otherwise it passes the native source object identity as the callback's
    // one argument. The retained script is called directly, without a Lua shim.
    Status on_open(std::uintptr_t opener, std::string& error);
    Status reset(std::string& error) noexcept;

    bool ready() const noexcept;
    std::uintptr_t caller_identity() const noexcept;
    std::uintptr_t vm_identity() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    bool busy_{};
};

const char* status_name(Status) noexcept;

} // namespace dh2::destructible_container_external_script_v1
