#pragma once

#include "character_runtime_factory_v1.hpp"
#include "level_savegame_object_manager_v1.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::level_savegame_player_load_v1 {

namespace manager = object_manager_runtime_owner_v1;
namespace factory = character_runtime_factory_v1;
namespace save = level_savegame_owner_v1;

// Decoded source fields from Character::Deserialize. Arrays retain exact bytes
// so float bit patterns and the source GameObject payload are not normalized.
struct PlayerState {
    std::uint8_t object_base_flag_80{};
    std::uint8_t object_base_flag_8a{};
    std::int32_t game_object_word_270{};
    std::int32_t state_machine_state{};
    std::array<std::uint8_t, 12> position{};
    std::array<std::uint8_t, 12> rotation{};
    std::uint8_t dead{};
    std::array<std::uint8_t, 12> vector_1306{};
    std::array<std::uint8_t, 12> vector_1309{};
    std::uint8_t has_group_info_tail{};
    std::int32_t group_word_36{};
    std::uint8_t group_flag_40{};
    std::uint8_t group_flag_41{};
};

// CharacterProperties is a four-byte legacy vptr word followed by the exact
// 224 source int32 values. The vptr is retained as opaque metadata and never
// copied into or used to bind a native PropertyState.
struct FactoryPropertyImages {
    std::array<std::uint8_t, 4> saved_header{};
    std::array<std::int32_t, 224> saved{};
    std::array<std::uint8_t, 4> resolved_header{};
    std::array<std::int32_t, 224> resolved{};
};

// The source Character row after its full bounded wire parse. Binary fields
// retain the exact bytes read from the stream; float validation and owner
// conversion happen in the production provider before any writes.
struct FactoryDeserializeState {
    std::uint8_t object_base_flag_80{};
    std::uint8_t object_base_flag_8a{};
    std::int32_t game_object_word_270{};
    std::int32_t state_machine_state{};
    FactoryPropertyImages properties{};
    std::array<std::uint8_t, 12> position{};
    std::array<std::uint8_t, 12> rotation{};
    std::uint8_t dead{};
    std::array<std::uint8_t, 12> vector_1468{};
    std::array<std::uint8_t, 12> vector_1474{};
    std::uint8_t has_group_info_tail{};
    std::int32_t group_word_36{};
    std::uint8_t group_flag_40{};
    std::uint8_t group_flag_41{};
};

// Exact source ObjectBase/GameObject prefix decoded before a concrete
// non-Character Deserialize. GameObject::Deserialize checks +0x270 against
// the already-consumed InitPost cache; it does not overwrite that cache.
struct GameObjectDeserializeState {
    std::uint8_t object_base_flag_80{};
    std::uint8_t object_base_flag_8a{};
    std::int32_t game_object_word_270{};
};
struct DoorDeserializeState {
    GameObjectDeserializeState game_object{};
    std::int32_t door_state_3a8{};
};
struct TriggerDeserializeState {
    GameObjectDeserializeState game_object{};
    std::int32_t pending_3b8{};
    std::int32_t max_count_3b4{};
};
struct ContainerDeserializeState {
    GameObjectDeserializeState game_object{};
    std::uint8_t state_394{};
};
enum class SourceNoncharacterKind : std::uint8_t {
    game_object, door, trigger, container
};
struct SourceNoncharacterDeserializeState {
    SourceNoncharacterKind kind{};
    GameObjectDeserializeState game_object{};
    std::int32_t door_state_3a8{};
    TriggerDeserializeState trigger{};
    ContainerDeserializeState container{};
};

