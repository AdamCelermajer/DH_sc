#include "passive_skill_owner_v1.hpp"

namespace dh::foundation::generic_skills {

bool RuntimePassiveSkillOwnerV1::set_rank(CombatSession& session, ActorId actor, const PassiveStatRowV1& row,
                                          std::uint32_t rank, std::string& error) {
    error.clear();
    const auto key = std::make_pair(actor, row.skill_key);
    const auto found = ranks_.find(key);
    const std::uint32_t previous = found == ranks_.end() ? 0u : found->second;
    if (found != ranks_.end() && previous == rank) return true;
    if (previous != 0 && !buffs_->remove_timed_buff(session, actor, row.class_name, error)) return false;
    if (rank == 0) {
        ranks_.erase(key);
        return true;
    }
    TimedBuffSpecV1 spec;
    spec.class_name = row.class_name;
    spec.strength = rank;
    spec.duration_ms = 0.0;
    spec.permanent = true;
    spec.capacity = 1;
    spec.source = row.source;
    if (!buffs_->apply_timed_buff(session, actor, spec, error)) return false;
    ranks_[key] = rank;
    return true;
}

std::uint32_t RuntimePassiveSkillOwnerV1::rank(ActorId actor, const std::string& skill_key) const {
    const auto found = ranks_.find(std::make_pair(actor, skill_key));
    return found == ranks_.end() ? 0u : found->second;
}

} // namespace dh::foundation::generic_skills
