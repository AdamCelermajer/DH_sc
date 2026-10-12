#include "world.hpp"
#include "source_layout_adapter.h"
#include "../game-data/data.hpp"

#include <cmath>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

namespace {
void set_error(char* output, std::size_t capacity, const char* message) {
    if (!output || !capacity) return;
    std::snprintf(output, capacity, "%s", message ? message : "Source layout rejected");
}

void write_word(std::uint8_t* output, std::uint32_t value) {
    output[0] = static_cast<std::uint8_t>(value);
    output[1] = static_cast<std::uint8_t>(value >> 8);
    output[2] = static_cast<std::uint8_t>(value >> 16);
    output[3] = static_cast<std::uint8_t>(value >> 24);
}

bool write_float(std::uint8_t* output, float value) {
    if (!std::isfinite(value)) return false;
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    write_word(output, bits);
    return true;
}

bool parse_entrypoint_id(const char* text, std::int32_t& result) {
    if (!text || !*text) return false;
    std::uint32_t value = 0;
    for (const auto* p = reinterpret_cast<const unsigned char*>(text); *p; ++p) {
        if (*p < '0' || *p > '9') return false;
        const auto digit = static_cast<std::uint32_t>(*p - '0');
        if (value > (static_cast<std::uint32_t>(INT32_MAX) - digit) / 10) return false;
        value = value * 10 + digit;
    }
    result = static_cast<std::int32_t>(value);
    return true;
}

bool condition_absent(const char* value) {
    if (!value) return true;
    while (*value && std::isspace(static_cast<unsigned char>(*value))) ++value;
    const char* end = value + std::strlen(value);
    while (end != value && std::isspace(static_cast<unsigned char>(end[-1]))) --end;
    if (end == value) return true;
    static constexpr char invalid[] = "invalid";
    if (end - value != static_cast<std::ptrdiff_t>(sizeof(invalid) - 1)) return false;
    for (std::size_t i = 0; i < sizeof(invalid) - 1; ++i)
        if (static_cast<char>(std::tolower(static_cast<unsigned char>(value[i]))) != invalid[i])
            return false;
    return true;
}

bool unconditional(const dh2::world::Object& object) {
    return condition_absent(dh2_world_field(&object, "activate_cond")) &&
        condition_absent(dh2_world_field(&object, "deactivate_cond"));
}

void write_dword(std::uint8_t* output, std::uint32_t value) {
    write_word(output, value);
}

bool write_entrypoint(std::uint8_t* record, const dh2::world::Object& object) {
    std::int32_t id = 0;
    (void)parse_entrypoint_id(dh2_world_field(&object, "entrypointID"), id);
    std::uint32_t id_bits = 0;
    std::memcpy(&id_bits, &id, sizeof(id_bits));
    write_dword(record, id_bits);
    write_dword(record + 4, object.module_index);
    const auto name_length = std::strlen(object.name);
    std::memcpy(record + 8, object.name, name_length);
    std::size_t offset = 72;
    for (unsigned axis = 0; axis < 3; ++axis, offset += 4)
        if (!write_float(record + offset, object.local.position[axis])) return false;
    for (unsigned axis = 0; axis < 3; ++axis, offset += 4)
        if (!write_float(record + offset, object.local.rotation_degrees[axis])) return false;
    for (unsigned axis = 0; axis < 3; ++axis, offset += 4)
        if (!write_float(record + offset, object.local.scale[axis])) return false;
    for (unsigned axis = 0; axis < 3; ++axis, offset += 4)
        if (!write_float(record + offset, object.world_position[axis])) return false;
    // The importer currently accepts only translation-only module placement.
    // Consequently world rotation and scale equal the object-local values.
    for (unsigned axis = 0; axis < 3; ++axis, offset += 4)
        if (!write_float(record + offset, object.local.rotation_degrees[axis])) return false;
    for (unsigned axis = 0; axis < 3; ++axis, offset += 4)
        if (!write_float(record + offset, object.local.scale[axis])) return false;
    return true;
}

bool fixed_ascii(std::uint8_t* output, std::size_t width, const char* value) {
    if (!value) return false;
    const auto length = std::strlen(value);
    if (!length || length >= width) return false;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = static_cast<unsigned char>(value[i]);
        if (c < 32 || c > 126) return false;
    }
    std::memcpy(output, value, length);
    return true;
}

bool copy_source_text(char* output, std::size_t capacity, const char* value) {
    if (!output || !capacity) return false;
    if (!value) value = "";
    const auto length = std::strlen(value);
    if (length >= capacity) return false;
    std::memcpy(output, value, length + 1);
    return true;
}

bool parse_source_global(const char* value, std::uint8_t& output) {
    // ObjectBase::DeclareProperties supplies the source default of zero when
    // the MGP omits isGlobal. Explicit values go through StrToObj(bool)
    // 0x30f020, which is atoi(value) != 0; DARKWOOD's authored
    // "1charpropsname = WanderingEddie" typo therefore reads as true.
    output = 0;
    if (!value) return true;
    output = std::atoi(value) != 0 ? 1 : 0;
    return true;
}

struct ExpectedDirectActor {
    std::uint32_t module_index;
    std::uint32_t source_record;
    const char* name;
    const char* character;
};

constexpr std::array<ExpectedDirectActor, 5> expected_direct_actors{{
    {2, 1, "_prim_Monster_53_03_001", "Troll"},
    {3, 1, "_prim_Monster_04_324", "Swamp_LizadMan_Type2"},
    {3, 3, "_prim_Monster_08_329", "Swamp_LizadMan_Type1"},
    {5, 0, "_prim_Monster_3_276", "Troll"},
    {5, 2, "_prim_Monster_3", "Swamp_LizadMan_Type2"},
}};

struct ExpectedStaticDecor {
    std::uint32_t module_index;
    std::uint32_t source_record;
    const char* name;
    const char* root;
    const char* source_path;
};

constexpr const char* swamp_prop_dae =
    "data/3d/props/swamp/prop_swamp_corpses.bdae";
