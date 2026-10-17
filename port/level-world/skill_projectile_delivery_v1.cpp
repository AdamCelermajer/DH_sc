#include "skill_projectile_delivery_v1.hpp"

#include <cmath>
#include <limits>

namespace dh2::skill_projectile_delivery_v1 {
namespace {
bool point(const Value& value, const Services& services,
           Point3f* output) noexcept {
    if (!output || value.type != ValueType::userdata || !value.identity)
        return false;
    if (services.read_point) {
        try { return services.read_point(services.context,value.identity,output)==0; }
        catch (...) { return false; }
    }
    if (!std::isfinite(value.xyz[0]) || !std::isfinite(value.xyz[1]) ||
        !std::isfinite(value.xyz[2])) return false;
    *output = {value.xyz[0], value.xyz[1], value.xyz[2]};
    return true;
}
}

Status spawn(const Value* arguments, std::uint32_t count,
             std::uintptr_t character, std::uintptr_t skill_script,
             const Services* services, Result* output,
             std::string& error) noexcept {
    error.clear();
    if (!services || !services->spawn || !output || !arguments || count < 2 ||
        !character || !skill_script || arguments[0].type != ValueType::number ||
        arguments[1].type != ValueType::number ||
        !std::isfinite(arguments[0].number) ||
        !std::isfinite(arguments[1].number) ||
        arguments[0].number < 0.0f || arguments[0].number >= 2147483648.0f ||
        arguments[1].number < 0.0f ||
        arguments[1].number >= 2147483648.0f) {
        error = "SpawnSkillProjectile requires Character, skill script and two source numbers";
        return Status::invalid_request;
    }

    Result result{};
    result.extra_arguments = count > 2;
    Launch launch{};
    launch.character = character;
    launch.skill_script = skill_script;
    launch.projectile_id = static_cast<std::int32_t>(arguments[1].number);
    launch.skill_index = static_cast<std::int32_t>(arguments[0].number);
    if (count <= 2) {
        launch.info_kind = projectile_lifecycle_runtime_v1::SetInfoKind::flag_bool;
        launch.flag = false;
    } else if (arguments[2].type == ValueType::boolean) {
        launch.info_kind = projectile_lifecycle_runtime_v1::SetInfoKind::flag_bool;
        launch.flag = arguments[2].boolean != 0;
    } else if (arguments[2].type == ValueType::number &&
               std::isfinite(arguments[2].number)) {
        launch.info_kind = projectile_lifecycle_runtime_v1::SetInfoKind::angle_float;
        launch.angle_degrees = arguments[2].number;
    }
    if (count > 2 && arguments[2].type == ValueType::userdata) {
        if (!point(arguments[2], *services, &launch.position)) {
            error = "SpawnSkillProjectile position userdata is unavailable";
            return Status::invalid_request;
        }
        launch.has_position = true;
    }
    if (count > 3 && arguments[3].type == ValueType::userdata) {
        if (!point(arguments[3], *services, &launch.destination)) {
            error = "SpawnSkillProjectile destination userdata is unavailable";
            return Status::invalid_request;
        }
        launch.has_destination = true;
    }

    std::uintptr_t projectile = 0;
    try {
        if (services->spawn(services->context, launch, &projectile) != 0) {
            error = "ProjectileManager::Spawn provider failed";
            return Status::service_failed;
        }
        result.projectile = projectile;
        if (launch.has_position) {
            if (!services->set_position ||
                services->set_position(services->context, projectile,
                                       launch.position) != 0) {
                error = "Projectile::SetPosition provider failed";
                *output = result;
                return Status::service_failed;
            }
            result.position_written = true;
        }
        if (launch.has_destination) {
            if (!services->set_destination ||
                services->set_destination(services->context, projectile,
                                          launch.destination) != 0) {
                error = "Projectile::SetDestination provider failed";
                *output = result;
                return Status::service_failed;
            }
            result.destination_written = true;
        }
        // The source only pushes the result in its <=2 argument branch.
        result.returned = count <= 2 ? 1u : 0u;
        *output = result;
        error.clear();
        return Status::complete;
    } catch (...) {
        *output = result;
        error = "SpawnSkillProjectile provider threw";
        return Status::service_failed;
    }
}

Status set_target(const Value* arguments, std::uint32_t count,
                 const Services* services, Result* output,
                 std::string& error) noexcept {
    error.clear();
    if (!services || !output) {
        error = "SetProjectileTarget provider is unavailable";
        return Status::invalid_request;
    }
    Result result{};
    if (!arguments || count < 2 || arguments[0].type != ValueType::pointer ||
        !arguments[0].identity) {
        *output = result;
        return Status::complete;
    }
    if (!services->set_target) {
        error = "SetProjectileTarget owner provider is unavailable";
        return Status::missing_service;
    }
    const auto target = arguments[1].type == ValueType::userdata
        ? arguments[1].identity : std::uintptr_t{0};
    try {
        if (services->set_target(services->context, arguments[0].identity,
                                 target) != 0) {
            error = "Projectile::SetTarget provider failed";
            return Status::service_failed;
        }
    } catch (...) {
        error = "SetProjectileTarget provider threw";
        return Status::service_failed;
    }
    result.projectile = arguments[0].identity;
    result.target_written = true;
    *output = result;
    return Status::complete;
}

} // namespace dh2::skill_projectile_delivery_v1
