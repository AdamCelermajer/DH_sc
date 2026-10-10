#pragma once
#include "../../animation_markers.hpp"
#include "../../../level-world/character_mesh_fx_owner_v4.hpp"
#include "../../../level-world/character_animation_step_fx_v2.hpp"
#include <set>
#include <tuple>

namespace dh::foundation::effects {
// Actor identity is the actual source owner; socket identity is the original
// subobject owner if one is supplied by the original attachment producer.
// Both must resolve through the manager's SAME live Scene services.
struct ActorAttachment {
    std::uintptr_t actor{}, socket{};
    float target_position[3]{}, source_rotation[3]{};
};
struct PhaseOccurrence {
    std::uintptr_t actor{};
    std::uint64_t generation{}, occurrence{};
    std::vector<std::size_t> source_path;
};
struct StepServices {
    void* context{};
    // Required whole original equipment/Swoosh decision; never infer this gate
    // from rendered weapon appearance or an actor name.
    int (*swoosh_fx_gate)(void*, const dh2::data::AnimationStep&, bool&){};
};
enum class DispatchResult { ignored, delivered, duplicate, required_failure };

// Borrows the campaign's actual retained VisualFXManager and its resource pool.
// No animation clock, scene, resource cache or actor state machine is created.
class EffectsExecutor {
public:
    explicit EffectsExecutor(dh2::fx::CharacterMeshFxOwnerV4& manager) : manager_(manager) {}
    DispatchResult marker(const ActorAttachment&, const std::string& clip,
                          const MarkerOccurrence&, std::string& error);
    DispatchResult phase(const ActorAttachment&, const PhaseOccurrence&,
                         const dh2::data::AnimationStep&, StepServices,
                         std::string& error);
    // Explicit authored producers (skill result, fairy owner, weapon producer)
    // supply their real selected set ID and anchor policy. -1 remains no effect.
    bool play_set(std::int32_t set, const ActorAttachment&, bool anchored,
                  bool attach_to_socket, std::uintptr_t* source_identity,
                  std::string& error);
    bool stop(std::uintptr_t& source_identity, std::string& error);
    void release_actor(std::uintptr_t actor, std::uintptr_t nullable_socket = 0);
    // Invoke in original source order: SceneManager sampling, then manager
    // update. The caller supplies source absolute time/dt; draw never ticks.
    bool scene_phase(std::int32_t source_absolute_ms, std::int32_t source_app_dt,
                     std::string& error);
    bool manager_phase(std::int32_t source_app_dt, std::string& error);
    bool draw_sources(std::vector<dh2::fx::CharacterFxMeshDrawSourceV4>&,
                      std::vector<dh2::fx::CharacterParticleDrawSourceV3>&,
                      std::string& error) const;
private:
    using MarkerKey = std::tuple<std::uintptr_t, std::string, std::uint64_t,
                                 std::uint64_t, std::uint32_t>;
    using PhaseKey = std::tuple<std::uintptr_t, std::uint64_t, std::uint64_t,
                                std::vector<std::size_t>>;
    dh2::fx::CharacterMeshFxOwnerV4& manager_;
    std::set<MarkerKey> markers_;
    std::set<PhaseKey> phases_;
};
} // namespace dh::foundation::effects
