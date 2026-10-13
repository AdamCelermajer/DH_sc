#pragma once
// v21.8 es-runtime: registry of external skill owners (N1).
//
// The cast coordinator keeps its single `external_owner` slot (RuntimeSkillCastRequestV1::external_owner). The host
// puts this registry in that slot, so several owners (Mage today; Archer, Necromancer, Illusionist later) coexist. The
// registry forwards handles() to every owner, runs pre()/use() on the first registered owner that handles the profile,
// and forwards advance()/cleanup()/reset() to all of them in registration order.
//
// Owners are borrowed: the host keeps them alive for as long as the registry is in use.
#include "runtime_skill_external_owner_v1.hpp"

#include <cstddef>
#include <vector>

namespace dh::foundation::generic_skills {

class RuntimeSkillExternalOwnerRegistryV1 final : public RuntimeSkillExternalOwnerV1 {
public:
    // False for a null owner or one that is already registered.
    bool add(RuntimeSkillExternalOwnerV1* owner);
    std::size_t size() const noexcept { return owners_.size(); }
    // The owner that would run this profile (the first registered owner whose handles() is true), or nullptr.
    RuntimeSkillExternalOwnerV1* owner_for(const RuntimeSkillUseProfileV1& profile) const;

    bool handles(const RuntimeSkillUseProfileV1& profile) const override;
    bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    bool use(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    bool advance(CombatSession&, double dt_ms, std::string& error) override;
    void cleanup(CombatSession&, ActorId actor) override;
    void reset() override;

private:
    std::vector<RuntimeSkillExternalOwnerV1*> owners_;
};

} // namespace dh::foundation::generic_skills
