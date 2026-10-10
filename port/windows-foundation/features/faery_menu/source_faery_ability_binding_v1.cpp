#include "source_faery_ability_binding_v1.hpp"

#include "../../../level-world/character_stance.hpp"

#include <cstring>
#include <limits>

namespace dh::foundation::faery_menu {
namespace {
using dh2::world::CanonicalCharacterCandidateRecordV60;

bool same_graph(const SourceFaeryAbilityV1& source,
                CanonicalCharacterCandidateRecordV60*& record,
                std::string& error) {
    const auto& native = source.native;
    if (!native.selected_character_owner || !native.record || !native.record->actor ||
        !native.record->actor->object || !native.record->actor->machine ||
        !native.record->save || !native.record->properties ||
        !native.record->load || !native.record->profile_bootstrap ||
        native.record->failed ||
        !native.record->player_script_owner_v62 || !native.record->faery_association_v68 ||
        !native.query_graph || !native.actions || !native.faery_tables ||
        native.query_graph->owner.get() != native.selected_character_owner.get() ||
        native.actions->bindings().owner.get() != native.selected_character_owner.get() ||
        native.query_graph->actions != native.actions ||
        native.query_graph->faery_actions != native.faery_actions ||
        native.actions->bindings().save != native.record->save.get() ||
        native.record->player_script_owner_v62->native_savegame() !=
            native.record->save.get() ||
        native.actions->bindings().skills.identity() !=
            native.record->player_script_owner_v62.get()) {
        error = "Faery ability requires the same retained canonical Player/Save/skill/action owner";
        return false;
    }
    auto& actual = *native.record;
    const auto identity = actual.actor->object->identity;
    const auto* save_slot = actual.save_fields ? actual.save_fields->save_slot14e8() : nullptr;
    const auto profile = actual.profile_bootstrap->profile();
    if (!identity || actual.save->character() != identity ||
        !profile || !profile->ready() ||
        actual.profile_bootstrap->save() != actual.save ||
        actual.profile_bootstrap->load_owner() != actual.load ||
        actual.load->profile().identity != profile->receiver().identity ||
        actual.player_script_owner_v62->native_savegame() != actual.save.get() ||
        !save_slot || *save_slot != reinterpret_cast<std::uintptr_t>(actual.save.get()) ||
        actual.faery_association_v68->character != identity ||
        !actual.actor->source_ai_pointers_v105() ||
        actual.actor->source_ai_pointers_v105()->auxiliary58 !=
            actual.faery_association_v68->faery420 ||
        !actual.services.faeries_v70 ||
        &actual.services.faeries_v70.faeries() != &native.faery_tables.faeries() ||
        &actual.services.faeries_v70.lists() != &native.faery_tables.lists() ||
        native.faery_tables.lists().empty() ||
        native.faery_tables.lists().front() != std::vector<std::int32_t>{2, 4, 5, 6, 3}) {
        error = "Faery ability Save/Character+420/CharAI+58/FaeryTables aliases differ";
        return false;
    }
    if (!native.actions->validate_graph(true, error)) return false;
    record = &actual;
    error.clear();
    return true;
}

bool selected_slot(const SourceFaeryAbilityV1& source,
                   CanonicalCharacterCandidateRecordV60& record,
                   std::int32_t& difficulty, std::int32_t& slot,
                   std::string& error) {
    if (!source.native.query_graph->difficulty ||
        !source.native.query_graph->difficulty(difficulty, error)) {
        if (error.empty()) error = "Required current difficulty producer for same Player Save";
        return false;
    }
    if (difficulty < 0 || difficulty >= 3) {
        error = "Source current difficulty is outside the three Player Save faery cells";
        return false;
    }
    slot = record.save->current_faery(static_cast<std::uint32_t>(difficulty));
    if (slot < 0 || slot >= 5) {
        error = "Same Player Save current-faery value is not one of source slots 0..4";
        return false;
    }
    error.clear();
    return true;
}

bool validate_sink(const SourceFaeryAbilityV1& source,
                   CanonicalCharacterCandidateRecordV60& record,
                   std::string& error) {
    const auto& sink = source.state_sink;
    const auto identity = record.actor->object->identity;
    if (!sink.owner || sink.owner.get() != source.native.selected_character_owner.get() ||
        sink.character != identity || sink.state != &record.actor->machine->state() ||
        !sink.context || !sink.raise_event || !sink.set_state) {
        error = "Cast state sink must be the selected-character lease and its same live State coordinator";
        return false;
    }
    error.clear();
    return true;
}
}

bool active_faery_ability_v1(const SourceFaeryAbilityV1& source,
                             ActiveFaeryAbilityV1& output,
                             std::string& error) {
    CanonicalCharacterCandidateRecordV60* record{};
    if (!same_graph(source, record, error)) return false;
    auto& skills = *record->player_script_owner_v62;
    if (!skills.ready()) {
        error = "Same Player V6 skill/spell VM has not reached source-ready state";
        return false;
    }
    ActiveFaeryAbilityV1 result{};
    if (!selected_slot(source, *record, result.difficulty, result.save_slot, error)) return false;

    // Character GetPyCst(29) picks the one live FaeryList; invalid cached
    // indices use source list0 fallback. It is the same immutable table borrow
    // held by canonical Player construction and this UI owner.
    auto list_index = record->properties->resolved[29];
    if (list_index < 0 || std::size_t(list_index) >= source.native.faery_tables.lists().size())
        list_index = 0;
    const auto& list = source.native.faery_tables.lists()[std::size_t(list_index)];
    const auto slot = static_cast<std::size_t>(result.save_slot);
    std::int32_t declared_count{}, row_count{};
    if (skills.session().constant("FaeryTypes", "COUNT", declared_count) ||
        result.save_slot >= declared_count ||
        skills.session().constant("FaeryTypes", "COUNT", row_count) ||
        row_count < 0 || list.size() != static_cast<std::size_t>(row_count) ||
        slot >= list.size()) {
        error = "Current Save slot or FaeryList dimensions fail the source FaeryTypes/COUNT checks";
        return false;
    }
    result.table_record_id = list[slot];
    const auto& rows = source.native.faery_tables.faeries();
    if (result.table_record_id < 0 || std::size_t(result.table_record_id) >= rows.size() ||
        rows[std::size_t(result.table_record_id)].scalar.words[8] !=
            static_cast<std::uint32_t>(result.save_slot)) {
        error = "Current FaeryList row does not match its authored source slot";
        return false;
    }
    const auto& row = rows[std::size_t(result.table_record_id)];
    result.spell_type = static_cast<std::int32_t>(row.scalar.words[7]);
    result.spell_script = row.script;

    const auto& spells = skills.state().spells;
    if (slot < spells.count && spells.items) result.spell_instance = spells.items[slot];
    output = std::move(result);
    error.clear();
    return true;
}

bool source_cast_animation_v1(const SourceFaeryAbilityV1& source,
                              CastAnimationV1& output, bool& has_sequence,
                              std::string& error) {
    has_sequence = false;
    CanonicalCharacterCandidateRecordV60* record{};
    if (!same_graph(source, record, error)) return false;
    std::int32_t difficulty{}, slot{};
    if (!selected_slot(source, *record, difficulty, slot, error)) return false;
    const auto* tables = record->services.animation_tables;
    if (!tables || !record->properties) {
        error = "Required loaded CharAnimTable and Character property2 sources";
        return false;
    }
    auto& skills = *record->player_script_owner_v62;
    if (!skills.ready()) {
        error = "Same Player V6 skill/spell VM has not reached source-ready state";
        return false;
    }
    const auto table = dh2_character_animation_table_id(
        record->properties->resolved[2], static_cast<std::int32_t>(tables->characters.size()));
    if (table < 0 || std::size_t(table) >= tables->characters.size()) {
        // SM_SetCastState's invalid global CharAnimTable ID is a source no-op.
        output = {};
        output.save_slot = slot;
        output.animation_table = table;
        error.clear();
        return true;
    }
    const auto& spells = tables->characters[std::size_t(table)].fields[31];
    if (std::size_t(slot) >= spells.size()) {
        // Source returns before querying stanced constants in this branch.
        output = {};
        output.save_slot = slot;
        output.animation_table = table;
        error.clear();
        return true;
    }

    std::int32_t stance_mask{};
    if (skills.session().constant("AnimStancedAnim", "SL__LIST_IPHONE", stance_mask)) {
        error = "Required same Session AnimStancedAnim/SL__LIST_IPHONE constant";
        return false;
    }
    std::int32_t actual_stance{};
    if (std::uint32_t(stance_mask) & 0x400000u) {
        std::int32_t count{};
        if (skills.session().constant("AnimStances", "COUNT_IPHONE", count)) {
            error = "Required same Session AnimStances/COUNT_IPHONE constant";
            return false;
        }
        auto* equipment = source.native.actions->bindings().equipment;
        if (!equipment) {
            error = "Required same initialized Player Gear stance producer";
            return false;
        }
        dh2::character::StanceFacts16 facts{};
        if (!equipment->stance_facts(true, count, facts, error)) return false;
        if (dh2_character_anim_stance(&actual_stance, &facts) != 1) {
            error = "Source Character::GetAnimStance rejected actual same Player Gear facts";
            return false;
        }
    }
    // The pure tail retains the exact source 32-bit addition and the stored
    // negative sequence values; they are not normalized as missing rows.
    CastAnimationV1 result{};
    if (!source_cast_sequence_facts_v1(spells, table, slot, stance_mask,
                                       actual_stance, result)) {
        error = "Source CharAnimTable Spells row disappeared during cast selection";
        return false;
    }
    output = result;
    has_sequence = true;
    error.clear();
    return true;
}

bool source_set_cast_state_v1(const SourceFaeryAbilityV1& source,
                              bool direct_set, std::uintptr_t payload,
                              bool& source_noop, std::string& error) {
    source_noop = false;
    CanonicalCharacterCandidateRecordV60* record{};
    if (!same_graph(source, record, error) || !validate_sink(source, *record, error)) return false;
    CastAnimationV1 animation{};
    bool has_sequence{};
    if (!source_cast_animation_v1(source, animation, has_sequence, error)) return false;
    if (!has_sequence) {
        source_noop = true;
        error.clear();
        return true;
    }
    auto& state = *source.state_sink.state;
    state.animation_override = static_cast<std::int32_t>(animation.animation_override);
    constexpr std::int32_t source_event = 0xc356;
    const bool delivered = direct_set
        ? source.state_sink.set_state(source.state_sink.context, state, 7,
                                      source_event, payload, error)
        : source.state_sink.raise_event(source.state_sink.context, state,
                                        source_event, payload, error);
    if (!delivered) {
        if (error.empty()) error = "Required same Character State coordinator rejected SM_SetCastState dispatch";
        return false;
    }
    error.clear();
    return true;
}

bool source_faery_animation_event_v1(
    const SourceFaeryAbilityV1& source,
    const dh2::animation::TriggeredEvent& event,
    std::string& error) {
    CanonicalCharacterCandidateRecordV60* record{};
    if (!same_graph(source, record, error)) return false;
    if (!event.name || std::strcmp(event.name, "do_spell") != 0 ||
        record->actor->machine->state().current != 7) {
        error.clear();
        return true;
    }
    auto& skills = *record->player_script_owner_v62;
    if (!skills.ready()) {
        error = "Authored do_spell reached before same Player V6 VM is source-ready";
        return false;
    }
    std::int32_t difficulty{}, slot{};
    if (!selected_slot(source, *record, difficulty, slot, error)) return false;

    // This is CharAISkillScript::OnSkill Use, kind1 + callback operation1,
    // reached only after the real animator's exact authored marker. The VM
    // retains its own null-instance no-op and required-provider behavior.
    std::uint32_t ignored_result{};
    if (skills.native_instance_callback_v68(1, static_cast<std::uint32_t>(slot),
            dh2::character::skills::skill_use_v3, &ignored_result) < 0) {
        error = "Same Player Spell Use callback failed: " + skills.error();
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
