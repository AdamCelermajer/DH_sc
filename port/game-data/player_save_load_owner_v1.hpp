#pragma once

#include "player_profile_index_v1.hpp"
#include "player_savegame_v1.hpp"
#include <functional>

namespace dh2::data {
struct CharacterTable;

struct PlayerSaveProfileV1 {
    std::uintptr_t identity{};
    std::shared_ptr<void> owner;
    // Optional for opaque source streams; mandatory for metadata readers.
    PlayerProfileIndexV1::Borrow campaign;
};
enum class PlayerSaveLoadOpV1 : std::uint32_t {
    filename, create_profile, load_section, init_levels, init_skills,
    init_faeries, init_quests, online, hosting_quest_flag, local_hosting,
    load_volatile_flag, volatile_stream, stream_size, stream_seek,
    quest_definition, unpack_quests
};
struct PlayerSaveLoadRequestV1 {
    PlayerSaveLoadOpV1 operation{};
    PlayerSavegameV1* save{};
    PlayerSaveProfileV1 profile;
    const char* section{};
    const char* filename{};
    std::uint32_t argument{};
    bool reader_enabled{true};
    // Source Savegame::load registers the writer even when CFEE reader=null.
    bool writer_enabled{true};
    std::int32_t definition{};
    PlayerSaveLoadRequestV1() = default;
    PlayerSaveLoadRequestV1(PlayerSaveLoadOpV1 op) : operation(op) {}
};
struct PlayerSaveLoadResponseV1 {
    PlayerSaveProfileV1 profile;
    std::string text;
    std::uint64_t amount{};
    std::int32_t value{};
    bool flag{};
};
struct PlayerSaveLoadServicesV1 {
    std::shared_ptr<void> owner;
    std::function<bool(const PlayerSaveLoadRequestV1&,
                       PlayerSaveLoadResponseV1&, std::string&)> invoke;
};

// Complete SG_Load mask/slot/profile/global ordering. Borrows the sole Save
// and caller's canonical source +8 profile slot. It owns neither Save fields,
// properties, VM, quest/inventory storage nor an online state. Every reached
// external operation is mandatory. Explicit later loads remain source-valid;
// failed loads retain their delivered prefix and never automatically retry.
// Borrowed Save/profile slots must outlive this owner and synchronous calls;
// services must not destroy/rebind this owner or the Save during a callback.
class PlayerSaveLoadOwnerV1 {
    PlayerSavegameV1& save_;
    PlayerSaveProfileV1& profile_;
    PlayerSaveLoadServicesV1 services_;
    std::uint32_t phase_{}, calls_{}, section_calls_{};
    bool active_{};
    bool send(PlayerSaveLoadRequestV1, PlayerSaveLoadResponseV1&, std::string&);
    bool section(const char*, const PlayerSaveProfileV1&, bool, std::string&);
    bool initialize(PlayerSaveLoadOpV1, std::uint32_t, std::string&);
    bool load_fields(std::uint32_t, std::string&);
    bool load_volatile(std::uint32_t, std::string&);
public:
    PlayerSaveLoadOwnerV1(PlayerSavegameV1&, PlayerSaveProfileV1& canonical_profile,
                         PlayerSaveLoadServicesV1 = {});
    PlayerSaveLoadOwnerV1(const PlayerSaveLoadOwnerV1&) = delete;
    PlayerSaveLoadOwnerV1& operator=(const PlayerSaveLoadOwnerV1&) = delete;
    PlayerSavegameV1& save() const noexcept { return save_; }
    const PlayerSaveProfileV1& profile() const noexcept { return profile_; }
    bool publish_profile(PlayerSaveProfileV1, std::string&);
    bool load(std::int32_t source_mask, std::string&);
    std::uint32_t reached_phase() const noexcept { return phase_; }
    std::uint32_t delivered_calls() const noexcept { return calls_; }
    std::uint32_t delivered_section_calls() const noexcept { return section_calls_; }
};

struct PlayerMetadataServicesV1 {
    // __LoadPlayerClass uses Arrays::CharacterTable names, NOT ClassTables.
    const CharacterTable* characters{};
    void* context{};
    bool (*store_selected_difficulty)(void*, std::int32_t, std::string&){};
};
// Exact seven mask1 reader bodies over the request's retained campaign bytes
// and same Save. Missing/zero-size tags deliver Savegame::load's no-read branch.
// Callback registration/write/file ownership remains the load_section service
// boundary. This helper performs no healing, init_skills or full-player load.
bool load_player_metadata_section_v1(const PlayerSaveLoadRequestV1&,
    const PlayerMetadataServicesV1&, std::size_t& consumed, std::string&);
}
