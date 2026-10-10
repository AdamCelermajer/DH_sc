#pragma once

#include "equipment_main_page.hpp"
#include "source_equipment_appearance.hpp"
#include "source_equipment_render_bridge.hpp"
#include "preview/runtime_equipment_preview_v1.hpp"
#include "../../combat_session.hpp"
#include "../../playable_actor_world.hpp"
#include <functional>
#include <vector>

namespace dh::foundation::equipment_menu {

struct RuntimeEquipmentOptionsV1 {
    std::string table_root = "original-cache/data/pydata";
    std::vector<std::string> slots{
        "slot0", "slot1", "slot2", "slot3", "slot4", "slot5", "slot6", "slot7", "slot8"};
    bool online_requirements_bypass = false;
    bool dual_wield = false;
    bool one_hand_two_hander = false;
    EquipmentPowerResolver powers;
    equipment_menu::Options menu;
    // `load` validates the caller's retained Debug owner; `query` reaches its
    // real Debug.GetSwitch provider (SourceRootScopes' combined adapter also
    // performs the corresponding genuine Debug.Load). No value is fabricated.
    SourceAppearanceDebugServices source_appearance_debug;
};

struct RuntimeEquipmentPreviewBorrowV1 {
    std::uint64_t revision{};
    ActorId actor_id = invalid_actor_id;
    std::string class_id;
    const CharacterVisual* visual{};
    const dh2::scene::Scene* same_scene{};
    // Full VisualSkinOwner draw_views topology, including source weapon rows.
    // A caller that also submits EquipmentAttachmentSet should skip views
    // whose weapon_slot is nonzero to avoid drawing the same weapon twice.
    const std::vector<dh2::skinning::VisualDrawViewV32>* source_views{};
    const EquipmentAttachmentSet* attachments{};
};
using RuntimeEquipmentPreviewBorrowCallbackV1 =
    std::function<bool(const RuntimeEquipmentPreviewBorrowV1&, std::string&)>;

// A packet frame combines the authored Equipment camera/pane/rebase with
// real packets copied from the same retained VisualSkinOwner draw views.
// Packet worlds remain in source space: the synchronous renderer applies
// `presentation.source_inventory_rebase * packet.world`. Source weapon rows
// are included in packets and must not also be submitted from attachments.
struct RuntimeEquipmentPreviewPacketsV1 {
    RuntimeEquipmentPreviewFrameV1 presentation;
    std::shared_ptr<const SourceEquipmentRenderFrameV1> packets;
};
using RuntimeEquipmentPreviewPacketsCallbackV1 =
    std::function<bool(const RuntimeEquipmentPreviewPacketsV1&, std::string&)>;

// A receipt is a non-owning description of the feature's committed render
// model. It borrows the exact CombatSession visual Scene and the attachment
// owner held by this binding; root must consume it synchronously or copy/pin
// the models before the next equipment mutation or binding destruction.
struct RuntimeEquipmentRenderChangeV1 {
    std::uint64_t revision{};
    ActorId actor_id = invalid_actor_id;
    const CharacterVisual* same_session_visual{};
    const dh2::scene::Scene* same_scene{};
    const EquipmentAttachmentSet* attachments{};
};

// Feature-side runtime owner over one caller-owned CharacterState and the
// same CombatSession player/world actor/combat sheets. It owns only parsed
// source tables, adapter/menu glue, one source skin renderer and pinned body
// image lease bound to that exact session Scene, and staged weapon attachments. It does not create a
// second character, actor, combat world, property owner, inventory, or UI art.
class RuntimeEquipmentBindingV1 {
    struct Impl;
    std::unique_ptr<Impl> impl_;
public:
    RuntimeEquipmentBindingV1();
    ~RuntimeEquipmentBindingV1();
    RuntimeEquipmentBindingV1(const RuntimeEquipmentBindingV1&) = delete;
    RuntimeEquipmentBindingV1& operator=(const RuntimeEquipmentBindingV1&) = delete;

    bool bind(CombatSession&, CharacterState& same_character_state,
              const AssetCatalog& item_assets, const AssetCatalog& body_assets,
              const AssetCatalog& weapon_assets, const OriginalPropertyDatabase&,
              RuntimeEquipmentOptionsV1, std::string& error);
    bool ready(std::string& error) const;
    // Composition guard: menu page must borrow the exact CharacterState used
    // to prepare this runtime binding, never a copied inventory projection.
    bool uses_character_state(const CharacterState&, std::string& error) const;

    const dh2::data::ItemTable* item_table(std::string& error) const;
    const dh2::data::Item* item_for_instance(const std::string& instance_id,
                                             std::string& error) const;
    bool select_slot(unsigned source_slot, std::string& error);
    bool select_instance(const std::string& instance_id, std::string& error);
    bool selected_items(std::vector<OwnedSelection>&, std::string& error) const;
    bool frame(character_menu::Frame&, std::string& error) const;
    Presenter* presenter_for_composition(std::string& error) const;
    NativeEquipmentActions menu_actions(std::string& error);

    bool equip_selected(std::string& error);
    bool unequip_selected_slot(std::string& error);
    bool equip_to_slot(const std::string& instance_id, unsigned source_slot,
                       std::string& error);
    // Consumes RuntimeEquipmentPage's typed request_auto_equip command through
    // the existing dh2_equipment_auto_v3/EquipmentAdapter kernel.
    bool auto_equip(const std::string& instance_id, std::string& error);
    // Original NativeInvAutoEquipSlot(slot) / NativeInvAutoEquipSlot(-1): per-slot best-item auto-equip
    // and the whole-sheet ALL button. Same render/appearance refresh as the other mutations.
    bool auto_equip_slot(unsigned source_slot, std::string& error);
    bool auto_equip_all(std::string& error);
    bool unequip(unsigned source_slot, std::string& error);

    // Call after CombatSession samples/advances its live player visual. The
    // staged source weapon socket matrices are refreshed from that exact pose.
    bool sample_render_pose(std::string& error);
    // Reaches the exact source skin owner over this binding's live Scene.
    // Callback receives non-owning views only for its synchronous call; it
    // must not advance animation or retain pointers/positions afterward.
    bool with_preview_borrow(const RuntimeEquipmentPreviewBorrowCallbackV1&,
                             std::string& error);
    // Synchronously prepares actual source geometry/material packets through
    // SourceEquipmentRenderBridgeV1 over the one pinned body-image lease and
    // same skin owner used for modular updates. The callback may submit packets
    // to the existing renderer but must consume presentation's raw borrows
    // before returning and must not advance the paused visual clock.
    bool with_preview_packets(const SourceEquipmentRenderServicesV1&,
                              const RuntimeEquipmentPreviewPacketsCallbackV1&,
                              std::string& error);
    // Equipment avatar pane clock (B051). The pane always shows the bound idle
    // locomotion clip (Session::with_locomotion_preview_pose), never the actor's
    // current action clip, even while gameplay is paused for the page. Like the
    // original Show/RenderCharacterPane pair: opening the page restarts idle, and
    // each rendered frame advances the pane by real frame time.
    void restart_preview_clock();
    void advance_preview_clock(double seconds);
    // Returns false with an empty error when there is no unconsumed change.
    bool take_render_change(RuntimeEquipmentRenderChangeV1&, std::string& error);
};

} // namespace dh::foundation::equipment_menu
