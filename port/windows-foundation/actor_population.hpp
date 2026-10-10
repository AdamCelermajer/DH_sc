#pragma once

#include "actor_definitions.hpp"
#include "actor_profiles.hpp"
#include "../game-data/data.hpp"
#include "../game-data/character_templates_v78.hpp"
#include <functional>

namespace dh::foundation {

enum class PopulationDecision { include, exclude, unknown, deferred };
using PopulationPolicy = std::function<PopulationDecision(const ActorDefinition&)>;
using PopulationCustomization = std::function<ActorCustomization(const ActorDefinition&, const ActorProfile&)>;

// Optional bridge for authored `char_template` references. The callback must
// draw from the caller's shared source RNG and return an index in [0, bound).
// CharacterTemplateTableV78 keeps duplicate member IDs as authored weights.
struct PopulationTemplateSelectionV1 {
    const dh2::data::CharacterTable* characters = nullptr;
    const dh2::data::CharacterTemplateTableV78* templates = nullptr;
    std::function<bool(std::int32_t bound,std::int32_t& index,std::string& error)> random_index;
};

struct PopulationActor {
    ActorDefinition definition;
    std::string profileId;
    // Selected source CharacterTable row and Charater_Templates row. These are
    // diagnostic/cache facts; definition and its stable identity remain intact.
    std::int16_t source_character_cache = -1;
    std::int16_t source_template_cache = -1;
    CharacterVisual visual;
    // Authored actor placement multiplied by the profile's documented scales.
    Mat4 transform{};
    // Explicit caller-owned activation. Deferred actors have ready visuals but
    // start disabled; neither conditions nor auto_spawn are guessed here.
    bool enabled = true;
    bool initially_enabled = true;
    PopulationDecision initial_decision = PopulationDecision::include;
};
struct PopulationNotice { std::string sourceId, reason; };

class ActorPopulation {
public:
    // Caller owns condition evaluation and equipment/skeleton policy. Unknown
    // decisions, missing profiles and unsupported visuals remain explicit gaps.
    // Explicit charpropsname bindings bypass template selection. Symbolic
    // char_template rows can resolve through the optional actual source tables
    // and caller-owned shared RNG; spawn probabilities are never sampled here.
    // Loaded clips preserve all profile variants; the host selects initial pose.
    bool load(const AssetCatalog& assets, const std::string& levelURI,
              const ActorProfileLibrary& profiles, const PopulationPolicy& policy,
              const PopulationCustomization& customization, std::string& error,
              const PopulationTemplateSelectionV1* template_selection = nullptr);
    std::vector<PopulationActor>& actors() noexcept { return actors_; }
    const std::vector<PopulationActor>& actors() const noexcept { return actors_; }
    const std::vector<PopulationNotice>& notices() const noexcept { return notices_; }
    // Complete authored definitions, including exclusions and unknowns, remain
    // available to source scripts/spawn policy without reconstructing XML IDs.
    const std::vector<ActorDefinition>& definitions() const noexcept { return definitions_; }
    bool set_enabled(std::uint64_t stableId, bool enabled, std::string& error);
    std::size_t enabled_count() const noexcept;
    std::size_t initial_deferred_count() const noexcept;
    std::size_t authored_count() const noexcept { return authored_count_; }
    std::size_t excluded_count() const noexcept { return excluded_count_; }
    // Includes both explicit policy exclusions and unresolved/unavailable actors.
    std::size_t skipped_count() const noexcept { return authored_count_ - actors_.size(); }

private:
    std::vector<PopulationActor> actors_;
    std::vector<PopulationNotice> notices_;
    std::vector<ActorDefinition> definitions_;
    std::size_t authored_count_ = 0, excluded_count_ = 0;
};

} // namespace dh::foundation