constexpr std::array<ExpectedStaticDecor, 10> expected_static_decors{{
    {1, 5, "_prop_corpse_01_001", "_prop_corpse_01",
        "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {1, 6, "_prop_corpse_02_001", "_prop_corpse_02",
        "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {1, 7, "_prop_small_bloodstain_001", "_prop_small_bloodstain",
        "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {1, 8, "_prop_small_bloodstain_02", "_prop_small_bloodstain",
        "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {2, 3, "_prop_corpse_02_001", "_prop_corpse_02",
        "data/3d/modules/swamp/mvp/obj_1of4_brdwalk_nse_00.mvp"},
    {3, 1, "_prop_corpses_small_pile_001", "_prop_corpses_small_pile",
        "data/3d/modules/swamp/mvp/corner_ruin_ws_00.mvp"},
    {4, 6, "_prop_corpse_01_001", "_prop_corpse_01",
        "data/3d/modules/swamp/mvp/merchantcamp_ruins_swe_00.mvp"},
    {4, 7, "_prop_corpse_02_001", "_prop_corpse_02",
        "data/3d/modules/swamp/mvp/merchantcamp_ruins_swe_00.mvp"},
    {6, 6, "_prop_small_bloodstain_001", "_prop_small_bloodstain",
        "data/3d/modules/swamp/mvp/deadend_brdwalk_w_00.mvp"},
    {8, 3, "_prop_corpses_pile_001", "_prop_corpses_pile",
        "data/3d/modules/swamp/mvp/obj_2of4_brdwalk_sw_00.mvp"},
}};

bool contains_ascii_case_insensitive(const char* text, const char* needle) {
    if (!text || !needle || !*needle) return false;
    for (const char* start = text; *start; ++start) {
        const char* left = start;
        const char* right = needle;
        while (*left && *right &&
            std::tolower(static_cast<unsigned char>(*left)) ==
                std::tolower(static_cast<unsigned char>(*right))) {
            ++left;
            ++right;
        }
        if (!*right) return true;
    }
    return false;
}

bool non_whitespace(const char* value) {
    if (!value) return false;
    for (; *value; ++value)
        if (!std::isspace(static_cast<unsigned char>(*value))) return true;
    return false;
}

bool static_decor_has_condition_or_script(const dh2::world::Object& object) {
    for (std::uint32_t i = 0; i < object.field_count; ++i) {
        const auto& field = object.fields[i];
        if (contains_ascii_case_insensitive(field.name, "cond") &&
            !condition_absent(field.value))
            return true;
        if (contains_ascii_case_insensitive(field.name, "script") &&
            non_whitespace(field.value))
            return true;
    }
    return false;
}

bool direct_monster_record(const dh2::world::Object& object) {
    const auto* character = dh2_world_field(&object, "charpropsname");
    const auto* template_name = dh2_world_field(&object, "_templateName");
    if (std::strcmp(object.gametype, "Character") ||
        !template_name || std::strcmp(template_name, "Monster") ||
        !character || !*character ||
        !unconditional(object))
        return false;
    const auto* auto_spawn = dh2_world_field(&object, "auto_spawn");
    if (auto_spawn && !std::strcmp(auto_spawn, "0")) return false;
    const auto* ai_state = dh2_world_field(&object, "ai_state");
    return !ai_state || !*ai_state || !std::strcmp(ai_state, "Idle");
}

bool direct_monster_candidate(const dh2::world::Object& object) {
    if (!direct_monster_record(object)) return false;
    const auto* spawn_probability = dh2_world_field(&object, "spawn_prob");
    // Missing/empty uses the current source default (100). The selected set
    // only supports an explicit canonical 100; any changed probability must
    // fail closed instead of being approximated by unconditional DACT rows.
    return !spawn_probability || !*spawn_probability ||
        !std::strcmp(spawn_probability, "100");
}

struct SourceOwner {
    dh2::world::SourceLevel level{};
    ~SourceOwner() { dh2_world_free(&level); }
};

bool valid_dact_float(float value, float limit) {
    return std::isfinite(value) && std::abs(value) <= limit;
}
}

extern "C" int dh2_world_compile_static_layout(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    std::uint8_t* output, std::size_t output_capacity, std::size_t* output_size,
    char* error, std::size_t error_capacity) {
    if (output_size) *output_size = 0;
    if (error && error_capacity) error[0] = 0;
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path || !output ||
        !output_size) {
        set_error(error, error_capacity, "Missing original source layout input/output");
        return 0;
    }

    dh2::world::SourceLevel source{};
    dh2::world::Diagnostic diagnostic{};
    const auto status = dh2_world_import_level(&source, level_name, source_path,
        mlx, mlx_size, &diagnostic);
    if (status != dh2::world::Error::ok) {
        set_error(error, error_capacity, diagnostic.message);
        dh2_world_free(&source);
        return 0;
    }
    if (!source.module_count || source.module_count > 256) {
        set_error(error, error_capacity, "Source Module count is outside the runtime limit");
        dh2_world_free(&source);
        return 0;
    }

    // DWLD v1 (128-byte records) whenever every Module is untransformed and has MGP/MVP
    // children, so existing levels keep their bytes. Otherwise v2 (152-byte records)
    // adds rotation degrees (MLX order), scale and flags (1 = children, 2 = visual-only).
    bool needs_v2 = false;
    for (std::uint32_t i = 0; i < source.module_count; ++i) {
        const auto& module = source.modules[i];
        for (unsigned axis = 0; axis < 3; ++axis) {
            if (module.record.local.rotation_degrees[axis] != 0.0f ||
                module.record.local.scale[axis] != 1.0f)
                needs_v2 = true;
        }
        if (!module.cache_mgp) needs_v2 = true;
    }
    const std::size_t record_size = needs_v2 ? 152 : 128;
    const auto required = std::size_t(24) + std::size_t(source.module_count) * record_size;
    if (output_capacity < required) {
        set_error(error, error_capacity, "Source layout output buffer is too small");
        dh2_world_free(&source);
        return 0;
    }
    std::memset(output, 0, required);
    std::memcpy(output, "DWLD", 4);
    write_word(output + 4, needs_v2 ? 2u : 1u);
    write_word(output + 8, source.module_count);
    // The Level and Module records do not define an entrypoint. The runtime
    // owner fills these coordinates from its selected SpawnPoint source.
    for (std::uint32_t i = 0; i < source.module_count; ++i) {
        const auto& module = source.modules[i];
        if (!module.catalogue_node_id || !*module.catalogue_node_id) {
            set_error(error, error_capacity, "Source Module has no catalogue node ID");
            dh2_world_free(&source);
            return 0;
        }
        const auto length = std::strlen(module.catalogue_node_id);
        if (length >= 112) {
            set_error(error, error_capacity, "Source Module catalogue node ID is too long");
            dh2_world_free(&source);
            return 0;
        }
        auto* record = output + 24 + std::size_t(i) * record_size;
        std::memcpy(record, module.catalogue_node_id, length);
        for (unsigned axis = 0; axis < 3; ++axis) {
            if (!write_float(record + 112 + axis * 4, module.record.local.position[axis])) {
                set_error(error, error_capacity, "Source Module position is non-finite");
                dh2_world_free(&source);
                return 0;
            }
        }
        if (!needs_v2) {
            write_word(record + 124, 0);
            continue;
        }
        for (unsigned axis = 0; axis < 3; ++axis) {
            if (!write_float(record + 124 + axis * 4, module.record.local.rotation_degrees[axis]) ||
                !write_float(record + 136 + axis * 4, module.record.local.scale[axis])) {
                set_error(error, error_capacity, "Source Module rotation or scale is non-finite");
                dh2_world_free(&source);
                return 0;
            }
        }
        write_word(record + 148, module.cache_mgp ? 1u : 2u);
    }
    *output_size = required;
    dh2_world_free(&source);
    return 1;
}

