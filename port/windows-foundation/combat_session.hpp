#pragma once
#include "original_combat_visual_plan.hpp"
#include "actor_population.hpp"
#include "playable_actor_world.hpp"
#include "actor_combat_runtime.hpp"
#include "input_actions.hpp"
#include "original_attack_sequence.hpp"
#include "original_actor_animation_events.hpp"
#include "features/audio/retained_frame_audio_clock.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation {
// Actual hierarchy step entry before its animation/hit markers. Configured
// seeded injury/death leaves report entry after successful pose selection,
// before advancement; restoring a corpse does not report a new occurrence.
// Sequence/step identify the original AnimTable row; consumers read its Sound,
// Swoosh and FX fields rather than deriving cues from combat hit outcomes.
struct CombatSessionStepEntry {
    ActorId actor=invalid_actor_id;
    std::weak_ptr<const void> binding_lease;
    std::uint64_t occurrence=0,update_serial=0;
    std::int64_t sequence_id=-1;
    std::uint32_t depth=0,step=0;
    std::vector<std::size_t> container_path,leaf_path;
    std::optional<RetainedFrameAudioClock> audio_clock;
};
using CombatSessionStepObserver=std::function<void(const CombatSessionStepEntry&)>;
struct CombatSessionChoice {
    std::string state;
    std::size_t variant=0;
    std::vector<std::size_t> leafPath;
};
struct CombatSessionProfile {
    CombatSessionChoice action, initialIdle;
    // When supplied, executes the explicitly selected source phase group.
    // actor_rate is a caller multiplier (normally explicit 1); session multiplies
    // it by original resolved PROPS_AttackSpeed property48 after actual gear.
    std::optional<OriginalAttackSelection> sequenceAction;
    bool retainedPhaseClock=false;
    // Opt-in exact source input windows and hierarchy progression. Requires a
    // retained full-root action; scoped diagnostic groups remain unchanged.
    bool sourceCombo=false;
    std::optional<CombatSessionChoice> reaction, death;
    // Source marker names and main/off source choice must be explicit.
    std::vector<std::string> damageMarkerNames;
    bool offHand=false, alternate=false;
    ActorCustomization customization;
    // Explicit host root selection for supplied NPC visuals. Empty retains
    // unconsumed model-space playback; "auto" uses original GetAnimRoot order.
    std::string motionRoot;
    OriginalActorPropertyOptions propertyOptions;
    std::int32_t originalCombatState=-1;
    // Explicit source skill/cinematic clip aliases appended before this actor's
    // visual is loaded. Applies equally to players and NPCs. Selection and
    // source program metadata remain caller-owned; no class bank is inferred.
    std::vector<std::pair<std::string,std::string>> sourceAnimationClips;
    // Exact authored state gate supplied by source actor/spawn owner.
    bool diagnosticAIEnabled=false;
    std::string requiredAIState;
    // Own the exact source visual/ActorId in this session without combat
    // admission. By default these actors are untargetable with no attack records;
    // their authored Idle and explicit source animation programs use the same
    // retained pose owner and session update/restore lifecycle. The independent
    // incoming-only capability below can admit damage without outgoing attacks.
    bool animationOnly=false;
    // Independent incoming capability for a no-outgoing profile above.
    // Requires this profile's explicit original Injury/Died choices. Does not
    // create attack IDs or admit player/AI outgoing attack commands.
    bool receiveDamage=false;
    // Source MP_MinimalRandoms policy for flat Type2 Injury. true selects step0
    // without a draw; false uses the shared app-normal RNG once. Unset keeps
    // the explicitly pinned legacy reaction leaf. Caller owns this policy.
    std::optional<bool> reactionMinimalRandoms;
    // Explicit same-profile equipped Attack/AttackStatic graphs from original
    // tables. Their named clips are loaded before this visual is published;
    // Idle/Injured/Died and other source state metadata remain unchanged.
    // Requires sequenceAction and its explicit choice. This does not hot-swap
    // a running actor, admit a controller request, or create ranged hits.
    std::optional<OriginalCombatVisualPlan> sourceAttackBank;
    OriginalSequencePolicies sourceAttackPolicies;
    // Melee state5 original named events choose the original hand separately.
    // Opt-in; legacy diagnostic marker/profile-wide offHand remains supported.
    bool sourceMeleeHandMarkers=false;
    // Original player CSAttack focus: previous state4 chooses Attack, otherwise
    // AttackStatic, only after a new command is admitted. Both exact source
    // roots must be preloaded above; held continuation never replaces a root.
    bool sourceAttackStateSelection=false;
};
struct CombatSessionConfig {
    std::optional<std::uint32_t> diagnosticRngSeed;
    // Continue caller-owned startup draws (for example authored population
    // selection) without reseeding or dropping the persisted call count.
    // Supply exactly one of this state and diagnosticRngSeed.
    std::optional<dh2::data::CombatRandom> initialRandomState;
    ActorId playerId=invalid_actor_id;
    std::string playerProfileId;
    std::string tableRoot;
    // Exact source ItemTable names, one entry per equipped occurrence. Repeated
    // definitions remain distinct. Hand selectors claim distinct occurrences;
    // an omitted hand occurrence is appended for legacy CLI callers.
    // Generated powers/real persistent instance IDs require a separate host.
    std::vector<std::string> equippedItemIds;
    std::string mainItemId, offItemId;
    CharacterVisualConfig playerVisualConfig;
    std::map<std::string,CombatSessionProfile> profiles;
    // Actual selected save/creation state, borrowed only during initialize.
    // Project onto equipped source properties before binding player vitals;
    // never refill saved health or retain the caller's CharacterState pointer.
    // Null preserves legacy diagnostic startup policy.
    const CharacterState* selectedPlayerProfile=nullptr;
};
struct RetainedAnimationEvent;
struct CombatSessionComboBoundary {
    ActorId actor=0;std::uint64_t generation=0;
    bool beginning=false;
    std::uint32_t depth=0,step=0,count=0;
    std::string source_clip;
};
struct CombatSessionStateAnimationServices {
    std::function<bool(ActorId,const RetainedAnimationEvent&,std::string&)> event;
    std::function<bool(ActorId,std::string&)> finished;
    // Accepted source OnBlur, before the incoming pose is selected. It also
    // runs when Use has not fired. The owning feature retires its occurrence
    // and performs the audited Post; no generic damage-to-Post inference.
    std::function<bool(ActorId,std::int32_t from_state,std::int32_t to_state,std::string&)> departed;
    // Retained after generic completion/departure: transient cooldown owners
    // must reject checkpoints while a timer cannot be persisted faithfully.
    std::function<bool(std::string&)> checkpoint;
};
struct CombatSessionSourceSequencePolicy {
    std::int32_t original_state=-1; // Audited Skill6 or Cast7.
    std::uint32_t state_flags=0;    // Actual focus flags; not a class guess.
    std::uint64_t generation=0;    // Calling feature's cast occurrence.
};
struct CombatSessionObjectAnimationServices {
    // Consumers may select/remove object visuals, but must defer Session
    // replacement/restore until update returns (same rule as actor consumers).
    std::function<bool(ObjectId,const RetainedAnimationEvent&,std::string&)> event;
    // Source generic container completion passes timeline.loop != 0.
    std::function<bool(ObjectId,std::uint64_t generation,bool loop,std::string&)> finished;
};
struct DiagnosticControllerAdmissionFacts {
    ActorId owner_id=invalid_actor_id;
    std::uintptr_t controllable=0;
    std::uint32_t global_blocked=0,local_locked=0,forced=0,network_enabled=0;
};
struct CombatSessionAnimationDispatch {
    ActorId actor=invalid_actor_id;
    std::uint32_t event=0,service=0;
    std::int32_t lag_ms=0;
    std::string authored_name;
    bool synchronous_source_hit=false;
};
struct CombatSessionAnimationNotificationServices {
    // Same live actor ownership as combat/lifecycle. Numeric22..27 callback
    // returns the actual consumer result; no begin/end state effect is assumed.
    std::function<bool(ActorId,const dh2::character::AnimationEventRequest&,std::int32_t&,std::string&)> notification;
    std::function<bool(ActorId,std::uint32_t,const RetainedAnimationEvent*,std::string&)> state_event;
};
// One admitted skill/spell animation event. Generation is the caller's cast
// occurrence, not a frame timer; event_index distinguishes authored multi-hits.
// The formula sheet is borrowed only during this synchronous call.
struct CombatSessionSourceHit {
    ActorId attacker=invalid_actor_id,target=invalid_actor_id;
    std::weak_ptr<const void> binding_lease;
    std::uint64_t generation=0;
    std::uint32_t event_index=0,mask=0;
    std::string source_id,marker_name;
    std::int32_t category=-1,element=-1,direct_amount=0;
    const dh2::data::PropertySheet* attacker_formula_sheet=nullptr;
};
struct CombatSessionSourceCalculation {
    bool calculated=false;
    OriginalMeleeResolution result;
};
struct CombatSessionMotionSample {
    Vec3 authored_motion{};
    bool move_go=false;
};
enum class CombatSessionTransitionStage { blur, focus_prefix, focus_suffix };
struct CombatSessionActorTransition {
    ActorId actor=invalid_actor_id;
    std::int32_t from_state=-1,to_state=-1;
    std::uint64_t generation=0,occurrence=0,update_serial=0;
    CombatRuntimeTransitionCause cause=CombatRuntimeTransitionCause::attack;
    CombatSessionTransitionStage stage=CombatSessionTransitionStage::blur;
    std::weak_ptr<const void> binding_lease;
    // Exact authored root already prepared for this accepted attack. Unknown
    // for other transitions/unrecognized roots; never inferred from a clip.
    std::optional<bool> source_attack_moving;
};
// Read-only modern borrow witness. Session destruction invalidates it even if
// a borrower temporarily pins this token; it does not keep the Session alive.
class CombatSessionLifetime final {
public:
    bool alive()const noexcept{return alive_;}
private:
    bool alive_=true;
    friend class CombatSession;
};
class CombatSession {
public:
    CombatSession();
    ~CombatSession();
    CombatSession(const CombatSession&)=delete;
    CombatSession& operator=(const CombatSession&)=delete;
    bool initialize(const AssetCatalog&,const OriginalPropertyDatabase&,
                    const OriginalMeleeBindings&,const CombatSessionConfig&,
                    CharacterVisual& player,ActorPopulation&,Vec3 initialPlayerPosition,
                    const ActorCustomization& playerCustomization,std::string& error);
    bool update(double dt,const InputActions&,Vec3 playerPosition,float facingRadians,std::string& error,
                const RetainedFrameAudioClock* audio_clock=nullptr);
    std::uint64_t update_serial()const noexcept;
    // Presentation-only observers must not mutate/reenter gameplay. Exceptions
    // are diagnosed without vetoing the source cursor or combat update.
    void set_step_entry_observer(CombatSessionStepObserver);
    void clear_step_entry_observer();
    const std::vector<std::string>& step_entry_diagnostics()const noexcept;
    // Optional diagnostic sidecar after the original gameplay animation-event
    // consumer. Audio observer results never change gameplay callback results.
    void set_retained_frame_audio_observer(RetainedFrameAudioObserver);
    void clear_retained_frame_audio_observer();
    const std::vector<RetainedFrameAudioObserverDiagnostic>& retained_frame_audio_diagnostics()const noexcept;
    // Bind host aliases to explicit authored leaf metadata. Rate multiplier is
    // caller policy (e.g. resolved movement modifier), never inferred from class.
    // Session.update is the sole animation advancement after selecting an alias.
    bool bind_player_locomotion(const std::string& alias,const CombatSessionChoice&,
                                double actor_rate,bool loop,std::string& error);
    bool select_player_locomotion(const std::string& alias,std::string& error);
    bool bind_actor_locomotion(ActorId,const std::string& alias,const CombatSessionChoice&,
                               double actor_rate,bool loop,std::string& error);
    // External original-data banks permit equipment stance changes without
    // regenerating the actor or limiting choices to its initial exported bank.
    // Named clips must already be loaded on this actor's visual. Metadata is
    // copied; the caller's bank need not outlive this call.
    bool bind_actor_locomotion_from_bank(ActorId,const std::string& alias,
                               const OriginalCombatVisualPlan&,const CombatSessionChoice&,
                               double actor_rate,bool loop,std::string& error);
    bool select_actor_locomotion(ActorId,const std::string& alias,std::string& error);
    bool uses_retained_player_locomotion()const noexcept;
    bool has_player_locomotion(const std::string& alias)const noexcept;
    const class RetainedPosePlayback* retained_player_pose()const noexcept;
    const class RetainedPosePlayback* retained_actor_pose(ActorId)const noexcept;
    // Same visual animated by this session. Borrow only while the current
    // actor_binding_lease is retained; reacquire after detach/restore.
    const CharacterVisual* retained_actor_visual_borrow(ActorId)const noexcept;
    // Host lifetime witness for external native borrowers. Expires on detach
    // or session replacement; a successful restore rebind issues a new lease.
    // This is bookkeeping, not an original Character field or gameplay state.
    std::weak_ptr<const void> actor_binding_lease()const noexcept;
    // Capture once when binding a borrowed Session reference and inspect the
    // token BEFORE dereferencing that reference. Survives same-object restore/
    // initialize; actor_binding_lease still governs actor/pose freshness.
    std::weak_ptr<const CombatSessionLifetime> lifetime_lease()const noexcept;
    // Neutral presentation subset of the same world. No character/combat state.
    // Initial load does not execute gameplay markers. Session.update is the sole
    // frame advancement after explicit clip selection. Registration order is
    // retained; the caller supplies authored object order.
    bool bind_object_visual(ObjectId,const AssetCatalog&,std::string& error);
    bool unbind_object_visual(ObjectId);
    CharacterVisual* retained_object_visual_borrow(ObjectId)noexcept;
    const CharacterVisual* retained_object_visual_borrow(ObjectId)const noexcept;
    bool bind_object_animation_services(ObjectId,CombatSessionObjectAnimationServices,std::string& error);
    bool play_object_clip(ObjectId,const std::string&,bool loop,bool& accepted,std::string& error);
    // After source state decode: select a stable saved pose silently. Does not
    // persist/replay a transient activation or infer a container state policy.
    bool restore_object_pose(ObjectId,const std::string&,bool loop,std::string& error);
    bool set_object_scene_flags(ObjectId,std::uint32_t clear,std::uint32_t set,std::string& error);
    // Object borrows/callbacks are invalidated on detach; owning features must
    // rebind the saved pose and callbacks after rebind_after_restore.
    using CombatPermissionProvider=std::function<bool(ActorId)>;
    void set_actor_combat_permission_provider(CombatPermissionProvider);
    // Optional source wrapper around this session's EXPLICIT DIAGNOSTIC attack
    // body. This is not a complete original Character/FSM/network backend.
    // Provider runs fresh per player/AI request; network query occurs only after
    // original wrapper admission. An enabled online speculative path is rejected.
    using DiagnosticControllerAdmissionProvider=std::function<bool(ActorId,DiagnosticControllerAdmissionFacts&,std::string&)>;
    using ControllerNetworkModeProvider=std::function<bool(ActorId,bool&,std::string&)>;
    void set_diagnostic_controller_admission_provider(DiagnosticControllerAdmissionProvider,
                                                       ControllerNetworkModeProvider);
    // Drops transient providers without interrupting poses or changing targets.
    void clear_diagnostic_controller_admission_provider();
    bool uses_diagnostic_controller_admission()const noexcept;
    void set_animation_notification_services(CombatSessionAnimationNotificationServices);
    void clear_animation_notification_services();
    // Explicitly stateless notifications whose gameplay effects are represented
    // in the saved World. Validator checks that representation against live
    // host resources. Detach drops callbacks; bind again on the new actor lease
    // before update. The legacy setter remains volatile for checkpoints.
    bool bind_reconstructible_animation_notifications(
        CombatSessionAnimationNotificationServices,
        std::function<bool(std::string&)> checkpoint_validator,std::string& error);
    // Only a genuine scheduler owner may emit numeric22..27; authored names
    // are event28 and never guessed as a numeric begin/end notification.
    bool deliver_original_animation_notification(ActorId,std::uint32_t event,
                                                 std::uintptr_t payload,std::string& error);
    const std::vector<CombatSessionAnimationDispatch>& original_animation_dispatches()const noexcept;
    // Refresh publishes targetable victim traits, interrupts forbidden attackers,
    // and clears selections. Call before external target queries after a change.
    bool refresh_actor_combat_permissions(std::string& error);
    bool set_actor_original_state(ActorId,std::int32_t state,std::string& error);
    bool select_actor_state_leaf(ActorId,const CombatSessionChoice&,double actor_rate,bool frozen,
                                CombatSessionStateAnimationServices,std::string& error);
    bool play_actor_state_sequence(ActorId,const OriginalAttackSelection&,
                                  CombatSessionStateAnimationServices,std::string& error);
    // External original skill/cinematic programs execute through the SAME
    // retained pose owner and session clock. Their clips must already be loaded
    // in this actor's visual. Preparation copies metadata; caller banks need
    // only outlive this call, while callback captures must outlive playback.
    // This does not construct skill/FSM owners or admit a gameplay command.
    bool play_actor_source_sequence(ActorId,const OriginalCombatVisualPlan&,
                                   const OriginalSequencePolicies&,const OriginalAttackSelection&,
                                   CombatSessionStateAnimationServices,std::string& error);
    // Generic skill/spell owner with explicit source state and checkpoint
    // policy, separate from unpersisted campaign/lifecycle services above.
    bool play_actor_source_sequence(ActorId,const OriginalCombatVisualPlan&,
                                   const OriginalSequencePolicies&,const OriginalAttackSelection&,
                                   CombatSessionStateAnimationServices,const CombatSessionSourceSequencePolicy&,
                                   std::string& error);
    // Call only after a real source transition has been admitted. Stale or
    // duplicate occurrences succeed with departed=false. Cancels the same
    // retained cursor and dispatches OnBlur/Post once, before incoming focus.
    bool cancel_actor_source_sequence(ActorId,std::weak_ptr<const void> binding_lease,
                                      std::uint64_t generation,std::int32_t next_state,
                                      bool& departed,std::string& error);
    bool freeze_actor_state_animation(ActorId,std::string& error);
    bool validate_lifecycle_checkpoint(std::string& error)const;
    // Host must clear its OriginalActorLifecycle borrowed records as well before
    // actor replacement. These lifecycle providers/state flags are not serialized.
    bool clear_lifecycle_services(std::string& error);
    using MotionHandler=std::function<bool(ActorState&,Vec3,bool,std::string&)>;
    // Synchronous host collision service; also retained across save rebinds.
    // Extraction depends on each visual's configured root-motion policy.
    void set_motion_handler(MotionHandler);
    // Opt-in actor position phase after this Session's animation/event pass.
    // Actual per-actor samples are collected in their order rather than moved
    // immediately. Called once per registered actor/frame, including no-sample
    // actors; the host composes physics import, source root policy and floor/
    // visual publication using the same owner. No FSM/pin policy is inferred.
    // Does not establish original global ObjectManager traversal chronology.
    // Consumer must not replace the Session or advance/select animation.
    using MotionPhaseHandler=std::function<bool(ActorState&,std::uint64_t frame,
        double dt,const std::vector<CombatSessionMotionSample>&,std::string&)>;
    bool set_motion_phase_handler(MotionPhaseHandler,std::string& error);
    bool clear_motion_phase_handler(std::string& error);
    // Essential synchronous effects at accepted state boundaries, distinct
    // from presentation/leaf observers. Blur precedes outgoing action mutation
    // and generic Post; source-state publication precedes focus_prefix, and
    // focus_suffix follows successful pose/action publication or completed
    // outgoing program retirement. The free-pose pass selects Idle afterwards.
    // Consumers own
    // body/navigation effects, not animation selection or another state graph.
    // Validator admits only effects represented/reconstructible from saved
    // World facts. Detach drops the callback; fresh bind is required afterwards.
    using ActorTransitionHandler=std::function<bool(const CombatSessionActorTransition&,std::string&)>;
    bool bind_reconstructible_actor_transition_handler(ActorTransitionHandler,
        std::function<bool(std::string&)> checkpoint_validator,std::string& error);
    // Detach invalidates the phase callback; its opt-in requirement survives.
    // Rebind a fresh handler after restore before the next update. Unprocessed
    // samples reject checkpoints/mode changes and never replay after detach.
    // Must run BEFORE any successful world actor replacement invalidates pointers.
    // Always call rebind afterwards, including when the restore itself failed.
    void detach_for_restore();
    // Validate while external resources still exist, release them with the
    // current actor lease valid, then detach. Failure keeps the reached prefix.
    // Teardown must not replace/destroy the Session or its World.
    bool detach_for_restore(const std::function<bool(std::string&)>& teardown,
                            std::string& error);
    bool rebind_after_restore(std::string& error);
    PlayableActorWorld* world() noexcept;
    const PlayableActorWorld* world() const noexcept;
    // Gameplay decisions share this session's actors, targeting, cooldowns and
    // authored attack/event playback. The provider must not replace the session
    // or advance animation; update() remains the sole frame advancement.
    using ActorDecisionProvider=std::function<bool(CombatSession&,double,std::string&)>;
    // Actual current frame serial/dt, before commands and animation events.
    // Timer owners tick here without replacing/suppressing the AI provider.
    // Do not replace the Session or reenter update/animation from this callback.
    void set_frame_begin_provider(ActorDecisionProvider);
    void clear_frame_begin_provider();
    void set_actor_decision_provider(ActorDecisionProvider);
    void clear_actor_decision_provider();
    // Explicit target-cycle command, shared by InputActions and host controls.
    // Returns the committed target (possibly invalid_actor_id), without a frame,
    // timer, animation, RNG or attack advance. PC cycling is an input adaptation.
    bool select_next_player_target(ActorId& selected,std::string& error);
    // A successful call means the command was processed; original admission,
    // range and cooldown rules may legitimately leave the actor idle.
    bool request_actor_attack(ActorId,ActorId target,double dt,std::string& error);
    // Same-world calculation -> basic health -> existing Injury/death runtime.
    // Repeated/stale occurrences return success with receipt.applied=false and
    // consume no RNG. Frame events include the receipt on the next update.
    // Admission, target query, cast animation and other result statuses remain
    // source feature responsibilities; this is not a complete F_ApplyResult.
    bool apply_source_result(const CombatSessionSourceHit&,DamageEvent&,std::string& error);
    // A reached source script may calculate again after its preceding hit was
    // lethal. Uses the SAME occurrence dedup/RNG/full result, without health,
    // reaction or damage-event publication. Duplicates report calculated=false.
    bool resolve_source_result_only(const CombatSessionSourceHit&,CombatSessionSourceCalculation&,std::string& error);
    const dh2::data::AiTables* original_ai_tables()const noexcept;
    std::int32_t original_actor_state(ActorId)const noexcept;
    // Scoped immediate borrow of the existing per-actor rotation turn field.
    // Reacquire after restore/reload; no second rotation owner is introduced.
    bool borrow_rotation_turn(ActorId,std::uint32_t*& turn_positive,std::string& error);
    struct AttackOwnerFacts {std::uint32_t flags528=0,heading_active=0;ActorId object_of_interest=0;std::int32_t object_of_interest_type=-1;};
    using AttackOwnerProvider=std::function<bool(ActorId,AttackOwnerFacts&,std::string&)>;
    void set_attack_owner_provider(AttackOwnerProvider);
    const dh2::character::AttackState64* source_attack_state(ActorId)const noexcept;
    const std::vector<CombatSessionComboBoundary>& combo_boundaries()const noexcept;
    ActorState* actor(ActorId) noexcept;
    const ActorState* actor(ActorId) const noexcept;
    ActorId player_id() const noexcept;
    bool owns_pose(ActorId) const noexcept;
    bool owns_population_pose(std::uint64_t stableId) const noexcept;
    const ActorState* selectedactor() const noexcept;
    const std::vector<DamageEvent>& events() const noexcept;
    const std::vector<PlayableCombatResolution>& resolutions() const noexcept;
    const std::vector<std::string>& logs() const noexcept;
    const OriginalAttackSequence* attack_sequence(ActorId) const noexcept;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
