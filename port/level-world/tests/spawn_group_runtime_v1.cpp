#include "../spawn_group_runtime_v1.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace spawn = dh2::spawn_group_runtime_v1;

struct Fixture {
    std::vector<std::uint32_t> draws;
    std::size_t draw_index{};
    std::vector<std::string> names;
    std::vector<std::uintptr_t> placed_spots;
    std::vector<std::int32_t> character_ids;
    std::vector<std::array<float, 3>> positions;
    std::vector<std::uintptr_t> actors;
    std::uint32_t next_actor{1};
    bool fail_first_create{true};
    bool source_is_null{true};
    bool type_is_character{true};
    bool skip_init_post{true};
};

bool random_below(void* context, std::uint32_t bound, std::uint32_t* value) {
    auto& f = *static_cast<Fixture*>(context);
    if (f.draw_index >= f.draws.size()) return false;
    *value = f.draws[f.draw_index++];
    return *value < bound;
}

std::uintptr_t create(void* context, const char* source, const char* type,
                      const char* name, bool skip_init_post) {
    auto& f = *static_cast<Fixture*>(context);
    f.source_is_null &= source == nullptr;
    f.type_is_character &= std::string(type) == "Character";
    f.skip_init_post &= skip_init_post;
    f.names.emplace_back(name);
    if (f.fail_first_create && f.names.size() == 1) return 0;
    return f.next_actor++;
}

void init_spawned(void* context, std::uintptr_t actor,
                  std::int32_t character_id, const float position[3]) {
    auto& f = *static_cast<Fixture*>(context);
    f.actors.push_back(actor);
    f.character_ids.push_back(character_id);
    f.positions.push_back({position[0], position[1], position[2]});
}

void place_object(void* context, std::uintptr_t spot, std::uintptr_t actor) {
    auto& f = *static_cast<Fixture*>(context);
    if (!f.actors.empty() && f.actors.back() == actor)
        f.placed_spots.push_back(spot);
}

int main() {
    Fixture f{};
    f.draws = {0, 0, 0}; // weighted entry, then each remaining eligible spot
    const spawn::Services services{&f, &random_below, &create,
                                   &init_spawned, &place_object};
    spawn::Owner owner;
    owner.define_group(9, spawn::GroupDefinition{
        true, 2500, {{-1, 10, 3}}
    });
    const spawn::Spot outside{101, true, true, false, {1.0f, 2.0f, 3.0f}};
    const spawn::Spot inside{102, true, true, true, {4.0f, 5.0f, 6.0f}};
    const spawn::Spot non_interactive{103, false, true, false,
                                      {7.0f, 8.0f, 9.0f}};
    bool ok = owner.add_spot(9, outside) && owner.add_spot(9, inside) &&
              owner.add_spot(9, non_interactive);
    ok &= owner.update(0, &services) == spawn::Status::complete;
    ok &= owner.timer_ms(9) == 2500;
    ok &= f.names.size() == 2 && f.names[0] == "Spawn_00001" &&
          f.names[1] == "Spawn_00002";
    ok &= f.source_is_null && f.type_is_character && f.skip_init_post;
    ok &= f.character_ids.size() == 1 && f.character_ids[0] == -1;
    ok &= f.positions.size() == 1 && f.positions[0][0] == 7.0f;
    ok &= f.placed_spots.size() == 1 && f.placed_spots[0] == 103;
    ok &= owner.update(500, &services) == spawn::Status::complete;
    ok &= owner.timer_ms(9) == 2000;

    spawn::Owner empty_owner;
    empty_owner.define_group(3, spawn::GroupDefinition{true, 4000, {{7, 1, 1}}});
    const spawn::Spot only_outside{201, true, true, false, {}};
    ok &= empty_owner.add_spot(3, only_outside);
    Fixture no_spots{};
    no_spots.draws = {0};
    const spawn::Services no_spot_services{&no_spots, &random_below, &create,
                                          &init_spawned, &place_object};
    ok &= empty_owner.update(0, &no_spot_services) == spawn::Status::complete;
    ok &= empty_owner.timer_ms(3) == 1000;
    ok &= no_spots.names.empty();

    empty_owner.remove_spot(201);
    ok &= empty_owner.group_count() == 0;

    if (!ok) {
        std::cerr << "checks: names=" << f.names.size()
                  << " timer=" << owner.timer_ms(9)
                  << " chars=" << f.character_ids.size()
                  << " positions=" << f.positions.size()
                  << " placed=" << f.placed_spots.size()
                  << " eligible-retry=" << empty_owner.timer_ms(3)
                  << " groups-after-remove=" << empty_owner.group_count()
                  << " draws=" << f.draw_index << '\n';
        for (const auto& name : f.names) std::cerr << name << ' ';
        std::cerr << '\n';
        return 1;
    }
    std::cout << "{\"weighted_spawn_handoff\":true,"
                 "\"active_spot_filter_and_removal\":true,"
                 "\"empty_spot_retry_timer\":true,\"mismatches\":0}\n";
    return 0;
}
