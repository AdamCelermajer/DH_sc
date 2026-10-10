#include "source_campaign_backend_v1.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_character_zoning_v108.hpp"

#include "../../../level-world/canonical_object_manager_v1.hpp"
#include "../../../level-world/canonical_property_map_v1.hpp"
#include "../../../level-world/gameobject_scene_root_registry_v1.hpp"
#include "../../../level-world/character_world_runtime_v1.hpp"
#include "../../../level-world/floors.hpp"
#include "../../../level-loader/canonical_level_context_v1.hpp"
#include "../../../level-loader/native_gslevel_runtime_v27.hpp"

#include <iostream>
#include <stdexcept>

using namespace dh::foundation::actor_frame;
using namespace dh2;

namespace {
void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

struct ActualWorldStorage {
    data::AiTables ai;
    character::skills::CharacterWorldRuntimeV1 targets;
    ActualWorldStorage() : targets(ai) {}
};

struct EmptySourceGraph {
    std::shared_ptr<application::ApplicationServicesOwnerV5> app =
        std::make_shared<application::ApplicationServicesOwnerV5>();
    std::shared_ptr<ActualWorldStorage> actual_world = std::make_shared<ActualWorldStorage>();
    std::shared_ptr<void> world_owner = actual_world;
    std::shared_ptr<world::CanonicalObjectManagerV1> objects =
        std::make_shared<world::CanonicalObjectManagerV1>(world::CanonicalObjectManagerServicesV1{});
    world::CanonicalPropertyMapV1 properties{world::CanonicalPropertySourceServicesV1{}};
    std::shared_ptr<world::GameObjectSceneRootRegistryV1> roots =
        std::make_shared<world::GameObjectSceneRootRegistryV1>();
    std::shared_ptr<floors::World> floors = std::make_shared<floors::World>();
    std::shared_ptr<void> navigation = std::make_shared<int>(7);
    std::shared_ptr<loader::NativeGSLevelGlobalsV27> gs =
        std::make_shared<loader::NativeGSLevelGlobalsV27>();
    std::shared_ptr<loader::NativeGSLevelRuntimeV27> gs_runtime =
        std::make_shared<loader::NativeGSLevelRuntimeV27>(gs);
    data::LevelTables levels;
    std::shared_ptr<character::skills::CharacterWorldRuntimeV1> targets{
        actual_world, &actual_world->targets};
    std::shared_ptr<loader::CanonicalLevelContextV1> candidate_level;
    std::shared_ptr<model_renderer::SourceCanonicalBorrowV61> canonical;
    std::shared_ptr<model_renderer::SourceWorldBorrowV61> source_world =
        std::make_shared<model_renderer::SourceWorldBorrowV61>();
    std::shared_ptr<SourceCurrentLevelBackendV1> current_level;
    unsigned zoning_calls{};
    std::shared_ptr<world::CanonicalCharacterCandidateFactoryV60> characters =
        std::make_shared<world::CanonicalCharacterCandidateFactoryV60>(
            world::CanonicalCharacterCandidateServicesV60{});

    EmptySourceGraph() {
        std::string error;
        loader::LevelSourceRequestV1 request;
        request.identity = "menu-candidate-level";
        request.definition = "menu-candidate.mlx";
        request.kind = loader::LevelSourceKindV1::fixed;
        auto level_services = std::make_shared<int>(8);
        check(loader::CanonicalLevelContextV1::create(request, level_services,
                candidate_level, error), "create retained candidate Level");

        canonical = std::make_shared<model_renderer::SourceCanonicalBorrowV61>(
            world_owner, objects, *objects, properties, roots);
        source_world->owner = world_owner;
        source_world->application = app;
        source_world->canonical_world = canonical;

        SourceCurrentLevelGraphV1 current_graph;
        current_graph.root_scope = world_owner;
        current_graph.application = app;
        current_graph.world = world_owner;
        current_graph.objects = objects;
        current_graph.navigation = navigation;
        current_graph.floors = floors;
        current_graph.gs_globals = gs;
        current_graph.gs_runtime = gs_runtime;
        current_graph.actor_world = targets;
        current_graph.level_tables = &levels;
        current_level = std::make_shared<SourceCurrentLevelBackendV1>(std::move(current_graph));
    }

    SourceCampaignBackendGraphV1 graph(SourceCampaignBackendModeV1 mode) {
        SourceCampaignBackendGraphV1 result;
        result.mode = mode;
        result.world = source_world;
        result.characters = characters;
        result.current_level = current_level;
        result.candidate.actual_world = world_owner;
        result.candidate.application = app;
        result.candidate.level = candidate_level;
        result.candidate.objects = objects;
        result.candidate.properties = &properties;
        result.candidate.floors = floors;
        result.candidate.roots = roots;
        result.services.level = candidate_level;
        result.services.zoning = [this](std::uintptr_t, bool, std::string&) {
            ++zoning_calls;
            return true;
        };
        return result;
    }
};
}

int main() {
    try {
        EmptySourceGraph graph;
        check(!graph.gs->s_level, "GS source slot is genuinely empty before validation");
        check(!graph.gs_runtime->connection(), "GS runtime has no constructed connection");
        check(!graph.current_level->construction_attempted(), "current-Level backend has not attempted construction");
        check(graph.gs_runtime->owns_globals_v50(graph.gs), "same native GS runtime owns same global storage");

        auto menu = std::make_shared<SourceCampaignBackendContextV1>(
            graph.graph(SourceCampaignBackendModeV1::menu));
        std::string error;
        check(menu->validate(error), "menu mode accepts the actual empty GS slot and candidate Level");
        check(SourceCampaignBackendContextV1::register_current(menu, error),
            "register menu context from the real source graph");

        model_renderer::SourceCampaignCandidateBorrowV55 actual;
        check(model_renderer::borrow_source_campaign_candidate_v55(actual, error),
            "existing candidate helper borrows the menu graph");
        std::shared_ptr<model_renderer::SourceWorldBorrowV61> world_borrow;
        check(model_renderer::borrow_source_campaign_condition_world_v70(actual, world_borrow, error) &&
                world_borrow == graph.source_world,
            "existing condition-world helper borrows same menu World");

        // A reached campaign-level service fails at its mode guard before the
        // service callback can run, even though candidateLevel is nonnull.
        // The registered context remains the same source graph; no mock current
        // callback is substituted for the empty GS slot.
        check(!model_renderer::source_campaign_character_zoning_v108(
                  graph.world_owner, 0x1234, true, error) && graph.zoning_calls == 0,
            "menu zoning rejects reached Level-dependent callback");
        check(error.find("actual nonnull campaign Level") != std::string::npos,
            "menu zoning reports the exact required Level provider");

        SourceCampaignBackendContextV1::unregister_current(menu);
        auto campaign = std::make_shared<SourceCampaignBackendContextV1>(
            graph.graph(SourceCampaignBackendModeV1::campaign));
        check(!campaign->validate(error), "campaign mode rejects empty actual GS slot despite candidate Level");
        check(error.find("campaign GS s_level") != std::string::npos,
            "campaign mode preserves actual empty GS s_level failure");
        check(!graph.gs->s_level && !graph.current_level->construction_attempted(),
            "failed campaign validation did not synthesize/null-recover a Level");
        std::cout << "PASS: verified empty source GS slot admits menu candidate lookup; campaign-only callbacks and campaign mode still require live Level\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
