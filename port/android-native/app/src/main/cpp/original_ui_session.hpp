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
#include "multi_menu_resource_v93.hpp"
#include "menu_postmovie_v62.hpp"

namespace dh2::data {struct QuestRewardDefinitionV51;}
namespace dh2::ui {class FlashAnimManagerV92;class AuthoredGameplayHudV1;struct HudTextEnvironmentV1;}
namespace model_renderer {struct SourceDeathRewardsLeavesV84;}
namespace model_renderer {struct SourceCampaignItemLeavesV88;}
namespace dh2::android_ui {struct SourceScriptUiLeavesV96;}
namespace dh2::android_ui {class SourceProcessArraysV101;}
namespace dh2::android_ui {struct ProcessTrophyServicesV100;}
namespace dh2::loader {struct LevelDestroyServicesV1;}
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
    bool source_load_hud_primary_v63(const std::string& private_directory,std::string&);
    bool source_scan_loaded_hud_v63(std::uintptr_t,std::string&);
    bool attach_player(const std::string& private_directory,std::string&);
    bool activate_source_player_v67(const model_renderer::PlayerGameplayBinding&,
                                   const std::shared_ptr<void>& actual_menu_manager,std::string&);
    bool render_source_player_v67(int width,int height,std::string&);
    bool prepare_player_frame(int width,int height,std::string&);
    // One source text platform per additional movie/player, borrowing this
    // actual APK resource/GPU owner. Returned services retain both providers
    // and application callbacks through the movie's complete destruction.
    bool movie_services(const ui::SwfServices& application,ui::SwfServices&,std::string&,bool load_hud=true);
    // Enrich the caller's ACTUAL MenuManager/lifecycle/render graph with this
    // same source asset/GPU/font/localization platform. Does not create a
    // second CharacterPanelSession or replace missing application receivers.
    bool character_panel_services(const ui::SwfServices& application,
                                   ui::AuthoredCharacterPanelServicesV2&,std::string&,bool load_hud=true);
    bool hud_movie_borrow_v4(ui::SwfMovie*&,std::shared_ptr<void>&,std::string&);
    bool authored_action_cache_v62(ui::AuthoredGameplayHudV1*&,std::shared_ptr<void>&,std::string&);
    bool source_action_icon_cell_v62(const std::int32_t*&,std::shared_ptr<void>&,std::string&);
    bool renderer_set_wire_frame_v62(bool,std::string&);
    using HudWorldOperationV62=std::function<int(const ui::HudManagerRequest&,ui::HudManagerResponse&,std::string&)>;
    bool bind_hud_world_v62(std::shared_ptr<void> transport,HudWorldOperationV62,
                           const ui::HudControlsServicesV62&,std::string&,std::shared_ptr<void> actual_world={});
    bool require_unbound_campaign_aliases_v115(std::string&)const;
    bool can_retire_hud_campaign_v104(const std::shared_ptr<void>& actual_world,
                                    const std::shared_ptr<void>& actual_transport,std::string&)const;
    bool retire_hud_campaign_v104(const std::shared_ptr<void>& actual_world,
                                 const std::shared_ptr<void>& actual_transport,std::string&);
    bool info_hud_update_v62(std::string&);
    bool bind_level_ui_release_v107(dh2::loader::LevelDestroyServicesV1&,std::string&);
    bool hud_controls_update_v62(std::string&);
    bool hud_controls_native_event_v68(dh2::ui::SwfEvent48&,std::string&);
    bool construct_hud_controls_v62(std::shared_ptr<events::EventManagerOwnerV12>,std::string&);
    //Only the actual LoadMenu(3) suffix may call these stores/initializers.
    bool loadmenu3_info_store_v62(std::uintptr_t expected_movie,std::string&);
    bool loadmenu3_controls_store_v62(std::uintptr_t expected_movie,std::string&);
    bool localization_borrow_v4(ui::HudTextV1*&,std::shared_ptr<void>&,std::string&);
    // SAME retained process cache, available before any HUD movie/World.
    bool process_string_manager_borrow_v101(ui::HudTextV1*&,std::shared_ptr<void>&,std::string&);
    //One original PyDataConstants.Load stage per call; stage27 returns ready.
    bool load_process_constants_stage_v101(const std::function<bool(const char*,std::string&)>&,
                                           bool& complete,std::string&);
    bool load_process_arrays_stage_v101(bool& complete,std::string&);
    bool process_arrays_borrow_v101(std::shared_ptr<SourceProcessArraysV101>&,std::string&);
    bool process_trophy_services_v119(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
                                      ProcessTrophyServicesV100&,std::string&);
    bool process_cache_read_v119(const std::string&,bool& found,std::vector<std::uint8_t>&,
                                std::uint32_t maximum_bytes,std::string&);
    bool gameplay_text_environment_v67(ui::HudTextEnvironmentV1&,std::shared_ptr<void>&,std::string&);
    bool status_messages_v26(std::shared_ptr<ui::MenuStatusMessagesV26>&,std::string&);
    bool source_script_ui_leaves_v97(const model_renderer::SourceCampaignCandidateBorrowV55&,
                                    SourceScriptUiLeavesV96&,std::string&);
    bool quest_completed_dialog_v108(std::int32_t title,std::int32_t style,const std::vector<data::QuestRewardDefinitionV51>&,std::string&);
    bool refresh_message_caches_stage26_v66(std::string&);
    bool complete_refresh_stage26_v66(std::string&);
    bool display_fast_travel_v83(bool,const char* localized,const char* level,std::int32_t entry,std::string&);
    bool source_integer_string_v83(std::int32_t,std::string&,std::string&);
    bool enqueue_tutorial_v118(const std::shared_ptr<void>&,std::int32_t,std::int32_t,std::string&);
    bool skip_tutorial_v118(const std::shared_ptr<void>&,bool all,std::string&);
    bool bind_combat_presentation_v115(const model_renderer::SourceCampaignCandidateBorrowV55&,std::string&);
    bool bind_character_reward_text_v114(const model_renderer::SourceCampaignCandidateBorrowV55&,std::string&);
    bool bind_death_rewards_text_v84(const model_renderer::SourceCampaignCandidateBorrowV55&,
                                    model_renderer::SourceDeathRewardsLeavesV84&,std::string&);
    bool bind_item_presentation_v88(const model_renderer::SourceCampaignCandidateBorrowV55&,
                                    model_renderer::SourceCampaignItemLeavesV88&,std::string&);
    bool character_inventory_preview_v4(ui::SwfMovie&,std::string&);
    bool character_parsed_string_v4(const gameswf::fn_call&,std::string&);
    bool character_swap_hud_v4(const char* actual_callback,std::string&);
    bool hud_pointer(int action,int pointer,float x,float y,std::string& command,std::string& error);
    std::shared_ptr<ui::FlashAnimManagerV92> flash_anim_manager_v92()const noexcept;
    bool reset_flash_for_movie_v92(std::uintptr_t actual_menu_fx,std::string&);
    bool destroy_hud_movie_slot_v92(std::uintptr_t expected_facade,std::string&);
    bool hud_movie_slot_v93(ui::MenuMovieBorrowV58&,std::string&);
    bool hud_movie_virtual10_v93(std::uintptr_t,std::int32_t,bool,std::string&);
    bool deleting_hud_movie_v93(std::uintptr_t,std::string&);
    bool clear_hud_movie_slot_v93(std::string&);
    bool claim_menu_transport_v93(std::shared_ptr<void>,std::string&);
    bool release_menu_transport_v93(const std::shared_ptr<void>&,std::string&);

    bool destroy_hud_auxiliary_v92(std::string&);
    bool hud_camera_slot_v93(ui::MenuCameraBorrowV93&,std::string&);
    bool delete_hud_camera_v93(std::uintptr_t expected,std::string&);
    bool clear_hud_camera_slot_v93(std::string&);
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
    std::shared_ptr<ui::FlashAnimManagerV92> flash_manager_; // process host, not HUD resource
};
}
