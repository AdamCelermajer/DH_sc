#pragma once

#include "character_level_save_serialize_v1.hpp"
#include "level_savegame_owner_v1.hpp"
#include "object_manager_runtime_owner_v1.hpp"

namespace dh2::level_savegame_object_manager_v1 {
namespace manager = object_manager_runtime_owner_v1;
namespace factory = character_runtime_factory_v1;

struct FactsServices {
    void* context{};
    // Read from the same live ObjectBase as this manager row: name at +0x5c,
    // signed ObjectManager key, type word at +0x64, base eligibility byte
    // +0x28, and the source IsCharacter/IsPlayer/IsLocalPlayer/+0x81 facts.
    // Character::Serialize's IsPlayer must agree with these save-gate facts.
    // This is called for every row before source eligibility filtering, so it
    // must be a read-only identity-checked projection, not a guessed default.
    // Returns true on failure.
    bool (*read_object_facts)(void*, const manager::GameObject&,
                              const factory::Record*,
                              level_savegame_owner_v1::Object*, std::string&){};
    // Returns true on failure. Implements the concrete class's exact source
    // virtual Serialize (+0x10) bytes only when its canonical owner exposes
    // every byte. Current native renderer rows have no provider; keep null
    // until source fields are available rather than writing an extension.
    bool (*serialize_noncharacter)(void*, const manager::GameObject&,
                                   std::vector<std::uint8_t>&, std::string&){};
    const character_level_save_serialize_v1::CanonicalServices* character{};
    // Identity-checked path for a canonical Character that is owned outside
    // CharacterRuntimeFactory (the retained Player is one such source owner).
    // This does not enroll a synthetic factory Record or alter ObjectManager.
    const character_level_save_serialize_v1::Services* character_identity{};
    // Identity-checked admission for Character rows that are canonical in the
    // ObjectManager/CharacterList but do not own a CharacterRuntimeFactory
    // Record (the retained Player and direct native actor owners). The facts
    // provider remains responsible for verifying the exact live source row.
    bool (*is_canonical_character_identity)(void*, std::uintptr_t){};
    // Compatibility-specific Player check: keeps the retained local Player
    // distinct from any other Record-less canonical Character.
    bool (*is_canonical_player_identity)(void*, std::uintptr_t){};
};

enum class Status : std::uint8_t {
    saved, invalid_state, manager_failed, identity_mismatch,
    facts_unavailable, facts_failed, serializer_unavailable,
    serializer_failed, owner_failed
};

struct Result {
    std::uint32_t manager_rows{};
    std::uint32_t character_rows{};
    level_savegame_owner_v1::SaveResult save{};
};

// Snapshot the one source-key-ordered ObjectManager, resolve Character rows
// through its canonical factory, and dispatch exact source serializers into
// the existing per-level Savegame owner. It creates no parallel world/save.
Status save_all(level_savegame_owner_v1::Owner&, manager::Owner&,
                const factory::Owner&, std::uint32_t online,
                const FactsServices*,
                const level_savegame_owner_v1::PublishServices*,
                Result*, std::string& error);

} // namespace dh2::level_savegame_object_manager_v1
