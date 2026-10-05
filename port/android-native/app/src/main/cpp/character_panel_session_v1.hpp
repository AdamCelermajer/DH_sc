#pragma once
#include "original_cache_assets_v1.hpp"
#include "hud_text_v1.hpp"
#include "player_gameplay_binding.hpp"
#include "character_panel_presentation_v1.hpp"
namespace dh2::android_ui {
bool initialize_player_skill_slots_v1(const model_renderer::PlayerGameplayBinding&,std::string&);
// Android view transport only. Every query/action borrows the sole live World
// authority on the GL thread; this owner never stores player/gear/Save copies.
class CharacterPanelSessionV1 {
 OriginalCacheAssetsV1 assets_;
 CharacterPanelPresentationV1 presentation_;
 ui::HudTextV1 text_;
 bool ready_=false;
 bool prepare(std::string&);
public:
 explicit CharacterPanelSessionV1(AAssetManager* assets):assets_(assets),presentation_(assets_){}
 std::string snapshot(const model_renderer::PlayerGameplayBinding&);
 std::string action(const model_renderer::PlayerGameplayBinding&,int operation,int index,int slot);
};
}
