#include "runtime_creation_persistence_v1.hpp"

#include <cstring>

#include "../../../character_state.hpp"
#include "../../../save_store.hpp"
#include "../../menu_metadata/menu_metadata_v1.hpp"
#include "../../../../game-data/loot_creation_v8.hpp"

#include <algorithm>
#include <cmath>
#include <exception>
#include <limits>
#include <utility>

namespace dh::foundation::frontend::creation {
namespace {

bool valid_text(const std::string& text) {
    if (text.empty() || text.size() > character_text_limit) return false;
    for (unsigned char ch : text) if (ch < 0x20 || ch == 0x7f) return false;
    return true;
}

bool same_owner(const std::shared_ptr<CharacterState>& a,
                const std::shared_ptr<CharacterState>& b) noexcept {
    return a && b && a.get() == b.get() &&
           !a.owner_before(b) && !b.owner_before(a);
}

bool same_state(const CharacterState& a, const CharacterState& b) noexcept {
    if (a.schema_version != b.schema_version || a.id != b.id || a.name != b.name ||
        a.class_id != b.class_id || a.stats.level != b.stats.level ||
        a.stats.health != b.stats.health || a.stats.max_health != b.stats.max_health ||
        a.stats.resource != b.stats.resource || a.stats.max_resource != b.stats.max_resource ||
        a.stats.strength != b.stats.strength || a.stats.dexterity != b.stats.dexterity ||
        a.stats.intelligence != b.stats.intelligence || a.stats.endurance != b.stats.endurance ||
        a.stats.energy != b.stats.energy || a.experience != b.experience ||
        a.gold != b.gold || a.inventory.size() != b.inventory.size() ||
        a.equipment.size() != b.equipment.size() || a.skills.size() != b.skills.size() ||
        a.unlocks != b.unlocks || a.source_stat_points != b.source_stat_points ||
        a.source_skill_points != b.source_skill_points ||
        a.source_endurance_energy_known != b.source_endurance_energy_known ||
        a.source_points_known != b.source_points_known ||
        a.source_skill_slots_known != b.source_skill_slots_known ||
        a.source_faery_state_known != b.source_faery_state_known ||
        a.skill_slots.size() != b.skill_slots.size() ||
        a.source_faery_list_id != b.source_faery_list_id) return false;
    for (std::size_t i = 0; i < a.inventory.size(); ++i) {
        const auto& x = a.inventory[i]; const auto& y = b.inventory[i];
        if (x.instance_id != y.instance_id || x.definition_id != y.definition_id ||
            x.quantity != y.quantity) return false;
    }
    for (std::size_t i = 0; i < a.equipment.size(); ++i)
        if (a.equipment[i].slot != b.equipment[i].slot ||
            a.equipment[i].item_instance_id != b.equipment[i].item_instance_id ||
            a.equipment[i].equipment_set != b.equipment[i].equipment_set ||
            a.equipment[i].source_slot != b.equipment[i].source_slot) return false;
    for (std::size_t i = 0; i < a.skills.size(); ++i)
        if (a.skills[i].id != b.skills[i].id || a.skills[i].rank != b.skills[i].rank) return false;
    for (std::size_t i = 0; i < a.skill_slots.size(); ++i)
        if (a.skill_slots[i].equipment_set != b.skill_slots[i].equipment_set ||
            a.skill_slots[i].slot != b.skill_slots[i].slot ||
            a.skill_slots[i].saved_skill_row != b.skill_slots[i].saved_skill_row) return false;
    for (std::size_t d = 0; d < a.faery_by_difficulty.size(); ++d) {
        if (a.faery_by_difficulty[d].current_faery != b.faery_by_difficulty[d].current_faery) return false;
        for (std::size_t f = 0; f < a.faery_by_difficulty[d].faeries.size(); ++f)
            if (a.faery_by_difficulty[d].faeries[f].state != b.faery_by_difficulty[d].faeries[f].state ||
                a.faery_by_difficulty[d].faeries[f].level != b.faery_by_difficulty[d].faeries[f].level) return false;
    }
    return true;
}

void fail(RuntimeCreationResultV1& result, RuntimeCreationStatusV1 status,
          std::string message) {
    result.status = status;
    result.error = std::move(message);
}

std::int32_t source_integer(std::int32_t value) noexcept { return value >> 8; }

bool fresh_unlocked_difficulty(const std::vector<std::uint8_t>& bytes,
                               std::uint32_t& difficulty, std::string& error) {
    auto word = [&](std::size_t at, std::uint32_t& out) {
        if (at > bytes.size() || bytes.size() - at < 4) return false;
        out = std::uint32_t(bytes[at]) | (std::uint32_t(bytes[at + 1]) << 8) |
              (std::uint32_t(bytes[at + 2]) << 16) | (std::uint32_t(bytes[at + 3]) << 24);
        return true;
    };
    std::uint32_t count{};
    if (!word(0, count) || count > 64) { error = "Fresh Save metadata section count is invalid"; return false; }
    std::size_t at = 4;
    bool found = false;
    for (std::uint32_t i = 0; i < count; ++i) {
        std::uint32_t payload_size{};
        if (!word(at, payload_size)) { error = "Fresh Save metadata section header is truncated"; return false; }
        at += 4;
        if (payload_size > bytes.size() - at || bytes.size() - at - payload_size < 4) {
            error = "Fresh Save metadata section exceeds its source stream"; return false;
        }
        const bool pdf = bytes[at] == 'P' && bytes[at + 1] == 'D' && bytes[at + 2] == 'F' && bytes[at + 3] == 'L';
        if (pdf) {
            std::uint32_t unlocked{};
            if (found || payload_size != 8 || !word(at + 8, unlocked)) {
                error = "Fresh PDFL must contain exactly the source current/unlocked difficulty words";
                return false;
            }
            const auto signed_unlocked = static_cast<std::int32_t>(unlocked);
            if (signed_unlocked < 0 || signed_unlocked > 2) {
                error = "Fresh PDFL unlocked difficulty is outside the original three-mode range";
                return false;
            }
            difficulty = static_cast<std::uint32_t>(signed_unlocked);
            found = true;
        }
        at += 4 + payload_size;
    }
    if (at != bytes.size() || !found) {
        error = "Fresh Save metadata did not contain one complete PDFL difficulty section";
        return false;
    }
    error.clear();
    return true;
}

std::int32_t equipment_set_for_slot(std::int32_t slot) noexcept {
    // Fresh PlayerEquipment starts selected set 0. The native SetForSlot
    // source routes general armor/rings to set 0 and slots 1/2 to selection.
    (void)slot;
    return 0;
}

bool project_source_auto_equip(CharacterState& state, const dh2::data::ItemTable& items,
                               bool dual_wield, bool offhand_allowed, std::string& error) {
    std::array<bool, 9> occupied{};
    for (std::size_t i = 0; i < state.inventory.size(); ++i) {
        const auto item_id = dh2::data::item_id(items, state.inventory[i].definition_id);
        if (item_id < 0 || std::size_t(item_id) >= items.rows.size()) {
            error = "Starter inventory identifier is absent from the original ItemTable";
            return false;
        }
        const auto& item = items.rows[std::size_t(item_id)];
        auto slot = item.record.words[26];
        const auto type = dh2::data::item_type(item);
        if (type != 5 && type != 4) {
            if (slot == 1 && dual_wield) slot = -3;
            else if (slot == -4 && offhand_allowed) slot = 1;
        }
        std::int32_t target = -1;
        if (slot >= 0 && slot < 9) {
            target = slot;
            if (slot == 2 && occupied[1]) {
                error = "Source two-handed auto-equip requires an unsupported prior equipment transition";
                return false;
            }
        } else if (slot == -3 || slot == -2) {
            const auto first = slot == -3 ? 1 : 5;
            const auto second = first + 1;
            target = occupied[std::size_t(first)] ? second : first;
            if (occupied[std::size_t(target)]) continue;
        } else if (slot == -4) {
            if (offhand_allowed) target = 1;
            else {
                if (occupied[2]) occupied[2] = false;
                target = 1;
            }
        }
        if (target < 0) continue;
        if (occupied[std::size_t(target)]) {
            error = "Source starter auto-equip would replace a previously granted item; generic replacement is unsupported";
            return false;
        }
        if (state.inventory[i].quantity != 1) {
            error = "Source starter auto-equip requires a split item instance not represented by the current generic grant path";
            return false;
        }
        occupied[std::size_t(target)] = true;
        state.equipment.push_back({"slot-" + std::to_string(target), state.inventory[i].instance_id,
                                   equipment_set_for_slot(target), target});
    }
    error.clear();
    return true;
}

bool build_source_starter(const RuntimeCreationRequestV1& request,
                          const RuntimeCreationServicesV1& services,
                          RuntimeCreationResultV1& result,
                          CharacterState& candidate) {
    const auto& source = services.source;
    if (!source.properties || !source.loot || !source.skills) {
        fail(result, RuntimeCreationStatusV1::unavailable_source,
             "Original property, loot, and skill-table owners are required");
        return false;
    }
    if (!source.source_loot_random) {
        fail(result, RuntimeCreationStatusV1::unavailable_source,
             "The caller's original application LootRandom8 owner is required for source ItemList selection");
        return false;
    }

    const auto* choice = find_class(request.class_token);
    if (!choice) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Requested class is not one of the original fresh playable base rows");
        return false;
    }
    const auto& properties = *source.properties;
    auto character_row = std::find(properties.characters.names.begin(),
                                   properties.characters.names.end(), choice->profile_token);
    if (character_row == properties.characters.names.end()) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Original CharacterTable class row is absent");
        return false;
    }
    result.character_row = static_cast<std::int32_t>(character_row - properties.characters.names.begin());

