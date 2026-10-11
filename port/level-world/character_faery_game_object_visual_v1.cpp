#include "character_faery_game_object_visual_v1.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace dh2::character_faery_game_object_visual_v1 {
namespace {

using Component = factory::ctor::Component;

std::string basename(const std::string& path) {
    const auto separator = path.find_last_of("/\\");
    const auto name = path.substr(separator == std::string::npos
        ? 0 : separator + 1);
    if (name.empty() || name == "." || name == ".." ||
        name.find("..") != std::string::npos ||
        name.find_first_of("/\\") != std::string::npos)
        return {};
    return name;
}

bool finite(const Placement& placement) noexcept {
    for (const auto value : placement.position)
        if (!std::isfinite(value)) return false;
    for (const auto value : placement.rotation)
        if (!std::isfinite(value)) return false;
    for (const auto value : placement.scale)
        if (!std::isfinite(value)) return false;
    return true;
}

bool resolve_model(ModelDictView dictionary, const std::string& source_name,
                   std::int32_t& id, std::string& file,
                   Status& status) {
    id = -1;
    file = basename(source_name);
    if (file.empty()) {
        status = Status::model_unavailable;
        return false;
    }

    std::size_t exact_matches = 0;
    std::size_t exact_index = 0;
    for (std::size_t i = 0; i < dictionary.count; ++i) {
        if (dictionary.entries[i] == source_name) {
            ++exact_matches;
            exact_index = i;
        }
    }
    std::size_t selected = 0;
    if (exact_matches == 1) {
        selected = exact_index;
    } else if (exact_matches > 1) {
        status = Status::ambiguous_model;
        return false;
    } else {
        std::size_t basename_matches = 0;
        for (std::size_t i = 0; i < dictionary.count; ++i) {
            if (basename(dictionary.entries[i]) == file) {
                ++basename_matches;
                selected = i;
            }
        }
        if (basename_matches == 0) {
            status = Status::model_not_in_dictionary;
            return false;
        }
        if (basename_matches != 1) {
            status = Status::ambiguous_model;
            return false;
        }
    }
    if (selected > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max())) {
        status = Status::model_unavailable;
        return false;
    }
    id = static_cast<std::int32_t>(selected);
    status = Status::complete;
    return true;
}

bool valid_record(const factory::Record& record) noexcept {
    const auto identity = record.character.identity;
    return identity && record.constructor.identity == identity &&
        record.game_object.identity == identity &&
        record.character.object == &record.aggro_object &&
        record.constructor.constructor_complete && record.object_registered &&
        record.character_listed && record.source_properties_ready() &&
        !record.init_post_complete() && !record.init_final_complete() &&
        record.components.find(Component::game_object) == &record.game_object &&
        record.components.find(Component::animator) == &record.faery_visual &&
        record.source_model_name.size() < 4096;
}

void sync_facade(factory::Record& record, const Placement& placement) noexcept {
    auto& facade = record.faery_visual;
    facade.position = placement.position;
    facade.rotation = placement.rotation;
    facade.scale = placement.scale;
    facade.visible = placement.visible;
}

}  // namespace

