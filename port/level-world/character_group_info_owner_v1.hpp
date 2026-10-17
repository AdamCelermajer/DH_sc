#pragma once

#include "character_ai_initialization.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dh2::character_group_info_owner_v1 {

enum class Role : std::uint8_t { normal, leader, master_leader, group };
enum class Status : std::uint8_t {
    complete, source_ignored, invalid_argument, owner_conflict,
    allocation_failed, not_found,
};

// The source GroupInfo owns three vectors at +0/+12/+24 and an exact scalar
// tail at +36/+40/+41. AddToGroup's raw entry receives Character* and stores
// that identity in each vector; the decompiler's implicit-this annotation is
// misleading for this static-like entry.
struct GroupInfo {
    std::vector<std::uintptr_t> members_master_leader;
    std::vector<std::uintptr_t> members_leader;
    std::vector<std::uintptr_t> members_normal_group;
    std::int32_t word_36 = -1;
    std::uint8_t flag_40 = 0;
    std::uint8_t flag_41 = 0;
};

struct Tail {
    std::int32_t word_36{};
    std::uint8_t flag_40{};
    std::uint8_t flag_41{};
};

// One level-scoped native map equivalent to CharAI::s_groups. GroupInfo
// addresses are stable until clear(); the associated Character/CharAI state
// stores the address only as the full-width semantic value of Character+0x3fc.
class Owner final {
public:
    Owner() = default;
    ~Owner();
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;

    Status add_to_group(character_ai_initialization::State& ai,
                        std::uintptr_t character_identity,
                        std::int32_t module_id_64,
                        const std::string& group_name,
                        const std::string& role_name,
                        std::uintptr_t* group_identity = nullptr) noexcept;
    Status remove_from_group(character_ai_initialization::State& ai) noexcept;
    struct DeathServices {
        void* context{};
        // Character::IsDead and v2Controller::Cmd_Kill source-equivalent
        // dispatchers. False means the canonical owner/provider was absent or
        // rejected the operation; no missing callback is treated as success.
        bool (*is_dead)(void*, std::uintptr_t character, bool* dead){};
        bool (*cmd_kill)(void*, std::uintptr_t character,
                         std::uintptr_t killer, bool force){};
    };
    struct EnemySpottedServices {
        void* context{};
        // Source CharStateMachine::SM_IsAwaitingToSpawn and
        // SM_SetSpawnState(false,false), bound to each member's canonical
        // SpawnOwner/Coordinator. A successful transition selects state 1.
        int (*is_awaiting_to_spawn)(void*, std::uintptr_t character,
                                    bool* awaiting){};
        int (*set_spawn_state)(void*, std::uintptr_t character){};
    };
    struct EnemySpottedResult {
        std::uint32_t awaiting_queries{};
        std::uint32_t spawn_transitions{};
        std::uint8_t one_shot_flag_set{};
        std::uint8_t completion_word_set{};
    };
    // CharAI::GroupInfo::OnEnemySpotted, before the caller CharAI's ordinary
    // spawn/limbus, aggro and selected-AIS dispatch gates. Membership vectors
    // and the source +0x41/+0x36 one-shot fields remain owned by this Owner.
    Status on_enemy_spotted(character_ai_initialization::State& ai,
                            std::uintptr_t character_identity,
                            std::uintptr_t enemy_identity,
                            std::int32_t source_state,
                            const EnemySpottedServices& services,
                            EnemySpottedResult* result = nullptr) noexcept;
    Status on_died(character_ai_initialization::State& ai,
                   std::uintptr_t character_identity, std::uintptr_t killer,
                   const DeathServices& services) noexcept;
    bool read_tail(std::uintptr_t group_identity, Tail* output) const noexcept;
    // Character::Deserialize restores the optional six-byte GroupInfo tail
    // through the Character's existing CharAI+0x34 identity. Never creates or
    // replaces a group; false means the saved pointee has no active owner.
    bool write_tail(std::uintptr_t group_identity, const Tail& value) noexcept;
    bool owns(std::uintptr_t group_identity) const noexcept;
    void clear() noexcept;
    std::size_t group_count() const noexcept { return groups_.size(); }
    std::size_t binding_count() const noexcept { return bindings_.size(); }

private:
    struct Key {
        std::string name;
        std::int32_t module_id_64{};
        bool operator<(const Key& other) const noexcept {
            return name < other.name ||
                (!(other.name < name) && module_id_64 < other.module_id_64);
        }
    };
    struct Binding {
        character_ai_initialization::State* ai{};
        std::uintptr_t character_identity{};
        std::uintptr_t group_identity{};
        Role role{};
        bool member_present=true;
    };
    std::map<Key, std::unique_ptr<GroupInfo>> groups_;
    // Source GroupInfo stores Character identities and Character+0x3fc points
    // back to GroupInfo. Bindings are therefore keyed by Character identity.
    std::map<std::uintptr_t, Binding> bindings_;
};

} // namespace dh2::character_group_info_owner_v1
