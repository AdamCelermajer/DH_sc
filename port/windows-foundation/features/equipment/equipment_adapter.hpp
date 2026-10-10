#pragma once
#include "../../actor_state.hpp"
#include "../../original_actor_properties.hpp"
#include "../../equipment_visual.hpp"
#include "../../original_combat_properties.hpp"
#include "../../../game-data/item_gear_properties_v5.hpp"
#include <functional>

namespace dh::foundation {
using EquipmentPowerResolver = std::function<bool(const InventoryItem&,
    std::vector<dh2::data::GearPowerProperty12V5>&, std::string&)>;
using EquipmentVisualResolver = std::function<bool(const CharacterState&, const ActorState&,
    std::vector<EquipmentVisualDefinition>&, std::string&)>;
struct EquipmentAdapterOptions {
    std::vector<std::string> slots{"slot0","slot1","slot2","slot3","slot4","slot5","slot6","slot7","slot8"};
    bool dual_wield = false, one_hand_two_hander = false;
    // Explicit original GetOnline && character virtual predicate result.
    bool online_requirements_bypass = false;
    EquipmentPowerResolver powers;
    EquipmentVisualResolver visuals;
    const AssetCatalog* assets = nullptr;
    const CharacterVisual* body = nullptr;
    EquipmentAttachmentSet* attachments = nullptr;
    // Host publishes through PlayableActorWorld::update_combat_properties with
    // genuine traits/main-item reference. Must prepare all failure-prone work
    // before replacing the live owner. Required by the combat-owner overload.
    std::function<bool(const ActorState&, const OriginalCombatProperties&, std::string&)> publish_combat;
    // Actual live source-cell producer may refresh the candidate snapshot before
    // gear recalc. It must borrow canonical base/saved/raw vitals; host float
    // mirrors are not converted into guessed saved-sheet contributions here.
    std::function<bool(const ActorState&,dh2::data::PropertyState&,std::string&)> prepare_canonical_properties;
};
bool equipment_meets_requirements(const dh2::data::Item&, const OriginalActorProperties&,
                                  bool online_bypass = false) noexcept;
bool equipment_meets_requirements(const dh2::data::Item&, const dh2::data::PropertyState&,
                                  bool online_bypass = false) noexcept;
// Borrows shared ownership; no private inventory or persistent equipment copy.
// Failed preparation commits neither ownership, properties, actor nor visuals.
class EquipmentAdapter {
public:
    EquipmentAdapter(CharacterState&, ActorState&, OriginalActorProperties&,
        const dh2::data::ItemTable&, const OriginalPropertyDatabase&,
        EquipmentAdapterOptions = {});
    EquipmentAdapter(CharacterState&, ActorState&, const OriginalCombatProperties&,
        const dh2::data::ItemTable&, const OriginalPropertyDatabase&,
        EquipmentAdapterOptions);
    bool equip(const std::string& instance_id, std::string& error);
    // Uses the recovered source automatic slot-selection kernel over this
    // adapter's actual equipment and property projections.
    bool auto_equip(const std::string& instance_id, std::string& error);
    bool equip_to_slot(const std::string& instance_id,unsigned source_slot,std::string& error);
    bool unequip(unsigned source_slot, std::string& error);
    bool refresh(std::string& error);
private:
    bool change(const std::string*, unsigned, std::string&);
    CharacterState& character_; ActorState& actor_; OriginalActorProperties* properties_=nullptr;
    const OriginalCombatProperties* combat_=nullptr;
    const dh2::data::ItemTable& items_; const OriginalPropertyDatabase& database_;
    EquipmentAdapterOptions options_;
};
}
