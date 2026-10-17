#pragma once

#include <cstdint>
#include <string>

namespace dh2::character_init_post_player_v1 {

// This is an ordered adapter for the pinned middle block of Character::InitPost,
// not a Character owner. All identities and fields are borrowed from the
// caller's existing Character, CharProperties and Save graph.
enum class Operation : std::uint32_t {
    load_save_mask,
    current_level,
    set_difficulty,
    is_local_player,
    init_equipment,
    reset_gear_properties,
    quest_sync,
    load_base_properties,
    load_gear_properties,
    recalc_properties,
    init_skill_slots,
};

struct Request {
    Operation operation{};
    std::uintptr_t subject{};
    std::uint32_t source_callsite{};
    std::uint32_t query_ordinal{};
    std::int32_t argument{};
};

struct Reply {
    // current_level: identity is the borrowed Level and word is Level+0x118.
    // is_local_player: word is the original truth value.
    std::uintptr_t identity{};
    std::uint32_t word{};
};

struct Services {
    void* context{};
    // Dispatch to already selected Save/equipment/skills/property services.
    // A zero return means success; no service or owner is synthesized here.
    int (*invoke)(void*, const Request&, Reply&, std::string&){};
};

struct Bindings {
    std::uintptr_t character{};
    std::uintptr_t property_owner{};
    // Borrowed projections of Character+0x13c8 (signed halfword) and
    // Character+0x14e8 (current Save pointer). They must stay live throughout
    // this synchronous sequence; both are reread at their original use sites.
    const std::int16_t* property_id_13c8{};
    std::uintptr_t* current_save_14e8{};
    Services services{};
};

enum class Status { complete, invalid_argument, busy, failed };
enum class Stage {
    not_started,
    load_save_mask,
    current_level,
    set_difficulty,
    local_player_equipment_query,
    init_equipment,
    reset_gear_properties,
    quest_sync,
    load_base_properties,
    load_gear_properties,
    recalc_properties,
    local_player_skills_query,
    init_skill_slots,
    covered_cutoff,
};

struct Result {
    Stage stage{Stage::not_started};
    std::uintptr_t captured_character{};
    std::uintptr_t captured_property_owner{};
    std::uintptr_t captured_save{};
    std::int32_t property_id{};
    std::uint32_t provider_calls{};
    std::uint32_t save_load_calls{};
    std::uint32_t locality_queries{};
    bool difficulty_set{};
    bool equipment_initialized{};
    bool skills_initialized{};
};

class Runtime {
    Bindings bindings_;
    bool busy_{};
public:
    explicit Runtime(Bindings);
    Status initialize(Result*, std::string& error);
};

// Exact source-order block at Character::InitPost 0x3b513c..0x3b51bc:
// SG_Load(4), optional current-Level difficulty sync, fresh locality query,
// optional initial equipment/gear reset/Save quest sync, base+gear property
// loads, RecalcProperties(true), then a second fresh locality query and
// optional skill-slot initialization. The call at 0x3b51bc is the exclusive
// cutoff; later InitPost work is deliberately outside this adapter.
//
// The service dispatcher must route load_save_mask to the existing
// CharacterGameplaySave runtime, init_equipment to the selected initial
// equipment runtime, and init_skill_slots to selected player-skill grants.
// Level/locality/quest/property operations remain explicit live source
// providers. The dispatcher may not replace their shared Save/property/skill
// owners or fabricate a Character. Provider effects before failure are kept.

} // namespace dh2::character_init_post_player_v1
