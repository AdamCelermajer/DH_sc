#pragma once
#include <android/asset_manager.h>
#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include "enemy_status_hud_v1.hpp"
#include "model_renderer.hpp"
#include "swf_movie.hpp"
#include "authored_character_panel_v2.hpp"

namespace dh2::android_ui {
// Retained authored HUD owner. Movie/font/texture CPU state survives GL
// recreation; the connected status view borrows actual world properties.
class OriginalUiSession {
public:
    OriginalUiSession();
    ~OriginalUiSession();
    OriginalUiSession(const OriginalUiSession&)=delete;
    OriginalUiSession& operator=(const OriginalUiSession&)=delete;
    bool initialize(AAssetManager*,std::string&);
    void bind_platform_music(std::function<bool(std::int32_t&,std::string&)>);
    bool load_health_panel(const std::string& private_directory,std::string&);
    bool attach_player(const std::string& private_directory,std::string&);
    bool prepare_player_frame(int width,int height,std::string&);
    // One source text platform per additional movie/player, borrowing this
    // actual APK resource/GPU owner. Returned services retain both providers
    // and application callbacks through the movie's complete destruction.
    bool movie_services(const ui::SwfServices& application,ui::SwfServices&,std::string&);
    // Enrich the caller's ACTUAL MenuManager/lifecycle/render graph with this
    // same source asset/GPU/font/localization platform. Does not create a
    // second CharacterPanelSession or replace missing application receivers.
    bool character_panel_services(const ui::SwfServices& application,
                                   ui::AuthoredCharacterPanelServicesV2&,std::string&);
    bool hud_movie_borrow_v4(ui::SwfMovie*&,std::shared_ptr<void>&,std::string&);
    bool localization_borrow_v4(ui::HudTextV1*&,std::shared_ptr<void>&,std::string&);
    bool status_messages_v26(std::shared_ptr<ui::MenuStatusMessagesV26>&,std::string&);
    bool character_inventory_preview_v4(ui::SwfMovie&,std::string&);
    bool character_parsed_string_v4(const gameswf::fn_call&,std::string&);
    bool character_swap_hud_v4(const char* actual_callback,std::string&);
    bool hud_pointer(int action,int pointer,float x,float y,std::string& command,std::string& error);
    model_renderer::CombatTextSinkV1 combat_text_sink();
    bool render_combat_text(const model_renderer::CombatTextFrameV1&,std::string&);
    bool render_player(int width,int height,const std::int32_t*,std::size_t,
                       std::uintptr_t character,std::string&,
                       const ui::EnemyHudWorldBorrowV1* enemy=nullptr);
    bool render(int width,int height,std::string&);
    bool update_enemy(const ui::EnemyHudWorldBorrowV1&,std::string&);
    bool active() const;
    bool overlays_player() const;
    void deactivate();
private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};
}
