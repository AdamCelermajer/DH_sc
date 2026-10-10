#pragma once
#include "original_character.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation {
class AssetCatalog;
struct ActorClip { std::string uri; double weight = 1.0; bool weight_authored = false; };
struct ActorRawProperty { std::int64_t raw = 0; std::string encoding; };
struct ActorProfile {
    std::string id, character_uri, model_uri, template_clip_uri;
    std::string property_row, animation_table, animation_table_name;
    std::map<std::string, std::vector<ActorClip>> states;
    std::map<std::string, ActorRawProperty> raw_properties;
};
class ActorProfileLibrary {
public:
    bool load(const AssetCatalog&, const std::string& uri, std::string& error);
    const ActorProfile* find(const std::string& id) const noexcept;
    // P16 PROFILES: publishes a profile derived from the original tables; rejects an existing ID.
    bool add_derived(ActorProfile profile, std::string& error);
    const std::map<std::string, ActorProfile>& profiles() const noexcept { return profiles_; }
private:
    std::map<std::string, ActorProfile> profiles_;
};
// The caller must supply the equipment selector and missing-target policy.
// No class, actor, equipment, or optimized-skeleton policy is inferred.
struct ActorCustomization {
    std::string skin_id_contains;
    unsigned expected_controller_count = 0;
    bool include_static_instances = false;
    bool allow_missing_animation_targets = false;
    std::vector<std::string> controller_ids;
    bool use_authored_modular_defaults=false;
};
// All state variants remain in the profile. The visual bank names first variants
// by state and subsequent variants as state#2, state#3, ...; AI chooses weights.
bool make_visual_config(const AssetCatalog&, const ActorProfile&,
                        const ActorCustomization&, CharacterVisualConfig&,
                        std::string& error);
} // namespace dh::foundation
