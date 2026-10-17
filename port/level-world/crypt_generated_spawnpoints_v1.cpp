#include "crypt_generated_spawnpoints_v1.hpp"

#include "../world-data/world.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

namespace dh2::world {
namespace {

constexpr std::size_t header_size = 16;
constexpr std::size_t record_size = 144;
constexpr std::uint32_t max_spawnpoints = 512;
constexpr float max_component = 10000000.0f;

struct SourceOwner {
    SourceLevel value{};
    ~SourceOwner() { dh2_world_free(&value); }
};

struct SpawnRecord {
    std::int32_t id = 0;
    std::uint32_t module = 0;
    std::string name;
    float transforms[18]{};
};

void set_error(std::string& output, const char* message) {
    output = message ? message : "Generated SpawnPoint compile failed";
}

bool parse_id(const char* text, std::int32_t& result) {
    if (!text || !*text) return false;
    bool negative = false;
    if (*text == '+' || *text == '-') {
        negative = *text == '-';
        ++text;
    }
    if (!*text) return false;

    const std::uint64_t limit = negative ? 2147483648ULL : 2147483647ULL;
    std::uint64_t value = 0;
    for (const auto* p = reinterpret_cast<const unsigned char*>(text); *p; ++p) {
        if (*p < '0' || *p > '9') return false;
        const auto digit = static_cast<std::uint64_t>(*p - '0');
        if (value > (limit - digit) / 10) return false;
        value = value * 10 + digit;
    }
    if (negative) {
        result = value == 2147483648ULL
            ? std::numeric_limits<std::int32_t>::min()
            : -static_cast<std::int32_t>(value);
    } else {
        result = static_cast<std::int32_t>(value);
    }
    return true;
}

bool nonempty(const char* text) {
    if (!text) return false;
    for (; *text; ++text)
        if (*text != ' ' && *text != '\t' && *text != '\r' && *text != '\n')
            return true;
    return false;
}

bool valid_component(float value) {
    return std::isfinite(value) && std::abs(value) <= max_component;
}

void write_u32(std::uint8_t* output, std::uint32_t value) {
    output[0] = static_cast<std::uint8_t>(value);
    output[1] = static_cast<std::uint8_t>(value >> 8);
    output[2] = static_cast<std::uint8_t>(value >> 16);
    output[3] = static_cast<std::uint8_t>(value >> 24);
}

void write_i32(std::uint8_t* output, std::int32_t value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    write_u32(output, bits);
}

void write_float(std::uint8_t* output, float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    write_u32(output, bits);
}

bool append_xml_attribute(std::string& output, const char* name, const char* value) {
    if (!name || !value) return false;
    output.push_back(' ');
    output += name;
    output += "=\"";
    for (const auto* p = reinterpret_cast<const unsigned char*>(value); *p; ++p) {
        switch (*p) {
        case '&': output += "&amp;"; break;
        case '<': output += "&lt;"; break;
        case '>': output += "&gt;"; break;
        case '"': output += "&quot;"; break;
        default:
            if (*p < 0x20 && *p != '\t' && *p != '\r' && *p != '\n')
                return false;
            output.push_back(static_cast<char>(*p));
        }
    }
    output.push_back('"');
    return true;
}

bool append_identity_level(const SourceLevel& original, std::string& xml) {
    xml = "<Level><GameObject";
    if (!append_xml_attribute(xml, "type", "level") ||
        !append_xml_attribute(xml, "name", "level_config") ||
        !append_xml_attribute(xml, "gametype", "LevelConfig") ||
        !append_xml_attribute(xml, "position", "0,0,0") ||
        !append_xml_attribute(xml, "rotation", "0,0,0") ||
        !append_xml_attribute(xml, "scale", "1,1,1")) return false;
    xml += "/>";

    for (std::uint32_t i = 0; i < original.module_count; ++i) {
        const auto& module = original.modules[i].record;
        const auto* type = dh2_world_field(&module, "type");
        const auto* dae = dh2_world_field(&module, "dae");
        const auto* mgp = dh2_world_field(&module, "mgp");
        const auto* mvp = dh2_world_field(&module, "mvp");
        const auto* xref = dh2_world_field(&module, "xrefobject");
        if (!module.name || !type || !dae || !mgp || !mvp || !xref) return false;
        xml += "<GameObject";
        if (!append_xml_attribute(xml, "type", type) ||
            !append_xml_attribute(xml, "name", module.name) ||
            !append_xml_attribute(xml, "gametype", "Module") ||
            !append_xml_attribute(xml, "position", "0,0,0") ||
            !append_xml_attribute(xml, "rotation", "0,0,0") ||
            !append_xml_attribute(xml, "scale", "1,1,1") ||
            !append_xml_attribute(xml, "dae", dae) ||
            !append_xml_attribute(xml, "mgp", mgp) ||
            !append_xml_attribute(xml, "mvp", mvp) ||
            !append_xml_attribute(xml, "xrefobject", xref)) return false;
        xml += "/>";
    }
    xml += "</Level>";
    return true;
}

bool build_records(const SourceLevel& placements, const SourceLevel& objects,
                   const std::int32_t* selected_entrypoint_id,
                   std::vector<SpawnRecord>& records, std::string& error) {
    for (std::uint32_t i = 0; i < objects.entity_count; ++i) {
        const auto& object = objects.entities[i];
        if (!object.gametype || std::strcmp(object.gametype, "SpawnPoint")) continue;
        if (object.module_index >= placements.module_count) {
            set_error(error, "SpawnPoint has no corresponding generated Module");
            return false;
        }
        SpawnRecord record;
        if (!parse_id(dh2_world_field(&object, "entrypointID"), record.id)) {
            set_error(error, "SpawnPoint entrypointID is missing or malformed");
            return false;
        }
        if (selected_entrypoint_id && record.id != *selected_entrypoint_id) {
            continue;
        }
        if (nonempty(dh2_world_field(&object, "activate_cond")) ||
            nonempty(dh2_world_field(&object, "deactivate_cond"))) {
            set_error(error, "SPWN v1 cannot preserve SpawnPoint activation conditions");
            return false;
        }
        if (nonempty(dh2_world_field(&object, "script"))) {
            set_error(error, "SPWN v1 cannot preserve SpawnPoint scripts");
            return false;
        }
        if (!object.name || !*object.name || std::strlen(object.name) >= 64) {
            set_error(error, "SpawnPoint name does not fit SPWN v1");
            return false;
        }
        record.name = object.name;
        record.module = object.module_index;

        const auto& module = placements.modules[record.module].record;
        for (unsigned axis = 0; axis < 3; ++axis) {
            const float module_origin = module.local.position[axis];
            const float local_position = object.local.position[axis];
            const float world_position = local_position + module_origin;
            if (!valid_component(local_position) || !valid_component(module_origin) ||
                !valid_component(world_position) ||
                !valid_component(object.local.rotation_degrees[axis]) ||
                !valid_component(object.local.scale[axis])) {
                set_error(error, "SpawnPoint transform exceeds SPWN v1 limits");
                return false;
            }
            record.transforms[axis] = local_position;
            record.transforms[3 + axis] = object.local.rotation_degrees[axis];
            record.transforms[6 + axis] = object.local.scale[axis];
            record.transforms[9 + axis] = world_position;
            // ObjectManager::LoadFromXML adds the module origin to XYZ only;
            // SpawnPoint::PlaceObject consumes the resulting point rotation.
            record.transforms[12 + axis] = object.local.rotation_degrees[axis];
            record.transforms[15 + axis] = object.local.scale[axis];
        }
        const auto prior = std::find_if(records.begin(), records.end(),
            [&](const SpawnRecord& value) { return value.id == record.id; });
        if (prior != records.end()) {
            if (!selected_entrypoint_id) {
                set_error(error, "SPWN v1 runtime reader rejects duplicate entrypoint IDs");
                return false;
            }
            // Level::_LoadPlayer applies every matching point in source order.
            // With unsupported scripts/conditions rejected above, the final
            // player transform is the last matching source record.
            *prior = std::move(record);
        } else {
            records.push_back(std::move(record));
        }
        if (records.size() > max_spawnpoints) {
            set_error(error, "SpawnPoint count exceeds the SPWN v1 runtime limit");
            return false;
        }
    }
    if (records.empty()) {
        set_error(error, "Generated level contains no authored SpawnPoints");
        return false;
    }
    return true;
}

} // namespace

static bool compile_generated_spawnpoints_impl_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedMgpView* mgps, std::size_t mgp_count,
    const std::int32_t* selected_entrypoint_id,
    std::vector<std::uint8_t>& output, std::string& error) {
    output.clear();
    error.clear();
    if (!level_xml || !level_size || !level_name || !*level_name ||
        !level_source_path || !mgps) {
        set_error(error, "Missing generated Level/MGP input or output");
        return false;
    }

    SourceOwner placements;
    Diagnostic diagnostic{};
    const auto status = dh2_world_import_level(&placements.value, level_name,
        level_source_path, level_xml, level_size, &diagnostic);
    if (status != Error::ok) {
        set_error(error, diagnostic.message);
        return false;
    }
    if (!placements.value.module_count || placements.value.module_count > 256 ||
        mgp_count != placements.value.module_count) {
        set_error(error, "Generated level needs one supplied MGP per source Module");
        return false;
    }

    // The shared source importer intentionally rejects transformed Module
    // placements. Import into an identity proxy to retain its MGP parser,
    // cache-path checks, fields and provenance; apply the native Level
    // translation afterwards from the original Module records. IDA shows the
    // original ObjectManager adds the Module origin to MGP XYZ only.
    std::string proxy_xml;
    if (!append_identity_level(placements.value, proxy_xml)) {
        set_error(error, "Generated Module metadata cannot form the MGP import proxy");
        return false;
    }
    SourceOwner objects;
    const auto proxy_status = dh2_world_import_level(&objects.value, level_name,
        level_source_path, reinterpret_cast<const std::uint8_t*>(proxy_xml.data()),
        proxy_xml.size(), &diagnostic);
    if (proxy_status != Error::ok) {
        set_error(error, diagnostic.message);
        return false;
    }

    for (std::uint32_t i = 0; i < placements.value.module_count; ++i) {
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            set_error(error, "Generated MGP input is incomplete");
            return false;
        }
        const auto import_status = dh2_world_import_module_objects(&objects.value, i,
            RecordKind::mgp, mgps[i].source_path, mgps[i].data, mgps[i].size,
            &diagnostic);
        if (import_status != Error::ok) {
            set_error(error, diagnostic.message);
            return false;
        }
    }

    std::vector<SpawnRecord> records;
    if (!build_records(placements.value, objects.value, selected_entrypoint_id,
                       records, error)) return false;
    std::vector<std::uint8_t> serialized(header_size + records.size() * record_size, 0);
    std::memcpy(serialized.data(), "SPWN", 4);
    write_u32(serialized.data() + 4, 1);
    write_u32(serialized.data() + 8, static_cast<std::uint32_t>(records.size()));
    write_u32(serialized.data() + 12, 0);
    for (std::size_t i = 0; i < records.size(); ++i) {
        auto* row = serialized.data() + header_size + i * record_size;
        write_i32(row, records[i].id);
        write_u32(row + 4, records[i].module);
        std::memcpy(row + 8, records[i].name.data(), records[i].name.size());
        for (unsigned j = 0; j < 18; ++j)
            write_float(row + 72 + j * 4, records[i].transforms[j]);
    }
    output = std::move(serialized);
    return true;
}

bool compile_generated_spawnpoints_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedMgpView* mgps, std::size_t mgp_count,
    std::vector<std::uint8_t>& output, std::string& error) {
    return compile_generated_spawnpoints_impl_v1(
        level_xml, level_size, level_name, level_source_path, mgps, mgp_count,
        nullptr, output, error);
}

bool compile_generated_spawnpoint_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedMgpView* mgps, std::size_t mgp_count,
    std::int32_t selected_entrypoint_id,
    std::vector<std::uint8_t>& output, std::string& error) {
    return compile_generated_spawnpoints_impl_v1(
        level_xml, level_size, level_name, level_source_path, mgps, mgp_count,
        &selected_entrypoint_id, output, error);
}

} // namespace dh2::world
