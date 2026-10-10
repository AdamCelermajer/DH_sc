#pragma once

#include "native_binding_v1.hpp"
#include "../../../engine-animation/events.hpp"
#include "../../../level-world/character_state.hpp"

namespace dh::foundation::faery_menu {

// Sink for the already-owned canonical Character state coordinator. It is a
// dispatch edge, not another FSM: state must be the same State inside the
// canonical actor and owner must be the selected-character lease.
struct CastStateSinkV1 {
    std::shared_ptr<void> owner;
    dh2::character::State* state{};
    std::uintptr_t character{};
    void* context{};
    bool (*raise_event)(void*, dh2::character::State&, std::int32_t event,
                        std::uintptr_t payload, std::string&){};
    bool (*set_state)(void*, dh2::character::State&, std::int32_t next_state,
                      std::int32_t event, std::uintptr_t payload,
                      std::string&){};
};

struct SourceFaeryAbilityV1 {
    NativeOwnerInputV1 native;
    CastStateSinkV1 state_sink;
};

struct ActiveFaeryAbilityV1 {
    std::int32_t difficulty{-1};
    std::int32_t save_slot{-1};       // current-faery Save slot 0..4
    std::int32_t table_record_id{-1}; // selected Character FaeryList record
    std::int32_t spell_type{-1};      // authored Faery row word +7
    std::string spell_script;         // actual Faery row script bytes
    const dh2::character::skills::Instance32* spell_instance{}; // borrowed V6 slot
};

struct CastAnimationV1 {
    std::int32_t save_slot{-1};
    std::int32_t animation_table{-1};
    std::int32_t source_sequence{-1}; // CharAnimTable Spells[save_slot]
    std::int32_t stance{};
    std::uint32_t animation_override{}; // source 32-bit sequence + stance sum
};

// Pure reached-source arithmetic after the actual Character CharAnimTable row,
// Save slot, constants and optional Gear stance have been read. False means
// the source row/index branch returned without writing State.
inline bool source_cast_sequence_facts_v1(const std::vector<std::int32_t>& spells,
                                          std::int32_t animation_table,
                                          std::int32_t save_slot,
                                          std::int32_t stanced_mask,
                                          std::int32_t actual_stance,
                                          CastAnimationV1& output) {
    if (animation_table < 0 || save_slot < 0 ||
        static_cast<std::size_t>(save_slot) >= spells.size()) return false;
    CastAnimationV1 result{};
    result.animation_table = animation_table;
    result.save_slot = save_slot;
    result.source_sequence = spells[static_cast<std::size_t>(save_slot)];
    result.stance = (std::uint32_t(stanced_mask) & 0x400000u) ? actual_stance : 0;
    result.animation_override = std::uint32_t(result.source_sequence) +
                                std::uint32_t(result.stance);
    output = result;
    return true;
}

// Returns a source no-op (no sequence) for the original invalid table/index
// and short Spells-array branches. A stored -1 sequence remains a real source
// value and is not normalized into a no-op.
bool source_cast_animation_v1(const SourceFaeryAbilityV1&, CastAnimationV1&,
                              bool& has_sequence, std::string& error);

// Reads the actual Player Save current-faery slot, the same Character's
// FaeryList mapping/row and V6 Spell slot. A nullable spell instance is kept
// nullable because the source callback has a null-instance no-op branch.
bool active_faery_ability_v1(const SourceFaeryAbilityV1&,
                             ActiveFaeryAbilityV1&, std::string& error);

// Mirrors SM_SetCastState's reached prefix: if the original row/index checks
// pass, write animation_override on the same live Character State, then call
// the supplied original RaiseStateEvent or SetState sink. It never starts an
// animation, fabricates an event, or completes a cast itself.
bool source_set_cast_state_v1(const SourceFaeryAbilityV1&, bool direct_set,
                              std::uintptr_t payload, bool& source_noop,
                              std::string& error);

// The existing animator coordinator must pass its real TriggeredEvent here.
// Only state 7 + exact authored "do_spell" reaches the same V6 Spell Use
// callback; other events/states keep the source no-op result.
bool source_faery_animation_event_v1(const SourceFaeryAbilityV1&,
                                     const dh2::animation::TriggeredEvent&,
                                     std::string& error);

} // namespace dh::foundation::faery_menu
