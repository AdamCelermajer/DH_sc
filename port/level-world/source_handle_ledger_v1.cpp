#include "source_handle_ledger_v1.hpp"

#include "../world-data/world.hpp"

#include <climits>
#include <cstdio>
#include <map>
#include <new>

namespace dh2::world::source_handle_ledger_v1 {
namespace {

constexpr std::size_t max_module_files = 512;
constexpr std::size_t max_source_objects = 32768;

struct SourceOwner {
    SourceLevel value{};
    ~SourceOwner() { dh2_world_free(&value); }
};

void set_error(std::string& error, const char* message) {
    error = message ? message : "Source handle ledger failed";
}

bool source_kind(RecordKind kind) {
    return kind == RecordKind::mgp || kind == RecordKind::mvp;
}

RecordKind source_kind(ModuleFileKind kind) {
    return kind == ModuleFileKind::mgp ? RecordKind::mgp : RecordKind::mvp;
}

Provenance provenance(const SourceLevel& source, const Object& object) {
    const auto module_name = object.module_index == no_module
        ? std::string{} : std::string(source.modules[object.module_index].record.name);
    return {object.module_index, module_name, object.source_record,
            object.kind, object.name, object.gametype};
}

enum class RegisterStatus { ok, invalid_object, handle_overflow };

RegisterStatus register_object(
    const SourceLevel& source, const Object& object, Ledger& candidate,
    std::map<std::string, std::size_t, std::less<>>& handle_by_name,
    std::string& error) {
    if (!object.name || !*object.name || !object.gametype || !*object.gametype ||
        (object.module_index != no_module &&
         object.module_index >= source.module_count)) {
        set_error(error, "Imported object is missing source identity or provenance");
        return RegisterStatus::invalid_object;
    }

    auto found = handle_by_name.find(object.name);
    if (found == handle_by_name.end()) {
        if (candidate.next_handle_after > INT32_MAX) {
            set_error(error, "ObjectManager source handle counter overflow");
            return RegisterStatus::handle_overflow;
        }
        const auto key = static_cast<std::int32_t>(candidate.next_handle_after++);
        const auto source_record = provenance(source, object);
        const auto entry_index = candidate.entries.size();
        handle_by_name.emplace(source_record.name, entry_index);
        candidate.entries.push_back({key, 1, source_record});
        candidate.occurrences.push_back({key, false, source_record});
    } else {
        auto& entry = candidate.entries[found->second];
        auto source_record = provenance(source, object);
        ++entry.lookup_count;
        candidate.occurrences.push_back({entry.handle, true,
                                         std::move(source_record)});
    }
    return RegisterStatus::ok;
}

}  // namespace

Status build(const std::uint8_t* level_xml, std::size_t level_size,
             const char* level_name, const char* level_source_path,
             const OrderedModuleFile* ordered_files, std::size_t file_count,
             std::uint32_t first_source_handle,
             std::uint32_t initial_top_level_handle_offset,
             Ledger& output, std::string& error) {
    output = {};
    error.clear();
    if (!level_xml || !level_size || !level_name || !*level_name ||
        !level_source_path || !ordered_files || !file_count) {
        set_error(error, "Source handle ledger input is incomplete");
        return Status::invalid_argument;
    }
    if (file_count > max_module_files || first_source_handle > INT32_MAX ||
        initial_top_level_handle_offset > static_cast<std::uint32_t>(INT32_MAX)) {
        set_error(error, "Source handle ledger input exceeds its bounds");
        return Status::invalid_argument;
    }

    try {
        SourceOwner imported;
        Diagnostic diagnostic{};
        if (dh2_world_import_level(&imported.value, level_name,
                level_source_path, level_xml, level_size, &diagnostic) != Error::ok) {
            set_error(error, diagnostic.message[0] ? diagnostic.message
                                                   : "Generated Level import failed");
            return Status::invalid_level;
        }

        const auto module_count = static_cast<std::size_t>(imported.value.module_count);
        if (!module_count || module_count > max_module_files / 2 ||
            file_count != module_count * 2) {
            set_error(error, "Need one ordered MGP and MVP file per source Module");
            return Status::invalid_file_order;
        }

        for (std::size_t index = 0; index < file_count; ++index) {
            const auto& file = ordered_files[index];
            const auto expected_module = index / 2;
            if (file.module_index != expected_module ||
                !file.source_path || !file.data || !file.size) {
                set_error(error, "Module file inputs are incomplete or out of load order");
                return Status::invalid_file_order;
            }
            const auto pair_begin = expected_module * 2;
            if ((index == pair_begin && file.kind != ModuleFileKind::mgp) ||
                (index == pair_begin + 1 && file.kind != ModuleFileKind::mvp)) {
                set_error(error, "Each Module needs MGP followed by MVP in source load order");
                return Status::invalid_file_order;
            }
        }

        Ledger candidate;
        candidate.first_source_handle = first_source_handle;
        candidate.initial_top_level_handle_offset =
            initial_top_level_handle_offset;
        candidate.next_handle_after = first_source_handle;
        std::map<std::string, std::size_t, std::less<>> handle_by_name;

        // _LoadFromXML sends root records to ObjectManager before InitPost
        // loads Module contents. The generated SourceLevel supports one
        // LevelConfig followed by ordered Module records; other root types are
        // rejected by world-data rather than silently skipped here.
        if (imported.value.config.source_begin >=
            imported.value.modules[0].record.source_begin) {
            set_error(error, "Generated LevelConfig must precede Module roots");
            return Status::invalid_level;
        }
        auto registration = register_object(imported.value, imported.value.config,
                                            candidate, handle_by_name, error);
        if (registration != RegisterStatus::ok) {
            return registration == RegisterStatus::handle_overflow
                ? Status::handle_overflow : Status::invalid_level;
        }
        for (std::uint32_t index = 0; index < imported.value.module_count; ++index) {
            const auto& module = imported.value.modules[index].record;
            if (index > 0 && module.source_begin <=
                    imported.value.modules[index - 1].record.source_begin) {
                set_error(error, "Module root records are not in source load order");
                return Status::invalid_level;
            }
            registration = register_object(imported.value, module, candidate,
                                           handle_by_name, error);
            if (registration != RegisterStatus::ok) {
                return registration == RegisterStatus::handle_overflow
                    ? Status::handle_overflow : Status::invalid_level;
            }
        }
        if (candidate.next_handle_after - first_source_handle !=
                initial_top_level_handle_offset) {
            set_error(error, "Explicit top-level handle offset does not match unique root names");
            return Status::invalid_top_level_offset;
        }

        for (std::size_t index = 0; index < file_count; ++index) {
            const auto& file = ordered_files[index];
            const auto status = dh2_world_import_module_objects(
                &imported.value, file.module_index, source_kind(file.kind), file.source_path,
                file.data, file.size, &diagnostic);
            if (status != Error::ok) {
                set_error(error, diagnostic.message[0] ? diagnostic.message
                                                       : "Source Module object import failed");
                return status == Error::limit ? Status::object_limit
                                              : Status::module_import_failed;
            }
        }

        if (imported.value.entity_count > max_source_objects) {
            set_error(error, "Source object count exceeds the handle ledger bound");
            return Status::object_limit;
        }
        const auto total_object_count = static_cast<std::size_t>(
            imported.value.entity_count) + imported.value.module_count + 1;
        if (total_object_count > max_source_objects) {
            set_error(error, "Source object count including root records exceeds the ledger bound");
            return Status::object_limit;
        }
        candidate.occurrences.reserve(total_object_count);
        candidate.entries.reserve(total_object_count);
        for (std::uint32_t index = 0; index < imported.value.entity_count; ++index) {
            const auto& object = imported.value.entities[index];
            if (object.module_index >= imported.value.module_count ||
                !source_kind(object.kind)) {
                set_error(error, "Imported object is missing source identity or provenance");
                return Status::module_import_failed;
            }
            const auto registration = register_object(imported.value, object,
                candidate, handle_by_name, error);
            if (registration != RegisterStatus::ok) {
                return registration == RegisterStatus::handle_overflow
                    ? Status::handle_overflow : Status::module_import_failed;
            }
        }

        output = std::move(candidate);
        return Status::ok;
    } catch (const std::bad_alloc&) {
        output = {};
        set_error(error, "Source handle ledger allocation failed");
        return Status::allocation_failed;
    } catch (...) {
        output = {};
        set_error(error, "Unexpected source handle ledger failure");
        return Status::allocation_failed;
    }
}

}  // namespace dh2::world::source_handle_ledger_v1
