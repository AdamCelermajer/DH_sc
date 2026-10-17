#include "character_init_post_player_v1.hpp"

#include <limits>
#include <stdexcept>

namespace dh2::character_init_post_player_v1 {
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

bool valid(const Bindings& bindings) {
    Range class_id{}, save{}, service{};
    return bindings.character && bindings.property_owner &&
        range(bindings.property_id_13c8, class_id) &&
        range(bindings.current_save_14e8, save) &&
        range(&bindings.services, service) &&
        !overlap(class_id, save) && !overlap(class_id, service) &&
        !overlap(save, service);
}

bool separate(const Bindings& bindings, Range region) {
    Range class_id{}, save{}, service{};
    return range(bindings.property_id_13c8, class_id) &&
        range(bindings.current_save_14e8, save) &&
        range(&bindings.services, service) &&
        !overlap(region, class_id) && !overlap(region, save) &&
        !overlap(region, service);
}

struct Calls {
    Bindings& bindings;
    Result& result;
    std::string& error;

    bool invoke(Operation operation, std::uintptr_t subject,
                std::uint32_t callsite, std::uint32_t ordinal,
                std::int32_t argument, Reply& reply) {
        if (!valid(bindings)) {
            if (error.empty()) error = "InitPost borrowed source bindings changed";
            return false;
        }
        if (!bindings.services.invoke) {
            if (error.empty()) error = "InitPost reached an unavailable source provider";
            return false;
        }
        Request request{operation, subject, callsite, ordinal, argument};
        reply = {};
        ++result.provider_calls;
        try {
            if (bindings.services.invoke(bindings.services.context, request, reply, error)) {
                if (error.empty()) error = "InitPost source provider failed";
                return false;
            }
        } catch (const std::exception& exception) {
            if (error.empty()) error = exception.what();
            return false;
        } catch (...) {
            if (error.empty()) error = "InitPost source provider threw";
            return false;
        }
        if (!valid(bindings)) {
            if (error.empty()) error = "InitPost provider replaced borrowed source bindings";
            return false;
        }
        return true;
    }
};
}

Runtime::Runtime(Bindings bindings) : bindings_(bindings) {
    if (!valid(bindings_)) throw std::invalid_argument("Invalid borrowed InitPost source bindings");
}

Status Runtime::initialize(Result* output, std::string& error) {
    if (busy_) return Status::busy;
    Range result_range{}, error_range{}, runtime_range{};
    if (!valid(bindings_) || !range(output, result_range) || !range(&error, error_range) ||
        !range(this, runtime_range) || overlap(result_range, error_range) ||
        overlap(result_range, runtime_range) || overlap(error_range, runtime_range) ||
        !separate(bindings_, result_range) || !separate(bindings_, error_range))
        return Status::invalid_argument;

    *output = {};
    error.clear();
    output->captured_character = bindings_.character;
    output->captured_property_owner = bindings_.property_owner;
    busy_ = true;
    struct BusyGuard { bool& value; ~BusyGuard() { value = false; } } guard{busy_};
    Calls calls{bindings_, *output, error};
    Reply reply{};
    const auto fail = [&]() { return Status::failed; };

    output->stage = Stage::load_save_mask;
    if (!calls.invoke(Operation::load_save_mask, bindings_.character, 0x3b513c, 0, 4, reply)) return fail();
    ++output->save_load_calls;

    output->stage = Stage::current_level;
    if (!calls.invoke(Operation::current_level, 0, 0x3b5148, 0, 0, reply)) return fail();
    if (reply.identity) {
        output->stage = Stage::set_difficulty;
        if (!calls.invoke(Operation::set_difficulty, bindings_.character, 0x3b515c, 0,
                          static_cast<std::int32_t>(reply.word), reply)) return fail();
        output->difficulty_set = true;
    }

    output->stage = Stage::local_player_equipment_query;
    ++output->locality_queries;
    if (!calls.invoke(Operation::is_local_player, bindings_.character, 0x3b5170, 1, 0, reply)) return fail();
    if (reply.word) {
        output->stage = Stage::init_equipment;
        if (!calls.invoke(Operation::init_equipment, bindings_.character, 0x3b54e0, 0, 0, reply)) return fail();
        output->equipment_initialized = true;

        output->stage = Stage::reset_gear_properties;
        if (!calls.invoke(Operation::reset_gear_properties, bindings_.property_owner, 0x3b54e8, 0, 0, reply)) return fail();

        // The original directly reloads Character+0x14e8 after equipment and
        // gear reset. Keep this as a fresh read, not a captured constructor arg.
        output->captured_save = *bindings_.current_save_14e8;
        if (output->captured_save) {
            output->stage = Stage::quest_sync;
            if (!calls.invoke(Operation::quest_sync, output->captured_save, 0x3b54fc, 0, 0, reply)) return fail();
        }
    }

    output->stage = Stage::load_base_properties;
    output->property_id = *bindings_.property_id_13c8;
    if (!calls.invoke(Operation::load_base_properties, bindings_.property_owner, 0x3b5188, 0,
                      output->property_id, reply)) return fail();

    output->stage = Stage::load_gear_properties;
    if (!calls.invoke(Operation::load_gear_properties, bindings_.property_owner, 0x3b5190, 0, 0, reply)) return fail();

    output->stage = Stage::recalc_properties;
    if (!calls.invoke(Operation::recalc_properties, bindings_.property_owner, 0x3b519c, 0, 1, reply)) return fail();

    output->stage = Stage::local_player_skills_query;
    ++output->locality_queries;
    if (!calls.invoke(Operation::is_local_player, bindings_.character, 0x3b51b0, 2, 0, reply)) return fail();
    if (reply.word) {
        output->stage = Stage::init_skill_slots;
        if (!calls.invoke(Operation::init_skill_slots, bindings_.character, 0x3b54d4, 0, 0, reply)) return fail();
        output->skills_initialized = true;
    }

    output->stage = Stage::covered_cutoff;
    return Status::complete;
}

} // namespace dh2::character_init_post_player_v1
