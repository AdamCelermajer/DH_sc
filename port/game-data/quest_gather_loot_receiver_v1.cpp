#include "quest_gather_loot_receiver_v1.hpp"

namespace dh2::data::quest_gather_loot_receiver_v1 {
bool compile(Record& objective, bool level_matches, std::int32_t item_id,
             std::int32_t target_quantity, std::int32_t script_id,
             bool item_found, std::int32_t item_quantity,
             void* script_context, StartScript start_script,
             std::string& error) {
    if (!level_matches) {
        error.clear();
        return true;
    }
    if (item_id <= 0 || target_quantity <= 0 || item_quantity < 0) {
        error = "GatherLoot Compile inputs are outside the recovered source domain";
        return false;
    }
    if (item_found) objective.quantity_20 = std::uint32_t(item_quantity);
    objective.compiled_8 = 1;
    if (std::int32_t(objective.quantity_20) < target_quantity || objective.done_14) {
        error.clear();
        return true;
    }
    // Objective::SetIsCompleted stores done before starting its optional script.
    objective.done_14 = 1;
    if (script_id >= 0) {
        if (!start_script) {
            error = "GatherLoot Compile completion requires ScriptManager::StartScript and source script-base resolution";
            return false;
        }
        if (!start_script(script_context, script_id, error)) {
            if (error.empty()) error = "GatherLoot Compile completion script provider failed";
            return false;
        }
    }
    error.clear();
    return true;
}

std::int32_t Binding::receive(void* raw, EventRuntime&, Event& event,
                              std::string& error) {
    if (!raw) {
        error = "GatherLoot receiver context is null";
        return 1;
    }
    auto& binding = *static_cast<Binding*>(raw);
    if (!binding.objective || !binding.item_quantity || binding.item_id < 0 ||
        binding.target_quantity <= 0 ||
        (binding.script_id >= 0 && !binding.start_script)) {
        error = "GatherLoot receiver binding is incomplete";
        return 1;
    }
    auto& objective = *binding.objective;
    if (event.objective_type != binding.event_type ||
        event.character != objective.fields.character_10 ||
        event.item_id != binding.item_id) {
        return 0;
    }
    std::int32_t quantity = 0;
    bool found = false;
    if (!binding.item_quantity(binding.quantity_context, binding.item_id, found, quantity, error))
        return 1;
    if (quantity < 0) {
        error = "GatherLoot Item quantity is negative outside the source domain";
        return 1;
    }
    if (found) objective.quantity_20 = std::uint32_t(quantity);
    quantity = std::int32_t(objective.quantity_20);
    if (!event.flag1) {
        event.flag0 = 1;
        event.subject_id = quantity;
    } else if (quantity > event.subject_id) {
        event.subject_id = quantity;
    }
    if (quantity < binding.target_quantity || objective.done_14) {
        error.clear();
        return 0;
    }
    // Source Objective::SetIsCompleted stores done before StartScript.
    objective.done_14 = 1;
    if (binding.script_id >= 0 &&
        !binding.start_script(binding.script_context, binding.script_id, error)) {
        if (error.empty()) error = "GatherLoot completion script provider failed";
        return 1;
    }
    error.clear();
    return 0;
}
}  // namespace dh2::data::quest_gather_loot_receiver_v1