Status load_sync(factory::Record& record, ModelDictView dictionary,
                 const Placement& placement, std::int32_t faery_id,
                 const Services& services, Result* output,
                 std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !valid_record(record) || !finite(placement) ||
        !dictionary.entries || !dictionary.count || dictionary.count > 1'000'000 ||
        !services.resolve_asset || faery_id < -1) {
        error = "Faery GameObject::InitPost visual owner inputs are incomplete";
        return Status::invalid_record;
    }

    auto& result = *output;
    result.character = record.character.identity;
    result.faery_id = faery_id;

    std::int32_t model_id = -1;
    std::string model_file;
    Status resolution_status{};
    if (!resolve_model(dictionary, record.source_model_name, model_id,
                       model_file, resolution_status)) {
        error = resolution_status == Status::model_unavailable
            ? "Faery source ModelFile is empty or invalid"
            : resolution_status == Status::model_not_in_dictionary
                ? "Faery source ModelFile is absent from the retained ModelDict"
                : "Faery source ModelFile maps ambiguously in the retained ModelDict";
        return resolution_status;
    }
    result.model_dictionary_id = model_id;

    auto& facade = record.faery_visual;
    const auto facade_address = reinterpret_cast<std::uintptr_t>(&facade);
    if (record.game_object.visual_object_2d8 &&
        record.game_object.visual_object_2d8 != facade_address) {
        error = "Faery GameObject already has a different visual owner";
        return Status::visual_conflict;
    }
    if (visual::ready(facade, record.character.identity)) {
        if (facade.asset->model_dictionary_id != model_id ||
            basename(facade.asset->model_file) != model_file ||
            (faery_id >= 0 && facade.asset->faery_id !=
                static_cast<std::uint32_t>(faery_id))) {
            error = "Faery GameObject visual was already initialized from a different ModelDict entry";
            return Status::visual_conflict;
        }
        if (facade.generation == std::numeric_limits<std::uint64_t>::max()) {
            error = "Faery GameObject visual generation is exhausted";
            return Status::generation_exhausted;
        }
        sync_facade(record, placement);
        record.game_object.visual_object_2d8 = facade_address;
        ++facade.generation;
        result.asset_loaded = true;
        result.pose_created = true;
        result.pose_synced = true;
        result.facade_published = true;
        result.game_object_visual_bound = true;
        return Status::already_complete;
    }
    if (facade.asset || facade.pose || facade.character || facade.generation) {
        error = "Faery GameObject visual facade has an incomplete prior initialization";
        return Status::visual_conflict;
    }

    std::shared_ptr<const visual::Asset> asset;
    ++result.provider_calls;
    try {
        if (services.resolve_asset(services.context, model_id, model_file,
                faery_id, &asset, error) != 0 || !asset) {
            if (error.empty()) error = "Faery ModelDict asset loader returned no asset";
            return Status::asset_load_failed;
        }
    } catch (...) {
        error = "Faery ModelDict asset loader threw";
        return Status::asset_load_failed;
    }
    result.asset_loaded = true;
    if (asset->model_dictionary_id != model_id ||
        basename(asset->model_file) != model_file ||
        asset->idle_sequence < 0 || !asset->renderer_data ||
        (faery_id >= 0 && asset->faery_id != static_cast<std::uint32_t>(faery_id))) {
        error = "Faery ModelDict asset has no matching BDAE/Idle renderer owner";
        return Status::invalid_asset;
    }

    std::shared_ptr<visual::CharacterPose> pose;
    try {
        pose = asset->renderer_data->create_character_pose();
        if (!pose) {
            error = "Faery ModelDict asset could not create a Character pose";
            return Status::pose_create_failed;
        }
    } catch (...) {
        error = "Faery ModelDict Character pose creation threw";
        return Status::pose_create_failed;
    }
    result.pose_created = true;
    try {
        if (!pose->sample(asset->idle_sequence, 0, error)) {
            if (error.empty()) error = "Faery GameObject::InitPost initial pose Sync failed";
            return Status::pose_sync_failed;
        }
    } catch (...) {
        error = "Faery GameObject::InitPost initial pose Sync threw";
        return Status::pose_sync_failed;
    }
    result.pose_synced = true;

    if (facade.generation == std::numeric_limits<std::uint64_t>::max()) {
        error = "Faery GameObject visual generation is exhausted";
        return Status::generation_exhausted;
    }
    facade.character = record.character.identity;
    facade.asset = std::move(asset);
    facade.pose = std::move(pose);
    facade.clip_sequence = facade.asset->idle_sequence;
    facade.clip_time_ms = 0;
    sync_facade(record, placement);
    ++facade.generation;
    record.game_object.visual_object_2d8 = facade_address;
    if (!visual::ready(facade, record.character.identity)) {
        facade = {};
        record.game_object.visual_object_2d8 = 0;
        error = "Faery GameObject InitPost did not publish a ready same-Record visual facade";
        return Status::invalid_asset;
    }
    result.facade_published = true;
    result.game_object_visual_bound = true;
    return Status::complete;
}

}  // namespace dh2::character_faery_game_object_visual_v1
