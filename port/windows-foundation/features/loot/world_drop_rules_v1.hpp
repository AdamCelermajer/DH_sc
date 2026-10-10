#pragma once

// P14 DROPS: original ItemObject rules for items lying in the world.
// Evidence: IDA ItemObject::_GetRandomDropPos 0x3ec668, ::DropAndAwardLoot
// 0x3ec8a0, ::DropInventory 0x3ec974, ::OnCollisionBegins 0x3ec048,
// ::ShowTooltip 0x3ebd5c, ::Interact 0x3ed144, ItemObject::InitOnce 0x3ece80,
// ItemInventory::IsInventoryFull 0x3fe330, ItemInventory::GetNumPotions 0x3fc690.

#include "runtime_world_item_adapter_v1.hpp"
#include "runtime_world_item_interaction_v1.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::loot {

// ItemManager keeps five ItemObjects per ItemAudioVisual category and recycles
// them round-robin (ItemManager::Spawn 0x3eacd0 de-spawns a live slot).
inline constexpr std::size_t world_item_pool_slots_per_category_v1 = 5;
// ItemObject::InitOnce stores the float word 0x40C00000 (6.0) at +944 (GetSpeed).
inline constexpr float source_item_speed_word_v1 = 6.0f;
// PhysicalObject::setPosition (0x46e... 259131) scales game units by 0.01 into
// the Box2D body, but PhysicalObject::setLinearVelocity (259055) stores the
// speed unscaled as m/s. GameObject::UpdateTargetPosition (0x393d74) sets the
// velocity to normalize(destination - position) * GetSpeed, so the item slides
// at 6 m/s = 600 game units/s (1 m = 100 units).
inline constexpr float physics_units_per_meter_v1 = 100.0f;
inline constexpr float world_item_speed_units_per_second_v1 =
    source_item_speed_word_v1 * physics_units_per_meter_v1;
// GameObject::IsAtDestination (0x39361c): XY distance^2 < 6400, i.e. 80 units.
// ItemObject::Update (0x3ebee4) calls GameObject::Stop when that holds, so the
// item rests where it is, up to 80 units short of its landing point.
inline constexpr float world_item_arrival_radius_v1 = 80.0f;
// ItemObject sensor half extents: default +-100, non-flat items x1.5 then the
// two ApplyMeshBox expansions give +-225 XY (item-body-init-events-v2 notes).
// PC adaptation: the sensor-contact test uses this XY box around the item.
inline constexpr float world_item_sensor_half_extent_v1 = 225.0f;
// ItemObject::DropInventory: items dropped by a player are protected from that
// player's Interact for 5000 ms.
inline constexpr std::int32_t player_drop_protection_ms_v1 = 5000;
// ItemInventory::IsInventoryFull: more than 0x63 (99) item slots.
inline constexpr std::size_t source_inventory_slot_limit_v1 = 100;
// ItemTable word indices used by the world-item rules.
inline constexpr std::size_t item_word_pickup_type_v1 = 3;     // PickUpType: Automatic 0, MoveOn 1, Interact 2
inline constexpr std::size_t item_word_audio_visual_v1 = 21;
inline constexpr std::size_t item_word_type_v1 = 22;           // 13 gold, 14 potion
inline constexpr std::size_t item_word_slotting_v1 = 26;       // -1: not equippable
inline constexpr std::int32_t item_type_gold_v1 = 13;
inline constexpr std::int32_t item_type_potion_v1 = 14;
inline constexpr std::int32_t pickup_type_automatic_v1 = 0;

// _GetRandomDropPos. With a killer: direction killer-victim normalized,
// landing = victim + dir*(150+rand(200)) + lateral*(rand(300)-150), where the
// lateral axis is dir x Vec3f_K (0,0,1). Without a killer: XY +- rand(500)-250.
// Draw order is exactly the original (200 then 300; or 500 then 500).
bool scatter_destination_v1(dh2::data::LootRandom8V2& rng,
                            const std::array<float, 3>& victim,
                            const std::array<float, 3>* killer,
                            std::array<float, 3>& out, std::string& error);

// One movement step of a dropped ItemObject (GameObject::UpdateTargetPosition
// with ItemObject::Update's arrival Stop). The item is a ground-plane Box2D body:
// X/Y move at world_item_speed_units_per_second_v1 toward the landing point and
// stop once within world_item_arrival_radius_v1 (XY). Z is never synchronised
// from the body, so the item keeps its spawn height: there is no arc, bounce or
// apex in the original. Travel is clamped so the item stops exactly on the
// arrival radius (the original stops on the first whole frame inside it).
std::array<float, 3> advance_world_item_step_v1(const std::array<float, 3>& position,
                                                const std::array<float, 3>& destination,
                                                float dt_seconds) noexcept;

// Source ItemInstance::GetColor: number of powers -> ItemPowerColor key ->
// FontPalette row (constants from loot_audiovisual_pycst.bin:
// zero 6, one 4, two 1, three 5, four 3; more than four powers use row 2).
std::int32_t item_power_font_palette_row_v1(std::size_t power_count) noexcept;

