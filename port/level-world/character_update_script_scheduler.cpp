#include "character_update_script_scheduler.hpp"

#include <limits>

namespace dh2::character_update_script_scheduler {
namespace {
struct Range { std::uintptr_t begin, end; };

template<class T> bool object_range(const T* pointer, Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignof(T) ||
        begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(T))
        return false;
    out = {begin, begin + sizeof(T)};
    return true;
}

bool byte_range(const void* pointer, std::size_t bytes, Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || bytes == 0 ||
        begin > std::numeric_limits<std::uintptr_t>::max() - bytes)
        return false;
    out = {begin, begin + bytes};
    return true;
}

bool overlaps(Range a, Range b) {
    return a.begin < b.end && b.begin < a.end;
}

bool overlaps_any(Range value, const Range* ranges, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i)
        if (overlaps(value, ranges[i])) return true;
    return false;
}

struct ActiveFrame {
    const Character* character;
    const ConcurrentMap* map;
    ActiveFrame* previous;
};
thread_local ActiveFrame* active_frames = nullptr;

struct ActiveGuard {
    ActiveFrame frame;
    ActiveGuard(const Character* character, const ConcurrentMap* map)
        : frame{character, map, active_frames} { active_frames = &frame; }
    ~ActiveGuard() { active_frames = frame.previous; }
};

bool active(const Character* character, const ConcurrentMap* map) {
    for (auto* frame = active_frames; frame; frame = frame->previous)
        if (frame->character == character || frame->map == map) return true;
    return false;
}

struct ProjectionRanges {
    Range character;
    Range map;
    Range services;
    Range result;
    Range context;
    Range entries;
};

bool valid_projection_ranges(const Character* character,
                             const ConcurrentMap* map,
                             const Services* services, const Result* result,
                             const Services& bound, ProjectionRanges& ranges) {
    if (!object_range(character, ranges.character) ||
        !object_range(map, ranges.map) ||
        !object_range(services, ranges.services) ||
        !object_range(result, ranges.result)) return false;
    const Range primary[]{ranges.character, ranges.map, ranges.services,
                          ranges.result};
    for (std::size_t i = 0; i < 4; ++i)
        for (std::size_t j = i + 1; j < 4; ++j)
            if (overlaps(primary[i], primary[j])) return false;

    if (bound.context_extent == 0) {
        if (bound.context != nullptr) return false;
    } else {
        if (!byte_range(bound.context, bound.context_extent, ranges.context) ||
            overlaps_any(ranges.context, primary, 4)) return false;
    }

    // Only after the map descriptor is known to be aligned and isolated may
    // its size/capacity/backing fields be read.
    if (map->size > map->capacity) return false;
    if (map->capacity == 0) {
        if (map->entries != nullptr || map->entries_extent != 0) return false;
        ranges.entries = {0, 0};
    } else {
        // capacity is nonzero here. Check multiplication in size_t without a
        // tautological uint32-to-64-bit bound comparison on Android Clang.
        if (sizeof(Entry) > std::numeric_limits<std::size_t>::max() /
                                static_cast<std::size_t>(map->capacity)) return false;
        const auto bytes = static_cast<std::size_t>(map->capacity) * sizeof(Entry);
        if (map->entries_extent != bytes ||
            !byte_range(map->entries, map->entries_extent, ranges.entries) ||
            ranges.entries.begin % alignof(Entry) ||
            overlaps_any(ranges.entries, primary, 4) ||
            (bound.context_extent != 0 && overlaps(ranges.entries, ranges.context)))
            return false;
    }
    return true;
}

bool valid_map_facts(const Character* current, const ConcurrentMap* map,
                     const Services& bound, const ProjectionRanges& ranges,
                     const Entry* expected_entries,
                     std::uint32_t expected_capacity,
                     std::size_t expected_extent) {
    if (map->entries != expected_entries || map->capacity != expected_capacity ||
        map->entries_extent != expected_extent || map->size > map->capacity)
        return false;
    if (map->size != 0 && !map->entries) return false;
    for (std::uint32_t i = 0; i < map->size; ++i) {
        const Entry& entry = map->entries[i];
        Range char_range{};
        if (!object_range(entry.character, char_range)) return false;
        const bool is_current = entry.character == current;
        if ((!is_current && overlaps(char_range, ranges.character)) ||
            overlaps(char_range, ranges.map) ||
            overlaps(char_range, ranges.services) ||
            overlaps(char_range, ranges.result) ||
            (bound.context_extent != 0 && overlaps(char_range, ranges.context)) ||
            (ranges.entries.end != 0 && overlaps(char_range, ranges.entries)))
            return false;
        for (std::uint32_t j = 0; j < i; ++j) {
            if (entry.character == map->entries[j].character) continue;
            Range prior{};
            if (!object_range(map->entries[j].character, prior) ||
                overlaps(char_range, prior)) return false;
        }
        if (!entry.character->identity ||
            (i != 0 && map->entries[i - 1].key >= entry.key)) return false;
    }
    return true;
}

