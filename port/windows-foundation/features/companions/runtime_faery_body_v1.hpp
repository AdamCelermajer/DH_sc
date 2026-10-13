#pragma once
// v21.8 item 8 (faery visual): the player's own faery body.
//
// Original (libDungeonHunter2.so, offline IDA export):
//  * Character::GetCharModelName 0x3a54d4: for a Character whose AI row Type is Faerie (3), the model is NOT the
//    row's own ModelFile; it is the ModelFile of the master's CURRENT faery (GetCharFaery(master, SG_GetCurrentFaerieId))
//    resolved through the character model dictionary.
//  * Character::ChangeFaery 0x3ae99c: after SG_SetCurrentFaerie it re-reads that model name and calls
//    GameObject::SetVisualObject on the faery Character (the same Character, a new model).
//  * Level::PlaceFaeryAndFollowers 0x3f0898 (called at level start, after the player is placed, and by
//    NativeHUDSetActiveFaery after ChangeFaery): faery position = master target position + GetLookAtVec(master)
//    (a UNIT vector, sin(yaw), -cos(yaw), 0), then ForceUpdatePosition.
//  * The faery then follows through its AI row script (follower.luac: MoveTo/Stop/WarpBehind), which the existing
//    B077 follower controller already runs. AISFaery (native `__faery__` script) is not used by this AI row.
//
// The port keeps the actor-owned (session-owned, retained pose) visual untouched and shows the equipped faery
// through a private body visual per model that samples the SAME retained clip at the SAME time. No model is reloaded
// under the session.
#include "../../asset_catalog.hpp"
#include "../../original_character.hpp"

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace dh::foundation::companions {

// "data/3D/characters/faeries/<model_file>.bdae" (the character model dictionary path of a faery ModelFile).
std::string faery_body_model_uri_v1(std::string_view model_file);

// PlaceFaeryAndFollowers: master position + unit look vector. `master_heading` is the master actor yaw in the port
// convention (look-at = (cos, sin), the convention of follower_warp_behind_position_v1). Z is unchanged.
std::array<float, 3> faery_placement_position_v1(const float master_position[3], float master_heading) noexcept;

class RuntimeFaeryBodyV1 final {
public:
    // Returns the private visual for `model_uri`, loading it on first use with the configuration of `source`
    // (clip bank, template clip, motion root and policy flags) and only the model replaced. `on_loaded` runs once
    // after a successful load (texture binding). nullptr and `error` on failure; failures are remembered so a
    // missing model is reported once and not retried every frame.
    CharacterVisual* acquire(const AssetCatalog& assets, const CharacterVisual& source, const std::string& model_uri,
                             const std::function<void(CharacterVisual&)>& on_loaded, std::string& error);
    const CharacterVisual* find(const std::string& model_uri) const;
    std::size_t loaded_count() const noexcept { return bodies_.size(); }

    // Samples `clip` at `sample_ms` on the body's own skeleton and applies it (authored node ids missing in the body
    // are skipped by the same policy the actor visual uses; the body keeps its own particle/glow nodes).
    static bool mirror_pose(CharacterVisual& body, const std::string& clip, std::int32_t sample_ms, std::string& error);

private:
    std::map<std::string, std::unique_ptr<CharacterVisual>> bodies_;
    std::map<std::string, std::string> failures_;
};

} // namespace dh::foundation::companions
