#pragma once

#include "data.hpp"

namespace dh2::data {

// Portable projection of one source Structs::Projectile record. The source
// row is serialized as 59 packed bytes; this host type does not mirror its
// ARM32 in-memory size or padding.
struct ProjectileRowV1 {
    std::uint32_t row_index{};
    std::string table_name;
    bool dedicated{};
    bool disappear_on_hit{};
    std::int32_t expire_fx{};
    std::int32_t expire_sfx{};
    bool friendly_fire{};
    std::int32_t impact_fx{};
    std::int32_t impact_sfx{};
    bool is_laser{};
    bool is_magic{};
    float max_distance{};
    float max_rotation{};
    std::int32_t model_index{-1};
    std::string model_dictionary_name;
    std::string model_path;
    std::int32_t object_impact_fx{};
    std::int32_t object_impact_sfx{};
    bool stay_on_floor{};
    bool target_lock{};
    std::int32_t time_between_hit{};
    std::int32_t timer{};
    float velocity{};
    float velocity_damping{};
};

struct ProjectileTableV1 {
    std::vector<ProjectileRowV1> rows;
    Dictionary model_dictionary;
};

// Decodes the source ProjectileTable and its ProjectileDict. The expected
// 20-field schema and packed row width are checked before publishing output.
bool load_projectile_table_v1(Bytes records, Bytes names, Bytes schema,
                              Bytes dictionary_names, Bytes dictionary_values,
                              ProjectileTableV1& output, std::string& error);

const ProjectileRowV1* projectile_row_v1(const ProjectileTableV1& table,
                                         std::int32_t row) noexcept;
const ProjectileRowV1* projectile_row_v1(const ProjectileTableV1& table,
                                         const std::string& name) noexcept;

}  // namespace dh2::data
