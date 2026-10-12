#include "crypt_module_bounds_registry_v1.hpp"

#include "module_scene_root_bounds.hpp"
#include "../world-data/world.hpp"

#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <utility>

namespace dh2::crypt_module_bounds_registry_v1 { namespace {

constexpr std::uint32_t no_module = std::numeric_limits<std::uint32_t>::max();

bool add(std::uint64_t& total, std::uint32_t value) noexcept {
    if (total > std::numeric_limits<std::uint64_t>::max() - value) return false;
    total += value;
    return true;
}

bool finite_bounds(const dh2::octree::Box& box) noexcept {
    for (unsigned axis = 0; axis != 3; ++axis) {
        if (!std::isfinite(box.minimum[axis]) || !std::isfinite(box.maximum[axis]) ||
            box.minimum[axis] > box.maximum[axis]) return false;
    }
    return true;
}

// One opened source catalogue. The owning path is the dae name Module::LoadModule
// uses for that file; an empty path marks the single-catalogue form.
struct OpenedCatalogue {
    std::string path;
    dh2::resources::BresView view{};
    dh2::scene_payload::Scene scene{};
};

Status build_parsed(const dh2::world::SourceLevel* source,
                    const std::vector<std::unique_ptr<OpenedCatalogue>>& catalogues,
                    Owner* owner, Result* report) {
    if (!source || catalogues.empty() || !owner || !report) return Status::invalid_argument;
    for (const auto& opened : catalogues) {
        if (!opened || !opened->view.bytes || !opened->view.size ||
            opened->scene.image.bytes != opened->view.bytes ||
            opened->scene.image.size != opened->view.size)
            return Status::invalid_argument;
    }
    const bool single = catalogues.size() == 1 && catalogues[0]->path.empty();
    if (!source->module_count) return Status::empty_root;
    if (source->module_count > max_modules || !source->modules)
        return Status::module_limit;

    Owner candidate{};
    Result totals{};
    totals.failed_module_index = no_module;
    totals.module_count = source->module_count;
    candidate.modules.reserve(source->module_count);

    for (std::uint32_t i = 0; i < source->module_count; ++i) {
        const auto& module = source->modules[i];
        if (!module.record.name || !*module.record.name || !module.catalogue_node_id ||
            !*module.catalogue_node_id || !module.cache_dae || !*module.cache_dae) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::identity_missing;
        }
        // Each Module reads its own dae (Module::LoadModule), so the module's
        // catalogue is the opened file whose path equals its cache_dae.
        const OpenedCatalogue* catalogue = nullptr;
        if (single) {
            if (i == 0) {
                candidate.catalogue_path = module.cache_dae;
            } else if (std::strcmp(candidate.catalogue_path.c_str(), module.cache_dae) != 0) {
                totals.failed_module_index = i;
                *report = totals;
                return Status::catalogue_mismatch;
            }
            catalogue = catalogues[0].get();
        } else {
            for (const auto& opened : catalogues) {
                if (opened->path == module.cache_dae) { catalogue = opened.get(); break; }
            }
            if (!catalogue) {
                totals.failed_module_index = i;
                *report = totals;
                return Status::catalogue_mismatch;
            }
            if (i == 0) candidate.catalogue_path = module.cache_dae;
        }

        for (float coordinate : {module.record.world_position[0],
                                 module.record.world_position[1],
                                 module.record.world_position[2]}) {
            if (!std::isfinite(coordinate)) {
                totals.failed_module_index = i;
                *report = totals;
                return Status::unsupported_root;
            }
        }

        dh2::module_scene_root_bounds::Result root{};
        const auto root_status = dh2::module_scene_root_bounds::build(
            &module, &catalogue->scene, &catalogue->view, module.record.world_position, &root);
        if (root_status == dh2::module_scene_root_bounds::Status::unsupported_transform) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::unsupported_root;
        }
        if (root_status == dh2::module_scene_root_bounds::Status::empty_scene) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::empty_root;
        }
        if (root_status != dh2::module_scene_root_bounds::Status::complete) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::bounds_failure;
        }
        if (root.ignored_non_geometry_instances) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::non_geometry_instance;
        }
        if (!root.scene_nodes || !root.geometry_instances || !root.draw_buffers) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::empty_root;
        }
        if (!finite_bounds(root.bounds)) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::bounds_failure;
        }

        Entry entry{};
        entry.module_index = i;
        entry.source_record = module.record.source_record;
        entry.module_name = module.record.name;
        entry.root_id = module.catalogue_node_id;
        for (unsigned axis = 0; axis != 3; ++axis) {
            entry.origin[axis] = module.record.world_position[axis];
            entry.bounds.minimum[axis] = root.bounds.minimum[axis];
            entry.bounds.maximum[axis] = root.bounds.maximum[axis];
        }
        entry.scene_nodes = root.scene_nodes;
        entry.geometry_instances = root.geometry_instances;
        entry.draw_buffers = root.draw_buffers;
        entry.ignored_non_geometry_instances = root.ignored_non_geometry_instances;
        candidate.modules.push_back(std::move(entry));

        if (!add(totals.scene_nodes, root.scene_nodes) ||
            !add(totals.geometry_instances, root.geometry_instances) ||
            !add(totals.draw_buffers, root.draw_buffers)) {
            totals.failed_module_index = i;
            *report = totals;
            return Status::bounds_failure;
        }
    }

    owner->catalogue_path.swap(candidate.catalogue_path);
    owner->modules.swap(candidate.modules);
    *report = totals;
    return Status::complete;
}

