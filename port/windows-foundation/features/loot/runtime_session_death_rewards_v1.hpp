#pragma once

#include "runtime_death_rewards_v1.hpp"
#include "runtime_loot_source_owner_v1.hpp"
#include "runtime_world_item_adapter_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../frontend/creation/runtime_creation_source_loader_v1.hpp"

#include <memory>

namespace dh::foundation::loot {

// Callbacks borrow the caller's existing gameplay/menu/source owners. The
// context lease must pin that owner graph until reset(); no native Level,
// PlayerManager, or second inventory/store owner is made here.
struct RuntimeSessionDeathRewardBindingsV1 {
    std::shared_ptr<const void> gameplay_context_lease;
    void* context{};
    // Reads the authoritative current gameplay owner's Level+0x150 source
    // value. The ordinary source Level constructor initializes this to false.
    bool (*read_reward_admission)(void*, RuntimeDeathRewardAdmissionV1&,
                                  std::string&){};
    bool (*read_difficulty)(void*, std::int32_t& current,
                            std::int32_t& unlocked, std::string&){};
    bool (*source_loot_entry)(void*, const dh2::data::LootEntryRequestV8&,
                              std::int32_t&, std::string&){};
    bool (*query_debug_switch)(void*, const char* key, bool&, std::string&){};
    bool (*resolve_character)(void*, ActorId, RuntimeDeathActorV1&,
                              std::string&){};
};

// Main-callable composition of the already-existing source owners and generic
// same-session consumers. Bind after CombatSession initialization/rebind. Call
// reset immediately before session restore/teardown, then bind again after the
// session's restore rebind succeeds. An expired actor binding lease makes the
// old binding unusable and cannot silently attach it to the restored world.
class RuntimeSessionDeathRewardsV1 {
    RuntimeLootSourceOwnerV1 loot_source_owner_;
    RuntimeLootSourceV1 loot_source_;
    std::shared_ptr<RuntimeWorldItemAdapterV1> world_items_;
    std::shared_ptr<const void> gameplay_context_lease_;
    RuntimeSessionDeathRewardBindingsV1 bindings_{};
    const frontend::creation::RuntimeCreationSourceOwnerV1* creation_owner_{};
    dh::foundation::CombatSession* session_{};
    dh::foundation::PlayableActorWorld* world_{};
    std::weak_ptr<const void> session_binding_lease_;
    RuntimeDeathRewardsV1 rewards_;
    bool dispatching_{};
    dh2::data::LootRandom8V2* scatter_rng_{}; // same loot RNG as the roll, valid only inside after_update

    static bool loot_entry_thunk(void*, const dh2::data::LootEntryRequestV8&,
                                 std::int32_t&, std::string&);
    static bool one_kill_level_up_thunk(void*, bool&, std::string&);
    static bool resolve_character_thunk(void*, ActorId, RuntimeDeathActorV1&,
                                        std::string&);
    static bool spawn_world_item_thunk(void*, const RuntimeWorldItemRecordV1&,
                                       const ActorState&, const ActorState*,
                                       std::string&);

public:
    bool bind(dh::foundation::CombatSession&,
              const frontend::creation::RuntimeCreationSourceOwnerV1&,
              const AssetCatalog&,
              std::shared_ptr<RuntimeWorldItemAdapterV1>,
              RuntimeSessionDeathRewardBindingsV1,
              std::string& error);

    bool after_update(dh::foundation::CombatSession&,
                      std::vector<RuntimeDeathRewardOutcomeV1>&,
                      std::string& error);

    void reset() noexcept;
    bool bound() const noexcept { return session_ != nullptr; }
};

} // namespace dh::foundation::loot