extern "C" int dh2_world_source_module_refs(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    dh2_world_source_module_ref* output, std::size_t output_capacity,
    std::size_t* output_count, char* error, std::size_t error_capacity) {
    if (output_count) *output_count = 0;
    if (error && error_capacity) error[0] = 0;
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path ||
        !output_count || ((!output) != (output_capacity == 0))) {
        set_error(error, error_capacity, "Missing authored Module reference input/output");
        return 0;
    }

    SourceOwner owner;
    dh2::world::Diagnostic diagnostic{};
    const auto status = dh2_world_import_level(&owner.level, level_name,
        source_path, mlx, mlx_size, &diagnostic);
    if (status != dh2::world::Error::ok) {
        set_error(error, error_capacity, diagnostic.message);
        return 0;
    }
    const auto count = static_cast<std::size_t>(owner.level.module_count);
    if (!count || count > 256) {
        set_error(error, error_capacity,
            "Authored Module reference count is outside the runtime limit");
        return 0;
    }
    *output_count = count;
    if (!output) return 1;
    if (output_capacity < count) {
        *output_count = 0;
        set_error(error, error_capacity,
            "Authored Module reference output is too small");
        return 0;
    }

    std::vector<dh2_world_source_module_ref> result(count);
    for (std::size_t i = 0; i < count; ++i) {
        const auto& source = owner.level.modules[i];
        auto& target = result[i];
        target.module_index = static_cast<std::uint32_t>(i);
        if (!copy_source_text(target.name, sizeof(target.name),
                              source.record.name) ||
            !copy_source_text(target.mgp_path, sizeof(target.mgp_path),
                              source.cache_mgp) ||
            !copy_source_text(target.mvp_path, sizeof(target.mvp_path),
                              source.cache_mvp)) {
            *output_count = 0;
            set_error(error, error_capacity,
                "Authored Module identity or normalized cache path exceeds its bound");
            return 0;
        }
    }
    std::memcpy(output, result.data(), result.size() * sizeof(result[0]));
    return 1;
}

extern "C" int dh2_world_compile_static_spawnpoints(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, std::size_t mgp_count,
    std::uint8_t* output, std::size_t output_capacity, std::size_t* output_size,
    char* error, std::size_t error_capacity) {
    if (output_size) *output_size = 0;
    if (error && error_capacity) error[0] = 0;
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path ||
        !mgps || !output || !output_size) {
        set_error(error, error_capacity, "Missing original SpawnPoint source input/output");
        return 0;
    }

    dh2::world::SourceLevel source{};
    dh2::world::Diagnostic diagnostic{};
    auto status = dh2_world_import_level(&source, level_name, source_path,
        mlx, mlx_size, &diagnostic);
    if (status != dh2::world::Error::ok) {
        set_error(error, error_capacity, diagnostic.message);
        dh2_world_free(&source);
        return 0;
    }
    if (!source.module_count || source.module_count > 256 || mgp_count != source.module_count) {
        set_error(error, error_capacity, "Source MGP list must contain one file per MLX Module");
        dh2_world_free(&source);
        return 0;
    }

    // Import in source-module order. This keeps the original importer as the
    // authority for path validation, transforms, record parsing and provenance.
    for (std::uint32_t i = 0; i < source.module_count; ++i) {
        if (!source.modules[i].cache_mgp) continue;  // visual-only Module: no children
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            set_error(error, error_capacity, "Source MGP input is incomplete");
            dh2_world_free(&source);
            return 0;
        }
        status = dh2_world_import_module_objects(&source, i, dh2::world::RecordKind::mgp,
            mgps[i].source_path, mgps[i].data, mgps[i].size, &diagnostic);
        if (status != dh2::world::Error::ok) {
            set_error(error, error_capacity, diagnostic.message);
            dh2_world_free(&source);
            return 0;
        }
    }

    constexpr std::size_t header_size = 16;
    constexpr std::size_t record_size = 144;
    constexpr std::size_t entrypoint_count = 3;
    constexpr std::size_t required = header_size + entrypoint_count * record_size;
    if (output_capacity < required) {
        set_error(error, error_capacity, "SpawnPoint output buffer is too small");
        dh2_world_free(&source);
        return 0;
    }

    constexpr std::int32_t supported_ids[] = {0, 3, 13};
    bool found[entrypoint_count] = {};
    const dh2::world::Object* selected[entrypoint_count] = {};
    for (std::uint32_t i = 0; i < source.entity_count; ++i) {
        const auto& object = source.entities[i];
        if (std::strcmp(object.gametype, "SpawnPoint")) continue;

        std::int32_t id = 0;
        const auto* id_text = dh2_world_field(&object, "entrypointID");
        if (!parse_entrypoint_id(id_text, id)) {
            set_error(error, error_capacity, "SpawnPoint entrypointID is missing or malformed");
            dh2_world_free(&source);
            return 0;
        }
        int supported_index = -1;
        for (int n = 0; n < static_cast<int>(entrypoint_count); ++n)
            if (supported_ids[n] == id) supported_index = n;

        const bool is_unconditional = unconditional(object);
        if (supported_index >= 0) {
            if (found[supported_index]) {
                set_error(error, error_capacity, "Duplicate supported SpawnPoint entrypoint ID");
                dh2_world_free(&source);
                return 0;
            }
            if (!is_unconditional || !object.name || !*object.name ||
                std::strlen(object.name) >= 64) {
                set_error(error, error_capacity,
                    "Supported SpawnPoint must be uniquely named and unconditional");
                dh2_world_free(&source);
                return 0;
            }
            const auto* script = dh2_world_field(&object, "script");
            if (script && *script) {
                set_error(error, error_capacity, "Supported SpawnPoint script is not represented by SPWN v1");
                dh2_world_free(&source);
                return 0;
            }
            found[supported_index] = true;
            selected[supported_index] = &object;
            continue;
        }

        // SWAMP has condition-driven transition points with IDs 1 and 4.
        // The existing SPWN v1 owner cannot evaluate conditions, so defer
        // only those known conditional groups. Any new/unconditional record
        // is rejected instead of silently changing level transitions.
        if ((id == 1 || id == 4) && !is_unconditional) continue;
        set_error(error, error_capacity, "Unsupported unconditional or conditional SpawnPoint record");
        dh2_world_free(&source);
        return 0;
    }
    for (std::size_t i = 0; i < entrypoint_count; ++i) {
        if (!found[i] || !selected[i]) {
            set_error(error, error_capacity, "Required unconditional SpawnPoint ID is absent");
            dh2_world_free(&source);
            return 0;
        }
    }

    std::vector<std::uint8_t> serialized(required, 0);
    std::memcpy(serialized.data(), "SPWN", 4);
    write_word(serialized.data() + 4, 1);
    write_word(serialized.data() + 8, static_cast<std::uint32_t>(entrypoint_count));
    write_word(serialized.data() + 12, 0);
    for (std::size_t i = 0; i < entrypoint_count; ++i) {
        auto* record = serialized.data() + header_size + i * record_size;
        if (std::strlen(selected[i]->name) >= 64) {
            set_error(error, error_capacity, "SpawnPoint name exceeds SPWN v1 limit");
            dh2_world_free(&source);
            return 0;
        }
        if (!write_entrypoint(record, *selected[i])) {
            set_error(error, error_capacity, "SpawnPoint transform is non-finite");
            dh2_world_free(&source);
            return 0;
        }
        for (std::size_t offset = 72; offset < record_size; offset += 4) {
            // write_float rejects non-finite values; the source importer should
            // already enforce this, but keep the serialization boundary checked.
            std::uint32_t bits = 0;
            std::memcpy(&bits, record + offset, sizeof(bits));
            float value = 0;
            std::memcpy(&value, &bits, sizeof(value));
            if (!std::isfinite(value)) {
                set_error(error, error_capacity, "SpawnPoint transform is non-finite");
                dh2_world_free(&source);
                return 0;
            }
        }
    }
    std::memcpy(output, serialized.data(), required);
    *output_size = required;
    dh2_world_free(&source);
    return 1;
}

