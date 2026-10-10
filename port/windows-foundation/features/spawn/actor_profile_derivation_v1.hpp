#pragma once
// P16 PROFILES: derive an actor profile, melee/state sequence bindings and the
// enemy combat policy from the original pydata tables (CharacterTable, AnimTable,
// CharAnim, AnimDict, character models, AI). No hand-authored XML is read.
//
// Consumer contract (what the port reads, not invented values):
//  - profile states: the seven states authored actor profiles carry;
//  - melee states: the nine states authored melee bindings carry.
// Every other original CharAnim state stays in the tables and is not published.
#include "../../actor_profiles.hpp"
#include "../../combat_session.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../playable_actor_world.hpp"
#include "../../../game-data/animation_tables.hpp"

#include <string>
#include <vector>

namespace dh::foundation::spawn {

struct ProfileDerivationTablesV1 {
    OriginalPropertyDatabase properties;     // CharacterTable rows and field names
    dh2::data::AnimationTables animations;   // AnimTable sequences and CharAnim state lists
    dh2::data::Dictionary clips;             // AnimDict: step animation id -> clip URI
    dh2::data::Dictionary models;            // character models: ModelFile index -> model URI
    dh2::data::AiTables ai;                  // AI rows (AI field) and names
};

// Loads the tables from `pydata_root` (for example "original-cache/data/pydata").
bool load_profile_derivation_tables_v1(const AssetCatalog& assets, const std::string& pydata_root,
                                       ProfileDerivationTablesV1& output, std::string& error);

const std::vector<std::string>& profile_states_v1();
const std::vector<std::string>& melee_states_v1();

// Profile: id/character = row name; propertyRow = row index; animationTable = AnimTable
// (CharacterTable AnimTable field); model = ModelFile dictionary value; template = first
// clip of CharAnim Template; all 224 raw CharacterTable words, in table order.
bool derive_actor_profile_v1(const ProfileDerivationTablesV1& tables, const std::string& row,
                             ActorProfile& output, std::string& error);

// Melee: the same actor fields plus the nine melee states. Sequences keep their
// AnimTable id/name/loop/type; steps keep index/animationId/redirect/speed/
// blendOut/moveGO; redirect steps nest the referenced sequence; leaf steps carry
// the AnimDict clip URI. Clip timing and markers are not derived here.
bool derive_melee_actor_v1(const ProfileDerivationTablesV1& tables, const std::string& row,
                           OriginalMeleeActor& output, std::string& error);

// Combat policy equal to the explicit --combat-* choices recorded for authored
// enemies: idle Idle/0/{0}, reaction Injured/0/{0}, death Died/0/{0}, attack
// Attack/0 group {0}, source damage marker attack_mainhand, retained phase clock,
// motion root auto. The attack group choice is the authored default, not an IDA
// selection rule (see PROFILES-report.md).
bool derive_enemy_combat_policy_v1(const ProfileDerivationTablesV1& tables, const std::string& row,
                                   CombatSessionProfile& output, std::string& error);

} // namespace dh::foundation::spawn
