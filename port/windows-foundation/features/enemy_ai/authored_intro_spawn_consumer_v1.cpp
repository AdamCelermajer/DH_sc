#include "authored_intro_spawn_consumer_v1.hpp"

#include <array>
#include <map>

namespace dh::foundation::enemy_ai {
namespace {
constexpr std::array<const char*, 2> kIntroNames{
    "_prim_Monster_LizManIntro1", "_prim_Monster_LizManIntro2"};
constexpr const char* kSourcePath =
    "data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp";

bool property_is(const ActorDefinition& definition, const char* name,
                 const char* value) {
    const auto found = definition.properties.find(name);
    return found != definition.properties.end() && found->second == value;
}
}

bool bind_authored_intro_spawn_consumer_v1(
    const std::vector<ActorDefinition>& definitions,
    const SourceWorldObjects& source_objects,
    OriginalActorLifecycle& lifecycle,
    std::function<ActorState*(ActorId)> session_actor,
    OriginalCampaignWorldProviders& providers,
    AuthoredIntroSpawnBindingV1& binding,
    std::string& error) {
    if (!session_actor) {
        error = "Authored intro spawn requires the caller's actor lookup";
        return false;
    }

    std::map<std::string, const ActorDefinition*> matched;
    for (const auto& definition : definitions) {
        if (definition.name != kIntroNames[0] && definition.name != kIntroNames[1]) continue;
        if (!matched.emplace(definition.name, &definition).second) {
            error = "Authored intro spawn declaration name is ambiguous";
            return false;
        }
    }
    if (matched.size() != kIntroNames.size()) {
        error = "Both authored 001_swamp intro Characters are required";
        return false;
    }

    AuthoredIntroSpawnBindingV1 staged;
    for (std::size_t i = 0; i < kIntroNames.size(); ++i) {
        const auto found = matched.find(kIntroNames[i]);
        const auto& definition = *found->second;
        const std::string prefixed_source = std::string("original-cache/") + kSourcePath;
        const bool expected_source = definition.sourcePath == kSourcePath ||
                                     definition.sourcePath == prefixed_source;
        if (definition.stableId == invalid_actor_id || definition.gametype != "Character" ||
            !expected_source || definition.moduleName.empty() ||
            !property_is(definition, "ai_state", "Limbus") ||
            !property_is(definition, "auto_spawn", "0") ||
            !property_is(definition, "ai_state_visible", "0") ||
            !property_is(definition, "charpropsname", "Swamp_LizadMan_Type1")) {
            error = "Authored intro Character facts do not match the recovered source declaration";
            return false;
        }
        if (i == 0) staged.module_occurrence = definition.moduleName;
        else if (staged.module_occurrence != definition.moduleName) {
            error = "Authored intro Characters must belong to the same Module occurrence";
            return false;
        }
        const auto* retained = source_objects.definition(definition.stableId);
        if (!retained || retained->sourceId != definition.sourceId ||
            retained->name != definition.name || retained->moduleName != definition.moduleName) {
            error = "Authored intro declaration is not the retained SourceWorldObjects identity";
            return false;
        }
        auto* actor = session_actor(definition.stableId);
        const auto* state = lifecycle.status(definition.stableId);
        if (!actor || actor->id != definition.stableId || !state || state->failed ||
            state->state != static_cast<int>(OriginalLifecycleState::pre_spawn)) {
            error = "Authored intro Character must have a matching actor lookup and PreSpawn lifecycle record";
            return false;
        }
        if (i == 0) staged.first = definition.stableId;
        else staged.second = definition.stableId;
    }

    const auto existing = providers.named_character;
    providers.named_character = [&source_objects, &lifecycle,
                                 session_actor = std::move(session_actor),
                                 expected = staged, existing](
        const std::string& name, int module, ActorId& id, bool& found,
        std::string& callback_error) {
        if (name != kIntroNames[0] && name != kIntroNames[1]) {
            if (existing) return existing(name, module, id, found, callback_error);
            callback_error = "Authored intro spawn provider does not own this character name";
            return false;
        }
        const ActorId required = name == kIntroNames[0] ? expected.first : expected.second;
        ActorId resolved = invalid_actor_id;
        bool resolved_found = false;
        if (!source_objects.named_character(name, module, resolved, resolved_found,
                                            callback_error)) return false;
        if (!resolved_found || resolved != required) {
            callback_error = "Source module lookup did not resolve the exact authored intro Character";
            return false;
        }
        auto* actor = session_actor(resolved);
        const auto* state = lifecycle.status(resolved);
        if (!actor || actor->id != resolved || !state || state->failed ||
            state->state != static_cast<int>(OriginalLifecycleState::pre_spawn)) {
            callback_error = "Authored intro spawn lost its matching PreSpawn actor/lifecycle facts";
            return false;
        }
        id = resolved;
        found = true;
        callback_error.clear();
        return true;
    };
    binding = std::move(staged);
    error.clear();
    return true;
}

} // namespace dh::foundation::enemy_ai
