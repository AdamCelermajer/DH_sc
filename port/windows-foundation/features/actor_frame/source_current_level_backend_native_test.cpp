// Reuse the existing native GS/C1 test's original-data fixture helpers, but
// exercise the foundation-owned current-Level provider around that runtime.
#define main prior_native_gslevel_runtime_v27_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "../../../level-loader/tests/native_gslevel_runtime_v27.cpp"
#pragma GCC diagnostic pop
#undef main

#include "source_current_level_backend_v1.hpp"

using namespace dh2;
using namespace dh2::loader;
using namespace dh::foundation::actor_frame;

namespace {
struct ActorFixture {
    target_providers::Handle16 handle{0, UINT32_MAX, 0};
    std::uint32_t type{3};
    std::uint8_t across{};
    std::int32_t room{-1};
    target_search::Object48 search{};
    std::shared_ptr<void> lease;
    std::string name, archetype;
    explicit ActorFixture(std::uintptr_t id) { search.identity = id; }
    static int refresh(void* raw, character::skills::WorldTargetActorBorrowV1* out) {
        auto& self = *static_cast<ActorFixture*>(raw);
        *out = {};
        out->identity = self.search.identity;
        out->search = &self.search;
        out->receiver_lease = self.lease;
        return 0;
    }
    static bool set_name(void* raw, const char* value, std::string&) {
        static_cast<ActorFixture*>(raw)->name = value; return true;
    }
    static bool set_archetype(void* raw, const char* value, std::string&) {
        static_cast<ActorFixture*>(raw)->archetype = value; return true;
    }
    static bool as_character(void* raw, std::uintptr_t& out, std::string&) {
        out = static_cast<ActorFixture*>(raw)->search.identity; return true;
    }
};
struct WorldRootFixture {
    data::AiTables ai;
    character::skills::CharacterWorldRuntimeV1 actor_world;
    WorldRootFixture() : actor_world(ai) {}
};
}

