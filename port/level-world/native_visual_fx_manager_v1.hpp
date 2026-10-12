#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::native_visual_fx_manager_v1 {

struct Bytes { const std::uint8_t* data{}; std::size_t size{}; };

struct EffectStep {
    std::int32_t effect_dict_id{-1};
    bool force_cancel{};
    std::int32_t loop{};
    bool orient_once{};
    bool orient_with_anchor{};
    std::int32_t play_time{-1};
    std::int32_t pool_size{};
    std::int32_t redirect{};
    bool scale_with_anchor{true};
    bool self_illum{true};
    float speed{1.0f};
    std::string sub_object;
};

struct EffectSequence {
    std::uint32_t row{};
    std::string name;
    bool template_flag{};
    std::int32_t loop{};
    std::vector<EffectStep> steps;
    std::int32_t type{};
};

struct CharacterEffect {
    std::uint32_t row{};
    std::string name;
    std::int32_t blood_death_effect{-1};
    std::int32_t blood_effect{-1};
    std::int32_t footprint_effect{-1};
    std::int32_t swoosh_effect{-1};
    bool trigger_floor_fx{};
};

// FootstepEffectTable (effects_pyarray.bin's third serialized table). The
// source CharAI reads Effect and Floortype for material-specific floor FX;
// RunSound and WalkSound are retained because they are part of the source row.
struct FootstepEffect {
    std::uint32_t row{};
    std::string name;
    std::int32_t effect{-1};
    std::string floor_type;
    std::vector<std::int32_t> run_sound;
    std::vector<std::int32_t> walk_sound;
};

struct Catalog {
    std::vector<EffectSequence> sequences;
    std::vector<CharacterEffect> character_effects;
    std::vector<FootstepEffect> footstep_effects;
    std::vector<std::string> effect_names;
    std::vector<std::string> effect_paths;
    std::vector<std::string> dictionary_names;
    // The source AnimatedEffectTable finishes at byte 12700; the original
    // file then serializes CharEffectTable and FootstepEffectTable rows.
    std::size_t effect_array_consumed{};
    std::size_t effect_array_suffix{};
    // CharEffectTable and FootstepEffectTable follow AnimatedEffectTable in
    // effects_pyarray.bin. These offsets retain the parsed source boundaries.
    std::size_t character_effect_table_consumed{};
    std::size_t remaining_effect_array_suffix{};
};

// Runtime values assembled by source GetAnimFXData for PlayAnimFXSet. The
// tokens point to stable manager-owned AnimFXSetData projections and remain
// valid until Owner::Flush, matching the source set-data lists.
struct AnimFXSetPlayback {
    std::int32_t effect_table_id{-1};
    std::size_t step_index{};
    std::array<float, 3> position{};
    std::array<float, 3> origin{};
    bool orient_once{};
    bool orient_with_anchor{};
    bool self_illum{true};
    bool scale_with_anchor{true};
    float speed{1.0f};
    std::int32_t play_time{-1};
    std::int32_t loop_count{};
    void* source_game_object{};
    const void* set_data_token{};
    const void* parent_set_data_token{};
};

// Keep the renderer's missing optional EffectDict asset outcome distinct from
// malformed resource/backend failures. The manager maps source_no_launch to
// the source null-handle/no-effect path without allocating a pool slot.
enum class CreateResult : std::uint8_t { created, source_no_launch, failed };

// True only for the renderer's exact missing actor asset diagnostic for this
// selected EffectDict path; texture misses and other path failures do not match.
bool exact_missing_effect_dict_asset(std::string_view diagnostic,
                                     std::string_view source_path) noexcept;

bool load_catalog(Bytes effect_array, Bytes effect_names, Bytes effect_fields,
                  Bytes effect_dictionary, Bytes dictionary_names,
                  Catalog& output, std::string& error);

