#pragma once

#include "asset_catalog.hpp"
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation {

using OriginalBindingProperties = std::map<std::string, std::string>;
// Retains extensions, symbolic references and marker annotations without assigning
// gameplay meanings to unknown fields. Attribute spelling and text are preserved.
struct OriginalBindingNode {
    std::string tag, text;
    OriginalBindingProperties properties;
    std::vector<OriginalBindingNode> children;
};
struct OriginalMeleeStep {
    std::int64_t index = 0, animationId = -1, redirect = 0;
    std::int64_t blendOut = 0, moveGO = 0;
    double speed = 0;
    std::string uri;
    OriginalBindingProperties properties;
    std::vector<OriginalMeleeStep> children;
    std::vector<OriginalBindingNode> extraNodes;
};
struct OriginalMeleeSequence {
    std::int64_t id = -1, loop = 0, type = 0;
    std::string name;
    OriginalBindingProperties properties;
    // Ordered combo phases remain grouped under each redirect step.
    std::vector<OriginalMeleeStep> steps;
    std::vector<OriginalBindingNode> extraNodes;
};
struct OriginalMeleeActor {
    std::string id, aiName, model, templateClip;
    std::int64_t propertyRow = -1, factionId = -1, aiId = -1;
    OriginalBindingProperties properties, aiProperties;
    // Sequence repetitions are retained; explicit variant selection belongs to
    // the host. Empty states remain distinguishable from absent states.
    std::map<std::string, std::vector<OriginalMeleeSequence>> states;
    std::vector<OriginalBindingNode> extraNodes;
};
struct OriginalFaction {
    std::int64_t id = -1;
    std::string name;
    OriginalBindingProperties properties;
    std::map<std::int64_t, std::int64_t> relations;
};
struct OriginalMeleeMarker {
    std::string name;
    std::int64_t timeMs = 0, authoredTimeMs = 0;
    OriginalBindingProperties properties;
};
struct OriginalMeleeClip {
    std::string uri;
    std::int64_t startMs = 0, endMs = 0;
    OriginalBindingProperties properties;
    // Repeated same-name events remain separate ordered occurrences.
    std::vector<OriginalMeleeMarker> markers;
    std::vector<OriginalBindingNode> extraNodes;
};

class OriginalMeleeBindings {
public:
    bool load(const AssetCatalog&, const std::string& uri, std::string& error);
    // Validation and publication are transactional. No referenced clips/models
    // are loaded or substituted while parsing the original metadata.
    bool decode(const std::vector<std::uint8_t>& bytes, std::string& error);
    const OriginalMeleeActor* find_actor(const std::string& profileId) const noexcept;
    const OriginalMeleeClip* find_clip(const std::string& uri) const;
    std::vector<const OriginalMeleeActor*> actors_for_row(std::int64_t row) const;
    const OriginalMeleeSequence* sequence(const std::string& profileId,
                                         const std::string& state,
                                         std::size_t explicitVariant) const noexcept;
    // Directed raw signed value. Missing differs from an explicit neutral zero;
    // this method never guesses a faction or applies the authored fallback.
    std::optional<std::int64_t> relationship(std::int64_t sourceFaction,
                                            std::int64_t targetFaction) const noexcept;
    std::optional<std::int64_t> fallback_faction_id() const noexcept { return fallback_; }
    const std::map<std::string, OriginalMeleeActor>& actors() const noexcept { return actors_; }
    const std::map<std::int64_t, OriginalFaction>& factions() const noexcept { return factions_; }
    const std::map<std::string, OriginalMeleeClip>& clips() const noexcept { return clips_; }
    const OriginalBindingProperties& properties() const noexcept { return properties_; }
    const std::vector<OriginalBindingNode>& extra_nodes() const noexcept { return extraNodes_; }

private:
    std::map<std::string, OriginalMeleeActor> actors_;
    std::map<std::int64_t, OriginalFaction> factions_;
    std::map<std::string, OriginalMeleeClip> clips_;
    OriginalBindingProperties properties_;
    std::optional<std::int64_t> fallback_;
    std::vector<OriginalBindingNode> extraNodes_;
};

} // namespace dh::foundation
