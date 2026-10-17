#include "character_aggro_character_list.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>

namespace dh2::character::aggro_character_list {
namespace {

using namespace aggro_search;

bool aligned(const void* pointer, std::uintptr_t alignment = 8) {
    return pointer && (reinterpret_cast<std::uintptr_t>(pointer) & (alignment - 1U)) == 0;
}

bool valid_list(const TargetList* list) {
    return aligned(list) && aligned(list->heap) && list->capacity > 0 &&
           list->capacity <= 65536 && list->count <= list->capacity &&
           aligned(list->owner) && list->owner->object &&
           list->sort_type == kSourceSortClosest && list->reserved == 0;
}

bool valid_services(const Services* services) {
    return aligned(services) && services->invoke;
}

bool invoke(const Services* services, Operation operation,
            std::uintptr_t subject, std::uintptr_t other, Response& output) {
    output = {};
    const Request request{static_cast<std::uint32_t>(operation), 0, subject, other};
    return services->invoke(services->context, &request, &output) == 0;
}

float add(float a, float b) {
    volatile float result = a + b;
    return result;
}

float sub(float a, float b) {
    volatile float result = a - b;
    return result;
}

float mul(float a, float b) {
    volatile float result = a * b;
    return result;
}

float divide(float a, float b) {
    volatile float result = a / b;
    return result;
}

float length(const float* vector) {
    return std::sqrt(add(add(mul(vector[0], vector[0]),
                             mul(vector[1], vector[1])),
                         mul(vector[2], vector[2])));
}

float angle(const float* first, const float* second) {
    const float dot = add(add(mul(first[0], second[0]),
                              mul(first[1], second[1])),
                          mul(first[2], second[2]));
    float result = std::acos(divide(dot, mul(length(first), length(second))));
    std::uint32_t bits = 0;
    std::memcpy(&bits, &result, sizeof(bits));
    bits &= UINT32_C(0x7fffffff);
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

const float* target_center(const GameObject* object) {
    return object->has_target_position ? object->target_position : object->position;
}

bool lower_priority(const TargetInfo& first, const TargetInfo& second) {
    if ((first.flags & 1U) != (second.flags & 1U)) {
        return (second.flags & 1U) != 0;
    }
    return first.distance > second.distance;
}

void heap_push(TargetList* list, const TargetInfo& value) {
    auto hole = list->count++;
    while (hole) {
        const auto parent = (hole - 1U) / 2U;
        if (!lower_priority(list->heap[parent], value)) break;
        list->heap[hole] = list->heap[parent];
        hole = parent;
    }
    list->heap[hole] = value;
}

void heap_pop(TargetList* list) {
    const auto count = --list->count;
    if (!count) return;
    const auto value = list->heap[count];
    std::uint32_t hole = 0;
    std::uint32_t right = 2;
    while (right < count) {
        const auto child = lower_priority(list->heap[right], list->heap[right - 1U])
                               ? right - 1U : right;
        list->heap[hole] = list->heap[child];
        hole = child;
        right = (child + 1U) * 2U;
    }
    if (right == count) {
        list->heap[hole] = list->heap[right - 1U];
        hole = right - 1U;
    }
    while (hole) {
        const auto parent = (hole - 1U) / 2U;
        if (!lower_priority(list->heap[parent], value)) break;
        list->heap[hole] = list->heap[parent];
        hole = parent;
    }
    list->heap[hole] = value;
}

bool overlaps(const void* a, std::size_t a_size,
              const void* b, std::size_t b_size) {
    const auto a_begin = reinterpret_cast<std::uintptr_t>(a);
    const auto b_begin = reinterpret_cast<std::uintptr_t>(b);
    if (a_begin > UINTPTR_MAX - a_size || b_begin > UINTPTR_MAX - b_size) return true;
    return a_size && b_size && a_begin < b_begin + b_size && b_begin < a_begin + a_size;
}

}  // namespace

extern "C" int dh2_aggro_character_list_init(CharacterList* list, Entry* sentinel) {
    if (!aligned(list) || !aligned(sentinel) || !aligned(sentinel->next)) {
        return invalid_argument;
    }
    *list = {sentinel, sentinel->next, sentinel};
    return complete;
}

extern "C" int dh2_aggro_character_list_reset(CharacterList* list) {
    if (!aligned(list) || !aligned(list->sentinel) ||
        !aligned(list->sentinel->next) || list->end != list->sentinel) {
        return invalid_argument;
    }
    list->current = list->sentinel->next;
    return complete;
}

extern "C" int dh2_aggro_character_list_at_end(const CharacterList* list,
                                                  std::uint32_t* output) {
    if (!aligned(list) || !aligned(output, alignof(std::uint32_t)) ||
        !aligned(list->sentinel) ||
        !aligned(list->current) || list->end != list->sentinel) {
        return invalid_argument;
    }
    *output = list->current == list->end;
    return complete;
}

extern "C" int dh2_aggro_character_list_get(const CharacterList* list,
                                               GameObject** output) {
    if (!aligned(list) || !aligned(output) || !aligned(list->current) ||
        !aligned(list->end) || list->current == list->end) {
        return invalid_argument;
    }
    auto* character = list->current->character;
    *output = character ? character->object : nullptr;
    return complete;
}

extern "C" int dh2_aggro_character_list_get_char(const CharacterList* list,
                                                   Character** output) {
    if (!aligned(list) || !aligned(output) || !aligned(list->current) ||
        !aligned(list->end) || list->current == list->end) {
        return invalid_argument;
    }
    *output = list->current->character;
    return complete;
}

extern "C" int dh2_aggro_character_list_next(CharacterList* list) {
    if (!aligned(list) || !aligned(list->current) || !aligned(list->end) ||
        list->current == list->end || !aligned(list->current->next)) {
        return invalid_argument;
    }
    list->current = list->current->next;
    return complete;
}

extern "C" int dh2_aggro_target_search_character_list(
    TargetList* list, CharacterList* characters, float view_radius, float cone,
    const Services* services) {
    if (!valid_list(list) || !aligned(characters) || !aligned(characters->sentinel) ||
        !aligned(characters->current) || characters->end != characters->sentinel ||
        (list && overlaps(list, sizeof(*list), characters, sizeof(*characters))) ||
        (list && overlaps(list->heap, static_cast<std::size_t>(list->capacity) * sizeof(TargetInfo),
                           characters, sizeof(*characters))) ||
        (services && overlaps(services, sizeof(*services), characters, sizeof(*characters)))) {
        return invalid_argument;
    }
    const ObjectListMethods methods{characters,
        [](void* p) {return dh2_aggro_character_list_reset(static_cast<CharacterList*>(p));},
        [](void* p,std::uint32_t* out) {return dh2_aggro_character_list_at_end(static_cast<CharacterList*>(p),out);},
        [](void* p,GameObject** out) {return dh2_aggro_character_list_get(static_cast<CharacterList*>(p),out);},
        [](void* p,Character** out) {return dh2_aggro_character_list_get_char(static_cast<CharacterList*>(p),out);},
        [](void* p) {return dh2_aggro_character_list_next(static_cast<CharacterList*>(p));}};
    return dh2_aggro_target_search_object_list(list,&methods,view_radius,cone,services);
}

extern "C" int dh2_aggro_target_search_object_list(
    TargetList* list, const ObjectListMethods* methods, float view_radius, float cone,
    const Services* services) {
    if (!valid_list(list) || !aligned(methods) || !methods->context || !methods->reset || !methods->at_end ||
        !methods->get || !methods->get_char || !methods->next ||
        !valid_services(services) || !std::isfinite(view_radius) ||
        !std::isfinite(cone) || cone < 0.0f ||
        overlaps(list, sizeof(*list), methods, sizeof(*methods)) ||
        overlaps(list, sizeof(*list), services, sizeof(*services)) ||
        overlaps(list->heap, static_cast<std::size_t>(list->capacity) * sizeof(TargetInfo),
                 methods, sizeof(*methods)) ||
        overlaps(list->heap, static_cast<std::size_t>(list->capacity) * sizeof(TargetInfo),
                 services, sizeof(*services))) {
        return invalid_argument;
    }

    auto* owner_object = list->owner->object;
    const float* origin = target_center(owner_object);
    const float* forward = owner_object->forward;
    Response response{};
    if (!invoke(services, ai_melee_radius, list->owner->identity, 0, response)) {
        return source_service_failed;
    }
    const float owner_radius = response.number;

    while (list->count) heap_pop(list);
    if (methods->reset(methods->context) != complete) return invalid_topology;
    std::uint32_t visited = 0;
    constexpr float pi = 3.1415927410125732421875f;

    for (;;) {
        std::uint32_t at_end = 0;
        if (methods->at_end(methods->context, &at_end) != complete) {
            return invalid_topology;
        }
        if (at_end) break;
        if (++visited > 65536) return invalid_topology;

        GameObject* object = nullptr;
        Character* character = nullptr;
        if (methods->get(methods->context, &object) != complete ||
            methods->get_char(methods->context, &character) != complete) {
            return invalid_topology;
        }

        if (object && object != owner_object && object->visible) {
            if (!aligned(object)) {
                return invalid_topology;
            }

            Response zonable{};
            if (!invoke(services, is_zonable, object->identity, 0, zonable)) {
                return source_service_failed;
            }
            if (!(zonable.word && object->character_2ee && !object->character_2f0)) {
                Response can_interact{};
                if (!invoke(services, is_interactive, object->identity,
                            owner_object->identity, can_interact)) {
                    return source_service_failed;
                }
                // The original Search visits the visible object's zonable and
                // interactive virtuals before it rejects a non-Character
                // ObjectList entry. Keep those observable calls in source
                // order; Character-only source-word/range work starts here.
                if (can_interact.word && character) {
                    if (!aligned(character->object) || character->object != object ||
                        !character->identity) {
                        return invalid_topology;
                    }
                    if (list->owner->source_word_1314 >= character->source_word_1310) {
                        Response target_radius_result{};
                        if (!invoke(services, interaction_radius, object->identity, 0,
                                    target_radius_result)) {
                            return source_service_failed;
                        }
                        const auto* target = target_center(object);
                        const float delta[3] = {
                            sub(target[0], origin[0]), sub(target[1], origin[1]),
                            sub(target[2], origin[2])
                        };
                        const float adjusted_distance =
                            sub(sub(length(delta), target_radius_result.number), owner_radius);
                        if (!(adjusted_distance > view_radius)) {
                            const float target_angle = angle(delta, forward);
                            if (!(cone < pi && cone < target_angle)) {
                                if (list->count >= list->capacity) return capacity_exhausted;
                                heap_push(list, {object->identity, character->identity,
                                                 adjusted_distance, target_angle, 1U, 0U});
                            }
                        }
                    }
                }
            }
        }

        // CharacterList::Next reads current->next after all candidate
        // callbacks, preserving source live-link advancement rather than a
        // list snapshot taken before the search.
        if (methods->next(methods->context) != complete) {
            return invalid_topology;
        }
    }
    return complete;
}

}  // namespace dh2::character::aggro_character_list
