#include "runtime_skill_external_owner_registry_v1.hpp"

#include <algorithm>

namespace dh::foundation::generic_skills {

bool RuntimeSkillExternalOwnerRegistryV1::add(RuntimeSkillExternalOwnerV1* owner) {
    if (!owner || std::find(owners_.begin(), owners_.end(), owner) != owners_.end()) return false;
    owners_.push_back(owner);
    return true;
}

RuntimeSkillExternalOwnerV1* RuntimeSkillExternalOwnerRegistryV1::owner_for(const RuntimeSkillUseProfileV1& profile) const {
    for (auto* owner : owners_)
        if (owner->handles(profile)) return owner;
    return nullptr;
}

bool RuntimeSkillExternalOwnerRegistryV1::handles(const RuntimeSkillUseProfileV1& profile) const {
    return owner_for(profile) != nullptr;
}

bool RuntimeSkillExternalOwnerRegistryV1::pre(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                              std::string& error) {
    error.clear();
    if (!context.profile) {
        error = "External owner registry needs the skill profile to pick an owner";
        return false;
    }
    auto* owner = owner_for(*context.profile);
    if (!owner) {
        error = std::string("No registered external owner handles skill ") + context.profile->skill_name;
        return false;
    }
    return owner->pre(session, context, error);
}

bool RuntimeSkillExternalOwnerRegistryV1::use(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                              std::string& error) {
    error.clear();
    if (!context.profile) {
        error = "External owner registry needs the skill profile to pick an owner";
        return false;
    }
    auto* owner = owner_for(*context.profile);
    if (!owner) {
        error = std::string("No registered external owner handles skill ") + context.profile->skill_name;
        return false;
    }
    return owner->use(session, context, error);
}

bool RuntimeSkillExternalOwnerRegistryV1::advance(CombatSession& session, double dt_ms, std::string& error) {
    error.clear();
    for (auto* owner : owners_)
        if (!owner->advance(session, dt_ms, error)) return false;
    return true;
}

void RuntimeSkillExternalOwnerRegistryV1::cleanup(CombatSession& session, ActorId actor) {
    for (auto* owner : owners_) owner->cleanup(session, actor);
}

void RuntimeSkillExternalOwnerRegistryV1::reset() {
    for (auto* owner : owners_) owner->reset();
}

} // namespace dh::foundation::generic_skills