struct SourceOwner {
    dh2::world::SourceLevel value{};
    ~SourceOwner() { dh2_world_free(&value); }
};

} // namespace

Status build_from_catalogues(const char* level_name, const char* source_path,
                             const std::uint8_t* mlx_bytes, std::size_t mlx_size,
                             const std::vector<CatalogueBytes>& catalogues,
                             Owner* owner, Result* report) noexcept {
    if (!report) return Status::invalid_argument;
    Result initial{};
    initial.failed_module_index = no_module;
    *report = initial;
    if (!level_name || !*level_name || !source_path || !*source_path ||
        !mlx_bytes || !mlx_size || catalogues.empty() || !owner)
        return Status::invalid_argument;

    try {
        SourceOwner source{};
        dh2::world::Diagnostic diagnostic{};
        if (dh2_world_import_level(&source.value, level_name, source_path,
                mlx_bytes, mlx_size, &diagnostic) != dh2::world::Error::ok)
            return Status::import_failure;

        std::vector<std::unique_ptr<OpenedCatalogue>> opened;
        for (const auto& input : catalogues) {
            if (!input.bytes || !input.size) return Status::invalid_argument;
            auto item = std::make_unique<OpenedCatalogue>();
            item->path = input.path;
            if (dh2_bres_open(&item->view, input.bytes, input.size) !=
                dh2::resources::BresError::ok) return Status::catalogue_failure;
            if (dh2_scene_open(&item->scene, &item->view) != dh2::scene_payload::Error::ok)
                return Status::scene_failure;
            opened.push_back(std::move(item));
        }
        return build_parsed(&source.value, opened, owner, report);
    } catch (const std::bad_alloc&) {
        return Status::allocation_failure;
    } catch (...) {
        return Status::allocation_failure;
    }
}

Status build_from_assets(const char* level_name, const char* source_path,
                         const std::uint8_t* mlx_bytes, std::size_t mlx_size,
                         const std::uint8_t* bres_bytes, std::size_t bres_size,
                         Owner* owner, Result* report) noexcept {
    // Single-catalogue form: every module of the level comes from this BRES.
    return build_from_catalogues(level_name, source_path, mlx_bytes, mlx_size,
        {CatalogueBytes{std::string(), bres_bytes, bres_size}}, owner, report);
}

const char* status_name(Status status) noexcept {
    switch (status) {
    case Status::complete: return "complete";
    case Status::invalid_argument: return "invalid_argument";
    case Status::import_failure: return "import_failure";
    case Status::catalogue_failure: return "catalogue_failure";
    case Status::scene_failure: return "scene_failure";
    case Status::module_limit: return "module_limit";
    case Status::identity_missing: return "identity_missing";
    case Status::catalogue_mismatch: return "catalogue_mismatch";
    case Status::unsupported_root: return "unsupported_root";
    case Status::empty_root: return "empty_root";
    case Status::non_geometry_instance: return "non_geometry_instance";
    case Status::bounds_failure: return "bounds_failure";
    case Status::allocation_failure: return "allocation_failure";
    }
    return "unknown";
}

} // namespace dh2::crypt_module_bounds_registry_v1
