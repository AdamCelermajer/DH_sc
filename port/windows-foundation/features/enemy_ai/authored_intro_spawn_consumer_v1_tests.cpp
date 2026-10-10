#include "authored_intro_spawn_consumer_v1.hpp"
#include "../../original_campaign_runtime.hpp"
#include "../../asset_catalog.hpp"
#include "../../actor_definitions.hpp"

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;
namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Repository root required");
        const std::filesystem::path root(argv[1]);
        AssetCatalog source_assets(root / ".local-inputs/windows-encounter-source");
        OriginalCampaignRuntime campaign;
        std::string error;
        check(campaign.load(source_assets, "original-campaign.xml", error), error);

        AssetCatalog game_assets(root / "port/windows-foundation/assets");
        std::vector<ActorDefinition> definitions;
        check(load_actor_definitions(game_assets, "original-cache/data/scene/001_swamp.mlx",
                                     definitions, error), error);
        std::vector<const ActorDefinition*> authored;
        for (const auto& definition : definitions)
            if (definition.name == "_prim_Monster_LizManIntro1" ||
                definition.name == "_prim_Monster_LizManIntro2") authored.push_back(&definition);
        check(authored.size() == 2, "Exact authored intro declarations unavailable");

        SourceWorldObjects source_objects;
        check(source_objects.load(definitions, error), error);
        const int source_module = 7; // Constructor receipt value supplied by this focused test host.
        check(source_objects.bind_module(authored.front()->moduleName, source_module, error), error);

        std::map<ActorId, ActorState> same_session;
        OriginalActorLifecycle lifecycle;
        std::vector<ActorId> spawn_order;
        OriginalLifecycleServices life;
        life.floor_height = [](std::array<float, 3> p, bool& found, float& height,
                               std::string&) { found = false; height = p[2]; return true; };
        life.invoke = [&](const OriginalLifecycleRequest& request, std::string&) {
            if (request.operation == OriginalLifecycleOperation::select_state_animation &&
                request.state == static_cast<int>(OriginalLifecycleState::spawn))
                spawn_order.push_back(request.actor->id);
            return true;
        };
        lifecycle.bind(std::move(life));
        for (const auto* definition : authored) {
            ActorState actor;
            actor.id = definition->stableId;
            actor.health = actor.max_health = 100;
            same_session.emplace(actor.id, actor);
            OriginalLifecycleFacts facts;
            facts.preset_ai_state = "Limbus";
            facts.initially_enabled = false;
            facts.pre_spawn_has_animation = false;
            facts.pre_spawn_stay_enabled = false;
            facts.initial_transform.position = {definition->placement[12],
                                                definition->placement[13],
                                                definition->placement[14]};
            check(lifecycle.add(same_session.at(actor.id), facts, error), error);
        }

        OriginalCampaignWorldProviders providers;
        AuthoredIntroSpawnBindingV1 bound;
        check(bind_authored_intro_spawn_consumer_v1(
                  definitions, source_objects, lifecycle,
                  [&](ActorId id) -> ActorState* {
                      const auto found = same_session.find(id);
                      return found == same_session.end() ? nullptr : &found->second;
                  }, providers, bound, error), error);
        check(bound.module_occurrence == authored.front()->moduleName &&
              bound.first != bound.second && bound.first != invalid_actor_id &&
              bound.second != invalid_actor_id, "Consumer lost exact module/stable identities");

        OriginalCampaignWorldAdapter adapter(lifecycle);
        adapter.bind(providers);
        const auto script_id = campaign.script_id("LizardMan_Intro", false);
        check(script_id >= 0, "Original LizardMan_Intro source script missing");
        const auto& script = campaign.scripts().at(static_cast<std::size_t>(script_id));
        check(script.commands.size() > 6 && script.commands[4].kind == 30 &&
              script.commands[5].kind == 26 && script.commands[6].kind == 30,
              "Original source spawn/wait/spawn event order changed");
        check(script.commands[4].strings.at(12) == "_prim_Monster_LizManIntro1" &&
              script.commands[6].strings.at(12) == "_prim_Monster_LizManIntro2" &&
              script.commands[5].scalars.at(8) == 1500,
              "Original authored names/order/wait changed");

        bool blocking = false;
        check(!adapter.command(CampaignCommandPhase::execute, script.commands[4],
                              source_module + 1, false, blocking, error),
              "Wrong Module context resolved an authored intro actor");
        check(lifecycle.status(bound.first)->state ==
                  static_cast<int>(OriginalLifecycleState::pre_spawn) &&
              lifecycle.status(bound.second)->state ==
                  static_cast<int>(OriginalLifecycleState::pre_spawn) &&
              spawn_order.empty(),
              "Wrong Module request mutated an actor before the valid spawn sequence");

        check(adapter.command(CampaignCommandPhase::execute, script.commands[4],
                              source_module, false, blocking, error), error);
        check(adapter.command(CampaignCommandPhase::execute, script.commands[6],
                              source_module, false, blocking, error), error);
        check(spawn_order == std::vector<ActorId>({bound.first, bound.second}),
              "Source kind30 calls did not spawn the exact same-session actors in authored order");
        check(lifecycle.status(bound.first)->state == static_cast<int>(OriginalLifecycleState::spawn) &&
              lifecycle.status(bound.second)->state == static_cast<int>(OriginalLifecycleState::spawn),
              "Source kind30 did not use OriginalActorLifecycle PreSpawn->Spawn");

        check(!adapter.command(CampaignCommandPhase::execute, script.commands[4],
                               source_module, false, blocking, error),
              "Repeated intro spawn command was accepted after leaving PreSpawn");
        check(spawn_order == std::vector<ActorId>({bound.first, bound.second}),
              "Repeated spawn command invoked lifecycle a second time");
        check(lifecycle.status(bound.first)->state == static_cast<int>(OriginalLifecycleState::spawn),
              "Repeated spawn request changed the first actor state");

        std::cout << "PASS exact 001_swamp authored IDs=" << bound.first << ',' << bound.second
                  << " module_occurrence=" << bound.module_occurrence
                  << " spawn_order=source-kind30[4,6] lifecycle=PreSpawn17->Spawn1"
                  << " wrong_module_pre_mutation_rejected=true duplicate_rejected=true"
                  << " trigger/prefix_providers=not_claimed\n";
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
    return 0;
}
