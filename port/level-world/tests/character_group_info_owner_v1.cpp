#include "../character_group_info_owner_v1.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct DeathTrace {
    std::uintptr_t expected[2]{};
    std::uintptr_t seen[2]{};
    std::size_t calls{};
};
bool source_cmd_kill(void* raw, std::uintptr_t character,
                     std::uintptr_t killer, bool force) {
    auto& trace = *static_cast<DeathTrace*>(raw);
    if (trace.calls >= 2 || killer != 0x900001 || force) return false;
    trace.seen[trace.calls++] = character;
    return true;
}
struct SpawnTrace {
    std::unordered_map<std::uintptr_t, bool> awaiting;
    std::vector<std::uintptr_t> queries;
    std::vector<std::uintptr_t> transitions;
    std::uintptr_t fail_transition{};
};
int source_is_awaiting(void* raw, std::uintptr_t character, bool* awaiting) {
    auto& trace = *static_cast<SpawnTrace*>(raw);
    if (!character || !awaiting) return 1;
    trace.queries.push_back(character);
    *awaiting = trace.awaiting[character];
    return 0;
}
int source_set_spawn(void* raw, std::uintptr_t character) {
    auto& trace = *static_cast<SpawnTrace*>(raw);
    if (!character || character == trace.fail_transition) return 1;
    trace.transitions.push_back(character);
    trace.awaiting[character] = false;
    return 0;
}
}

