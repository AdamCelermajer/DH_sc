#pragma once

#include "level_quick_save_v1.hpp"
#include "level_savegame_owner_v1.hpp"
#include "level_savegame_save_v1.hpp"
#include "character_oid_cache_v1.hpp"
#include "navigation_motion.hpp"
#include "object_manager_runtime_owner_v1.hpp"
#include "game_event_manager_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::source_level_owner_v1 {

// Owns the reconstructed scene lifecycle and borrows the renderer's one world
// as a projection. Source GSLevel/Level/LevelSavegame identities are optional
// until a real constructor/provider is selected; they are never inferred from
// the projection pointer or a matching level name.
enum class Phase : std::uint32_t {
    empty, constructed, load_requested, loading, active, suspended, tearing_down
};

struct Snapshot {
    Phase phase{Phase::empty};
    const void* renderer_projection{};
    std::uintptr_t source_gslevel{};
    std::uintptr_t source_level{};
    std::uintptr_t source_level_savegame{}; // Native LevelSavegame facade identity.
    std::uintptr_t canonical_player_savegame{};
    std::uint32_t generation{};
    std::int32_t source_level_state{};
    // Level +0x144 is initialized to zero by the source Level constructor and
    // becomes one when Level::Update reaches its loaded-state write. This is
    // retained semantic state, not a guess derived from `active`.
    std::uint8_t transition_flag_144{};
    // Level transition fields proven from the native constructor and the
    // Application/Level lifecycle. These remain semantic bytes on the same
    // retained Level identity, not a fabricated source memory image.
    std::uint8_t application_flag_145{};
    std::uint8_t loading_callback_guard_198{};
    std::uint8_t unload_force_f0{};
    // Level::LoadCheckpoint writes +0xf4 on the same retained source Level
    // around LoadCheckPoint, GameEventManager::ReInit, and Player SG restore.
    std::uint8_t checkpoint_load_in_progress{};
    // DWORD indexes 74/75 are Level+0x128/+0x12c cleanup pointers. The
    // reconstructed Level constructor initializes both to null; non-null
    // owners need an explicit provider before a queued unload can proceed.
    bool cleanup_index_74_known{};
    bool cleanup_index_75_known{};
    bool has_cleanup_index_74{};
    bool has_cleanup_index_75{};
    std::uintptr_t cleanup_index_74_identity{};
    std::uintptr_t cleanup_index_75_identity{};
    // Level constructors initialize +0x148 to -1. This remains explicit and
    // source-owned; an unconstructed/unknown Level never borrows the sentinel.
    std::int32_t pending_script_id{};
    bool pending_script_id_known{};
    std::int32_t source_save_slot{-1};
    const void* level_fields{};
    const void* quest_owner{};
    std::uintptr_t player_character{};
    std::int32_t level_row{-1};
    std::int32_t entry_point{-1};
    std::int32_t difficulty{-1};
    std::uint32_t floor_query_generation{};
    bool floor_query_bound{};
    bool source_level_caches_cleared{};
    bool object_manager_flushed{};
    std::uint64_t object_manager_flush_count{};
    std::uintptr_t object_manager_identity{};
};

using FloorHeightQuery = int (*)(navigation::HeightHit*,
    const navigation::CollisionWorld*, const float*, std::uint32_t include_special);

struct NativeLevelBinding {
    const void* level_fields{}; // Existing LevelList constructor-field owner.
    const void* quest_owner{};  // Existing same-Save QEST owner.
    std::uintptr_t player_savegame{}; // Canonical PlayerSavegameV1 identity.
    std::uintptr_t player_character{};
    std::int32_t level_row{-1};
    std::int32_t entry_point{-1};
    std::int32_t difficulty{-1};
    std::int32_t level_state{};
    // Optional observed value from the source Level owner. Do not derive this
    // from the projection or assume the constructor sentinel at bind time.
    std::int32_t pending_script_id{};
    bool pending_script_id_known{};
    // PlayerSavegame::slot() from the same selected Save owner. Required to
    // build LevelSavegame::GetFilename without guessing from UI state.
    std::int32_t source_save_slot{-1};
    // LevelSavegame::GetFilename's second argument is Level+0x40, populated
    // from the selected LevelList row's +0x10 hub field. It is distinct from
    // the Level entry point (+0x110) and must come from the same row owner.
    std::int32_t level_list_hub{};
    // Exact row count from the retained CharacterTable used by this Level.
    // Required for the one Level-owned Character::s_cachedCharOIDs table.
    std::uint32_t character_table_size{};
};

