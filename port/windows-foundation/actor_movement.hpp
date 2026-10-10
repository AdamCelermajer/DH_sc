#pragma once
#include "input_actions.hpp"
#include "renderer.hpp"
#include <string>
#include <functional>
#include "../level-world/actor_rotation.hpp"

namespace dh::foundation {
struct CameraMovementBasis;
struct CollisionScene;

// Values use original scene units, seconds, radians/sec and degrees. The host
// supplies recovered or explicitly configured values; no class/map presets live here.
struct ActorMovementConfig {
    float walkSpeed = 0, runSpeed = 0, turnSpeedRadians = 0;
    float bodyRadius = 0, bodyHeight = 0;
    float maxStepUp = 0, maxStepDown = 0, maxSlopeDegrees = 0;
    Vec3 forwardAxis{1,0,0}; // Authored model/animation forward, in its XY plane.
    std::string idleAnimation = "idle", walkAnimation = "walk";
    std::string runAnimation = "run", attackAnimation = "attack";
};
enum class ActorAction { Idle, Walk, Run, Attack };
struct ActorDesiredHeading {
    Vec3 direction{}; // Source HeadingState direction1b8: XY, Z zero.
    float sourceAngleRadians = 0; // Source -Y-forward heading convention.
    bool active = false; // Original squared XY length > 1e-4 gate.
};
struct ActorMovementState {
    Vec3 position{}; // Feet in original XY-ground, Z-up coordinates.
    float facingRadians = 0; // Z rotation applied to configured authored forwardAxis.
    ActorDesiredHeading desiredHeading; // Command intent, distinct from bounded visual facing.
    ActorAction action = ActorAction::Idle;
    std::string animationName = "idle";
    bool grounded = false;
};

class ActorMovement {
public:
    struct SourceMotionAdmissionResult {Vec3 position{};bool grounded=false;};
    using SourceMotionAdmission=std::function<bool(Vec3 current_world,Vec3 world_delta,
        SourceMotionAdmissionResult& admitted,std::string& error)>;
    struct SourceRotationBorrow {
        float* euler_xyz=nullptr;
        const float* heading_angle=nullptr;
        std::uint32_t* turn_positive=nullptr;
    };
    using SourceRotationSync=std::function<bool(const float* actual_euler_xyz,std::string& error)>;
    explicit ActorMovement(ActorMovementConfig config);
    const ActorMovementConfig& config() const noexcept { return config_; }
    const ActorMovementState& state() const noexcept { return state_; }
    void setPosition(Vec3 feet, float facingRadians = 0);
    // Publish current authoritative actor placement during the source
    // SubObjects phase. Preserve the already admitted heading/action/analog
    // intent; setPosition remains the startup/restore reset operation.
    void sync_source_position(Vec3 feet, float facingRadians);
    void setFacingRadians(float facingRadians);
    // Bind a source-produced game-unit radius without resetting motion/heading.
    bool set_body_radius(float radius,std::string& error);
    // Recovered GameObject SetHeadingDirection only. Does not turn the visual
    // or imply attack/movement state. Host must apply original command gates
    // (controller block/force, skill/cast, target admission) before calling.
    // XY magnitudes <=1 are retained; larger magnitudes normalize. Invalid
    // finite/overflow inputs return false without changing heading or facing.
    bool setDesiredHeading(Vec3 direction, bool rotateHeadingAngle = true);
    // Local stores of original GameObject.Stop: clear active/direction while
    // retaining heading angle. Navigation path/body/event effects are external.
    void stopDesiredHeading() noexcept;
    // Invalid inputs/deltas are ignored. Collision rejects unsupported ground,
    // excessive steps/slopes and swept body contact; axes slide along obstacles.
    // Floor admission samples the center, independently of body obstacle radius.
    // Swept 3D rays, configured height/slope limits and axis sliding are current
    // development approximations, not the original navigation edge-slide policy.
    // Returns true when the feet moved. Combat duration remains a gameplay concern.
    bool step(const InputActions&, const CameraMovementBasis&, const CollisionScene&, double dt);
    // Frame ordering for authored root motion: steer, choose/sample the resulting
    // animation, then apply_root_motion. Steering returns false on invalid input.
    // Action describes intent even when an obstacle prevents displacement.
    // Host input policy: zero nonattack intent maps to the local Stop stores;
    // zero-input attack preserves heading for an explicit combat provider.
    // This mapping does not prove original input dispatch or its command gates.
    bool steer(const InputActions&, const CameraMovementBasis&, double dt);
    // Source input intent phase: same heading/action/analog producer, no preview
    // bounded turn. Early scene/root displacement therefore uses previous facing.
    // Caller must supply proved controller/FSM input gates before this operation.
    bool steer_source_intent(const InputActions&,const CameraMovementBasis&,double dt);
    // Explicit LATE path -> rotation -> subobjects phase. Euler/heading/turn flag
    // borrow SAME actor fields; no private turn flag or source state is invented.
    // No angle normalization, physical-body rotation or retroactive displacement.
    // Source actor/motor angle writes precede SyncRotation; failed sync preserves
    // that completed source prefix and returns its error. Missing required sync
    // or malformed source policy is rejected before those angle writes.
    bool apply_source_rotation_late(SourceRotationBorrow,const dh2::actor::RotationPolicy&,
                                    const SourceRotationSync&,std::string& error);
    bool apply_source_character_rotation_late(SourceRotationBorrow,const std::uint32_t* actual_flags520,
        const std::int32_t* resolved224,std::uint32_t unsigned_dt_ms,bool visual_present,
        const SourceRotationSync&,std::string& error);
    // Delta is authored local XY displacement already scaled to world units.
    // Rotate by facing, scale analog movement, then use the shared collision solve.
    // Grounded locomotion follows floor height; local vertical root motion is ignored.
    // Idle ignores root drift; attack accepts authored attack/lunge translation.
    bool apply_root_motion(Vec3 localDelta, const CollisionScene&);
    bool apply_authored_root_motion(Vec3 localDelta, const CollisionScene&);
    // Same local-root rotation/analog policy, authoritative source admission.
    // No preview floor limits, body sweeps, substeps or axis slides are used.
    // true means a valid solve (including blocked/no movement); moved is separate.
    // Missing/failing/malformed source solve preserves movement state; no fallback.
    bool apply_root_motion(Vec3 localDelta,const SourceMotionAdmission&,bool& moved,std::string& error);
    bool apply_authored_root_motion(Vec3 localDelta,const SourceMotionAdmission&,bool& moved,std::string& error);
    bool step_root_motion(const InputActions&, const CameraMovementBasis&,
                          const CollisionScene&, Vec3 localDelta, double dt);
    bool step_root_motion(const InputActions&,const CameraMovementBasis&,
                          const SourceMotionAdmission&,Vec3 localDelta,double dt,
                          bool& moved,std::string& error);
private:
    bool steer_internal(const InputActions&,const CameraMovementBasis&,double dt,bool preview_turn);
    bool root_world_delta(Vec3 localDelta,Vec3& worldDelta)const;
    bool advance(Vec3 worldDelta, const CollisionScene&);
    ActorMovementConfig config_;
    ActorMovementState state_;
    float motionScale_ = 0;
};
}
