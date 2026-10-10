#pragma once

// P14 DROPS: main-callable composition of the world-item presentation. It binds
// the existing SourceWorldItemDropRenderV1 projection (itemdrops.bdae geometry,
// exact BRES materials) to the live Renderer's texture upload and exposes a
// per-frame packet list plus the data needed for the target name label.
// It owns no item store and no pickup authority.

#include "source_world_item_drop_material_v1.hpp"
#include "source_world_item_drop_render_v1.hpp"
#include "../loot/world_drop_rules_v1.hpp"
#include "../../../engine-ui/character_menu_font_palette_v1.hpp"
#include <memory>
#include <string>

namespace dh::foundation {
class Renderer;
}

namespace dh::foundation::interactions {

class WorldDropRuntimeV1 {
public:
    WorldDropRuntimeV1();
    ~WorldDropRuntimeV1();
    WorldDropRuntimeV1(const WorldDropRuntimeV1&) = delete;
    WorldDropRuntimeV1& operator=(const WorldDropRuntimeV1&) = delete;

    // `assets` is the content catalog (itemdrops.bdae, atlas textures, and the
    // authored external effect data/gfx/effects/GL_Diffuse_L1_VC_iPhone.bdae).
    bool load(AssetCatalog& assets, std::shared_ptr<loot::RuntimeWorldItemAdapterV1> store,
              dh2::data::LootAudioVisualV8::Borrow audiovisual, Renderer& renderer,
              std::string& error);
    bool loaded() const noexcept { return bool(renderer_); }

    // Builds the packets for the current store state. The frame stays valid
    // until the next call and must outlive RenderQueue::flush of its meshes.
    bool prepare(std::string& error);
    const SourceWorldItemDropRenderFrameV1* frame() const noexcept { return frame_.get(); }

    // Source ItemInstance::GetColor -> FontPalette textcolor (0xRRGGBB) for the
    // name label/status text. Items dropped by this port carry no powers.
    bool item_color(const loot::RuntimeWorldItemEntryV1&, std::uint32_t& rgb,
                    std::string& error) const;

    std::vector<std::string> resolved_visuals() const;
    const std::map<std::string, std::string>& unresolved_visuals() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::unique_ptr<SourceWorldItemDropRenderV1> renderer_;
    std::shared_ptr<const SourceWorldItemDropRenderFrameV1> frame_;
    dh2::ui::CharacterMenuFontPaletteV1 palette_;
};

} // namespace dh::foundation::interactions