    const auto& fields = properties.characters.fields;
    if (fields.size() != 224 || fields[9] != "Loot" || fields[28] != "SkillTree" ||
        fields[29] != "FaeryList" || fields[148] != "Stat_Points" || fields[157] != "Skill_Points" ||
        fields[149] != "Stat_Strength" || fields[150] != "Stat_Dexterity" ||
        fields[151] != "Stat_Endurance" || fields[152] != "Stat_Energy") {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Original CharacterTable property layout does not match the source starter fields");
        return false;
    }

    std::string error;
    if (!dh2::data::fresh_player_profile_v1(properties.characters,
            choice->profile_token.data(), request.player_name.c_str(), request.source_timer,
            request.saved_date, result.source_profile_metadata, error)) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             error.empty() ? "Original fresh profile metadata producer failed" : error);
        return false;
    }
    if (result.source_profile_metadata.character_row != result.character_row) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Fresh profile metadata row differs from the selected original character row");
        return false;
    }
    std::uint32_t fresh_difficulty{};
    if (!fresh_unlocked_difficulty(result.source_profile_metadata.bytes, fresh_difficulty, error)) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             error.empty() ? "Fresh source Save difficulty is unavailable" : error);
        return false;
    }

    OriginalActorProperties actor;
    if (!resolve_original_fresh_player(properties, std::string(choice->profile_token), actor, error)) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             error.empty() ? "Original class/property formula resolver failed" : error);
        return false;
    }
    const auto loot_row = actor.sheets.resolved[9];
    const auto skill_row = actor.sheets.resolved[28];
    const auto faery_row = actor.sheets.resolved[29];
    result.starting_loot_row = loot_row;
    result.skill_list_row = skill_row;
    if (loot_row != static_cast<std::int32_t>(choice->starting_loot_row) ||
        skill_row != static_cast<std::int32_t>(choice->skill_list_row) ||
        faery_row != static_cast<std::int32_t>(choice->faery_list_row)) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Resolved original class properties disagree with the recovered starter loot/skill rows");
        return false;
    }

    if (std::size_t(skill_row) >= source.skills.lists().size() ||
        choice->first_skill_dictionary_row >= source.skills.skill_names().size()) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Original starter SkillList or skill dictionary row is absent");
        return false;
    }
    const auto& skill_list = source.skills.lists()[std::size_t(skill_row)];
    if (skill_list.empty() || skill_list.front() != static_cast<std::int32_t>(choice->first_skill_dictionary_row) ||
        source.skills.skill_names()[choice->first_skill_dictionary_row] != choice->first_skill_token) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Original class SkillList first grant does not match the recovered source skill");
        return false;
    }

    if (loot_row < 0 || std::size_t(loot_row) >= source.loot.loots().size()) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Original class starting Loot row is outside the borrowed LootTable");
        return false;
    }
    const auto& loot = source.loot.loots()[std::size_t(loot_row)];
    if (loot.roll_type != 0 || !loot.random_entries.empty() || !loot.sub_loots.empty()) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "This starter requires original random Loot/SubLoot selection services not present in the minimal profile path");
        return false;
    }
    const auto& item_table = source.loot.items();
    if (item_table.identifiers.size() != item_table.rows.size()) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Original ItemTable identifiers do not match its rows");
        return false;
    }

    candidate = CharacterState{};
    candidate.id = request.character_id;
    candidate.name = request.player_name;
    candidate.class_id = request.class_token;
    candidate.stats.level = initial_player_level;
    candidate.stats.health = actor.health;
    candidate.stats.max_health = actor.max_health;
    candidate.stats.resource = actor.resource;
    candidate.stats.max_resource = actor.max_resource;
    candidate.stats.strength = original_signed256(actor.sheets.resolved[149]);
    candidate.stats.dexterity = original_signed256(actor.sheets.resolved[150]);
    candidate.stats.endurance = original_signed256(actor.sheets.resolved[151]);
    candidate.stats.energy = original_signed256(actor.sheets.resolved[152]);
    candidate.stats.intelligence = 0.0f; // retained generic field; source has no corresponding property
    const auto stat_points = source_integer(actor.sheets.resolved[148]);
    const auto skill_points = source_integer(actor.sheets.resolved[157]);
    if (stat_points < 0 || skill_points < 0) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Original source points resolved to a negative signed value");
        return false;
    }
    candidate.source_stat_points = static_cast<std::uint32_t>(stat_points);
    candidate.source_skill_points = static_cast<std::uint32_t>(skill_points);
    candidate.source_endurance_energy_known = true;
    candidate.source_points_known = true;
    candidate.source_faery_state_known = true;
    candidate.source_faery_list_id = faery_row;

    // Native SkillList initialization creates every authored row at rank 0.
    // The class constructor's subsequent free-skill increment is a distinct
    // operation and is applied only with actual source difficulty/cap inputs.
    if (!initialize_source_skill_rows_v1(source.skills, skill_row, candidate, error)) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             error.empty() ? "Original SkillList projection failed" : error);
        return false;
    }
    if (candidate.skills.empty() || candidate.skills.front().id != choice->first_skill_token) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             "Mapped saved skill row zero differs from the authored first SkillList grant");
        return false;
    }
    if (services.source.source_skill_grant_context_known) {
        const auto difficulty = fresh_difficulty;
        if (difficulty >= services.source.source_skill_level_caps.size()) {
            fail(result, RuntimeCreationStatusV1::unavailable_source,
                 "Original unlocked difficulty is outside the source CharacterDesign cap set");
            return false;
        }
        const auto skill_id = skill_list.front();
        if (std::size_t(skill_id) >= source.skills.skills().size()) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "First SkillList row has no original SkillTable record");
            return false;
        }
        const auto level = source_integer(actor.sheets.resolved[19]);
        const auto required_level = source.skills.skills()[std::size_t(skill_id)].scalar.words[8];
        const auto cap = services.source.source_skill_level_caps[difficulty];
        // Source IncSkill: point available, class SkillList membership, level
        // requirement, cap, and current rank bound. The callback contains only
        // actual original CharacterDesign/unlock values.
        if (candidate.source_skill_points && level >= required_level &&
            0 <= level - required_level && 0 < cap) {
            candidate.skills.front().rank = 1;
            --candidate.source_skill_points;
        }
    }
    for (auto& difficulty : candidate.faery_by_difficulty) {
        difficulty.current_faery = 0;
        for (auto& faery : difficulty.faeries) faery = {};
    }

    const std::int32_t expected_fixed_words[] = {-1, 0, -1, -1, 0, 0, 0};
    for (const auto& entry : loot.fixed_entries) {
        if (!std::equal(std::begin(expected_fixed_words), std::end(expected_fixed_words), entry.words + 1)) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Original starter Loot requires unsupported power/roll flags");
            return false;
        }
        const auto list_id = entry.words[0];
        if (list_id < 0 || std::size_t(list_id) >= source.loot.item_lists().size()) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Original starter Loot references an unavailable ItemList row");
            return false;
        }
        const auto& list = source.loot.item_lists()[std::size_t(list_id)];
        // All three authored class starter ItemLists are singleton, probability
        // one rows. The source still advances its shared RNG once per list.
        if (list.size() != 1 || list.front().probability != 1 || list.front().item < 0 ||
            std::size_t(list.front().item) >= item_table.rows.size() || !list.front().quantity) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Original starter ItemList is not in the verified singleton source form");
            return false;
        }
        std::int32_t weighted_draw{};
        if (dh2_loot_v2_random(source.source_loot_random, 1, &weighted_draw) != 0 || weighted_draw != 0) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Original LootRandom8 source selector failed for the authored singleton ItemList");
            return false;
        }
        const auto& selected = list.front();
        std::int32_t item_id = selected.item;
        if (!select_loot_item_variant_v8(item_table, item_id, 0, false, item_id, error)) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 error.empty() ? "Original AddLoot item variant resolver failed" : error);
            return false;
        }
        const auto& item = item_table.rows[std::size_t(item_id)];
        if (dh2::data::item_type(item) == 13) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Starter grant includes source gold requiring original gold valuation/notification services");
            return false;
        }
        if (item.record.words[4] == 2 || item.record.words[4] == 3) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Starter item count depends on the original PlayerManager class count provider");
            return false;
        }
        std::int8_t signed_quantity{};
        static_assert(sizeof(signed_quantity) == sizeof(selected.quantity));
        std::memcpy(&signed_quantity, &selected.quantity, sizeof(signed_quantity));
        if (signed_quantity == -2) signed_quantity = 99;
        if (signed_quantity < 0) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Starter item uses a negative source SetQty value without its assertion continuation");
            return false;
        }
        if (std::size_t(item_id) >= item_table.identifiers.size()) {
            fail(result, RuntimeCreationStatusV1::source_initialization_failed,
                 "Resolved starter ItemID has no original identifier");
            return false;
        }
        candidate.inventory.push_back({
            "starter-item-" + std::to_string(candidate.inventory.size()),
            item_table.identifiers[std::size_t(item_id)],
            static_cast<std::uint32_t>(signed_quantity)});
    }

    if (!project_source_auto_equip(candidate, item_table,
                                   actor.sheets.resolved[202] != 0,
                                   actor.sheets.resolved[203] != 0, error)) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             error.empty() ? "Original source starter auto-equip projection failed" : error);
        return false;
    }
    const auto validation = validate_character_state(candidate);
    if (!validation.ok()) {
        fail(result, RuntimeCreationStatusV1::source_initialization_failed,
             validation.errors.empty() ? "Source starter projection is invalid" : validation.errors.front());
        return false;
    }
    return true;
}

} // namespace

