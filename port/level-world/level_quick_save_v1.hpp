#pragma once

#include <cstdint>
#include <string>

namespace dh2::level_quick_save_v1 {

// Borrowed facts from the one live source Level, its LevelSavegame, the
// PlayerManager local Character and Application/network state. This is a
// source-order kernel, not an owner for any of those objects.
struct State {
    std::uintptr_t level{};
    std::uintptr_t level_savegame{};
    std::uintptr_t local_character{};
    std::int32_t level_state{};
    // Result of Character's virtual call at vtable+0x34; QuickSave proceeds
    // only when the source result is zero.
    std::int32_t character_quicksave_predicate{};
    std::uint32_t online{};
    std::uint32_t local_player_is_host{};
    // Application byte at +0x719, preserved as an unnamed source gate.
    std::uint32_t application_gate_719{};
    // Original Character words copied by Level::QuickSave: +0x168, +0x160,
    // +0x164 into +0x1470, +0x1468, +0x146c respectively.
    std::uint32_t character_word_168{}, character_word_160{}, character_word_164{};
    std::uint8_t level_savegame_gate_39{};
};

struct Services {
    void* context{};
    // Writes the source Character fields +0x1470,+0x1468,+0x146c from the
    // supplied words captured at +0x168,+0x160,+0x164 respectively.
    std::int32_t (*copy_checkpoint_position)(void*, std::uintptr_t character,
        std::uint32_t word_168, std::uint32_t word_160, std::uint32_t word_164,
        std::string& error){};
    // Writes LevelSavegame byte +0x39 only when force=true: source writes zero
    // before Save and restores the captured byte afterward. QuickSave(false)
    // leaves +0x39 unchanged.
    std::int32_t (*write_savegame_gate_39)(void*, std::uintptr_t savegame,
        std::uint8_t value, std::string& error){};
    // Calls the same LevelSavegame::Save instance; no second save store.
    std::int32_t (*save_level_savegame)(void*, std::uintptr_t savegame,
        std::string& error){};
};

enum class Status : std::uint32_t {
    saved, skipped, invalid_argument, service_unavailable, service_failed
};

struct Result {
    Status status{Status::invalid_argument};
    std::uint32_t service_calls{};
    std::uint32_t position_copied{};
    std::uint32_t save_called{};
    std::uint32_t original_gate_restored{};
};

// Level::QuickSave(bool) at ELF 0x3f059c. Its LevelSavegame lifetime,
// PlayerManager query and Character predicates remain providers; this owner
// implements only the observed admission, field-copy and temporary-gate
// transaction. Prefix side effects remain applied if a later provider fails.
Status run(const State*, std::uint32_t force, const Services*, Result*,
           std::string& error);

} // namespace dh2::level_quick_save_v1
