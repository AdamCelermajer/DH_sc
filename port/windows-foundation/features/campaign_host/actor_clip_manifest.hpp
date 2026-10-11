#pragma once
// P16 OPENING: the scripted actor clips (PlayActorAnim, kind 45) that actor visuals must hold before they load.
//
// The source binds a dictionary clip to a character's animation rows at script time (Script_PlayActorAnim::Init adds
// the clips to the character's CharAnimator set). Here the same clips are named visual clips ("cs:<dictionary id>") in
// the actor's profile (or the local player's visual config) before the population loads, so CharacterVisual::select can
// play them. Nothing is keyed by level or actor name: every name comes from the authored scripts and declarations.
#include "../../original_campaign_runtime.hpp"
#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::campaign_host {

struct ActorClipRequest {
    std::string actor;             // script actor name (string 24 of PlayActorAnim)
    std::int32_t dictionary_id=-1; // animations_dictionary id (scalar 8, chained scalar 12)
};

// Every PlayActorAnim clip of every loaded script. Chained clips (scalar 12, not -1) are included.
std::vector<ActorClipRequest> collect_actor_clip_requests(const OriginalCampaignRuntime& runtime);

// P16 OPENING4: Script_PlayAnimByName (kind 19) requests: scene object @24 plays clip @12 (every loaded script).
struct SceneObjectClipRequest {
    std::string object;
    std::string clip;
};
std::vector<SceneObjectClipRequest> collect_scene_object_clip_requests(const OriginalCampaignRuntime& runtime);

// B080: actors that scripts bring into play with Script_SpawnCharacter (kind 30, name @12) and never retire with
// Script_PutCharacterInLimbus (kind 29, name @16). A script-spawned actor that no script removes is a gameplay actor
// (it ends in the Idle state the script puts it in and the monster AI takes over); one that is retired again is a
// cutscene prop. Names come from the authored scripts only.
std::set<std::string> collect_script_gameplay_actors(const OriginalCampaignRuntime& runtime);

// The visual clip name of a dictionary clip (the same name everywhere: profile bank, player bank, the session).
std::string actor_clip_name(std::int32_t dictionary_id);

} // namespace dh::foundation::campaign_host
