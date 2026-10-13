#pragma once

#include "native_current_level_talk_marker_design_settings_v1.hpp"
#include "native_quest_owner.hpp"
#include "../../../../../../port/level-world/character_aggro_object_manager_list.hpp"
#include "../../../../../../port/level-world/character_aggro_target_search.hpp"
#include "../../../../../../port/level-world/object_manager_runtime_owner_v1.hpp"
#include "../../../../../../port/level-world/native_visual_fx_manager_v1.hpp"
#include "../../../../../../port/level-world/source_level_owner_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::native::current_level_talk_objective_marker_v1 {

namespace list = dh2::character::aggro::object_manager_list;
namespace aggro = dh2::character::aggro_search;
using ObjectManager = dh2::object_manager_runtime_owner_v1::Owner;
using VisualFXManager = dh2::native_visual_fx_manager_v1::Owner;
using Operation = dh2::native::quests::TalkObjectiveMarkerOperationV1;

// Callbacks borrow the one existing Character SafeGet provider and shared
// VisualFXManager owner. `objective_fx_slot` is the canonical
// quest_objective_factory_v1::Record::derived_28 (+0x28) field. The manager
// callbacks must not retain that pointer or keep another objective handle.
struct Services {
    const list::Owner* characters{};
    const dh2::source_level_owner_v1::Owner* level_owner{};
    ObjectManager* object_manager{};
    std::uintptr_t level_identity{};
    void* property_context{};
    bool (*safe_get_character_property_id)(void*, std::uintptr_t character,
        std::int32_t& property_id, std::string& error){};
    void* visual_fx_context{};
    bool (*install_marker_fx)(void*, std::int32_t effect_id,
        aggro::Character* target_character,
        std::uintptr_t* objective_fx_slot, std::string& error){};
    bool (*remove_marker_fx)(void*, aggro::Character* target_character,
        std::uintptr_t* objective_fx_slot, std::string& error){};
};

// Keep this context owned by the active Level/quest provider for at least as
// long as the registered TalkToNPC Objective. Quest Owner retains the callback
// context while its canonical EventManager receiver is attached.
struct Context {
    Services services{};
    current_level_talk_marker_design_settings_v1::View design_settings{};
};

inline bool install_shared_fx_owner(void* context, std::int32_t effect_id,
                                    aggro::Character* target_character,
                                    std::uintptr_t* objective_fx_slot,
                                    std::string& error) {
    auto* manager = static_cast<VisualFXManager*>(context);
    if (!manager || !target_character || !objective_fx_slot) {
        error = "TalkToNPC shared VisualFXManager or canonical Objective slot is unavailable";
        return false;
    }
    const auto status = manager->InstallObjectiveMarker(objective_fx_slot,
        effect_id, target_character, nullptr, error);
    // Original InstallObjectiveMarker (0x47d9cc..0x47d9d4) stores a null
    // GrabAnimFX result and returns normally. Exhausting the source pool is
    // therefore a completed marker attempt, not a failed Quest compilation.
    if (status == dh2::native_visual_fx_manager_v1::Status::pool_exhausted &&
        !*objective_fx_slot) {
        error.clear();
        return true;
    }
    if (status != dh2::native_visual_fx_manager_v1::Status::ok) {
        if (error.empty()) error = "TalkToNPC shared VisualFXManager install failed";
        return false;
    }
    error.clear();
    return true;
}

inline bool remove_shared_fx_owner(void* context,
                                  aggro::Character* target_character,
                                  std::uintptr_t* objective_fx_slot,
                                  std::string& error) {
    auto* manager = static_cast<VisualFXManager*>(context);
    if (!manager || !target_character || !objective_fx_slot ||
        !*objective_fx_slot) {
        error = "TalkToNPC shared VisualFXManager remove binding is unavailable";
        return false;
    }
    const auto status = manager->RemoveObjectiveMarker(objective_fx_slot,
        target_character, error);
    if (status != dh2::native_visual_fx_manager_v1::Status::ok) {
        if (error.empty()) error = "TalkToNPC shared VisualFXManager remove failed";
        return false;
    }
    return true;
}

