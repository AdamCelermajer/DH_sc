#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

extern "C" {
#include "../adam-script-runtime/script_runtime.h"
}

namespace dh2::timer_trap_external_script_v1 {

struct Source {
    const void* bytes{};
    std::size_t size{};
    const char* path{};
};

// Providers for the natives TriggerTrap::createBindings (0x39dd98) and the
// GameObject base bindings expose to the trap's per-GameObject external script
// (data/scripts/objects/<Script>.luac, GameObject::LoadExternalScript):
//   GetOwner      TriggerTrap::_GetOwner    0x39ea90
//   GetDamager    TriggerTrap::_GetDamager  0x39ea64
//   Remove        TriggerTrap::_Remove      0x39dd18
//   DealDamages   GameObject::_DealDamages  0x3906fc
//   GetPyOID / GetPyCst / Trace              LuaScript natives
// A missing provider is a required-service failure; callbacks never throw.
struct Services {
    void* context{};
    bool (*get_py_oid)(void*, const char* category, const char* member,
                       std::int32_t& value, std::string& error){};
    bool (*get_py_constant)(void*, const char* category, const char* member,
                            std::int32_t& value, std::string& error){};
    // _DealDamages after its argument checks: F_GOAttack + F_ApplyResult of
    // `damager` (a GameObjectDamager OID) against the Character `victim`;
    // `character` is false for an identity that is not a Character and `amount`
    // is the raw attack amount pushed back to the script.
    bool (*deal_damages)(void*, std::uintptr_t victim, std::int32_t damager,
                         bool& character, std::int32_t& amount,
                         std::string& error){};
    bool (*remove)(void*, std::string& error){};
    std::uintptr_t (*owner)(void*){};
    std::int32_t (*damager)(void*){};
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
    // binds the trap natives, then loads the one exact external script into
    // this per-GameObject VM (the original TriggerTrap owns one LuaScript).
    Status initialize(Source, const Services&, std::string& error,
                      std::size_t memory_limit = 2U * 1024U * 1024U);
    // TriggerTrap::CanTrigger: LuaScript::Call("OnTrigger", victim); only a
    // first return of boolean false denies.
    Status on_trigger(std::uintptr_t victim, bool& allowed, std::string& error);
    // TriggerTrap::Update: LuaScript::Call("OnDamageRoll", victim).
    Status on_damage_roll(std::uintptr_t victim, std::string& error);
    // TriggerTrap::__EventCallback: LuaScript::Call("OnAnimEvent", name).
    Status on_anim_event(const char* name, std::string& error);
    Status reset(std::string& error) noexcept;

    bool ready() const noexcept;
    std::uintptr_t vm_identity() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    bool busy_{};
};

const char* status_name(Status) noexcept;

} // namespace dh2::timer_trap_external_script_v1