bool initialize_source_skill_rows_v1(dh2::data::SkillTables::Borrow skills,
                                     std::int32_t source_skill_list_id,
                                     CharacterState& state,
                                     std::string& error) {
    if (!skills) {
        error = "Original SkillTables owner is unavailable";
        return false;
    }
    if (state.source_skill_slots_known || !state.skills.empty() || !state.skill_slots.empty()) {
        error = "CharacterState already has skill rows, slot maps, or known source slot state; refusing to reseed";
        return false;
    }
    const auto& lists = skills.lists();
    const auto& names = skills.skill_names();
    if (source_skill_list_id < 0 || std::size_t(source_skill_list_id) >= lists.size()) {
        error = "Source SkillList ID is outside the loaded SkillTables";
        return false;
    }
    const auto& list = lists[std::size_t(source_skill_list_id)];
    if (list.empty()) {
        error = "Source SkillList is empty; initial slot-zero maps have no saved row";
        return false;
    }

    std::vector<SkillProgress> rows;
    rows.reserve(list.size());
    for (const auto dictionary_id : list) {
        if (dictionary_id < 0 || std::size_t(dictionary_id) >= names.size()) {
            error = "Source SkillList references an absent SkillTable dictionary row";
            return false;
        }
        const auto& name = names[std::size_t(dictionary_id)];
        if (!valid_text(name)) {
            error = "Source SkillTable dictionary name is not a valid saved skill identifier";
            return false;
        }
        rows.push_back({name, 0});
    }
    std::vector<SkillSlotBinding> slots{{0, 0, 0}, {1, 0, 0}};
    state.skills.swap(rows);
    state.skill_slots.swap(slots);
    state.source_skill_slots_known = true;
    error.clear();
    return true;
}

