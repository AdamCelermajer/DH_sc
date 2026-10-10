#include "source_create_npc.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include "../../../level-world/canonical_class_receiver_bindings_v1.hpp"
#include "../../../level-world/canonical_object_manager_v1.hpp"
#include "../../../level-world/canonical_property_map_v1.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>

using namespace dh2;

namespace {
std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Required original data file: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}

struct FactoryContext {
    std::shared_ptr<world::CanonicalCharacterCandidateFactoryV60> factory;
    std::map<std::uintptr_t, world::CanonicalClassReceiverV1> observed_receivers;
};

bool construct_character(void* raw, const world::CanonicalSourceObjectRequestV1& request,
    world::CanonicalClassReceiverV1& receiver, std::string& error) {
    auto& context = *static_cast<FactoryContext*>(raw);
    const world::CanonicalFactoryEntryV1 entry{"Character", 0x340800};
    if (!context.factory->construct(entry, request, receiver, error)) return false;
    context.observed_receivers[receiver.object.identity] = receiver;
    return true;
}

struct SpawnContext {
    world::CanonicalClassReceiverBindingsV1* receivers{};
    std::shared_ptr<world::CanonicalObjectManagerV1> manager;
    FactoryContext* factory_context{};
};

bool spawn_construct(void* raw, const world::CanonicalFactoryEntryV1& entry,
    world::CanonicalClassReceiverV1& receiver, std::string& error) {
    auto& context = *static_cast<SpawnContext*>(raw);
    const auto services = context.receivers->services();
    world::CanonicalSourceObjectRequestV1 request;
    auto name_owner = std::make_shared<std::string>("0001");
    request.source_lease = name_owner;
    request.native_spawn_name_v68 = name_owner->c_str();
    world::CanonicalObjectBorrowV1 object;
    if (!services.construct || !services.construct(services.context, entry, request, object, error))
        return false;
    auto found = context.factory_context->observed_receivers.find(object.identity);
    if (found == context.factory_context->observed_receivers.end()) {
        error = "actual CandidateFactory receiver was not retained";
        return false;
    }
    receiver = found->second;
    return true;
}

bool spawn_resolve(void* raw, target_providers::Handle16& handle, bool asserted,
    const world::CanonicalObjectBorrowV1*& result, std::string& error) {
    auto& context = *static_cast<SpawnContext*>(raw);
    return context.manager->resolve_handle_v4(handle, asserted, result, {}, error);
}

bool spawn_condition(void*, const world::CanonicalObjectBorrowV1&, bool,
    std::string& error) {
    error = "deferred source spawn must not reach TestEnableCondition";
    return false;
}

bool spawn_virtual38(void* raw, const world::CanonicalObjectBorrowV1& object,
    bool& accepted, std::string& error) {
    auto& context = *static_cast<SpawnContext*>(raw);
    const auto found = context.factory_context->observed_receivers.find(object.identity);
    if (found == context.factory_context->observed_receivers.end()) {
        error = "Required retained actual CandidateFactory receiver";
        return false;
    }
    const auto& receiver = found->second;
    if (!receiver.source_is_updatable_v95) {
        error = "Required actual CandidateFactory virtual38 receiver";
        return false;
    }
    return receiver.source_is_updatable_v95(accepted, error);
}

bool spawn_append(void* raw, const world::CanonicalObjectBorrowV1& object,
    std::string& error) {
    return static_cast<SpawnContext*>(raw)->manager->append_pending(object, error);
}

bool spawn_receiver(void* raw, const world::CanonicalObjectBorrowV1& object,
    const world::CanonicalClassReceiverV1*& receiver, std::string& error) {
    auto& context = *static_cast<SpawnContext*>(raw);
    const auto found = context.factory_context->observed_receivers.find(object.identity);
    if (found == context.factory_context->observed_receivers.end()) {
        error = "Required retained actual CandidateFactory receiver";
        receiver = nullptr;
        return false;
    }
    receiver = &found->second;
    return true;
}

