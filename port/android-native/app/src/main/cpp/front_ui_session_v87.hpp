#pragma once
#include <android/asset_manager.h>
#include <memory>
#include <string>
#include <cstdint>
#include "menu_avatar_preview_v1.hpp"
#include "loading_menu_v1.hpp"
#include "swf_menu_device_v1.hpp"
#include "front_selected_profile_v50.hpp"
#include "multi_menu_resource_v93.hpp"
#include <menu_main_menu_state_v114.hpp>
#include <vector>
namespace dh2::ui {class SwfMovie;class OwnedHudSettingsV1;class HudTextV1;}
namespace dh2::application {class ApplicationServicesOwnerV5;}

namespace dh2::android_ui {
struct FrontCampaignQuestServicesV87 {
    void* context{};
    // Actual retained Game/Online owner supplies GetOnline()->byte5.
    bool (*use_volatile_quest_acts)(void*,bool&,std::string&){};
};
// Retained authored HUD owner. Movie/font/texture CPU state survives GL
// recreation; the connected status view borrows actual world properties.
struct FrontMovieDrawV93 {std::uint32_t slot{};std::uintptr_t expected_movie{};std::string actual_clip_path;};
class FrontUiSessionV87 {
public:
    FrontUiSessionV87();
    ~FrontUiSessionV87();
    FrontUiSessionV87(const FrontUiSessionV87&)=delete;
    FrontUiSessionV87& operator=(const FrontUiSessionV87&)=delete;
    bool initialize(AAssetManager*,std::string&);
    bool bind_main_menu_state_services_v114(ui::GSFlashMenuServicesV114,std::string&);
    bool borrow_main_menu_state_v114(loader::MainMenuStateBorrowV114&,std::string&);
    bool enter_main_menu_state_v114(const loader::MainMenuStateBorrowV114&,std::string&);
    bool resume_main_menu_state_v114(std::string&);
    bool resume_main_menu_state_v114(bool& completed_entry,std::string&);
    bool arm_main_menu_state_resume_v114(std::string&);
    // Call before SAME existing MenuManager frame Update, only while this
    // actual GSFlashMenu state is active. Does not supply another movie clock.
    bool update_main_menu_request_v114(std::string&);
    // Native active-state receipt produced by actual Ctor/explicit resume.
    bool main_menu_state_active_v114()const;

