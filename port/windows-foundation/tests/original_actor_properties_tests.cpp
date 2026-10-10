#include "../original_actor_properties.hpp"
#include "../asset_catalog.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
static void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}
int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("Supply staged original asset root");
        AssetCatalog assets(argv[1]); OriginalPropertyDatabase database; std::string error;
        check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
        unsigned player_index = 0;
        const std::int32_t original_hp[] = {42265, 37580, 36096};
        const std::int32_t original_mp[] = {6976, 10304, 20096};
        const std::int32_t original_classes[] = {77, 120, 94};
        for (const auto* row : {"KnightPlayerBase", "RoguePlayerBase", "MagePlayerBase"}) {
            OriginalActorProperties actor;
            check(resolve_original_fresh_player(database,row,actor,error),error);
            check(actor.level_raw == 256, "fresh original player level differs");
            check(actor.max_health > 0 && actor.max_resource > 0, "original class maxima unresolved");
            check(actor.health == actor.max_health && actor.resource == actor.max_resource,
                  "fresh vital refill differs");
            check(actor.class_id >= 0, "original class ID absent");
            check(actor.sheets.resolved[36] == original_hp[player_index] &&
                  actor.sheets.resolved[41] == original_mp[player_index] &&
                  actor.class_id == original_classes[player_index], "original cache regression");
            ++player_index;
            std::cout << row << " HP=" << actor.health << " MP=" << actor.resource
                      << " class=" << actor.class_id << " faction=" << actor.faction_id
                      << " walk=" << actor.walk_multiplier << " turn=" << actor.turn_radians_per_second
                      << " rawHP=" << actor.sheets.resolved[36] << " rawMP=" << actor.sheets.resolved[41] << '\n';
        }
        OriginalActorProperties mob;
        check(resolve_original_actor_properties(database.characters,database.classes,
            "Swamp_LizadMan_Type1", {}, mob,error),error);
        std::cout << "Swamp_LizadMan_Type1 HP=" << mob.health << " MaxHP=" << mob.max_health
                  << " MP=" << mob.resource << " MaxMP=" << mob.max_resource
                  << " levelRaw=" << mob.level_raw << " class=" << mob.class_id
                  << " rawHP=" << mob.sheets.resolved[36] << '\n';
        check(mob.sheets.resolved[36] == -1 && mob.max_health == 47.5f && mob.max_resource == 22,
              "original mob sentinel/class maxima regression");
        check(resolve_original_actor_properties(database.characters,database.classes,
            "Swamp_LizadMan_Type1", {std::nullopt, true}, mob,error),error);
        check(mob.health == 47.5f && mob.resource == 22,
              "explicit mob spawn refill did not apply original maxima");
        OriginalActorProperties npc;
        check(resolve_original_actor_properties(database.characters,database.classes,
            "WanderingPriest", {}, npc,error),error);
        std::cout << "WanderingPriest HP=" << npc.health << " MaxHP=" << npc.max_health
                  << " MP=" << npc.resource << " class=" << npc.class_id << '\n';
        OriginalActorProperties preserved = npc;
        check(!resolve_original_fresh_player(database,"WanderingPriest",npc,error),"NPC accepted as fresh player");
        check(npc.sheets.resolved == preserved.sheets.resolved,"failed resolve mutated output");
        auto malformed = database.characters; malformed.fields[16] = "wrong";
        check(!resolve_original_actor_properties(malformed,database.classes,"KnightPlayerBase",{},npc,error),
              "different original layout accepted");
        check(npc.sheets.resolved == preserved.sheets.resolved,"layout failure mutated output");
        check(original_speed_modifier(-25600) == 0 && original_speed_modifier(25600) == 2,
              "original percent modifier clamp differs");
        check(!load_original_property_tables(assets,"missing-table-root",database,error),"missing tables accepted");
        check(!database.characters.names.empty(),"failed table load discarded old database");
        std::cout << "original_actor_properties PASS\n";
    } catch (const std::exception& ex) { std::cerr << ex.what() << '\n'; return 1; }
}
