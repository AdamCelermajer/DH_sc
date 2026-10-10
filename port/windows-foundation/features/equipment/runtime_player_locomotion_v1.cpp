#include "runtime_player_locomotion_v1.hpp"

#include "../../../level-world/player_equipment_queries_v1.hpp"
#include "../../../level-world/character_stance.hpp"

#include <algorithm>
#include <set>
#include <stdexcept>

namespace dh::foundation::equipment_menu {
namespace {
using namespace dh2::data;

std::string required_error(const std::string& error, const char* key) {
    return error.empty() ? std::string("Required same-owner locomotion constant ") + key : error;
}

bool has_single_bit(std::int32_t value) {
    if (value <= 0) return false;
    const auto bits = static_cast<std::uint32_t>(value);
    return (bits & (bits - 1u)) == 0;
}

bool reachable_sequences(std::int32_t root, const AnimationTables& animations,
        const Dictionary& clips, RuntimePlayerLocomotionStateV1& state,
        std::string& error) {
    std::set<std::int32_t> visited, active;
    std::function<bool(std::int32_t, unsigned)> visit = [&](std::int32_t id, unsigned depth) {
        if (depth >= 3 || id < 0 || static_cast<std::size_t>(id) >= animations.sequences.size() ||
            static_cast<std::size_t>(id) >= animations.sequence_names.size()) {
            error = "Source locomotion sequence redirect exceeds the original three-layer domain";
            return false;
        }
        if (active.count(id)) {
            error = "Source locomotion sequence contains a redirect cycle";
            return false;
        }
        if (visited.count(id)) return true;
        active.insert(id);
        const auto& sequence = animations.sequences[static_cast<std::size_t>(id)];
        if (sequence.steps.empty()) {
            error = "Source locomotion sequence has no authored phases";
            return false;
        }
        RuntimePlayerLocomotionSequenceV1 projected;
        projected.sequence_id = id;
        projected.alias = animations.sequence_names[static_cast<std::size_t>(id)];
        projected.loop = sequence.loop;
        projected.type = sequence.type;
        projected.steps.reserve(sequence.steps.size());
        for (const auto& source : sequence.steps) {
            RuntimePlayerLocomotionStepV1 step;
            step.animation_id = source.anim;
            step.blend_out = source.blend_out;
            step.camera = source.cam;
            step.effect = source.fx;
            step.redirect = source.redir;
            step.sound = source.sound;
            step.speed = source.speed;
            step.anchor_fx = source.anchor_fx;
            step.camera_direction = source.cam_dir;
            step.move_go = source.move_go;
            step.swoosh = source.swoosh;
            step.random_camera = source.random_cam;
            if (source.redir == 1) {
                if (source.anim < 0 || static_cast<std::size_t>(source.anim) >= animations.sequences.size() ||
                    static_cast<std::size_t>(source.anim) >= animations.sequence_names.size()) {
                    error = "Source locomotion redirect sequence is outside the loaded AnimTable";
                    return false;
                }
                step.redirected_sequence_alias = animations.sequence_names[static_cast<std::size_t>(source.anim)];
                if (!visit(source.anim, depth + 1)) return false;
            } else if (source.redir == 0) {
                if (source.anim < 0 || static_cast<std::size_t>(source.anim) >= clips.values.size() ||
                    static_cast<std::size_t>(source.anim) >= clips.names.size()) {
                    error = "Source locomotion phase references a missing clip dictionary entry";
                    return false;
                }
                step.clip_alias = clips.names[static_cast<std::size_t>(source.anim)];
                step.clip_uri = clips.values[static_cast<std::size_t>(source.anim)];
                if (step.clip_uri.empty()) {
                    error = "Source locomotion clip dictionary URI is empty";
                    return false;
                }
            } else {
                error = "Source locomotion phase has an unsupported redirection value";
                return false;
            }
            projected.steps.push_back(std::move(step));
        }
        active.erase(id);
        visited.insert(id);
        state.reachable_sequences.push_back(std::move(projected));
        return true;
    };
    return visit(root, 0);
}

} // namespace

bool load_runtime_player_locomotion_constants_v1(
        const RuntimePlayerLocomotionConstantLookupV1& lookup,
        RuntimePlayerLocomotionConstantsV1& output, std::string& error) {
    error.clear();
    if (!lookup) {
        error = "Required same-owner PyDataConstants lookup for player locomotion";
        return false;
    }
    RuntimePlayerLocomotionConstantsV1 next;
    auto get = [&](const char* group, const char* key, std::int32_t& value) {
        std::string provider_error;
        if (!lookup(group, key, value, provider_error)) {
            error = required_error(provider_error, key);
            return false;
        }
        return true;
    };
    if (!get("AnimStancedAnim", "SL__LIST_IPHONE", next.stanced_list_mask) ||
        !get("AnimStances", "COUNT_IPHONE", next.stance_count) ||
        !get("AnimStancedAnim", "SL_IDLE", next.idle_stanced_bit) ||
        !get("AnimStancedAnim", "SL_WALK", next.walk_stanced_bit) ||
        !get("AnimStancedAnim", "SL_RUN", next.run_stanced_bit)) return false;
    if (next.stanced_list_mask <= 0 || next.stance_count <= 0 || next.stance_count > 32 ||
        !has_single_bit(next.idle_stanced_bit) || !has_single_bit(next.walk_stanced_bit) ||
        !has_single_bit(next.run_stanced_bit)) {
        error = "Original player locomotion stance constants are outside the source domain";
        return false;
    }
    output = next;
    error.clear();
    return true;
}

bool resolve_runtime_player_locomotion_v1(
        std::int32_t animation_table, const ItemTable& items,
        std::int32_t main_hand_item_id, std::int32_t off_hand_item_id,
        std::int32_t character_flag_1324,
        const RuntimePlayerLocomotionConstantsV1& constants,
        const AnimationTables& animations, const Dictionary& clip_dictionary,
        RuntimePlayerLocomotionV1& output, std::string& error) {
    error.clear();
    try {
        if (animation_table < 0 || static_cast<std::size_t>(animation_table) >= animations.characters.size() ||
            main_hand_item_id < -1 || off_hand_item_id < -1 || constants.stance_count <= 0 ||
            constants.stance_count > 32 || constants.stanced_list_mask <= 0 ||
            !has_single_bit(constants.idle_stanced_bit) || !has_single_bit(constants.walk_stanced_bit) ||
            !has_single_bit(constants.run_stanced_bit))
            throw std::runtime_error("Required source player locomotion table, item or constant facts");

        const Item* main = main_hand_item_id == -1 ? nullptr : item(items, main_hand_item_id);
        const Item* off = off_hand_item_id == -1 ? nullptr : item(items, off_hand_item_id);
        if ((main_hand_item_id >= 0 && !main) || (off_hand_item_id >= 0 && !off))
            throw std::runtime_error("Player locomotion equipment ID is outside the actual ItemTable");

        dh2::player::EquipmentQueries12V1 queries;
        if (dh2_equipment_queries_v1(&queries, main ? &main->record : nullptr,
                                     off ? &off->record : nullptr, character_flag_1324) != 0)
            throw std::runtime_error("Original player equipment queries rejected source ItemTable rows");
        dh2::character::StanceFacts16 facts;
        facts.count = constants.stance_count;
        facts.predicates = dh2::character::stance_is_player;
        if (queries.flags & dh2::player::query_main) facts.predicates |= dh2::character::stance_has_main_hand;
        if (queries.flags & dh2::player::query_staff) facts.predicates |= dh2::character::stance_has_staff;
        if (queries.flags & dh2::player::query_bow) facts.predicates |= dh2::character::stance_has_bow;
        if (queries.flags & dh2::player::query_dual) facts.predicates |= dh2::character::stance_dual_wielding;
        if (queries.flags & dh2::player::query_two_effective) facts.predicates |= dh2::character::stance_has_two_hander;
        std::int32_t stance{};
        if (dh2_character_anim_stance(&stance, &facts) != 1)
            throw std::runtime_error("Original GetAnimStance rejected actual ItemTable/constants facts");

        RuntimePlayerLocomotionV1 next;
        next.animation_table = animation_table;
        next.stance = stance;
        const struct StateQuery { const char* name; std::int32_t bit; } states[] = {
            {"Idle", constants.idle_stanced_bit},
            {"Walk", constants.walk_stanced_bit},
            {"Run", constants.run_stanced_bit}
        };
        for (const auto& source_state : states) {
            const auto* base = animation_state(animations, animation_table, source_state.name);
            if (!base) throw std::runtime_error(std::string("Source CharAnimTable lacks ") + source_state.name);
            const auto base_offset = base - animations.sequences.data();
            if (base_offset < 0 || static_cast<std::size_t>(base_offset) >= animations.sequences.size())
                throw std::runtime_error("Source CharAnimTable base sequence escaped the loaded AnimTable");
            RuntimePlayerLocomotionStateV1 state;
            state.name = source_state.name;
            state.base_sequence = static_cast<std::int32_t>(base_offset);
            state.stance_variants_enabled =
                (static_cast<std::uint32_t>(constants.stanced_list_mask) &
                 static_cast<std::uint32_t>(source_state.bit)) != 0;
            state.selected_sequence = state.base_sequence +
                (state.stance_variants_enabled ? stance : 0);
            if (state.selected_sequence < 0 ||
                static_cast<std::size_t>(state.selected_sequence) >= animations.sequences.size())
                throw std::runtime_error(std::string("Source ") + source_state.name +
                                         " stance sequence is outside the loaded AnimTable");
            if (!reachable_sequences(state.selected_sequence, animations, clip_dictionary, state, error))
                throw std::runtime_error(error.empty() ? "Source locomotion sequence projection failed" : error);
            next.states.push_back(std::move(state));
        }
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

} // namespace dh::foundation::equipment_menu