int main() {
    using namespace dh2::character_ai_initialization;
    using namespace dh2::character_group_info_owner_v1;
    using GroupStatus = dh2::character_group_info_owner_v1::Status;
    try {
        Owner owner;
        State normal{}; normal.identity = 0x100001; normal.owner_04 = 0x200001;
        State leader{}; leader.identity = 0x100002; leader.owner_04 = 0x200002;
        State peer_normal{}; peer_normal.identity = 0x100005;
        peer_normal.owner_04 = 0x200005;
        State other_module{}; other_module.identity = 0x100003;
        other_module.owner_04 = 0x200003;
        std::uintptr_t normal_group = 0, leader_group = 0, separate_group = 0;
        require(owner.add_to_group(normal, normal.owner_04, 0, "crypt-party",
                    "MasterLeader", &normal_group) ==
                    dh2::character_group_info_owner_v1::Status::complete,
                "source MasterLeader Character did not join GroupInfo first vector");
        require(owner.add_to_group(leader, leader.owner_04, 0, "crypt-party",
                    "Leader", &leader_group) ==
                    dh2::character_group_info_owner_v1::Status::complete,
                "source Leader Character did not join GroupInfo second vector");
        require(owner.add_to_group(peer_normal, peer_normal.owner_04, 0,
                    "crypt-party", "Normal") ==
                    dh2::character_group_info_owner_v1::Status::complete,
                "source Normal Character did not join GroupInfo third vector");
        require(normal_group == leader_group && normal.group_info_identity == normal_group &&
                    leader.group_info_identity == normal_group && owner.group_count() == 1 &&
                    normal.word_38 == 2 && leader.word_38 == 1,
                "same source group name/module key did not share one full-width owner");
        require(owner.add_to_group(other_module, other_module.owner_04, 1,
                    "crypt-party", "Group", &separate_group) ==
                    dh2::character_group_info_owner_v1::Status::complete &&
                    separate_group != normal_group && owner.group_count() == 2,
                "Character+0x64 module key did not separate source GroupInfo owners");
        Tail tail{};
        require(owner.read_tail(normal_group, &tail) && tail.word_36 == -1 &&
                    tail.flag_40 == 0 && tail.flag_41 == 0,
                "new source GroupInfo did not retain constructor tail bytes");
        DeathTrace trace{{leader.owner_04, peer_normal.owner_04}};
        const Owner::DeathServices death_services{&trace,nullptr,source_cmd_kill};
        require(owner.on_died(normal, normal.owner_04, 0x900001, death_services) ==
                    dh2::character_group_info_owner_v1::Status::complete && trace.calls == 2 &&
                    trace.seen[0] == leader.owner_04 &&
                    trace.seen[1] == peer_normal.owner_04,
                "MasterLeader OnDied did not Cmd_Kill Character members in source vector order");
        require(owner.read_tail(normal_group, &tail) && tail.flag_40 == 1,
                "MasterLeader OnDied did not set GroupInfo+40 before dispatch");

        State idle_normal{}; idle_normal.identity = 0x110001;
        idle_normal.owner_04 = 0x210001;
        State idle_peer{}; idle_peer.identity = 0x110002;
        idle_peer.owner_04 = 0x210002;
        std::uintptr_t idle_group = 0;
        require(owner.add_to_group(idle_normal, idle_normal.owner_04, 0,
                    "spotted-idle", "Normal", &idle_group) == GroupStatus::complete &&
                    owner.add_to_group(idle_peer, idle_peer.owner_04, 0,
                    "spotted-idle", "Normal") == GroupStatus::complete,
                "source idle-spotted GroupInfo members were not retained");
        SpawnTrace spawn_trace{};
        spawn_trace.awaiting[idle_normal.owner_04] = true;
        spawn_trace.awaiting[idle_peer.owner_04] = true;
        const Owner::EnemySpottedServices spotted_services{
            &spawn_trace, source_is_awaiting, source_set_spawn};
        Owner::EnemySpottedResult spotted_result{};
        require(owner.on_enemy_spotted(idle_normal, idle_normal.owner_04,
                    0x900002, 0, spotted_services, &spotted_result) == GroupStatus::complete &&
                    spawn_trace.transitions.size() == 2 &&
                    spawn_trace.transitions[0] == idle_normal.owner_04 &&
                    spawn_trace.transitions[1] == idle_peer.owner_04 &&
                    spotted_result.one_shot_flag_set && idle_group &&
                    owner.read_tail(idle_group, &tail) && tail.flag_41 == 1,
                "state-0 GroupInfo::OnEnemySpotted did not spawn normal members then set +0x41");
        const auto queries_after_idle = spawn_trace.queries.size();
        require(owner.on_enemy_spotted(idle_normal, idle_normal.owner_04,
                    0x900002, 0, spotted_services, &spotted_result) == GroupStatus::complete &&
                    spawn_trace.queries.size() == queries_after_idle,
                "GroupInfo +0x41 did not suppress the second source spotted pass");

        State state_one_master{}; state_one_master.identity = 0x110007;
        state_one_master.owner_04 = 0x210007;
        State state_one_normal{}; state_one_normal.identity = 0x110008;
        state_one_normal.owner_04 = 0x210008;
        State state_one_peer{}; state_one_peer.identity = 0x110009;
        state_one_peer.owner_04 = 0x210009;
        std::uintptr_t state_one_group = 0;
        require(owner.add_to_group(state_one_master, state_one_master.owner_04, 0,
                    "spotted-state-one", "MasterLeader", &state_one_group) == GroupStatus::complete &&
                    owner.add_to_group(state_one_normal, state_one_normal.owner_04, 0,
                    "spotted-state-one", "Normal") == GroupStatus::complete &&
                    owner.add_to_group(state_one_peer, state_one_peer.owner_04, 0,
                    "spotted-state-one", "Group") == GroupStatus::complete,
                "state-1 spotted GroupInfo members were not retained");
        SpawnTrace state_one_trace{};
        state_one_trace.awaiting[state_one_normal.owner_04] = true;
        state_one_trace.awaiting[state_one_peer.owner_04] = true;
        const Owner::EnemySpottedServices state_one_services{
            &state_one_trace, source_is_awaiting, source_set_spawn};
        require(owner.on_enemy_spotted(state_one_normal, state_one_normal.owner_04,
                    0x900005, 1, state_one_services, &spotted_result) == GroupStatus::complete &&
                    state_one_trace.transitions.size() == 2 &&
                    state_one_trace.transitions[0] == state_one_normal.owner_04 &&
                    state_one_trace.transitions[1] == state_one_peer.owner_04 &&
                    std::count(state_one_trace.queries.begin(), state_one_trace.queries.end(),
                        state_one_normal.owner_04) == 2 &&
                    owner.read_tail(state_one_group, &tail) && tail.flag_41 == 0,
                "state-1 GroupInfo::OnEnemySpotted did not transition normal members then re-query owner");

        State state_two_leader{}; state_two_leader.identity = 0x110003;
        state_two_leader.owner_04 = 0x210003;
        State state_two_peer{}; state_two_peer.identity = 0x110004;
        state_two_peer.owner_04 = 0x210004;
        State state_two_normal{}; state_two_normal.identity = 0x110005;
        state_two_normal.owner_04 = 0x210005;
        std::uintptr_t state_two_group = 0;
        require(owner.add_to_group(state_two_leader, state_two_leader.owner_04, 0,
                    "spotted-state-two", "Leader", &state_two_group) == GroupStatus::complete &&
                    owner.add_to_group(state_two_peer, state_two_peer.owner_04, 0,
                    "spotted-state-two", "Leader") == GroupStatus::complete &&
                    owner.add_to_group(state_two_normal, state_two_normal.owner_04, 0,
                    "spotted-state-two", "Normal") == GroupStatus::complete,
                "state-2 spotted GroupInfo members were not retained");
        SpawnTrace state_two_trace{};
        state_two_trace.awaiting[state_two_leader.owner_04] = true;
        state_two_trace.awaiting[state_two_peer.owner_04] = true;
        state_two_trace.awaiting[state_two_normal.owner_04] = true;
        const Owner::EnemySpottedServices state_two_services{
            &state_two_trace, source_is_awaiting, source_set_spawn};
        require(owner.on_enemy_spotted(state_two_leader, state_two_leader.owner_04,
                    0x900003, 2, state_two_services, &spotted_result) == GroupStatus::complete &&
                    state_two_trace.transitions.size() == 3 &&
                    state_two_trace.transitions[0] == state_two_leader.owner_04 &&
                    state_two_trace.transitions[1] == state_two_peer.owner_04 &&
                    state_two_trace.transitions[2] == state_two_normal.owner_04 &&
                    owner.read_tail(state_two_group, &tail) && tail.flag_41 == 1,
                "state-2 GroupInfo::OnEnemySpotted did not walk leaders before normal members");

        State state_three{}; state_three.identity = 0x110006;
        state_three.owner_04 = 0x210006;
        std::uintptr_t state_three_group = 0;
        require(owner.add_to_group(state_three, state_three.owner_04, 0,
                    "spotted-state-three", "Group", &state_three_group) == GroupStatus::complete,
                "state-3 spotted GroupInfo was not retained");
        SpawnTrace state_three_trace{};
        state_three_trace.awaiting[state_three.owner_04] = true;
        const Owner::EnemySpottedServices state_three_services{
            &state_three_trace, source_is_awaiting, source_set_spawn};
        require(owner.on_enemy_spotted(state_three, state_three.owner_04,
                    0x900004, 3, state_three_services, &spotted_result) == GroupStatus::complete &&
                    state_three_trace.transitions.size() == 1 &&
                    owner.read_tail(state_three_group, &tail) && tail.word_36 == 0 &&
                    tail.flag_41 == 1 && spotted_result.completion_word_set,
                "state-3 GroupInfo::OnEnemySpotted did not spawn members and set +0x36/+0x41");
        const Tail restored_tail{77, 0, 1};
        require(owner.write_tail(normal_group, restored_tail) &&
                    owner.read_tail(normal_group, &tail) && tail.word_36 == 77 &&
                    tail.flag_40 == 0 && tail.flag_41 == 1 &&
                    !owner.write_tail(0xdeadbeef, restored_tail) &&
                    !owner.write_tail(normal_group, Tail{0, 2, 0}),
                "Character::Deserialize GroupInfo tail did not update only an existing valid owner");
        require(owner.remove_from_group(leader) ==
                    dh2::character_group_info_owner_v1::Status::complete &&
                    leader.group_info_identity == normal_group &&
                    leader.word_38 == 1 && normal.group_info_identity == normal_group &&
                    owner.group_count() == 6,
                "RemoveFromGroup did not remove only the member from its source vector");
        require(owner.remove_from_group(leader) == GroupStatus::complete &&
                    leader.group_info_identity == normal_group && owner.group_count() == 6,
                "repeated source RemoveFromGroup did not preserve its no-op behavior");

        Owner lifecycle_owner;
        State lifecycle_master{}; lifecycle_master.identity = 0x120001;
        lifecycle_master.owner_04 = 0x220001;
        State lifecycle_leader{}; lifecycle_leader.identity = 0x120002;
        lifecycle_leader.owner_04 = 0x220002;
        std::uintptr_t lifecycle_group = 0;
        require(lifecycle_owner.add_to_group(lifecycle_master,
                    lifecycle_master.owner_04, 0, "death-order", "MasterLeader",
                    &lifecycle_group) == GroupStatus::complete &&
                    lifecycle_owner.add_to_group(lifecycle_leader,
                    lifecycle_leader.owner_04, 0, "death-order", "Leader") ==
                    GroupStatus::complete,
                "lifecycle OnDied fixture group did not retain its members");
        const Owner::DeathServices missing_kill_service{nullptr, nullptr, nullptr};
        require(lifecycle_owner.on_died(lifecycle_master,
                    lifecycle_master.owner_04, 0x900010, missing_kill_service) ==
                    GroupStatus::not_found &&
                    lifecycle_owner.read_tail(lifecycle_group, &tail) && tail.flag_40 == 1,
                "state-2 OnDied did not set +0x40 before a missing Cmd_Kill provider");

        Owner empty_death_owner;
        State lone_master{}; lone_master.identity = 0x120010;
        lone_master.owner_04 = 0x220010;
        std::uintptr_t lone_master_group = 0;
        require(empty_death_owner.add_to_group(lone_master, lone_master.owner_04, 0,
                    "empty-master", "MasterLeader", &lone_master_group) ==
                    GroupStatus::complete &&
                    empty_death_owner.remove_from_group(lone_master) == GroupStatus::complete &&
                    empty_death_owner.on_died(lone_master, lone_master.owner_04,
                    0x900011, missing_kill_service) == GroupStatus::complete &&
                    empty_death_owner.read_tail(lone_master_group, &tail) && tail.flag_40 == 1,
                "state-2 OnDied required Cmd_Kill for empty source member vectors");

        State lone_leader{}; lone_leader.identity = 0x120011;
        lone_leader.owner_04 = 0x220011;
        std::uintptr_t lone_leader_group = 0;
        require(empty_death_owner.add_to_group(lone_leader, lone_leader.owner_04, 0,
                    "empty-leader", "Leader", &lone_leader_group) == GroupStatus::complete &&
                    empty_death_owner.remove_from_group(lone_leader) == GroupStatus::complete &&
                    empty_death_owner.on_died(lone_leader, lone_leader.owner_04,
                    0x900012, missing_kill_service) == GroupStatus::complete &&
                    empty_death_owner.read_tail(lone_leader_group, &tail) && tail.flag_40 == 1,
                "state-1 OnDied did not retain optimistic +0x40 for an empty member vector");

        State lone_group{}; lone_group.identity = 0x120012;
        lone_group.owner_04 = 0x220012;
        std::uintptr_t lone_group_id = 0;
        require(empty_death_owner.add_to_group(lone_group, lone_group.owner_04, 0,
                    "empty-group", "Group", &lone_group_id) == GroupStatus::complete &&
                    empty_death_owner.write_tail(lone_group_id, Tail{0, 0, 0}) &&
                    empty_death_owner.remove_from_group(lone_group) == GroupStatus::complete &&
                    empty_death_owner.on_died(lone_group, lone_group.owner_04,
                    0x900013, missing_kill_service) == GroupStatus::complete &&
                    empty_death_owner.read_tail(lone_group_id, &tail) && tail.word_36 == 1,
                "state-3 OnDied required Character::IsDead for an empty source member vector");

        State live_leader{}; live_leader.identity = 0x120013;
        live_leader.owner_04 = 0x220013;
        State live_peer{}; live_peer.identity = 0x120014;
        live_peer.owner_04 = 0x220014;
        std::uintptr_t live_leader_group = 0;
        require(empty_death_owner.add_to_group(live_leader, live_leader.owner_04, 0,
                    "missing-dead-provider", "Leader", &live_leader_group) ==
                    GroupStatus::complete &&
                    empty_death_owner.add_to_group(live_peer, live_peer.owner_04, 0,
                    "missing-dead-provider", "Leader") == GroupStatus::complete &&
                    empty_death_owner.remove_from_group(live_leader) == GroupStatus::complete &&
                    empty_death_owner.on_died(live_leader, live_leader.owner_04,
                    0x900014, missing_kill_service) == GroupStatus::not_found &&
                    empty_death_owner.read_tail(live_leader_group, &tail) && tail.flag_40 == 1,
                "state-1 OnDied rolled back source +0x40 after a missing IsDead provider");
        State ungrouped{}; ungrouped.identity = 0x100004; ungrouped.owner_04 = 0x200004;
        require(owner.add_to_group(ungrouped, ungrouped.owner_04, 0, "", "Normal") ==
                    dh2::character_group_info_owner_v1::Status::source_ignored &&
                    owner.group_count() == 6 &&
                    !ungrouped.group_info_identity,
                "empty source char_group created or assigned a GroupInfo");
        owner.clear();
        require(owner.group_count() == 0 && owner.binding_count() == 0 &&
                    !normal.group_info_identity && !other_module.group_info_identity,
                "level GroupInfo retirement left stale actor pointers");
        std::cout << "character_group_info_owner_v1: PASS\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "character_group_info_owner_v1: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
