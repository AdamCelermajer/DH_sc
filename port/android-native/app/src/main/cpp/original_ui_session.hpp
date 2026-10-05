#pragma once
#include <android/asset_manager.h>
#include <memory>
#include <string>
#include <cstdint>
#include "enemy_status_hud_v1.hpp"
#include "model_renderer.hpp"

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
    bool load_health_panel(const std::string& private_directory,std::string&);
    bool attach_player(const std::string& private_directory,std::string&);
    bool prepare_player_frame(int width,int height,std::string&);
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