bool unknown_type(void*, const char* type, std::string& error) {
    error = std::string("Unexpected source factory type: ") + (type ? type : "<null>");
    return false;
}
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) return 2;
        const std::string directory = argv[1];
        std::vector<std::vector<std::uint8_t>> raw;
        auto bytes = [&](const std::string& name) {
            raw.push_back(read_file(directory + "/" + name));
            return data::Bytes{raw.back().data(), raw.back().size()};
        };

        std::string error;
        character::GameDesignInputs256 input{};
        character::GameDesignTableInput48* tables[]{
            &input.characters, &input.classes, &input.ai, &input.factions, &input.levels};
        const char* names[]{"character_properties", "character_classes", "ai", "ai_factions", "levels"};
        for (std::size_t i = 0; i < 5; ++i) {
            *tables[i] = {bytes(std::string(names[i]) + "_pyarray.bin"),
                bytes(std::string(names[i]) + "_pyarraynames.bin"),
                bytes(std::string(names[i]) + "_pystructnames.bin")};
        }
        character::CharacterGameDesign design;
        if (!design.initialize(input, error)) throw std::runtime_error(error);
        data::LootTablesV2 loot;
        if (!loot.load(bytes("loot_table_pyarray.bin"), bytes("loot_table_pyarraynames.bin"),
                bytes("loot_table_pystructnames.bin"), error)) throw std::runtime_error(error);
        data::Dictionary animations;
        if (!data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),
                bytes("animations_dictionary_pyarray.bin"), animations, error))
            throw std::runtime_error(error);

        auto world_lease = std::make_shared<std::string>(directory);
        auto scene_manager = std::make_shared<world::GameObjectSceneRootRegistryV1>();
        auto animation_manager = std::make_shared<character::CharacterAnimationSetCacheV6>();
        data::LootRandom8V2 random{};
        auto manager = std::make_shared<world::CanonicalObjectManagerV1>(world::CanonicalObjectManagerServicesV1{});
        world::CanonicalCharacterCandidateServicesV60 candidate_services;
        candidate_services.world = world_lease;
        candidate_services.design = &design;
        candidate_services.loot_tables = &loot;
        candidate_services.random = &random;
        candidate_services.canonical_objects = manager;
        candidate_services.visual = [world_lease, scene_manager, animation_manager, &animations]
            (auto&, auto& visual, std::string& e) {
                visual.world = world_lease;
                visual.scene_manager = scene_manager;
                visual.animation_manager = animation_manager;
                visual.animation_dictionary = &animations;
                e.clear();
                return true;
            };
        auto factory = std::make_shared<world::CanonicalCharacterCandidateFactoryV60>(candidate_services);
        FactoryContext factory_context{factory};

        world::CanonicalPropertySourceServicesV1 property_source{};
        property_source.position_rotation_default = &world::canonical_vec3_origin_v1();
        world::CanonicalPropertyMapV1 properties(property_source);
        world::CanonicalReceiverConstructionV1 construction{};
        construction.context = &factory_context;
        construction.character = construct_character;
        world::CanonicalClassReceiverBindingsV1 receivers(properties, construction);
        SpawnContext spawn_context{&receivers, manager, &factory_context};
        world::CanonicalSpawnServicesV1 spawn{
            &spawn_context, spawn_construct, unknown_type, spawn_resolve,
            spawn_condition, spawn_virtual38, spawn_append, spawn_receiver};
        enemy_ai::SourceCanonicalSpawnOwnerV1 owner{manager.get(), &properties, spawn};

        // This is the actual source ObjectManager::Spawn deferred prefix used
        // by _Summon: native CandidateFactory construction, receiver retention,
        // same manager Add, same PropertyMap defaults, same virtual38 and pending.
        world::CanonicalClassReceiverV1 spawned;
        bool created{};
        if (!enemy_ai::source_canonical_manager_spawn_v1(owner, "Character", "0001",
                true, false, spawned, created, error)) throw std::runtime_error(error);
        if (!created || !spawned.object.identity) throw std::runtime_error("source Spawn did not create a Character");
        auto record = factory->find(spawned.object.identity);
        if (!record || !record->actor || !record->actor->object || record->failed ||
            record->init_attempted || record->init_complete)
            throw std::runtime_error("source Spawn did not retain the same uninitialized CandidateFactory record");
        const auto* published = manager->object(spawned.object.shared_handle->key);
        if (!published || published->identity != spawned.object.identity ||
            published->lease.get() != spawned.object.lease.get() ||
            published->lease.owner_before(spawned.object.lease) ||
            spawned.object.lease.owner_before(published->lease) ||
            receivers.retained_count() != 1 ||
            record->actor->object->identity != spawned.object.identity ||
            manager->pending().size() != 1 || manager->pending().front() != spawned.object.identity)
            throw std::runtime_error("source Spawn record/receiver/manager/pending identities diverged");
        if (random.calls != 0) throw std::runtime_error("deferred constructor/publication consumed a fabricated random draw");
        std::cout << "actual CandidateFactory -> retained receiver -> same ObjectManager/Add -> PropertyMap -> virtual38 -> pending PASS\n";
        std::cout << "identity=" << spawned.object.identity << " handle-key=" << spawned.object.shared_handle->key
                  << " pending=" << manager->pending().front() << " record-init=deferred\n";
        std::cout << "No InitPost/InitFinal/SetIdleState/RoomZone/PF/current-Level or full _Summon completion claimed\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
