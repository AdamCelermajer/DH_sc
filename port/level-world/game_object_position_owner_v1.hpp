#pragma once

#include <cstdint>
#include <string>

namespace dh2::game_object_position_owner_v1 {

using Identity = std::uint64_t;

struct Point3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Owner {
    Identity identity = 0;
    Identity instance_transform = 0;
    Identity physical_object = 0;
    Identity visual_object = 0;
    Point3 position{};
};

enum class Status { complete, invalid_argument, service_failed };

struct Services {
    void* context = nullptr;
    int (*translate_instance_transform)(void*, Identity owner,
        Identity transform, Point3 delta, std::string&) = nullptr;
    int (*update_absolute_aabb)(void*, Identity owner, std::string&) = nullptr;
    int (*physical_set_position)(void*, Identity owner, Identity physical,
        float x, float y, std::string&) = nullptr;
    int (*visual_sync_position)(void*, Identity owner, Identity visual,
        std::string&) = nullptr;
    int (*set_destination)(void*, Identity owner, Point3,
        std::string&) = nullptr;
    int (*visual_force_update_position)(void*, Identity owner,
        Identity visual, std::string&) = nullptr;
};

Status set_position(Owner*, Point3, bool update_destination,
                    const Services*, std::string& error);
Status force_update_position(const Owner*, const Services*,
                             std::string& error);

} // namespace dh2::game_object_position_owner_v1