Status call(const Services& services, Character* character,
            ConcurrentMap* map, Result& result, Operation operation,
            Character* subject_character, std::uintptr_t subject_identity,
            std::uint32_t argument0, std::uint32_t argument1,
            Response& response) {
    if (!services.invoke) return Status::service_unavailable;
    const Request request{operation, subject_character, subject_identity,
                          argument0, argument1};
    response = {};
    ++result.service_calls;
    try {
        return services.invoke(services.context, character, map, &request,
                               &response) == 0
            ? Status::complete : Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
}

Status failed(Status status, ConcurrentMap* map, Result& result) {
    result.map_size_after = map ? map->size : 0;
    return status;
}

std::uint32_t lower_bound(const ConcurrentMap& map, std::int32_t key) {
    std::uint32_t low = 0, high = map.size;
    while (low < high) {
        const std::uint32_t middle = low + (high - low) / 2;
        if (map.entries[middle].key < key) low = middle + 1;
        else high = middle;
    }
    return low;
}

std::int32_t signed_word(std::uint32_t raw) {
    const std::int64_t signed_value = raw <= 0x7fffffffu
        ? static_cast<std::int64_t>(raw)
        : static_cast<std::int64_t>(raw) - 0x100000000ll;
    return static_cast<std::int32_t>(signed_value);
}

Status insert_or_replace(ConcurrentMap& map, std::int32_t key,
                         Character* character, bool& replaced) {
    const std::uint32_t at = lower_bound(map, key);
    if (at < map.size && map.entries[at].key == key) {
        // Character::Update writes [returned node + 0x14] after both unique
        // insert helpers, including the already-present-key path.
        map.entries[at].character = character;
        replaced = true;
        return Status::complete;
    }
    if (map.size == map.capacity) return Status::map_storage_exhausted;
    for (std::uint32_t i = map.size; i > at; --i)
        map.entries[i] = map.entries[i - 1];
    map.entries[at] = {key, character};
    ++map.size;
    replaced = false;
    return Status::complete;
}

}  // namespace

