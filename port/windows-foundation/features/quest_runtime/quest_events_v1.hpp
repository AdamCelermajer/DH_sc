#pragma once
// Generic quest event bus (Preview 16). Producers in combat, zone triggers,
// NPC talk and pickups call raise_quest_event(); the live character session
// binds ONE sink (the QuestRuntimeV1 of that session). Ids are authored source
// ids (v2Quest objective oid1/oid2 values), never map or class names.
#include <cstdint>
#include <functional>
#include <string>

namespace dh::foundation::quest_runtime {

struct QuestEvent {
    enum class Kind : std::uint8_t {
        // Character killed. Matches KillXEnemies/ClearEnemies by property_id
        // and KillEnemyTemplate/ClearEnemyTemplate by template_id (original
        // Character::Kill raises all four constants from the same kill).
        kill,
        // A trigger zone was entered. Matches MoveInZone by zone name.
        zone_enter,
        // NPC talk (Character::Interact TalkToNPC). Matches TalkToNPC oid1/oid2.
        talk_to_npc,
        // Trigger/object interaction (TriggerPlate, TriggerOn, OpenGameObject,
        // DestroyGameObject, PickedUpLiftable). Matches oid1 == object_id.
        object_interact,
        // Item picked up. Reserved for GatherLoot-style progress (not counted yet).
        item_pickup,
    };
    Kind kind{Kind::kill};
    std::int32_t property_id{-1};  // kill: character property id
    std::int32_t template_id{-1};  // kill: character template (CharacterTable row)
    std::int32_t object_id{-1};    // talk / object_interact / item_pickup id (oid1)
    std::int32_t secondary_id{-1}; // talk: second authored id (oid2)
    std::string zone;              // zone_enter: authored trigger zone name (str2 of MoveInZone)
    std::int32_t level_row{-1};    // zone_enter: current level row (-1 when unknown)
};

using QuestEventSink = std::function<void(const QuestEvent&)>;

// Binds the single live sink (empty function unbinds). Returns the previous one.
QuestEventSink bind_quest_event_sink(QuestEventSink sink);

// Delivers one event to the bound sink. Returns false when no sink is bound
// (the event is dropped and the caller can report it; nothing is queued).
bool raise_quest_event(const QuestEvent& event);

} // namespace dh::foundation::quest_runtime