static int compile_static_dact_impl(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, std::size_t mgp_count,
    const dh2::data::CharacterTable* characters,
    const dh2::data::Dictionary* models,
    std::uint8_t* output, std::size_t output_capacity, std::size_t* output_size,
    dh2_world_source_static_character* source_rows, std::size_t source_capacity,
    std::size_t* source_count,
    char* error, std::size_t error_capacity) {
    if (output_size) *output_size = 0;
    if (source_count) *source_count = 0;
    if (error && error_capacity) error[0] = 0;
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path ||
        !mgps || !characters || !models || !output || !output_size) {
        set_error(error, error_capacity, "Missing original DACT source input/output");
        return 0;
    }

    constexpr std::size_t header_size = 16;
    constexpr std::size_t record_size = 256;
    constexpr std::size_t actor_count = expected_direct_actors.size();
    constexpr std::size_t required = header_size + actor_count * record_size;
    if (source_rows && source_capacity < actor_count) {
        set_error(error, error_capacity, "Source DACT identity output is too small");
        return 0;
    }
    if (source_rows && !source_count) {
        set_error(error, error_capacity, "Source DACT identity count output is missing");
        return 0;
    }
    if (output_capacity < required) {
        set_error(error, error_capacity, "Source DACT output buffer is too small");
        return 0;
    }

    SourceOwner owner;
    dh2::world::Diagnostic diagnostic{};
    auto status = dh2_world_import_level(&owner.level, level_name, source_path,
        mlx, mlx_size, &diagnostic);
    if (status != dh2::world::Error::ok) {
        set_error(error, error_capacity, diagnostic.message);
        return 0;
    }
    if (owner.level.module_count != 9 || mgp_count != owner.level.module_count) {
        set_error(error, error_capacity,
            "Source DACT currently requires the reviewed nine-module SWAMP MGP set");
        return 0;
    }
    for (std::uint32_t i = 0; i < owner.level.module_count; ++i) {
        if (!owner.level.modules[i].cache_mgp) continue;  // visual-only Module: no children
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            set_error(error, error_capacity, "Source DACT MGP input is incomplete");
            return 0;
        }
        status = dh2_world_import_module_objects(&owner.level, i,
            dh2::world::RecordKind::mgp, mgps[i].source_path, mgps[i].data,
            mgps[i].size, &diagnostic);
        if (status != dh2::world::Error::ok) {
            set_error(error, error_capacity, diagnostic.message);
            return 0;
        }
    }

    std::vector<std::uint8_t> serialized(required, 0);
    std::memcpy(serialized.data(), "DACT", 4);
    write_word(serialized.data() + 4, 1);
    write_word(serialized.data() + 8, static_cast<std::uint32_t>(actor_count));
    write_word(serialized.data() + 12, 0);

    std::size_t selected_count = 0;
    for (std::uint32_t i = 0; i < owner.level.entity_count; ++i) {
        const auto& object = owner.level.entities[i];
        if (!direct_monster_record(object)) continue;
        if (!direct_monster_candidate(object)) {
            set_error(error, error_capacity,
                "Eligible SWAMP Monster has unsupported spawn_prob (expected default or 100)");
            return 0;
        }
        if (selected_count >= actor_count) {
            set_error(error, error_capacity, "Unsupported eligible direct SWAMP Monster record");
            return 0;
        }
        const auto& expected = expected_direct_actors[selected_count];
        const auto* character = dh2_world_field(&object, "charpropsname");
        if (object.module_index != expected.module_index ||
            object.source_record != expected.source_record ||
            std::strcmp(object.name, expected.name) || !character ||
            std::strcmp(character, expected.character)) {
            set_error(error, error_capacity,
                "Eligible SWAMP Monster does not match the five supported source records");
            return 0;
        }
        const auto* script = dh2_world_field(&object, "script");
        if (script && *script) {
            set_error(error, error_capacity,
                "Supported direct SWAMP Monster has an unrepresented script");
            return 0;
        }
        if (object.module_index >= owner.level.module_count) {
            set_error(error, error_capacity, "Supported SWAMP Monster module index is invalid");
            return 0;
        }

        const std::string character_name(character);
        const auto* model_id = dh2::data::property(*characters, character_name, "ModelFile");
        const auto* scale_x = dh2::data::property(*characters, character_name, "Scale_X");
        const auto* scale_y = dh2::data::property(*characters, character_name, "Scale_Y");
        const auto* scale_z = dh2::data::property(*characters, character_name, "Scale_Z");
        if (!model_id || *model_id < 0 ||
            static_cast<std::size_t>(*model_id) >= models->values.size() ||
            !scale_x || !scale_y || !scale_z) {
            set_error(error, error_capacity,
                "CharacterTable ModelFile or authored scale property is unresolved");
            return 0;
        }
        const auto& model_path = models->values[static_cast<std::size_t>(*model_id)];
        const auto slash = model_path.find_last_of("/\\");
        const auto model_name = model_path.substr(slash == std::string::npos ? 0 : slash + 1);
        if (model_name.empty() || model_name == "." || model_name == ".." ||
            model_name.find("..") != std::string::npos ||
            model_name.find_first_of("/\\") != std::string::npos) {
            set_error(error, error_capacity, "CharacterTable ModelFile basename is invalid for DACT");
            return 0;
        }

        const std::int32_t scale_percentages[] = {*scale_x, *scale_y, *scale_z};
        float transform[9]{};
        for (unsigned axis = 0; axis < 3; ++axis) {
            transform[axis] = object.world_position[axis];
            transform[3 + axis] = object.local.rotation_degrees[axis];
            transform[6 + axis] = static_cast<float>(
                static_cast<double>(object.local.scale[axis]) * scale_percentages[axis] / 100.0);
        }
        for (unsigned axis = 0; axis < 3; ++axis) {
            if (!valid_dact_float(transform[axis], 10000000.f) ||
                !valid_dact_float(transform[3 + axis], 3600.f) ||
                !valid_dact_float(transform[6 + axis], 100.f) || transform[6 + axis] <= 0) {
                set_error(error, error_capacity,
                    "Supported SWAMP Monster placement exceeds DACT v1 limits");
                return 0;
            }
        }

        auto* record = serialized.data() + header_size + selected_count * record_size;
        write_word(record, 1);
        write_word(record + 4, object.module_index);
        if (!fixed_ascii(record + 8, 64, object.name) ||
            !fixed_ascii(record + 72, 64, character) ||
            !fixed_ascii(record + 136, 64, model_name.c_str())) {
            set_error(error, error_capacity, "Supported SWAMP Monster identity does not fit DACT v1");
            return 0;
        }
        if (source_rows) {
            auto& source = source_rows[selected_count];
            source = {};
            source.module_index = object.module_index;
            source.source_record = object.source_record;
            if (!fixed_ascii(reinterpret_cast<std::uint8_t*>(source.name),
                             sizeof(source.name), object.name) ||
                !fixed_ascii(reinterpret_cast<std::uint8_t*>(source.character),
                             sizeof(source.character), character)) {
                set_error(error, error_capacity,
                    "Source DACT provenance identity does not fit its bounded output");
                return 0;
            }
        }
        for (unsigned axis = 0; axis < 9; ++axis) {
            if (!write_float(record + 200 + axis * 4, transform[axis])) {
                set_error(error, error_capacity, "Supported SWAMP Monster transform is non-finite");
                return 0;
            }
        }
        ++selected_count;
    }
    if (selected_count != actor_count) {
        set_error(error, error_capacity,
            "Original SWAMP MGPs do not contain all five supported direct Monsters");
        return 0;
    }

    std::memcpy(output, serialized.data(), required);
    *output_size = required;
    if (source_count) *source_count = selected_count;
    return 1;
}

