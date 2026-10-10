#pragma once

#include "source_container_loot_v1.hpp"
#include "../loot/runtime_world_item_adapter_v1.hpp"
#include "../../combat_session.hpp"
#include <memory>

namespace dh::foundation::interactions {

// Exact source bonus lookup for ItemTable type13 rows. The modern neutral
// object path has no implicit CharacterProperties fallback; a host must provide
// the opener's actual source value before GoldStack can be published.
using SessionContainerGoldBonusV1 = bool(*)(
    void*,ActorId opener,std::int32_t& bonus256,std::string& error);

struct SessionContainerModernDropInputsV1 {
    dh2::data::LootTablesV2::Borrow tables;
    dh2::data::LootPowerResourcesV7::Borrow powers;
    dh2::data::LootEntryServicesV8 entry{};
    loot::RuntimeWorldItemAdapterV1* store{};
    void* gold_bonus_context{};
    SessionContainerGoldBonusV1 gold_bonus256{};
};

// Composes only the modern source leaves that have real owners: the current
// CombatSession's scoped PlayableActorWorld RNG and the existing world-item
// store. It does not admit interactions, evaluate source conditions, or
// substitute audio/physics/script/quest policy.
class SessionContainerModernDropV1 final
    : public std::enable_shared_from_this<SessionContainerModernDropV1> {
public:
    static bool create(CombatSession&,SessionContainerModernDropInputsV1,
                       std::shared_ptr<SessionContainerModernDropV1>&,
                       std::string& error);
    SessionContainerModernDropV1(const SessionContainerModernDropV1&)=delete;

    bool services(SourceContainerLootServicesV1&,std::string& error);
    bool current_object(const ActorDefinition&,WorldObject*&,std::string& error)const;

private:
    SessionContainerModernDropV1(CombatSession&,SessionContainerModernDropInputsV1);
    bool current(std::string& error)const;
    static bool with_rng(void*,const SourceContainerLootRngOperationV1&,
                         std::string& error);
    static bool publish(void*,const SourceContainerDropItemV1&,
                        dh2::data::LootRandom8V2&,std::string& error);

    CombatSession* session_{};
    SessionContainerModernDropInputsV1 inputs_;
    std::shared_ptr<const void> lease_;
};

} // namespace dh::foundation::interactions