struct CommitServices {
    void* context{};
    // Read-only preflight: confirms every destination field has a canonical
    // mutable owner and the identity still matches before any mutation.
    bool (*prepare_player)(void*, const factory::Record&,
                           const PlayerState&, std::string&){};
    // Must be all-or-nothing: false means no destination field was changed.
    // This seam intentionally does not emulate Character::Deserialize's
    // incremental writes in an unsafe partial native projection.
    bool (*commit_player_atomically)(void*, const factory::Record&,
                                     const PlayerState&, std::string&){};
    // The retained native Player Character is a canonical ObjectManager row
    // but intentionally has no CharacterRuntimeFactory Record. This parallel
    // identity path mutates that existing Player owner without synthesizing a
    // factory Record or shadow Character.
    bool (*prepare_player_identity)(void*, std::uintptr_t,
                                    const PlayerState&, std::string&){};
    bool (*commit_player_identity_atomically)(void*, std::uintptr_t,
                                              const PlayerState&,
                                              std::string&){};
    // Character::Deserialize's bounded alive Faery closure uses this same
    // Record for both property sheets, transform, vectors, flags, group tail,
    // visibility and source Idle/AI lifecycle effects.
    bool (*supports_factory_character_restore)(void*, const factory::Record&,
                                               const FactoryDeserializeState&,
                                               bool* supported, std::string&){};
    bool (*prepare_factory_character_restore)(void*, const factory::Record&,
                                              const FactoryDeserializeState&,
                                              std::string&){};
    bool (*commit_factory_character_restore)(void*, const factory::Record&,
                                             const FactoryDeserializeState&,
                                             std::string&){};
    // Admission is concrete-class/vtable backed, never a type-string guess.
    // Return supported=false for unknown or incomplete source Deserialize
    // closures. The same canonical ObjectManager row is passed to both phases.
    bool (*classify_source_noncharacter_restore)(
        void*, const manager::GameObject&, const save::Object&,
        SourceNoncharacterKind*, bool* supported, std::string&){};
    bool (*prepare_source_noncharacter_restore)(
        void*, const manager::GameObject&,
        const SourceNoncharacterDeserializeState&, std::string&){};
    bool (*commit_source_noncharacter_restore)(
        void*, const manager::GameObject&,
        const SourceNoncharacterDeserializeState&, std::string&){};
};

enum class Status : std::uint8_t {
    loaded_player_only, invalid_argument, malformed_save, unsupported_record,
    manager_failed, identity_mismatch, provider_unavailable,
    provider_failed, commit_failed
};

struct Result {
    std::uint32_t sections{};
    std::uint32_t object_rows{};
    std::uint32_t player_rows{};
    std::uint32_t unapplied_object_rows{};
    std::int32_t source_handle{};
    std::uintptr_t identity{};
    std::uint32_t payload_bytes{};
};

struct FactoryLifecycleResult {
    std::uint32_t rows_examined{};
    std::uint32_t rows_fully_applied{};
    // The row is still incomplete: property values were applied, but the
    // original headers and all other Deserialize effects remain in the source
    // image and replacement-save protection stays active.
    std::uint32_t rows_still_unapplied{};
};

// Bounded LevelSavegame file reader for the current Player slice. It
// accepts the source INFO+OBJS layout, applies only the unique existing
// PlayerCharacter_0 row, and reports all other rows as unapplied. Callers must
// preserve the source image and block replacement saves while unapplied rows
// remain. All records are parsed before prepare/commit can run.
Status load_player(const std::uint8_t* image, std::size_t image_size,
                   manager::Owner&, const factory::Owner&,
                   const level_savegame_object_manager_v1::FactsServices*,
                   const CommitServices*, Result*, std::string& error);

// Restore fully supported alive non-player Character rows into the existing
// factory Record. A row is counted as applied only after every source effect
// in this admitted closure has been committed.
Status restore_factory_character_rows(
    const std::uint8_t* image, std::size_t image_size, manager::Owner&,
    const factory::Owner&,
    const level_savegame_object_manager_v1::FactsServices*,
    const CommitServices*, FactoryLifecycleResult*, std::string& error);

// Restore only source non-Character classes whose exact Deserialize closure
// is admitted by the canonical same-row provider. Unknown rows remain
// unapplied so save replacement stays protected.
Status restore_source_noncharacter_rows(
    const std::uint8_t* image, std::size_t image_size, manager::Owner&,
    const factory::Owner&,
    const level_savegame_object_manager_v1::FactsServices*,
    const CommitServices*, FactoryLifecycleResult*, std::string& error);

} // namespace dh2::level_savegame_player_load_v1
