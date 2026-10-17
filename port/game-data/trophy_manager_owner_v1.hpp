#pragma once

#include "data.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::data {

// TrophyTable records serialize the seven Structs::Trophy integer fields in
// this order. The source object itself has a vptr before these fields; it is
// not part of the cache record.
struct TrophyTableRowV1 {
    std::string table_name;
    std::int32_t desc{};
    std::int32_t gl_index{};
    std::int32_t gl_live{};
    std::int32_t grade{};
    std::int32_t label{};
    std::int32_t name{};
    std::int32_t type{};
};

class TrophyTableV1 {
    std::vector<TrophyTableRowV1> rows_;
public:
    const std::vector<TrophyTableRowV1>& rows() const noexcept { return rows_; }
    bool load(Bytes records, Bytes names, Bytes fields, std::string& error);
};

// Native TrophyData is a separate per-manager projection of its source table
// row. Fields here are IDs/values, not pointers into the source 32-byte object.
struct TrophyDataV1 {
    std::int32_t id{};
    std::int32_t name_text_id{};
    std::int32_t desc_text_id{};
    bool unlocked{};
    std::int32_t type{};
    std::int32_t grade{};
    std::int32_t label{};
    std::int32_t gl_live{};
    std::int32_t gl_index{};
};

struct TrophyUnlockEventV1 {
    std::int32_t id{};
    std::int32_t name_text_id{};
    std::int32_t desc_text_id{};
    std::int32_t type{};
    std::int32_t grade{};
    std::int32_t label{};
    std::int32_t gl_live{};
    std::int32_t gl_index{};
};

enum class TrophyUnlockStatusV1 {
    started,
    completed,
    invalid_id,
    failed,
    already_unlocked,
    already_unlocking,
    not_unlocking
};

struct TrophyIntegrationAvailabilityV1 {
    bool game_center{};
    bool gl_live{};
    bool google_play{};
};

// One source TrophyManager state owner. UnlockTrophy adds an ID to its
// in-progress list and directly invokes TrophyUnlockedCB; adapters should
// therefore call begin_unlock() and complete_unlock() synchronously. The
// platform notifications are separate unavailable integrations.
class TrophyManagerOwnerV1 {
    std::vector<TrophyDataV1> rows_;
    std::vector<std::string> table_names_;
    std::vector<std::int32_t> unlocking_;
    bool initialized_{};
public:
    static constexpr std::uint32_t source_save_bit_count = 128;

    bool initialize(const TrophyTableV1& table, std::string& error);
    bool initialized() const noexcept { return initialized_; }
    std::size_t size() const noexcept { return rows_.size(); }
    const TrophyDataV1* get(std::int32_t id) const noexcept;
    // Arrays::GetMemberIDByString<TrophyTable> exact, case-sensitive name lookup.
    std::int32_t find_id_by_name(const std::string& table_name) const noexcept;
    const std::vector<std::string>& table_names() const noexcept { return table_names_; }
    bool is_unlocked(std::int32_t id) const noexcept;
    bool is_unlocking(std::int32_t id) const noexcept;
    const std::vector<TrophyDataV1>& rows() const noexcept { return rows_; }

    TrophyUnlockStatusV1 begin_unlock(std::int32_t id) noexcept;
    TrophyUnlockStatusV1 complete_unlock(std::int32_t id,
                                         TrophyUnlockEventV1* event = nullptr) noexcept;
    TrophyUnlockStatusV1 unlock(std::int32_t id,
                                TrophyUnlockEventV1* event = nullptr) noexcept;

    // Source SaveTrophies serializes std::bitset<128> as four native
    // little-endian 32-bit words. This codec owns payload semantics only;
    // callers own the separate achievements.savegame file and its lifetime.
    bool encode_savegame_bits(std::array<std::uint8_t, 16>& payload,
                              std::string& error) const;
    bool load_savegame_bits(Bytes payload, std::string& error);

    static constexpr TrophyIntegrationAvailabilityV1 integrations() noexcept {
        return {false, false, false};
    }
    // This table/progress owner provides only the 128-bit payload codec.
    // Android's process Trophy runtime owns the source sidecar file boundary.
    static constexpr bool source_persistence_available = false;
};

} // namespace dh2::data
