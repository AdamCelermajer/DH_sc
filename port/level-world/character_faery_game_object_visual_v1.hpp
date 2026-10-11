#pragma once

#include "character_faery_visual_facade_v1.hpp"
#include "character_runtime_factory_v1.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace dh2::character_faery_game_object_visual_v1 {

namespace factory = character_runtime_factory_v1;
namespace visual = character_faery_visual_facade_v1;

struct ModelDictView {
    const std::string* entries{};
    std::size_t count{};
};

struct Placement {
    std::array<float, 3> position{};
    std::array<float, 4> rotation{0.f, 0.f, 0.f, 1.f};
    std::array<float, 3> scale{1.f, 1.f, 1.f};
    bool visible{};
};

struct Services {
    void* context{};
    // The renderer reads the selected BDAE/idle resources and returns the
    // immutable asset owner. The per-Character mutable pose is created here.
    int (*resolve_asset)(void*, std::int32_t model_dictionary_id,
                         const std::string& model_file,
                         std::int32_t faery_id,
                         std::shared_ptr<const visual::Asset>* output,
                         std::string& error){};
};

enum class Status : std::uint8_t {
    complete,
    already_complete,
    invalid_record,
    model_unavailable,
    model_not_in_dictionary,
    ambiguous_model,
    asset_load_failed,
    invalid_asset,
    visual_conflict,
    pose_create_failed,
    pose_sync_failed,
    generation_exhausted,
};

struct Result {
    visual::Identity character{};
    std::int32_t model_dictionary_id{-1};
    std::int32_t faery_id{-1};
    std::uint32_t provider_calls{};
    bool asset_loaded{};
    bool pose_created{};
    bool pose_synced{};
    bool facade_published{};
    bool game_object_visual_bound{};
};

// Source GameObject::InitPost calls LoadVisualObject and then VisualObject::Sync.
// This adapter resolves the canonical Record's GetCharModelName result against
// ModelDict, asks the active renderer to load its real asset, and publishes one
// fresh mutable pose into the same Record's facade. POFaerie/physics attachment
// is a separate later lifecycle stage.
Status load_sync(factory::Record&, ModelDictView, const Placement&,
                 std::int32_t faery_id, const Services&, Result*,
                 std::string& error);

}  // namespace dh2::character_faery_game_object_visual_v1