    bool bind_process_text_v101(ui::HudTextV1*,std::shared_ptr<void>,std::string&);
    bool prepare_process_settings_v105(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
        const std::string&,std::function<bool(std::uint32_t&,std::string&)>,std::string&);
    bool load_process_settings_v105(bool language_only,std::string&);
    bool prepare_process_front_v119(const std::string&,int,int,std::string&);
    bool borrow_menu_avatar_state_v121(ui::MenuAvatarPreviewStateV1*&,std::shared_ptr<void>&,std::string&);
    bool bind_menu_avatar_services_v121(ui::MenuAvatarPreviewServicesV1,std::shared_ptr<void>,std::string&);
    bool menu_save_exists_v121(std::int32_t,bool&,std::string&);
    bool load_process_splash_v119(std::int32_t language,std::string&);
    bool render_process_splash_v119(int,int,std::string&);
    bool clear_process_splash_v119(std::string&);
    bool save_process_settings_v119(std::string&);
    bool load_process_property_names_v119(std::string&);
    void unload_process_property_names_v119();
    void bind_campaign_language_scene_v109(std::weak_ptr<void>,std::function<bool(std::string&)>);
    bool set_process_language_v109(std::int32_t,std::string&);
    bool project_selected_profile_file_v114(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
        std::int32_t slot,std::int32_t difficulty,data::CampaignProfileFileV1,
        std::shared_ptr<const FrontSelectedProfileV50>&,std::string&);
    bool reread_selected_profile_v101(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
        std::int32_t actual_slot,std::int32_t actual_difficulty,std::shared_ptr<const FrontSelectedProfileV50>&,std::string&);
    bool bind_profile_application_v114(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,std::string&);
    // Android Build facts and actual GLES2 backend, supplied on the GL thread.
    void bind_menu_device(const std::string& manufacturer,const std::string& model,std::uint32_t driver_type);
    bool menu_device_borrow_v4(ui::MenuDeviceFactsV1&,std::string&) const;
    bool menu_device_cells_v98(std::shared_ptr<void>&,const std::uint8_t*&,const std::uint8_t*&,std::string&) const;
    bool base_movie_borrow_v4(ui::SwfMovie*&,std::shared_ptr<void>&,std::string&);
    bool main_movie_borrow_v4(ui::SwfMovie*&,std::shared_ptr<void>&,std::string&);
    bool character_menu_sound_v4(const gameswf::fn_call&,std::string&);
    bool gameplay_settings_borrow_v67(std::shared_ptr<ui::OwnedHudSettingsV1>&,std::string&)const;
    // Source file slots: base0/main2. Loading is a base0 state, not a file slot.
    // Nullable getters return true+empty only for actually absent owned slots.
    bool movie_slot_v93(std::uint32_t,ui::MenuMovieBorrowV58&,std::string&);
    bool load_main_movie_slot_v93(std::string&);
    bool source_main_movie_borrow_v114(ui::MenuMovieBorrowV58&,std::string&);
    bool source_load_main_movie_v114(const char*,const std::function<bool(std::int32_t&,std::int32_t&,std::string&)>&,ui::MenuMovieBorrowV58&,std::string&);
    bool load_base_movie_slot_v105(std::string&);
    bool camera_slot_v93(std::uint32_t,ui::MenuCameraBorrowV93&,std::string&);
    bool movie_virtual10_v93(std::uint32_t,std::uintptr_t,std::int32_t,bool,std::string&);
    bool deleting_movie_v93(std::uint32_t,std::uintptr_t,std::string&);
    bool clear_movie_slot_v93(std::uint32_t,std::string&);
    bool deleting_camera_v93(std::uint32_t,std::uintptr_t,std::string&);
    bool clear_camera_slot_v93(std::uint32_t,std::string&);
    // Actual MenuManager supplies live render order/clip selection. Claim
    // disables legacy render-time advances/recreation; root owns all timing.
    bool claim_menu_transport_v93(std::shared_ptr<void>,
        std::function<bool(std::vector<FrontMovieDrawV93>&,std::string&)>,std::string&);
    bool release_menu_transport_v93(const std::shared_ptr<void>&,std::string&);
    bool render_source_movies_v93(int,int,std::string&);

    // Retained Application/Level providers; bind on their GL owner thread.
    // No implicit Level/Online state or loading-success fallback is supplied.
    bool finish_loading_fs_v114(const char* command,const char* args,bool& handled,std::string&);
    void bind_loading_state_services(const ui::LoadingMenuStateServicesV1&);
    void bind_loading_multiplayer_services(const ui::LoadingMenuMultiplayerServicesV1&);
    // GL owner thread. Actual GSLevel/Level/Online lifecycle calls these;
    // no frame timer supplies progress or advances readiness.
    bool show_game_loading(std::string&);
    bool refresh_game_loading(std::string&);
    void bind_campaign_quest_services(const FrontCampaignQuestServicesV87&);
    // GL-owner thread; provider context must outlive binding and every call.
    // Binding never constructs a preview by itself; authored callback owns it.
    void bind_menu_avatar_services(const ui::MenuAvatarPreviewServicesV1&);
    bool load_front_screen(const std::string& private_directory,const std::string& screen,std::string&);
    // GL owner thread: load the retained front resources without a frame tick.
    bool prepare_front_resources_v114(int width,int height,std::string&);
    struct LaunchRequest {std::int32_t slot=-1,difficulty=0;
        std::shared_ptr<const FrontSelectedProfileV50> profile;};
    // Consume after authored dispatch unwinds, on the same GL owner thread.
    bool consume_launch_request(LaunchRequest&);
    void demo_persona_mode();
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
    std::shared_ptr<Impl> impl_;
};
}
