#pragma once

#include "../../character_state.hpp"
#include "../../combat_session.hpp"
#include "../../original_attack_sequence.hpp"
#include "../../../game-data/data.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation::generic_skills {

// A read-only request resolved from the same CharacterState and original
// CharacterTable/SkillTables. The animation root is SkillTable.Anim, not a
// guessed class-to-clip mapping.
struct SkillVisualRequestV1 {
    std::string character_state_id;
    std::string class_id;
    int class_skill_position = -1;
    int active_skill_list_id = -1;
    int skill_table_id = -1;
    int saved_skill_row = -1;
    std::uint32_t saved_rank = 0;
    std::int32_t animation_sequence_id = -1;
    std::string source_skill_name;
    std::string source_script;
};

// A successful source fact must come from the original usable/cast provider
// for this same state and skill. `unknown` is deliberately distinct from a
// normal source rejection; neither is converted into a generic mana rule.
enum class SkillCastAdmissionStatusV1 : std::uint8_t {
    unknown,
    rejected,
    accepted
};

struct SkillCastAdmissionFactV1 {
    SkillCastAdmissionStatusV1 status = SkillCastAdmissionStatusV1::unknown;
    std::string source;
    std::string reason;
    ActorId actor = invalid_actor_id;
    std::string character_state_id;
    std::string class_id;
    int active_skill_list_id = -1;
    int class_skill_position = -1;
    int skill_table_id = -1;
    std::uint32_t saved_rank = 0;
};

enum class SkillCastAnimationOutcomeV1 : std::uint8_t {
    source_admission_unknown,
    source_rejected,
    source_admission_accepted,
    playback_started
};

// Source Lua/cast usability, including mana and current FSM conditions, is not
// represented by generic CharacterState/SkillTables. This type resolves only
// the exact saved skill row and source animation root. Rank must be known and
// positive and the source Character level requirement must pass.
bool resolve_skill_visual_request_v1(
    const CharacterState&, const dh2::data::CharacterTable&,
    dh2::data::SkillTables::Borrow, int class_skill_position,
    SkillVisualRequestV1&, std::string& error);

// Classify a source callback fact without manufacturing a cast gate. This
// pure helper is usable even when no CombatSession is linked into a caller.
bool resolve_skill_cast_admission_v1(
    const SkillCastAdmissionFactV1&, SkillCastAnimationOutcomeV1&,
    std::string& error);

bool validate_skill_cast_admission_binding_v1(
    const SkillVisualRequestV1&, const SkillCastAdmissionFactV1&, ActorId,
    std::string& error);

// Presentation-only request. It executes the given authored source sequence
// on the same actor/CombatSession; it does not admit a gameplay cast or mutate
// mana, health, rank, slots, or CharacterState.
bool play_skill_visual_request_v1(
    const SkillVisualRequestV1&, ActorId, CombatSession&,
    const OriginalCombatVisualPlan&, const OriginalSequencePolicies&,
    const OriginalAttackSelection&, CombatSessionStateAnimationServices,
    std::string& error);

// A cast-animation request is fail-closed unless the caller supplies a
// source-backed admission fact. On source rejection/unknown it returns true
// with a non-started outcome; only an accepted fact starts presentation.
// Playback still does not claim source damage, mana mutation, or FSM casting.
bool request_skill_cast_animation_v1(
    const SkillVisualRequestV1&, const SkillCastAdmissionFactV1&,
    ActorId, CombatSession&, const OriginalCombatVisualPlan&,
    const OriginalSequencePolicies&, const OriginalAttackSelection&,
    CombatSessionStateAnimationServices, SkillCastAnimationOutcomeV1&,
    std::string& error);

} // namespace dh::foundation::generic_skills
