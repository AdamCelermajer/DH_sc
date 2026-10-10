#pragma once

#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/items.hpp"

#include <functional>

namespace dh::foundation::equipment_menu {

struct RuntimePlayerLocomotionConstantsV1 {
    std::int32_t stanced_list_mask{};
    std::int32_t stance_count{};
    std::int32_t idle_stanced_bit{};
    std::int32_t walk_stanced_bit{};
    std::int32_t run_stanced_bit{};
};

using RuntimePlayerLocomotionConstantLookupV1 = std::function<bool(
    const char* group, const char* key, std::int32_t& value, std::string& error)>;

bool load_runtime_player_locomotion_constants_v1(
    const RuntimePlayerLocomotionConstantLookupV1&, RuntimePlayerLocomotionConstantsV1&,
    std::string& error);

struct RuntimePlayerLocomotionStepV1 {
    std::int32_t animation_id{-1};
    std::int32_t blend_out{};
    std::int32_t camera{-1};
    std::int32_t effect{-1};
    std::int32_t redirect{};
    std::int32_t sound{-1};
    float speed{1.0f};
    bool anchor_fx{}, camera_direction{}, move_go{}, swoosh{};
    std::vector<std::int32_t> random_camera;
    std::string clip_alias, clip_uri, redirected_sequence_alias;
};

struct RuntimePlayerLocomotionSequenceV1 {
    std::int32_t sequence_id{-1};
    std::int32_t loop{}, type{};
    std::string alias;
    std::vector<RuntimePlayerLocomotionStepV1> steps;
};

struct RuntimePlayerLocomotionStateV1 {
    std::string name;
    std::int32_t base_sequence{-1}, selected_sequence{-1};
    bool stance_variants_enabled{};
    std::vector<RuntimePlayerLocomotionSequenceV1> reachable_sequences;
};

struct RuntimePlayerLocomotionV1 {
    std::int32_t animation_table{-1}, stance{};
    std::vector<RuntimePlayerLocomotionStateV1> states;
};

// Reads true source ItemTable rows for the currently equipped main/offhand IDs,
// executes the existing original equipment-query and stance kernels, and
// projects the source Idle/Walk/Run sequence graph. It does not start or tick
// an Animator, mutate Gear, choose random steps, or advance clip time.
bool resolve_runtime_player_locomotion_v1(
    std::int32_t animation_table, const dh2::data::ItemTable& items,
    std::int32_t main_hand_item_id, std::int32_t off_hand_item_id,
    std::int32_t character_flag_1324,
    const RuntimePlayerLocomotionConstantsV1& constants,
    const dh2::data::AnimationTables& animations,
    const dh2::data::Dictionary& clip_dictionary,
    RuntimePlayerLocomotionV1& output, std::string& error);

} // namespace dh::foundation::equipment_menu