extern "C" int dh2_world_compile_static_dact(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, std::size_t mgp_count,
    const dh2::data::CharacterTable* characters,
    const dh2::data::Dictionary* models,
    std::uint8_t* output, std::size_t output_capacity, std::size_t* output_size,
    char* error, std::size_t error_capacity) {
    return compile_static_dact_impl(mlx, mlx_size, level_name, source_path,
        mgps, mgp_count, characters, models, output, output_capacity,
        output_size, nullptr, 0, nullptr, error, error_capacity);
}

extern "C" int dh2_world_compile_static_dact_with_sources(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, std::size_t mgp_count,
    const dh2::data::CharacterTable* characters,
    const dh2::data::Dictionary* models,
    std::uint8_t* output, std::size_t output_capacity, std::size_t* output_size,
    dh2_world_source_static_character* source_rows, std::size_t source_capacity,
    std::size_t* source_count, char* error, std::size_t error_capacity) {
    return compile_static_dact_impl(mlx, mlx_size, level_name, source_path,
        mgps, mgp_count, characters, models, output, output_capacity,
        output_size, source_rows, source_capacity, source_count,
        error, error_capacity);
}

extern "C" int dh2_world_census_source_characters(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, std::size_t mgp_count,
    dh2_world_source_character_row* output, std::size_t output_capacity,
    std::size_t* output_count, char* error, std::size_t error_capacity) {
    if (output_count) *output_count = 0;
    if (error && error_capacity) error[0] = 0;
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path ||
        !mgps || !mgp_count || mgp_count > 256 || !output_count ||
        ((!output) != (output_capacity == 0))) {
        set_error(error, error_capacity, "Missing source Character census input/output");
        return 0;
    }

    SourceOwner owner;
    dh2::world::Diagnostic diagnostic{};
    auto status = dh2_world_import_level(&owner.level, level_name, source_path,
        mlx, mlx_size, &diagnostic);
    if (status != dh2::world::Error::ok) {
        set_error(error, error_capacity, diagnostic.message);
        return 0;
    }
    if (owner.level.module_count != mgp_count) {
        set_error(error, error_capacity, "Source Character census requires one MGP per MLX Module");
        return 0;
    }
    for (std::uint32_t i = 0; i < owner.level.module_count; ++i) {
        if (!owner.level.modules[i].cache_mgp) continue;  // visual-only Module: no children
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            set_error(error, error_capacity, "Source Character census MGP input is incomplete");
            return 0;
        }
        status = dh2_world_import_module_objects(&owner.level, i,
            dh2::world::RecordKind::mgp, mgps[i].source_path, mgps[i].data,
            mgps[i].size, &diagnostic);
        if (status != dh2::world::Error::ok) {
            set_error(error, error_capacity, diagnostic.message);
            return 0;
        }
    }

    std::size_t count = 0;
    for (std::uint32_t i = 0; i < owner.level.entity_count; ++i) {
        const auto& object = owner.level.entities[i];
        if (object.kind != dh2::world::RecordKind::mgp ||
            std::strcmp(object.gametype, "Character")) continue;
        if (count >= 32768) {
            set_error(error, error_capacity, "Source Character census exceeds its bounded row count");
            return 0;
        }
        ++count;
    }
    if (!output) {
        *output_count = count;
        return 1;
    }
    if (output_capacity < count) {
        set_error(error, error_capacity, "Source Character census output is too small");
        return 0;
    }

    std::size_t selected_count = 0;
    for (std::uint32_t i = 0; i < owner.level.entity_count; ++i) {
        const auto& object = owner.level.entities[i];
        if (object.kind != dh2::world::RecordKind::mgp ||
            std::strcmp(object.gametype, "Character")) continue;
        dh2_world_source_character_row row{};
        row.module_index = object.module_index;
        row.source_record = object.source_record;
        const auto* character = dh2_world_field(&object, "charpropsname");
        const auto* template_name = dh2_world_field(&object, "_templateName");
        const auto* char_template_pydata = dh2_world_field(&object, "char_template_pydata");
        const auto* char_template = dh2_world_field(&object, "char_template");
        const auto* activate = dh2_world_field(&object, "activate_cond");
        const auto* deactivate = dh2_world_field(&object, "deactivate_cond");
        const auto* script = dh2_world_field(&object, "script");
        const auto* spawn_probability = dh2_world_field(&object, "spawn_prob");
        const auto* auto_spawn = dh2_world_field(&object, "auto_spawn");
        const auto* ai_state = dh2_world_field(&object, "ai_state");
        const auto* char_group = dh2_world_field(&object, "char_group");
        const auto* char_group_role = dh2_world_field(&object, "char_group_role");
        const auto* spawn_view_radius = dh2_world_field(&object, "spawn_view_radius");
        const auto* is_global = dh2_world_field(&object, "isGlobal");
        if (!copy_source_text(row.name, sizeof(row.name), object.name) ||
            !copy_source_text(row.character, sizeof(row.character), character) ||
            !copy_source_text(row.template_name, sizeof(row.template_name), template_name) ||
            !copy_source_text(row.char_template_pydata, sizeof(row.char_template_pydata), char_template_pydata) ||
            !copy_source_text(row.char_template, sizeof(row.char_template), char_template) ||
            !copy_source_text(row.activate_condition, sizeof(row.activate_condition), activate) ||
            !copy_source_text(row.deactivate_condition, sizeof(row.deactivate_condition), deactivate) ||
            !copy_source_text(row.script, sizeof(row.script), script) ||
            !copy_source_text(row.spawn_probability, sizeof(row.spawn_probability), spawn_probability) ||
            !copy_source_text(row.auto_spawn, sizeof(row.auto_spawn), auto_spawn) ||
            !copy_source_text(row.ai_state, sizeof(row.ai_state), ai_state) ||
            !copy_source_text(row.char_group, sizeof(row.char_group), char_group) ||
            !copy_source_text(row.char_group_role, sizeof(row.char_group_role), char_group_role) ||
            !copy_source_text(row.spawn_view_radius, sizeof(row.spawn_view_radius), spawn_view_radius)) {
            set_error(error, error_capacity, "Source Character identity/activation field exceeds census bounds");
            return 0;
        }
        if (!parse_source_global(is_global, row.is_global)) {
            set_error(error, error_capacity, "Source Character isGlobal value is not numeric 0 or 1");
            return 0;
        }

        std::uint32_t reasons = DH2_SOURCE_CHARACTER_DEFER_NONE;
        if (!condition_absent(activate)) reasons |= DH2_SOURCE_CHARACTER_DEFER_ACTIVATE_CONDITION;
        if (!condition_absent(deactivate)) reasons |= DH2_SOURCE_CHARACTER_DEFER_DEACTIVATE_CONDITION;
        if (script && *script) reasons |= DH2_SOURCE_CHARACTER_DEFER_SCRIPT;
        if (auto_spawn && !std::strcmp(auto_spawn, "0"))
            reasons |= DH2_SOURCE_CHARACTER_DEFER_AUTOSPAWN_DISABLED;
        if (ai_state && *ai_state && std::strcmp(ai_state, "Idle"))
            reasons |= DH2_SOURCE_CHARACTER_DEFER_NON_IDLE_AI;
        if (!template_name || std::strcmp(template_name, "Monster"))
            reasons |= DH2_SOURCE_CHARACTER_DEFER_UNSUPPORTED_TEMPLATE;
        if (direct_monster_record(object) && !direct_monster_candidate(object))
            reasons |= DH2_SOURCE_CHARACTER_DEFER_SPAWN_PROBABILITY;

        bool supported = false;
        if (direct_monster_candidate(object) && !(reasons & DH2_SOURCE_CHARACTER_DEFER_SCRIPT)) {
            for (const auto& expected : expected_direct_actors) {
                if (object.module_index == expected.module_index &&
                    object.source_record == expected.source_record &&
                    !std::strcmp(object.name, expected.name) && character &&
                    !std::strcmp(character, expected.character)) {
                    supported = true;
                    break;
                }
            }
        }
        if (!supported) reasons |= DH2_SOURCE_CHARACTER_DEFER_UNSUPPORTED_DIRECT_ROW;
        row.supported_direct_dact_v1 = supported ? 1u : 0u;
        row.deferred_reason_mask = supported
            ? static_cast<std::uint32_t>(DH2_SOURCE_CHARACTER_DEFER_NONE) : reasons;

        if (object.module_index >= owner.level.module_count) {
            set_error(error, error_capacity, "Source Character MGP module index is invalid");
            return 0;
        }
        for (unsigned axis = 0; axis < 3; ++axis) {
            row.local_transform[axis] = object.local.position[axis];
            row.local_transform[3 + axis] = object.local.rotation_degrees[axis];
            row.local_transform[6 + axis] = object.local.scale[axis];
            row.world_transform[axis] = object.world_position[axis];
            row.world_transform[3 + axis] = object.local.rotation_degrees[axis];
            row.world_transform[6 + axis] = object.local.scale[axis];
            if (!std::isfinite(row.local_transform[axis]) ||
                !std::isfinite(row.local_transform[3 + axis]) ||
                !std::isfinite(row.local_transform[6 + axis]) ||
                !std::isfinite(row.world_transform[axis]) ||
                !std::isfinite(row.world_transform[3 + axis]) ||
                !std::isfinite(row.world_transform[6 + axis])) {
                set_error(error, error_capacity, "Source Character transform is non-finite");
                return 0;
            }
        }
        output[selected_count++] = row;
    }
    *output_count = selected_count;
    return 1;
}

