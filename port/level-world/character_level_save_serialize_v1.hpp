#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "character_runtime_factory_v1.hpp"
#include "level_savegame_owner_v1.hpp"

namespace dh2::character_level_save_serialize_v1 {

struct ByteSpan {
    const std::uint8_t* data{};
    std::size_t size{};
};

// Borrowed fields from the one live Character, its CharacterProperties,
// position, state-machine and optional AI owners. Payload sizes follow the
// pinned ARM32 Character::Serialize calls, not native C++ struct layout.
struct View {
    std::uint32_t state_machine_state{};
    std::uint8_t is_player{};
    ByteSpan properties_primary{};   // Structs::CharacterProperties, 900 bytes.
    ByteSpan properties_secondary{}; // Structs::CharacterProperties, 900 bytes.
    ByteSpan position{};             // Point3D<float>, 12 bytes.
    ByteSpan target_position{};       // Point3D<float>, 12 bytes.
    std::uint8_t dead{};
    ByteSpan vector_1306{};           // Point3D<float>, 12 bytes.
    ByteSpan vector_1309{};           // Point3D<float>, 12 bytes.
    std::uint8_t has_ai{};
    std::int32_t ai_word_36{};
    std::uint8_t ai_flag_40{};
    std::uint8_t ai_flag_41{};
};

struct Services {
    void* context{};
    bool (*read_view)(void*, std::uintptr_t character, View*, std::string&){};
    // Existing same-identity ObjectBase/GameObject fields used by virtual
    // GameObject::Serialize at +0x10.
    bool (*read_game_object_fields)(void*, std::uintptr_t character,
                                    level_savegame_owner_v1::GameObjectFields*,
                                    std::string&){};
};

// Typed bridge for the one live CharacterRuntimeFactory record. The callbacks
// read its already-owned components (properties, state machine, AI and source
// object/transform services); this layer never creates shadow owners. The
// factory's projected identity is the sole Character identity passed through.
struct CanonicalServices {
    void* context{};
    bool (*read_view)(void*, const character_runtime_factory_v1::Record&,
                      View*, std::string&){};
    bool (*read_game_object_fields)(
        void*, const character_runtime_factory_v1::Record&,
        level_savegame_owner_v1::GameObjectFields*, std::string&){};
};

enum class Status : std::uint32_t {
    complete, invalid_argument, provider_unavailable, provider_failed,
    invalid_view, allocation_failed
};

struct Result {
    std::uint32_t game_object_bytes{};
    std::uint32_t character_bytes{};
    std::uint32_t property_blocks{};
    std::uint32_t ai_tail_bytes{};
};

// Character::Serialize at ELF 0x3a65c4. This is the exact virtual payload
// consumed by LevelSavegame::__SaveObjects; it does not append port metadata.
Status serialize(std::uintptr_t character, const Services*,
                 std::vector<std::uint8_t>& output, Result*,
                 std::string& error);

// Factory-record entry point used by level OBJS serialization. It proves that
// the required canonical components exist and that optional AI bytes have an
// AI owner before delegating to the exact source-order serializer.
Status serialize_record(const character_runtime_factory_v1::Record&,
                        const CanonicalServices*,
                        std::vector<std::uint8_t>& output, Result*,
                        std::string& error);

} // namespace dh2::character_level_save_serialize_v1
