#pragma once

#include "asset_catalog.hpp"
#include "renderer.hpp"
#include <map>
#include <string>
#include <vector>

namespace dh::foundation {

struct ActorDefinition {
    std::uint64_t stableId = 0;
    // Canonical source identity includes module instance, file and XML ordinal.
    std::string sourceId, sourcePath, moduleName;
    std::string name, gametype, role, templateReference;
    std::string modelPath, animationConfig;
    Mat4 placement{};
    std::map<std::string, std::string> properties;
    std::vector<std::string> references, unresolvedReferences;
};

struct ActorLoadOptions {
    // Optional XML template/property tables. Entries indexed by authored name/id.
    std::vector<std::filesystem::path> referenceDocuments;
    bool requireReferencedFiles = false;
};

// Loads level GameObjects and each placed Module's MGP/MVP declarations.
// Preserves every authored attribute, including conditions and data identifiers;
// no NPC/enemy names, gameplay conditions, probabilities or AI are interpreted.
// Module source offsets follow original XML loading: add module position to
// child positions without rotating child declarations. Templates inherit defaults
// from supplied reference documents, and authored instance attributes win.
// Missing optional refs are retained in unresolvedReferences; malformed loaded
// XML, duplicate identity and reference cycles fail without changing output.
bool load_actor_definitions(const AssetCatalog& assets,
                            const std::filesystem::path& levelPath,
                            std::vector<ActorDefinition>& output,
                            std::string& error,
                            const ActorLoadOptions& options = {});

// Child scope key of each Module declaration (gametype "Module") in staged order: "<declaration moduleName>/<name>#<ordinal>",
// the moduleName load_actor_definitions gives that Module's own child GameObjects. A Module whose mgp/mvp declare no GameObject
// (an empty <Module/>, e.g. Darkwood_08_01) has no child carrying the key; the key is still its host occurrence.
std::vector<std::pair<const ActorDefinition*, std::string>> module_child_occurrences(const std::vector<ActorDefinition>& definitions);

} // namespace dh::foundation
