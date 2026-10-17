#include "loot_entry_selection_v1.hpp"

#include <cstring>

namespace {

std::int32_t signed_word(std::uint32_t value) noexcept {
    std::int32_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

bool span(const void* pointer, std::size_t size, std::size_t alignment) noexcept {
    const auto address = reinterpret_cast<std::uintptr_t>(pointer);
    return pointer && address % alignment == 0 && size <= UINTPTR_MAX - address;
}

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) noexcept {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a < b + right_size && b < a + left_size;
}

bool fixture_random(void* context, std::int32_t bound, std::uint32_t stream,
                    std::int32_t& value, std::string& error) {
    if (!context || stream != 0 ||
        dh2_loot_v2_random(static_cast<dh2::data::LootRandom8V2*>(context),
                           bound, &value)) {
        error = "Invalid borrowed fixture RNG";
        return false;
    }
    return true;
}

int draw(const dh2::data::InventoryRandomServiceV4& random,
         std::int32_t bound, std::uint32_t stream, std::int32_t& value,
         std::string& error) noexcept {
    if (!random.next) {
        error = "Required borrowed source RNG missing";
        return -2;
    }
    try {
        if (random.next(random.context, bound, stream, value, error)) return 0;
    } catch (...) {
        error = "Borrowed source RNG provider threw";
        return -2;
    }
    if (error.empty()) error = "Required borrowed source RNG failed";
    return -2;
}

}

namespace dh2::data {

bool loot_entry_uses_percent_v1(const LootEntry32V2& entry,
                                bool infinite_drops) noexcept {
    // The original uses ARM BHI/BLS after CMP #100: this is an unsigned
    // comparison even though the decoded cache field is stored as int32.
    return infinite_drops || std::uint32_t(entry.words[4]) <= 100u;
}

std::int32_t loot_entry_effective_probability_v1(
    const LootEntry32V2& entry, const LootPlayerClassCountsV1& counts,
    bool infinite_drops) noexcept {
    if (loot_entry_uses_percent_v1(entry, infinite_drops)) return 0;
    // The ARM source uses 32-bit MLA/ADD operations, so overflow wraps.
    auto result = std::uint32_t(entry.words[5]);
    result += std::uint32_t(entry.words[2]) * std::uint32_t(counts.mage);
    result += std::uint32_t(entry.words[6]) * std::uint32_t(counts.rogue);
    result += std::uint32_t(entry.words[7]) * std::uint32_t(counts.warrior);
    return signed_word(result);
}

bool loot_entry_do_percent_roll_v1(
    const LootEntry32V2& entry, bool infinite_drops,
    const InventoryRandomServiceV4& random,
    bool& accepted, std::string& error) noexcept {
    error.clear();
    bool result = false;
    if (infinite_drops) {
        result = true;
    } else if (loot_entry_uses_percent_v1(entry, false)) {
        std::int32_t value = 0;
        if (draw(random, 100, 0, value, error)) return false;
        result = value <= entry.words[4];
    }
    accepted = result;
    return true;
}

bool loot_entries_choose_weighted_v1(
    const LootEntry32V2* entries, std::uint32_t count,
    const LootPlayerClassCountsV1& counts, bool infinite_drops,
    const InventoryRandomServiceV4& random,
    std::uint32_t& selected_index, std::string& error) noexcept {
    error.clear();
    if (count > 65536 || (count && !span(entries,
            std::size_t(count) * sizeof(*entries), alignof(LootEntry32V2)))) {
        error = "Invalid loot-entry selection projection";
        return false;
    }

    std::uint32_t total = 0;
    for (std::uint32_t i = 0; i < count; ++i) {
        if (!loot_entry_uses_percent_v1(entries[i], infinite_drops))
            total += std::uint32_t(loot_entry_effective_probability_v1(
                entries[i], counts, infinite_drops));
    }
    if (!total) {
        selected_index = 0;
        return true;
    }

    std::int32_t remainder = 0;
    if (draw(random, signed_word(total), 0, remainder, error)) return false;
    auto remaining = std::uint32_t(remainder);
    for (std::uint32_t i = 0; i < count; ++i) {
        if (loot_entry_uses_percent_v1(entries[i], infinite_drops)) continue;
        const auto weight = std::uint32_t(loot_entry_effective_probability_v1(
            entries[i], counts, infinite_drops));
        if (weight > remaining) {
            selected_index = i;
            return true;
        }
        remaining -= weight;
    }

    // The source returns index zero after its diagnostic-only fallback path.
    selected_index = 0;
    return true;
}

}

