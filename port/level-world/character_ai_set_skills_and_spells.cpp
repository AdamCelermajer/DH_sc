#include "character_ai_set_skills_and_spells.hpp"

#include <cmath>
#include <cstring>
#include <limits>

namespace dh2::character_ai_set_skills_and_spells {
namespace {
constexpr const char kDebugKey[] = "Lua_LoadMemUsage";
constexpr const char kSkillPath[] = "data/scripts/skills/";
constexpr const char kCommonScript[] = "_commons";
constexpr const char kDeclareSkill[] = "DeclareSkill";
constexpr std::uint32_t kMaximumSourceSlots = 1u << 20;

struct AddressRange { std::uintptr_t first, end; };

bool range(const void* p, std::size_t n, std::size_t alignment, AddressRange& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(p);
    if (!p || first % alignment || first > std::numeric_limits<std::uintptr_t>::max() - n)
        return false;
    out = {first, first + n};
    return true;
}
bool overlaps(AddressRange a, AddressRange b) {
    return a.first < b.end && b.first < a.end;
}

bool vector_size(const ScriptVector& vector, std::size_t& size) {
    if (vector.begin == nullptr && vector.end == nullptr && vector.capacity == nullptr) {
        size = 0;
        return true;
    }
    if (!vector.begin || !vector.end || !vector.capacity) return false;
    const auto begin = reinterpret_cast<std::uintptr_t>(vector.begin);
    const auto end = reinterpret_cast<std::uintptr_t>(vector.end);
    const auto capacity = reinterpret_cast<std::uintptr_t>(vector.capacity);
    if (begin % alignof(std::uintptr_t) || end % alignof(std::uintptr_t) ||
        capacity % alignof(std::uintptr_t) || begin > end || end > capacity ||
        (end - begin) % sizeof(std::uintptr_t) ||
        (capacity - begin) % sizeof(std::uintptr_t)) return false;
    size = (end - begin) / sizeof(std::uintptr_t);
    return true;
}

std::uintptr_t receiver_for(const State& state, Operation operation) {
    switch (operation) {
    case Operation::debug_load:
    case Operation::debug_get_switch:
        return 0; // The provider resolves the live global DebugSwitches owner.
    case Operation::get_skill_list:
    case Operation::get_skill:
    case Operation::get_faery_list:
    case Operation::get_faery_list_id:
        return state.owner;
    case Operation::reserve:
    case Operation::append_skill_script:
    case Operation::arguments_push_string:
    case Operation::arguments_push_integer:
    case Operation::arguments_set_string:
    case Operation::arguments_set_number:
    case Operation::arguments_destroy:
        return 0; // invoke() supplies the vector/Arguments identity below.
    case Operation::release_script_path:
    case Operation::release_skill_script_allocation:
        return 0; // invoke() supplies the leased path/allocation identity.
    case Operation::allocate_skill_script:
    case Operation::arguments_construct:
        return 0;
    default:
        return state.ai; // Source reloads CharAI+0x1c for each AIS call.
    }
}

Status invoke(State* state, const Services& services, Result& result,
              Operation operation, List list, std::uint32_t slot,
              std::uintptr_t script_name, std::uintptr_t display_name,
              std::uintptr_t arguments, std::uint32_t integer,
              std::uint32_t number_bits, const char* text, std::size_t text_size,
              const char* saved_path = nullptr, std::size_t saved_path_size = 0,
              Response* output = nullptr, std::uint32_t allocation_bytes = 0,
              std::uint32_t allocation_hint = 0) {
    if (!services.invoke) return Status::service_unavailable;
    Request request{};
    request.operation = operation;
    request.list = list;
    request.slot = slot;
    request.receiver = receiver_for(*state, operation);
    if (operation == Operation::reserve || operation == Operation::append_skill_script ||
        operation == Operation::arguments_push_string ||
        operation == Operation::arguments_push_integer ||
        operation == Operation::arguments_set_string ||
        operation == Operation::arguments_set_number ||
        operation == Operation::arguments_destroy)
        request.receiver = arguments;
    if (operation == Operation::release_script_path)
        request.receiver = script_name;
    if (operation == Operation::release_skill_script_allocation)
        request.receiver = script_name;
    request.script_name = script_name;
    request.display_name = display_name;
    request.arguments = arguments;
    request.integer = integer;
    request.number_bits = number_bits;
    request.allocation_bytes = allocation_bytes;
    request.allocation_hint = allocation_hint;
    request.text = text;
    request.text_size = text_size;
    request.saved_path = saved_path;
    request.saved_path_size = saved_path_size;
    Response response{};
    ++result.service_calls;
    result.last_operation = static_cast<std::uint32_t>(operation);
    try {
        if (services.invoke(services.context, state, &request, &response) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    if (output) *output = response;
    return Status::complete;
}

std::uint32_t float_bits(float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

std::int32_t signed_word(std::uint32_t value) {
    std::int32_t result = 0;
    static_assert(sizeof(result) == sizeof(value), "ARM source words are 32 bit");
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

Status check_vector(const ScriptVector& vector, std::size_t& size) {
    if (!vector.identity || !vector_size(vector, size) || size > kMaximumSourceSlots)
        return Status::invalid_source_fact;
    return Status::complete;
}

ScriptVector& vector_for(State& state, List list) {
    return list == List::skill ? state.skills : state.faeries;
}

// The service protocol keeps the actual source vector allocator and pointer
// ownership with its adapter. This helper deliberately rejects successful
// callbacks which did not visibly append the requested full-width pointer.
Status append_source(State* state, const Services& services, Result& result,
                    List list, std::uint32_t slot, std::uintptr_t identity) {
    auto& vector = vector_for(*state, list);
    std::size_t before = 0;
    if (check_vector(vector, before) != Status::complete) return Status::invalid_source_fact;
    if (!services.invoke) return Status::service_unavailable;
    Request request{};
    request.operation = Operation::append_skill_script;
    request.list = list;
    request.slot = slot;
    request.receiver = vector.identity;
    request.arguments = vector.identity;
    request.script_name = identity; // full-width append value
    ++result.service_calls;
    result.last_operation = static_cast<std::uint32_t>(Operation::append_skill_script);
    Response response{};
    try {
        if (services.invoke(services.context, state, &request, &response) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    std::size_t after = 0;
    if (check_vector(vector, after) != Status::complete || after != before + 1 ||
        !vector.begin || vector.begin[before] != identity)
        return Status::service_failed;
    if (identity == 0) ++result.null_appends;
    return Status::complete;
}

Status invoke_void(State* state, const Services& services, Result& result,
                   Operation operation, List list, std::uint32_t slot,
                   std::uintptr_t script_name = 0, std::uintptr_t display_name = 0,
                   std::uintptr_t arguments = 0, std::uint32_t integer = 0,
                   std::uint32_t number_bits = 0, const char* text = nullptr,
                   std::size_t text_size = 0, const char* saved_path = nullptr,
                   std::size_t saved_path_size = 0) {
    return invoke(state, services, result, operation, list, slot, script_name,
                  display_name, arguments, integer, number_bits, text, text_size,
                  saved_path, saved_path_size);
}

Status declare_with_args(State* state, const Services& services, Result& result,
                         List list, std::uint32_t slot, std::uintptr_t script_name,
                         std::uintptr_t display_name, std::uintptr_t arguments,
                         std::uint32_t number_bits) {
    Status status = invoke_void(state, services, result, Operation::arguments_set_string,
                                list, slot, script_name, display_name, arguments,
                                0, 0, nullptr, 0);
    if (status != Status::complete) return status;
    status = invoke_void(state, services, result, Operation::arguments_set_number,
                         list, slot, script_name, display_name, arguments,
                         0, number_bits, nullptr, 0);
    if (status != Status::complete) return status;
    status = invoke_void(state, services, result, Operation::call_script,
                         list, slot, script_name, display_name, arguments,
                         0, 0, kDeclareSkill, sizeof(kDeclareSkill) - 1);
    if (status == Status::complete) ++result.declarations;
    return status;
}

Status create_script(State* state, const Services& services, Result& result,
                     List list, std::uint32_t slot, std::uintptr_t script_name,
                     std::uint32_t constructor_index, std::uintptr_t& created) {
    Response allocated{};
    Status status = invoke(state, services, result,
                           Operation::allocate_skill_script, list, slot,
                           script_name, 0, 0, constructor_index, 0,
                           nullptr, 0, nullptr, 0, &allocated, 0x1cu, 0u);
    if (status != Status::complete) return status;
    if (!allocated.identity) return Status::service_failed;
    auto release_allocation = [&]() {
        Response ignored{};
        (void)invoke(state, services, result,
                     Operation::release_skill_script_allocation, list, slot,
                     allocated.identity, 0, 0, 0x1cu, 0, nullptr, 0,
                     nullptr, 0, &ignored, 0x1cu, 0u);
    };
    if (!services.constructor_services || !services.constructor_globals) {
        release_allocation();
        return Status::service_unavailable;
    }

    character_ai_skill_script_constructor::State child{};
    child.identity = allocated.identity;
    character_ai_skill_script_constructor::Result child_result{};
    auto* globals = const_cast<character_ai_skill_script_constructor::Globals*>(
        services.constructor_globals);
    const char* name = reinterpret_cast<const char*>(script_name);
    const auto child_status = character_ai_skill_script_constructor::construct(
        &child, state->owner, name, constructor_index, globals,
        services.constructor_services, &child_result);
    if (child_status != character_ai_skill_script_constructor::Status::complete) {
        // A C++ new-expression releases its allocation if the constructor
        // throws. Here a failed child status is the adapter's equivalent
        // failure boundary, so request the same cleanup without rolling back
        // any constructor-side writes or completed callbacks.
        release_allocation();
        return Status::child_kernel_failed;
    }
    created = child_result.returned_identity;
    if (!created) return Status::service_failed;
    ++result.script_allocations;
    return Status::complete;
}

Status finish_arguments(State* state, const Services& services, Result& result,
                        List list, std::uintptr_t arguments);

Status setup_arguments(State* state, const Services& services, Result& result,
                       List list, std::uintptr_t& arguments) {
    Response created{};
    Status status = invoke(state, services, result, Operation::arguments_construct,
                           list, 0, 0, 0, 0, 0, 0, nullptr, 0,
                           nullptr, 0, &created);
    if (status != Status::complete) return status;
    if (!created.identity) return Status::service_failed;
    arguments = created.identity;
    status = invoke_void(state, services, result, Operation::arguments_push_string,
                         list, 0, 0, 0, arguments, 0, 0, "", 0);
    if (status == Status::complete)
        status = invoke_void(state, services, result, Operation::arguments_push_integer,
                             list, 0, 0, 0, arguments, 0xffffffffu, 0, nullptr, 0);
    if (status != Status::complete) {
        (void)finish_arguments(state, services, result, list, arguments);
        arguments = 0;
    }
    return status;
}

Status finish_arguments(State* state, const Services& services, Result& result,
                        List list, std::uintptr_t arguments) {
    return invoke_void(state, services, result, Operation::arguments_destroy,
                       list, 0, 0, 0, arguments, 0, 0, nullptr, 0);
}

class ScriptPathLease {
public:
    ScriptPathLease(State* state, const Services& services, Result& result,
                    std::uintptr_t identity, const char* path, std::size_t size)
        : state_(state), services_(&services), result_(&result), identity_(identity),
          path_(path), size_(size), active_(true) {}
    ScriptPathLease(const ScriptPathLease&) = delete;
    ScriptPathLease& operator=(const ScriptPathLease&) = delete;
    ~ScriptPathLease() {
        if (active_) (void)release();
    }
    Status release() {
        if (!active_) return Status::complete;
        active_ = false;
        return invoke_void(state_, *services_, *result_, Operation::release_script_path,
                           List::none, 0, identity_, 0, 0, 0, 0,
                           nullptr, 0, path_, size_);
    }
private:
    State* state_;
    const Services* services_;
    Result* result_;
    std::uintptr_t identity_;
    const char* path_;
    std::size_t size_;
    bool active_;
};

Status load_common(State* state, const Services& services, Result& result,
                   List list, std::uint32_t slot) {
    Response ignored{};
    return invoke(state, services, result, Operation::load_script, list, slot,
                  0, 0, 0, 0, 0, kCommonScript,
                  sizeof(kCommonScript) - 1, nullptr, 0, &ignored);
}

Status script_loaded(State* state, const Services& services, Result& result,
                     List list, std::uint32_t slot, std::uintptr_t script_name,
                     bool& loaded) {
    Response response{};
    Status status = invoke(state, services, result, Operation::load_script, list,
                           slot, script_name, 0, 0, 0, 0, nullptr, 0,
                           nullptr, 0, &response);
    if (status == Status::complete) loaded = response.loaded != 0;
    return status;
}

Status declare_without_args(State* state, const Services& services, Result& result,
                            List list, std::uint32_t slot) {
    Status status = invoke_void(state, services, result, Operation::call_script,
                                list, slot, 0, 0, 0, 0, 0,
                                kDeclareSkill, sizeof(kDeclareSkill) - 1);
    if (status == Status::complete) ++result.declarations;
    return status;
}

Status reserve_vector(State* state, const Services& services, Result& result,
                      List list, std::uint32_t count) {
    auto& vector = vector_for(*state, list);
    const auto before_identity = vector.identity;
    Status status = invoke_void(state, services, result, Operation::reserve,
                                list, 0, 0, 0, before_identity, count, 0,
                                nullptr, 0);
    if (status != Status::complete) return status;
    std::size_t size = 0;
    if (vector.identity != before_identity || check_vector(vector, size) != Status::complete ||
        size != 0 || (count && (!vector.begin || !vector.capacity ||
                              vector.capacity - vector.begin < count)))
        return Status::service_failed;
    return Status::complete;
}

Status build_skill_vector(State* state, const Services& services, Result& result) {
    Response list{};
    Status status = invoke(state, services, result, Operation::get_skill_list,
                           List::skill, 0, 0, 0, 0, 0, 0, nullptr, 0,
                           nullptr, 0, &list);
    if (status != Status::complete) return status;
    if (list.count > kMaximumSourceSlots) return Status::invalid_source_fact;
    status = reserve_vector(state, services, result, List::skill, list.count);
    if (status != Status::complete) return status;
    result.skill_slots = list.count;

    std::uintptr_t arguments = 0;
    status = setup_arguments(state, services, result, List::skill, arguments);
    if (status != Status::complete) return status;
    const auto process = [&]() -> Status {
      for (std::uint32_t slot = 0; slot < list.count; ++slot) {
        Response row_response{};
        status = invoke(state, services, result, Operation::get_skill,
                        List::skill, slot, 0, 0, 0, slot, 0, nullptr, 0,
                        nullptr, 0, &row_response);
        if (status != Status::complete) return status;
        const auto gate = row_response.skill.script_gate;
        const auto script_name = row_response.skill.script_name;
        if (!gate) {
            status = append_source(state, services, result, List::skill, slot, 0);
            if (status != Status::complete) return status;
            continue;
        }

        status = load_common(state, services, result, List::skill, slot);
        if (status != Status::complete) return status;
        status = declare_with_args(state, services, result, List::skill, slot,
                                   script_name, script_name, arguments,
                                   float_bits(static_cast<float>(slot)));
        if (status != Status::complete) return status;
        bool loaded = false;
        status = script_loaded(state, services, result, List::skill, slot,
                               script_name, loaded);
        if (status != Status::complete) return status;
        std::uintptr_t script = 0;
        if (loaded) {
            status = create_script(state, services, result, List::skill, slot,
                                   script_name, slot, script);
            if (status != Status::complete) return status;
        }
        status = append_source(state, services, result, List::skill, slot, script);
        if (status != Status::complete) return status;
        status = declare_without_args(state, services, result, List::skill, slot);
        if (status != Status::complete) return status;
      }
      return Status::complete;
    };
    const Status work = process();
    const Status cleanup = finish_arguments(state, services, result, List::skill, arguments);
    return work == Status::complete ? cleanup : work;
}

Status build_faery_vector(State* state, const Services& services, Result& result) {
    Response list{};
    Status status = invoke(state, services, result, Operation::get_faery_list,
                           List::faery, 0, 0, 0, 0, 0, 0, nullptr, 0,
                           nullptr, 0, &list);
    if (status != Status::complete) return status;
    if (list.count > kMaximumSourceSlots) return Status::invalid_source_fact;
    status = reserve_vector(state, services, result, List::faery, list.count);
    if (status != Status::complete) return status;
    result.faery_slots = list.count;

    std::uintptr_t arguments = 0;
    status = setup_arguments(state, services, result, List::faery, arguments);
    if (status != Status::complete) return status;
    const auto process = [&]() -> Status {
      for (std::uint32_t slot = 0; slot < list.count; ++slot) {
        // The source GetCharFaery wrapper calls GetCharFaeryListId afresh for
        // every slot. Capture the caller's fresh Character pointer before the
        // getter; the source leaf continues using that receiver even if a
        // provider mutates CharAI's owner projection during the call.
        const auto slot_owner = state->owner;
        Response id{};
        status = invoke(state, services, result, Operation::get_faery_list_id,
                        List::faery, slot, 0, 0, 0, slot, 0, nullptr, 0,
                        nullptr, 0, &id);
        if (status != Status::complete) return status;
        auto character = state->faery_binding.character;
        character.identity = slot_owner;
        character.faery_list_id_106c = signed_word(id.word);
        if (!state->faery_binding.globals || !state->faery_binding.services)
            return Status::service_unavailable;
        auto globals = *state->faery_binding.globals;
        globals.assert_level = state->assert_level;
        const auto* const selected_source_rows =
            globals.tables ? globals.tables->faery_rows : nullptr;
        const auto selected_source_count =
            globals.tables ? globals.tables->faery_count : 0u;
        character_faery_selection::Result selected{};
        ++result.faery_selections;
        const auto selector_status = character_faery_selection::select(
            &character, static_cast<std::int32_t>(slot), &globals,
            state->faery_binding.services, &selected);
        if (selector_status != character_faery_selection::Status::complete)
            return selector_status == character_faery_selection::Status::fatal_source_assertion
                       ? Status::fatal_source_assertion : Status::child_kernel_failed;
        if (!selected.row) return Status::invalid_source_fact;
        const auto gate = static_cast<std::uintptr_t>(selected.row->words[5]); // +0x14
        auto script_name = static_cast<std::uintptr_t>(selected.row->words[6]); // +0x18
        if (state->faery_binding.full_width_script_names) {
            const auto& names = *state->faery_binding.full_width_script_names;
            if (!selected_source_rows || names.source_rows != selected_source_rows ||
                !names.values || names.count != selected_source_count)
                return Status::invalid_source_fact;
            const auto rows_begin = reinterpret_cast<std::uintptr_t>(selected_source_rows);
            const auto row_address = reinterpret_cast<std::uintptr_t>(selected.row);
            if (selected_source_count >
                    (std::numeric_limits<std::uintptr_t>::max() - rows_begin) /
                        sizeof(character_faery_selection::FaeryRow) ||
                row_address < rows_begin ||
                row_address - rows_begin >= selected_source_count *
                    sizeof(character_faery_selection::FaeryRow) ||
                (row_address - rows_begin) % sizeof(character_faery_selection::FaeryRow))
                return Status::invalid_source_fact;
            const auto row_index = (row_address - rows_begin) /
                sizeof(character_faery_selection::FaeryRow);
            script_name = names.values[row_index];
        }
        if (!gate) {
            status = append_source(state, services, result, List::faery, slot, 0);
            if (status != Status::complete) return status;
            continue;
        }

        status = load_common(state, services, result, List::faery, slot);
        if (status != Status::complete) return status;
        status = declare_with_args(state, services, result, List::faery, slot,
                                   script_name, script_name, arguments,
                                   float_bits(-1.0f));
        if (status != Status::complete) return status;
        bool loaded = false;
        status = script_loaded(state, services, result, List::faery, slot,
                               script_name, loaded);
        if (status != Status::complete) return status;
        std::uintptr_t script = 0;
        if (loaded) {
            status = create_script(state, services, result, List::faery, slot,
                                   script_name, 0xffffffffu, script);
            if (status != Status::complete) return status;
        }
        status = append_source(state, services, result, List::faery, slot, script);
        if (status != Status::complete) return status;
        status = declare_without_args(state, services, result, List::faery, slot);
        if (status != Status::complete) return status;
      }
      return Status::complete;
    };
    const Status work = process();
    const Status cleanup = finish_arguments(state, services, result, List::faery, arguments);
    return work == Status::complete ? cleanup : work;
}

} // namespace

Status prepare(State* state, const Services* services, Result* result) {
    AddressRange ranges[3];
    if (!range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !range(services, sizeof(*services), alignof(Services), ranges[1]) ||
        !range(result, sizeof(*result), alignof(Result), ranges[2]) ||
        overlaps(ranges[0], ranges[1]) || overlaps(ranges[0], ranges[2]) ||
        overlaps(ranges[1], ranges[2]) || !state->owner ||
        !state->skills.identity || !state->faeries.identity)
        return Status::invalid_argument;
    std::size_t skill_size = 0, faery_size = 0;
    if (check_vector(state->skills, skill_size) != Status::complete ||
        check_vector(state->faeries, faery_size) != Status::complete)
        return Status::invalid_source_fact;

    const Services bound = *services;
    *result = {};
    Result& output = *result;

    // The source asserts on a missing active AIS, then continues into the
    // dereference. Do not claim a normal return for this invalid source state.
    if (!state->ai) {
        return state->assert_level == 2 ? Status::fatal_source_assertion
                                        : Status::invalid_source_fact;
    }
    Status status = invoke_void(state, bound, output, Operation::debug_load,
                                List::none, 0);
    if (status != Status::complete) return status;
    status = invoke_void(state, bound, output, Operation::debug_get_switch,
                         List::none, 0, 0, 0, 0, 0, 0,
                         kDebugKey, sizeof(kDebugKey) - 1);
    if (status != Status::complete) return status;

    Response path{};
    status = invoke(state, bound, output, Operation::capture_script_path,
                    List::none, 0, 0, 0, 0, 0, 0, nullptr, 0,
                    nullptr, 0, &path);
    if (status != Status::complete) return status;
    ScriptPathLease path_lease(state, bound, output, path.identity,
                               path.path, path.path_size);
    if (!path.identity || !path.path || path.path_size > (1u << 20))
        return Status::service_failed;
    status = invoke_void(state, bound, output, Operation::set_script_path,
                         List::none, 0, 0, 0, 0, 0, 0,
                         kSkillPath, sizeof(kSkillPath) - 1);
    if (status != Status::complete) return status;

    if (skill_size == 0) {
        status = build_skill_vector(state, bound, output);
        if (status != Status::complete) return status;
    }
    if (faery_size == 0) {
        status = build_faery_vector(state, bound, output);
        if (status != Status::complete) return status;
    }

    status = invoke_void(state, bound, output, Operation::set_script_path,
                         List::none, 0, 0, 0, 0, 0, 0, nullptr, 0,
                         path.path, path.path_size);
    if (status != Status::complete) return status;
    status = invoke_void(state, bound, output, Operation::debug_load,
                         List::none, 0);
    if (status != Status::complete) return status;
    status = invoke_void(state, bound, output, Operation::debug_get_switch,
                         List::none, 0, 0, 0, 0, 0, 0,
                         kDebugKey, sizeof(kDebugKey) - 1);
    if (status != Status::complete) return status;
    status = invoke_void(state, bound, output, Operation::init_vcb, List::none, 0);
    if (status == Status::complete) {
        output.init_vcb_called = 1;
        status = path_lease.release();
    }
    return status;
}

} // namespace dh2::character_ai_set_skills_and_spells