Status run(Character* character, ConcurrentMap* map,
           const Services* services, Result* out) {
    Range character_range{}, map_range{}, services_range{}, result_range{};
    if (!object_range(character, character_range) ||
        !object_range(map, map_range) ||
        !object_range(services, services_range) ||
        !object_range(out, result_range) ||
        overlaps(character_range, map_range) ||
        overlaps(character_range, services_range) ||
        overlaps(character_range, result_range) ||
        overlaps(map_range, services_range) ||
        overlaps(map_range, result_range) ||
        overlaps(services_range, result_range))
        return Status::invalid_argument;

    const Services bound = *services;
    ProjectionRanges ranges{};
    if (!valid_projection_ranges(character, map, services, out, bound, ranges))
        return Status::invalid_argument;
    if (active(character, map)) return Status::reentrant_call;
    if (!character->identity || !character->ai)
        return Status::invalid_argument;
    const auto character_identity = character->identity;
    const auto character_ai = character->ai;
    const auto* const map_entries = map->entries;
    const auto map_capacity = map->capacity;
    const auto map_entries_extent = map->entries_extent;
    if (!valid_map_facts(character, map, bound, ranges, map_entries,
                         map_capacity, map_entries_extent))
        return Status::invalid_source_fact;
    ActiveGuard active_guard(character, map);

    *out = {};
    out->decision = Decision::not_started;
    out->map_size_before = map->size;
    out->evicted_key = 0;

    const auto source_identity_stable = [&]() {
        return character->identity == character_identity &&
               character->ai == character_ai;
    };
    const auto live_map_valid = [&]() {
        return source_identity_stable() &&
               valid_map_facts(character, map, bound, ranges, map_entries,
                               map_capacity, map_entries_extent);
    };

    // These are the two live SM_GetState calls at 0x3ac34c and 0x3ac358.
    // No source callback runs between them, so one projected state word
    // faithfully supplies both comparisons.
    if (character->state_id == 12) {
        out->decision = Decision::state_12_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }
    if (character->state_id == 2) {
        out->decision = Decision::state_2_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }
    if (character->field_1480 != 0) {
        out->decision = Decision::field_1480_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }
    if (character->active_ais_3e4 != 0) {
        out->decision = Decision::field_3e4_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }

    Response response{};
    auto status = call(bound, character, map, *out,
                       Operation::current_level, nullptr, 0, 0, 0, response);
    if (status != Status::complete) return failed(status, map, *out);
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    // Original immediately dereferences CurrentLevel+0x3c. A missing Level is
    // not a source fallback; reject it at this adapter boundary.
    if (!response.identity) return failed(Status::invalid_source_fact, map, *out);
    const std::int32_t level_oid = signed_word(response.raw);
    const std::uint32_t threshold = level_oid == 29 ? 24u : 8u;
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    out->map_size_before = map->size;
    out->eviction_threshold = threshold;

    // OID 29 reaches 0x3aca48, where CMP #0x18 chooses eviction only for
    // size>24. Every other OID uses CMP #8 at 0x3ac3a8 (size>8).
    if (map->size > threshold) {
        out->eviction_attempted = 1;
        const Entry oldest = map->entries[0];
        Character* victim = oldest.character;
        if (!victim || !victim->identity || !victim->controller || !victim->ai)
            return failed(Status::invalid_source_fact, map, *out);
        const auto victim_identity = victim->identity;
        const auto victim_controller = victim->controller;
        out->evicted_key = oldest.key;
        out->evicted_character = victim_identity;

        status = call(bound, character, map, *out,
                      Operation::controller_kill, victim,
                      victim_controller, 0, 1, response);
        if (status != Status::complete) return failed(status, map, *out);
        // The source retains the begin iterator across Cmd_Kill. If a port
        // adapter changed the map at that point, refuse to pass a stale
        // iterator into UnLoadScriptProcess; the kill side effect remains.
        if (!live_map_valid() || victim->identity != victim_identity ||
            map->size != out->map_size_before || map->size == 0 ||
            map->entries[0].key != oldest.key ||
            map->entries[0].character != victim)
            return failed(Status::invalid_source_fact, map, *out);

        status = call(bound, character, map, *out,
                      Operation::unload_script_process, victim,
                      victim_identity, static_cast<std::uint32_t>(oldest.key),
                      1, response);
        if (status != Status::complete) return failed(status, map, *out);
        if (!live_map_valid() || map->size >= out->map_size_before ||
            out->map_size_before - map->size != 1)
            return failed(Status::invalid_source_fact, map, *out);
    }

    status = call(bound, character, map, *out,
                  Operation::load_n_init_script_process, character,
                  character_ai, 1, 0, response);
    if (status != Status::complete) return failed(status, map, *out);
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    if (response.raw == 0) {
        out->decision = Decision::load_failed;
        out->map_size_after = map->size;
        return Status::complete;
    }

    status = call(bound, character, map, *out,
                  Operation::is_monster, character,
                  character_identity, 0, 0, response);
    if (status != Status::complete) return failed(status, map, *out);
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    if (response.raw == 0) {
        out->decision = Decision::type_gate_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }
    status = call(bound, character, map, *out,
                  Operation::is_mini_boss, character,
                  character_identity, 0, 0, response);
    if (status != Status::complete) return failed(status, map, *out);
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    if (response.raw != 0) {
        out->decision = Decision::type_gate_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }
    status = call(bound, character, map, *out,
                  Operation::is_boss, character,
                  character_identity, 0, 0, response);
    if (status != Status::complete) return failed(status, map, *out);
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    if (response.raw != 0) {
        out->decision = Decision::type_gate_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }

    status = call(bound, character, map, *out,
                  Operation::get_online_byte, nullptr, 0, 0, 0, response);
    if (status != Status::complete) return failed(status, map, *out);
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    if (!response.identity || response.raw > 0xffu)
        return failed(Status::invalid_source_fact, map, *out);
    if (response.raw != 0) {
        out->decision = Decision::online_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }

    // Character+0x3ec is loaded only after the online byte check.
    if (character->field_3ec == 0) {
        out->decision = Decision::field_3ec_skip;
        out->map_size_after = map->size;
        return Status::complete;
    }

    status = call(bound, character, map, *out,
                  Operation::real_time_ms, nullptr, 0, 0, 0, response);
    if (status != Status::complete) return failed(status, map, *out);
    if (!live_map_valid()) return failed(Status::invalid_source_fact, map, *out);
    out->timestamp_word = response.raw;
    out->timestamp_key = signed_word(response.raw);

    bool replaced = false;
    if (!valid_map_facts(character, map, bound, ranges, map_entries,
                         map_capacity, map_entries_extent))
        return failed(Status::invalid_source_fact, map, *out);
    status = insert_or_replace(*map, out->timestamp_key, character, replaced);
    if (status != Status::complete) return failed(status, map, *out);
    out->decision = replaced ? Decision::timestamp_replaced : Decision::inserted;
    out->map_size_after = map->size;
    return Status::complete;
}

}  // namespace dh2::character_update_script_scheduler
