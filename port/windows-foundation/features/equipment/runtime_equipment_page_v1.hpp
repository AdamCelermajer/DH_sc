#pragma once

#include "runtime_equipment_binding_v1.hpp"
#include "../character_menu/source_composition.hpp"

namespace dh::foundation::equipment_menu {

// Optional read-only identity check for callers that maintain a live source
// inventory projection/resolver alongside CharacterState. It must resolve
// this stable ID to the same source instance and verify the actual item row;
// it must never mutate Gear or synthesize powered descriptors.
using RuntimeEquipmentSourceInstanceCheckV1 = std::function<bool(
    const InventoryItem&, const dh2::data::Item&, std::string&)>;

struct RuntimeEquipmentPageBindingsV1 {
    inventory::MenuBindings inventory_text;
    inventory::DetailBindings details_text;
    RuntimeEquipmentSourceInstanceCheckV1 source_instance_current;
};

struct RuntimeEquipmentPageReleaseV1 {
    MainPageCommand command = MainPageCommand::none;
    bool has_render_change = false;
    RuntimeEquipmentRenderChangeV1 render_change;
    bool has_pending_command = false;
    struct PendingCommand {
        MainPageCommand command = MainPageCommand::none;
        std::string selected_instance_id;
        unsigned source_slot = 10;
    } pending_command;
};

// Full original-art equipment MAINPAGE over one RuntimeEquipmentBinding and
// its exact CharacterState/ItemTable. The page owns only UI presenter glue;
// inventory IDs and source sheets remain in the existing owners.
class RuntimeEquipmentPageV1 : public std::enable_shared_from_this<RuntimeEquipmentPageV1> {
    struct Impl;
    std::unique_ptr<Impl> impl_;
public:
    RuntimeEquipmentPageV1();
    ~RuntimeEquipmentPageV1();
    RuntimeEquipmentPageV1(const RuntimeEquipmentPageV1&) = delete;
    RuntimeEquipmentPageV1& operator=(const RuntimeEquipmentPageV1&) = delete;

    bool bind(RuntimeEquipmentBindingV1&, CharacterState& same_character_state,
              RuntimeEquipmentPageBindingsV1, std::string& error);
    bool ready(std::string& error) const;
    bool content(character_menu::Tab, character_menu::Frame&, std::string& error);
    std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)> content_callback(
        std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)> other_tabs = {});
    bool release(float authored_x, float authored_y, RuntimeEquipmentPageReleaseV1&,
                 std::string& error);
    // Adapts this same page owner to SourceCompositionV1. Provider callbacks
    // retain the page, while owner must be the root's same selected-character
    // lease token used when constructing SourceCompositionV1.
    bool source_page_provider(std::shared_ptr<void> same_source_owner,
                              character_menu::SourcePageProviderV1&,
                              std::string& error);
    // SourceComposition release callbacks cannot return a feature receipt;
    // root consumes this immediately after its typed page release dispatch.
    // False with an empty error means the last source action had no render change.
    bool take_source_render_change(RuntimeEquipmentRenderChangeV1&,
                                   std::string& error);
    // One-shot source action handoff for drop/auto-equip/transmute. The page
    // does not execute these external-owner commands. False with an empty
    // error means there is no pending source command.
    bool take_source_pending_command(RuntimeEquipmentPageReleaseV1::PendingCommand&,
                                     std::string& error);
    void leave_page() noexcept;

private:
    bool release_for_source(float authored_x, float authored_y,
                            std::string& error);
};

} // namespace dh::foundation::equipment_menu
