#pragma once
#include "playable_actor_world.hpp"
#include <filesystem>
namespace dh::foundation {
struct GameSave {
    std::uint32_t version = 1;
    CharacterState character;
    std::string level_uri;
    ActorId controlled_actor_id = invalid_actor_id;
    dh2::data::CombatRandom random;
    std::vector<PersistedPlayableActor> actors;
    // Version2 appends neutral authored objects and feature-owned component
    // bytes. Actor-only version1 remains readable/writable without conversion.
    std::vector<WorldObject> objects;
    // Version3 appends actors' optional actual physical-presence witnesses.
    // Legacy versions omit them and retain unknown; no receiver is serialized.
};
bool validate_game_save(const GameSave&,std::string& error);
bool capture_game_save(const std::string& level_uri,ActorId controlled,
                       const CharacterState&,const PlayableActorWorld&,GameSave&,std::string& error);
bool save_game(const std::filesystem::path&,const GameSave&,std::string& error);
bool load_game(const std::filesystem::path&,GameSave&,std::string& error);
// Version1 checkpoint policy requires matching level, ID/definition roster,
// authorized action groups, original profile/class/faction/role and equipped
// source definitions/records/gear sheet. Changed loadouts must be reconstructed
// by a future host-aware restore before this policy can be relaxed. Failure
// preserves world, RNG and character. Success invalidates actor/object borrows.
// Version2 also requires matching neutral object IDs/names/model/material and
// restores their component bytes; owning features must validate their codecs.
// Old version1 checkpoints leave the currently loaded neutral state intact.
// Version3 additionally restores actual physical presence values. The host
// assembles its existing pool from these facts before Step/update/render.
bool restore_game_save(const GameSave&,const std::string& expected_level_uri,
                       PlayableActorWorld&,CharacterState&,std::string& error);
} // namespace dh::foundation
