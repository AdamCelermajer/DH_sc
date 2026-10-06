#pragma once
#include <android/asset_manager.h>
#include <memory>
#include <string>
#include <cstdint>
#include "menu_avatar_preview_v1.hpp"
#include "loading_menu_v1.hpp"

namespace dh2::android_ui {
struct OriginalUiCampaignQuestServicesV1 {
    void* context{};
    // Actual retained Game/Online owner supplies GetOnline()->byte5.
    bool (*use_volatile_quest_acts)(void*,bool&,std::string&){};
};
// Retained authored HUD owner. Movie/font/texture CPU state survives GL
// recreation; the connected status view borrows actual world properties.
class OriginalUiSession {
public:
    OriginalUiSession();
    ~OriginalUiSession();
    OriginalUiSession(const OriginalUiSession&)=delete;
    OriginalUiSession& operator=(const OriginalUiSession&)=delete;
    bool initialize(AAssetManager*,std::string&);
    // Android Build facts and actual GLES2 backend, supplied on the GL thread.
    void bind_menu_device(const std::string& manufacturer,const std::string& model,std::uint32_t driver_type);
    // Retained Application/Level providers; bind on their GL owner thread.
    // No implicit Level/Online state or loading-success fallback is supplied.
    void bind_loading_state_services(const ui::LoadingMenuStateServicesV1&);
    void bind_loading_multiplayer_services(const ui::LoadingMenuMultiplayerServicesV1&);
    void bind_loading_hud_services(const ui::LoadingMenuHudServicesV1&);
    // GL owner thread. Actual GSLevel/Level/Online lifecycle calls these;
    // no frame timer supplies progress or advances readiness.
    bool show_game_loading(std::string&);
    bool refresh_game_loading(std::string&);
    void bind_campaign_quest_services(const OriginalUiCampaignQuestServicesV1&);
    // GL-owner thread; provider context must outlive binding and every call.
    // Binding never constructs a preview by itself; authored callback owns it.
    void bind_menu_avatar_services(const ui::MenuAvatarPreviewServicesV1&);
    bool load_front_screen(const std::string& private_directory,const std::string& screen,std::string&);
    bool load_health_panel(const std::string& private_directory,std::string&);
    bool attach_player(const std::string& private_directory,std::string&);
    bool render_player(int width,int height,const std::int32_t*,std::size_t,
                       std::uintptr_t character,std::string&);
    bool render(int width,int height,std::string&);
    std::string consume_menu_sound(); // GL owner thread; drains one source request
    std::string consume_menu_audio(); // GL owner -> Android audio control delivery
    std::string consume_menu_browser(); // GL owner -> Android ACTION_VIEW delivery
    int consume_menu_catalog(); // GL owner -> original portrait IGP Activity, -1 when empty
    bool consume_menu_exit(std::string&); // GL owner, after frame/AS dispatch
    bool debug_menu_sound(const std::string& probe,std::string&);
    bool touch(float x,float y,int action,std::string&);
    bool active() const;
    bool overlays_player() const;
    void deactivate();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