bool RuntimeCreationResultV1::same_state_owner(
    const std::shared_ptr<CharacterState>& expected) const noexcept {
    return same_owner(shared_state, expected);
}

const std::vector<std::string>& RuntimeCreationPersistenceV1::representation_limits() noexcept {
    static const std::vector<std::string> limits{
        "Source skill rows start at rank 0. The later native InitPost free-skill increment is applied only when real CharacterDesign caps and unlocked difficulty are supplied.",
        "The generic model preserves source starter inventory IDs, quantities and auto-equip set/slot bindings; it does not construct native ItemInstance names, effects, requirements or gear-derived recalculated stats.",
        "Source faery list and zero progress are preserved; live faery unlock mutations remain a separate campaign service.",
        "Generic save/reload plus start_same_state is not a NativeStartGame receipt, completed InitPost, or native gameplay handoff."};
    return limits;
}

RuntimeCreationResultV1 RuntimeCreationPersistenceV1::create_reload(
    const RuntimeCreationRequestV1& request, const RuntimeCreationServicesV1& services) {
    RuntimeCreationResultV1 result;
    result.shared_state = request.shared_state;
    result.representation_limits = representation_limits();

    if (!request.shared_state || request.save_path.empty() ||
        !valid_text(request.character_id) || !valid_text(request.player_name) ||
        !find_class(request.class_token)) {
        fail(result, RuntimeCreationStatusV1::invalid_request,
             "Require a caller-owned CharacterState, unique nonempty save path, valid id/name and original base-class token");
        return result;
    }
    std::error_code path_error;
    const bool destination_exists = std::filesystem::exists(request.save_path, path_error);
    if (path_error) {
        fail(result, RuntimeCreationStatusV1::invalid_request,
             "Could not verify that the new-character save destination is absent: " + path_error.message());
        return result;
    }
    if (destination_exists) {
        fail(result, RuntimeCreationStatusV1::destination_exists,
             "New-character creation refuses to overwrite an existing profile; use the selected-profile load route");
        return result;
    }
    CharacterState candidate;
    if (!build_source_starter(request, services, result, candidate)) return result;

    std::string error;
    // P14 schema: FS_StartGame (0x4220a0) stamps the fresh profile: save date, LevelList row 41, act 1, Normal.
    menu_metadata::initialize_fresh_menu_metadata(candidate, request.saved_date);
    if (!save_character(request.save_path, candidate, error)) {
        fail(result, RuntimeCreationStatusV1::persistence_failed,
             error.empty() ? "Existing generic save API failed" : error);
        return result;
    }
    result.saved = true;

    CharacterState reloaded;
    if (!load_character(request.save_path, reloaded, error)) {
        fail(result, RuntimeCreationStatusV1::reload_failed,
             error.empty() ? "Existing generic save reload failed" : error);
        return result;
    }
    result.reloaded = true;
    if (!same_state(candidate, reloaded)) {
        fail(result, RuntimeCreationStatusV1::reload_mismatch,
             "Generic save readback differs from the source-built starter value");
        return result;
    }

    const auto state_pointer = request.shared_state.get();
    const auto state_owner = request.shared_state;
    *request.shared_state = std::move(reloaded);
    result.published_to_shared_state = true;
    if (request.shared_state.get() != state_pointer || !same_owner(state_owner, request.shared_state)) {
        fail(result, RuntimeCreationStatusV1::start_failed,
             "Caller shared-state identity/owner changed while publishing the validated reload");
        return result;
    }
    result.status = RuntimeCreationStatusV1::prepared_for_start;
    result.error.clear();
    return result;
}

