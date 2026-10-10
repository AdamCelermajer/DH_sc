#pragma once
#include "../../../equipment_visual.hpp"
#include "../../../../game-data/class_preview_setup.hpp"
#include <array>
#include <functional>
#include <utility>

namespace dh::foundation::frontend {
struct CreationPreviewActor {
    dh2::data::ClassPreviewDefinition definition;
    CharacterVisual body;
    EquipmentAttachmentSet equipment;
    bool selection_active=false;
    bool idle_transition_pending=false;
    bool idle_transition_delivered=false;
    Mat4 scale{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
};
// Original starting appearance, never a saved-slot inventory substitute.
// GPU texture ownership stays with the host's existing material binder.
class CreationPreview {
public:
    using IdleTransitionProvider=std::function<bool(CreationPreviewActor&,std::string&)>;
    bool load(const AssetCatalog&, std::string& error);
    bool select(int classIndex, std::string& error);
    bool update(double seconds, std::string& error);
    // CSAnim event34 -> SM_SetIdleState must reach the genuine owner. An
    // absent provider holds the authored OnSelect endpoint and records this
    // required boundary; it does not silently select a substitute MenuIdle.
    void set_idle_transition_provider(IdleTransitionProvider provider) {idle_provider_=std::move(provider);}
    bool idle_transition_required() const noexcept;
    const char* idle_transition_status() const noexcept;
    const char* required_idle_owner()const noexcept {return "Required SAME class Character/NativeFsm24/CharacterStateOwner CSAnim14 event34 -> SM_SetIdleState(false), equipment stance facts and actual ordinary-idle animator services";}
    bool loaded() const noexcept { return loaded_; }
    int selected() const noexcept { return selected_; }
    std::array<CreationPreviewActor,3>& actors() noexcept { return actors_; }
    const std::array<CreationPreviewActor,3>& actors() const noexcept { return actors_; }
private:
    std::array<CreationPreviewActor,3> actors_;
    bool loaded_=false;
    int selected_=-1;
    IdleTransitionProvider idle_provider_;
};
// MainMenu::CreateAvatarCamera source setters. Its viewport submission may
// temporarily use the physical surface aspect, then restore these values.
// ClassPreviewScene owns a distinct CLASS_SELECTION camera fixed at 4:3;
// never route that camera through a viewport-derived projection.
Camera original_menu_preview_camera();
// Source authored backdrop; explicit failure if this asset was not staged.
bool load_menu_preview_backdrop(const AssetCatalog&, OriginalScene&, std::string&);
} // namespace dh::foundation::frontend
