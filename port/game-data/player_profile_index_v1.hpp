#pragma once

#include "data.hpp"
#include <memory>

namespace dh2::data {

struct ProfileSection12V1 {
    std::uint8_t tag[4]{};
    std::uint32_t offset{}, size{};
};
struct ProfileIndexSpan24V1 {
    const std::uint8_t* bytes{};
    std::uint32_t size{}, cursor{}, source_count{}, reserved{};
};
struct ProfileIndexServices16V1 {
    void* context{};
    bool (*section)(void*, const ProfileSection12V1*){};
};
static_assert(sizeof(ProfileSection12V1) == 12);
static_assert(sizeof(ProfileIndexSpan24V1) == 24);
static_assert(sizeof(ProfileIndexServices16V1) == 16);

// Savegame::_cacheFile's campaign count/size/tag/payload format. A snapshot
// owns indexed bytes only. Save fields, file/backup I/O and load orchestration
// belong to their existing owners. No settings-stream or new-game defaults.
class PlayerProfileIndexV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class PlayerProfileIndexV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p) : snapshot_(std::move(p)) {}
    public:
        Borrow() = default;
        explicit operator bool() const noexcept { return bool(snapshot_); }
        const std::vector<std::uint8_t>& bytes() const;
        const std::vector<ProfileSection12V1>& source_sections() const;
        // Four-byte file tags have source C-string identity. Duplicates keep
        // the last offset/size; callers' lookup strings are not truncated.
        const ProfileSection12V1* section(const char* tag) const noexcept;
        Bytes payload(const char* tag) const noexcept;
    };
    PlayerProfileIndexV1() = default;
    PlayerProfileIndexV1(const PlayerProfileIndexV1&) = delete;
    PlayerProfileIndexV1& operator=(const PlayerProfileIndexV1&) = delete;
    // Save adapters must verify that the borrowed campaign view belongs to
    // this exact canonical index before replacing sections in place.
    bool owns(const Borrow& view) const noexcept {
        return snapshot_ && snapshot_ == view.snapshot_;
    }
    void swap(PlayerProfileIndexV1& other) noexcept {
        snapshot_.swap(other.snapshot_);
    }
    bool load(Bytes, std::string&);
    Borrow borrow() const { return Borrow(snapshot_); }
};
}

// 0 delivered; -1 malformed/aliased entry; -2 unsafe/truncated native span;
// -3 failed storage callback; -4 source corruption marker. Cursor/callback
// prefixes are retained. Malformed spans are safely rejected before an
// out-of-bounds section is published (not original backup-path parity).
extern "C" int dh2_player_profile_v1_index(
    dh2::data::ProfileIndexSpan24V1*,
    const dh2::data::ProfileIndexServices16V1*) noexcept;
