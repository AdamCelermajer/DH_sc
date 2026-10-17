#pragma once

#include "character_runtime_factory_v1.hpp"
#include "character_ai_initialization.hpp"
#include "character_script_lifecycle.hpp"
#include "../game-data/ai.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_init_post_nonplayer_v1 {

namespace factory = character_runtime_factory_v1;

enum class Status : std::uint8_t {
    awaiting_scene_runtime,
    spawn_rejected,
    invalid_argument,
    service_unavailable,
    wrong_character_kind,
    provider_failed,
};

enum class Stage : std::uint8_t {
    not_started,
    check_spawn_probability,
    resolve_properties_id,
    load_base_properties,
    recalc_properties,
    get_model_name,
    read_base_scale,
    apply_owner_scale,
    game_object_init_post,
    meet_condition,
    load_save_mask_2,
    waiting_for_scene_runtime,
    spawn_rejected,
};

struct Services {
    void* context{};
    int (*is_player)(void*, const factory::Record&, bool*){};
    int (*check_spawn_probability)(void*, factory::Record&, bool*){};
    int (*safe_get_properties_id)(void*, factory::Record&, std::int16_t*){};
    int (*load_base_properties)(void*, void* properties_owner, std::int16_t){};
    int (*recalc_properties)(void*, void* properties_owner, bool force){};
    int (*get_model_name)(void*, factory::Record&, std::string*){};
    int (*read_base_scale)(void*, void* properties_owner, std::int32_t xyz[3]){};
    int (*apply_owner_scale)(void*, void* game_object_owner, const float xyz[3]){};
    int (*game_object_init_post)(void*, void* game_object_owner){};
    int (*meet_condition)(void*, void* game_object_owner, bool*){};
    int (*load_save_mask2)(void*, factory::Record&,
                           factory::gameplay_save::Result*, std::string&){};
};

struct Result {
    Stage stage{Stage::not_started};
    std::uintptr_t character_identity{};
    std::uintptr_t game_object_owner{};
    std::uintptr_t properties_owner{};
    std::int16_t property_id{-1};
    std::uint32_t provider_calls{};
    bool spawn_accepted{};
    bool condition_met{};
    bool prefix_complete{};
    bool full_init_post_complete{};
    bool save_mask2_loaded{};
};

// Executes the source-backed non-player prefix through Character::SG_Load(2).
// The original continues with AI/FX initialization, position and
// physics/visual setup. This adapter records the completed prefix but does not
// mark factory InitPost complete; full activation remains gated on those owners.
Status run_prefix(factory::Record&, const Services&, Result*,
                  std::string& error);

enum class AiBranchStatus : std::uint8_t {
    complete,
    already_complete,
    invalid_argument,
    service_unavailable,
    provider_failed,
};

struct AiBranchServices {
    void* context{};
    // The real Character::GetCharAIId provider, including source fallback.
    int (*get_char_ai_id)(void*, factory::Record&, std::int32_t*){};
    const data::AiTables* ai_tables{};
    // Forwarded to the already selected Character ScriptLifecycle kernel.
    // The adapter projects its transient state into the canonical CharAI
    // component before/after each callback; it creates no script/VM owner.
    character::ScriptLifecycleServices16 script{};
};

struct AiBranchResult {
    std::uintptr_t character_identity{};
    std::uintptr_t ai_identity{};
    std::int32_t ai_id{-1};
    std::uint32_t delayed_load{};
    std::uint32_t lifecycle_calls{};
    std::int32_t script_load_result{};
    bool delayed_init_requested{};
    bool script_load_called{};
    bool completed{};
};

// Exact non-player branch immediately after Character::SG_Load(2): resolve
// GetCharAIId and AITable delayed-load. Delayed rows set the Character's
// +0x3ec projection to one; non-delayed rows execute the existing CharAI
// LoadScriptProcess kernel against the canonical AI component. Completion
// stops before GrabAnimFX/RegisterCharacterFXTable/animation/sounds; those and
// scene/physics InitPost dependencies remain gated.
AiBranchStatus run_ai_branch(factory::Record&,
                             character_ai_initialization::State&,
                             const AiBranchServices&, AiBranchResult*,
                             std::string& error);

enum class EffectOperation : std::uint8_t {
    grab_anim_fx,
    register_character_fx_table,
    set_animation_set,
    init_sounds,
};

enum class EffectChainStatus : std::uint8_t {
    complete,
    already_complete,
    invalid_argument,
    service_unavailable,
    provider_failed,
};

struct EffectChainServices {
    void* context{};
    // Reads the canonical Character+0x1488 semantic field. This must be a
    // source-owned value, not a guessed zero or the renderer's FX projection.
    int (*read_anim_fx_base)(void*, factory::Record&, std::int32_t*){};
    // Source VisualFXManager::GrabAnimFX(base + AITable.self_fx, Character).
    int (*grab_anim_fx)(void*, std::uint32_t fx_id, std::uintptr_t character,
                        std::uintptr_t* handle){};
    int (*register_character_fx_table)(void*, factory::Record&){};
    // Receives the canonical factory Animator component, never a parallel one.
    int (*set_animation_set)(void*, void* animator){};
    int (*init_sounds)(void*, factory::Record&){};
};

struct EffectChainResult {
    std::uintptr_t character_identity{};
    std::uintptr_t animator_identity{};
    std::int32_t ai_id{-1};
    std::int32_t fx_id{};
    std::uintptr_t fx_handle{};
    std::uint32_t provider_calls{};
    std::uint8_t completed_operations{};
    bool source_fx_base_loaded{};
    bool complete{};
};

// Continues the verified non-player InitPost sequence after SG_Load(2) and
// the AITable delayed/script branch. The exact provider order is GrabAnimFX,
// RegisterCharacterFXTable, CharAnimator::SetAnimationSet, _InitSounds. The
// remaining source Level/scene/physics/HP stages stay outside this boundary.
EffectChainStatus run_effect_chain(factory::Record&,
        const data::AiTables&, const EffectChainServices&,
        EffectChainResult*, std::string& error);

} // namespace dh2::character_init_post_nonplayer_v1
