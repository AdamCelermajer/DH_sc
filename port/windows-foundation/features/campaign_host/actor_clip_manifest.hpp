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

// B080/B154: actors that scripts bring into play with Script_SpawnCharacter (kind 30, name @12). A script-spawned
// actor is a gameplay actor (combat policy; the AI takes over once a script puts it in Idle) unless a script retires
// it with Script_PutCharacterInLimbus (kind 29, name @16) and no script names it in a gameplay lifecycle step: a
// by-name Script_PutCharacterInIdle (kind 31, @12) or Script_KillActor (kind 44, @12). A spawned-then-retired actor
// with no such step is a cutscene prop (the opening troll, spawned and retired by Swamp_Intro; the Darkwood lost
// merchant). The Bogwitch is spawned by Intro_Witch and retired only by the skip-recovery zone script
// SkipCutscene_Bugfix, but WitchTeleport puts her in Idle by name and Return_Cinematic kills her by name, so she stays
// a gameplay actor. Names come from the authored scripts only.
std::set<std::string> collect_script_gameplay_actors(const OriginalCampaignRuntime& runtime);
// B155: actors that a script puts in Idle (kind 31) or kills (kind 44) by name, whether or not a script spawns them
// ("All"/player names excluded). The host treats such a placed Monster declaration (auto_spawn=0, AI Type 4) that a
// script also animates as a gameplay actor, not a cutscene prop: the Swamp King is placed in the boss room and
// Swamp_Serpent_Intro ends with his PutCharacterInIdle (likewise the Darkwood Dark Queen, HG2_boss_intro). His intro
// lizards are only shown, animated and put in Limbus, so they stay props; companions/NPCs are not Monster rows.
std::set<std::string> collect_script_idled_actors(const OriginalCampaignRuntime& runtime);

// The visual clip name of a dictionary clip (the same name everywhere: profile bank, player bank, the session).
std::string actor_clip_name(std::int32_t dictionary_id);

} // namespace dh::foundation::campaign_host
