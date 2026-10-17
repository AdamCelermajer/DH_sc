#include "../player_sct_position_provider_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace dh2::scene { struct Scene {}; }

namespace {
void expect(bool value, const char* message) {
    if (!value) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    using namespace dh2;
    dh2::subobjects::State runtime{};
    runtime.position[0] = 12.5f;
    runtime.position[1] = -3.0f;
    runtime.position[2] = 8.25f;
    runtime.local_bounds[2] = -1.5f;
    runtime.local_bounds[5] = 2.5f;
    scene::Scene visual_scene{};
    std::uint8_t visible = 1;
    player_sct_position_provider_v1::Owner owner{
        0x1234, 0x1234, &runtime, &visual_scene, &visible};
    ui::ScrollingCombatTextPositionFactsV1 facts{};
    std::string error;

    expect(player_sct_position_provider_v1::lookup(
               &owner, 0x1234, facts, error), "canonical Player lookup");
    expect(facts.identity == 0x1234 && facts.game_object_position == runtime.position &&
               facts.relative_box == runtime.local_bounds &&
               facts.visual_scene == &visual_scene && facts.source_visible_80 == 1,
           "provider borrows exact Prince runtime and retained scene projections");

    auto saved = facts;
    expect(!player_sct_position_provider_v1::lookup(
               &owner, 0x4321, facts, error) && facts.identity == saved.identity,
           "foreign Character identity rejected without changing output");
    owner.save_character_identity = 0x4321;
    expect(!player_sct_position_provider_v1::lookup(
               &owner, 0x1234, facts, error), "cross-Save Character rejected");
    owner.save_character_identity = 0x1234;
    owner.source_visible_80 = nullptr;
    expect(!player_sct_position_provider_v1::lookup(
               &owner, 0x1234, facts, error), "missing source visibility fails closed");
    owner.source_visible_80 = &visible;
    visible = 2;
    expect(!player_sct_position_provider_v1::lookup(
               &owner, 0x1234, facts, error), "invalid visibility byte rejected");
    visible = 1;
    runtime.position[1] = 0.0f / 0.0f;
    expect(!player_sct_position_provider_v1::lookup(
               &owner, 0x1234, facts, error), "non-finite position rejected");

    std::cout << "player_sct_position_provider_v1 PASS (6 checks; fail-closed projection only)\n";
}
