#pragma once

#include <cstddef>
#include <cstdint>
#include <map>

namespace dh2::character_oid_cache_v1 {

// Bounded reconstruction of Character::s_cachedCharOIDs. One current Level
// owns the table; the values are source counts, not counts inferred from the
// renderer's actor list.
enum class Status : std::uint8_t {
    complete, invalid_argument, no_level, out_of_range, stale_level
};

struct Result {
    Status status=Status::complete;
    std::uint32_t value=0;
    bool changed=false;
};

class Owner {
    std::uintptr_t level_identity_=0;
    std::uint32_t character_table_size_=0;
    std::uint64_t generation_=0;
    std::map<std::uint32_t,std::uint32_t> counts_;
public:
    // Level construction replaces the process-wide source cache. Repeating
    // the same bind is idempotent; changing identity clears prior Level data.
    Status begin_level(std::uintptr_t level_identity,
                       std::uint32_t character_table_size) noexcept;
    // Character::AddCharOIDToCache: retain max(existing, requested).
    Status add_char_oid(std::uint32_t character_table_id,
                        std::uint32_t requested_count,Result*);
    // Character::HasCharOIDInCache: absent IDs read as zero.
    Status count(std::uint32_t character_table_id,Result*) const noexcept;
    // Level::~Level clears the source tree. A stale Level cannot clear a newer
    // Level's cache.
    Status clear_level(std::uintptr_t level_identity,
                       std::uint64_t generation) noexcept;

    std::uintptr_t level_identity() const noexcept { return level_identity_; }
    std::uint32_t character_table_size() const noexcept { return character_table_size_; }
    std::uint64_t generation() const noexcept { return generation_; }
    std::size_t entry_count() const noexcept { return counts_.size(); }
};

enum class ValueKind : std::uint8_t { unsigned_integer, other };
struct Argument { ValueKind kind=ValueKind::other; std::uint64_t value=0; };
struct Arguments { const Argument* values=nullptr; std::size_t count=0; };

// Reached GameObject::_RegisterSummon argument contract. Bad/missing arg0 or
// an ID outside CharacterTable is the original no-op. Arg1 defaults to one
// unless it exists and is the source UInteger type.
Status register_summon(Owner&,const Arguments&,Result*);

} // namespace dh2::character_oid_cache_v1
