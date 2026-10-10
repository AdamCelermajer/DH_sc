#include "container_declarations_v1.hpp"
#include "../../content_paths.hpp"
#include "../../../level-world/openable_container_owner_v1.hpp"
#include "../../../level-world/destructible_container_data_v16.hpp"

#include <filesystem>

namespace dh::foundation::containers {

namespace {
std::string property(const ActorDefinition& definition, const char* key) {
    const auto it = definition.properties.find(key);
    return it == definition.properties.end() ? std::string() : it->second;
}
} // namespace

bool ContainerTablesV1::load(const AssetCatalog& assets, std::string& error) {
    ready_ = false;
    borrow_ = {};
    try {
        const auto records = read_content(assets, "data/game_objects_pyarray.bin");
        const auto names = read_content(assets, "data/game_objects_pyarraynames.bin");
        const auto dictionary = read_content(assets, "data/game_objects_dictionary_pyarray.bin");
        const auto dictionaryNames = read_content(assets, "data/game_objects_dictionary_pyarraynames.bin");
        dh2::loader::GameObjectArraysOwnerV81 owner;
        if (!owner.initialize(records.data(), records.size(), names.data(), names.size(),
                              dictionary.data(), dictionary.size(), dictionaryNames.data(),
                              dictionaryNames.size(), error))
            return false;
        if (!owner.borrow(borrow_, error)) return false;
    } catch (const std::exception& exception) {
        error = std::string("Original game_objects tables unavailable: ") + exception.what();
        return false;
    }
    ready_ = true;
    error.clear();
    return true;
}

bool load_container_instances_v1(const ContainerTablesV1& tables,
                                 const ContainerClassRegistryV1& registry,
                                 const std::vector<ActorDefinition>& definitions,
                                 std::vector<ContainerInstanceV1>& out,
                                 ContainerLoadReportV1& report,
                                 std::string& error) {
    out.clear();
    report = {};
    error.clear();
    if (!tables.ready()) {
        error = "Container tables are not loaded";
        return false;
    }
    const auto& borrow = tables.borrow();
    for (const auto& definition : definitions) {
        if (ContainerClassRegistryV1::handled_elsewhere(definition.gametype)) continue;
        const auto* entry = registry.find(definition.gametype);
        if (!entry) {
            ++report.unsupported[definition.gametype];
            continue;
        }
        ++report.declarations;
        ContainerInstanceV1 instance;
        instance.stableId = definition.stableId;
        instance.sourceId = definition.sourceId;
        instance.name = definition.name;
        instance.gametype = definition.gametype;
        instance.data_desc = property(definition, "data_desc");
        instance.family = entry->family;
        instance.interaction_type = entry->interaction_type;
        instance.transform = definition.placement;
        const auto fail = [&](const char* reason) {
            report.notices.push_back(definition.gametype + " " + definition.name + " data_desc=" +
                                     instance.data_desc + ": " + reason);
        };
        if (entry->family == ContainerFamilyV1::openable) {
            if (!borrow.openable) { fail("OpenableContainers table missing"); continue; }
            std::int32_t id = -1;
            dh2::world::OpenableContainerRowV1 row;
            std::string resolveError;
            if (!borrow.openable->resolve(instance.data_desc, id, row, resolveError) || id < 0) {
                fail("row not in OpenableContainers");
                continue;
            }
            instance.row = id;
            instance.sound_id = row.sound;
            instance.loot_id = row.loot;
            instance.visual_id = row.visual;
        } else {
            if (!borrow.destructible) { fail("DestructibleContainers table missing"); continue; }
            const auto id = borrow.destructible->data_id(instance.data_desc);
            const auto* row = borrow.destructible->row(id);
            if (!row) {
                fail("row not in DestructibleContainers");
                continue;
            }
            instance.row = id;
            instance.sound_id = row->sound();
            instance.loot_id = row->loot();
            instance.visual_id = row->visual();
        }
        const std::string* file = nullptr;
        std::string dictionaryError;
        if (!borrow.dictionary || !borrow.dictionary->file(instance.visual_id, file, dictionaryError) || !file) {
            fail("visual id not in GameObjectDict");
        } else {
            instance.visual_file = *file;
        }
        out.push_back(std::move(instance));
        ++report.instantiated;
    }
    return true;
}

bool ContainerVisualsV1::load(const AssetCatalog& assets, std::vector<ContainerInstanceV1>& instances,
                              std::vector<std::string>& notices) {
    std::map<std::string, bool> decoded;
    for (auto& instance : instances) {
        instance.visual_ready = false;
        if (instance.visual_file.empty()) continue;
        auto it = decoded.find(instance.visual_file);
        if (it == decoded.end()) {
            auto visual = std::make_unique<CharacterVisual>();
            std::string loadError;
            bool ok = false;
            try {
                ok = visual->load_embedded_scene(assets, instance.visual_file, loadError) &&
                     !visual->meshes().empty();
                // Closed pose: the source idle clip when the BDAE authors one.
                if (ok && !visual->select("idle", true, loadError)) loadError.clear();
            } catch (const std::exception& exception) {
                loadError = exception.what();
                ok = false;
            }
            if (!ok) notices.push_back("container visual unavailable " + instance.visual_file + ": " + loadError);
            if (ok) visuals_[instance.visual_file] = std::move(visual);
            it = decoded.emplace(instance.visual_file, ok).first;
        }
        instance.visual_ready = it->second;
    }
    return true;
}

} // namespace dh::foundation::containers