extern "C" int dh2_world_census_source_objects(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, std::size_t mgp_count,
    const char* gametype_filter, dh2_world_source_object_row* output,
    std::size_t output_capacity, std::size_t* output_count,
    char* error, std::size_t error_capacity) {
    if (output_count) *output_count = 0;
    if (error && error_capacity) error[0] = 0;
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path ||
        !*source_path || !mgps || !mgp_count || mgp_count > 256 ||
        !output_count || ((!output) != (output_capacity == 0))) {
        set_error(error, error_capacity, "Missing generic source MGP census input/output");
        return 0;
    }

    SourceOwner owner;
    dh2::world::Diagnostic diagnostic{};
    auto status = dh2_world_import_level(&owner.level, level_name, source_path,
        mlx, mlx_size, &diagnostic);
    if (status != dh2::world::Error::ok) {
        set_error(error, error_capacity, diagnostic.message);
        return 0;
    }
    if (owner.level.module_count != mgp_count) {
        set_error(error, error_capacity, "Generic source MGP census requires one file per MLX Module");
        return 0;
    }
    for (std::uint32_t i = 0; i < owner.level.module_count; ++i) {
        if (!owner.level.modules[i].cache_mgp) continue;  // visual-only Module: no children
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            set_error(error, error_capacity, "Generic source MGP census input is incomplete");
            return 0;
        }
        status = dh2_world_import_module_objects(&owner.level, i,
            dh2::world::RecordKind::mgp, mgps[i].source_path, mgps[i].data,
            mgps[i].size, &diagnostic);
        if (status != dh2::world::Error::ok) {
            set_error(error, error_capacity, diagnostic.message);
            return 0;
        }
    }

    const auto matches = [gametype_filter](const dh2::world::Object& object) {
        return object.kind == dh2::world::RecordKind::mgp &&
            (!gametype_filter || !*gametype_filter ||
             !std::strcmp(object.gametype, gametype_filter));
    };
    std::size_t count = 0;
    for (std::uint32_t i = 0; i < owner.level.entity_count; ++i) {
        if (!matches(owner.level.entities[i])) continue;
        if (count >= 32768) {
            set_error(error, error_capacity, "Generic source MGP census exceeds its bounded row count");
            return 0;
        }
        ++count;
    }
    if (!output) {
        *output_count = count;
        return 1;
    }
    if (output_capacity < count) {
        set_error(error, error_capacity, "Generic source MGP census output is too small");
        return 0;
    }

    std::size_t selected_count = 0;
    for (std::uint32_t i = 0; i < owner.level.entity_count; ++i) {
        const auto& object = owner.level.entities[i];
        if (!matches(object)) continue;
        if (object.module_index >= owner.level.module_count) {
            set_error(error, error_capacity, "Generic source MGP module index is invalid");
            return 0;
        }
        dh2_world_source_object_row row{};
        row.module_index = object.module_index;
        if (object.module_index > static_cast<std::uint32_t>(
                std::numeric_limits<std::int32_t>::max())) {
            set_error(error, error_capacity,
                      "Generic source MGP Module ID exceeds the runtime signed field");
            return 0;
        }
        row.source_module_id = static_cast<std::int32_t>(object.module_index);
        row.source_record = object.source_record;
        const auto* data_desc_pydata = dh2_world_field(&object, "data_desc_pydata");
        // DestructibleContainer::GetDataId (0x3a11c0) dispatches directly to
        // Arrays::GetMemberIDByString<Arrays::DestructibleContainers> using
        // the authored data_desc. Its MGP rows therefore have no explicit
        // data_desc_pydata member; retain the table selected by that source
        // class virtual instead of treating the absent XML field as unknown.
        if ((!data_desc_pydata || !*data_desc_pydata) && object.gametype &&
            !std::strcmp(object.gametype, "DestructibleContainer"))
            data_desc_pydata = "DestructibleContainers";
        if (!copy_source_text(row.name, sizeof(row.name), object.name) ||
            !copy_source_text(row.gametype, sizeof(row.gametype), object.gametype) ||
            !copy_source_text(row.template_name, sizeof(row.template_name),
                              dh2_world_field(&object, "_templateName")) ||
            !copy_source_text(row.activate_condition, sizeof(row.activate_condition),
                              dh2_world_field(&object, "activate_cond")) ||
            !copy_source_text(row.deactivate_condition, sizeof(row.deactivate_condition),
                              dh2_world_field(&object, "deactivate_cond")) ||
            !copy_source_text(row.spawn_probability, sizeof(row.spawn_probability),
                              dh2_world_field(&object, "spawn_prob")) ||
            !copy_source_text(row.min_difficulty, sizeof(row.min_difficulty),
                              dh2_world_field(&object, "min_difficulty")) ||
            !copy_source_text(row.is_demo_only, sizeof(row.is_demo_only),
                              dh2_world_field(&object, "is_demo_only")) ||
            !copy_source_text(row.dimensions, sizeof(row.dimensions),
                              dh2_world_field(&object, "dimensions")) ||
            !copy_source_text(row.data_desc, sizeof(row.data_desc),
                              dh2_world_field(&object, "data_desc")) ||
            !copy_source_text(row.door_data, sizeof(row.door_data),
                              dh2_world_field(&object, "data")) ||
            !copy_source_text(row.door_opened, sizeof(row.door_opened),
                              dh2_world_field(&object, "opened")) ||
            !copy_source_text(row.door_collision, sizeof(row.door_collision),
                              dh2_world_field(&object, "door_collision")) ||
            !copy_source_text(row.data_desc_pydata, sizeof(row.data_desc_pydata),
                              data_desc_pydata) ||
            !copy_source_text(row.level_id, sizeof(row.level_id),
                              dh2_world_field(&object, "levelID")) ||
            !copy_source_text(row.level_name, sizeof(row.level_name),
                              dh2_world_field(&object, "levelName")) ||
            !copy_source_text(row.entrypoint_id, sizeof(row.entrypoint_id),
                              dh2_world_field(&object, "entrypointID")) ||
            !copy_source_text(row.fasttravel, sizeof(row.fasttravel),
                              dh2_world_field(&object, "fasttravel")) ||
            !copy_source_text(row.skip_checkpoint, sizeof(row.skip_checkpoint),
                              dh2_world_field(&object, "skip_checkpoint")) ||
            !copy_source_text(row.visible, sizeof(row.visible),
                              dh2_world_field(&object, "visible")) ||
            !copy_source_text(row.target_node, sizeof(row.target_node),
                              dh2_world_field(&object, "target_node"))) {
            set_error(error, error_capacity, "Generic source MGP identity/condition field exceeds census bounds");
            return 0;
        }
        for (unsigned axis = 0; axis < 3; ++axis) {
            row.local_transform[axis] = object.local.position[axis];
            row.local_transform[3 + axis] = object.local.rotation_degrees[axis];
            row.local_transform[6 + axis] = object.local.scale[axis];
            row.world_transform[axis] = object.world_position[axis];
            row.world_transform[3 + axis] = object.local.rotation_degrees[axis];
            row.world_transform[6 + axis] = object.local.scale[axis];
            if (!std::isfinite(row.local_transform[axis]) ||
                !std::isfinite(row.local_transform[3 + axis]) ||
                !std::isfinite(row.local_transform[6 + axis]) ||
                !std::isfinite(row.world_transform[axis]) ||
                !std::isfinite(row.world_transform[3 + axis]) ||
                !std::isfinite(row.world_transform[6 + axis])) {
                set_error(error, error_capacity, "Generic source MGP transform is non-finite");
                return 0;
            }
        }
        output[selected_count++] = row;
    }
    *output_count = selected_count;
    return 1;
}