class Owner {
public:
    bool begin_load() noexcept;
    bool request_level(std::int32_t level_row, std::int32_t entry_point,
                       std::int32_t difficulty) noexcept;
    bool begin_source_load() noexcept;
    bool publish_projection(const void* projection) noexcept;
    bool suspend_projection() noexcept;
    bool begin_teardown() noexcept;
    // Source Level destruction clears its Character OID cache before calling
    // Application::ObjectManager::Flush. Keep that order explicit so manager
    // rows/names survive EGL recreation and are reset only at the source flush.
    bool mark_source_level_caches_cleared() noexcept;
    bool flush_object_manager_after_source_level_clear() noexcept;
    bool finish_teardown() noexcept;
    // Only a selected, real source owner may bind these borrowed identities.
    bool bind_source_chain(std::uintptr_t gslevel, std::uintptr_t level,
                           std::uintptr_t level_savegame,
                           std::int32_t source_level_state) noexcept;
    // Build one native lifecycle identity from the selected LevelList row and
    // borrow the already authoritative PlayerSavegame/QEST owners. This does
    // not allocate a second world, Save, inventory, or quest owner.
    bool bind_native_level(const NativeLevelBinding&) noexcept;
    // Called only after the selected Level::Update provider completes for the
    // same active Level generation. Mirrors its source +0x144 write.
    bool publish_level_update_complete(std::uint32_t generation) noexcept;
    bool set_application_flag_145(std::uintptr_t level,
                                  std::uint8_t value) noexcept;
    bool set_unload_force_f0(std::uintptr_t level,
                             std::uint8_t value) noexcept;
    // Level::Unload writes +0x130 = 0 after saving and stopping audio. Keep
    // that source write on the same active Level identity.
    bool set_source_level_state(std::uintptr_t level,
                                std::int32_t value) noexcept;
    bool begin_checkpoint_load(std::uintptr_t level) noexcept;
    bool finish_checkpoint_load(std::uintptr_t level) noexcept;
    bool reset_is_loaded(std::uintptr_t level) noexcept;
    // Bind the two live owners installed by Level::_LoadCamera. Identities
    // name existing semantic camera owners; they are not copied source pointers.
    bool bind_level_cleanup_owners(std::uintptr_t level,
                                   std::uintptr_t camera_level,
                                   std::uintptr_t camera_overview) noexcept;
    bool clear_level_cleanup_owner(std::uintptr_t level, std::uint32_t index,
                                   std::uintptr_t owner_identity) noexcept;
    // Level::SG_BlockLevel(bool) writes this byte on the same owned
    // LevelSavegame (+0x39). Reject stale Level identities.
    bool sg_block_level(std::uintptr_t level, std::uint32_t blocked) noexcept;
    // Borrow the selected Level's collision world and its source-backed query
    // implementation. The callback must implement PFWorld filtered traversal;
    // the owner supplies include_special=false and guards the borrowed lifetime
    // with the active Level generation.
    bool bind_floor_query(const navigation::CollisionWorld*,
                          FloorHeightQuery) noexcept;
    // Applies Character::SetInitialPosition's floor snap to XYZ. Returns 1 on
    // hit, 0 on miss (leaving Z unchanged), and -1 if stale/unavailable/invalid.
    int set_initial_position_height(std::uint32_t generation,
                                    float xyz[3]) const noexcept;
    // Query the active Level's same PFWorld floor owner without projecting
    // height into the point. `include_special` is the source query byte.
    // Returns 1 on hit, 0 on miss, and -1 for stale/unavailable/invalid.
    int query_floor(std::uint32_t generation, const float xyz[3],
                    std::uint32_t include_special,
                    navigation::HeightHit* hit,
                    const navigation::CollisionWorld** world) const noexcept;
    Snapshot snapshot() const noexcept;

    // The Application-lifetime canonical ObjectManager authority. Callers
    // borrow this same owner; they must not construct a parallel manager.
    object_manager_runtime_owner_v1::Owner& object_manager() noexcept {
        return object_manager_;
    }
    const object_manager_runtime_owner_v1::Owner& object_manager() const noexcept {
        return object_manager_;
    }

    level_quick_save_v1::Status quick_save(
        const level_quick_save_v1::State*, std::uint32_t force,
        const level_quick_save_v1::Services*, level_quick_save_v1::Result*,
        std::string& error);
    level_savegame_save_v1::Status save_level(
        const level_savegame_save_v1::State*,
        const level_savegame_save_v1::Services*,
        level_savegame_save_v1::Result*, std::string& error) const;
    // The LevelSavegame owner bound at Level construction. Object rows passed
    // to save_level_objects must be one ObjectManager map snapshot in ascending
    // signed source-handle order.
    level_savegame_owner_v1::Owner& level_savegame() noexcept {
        return level_savegame_;
    }
    const level_savegame_owner_v1::Owner& level_savegame() const noexcept {
        return level_savegame_;
    }
    character_oid_cache_v1::Owner& character_oid_cache() noexcept {
        return character_oid_cache_;
    }
    const character_oid_cache_v1::Owner& character_oid_cache() const noexcept {
        return character_oid_cache_;
    }
    // Canonical source-Level GameEventManager. Its address remains stable
    // across renderer/EGL projection recreation; close it before the borrowed
    // Native Quest ObjectiveFactory owner retires.
    dh2::data::game_event_manager_owner_v1::Owner& game_event_manager() noexcept {
        return game_event_manager_;
    }
    const dh2::data::game_event_manager_owner_v1::Owner& game_event_manager() const noexcept {
        return game_event_manager_;
    }
    level_savegame_save_v1::Status save_level_objects(
        const level_savegame_save_v1::State*,
        const level_savegame_owner_v1::Object*, std::size_t,
        std::uint32_t online,
        const level_savegame_owner_v1::SerializeServices*,
        const level_savegame_owner_v1::PublishServices*,
        level_savegame_save_v1::Result*,
        level_savegame_owner_v1::SaveResult*, std::string& error);

private:
    Snapshot state_{};
    object_manager_runtime_owner_v1::Owner object_manager_{};
    std::uint64_t object_manager_flush_count_{};
    struct NativeLevelIdentity { std::uint64_t generation{}; } native_level_identity_{};
    struct LevelSavegameIdentity {
        std::uintptr_t player_savegame{};
        const void* quest_owner{};
        std::uintptr_t player_character{};
    } level_savegame_identity_{};
    level_savegame_owner_v1::Owner level_savegame_{};
    character_oid_cache_v1::Owner character_oid_cache_{};
    dh2::data::game_event_manager_owner_v1::Owner game_event_manager_{};
    bool retained_logical_level_{};
    const navigation::CollisionWorld* floor_world_{};
    FloorHeightQuery floor_height_query_{};
    std::uint32_t floor_query_generation_{};
};

} // namespace dh2::source_level_owner_v1
