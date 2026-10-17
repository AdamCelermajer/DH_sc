#include "character_current_equipped_faery_v1.hpp"

#include <cstdio>
#include <cstring>

namespace dh2::character_current_equipped_faery_v1 {
namespace {
struct Range { std::uintptr_t begin, end; };

bool range(const void* pointer, std::size_t size, std::size_t alignment,
           Range& result) noexcept {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size) return false;
    result = {begin, begin + size};
    return true;
}

bool overlaps(Range a, Range b) noexcept {
    return a.begin < b.end && b.begin < a.end;
}

template<class T>
bool span(const T* pointer, Range& result) noexcept {
    return range(pointer, sizeof(T), alignof(T), result);
}

int fail(char* text, std::size_t size, const char* reason) noexcept {
    if (text && size) std::snprintf(text, size, "%s", reason);
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}

Status query(std::uintptr_t character,
             const character_current_spell_v1::Services* services,
             Result* result, bool read_level) {
    Range service_range{}, result_range{};
    if (!character || !span(services, service_range) || !span(result, result_range) ||
        overlaps(service_range, result_range) || !services->invoke)
        return Status::invalid_argument;

    const auto bound = *services;
    *result = {};
    result->character = character;

    const auto invoke = [&](character_current_spell_v1::Operation operation,
                            std::uint32_t id,
                            character_current_spell_v1::Response& response) {
        ++result->calls;
        const character_current_spell_v1::Request request{operation, id, -1, character};
        response = {};
        try {
            return bound.invoke(bound.context, &request, &response) == 0;
        } catch (...) {
            return false;
        }
    };

    character_current_spell_v1::Response response{};
    if (!invoke(character_current_spell_v1::Operation::selected_faery, 0, response))
        return Status::provider_failed;
    result->faery_id = response.value;

    if (read_level) {
        if (!invoke(character_current_spell_v1::Operation::saved_level,
                    static_cast<std::uint32_t>(result->faery_id), response))
            return Status::provider_failed;
        result->level = response.value;
    }

    result->complete = 1;
    return Status::complete;
}

int callback(void* raw, const dh2_script_value*, std::uint32_t,
             dh2_script_value* output, std::uint32_t capacity,
             std::uint32_t* returned, char* text, std::size_t text_size,
             bool read_level) noexcept {
    auto* bindings = static_cast<Bindings*>(raw);
    Range binding_range{}, returned_range{}, output_range{};
    if (!span(bindings, binding_range) || !span(returned, returned_range) ||
        !capacity || !span(output, output_range) || overlaps(binding_range, returned_range) ||
        overlaps(binding_range, output_range) || overlaps(returned_range, output_range))
        return fail(text, text_size, "invalid current-equipped-faery callback controls");

    *returned = 0;
    Result result{};
    const auto status = query(bindings->character, &bindings->services, &result, read_level);
    if (status != Status::complete)
        return fail(text, text_size,
                    "current-equipped-faery callback requires the retained save provider");

    output[0] = {};
    output[0].type = DH2_SCRIPT_NUMBER;
    output[0].number = static_cast<float>(read_level ? result.level : result.faery_id);
    *returned = 1;
    return 0;
}
}  // namespace

Status current_equipped_faery_id(
    std::uintptr_t character,
    const character_current_spell_v1::Services* services,
    Result* result) {
    return query(character, services, result, false);
}

Status current_equipped_faery_level(
    std::uintptr_t character,
    const character_current_spell_v1::Services* services,
    Result* result) {
    return query(character, services, result, true);
}

int current_equipped_faery_id_v1(void* bindings, const dh2_script_value* arguments,
                                 std::uint32_t argument_count, dh2_script_value* output,
                                 std::uint32_t capacity, std::uint32_t* returned,
                                 char* text, std::size_t text_size) noexcept {
    return callback(bindings, arguments, argument_count, output, capacity, returned,
                    text, text_size, false);
}

int current_equipped_faery_level_v1(void* bindings, const dh2_script_value* arguments,
                                    std::uint32_t argument_count, dh2_script_value* output,
                                    std::uint32_t capacity, std::uint32_t* returned,
                                    char* text, std::size_t text_size) noexcept {
    return callback(bindings, arguments, argument_count, output, capacity, returned,
                    text, text_size, true);
}

}  // namespace dh2::character_current_equipped_faery_v1
