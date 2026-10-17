#include "character_group_info_owner_v1.hpp"

#include <algorithm>
#include <new>

namespace dh2::character_group_info_owner_v1 {
namespace {
bool decode_role(const std::string& value, Role& output) noexcept {
    if (value == "Normal") output = Role::normal;
    else if (value == "Leader") output = Role::leader;
    else if (value == "MasterLeader") output = Role::master_leader;
    else if (value == "Group") output = Role::group;
    else return false;
    return true;
}

std::vector<std::uintptr_t>* members(GroupInfo& group, Role role) noexcept {
    switch (role) {
    case Role::normal:
    case Role::group: return &group.members_normal_group;
    case Role::leader: return &group.members_leader;
    case Role::master_leader: return &group.members_master_leader;
    }
    return nullptr;
}
}

Owner::~Owner() { clear(); }

Status Owner::add_to_group(character_ai_initialization::State& ai,
                           std::uintptr_t character_identity,
                           std::int32_t module_id_64,
                           const std::string& group_name,
                           const std::string& role_name,
                           std::uintptr_t* group_identity) noexcept {
    if (group_identity) *group_identity = 0;
    if (!ai.identity || !character_identity || ai.owner_04 != character_identity)
        return Status::invalid_argument;
    // Source AddToGroup returns before role parsing for an empty group name.
    if (group_name.empty()) return Status::source_ignored;
    Role role{};
    // Unknown source role strings also fall through without creating a group.
    if (!decode_role(role_name, role)) return Status::source_ignored;
    if (ai.group_info_identity) return Status::owner_conflict;
    if (bindings_.find(character_identity) != bindings_.end()) return Status::owner_conflict;

    try {
        Key key{group_name, module_id_64};
        auto found = groups_.find(key);
        if (found == groups_.end()) {
            auto inserted = groups_.emplace(std::move(key),
                std::unique_ptr<GroupInfo>(new GroupInfo{}));
            found = inserted.first;
        }
        auto* group = found->second.get();
        auto* member_list = members(*group, role);
        if (!group || !member_list) return Status::allocation_failed;
        member_list->push_back(character_identity);
        const auto identity = reinterpret_cast<std::uintptr_t>(group);
        try {
            const auto inserted = bindings_.emplace(character_identity,
                Binding{&ai, character_identity, identity, role, true});
            if (!inserted.second) {
                member_list->pop_back();
                return Status::owner_conflict;
            }
        } catch (...) {
            member_list->pop_back();
            throw;
        }
        ai.group_info_identity = identity;
        ai.word_38 = static_cast<std::uint32_t>(role);
        if (group_identity) *group_identity = identity;
        return Status::complete;
    } catch (const std::bad_alloc&) {
        return Status::allocation_failed;
    } catch (...) {
        return Status::allocation_failed;
    }
}

Status Owner::remove_from_group(character_ai_initialization::State& ai) noexcept {
    if (!ai.owner_04) return Status::invalid_argument;
    const auto found = bindings_.find(ai.owner_04);
    if (found == bindings_.end())
        return ai.group_info_identity ? Status::owner_conflict : Status::not_found;
    const auto binding = found->second;
    if (binding.ai != &ai || binding.group_identity != ai.group_info_identity ||
        binding.character_identity != ai.owner_04) return Status::owner_conflict;
    // The source void method silently does nothing when the Character has
    // already been removed from its role vector.
    if (!binding.member_present) return Status::complete;
    for (auto& entry : groups_) {
        if (reinterpret_cast<std::uintptr_t>(entry.second.get()) != binding.group_identity)
            continue;
        auto* list = members(*entry.second, binding.role);
        if (!list) return Status::owner_conflict;
        const auto member = std::find(list->begin(), list->end(), binding.character_identity);
        if (member == list->end()) {
            found->second.member_present = false;
            return Status::complete;
        }
        list->erase(member);
        found->second.member_present = false;
        return Status::complete;
    }
    return Status::owner_conflict;
}

Status Owner::on_enemy_spotted(character_ai_initialization::State& ai,
                              std::uintptr_t character_identity,
                              std::uintptr_t enemy_identity,
                              std::int32_t source_state,
                              const EnemySpottedServices& services,
                              EnemySpottedResult* output) noexcept {
    EnemySpottedResult local{};
    auto& result = output ? *output : local;
    result = {};
    if (!character_identity || !enemy_identity || ai.owner_04 != character_identity ||
        !ai.group_info_identity) return Status::invalid_argument;
    const auto binding_it = bindings_.find(character_identity);
    if (binding_it == bindings_.end()) return Status::not_found;
    const auto& binding = binding_it->second;
    if (binding.ai != &ai || binding.character_identity != character_identity ||
        binding.group_identity != ai.group_info_identity ||
        static_cast<std::uint32_t>(binding.role) != ai.word_38)
        return Status::owner_conflict;

    GroupInfo* group = nullptr;
    for (auto& entry : groups_) {
        if (reinterpret_cast<std::uintptr_t>(entry.second.get()) ==
            binding.group_identity) {
            group = entry.second.get();
            break;
        }
    }
    if (!group) return Status::owner_conflict;
    if (group->flag_41) return Status::complete;

    const auto transition_members = [&](const std::vector<std::uintptr_t>& members) {
        if (members.size() > UINT32_MAX) return Status::invalid_argument;
        for (const auto member : members) {
            if (!member || !services.is_awaiting_to_spawn)
                return Status::not_found;
            bool awaiting = false;
            ++result.awaiting_queries;
            try {
                if (services.is_awaiting_to_spawn(services.context, member, &awaiting))
                    return Status::not_found;
            } catch (...) { return Status::not_found; }
            if (!awaiting) continue;
            if (!services.set_spawn_state) return Status::not_found;
            try {
                if (services.set_spawn_state(services.context, member))
                    return Status::not_found;
            } catch (...) { return Status::not_found; }
            ++result.spawn_transitions;
        }
        return Status::complete;
    };
    const auto transition_owner_if_awaiting = [&]() {
        if (!services.is_awaiting_to_spawn) return Status::not_found;
        bool awaiting = false;
        ++result.awaiting_queries;
        try {
            if (services.is_awaiting_to_spawn(services.context, character_identity,
                                              &awaiting))
                return Status::not_found;
        } catch (...) { return Status::not_found; }
        if (!awaiting) return Status::complete;
        if (!services.set_spawn_state) return Status::not_found;
        try {
            if (services.set_spawn_state(services.context, character_identity))
                return Status::not_found;
        } catch (...) { return Status::not_found; }
        ++result.spawn_transitions;
        return Status::complete;
    };

    // Source CharAI::GroupInfo::OnEnemySpotted at 0x3d27cc switches on the
    // spotted owner's current state, traverses the corresponding retained
    // vectors in order, and calls SM_SetSpawnState(false,false) only after a
    // fresh state-17 predicate. Earlier member effects are not rolled back.
    Status status = Status::complete;
    switch (source_state) {
    case 0:
        if (group->members_master_leader.empty() && group->members_leader.empty()) {
            status = transition_members(group->members_normal_group);
            if (status != Status::complete) return status;
            group->flag_41 = 1;
            result.one_shot_flag_set = 1;
        }
        break;
    case 1:
        status = transition_members(group->members_normal_group);
        if (status != Status::complete) return status;
        status = transition_owner_if_awaiting();
        if (status != Status::complete) return status;
        if (group->members_master_leader.empty()) {
            group->flag_41 = 1;
            result.one_shot_flag_set = 1;
        }
        break;
    case 2:
        status = transition_members(group->members_leader);
        if (status != Status::complete) return status;
        group->flag_41 = 1;
        result.one_shot_flag_set = 1;
        status = transition_members(group->members_normal_group);
        if (status != Status::complete) return status;
        status = transition_owner_if_awaiting();
        if (status != Status::complete) return status;
        if (group->members_master_leader.empty()) {
            group->flag_41 = 1;
            result.one_shot_flag_set = 1;
        }
        break;
    case 3:
        if (group->word_36 == -1) {
            status = transition_members(group->members_normal_group);
            if (status != Status::complete) return status;
            group->word_36 = 0;
            group->flag_41 = 1;
            result.completion_word_set = 1;
            result.one_shot_flag_set = 1;
        }
        break;
    default:
        break;
    }
    return Status::complete;
}

Status Owner::on_died(character_ai_initialization::State& ai,
                      std::uintptr_t character_identity, std::uintptr_t killer,
                      const DeathServices& services) noexcept {
    if (!character_identity || ai.owner_04 != character_identity ||
        !ai.group_info_identity)
        return Status::invalid_argument;
    const auto binding_it = bindings_.find(character_identity);
    if (binding_it == bindings_.end()) return Status::not_found;
    const auto& binding = binding_it->second;
    if (binding.ai != &ai || binding.character_identity != character_identity ||
        binding.group_identity != ai.group_info_identity ||
        static_cast<std::uint32_t>(binding.role) != ai.word_38)
        return Status::owner_conflict;

    GroupInfo* group = nullptr;
    for (auto& entry : groups_) {
        if (reinterpret_cast<std::uintptr_t>(entry.second.get()) == binding.group_identity) {
            group = entry.second.get();
            break;
        }
    }
    if (!group) return Status::owner_conflict;

    switch (binding.role) {
    case Role::normal:
        return Status::complete;
    case Role::master_leader:
        // Source sets +40 before walking Leader (+12), then Normal/Group (+24).
        // It dispatches Cmd_Kill(killer,false) to each Character in those lists.
        group->flag_40 = 1;
        for (const auto member : group->members_leader) {
            if (!services.cmd_kill) return Status::not_found;
            try {
                if (services.cmd_kill(services.context, member, killer, false))
                    continue;
            } catch (...) { return Status::not_found; }
            return Status::not_found;
        }
        for (const auto member : group->members_normal_group) {
            if (!services.cmd_kill) return Status::not_found;
            try {
                if (services.cmd_kill(services.context, member, killer, false))
                    continue;
            } catch (...) { return Status::not_found; }
            return Status::not_found;
        }
        return Status::complete;
    case Role::leader: {
        if (group->flag_40) return Status::complete;
        // The source commits its optimistic one-shot flag before querying any
        // member. A provider failure therefore keeps the earlier source write.
        group->flag_40 = 1;
        for (const auto member : group->members_leader) {
            if (!services.is_dead) return Status::not_found;
            bool dead = false;
            try {
                if (!services.is_dead(services.context, member, &dead))
                    return Status::not_found;
            } catch (...) { return Status::not_found; }
            if (!dead) {
                group->flag_40 = 0;
                break; // Source stops on the first live Character.
            }
        }
        return Status::complete;
    }
    case Role::group: {
        // Group-role completion is the source int at +36. Empty vectors are
        // vacuously complete and do not require a Character::IsDead provider.
        if (group->word_36 != 0) return Status::complete;
        bool all_dead = true;
        for (const auto member : group->members_normal_group) {
            if (!services.is_dead) return Status::not_found;
            bool dead = false;
            try {
                if (!services.is_dead(services.context, member, &dead))
                    return Status::not_found;
            } catch (...) { return Status::not_found; }
            if (!dead) {
                all_dead = false;
                break; // Source stops on the first live Character.
            }
        }
        group->word_36 = all_dead ? 1 : 0;
        return Status::complete;
    }
    }
    return Status::invalid_argument;
}

bool Owner::read_tail(std::uintptr_t group_identity, Tail* output) const noexcept {
    if (!group_identity || !output || !owns(group_identity)) return false;
    for (const auto& entry : groups_) {
        const auto* group = entry.second.get();
        if (reinterpret_cast<std::uintptr_t>(group) != group_identity) continue;
        *output = {group->word_36, group->flag_40, group->flag_41};
        return true;
    }
    return false;
}

bool Owner::write_tail(std::uintptr_t group_identity, const Tail& value) noexcept {
    if (!group_identity || value.flag_40 > 1 || value.flag_41 > 1 ||
        !owns(group_identity)) return false;
    for (auto& entry : groups_) {
        auto* group = entry.second.get();
        if (reinterpret_cast<std::uintptr_t>(group) != group_identity) continue;
        group->word_36 = value.word_36;
        group->flag_40 = value.flag_40;
        group->flag_41 = value.flag_41;
        return true;
    }
    return false;
}

bool Owner::owns(std::uintptr_t group_identity) const noexcept {
    if (!group_identity) return false;
    for (const auto& entry : groups_)
        if (reinterpret_cast<std::uintptr_t>(entry.second.get()) == group_identity)
            return true;
    return false;
}

void Owner::clear() noexcept {
    for (auto& entry : bindings_) {
        auto& binding = entry.second;
        if (binding.ai && binding.ai->owner_04 == entry.first &&
            binding.ai->owner_04 == binding.character_identity &&
            binding.ai->group_info_identity == binding.group_identity)
            binding.ai->group_info_identity = 0;
    }
    bindings_.clear();
    groups_.clear();
}

} // namespace dh2::character_group_info_owner_v1
