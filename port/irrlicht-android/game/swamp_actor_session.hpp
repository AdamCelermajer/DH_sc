#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>

namespace dh2::irrlicht_game {
class PrinceActor;
class PrinceCharacterRuntime;
class SwampActorFloorBridge;

// Caller-owned source products needed to create the native player. The bounds
// are the source CharacterOwnerBounds result; world bounds are Box2D/physical
// coordinates (source XY units multiplied by 0.01). This keeps property,
// equipment and level-bound production outside this bounded SWAMP adapter.
struct SwampActorSessionConfig {
    std::array<float, 4> physics_world_bounds{};
    std::array<float, 3> initial_position{};
    std::array<float, 6> local_bounds{};
    std::array<float, 6> absolute_bounds{};
    std::array<std::int32_t, 224> resolved_character_properties{};
};

struct SwampActorFrameInput {
    std::uint32_t dt_ms = 0;
    float input_x = 0.0f;
    float input_y = 0.0f;
    bool accepted = false;
};

enum class SwampBodyServiceCall : std::uint32_t {
    pin = 1,
    unpin = 2,
    stop = 3,
};

struct SwampActorFrameResult {
    std::uint32_t frame = 0;
    std::uint32_t world_steps = 0;
    std::uint32_t state_flags = 0;
    std::uint32_t actor_phase = 0;
    std::uint32_t source_floor = 0xffffffffu;
    std::uint32_t body_pinned = 0;
    std::uint32_t body_present = 0;
    std::uint32_t body_service_count = 0;
    std::array<std::uint32_t, 16> body_service_order{};
    std::uint32_t source_path_segments = 0;
    std::uint32_t source_path_requested = 0;
    std::array<float, 3> position{};
    std::array<float, 2> physics_position{};
    std::int32_t source_state = -1;
    std::int32_t sequence = -1;
    std::int32_t clip = -1;
    std::uint32_t path_boundary_checked = 0;
    std::uint32_t path_direction_valid = 0;
    // Per-completed-frame movement diagnostics. Desired is the caller's touch
    // direction; validated is the PF controller's post-boundary direction.
    std::array<float, 2> desired_heading{};
    std::array<float, 2> validated_heading{};
    float desired_rotation = 0.0f;
    float current_rotation = 0.0f;
    float body_radius_physics = 0.0f;
    float body_radius_source = 0.0f;
    std::array<float, 3> source_owner_delta{};
    std::array<float, 3> animation_root_delta{};
};

// A bounded source-frame composition, not a second Character FSM. It consumes
// PrinceCharacterRuntime's recovered Coordinator and BlendedPlayback, bridges
// their owner position into actor_runtime, steps one NativeWorld, and validates
// actor movement against the source module-zero floor bridge. The Irrlicht
// SWAMP NativeActivity owns one session for its source-driven actor frames.
class SwampActorSession final {
    struct Impl;
    std::unique_ptr<Impl> impl_;

public:
    SwampActorSession();
    ~SwampActorSession();
    SwampActorSession(SwampActorSession&&) noexcept;
    SwampActorSession& operator=(SwampActorSession&&) noexcept;
    SwampActorSession(const SwampActorSession&) = delete;
    SwampActorSession& operator=(const SwampActorSession&) = delete;

    bool initialize(PrinceActor& actor, PrinceCharacterRuntime& character,
                    const SwampActorFloorBridge& floor,
                    const SwampActorSessionConfig& config,
                    std::string& error);
    bool frame(const SwampActorFrameInput& input,
               SwampActorFrameResult& output, std::string& error);
    void shutdown();

    bool ready() const;
    const SwampActorFrameResult& last_frame() const;
};

} // namespace dh2::irrlicht_game
