#pragma once

#include "projectile_manager_runtime_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::skill_projectile_delivery_v1 {

enum class ValueType : std::uint8_t {
    nil = 0, boolean = 1, pointer = 2, number = 3, string = 4,
    userdata = 7
};

struct Value {
    ValueType type{ValueType::nil};
    float number{};
    std::uint8_t boolean{};
    std::uintptr_t identity{};
    float xyz[3]{};
};

struct Point3f { float x{}, y{}, z{}; };

struct Launch {
    std::uintptr_t character{};
    std::uintptr_t skill_script{};
    std::int32_t skill_index{};
    std::int32_t projectile_id{};
    projectile_lifecycle_runtime_v1::SetInfoKind info_kind{
        projectile_lifecycle_runtime_v1::SetInfoKind::flag_bool};
    float angle_degrees{};
    bool flag{};
    std::uintptr_t collision_check_callback{0x3daff4u};
    std::uintptr_t collision_hit_callback{0x3dae7cu};
    Point3f position{};
    Point3f destination{};
    bool has_position{};
    bool has_destination{};
};

struct Services {
    void* context{};
    // Null projectile is a successful source result when the row is rejected
    // or the source pool cannot create a product. Nonzero means owner failure.
    int (*spawn)(void*, const Launch&, std::uintptr_t* projectile){};
    int (*set_position)(void*, std::uintptr_t projectile,
                        const Point3f&){};
    int (*set_destination)(void*, std::uintptr_t projectile,
                           const Point3f&){};
    int (*set_target)(void*, std::uintptr_t projectile,
                      std::uintptr_t target){};
    int (*read_point)(void*, std::uintptr_t point_userdata,
                      Point3f*){};
};

struct Result {
    std::uintptr_t projectile{};
    std::uint32_t returned{};
    bool extra_arguments{};
    bool position_written{};
    bool destination_written{};
    bool target_written{};
};

enum class Status : std::uint8_t {
    complete, invalid_request, missing_service, service_failed
};

// Recovered from Character::_SpawnSkillProjectile (0x3b99d0). The source
// calls SpawnProjectile first, then applies optional position/destination;
// only the <=2-argument branch pushes the resulting Pointer into ReturnValues.
Status spawn(const Value* arguments, std::uint32_t count,
             std::uintptr_t character, std::uintptr_t skill_script,
             const Services*, Result*, std::string& error) noexcept;

// Recovered from Character::_SetProjectileTarget (0x3ba298). A non-Pointer
// first argument or fewer than two values is a source no-op. The target is
// read through Value::getUserData and stored on the manager-owned projectile.
Status set_target(const Value* arguments, std::uint32_t count,
                 const Services*, Result*, std::string& error) noexcept;

} // namespace dh2::skill_projectile_delivery_v1
