#include "player_add_loot_v1.hpp"

#include <limits>
#include <stdexcept>

namespace dh2::data::player_add_loot_v1 {
namespace {
struct Range { std::uintptr_t begin{}, end{}; };

template<class T> bool range(const T* value, Range& out) {
    const auto address = reinterpret_cast<std::uintptr_t>(value);
    if (!value || address % alignof(T) || address > UINTPTR_MAX - sizeof(T)) return false;
    out = {address, address + sizeof(T)};
    return true;
}

bool overlap(Range left, Range right) {
    return left.begin < right.end && right.begin < left.end;
}

bool valid(const Bindings& bindings, const Request& request, std::string& error) {
    Range inventory{}, loot{}, selection{}, pending{}, services{}, creation{};
    Range property_state{}, text{}, binding_object{};
    if (!bindings.character || bindings.character != request.character ||
        !range(bindings.inventory, inventory) || !range(bindings.resolved_loot_property, loot) ||
        !range(bindings.selection, selection) || !range(bindings.pending.value, pending) ||
        !range(bindings.inventory_services, services) || !range(bindings.creation, creation) ||
        !bindings.powers || !range(&bindings.text, text) ||
        !range(&bindings, binding_object)) {
        error = "Invalid or mismatched AddLoot owner binding";
        return false;
    }
    const auto* properties = bindings.inventory->properties();
    if (!range(properties, property_state) ||
        bindings.inventory->character() != bindings.character ||
        bindings.resolved_loot_property != properties->resolved.data() + 9) {
        error = "AddLoot requires the live Character inventory and resolved Loot property";
        return false;
    }
    if (*bindings.pending.value) {
        error = "AddLoot pending Item slot must be empty";
        return false;
    }
    if (!bindings.inventory_services->invoke || !bindings.creation ||
        !bindings.text.invoke || !bindings.text.metadata ||
        bindings.text.context != bindings.inventory_services->context) {
        error = "Required AddLoot inventory, power, or text provider unavailable";
        return false;
    }
    if (bindings.difficulty < 0 || bindings.difficulty > 2 ||
        bindings.selection->player_counts.mage < 0 ||
        bindings.selection->player_counts.rogue < 0 ||
        bindings.selection->player_counts.warrior < 0) {
        error = "Invalid AddLoot difficulty or source class-count facts";
        return false;
    }
    if (overlap(inventory, loot) || overlap(inventory, selection) || overlap(inventory, pending) ||
        overlap(inventory, services) || overlap(inventory, creation) || overlap(inventory, text) ||
        overlap(inventory, property_state) || overlap(loot, selection) || overlap(loot, pending) ||
        overlap(loot, services) || overlap(loot, creation) || overlap(loot, text) ||
        overlap(selection, pending) || overlap(selection, services) ||
        overlap(selection, creation) || overlap(selection, text) || overlap(selection, property_state) ||
        overlap(pending, services) || overlap(pending, creation) || overlap(pending, text) ||
        overlap(pending, property_state) || overlap(services, creation) || overlap(services, text) ||
        overlap(services, property_state) || overlap(creation, text) || overlap(creation, property_state) ||
        overlap(text, property_state) || overlap(binding_object, inventory) ||
        overlap(binding_object, loot) || overlap(binding_object, selection) ||
        overlap(binding_object, pending) || overlap(binding_object, services) ||
        overlap(binding_object, creation) ||
        overlap(binding_object, property_state)) {
        error = "AddLoot binding storage overlaps a live owner";
        return false;
    }
    return true;
}
}

Status invoke(const Bindings& bindings, const Request& request, Result* output,
              std::string& error) {
    Range result_range{}, request_range{}, error_range{}, binding_range{};
    if (!range(output, result_range) || !range(&request, request_range) ||
        !range(&error, error_range) || !range(&bindings, binding_range) ||
        overlap(result_range, request_range) ||
        overlap(result_range, error_range) || overlap(request_range, error_range) ||
        overlap(result_range, binding_range) || overlap(request_range, binding_range) ||
        overlap(error_range, binding_range))
        return Status::invalid_argument;

    error.clear();
    if (!valid(bindings, request, error)) return Status::invalid_argument;

    const auto reject = [&](const char* reason) {
        error = reason;
        return Status::invalid_argument;
    };
    if (request.arguments[0] != *bindings.resolved_loot_property)
        return reject("AddLoot table argument differs from the live Character Loot property");
    if (request.arguments[1] != 0 || request.arguments[2] != 0 || request.arguments[4] != 0)
        return reject("AddLoot arguments are outside the recovered _InitEquipment call");

    *output = {};
    output->loot_table = request.arguments[0];
    if (bindings.inventory->items().size() > std::numeric_limits<std::uint32_t>::max())
        return reject("AddLoot inventory count exceeds its bounded result field");
    output->items_before = static_cast<std::uint32_t>(bindings.inventory->items().size());
    output->attempted = true;

    const OwnedLootEffectsV7 effects{
        bindings.creation, bindings.powers, bindings.text,
        request.arguments[1], request.arguments[2], request.arguments[3], bindings.difficulty};
    bool completed = false;
    try {
        completed = bindings.inventory->add_loot_table(
            request.arguments[0], *bindings.selection, bindings.pending,
            *bindings.inventory_services, effects, error);
    } catch (const std::exception& exception) {
        if (error.empty()) error = exception.what();
    } catch (...) {
        if (error.empty()) error = "AddLoot source provider threw";
    }

    const auto count = bindings.inventory->items().size();
    output->items_after = count > std::numeric_limits<std::uint32_t>::max()
        ? std::numeric_limits<std::uint32_t>::max() : static_cast<std::uint32_t>(count);
    output->pending_item = *bindings.pending.value != nullptr;
    if (!completed) {
        if (error.empty()) error = "Source AddLoot table continuation failed";
        return Status::failed;
    }
    error.clear();
    return Status::complete;
}

} // namespace dh2::data::player_add_loot_v1