extern "C" int dh2_loot_entry_is_percent_v1(
    std::uint32_t* out, const dh2::data::LootEntry32V2* entry,
    std::uint32_t infinite_drops) noexcept {
    if (!span(out, sizeof(*out), alignof(std::uint32_t)) ||
        !span(entry, sizeof(*entry), alignof(dh2::data::LootEntry32V2)) ||
        overlaps(out, sizeof(*out), entry, sizeof(*entry))) return -1;
    *out = dh2::data::loot_entry_uses_percent_v1(*entry, infinite_drops != 0);
    return 0;
}

extern "C" int dh2_loot_entry_effective_probability_v1(
    std::int32_t* out, const dh2::data::LootEntry32V2* entry,
    const dh2::data::LootPlayerClassCountsV1* counts,
    std::uint32_t infinite_drops) noexcept {
    if (!span(out, sizeof(*out), alignof(std::int32_t)) ||
        !span(entry, sizeof(*entry), alignof(dh2::data::LootEntry32V2)) ||
        !span(counts, sizeof(*counts), alignof(dh2::data::LootPlayerClassCountsV1)) ||
        overlaps(out, sizeof(*out), entry, sizeof(*entry)) ||
        overlaps(out, sizeof(*out), counts, sizeof(*counts)) ||
        overlaps(entry, sizeof(*entry), counts, sizeof(*counts))) return -1;
    *out = dh2::data::loot_entry_effective_probability_v1(
        *entry, *counts, infinite_drops != 0);
    return 0;
}

extern "C" int dh2_loot_entry_do_percent_roll_v1(
    std::uint32_t* out, const dh2::data::LootEntry32V2* entry,
    std::uint32_t infinite_drops, dh2::data::LootRandom8V2* random) noexcept {
    if (!span(out, sizeof(*out), alignof(std::uint32_t)) ||
        !span(entry, sizeof(*entry), alignof(dh2::data::LootEntry32V2)) ||
        !span(random, sizeof(*random), alignof(dh2::data::LootRandom8V2)) ||
        overlaps(out, sizeof(*out), entry, sizeof(*entry)) ||
        overlaps(out, sizeof(*out), random, sizeof(*random)) ||
        overlaps(entry, sizeof(*entry), random, sizeof(*random))) return -1;
    bool accepted = false;
    std::string error;
    if (!dh2::data::loot_entry_do_percent_roll_v1(
            *entry, infinite_drops != 0,
            {random, fixture_random}, accepted, error)) return -2;
    *out = accepted;
    return 0;
}

extern "C" int dh2_loot_entries_choose_weighted_v1(
    std::uint32_t* out, const dh2::data::LootEntry32V2* entries,
    std::uint32_t count, const dh2::data::LootPlayerClassCountsV1* counts,
    std::uint32_t infinite_drops, dh2::data::LootRandom8V2* random) noexcept {
    if (!span(out, sizeof(*out), alignof(std::uint32_t)) ||
        !span(counts, sizeof(*counts), alignof(dh2::data::LootPlayerClassCountsV1)) ||
        !span(random, sizeof(*random), alignof(dh2::data::LootRandom8V2)) ||
        count > 65536) return -1;
    const auto input_size = std::size_t(count) * sizeof(dh2::data::LootEntry32V2);
    if (count && (!span(entries, input_size, alignof(dh2::data::LootEntry32V2)) ||
        overlaps(out, sizeof(*out), entries, input_size) ||
        overlaps(random, sizeof(*random), entries, input_size))) return -1;
    if (overlaps(out, sizeof(*out), counts, sizeof(*counts)) ||
        overlaps(out, sizeof(*out), random, sizeof(*random)) ||
        overlaps(counts, sizeof(*counts), random, sizeof(*random))) return -1;
    std::uint32_t index = 0;
    std::string error;
    if (!dh2::data::loot_entries_choose_weighted_v1(
            entries, count, *counts, infinite_drops != 0,
            {random, fixture_random}, index, error)) return -2;
    *out = index;
    return 0;
}
