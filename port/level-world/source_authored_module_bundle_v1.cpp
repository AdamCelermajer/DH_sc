#include "source_authored_module_bundle_v1.hpp"

#include "../world-data/source_layout_adapter.h"

#include <algorithm>
#include <new>
#include <utility>

namespace dh2::world::source_authored_module_bundle_v1 {
namespace {

void fail(std::string& error, const char* text) {
    error = text ? text : "Authored Module bundle failed";
}

bool valid_module_ref(const dh2_world_source_module_ref& ref,
                      std::size_t expected_index) {
    return ref.module_index == expected_index && ref.name[0] &&
        ref.mgp_path[0] && ref.mvp_path[0];
}

}  // namespace

Status load(const std::uint8_t* mlx, std::size_t mlx_size,
            const char* level_name, const char* source_path,
            const Services& services,
            std::uint32_t first_source_handle,
            std::uint32_t initial_top_level_handle_offset,
            Bundle& output, std::string& error) {
    output = {};
    error.clear();
    if (!mlx || !mlx_size || !level_name || !*level_name || !source_path ||
        !*source_path || !services.read_cache) {
        fail(error, "Authored Module bundle input or cache reader is missing");
        return Status::invalid_input;
    }

    try {
        std::size_t module_count = 0;
        char message[160]{};
        if (!dh2_world_source_module_refs(mlx, mlx_size, level_name,
                source_path, nullptr, 0, &module_count, message,
                sizeof(message))) {
            error = message[0] ? message : "Authored MLX Module import failed";
            return Status::module_refs_failed;
        }
        if (!module_count || module_count > 256) {
            fail(error, "Authored MLX Module count is outside the runtime bound");
            return Status::module_refs_failed;
        }

        std::vector<dh2_world_source_module_ref> refs(module_count);
        if (!dh2_world_source_module_refs(mlx, mlx_size, level_name,
                source_path, refs.data(), refs.size(), &module_count,
                message, sizeof(message))) {
            error = message[0] ? message : "Authored MLX Module reference read failed";
            return Status::module_refs_failed;
        }
        if (module_count != refs.size()) {
            fail(error, "Authored MLX Module count changed between bounded reads");
            return Status::module_refs_failed;
        }

        Bundle candidate;
        candidate.modules.reserve(module_count);
        for (std::size_t index = 0; index < module_count; ++index) {
            const auto& ref = refs[index];
            if (!valid_module_ref(ref, index)) {
                fail(error, "Authored MLX has an empty or out-of-order MGP/MVP reference");
                return Status::module_refs_failed;
            }
            ModuleFiles module;
            module.module_index = ref.module_index;
            module.name = ref.name;
            module.mgp_path = ref.mgp_path;
            module.mvp_path = ref.mvp_path;
            if (!services.read_cache(services.context, module.mgp_path.c_str(),
                                     module.mgp_bytes, error) ||
                module.mgp_bytes.empty()) {
                if (error.empty()) error = "Authored MGP cache read returned no bytes";
                return Status::cache_read_failed;
            }
            if (!services.read_cache(services.context, module.mvp_path.c_str(),
                                     module.mvp_bytes, error) ||
                module.mvp_bytes.empty()) {
                if (error.empty()) error = "Authored MVP cache read returned no bytes";
                return Status::cache_read_failed;
            }
            candidate.modules.push_back(std::move(module));
        }

        candidate.mgp_views.reserve(module_count);
        candidate.mvp_views.reserve(module_count);
        candidate.ordered_files.reserve(module_count * 2);
        for (const auto& module : candidate.modules) {
            candidate.mgp_views.push_back({module.mgp_path.c_str(),
                module.mgp_bytes.data(), module.mgp_bytes.size()});
            candidate.mvp_views.push_back({module.mvp_path.c_str(),
                module.mvp_bytes.data(), module.mvp_bytes.size()});
            candidate.ordered_files.push_back({module.module_index,
                source_handle_ledger_v1::ModuleFileKind::mgp,
                module.mgp_path.c_str(), module.mgp_bytes.data(),
                module.mgp_bytes.size()});
            candidate.ordered_files.push_back({module.module_index,
                source_handle_ledger_v1::ModuleFileKind::mvp,
                module.mvp_path.c_str(), module.mvp_bytes.data(),
                module.mvp_bytes.size()});
        }

        const auto ledger_status = source_handle_ledger_v1::build(
            mlx, mlx_size, level_name, source_path,
            candidate.ordered_files.data(), candidate.ordered_files.size(),
            first_source_handle, initial_top_level_handle_offset,
            candidate.handle_ledger, error);
        if (ledger_status != source_handle_ledger_v1::Status::ok)
            return Status::ledger_failed;

        output = std::move(candidate);
        error.clear();
        return Status::complete;
    } catch (const std::bad_alloc&) {
        output = {};
        fail(error, "Authored Module bundle allocation failed");
        return Status::allocation_failed;
    } catch (...) {
        output = {};
        fail(error, "Unexpected authored Module bundle failure");
        return Status::allocation_failed;
    }
}

}  // namespace dh2::world::source_authored_module_bundle_v1
