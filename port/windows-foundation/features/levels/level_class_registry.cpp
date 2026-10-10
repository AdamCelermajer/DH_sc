#include "level_class_registry.hpp"

#include <iostream>
#include <map>
#include <mutex>
#include <set>

namespace dh::foundation::levels {

const std::vector<ClassInfo>& class_registry() {
    static const std::vector<ClassInfo> table{
        {"Character", ClassStatus::consumed, "actor_population.cpp (visual + population)"},
        {"Module", ClassStatus::consumed, "level_manifest.cpp, assembled_level.cpp, actor_definitions.cpp"},
        {"LevelConfig", ClassStatus::consumed, "original_camera_config.cpp (camera), features/audio/level_music_v1.cpp"},
        {"LightPoint", ClassStatus::consumed, "actor_lighting.cpp"},
        {"SpawnPoint", ClassStatus::library_only, "actor_population.cpp (reports, no spawn policy)"},
        {"DestructibleContainer", ClassStatus::library_only, "features/interactions/session_admitted_destructible_v1.cpp"},
        {"OpenableContainer", ClassStatus::library_only, "features/interactions/session_container_admitted_openable_v1.cpp"},
        {"TriggerZone", ClassStatus::library_only, "features/encounters/trigger_constructors.cpp"},
        {"Dummy", ClassStatus::library_only, "source_world_objects.cpp"},
        {"AnimatedDecor", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"Decor", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"Door", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"TriggerObject", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"TriggerTrap", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"TriggerZoneExitLevel", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"CheckpointZone", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"QuestMoveInZone", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"SoundEmitter", ClassStatus::ported_elsewhere, "port/level-loader/catalog_auxiliary_v67.cpp, canonical_auxiliary_families_v16.cpp"},
        {"link", ClassStatus::unsupported, "- (module link declarations, not a class; used by the procedural generator)"},
    };
    return table;
}

ClassStatus class_status(const std::string& gametype) {
    for (const auto& row : class_registry())
        if (gametype == row.gametype) return row.status;
    return ClassStatus::unsupported; // not in the table: unknown to this build
}

const char* class_status_name(ClassStatus status) {
    switch (status) {
    case ClassStatus::consumed: return "consumed";
    case ClassStatus::library_only: return "library_only";
    case ClassStatus::ported_elsewhere: return "ported_elsewhere";
    case ClassStatus::unsupported: return "unsupported";
    }
    return "unsupported";
}

std::size_t log_unsupported_classes_once(const std::vector<ActorDefinition>& definitions) {
    static std::mutex guard;
    static std::set<std::string> logged;
    std::map<std::string, std::size_t> counts;
    for (const auto& definition : definitions) {
        const std::string gametype = definition.gametype.empty() ? std::string("(none)") : definition.gametype;
        if (class_status(gametype) != ClassStatus::consumed) counts[gametype]++;
    }
    std::lock_guard<std::mutex> lock(guard);
    std::size_t fresh = 0;
    for (const auto& entry : counts) {
        if (!logged.insert(entry.first).second) continue;
        ++fresh;
        std::cerr << "Unsupported class " << entry.first << " status=" << class_status_name(class_status(entry.first))
                  << " declarations=" << entry.second << " (logged once)\n";
    }
    return fresh;
}

} // namespace dh::foundation::levels
