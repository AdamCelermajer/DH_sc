#pragma once

#include "../../../level-world/openable_container_owner_v1.hpp"
#include "../../../level-world/canonical_destructible_container_v16.hpp"
#include "../../../level-world/world_loot_gameplay_v23.hpp"

namespace dh::foundation::interactions {

struct ContainerLootSourceV104 {
    std::uintptr_t identity{};
    std::shared_ptr<void> lease;
};

using ContainerLootSourceResolverV104 = std::function<bool(
    ContainerLootSourceV104&, std::string&)>;
using ContainerLiveDropTableV104 = bool(*)(
    dh2::character::WorldLootGameplayV23&, std::int32_t, std::uintptr_t,
    std::uintptr_t, std::int32_t, bool, std::string&);

// Canonical target for the required adapter parameter below. A root caller
// passes this function (or an equivalent same-owner wrapper) so the original
// service's table, published source identity, opener, fixed-power value and
// source flag reach the existing CharacterLootLiveV22 method unchanged.
inline bool container_live_drop_table_v104(
    dh2::character::WorldLootGameplayV23& gameplay,
    std::int32_t table, std::uintptr_t source, std::uintptr_t opener,
    std::int32_t fixed_powers, bool source_flag, std::string& error) {
    if (!gameplay.ready() || !gameplay.loot() || !gameplay.items() ||
        gameplay.loot()->items() != &gameplay.items()->pool()) {
        error = "Interactions require initialized SAME CharacterLootLiveV22/WorldItemLiveOwnerV5 pool";
        return false;
    }
    return gameplay.loot()->drop_table_v104(table, source, opener, fixed_powers,
                                             source_flag, error);
}

// Both original receiver services have the same source signature. Resolve the
// actual container identity at delivery time, then pass it and the authored
// opener to the already initialized world loot owner. WorldLootGameplayV23
// owns the SAME CharacterLootLiveV22 and WorldItemLiveOwnerV5/Item145 pool;
// no alternate inventory or manager is created here.
template<class ContainerServices>
inline bool bind_container_live_loot_v104(
    ContainerServices& services,
    std::shared_ptr<dh2::character::WorldLootGameplayV23> gameplay,
    ContainerLiveDropTableV104 drop_table,
    std::shared_ptr<void> world_lease,
    ContainerLootSourceResolverV104 resolve_source,
    std::string& error) {
    error.clear();
    if (!gameplay) {
        error = "Interactions require initialized SAME WorldLootGameplayV23 owner";
        return false;
    }
    if (!drop_table) {
        error = "Interactions require canonical CharacterLootLiveV22 DropLootTable adapter";
        return false;
    }
    if (!world_lease || !resolve_source) {
        error = "Interactions require retained SAME World/container source resolver";
        return false;
    }
    if (!gameplay->ready() || !gameplay->loot() || !gameplay->items() ||
        gameplay->loot()->items() != &gameplay->items()->pool()) {
        error = "Interactions require initialized SAME CharacterLootLiveV22/WorldItemLiveOwnerV5 pool";
        return false;
    }
    if (services.drop_loot_table) {
        error = "Interactions container DropLootTable already bound";
        return false;
    }

    services.drop_loot_table = [gameplay = std::move(gameplay), drop_table,
        world_lease = std::move(world_lease),
        resolve_source = std::move(resolve_source)](
        std::int32_t table, std::uintptr_t opener, std::int32_t fixed_powers,
        bool source_flag, std::string& e) {
        if (!world_lease || !gameplay || !gameplay->ready() || !gameplay->loot() ||
            !gameplay->items() || gameplay->loot()->items() != &gameplay->items()->pool()) {
            e = "Interactions lost initialized SAME CharacterLootLiveV22/WorldItemLiveOwnerV5 pool";
            return false;
        }
        ContainerLootSourceV104 source;
        if (!resolve_source(source, e)) return false;
        if (!source.identity || !source.lease) {
            e = "Interactions require published same-world container loot source";
            return false;
        }
        return drop_table(*gameplay, table, source.identity, opener, fixed_powers,
                          source_flag, e);
    };
    return true;
}

} // namespace dh::foundation::interactions