RuntimeCreationResultV1 RuntimeCreationPersistenceV1::create_reload_start(
    const RuntimeCreationRequestV1& request, const RuntimeCreationServicesV1& services) {
    if (!services.start_same_state) {
        RuntimeCreationResultV1 result;
        result.shared_state = request.shared_state;
        result.representation_limits = representation_limits();
        fail(result, RuntimeCreationStatusV1::unavailable_start,
             "Existing same-state start provider is unavailable");
        return result;
    }
    auto result = create_reload(request, services);
    if (result.status != RuntimeCreationStatusV1::prepared_for_start) return result;
    std::string error;
    try {
        if (!services.start_same_state(request.shared_state, error)) {
            fail(result, RuntimeCreationStatusV1::start_failed,
                 error.empty() ? "Same-state start provider failed" : error);
            return result;
        }
    } catch (const std::exception& ex) {
        fail(result, RuntimeCreationStatusV1::start_failed, ex.what());
        return result;
    } catch (...) {
        fail(result, RuntimeCreationStatusV1::start_failed,
             "Same-state start provider threw an unknown exception");
        return result;
    }
    result.status = RuntimeCreationStatusV1::start_provider_returned_success;
    result.start_provider_succeeded = true;
    result.error.clear();
    return result;
}

} // namespace dh::foundation::frontend::creation
