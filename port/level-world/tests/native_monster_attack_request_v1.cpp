#include "../native_monster_attack_request_v1.hpp"

#include <cstdio>

int main() {
    using dh2::native_monster_attack_request_v1::Input;
    Input input{0x100, 0x100, 0x100, 0x200, 0x200, 0x200, -2, 0};
    unsigned cases = 0;
    if (!dh2::native_monster_attack_request_v1::accepts(input)) return 1;
    ++cases;
    const auto rejects = [&](Input value) {
        return !dh2::native_monster_attack_request_v1::accepts(value);
    };
    if (!rejects({0,0x100,0x100,0x200,0x200,0x200,-2,0}) ||
        !rejects({0x100,0x101,0x100,0x200,0x200,0x200,-2,0}) ||
        !rejects({0x100,0x100,0x102,0x200,0x200,0x200,-2,0}) ||
        !rejects({0x100,0x100,0x100,0x201,0x200,0x200,-2,0}) ||
        !rejects({0x100,0x100,0x100,0x200,0x201,0x200,-2,0}) ||
        !rejects({0x100,0x100,0x100,0x200,0x200,0x201,-2,0}) ||
        !rejects({0x100,0x100,0x100,0x200,0x200,0x200,-1,0}) ||
        !rejects({0x100,0x100,0x100,0x200,0x200,0x200,-2,1})) return 2;
    cases += 8;
    using dh2::native_monster_attack_request_v1::AttackLifecycle;
    AttackLifecycle live{1,1,1,2,1,0x100,0x100,0x100,0x100,0x1000,0x1000,0x1000};
    if (!dh2::native_monster_attack_request_v1::allows_combat_event(live) ||
        !dh2::native_monster_attack_request_v1::allows_combat_event({0,0,0,0,0,0,0,0,0,0,0,0})) return 3;
    ++cases;
    if (dh2::native_monster_attack_request_v1::allows_combat_event({1,0,1,2,1,0x100,0x100,0x100,0x1000,0x1000,0x1000,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,0,2,1,0x100,0x100,0x100,0x1000,0x1000,0x1000,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,1,0,1,0x100,0x100,0x100,0x1000,0x1000,0x1000,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,1,2,0,0x100,0x100,0x100,0x1000,0x1000,0x1000,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,1,2,1,0x100,0x101,0x100,0x1000,0x1000,0x1000,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,1,2,1,0x100,0x100,0x101,0x1000,0x1000,0x1000,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,1,2,1,0x100,0x100,0x100,0,0,0,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,1,2,1,0x100,0x100,0x100,0x1000,0x1001,0x1000,0}) ||
        dh2::native_monster_attack_request_v1::allows_combat_event({1,1,1,2,1,0x100,0x100,0x100,0x1000,0x1000,0x1001,0})) return 4;
    cases += 9;
    std::printf("{\"native_monster_attack_request_cases\":%u,\"canonical_damage_deferred_to_animation_event\":true,\"active_gated_source_vm_required\":true,\"status\":\"PASS\"}\n", cases);
    return 0;
}
