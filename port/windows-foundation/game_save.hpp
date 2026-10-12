#pragma once
#include "playable_actor_world.hpp"
#include <filesystem>
namespace dh::foundation {
struct GameSave {
    std::uint32_t version = 6;
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
    // Version4 appends each actor's explicit source-dead bool in actor order.
    // Version5 carries Character schema5's per-item source metadata.
    // Version6 carries the source-specific Limbus restore fact separately from +5193 and HP.
    // Earlier PC checkpoints use their old HP classification as a migration.
};
bool validate_game_save(const GameSave&,std::string& error,bool checkpointUnboundVitals=false);
bool capture_game_save(const std::string& level_uri,ActorId controlled,
                       const CharacterState&,const PlayableActorWorld&,GameSave&,std::string& error,bool checkpointCapture=false);
bool save_game(const std::filesystem::path&,const GameSave&,std::string& error,bool checkpointWrite=false);
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
// B143: a per-level world snapshot (LevelSavegame) restores the WORLD of its level. The original LevelSavegame::Load
// deserializes the level's objects and the local player's position/state; the profile (inventory, skills, XP, points,
// potions, quest state) is not reloaded from it (MenuBase::FS_ContinueGame + LevelSavegame::Load, see B086). The GameSave
// of this port embeds a full Character, which is only as new as the snapshot and so can predate the profile. This makes
// `snapshot` carry the live session instead: the Character of `live` (a capture of the session built from the profile)
// and the live record of the controlled actor, whose saved transform is kept when `keep_saved_hero_transform`.
// Every other actor, object and the RNG stay as snapshotted. Refuses (snapshot untouched) when the controlled actor of the
// two saves differs by id or definition.
bool adopt_live_character_for_world_restore(GameSave& snapshot,const GameSave& live,bool keep_saved_hero_transform,
                                            std::string& error);
} // namespace dh::foundation
