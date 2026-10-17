#include "character_aggro_target_search.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>

namespace dh2::character::aggro_search {
namespace {

bool aligned(const void* pointer, std::uintptr_t alignment = 8) {
    return pointer && (reinterpret_cast<std::uintptr_t>(pointer) & (alignment - 1U)) == 0;
}

bool valid_services(const Services* services) {
    return aligned(services) && services->invoke;
}

bool valid_list(const TargetList* list) {
    return aligned(list) && aligned(list->heap) && list->capacity > 0 &&
           list->capacity <= 65536 && list->count <= list->capacity &&
           aligned(list->owner) && list->owner->object &&
           list->sort_type == kSourceSortClosest && list->reserved == 0;
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
    // Source TargetSorter::_sortClosest compares float distance strictly.
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

bool contains_range(const void* pointer, std::size_t bytes, const void* other,
                    std::size_t other_bytes) {
    const auto start = reinterpret_cast<std::uintptr_t>(pointer);
    const auto other_start = reinterpret_cast<std::uintptr_t>(other);
    if (start > UINTPTR_MAX - bytes || other_start > UINTPTR_MAX - other_bytes) return true;
    return bytes && other_bytes && start < other_start + other_bytes &&
           other_start < start + bytes;
}

int reject_source_search(TargetList* list, const RoomRegistry* registry,
                         float radius, float cone, const Services* services) {
    if (!valid_list(list) || !aligned(registry) || !aligned(registry->rooms) ||
        !valid_services(services) || !std::isfinite(radius) ||
        !std::isfinite(cone) || cone < 0.0f ||
        list->capacity > SIZE_MAX / sizeof(TargetInfo)) {
        return invalid_argument;
    }
    const auto heap_bytes = static_cast<std::size_t>(list->capacity) * sizeof(TargetInfo);
    if (contains_range(list, sizeof(*list), registry, sizeof(*registry)) ||
        contains_range(list, sizeof(*list), services, sizeof(*services)) ||
        contains_range(list->heap, heap_bytes, registry, sizeof(*registry)) ||
        contains_range(list->heap, heap_bytes, services, sizeof(*services))) {
        return invalid_argument;
    }
    return complete;
}

}  // namespace

extern "C" int dh2_aggro_target_list_init(TargetList* list, TargetInfo* heap,
                                            std::uint32_t capacity,
                                            Character* owner) {
    if (!aligned(list) || !aligned(heap) || !aligned(owner) ||
        !aligned(owner->object) || !owner->identity || !owner->object->identity ||
        capacity == 0 || capacity > 65536 ||
        capacity > SIZE_MAX / sizeof(TargetInfo)) {
        return invalid_argument;
    }
    const auto heap_bytes = static_cast<std::size_t>(capacity) * sizeof(TargetInfo);
    if (contains_range(list, sizeof(*list), heap, heap_bytes) ||
        contains_range(list, sizeof(*list), owner, sizeof(*owner)) ||
        contains_range(heap, heap_bytes, owner, sizeof(*owner)) ||
        contains_range(heap, heap_bytes, owner->object, sizeof(*owner->object))) {
        return invalid_argument;
    }
    *list = {heap, 0, capacity, owner, kSourceSortClosest, 0};
    return complete;
}

extern "C" int dh2_aggro_target_search(TargetList* list,
                                         const RoomRegistry* registry,
                                         float view_radius, float cone,
                                         const Services* services) {
    const auto validated = reject_source_search(list, registry, view_radius, cone, services);
    if (validated != complete) return validated;

    // SearchEff supplies the owner's source target position and look vector.
    // `has_target_position` and `forward` are snapshots from those getters.
    const auto* owner_object = list->owner->object;
    const float* origin = target_center(owner_object);
    const float* forward = owner_object->forward;
    Response response{};
    if (!invoke(services, ai_melee_radius, list->owner->identity, 0, response)) {
        return source_service_failed;
    }
    const float owner_radius = response.number;

    while (list->count) heap_pop(list);
    auto sentinel = registry->rooms;
    auto room = sentinel->next;
    std::uint32_t visited_rooms = 0;
    if (!aligned(room)) return invalid_topology;
    auto* entry = room == sentinel ? nullptr :
                  (room->objects ? room->objects->next : nullptr);

    while (room != sentinel) {
        if (++visited_rooms > 65536 || !aligned(room) ||
            !aligned(room->objects) || !aligned(entry)) {
            return invalid_topology;
        }
        if (entry == room->objects) {
            room = room->next;
            if (!aligned(room)) return invalid_topology;
            entry = room == sentinel ? nullptr :
                    (room->objects ? room->objects->next : nullptr);
            continue;
        }

        auto* object = entry->object;
        Response resolved{};
        if (!invoke(services, resolve_character,
                    object ? object->identity : 0, 0, resolved)) {
            return source_service_failed;
        }
        auto* character = reinterpret_cast<Character*>(resolved.word);
        if (character && !aligned(character)) return invalid_topology;

        if (object && object != owner_object && object->visible && character) {
            if (!aligned(object) || !aligned(character->object) ||
                character->object != object || !character->identity) {
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
                if (can_interact.word &&
                    list->owner->source_word_1314 >= character->source_word_1310) {
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
                        constexpr float pi = 3.1415927410125732421875f;
                        if (!(cone < pi && cone < target_angle)) {
                            if (list->count >= list->capacity) return capacity_exhausted;
                            heap_push(list, {object->identity, character->identity,
                                             adjusted_distance, target_angle, 1U, 0U});
                        }
                    }
                }
            }
        }
        // RoomObjectList::Next reads the current intrusive next link after
        // candidate callbacks, rather than snapshotting the full room list.
        entry = entry->next;
    }
    return complete;
}

extern "C" int dh2_aggro_target_pop(TargetList* list, TargetInfo* output) {
    if (!valid_list(list) || !aligned(output) ||
        contains_range(output, sizeof(*output), list, sizeof(*list)) ||
        contains_range(output, sizeof(*output), list->heap,
                       static_cast<std::size_t>(list->capacity) * sizeof(TargetInfo))) {
        return invalid_argument;
    }
    if (!list->count) return 1;
    *output = list->heap[0];
    heap_pop(list);
    return complete;
}

}  // namespace dh2::character::aggro_search
