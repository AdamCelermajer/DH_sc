#pragma once
// w3-pose: the CSStunned / CSScared state pose through the general Session source-state path.
//
// Original (IDA libDungeonHunter2.so pseudocode-all.c):
//   CharStateMachine::SM_SetStunState 0x3c5ffc / SM_SetScareState 0x3c6144 read the CharAnimTable dword +140 (Stunned) / +124 (Scared)
//   of the actor's own row, add the actor's AnimStance when the AnimStancedAnim constant has bit 0x200 (stun) / 0x100 (scare), store it in
//   the state machine and enter state 9 / 8 (a forced _SetState when the 5th argument is true, else RaiseStateEvent 50012 / 50013).
//   CSStunned::OnFocus 0x3c3cc0 / CSScared::OnFocus 0x3c45c4: flags dword +0x520 = 8706 / 8768, CharAnimator::ANIM_Set(row +140 / +124
//   [+ stance]). CSStunned::OnUpdate 0x3c549c returns to Idle (SM_SetIdleState) when the stun bit (sm+44 & 2) is clear.
// The animation table field indices (alphabetical schema) are Scared = 29 and Stunned = 32 (dword +124 / +140: fields after Interact (15)
// and Spells (31) are array-sized, so dword = field + 2 / + 3).
#include "../../combat_session.hpp"
#include "../../../game-data/animation_tables.hpp"

namespace dh::foundation::combat {

// How the Session plays one status pose (derived from the source sequence hierarchy, see build_session_status_animation_bank_v1).
struct SessionStatusPoseProgramV1 {
    bool has_entry = false;                       // play `entry_group_path` once first (stun: pre_stunned)
    std::vector<std::size_t> entry_group_path;
    bool has_loop = false;                        // then seed the infinite leaf (stun: stunned loop; scare: the walk leaf)
    std::vector<std::size_t> loop_leaf_path;
    bool loop_only = false;                       // no entry clip: the loop leaf plays at once
};

struct SessionStatusAnimationBankV1 {
    OriginalCombatVisualPlan plan;
    OriginalSequencePolicies policies;
    std::string stun_state = "Stunned";
    std::string scare_state = "Scared";
    std::int32_t stun_sequence = -1;   // resolved AnimTable root (-1: the row has no stun clip)
    std::int32_t scare_sequence = -1;
    std::int32_t stance = 0;
    std::uint32_t stanced_animation_mask = 0;
    bool stun_looping = false;         // sequence Loop != 0 (diagnostic)
    bool scare_looping = false;
    std::string stun_description, scare_description; // diagnostic: the source hierarchy (id:type/loop[steps])
    SessionStatusPoseProgramV1 stun_program, scare_program;
};

// Resolves the actor's own CharAnimTable Stunned (field 32, stance bit 0x200) and Scared (field 29, stance bit 0x100) roots.
// A row without either root fails with an error; one absent root stays -1 (the original state setter plays no clip).
bool build_session_status_animation_bank_v1(const AssetCatalog&, const dh2::data::AnimationTables&, const dh2::data::Dictionary&,
    std::int32_t target_animation_table, std::int32_t stance, std::uint32_t stanced_animation_mask,
    const CharacterVisualConfig&, const std::string& role, SessionStatusAnimationBankV1&, std::string& error);

// Plays the stun (stun=true) or scare pose on the actor through CombatSession::play_actor_source_sequence (original state 9 / 8).
// `forced` = the original's immediate _SetState path. `started` is false when the bank has no clip for the kind (a source no-op).
bool play_session_status_pose_v1(CombatSession&, ActorId actor, bool stun, bool forced, std::uint64_t generation,
    const SessionStatusAnimationBankV1&, CombatSessionStateAnimationServices, bool& started, std::string& error);

} // namespace dh::foundation::combat
