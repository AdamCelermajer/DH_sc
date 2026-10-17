#pragma once
#include "character_faery_selection.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include "../adam-script-runtime/script_runtime.h"
#include <cstddef>
#include <cstdint>

namespace dh2::character_current_spell_v1 {
// Adapted from Adam c3ae797 character_current_spell_v1. The source callback
// captures its Character and ignores Arguments; the two selection reads are
// separate calls and the GetCharFaery validation result is discarded.
enum class Operation : std::uint32_t {selected_faery,validate_faery,saved_level};
struct Request {
    Operation operation;
    std::uint32_t id;
    std::int32_t difficulty;
    std::uintptr_t character;
};
struct Response {std::int32_t value;};
struct Services {
    void* context;
    // Zero is normal return. Nonzero/exception stops at this provider after
    // its completed effects. No extra validation/read/cleanup is added.
    int (*invoke)(void*,const Request*,Response*);
};
struct Result {
    std::uintptr_t character;
    std::uint32_t first_id,second_id,calls,complete;
    std::int32_t level;
    Operation last_operation;
};
enum class Status : std::int32_t {complete=1,invalid_argument=-1,provider_failed=-2};
Status query(std::uintptr_t character,const Services*,Result*);
struct Bindings {std::uintptr_t character;Services services;};
int current_spell_info_v1(void*,const dh2_script_value*,std::uint32_t,
                         dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t) noexcept;

// No saved/faery/property owner is created here. These are live source slots:
// Character+14e8, PlayerSavegame::m_difficultyLevel, Character+106c, and the
// canonical retained faery tables/constants used by GetCharFaery.
struct SavedBindings {
    std::uintptr_t character;
    const data::PlayerSavegameV1* const* savegame_14e8;
    const std::int32_t* difficulty;
    const std::int32_t* faery_list_106c;
    const character_faery_selection::Globals* faery_globals;
    const character_faery_selection::Services* faery_services;
};
Services saved_services(SavedBindings*) noexcept;
// Nonnull saves must belong to this Character. Saved-level delivery requires
// genuine initialized five-row faery backing; unsafe difficulty/ID/assertion
// domains fail explicitly. Null saves preserve source ID0 and level-1 without
// reading difficulty. The existing source selector supplies real validation.
// One thread; all controls, captured Character, provider backing and VM callback
// context remain live through synchronous return and VM close. No same-output
// reentry/retirement/rebinding. Distinct outputs may nest. Output/error storage
// must not alias opaque provider-owned backing. No rollback or second owner.
}