// Property 194 (potion capacity) is stored q8 fixed point (x256) like every
// CharProperties stat; the rule takes the whole number, clamped at 0.
std::int32_t potion_capacity_from_property_v1(std::int32_t q8_property) noexcept;

// What a failed or refused ItemObject::Interact looked like.
enum class WorldItemPickupOutcomeV1 {
    picked_up,
    rejected_looted_or_unknown,
    rejected_owner_protection,    // owner window gate (Interact step 3)
    rejected_not_local_player,
    inventory_full,               // GAMEPLAYMENUS_INVENTORY_FULL, item stays
    potion_capacity,              // potion stack >= capacity, item stays (silent)
    auto_transmute_unavailable,   // AutoTransmute option > 0 and powers below it
    failed
};

struct WorldItemPickupRulesV1 {
    // Application saved option "AutoTransmute". Not bound in the port: stays 0,
    // which disables the branch (original default is a user option).
    std::uint32_t auto_transmute_option{0};
    // Property 194 (CharProperties::PROPS_GetInt, clamped at 0): Character+936.
    std::int32_t potion_capacity{0};
    // ItemInventory::IsInventoryFull debug switch InfiniteInventory.
    bool infinite_inventory{false};
};

struct WorldItemPickupReportV1 {
    WorldItemPickupOutcomeV1 outcome{WorldItemPickupOutcomeV1::failed};
    RuntimeWorldItemInteractionReceiptV1 receipt;
    std::string item_identifier;
    std::int32_t item_type{};
    std::string error;
};

// ItemObject::Interact (0x3ed144) over the existing store/CharacterState
// owners. Order of gates: unknown item, local player, owner-protection window,
// then item branches: slotting != -1 -> AutoTransmute / inventory full;
// type 14 -> potion capacity; then CharacterState transaction (stack merge /
// gold add) through RuntimeWorldItemAdapterV1::pickup. Returns true only for
// picked_up.
bool interact_world_item_v1(RuntimeWorldItemAdapterV1&, RuntimeWorldItemIdV1 item,
                            ActorId player, bool is_local_player,
                            const ActorState* player_state,
                            const RuntimeWorldItemInteractionServicesV1&,
                            const WorldItemPickupRulesV1&,
                            WorldItemPickupReportV1&);

// Sensor contact. Returns the nearest item (XY distance) whose sensor box
// contains the player; invalid id when none. PC adaptation of
// OnCollisionBegins/OOI: contact only makes the item the target.
RuntimeWorldItemIdV1 select_world_item_target_v1(const RuntimeWorldItemAdapterV1&,
                                                 const std::array<float, 3>& player_position);

// B063 walk-over pickup (PickUpType "MoveOn": every ItemTable row in the Act 1 data).
// Original: POItem::onCollisionBegins 0x4702a8 -> ItemObject::OnCollisionBegins
// 0x3ec048. GetInteractionType (0x3ebeb4) always returns -1, so when a Character's
// body begins contact with the item sensor and CharStateMachine::SM_IsMoving(0) holds,
// the character is stored in the item (+0x2E4); GameObject::Update 0x38cbe8 then calls
// Interact(character) (vtable +152) on the next update and clears it. No key and no
// action button. This tracker reports the items whose sensor BEGINS contact with the
// player while the player is moving, nearest first. Contact that begins while the player
// is idle is remembered but reports nothing (original: no pickup until contact begins
// again). Each contact is attempted once: a rejected item (inventory full, owner window,
// potion capacity) is not retried until the player leaves its sensor and re-enters, so
// a dropped item is not instantly re-collected. An item sliding into a walking player
// begins contact when its box overlaps.
class WorldItemContactTrackerV1 {
public:
    std::vector<RuntimeWorldItemIdV1> begin_contacts(const RuntimeWorldItemAdapterV1&,
                                                     const std::array<float, 3>& player_position,
                                                     bool player_moving);
    void clear() noexcept { inside_.clear(); }
private:
    std::set<RuntimeWorldItemIdV1> inside_;
};

// PickUpType == "Automatic" (0), read from the ItemTable row.
bool world_item_is_automatic_pickup_v1(const RuntimeWorldItemEntryV1&) noexcept;

// Public entry for the Equip stream: removes `quantity` of an unequipped
// inventory item from the CharacterState and publishes it at `position`
// (ItemObject::DropInventory: owner protected for 5 s). Atomic: on failure the
// CharacterState is unchanged. quantity must be 1..255 (record limit).
bool drop_item_to_world(RuntimeWorldItemAdapterV1&, dh::foundation::CharacterState&,
                        const std::string& instance_id, std::uint32_t quantity,
                        const std::array<float, 3>& position, ActorId owner,
                        RuntimeWorldItemIdV1& published, std::string& error);

} // namespace dh::foundation::loot