int main(int argc, char** argv) {
    try {
        check(argc == 4);
        Inputs inputs(argv[1]);
        character::CharacterGameDesign design;
        std::string error;
        check(design.initialize(inputs.input, error));
        auto actual = std::make_shared<character::CharacterGameDesign::Borrow>(design.borrow());
        check(actual->levels() != nullptr);

        auto files = std::make_shared<ApplicationFilesFixture>();
        files->script_dir = argv[2]; files->save_dir = argv[3];
        auto cache = std::make_shared<scripts::LuaScriptCacheOwnerV13>(
            scripts::LuaScriptCacheServicesV13{files, files.get(), ApplicationFilesFixture::script});
        std::uint32_t debug{}, module{};
        LevelConstructorApplicationV4 application;
        application.owner = actual; application.debug_level_load_count = &debug;
        application.module_id_global = &module; application.levels = actual->levels();
        application.lua_cache = cache; application.lua.owner = actual;
        application.lua.design = *actual->design(); application.private_vm_limit = 16u * 1024u * 1024u;
        application.saves.files.context = files.get();
        application.saves.files.read_file = ApplicationFilesFixture::save;
        application.saves.files.storage_lease = files;
        application.online_byte5 = [](std::uint8_t& value, std::string&) { value = 0; return true; };

        auto world_root = std::make_shared<WorldRootFixture>();
        std::shared_ptr<void> world_owner = world_root;
        std::shared_ptr<void> navigation = std::make_shared<int>(2);
        std::shared_ptr<void> floors = std::make_shared<int>(3);
        world::CanonicalObjectManagerServicesV1 manager_services;
        manager_services.assign_network_id = [](void*, world::CanonicalObjectBorrowV1&,
                                                  std::string&) { return true; };
        auto objects = std::make_shared<world::CanonicalObjectManagerV1>(manager_services);
        std::shared_ptr<character::skills::CharacterWorldRuntimeV1> actor_world(
            world_root, &world_root->actor_world);
        const std::uintptr_t actor_id = 0x101010;
        auto actor = std::make_shared<ActorFixture>(actor_id);
        actor->lease = std::make_shared<int>(4);
        target_providers::Handle16 object_handle{};
        world::CanonicalObjectBorrowV1 object{};
        object.identity = actor_id; object.lease = actor->lease; object.shared_handle = &actor->handle;
        object.type_f4 = &actor->type; object.across_rooms87 = &actor->across;
        object.room64 = &actor->room; object.context = actor.get();
        object.set_name = ActorFixture::set_name; object.set_archetype = ActorFixture::set_archetype;
        object.as_character = ActorFixture::as_character;
        if (!objects->add(object, "SourceTestActor", "Character", 0, true, object_handle, error))
            throw std::runtime_error("ObjectManager fixture add: " + error);
        character::skills::WorldActorRegistrationV1 registration{};
        registration.identity = actor_id; registration.handle_key = object_handle.key;
        registration.context = actor.get(); registration.refresh = ActorFixture::refresh;
        registration.shared_handle = &actor->handle;
        check(actor_world->add(registration) == 0);

        auto globals = std::make_shared<NativeGSLevelGlobalsV27>();
        auto runtime = std::make_shared<NativeGSLevelRuntimeV27>(globals);
        SourceCurrentLevelGraphV1 graph;
        graph.root_scope = actual; graph.application = actual; graph.world = world_owner;
        graph.objects = objects; graph.navigation = navigation; graph.floors = floors;
        graph.gs_globals = globals; graph.gs_runtime = runtime;
        graph.actor_world = actor_world; graph.level_tables = actual->levels();
        SourceCurrentLevelBackendV1 backend(std::move(graph));

        // Select a definition from the captured source LevelTable itself. C1
        // performs its own original first-substring lookup over this name.
        check(!actual->levels()->levels.empty());
        const auto& source_row = actual->levels()->levels.front();
        std::string source_name = source_row.file;
        for (char& ch : source_name) ch = char(std::tolower(static_cast<unsigned char>(ch)));
        LevelSourceRequestV1 request; request.identity = source_name;
        request.definition = source_row.file; request.seed = 7;
        GSLevelArgumentsV2 arguments{source_name, 0, 7, 1, 0, 1, 0, -1, 0};
        GSLevelServicesV2<CanonicalLevelContextV1> gs;
        gs.flush_animation_sets = [](std::string&) { return true; };
        gs.get_menu = [](const char*, std::uintptr_t& menu, std::string&) { menu = 0; return true; };
        gs.online_byte5 = application.online_byte5;
        gs.online_state34 = [](std::int32_t&, std::string& e) { e = "unreached offline state"; return false; };
        gs.unload_level = [](const auto&, std::string&) { return true; };
        gs.destroy_level = [](const auto&, std::string&) { return true; };
        check(backend.construct(std::move(request), std::move(arguments), application,
                               std::move(gs), [](std::uint32_t& online, std::string&) {
                                   online = 0; return true;
                               }, error));

        SourceCurrentLevelBorrowV1 pinned;
        check(backend.current(pinned, error));
        check(pinned && pinned.level().get() == globals->s_level.get());
        check(pinned.level() == runtime->fields().level34);
        check(pinned.kill_level() == pinned.level()->kill_level());
        check(pinned.kill_level()->loot_gate150 == 0);
        check(pinned.level()->source_word150() == 0);
        check(pinned.difficulty118() == &pinned.level()->constructor_fields_v3().mode118);
        check(pinned.level()->constructor_fields_v3().row3c == 0);
        check(backend.still_current(pinned, error));
        CanonicalCurrentLevelBorrowV1 native_borrow;
        check(runtime->current(native_borrow, error) && native_borrow.level() == pinned.level());
        check(backend.validate_actor_manager_identity(actor_id, error));
        actor->handle.cached = actor_id + 1;
        check(!backend.validate_actor_manager_identity(actor_id, error));
        actor->handle.cached = actor_id;

        std::weak_ptr<CanonicalLevelContextV1> weak = pinned.level();
        check(backend.destroy(error));
        check(!globals->s_level && !runtime->fields().level34);
        check(!backend.still_current(pinned, error));
        check(pinned && pinned.kill_level() == pinned.level()->kill_level());
        check(!weak.expired()); // a stale typed loan pins fields but cannot claim current status
        std::cout << "PASS native GS/Level C1, exact current-slot and word+150 identity, same actor World/ObjectManager handle, unload and stale-lease guard; checks="
                  << checks << " | source=" << source_row.file << '\n';
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