extern "C" int dh2_world_compile_static_mvp(const std::uint8_t* mlx,
    std::size_t mlx_size, const char* level_name, const char* source_path,
    const dh2_world_source_mvp* mvps, std::size_t mvp_count,
    dh2_world_source_static_decor* output, std::size_t output_capacity,
    std::size_t* output_count, char* error, std::size_t error_capacity) {
    if (output_count) *output_count = 0;
    if (error && error_capacity) error[0] = 0;
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path ||
        !mvps || !output || !output_count) {
        set_error(error, error_capacity, "Missing original source MVP input/output");
        return 0;
    }
    constexpr std::size_t decor_count = expected_static_decors.size();
    if (output_capacity < decor_count) {
        set_error(error, error_capacity, "Source MVP decor output is too small");
        return 0;
    }

    SourceOwner owner;
    dh2::world::Diagnostic diagnostic{};
    auto status = dh2_world_import_level(&owner.level, level_name, source_path,
        mlx, mlx_size, &diagnostic);
    if (status != dh2::world::Error::ok) {
        set_error(error, error_capacity, diagnostic.message);
        return 0;
    }
    if (owner.level.module_count != 9 || mvp_count != owner.level.module_count) {
        set_error(error, error_capacity,
            "Source static Decor currently requires the reviewed nine-module SWAMP MVP set");
        return 0;
    }
    for (std::uint32_t i = 0; i < owner.level.module_count; ++i) {
        if (!owner.level.modules[i].cache_mgp) continue;  // visual-only Module: no children
        if (!mvps[i].source_path || !mvps[i].data || !mvps[i].size) {
            set_error(error, error_capacity, "Source MVP input is incomplete");
            return 0;
        }
        status = dh2_world_import_module_objects(&owner.level, i,
            dh2::world::RecordKind::mvp, mvps[i].source_path, mvps[i].data,
            mvps[i].size, &diagnostic);
        if (status != dh2::world::Error::ok) {
            set_error(error, error_capacity, diagnostic.message);
            return 0;
        }
    }

    std::array<dh2_world_source_static_decor, decor_count> rows{};
    std::size_t selected_count = 0;
    for (std::uint32_t i = 0; i < owner.level.entity_count; ++i) {
        const auto& object = owner.level.entities[i];
        if (object.kind != dh2::world::RecordKind::mvp ||
            std::strcmp(object.gametype, "Decor"))
            continue;
        if (static_decor_has_condition_or_script(object)) {
            set_error(error, error_capacity,
                "SWAMP static Decor has an unsupported condition or script");
            return 0;
        }
        if (selected_count >= decor_count) {
            set_error(error, error_capacity, "Unexpected extra SWAMP static Decor record");
            return 0;
        }

        const auto& expected = expected_static_decors[selected_count];
        const auto* root = dh2_world_field(&object, "xrefobject");
        const auto* dae = dh2_world_field(&object, "dae");
        const auto* type = dh2_world_field(&object, "type");
        const auto* template_name = dh2_world_field(&object, "_templateName");
        char canonical_dae[256]{};
        if (!dae || dh2_world_cache_path(canonical_dae, sizeof(canonical_dae),
                dae, &diagnostic) != dh2::world::Error::ok) {
            set_error(error, error_capacity, "SWAMP static Decor DAE path is missing or invalid");
            return 0;
        }
        if (object.module_index != expected.module_index ||
            object.source_record != expected.source_record || !object.name ||
            std::strcmp(object.name, expected.name) || !root ||
            std::strcmp(root, expected.root) || std::strcmp(canonical_dae, swamp_prop_dae) ||
            !object.source_path || std::strcmp(object.source_path, expected.source_path) ||
            !type || std::strcmp(type, "Block") || !template_name ||
            std::strcmp(template_name, "Props")) {
            set_error(error, error_capacity,
                "SWAMP static Decor does not match the exact ten-row source allowlist/root set");
            return 0;
        }

        auto& row = rows[selected_count];
        row.module_index = object.module_index;
        row.source_record = object.source_record;
        if (!fixed_ascii(reinterpret_cast<std::uint8_t*>(row.name), sizeof(row.name), object.name) ||
            !fixed_ascii(reinterpret_cast<std::uint8_t*>(row.xrefobject),
                sizeof(row.xrefobject), root) ||
            !fixed_ascii(reinterpret_cast<std::uint8_t*>(row.dae_path),
                sizeof(row.dae_path), canonical_dae) ||
            !fixed_ascii(reinterpret_cast<std::uint8_t*>(row.source_path),
                sizeof(row.source_path), object.source_path)) {
            set_error(error, error_capacity, "SWAMP static Decor identity exceeds adapter limits");
            return 0;
        }
        for (unsigned axis = 0; axis < 3; ++axis) {
            row.local_transform[axis] = object.local.position[axis];
            row.local_transform[3 + axis] = object.local.rotation_degrees[axis];
            row.local_transform[6 + axis] = object.local.scale[axis];
            row.world_transform[axis] = object.world_position[axis];
            row.world_transform[3 + axis] = object.local.rotation_degrees[axis];
            row.world_transform[6 + axis] = object.local.scale[axis];
            if (!std::isfinite(row.local_transform[axis]) ||
                !std::isfinite(row.local_transform[3 + axis]) ||
                !std::isfinite(row.local_transform[6 + axis]) ||
                !std::isfinite(row.world_transform[axis]) ||
                !std::isfinite(row.world_transform[3 + axis]) ||
                !std::isfinite(row.world_transform[6 + axis])) {
                set_error(error, error_capacity, "SWAMP static Decor transform is non-finite");
                return 0;
            }
        }
        ++selected_count;
    }
    if (selected_count != decor_count) {
        set_error(error, error_capacity,
            "Original SWAMP MVPs do not contain all ten supported static Decor records");
        return 0;
    }

    std::memcpy(output, rows.data(), sizeof(rows));
    *output_count = decor_count;
    return 1;
}
