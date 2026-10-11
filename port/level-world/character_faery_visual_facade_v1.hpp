#pragma once

#include "objects.hpp"
#include "character_anim_set_owner_v1.hpp"
#include "faery_animation_owner_v1.hpp"

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
    virtual bool interval(std::int32_t sequence, std::int32_t& start_ms,
                          std::int32_t& end_ms, std::string& error) const {
        (void)sequence;
        start_ms = end_ms = 0;
        error = "Faery Character pose does not expose an authored playback interval";
        return false;
    }
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
    character_anim_set_owner_v1::AnimSetState anim_set;
    // Canonical source animator state for this Record. The owner uses the
    // existing AnimationScheduler and timeline kernel with the engine's frame
    // timestamp; it does not create a renderer clock or another Character.
    faery_animation_owner_v1::State animator;
    std::shared_ptr<const Asset> asset;
    std::shared_ptr<CharacterPose> pose;
    std::array<float, 3> position{};
    std::array<float, 4> rotation{0.f, 0.f, 0.f, 1.f};
    std::array<float, 3> scale{1.f, 1.f, 1.f};
    std::int32_t clip_sequence{-1};
    std::uint32_t clip_time_ms{};
    // Source VisualObject+0x40 LightSet index, written by GameObject/Character
    // InitFinal against the one shared scene light-name table.
    std::int32_t light_set_id_40{};
    bool visible{};
    std::uint64_t generation{};
};

inline bool ready(const CharacterVisual& visual, Identity character) noexcept {
    if (!(character && visual.character == character && visual.asset &&
        visual.asset->renderer_data && visual.asset->model_dictionary_id >= 0 &&
        visual.asset->idle_sequence >= 0 && !visual.asset->model_file.empty() &&
        visual.pose)) return false;
    if (visual.clip_sequence == visual.asset->idle_sequence) return true;
    return visual.anim_set.character == character &&
        visual.anim_set.phase == character_anim_set_owner_v1::Phase::ready &&
        [&]() noexcept {
            for (const auto& registration : visual.anim_set.registrations)
                if (registration.kind ==
                        character_anim_set_owner_v1::RegistrationKind::animation &&
                    registration.source_sequence_id == visual.clip_sequence &&
                    registration.animation_dictionary_id >= 0)
                    return true;
            return false;
        }();
}

} // namespace dh2::character_faery_visual_facade_v1
