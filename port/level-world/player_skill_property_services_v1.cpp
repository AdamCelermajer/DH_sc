#include "player_skill_property_services_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

namespace dh2::player_skill_property_services_v1 {
namespace {
using Function = character_native_bindings::Function;
using data::ClassTables;
using data::PropertySheet;

bool span(const void* p, std::size_t n, std::size_t alignment) noexcept {
    const auto begin = reinterpret_cast<std::uintptr_t>(p);
    return p && alignment && begin % alignment == 0 && begin <= UINTPTR_MAX - n;
}

bool overlaps(const void* a, std::size_t an, const void* b, std::size_t bn) noexcept {
    if (!a || !b || !an || !bn) return false;
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (x > UINTPTR_MAX - an || y > UINTPTR_MAX - bn) return true;
    return x < y + bn && y < x + an;
}

int fail(char* error, std::size_t capacity, const char* message) noexcept {
    if (error && capacity) std::snprintf(error, capacity, "%s", message);
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}

bool no_output_alias(const Bindings& b, const dh2_script_value* args, std::uint32_t count,
                    dh2_script_value* results, std::uint32_t capacity,
                    std::uint32_t* returned) noexcept {
    if (!span(returned, sizeof(*returned), alignof(std::uint32_t))) return false;
    // Every supported source callback takes at most three values and writes
    // at most one. Bound the spans before multiplying so hostile uint32 counts
    // cannot wrap size_t on 32-bit Android builds.
    if (count > 3 || (count && !span(args, std::size_t(count) * sizeof(*args), alignof(dh2_script_value)))) return false;
    const std::size_t result_bytes = capacity ? sizeof(*results) : 0;
    if (capacity && !span(results, result_bytes, alignof(dh2_script_value))) return false;
    const auto state_bytes = sizeof(data::PropertyState);
    const auto sheet_bytes = sizeof(PropertySheet);
    const auto rules_bytes = sizeof(data::PropertyRules);
    const auto class_bytes = sizeof(ClassTables);
    if (overlaps(returned, sizeof(*returned), args, std::size_t(count) * sizeof(*args)) ||
        overlaps(returned, sizeof(*returned), results, result_bytes) ||
        overlaps(returned, sizeof(*returned), &b, sizeof(b)) ||
        overlaps(returned, sizeof(*returned), b.state, state_bytes) ||
        overlaps(returned, sizeof(*returned), b.shared_temp, sheet_bytes) ||
        overlaps(returned, sizeof(*returned), b.rules, rules_bytes) ||
        overlaps(returned, sizeof(*returned), b.classes, class_bytes) ||
        overlaps(returned, sizeof(*returned), b.owner_view, sizeof(data::PropertyView))) return false;
    if (overlaps(results, result_bytes, args, std::size_t(count) * sizeof(*args)) ||
        overlaps(results, result_bytes, &b, sizeof(b)) ||
        overlaps(results, result_bytes, b.state, state_bytes) ||
        overlaps(results, result_bytes, b.shared_temp, sheet_bytes) ||
        overlaps(results, result_bytes, b.rules, rules_bytes) ||
        overlaps(results, result_bytes, b.classes, class_bytes) ||
        overlaps(results, result_bytes, b.owner_view, sizeof(data::PropertyView))) return false;
    return true;
}

bool valid_bindings(const Bindings* b) noexcept {
    if (!span(b, sizeof(*b), alignof(Bindings)) || !b->character || !b->rules || !b->state || !b->shared_temp)
        return false;
    if (!span(b->rules, sizeof(*b->rules), alignof(data::PropertyRules)) ||
        !span(b->state, sizeof(*b->state), alignof(data::PropertyState)) ||
        !span(b->shared_temp, sizeof(*b->shared_temp), alignof(PropertySheet))) return false;
    if (b->owner_view &&
        (!span(b->owner_view, sizeof(*b->owner_view), alignof(data::PropertyView)) ||
         overlaps(b->owner_view, sizeof(*b->owner_view), b, sizeof(*b)) ||
         overlaps(b->owner_view, sizeof(*b->owner_view), b->state, sizeof(*b->state)) ||
         overlaps(b->owner_view, sizeof(*b->owner_view), b->rules, sizeof(*b->rules)) ||
         overlaps(b->owner_view, sizeof(*b->owner_view), b->shared_temp, sizeof(*b->shared_temp)) ||
         b->owner_view->defaults != b->rules->defaults.data() ||
         b->owner_view->types != b->rules->types.data() ||
         b->owner_view->base != b->state->base.data() ||
         b->owner_view->saved != b->state->saved.data() ||
         b->owner_view->gear != b->state->gear.data() ||
         b->owner_view->resolved != b->state->resolved.data() ||
         dh2_property_validate(b->owner_view))) return false;
    return !overlaps(b->shared_temp, sizeof(*b->shared_temp), b->state, sizeof(*b->state)) &&
           !overlaps(b->shared_temp, sizeof(*b->shared_temp), b->rules, sizeof(*b->rules));
}

bool number_to_i32(const dh2_script_value& value, std::int32_t& out) noexcept {
    if (value.type != DH2_SCRIPT_NUMBER || !std::isfinite(value.number) ||
        value.number < -2147483648.0f || value.number >= 2147483648.0f) return false;
    out = static_cast<std::int32_t>(value.number); // source __aeabi_f2iz for its defined range
    return true;
}

bool property_id(const dh2_script_value& value, std::uint32_t& out) noexcept {
    std::int32_t n = 0;
    if (!number_to_i32(value, n) || n < 0 || n >= 224) return false;
    out = static_cast<std::uint32_t>(n);
    return true;
}

bool source_skill_property(const data::PropertyRules& rules, std::uint32_t id) noexcept {
    return id < 224 && rules.types[id] == 8;
}

void number_result(dh2_script_value& out, std::int32_t value) noexcept {
    out = {};
    out.type = DH2_SCRIPT_NUMBER;
    out.number = static_cast<float>(value);
}

struct Busy {
    Bindings& bindings;
    explicit Busy(Bindings& b) noexcept : bindings(b) { bindings.busy = true; }
    ~Busy() { bindings.busy = false; }
};
}

int invoke(Bindings* bindings, std::uintptr_t character, Function function,
           const dh2_script_value* arguments, std::uint32_t argument_count,
           dh2_script_value* results, std::uint32_t result_capacity,
           std::uint32_t* result_count,
           char* error_text, std::size_t error_capacity) noexcept {
    if (!result_count || !span(result_count, sizeof(*result_count), alignof(std::uint32_t)))
        return fail(error_text, error_capacity, "invalid skill-property result count");
    if (!bindings || !valid_bindings(bindings) ||
        !no_output_alias(*bindings, arguments, argument_count, results, result_capacity, result_count))
        return fail(error_text, error_capacity, "invalid or aliased skill-property controls");
    *result_count = 0;
    if (bindings->busy) return fail(error_text, error_capacity, "reentrant skill-property callback");
    if (character != bindings->character) return fail(error_text, error_capacity, "skill-property Character changed");
    if ((argument_count && !arguments) || argument_count > 3)
        return fail(error_text, error_capacity, "invalid skill-property argument vector");

    bool supported = function == Function::character_set_prop || function == Function::character_get_prop ||
                     function == Function::character_apply_prop_class || function == Function::character_clear_props;
    if (!supported) return fail(error_text, error_capacity, "unsupported Character property callback");

    try {
        Busy busy(*bindings);
        auto owner_view = data::property_view(*bindings->rules, *bindings->state);
        if (dh2_property_validate(&owner_view))
            return fail(error_text, error_capacity, "invalid current Character property view");

        if (function == Function::character_clear_props) {
            if (argument_count == 0) return 0;
            if (argument_count != 1 || arguments[0].type != DH2_SCRIPT_BOOLEAN)
                return fail(error_text, error_capacity, "unsupported ClearProps argument mode");
            if (arguments[0].boolean) *bindings->shared_temp = bindings->state->resolved;
            return 0;
        }

        if (function == Function::character_set_prop) {
            if (argument_count < 2 || argument_count > 3)
                return fail(error_text, error_capacity, "unsupported SetProp arity");
            if (argument_count == 3 && arguments[2].type == DH2_SCRIPT_IDENTITY)
                return fail(error_text, error_capacity, "external SetProp sheet identity is unresolved");
            std::uint32_t id = 0;
            std::int32_t value = 0;
            if (!property_id(arguments[0], id) || !number_to_i32(arguments[1], value) ||
                !source_skill_property(*bindings->rules, id))
                return fail(error_text, error_capacity, "unsupported SetProp value or property");
            if (dh2_property_set(&owner_view, static_cast<std::int32_t>(id), value))
                return fail(error_text, error_capacity, "source SetProp rejected property view");
            return 0;
        }

        if (function == Function::character_get_prop) {
            if (argument_count < 1 || argument_count > 2 || result_capacity < 1 || !results)
                return fail(error_text, error_capacity, "unsupported GetProp arity/output");
            std::uint32_t id = 0;
            // Source _GetProp delegates valid property IDs to CharProperties
            // _GetProperty without the type-8 restriction used by SetProp.
            // Keep the schema range check, but expose every property on the
            // owner sheet (or the caller's shared temp sheet) through this
            // same authoritative adapter.
            if (!property_id(arguments[0], id))
                return fail(error_text, error_capacity, "unsupported GetProp property ID");
            bool from_temp = false;
            if (argument_count == 2) {
                if (arguments[1].type == DH2_SCRIPT_IDENTITY)
                    return fail(error_text, error_capacity, "external GetProp sheet identity is unresolved");
                if (arguments[1].type != DH2_SCRIPT_BOOLEAN)
                    return fail(error_text, error_capacity, "unsupported GetProp sheet selector");
                from_temp = arguments[1].boolean != 0;
            }
            std::int32_t value = 0;
            if (from_temp) value = (*bindings->shared_temp)[id];
            else value = bindings->state->resolved[id];
            number_result(results[0], value);
            *result_count = 1;
            return 0;
        }

        if (argument_count < 1 || argument_count > 2 || !bindings->classes)
            return fail(error_text, error_capacity, "unsupported ApplyPropClass arity/dependency");
        std::int32_t class_id = 0;
        if (!number_to_i32(arguments[0], class_id) || class_id < 0 ||
            bindings->classes->rows.empty() || bindings->classes->rows.size() > 10000 ||
            static_cast<std::size_t>(class_id) >= bindings->classes->rows.size())
            return fail(error_text, error_capacity, "unsupported ApplyPropClass class/table");
        bool to_temp = false;
        if (argument_count == 2) {
            if (arguments[1].type == DH2_SCRIPT_IDENTITY)
                return fail(error_text, error_capacity, "external ApplyPropClass sheet identity is unresolved");
            if (arguments[1].type != DH2_SCRIPT_BOOLEAN)
                return fail(error_text, error_capacity, "unsupported ApplyPropClass sheet selector");
            to_temp = arguments[1].boolean != 0;
        }
        PropertySheet& target = to_temp ? *bindings->shared_temp : bindings->state->resolved;
        if (!to_temp && !bindings->owner_view)
            return fail(error_text, error_capacity, "owner ApplyPropClass requires the live property view");
        // PROPS_ApplyClass selects the resolved owner sheet with buff=false,
        // or the shared s_temp sheet with buff=true. _LoadClass accepts every
        // formula destination; property type flags do not gate these raw
        // sheet writes. In particular Rogue Roundhouse writes Critical(63).
        // Use the sole selected class kernel directly so a reached malformed
        // formula retains preceding source writes instead of rolling back.
        std::vector<data::ClassRow> rows;
        rows.reserve(bindings->classes->rows.size());
        for (const auto& row : bindings->classes->rows) {
            if (row.size() > 10000)
                return fail(error_text, error_capacity, "source skill ApplyPropClass row outside limit");
            rows.push_back({row.data(), static_cast<std::uint32_t>(row.size())});
        }
        const auto status = to_temp ?
            dh2_class_apply(rows.data(), static_cast<std::uint32_t>(rows.size()), class_id,
                            target.data(), bindings->state->resolved.data()) :
            dh2_class_apply_to_resolved(rows.data(), static_cast<std::uint32_t>(rows.size()),
                                        class_id, target.data(), bindings->owner_view);
        if (status)
            return fail(error_text, error_capacity, "source skill ApplyPropClass failed");
        return 0;
    } catch (...) {
        return fail(error_text, error_capacity, "skill-property provider exception");
    }
}

} // namespace dh2::player_skill_property_services_v1