inline bool apply(const Services& services,
                  const current_level_talk_marker_design_settings_v1::View& design_settings,
                  std::uintptr_t objective_identity,
                  std::int32_t target_props_id, Operation operation,
                  std::int32_t priority, std::int32_t quest_state,
                  std::uintptr_t* objective_fx_slot, std::string& error) {
    if (!services.characters || !services.level_owner || !services.object_manager ||
        !services.level_identity || !services.safe_get_character_property_id ||
        !list::validate(services.characters)) {
        error = "TalkToNPC requires the active Level CharacterList, ObjectManager, and SafeGet provider";
        return false;
    }
    const auto level = services.level_owner->snapshot();
    if (level.phase != dh2::source_level_owner_v1::Phase::active ||
        level.source_level != services.level_identity ||
        level.object_manager_identity !=
            reinterpret_cast<std::uintptr_t>(services.object_manager) ||
        &services.level_owner->object_manager() != services.object_manager) {
        // Owner teardown clears the receivers' target marks after the Level has
        // left its active phase; the Characters they marked went with it, so
        // there is nothing left to unmark.
        if (operation == Operation::unregister_target ||
            operation == Operation::remove) {
            error.clear();
            return true;
        }
        error = "TalkToNPC providers do not belong to the active source Level";
        return false;
    }
    if (target_props_id < -1) {
        error = "TalkToNPC source target CharacterTable ID is invalid";
        return false;
    }
    if ((operation == Operation::install || operation == Operation::remove) &&
        (!objective_identity || !objective_fx_slot)) {
        error = "TalkToNPC marker operation lost its canonical Objective +0x28 FX slot";
        return false;
    }

    const auto generation = services.characters->mutation_generation;
    auto* node = services.characters->sentinel.next;
    std::size_t visited = 0;
    aggro::Character* target = nullptr;
    while (node != &services.characters->sentinel) {
        if (!node || visited >= services.characters->character_count ||
            !node->character) {
            error = "TalkToNPC current CharacterList has malformed links";
            return false;
        }
        auto* character = static_cast<aggro::Character*>(node->character);
        const auto* object = character->identity
            ? services.object_manager->find_by_identity(character->identity)
            : nullptr;
        if (!character->identity || !character->object ||
            character->object->identity != character->identity || !object ||
            object->identity != character->identity) {
            error = "TalkToNPC CharacterList row is outside the active ObjectManager";
            return false;
        }
        std::int32_t property_id = -1;
        if (!services.safe_get_character_property_id(services.property_context,
                character->identity, property_id, error)) {
            if (error.empty()) error = "TalkToNPC SafeGetCharPropsId failed";
            return false;
        }
        if (services.characters->mutation_generation != generation ||
            !list::validate(services.characters)) {
            error = "TalkToNPC CharacterList changed during SafeGetCharPropsId";
            return false;
        }
        if (property_id == target_props_id) {
            target = character;
            break; // Source walks the ordered CharacterList and stops on first match.
        }
        ++visited;
        node = node->next;
    }
    if (!target) {
        // Source marker routines return without writing anything when no
        // Character with the requested SafeGetCharPropsId is loaded.
        error.clear();
        return true;
    }

    switch (operation) {
    case Operation::register_target:
        target->talk_objective_marker_2fa = 1;
        error.clear();
        return true;
    case Operation::unregister_target:
        target->talk_objective_marker_2fa = 0;
        error.clear();
        return true;
    case Operation::install: {
        if (!services.install_marker_fx) {
            error = "TalkToNPC InstallObjectiveMarker requires the shared VisualFXManager owner";
            return false;
        }
        std::int32_t effect_id = 0;
        if (!current_level_talk_marker_design_settings_v1::read_effect_id(
                design_settings, priority, quest_state, effect_id, error))
            return false;
        const auto prior_marker = target->talk_objective_marker_2fa;
        const auto prior_priority = target->talk_objective_priority_marker_2fb;
        const auto prior_fx = *objective_fx_slot;
        target->talk_objective_marker_2fa = 1;
        target->talk_objective_priority_marker_2fb =
            static_cast<std::uint8_t>(priority == 3);
        if (!services.install_marker_fx(services.visual_fx_context, effect_id,
                target, objective_fx_slot, error)) {
            // A missing/invalid shared manager can reject before source FX
            // creation. Avoid leaving new Character marker bits when neither
            // the Objective handle nor the FX owner changed. If the manager
            // did retire/replace the handle, retain the reached source writes
            // and report the partial operation to its caller.
            if (*objective_fx_slot == prior_fx) {
                target->talk_objective_marker_2fa = prior_marker;
                target->talk_objective_priority_marker_2fb = prior_priority;
            }
            if (error.empty()) error = "TalkToNPC source marker FX installation failed";
            return false;
        }
        error.clear();
        return true;
    }
    case Operation::remove:
        target->talk_objective_priority_marker_2fb = 0;
        target->talk_objective_marker_2fa = 0;
        if (*objective_fx_slot) {
            if (!services.remove_marker_fx) {
                error = "TalkToNPC RemoveObjectiveMarker requires the shared VisualFXManager owner";
                return false;
            }
            if (!services.remove_marker_fx(services.visual_fx_context,
                    target, objective_fx_slot, error)) {
                if (error.empty()) error = "TalkToNPC source marker FX removal failed";
                return false;
            }
        }
        error.clear();
        return true;
    }
    error = "TalkToNPC marker operation is not implemented";
    return false;
}

inline bool callback(void* raw, std::uintptr_t objective_identity,
                     std::uintptr_t* objective_fx_slot,
                     std::int32_t target_props_id, Operation operation,
                     std::int32_t priority, std::int32_t quest_state,
                     std::string& error) {
    if (!raw) {
        error = "TalkToNPC active-Level marker context is unavailable";
        return false;
    }
    auto& context = *static_cast<Context*>(raw);
    return apply(context.services, context.design_settings,
        objective_identity, target_props_id, operation, priority, quest_state,
        objective_fx_slot, error);
}

} // namespace dh2::native::current_level_talk_objective_marker_v1