struct Backend {
    void* context{};
    // A found asset produces a loaded instance. A missing optional source
    // EffectDict is source_no_launch and produces no instance; malformed BDAE
    // or backend problems are failed.
    CreateResult (*create)(void*, std::int32_t effect_dict_id,
                           std::string_view source_path, void** instance,
                           std::string& error){};
    void (*destroy)(void*, void* instance){};
    // Starts one source step on the already loaded instance. Owner selects
    // and allocates the EffectDict pool entry for step_index.
    bool (*start_step)(void*, void* instance,
                       const EffectSequence& sequence,
                       std::size_t step_index,
                       bool grab_entry,
                       void* source_game_object,
                       std::string& error){};
    // DropAnimatedFX's reset path, including source/object detachment and
    // Irrlicht synchronization. This is distinct from objective-marker hide.
    bool (*reset_for_pool)(void*, void* instance, std::string& error){};
    bool (*bind_game_object)(void*, void* instance, void* object,
                             std::string& error){};
    bool (*bind_animator_character)(void*, void* instance, void* character,
                                    std::string& error){};
    bool (*sync_irr_data)(void*, void* instance, bool force,
                          std::string& error){};
    bool (*set_visible)(void*, void* instance, bool visible,
                        std::string& error){};
    bool (*resume_animator)(void*, void* instance,
                            std::string& error){};
    // Advances one AnimatedFX and reports source end-of-loop callback and
    // later source-hidden boundaries separately. Owner advances the sequence
    // and acquires any following step from its shared per-EffectDict pools.
    bool (*update)(void*, void* instance, const EffectSequence& sequence,
                   std::size_t step_index,
                   bool& end_of_loop_callback_due,
                   bool& source_hidden,
                   std::string& error){};
    // Renderer-side submission for an active instance. A backend that uses
    // the shared scene renderer may register its scene node there and leave
    // this callback empty; otherwise the manager's render phase calls it.
    bool (*render)(void*, void* instance, std::string& error){};
    // Set only when the concrete backend registers its AnimatedFX scene nodes
    // with the shared renderer and submits them outside Owner::Render.
    bool draw_registered_by_scene{};
    // Source Random::GetRandom borrows the process stream. The manager calls
    // this only after _GetAnimFX has produced an instance for a type-2 set.
    // The concrete provider must use the canonical source RNG owner.
    bool (*random_below)(void*, std::uint32_t bound, std::uint32_t& value,
                         std::string& error){};
    // PlayAnimFXSet leaves use the same AnimatedFX pool but distinct
    // SetAnimFX parameters and a source callback context. This callback is
    // appended so existing backend aggregate initialization stays compatible.
    bool (*start_anim_fx_set_step)(void*, void* instance,
                                   const EffectSequence& sequence,
                                   const AnimFXSetPlayback& playback,
                                   std::string& error){};
    // CharProperties::PROPS_AddBuff obtains AnimatedFX::GetAnimController
    // from the same pooled instance and invokes its numeric PlayClip method.
    // A missing controller is a source null result; controller tokens remain
    // owned by the corresponding AnimatedFX allocation.
    bool (*get_anim_controller)(void*, void* instance, void** controller,
                                std::string& error){};
    // Source AnimController::PlayClip(index, enabled, 0, 0). The trailing
    // source arguments are fixed by PROPS_AddBuff's callsite.
    bool (*play_anim_controller_clip)(void*, void* controller,
                                      std::uint32_t clip_index, bool enabled,
                                      std::string& error){};
    // Source Character::Update binds the target-circle AnimatedFX to an
    // arbitrary GameObject projection. Appended to preserve existing backend
    // aggregate initialization and never reinterpret the object as Character.
    bool (*bind_target_object)(void*, void* instance, void* object,
                               std::string& error){};
};

enum class Status : std::uint8_t {
    ok, invalid_request, catalog_missing, backend_missing, load_failed,
    start_failed, pool_exhausted, stale_instance, lifecycle_failed,
    allocation_failed, unsupported_sequence,
    source_no_launch // internal start result; public source calls normalize to null/ok
};

struct PoolSnapshot {
    std::int32_t effect_dict_id{-1};
    std::size_t allocated{};
    std::size_t active{};
    std::size_t free{};
    std::size_t source_limit{6};
};

// One level-owned projection of the source VisualFXManager. It owns one
// AnimatedFX allocation pool per EffectDict row (six maximum), while the
// engine backend owns the actual scene/animation objects. Objective records
// retain the returned opaque AnimatedFX* at source Objective+0x28. Hiding an
// objective marker does not release that pointer; DropAnimatedFX or manager
// flush does.
class Owner final {
public:
    explicit Owner(const Catalog* catalog = nullptr) noexcept;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    ~Owner();

