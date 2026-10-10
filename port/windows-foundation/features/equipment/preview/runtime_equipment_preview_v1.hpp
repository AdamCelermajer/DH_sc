#pragma once

#include "../../../renderer.hpp"
#include "../../../../engine-skinning/visual_skin_owner_v6.hpp"
#include "../../../equipment_visual.hpp"
#include "../../../original_character.hpp"
#include "../../../actor_state.hpp"
#include <cstdint>
#include <string>

namespace dh::foundation::equipment_menu {

// Authored in the 480x320 Equipment MAIN SWF coordinate system. These bounds
// are the composed `menu_InventorySheetMain/avatarpane` shape, not a guessed
// rectangle from a modern layout.
struct EquipmentPreviewPaneV1 {
    float left = 155.05f;
    float top = 87.70f;
    float right = 321.10f;
    float bottom = 282.75f;
};

struct EquipmentPreviewSourceV1 {
    std::uint64_t revision{};
    ActorId actor_id = invalid_actor_id;
    std::string class_id;
    const CharacterVisual* visual{};
    const dh2::scene::Scene* same_scene{};
    const std::vector<dh2::skinning::VisualDrawViewV32>* source_views{};
    const EquipmentAttachmentSet* attachments{};
};

// The result is a synchronous non-owning frame. Body draw views and gear
// attachments remain borrowed; a host must consume them before the source
// owner mutates or destroys them. Session identity/lifetime must be checked by
// the binding callback before calling this pure frame composer.
struct RuntimeEquipmentPreviewFrameV1 {
    std::uint64_t revision{};
    ActorId actor_id = invalid_actor_id;
    std::string class_id;
    EquipmentPreviewPaneV1 pane;
    Camera camera;
    const CharacterVisual* visual{};
    const dh2::scene::Scene* same_scene{};
    const std::vector<dh2::skinning::VisualDrawViewV32>* source_views{};
    const EquipmentAttachmentSet* attachments{};
    // Borrowed from the source-priority root node in the exact retained Scene.
    // Consume synchronously; never retain across visual update or release.
    const dh2::scene::Node* same_visual_root{};
    Mat4 same_visual_root_world{};
    Mat4 source_inventory_rebase{};
};

// Creates the original Equipment avatar camera in source world units. The
// aspect ratio is the authored avatarpane bounds; viewport placement remains
// the caller's normal menu composition responsibility.
Camera equipment_preview_camera_v1() noexcept;

// Reproduces RenderCharacterPane's source transform: retain root Euler X/Y,
// apply its literal Z=-0.5 radians, preserve authored scale, and remove the
// original root translation/orientation from the current draw matrices.
bool equipment_preview_rebase_v1(const dh2::scene::Node&, Mat4&, std::string& error);

// Composes only data proven to belong to one live visual/Scene. The input's
// borrowed pointers are valid only for the caller's synchronous frame scope.
bool compose_runtime_equipment_preview_frame_v1(const EquipmentPreviewSourceV1&,
    RuntimeEquipmentPreviewFrameV1&, std::string& error);

} // namespace dh::foundation::equipment_menu
