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
// position, state-machine and optional GroupInfo owners. Payload sizes follow the
// pinned ARM32 Character::Serialize calls, not native C++ struct layout.
struct View {
    std::uint32_t state_machine_state{};
    std::uint8_t is_player{};
    ByteSpan properties_primary{};   // Structs::CharacterProperties, 900 bytes.
    ByteSpan properties_secondary{}; // Structs::CharacterProperties, 900 bytes.
    ByteSpan position{};              // GameObject position at Character+0x160.
    ByteSpan rotation{};              // GameObject rotation at Character+0x16c.
    std::uint8_t dead{};              // Character+0x1449.
    ByteSpan vector_1306{};           // Serialized Point3D at Character+0x1468.
    ByteSpan vector_1309{};           // Revive position at Character+0x1474.
    // Character+0x3fc is CharAI+52, the optional GroupInfo pointer. The six
    // trailing source bytes come from that pointed GroupInfo, not CharAI.
    std::uint8_t has_group_info{};
    std::int32_t group_word_36{};
    std::uint8_t group_flag_40{};
    std::uint8_t group_flag_41{};
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
// read its already-owned components (properties, state machine and source
// object/transform services); this layer never creates shadow owners. The
// factory bridge independently validates the retained CharAI GroupInfo slot;
// a non-null slot still requires a canonical pointee owner. The factory's
// projected identity is the sole Character identity passed through.
struct CanonicalServices {
    void* context{};
    bool (*read_view)(void*, const character_runtime_factory_v1::Record&,
                      View*, std::string&){};
    bool (*read_game_object_fields)(
        void*, const character_runtime_factory_v1::Record&,
        level_savegame_owner_v1::GameObjectFields*, std::string&){};
    // Resolve the factory Record's non-null CharAI+0x34 GroupInfo identity
    // through the canonical level owner. The serializer supplies the exact
    // pointer identity and accepts only this tail for the optional stream.
    bool (*read_group_info_tail)(void*, std::uintptr_t group_identity,
                                 std::int32_t* word_36,
                                 std::uint8_t* flag_40,
                                 std::uint8_t* flag_41, std::string&){};
};

enum class Status : std::uint32_t {
    complete, invalid_argument, provider_unavailable, provider_failed,
    invalid_view, allocation_failed
};

struct Result {
    std::uint32_t game_object_bytes{};
    std::uint32_t character_bytes{};
    std::uint32_t property_blocks{};
    std::uint32_t group_info_tail_bytes{};
};

// Character::Serialize at ELF 0x3a65c4. This is the exact virtual payload
// consumed by LevelSavegame::__SaveObjects; it does not append port metadata.
Status serialize(std::uintptr_t character, const Services*,
                 std::vector<std::uint8_t>& output, Result*,
                 std::string& error);

// Factory-record entry point used by level OBJS serialization. It proves that
// required canonical components and the CharAI GroupInfo pointer slot belong
// to the record. Null slots omit the tail; non-null slots require a separate
// retained GroupInfo pointee owner before the optional tail can be serialized.
Status serialize_record(const character_runtime_factory_v1::Record&,
                        const CanonicalServices*,
                        std::vector<std::uint8_t>& output, Result*,
                        std::string& error);

} // namespace dh2::character_level_save_serialize_v1