    void bind_catalog(const Catalog* catalog) noexcept { catalog_ = catalog; }
    void bind_backend(Backend backend) noexcept { backend_ = backend; }
    Status GrabAnimFX(std::int32_t effect_table_id, void* source_game_object,
                      void** output, std::string& error);
    // Source RegisterFXSetToLoad expands set steps recursively. Direct
    // EffectDict registrations are retained on this same manager for callers
    // that need the source registration view; instance pools remain lazy and
    // are still allocated only through GrabAnimFX/PlayAnimFXStep.
    Status RegisterFXSetToLoad(std::int32_t effect_table_id,
                               std::string& error);
    // Source marker events name an AnimatedEffectTable set. A miss is a
    // source no-op (`matched=false`); a valid row can still produce no FX
    // when the source EffectDict pool is exhausted (`launched=false`).
    Status PlayAnimFXSetByName(std::string_view source_name,
                               const std::array<float, 3>& position,
                               void* source_game_object,
                               bool& matched, bool& launched,
                               std::string& error);
    Status PlayAnimFXSet(std::int32_t effect_table_id,
                         const std::array<float, 3>& position,
                         void* source_game_object,
                         bool& matched, bool& launched,
                         std::string& error);
    // Source PlayAnimFXSet(id, const GameObject*, data) (0x495f04), the
    // overload GameObject::_PlayFX calls for `PlayFX(id)` / `obj:PlayFX(id)`.
    // It forwards the Point3D overload with Vec3f_Origin and the GameObject,
    // and VisualFXManager::PlayAnimFX stores that GameObject at AnimatedFX+0x28
    // and syncs it, so every launched leaf follows the object. The renderer
    // supplies the object as the typed Character projection (BindCharacter
    // semantics) or, for a non-Character GameObject, `target_object`
    // (BindTargetObject); exactly one of them must be non-null. Nested sets
    // keep the binding.
    Status PlayAnimFXSetAttached(std::int32_t effect_table_id,
                                 void* source_game_object,
                                 void* target_character, void* target_object,
                                 bool& matched, bool& launched,
                                 std::string& error);
    const std::vector<std::int32_t>& registered_effect_sets() const noexcept {
        return registered_effect_sets_;
    }
    const std::vector<std::int32_t>& registered_effect_dicts() const noexcept {
        return registered_effect_dicts_;
    }
    Status DropAnimatedFX(void** instance, std::string& error);
    // Source VisualFXManager::DropAnimatedFXSetByID (0x494660) for the
    // StopEffect(set) script command: releases the active AnimatedFX that the
    // set started in its first step's EffectDict pool. No match is a no-op.
    Status DropAnimatedFXSetByID(std::int32_t effect_table_id, std::string& error);
    // Source FX methods accept any still allocated AnimatedFX, even after the
    // manager returned it to a free pool. DropAnimatedFX separately requires
    // active membership; these direct source object operations do not.
    Status BindCharacter(void* instance, void* character, std::string& error);
    Status BindTargetObject(void* instance, void* object, std::string& error);
    Status BindAnimatorCharacter(void* instance, void* character,
                                 std::string& error);
    Status SyncIrrData(void* instance, bool force, std::string& error);
    Status SetVisible(void* instance, bool visible, std::string& error);
    Status ResumeAnimator(void* instance, std::string& error);
    Status GetAnimController(void* instance, void** controller,
                             std::string& error);
    Status PlayAnimControllerClip(void* controller, std::uint32_t clip_index,
                                  bool enabled, std::string& error);
    Status Update(std::string& error);
    Status Render(std::string& error);
    // Objective+0x28 is the single persistent handle slot. Install invokes
    // source RemoveObjectiveMarker on any prior handle (detach/hide but keep
    // it owned), then overwrites the slot with the new GrabAnimFX result before
    // applying the source marker attachment sequence. Source pool exhaustion
    // or an unavailable optional EffectDict yields a null Grab and clears
    // Objective+0x28; malformed resource/backend errors remain visible.
    // Remove deliberately leaves the retained slot untouched.
    Status InstallObjectiveMarker(std::uintptr_t* objective_fx_slot,
                                  std::int32_t effect_table_id,
                                  void* target_character,
                                  void* source_game_object,
                                  std::string& error);
    Status RemoveObjectiveMarker(std::uintptr_t* objective_fx_slot,
                                 void* target_character,
                                 std::string& error);
    void Flush() noexcept;
    PoolSnapshot pool(std::int32_t effect_dict_id) const noexcept;
    const EffectSequence* sequence(std::int32_t effect_table_id) const noexcept;
    const FootstepEffect* footstep_effect(std::string_view floor_type) const noexcept;

private:
    struct SetData;
    struct Slot {
        void* instance{};
        std::int32_t effect_dict_id{-1};
        std::int32_t effect_table_id{-1};
        std::size_t step_index{};
        std::int32_t sequence_loops_remaining{};
        void* source_game_object{};
        void* target_character{};
        void* target_object{};
        void* animator_character{};
        void* anim_controller{};
        const SetData* set_data{};
        bool active{};
    };
    Slot* find(void* instance) noexcept;
    const Slot* find(void* instance) const noexcept;
    Status start_step_instance(std::int32_t effect_table_id,
                               std::size_t pool_step_index,
                               std::size_t playback_step_index,
                               std::int32_t sequence_loops_remaining,
                               bool grab_entry,
                               bool random_set_entry,
                               void* source_game_object,
                               void* target_character,
                               void* target_object,
                               void* animator_character,
                               void** output,
                               std::string& error,
                               const SetData* set_data = nullptr);
    Status advance_sequence_step(void* instance, std::string& error);
    Status begin_anim_fx_set(std::int32_t effect_table_id,
                             const std::array<float, 3>& position,
                             const std::array<float, 3>& origin,
                             void* source_game_object,
                             SetData* parent, std::size_t depth,
                             bool& launched, std::string& error,
                             void* target_character = nullptr,
                             void* target_object = nullptr);
    Status dispatch_anim_fx_set_step(SetData& set_data, bool& launched,
                                     std::string& error);
    Status handle_anim_fx_set_end(const SetData* set_data,
                                  std::string& error);
    Status advance_anim_fx_set(SetData& set_data, std::string& error);
    Status complete_anim_fx_set(SetData& set_data, std::string& error);
    Status register_fx_set_recursive(std::int32_t effect_table_id,
                                     std::vector<std::int32_t>& recursion_stack,
                                     std::string& error);
    const Catalog* catalog_{};
    Backend backend_{};
    std::vector<Slot> slots_;
    std::vector<std::unique_ptr<SetData>> set_data_;
    std::vector<std::int32_t> registered_effect_sets_;
    std::vector<std::int32_t> registered_effect_dicts_;
};

} // namespace dh2::native_visual_fx_manager_v1
