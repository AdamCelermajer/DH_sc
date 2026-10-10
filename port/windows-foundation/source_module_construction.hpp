#pragma once
#include "../level-world/canonical_level_module_bindings_v2.hpp"
#include "../level-world/character_set_position_v7.hpp"
#include "../level-world/native_conditions_v69.hpp"
#include "../level-world/application_spawn_random_owner_v4.hpp"

namespace dh::foundation {
// Borrow the application-lifetime Random channels and exact network byte.
// Manager/visual callbacks borrow their containing scope weakly; this context
// must not own the Module records retained by that manager.
struct SourceModuleSpawnApplication {
    std::shared_ptr<void> applicationOwner;
    dh2::world::ApplicationSpawnRandomOwnerV4* random = nullptr;
    const std::uint8_t* onlineByte5 = nullptr;
    std::weak_ptr<dh2::world::CanonicalObjectManagerV1> manager;
    std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,
        std::uintptr_t actualVisual, std::string&)> syncVisibility;
};
struct SourceModuleConstructionProviders {
    std::shared_ptr<void> candidateOwner, globalsOwner, previousOwner;
    dh2::world::ModuleRuntimeGlobalsV1* globals = nullptr;
    std::shared_ptr<dh2::world::CanonicalPropertyMapV1> properties;
    dh2::world::CanonicalClassServicesV1 previous;
    dh2::character::CharacterPositionServicesV7 positionBackends;
    // Optional actual Module InitPost/InitFinal providers. Missing reached
    // services remain source failures. Construction does not run these phases.
    std::function<bool(const dh2::world::CanonicalSourceObjectRequestV1&,
        const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>&,
        dh2::world::GameObjectInitializationServicesV1&,dh2::world::ModuleInitServicesV1&,
        std::string&)> initialization;
    // SAME independent immutable Arrays table/native ConditionList arena.
    // Optional only for the source empty/Invalid-name and NULL-clear leaves.
    // A reached named condition without it fails; no table is synthesized.
    std::shared_ptr<dh2::world::NativeConditionRuntimeV69> conditions;
    std::shared_ptr<SourceModuleSpawnApplication> spawnApplication;
};
class SourceModuleConstruction {
public:
    explicit SourceModuleConstruction(SourceModuleConstructionProviders);
    dh2::world::CanonicalClassServicesV1 services() const noexcept;
    std::shared_ptr<void> lease() const noexcept;
    bool record(std::uintptr_t,std::shared_ptr<dh2::world::CanonicalModuleRecordV2>&,
                std::string& error) const;
    const std::vector<std::shared_ptr<dh2::world::CanonicalModuleRecordV2>>& records() const noexcept;
    // Ordered actual embedded ConditionData destruction before receiver drop.
    // On failure retains the exact uncleared pointer and independent arena.
    bool clear_conditions(std::uintptr_t actualIdentity, std::string& error) const;
    bool check_spawn_probability(std::uintptr_t actualIdentity, std::int32_t& roll,
        std::int32_t& probability, std::string& error) const;
private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace dh::foundation
