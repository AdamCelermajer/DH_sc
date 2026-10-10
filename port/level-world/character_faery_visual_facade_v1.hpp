#pragma once

#include "objects.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <string>

namespace dh2::character_faery_visual_facade_v1 {

using Identity = std::uintptr_t;

// Immutable renderer cache entry. AssetData is shared by Characters selecting
// the same Faery. The current BDAE Resource still combines static geometry
// with mutable scene/skinned vertices and a non-copyable Player, so adapters
// must not claim parsed-geometry sharing until those owners are separated.
// This is a port resource contract, not an Irrlicht VisualObject.
struct CharacterPose {
    virtual ~CharacterPose() = default;
    // Existing native BDAE renderer consumes the same Resource/Scene/mesh
    // layout as ObjectGroup. Ownership remains per Character while GPU Draws
    // and materials can be borrowed from the matching cached ObjectGroup.
    virtual objects::Resource& bdae_resource() noexcept = 0;
    virtual bool sample(std::int32_t sequence, std::uint32_t time_ms,
                        std::string& error) = 0;
};

struct AssetData {
    virtual ~AssetData() = default;
    // Return a fresh mutable scene/pose for each CharacterVisual. Immutable
    // source bytes, textures, and any separated draw data may stay shared.
    virtual std::shared_ptr<CharacterPose> create_character_pose() const = 0;
};

struct Asset {
    std::uint32_t faery_id{};
    std::int32_t model_dictionary_id{-1};
    std::int32_t idle_sequence{-1};
    std::string model_file;
    std::shared_ptr<const AssetData> renderer_data;
};

// Mutable state belongs to one factory Character Record; only immutable
// model/texture/clip data is shared between Records.
struct CharacterVisual {
    Identity character{};
    std::shared_ptr<const Asset> asset;
    std::shared_ptr<CharacterPose> pose;
    std::array<float, 3> position{};
    std::array<float, 4> rotation{0.f, 0.f, 0.f, 1.f};
    std::array<float, 3> scale{1.f, 1.f, 1.f};
    std::int32_t clip_sequence{-1};
    std::uint32_t clip_time_ms{};
    bool visible{};
    std::uint64_t generation{};
};

inline bool ready(const CharacterVisual& visual, Identity character) noexcept {
    return character && visual.character == character && visual.asset &&
        visual.asset->renderer_data && visual.asset->model_dictionary_id >= 0 &&
        visual.asset->idle_sequence >= 0 && !visual.asset->model_file.empty() &&
        visual.pose &&
        visual.clip_sequence == visual.asset->idle_sequence;
}

} // namespace dh2::character_faery_visual_facade_v1
