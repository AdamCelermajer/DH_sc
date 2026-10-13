#include "original_ui_session.hpp"
#include "gameplay_main_menu_request_v1.hpp"
#include "native_gameplay_touch_v1.hpp"
#include "../../../../../engine-ui/inventory_click_binding_v1.hpp"
#include "../../../../../engine-ui/hud_manager.hpp"
#include "../../../../../engine-ui/native_use_potion_call_v1.hpp"
#include "menu_class_selection_v1.hpp"
#include "original_ui_assets.hpp"
#include "swf_gpu.hpp"
#include "swf_hud_freetype_provider.hpp"
#include "swf_font_resolver.hpp"
#include "localization.hpp"
extern "C" {
#include "../../../../../pydata-constants/constants.h"
}
#include "swf_texture.hpp"
#include "swf_input_history.hpp"
#include "swf_frame_connection.hpp"
#include "player_status_hud.hpp"
#include "authored_gameplay_hud_v1.hpp"
#include "gameplay_hud_usable_refresh_v1.hpp"
#include "deferred_hud_refresh_v1.hpp"
#include "deferred_hud_skills_refresh_v1.hpp"
#include "gameplay_menu_stack_v1.hpp"
#include "authored_joystick_v1.hpp"
#include "authored_hud_edge_layout_v6.hpp"
#include "textures.hpp"
#include "original_menu_sound_data.hpp"
#include "player_gameplay_audio.hpp"
#include "swf_menu_sound.hpp"
#include "swf_menu_navigation.hpp"
#include "swf_menu_options.hpp"
#include "settings_native_files_v1.hpp"
#include "settings_language_scene_v1.hpp"
#include "menu_native_event_v1.hpp"
#include "menu_frame_clock.hpp"
#include "menu_diagnostic_filter.hpp"
#include "swf_menu_save_slots.hpp"
#include "menu_save_slot_projection_v1.hpp"
#include "campaign_profile_files_v1.hpp"
#include "original_menu_viewport_v1.hpp"
#include "data.hpp"
#include "savegame_options_v1.hpp"
#include "hud_text_format_v1.hpp"
#include "item_text_varargs_v5.hpp"
#include "swf_menu_launch_v1.hpp"
#include "../../../../../engine-ui/fscommand_continue_v1.hpp"
#include "merchant_item_details_gate_v1.hpp"
#include "../../../../../engine-ui/source_dialog_message_queue_v1.hpp"
#include "../../../../../engine-ui/source_status_message_queue_v1.hpp"
#include "../../../../../engine-ui/source_char_menu_tutorial_queue_v1.hpp"
#include "../../../../../engine-ui/skill_equip_callback_v1.hpp"
#include "font_palette_projection_v1.hpp"
#include "skill_train_callback_v1.hpp"
#include "skill_details_preview_v1.hpp"
#include "character_menu_reload_v1.hpp"
#include "model_renderer.hpp"
#include "native_quest_owner.hpp"
#include "native_quest_cursor.hpp"
#include "native_quest_journal_callback_v1.hpp"
#include "gameswf/gameswf_as_classes/as_array.h"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_player.h"
#include <android/log.h>
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <algorithm>
#include <atomic>
#include <deque>
#include <cmath>
#include <vector>
#include <filesystem>

namespace dh2::android_ui {
namespace {
constexpr const char* tag="DH2Native";
constexpr const char* panel="_root.menu_HUD_0.HUDelements.HealthBars.player";
constexpr const char* hud_sha="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
struct AssetClose {void operator()(AAsset* value)const{if(value)AAsset_close(value);}};
bool valid_private_directory(const std::string& directory,std::string& error,const char* diagnostic){
    if(directory.empty()||directory.find('\0')!=std::string::npos){error=diagnostic;return false;}
    const auto path=std::filesystem::u8path(directory);
    if(!path.is_absolute()||path==path.root_path()||path.filename().empty()){
        error=diagnostic;return false;
    }
    for(const auto& component:path)if(component==".."){error=diagnostic;return false;}
    return true;
}
}
struct OriginalUiSession::Impl {
    AAssetManager* manager{};
    FrontRuntimeServices runtime;
    std::deque<NativeStartGameIntentV1> launch_requests;
    inline static Impl* active_fscommand_owner{};
    bool launch_delivered=false;
    OriginalUiAssets assets;
    SwfGpu gpu;
    std::string directory,font_failure,provider_failure;
    std::array<std::vector<std::uint8_t>,4> constant_bytes;
    std::array<dh2_pycst_view,4> constants{};
    std::vector<std::uint8_t> dialog_constant_bytes;
    dh2_pycst_view dialog_constants{};
    std::vector<std::int32_t> dialog_actor_name_ids;
    struct SourceDialogStepV1 {
        std::int32_t style=-1;
        std::int32_t actor=-1;
        std::int32_t text=-1;
        std::int32_t title=-1;
    };
    std::vector<std::vector<SourceDialogStepV1>> source_dialog_table;
    bool source_dialog_table_loaded=false;
    ui::SourceDialogMessageQueueV1 source_dialog_messages;
    ui::SourceStatusMessageQueueV1 source_status_messages;
    ui::SourceCharMenuTutorialQueueV1 source_char_menu_tutorial_messages;
    std::vector<std::uint8_t> font_palette_bytes;
    ui::Localization localization;
    const dh2::data::ItemTable* item_text_items{};
    const dh2::data::CharacterTable* item_text_characters{};
    ui::GameOptionTableV1 option_table;
    std::unique_ptr<ui::OwnedHudSettingsV1> settings;
    std::unique_ptr<ui::SettingsNativeFilesV1> settings_files;
    std::int32_t language_selection=-1;
    dh2::data::CharacterTable menu_characters;
    dh2::data::LevelTables menu_levels;
    dh2::data::quest_table_bindings_v1::Owner menu_quest_tables;
    std::shared_ptr<const std::vector<std::uint8_t>> menu_quest_records,menu_quest_names,
        menu_quest_constants_bytes;
    dh2::native::quests::Constants menu_quest_constants;
    // Front-only selected difficulty; gameplay ownership is a separate handoff.
    std::int32_t menu_selected_difficulty=0;
    // The production front owner loads a static menu scene, never gameplay
    // Characters/items. Its actor registries start empty and are discarded
    // when a gameplay attachment changes ownership; gameplay localization
    // must use that level's registries, not these front-only sentinels.
    ui::SettingsCharacterNode16V1 front_character_end{&front_character_end,0};
    ui::SettingsObjectNode32V1 front_object_end{};
    ui::SettingsLanguageScene24V1 front_language_scene{&front_character_end,&front_object_end,&front_object_end};
    std::map<std::uintptr_t,std::vector<std::uint8_t>> leases;
    std::uintptr_t next_lease=1;
    std::map<std::string,ui::SwfTexture> exports;
    bool selected=false,loaded=false,live_player=false,menu_string_flag=false,report_frame=true;
    std::string front_screen;
    std::atomic_bool front_name_input_active{false};
    // Initial owned navigation slice. Other registered native menu types,
    // shared-renderer settings and transition animations remain pending.
    std::vector<std::string> menu_stack;
    // Adam's original dqcharmenu renderer is a separate source renderer from
    // the gameplay HUD. Its screens borrow the live game owners through native
    // callbacks; this stack stores navigation only and owns no player data.
    std::vector<std::string> game_menu_stack;
    // The original Level HUD-active flag is toggled by NativeAwayFromHud /
    // NativeBackToHud. Its finger map reset is deferred until the current SWF
    // event has unwound, so the HUD input graph is never re-entered recursively.
    bool gameplay_hud_active=true;
    bool gameplay_hud_input_reset_pending=false;
    bool gameplay_back_to_hud_draining_stack=false;
    // NativeBackToHud is a synchronous SWF callback in the original game, but
    // its DisplayRightHud action re-enters the shared GameSWF core. Queue the
    // owner transition until the current input dispatch has fully unwound.
    bool gameplay_back_to_hud_pending=false;
    // The authored NativeBackToHud callback returns to the HUD from any menu
    // depth; Android system Back instead dismisses one nested overlay first.
    bool gameplay_native_back_to_hud_pending=false;
    // NativeReviveAllPlayers mutates the level and runs menu transitions. Defer
    // its work until the active GameSWF input dispatch has released its core.
    bool gameplay_native_revive_pending=false;
    // NativeGoToMainMenu tears down the source Level and Players. Defer that
    // operation until the current authored HUD click has returned.
    dh2::native::GameplayMainMenuRequestV1 gameplay_main_menu_request;
    ui::DeferredHudRefreshV1 gameplay_hud_refresh;
    // NativeSetMultitouch writes MenuManager+0x110, which MenuManager::onEvent
    // (0x430d68) reads as a lock: while it is set, fingers 1..3 are consumed
    // without reaching any menu. The skill-tree drag raises it; it is clear by default.
    bool gameplay_multitouch_locked=false;
    std::int32_t hud_style=-1;
    model_renderer::UiPlayerHudProjectionV1 gameplay_hud_state{};
    ui::HudManagerState gameplay_enemy_hud_state{};
    // Monster shown by the enemy bar this frame; consumed after display() so the
    // group follows it (DELIBERATE USER-APPROVED DEVIATION, see enemy_follow).
    std::uintptr_t gameplay_enemy_follow_identity{};
    std::uintptr_t gameplay_enemy_hud_player_identity{};
    ui::GameplayHudUsableRefreshV1 gameplay_hud_usable_refresh;
    ui::DeferredHudSkillsRefreshV1 gameplay_hud_skills_refresh;
    bool gameplay_hud_projection_warned=false;
    bool inventory_callback_probe_logged=false;
    std::uintptr_t gameplay_character_button{};
    std::array<bool,4> gameplay_character_button_pressed{{false,false,false,false}};
    // NativeMerchantSellItem consumes MenuManager's one-shot gate. MenuBase::Hide
    // resets that source gate for the next merchant-menu lifetime.
    bool merchant_sell_one_shot_used=false;
    std::string active_menu_path()const{return "_root."+(menu_stack.empty()?std::string("menu_MainMenu"):menu_stack.back());}
    // menu_Loading is authored only in dqshared (not dqcharmenu or the HUD).
    static bool shared_state(const std::string& name){return name=="menu_HelpButtons"||name=="menu_Help"||name=="menu_About"||name=="menu_Options"||name=="menu_Loading";}
    static bool character_menu_state(const std::string& name){
        return name=="menu_CharacterMenu"||name=="menu_CharacterSheetNew"||
               name=="menu_CharacterSheetRecovery"||name=="menu_CharacterSheetMagic"||
               name=="menu_CharacterSheetDefense"||name=="menu_CharacterSheetOffense"||
               name=="menu_CharacterSheetStats"||name=="menu_InventorySheetMain"||
               name=="menu_InventorySheetDetails"||name=="menu_SkillTreeSheetNew"||
               name=="menu_FaerySheet"||name=="menu_QuestLogSheetNEW"||
               name=="menu_MapSheet"||name=="menu_Specialisation"||
               name=="menu_Merchant"||name=="menu_MultiplayerLobbyMulti"||
               name=="menu_FriendInvitationMulti"||name=="menu_confirm2";
    }
    static bool gameplay_menu_state(const std::string& name){
        // This state is authored in dqhud_droid.swf (SpriteID 429); it is the
        // pause panel itself, not a dqcharmenu screen.
        return ui::gameplay_menu_state_v1(name);
    }
    ui::SwfMovie* menu_movie(const std::string& name)const{
        if(live_player&&character_menu_state(name))return character_menu_movie.get();
        return shared_state(name)?shared_menu_movie.get():movie.get();
    }
    ui::SwfMovie* active_menu_movie()const{return menu_stack.empty()?movie.get():menu_movie(menu_stack.back());}
    ui::SwfMovie* input_dispatch_movie{};
    ui::SwfFrameConnection* input_dispatch_frames{};
    std::int32_t last_menu_dt{};
    int class_index=0;
    std::uintptr_t class_left=0,class_right=0;
    std::chrono::steady_clock::time_point frame_time{};
    std::chrono::steady_clock::time_point game_menu_frame_time{};
    bool confirmation_state_probed=false;
    bool talent_detail_callback_logged=false;
    MenuFrameClock menu_clock;
    MenuFrameClock game_menu_clock;
    int driver_width=480,driver_height=320;
    ui::FlashCamera40 camera{};
    ui::FlashCamera40 shared_camera{};
    ui::FlashCamera40 character_camera{};
    std::array<std::int32_t,5> reported_frames{{-1,-1,-1,-1,-1}};
    unsigned glyph_uploads=0,bitmap_uploads=0,string_calls=0,core_errors=0,packed_glyphs=0;
    unsigned strips=0,lines=0,masks=0;
    std::array<bool,5> reported_hardcoded_labels{};
    bool loading_bitmap_reported=false;
    std::deque<std::string> menu_sounds;
    std::deque<std::string> menu_audio;
    int last_width=0,last_height=0;
    // Reverse destruction keeps every provider and owned texture alive until
    // the last movie and its reachable ActionScript graph have been released.
    struct FrameOwner {
        std::shared_ptr<ui::SwfInputHistory> history=std::make_shared<ui::SwfInputHistory>();
        ui::SwfFrameConnection frames;
        ui::MenuNativeEventV1 main_events;
        std::uint32_t input_selection=0;
    };
    std::shared_ptr<FrameOwner> frame_owner;
    std::shared_ptr<FrameOwner> shared_frame_owner;
    std::shared_ptr<FrameOwner> character_frame_owner;
    std::unique_ptr<ui::SwfHudFreetypeProvider> fonts;
    std::unique_ptr<ui::SwfMovie> movie;
    std::unique_ptr<ui::SwfMovie> shared_menu_movie;
    std::unique_ptr<ui::SwfMovie> character_menu_movie;
    std::unique_ptr<ui::PlayerStatusHud> status;
    // The gameplay HUD adapter borrows the existing dqhud movie and the
    // current authored player; it creates no second SWF/player owner.
    std::unique_ptr<ui::AuthoredGameplayHudV1> gameplay_hud;
    bool gameplay_hud_bound=false;
    // One entry per source finger index (MenuManager::onEvent cursor 0..3). The
    // authored joystick and attack buttons are separate SWF buttons, so two
    // fingers hold them at once; a control is captured by one finger only.
    std::array<int,4> gameplay_hud_pointers{{-1,-1,-1,-1}};
    // The menus consumed this finger's press, so ZoomHandler never sees it.
    std::array<bool,4> gameplay_ui_touch_owned{{false,false,false,false}};
    // ZoomHandler's finger map: fingers the menus left alone drive the camera.
    dh2::native::gameplay_touch_v1::CameraFingerMap camera_fingers;
    unsigned camera_pans_delivered=0,camera_pans_applied=0,camera_pinches_delivered=0;
    bool hud_control_held(int control)const noexcept{
        return std::find(gameplay_hud_pointers.begin(),gameplay_hud_pointers.end(),control)!=gameplay_hud_pointers.end();
    }
    // No menu covers the world: the gameplay HUD layer is active, and the only menu
    // that may be open is the map, which drives its own CameraLevel.
    bool camera_layer_open()const noexcept{
        return front_screen.empty()&&live_player&&gameplay_hud_active&&
            (game_menu_stack.empty()||game_menu_stack.back()=="menu_MapSheet");
    }
    // A tap that landed on the authored DialogBox is owned by it until released.
    bool dialog_touch_owned=false;
    // StopDialog popped a DialogMsg and another one waits: the SWF StartDialog
    // callback is re-entered once the dispatch that popped it has unwound.
    bool dialog_start_pending=false;
    // Same for StatusMsg: onStatusMessage is re-entered after the dispatch unwinds.
    bool status_start_pending=false;
    bool source_cutscene_active=false;
    ui::AuthoredJoystickStateV1 authored_joystick{};
    bool authored_joystick_ready=false;
    unsigned authored_joystick_drag_trace_count=0;
    std::string status_panel_path()const{
        return gameplay_hud&&gameplay_hud_bound?gameplay_hud->elements_path()+".HealthBars":
            std::string("_root.menu_HUD_0.HUDelements.HealthBars");
    }
    std::string status_player_path()const{
        return gameplay_hud&&gameplay_hud_bound?status_panel_path()+".player":std::string(panel);
    }
    static bool authored_joystick_position(void* context,std::int32_t x,std::int32_t y,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.gameplay_hud||!self.gameplay_hud_bound){error="Authored gameplay HUD joystick owner unavailable";return false;}
        return self.gameplay_hud->joystick_stick_position(x,y,error);
    }
    static bool authored_joystick_controller_allowed(void*,bool& allowed,std::string& error){
        // Query the same live Character controller gate as the source
        // HUDControls owner. The authored command then enters HeadTowards
        // directly, without the Android gamepad/camera adapter a second time.
        std::uintptr_t identity=0;
        if(!model_renderer::ui_player_identity(identity,error))return false;
        if(identity==0){allowed=false;error.clear();return true;}
        return model_renderer::ui_player_controller_allowed(identity,allowed,error);
    }
    static bool authored_joystick_rotate(void*,const float base[3],float degrees,float out[3],std::string& error){
        return ui::authored_joystick_rotate_direction_v1(base,degrees,out,error);
    }
    static bool authored_joystick_head_towards(void*,const float direction[3],std::string& error){
        if(!direction||!std::isfinite(direction[0])||!std::isfinite(direction[1])||!std::isfinite(direction[2])){
            error="Malformed authored joystick movement vector";return false;
        }
        // Source Joystick.Update already rotates/scales this vector. Preserve
        // it for Character::HeadTowards instead of reapplying camera mapping.
        return model_renderer::authored_hud_command(direction,false,error);
    }
    static bool authored_joystick_stop(void*,std::string& error){return model_renderer::authored_hud_command(nullptr,true,error);}
    ui::AuthoredJoystickServicesV1 authored_joystick_services(){
        return {this,authored_joystick_position,authored_joystick_controller_allowed,
            authored_joystick_rotate,authored_joystick_head_towards,authored_joystick_stop};
    }
    std::array<std::int32_t,4> front_rectangle()const{
        const auto& state=!front_screen.empty()?front_screen:
            !game_menu_stack.empty()?game_menu_stack.back():front_screen;
        return ui::original_menu_viewport_v1::viewport_for_screen_state_v1(
            state,driver_width,driver_height);
    }
    std::array<std::int32_t,4> viewport_rectangle()const{
        return ui::original_menu_viewport_v1::full_surface(driver_width,driver_height);
    }
    static bool input_accepts(void*,ui::SwfEvent48&,bool& accepted,std::string&){
        // MenuBase::CanHandleEvent, 0x41f3fc, returns true.
        accepted=true;return true;
    }
    static bool input_advance(void* context,gameswf::root* root,float seconds,bool flag,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.input_dispatch_frames){error="Native SWF input has no selected frame owner";return false;}
        return self.input_dispatch_frames->advance(root,seconds,flag,error);
    }
    static bool shared_input_advance(void* context,gameswf::root* root,float seconds,bool flag,std::string& error){
        return static_cast<Impl*>(context)->shared_frame_owner->frames.advance(root,seconds,flag,error);
    }
    static bool raw_event_position(void* context,int& x,int& y,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.input_dispatch_movie){error="Native menu event outside retained input delivery";return false;}
        return self.input_dispatch_movie->input_raw_position(x,y,error);
    }
    static bool input_native_event(void* context,ui::SwfEvent48& event,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        ui::MenuNativeEventServicesV1 services;services.context=context;services.raw_position=raw_event_position;
        if(!self.menu_stack.empty()&&self.menu_stack.back()=="menu_SelectClass"&&event.kind==2){
            // MenuCharacterSelect::OnEvent 0x4282c8..0x42839c: compare
            // actual cached characters, clamp the native index to 0..2.
            // Source OnEvent gates selector actions with field fc; Update clears
            // it during a camera transition and restores it after IsAnimOver.
            int selected=self.class_index;
            const bool changed=ui::menu_class_selection_click_v1(event.kind,event.character,
                self.class_left,self.class_right,model_renderer::class_scene_input_enabled(),
                self.class_index,selected);
            if(changed){
                self.class_index=selected;
                // Start immediately in this input batch, so a second queued tap
                // cannot skip from class 0 to 2 before the next frame update.
                if(!model_renderer::select_class_scene(self.class_index,0,error)||
                   !self.movie->menu_action_script(&self,update_class,error))return false;
            }
        }
        auto owner=self.input_dispatch_movie==self.character_menu_movie.get()?self.character_frame_owner:
                   self.input_dispatch_movie==self.shared_menu_movie.get()?self.shared_frame_owner:self.frame_owner;
        if(!owner){error="Native menu input owner unavailable";return false;}
        const bool delivered=self.front_screen=="main"&&(self.menu_stack.empty()||self.menu_stack.back()=="menu_MainMenu")
            ?owner->main_events.main(event,services,error):owner->main_events.base(event,services,error);
        if(delivered&&self.live_player&&self.front_screen.empty()&&self.gameplay_hud_active&&
           self.input_dispatch_movie==self.movie.get()&&event.kind==6&&!event.consumed&&
           event.character==self.gameplay_character_button&&self.game_menu_stack.empty()){
            __android_log_print(ANDROID_LOG_INFO,tag,"Authored HUD character-menu release | name %s | source character button id 163",
                event.name?event.name:"<null>");
            if(!push_game_menu(&self,"menu_CharacterMenu",error))return false;
        }
        __android_log_print(delivered?ANDROID_LOG_INFO:ANDROID_LOG_WARN,tag,
            "Original main native event stage | kind %u | name %s | delivered %d | consumed %u",event.kind,event.name?event.name:"<null>",delivered,event.consumed);
        return delivered;
    }


    struct HudCharacterBinding {Impl* self;std::uintptr_t* identity;};
    static bool bind_hud_character_button(void* context,ui::SwfAsGraph& graph,std::string& error){
        auto& binding=*static_cast<HudCharacterBinding*>(context);ui::SwfAsValue root,target;
        if(!binding.self->gameplay_hud){error="Authored gameplay HUD binding unavailable";return false;}
        const auto path=binding.self->gameplay_hud->control_path(ui::AuthoredHudControlV1::character);
        if(!graph.root_value(root,error)||!graph.find_target(root,path.c_str(),target,error))return false;
        if(!target.identity()){error="Required authored gameplay character-menu button identity";return false;}
        *binding.identity=target.identity();return true;
    }
    bool gameplay_character_button_hit(ui::SwfMovie& source,float x,float y,bool& hit,std::string& error){
        if(!gameplay_hud||!gameplay_hud_bound){error="Selected authored gameplay HUD unavailable for hit testing";return false;}
        HudCharacterBinding binding{this,&gameplay_character_button};
        if(!source.action_script(&binding,bind_hud_character_button,error))return false;
        ui::AuthoredHudGeometryV1 geometry;
        if(!gameplay_hud->geometry(ui::AuthoredHudControlV1::character,x,y,geometry,error))return false;
        float logical[2]{x,y};
        if(!source.screen_to_logical(logical,error))return false;
        const float stage_x=logical[0]*20.f,stage_y=logical[1]*20.f;
        const bool bounds_hit=stage_x>=geometry.bounds[0]&&stage_x<=geometry.bounds[1]&&
                              stage_y>=geometry.bounds[2]&&stage_y<=geometry.bounds[3];
        ui::SwfClipInfo state;
        const auto path=gameplay_hud->control_path(ui::AuthoredHudControlV1::character);
        if(!source.clip(path.c_str(),state,error))return false;
        const bool visible=state.visible;
        hit=visible&&(geometry.hit||bounds_hit);
        if(x<600.f&&y<400.f)__android_log_print(ANDROID_LOG_INFO,tag,
            "Authored HUD character-menu geometry | style %d | screen %.1f %.1f | stage-twips %.1f %.1f | local-twips %.1f %.1f | bounds %.1f %.1f %.1f %.1f | shape %d | bounds-hit %d | visible %d | hit %d",
            hud_style,x,y,stage_x,stage_y,geometry.local[0],geometry.local[1],geometry.bounds[0],geometry.bounds[1],geometry.bounds[2],geometry.bounds[3],geometry.hit,bounds_hit,visible,hit);
        return true;
    }

    static bool orientation(void*,std::int32_t& out,std::string&){
        // The modern GLES owner draws in Android's already oriented surface.
        // Its explicit renderer orientation is 0; no aspect-derived enum.
        out=0;return true;
    }
    static bool dimensions(void* context,std::int32_t& width,std::int32_t& height,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(self.driver_width<=0||self.driver_height<=0){error="HUD surface dimensions unavailable";return false;}
        width=self.driver_width;height=self.driver_height;return true;
    }
    bool viewport(int width,int height,std::string& error){
        if(width<=0||height<=0){error="Invalid HUD surface dimensions";return false;}
        if(driver_width==width&&driver_height==height&&last_width==width&&last_height==height)return true;
        driver_width=width;driver_height=height;
        if(!movie->update_viewport(camera,error))return false;
        if(shared_menu_movie&&!shared_menu_movie->update_viewport(shared_camera,error))return false;
        if(character_menu_movie&&!character_menu_movie->update_viewport(character_camera,error))return false;
        last_width=width;last_height=height;report_frame=true;return true;
    }

    bool raw_asset(const char* uri,std::vector<std::uint8_t>& out,std::string& error) {
        std::unique_ptr<AAsset,AssetClose> asset(AAssetManager_open(manager,uri,AASSET_MODE_STREAMING));
        if(!asset){error=std::string("Required bundled UI input unavailable: ")+uri;return false;}
        const auto n=AAsset_getLength64(asset.get());
        if(n<=0||n>32*1024*1024){error="UI input length outside bounds";return false;}
        std::vector<std::uint8_t> candidate(static_cast<std::size_t>(n));std::size_t at=0;
        while(at<candidate.size()) {const int count=AAsset_read(asset.get(),candidate.data()+at,candidate.size()-at);
            if(count<=0){error="Short required UI input read";return false;}at+=static_cast<std::size_t>(count);}
        out=std::move(candidate);error.clear();return true;
    }
    bool original(const char* uri,bool& found,std::vector<std::uint8_t>& bytes,std::string& error) {
        if(assets.read(uri,bytes,error)){found=true;return true;}
        // An absent URI is an actual miss in this explicitly scoped APK
        // resource owner. Corruption/short reads are required delivery failures.
        if(error.rfind("Original UI resource unavailable: ",0)==0){found=false;error.clear();return true;}
        return false;
    }
    bool lease(std::vector<std::uint8_t> bytes,std::uintptr_t& id,std::string& error) {
        if(next_lease==std::numeric_limits<std::uintptr_t>::max()){error="UI resource lease exhausted";return false;}
        id=next_lease++;leases.emplace(id,std::move(bytes));return true;
    }
    bool debug_load(std::string& error) {
        if(!runtime.debug_load){error="Canonical DebugSwitches load provider unavailable";return false;}
        return runtime.debug_load(runtime.context,error);
    }
    bool debug_query(const char* key,std::string& error) {
        if(!runtime.debug_query){error="Canonical DebugSwitches query provider unavailable";return false;}
        return runtime.debug_query(runtime.context,key,error);
    }
    bool quest_journal_predicate(dh2::native::ui::QuestJournalPredicateV1& policy,
                                 std::string& error) {
        // Native GetQuestFunctor loads and reads these switches for each
        // journal query. Read through the one process-wide DebugSwitches
        // owner already used by the authored UI and gameplay callbacks.
        bool display_all_non_debug=false,display_debug_only=false;
        if(!model_renderer::menu_debug_load(manager,error)||
           !model_renderer::menu_debug_switch_value(manager,
                "DisplayAllQuestInLog",display_all_non_debug,error))return false;
        if(!display_all_non_debug&&
           !model_renderer::menu_debug_switch_value(manager,
                "DisplayAllDebugOnlyQuestInLog",display_debug_only,error))return false;
        std::uint32_t debug_priority=0;
        if(!constant(this,"v2QuestPriority","Debug",debug_priority,error))return false;
        policy={display_all_non_debug,display_debug_only,
                static_cast<std::int32_t>(debug_priority)};
        error.clear();return true;
    }
    static bool localization_debug(void* context,const char* key,std::string& error) {
        auto& self=*static_cast<Impl*>(context);return self.debug_load(error)&&self.debug_query(key,error);
    }
    static bool text_open(void* context,const char* uri,bool& found,std::vector<std::uint8_t>& bytes,
                          std::uintptr_t& id,std::string& error) {
        auto& self=*static_cast<Impl*>(context);id=0;
        const auto path=std::string("data/")+uri;
        if(!self.original(path.c_str(),found,bytes,error))return false;
        return !found||self.lease(bytes,id,error);
    }
    static bool text_close(void* context,std::uintptr_t id,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(self.leases.erase(id)!=1){error="Required UI lease close failed";return false;}return true;
    }
    static bool constant(void* context,const char* group,const char* key,std::uint32_t& value,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        // Immutable common-text, font and design constant views use the existing
        // parser over their retained input bytes. Later file wins, matching
        // the source group load order; no ScriptVM or mutable constants map.
        for(auto it=self.constants.rbegin();it!=self.constants.rend();++it){
            dh2_pycst_result result{};
            if(dh2_pycst_get(&*it,group,std::strlen(group),key,std::strlen(key),&result)!=0){error="Malformed retained UI constant view";return false;}
            if(result.found){value=static_cast<std::uint32_t>(result.value);return true;}
        }
        error=std::string("Required UI constant missing: ")+group+"."+key;return false;
    }
    static bool item_name_with_palette(Impl& self,const std::string& name,
                                       std::int32_t power_count,std::string& output,
                                       std::string& error) {
        if(power_count<0){error="Inventory item has a negative power count";return false;}
        static constexpr const char* power_keys[]={"zero","one","two","three","four"};
        std::uint32_t palette_index=2; // Original GetFontDef fallback for >=5 powers.
        if(power_count<5){
            std::uint32_t index_bits=0;
            if(!constant(&self,"ItemPowerColor",power_keys[power_count],index_bits,error))return false;
            std::int32_t signed_index=0;std::memcpy(&signed_index,&index_bits,sizeof(signed_index));
            if(signed_index<0){error="ItemPowerColor resolved to a negative FontPalette index";return false;}
            palette_index=static_cast<std::uint32_t>(signed_index);
        }
        std::uint32_t text_color=0;
        return ui::font_palette_text_color_at_index_v1(self.font_palette_bytes.data(),
                self.font_palette_bytes.size(),palette_index,text_color,error)&&
            ui::format_item_name_v1(name,text_color,output);
    }
    static bool player_character(void* context,std::uintptr_t& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.live_player){
            // The front renderer has no attached gameplay PlayerInfo; source
            // localization takes its genuine null-character branch.
            out=0;error.clear();return true;
        }
        return model_renderer::ui_player_identity(out,error);
    }
    static bool player_name(void* context,std::uintptr_t identity,std::string& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.live_player){error="Front menu has no Player Character name owner";return false;}
        return model_renderer::ui_player_name(identity,out,error);
    }
    ui::LocalizationServices text_services() {
        ui::LocalizationServices services{this,text_open,text_close,localization_debug,constant,player_character,player_name};
        if(front_screen=="main"){
            services.application_language=application_language;
            services.application_version=application_version;
        }
        return services;
    }
    static bool application_language(void* context,std::int32_t& value,std::string&){
        auto& self=*static_cast<Impl*>(context);value=self.settings?self.settings->language():self.localization.pack();return true;
    }
    static bool application_version(void*,std::string& value,std::string&){
        // Application::GetVersionString 0x31f6e0..0x31f708, ordinary Android
        // operator package (not special package 4), include-version=true.
        value="1.0.2";return true;
    }
    static bool refresh_settings_scene(void* context,ui::OwnedHudSettingsV1&,std::int32_t,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        // GSInit loads the persisted language before the intro movie and splash.
        // There are no authored menu actors to refresh until the main SWF is
        // constructed, so the loading-stage refresh is intentionally empty.
        if(self.front_screen=="loading"&&!self.live_player){error.clear();return true;}
        // This retained front session owns SWF menus and a static menu BDAE,
        // with no attached gameplay level, Character inventories or items.
        // A live gameplay attachment requires its real object traversal.
        if(self.front_screen!="main"||self.live_player){error="Settings language requires attached world localization traversal";return false;}
        const ui::SettingsSceneServices16V1 services{&self,[](void*,const ui::SettingsSceneRequest16V1*,std::uint32_t*)->int{
            // No inventory/item backend is manufactured for a future node.
            return 1;
        }};
        const int result=dh2_settings_v1_refresh_language_scene(&self.front_language_scene,&services);
        if(result){error="Front language actor traversal requires missing backend: "+std::to_string(result);return false;}
        error.clear();return true;
    }
    ui::SettingsLanguageServicesV1 settings_language(){return {this,refresh_settings_scene,nullptr,&localization};}
    void queue_volumes(){
        menu_audio.push_back("volume,"+std::to_string(settings->saved_option("VolumeMusic"))+","+std::to_string(settings->saved_option("VolumeFX")));
    }
    static bool load_settings(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.settings||!self.settings_files){error="Front settings owner unavailable";return false;}
        ui::SettingsLoadReceiptV1 receipt;const auto files=self.settings_files->services();auto language=self.settings_language();
        if(!self.settings->load(false,files,language,{},receipt,error))return false;
        // Application.GetDeviceLanguage is -1 in the original Android build;
        // actual front startup selects English for a fresh settings file.
        if(self.settings->language()==-1&&!self.settings->set_language(0,language,error))return false;
        self.queue_volumes();
        __android_log_print(ANDROID_LOG_INFO,tag,"Front settings loaded | found %d | options %zu | language %d | music %d | fx %d",receipt.found,self.settings->option_count(),self.settings->language(),self.settings->option("VolumeMusic"),self.settings->option("VolumeFX"));
        return true;
    }
    bool load_front_settings(std::string& error){
        if(settings&&settings_files&&settings->has_option("Language"))return true;
        std::array<std::vector<std::uint8_t>,3> metadata;
        const char* inputs[]={"design_pyarray.bin","design_pyarraynames.bin","design_pystructnames.bin"};
        for(unsigned i=0;i<3;++i)
            if(!raw_asset((std::string("original-cache/data/pydata/")+inputs[i]).c_str(),metadata[i],error))return false;
        // The renderer borrows the DesignSettings rows from this snapshot;
        // drop that view before the snapshot is replaced, then rebind it.
        model_renderer::bind_xp_design_settings(nullptr,0);
        option_table=ui::GameOptionTableV1();
        if(!option_table.load_design_cache({metadata[0].data(),metadata[0].size()},
            {metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
        {
            const auto design=option_table.borrow().design_settings_table();
            model_renderer::bind_xp_design_settings(design.data,design.size);
        }
        settings=std::make_unique<ui::OwnedHudSettingsV1>(option_table.borrow());
        settings_files=std::make_unique<ui::SettingsNativeFilesV1>(directory);
        return load_settings(this,error);
    }
    static bool save_settings(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);if(!self.settings||self.directory.empty()){error="Front settings save owner unavailable";return false;}
        const auto bytes=self.settings->serialized();const std::string path=self.directory+"/dh2_settings.savegame",temporary=path+".front.tmp";
        auto* file=std::fopen(temporary.c_str(),"wb");if(!file){error="Unable to open private settings output";return false;}
        const bool written=std::fwrite(bytes.data(),1,bytes.size(),file)==bytes.size();const bool closed=std::fclose(file)==0;
        if(!written||!closed){std::remove(temporary.c_str());error="Unable to write private settings output";return false;}
        // std::filesystem::rename replaces an existing target on Windows, where the C
        // rename fails once dh2_settings.savegame exists (every later settings save).
        std::error_code publish_error;
        std::filesystem::rename(temporary,path,publish_error);
        if(publish_error){std::remove(temporary.c_str());error="Unable to publish private settings output";return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Front settings saved | bytes %zu | music %d | fx %d | language %d",bytes.size(),self.settings->option("VolumeMusic"),self.settings->option("VolumeFX"),self.settings->language());return true;
    }
    static bool option_string(void* context,std::int32_t id,std::string& text,std::string& error){auto& self=*static_cast<Impl*>(context);return self.localization.string_id(std::uint32_t(id),self.text_services(),text,error);}
    static bool apply_option(void* context,const char* name,std::int32_t value,std::string& error){
        auto& self=*static_cast<Impl*>(context);if(!self.settings){error="Front settings unavailable";return false;}
        if(!std::strcmp(name,"Language")){
            const auto previous=self.language_selection<0?value:self.language_selection;
            if(value==6&&previous==3)value=4;else if(value==6&&previous==4)value=5;
            else if(value==3&&previous==6)value=5;else if(value==3&&previous==5)value=4;
            self.language_selection=value;
        }
        self.settings->set_option(name,value); // Original unknown key is ignored.
        (void)self.settings->saved_option("AutoOrientation"); // Android ResetOrientation is bx lr.
        if(!std::strcmp(name,"VolumeMusic")||!std::strcmp(name,"VolumeFX"))self.queue_volumes();
        __android_log_print(ANDROID_LOG_INFO,tag,"Front option changed | name %s | value %d",name,self.settings->option(name));return true;
    }
    static bool enter_options(void* context,std::string&){static_cast<Impl*>(context)->menu_audio.emplace_back("resume");return true;}
    static bool refresh_front_hud(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.live_player)return true;
        if(!self.gameplay_hud||!self.gameplay_hud_bound||!self.authored_joystick_ready){
            error="NativeRefreshHudManager requires the attached authored HUD and joystick owners";
            return false;
        }
        // IDA: NativeRefreshHudManager clears InfoHUDManager's active-touch
        // byte, then HUDControls::Touch clears its held-control state and
        // calls the current Player controller's Cmd_Stop. Reuse the attached
        // HUD/joystick and Player controller; do not construct another HUD.
        self.gameplay_hud_input_reset_pending=true;
        self.gameplay_hud_pointers.fill(-1);self.camera_fingers.reset();
        std::uintptr_t identity=0;
        if(!model_renderer::ui_player_identity(identity,error))return false;
        return ui::authored_joystick_release_v1(self.authored_joystick,identity!=0,
                                                self.authored_joystick_services(),error);
    }
    static bool input_behavior(void* context,std::int32_t slot,bool rollover,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        // The authored call sites use different stable renderer IDs: the
        // character-menu callbacks pass (rollover, 1), while dqhud passes
        // (rollover, 3). Route renderer 1 through the top gameplay state so
        // menu_Ingame remains on its owning HUD movie; renderer 3 is the HUD.
        auto* target=slot==0?self.shared_menu_movie.get():
            slot==1?(self.live_player&&!self.game_menu_stack.empty()?self.menu_movie(self.game_menu_stack.back()):self.movie.get()):
            slot==3&&self.live_player?self.movie.get():nullptr;
        if(!target){
            __android_log_print(ANDROID_LOG_WARN,tag,
                "Menu input renderer slot unconnected | slot %d | rollover %d | gameplay %d | menu depth %zu",
                slot,rollover,self.live_player,self.game_menu_stack.size());
            error="Menu input renderer slot unconnected";return false;
        }
        return target->menu_input_behavior(rollover?0x84:4,error);
    }
    ui::SwfMenuOptionServicesV1 option_services(){
        ui::SwfMenuOptionServicesV1 s;s.settings=settings.get();s.context=this;s.string_by_id=option_string;s.apply_option=apply_option;
        s.load=load_settings;s.save=save_settings;s.enter=enter_options;s.refresh_hud=refresh_front_hud;s.input_behavior=input_behavior;return s;
    }
    static int font_service(void* context,ui::FontResolveRequest40* request) {
        auto& self=*static_cast<Impl*>(context);std::string error;
        using Service=ui::FontResolveService;
        switch(request->kind) {
        case Service::debug_load:if(self.debug_load(error))return 0;break;
        case Service::debug_get_switch:if(self.debug_query(request->text,error))return 0;break;
        case Service::language:
            // Explicit English inspection selection, matching localization's
            // original constructor pack -1 -> English lookup branch.
            request->value=self.settings?self.settings->language():0;return 0;
        case Service::rewrite_path: {
            // Modern APK backing retains the exact logical original URI;
            // archive registration and FileManager rewrite policy are separate.
            const auto n=std::strlen(request->text);
            if(n<request->capacity){std::memcpy(request->buffer,request->text,n+1);return 0;}
            error="Original UI font URI exceeds capacity";break;
        }
        case Service::open_read: {
            bool found=false;std::vector<std::uint8_t> bytes;request->value=0;
            if(!self.original(request->text,found,bytes,error))break;
            if(!found||self.lease(std::move(bytes),request->value,error))return 0;break;
        }
        case Service::close_read:if(text_close(context,request->value,error))return 0;break;
        }
        self.provider_failure=error;return 1;
    }
    static bool font_read(void* context,const char* name,bool bold,bool italic,std::vector<std::uint8_t>& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);char uri[4096]{};
        ui::FontResolveInput24 input{name,"",bold?1u:0u,italic?1u:0u};
        ui::FontResolveOutput32 output{uri,sizeof(uri),0,0,0};
        ui::FontResolveServices16 services{context,font_service};
        const auto status=dh2_swf_font_resolve(&output,&input,&services);
        if(status||!output.found){error=self.provider_failure.empty()?std::string("Required original font unavailable: ")+name:self.provider_failure;return false;}
        if(std::strstr(uri,".fnt")){error="Required original GFNT provider connection unavailable";return false;}
        if(uri[0]=='#'){error=std::string("Required source system-font URI not yet connected: ")+uri;return false;}
        if(!self.assets.read(uri,out,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original UI font resolved | name %s | uri %s | bytes %zu",name,uri,out.size());return true;
    }
    static void font_diagnostic(void* context,const char* text) {
        auto& self=*static_cast<Impl*>(context);if(self.font_failure.empty())self.font_failure=text?text:"Required original font failed";
        __android_log_print(ANDROID_LOG_ERROR,tag,"Original UI font failed: %s",text?text:"");
    }
    static void bitmap_probe(void* context,const ui::HudBitmapInfo32& bitmap) {
        auto& self=*static_cast<Impl*>(context);
        if(bitmap.packed_conversion){++self.packed_glyphs;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original UI packed glyph decoded | code %u | size %d | mode %u | width %d | rows %d | pitch %d | grays %u",bitmap.code,bitmap.font_size,bitmap.pixel_mode,bitmap.width,bitmap.rows,bitmap.pitch,bitmap.num_grays);}
    }
    static bool movie_read(void* context,const char* uri,std::vector<std::uint8_t>& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);std::string path=uri;
        if(path.find('/')==std::string::npos)path="data/menus/"+path;
        if(!self.assets.read(path,out,error))return false;
        // Supplied Android SWF and splash atlas disagree for keyboard fills.
        // Preserve the byte-verified cache; use a documented compatibility movie.
        if(path=="data/menus/dqmenus_droid.swf") {
            if(!self.raw_asset("front-compat/dqmenus_droid.swf",out,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Keyboard atlas compatibility movie connected | seven repaired shapes");
        }
        return true;
    }
    static bool texture(void* context,const char* name,int,int,ui::SwfTexture& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);std::string uri;
        if(!scene::swf_texture_filename("",std::string("data/")+name,uri,error))return false;
        const auto found=self.exports.find(uri);if(found!=self.exports.end()){out=found->second;return true;}
        std::vector<std::uint8_t> encoded;if(!self.assets.read(uri,encoded,error))return false;
        textures::View view{};auto status=dh2_texture_open(encoded.data(),encoded.size(),&view);
        if(status!=textures::Error::ok){error=dh2_texture_error(status);return false;}
        std::vector<std::uint8_t> rgba(std::size_t(view.width)*view.height*4);
        status=dh2_texture_decode(&view,rgba.data(),rgba.size());
        if(status!=textures::Error::ok){error=dh2_texture_error(status);return false;}
        if(!self.gpu.image(view.width,view.height,4,rgba.data(),std::size_t(view.width)*4,out,error))return false;
        self.exports.emplace(uri,out);++self.bitmap_uploads;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original UI export delivered | name %s | uri %s | texture %d %d",name,uri.c_str(),out.width,out.height);return true;
    }
    static bool image(void* context,int w,int h,unsigned channels,const std::uint8_t* bytes,int pitch,ui::SwfTexture& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);if(pitch<0){error="Negative required UI image pitch";return false;}
        if(!self.gpu.image(w,h,channels,bytes,static_cast<std::size_t>(pitch),out,error))return false;
        if(channels==1)++self.glyph_uploads;else ++self.bitmap_uploads;return true;
    }
    static bool draw(void* context,const ui::SwfDraw& command,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.font_failure.empty()){error=self.font_failure;return false;}
        if(command.kind==ui::SwfDraw::triangle_strip)++self.strips;
        if(self.front_screen=="loading"&&!self.loading_bitmap_reported&&
           command.fill.kind==ui::SwfFill::bitmap&&command.xy.size()>=2){
            self.loading_bitmap_reported=true;
            const auto& m=command.fill.uv.value;
            const float x=command.xy[0],y=command.xy[1];
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Original loading bitmap draw | texture %d %d | matrix %.8f %.8f %.8f %.8f %.8f %.8f | vertex %.3f %.3f | texture pixel %.3f %.3f",
                command.fill.texture.width,command.fill.texture.height,m[0],m[1],m[2],m[3],m[4],m[5],
                x,y,m[0]*x+m[1]*y+m[2],m[3]*x+m[4]*y+m[5]);
        }
        if(command.kind==ui::SwfDraw::line_strip)++self.lines;
        if(command.kind==ui::SwfDraw::mask_begin)++self.masks;
        return self.gpu.draw(command,error);
    }
    static bool stencil(void* context,const float bounds[4],std::uint8_t pattern,bool& out,std::string& error) {
        return static_cast<Impl*>(context)->gpu.stencil(bounds,pattern,out,error);
    }
    static bool native(void* context,const char* name,const std::vector<ui::SwfValue>& args,ui::SwfValue& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(std::strcmp(name,"NativeGetStringFromSymbol")||args.size()!=1||args[0].kind!=ui::SwfValue::text){error="Required native UI function/arguments unsupported";return false;}
        ui::LocalizationResult result;
        if(!self.localization.native_string(args[0].string,self.text_services(),result,error))return false;
        if(result.sets_menu_string_flag)self.menu_string_flag=true;
        out.kind=ui::SwfValue::text;out.string=result.text;++self.string_calls;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original UI string delivered | symbol %s | found %d | text %s",args[0].string.c_str(),result.found,result.text.c_str());return true;
    }
    static void diagnostic(void* context,bool error,const char* text) {
        auto& self=*static_cast<Impl*>(context);if(error)++self.core_errors;
        // Filter only repeated Android log transport. The six authored trace
        // calls (Equipment occurs twice) still execute in the retained AS VM.
        // Errors and every other diagnostic are delivered unchanged.
        if(!error&&text){
            const int i=hardcoded_menu_label(text);
            if(i>=0){
                if(self.reported_hardcoded_labels[i])return;
                self.reported_hardcoded_labels[i]=true;
            }
        }
        __android_log_print(error?ANDROID_LOG_WARN:ANDROID_LOG_INFO,tag,"Original SWF core diagnostic: %s",text?text:"");
    }
    static bool menu_slot_exists(void* context,std::uint32_t slot,bool& occupied,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return dh2::data::campaign_profile_exists_v1(self.directory,slot,occupied,error);
    }
    static bool menu_store_difficulty(void* context,std::int32_t value,std::string& error){
        static_cast<Impl*>(context)->menu_selected_difficulty=value;error.clear();return true;
    }
    static bool menu_slot_string(void* context,std::uint32_t id,std::string& text,std::string& error){
        auto& self=*static_cast<Impl*>(context);return self.localization.string_id(id,self.text_services(),text,error);
    }
    static bool menu_slot_date(void*,std::uint32_t raw,std::tm& calendar,std::string& error){return ui::menu_save_slot_local_date_v1(raw,calendar,error);}
    static bool menu_slot_constant(void* context,const char* group,const char* key,std::int32_t& value,std::string& error){
        std::uint32_t raw{};if(!constant(context,group,key,raw,error))return false;
        std::memcpy(&value,&raw,sizeof(value));return true;
    }
    bool ensure_menu_quest_resources(std::string& error){
        if(menu_quest_tables.borrow()&&menu_quest_constants.view.bytes){error.clear();return true;}
        auto records=std::make_shared<std::vector<std::uint8_t>>();
        auto names=std::make_shared<std::vector<std::uint8_t>>();
        auto constants_bytes=std::make_shared<std::vector<std::uint8_t>>();
        if(!raw_asset("data/v2quests_pyarray.bin",*records,error)||
           !raw_asset("data/v2quests_pyarraynames.bin",*names,error)||
           !raw_asset("data/v2quests_pycst.bin",*constants_bytes,error))return false;
        dh2::data::quest_table_bindings_v1::Input input;
        if(dh2_quests_open(&input.table,records->data(),std::uint32_t(records->size()))){
            error="Occupied-slot quest catalogue decode failed";return false;
        }
        input.packed_owner=records;input.names=names->data();input.names_size=names->size();input.names_owner=names;
        if(!menu_quest_tables.load(input,error))return false;
        dh2::native::quests::Constants constants;
        if(dh2_pycst_open(&constants.view,constants_bytes->data(),
                          std::uint32_t(constants_bytes->size()))){
            error="Occupied-slot quest constants decode failed";return false;
        }
        constants.owner=constants_bytes;
        menu_quest_records=std::move(records);menu_quest_names=std::move(names);
        menu_quest_constants_bytes=std::move(constants_bytes);menu_quest_constants=std::move(constants);
        error.clear();return true;
    }
    static bool menu_load_quest_acts(void* context,
        const std::shared_ptr<dh2::data::PlayerSavegameV1>& save,
        const dh2::data::PlayerProfileIndexV1::Borrow& profile,
        std::array<std::int32_t,3>& regular,std::array<std::int32_t,3>& volatile_acts,
        std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!save||!profile||!profile.section("QEST")){
            error="Occupied-slot QEST provider requires the exact indexed metadata Save";return false;
        }
        if(!self.ensure_menu_quest_resources(error))return false;
        dh2::native::quests::Owner owner(save,self.menu_quest_tables.borrow(),
                                        self.menu_quest_constants);
        if(!owner.initialize(0,error)||!owner.initialize(1,error))return false;
        dh2::native::quests::Cursor cursor(profile,"QEST");
        if(!owner.load_quests(cursor,error))return false;
        const auto& fields=save->level_name_fields();
        regular=fields.quest_wordfc;volatile_acts=fields.quest_word15c;
        error.clear();return true;
    }
    static bool menu_slot_details(void* context,std::uint32_t slot,bool occupied,std::int32_t difficulty,
        ui::SwfFrontSaveSlotDetailsV1& details,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!occupied){details={};error.clear();return true;}
        dh2::data::CampaignProfileFileV1 file;
        if(!dh2::data::read_campaign_profile_v1(self.directory,slot,file,error))return false;
        dh2::data::MenuProfileMetadataV1 metadata;
        // The canonical short-lived Quest owner reads QEST through this
        // metadata transaction's same Save and whole-profile cursor.
        dh2::data::MenuProfileMetadataServicesV1 load_services{
            &self,menu_store_difficulty,menu_load_quest_acts};
        if(!dh2::data::load_menu_profile_metadata_v1({file.bytes.data(),file.bytes.size()},self.menu_characters,
            static_cast<std::int32_t>(slot),self.menu_selected_difficulty,load_services,metadata,error))return false;
        ui::MenuSaveSlotPresentationServicesV1 display_services{&self,menu_slot_constant,menu_slot_string,menu_slot_date};
        // This retained main front is offline. Online gameplay must supply its
        // actual regular/volatile choice through the agreed typed handoff.
        const auto language=self.settings?self.settings->language():0;
        if(!ui::project_menu_save_slot_v1(metadata,self.menu_characters,self.menu_levels,difficulty,false,
            static_cast<std::uint32_t>(language),display_services,details,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original occupied save-slot presentation | slot %u | backup %d | class %s | level %d | location %s",slot,file.origin==dh2::data::CampaignProfileOriginV1::backup,details.player_class.c_str(),details.player_level,details.player_location.c_str());
        return true;
    }
    struct ParseContext {Impl& self;std::deque<std::string> retained;};
    static bool parsed_text(void* context,const ui::HudTextRequestV1& request,ui::HudTextResponseV1& reply,std::string& error){
        auto& parsed=*static_cast<ParseContext*>(context);auto& self=parsed.self;std::string value;
        switch(request.operation){
        case ui::hud_text_constant_v1:{std::uint32_t raw=0;if(!constant(&self,request.group,request.key,raw,error))return false;std::memcpy(&reply.value,&raw,sizeof(raw));return true;}
        case ui::hud_text_integer_string_v1:
            if(!self.localization.string_id(static_cast<std::uint32_t>(request.value),self.text_services(),value,error))return false;
            break;
        case ui::hud_text_pack_v1:reply.value=self.localization.pack();return true;
        case ui::hud_text_version_v1:
            if(!application_version(&self,value,error))return false;
            break;
        case ui::hud_text_title_v1:{
            std::int32_t language=0;std::uint32_t id=0;
            if(!application_language(&self,language,error))return false;
            if(language==4){error="Application Japanese title provider unavailable";return false;}
            if(!constant(&self,"StrID","MENU_GAME_TITLE",id,error)||!self.localization.string_id(id,self.text_services(),value,error))return false;
            break;
        }
        default:error="Unknown source parseEx provider operation";return false;
        }
        if((request.operation==ui::hud_text_title_v1||request.operation==ui::hud_text_version_v1)&&value.size()>=request.limit){error="Application source C-string limit exceeded";return false;}
        parsed.retained.push_back(std::move(value));reply.text=parsed.retained.back().c_str();return true;
    }
    static bool format_integer_text(Impl& self,const char* pattern,std::int32_t value,
        std::string& output,std::string& error){
        ParseContext parse{self,{}};
        const ui::HudTextServicesV1 services{&parse,parsed_text};
        const ui::HudTextVariantV1 argument{static_cast<float>(value),value,nullptr};
        bool changed=false;
        return ui::hud_text_parse_ex_v1(pattern,&argument,1,services,output,changed,error);
    }
    static std::int32_t skill_fixed_integer(std::int32_t raw){
        const auto wide=std::int64_t(raw);
        return wide>=0?std::int32_t(wide/256):
            -std::int32_t((-wide+255)/256);
    }
    static bool format_skill_level_text(Impl& self,const std::string& pattern,
        const std::vector<std::int32_t>& fixed_properties,std::string& output,
        std::string& error){
        std::vector<ui::HudTextVariantV1> arguments;
        arguments.reserve(fixed_properties.size());
        for(const auto fixed:fixed_properties){
            arguments.push_back({static_cast<float>(fixed)*(1.0f/256.0f),
                skill_fixed_integer(fixed),nullptr});
        }
        ParseContext parse{self,{}};
        const ui::HudTextServicesV1 services{&parse,parsed_text};
        bool changed=false;std::string formatted;
        if(!ui::hud_text_parse_ex_v1(pattern.c_str(),arguments.data(),
                arguments.size(),services,formatted,changed,error))return false;
        output=std::move(formatted);return true;
    }
    static const dh2::data::Item* item_text_metadata(void* context,const dh2::data::ItemInstanceV1& instance,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.item_text_items){error="Original Item table is not bound to the retained text owner";return nullptr;}
        const auto* row=dh2::data::item(*self.item_text_items,instance.id);
        if(!row)error="Original Item ID absent from the retained Item table";
        return row;
    }
    static bool item_text_invoke(void* context,dh2::data::ItemInstanceV1&,
        const dh2::data::ItemTextRequestV5& request,dh2::data::ItemTextResponseV5& reply,
        std::string& output,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.ensure_localization_ready(error))return false;
        using Op=dh2::data::ItemTextOperationV5;
        if(request.operation==Op::constant){
            std::uint32_t raw=0;
            if(!constant(&self,request.group,request.key,raw,error))return false;
            std::memcpy(&reply.value,&raw,sizeof(raw));return true;
        }
        if(request.operation==Op::integer_string){
            return self.localization.string_id(static_cast<std::uint32_t>(request.value),
                self.text_services(),reply.text,error);
        }
        if(request.operation==Op::class_name){
            if(!self.item_text_characters||request.value<0||
                std::size_t(request.value)>=self.item_text_characters->rows.size()){
                error="Source item requirement Character row unavailable";return false;
            }
            reply.value=self.item_text_characters->rows[std::size_t(request.value)][5];return true;
        }
        ParseContext parse{self,{}};
        const ui::HudTextServicesV1 services{&parse,parsed_text};
        if(request.operation==Op::parse_varargs){
            bool changed=false;
            return ui::item_text_varargs_v5(request.input,request.arguments,request.count,
                services,output,changed,error);
        }
        if(request.operation==Op::parse_ex){
            if(request.count>65536||(request.count&&!request.arguments)){
                error="Malformed Item parseEx argument span";return false;
            }
            std::vector<ui::HudTextVariantV1> values;values.reserve(request.count);
            for(std::uint32_t i=0;i<request.count;++i){
                const auto& value=request.arguments[i];
                values.push_back({value.number,value.integer,value.text});
            }
            bool changed=false;
            return ui::hud_text_parse_ex_v1(request.input,values.data(),values.size(),
                services,output,changed,error);
        }
        error="Unsupported source Item text operation";return false;
    }
    static bool start_integer(void* context,double value,std::int32_t& result,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.runtime.eabi_integer){error="Source EABI integer provider unavailable";return false;}
        return self.runtime.eabi_integer(self.runtime.context,value,result,error);
    }
    static bool start_request(void* context,bool numeric,std::int32_t difficulty,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.runtime.request_start_game){error="NativeStartGame continuation unavailable";return false;}
        std::int32_t slot=-1;
        if(!self.runtime.request_start_game(self.runtime.context,numeric,difficulty,slot,error))return false;
        // NativeStartGame has a source state-38 early return. The renderer
        // reports it as a successful no-op with no assigned launch slot.
        if(slot<0)return true;
        self.launch_requests.push_back({slot,numeric,difficulty,
            NativeStartGameIntentV1::Kind::start_game,{},0.0});
        __android_log_print(ANDROID_LOG_INFO,tag,"Authored NativeStartGame continuation queued | assigned PlayerInfo slot %d | numeric difficulty %d | requested difficulty %d | deferred until SWF dispatch unwinds",slot,numeric,difficulty);
        return true;
    }
    static bool continue_resolve_slot(void* context,std::int32_t& slot,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.runtime.request_continue_game){error="FS_ContinueGame last-slot provider unavailable";return false;}
        return self.runtime.request_continue_game(self.runtime.context,slot,error);
    }
    static bool continue_queue_resume(void* context,std::int32_t slot,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(slot<0){error="FS_ContinueGame returned an invalid last slot";return false;}
        self.launch_requests.push_back({slot,false,0,
            NativeStartGameIntentV1::Kind::start_game,{},0.0});
        __android_log_print(ANDROID_LOG_INFO,tag,
            "Authored FS_ContinueGame queued | canonical last slot %d | saved level/difficulty resume deferred until SWF dispatch unwinds",slot);
        error.clear();return true;
    }
    static bool continue_fallback_start_game(void* context,const char*,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        // Source FS_ContinueGame dispatches its StartGame menu callback when
        // SavegameManager+8 is -1. Reuse the already-authored menu state.
        return push_menu(&self,"menu_StartGame",error);
    }
    static bool stop_source_dialog(Impl& self,std::string& error){
        if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
           !self.movie){
            error="Source StopDialog requires the retained gameplay HUD session";
            return false;
        }
        bool start_next=false;
        if(!self.source_dialog_messages.stop(start_next)){
            error.clear();return true;
        }
        // MenuBase::FS_StopDialog 0x427b08 / SkipEnqueuedMessage 0x442c1c invoke
        // StartDialog at once when another DialogMsg remains. Here the pop is
        // reached from inside the authored SWF's own dispatch (the DialogBox hide
        // animation's NativeStopMessage, the StopDialog FSCommand), where a nested
        // action_script is refused as busy and the next message was never shown.
        // The callback is deferred until that dispatch has unwound.
        if(start_next)self.dialog_start_pending=true;
        error.clear();return true;
    }
    static bool flush_pending_dialog_start(Impl& self,std::string& error){
        error.clear();
        if(self.status_start_pending){
            self.status_start_pending=false;
            if(self.source_status_messages.active()&&self.gameplay_hud&&self.gameplay_hud_bound&&
               !self.gameplay_hud->notify_status_message(error))return false;
            error.clear();
        }
        if(!self.dialog_start_pending)return true;
        self.dialog_start_pending=false;
        if(!self.source_dialog_messages.active())return true;
        if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
           !self.movie){
            error="Queued DialogMsg has no retained gameplay HUD movie";return false;
        }
        return self.movie->action_script(&self,invoke_source_dialog_start,error);
    }
    struct DialogBoxProbe {bool tappable{};};
    // The source DialogBox is a root-level clip above the HUD (depth 365 against
    // menu_HUD_n at 17) whose full-screen btn_next catches every tap while it is
    // visible: RenderFX hits the whole retained root. This port binds the HUD
    // input to menu_HUD_0 alone, so the dialog never saw a tap and its script
    // (LockCharacter "All" + WaitDialog) kept the Player locked forever.
    static bool dialog_box_tappable(Impl& self,bool& tappable,std::string& error){
        DialogBoxProbe probe;
        if(!self.movie->action_script(&probe,
            [](void* raw,ui::SwfAsGraph& graph,std::string& nested){
                auto& result=*static_cast<DialogBoxProbe*>(raw);
                ui::SwfAsValue root,box,button,visible;bool found=false,flag=false;
                if(!graph.root_value(root,nested)||
                   !graph.find_target(root,"_root.DialogBox",box,nested))return false;
                if(!box.identity()){nested.clear();return true;}
                if(!graph.get_member(box,"_visible",visible,found,nested))return false;
                if(!found||!graph.to_boolean(visible,flag,nested))return false;
                if(!flag){nested.clear();return true;}
                if(!graph.find_target(root,"_root.DialogBox.btn_next",button,nested))return false;
                if(!button.identity()){nested.clear();return true;}
                found=false;flag=false;
                if(!graph.get_member(button,"_visible",visible,found,nested))return false;
                if(!found||!graph.to_boolean(visible,flag,nested))return false;
                result.tappable=flag;nested.clear();return true;
            },error))return false;
        tappable=probe.tappable;return true;
    }
    static bool dialog_box_release(Impl& self,std::string& error){
        return self.movie->action_script(&self,
            [](void*,ui::SwfAsGraph& graph,std::string& nested){
                ui::SwfAsValue root,button,result;bool callable=false;
                if(!graph.root_value(root,nested)||
                   !graph.find_target(root,"_root.DialogBox.btn_next",button,nested))return false;
                if(!button.identity()){nested.clear();return true;}
                // btn_next.onRelease: isSkipable ? hideDialog() + sfx_tap_dialog.
                if(!graph.invoke(button,button,"onRelease",{},result,callable,nested))return false;
                nested.clear();return true;
            },error);
    }
    // Sets consumed when the touch belongs to the DialogBox (the HUD never sees it).
    static bool dialog_touch(Impl& self,int action,bool& consumed,std::string& error){
        consumed=false;error.clear();
        if(action==0){
            // A new press never inherits ownership from a release that was lost.
            self.dialog_touch_owned=false;
            // Visibility decides, not the queue: FlushMessages empties the queue but
            // leaves the box on screen to be tapped away, exactly like the source.
            if(self.gameplay_hud_pointers[0]>=0||!self.movie)return true;
            bool tappable=false;
            if(!dialog_box_tappable(self,tappable,error))return false;
            if(!tappable)return true;
            self.dialog_touch_owned=true;self.gameplay_ui_touch_owned[0]=true;consumed=true;
            return true;
        }
        if(!self.dialog_touch_owned)return true;
        consumed=true;
        if(action==2)return true;
        self.dialog_touch_owned=false;
        if(action==3)return true;
        return dialog_box_release(self,error);
    }
    static void fscommand_handler(gameswf::character*,const char* command,const char* argument){
        auto* self=active_fscommand_owner;
        if(self&&command&&!std::strcmp(command,"StopDialog")&&self->selected&&
           self->front_screen.empty()&&self->live_player&&self->loaded&&self->movie&&
           self->input_dispatch_movie==self->movie.get()){
            std::string error;
            if(!stop_source_dialog(*self,error))
                __android_log_print(ANDROID_LOG_ERROR,tag,"Authored StopDialog failed: %s",
                    error.empty()?"no diagnostic":error.c_str());
            return;
        }
        if(!self||!command||std::strcmp(command,"ContinueGame")||
           !self->selected||self->front_screen!="main"||!self->input_dispatch_movie||
           self->input_dispatch_movie!=self->active_menu_movie())return;
        const ui::FsContinueGameServicesV1 services{
            self,continue_resolve_slot,continue_queue_resume,continue_fallback_start_game};
        std::string error;
        if(!ui::fscommand_continue_game_v1(command,argument,services,error))
            __android_log_print(ANDROID_LOG_ERROR,tag,"Authored FS_ContinueGame failed: %s",
                error.empty()?"no diagnostic":error.c_str());
    }
    static bool invoke_source_dialog_start(void* raw,ui::SwfAsGraph& graph,std::string& error){
        auto& self=*static_cast<Impl*>(raw);
        if(!self.movie||!self.live_player||!self.loaded||!self.front_screen.empty()){
            error="Source StartDialog requires the retained gameplay HUD movie";return false;
        }
        ui::SwfAsValue root,result;bool callable=false;
        if(!graph.root_value(root,error))return false;
        const std::vector<ui::SwfAsValue> args{ui::SwfAsValue::number(0)};
        if(!graph.invoke(root,root,"StartDialog",args,result,callable,error))return false;
        if(!callable){error="Retained gameplay SWF root has no StartDialog callback";return false;}
        error.clear();return true;
    }
    struct CharacterReloadMenuLookup {Impl* self;std::uintptr_t identity{};};
    struct CharacterReloadPrompt {Impl* self;std::uintptr_t renderer,subject;
        const char* path;const char* method;bool argument;};
    static bool reload_current_menu_fx(void* raw,ui::SwfAsGraph& graph,std::string& error){
        auto& lookup=*static_cast<CharacterReloadMenuLookup*>(raw);
        if(!lookup.self||!lookup.self->live_player||!lookup.self->character_menu_movie){
            error="Current gameplay CharacterMenu movie/renderer is unavailable";return false;
        }
        ui::SwfAsValue root,menu;
        if(!graph.root_value(root,error)||
           !graph.find_target(root,"_root.menu_CharacterMenu",menu,error)||!menu.identity()){
            if(error.empty())error="Current CharacterMenu SWF target is absent";
            return false;
        }
        // The retained AS clip is the actual current menu render target in this
        // SWF facade; return its live graph identity without inventing another
        // renderer or caching a replacement object.
        lookup.identity=menu.identity();error.clear();return true;
    }
    static bool reload_invoke_spec_prompt(void* raw,ui::SwfAsGraph& graph,std::string& error){
        auto& prompt=*static_cast<CharacterReloadPrompt*>(raw);
        if(!prompt.self||!prompt.self->live_player||!prompt.self->character_menu_movie||
           !prompt.renderer||prompt.renderer!=prompt.subject||!prompt.path||
           std::strcmp(prompt.path,"_root.menu_CharacterMenu")||!prompt.method||
           std::strcmp(prompt.method,"IsSpecTime")){
            error="IsSpecTime requires the current CharacterMenu RenderFX identity and source path";return false;
        }
        ui::SwfAsValue root,menu,result;bool callable=false;
        if(!graph.root_value(root,error)||!graph.find_target(root,prompt.path,menu,error)||
           !menu.identity()||menu.identity()!=prompt.renderer||
           !graph.invoke(menu,menu,prompt.method,{ui::SwfAsValue::boolean(prompt.argument)},
                         result,callable,error))return false;
        // Character::ReloadSkills calls the void RenderFX::InvokeASCallback
        // wrapper and discards its AS result. That wrapper also silently
        // accepts a missing RenderFX::Find target, so an absent optional
        // IsSpecTime method must not reject the required skill reload prefix.
        error.clear();return true;
    }
    static int character_reload_menu_service(void* raw,std::uint32_t service,
        std::uint32_t argument,std::uintptr_t subject,const char* path,
        const char* callback,std::uintptr_t& identity,std::int32_t& value,
        std::string& error){
        auto* self=static_cast<Impl*>(raw);identity=0;value=0;
        if(!self||!self->character_menu_movie){error="Current CharacterMenu movie is unavailable";return -1;}
        if(service==ui::reload_menu_fx_v1){
            CharacterReloadMenuLookup lookup{self,0};
            if(!self->character_menu_movie->menu_action_script(&lookup,reload_current_menu_fx,error))return -1;
            identity=lookup.identity;return identity?0:-1;
        }
        if(service==ui::reload_spec_prompt_v1){
            CharacterReloadPrompt prompt{self,0,subject,path,callback,argument!=0};
            // Re-resolve the current clip inside the same movie scope and
            // require it to still be the RenderFX identity read immediately
            // before the source Invoke call.
            if(!self->character_menu_movie->menu_action_script(&prompt,
                [](void* context,ui::SwfAsGraph& graph,std::string& nested_error){
                    auto& call=*static_cast<CharacterReloadPrompt*>(context);
                    CharacterReloadMenuLookup lookup{call.self,0};
                    if(!reload_current_menu_fx(&lookup,graph,nested_error))return false;
                    call.renderer=lookup.identity;
                    return reload_invoke_spec_prompt(context,graph,nested_error);
                },error))return -1;
            return 0;
        }
        error="Unexpected non-menu service sent to CharacterMenu renderer";return -1;
    }
    static bool native_action(void* context,const char* name,const gameswf::fn_call& fn,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        auto local_character=[&](std::int32_t index,bool remote,std::uintptr_t& identity)->bool{
            identity=0;
            if(index!=0||remote)return true; // Other/remote PlayerInfo owners are not attached to this level.
            return model_renderer::ui_player_identity(identity,error);
        };
        auto faery_character=[&](std::int32_t source_index,std::uintptr_t& identity)->bool{
            // The shipped Faery SWF passes NativeGetPlayerChar slot 2 for its
            // single-player callbacks. This port binds one canonical Player;
            // adapt that source UI slot to the attached identity without
            // creating a second Save, skill VM, or Faery owner.
            if(source_index==2)return model_renderer::ui_player_identity(identity,error);
            return local_character(source_index,false,identity);
        };
        const ui::SwfMenuLaunchServicesV1 launch{self.runtime.context,self.runtime.create_save_slot,
            self.runtime.assign_save_slot,self.runtime.eabi_integer,self.runtime.change_preview_slot};
        if(!std::strcmp(name,"NativeLockZoom")||!std::strcmp(name,"NativeUnlockZoom")){
            // IDA: these authored no-argument callbacks call NativeSetLockZoom
            // with 1/0, writing CameraLevel +134 through the active ZoomHandler.
            if(!self.runtime.camera_zoom_lock){error="Native camera zoom-lock service unavailable";return false;}
            self.runtime.camera_zoom_lock(self.runtime.context,
                !std::strcmp(name,"NativeLockZoom"));
            error.clear();return true; // renderer preserves source no-op without an active camera
        }
        if(!std::strcmp(name,"NativeResetMapZoom")){
            // IDA 0x43aa84 calls MenuCharMenu_Map::ResetZoom (0x45387c),
            // which resets the map CameraLevel current/target pair to 0/1.
            return model_renderer::map_camera_reset_zoom(error);
        }
        if(!std::strcmp(name,"NativeDoWeHaveInternet")){
            // This build is intentionally offline. The authored Achievements
            // modal still works when its connectivity gate receives false.
            if(fn.result)fn.result->set_bool(false);
            return true;
        }
        if(!std::strcmp(name,"NativeSaveGame")){
            dh2::ui::SkillSaveCallbackResultV1 saved{};
            const auto to_integer=[&](std::size_t index,std::int32_t& value,
                                      std::string& failure){
                return start_integer(&self,fn.arg(static_cast<int>(index)).to_number(),
                                     value,failure);
            };
            const auto resolve_player=[&](std::int32_t index,
                    std::uintptr_t& identity,std::string& failure){
                if(!local_character(index,false,identity)){
                    failure=error;return false;
                }
                return true;
            };
            const auto save=[&](std::uintptr_t identity,std::string& failure){
                if(!self.runtime.save_game){
                    failure="NativeSaveGame transport is unavailable";return false;
                }
                return self.runtime.save_game(self.runtime.context,identity,failure);
            };
            const auto save_trophy_tail=[](std::uintptr_t identity,std::string& failure){
                return model_renderer::ui_player_save_trophy_side_effects(identity,failure);
            };
            return dh2::ui::apply_skill_save_callback_v1(
                std::size_t(fn.nargs),to_integer,resolve_player,save,
                save_trophy_tail,saved,error);
        }
        if(!std::strcmp(name,"NativeCreateSaveSlot"))return ui::swf_menu_create_save_slot_v1(fn,launch,error);
        if(!std::strcmp(name,"NativeAssignSaveSlotToPlayer"))return ui::swf_menu_assign_save_slot_v1(fn,launch,error);
        if(!std::strcmp(name,"NativeSetSaveSlotIDToMainMenu"))return ui::swf_menu_preview_save_slot_v1(fn,launch,error);
        if(!std::strcmp(name,"NativeGetSaveSlotDetails")){
            const ui::SwfFrontSaveSlotServicesV1 slots{&self,menu_slot_exists,menu_slot_details};
            return ui::swf_front_save_slot_details(fn,slots,error);
        }
        if(!std::strcmp(name,"NativeStartGame")){
            // IDA NativeStartGame checks current Level state 38 before reading
            // argc or converting the optional difficulty. The retained native
            // world is the port's single current-Level owner; a front-menu
            // launch has already retired it.
            if(model_renderer::source_start_game_blocked_by_current_world())return true;
            ui::SwfMenuLaunchServicesV1 start;
            start.context=&self;start.eabi_integer=start_integer;start.request_start_game=start_request;
            return ui::swf_menu_start_game_continuation_v1(fn,start,error);
        }
        if(!std::strcmp(name,"NativeGetPossibleClassSpec")){
            if(fn.nargs!=1||!fn.arg(0).is_object())return true;
            std::uintptr_t identity=0;if(!local_character(0,false,identity))return false;
            if(!identity)return true;
            std::array<std::int32_t,4> text_ids{};
            if(!model_renderer::ui_player_class_specialization_text_ids(identity,text_ids,error))return false;
            auto* object=fn.arg(0).to_object();
            if(!object)return true;
            std::array<std::string,4> text;
            for(std::size_t i=0;i<text_ids.size();++i)
                if(!self.localization.string_id(std::uint32_t(text_ids[i]),
                    self.text_services(),text[i],error))return false;
            if(!object->set_member("Class1Name",gameswf::as_value(text[0].c_str()))||
               !object->set_member("Class1Desc",gameswf::as_value(text[1].c_str()))||
               !object->set_member("Class2Name",gameswf::as_value(text[2].c_str()))||
               !object->set_member("Class2Desc",gameswf::as_value(text[3].c_str()))){
                error="NativeGetPossibleClassSpec object rejected source text IDs";return false;
            }
            if(fn.result)fn.result->set_as_object(object);
            return true;
        }
        if(!std::strcmp(name,"NativeSelectClassSpec")){
            // The authored specialization screen supplies the selected local
            // branch as one numeric selector. Apply it through the current
            // Player/Save/skill owners; never construct a parallel profile.
            if(fn.nargs!=1||!fn.arg(0).is_number())return true;
            std::int32_t selector=0;
            if(!start_integer(&self,fn.arg(0).to_number(),selector,error))return false;
            if(selector<0||selector>1)return true;
            std::uintptr_t identity=0;
            if(!local_character(0,false,identity))return false;
            if(!identity)return true;
            return model_renderer::ui_player_select_class_specialization(
                identity,selector,error);
        }
        if(!std::strcmp(name,"NativeTouchToMove")){
            // The original callback is an exact empty function (IDA: bx lr).
            return true;
        }
        if(!std::strcmp(name,"NativeHUDSkill")){
            // NativeHUDSkill is called by the authored HUD on release with
            // its three-slot index. Resolve and execute on the same retained
            // Character/AI/VM/playback owners; the renderer preserves the
            // source BeginSkill -> EndSkill order and authored do_skill event.
            if(fn.nargs!=1||!fn.arg(0).is_number())return true;
            std::int32_t slot=0;
            if(!start_integer(&self,fn.arg(0).to_number(),slot,error))return false;
            std::uintptr_t identity=0;
            if(!local_character(0,false,identity))return false;
            if(!identity)return true;
            // Both source skill checks can change before the ordinary HUD
            // polling interval. Recheck usability on the next update after a
            // successful action, but keep the cached result if dispatch fails.
            return ui::run_hud_action_and_refresh_usable_v1(
                self.gameplay_hud_usable_refresh,[&]{
                    return model_renderer::ui_player_use_skill_slot(identity,slot,error);
                });
        }
        if(!std::strcmp(name,"NativeHUDSpell")){
            // NativeHUDSpell has no SWF arguments. Keep source CTRLIsAllowed
            // and controller-command gates in the retained Player runtime.
            if(fn.nargs!=0)return true;
            std::uintptr_t identity=0;
            if(!local_character(0,false,identity))return false;
            if(!identity)return true;
            return ui::run_hud_action_and_refresh_usable_v1(
                self.gameplay_hud_usable_refresh,[&]{
                    return model_renderer::ui_player_use_spell(identity,error);
                });
        }
        if(!std::strcmp(name,"NativeGetParsedString")){
            ParseContext parse{self,{}};const ui::HudTextServicesV1 services{&parse,parsed_text};
            return ui::swf_menu_parsed_string_v1(fn,self.localization,self.text_services(),services,launch,error);
        }
        if(!std::strcmp(name,"NativeReloadSkills")){
            // Preserve the source wrapper's default-index behavior. Exactly
            // one argument is converted through the host's EABI boundary;
            // other arities select PlayerInfo index zero without reading args.
            std::int32_t index=0;
            if(fn.nargs==1&&!start_integer(&self,fn.arg(0).to_number(),index,error))return false;
            std::uintptr_t identity=0;
            if(!local_character(index,false,identity))return false;
            if(!identity)return true;
            model_renderer::UiPlayerReloadResultV1 result{};
            if(!model_renderer::ui_player_reload_skills(identity,&self,
                    character_reload_menu_service,result,error)){
                error="NativeReloadSkills failed at source phase "+std::to_string(result.phase)+
                    " after "+std::to_string(result.calls)+" providers: "+error;
                return false;
            }
            return true;
        }
        if(!std::strcmp(name,"NativeGetQuestCount")){
            // IDA 0x4401d4: [playerIndex, predicateName], with the active
            // source difficulty selected by Character::SG_GetNumQuests.
            if(fn.nargs!=2||!fn.arg(0).is_number())return true;
            std::int32_t player=0;
            if(!start_integer(&self,fn.arg(0).to_number(),player,error))return false;
            std::uintptr_t identity=0;if(!local_character(player,false,identity))return false;
            if(!identity)return true;
            std::vector<model_renderer::UiQuestReadV1> rows;
            if(!model_renderer::ui_player_quest_rows(identity,rows,error))return false;
            const std::string filter=fn.arg(1).to_xstring();
            dh2::native::ui::QuestJournalPredicateV1 policy{};
            if(!self.quest_journal_predicate(policy,error))return false;
            const auto count=std::count_if(rows.begin(),rows.end(),[&](const auto& row){
                return dh2::native::ui::quest_journal_matches_v1(
                    row,filter.c_str(),policy);
            });
            if(fn.result)fn.result->set_double(double(count));
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeGetQuestIDsInRange")){
            // IDA 0x443b18 and dqcharmenu SWF: [outputArray, playerIndex,
            // start, count, predicateName, isMultiplayer]. Native consumes
            // arguments 0..4; the authored sixth argument is ignored.
            if(fn.nargs!=5&&fn.nargs!=6)return true;
            std::string predicate=fn.arg(4).is_string()?fn.arg(4).to_xstring():std::string();
            std::array<dh2::native::ui::QuestJournalCallbackValueV1,6> args{};
            args[0].kind=fn.arg(0).is_object()?decltype(args[0].kind)::object:decltype(args[0].kind)::other;
            args[0].object=fn.arg(0).is_object()?fn.arg(0).to_object():nullptr;
            for(std::size_t i=1;i<=3;++i){
                if(fn.arg(i).is_number()){
                    args[i].kind=decltype(args[i].kind)::number;
                    args[i].number=fn.arg(i).to_number();
                }
            }
            if(fn.arg(4).is_string()){
                args[4].kind=decltype(args[4].kind)::string;
                args[4].text=predicate.c_str();
            }
            dh2::native::ui::QuestJournalListCallV1 call{};
            if(!dh2::native::ui::decode_quest_journal_list_call_v1(args.data(),fn.nargs,call))return true;
            auto* array=gameswf::cast_to<gameswf::as_array>(static_cast<gameswf::as_object*>(call.output_array));
            if(!array)return true;
            std::int32_t player=0,start=0,limit=0;
            if(!start_integer(&self,call.player_index,player,error)||
               !start_integer(&self,call.start,start,error)||
               !start_integer(&self,call.count,limit,error))return false;
            std::uintptr_t identity=0;if(!local_character(player,false,identity))return false;
            if(!identity)return true;
            std::vector<model_renderer::UiQuestReadV1> rows;
            if(!model_renderer::ui_player_quest_rows(identity,rows,error))return false;
            const auto source_order=rows;
            dh2::native::ui::QuestJournalPredicateV1 policy{};
            if(!self.quest_journal_predicate(policy,error))return false;
            rows.erase(std::remove_if(rows.begin(),rows.end(),[&](const auto& row){
                return !dh2::native::ui::quest_journal_matches_v1(
                    row,call.predicate,policy);
            }),rows.end());
            const auto begin=std::size_t(std::max(0,start));
            const auto end=std::min(rows.size(),begin+std::size_t(std::max(0,limit)));
            if(begin<rows.size())rows=std::vector<model_renderer::UiQuestReadV1>(
                rows.begin()+std::ptrdiff_t(begin),rows.begin()+std::ptrdiff_t(end));
            else rows.clear();
            if(!rows.empty()&&!model_renderer::ui_player_resolve_current_quest(
                    identity,source_order,rows,error))return false;
            struct LocalizedQuest {model_renderer::UiQuestReadV1 row;std::string title;};
            std::vector<LocalizedQuest> localized;localized.reserve(rows.size());
            for(const auto& row:rows){
                std::string title;
                const auto id=row.text_ids[0];
                if(id>=0&&id!=1835016){
                    if(!self.localization.string_id(std::uint32_t(id),self.text_services(),title,error))return false;
                }else title="not specified";
                localized.push_back({row,std::move(title)});
            }
            std::stable_sort(localized.begin(),localized.end(),[](const auto& a,const auto& b){
                return std::strcmp(a.title.c_str(),b.title.c_str())<0;
            });
            for(const auto& item:localized){
                auto* object=new gameswf::as_object(fn.get_player());
                if(!object->set_member("QuestTitle",gameswf::as_value(item.title.c_str()))||
                   !object->set_member("QuestID",gameswf::as_value(item.row.id))||
                   !object->set_member("IsCurrent",gameswf::as_value(item.row.current))){
                    error="NativeGetQuestIDsInRange output rejected a source field";return false;
                }
                array->push(gameswf::as_value(object));
            }
            if(fn.result)fn.result->set_double(double(localized.size()));
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeGetQuestDetails")){
            // IDA 0x44c444 and dqcharmenu SWF: [outputObject, playerIndex,
            // QuestID, isMultiplayer]. The native selects by Quest::id, not
            // by its ordinal in the player's quest vector.
            if(fn.nargs!=3&&fn.nargs!=4)return true;
            std::array<dh2::native::ui::QuestJournalCallbackValueV1,4> args{};
            if(fn.arg(0).is_object()){
                args[0].kind=decltype(args[0].kind)::object;
                args[0].object=fn.arg(0).to_object();
            }
            for(std::size_t i=1;i<=2;++i)if(fn.arg(i).is_number()){
                args[i].kind=decltype(args[i].kind)::number;
                args[i].number=fn.arg(i).to_number();
            }
            dh2::native::ui::QuestJournalDetailsCallV1 call{};
            if(!dh2::native::ui::decode_quest_journal_details_call_v1(args.data(),fn.nargs,call))return true;
            auto* object=static_cast<gameswf::as_object*>(call.output_object);
            if(!object)return true;
            std::int32_t player=0,quest_id=0;
            if(!start_integer(&self,call.player_index,player,error)||
               !start_integer(&self,call.quest_id,quest_id,error))return false;
            std::uintptr_t identity=0;if(!local_character(player,false,identity))return false;
            if(!identity)return true;
            std::vector<model_renderer::UiQuestReadV1> rows;
            const dh2::native::quests::QuestTextServicesV1 objective_text{
                &self,Impl::option_string};
            if(!model_renderer::ui_player_quest_rows(identity,rows,error,&objective_text))return false;
            const auto* selected=dh2::native::ui::find_quest_journal_row_by_id_v1(rows,quest_id);
            if(!selected)return true;
            const auto& row=*selected;
            const auto text=[&](std::int32_t id,std::string& value){
                // Quest::GetObjectiveDescription delegates this sentinel to
                // ObjectiveList::GetDesc, composed by the canonical Quest
                // owner through this borrowed StringManager service.
                if(id==1835016){value=row.objective_description;return true;}
                if(id<0){value="not specified";return true;}
                return self.localization.string_id(std::uint32_t(id),self.text_services(),value,error);
            };
            std::array<std::string,4> values;
            for(std::size_t i=0;i<values.size();++i)if(!text(row.text_ids[i],values[i]))return false;
            std::uint32_t primary=0;
            if(!constant(&self,"v2QuestPriority","Primary",primary,error))return false;
            if(!object->set_member("Title",gameswf::as_value(values[0].c_str()))||
               !object->set_member("PreDescription",gameswf::as_value(values[1].c_str()))||
               !object->set_member("ObjectiveDescription",gameswf::as_value(values[2].c_str()))||
               !object->set_member("PostDescription",gameswf::as_value(values[3].c_str()))||
               !object->set_member("IsPrimary",gameswf::as_value(row.priority==std::int32_t(primary)))){
                error="NativeGetQuestDetails output rejected a source field";return false;
            }
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeGetWorldMapLocations")){
            // IDA 0x4435e4: [outputArray, playerIndex]. Preserve FastTravelList
            // order and source keys; visibility comes from this difficulty's
            // unlock words in the active Character's canonical Save.
            if(fn.nargs!=2||!fn.arg(0).is_object()||!fn.arg(1).is_number())return true;
            auto* array=gameswf::cast_to<gameswf::as_array>(fn.arg(0).to_object());
            if(!array)return true;
            std::int32_t player=0;
            if(!start_integer(&self,fn.arg(1).to_number(),player,error))return false;
            std::uintptr_t identity=0;
            if(!local_character(player,false,identity))return false;
            if(identity){
                std::vector<model_renderer::UiFastTravelReadV1> rows;
                if(!model_renderer::ui_player_fast_travel_rows(identity,rows,error))return false;
                for(const auto& row:rows){
                    std::string name,description;
                    if(!self.localization.string_id(std::uint32_t(row.string_id),
                            self.text_services(),name,error)||
                       !self.localization.string_id(std::uint32_t(row.description_id),
                            self.text_services(),description,error))return false;
                    auto* object=new gameswf::as_object(fn.get_player());
                    if(!object->set_member("Name",gameswf::as_value(name.c_str()))||
                       !object->set_member("Importance",gameswf::as_value(row.importance))||
                       !object->set_member("LevelName",gameswf::as_value(row.level_name.c_str()))||
                       !object->set_member("EntryPointId",gameswf::as_value(row.entry_point_id))||
                       !object->set_member("Description",gameswf::as_value(description.c_str()))||
                       !object->set_member("Visible",gameswf::as_value(row.visible))){
                        error="NativeGetWorldMapLocations output rejected a source field";return false;
                    }
                    array->push(gameswf::as_value(object));
                }
            }
            if(fn.result)fn.result->set_as_object(array);
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeGoToZone")){
            // IDA 0x4422b8 saves/quicksaves the live Level and Players, resolves
            // a LevelList row, calls DisplayFastTravel, then enters
            // Application::LoadLevel with the selected entry point, Save slot,
            // and difficulty. The existing launch request queue delivers the
            // transaction after this SWF callback stack has unwound.
            if(fn.nargs!=2||!fn.arg(0).is_string()||!fn.arg(1).is_number())return true;
            NativeStartGameIntentV1 request{};
            request.kind=NativeStartGameIntentV1::Kind::level_transition;
            request.destination_name=fn.arg(0).to_string();
            request.raw_entry_point=fn.arg(1).to_number();
            if(request.destination_name.empty())return true;
            self.launch_requests.push_back(std::move(request));
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Authored NativeGoToZone queued | %s | raw entry %.9g | deferred until SWF dispatch unwinds",
                self.launch_requests.back().destination_name.c_str(),
                self.launch_requests.back().raw_entry_point);
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeSetMultitouch")){
            // NativeSetMultitouch writes MenuManager+0x110. MenuManager::onEvent
            // (0x430d60-0x430d74) dispatches fingers 1..3 only while it is clear.
            self.gameplay_multitouch_locked=
                fn.nargs==1?fn.arg(0).to_bool():true;
            return true;
        }
        if(!std::strcmp(name,"NativeInvGetItemsListForSlot")){
            // IDA/Ghidra source ABI at 0x44bb94: [equipmentSlot, outputArray,
            // playerIndex]. The original appends Item objects to the supplied
            // array and returns that same array; V4 remains the sole inventory.
            if(!self.inventory_callback_probe_logged){
                self.inventory_callback_probe_logged=true;
                __android_log_print(ANDROID_LOG_INFO,tag,
                    "Inventory callback probe | argc %d | arg0 number %d | arg1 object %d | arg2 number %d",
                    fn.nargs,fn.nargs>0&&fn.arg(0).is_number(),
                    fn.nargs>1&&fn.arg(1).is_object(),fn.nargs>2&&fn.arg(2).is_number());
            }
            if(fn.nargs!=3||!fn.arg(0).is_number()||!fn.arg(1).is_object()||
               !fn.arg(2).is_number())return true;
            auto* array=gameswf::cast_to<gameswf::as_array>(fn.arg(1).to_object());
            if(!array)return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(2).to_int(),false,identity))return false;
            if(!identity)return true;
            std::vector<model_renderer::UiInventoryItemReadV1> items;
            if(!model_renderer::ui_player_inventory_slot(identity,fn.arg(0).to_int(),items,error))return false;
            const bool valuable_list=fn.arg(0).to_int()==9; // EquipmentSlots::Count
            for(const auto& item:items){
                auto* row=new gameswf::as_object(fn.get_player());
                // NativeInvGetItemsListForSlot only fills a numeric quantity
                // for the Count/valuable-items list. Equipment-slot rows get
                // a single blank, and Count rows are never marked equipped.
                const std::string quantity=valuable_list?std::to_string(item.quantity):" ";
                // The original callback formats each name with ItemInstance::GetColor
                // before the authored inventory row consumes it.
                std::string formatted_name;
                if(!item_name_with_palette(self,item.name,item.power_count,formatted_name,error))return false;
                if(!row->set_member("ItemName",gameswf::as_value(formatted_name.c_str()))||
                   !row->set_member("ItemIndex",gameswf::as_value(item.index))||
                   !row->set_member("ItemEquippable",gameswf::as_value(item.equippable))||
                   !row->set_member("ItemEquipped",gameswf::as_value(!valuable_list&&item.equipped))||
                   !row->set_member("ItemEquippedOtherHand",gameswf::as_value(item.equipped_other_hand))||
                   !row->set_member("ItemQuantity",gameswf::as_value(quantity.c_str()))){
                    error="NativeInvGetItemsListForSlot item object rejected a source field";return false;
            }
            array->push(gameswf::as_value(row));
        }
        if(fn.result)fn.result->set_as_object(array);
        __android_log_print(ANDROID_LOG_INFO,tag,
            "Inventory list callback complete | slot %d | rows %zu | player %d",
            fn.arg(0).to_int(),items.size(),fn.arg(2).to_int());
        return true;
    }
        if(!std::strcmp(name,"NativeInvDropItem")){
            // Authored doDrop ActionCallFunction encodes argc=2; native IDA
            // 0x43cfb4 reads only arg0 as the inventory index. Match that
            // behavior by accepting argc >= 1. Offline drop calls
            // TransferItemTo then ItemObject::DropInventory. V4 remains
            // the sole state owner.
            if(fn.nargs<1||!fn.arg(0).is_number())return true;
            std::int32_t item_index=0;
            if(!start_integer(&self,fn.arg(0).to_number(),item_index,error))return false;
            std::uintptr_t identity=0;
            if(!local_character(0,false,identity))return false;
            if(!identity)return true;
            if(!model_renderer::ui_player_drop_inventory_item(identity,item_index,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Native inventory click callback | action Drop | item %d | player 0 | Character %zx | canonical V4/world result 1",
                item_index,identity);
            return true;
        }
        if(!std::strcmp(name,"NativeInvGetEquipedItem")){
            // 0x43d790 writes ItemName, ItemIndex and ItemColor(power count)
            // into arg 1 and returns whether the requested slot is occupied.
            if(fn.nargs!=3||!fn.arg(0).is_number()||!fn.arg(1).is_object()||
               !fn.arg(2).is_number())return true;
            auto* object=fn.arg(1).to_object();if(!object)return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(2).to_int(),false,identity))return false;
            if(!identity)return true;
            model_renderer::UiEquippedItemReadV1 item;bool present=false;
            if(!model_renderer::ui_player_equipped_item(identity,fn.arg(0).to_int(),item,present,error))return false;
            std::string formatted_name;
            if(present&&!item_name_with_palette(self,item.name,item.power_count,formatted_name,error))return false;
            if(present&&(!object->set_member("ItemName",gameswf::as_value(formatted_name.c_str()))||
                         !object->set_member("ItemIndex",gameswf::as_value(item.index))||
                         !object->set_member("ItemColor",gameswf::as_value(item.power_count)))){
                error="NativeInvGetEquipedItem object rejected a source field";return false;
            }
            if(fn.result)fn.result->set_bool(present);
            return true;
        }
        if(!std::strcmp(name,"NativeInvEquipItem")){
            // IDA 0x43e600 calls NativeGetPlayerChar(arg2), then
            // Character::EquipItemToSlot(arg0, arg1). The SWF pushes
            // [playerIndex, ItemIndex, equipmentSlot], while gameswf::fn_call
            // exposes [equipmentSlot, ItemIndex, playerIndex]. The one
            // canonical Player/V4 equipment owner runs source slot replacement,
            // property refresh, requirement pruning, skin refresh and HP/MP
            // validation synchronously before this release callback returns.
            if(fn.nargs!=3||!fn.arg(0).is_number()||!fn.arg(1).is_number()||!fn.arg(2).is_number())return true;
            const auto call=dh2::ui::inventory_click_equip_call_v1(
                fn.arg(0).to_int(),fn.arg(1).to_int(),fn.arg(2).to_int());
            std::uintptr_t identity=0;
            if(!local_character(call.player_index,false,identity))return false;
            if(!identity)return true;
            if(!model_renderer::ui_player_equip_item(identity,call.item_index,call.equipment_slot,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Native inventory click callback | action Equip | slot %d | item %d | player %d | Character %zx | canonical V4 result 1",
                call.equipment_slot,call.item_index,call.player_index,identity);
            // NativeInvEquipItem drops its result value and writes the source
            // undefined tag after Character::EquipItemToSlot succeeds.
            if(fn.result)fn.result->set_undefined();
            return true;
        }
        if(!std::strcmp(name,"NativeInvUnequipItem")){
            // IDA 0x43ef64 calls NativeGetPlayerChar(arg1), then
            // Character::UnequipItemFromSlot(arg0): [equipmentSlot, player].
            if(fn.nargs!=2||!fn.arg(0).is_number()||!fn.arg(1).is_number())return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(1).to_int(),false,identity))return false;
            if(!identity)return true;
            if(!model_renderer::ui_player_unequip_item(identity,fn.arg(0).to_int(),error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Native inventory click callback | action Unequip | slot %d | item -1 | player %d | Character %zx | canonical V4 result 1",
                fn.arg(0).to_int(),fn.arg(1).to_int(),identity);
            return true;
        }
        if(!std::strcmp(name,"NativeInvAutoEquipSlot")){
            // IDA 0x43da2c calls NativeGetPlayerChar(arg1), then dispatches
            // Character::EquipSlotAuto(arg0); -1 requests EquipAllSlotsAuto:
            // [slot, player]. The source slot-auto wrapper refreshes gear
            // state after sorting candidates; it does not equip a candidate.
            if(fn.nargs!=2||!fn.arg(0).is_number()||!fn.arg(1).is_number())return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(1).to_int(),false,identity))return false;
            if(!identity)return true;
            return model_renderer::ui_player_auto_equip_slot(identity,fn.arg(0).to_int(),error);
        }
        if(!std::strcmp(name,"NativeInvTransmuteItem")){
            // 0x43f23c: [itemIndex, playerIndex], then INV_TransmuteItem and
            // INV_UpdateSkin. The canonical V4 inventory owns the mutation.
            if(fn.nargs!=2||!fn.arg(0).is_number()||!fn.arg(1).is_number())return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(1).to_int(),false,identity))return false;
            if(!identity)return true;
            std::uint32_t multiplier=0;
            if(!constant(&self,"CharacterDesign","TransmuteMultiplier",multiplier,error)||
               !model_renderer::ui_player_transmute_item(identity,fn.arg(0).to_int(),
                    multiplier,error))return false;
            if(fn.result)fn.result->set_undefined();
            return true;
        }
        if(!std::strcmp(name,"NativeSwapEquipment")){
            // 0x442198 swaps the canonical equipment, refreshes gear/Skin/
            // vitals, then calls DisplayRightHud and FillActionIcon. Defer the
            // two authored SWF calls until this native event has unwound.
            if(fn.nargs!=1||!fn.arg(0).is_number())return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(0).to_int(),false,identity))return false;
            if(!identity)return true;
            return ui::swap_equipment_and_defer_hud_refresh_v1(
                self.gameplay_hud_refresh,self.gameplay_hud&&self.gameplay_hud_bound,
                [&](std::string& swap_error){
                    return model_renderer::ui_player_swap_equipment(identity,swap_error);
                },error);
        }
        if(!std::strcmp(name,"NativeInvGetHasOffHandWeapon")||
           !std::strcmp(name,"NativeInvGetHasTwoHandedWeapon")){
            if(fn.nargs!=1||!fn.arg(0).is_number())return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(0).to_int(),false,identity))return false;
            if(!identity)return true;
            bool off_hand=false,two_handed=false;
            if(!model_renderer::ui_player_weapon_flags(identity,off_hand,two_handed,error))return false;
            if(fn.result)fn.result->set_bool(!std::strcmp(name,"NativeInvGetHasOffHandWeapon")?off_hand:two_handed);
            return true;
        }
        if(!std::strcmp(name,"NativeHUDGetActiveFaery")){
            // 0x44a820 returns the ID directly or fills the optional SWF
            // record with Id and Upgraded. The source only selects object
            // output when arg1 is an object; any other two-argument call
            // still returns the scalar active Faery ID.
            if((fn.nargs!=1&&fn.nargs!=2)||!fn.arg(0).is_number())return true;
            std::uintptr_t identity=0;
            if(!faery_character(fn.arg(0).to_int(),identity))return false;
            if(!identity)return true;
            std::int32_t id=-1,level=-1;
            if(!model_renderer::ui_player_active_faery(identity,id,level,error))return false;
            if(fn.nargs==1||!fn.arg(1).is_object()){
                if(fn.result)fn.result->set_int(id);return true;
            }
            auto* object=fn.arg(1).to_object();if(!object)return true;
            if(!object->set_member("Id",gameswf::as_value(id))||
               !object->set_member("Upgraded",gameswf::as_value(level>0))){
                error="NativeHUDGetActiveFaery object rejected a source field";return false;
            }
            if(fn.result)fn.result->set_as_object(object);
            return true;
        }
        if(!std::strcmp(name,"NativeHUDGetIsFaeryUnlocked")){
            // IDA NativeHUDGetIsFaeryUnlocked (0x43ed58) reads player index
            // from arg0 and faery ID from arg1 before calling
            // Character::SG_IsFaerieUnlocked(player, faeryId, -1).
            if(fn.nargs!=2||!fn.arg(0).is_number()||!fn.arg(1).is_number())return true;
            const auto raw_id=fn.arg(1).to_int();
            if(raw_id<0)return true;
            std::uintptr_t identity=0;
            if(!faery_character(fn.arg(0).to_int(),identity))return false;
            if(!identity)return true;
            bool unlocked=false;
            if(!model_renderer::ui_player_faery_unlocked(identity,std::uint32_t(raw_id),unlocked,error))return false;
            if(fn.result)fn.result->set_bool(unlocked);
            return true;
        }
        if(!std::strcmp(name,"NativeHUDSetActiveFaery")){
            // IDA NativeHUDSetActiveFaery (0x43ee40) reads [playerIndex,
            // faeryId], calls Character::ChangeFaery, then calls
            // Level::PlaceFaeryAndFollowers when a Level exists. The existing
            // renderer provider owns the canonical Save, skill update, live
            // Level placement, and persistence for that same Player.
            if(fn.nargs!=2||!fn.arg(0).is_number()||!fn.arg(1).is_number())return true;
            const auto raw_id=fn.arg(1).to_int();
            if(raw_id<0)return true;
            std::uintptr_t identity=0;
            if(!faery_character(fn.arg(0).to_int(),identity))return false;
            if(!identity)return true;
            if(!model_renderer::ui_player_set_active_faery(
                    identity,std::uint32_t(raw_id),error))return false;
            // CharacterMenu's Faery buttons update their own preview clip,
            // but do not refresh the retained HUD's skill buttons. Schedule
            // its authored onPush after this native dispatch returns so the
            // HUD reads the just-changed canonical Save.
            if(self.gameplay_hud&&self.gameplay_hud_bound)
                self.gameplay_hud_skills_refresh.request();
            // The source callback returns undefined after completing the
            // ChangeFaery and Level placement sequence.
            if(fn.result)fn.result->set_undefined();
            return true;
        }
        if(!std::strcmp(name,"NativeGetCharMenuTutorialMessage")){
            if(fn.nargs!=1||!fn.arg(0).is_object())return true;
            auto* object=fn.arg(0).to_object();if(!object)return true;
            ui::SourceCharMenuTutorialQueueV1::Message message;
            bool found=false;
            if(!self.source_char_menu_tutorial_messages.front(message,found)){
                error="Retained CharMenuTutorialMsg queue could not return its front record";return false;
            }
            if(found){
                std::string text;
                if(!self.ensure_localization_ready(error)||
                   !self.localization.string_id(static_cast<std::uint32_t>(message.string_id),
                                                 self.text_services(),text,error))return false;
                const auto assign=[&](const char* key,const gameswf::as_value& value){
                    if(object->set_member(key,value))return true;
                    error=std::string("NativeGetCharMenuTutorialMessage output rejected ")+key;
                    return false;
                };
                if(!assign("TutorialMessage",gameswf::as_value(text.c_str()))||
                   !assign("TutorialFrame",gameswf::as_value(message.frame.c_str()))||
                   !assign("TutorialMenu",gameswf::as_value(message.menu.c_str())))return false;
            }
            if(fn.result)fn.result->set_bool(false);
            if(fn.result&&found)fn.result->set_bool(true);
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeSkipCharMenuTutorialMessage")){
            if(!self.source_char_menu_tutorial_messages.skip_one()){
                error.clear();return true;
            }
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeGetNextStatusMessage")){
            // NativeGetNextStatusMessage (IDA 0x43fec4) accepts one numeric
            // argument and an optional preceding boolean, then returns the
            // head StatusMsg text. NativeStopMessage("status") owns removal.
            if((fn.nargs!=1&&fn.nargs!=2)||!fn.arg(0).is_number()||
               (fn.nargs==2&&!fn.arg(1).is_bool()))return true;
            std::string message;bool found=false;
            if(!self.source_status_messages.front(message,found)){
                error="Retained StatusMsg queue could not return its front text";return false;
            }
            if(fn.result&&found)fn.result->set_string(message.c_str());
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeStopMessage")){
            // NativeStopMessage 0x442ea4 reads the queue name from its first
            // argument and ignores any others (the DialogBox hide animation calls
            // it as NativeStopMessage("dialog"|"quest", 0)). "dialog" and "quest"
            // both end in MenuMessageManager<DialogMsg,1>::SkipEnqueuedMessage
            // 0x442c1c, the same pop MenuBase::FS_StopDialog performs. The
            // "tutorial" and "online" queues belong to features this port does
            // not run offline.
            if(fn.nargs<1||!fn.arg(0).is_string())return true;
            const char* queue=fn.arg(0).to_string();
            if(!std::strcmp(queue,"dialog")||!std::strcmp(queue,"quest"))
                return stop_source_dialog(self,error);
            // The source StatusMsg::s_SkipFuncName is null; only the queue
            // pop and next onStatusMessage callback are required here.
            if(std::strcmp(queue,"status"))return true;
            bool start_next=false;
            if(!self.source_status_messages.stop(start_next)||!start_next){
                error.clear();return true;
            }
            if(!self.gameplay_hud||!self.gameplay_hud_bound){
                error="Queued StatusMsg has no retained authored HUD callback";return false;
            }
            // Called from inside the SWF's own dispatch: a nested action_script is
            // refused as busy, so the next onStatusMessage runs once it unwound.
            self.status_start_pending=true;error.clear();return true;
        }
        if(!std::strcmp(name,"NativeStopDialog")){
            // The authored menu_FastTravel dialog box (dqhud_droid.swf sprite 508,
            // frame 20) calls this native directly; the gameplay DialogBox ends
            // through NativeStopMessage("dialog"). It is the UI equivalent of
            // MenuBase::FS_StopDialog:
            // pop one DialogMsg and re-enter the same root StartDialog only
            // when another queued message remains. WaitDialog observes this
            // exact queue and resumes on the following LevelVM update.
            if(fn.nargs!=0)return true;
            return stop_source_dialog(self,error);
        }
        if(!std::strcmp(name,"NativeGetNextDialogMessage")){
            // MenuManager::Init registers this exact one-object callback.
            // GetNextEnqueuedMessage peeks (StopDialog owns the pop) and the
            // SWF consumes these five source field names verbatim.
            if(fn.nargs!=1||!fn.arg(0).is_object())return true;
            auto* object=fn.arg(0).to_object();if(!object)return true;
            ui::SourceDialogMessageQueueV1::Message message;
            bool found=false;
            if(!self.source_dialog_messages.front(message,found)){
                error="Retained DialogMsg queue could not return its front record";return false;
            }
            const auto assign=[&](const char* key,const gameswf::as_value& value){
                if(object->set_member(key,value))return true;
                error=std::string("NativeGetNextDialogMessage output rejected ")+key;
                return false;
            };
            if(!assign("DialogTitle",gameswf::as_value(message.title.c_str()))||
               !assign("DialogMessage",gameswf::as_value(message.text.c_str()))||
               !assign("DialogStyle",gameswf::as_value(found?message.style:-1))||
               !assign("DialogStyleStr",gameswf::as_value(message.style_name.c_str()))||
               !assign("DialogActorStr",gameswf::as_value(message.actor_name.c_str())))return false;
            if(fn.result)fn.result->set_bool(found);
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeInvGetItemDetails")){
            // 0x44ca5c player call: [itemIndex, outputObject, playerIndex].
            // The merchant SWF's five-argument equipped-item comparison adds
            // [merchantName, merchantObjectId] but the native only selects its
            // merchant path for exactly six args, so it still reads the first
            // three values from the player's canonical V4 inventory. The
            // six-argument ABI is [itemIndex, outputObject, playerIndex,
            // merchantFlag, merchantName, merchantObjectId]. A true merchant
            // flag selects that exact Character's retained V4 inventory.
            const auto kind=[](const gameswf::as_value& value){
                using Kind=dh2::ui::MerchantDetailsValueKindV1;
                if(value.is_number())return Kind::number;
                if(value.is_object())return Kind::object;
                if(value.is_bool())return Kind::boolean;
                if(value.is_string())return Kind::string;
                return Kind::other;
            };
            dh2::ui::MerchantDetailsCallShapeV1 shape;
            shape.count=fn.nargs;
            for(std::size_t i=0;i<shape.arguments.size()&&i<std::size_t(fn.nargs);++i)
                shape.arguments[i]=kind(fn.arg(int(i)));
            const auto form=dh2::ui::merchant_details_call_form_v1(shape);
            const bool merchant_form=form==dh2::ui::MerchantDetailsCallFormV1::merchant_inventory&&
                                     fn.arg(3).to_bool();
            if(form!=dh2::ui::MerchantDetailsCallFormV1::player_inventory&&!merchant_form)return true;
            auto* object=fn.arg(1).to_object();if(!object)return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(2).to_int(),false,identity))return false;
            if(!identity)return true;
            std::uint32_t transmute_multiplier=0;
            if(!constant(&self,"CharacterDesign","TransmuteMultiplier",
                         transmute_multiplier,error))return false;
            model_renderer::UiItemDetailsReadV1 item;
            if(merchant_form){
                const auto merchant_name=fn.arg(4).to_xstring();
                if(!model_renderer::ui_source_merchant_item_details(
                       identity,fn.arg(5).to_int(),merchant_name,fn.arg(0).to_int(),
                       transmute_multiplier,item,error))return false;
            }else if(!model_renderer::ui_player_inventory_item_details(identity,
                    fn.arg(0).to_int(),transmute_multiplier,item,error))return false;
            std::array<std::string,4> powers{};
            for(std::size_t i=0;i<powers.size()&&i<item.power_descriptions.size();++i)
                powers[i]=item.power_descriptions[i];
            std::string value_text,buy_text,sell_text,transmute_text;
            if(!format_integer_text(self,"^d",item.value,value_text,error))return false;
            if(!format_integer_text(self,"^d",item.buy_value,buy_text,error)||
               !format_integer_text(self,"^d",item.sell_value,sell_text,error)||
               !format_integer_text(self,"^d",item.transmute_value,transmute_text,error))return false;
            const auto assign=[&](const char* key,const gameswf::as_value& value){
                if(object->set_member(key,value))return true;
                error=std::string("NativeInvGetItemDetails output rejected ")+key;
                return false;
            };
            if(!assign("ItemName",gameswf::as_value(item.name.c_str()))||
               !assign("ItemStatsDesc",gameswf::as_value(item.stats.c_str()))||
               !assign("ItemPowers0Desc",gameswf::as_value(powers[0].c_str()))||
               !assign("ItemPowers1Desc",gameswf::as_value(powers[1].c_str()))||
               !assign("ItemPowers2Desc",gameswf::as_value(powers[2].c_str()))||
               !assign("ItemPowers3Desc",gameswf::as_value(powers[3].c_str()))||
               !assign("ItemMagics",gameswf::as_value(
                   static_cast<int>(item.power_descriptions.size())))||
               !assign("ItemReqsDesc",gameswf::as_value(item.requirements.c_str()))||
               !assign("ItemEquippable",gameswf::as_value(item.equippable))||
               !assign("ItemIcon",gameswf::as_value(item.icon.c_str()))||
               !assign("IsStackable",gameswf::as_value(item.stackable))||
               !assign("ItemValueString",gameswf::as_value(value_text.c_str()))||
               !assign("ItemValue",gameswf::as_value(item.value))||
               !assign("ItemBuyValueString",gameswf::as_value(buy_text.c_str()))||
               !assign("ItemBuyValue",gameswf::as_value(item.buy_value))||
               !assign("ItemSellValueString",gameswf::as_value(sell_text.c_str()))||
               !assign("ItemSellValue",gameswf::as_value(item.sell_value))||
               !assign("ItemEquipped",gameswf::as_value(item.equipped))||
               !assign("ItemEquippedOtherHand",gameswf::as_value(item.equipped_other_hand))||
               !assign("ItemTransmuteValueString",gameswf::as_value(transmute_text.c_str()))||
               !assign("ItemTransmuteValue",gameswf::as_value(item.transmute_value)))return false;
            if(fn.result)fn.result->set_as_object(object);
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeMerchantGetItemsListForSlot")){
            // IDA 0x444168 ABI: [slot, outputArray, playerInfoIndex,
            // merchantName, merchantObjectId]. Source resolves the Player first,
            // then GetObjectByName(name,id), verifies Character::IsMerchant,
            // reads that Character's ItemInventory, selects GetItemListForSlot
            // or (EquipmentSlots::Count) GetValuableItems, sorts by
            // II_Item_sortbybuyback, and appends ItemName/ItemIndex/
            // ItemEquippable/ItemEquipped/ItemBuyback/ItemQuantity records.
            // The exact source (module filter,name) resolves through the shared
            // ObjectManager to the imported CharType 7 ObjectActor and its V4.
            // MerchantTable quoting and
            // item rows borrow those same canonical owners.
            if(fn.nargs!=5||!fn.arg(0).is_number()||!fn.arg(1).is_object()||
               !fn.arg(2).is_number()||!fn.arg(3).is_string()||!fn.arg(4).is_number())return true;
            auto* array=gameswf::cast_to<gameswf::as_array>(fn.arg(1).to_object());
            if(!array)return true;
            std::uintptr_t player_identity=0;
            if(!local_character(fn.arg(2).to_int(),false,player_identity))return false;
            if(!player_identity)return true;
            std::vector<model_renderer::UiMerchantItemReadV1> items;
            if(!model_renderer::ui_source_merchant_inventory_slot(fn.arg(4).to_int(),
                    fn.arg(3).to_xstring(),fn.arg(0).to_int(),items,error))return false;
            const bool valuable_list=fn.arg(0).to_int()==9;
            for(const auto& item:items){
                std::string formatted_name;
                if(!item_name_with_palette(self,item.name,item.power_count,formatted_name,error))return false;
                auto* row=new gameswf::as_object(fn.get_player());
                const std::string quantity=valuable_list?std::to_string(item.quantity):" ";
                if(!row->set_member("ItemName",gameswf::as_value(formatted_name.c_str()))||
                   !row->set_member("ItemIndex",gameswf::as_value(item.index))||
                   !row->set_member("ItemEquippable",gameswf::as_value(item.equippable))||
                   !row->set_member("ItemEquipped",gameswf::as_value(!valuable_list&&item.equipped))||
                   !row->set_member("ItemEquippedOtherHand",gameswf::as_value(item.equipped_other_hand))||
                   !row->set_member("ItemBuyback",gameswf::as_value(item.buyback))||
                   !row->set_member("ItemQuantity",gameswf::as_value(quantity.c_str()))){
                    error="NativeMerchantGetItemsListForSlot output rejected a source field";return false;
                }
                array->push(gameswf::as_value(row));
            }
            if(fn.result)fn.result->set_as_object(array);
            error.clear();
            return true;
        }
        if(!std::strcmp(name,"NativeMerchantBuyItem")){
            // IDA 0x4408d4 ABI: [merchantItemIndex, playerInfoIndex,
            // merchantName, merchantObjectId]. The renderer transaction resolves
            // the exact merchant Character and both canonical V4 inventories,
            // then preserves the source mutation prefix and status codes.
            if(fn.result)fn.result->set_double(0);
            if(fn.nargs!=4||!fn.arg(0).is_number()||!fn.arg(1).is_number()||
               !fn.arg(2).is_string()||!fn.arg(3).is_number())return true;
            std::uintptr_t player_identity=0;
            if(!local_character(fn.arg(1).to_int(),false,player_identity))return false;
            if(!player_identity){if(fn.result)fn.result->set_double(0);return true;}
            std::int32_t result=0;
            if(!model_renderer::ui_source_merchant_buy_transaction(fn.arg(3).to_int(),
                 fn.arg(2).to_xstring(),fn.arg(0).to_int(),player_identity,result,error))return false;
            if(fn.result)fn.result->set_double(double(result));
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeMerchantSellItem")){
            // The source MenuManager gate is consumed before argument parsing.
            // Its MenuBase::Hide path resets the gate; covering the menu does not.
            if(self.game_menu_stack.empty()||self.game_menu_stack.back()!="menu_Merchant"||
               self.merchant_sell_one_shot_used){
                if(fn.result)fn.result->set_bool(false);
                error.clear();return true;
            }
            self.merchant_sell_one_shot_used=true;
            // IDA 0x44058c ABI: [playerItemIndex, playerInfoIndex,
            // merchantName, merchantObjectId].
            if(fn.nargs!=4||!fn.arg(0).is_number()||!fn.arg(1).is_number()||
               !fn.arg(2).is_string()||!fn.arg(3).is_number()){
                if(fn.result)fn.result->set_bool(false);
                error.clear();return true;
            }
            std::uintptr_t player_identity=0;
            if(!local_character(fn.arg(1).to_int(),false,player_identity))return false;
            if(!player_identity){if(fn.result)fn.result->set_bool(false);return true;}
            std::int32_t result=0;
            if(!model_renderer::ui_source_merchant_sell_transaction(fn.arg(3).to_int(),
                 fn.arg(2).to_xstring(),fn.arg(0).to_int(),player_identity,result,error))return false;
            // NativeMerchantSellItem returns false after its transaction.
            if(fn.result)fn.result->set_bool(false);
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeInvGetPlayerGold")){
            // 0x44a5c4 and the inventory SWF agree on these call forms:
            // [playerIndex] returns the formatted text used by player_gold;
            // [playerIndex, outputObject] also fills GoldString/Gold and
            // returns that object; [playerIndex, ignored, remoteFlag] is the
            // remote string form. The SWF's one-argument form is the live path.
            if((fn.nargs<1||fn.nargs>3)||!fn.arg(0).is_number())return true;
            auto* object=(fn.nargs==2&&fn.arg(1).is_object())?fn.arg(1).to_object():nullptr;
            const bool remote=fn.nargs==3&&fn.arg(2).to_bool();
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(0).to_int(),remote,identity))return false;
            if(!identity)return true;
            std::int32_t gold=0;
            if(!model_renderer::ui_player_inventory_gold(identity,gold,error))return false;
            std::string gold_text;
            if(!format_integer_text(self,"^d",gold,gold_text,error))return false;
            if(object){
                if(!object->set_member("GoldString",gameswf::as_value(gold_text.c_str()))||
                   !object->set_member("Gold",gameswf::as_value(gold))){
                    error="NativeInvGetPlayerGold output rejected a source field";return false;
                }
                if(fn.result)fn.result->set_as_object(object);
            }else if(fn.result){
                fn.result->set_string(gold_text.c_str());
            }
            return true;
        }
        if(!std::strcmp(name,"NativeGetNumPotions")){
            // 0x44996c: [outputObject]. NativeGetLocalPlayer selects the local
            // player, then ItemInventory::GetNumPotions and the signed
            // potion-capacity byte populate these two ActionScript fields.
            if(fn.nargs!=1||!fn.arg(0).is_object())return true;
            auto* object=fn.arg(0).to_object();if(!object)return true;
            std::uintptr_t identity=0;
            if(!local_character(0,false,identity))return false;
            if(!identity)return true;
            std::int32_t count=0,capacity=0;
            if(!model_renderer::ui_player_potions(identity,count,capacity,error))return false;
            if(!object->set_member("NumPotions",gameswf::as_value(count))||
               !object->set_member("MaxNumPotions",gameswf::as_value(capacity))){
                error="NativeGetNumPotions output rejected a source field";return false;
            }
            return true;
        }
        if(!std::strcmp(name,"NativeGetStringNumPotions")){
            // 0x446978: [outputObject, playerInfoIndex]. The original calls
            // NativeGetPlayerChar(index, false), formats the localized
            // GAMEPLAYMENUS_YOUR_POTIONS template with the active Item count,
            // and stores the result in StrNumPotions.
            if(fn.nargs!=2||!fn.arg(0).is_object()||!fn.arg(1).is_number())return true;
            auto* object=fn.arg(0).to_object();if(!object)return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(1).to_int(),false,identity))return false;
            if(!identity)return true;
            std::int32_t count=0,capacity=0;
            if(!model_renderer::ui_player_potions(identity,count,capacity,error))return false;
            std::uint32_t text_id=0;std::string pattern,text;
            if(!constant(&self,"StrID","GAMEPLAYMENUS_YOUR_POTIONS",text_id,error)||
               !self.localization.string_id(text_id,self.text_services(),pattern,error)||
               !format_integer_text(self,pattern.c_str(),count,text,error))return false;
            if(!object->set_member("StrNumPotions",gameswf::as_value(text.c_str()))){
                error="NativeGetStringNumPotions output rejected StrNumPotions";return false;
            }
            return true;
        }
        if(!std::strcmp(name,"NativeUsePotion")){
            // The authored cmdHUDpotion($playerId) calls NativeUsePotion()
            // without forwarding its parameter. Preserve the native explicit
            // [playerIndex] form too; this offline HUD has local player 0.
            if(fn.nargs>1||(fn.nargs==1&&!fn.arg(0).is_number()))return true;
            std::int32_t supplied=0,player=0;
            if(fn.nargs==1&&!start_integer(&self,fn.arg(0).to_number(),supplied,error))return false;
            if(!dh2::ui::native_use_potion_player_v1(unsigned(fn.nargs),supplied,player))return true;
            std::uintptr_t identity=0;
            if(!local_character(player,false,identity))return false;
            if(!identity)return true;
            return model_renderer::ui_player_use_potion(identity,error);
        }
        if(!std::strcmp(name,"NativeSkillGetEquipedSkillsIDs")){
            if((fn.nargs!=2&&fn.nargs!=3)||!fn.arg(0).is_object()||!fn.arg(1).is_number()||
               (fn.nargs==3&&!fn.arg(2).is_undefined()&&!fn.arg(2).is_bool()))return true;
            auto* array=gameswf::cast_to<gameswf::as_array>(fn.arg(0).to_object());
            if(!array)return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(1).to_int(),fn.nargs==3&&fn.arg(2).is_bool()&&fn.arg(2).to_bool(),identity))return false;
            if(!identity)return true;
            std::array<std::int32_t,3> slots{};
            if(!model_renderer::ui_player_skill_slots(identity,slots,error))return false;
            for(const auto slot:slots)array->push(gameswf::as_value(slot));
            if(fn.result)fn.result->set_bool(true);
            return true;
        }
        if(!std::strcmp(name,"NativeGetSkillDetails")){
            // Native ABI: [skillListIndex, outputObject, playerInfoIndex,
            // optionalRemotePlayer]. Keep the native Save rank and equipped
            // slot distinct from the skill's level/cap predicates.
            if((fn.nargs!=3&&fn.nargs!=4)||!fn.arg(0).is_number()||
               !fn.arg(1).is_object()||!fn.arg(2).is_number())return true;
            auto* object=fn.arg(1).to_object();
            if(!object)return true;
            const auto raw_index=fn.arg(0).to_int();
            if(raw_index<0)return true;
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(2).to_int(),
                    fn.nargs==4&&fn.arg(3).to_bool(),identity))return false;
            if(!identity){if(fn.result)fn.result->set_as_object(object);return true;}
            model_renderer::UiSkillReadV1 skill;
            if(!model_renderer::ui_player_skill(identity,std::uint32_t(raw_index),skill,error))return false;
            std::string name_text,description_text,current_text,next_text;
            if(!self.localization.string_id(std::uint32_t(skill.name_text),
                    self.text_services(),name_text,error)||
               !self.localization.string_id(std::uint32_t(skill.description_text),
                    self.text_services(),description_text,error))return false;
            static constexpr const char* skill_cap_keys[]={
                "MaxSkillLevelBNormal","MaxSkillLevelCHard","MaxSkillLevelDVeryHard"};
            std::uint32_t cap_bits=0;
            std::int32_t skill_cap=0;
            if(skill.character_level>=skill.required_level){
                if(skill.difficulty<0||skill.difficulty>2){
                    error="NativeGetSkillDetails unlocked difficulty is outside the source cap table";return false;
                }
                if(!constant(&self,"CharacterDesign",skill_cap_keys[skill.difficulty],
                             cap_bits,error))return false;
                std::memcpy(&skill_cap,&cap_bits,sizeof(skill_cap));
            }
            const auto preview=dh2::ui::skill_detail_preview_v1(
                skill.character_level,skill.required_level,skill.level,
                skill_cap,skill.can_increment);
            // NativeGetSkillDetails offsets the rank StringIDs by
            // FaeryTable+8 for FairieDependantText (ELF 0x44615c/0x446244).
            // ui_player_skill resolves that word from the active Save's
            // selected Faery; use this session's existing Localization owner.
            const auto skill_rank_text_id=[&](std::int32_t id,std::uint32_t& result){
                if(skill.faerie_dependent_text&&id==-1)return false;
                auto value=std::uint32_t(id);
                if(skill.faerie_dependent_text)
                    value+=std::uint32_t(skill.faerie_text_offset);
                if(value&0x80000000u)return false;
                result=value;return true;
            };
            std::vector<std::int32_t> current_properties,next_properties;
            if(!preview.unlocked){
                std::uint32_t text_id=0;
                std::string pattern;
                if(!constant(&self,"StrID","GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL",
                             text_id,error)||
                   !self.localization.string_id(text_id,self.text_services(),pattern,error)||
                   !format_integer_text(self,pattern.c_str(),skill.required_level,
                                        current_text,error))return false;
            }else{
                if(!model_renderer::ui_player_skill_display_properties(identity,
                       std::uint32_t(raw_index),skill.level,current_properties,error))return false;
                if(skill.level==std::numeric_limits<std::int32_t>::max()){
                    error="NativeGetSkillDetails next-rank index overflow";return false;
                }
                if(!model_renderer::ui_player_skill_display_properties(identity,
                       std::uint32_t(raw_index),skill.level+1,next_properties,error))return false;
                if(preview.current==dh2::ui::SkillDetailCurrentTextV1::needs_skill_points){
                    std::uint32_t text_id=0;
                    if(!constant(&self,"StrID","GAMEPLAYMENUS_NEEDS_SKILL_POINTS",
                                 text_id,error)||
                       !self.localization.string_id(text_id,self.text_services(),
                                                   current_text,error))return false;
                }else if(preview.current==dh2::ui::SkillDetailCurrentTextV1::current_rank){
                    std::uint32_t current_text_id=0;
                    if(skill_rank_text_id(skill.current_text,current_text_id)&&
                       (!self.localization.string_id(current_text_id,
                            self.text_services(),current_text,error)||
                        !format_skill_level_text(self,current_text,current_properties,
                            current_text,error)))return false;
                }
                if(preview.next==dh2::ui::SkillDetailNextTextV1::next_rank){
                    std::uint32_t next_text_id=0;
                    if(skill_rank_text_id(skill.next_text,next_text_id)&&
                       (!self.localization.string_id(next_text_id,
                            self.text_services(),next_text,error)||
                        !format_skill_level_text(self,next_text,next_properties,
                            next_text,error)))return false;
                }else if(preview.next==dh2::ui::SkillDetailNextTextV1::maximum_level){
                    std::uint32_t text_id=0;
                    if(!constant(&self,"StrID","GAMEPLAYMENUS_MAX_SKILL_LEVEL",
                                 text_id,error)||
                       !self.localization.string_id(text_id,self.text_services(),
                                                   next_text,error))return false;
                }else if(preview.next==dh2::ui::SkillDetailNextTextV1::training_blocked){
                    std::uint32_t text_id=0;
                    if(!constant(&self,"StrID","GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING",
                                 text_id,error)||
                       !self.localization.string_id(text_id,self.text_services(),
                                                   next_text,error))return false;
                }
            }
            const auto assign=[&](const char* key,const gameswf::as_value& value){
                if(object->set_member(key,value))return true;
                error=std::string("NativeGetSkillDetails output rejected ")+key;
                return false;
            };
            if(!assign("SkillName",gameswf::as_value(name_text.c_str()))||
               !assign("SkillDescription",gameswf::as_value(description_text.c_str()))||
               !assign("SkillCurrLevel",gameswf::as_value(current_text.c_str()))||
               !assign("SkillNextLevel",gameswf::as_value(next_text.c_str()))||
               !assign("SkillAssignable",gameswf::as_value(skill.assignable))||
               !assign("SkillIcon",gameswf::as_value(skill.icon.c_str()))||
               !assign("SkillLevel",gameswf::as_value(skill.level))||
               !assign("SkillAssignedToSlot",gameswf::as_value(skill.slot)))return false;
            // The source sets this after both DisplayProps passes, after its
            // character-level gate. Keep it separate from CanIncrementSkill.
            if(!assign("SkillUnlocked",gameswf::as_value(preview.unlocked)))return false;
            if(fn.result)fn.result->set_as_object(object);
            if(!self.talent_detail_callback_logged){
                self.talent_detail_callback_logged=true;
                __android_log_print(ANDROID_LOG_INFO,tag,
                    "Talent details callback | Character %p | list index %d | skill id %d | level %d | slot %d | source Save and Player skill owner",
                    reinterpret_cast<void*>(identity),raw_index,
                    skill.id,skill.level,skill.slot);
            }
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeSkillsGetSkillPointsLeft")){
            if(fn.nargs!=1||!fn.arg(0).is_number())return true;
            std::uintptr_t identity=0;if(!local_character(fn.arg(0).to_int(),false,identity))return false;
            if(!identity)return true;
            std::int32_t points=0;if(!model_renderer::ui_player_skill_points(identity,points,error))return false;
            if(fn.result)fn.result->set_double(points);
            return true;
        }
        if(!std::strcmp(name,"NativeSkillsTrainSkill")){
            dh2::ui::SkillTrainCallbackResultV1 trained{};
            const auto to_integer=[&](std::size_t index,std::int32_t& value,std::string& failure){
                return start_integer(&self,fn.arg(static_cast<int>(index)).to_number(),value,failure);
            };
            const auto to_boolean=[&](std::size_t index){
                return fn.arg(static_cast<int>(index)).to_bool();
            };
            const auto resolve_player=[&](std::int32_t player_index,
                    std::uintptr_t& identity,std::string& failure){
                if(!local_character(player_index,false,identity)){
                    failure=error;return false;
                }
                if(!identity){
                    failure="NativeSkillsTrainSkill requires the attached local Player Character";
                    return false;
                }
                return true;
            };
            const auto read_points=[](std::uintptr_t identity,
                    std::int32_t& points,std::string& failure){
                return model_renderer::ui_player_skill_points(identity,points,failure);
            };
            const auto train=[](std::uintptr_t identity,std::int32_t index,
                    bool test_only,std::uint32_t& source_return,
                    std::int32_t& points,std::string& failure){
                if(index<0){failure="NativeSkillsTrainSkill negative index reached train provider";return false;}
                return model_renderer::ui_player_train_skill(identity,
                    std::uint32_t(index),test_only,source_return,points,failure);
            };
            if(!dh2::ui::apply_skill_train_callback_v1(
                   std::size_t(fn.nargs),to_integer,to_boolean,resolve_player,
                   read_points,train,trained,error))return false;
            if(trained.training_called)
                ui::refresh_usable_after_skill_train_v1(
                    self.gameplay_hud_usable_refresh,trained.test_only,
                    trained.source_return);
#ifndef NDEBUG
            if(trained.training_called){
                model_renderer::UiSkillReadV1 readback;std::string readback_error;
                const bool readback_ok=model_renderer::ui_player_skill(
                    trained.character,std::uint32_t(trained.skill_index),
                    readback,readback_error);
                __android_log_print(ANDROID_LOG_INFO,tag,
                    "NativeSkillsTrainSkill result | index %d | test %d | source %u | points %d | readback %d | level %d | skill %d | error %s",
                    trained.skill_index,trained.test_only,trained.source_return,trained.points_left,
                    readback_ok,readback_ok?readback.level:-1,
                    readback_ok?readback.id:-1,readback_error.c_str());
            }
#endif
            if(fn.result&&trained.has_result){
                if(trained.test_only)fn.result->set_bool(trained.source_return!=0);
                else fn.result->set_double(trained.points_left);
            }
            return true;
        }
        if(!std::strcmp(name,"NativeEquipSkill")){
            const auto to_integer=[&](std::size_t index,std::int32_t& value,
                                      std::string& failure){
                return start_integer(&self,fn.arg(static_cast<int>(index)).to_number(),
                                     value,failure);
            };
            const auto resolve_player=[&](std::int32_t player_index,
                    std::uintptr_t& identity,std::string& failure){
                if(!local_character(player_index,false,identity)){
                    failure=error;return false;
                }
                return true;
            };
            const auto equip_skill=[](std::uintptr_t identity,std::int32_t slot,
                    std::int32_t skill,std::string& failure){
                // Source returns undefined. The canonical Save setter
                // preserves its map write and same-owner UpdateSkills tail.
                return model_renderer::ui_player_equip_skill(identity,slot,
                                                               skill,failure);
            };
            return dh2::ui::apply_skill_equip_callback_v1(
                std::size_t(fn.nargs),to_integer,resolve_player,equip_skill,error);
        }
        if(!std::strcmp(name,"NativeGetPlayerStats")){
            if(fn.nargs!=2||!fn.arg(0).is_object()||!fn.arg(1).is_number())return true;
            auto* object=fn.arg(0).to_object();if(!object){error="Player stats callback requires a live ActionScript object";return false;}
            std::uintptr_t identity=0;if(!local_character(fn.arg(1).to_int(),false,identity))return false;
            if(!identity)return true;
            model_renderer::UiPlayerStatsReadV1 stats;
            if(!model_renderer::ui_player_stats(identity,stats,error))return false;
            const auto assign=[&](const char* key,const gameswf::as_value& value){
                if(object->set_member(key,value))return true;error=std::string("Player stats object rejected ")+key;return false;
            };
            if(!assign("Name",gameswf::as_value(stats.name.c_str()))||
               !assign("Class",gameswf::as_value(stats.class_name.c_str()))||
               !assign("Icon",gameswf::as_value(stats.icon))||
               !assign("Level",gameswf::as_value(stats.level))||
               !assign("HP",gameswf::as_value(stats.hp))||
               !assign("HP_Bonus",gameswf::as_value(stats.hp_bonus))||
               !assign("Max_HP",gameswf::as_value(stats.max_hp))||
               !assign("MP",gameswf::as_value(stats.mp))||
               !assign("MP_Bonus",gameswf::as_value(stats.mp_bonus))||
               !assign("Max_MP",gameswf::as_value(stats.max_mp))||
               !assign("XP",gameswf::as_value(stats.xp))||
               !assign("Max_XP",gameswf::as_value(stats.max_xp))||
               !assign("Stat_Strength",gameswf::as_value(stats.strength))||
               !assign("Stat_Dexterity",gameswf::as_value(stats.dexterity))||
               !assign("Stat_Endurance",gameswf::as_value(stats.endurance))||
               !assign("Stat_Energy",gameswf::as_value(stats.energy))||
               !assign("Stat_Points",gameswf::as_value(stats.points)))return false;
            const auto set_number=[&](const char* key,std::int32_t value){
                return assign(key,gameswf::as_value(value));
            };
            const auto set_flag=[&](const char* key,bool value){
                return assign(key,gameswf::as_value(value));
            };
            if(!set_number("Rating_Attack",stats.rating_attack)||
               !set_number("Rating_Critical",stats.rating_critical)||
               !set_number("Rating_Defense",stats.rating_defense)||
               !set_number("Rating_Dodge",stats.rating_dodge)||
               !set_number("Rating_Block",stats.rating_block)||
               !set_number("Resistance_Fire",stats.resistance_fire)||
               !set_number("Resistance_Earth",stats.resistance_earth)||
               !set_number("Resistance_Water",stats.resistance_water)||
               !set_number("Resistance_Air",stats.resistance_air)||
               !set_number("Resistance_Lightning",stats.resistance_lightning)||
               !set_number("Damage_Min_Main_Hand",stats.damage_min_main_hand)||
               !set_number("Damage_Max_Main_Hand",stats.damage_max_main_hand)||
               !set_number("Damage_Elemental_Min_Main_Hand",stats.damage_elemental_min_main_hand)||
               !set_number("Damage_Elemental_Max_Main_Hand",stats.damage_elemental_max_main_hand)||
               !set_number("Damage_Elemental_Type_Main_Hand",stats.damage_elemental_type_main_hand)||
               !set_number("Damage_Min_Off_Hand",stats.damage_min_off_hand)||
               !set_number("Damage_Max_Off_Hand",stats.damage_max_off_hand)||
               !set_number("Damage_Elemental_Min_Off_Hand",stats.damage_elemental_min_off_hand)||
               !set_number("Damage_Elemental_Max_Off_Hand",stats.damage_elemental_max_off_hand)||
               !set_number("Damage_Elemental_Type_Off_Hand",stats.damage_elemental_type_off_hand)||
               !set_number("Damage_Fire_Min_Main_Hand",stats.damage_fire_min_main_hand)||
               !set_number("Damage_Fire_Max_Main_Hand",stats.damage_fire_max_main_hand)||
               !set_number("Damage_Water_Min_Main_Hand",stats.damage_water_min_main_hand)||
               !set_number("Damage_Water_Max_Main_Hand",stats.damage_water_max_main_hand)||
               !set_number("Damage_Lightning_Min_Main_Hand",stats.damage_lightning_min_main_hand)||
               !set_number("Damage_Lightning_Max_Main_Hand",stats.damage_lightning_max_main_hand)||
               !set_number("Damage_Air_Min_Main_Hand",stats.damage_air_min_main_hand)||
               !set_number("Damage_Air_Max_Main_Hand",stats.damage_air_max_main_hand)||
               !set_number("Damage_Earth_Min_Main_Hand",stats.damage_earth_min_main_hand)||
               !set_number("Damage_Earth_Max_Main_Hand",stats.damage_earth_max_main_hand)||
               !set_flag("IsWeaponTwoHanded",stats.is_weapon_two_handed)||
               !set_flag("HasOffHandWeapon",stats.has_off_hand_weapon)||
               !set_flag("HasStaff",stats.has_staff)||
               !set_flag("HasBow",stats.has_bow)||
               !set_number("Menu_Average_Melee_To_Hit",stats.menu_average_melee_to_hit)||
               !set_number("Physical_Armor",stats.physical_armor)||
               !set_number("Spell_Rating_Dodge",stats.spell_rating_dodge)||
               !set_number("Menu_Melee_Damage_Reduction",stats.menu_melee_damage_reduction)||
               !set_number("Spell_Rating_Critical",stats.spell_rating_critical)||
               !set_number("Menu_Average_Spell_To_Hit",stats.menu_average_spell_to_hit)||
               !set_number("Spell_Damage_Bonus_Fire",stats.spell_damage_bonus_fire)||
               !set_number("Spell_Damage_Bonus_Earth",stats.spell_damage_bonus_earth)||
               !set_number("Spell_Damage_Bonus_Water",stats.spell_damage_bonus_water)||
               !set_number("Spell_Damage_Bonus_Air",stats.spell_damage_bonus_air)||
               !set_number("Spell_Damage_Bonus_Lightning",stats.spell_damage_bonus_lightning)||
               !set_number("Regen_HP",stats.regen_hp)||
               !set_number("Regen_MP",stats.regen_mp)||
               !set_number("Leech_HP",stats.leech_hp)||
               !set_number("Leech_MP",stats.leech_mp)||
               !set_number("Special_Loot_Gold_Multiplier",stats.special_loot_gold_multiplier)||
               !set_number("Special_Loot_Magical_Chance",stats.special_loot_magical_chance)||
               !set_number("Stun_Resist_Chance",stats.stun_resist_chance))return false;
            if(fn.result)fn.result->set_as_object(object);
            return true;
        }
        if(!std::strcmp(name,"NativeStatsAssignPoint")){
            // IDA 0x43eb7c: two numeric arguments [playerIndex, statIndex],
            // then Character::IncStat{Str,Dex,End,Nrg}; invalid arity/types or
            // an unavailable remote player are source no-ops.
            if(fn.nargs!=2||!fn.arg(0).is_number()||!fn.arg(1).is_number())return true;
            const auto raw_stat=fn.arg(1).to_int();
            std::uintptr_t identity=0;
            if(!local_character(fn.arg(0).to_int(),false,identity))return false;
            if(!identity)return true;
            // The source callback still clears its return value after the
            // dispatch switch for an out-of-range stat index. Keep that
            // harmless default case distinct from a malformed call or a
            // missing Player, neither of which writes the result.
            if(raw_stat>=0&&raw_stat<=3&&
               !model_renderer::ui_player_assign_stat(identity,
                    std::uint32_t(raw_stat),error))return false;
            if(fn.result)fn.result->set_undefined();
            return true;
        }
        if(!std::strcmp(name,"NativeSetCurrentQuest")){
            // dqcharmenu btn_Activate and IDA 0x43d3b4:
            // [playerIndex, SelectedQuest.ID]. Keep the authored QuestID
            // order and delegate mutation/persistence to the canonical Save.
            std::array<dh2::native::ui::QuestJournalCallbackValueV1,2> args{};
            for(std::size_t i=0;i<args.size();++i)if(fn.arg(i).is_number()){
                args[i].kind=decltype(args[i].kind)::number;
                args[i].number=fn.arg(i).to_number();
            }
            dh2::native::ui::QuestJournalSetCurrentCallV1 call{};
            if(!dh2::native::ui::decode_quest_journal_set_current_call_v1(
                    args.data(),fn.nargs,call))return true;
            std::int32_t player=0,quest_id=0;
            if(!start_integer(&self,call.player_index,player,error)||
               !start_integer(&self,call.quest_id,quest_id,error))return false;
            std::uintptr_t identity=0;
            if(!local_character(player,false,identity))return false;
            if(!identity)return true;
            return model_renderer::ui_player_set_current_quest(identity,quest_id,error);
        }
        const ui::SwfMenuNavigationServicesV1 navigation{
            context,push_menu,pop_menu,pop_top_menu,pop_above_menu};
        if(!std::strcmp(name,"NativePushState")){
            // Original 0x43aebc accepts exactly one STRING/OBJECT target.
            // menu_CharacterMenu and menu_Ingame route through GSFlashMenu
            // only while GSLevel is current in the StateMachine. The native
            // state wrapper is unavailable in this port, so its existing
            // retained gameplay menu owner performs the authored target's
            // transition/lifecycle. This is separate from the SWF's direct
            // NativePushMenu API, which always enters MenuManager::PushMenu.
            if(fn.nargs!=1||!fn.env||
               (!fn.arg(0).is_string()&&!fn.arg(0).is_object()))return true;
            const std::string requested=fn.arg(0).to_xstring();
            const bool level_state_active=self.live_player&&self.front_screen.empty();
            if(ui::gameplay_native_push_state_is_level_menu_target_v1(requested)){
                if(!ui::gameplay_native_push_state_routes_to_level_menu_v1(
                       requested,level_state_active)){
                    __android_log_print(ANDROID_LOG_INFO,tag,
                        "NativePushState source no-op | target %s | GSLevel absent",
                        requested.c_str());
                    return true;
                }
                __android_log_print(ANDROID_LOG_INFO,tag,
                    "NativePushState GSFlashMenu overlay | target %s | active state GSLevel",
                    requested.c_str());
                return push_game_menu(context,requested.c_str(),error);
            }
            error="NativePushState requires the original Application StateMachine owner for target: ";
            error+=requested;
            return false;
        }
        if(!std::strcmp(name,"NativePushMenu")){
            return ui::swf_menu_push(fn,navigation,error);
        }
        if(!std::strcmp(name,"NativePopMenu"))
            return ui::swf_menu_pop(fn,navigation,error);
        if(!std::strcmp(name,"NativePopAllAbove"))return ui::swf_menu_pop_above(fn,navigation,error);
        if(!std::strcmp(name,"NativeGetCreditMovement")){
            // 0x43ccec: HTC_DEVICES ? 2 : unsigned(Application.GetDt)/25+1.
            // This isolated emulator profile is non-HTC; integer menu dt is
            // the same live clock passed to both retained renderer timelines.
            return ui::swf_menu_credit_movement(fn,std::uint32_t(self.last_menu_dt),false,error);
        }
        if(!std::strcmp(name,"NativeBackToHud")){
            // Calling DisplayRightHud here re-enters ActionScript while the
            // current GameSWF event still owns its core lock (SWF core busy).
            // Preserve callback success and apply the HUD owner transition at
            // the first post-input-dispatch safe point in OriginalUiSession::touch.
            self.gameplay_back_to_hud_pending=true;
            self.gameplay_native_back_to_hud_pending=true;
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeGoToMainMenu")){
            // Application::GoToMainMenu removes the live Player/Character
            // owners. The existing JNI returnToMainMenu path performs that
            // canonical teardown, but only after this SWF callback unwinds.
            if(self.front_screen.empty()&&self.live_player)
                self.gameplay_main_menu_request.request();
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeAwayFromHud")){
            // IDA 0x43ab98: when a live Level exists, clear its HUD-active
            // byte and reset ZoomHandler's finger map plus the menu cursor.
            // The port's authoritative equivalent is the attached HUD owner;
            // defer its cursor reset until the native SWF event returns.
            if(self.front_screen.empty()&&self.live_player){
                __android_log_print(ANDROID_LOG_INFO,tag,
                    "Gameplay HUD transition | NativeAwayFromHud | active before %d | menu depth %zu",
                    self.gameplay_hud_active,self.game_menu_stack.size());
                self.gameplay_hud_active=false;
                self.gameplay_hud_input_reset_pending=true;
            }
            return true;
        }
        if(!std::strcmp(name,"NativeReviveAllPlayers")){
            if(!self.live_player||!self.front_screen.empty()){
                error="NativeReviveAllPlayers requires the attached gameplay session";return false;
            }
            if(fn.nargs!=0){error="NativeReviveAllPlayers takes no authored arguments";return false;}
            if(!self.runtime.revive_local_players){
                error="NativeReviveAllPlayers source host is not bound";return false;
            }
            self.gameplay_native_revive_pending=true;
            error.clear();return true;
        }
        if(!std::strcmp(name,"NativeShowStatusBar")){
            // IDA 0x43aa18 converts the authored visibility argument and calls
            // Application::ShowStatubBar (0x31f668), which is a source no-op.
            // Keep the menu's onShow transition successful without inventing
            // a second status-bar owner.
            if(fn.nargs>0)(void)fn.arg(0).to_bool();
            return true;
        }
        if(!std::strcmp(name,"NativeShowMinimapLegend")){
            // IDA 0x43aa58 stores arg0 in MenuCharMenu_Map+0x1e4. No reads
            // of that byte exist in the map class; the authored SWF owns the
            // LegendPopup show/hide timeline, so do not add a second flag.
            if(fn.nargs>0)(void)fn.arg(0).to_bool();
            return true;
        }
        if(!std::strcmp(name,"NativeScreenIsBlack")){
            // IDA 0x439fa4: offline is a no-op; only the unavailable online
            // PlayerManager branch advances its screen state from 1 to 2.
            return true;
        }
        if(!std::strcmp(name,"NativeIsMultiplayerEnabled")||
           !std::strcmp(name,"NativeIsMultiplayerGame")||
           !std::strcmp(name,"NativeHasPushNotification")){
            // NativeIsMultiplayerGame reads GetOnline()+5 (IDA 0x439fe8);
            // all three services are unavailable in this offline build.
            if(fn.result)fn.result->set_bool(false);
            return true;
        }
        if(!std::strcmp(name,"NativeStartFromGCInvite")){
            // The original invite path returns bool and may assign a save slot
            // or enter online verification. With no invite/Game Center owner,
            // take its source-compatible false result and leave player state
            // untouched so the ordinary single-player flow can continue.
            if(fn.result)fn.result->set_bool(false);
            return true;
        }
        if(!std::strcmp(name,"NativeUseIpodPlayer")){
            // NativeUseIpodPlayer returns nativeIsSupportMM(). This build has
            // no external-media player service, so report unsupported rather
            // than aborting the authored HUD's first frame.
            if(fn.result)fn.result->set_bool(false);
            return true;
        }
        if(!std::strcmp(name,"NativeUpdateOrientation")){
            // The native resets to its saved AutoOrientation option. This
            // Android activity already owns the landscape orientation policy.
            return true;
        }
        if(!std::strcmp(name,"NativePauseAllSounds")||
           !std::strcmp(name,"NatvieResumeAllSounds")||
           !std::strcmp(name,"NativePauseMusic")){
            // Keep game-authored controls separate from Activity lifecycle
            // pause/resume so game resume cannot revive background audio.
            if(!std::strcmp(name,"NativePauseAllSounds"))
                self.menu_audio.emplace_back("pause-all");
            else if(!std::strcmp(name,"NatvieResumeAllSounds"))
                self.menu_audio.emplace_back("resume-all");
            else
                self.menu_audio.emplace_back("pause-music");
            return true;
        }
        if(!std::strcmp(name,"NativePopAllMenus")){
            if(self.live_player&&self.front_screen.empty()){
                while(!self.game_menu_stack.empty())if(!pop_top_menu(context,error))return false;
                return true;
            }
            while(self.menu_stack.size()>1)if(!pop_top_menu(context,error))return false;
            return true;
        }
        if(!std::strcmp(name,"NativeGetOptionParameters")||
           !std::strcmp(name,"NativeSetOptions")||
           !std::strcmp(name,"NativeLoadSettings")||
           !std::strcmp(name,"NativeSaveSettings")||
           !std::strcmp(name,"NativeEnterOptionMenu")||
           !std::strcmp(name,"NativeRefreshHudManager")||
           !std::strcmp(name,"NativeIsJapaneseVersion")||
           !std::strcmp(name,"NativeIsKorean")||
           !std::strcmp(name,"NativeChangeRolloverInputBehavior")){
            auto options=self.option_services();
            return ui::swf_menu_settings_action(name,fn,options,error);
        }
        if(std::strcmp(name,"NativePlaySoundFX")){
            error="Unknown owned menu native action: ";
            error+=name?name:"(null)";
            return false;
        }
        // Original 0x43ae10: exactly one STRING/WIDE_STRING, lookup by name;
        // invalid arguments or absent ID are a genuine no-op. This core uses
        // its UTF-8 STRING representation (it has no separate wide-string tag).
        std::string requested;
        if(!ui::swf_menu_sound_argument(fn,requested))return true;
        for(std::size_t id=0;id<std::size(original_sounds);++id){
            const auto& record=original_sounds[id];
            if(requested!=record.name)continue;
            if(!record.menu_backend){
                // The authored gameplay movie invokes NativePlaySoundFX for HUD,
                // quest and item-detail cues (for example QuestNew, PickupWeapon).
                // 0x43ae10 resolves the name in the SDD Sounds table and calls
                // VoxSoundManager::PlayMenu: the shared gameplay audio queue
                // carries it to the platform effect backend. A source row whose
                // PCM is not in the supplied cache stays the original's silent
                // no-op; it must never fail the SWF event or its page transition.
                switch(dh2::player_gameplay_audio::enqueue_source_menu_sound(requested)){
                case dh2::player_gameplay_audio::MenuSoundResultV1::queued:
                    __android_log_print(ANDROID_LOG_INFO,tag,
                        "Original SoundFX queued | name %s | file %s",record.name,record.file);
                    break;
                case dh2::player_gameplay_audio::MenuSoundResultV1::rejected:
                    __android_log_print(ANDROID_LOG_WARN,tag,
                        "Original SoundFX dropped | name %s | gameplay audio queue full",record.name);
                    break;
                case dh2::player_gameplay_audio::MenuSoundResultV1::unavailable:
                case dh2::player_gameplay_audio::MenuSoundResultV1::unknown:
                    __android_log_print(ANDROID_LOG_INFO,tag,
                        "Original SoundFX skipped | name %s | file %s | no retained PCM for this source row",
                        record.name,record.file);
                    break;
                }
                return true;
            }
            self.menu_sounds.emplace_back(record.file);
            __android_log_print(ANDROID_LOG_INFO,tag,"Original menu sound requested | name %s | id %zu | file %s",requested.c_str(),id,record.file);
            return true;
        }
        return true;
    }
    enum class MenuChangeAction { cover, hide, show };
    struct MenuChange {Impl* self;std::string name;MenuChangeAction action;bool pushed;};
    static bool render_class_scene(void*,int width,int height,std::string& error){
        try{model_renderer::draw_class_scene(width,height);return true;}
        catch(const std::exception& e){error=e.what();return false;}
    }
    static bool render_map_scene(void*,int width,int height,std::string& error){
        return model_renderer::draw_map_scene(width,height,error);
    }
    static bool class_pane(void* context,const ui::SwfDraw& pane,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return self.gpu.scene_pane(pane.rect,&self,render_class_scene,error);
    }
    static bool map_pane(void* context,const ui::SwfDraw& pane,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return self.gpu.scene_pane(pane.rect,&self,render_map_scene,error);
    }
    static bool update_class(void* context,ui::SwfAsGraph& graph,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        const auto* choice=ui::menu_class_choice_v1(self.class_index);
        if(!choice){error="Original class selection index outside its three source classes";return false;}
        ui::SwfAsValue root,menu,result;bool callable=false,accepted=false;
        if(!graph.root_value(root,error)||!graph.find_target(root,"menu_SelectClass",menu,error))return false;
        const char* paths[]={"menu_SelectClass.class_title.text","menu_SelectClass.class_description.text"};
        const char* symbols[]={choice->title_symbol,choice->description_symbol};
        for(unsigned i=0;i<2;++i){
            ui::SwfAsValue field;std::uint32_t id=0;std::string text;
            if(!graph.find_target(root,paths[i],field,error)||!field.identity()){error="Required original class text field absent";return false;}
            if(!constant(&self,"StrID",symbols[i],id,error)||!self.localization.string_id(id,self.text_services(),text,error))return false;
            // Update 0x428540/0x42859c calls FormatHTML("%s", getString).
            // FormatHTML 0x7a947c calls SetText(..., true); htmlText uses
            // the retained original HTML parser and real field layout.
            if(!graph.set_member(field,"htmlText",ui::SwfAsValue::text(text.c_str()),accepted,error))return false;
        }
        for(unsigned i=0;i<2;++i){
            ui::SwfAsValue button;
            if(!graph.find_target(root,i?"menu_SelectClass.btn_right":"menu_SelectClass.btn_left",button,error)||!button.identity()){error="Required class selector button absent";return false;}
            (i?self.class_right:self.class_left)=button.identity();
            if(!graph.set_member(button,"_visible",ui::SwfAsValue::boolean(i?self.class_index<2:self.class_index>0),accepted,error))return false;
        }
        if(!graph.invoke(menu,menu,"CurrentClass",{ui::SwfAsValue::text(choice->profile_character)},result,callable,error))return false;
        if(!callable){error="Required original CurrentClass callback absent";return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Original class selection updated | index %d | class %s | preview actors pending",self.class_index,choice->profile_character);
        return true;
    }
    static bool change_menu(void* context,ui::SwfAsGraph& graph,std::string& error){
        auto& change=*static_cast<MenuChange*>(context);
        ui::SwfAsValue root,menu,result;bool callable=false,accepted=false;
        if(!graph.root_value(root,error)||!graph.find_target(root,change.name.c_str(),menu,error))return false;
        if(!menu.identity()){error="Required authored navigation clip absent";return false;}
        if(change.action!=MenuChangeAction::show){
            if(!graph.invoke(menu,menu,"onHide",{},result,callable,error))return false;
            if(change.name=="menu_MapSheet")model_renderer::map_camera_close();
            if(change.action==MenuChangeAction::cover)return true;
            if(change.name=="menu_Merchant")change.self->merchant_sell_one_shot_used=false;
            return graph.set_member(menu,"_visible",ui::SwfAsValue::boolean(false),accepted,error);
        }
        if(!graph.set_member(menu,"_visible",ui::SwfAsValue::boolean(true),accepted,error)||
           !change.self->menu_movie(change.name)->menu_input_context(("_root."+change.name).c_str(),error))return false;
        if(change.name=="menu_EnterName"||change.name=="menu_SelectClass"||
           change.name=="menu_confirm2"||change.name=="menu_FadeToBlackScreen"||
           change.name=="menu_FadeFromBlackScreen"){
            // Original MultiMenuManager::PushMenu 0x438488 invokes onPush,
            // then 0x4385f0..0x43860c plays the authored "show" animation
            // for the ordinary (non-0x40) renderer before MenuBase::Show.
            // The name menu's frame 15 action clears its actual name field.
            if(change.pushed&&!graph.invoke(menu,menu,"onPush",{},result,callable,error))return false;
            if(!graph.invoke(menu,menu,"gotoAndPlay",{ui::SwfAsValue::text("show")},result,callable,error))return false;
            if(!callable){error="Authored menu show timeline unavailable";return false;}
            if(!graph.invoke(menu,menu,"onShow",{},result,callable,error))return false;
            if(change.name=="menu_SelectClass"){
                // Show resets the previous index (0x428fe8), retaining the
                // current selection. The singleton constructor starts at 0.
                if(!update_class(change.self,graph,error)||
                   !change.self->movie->menu_display_callback("_root.menu_SelectClass.class_select",change.self,class_pane,error))return false;
            }
            return true;
        }
        if(change.pushed&&!graph.invoke(menu,menu,"onPush",{},result,callable,error))return false;
        if(!graph.invoke(menu,menu,"onShow",{},result,callable,error))return false;
        if(change.name=="menu_MapSheet"){
            // Source MenuCharMenu_Map::Show calls ShowLevelName after opening.
            // That reads the active LevelList row's +0x24 string ID, resolves
            // it through StringManager, and writes the MapName text field.
            const auto string_id=model_renderer::ui_current_level_name_id();
            if(string_id>=0){
                std::string text;ui::SwfAsValue field;
                if(!change.self->localization.string_id(static_cast<std::uint32_t>(string_id),
                        change.self->text_services(),text,error))return false;
                if(!graph.find_target(root,"menu_MapSheet.MapName",field,error)||!field.identity()){
                    error="Required authored map title field absent";return false;
                }
                if(!graph.set_member(field,"htmlText",ui::SwfAsValue::text(text.c_str()),accepted,error))return false;
                __android_log_print(ANDROID_LOG_INFO,tag,
                    "Original map level title updated | row string id %d | text %s",string_id,text.c_str());
            }
            if(!model_renderer::map_camera_open(error))return false;
            if(!change.self->menu_movie(change.name)->menu_display_callback(
                    "_root.menu_MapSheet.RenderMap",change.self,map_pane,error))return false;
        }
        return true;
    }
    bool transition_menu(const std::string& previous,const std::string& next,bool pushed,std::string& error){
        // MenuMainMenu::Show can synchronously call NativeSetSaveSlotIDToMainMenu.
        // Class-scene loading clears the menu preview's borrowed AAssetManager,
        // so restore the real main-menu scene before dispatching that callback.
        if(next=="menu_MainMenu"&&
           (!model_renderer::active()||model_renderer::class_scene_active())){
            const auto scene=model_renderer::load_menu_background(manager);
            if(scene.find("3D upload OK")!=0){error=scene;return false;}
        }
        // Source MenuMainMenu::Show calls SetupCharacter after the scene is
        // ready. Class selection tears down this visual resource, so restore
        // the retained source-selected slot before the MainMenu SWF is shown.
        if(next=="menu_MainMenu"&&!model_renderer::restore_menu_preview(error))return false;
        MenuChange hide{this,previous,MenuChangeAction::hide,false},show{this,next,MenuChangeAction::show,pushed};
        if(!menu_movie(previous)->menu_action_script(&hide,change_menu,error)||
           !menu_movie(next)->menu_action_script(&show,change_menu,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu renderer selected | name %s | renderer %s",next.c_str(),shared_state(next)?"shared":"main");
        return true;
    }
    bool transition_game_menu(const std::string& previous,const std::string& next,bool pushed,std::string& error){
        for(const auto& event:ui::gameplay_menu_transition_plan_v1(previous,next,pushed)){
            const auto action=event.action==ui::GameplayMenuTransitionActionV1::cover?MenuChangeAction::cover:
                event.action==ui::GameplayMenuTransitionActionV1::hide?MenuChangeAction::hide:MenuChangeAction::show;
            MenuChange change{this,event.menu,action,event.pushed};
            auto* renderer=menu_movie(event.menu);
            if(!renderer){error="Gameplay menu renderer unavailable";return false;}
            if(!renderer->menu_action_script(&change,change_menu,error))return false;
        }
        if(next.empty()&&!gameplay_back_to_hud_draining_stack&&
           live_player&&movie&&gameplay_hud){
            const auto hud_path=gameplay_hud->menu_path();
            if(!movie->menu_input_context(hud_path.c_str(),error))return false;
            // The authored Character/Skills Back action can empty the menu
            // stack through generic PopAllMenus without dispatching the
            // NativeBackToHud callback. The source callback's remaining work
            // is DisplayRightHud plus input reset; defer that same existing
            // owner transition until the current SWF dispatch has unwound.
            gameplay_back_to_hud_pending=true;
        }
        __android_log_print(ANDROID_LOG_INFO,tag,"Original gameplay menu selected | previous %s | current %s | depth %zu",
            previous.empty()?"<hud>":previous.c_str(),next.empty()?"<hud>":next.c_str(),game_menu_stack.size());
        return true;
    }
    static bool push_game_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!name){error="Missing gameplay menu name";return false;}
        if(!gameplay_menu_state(name)){
            __android_log_print(ANDROID_LOG_WARN,tag,"Gameplay menu push not connected | requested %s",name);
            error=std::string("Gameplay menu state is not connected: ")+name;return false;
        }
        const auto duplicate=std::find(self.game_menu_stack.begin(),self.game_menu_stack.end(),name);
        std::ostringstream stack;
        for(std::size_t i=0;i<self.game_menu_stack.size();++i){if(i)stack<<" > ";stack<<self.game_menu_stack[i];}
        const auto stack_text=stack.str();
        __android_log_print(ANDROID_LOG_INFO,tag,
            "Original gameplay menu push request | requested %s | depth %zu | stack %s | duplicate %d",
            name,self.game_menu_stack.size(),stack_text.empty()?"<empty>":stack_text.c_str(),
            duplicate!=self.game_menu_stack.end());
        if(duplicate!=self.game_menu_stack.end()){
            if(!ui::gameplay_menu_reveal_existing_requested_v1(name))return true;
            const auto index=static_cast<std::size_t>(duplicate-self.game_menu_stack.begin());
            if(!ui::gameplay_menu_reveal_existing_v1(self.game_menu_stack,name,[&]{
                return pop_top_menu(context,error);
            }))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Original gameplay menu resumed | requested %s | prior stack index %zu | depth %zu",
                name,index,self.game_menu_stack.size());
            return true;
        }
        const auto previous=self.game_menu_stack.empty()?std::string():self.game_menu_stack.back();
        self.game_menu_stack.emplace_back(name);
        if(!self.transition_game_menu(previous,name,true,error)){self.game_menu_stack.pop_back();return false;}
        return true;
    }
    static bool push_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(self.live_player&&self.front_screen.empty())return push_game_menu(context,name,error);
        if(!name||self.menu_stack.empty()){error="Native menu stack unavailable";return false;}
        // GetMenuByName returns null for unregistered states. Explicitly log
        // the partial registration rather than presenting those paths as done.
        if(std::strcmp(name,"menu_info")&&std::strcmp(name,"menu_MainMenu")&&std::strcmp(name,"menu_EnterName")&&std::strcmp(name,"menu_SelectClass")&&std::strcmp(name,"menu_StartGame")&&!shared_state(name)){
            __android_log_print(ANDROID_LOG_WARN,tag,"Menu navigation not connected | requested %s",name);
            error=std::string("Menu state is not connected: ")+name;return false;
        }
        if(std::find(self.menu_stack.begin(),self.menu_stack.end(),name)!=self.menu_stack.end())return true;
        const auto previous=self.menu_stack.back();
        self.menu_stack.emplace_back(name);
        if(!self.transition_menu(previous,name,true,error)){self.menu_stack.pop_back();return false;}
        self.front_name_input_active.store(std::strcmp(name,"menu_EnterName")==0,
                                           std::memory_order_release);
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu navigation | push %s | depth %zu",name,self.menu_stack.size());
        return true;
    }
    static bool pop_top_menu(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(self.live_player&&self.front_screen.empty()){
            if(self.game_menu_stack.empty())return true;
            const auto previous=self.game_menu_stack.back();self.game_menu_stack.pop_back();
            const auto next=self.game_menu_stack.empty()?std::string():self.game_menu_stack.back();
            if(!self.transition_game_menu(previous,next,false,error)){self.game_menu_stack.push_back(previous);return false;}
            return true;
        }
        if(self.menu_stack.size()<2)return true;
        const auto previous=self.menu_stack.back();self.menu_stack.pop_back();
        if(!self.transition_menu(previous,self.menu_stack.back(),false,error)){self.menu_stack.push_back(previous);return false;}
        self.front_name_input_active.store(self.menu_stack.back()=="menu_EnterName",std::memory_order_release);
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu navigation | pop %s | current %s | depth %zu",previous.c_str(),self.menu_stack.back().c_str(),self.menu_stack.size());
        return true;
    }
    static bool pop_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(self.live_player&&self.front_screen.empty()){
            if(name&&ui::gameplay_menu_named_pop_requested_v1(self.game_menu_stack,name))return pop_top_menu(context,error);
            return true;
        }
        if(name&&ui::gameplay_menu_named_pop_requested_v1(self.menu_stack,name))return pop_top_menu(context,error);
        return true;
    }
    static bool back_to_hud(void* context,std::string& error,bool authored_native=false){
        auto& self=*static_cast<Impl*>(context);
        error.clear();
        // 0x4449ac..0x4449b4 exits when Application.GetCurrentLevel is null.
        // Android system Back shares this same live level/menu-stack owner.
        if(self.front_screen=="main"&&!self.live_player)return true;
        if(self.front_screen.empty()&&self.live_player){
            // Android Back dismisses the top authored overlay first. In
            // particular, menu_confirm2 must return to the Inventory/Talent
            // caller instead of abandoning the whole gameplay menu stack.
            // The root CharacterMenu still follows the source BackToHud path.
            if(ui::gameplay_menu_back_dismisses_overlay_v1(
                   self.game_menu_stack,authored_native)){
                const auto dismissed=self.game_menu_stack.back();
                if(!pop_top_menu(context,error))return false;
                __android_log_print(ANDROID_LOG_INFO,tag,
                    "Gameplay menu Back | dismissed %s | current %s | depth %zu | HUD remains inactive",
                    dismissed.c_str(),self.game_menu_stack.back().c_str(),self.game_menu_stack.size());
                return true;
            }
            const auto prior_depth=self.game_menu_stack.size();
            self.gameplay_back_to_hud_draining_stack=true;
            while(!self.game_menu_stack.empty()){
                if(!pop_top_menu(context,error)){
                    self.gameplay_back_to_hud_draining_stack=false;
                    return false;
                }
            }
            self.gameplay_back_to_hud_draining_stack=false;
            self.gameplay_hud_active=true;
            // NativeBackToHud calls the authored root DisplayRightHud before
            // resetting input. That callback runs the selected HUD's onPush/
            // onShow path (including NativeReloadSkills); reuse the already
            // bound movie and attached Player rather than only exposing a
            // stale HUD projection after leaving the Character/Skills menu.
            if(!self.gameplay_hud||!self.gameplay_hud_bound){
                error="NativeBackToHud requires the bound authored gameplay HUD";return false;
            }
            if(!self.gameplay_hud->activate(error))return false;
            self.gameplay_hud_input_reset_pending=true;
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Gameplay HUD transition | BackToHud | active 1 | prior menu depth %zu | reset pending 1",
                prior_depth);
            return true;
        }
        error="BackToHud requires the attached gameplay level owner";return false;
    }
    static bool flush_deferred_back_to_hud(Impl& self,std::string& error){
        if(self.gameplay_back_to_hud_pending){
            self.gameplay_back_to_hud_pending=false;
            const bool authored_native=self.gameplay_native_back_to_hud_pending;
            self.gameplay_native_back_to_hud_pending=false;
            if(!back_to_hud(&self,error,authored_native)){
                // Keep a transient SWF-core contention queued for the next safe
                // renderer boundary; permanent owner errors still fail the caller.
                if(error=="SWF core busy"){
                    self.gameplay_back_to_hud_pending=true;
                    self.gameplay_native_back_to_hud_pending=authored_native;
                    error.clear();return true;
                }
                return false;
            }
        }
        if(!self.gameplay_hud_refresh.display_right_hud_pending&&
           !self.gameplay_hud_refresh.fill_action_icon_pending){error.clear();return true;}
        if(!self.gameplay_hud||!self.gameplay_hud_bound){
            error="Deferred equipment HUD refresh requires the bound authored gameplay HUD";return false;
        }
        return ui::flush_deferred_hud_refresh_v1(self.gameplay_hud_refresh,
            [&](std::string& nested){return self.gameplay_hud->activate(nested);},
            [&](std::string& nested){return self.gameplay_hud->refresh_action_icon(nested);},
            error);
    }
    static bool flush_deferred_faery_hud_skills(Impl& self,std::string& error){
        if(!self.gameplay_hud_skills_refresh.pending){error.clear();return true;}
        if(!self.gameplay_hud||!self.gameplay_hud_bound){
            // The selected gameplay HUD may have been torn down before the
            // queued refresh reaches a safe boundary. There is no retained HUD
            // movie to refresh in that lifetime.
            self.gameplay_hud_skills_refresh.pending=false;
            error.clear();return true;
        }
        return self.gameplay_hud_skills_refresh.flush(
            [&](std::string& nested){return self.gameplay_hud->refresh_skills(nested);},
            error);
    }
    static bool flush_deferred_native_revive(Impl& self,std::string& error){
        if(!self.gameplay_native_revive_pending){error.clear();return true;}
        if(!self.runtime.revive_local_players){
            error="NativeReviveAllPlayers source host is not bound";return false;
        }
        self.gameplay_native_revive_pending=false;
        // The source native first removes the authored death fade, then pushes
        // its return fade, and only then calls PlayerManager::ReviveLocalPlayers.
        if(!pop_menu(&self,"menu_FadeToBlackScreen",error)||
           !push_game_menu(&self,"menu_FadeFromBlackScreen",error)){
            if(error=="SWF core busy"){
                self.gameplay_native_revive_pending=true;error.clear();return true;
            }
            return false;
        }
        if(!self.runtime.revive_local_players(self.runtime.context,error))return false;
        // PlayerManager::ReviveLocalPlayers 0x37565c restores the SFX group gain
        // to the saved VolumeFX option right after Level::LoadCheckpoint.
        if(self.settings)self.queue_volumes();
        return true;
    }
    static bool pop_above_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!name){error="Missing pop-above menu name";return false;}
        if(self.live_player&&self.front_screen.empty()){
            while(std::find(self.game_menu_stack.begin(),self.game_menu_stack.end(),name)!=self.game_menu_stack.end()&&self.game_menu_stack.back()!=name)
                if(!pop_top_menu(context,error))return false;
            error.clear();return true;
        }
        // Original 0x4392b8 checks IsStateInStack on every iteration. Unknown
        // or absent targets and an already-current target are genuine no-ops.
        while(std::find(self.menu_stack.begin(),self.menu_stack.end(),name)!=self.menu_stack.end()&&self.menu_stack.back()!=name){
            if(!pop_top_menu(context,error))return false;
        }
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu navigation | pop above %s | current %s | depth %zu",name,self.menu_stack.empty()?"":self.menu_stack.back().c_str(),self.menu_stack.size());
        error.clear();return true;
    }
    static bool show_main(void*,ui::SwfAsGraph& graph,std::string& error) {
        ui::SwfAsValue root,menu,result;bool callable=false;
        if(!graph.root_value(root,error)||!graph.find_target(root,"menu_MainMenu",menu,error))return false;
        if(!model_renderer::restore_menu_preview(error))return false;
        if(!graph.invoke(menu,menu,"onShow",{},result,callable,error))return false;
        if(!callable){error="Authored main menu onShow missing";return false;}
        // The authored onShow may select the retained profile through
        // NativeSetSaveSlotIDToMainMenu. Recheck afterward, matching the
        // source Show path without creating a synthetic slot selection.
        return model_renderer::restore_menu_preview(error);
    }
    static bool verify_native_skill_menu_runtime(void*,ui::SwfAsGraph& graph,std::string& error){
        ui::SwfAsValue global,version;bool found=false;std::string value;
        if(!graph.global_value(global,error)||!graph.get_member(global,"$version",version,found,error)||
           !found||!graph.to_text(version,value,error))return false;
        if(value!="gameSWF"){
            error="Character menu SWF did not retain the native skill runtime version before frame 0";
            return false;
        }
        return true;
    }
    static bool probe_main_background(void*,ui::SwfAsGraph& graph,std::string& error){
        ui::SwfAsValue root;if(!graph.root_value(root,error))return false;
        for(const char* path:{"menu_bg","menu_bg.BrownBG","menu_bg.RenderedBG","menu_bg.TitleGraphic"}){
            ui::SwfAsValue clip,value;bool found=false;
            if(!graph.find_target(root,path,clip,error))return false;
            if(!clip.identity()){__android_log_print(ANDROID_LOG_INFO,tag,"Original background clip absent | path %s",path);continue;}
            for(const char* member:{"_visible","_alpha","_currentframe"}){
                if(!graph.get_member(clip,member,value,found,error))return false;
                double number=0;if(found&&!graph.to_number(value,number,error))return false;
                __android_log_print(ANDROID_LOG_INFO,tag,"Original background property | path %s | member %s | found %d | value %.3f",path,member,found,number);
            }
        }
        return true;
    }
    static bool graph_start(void* context,const ui::SwfAsLease& lease,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.frame_owner||!lease.player){error="Original UI frame owner unavailable";return false;}
        if(!self.frame_owner->history->bind(lease.player,error)||
           !self.frame_owner->frames.bind(lease.player,self.frame_owner->history,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original source frame/history bound before shared/root construction");
        return true;
    }
    static bool character_graph_start(void* context,const ui::SwfAsLease& lease,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.character_frame_owner||!lease.player){error="Original character-menu frame owner unavailable";return false;}
        if(!self.character_frame_owner->history->bind(lease.player,error)||
           !self.character_frame_owner->frames.bind(lease.player,self.character_frame_owner->history,error))return false;
        auto* global=lease.player->get_global();
        if(!global){error="Character menu global is unavailable before SWF frame 0";return false;}
        // `$version` is a built-in property backed by a getter with no setter.
        // Replacing it through set_member runs that getter-property setter;
        // use GameSWF's direct constructor registration path before frame 0.
        global->builtin_member("$version",gameswf::as_value("gameSWF"));
        gameswf::as_value version;
        if(!global->get_member("$version",&version)||std::strcmp(version.to_string(),"gameSWF")){
            error="Character menu global rejected the native skill runtime version before frame 0";
            return false;
        }
        __android_log_print(ANDROID_LOG_INFO,tag,"Original character-menu frame/history bound before SWF construction");
        return true;
    }
    static bool shared_graph_start(void* context,const ui::SwfAsLease& lease,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.shared_frame_owner||!lease.player){error="Shared menu frame owner unavailable";return false;}
        if(!self.shared_frame_owner->history->bind(lease.player,error)||
           !self.shared_frame_owner->frames.bind(lease.player,self.shared_frame_owner->history,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original shared renderer frame/history bound before root construction");
        return true;
    }
    bool ensure_localization_ready(std::string& error) {
        if(localization.ready()){error.clear();return true;}
        std::array<std::vector<std::uint8_t>,4> next_constant_bytes;
        std::array<dh2_pycst_view,4> next_constants{};
        const char* constant_uris[]={"data/pydata/common_text_pycst.bin","data/fonts_pycst.bin",
            "data/design_pycst.bin","source-data/font-palette/loot_audiovisual_pycst.bin"};
        for(unsigned i=0;i<4;++i){
            if(i==0){if(!assets.read(constant_uris[i],next_constant_bytes[i],error))return false;}
            else if(!raw_asset(constant_uris[i],next_constant_bytes[i],error))return false;
            if(dh2_pycst_open(&next_constants[i],next_constant_bytes[i].data(),
                static_cast<std::uint32_t>(next_constant_bytes[i].size()))!=0){
                error="Required UI constants load failed";return false;
            }
        }
        std::array<std::vector<std::uint8_t>,3> metadata;
        const char* metadata_uris[]={"data/pydata/common_text_pyarray.bin",
            "data/pydata/common_text_pyarraynames.bin","data/pydata/common_text_pystructnames.bin"};
        for(unsigned i=0;i<3;++i)if(!assets.read(metadata_uris[i],metadata[i],error))return false;
        ui::Localization next_localization;
        if(!next_localization.load({metadata[0].data(),metadata[0].size()},
            {metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
        constant_bytes=std::move(next_constant_bytes);
        constants=next_constants;
        localization=std::move(next_localization);
        error.clear();return true;
    }
    bool ensure_dialog_metadata_ready(std::string& error) {
        if(dialog_constants.bytes&&!dialog_actor_name_ids.empty()&&
           source_dialog_table_loaded){
            error.clear();return true;
        }
        std::vector<std::uint8_t> records,names,style_bytes;
        if(!assets.read("data/pydata/dialogs_pyarray.bin",records,error)||
           !assets.read("data/pydata/dialogs_pyarraynames.bin",names,error)||
           !assets.read("data/pydata/dialogs_pycst.bin",style_bytes,error))return false;
        const auto word=[](const std::vector<std::uint8_t>& bytes,std::size_t at,
                           std::uint32_t& value)->bool{
            if(at>bytes.size()||bytes.size()-at<4)return false;
            value=std::uint32_t(bytes[at])|(std::uint32_t(bytes[at+1])<<8)|
                  (std::uint32_t(bytes[at+2])<<16)|(std::uint32_t(bytes[at+3])<<24);
            return true;
        };
        std::size_t name_at=0,record_at=0;
        const auto parse_names=[&](std::vector<std::string>& parsed)->bool{
            std::uint32_t count=0;
            if(!word(names,name_at,count)||count>4096)return false;
            name_at+=4;parsed.clear();parsed.reserve(count);
            for(std::uint32_t i=0;i<count;++i){
            std::uint32_t length=0;
            if(!word(names,name_at,length)||!length||length>256||
               name_at>names.size()||names.size()-name_at<4||
               length>names.size()-(name_at+4))return false;
            name_at+=4;
            std::string actor(reinterpret_cast<const char*>(names.data()+name_at),length);
            if(actor.find('\0')!=std::string::npos||
               std::any_of(actor.begin(),actor.end(),[](unsigned char c){return c<32||c>126;})){
                return false;
            }
            parsed.push_back(std::move(actor));name_at+=length;
            }
            return true;
        };
        std::vector<std::string> actor_names,dialog_names;
        if(!parse_names(actor_names)||!parse_names(dialog_names)||name_at!=names.size()){
            error="Dialogs source name tables are truncated or malformed";return false;
        }
        std::uint32_t count=0;
        if(!word(records,record_at,count)||count!=actor_names.size()||
           !count||count>4096){error="DialogActors source table dimensions differ";return false;}
        record_at+=4;
        std::vector<std::int32_t> actor_ids;actor_ids.reserve(count);
        for(std::uint32_t i=0;i<count;++i){
            std::uint32_t frame=0,label_size=0,name_id=0;
            if(!word(records,record_at,frame)||!word(records,record_at+4,label_size)||
               label_size>4096||record_at>records.size()||records.size()-record_at<8||
               label_size>records.size()-(record_at+8)){
                error="DialogActors source record is truncated";return false;
            }
            (void)frame;
            record_at+=8+label_size;
            if(!word(records,record_at,name_id)||name_id>std::uint32_t(std::numeric_limits<std::int32_t>::max())){
                error="DialogActors StringManager ID is outside source range";return false;
            }
            actor_ids.push_back(static_cast<std::int32_t>(name_id));record_at+=4;
        }
        std::uint32_t dialog_count=0;
        if(!word(records,record_at,dialog_count)||dialog_count!=dialog_names.size()||
           dialog_count>4096){error="Dialogs source table dimensions differ";return false;}
        record_at+=4;
        std::vector<std::vector<SourceDialogStepV1>> dialog_table;
        dialog_table.reserve(dialog_count);
        std::size_t total_steps=0;
        for(std::uint32_t i=0;i<dialog_count;++i){
            std::uint32_t steps=0;
            if(!word(records,record_at,steps)||steps>4096||
               total_steps>16384-steps){error="Dialogs source step count is invalid";return false;}
            record_at+=4;total_steps+=steps;
            if(steps>(records.size()-std::min(record_at,records.size()))/16){
                error="Dialogs source step list is truncated";return false;
            }
            std::vector<SourceDialogStepV1> row;row.reserve(steps);
            for(std::uint32_t j=0;j<steps;++j){
                std::uint32_t style=0,actor=0,text=0,title=0;
                if(!word(records,record_at,style)||!word(records,record_at+4,actor)||
                   !word(records,record_at+8,text)||!word(records,record_at+12,title)){
                    error="Dialogs source step is truncated";return false;
                }
                row.push_back({static_cast<std::int32_t>(style),
                    static_cast<std::int32_t>(actor),static_cast<std::int32_t>(text),
                    static_cast<std::int32_t>(title)});
                record_at+=16;
            }
            dialog_table.push_back(std::move(row));
        }
        if(record_at!=records.size()){
            error="Dialogs source pyarray contains trailing data";return false;
        }
        dh2_pycst_view style_view{};
        if(dh2_pycst_open(&style_view,style_bytes.data(),
                static_cast<std::uint32_t>(style_bytes.size()))!=0){
            error="DialogStyles source constants are malformed";return false;
        }
        dialog_constant_bytes=std::move(style_bytes);
        dialog_constants=style_view;
        dialog_actor_name_ids=std::move(actor_ids);
        source_dialog_table=std::move(dialog_table);
        source_dialog_table_loaded=true;
        error.clear();return true;
    }
    // ParsePlayerName (0x433d00): the DialogMsg constructors rewrite the Player
    // name placeholder of every string they take.
    bool source_dialog_parse_player(std::string& out,std::string& error) {
        std::uintptr_t character=0;
        if(!player_character(this,character,error))return false;
        if(!character){error.clear();return true;}
        std::string name,parsed;
        if(!player_name(this,character,name,error)||
           !ui::localization_player(out,name,localization.pack()>=4&&localization.pack()<=6,
                                    parsed,error))return false;
        out=std::move(parsed);error.clear();return true;
    }
    bool source_dialog_string(std::int32_t id,std::string& out,std::string& error) {
        if(id<0){error="Source DialogMsg has a negative StringManager ID";return false;}
        if(!ensure_localization_ready(error)||
           !localization.string_id(static_cast<std::uint32_t>(id),text_services(),out,error))return false;
        return source_dialog_parse_player(out,error);
    }
    bool source_dialog_style_name(std::int32_t style,std::string& out,std::string& error) {
        if(!ensure_dialog_metadata_ready(error))return false;
        out.clear();
        std::string first;
        bool found=false;
        const auto group="DialogStyles";
        for(std::uint32_t i=0;i<dialog_constants.entries;++i){
            dh2_pycst_entry entry{};
            if(dh2_pycst_entry_at(&dialog_constants,i,&entry)!=0){
                error="DialogStyles source constant traversal failed";return false;
            }
            if(entry.value!=style||entry.group_length!=sizeof("DialogStyles")-1||
               std::memcmp(dialog_constant_bytes.data()+entry.group_offset,group,entry.group_length))continue;
            std::string name(reinterpret_cast<const char*>(dialog_constant_bytes.data()+entry.name_offset),
                             entry.name_length);
            if(!found||name<first){first=std::move(name);found=true;}
        }
        if(found)out=std::move(first);
        error.clear();return true;
    }
    bool load(std::string& error) {
        if(!ensure_localization_ready(error))return false;
        const auto front_menu_assets=ui::original_menu_viewport_v1::source_front_menu_assets_v1(driver_width);
        const auto viewport_assets=front_screen=="main"?front_menu_assets:
            ui::original_menu_viewport_v1::source_front_menu_assets_v1(854);
        if(!raw_asset("source-data/font-palette/fonts_pyarray.bin",font_palette_bytes,error))return false;
        std::uint32_t font_probe=0;
        if(!ui::font_palette_text_color_at_index_v1(font_palette_bytes.data(),font_palette_bytes.size(),
                                                     0,font_probe,error))return false;
        std::array<std::vector<std::uint8_t>,3> metadata;
        if(front_screen=="main"){
            const char* suffixes[]={"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("data/character_properties")+suffixes[i]).c_str(),metadata[i],error))return false;
            if(!dh2::data::load_characters({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},menu_characters,error))return false;
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("data/levels")+suffixes[i]).c_str(),metadata[i],error))return false;
            if(!dh2::data::load_levels({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},menu_levels,error))return false;
        }
        if(front_screen=="main"||front_screen=="loading")
            if(!load_front_settings(error))return false;
        fonts=std::make_unique<ui::SwfHudFreetypeProvider>(ui::SwfFontServices{this,font_read,font_diagnostic},1.f,bitmap_probe);
        movie=std::make_unique<ui::SwfMovie>();ui::SwfServices services;
        frame_owner=std::make_shared<FrameOwner>();
        services.native_owner=frame_owner;services.graph_start=graph_start;
        services.native_actions={"NativePlaySoundFX","NativePushMenu","NativePushState","NativePopMenu","NativePopAllAbove","NativePopAllMenus","NativeGetCreditMovement","NativeBackToHud","NativeGoToMainMenu","NativeAwayFromHud","NativeIsMultiplayerEnabled"};services.native_action=native_action;
        if(front_screen=="main")for(const auto* action:{"NativeGetSaveSlotDetails","NativeCreateSaveSlot","NativeAssignSaveSlotToPlayer","NativeSetSaveSlotIDToMainMenu","NativeStartGame","NativeHasPushNotification","NativeStartFromGCInvite"})services.native_actions.emplace_back(action);
        if(front_screen=="main"||live_player)services.native_actions.emplace_back("NativeGetParsedString");
        // Event 38 now reaches the retained Player AI from the authored
        // animation-step producer. Keep HUD use gated until the complete
        // Begin/End -> close -> CSSkill Blur path passes live gameplay.
        // OnSkill remains dispatched only by the authored do_skill marker.
        // Quest journal still lacks its source level/event owner.
        if(live_player)for(const auto* action:{"NativeUseIpodPlayer","NativeUpdateOrientation","NativePauseAllSounds","NatvieResumeAllSounds","NativePauseMusic","NativeScreenIsBlack","NativeGetPossibleClassSpec","NativeSelectClassSpec","NativeTouchToMove","NativeSkillGetEquipedSkillsIDs","NativeGetSkillDetails",
            "NativeSkillsGetSkillPointsLeft","NativeSkillsTrainSkill","NativeEquipSkill","NativeGetPlayerStats","NativeStatsAssignPoint","NativeSaveGame","NativeSetCurrentQuest","NativeIsMultiplayerGame",
            "NativeShowStatusBar",
            "NativeShowMinimapLegend",
            "NativeReloadSkills",
            "NativeSetMultitouch",
            "NativeGetCharMenuTutorialMessage","NativeSkipCharMenuTutorialMessage","NativeGetQuestCount","NativeGetQuestIDsInRange","NativeGetQuestDetails","NativeGetWorldMapLocations","NativeGoToZone","NativeHUDSkill","NativeHUDSpell","NativeHUDGetActiveFaery","NativeHUDGetIsFaeryUnlocked",
            "NativeGetNextStatusMessage",
            "NativeStopMessage",
            "NativeStopDialog",
            "NativeGetNextDialogMessage",
            "NativeHUDSetActiveFaery","NativeMerchantGetItemsListForSlot","NativeMerchantBuyItem","NativeMerchantSellItem",
            "NativeReviveAllPlayers",

            "NativeInvDropItem","NativeInvEquipItem","NativeInvUnequipItem","NativeInvAutoEquipSlot","NativeSwapEquipment","NativeInvGetItemsListForSlot","NativeInvGetItemDetails","NativeInvGetEquipedItem",
            "NativeInvTransmuteItem","NativeInvGetHasOffHandWeapon","NativeInvGetHasTwoHandedWeapon","NativeInvGetPlayerGold",
            "NativeGetNumPotions","NativeGetStringNumPotions","NativeUsePotion","NativeChangeRolloverInputBehavior",
            "NativeLockZoom","NativeUnlockZoom","NativeResetMapZoom"})services.native_actions.emplace_back(action);
        if(live_player)for(const auto action:ui::gameplay_settings_actions_v1)
            services.native_actions.emplace_back(action.data());
        if(front_screen=="main")for(const auto* action:{"NativeGetOptionParameters","NativeSetOptions","NativeLoadSettings","NativeSaveSettings","NativeEnterOptionMenu","NativeRefreshHudManager","NativeChangeRolloverInputBehavior","NativeIsJapaneseVersion","NativeIsKorean","NativeDoWeHaveInternet"})services.native_actions.emplace_back(action);
        services.context=this;services.read=movie_read;services.texture=texture;services.image=image;
        services.draw=draw;services.stencil=stencil;services.native_call=native;services.diagnostic=diagnostic;
        services.glyphs=fonts->borrowed_provider();
        if(front_screen=="main"||live_player){
            // The gameplay MenuManager keeps the same dqshared renderer
            // (menu_Loading for NativeGoToZone/ChangeLevel lives only there).
            // Original RenderFX::Load (0x7ab7c0..0x7ab7e0) constructs a
            // separate player per renderer. Importing common definitions into
            // the main player does not activate the shared menu renderer.
            shared_frame_owner=std::make_shared<FrameOwner>();
            shared_menu_movie=std::make_unique<ui::SwfMovie>();
            auto shared_services=services;
            shared_services.native_owner=shared_frame_owner;
            shared_services.graph_start=shared_graph_start;
            if(!shared_menu_movie->load({},front_menu_assets.shared_movie,shared_services,error))return false;
            const ui::ViewportState64 shared_seed{
                {0,float(front_menu_assets.width_twips),0,float(front_menu_assets.height_twips)},
                {0,0,front_menu_assets.width_pixels,front_menu_assets.height_pixels},
                {0,0,front_menu_assets.width_pixels,front_menu_assets.height_pixels},1.f,0,0};
            std::vector<std::string> shared_states;
            ui::SwfClipInfo options;
            if(!shared_menu_movie->connect_viewport(shared_seed,{this,orientation,dimensions},error)||
               !shared_menu_movie->update_viewport(shared_camera,error)||
               !shared_menu_movie->advance(0,error)||
               !shared_menu_movie->hide_menu_state_clips(shared_states,error)||
               !shared_menu_movie->clip("_root.menu_Options",options,error))return false;
            if(options.visible){error="Inactive Options state remained visible";return false;}
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Original shared menu renderer loaded | independent player | states %zu | Options id %d | frames %d | inactive | native stack pending",
                shared_states.size(),options.id,options.frames);
            ui::SwfInputCoreServices shared_input;shared_input.owner=shared_frame_owner;shared_input.context=this;
            shared_input.native_receiver=reinterpret_cast<std::uintptr_t>(&shared_frame_owner->main_events);
            shared_input.can_handle_event=input_accepts;shared_input.native_event=input_native_event;shared_input.advance=shared_input_advance;
            if(!shared_menu_movie->connect_input("_root.menu_HelpButtons",shared_frame_owner->history,0x84,
                shared_frame_owner->input_selection,{this,orientation,dimensions},shared_input,error))return false;
            shared_frame_owner->main_events.render_bound=true;
        }
        const char* movie_uri=front_screen=="main"?front_menu_assets.main_movie:front_screen=="loading"?"data/menus/loadanims_droid.swf":"data/menus/dqhud_droid.swf";
        if(!movie->load({viewport_assets.shared_movie},movie_uri,services,error))return false;
        if(shared_menu_movie){
            const auto primary=movie->player_identity(),shared=shared_menu_movie->player_identity();
            if(!primary||!shared||primary==shared){error="Menu renderers did not retain independent players";return false;}
            __android_log_print(ANDROID_LOG_INFO,tag,"Original menu renderer player identities | main %zx | shared %zx | distinct 1",primary,shared);
        }
        const ui::ViewportState64 seed{
            {0,float(viewport_assets.width_twips),0,float(viewport_assets.height_twips)},
            {0,0,viewport_assets.width_pixels,viewport_assets.height_pixels},
            {0,0,viewport_assets.width_pixels,viewport_assets.height_pixels},1.f,0,0};
        if(!movie->connect_viewport(seed,{this,orientation,dimensions},error)||
           !movie->update_viewport(camera,error)||!movie->advance(0,error))return false;
        if(!front_screen.empty()){
            const char* path=front_screen=="main"?"_root.menu_MainMenu":"_root.anim_loading_splash";
            if(front_screen=="main"){
                std::vector<std::string> state_names;
                if(!movie->hide_menu_state_clips(state_names,error))return false;
                for(const auto& name:state_names)__android_log_print(ANDROID_LOG_INFO,tag,"Original menu state visibility initialized | name %s | visible 0",name.c_str());
            }
            ui::SwfClipInfo front;
            if(!movie->clip(path,front,error)||!movie->set_visible(path,true,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original front screen loaded | screen %s | uri %s | clip %s | id %d | frames %d",front_screen.c_str(),movie_uri,path,front.id,front.frames);
            if(front_screen=="main"){
                menu_stack={"menu_MainMenu"};
                // Prepare the preview scene before onShow: the authored
                // callback can select a saved Character during this dispatch.
                const auto scene=model_renderer::load_menu_background(manager);
                if(scene.find("3D upload OK")!=0){error=scene;return false;}
                if(!movie->action_script(this,show_main,error)||!movie->set_visible("_root.menu_bg",true,error))return false;
                ui::SwfInputCoreServices input;input.owner=frame_owner;input.context=this;
                input.native_receiver=reinterpret_cast<std::uintptr_t>(&frame_owner->main_events);
                input.can_handle_event=input_accepts;input.native_event=input_native_event;input.advance=input_advance;
                // MenuManager::LoadMainMenu (0x4325ac) explicitly installs
                // 0x84 on renderer slot 2, beyond RenderFX's constructor default.
                if(!movie->connect_input(path,frame_owner->history,0x84,frame_owner->input_selection,{this,orientation,dimensions},input,error))return false;
                frame_owner->main_events.render_bound=true;
                const auto rectangle=front_rectangle();if(!movie->input_rectangle(rectangle.data(),error))return false;
                __android_log_print(ANDROID_LOG_INFO,tag,"Original source input connected | direct main event stage | MenuManager/HUDControls forwarding pending");
            }
            loaded=true;menu_clock.reset();frame_time=std::chrono::steady_clock::now();return true;
        }
        if(!live_player){error="Gameplay HUD load requires the attached player";return false;}
        if(!settings||!settings->has_option("HUDStyle")){
            error="Original gameplay HUD requires the retained Application HUDStyle option";return false;
        }
        hud_style=settings->option("HUDStyle");
        if(hud_style<0||hud_style>3){error="Saved HUDStyle is outside the four authored Android layouts";return false;}
        if(!font_failure.empty()){error=font_failure;return false;}
        gameplay_hud=std::make_unique<ui::AuthoredGameplayHudV1>(*movie);
        gameplay_hud_bound=false;gameplay_hud_pointers.fill(-1);camera_fingers.reset();gameplay_enemy_hud_state={};gameplay_enemy_hud_player_identity=0;
        gameplay_hud_skills_refresh={};
        // DisplayRightHud performs the original selected-HUD onPush then
        // onShow sequence, including the skill/faery refresh. Do not call
        // refresh_skills() again: it would replay onPush outside that order.
        // DisplayRightHud runs selected-HUD onPush then onShow and performs the
        // real skill/faery refresh; do not replay onPush outside source order.
        if(!gameplay_hud->bind(hud_style,error))return false;
        gameplay_hud_bound=true;
        float joystick_width=0.f;
        if(!gameplay_hud->joystick_background_width(joystick_width,error)||
           !ui::authored_joystick_initialize_v1(authored_joystick,joystick_width,error))return false;
        authored_joystick_ready=true;
        ui::SwfClipInfo clip;
        const auto selected_panel=gameplay_hud->elements_path()+".HealthBars.player";
        if(!movie->clip(selected_panel.c_str(),clip,error)||!clip.id){error="Required saved-style original health/mana parent differs";return false;}
        status=std::make_unique<ui::PlayerStatusHud>(*movie);
        if(!status->bind(hud_sha,gameplay_hud->menu_path().c_str(),error))return false;
        if(live_player){
            ui::SwfInputCoreServices hud_input;hud_input.owner=frame_owner;hud_input.context=this;
            hud_input.native_receiver=reinterpret_cast<std::uintptr_t>(&frame_owner->main_events);
            hud_input.can_handle_event=input_accepts;hud_input.native_event=input_native_event;hud_input.advance=input_advance;
            if(!movie->connect_input(gameplay_hud->menu_path().c_str(),frame_owner->history,0x84,
                frame_owner->input_selection,{this,orientation,dimensions},hud_input,error)||
               !movie->input_rectangle(viewport_rectangle().data(),error))return false;
            frame_owner->main_events.render_bound=true;
            const auto character_path=gameplay_hud->control_path(ui::AuthoredHudControlV1::character);
            {
                ui::SwfClipInfo state;std::string probe_error;
                if(movie->clip(character_path.c_str(),state,probe_error)){
                    __android_log_print(ANDROID_LOG_INFO,tag,
                        "Gameplay HUD input clip | path %s | id %d | depth %d | visible %d | frame %d/%d | matrix %.4f %.4f %.4f %.4f %.4f %.4f",
                        character_path.c_str(),state.id,state.depth,state.visible,state.frame,state.frames,
                        state.world.value[0],state.world.value[1],state.world.value[2],
                        state.world.value[3],state.world.value[4],state.world.value[5]);
                }else __android_log_print(ANDROID_LOG_INFO,tag,"Gameplay HUD input clip absent | path %s | reason %s",character_path.c_str(),probe_error.c_str());
            }
            HudCharacterBinding hud_button{this,&gameplay_character_button};
            if(!movie->action_script(&hud_button,bind_hud_character_button,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Gameplay authored HUD bound | style %d | character-menu target %zx | path %s | retained Application settings and same dqhud movie",
                hud_style,gameplay_character_button,character_path.c_str());

            character_frame_owner=std::make_shared<FrameOwner>();
            character_menu_movie=std::make_unique<ui::SwfMovie>();
            auto character_services=services;
            character_services.native_owner=character_frame_owner;
            character_services.graph_start=character_graph_start;
            if(!character_menu_movie->load({"data/menus/dqshared_droid.swf"},
                "data/menus/dqcharmenu_droid.swf",character_services,error))return false;
            if(!character_menu_movie->action_script(this,verify_native_skill_menu_runtime,error))return false;
            const ui::ViewportState64 character_seed{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1.f,0,0};
            std::vector<std::string> character_states;
            if(!character_menu_movie->connect_viewport(character_seed,{this,orientation,dimensions},error)||
               !character_menu_movie->update_viewport(character_camera,error)||
               !character_menu_movie->advance(0,error)||
               !character_menu_movie->hide_menu_state_clips(character_states,error))return false;
            if(!character_menu_movie->set_visible("_root.menu_CharacterMenu",false,error))return false;
            ui::SwfInputCoreServices character_input;character_input.owner=character_frame_owner;
            character_input.context=this;character_input.native_receiver=reinterpret_cast<std::uintptr_t>(&character_frame_owner->main_events);
            character_input.can_handle_event=input_accepts;character_input.native_event=input_native_event;
            character_input.advance=input_advance;
            if(!character_menu_movie->connect_input("_root.menu_CharacterMenu",character_frame_owner->history,0x84,
                character_frame_owner->input_selection,{this,orientation,dimensions},character_input,error)||
               !character_menu_movie->input_rectangle(front_rectangle().data(),error))return false;
            character_frame_owner->main_events.render_bound=true;
            ui::SwfClipInfo character_root;
            if(!character_menu_movie->clip("_root.menu_CharacterMenu",character_root,error)||!character_root.id){
                error="Original character menu root clip unavailable";return false;
            }
            // DisplayRightHud runs the selected HUD's source onPush/onShow
            // callbacks, including NativeReloadSkills. Load and bind the one
            // CharacterMenu movie first because Character::ReloadSkills
            // synchronously asks its RenderFX menu for IsSpecTime.
            if(!gameplay_hud->activate(error))return false;
            game_menu_stack.clear();game_menu_clock.reset();game_menu_frame_time=std::chrono::steady_clock::now();
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Original gameplay menu renderer loaded | source dqcharmenu_droid.swf | screens %zu | separate HUD and character menu players | inventory/skills/faery callbacks remain owner-bound",
                character_states.size());
        }
        loaded=true;return true;
    }
    bool reset_failed(std::string& error) {
        front_name_input_active.store(false,std::memory_order_release);
        gpu.abort();gameplay_hud.reset();gameplay_hud_bound=false;gameplay_hud_pointers.fill(-1);camera_fingers.reset();gameplay_enemy_hud_state={};gameplay_enemy_hud_player_identity=0;gameplay_hud_skills_refresh={};authored_joystick={};authored_joystick_ready=false;
        status.reset();character_menu_movie.reset();shared_menu_movie.reset();movie.reset();
        character_frame_owner.reset();shared_frame_owner.reset();frame_owner.reset();fonts.reset();loaded=false;selected=false;
        last_width=last_height=0;loading_bitmap_reported=false;reported_frames={{-1,-1,-1,-1,-1}};
        leases.clear();exports.clear();font_failure.clear();provider_failure.clear();
        menu_sounds.clear();
        menu_audio.clear();
        if(settings)data::savegame_options_v1::unbind_read_only_settings(settings.get());
        settings.reset();settings_files.reset();language_selection=-1;
        menu_stack.clear();game_menu_stack.clear();
        gameplay_hud_active=true;gameplay_hud_input_reset_pending=false;
        gameplay_back_to_hud_draining_stack=false;
        gameplay_back_to_hud_pending=false;gameplay_native_revive_pending=false;
        gameplay_native_back_to_hud_pending=false;
        gameplay_main_menu_request.reset();
        gameplay_hud_refresh={};
        source_status_messages.flush();
        // The SWF that shows and pops these messages is gone with the movie.
        source_dialog_messages.flush();dialog_start_pending=false;status_start_pending=false;dialog_touch_owned=false;
        gameplay_character_button=0;gameplay_character_button_pressed.fill(false);
        launch_requests.clear();
        launch_delivered=false;
        input_dispatch_movie=nullptr;input_dispatch_frames=nullptr;last_menu_dt=0;
        menu_clock.reset();
        reported_hardcoded_labels.fill(false);
        glyph_uploads=bitmap_uploads=string_calls=core_errors=packed_glyphs=strips=lines=masks=0;
        return gpu.reset_images(error);
    }
};
OriginalUiSession::OriginalUiSession():impl_(std::make_unique<Impl>()){}
OriginalUiSession::~OriginalUiSession(){
    if(impl_&&Impl::active_fscommand_owner==impl_.get()){
        Impl::active_fscommand_owner=nullptr;
        gameswf::register_fscommand_callback(nullptr);
    }
    if(impl_&&impl_->settings)data::savegame_options_v1::unbind_read_only_settings(impl_->settings.get());
}
void OriginalUiSession::bind_front_runtime(const FrontRuntimeServices& services){impl_->runtime=services;}
data::ItemTextServicesV5 OriginalUiSession::item_text_services(
    const data::ItemTable& items,const data::CharacterTable& characters) noexcept {
    impl_->item_text_items=&items;
    impl_->item_text_characters=&characters;
    return {impl_.get(),Impl::item_text_metadata,Impl::item_text_invoke};
}
bool OriginalUiSession::consume_launch_request(NativeStartGameIntentV1& request){
    if(impl_->launch_requests.empty())return false;
    request=impl_->launch_requests.front();impl_->launch_requests.pop_front();impl_->launch_delivered=true;return true;
}
bool OriginalUiSession::display_fast_travel(bool source_flag,
        const std::string& level_name,const std::string& repeated_level_name,
        std::int32_t entry_point,std::string& error) {
    if(!impl_||!impl_->loaded||!impl_->movie||!impl_->live_player||
       // IDA TriggerZoneExitLevel::Update 0x39c388: localized LevelList name,
       // then the raw LevelList key, then the entry point.
       level_name.empty()||repeated_level_name.empty()||entry_point<0) {
        error="DisplayFastTravel requires the retained gameplay HUD and matching source arguments";
        if(!impl_||!impl_->loaded||!impl_->movie)error+=" (HUD movie not loaded)";
        else if(!impl_->live_player)error+=" (no live player)";
        else error+=" (level '"+level_name+"'/'"+repeated_level_name+"' entry "+
            std::to_string(entry_point)+")";
        return false;
    }
    struct Call {
        bool source_flag{};
        const std::string* name{};
        const std::string* key{};
        std::int32_t entry{};
    } call{source_flag,&level_name,&repeated_level_name,entry_point};
    return impl_->movie->menu_action_script(&call,
        [](void* raw,ui::SwfAsGraph& graph,std::string& nested_error){
            const auto& call=*static_cast<Call*>(raw);
            ui::SwfAsValue root,hud,result;bool callable=false;
            if(!graph.root_value(root,nested_error)||
               !graph.find_target(root,"_root.menu_HUD_0",hud,nested_error)||
               !hud.identity()) {
                if(nested_error.empty())nested_error="Authored gameplay HUD movie is unavailable";
                return false;
            }
            const std::vector<ui::SwfAsValue> args{
                ui::SwfAsValue::boolean(call.source_flag),
                ui::SwfAsValue::text(call.name->c_str()),
                ui::SwfAsValue::text(call.key->c_str()),
                ui::SwfAsValue::number(call.entry)};
            if(!graph.invoke(hud,hud,"DisplayFastTravel",args,result,callable,nested_error))
                return false;
            // IDA 0x39c388 invokes on the HUD RenderFX's root movie.
            if(!callable&&!graph.invoke(root,root,"DisplayFastTravel",args,result,
                                        callable,nested_error))
                return false;
            if(!callable) {
                __android_log_print(ANDROID_LOG_WARN,tag,
                    "Authored gameplay HUD has no DisplayFastTravel method; fast-travel prompt skipped");
            }
            nested_error.clear();return true;
        },error);
}
bool OriginalUiSession::consume_gameplay_main_menu_request() noexcept{
    return impl_&&impl_->gameplay_main_menu_request.consume();
}
bool OriginalUiSession::game_difficulty_count(std::uint32_t& count,std::string& error){
    const auto table=impl_->option_table.borrow();
    if(!table){error="Actual frontend design table unavailable";return false;}
    count=table.difficulty_count();error.clear();return true;
}
dh2::ui::GameOptionBytesV1 OriginalUiSession::design_settings_table()const noexcept{
    if(!impl_||!impl_->settings)return {};
    return impl_->option_table.borrow().design_settings_table();
}
std::int32_t OriginalUiSession::language()const noexcept{
    return impl_&&impl_->settings?impl_->settings->language():-1;
}
bool OriginalUiSession::touch(float x,float y,int action,std::string& error){
    return touch(x,y,action,0,error);
}
bool OriginalUiSession::menu_text_input_active()const noexcept{
    return impl_&&impl_->front_name_input_active.load(std::memory_order_acquire);
}
bool OriginalUiSession::menu_key_event(std::uint32_t key_code,bool down,std::string& error){
    if(!menu_text_input_active()){error.clear();return true;}
    if(!impl_->selected||impl_->front_screen!="main"||!impl_->movie){
        error="Authored name-entry keyboard has no active front movie";return false;
    }
    return impl_->movie->input_key_event(key_code,down,error);
}
bool OriginalUiSession::touch(float x,float y,int action,std::uint32_t cursor_index,std::string& error){
    error.clear();
    const bool succeeded=[&]{
    const bool front=impl_->front_screen=="main";
    const bool gameplay=impl_->front_screen.empty()&&impl_->live_player;
    if(!impl_->selected||!impl_->loaded||(!front&&!gameplay)){error.clear();return true;}
    if(gameplay&&!impl_->gameplay_hud_active&&impl_->game_menu_stack.empty()){error.clear();return true;}
    if(action<0||action>3||cursor_index>=4||!std::isfinite(x)||!std::isfinite(y)){error="Malformed native UI touch";return false;}
    const bool gameplay_menu=gameplay&&!impl_->game_menu_stack.empty();
    struct TouchOwnerReset {Impl* self;int action;std::uint32_t slot;~TouchOwnerReset(){if(action==1||action==3)self->gameplay_ui_touch_owned[slot]=false;}} owner_reset{impl_.get(),action,cursor_index};
    if(gameplay&&action==0)impl_->gameplay_ui_touch_owned[cursor_index]=gameplay_menu;
    // MenuManager::onEvent (0x430d60-0x430d74) hands finger 0 to the menus
    // always and fingers 1..3 unless NativeSetMultitouch locked them out. The
    // front menu keeps its single-pointer FrontMenuTouchGesture contract.
    if(cursor_index!=0&&(front||impl_->gameplay_multitouch_locked)){error.clear();return true;}
    if(gameplay&&!gameplay_menu&&cursor_index==0){
        bool dialog_consumed=false;
        if(!Impl::dialog_touch(*impl_,action,dialog_consumed,error))return false;
        if(dialog_consumed){error.clear();return true;}
    }
    auto* selected=front?impl_->active_menu_movie():
        gameplay_menu?impl_->menu_movie(impl_->game_menu_stack.back()):impl_->movie.get();
    // HUD-hosted gameplay menus (fade/death screens) share the HUD movie and its
    // viewport mapping; character-menu movies use the front rectangle.
    const auto rectangle=(front||(gameplay_menu&&selected!=impl_->movie.get()))?
        impl_->front_rectangle():impl_->viewport_rectangle();
    if(!selected){error="Native UI renderer unavailable for touch";return false;}
    if(!selected->input_rectangle(rectangle.data(),error))return false;
    // Cancellation must clear a held touch without producing onRelease.
    // Android DOWN/MOVE retain the source cursor button; UP clears it.
    auto owner=selected==impl_->character_menu_movie.get()?impl_->character_frame_owner:
               selected==impl_->shared_menu_movie.get()?impl_->shared_frame_owner:impl_->frame_owner;
    if(!owner){error="Native UI touch frame owner unavailable";return false;}
    if(gameplay&&impl_->gameplay_hud_input_reset_pending&&impl_->movie&&impl_->frame_owner){
        auto* previous_movie=impl_->input_dispatch_movie;
        auto* previous_frames=impl_->input_dispatch_frames;
        impl_->input_dispatch_movie=impl_->movie.get();
        impl_->input_dispatch_frames=&impl_->frame_owner->frames;
        bool cleared=true;
        for(std::uint32_t index=0;index<4&&cleared;++index)
            cleared=impl_->movie->input_cancel(x,y,index,error);
        impl_->input_dispatch_movie=previous_movie;
        impl_->input_dispatch_frames=previous_frames;
        if(!cleared)return false;
        impl_->gameplay_hud_input_reset_pending=false;
    }
    // The source HUD's Attack and Joystick artwork is the input surface once
    // its Android overlays are hidden. Use the same retained SWF geometry and
    // player movement owner for down/move/up; all other buttons keep normal
    // authored GameSWF cursor dispatch below.
    if(gameplay&&!gameplay_menu&&impl_->gameplay_hud&&impl_->gameplay_hud_bound&&impl_->authored_joystick_ready){
        using Control=ui::AuthoredHudControlV1;
        const int attack=static_cast<int>(Control::attack),joystick=static_cast<int>(Control::joystick);
        if(action==0){
            for(const auto control:{Control::pause,Control::character,Control::potion,Control::faery,
                                    Control::skill1,Control::skill2,Control::skill3,Control::attack,Control::joystick}){
                ui::AuthoredHudGeometryV1 geometry;
                if(!impl_->gameplay_hud->geometry(control,x,y,geometry,error))return false;
                if(geometry.hit){impl_->gameplay_ui_touch_owned[cursor_index]=true;break;}
            }
        }
        int& captured=impl_->gameplay_hud_pointers[cursor_index];
        if(action==3&&captured>=0){
            if(captured==joystick){
                std::uintptr_t identity=0;std::string owner_error;
                const bool has_player=model_renderer::ui_player_identity(identity,owner_error)&&identity!=0;
                if(!ui::authored_joystick_release_v1(impl_->authored_joystick,has_player,
                    impl_->authored_joystick_services(),error))return false;
            }
            captured=-1;error.clear();return true;
        }
        if(action==0&&captured<0){
            for(const auto control:{Control::attack,Control::joystick,Control::faery}){
                ui::AuthoredHudGeometryV1 geometry;
                if(!impl_->gameplay_hud->geometry(control,x,y,geometry,error))return false;
                if(!geometry.hit)continue;
                // Each of these is one SWF button: a second finger on a control
                // another finger already holds is consumed without a second press.
                if(impl_->hud_control_held(static_cast<int>(control))){error.clear();return true;}
                captured=static_cast<int>(control);
                if(control==Control::faery){
                    // The faery HUD button is hit-tested by the same authored geometry as
                    // attack, but its source onRelease never reached NativeHUDSpell through
                    // the generic SWF cursor path (the SWF button stayed silent for press
                    // and release). Hold the finger here and cast on release, as onRelease does.
                    error.clear();return true;
                }
                if(control==Control::joystick){
                    const bool pressed=ui::with_authored_hud_edge_layout_v6(*impl_->movie,*impl_->gameplay_hud,
                        [&](std::string& nested){
                            ui::AuthoredHudGeometryV1 receiver;
                            return impl_->gameplay_hud->joystick_receiver_geometry(x,y,receiver,nested)&&
                                ui::authored_joystick_press_v1(impl_->authored_joystick,
                                    receiver.local[0],receiver.local[1],nested);
                    },error);
                    if(!pressed){captured=-1;return false;}
                }else{
                    std::uintptr_t interacted_identity=0;
                    std::string npc_interaction_error;
                    const bool npc_interacted=
                        model_renderer::ui_player_character_interact_target(
                            interacted_identity,npc_interaction_error);
                    if(!npc_interaction_error.empty()){
                        error=std::move(npc_interaction_error);
                        captured=-1;return false;
                    }
                    std::string merchant_name,interaction_error;std::int32_t module_filter=-1;
                    std::uintptr_t merchant_identity=0;
                    if(!model_renderer::ui_player_merchant_interaction_target(
                           merchant_name,module_filter,merchant_identity,interaction_error)){
                        if(!interaction_error.empty()){
                            error=std::move(interaction_error);captured=-1;return false;
                        }
                    }else{
                        struct MerchantOpen {
                            const std::string* name;
                            std::int32_t module_filter;
                        } context{&merchant_name,module_filter};
                        const bool opened=impl_->movie->action_script(&context,
                            [](void* raw,ui::SwfAsGraph& graph,std::string& nested){
                                const auto& request=*static_cast<MerchantOpen*>(raw);
                                ui::SwfAsValue root,result;bool callable=false;
                                if(!graph.root_value(root,nested))return false;
                                if(!graph.invoke(root,root,"openMerchantMenu",
                                     {ui::SwfAsValue::number(0)},result,callable,nested))return false;
                                if(!callable){nested="Authored openMerchantMenu callback is unavailable";return false;}
                                result={};callable=false;
                                if(!graph.invoke(root,root,"AdditionnalInfosForMerchant",
                                     {ui::SwfAsValue::text(*request.name),
                                      ui::SwfAsValue::number(request.module_filter)},
                                     result,callable,nested))return false;
                                if(!callable){nested="Authored AdditionnalInfosForMerchant callback is unavailable";return false;}
                                nested.clear();return true;
                            },error);
                        captured=-1;
                        if(!opened)return false;
                        __android_log_print(ANDROID_LOG_INFO,tag,
                            "Source Character::Interact merchant route | identity %zx | module filter %d | name %s | callbacks open then additionals",
                            merchant_identity,module_filter,merchant_name.c_str());
                        return true;
                    }
                    if(npc_interacted){
                        __android_log_print(ANDROID_LOG_INFO,tag,
                            "Source Character::Interact NPC route | identity %zx | HUD attack press",
                            interacted_identity);
                        return true;
                    }
                    const auto response=model_renderer::player_attack(-1);
                    __android_log_print(ANDROID_LOG_INFO,tag,"Authored HUD attack press | %s",response.c_str());
                }
                __android_log_print(ANDROID_LOG_INFO,tag,"Authored HUD pointer captured | control %d | style %d | source shape %d | %.1f %.1f",
                    captured,impl_->hud_style,geometry.character_id,x,y);
                return true;
            }
        }
        if(captured==joystick&&action==2){
            return ui::with_authored_hud_edge_layout_v6(*impl_->movie,*impl_->gameplay_hud,
                [&](std::string& nested){
                    ui::AuthoredHudGeometryV1 receiver;
                    if(!impl_->gameplay_hud->joystick_receiver_geometry(x,y,receiver,nested))return false;
                    const bool dragged=ui::authored_joystick_drag_v1(impl_->authored_joystick,
                        receiver.local[0],receiver.local[1],receiver.local_matrix[2],receiver.local_matrix[5],
                        impl_->authored_joystick_services(),nested);
                    if(dragged&&impl_->authored_joystick_drag_trace_count<8){
                        ++impl_->authored_joystick_drag_trace_count;
                        __android_log_print(ANDROID_LOG_INFO,tag,
                            "Authored joystick drag | screen %.1f %.1f | local %.1f %.1f | receiver %.1f %.1f | radius %d %d | center %d %d | active %u | magnitude %.6g | direction %.6g %.6g %.6g",
                            x,y,receiver.local[0],receiver.local[1],receiver.local_matrix[2],receiver.local_matrix[5],
                            impl_->authored_joystick.radius_x,impl_->authored_joystick.radius_y,
                            impl_->authored_joystick.center_x,impl_->authored_joystick.center_y,
                            unsigned(impl_->authored_joystick.active),impl_->authored_joystick.magnitude,
                            impl_->authored_joystick.direction[0],impl_->authored_joystick.direction[1],
                            impl_->authored_joystick.direction[2]);
                    }
                    return dragged;
                },error);
        }
        if(captured>=0&&action==1){
            const int held=captured;captured=-1;
            if(held==static_cast<int>(Control::faery)){
                // Source onRelease fires only when the finger is still over the button.
                ui::AuthoredHudGeometryV1 release_geometry;
                if(!impl_->gameplay_hud->geometry(Control::faery,x,y,release_geometry,error))return false;
                if(!release_geometry.hit){error.clear();return true;}
                std::uintptr_t identity=0;std::string identity_error;
                if(!model_renderer::ui_player_identity(identity,identity_error)||!identity){error.clear();return true;}
                return ui::run_hud_action_and_refresh_usable_v1(impl_->gameplay_hud_usable_refresh,[&]{
                    return model_renderer::ui_player_use_spell(identity,error);
                });
            }
            if(held==joystick){
                std::uintptr_t identity=0;std::string owner_error;
                const bool has_player=model_renderer::ui_player_identity(identity,owner_error)&&identity!=0;
                const bool released=ui::authored_joystick_release_v1(impl_->authored_joystick,has_player,
                    impl_->authored_joystick_services(),error);
                return released;
            }
            // HUDControls::Update polls the held attack state; releasing its
            // pointer only clears capture and must not trigger a new attack.
            error.clear();return true;
        }
        // Keep the held attack out of the SWF cursor. Its source operation is
        // polled from render_player at the HUDControls::Update cadence.
        if(captured==attack&&action==2)return true;
        if(captured==static_cast<int>(Control::faery)&&action==2)return true;
    }
    if(gameplay&&!gameplay_menu&&impl_->gameplay_character_button){
        if(action==3)impl_->gameplay_character_button_pressed[cursor_index]=false;
        else if(action==0){
            bool hit=false;if(!impl_->gameplay_character_button_hit(*impl_->movie,x,y,hit,error))return false;
            impl_->gameplay_character_button_pressed[cursor_index]=hit;
            if(hit)impl_->gameplay_ui_touch_owned[cursor_index]=true;
        }else if(action==2&&impl_->gameplay_character_button_pressed[cursor_index]){
            bool hit=false;if(!impl_->gameplay_character_button_hit(*impl_->movie,x,y,hit,error))return false;
            if(!hit)impl_->gameplay_character_button_pressed[cursor_index]=false;
        }
    }
    struct Dispatch {Impl& self;ui::SwfMovie* previous_movie;ui::SwfFrameConnection* previous_frames;
        ~Dispatch(){self.input_dispatch_movie=previous_movie;self.input_dispatch_frames=previous_frames;}}
        dispatch{*impl_,impl_->input_dispatch_movie,impl_->input_dispatch_frames};
    impl_->input_dispatch_movie=selected;
    impl_->input_dispatch_frames=&owner->frames;
    const auto dispatch_cursor=[&](std::string& nested){
        if(action==3)return selected->input_cancel(x,y,cursor_index,nested);
        if(!selected->input_cursor({x,y,0.f,(action==0||action==2)?1:0},cursor_index,nested))return false;
        if(gameplay&&!gameplay_menu&&action==1&&impl_->gameplay_character_button_pressed[cursor_index]){
            bool hit=false;
            if(!impl_->gameplay_character_button_hit(*impl_->movie,x,y,hit,nested))return false;
            impl_->gameplay_character_button_pressed[cursor_index]=false;
            if(hit&&impl_->game_menu_stack.empty())
                return Impl::push_game_menu(impl_.get(),"menu_CharacterMenu",nested);
        }
        return true;
    };
    const bool dispatched=gameplay&&!gameplay_menu&&impl_->gameplay_hud&&impl_->gameplay_hud_bound
        ?ui::with_authored_hud_edge_layout_v6(*impl_->movie,*impl_->gameplay_hud,dispatch_cursor,error)
        :dispatch_cursor(error);
    return dispatched;
    }();
    // Flush only after the touch-dispatch lambda returns: its Dispatch RAII
    // guard must restore input_dispatch_movie/frames before DisplayRightHud
    // re-enters the authored SWF.
    if(succeeded&&(impl_->dialog_start_pending||impl_->status_start_pending)&&
       !Impl::flush_pending_dialog_start(*impl_,error))return false;
    if(succeeded&&impl_->gameplay_native_revive_pending&&
       !Impl::flush_deferred_native_revive(*impl_,error))return false;
    if(succeeded&&impl_->gameplay_hud_skills_refresh.pending&&
       !Impl::flush_deferred_faery_hud_skills(*impl_,error))return false;
    if(succeeded&&(impl_->gameplay_back_to_hud_pending||
       impl_->gameplay_hud_refresh.display_right_hud_pending||
       impl_->gameplay_hud_refresh.fill_action_icon_pending)&&
       !Impl::flush_deferred_back_to_hud(*impl_,error))return false;
    if(!succeeded&&error.empty())error="Native UI touch dispatch failed";
    return succeeded;
}
bool OriginalUiSession::camera_pan_allowed()const noexcept{
    return camera_touch_allowed(0);
}
bool OriginalUiSession::camera_touch_allowed(std::uint32_t slot)const noexcept{
    if(!impl_||slot>=4||!impl_->camera_layer_open())return false;
    // With the map open (the only menu camera_layer_open admits) every finger is the map camera's.
    return !impl_->game_menu_stack.empty()||!impl_->gameplay_ui_touch_owned[slot];
}
void OriginalUiSession::camera_gesture_counts(unsigned& pans_delivered,unsigned& pans_applied,unsigned& pinches) const noexcept{
    pans_delivered=impl_?impl_->camera_pans_delivered:0;
    pans_applied=impl_?impl_->camera_pans_applied:0;
    pinches=impl_?impl_->camera_pinches_delivered:0;
}
bool OriginalUiSession::hud_control_screen_point(unsigned control,int width,int height,float& x,float& y,std::string& error){
    // Development probe for desktop scenarios: sample the same geometry() hit
    // test touch() uses and report the centroid of the area that accepts a press.
    if(!impl_||!impl_->gameplay_hud||!impl_->gameplay_hud_bound||width<=0||height<=0||
       control>static_cast<unsigned>(ui::AuthoredHudControlV1::joystick)){
        error="Authored gameplay HUD control probe requires the bound HUD";return false;
    }
    const auto id=static_cast<ui::AuthoredHudControlV1>(control);
    double sum_x=0,sum_y=0;unsigned hits=0;
    const int step=std::max(4,std::min(width,height)/90);
    for(int py=step/2;py<height;py+=step)for(int px=step/2;px<width;px+=step){
        ui::AuthoredHudGeometryV1 geometry;
        if(!impl_->gameplay_hud->geometry(id,static_cast<float>(px),static_cast<float>(py),geometry,error))return false;
        if(!geometry.hit)continue;
        sum_x+=px;sum_y+=py;++hits;
    }
    if(!hits){error="Authored HUD control has no pressable area on this viewport";return false;}
    x=static_cast<float>(sum_x/hits);y=static_cast<float>(sum_y/hits);error.clear();return true;
}
bool OriginalUiSession::pointer(float x,float y,int action,std::uint32_t slot,std::string& error){
    namespace gesture=dh2::native::gameplay_touch_v1;
    if(!impl_||slot>=gesture::kFingerSlots){error="Malformed native UI pointer";return false;}
    // MenuManager::onEvent runs first: the HUD (stick, attack, skills) and every
    // menu get the finger. ZoomHandler, attached later, only sees what is left.
    const bool delivered=touch(x,y,action,slot,error);
    struct Context{OriginalUiSession* session;}context{this};
    const gesture::CameraServices services{&context,
        [](void* raw,std::int32_t dx,std::int32_t dy){
            // A finger only reaches the camera map when the HUD left its press alone, so
            // only the menu state can still veto a pan (the stick finger is a different finger).
            auto& impl=*static_cast<Context*>(raw)->session->impl_;
            if(!impl.camera_layer_open())return;
            ++impl.camera_pans_delivered;
            if(model_renderer::camera_touch_pan(dx,dy))++impl.camera_pans_applied;
        },
        [](void* raw,float previous,float current){
            auto& impl=*static_cast<Context*>(raw)->session->impl_;
            if(!impl.camera_layer_open())return;
            ++impl.camera_pinches_delivered;
            model_renderer::camera_pinch_zoom(previous,current);
        },
        [](void* raw,std::uint32_t pinch_slot,float pinch_x,float pinch_y){
            // A pinch takes the fingers from the SWF cursor that saw their press.
            std::string ignored;
            (void)static_cast<Context*>(raw)->session->touch(pinch_x,pinch_y,3,pinch_slot,ignored);
        }};
    auto& fingers=impl_->camera_fingers;
    if(action==0)fingers.down(slot,x,y,camera_touch_allowed(slot),services);
    else if(action==2)fingers.move(slot,x,y,services);
    else fingers.up(slot,services);
    return delivered;
}
bool OriginalUiSession::back_to_hud(std::string& error){
    return Impl::back_to_hud(impl_.get(),error);
}
bool OriginalUiSession::push_source_loading_menu(std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.character_menu_movie){
        error="Source menu_Loading overlay requires the retained live gameplay UI session";
        return false;
    }
    ui::SwfClipInfo state;
    if(!self.shared_menu_movie){error="Source menu_Loading requires the retained dqshared movie";return false;}
    if(!self.shared_menu_movie->clip("_root.menu_Loading",state,error))return false;
    if(!state.id){error="Original menu_Loading state is absent from dqshared";return false;}
    return Impl::push_game_menu(&self,"menu_Loading",error);
}
bool OriginalUiSession::push_source_death_fade_menu(bool& pushed,std::string& error){
    pushed=false;
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.character_menu_movie){
        error="Source menu_FadeToBlackScreen requires the retained live gameplay UI session";
        return false;
    }
    // MenuBase::IsVisible gate from PlayerManager::_HandleLocalDeaths 0x375fe8:
    // the fade menu is visible exactly while it is on the gameplay MenuManager stack.
    if(std::find(self.game_menu_stack.begin(),self.game_menu_stack.end(),
                 "menu_FadeToBlackScreen")!=self.game_menu_stack.end()){error.clear();return true;}
    if(!Impl::push_game_menu(&self,"menu_FadeToBlackScreen",error))return false;
    pushed=true;
    // The same _HandleLocalDeaths branch then calls VoxSoundManager::SetSoundVolume(
    // group 1 = SFX, 0): effects fall silent until ReviveLocalPlayers restores the
    // saved option. Music keeps its own volume, so only the FX field drops to zero.
    if(self.settings)
        self.menu_audio.push_back("volume,"+std::to_string(self.settings->saved_option("VolumeMusic"))+",0");
    error.clear();return true;
}
bool OriginalUiSession::pop_source_loading_menu(std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.character_menu_movie){
        error="Source menu_Loading overlay requires the retained live gameplay UI session";
        return false;
    }
    ui::SwfClipInfo state;
    if(!self.shared_menu_movie){error="Source menu_Loading requires the retained dqshared movie";return false;}
    if(!self.shared_menu_movie->clip("_root.menu_Loading",state,error))return false;
    if(!state.id){error="Original menu_Loading state is absent from dqshared";return false;}
    return Impl::pop_menu(&self,"menu_Loading",error);
}
bool OriginalUiSession::source_loading_menu_visible(bool& visible,std::string& error){
    visible=false;
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.character_menu_movie){
        error="Source menu_Loading visibility requires the retained live gameplay UI session";
        return false;
    }
    ui::SwfClipInfo state;
    if(!self.shared_menu_movie){error="Source menu_Loading requires the retained dqshared movie";return false;}
    if(!self.shared_menu_movie->clip("_root.menu_Loading",state,error))return false;
    if(!state.id){error="Original menu_Loading state is absent from dqshared";return false;}
    // MenuBase::IsVisible reads the actual source +0x74 visibility byte.
    visible=state.visible;
    error.clear();return true;
}
bool OriginalUiSession::close_source_hud(std::string& error){
    if(!impl_||!impl_->selected||!impl_->loaded||!impl_->live_player||
       !impl_->front_screen.empty()||!impl_->movie){
        error="GSLevel HUD hide requires the retained gameplay UI session";
        return false;
    }
    auto& self=*impl_;
    // GSLevel::Dtor 0x3860bc queries menu_HUD_0 and invokes virtual Hide.
    // MenuBase::Hide 0x424af4 exits when hidden; otherwise it sets visibility
    // false, unregisters the listener, then invokes the authored onPop.
    const bool hidden=self.movie->menu_action_script(&self,
        [](void* raw,ui::SwfAsGraph& graph,std::string& nested_error){
            auto& owner=*static_cast<Impl*>(raw);
            ui::SwfAsValue root,hud,visibility,result;
            bool visible=false,accepted=false,callable=false,found=false;
            if(!graph.root_value(root,nested_error)||
               !graph.find_target(root,"_root.menu_HUD_0",hud,nested_error))return false;
            // GetMenuByName returning null is a legitimate source branch.
            if(!hud.identity()){nested_error.clear();return true;}
            if(!graph.get_member(hud,"_visible",visibility,found,nested_error))return false;
            if(!found){nested_error="Authored HUD visibility member absent";return false;}
            if(!graph.to_boolean(visibility,visible,nested_error))return false;
            if(!visible){nested_error.clear();return true;}
            if(!graph.set_member(hud,"_visible",ui::SwfAsValue::boolean(false),
                                 accepted,nested_error))return false;
            if(!accepted){nested_error="Authored HUD visibility write rejected";return false;}
            // This session's listener/input dispatch consults this same flag.
            // Cancel deferred reactivation so a later UI frame cannot reopen
            // the old HUD between Hide and Level destruction.
            owner.gameplay_hud_active=false;
            owner.gameplay_back_to_hud_pending=false;
            owner.gameplay_native_back_to_hud_pending=false;
            owner.gameplay_hud_refresh={};
            owner.gameplay_hud_skills_refresh={};
            return graph.invoke(hud,hud,"onPop",{},result,callable,nested_error);
        },error);
    // Level::~Level flushes MenuMessageManager<StatusMsg,4> after hiding its
    // HUD. Do the same only after the authored hide/onPop sequence succeeds.
    if(hidden)self.source_status_messages.flush();
    return hidden;
}
bool OriginalUiSession::complete_source_hud_refresh(std::string& error){
    if(!impl_||!impl_->loaded||!impl_->movie){
        // MenuManager::GetHUDRoot may return null; RemoveCharacter skips the
        // ActionScript callback in that branch.
        error.clear();return true;
    }
    auto& self=*impl_;
    return self.movie->menu_action_script(&self,
        [](void* raw,ui::SwfAsGraph& graph,std::string& nested_error){
            auto& owner=*static_cast<Impl*>(raw);
            ui::SwfAsValue root,hud,result;
            bool callable=false;
            if(!graph.root_value(root,nested_error)||
               !graph.find_target(root,"_root.menu_HUD_0",hud,nested_error))return false;
            // Match GetHUDRoot's null branch. A hidden but present movie still
            // receives the source callback.
            if(!hud.identity()){nested_error.clear();return true;}
            if(!graph.invoke(hud,hud,"completeRefresh",{},result,callable,
                             nested_error))return false;
            // RenderFX::InvokeASCallback on an undefined member returns undefined
            // (e.g. while the HUD is closed for a Level transition).
            if(!callable){nested_error.clear();return true;}
            owner.gameplay_hud_usable_refresh.invalidate();
            nested_error.clear();return true;
        },error);
}
bool OriginalUiSession::enter_source_cutscene(std::string& error){
    // Script_EnterCutSceneMode 0x45a384 invokes StartCutscene on the HUD root, then sets byte_9A600C.
    error.clear();
    if(!impl_){error="Cutscene mode requires the UI session";return false;}
    auto& self=*impl_;
    if(self.loaded&&self.movie){
        const bool invoked=self.movie->menu_action_script(&self,
            [](void*,ui::SwfAsGraph& graph,std::string& nested_error){
                ui::SwfAsValue root,result;
                bool callable=false;
                if(!graph.root_value(root,nested_error))return false;
                // RenderFX::InvokeASCallback is a no-op when the member is absent.
                if(!graph.invoke(root,root,"StartCutscene",{},result,callable,nested_error))return false;
                nested_error.clear();return true;
            },error);
        if(!invoked)return false;
    }
    self.source_cutscene_active=true;
    return true;
}
bool OriginalUiSession::exit_source_cutscene(std::string& error){
    // Script_ExitCutSceneMode 0x4599ac invokes StopCutscene, then clears byte_9A600C.
    error.clear();
    if(!impl_){error="Cutscene mode requires the UI session";return false;}
    auto& self=*impl_;
    if(self.loaded&&self.movie){
        const bool invoked=self.movie->menu_action_script(&self,
            [](void*,ui::SwfAsGraph& graph,std::string& nested_error){
                ui::SwfAsValue root,result;
                bool callable=false;
                if(!graph.root_value(root,nested_error))return false;
                // RenderFX::InvokeASCallback is a no-op when the member is absent.
                if(!graph.invoke(root,root,"StopCutscene",{},result,callable,nested_error))return false;
                nested_error.clear();return true;
            },error);
        if(!invoked)return false;
    }
    self.source_cutscene_active=false;
    return true;
}
bool OriginalUiSession::source_cutscene_active()const noexcept{
    return impl_&&impl_->source_cutscene_active;
}
std::string OriginalUiSession::consume_menu_sound(){
    auto& queue=impl_->menu_sounds;
    if(queue.empty())return {};
    auto value=std::move(queue.front());queue.pop_front();return value;
}
std::string OriginalUiSession::consume_menu_audio(){
    auto& queue=impl_->menu_audio;if(queue.empty())return {};auto value=std::move(queue.front());queue.pop_front();return value;
}
bool OriginalUiSession::debug_menu_sound(const std::string& probe,std::string& error){
    if(!impl_->loaded||impl_->front_screen!="main"){error="Menu sound probe requires the retained main movie";return false;}
    struct Probe {const std::string& name;Impl* self;} context{probe,impl_.get()};
    return impl_->movie->action_script(&context,[](void* raw,ui::SwfAsGraph& graph,std::string& error){
        const auto& name=static_cast<Probe*>(raw)->name;
        ui::SwfAsValue root,receiver,result;bool callable=false;
        if(!graph.root_value(root,error))return false;
        if(name=="inspect-front"){
            auto& self=*static_cast<Probe*>(raw)->self;
            GLint framebuffer=0,viewport[4]{},program=0;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);glGetIntegerv(GL_VIEWPORT,viewport);glGetIntegerv(GL_CURRENT_PROGRAM,&program);
            __android_log_print(ANDROID_LOG_INFO,tag,"Front GPU state inspected | framebuffer %d | viewport %d %d %d %d | program %d",framebuffer,viewport[0],viewport[1],viewport[2],viewport[3],program);
            for(unsigned i=0;i<3;++i){
                const int x=viewport[0]+viewport[2]*int(i+1)/4,y=viewport[1]+viewport[3]/2;
                std::uint8_t rgba[4]{};glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,rgba);
                __android_log_print(ANDROID_LOG_INFO,tag,"Front GPU pixel inspected | position %d %d | rgba %u %u %u %u | error %u",x,y,rgba[0],rgba[1],rgba[2],rgba[3],glGetError());
            }
            __android_log_print(ANDROID_LOG_INFO,tag,"Front state inspected | selected %s | dimensions %d %d | viewport %d %d",self.active_menu_path().c_str(),self.driver_width,self.driver_height,self.last_width,self.last_height);
            for(const char* path:{"","menu_MainMenu","menu_MainMenu.btn_MENU_SINGLE_PLAYER","menu_EnterName","menu_SelectClass"}){
                ui::SwfAsValue target=root;
                if(*path&&!graph.find_target(root,path,target,error))return false;
                if(!target.identity())continue;
                for(const char* member:{"_visible","_alpha","_currentframe","_x","_y","_xscale","_yscale"}){
                    ui::SwfAsValue value;bool found=false;std::string text;
                    if(!graph.get_member(target,member,value,found,error)||!graph.to_text(value,text,error))return false;
                    __android_log_print(ANDROID_LOG_INFO,tag,"Front clip inspected | path %s | member %s | found %d | value %s",path,member,found,text.c_str());
                }
            }
            return Impl::probe_main_background(&self,graph,error);
        }
        if(name=="inspect-class"){
            for(const auto& item:std::array<std::pair<const char*,const char*>,3>{{{"","PlayerClass"},{"menu_SelectClass.class_title.text","text"},{"menu_SelectClass.class_description.text","text"}}}){
                ui::SwfAsValue target=root,value;bool found=false;std::string text;
                if((*item.first&&!graph.find_target(root,item.first,target,error))||!graph.get_member(target,item.second,value,found,error)||!graph.to_text(value,text,error))return false;
                __android_log_print(ANDROID_LOG_INFO,tag,"Original class state inspected | path %s | member %s | found %d | text %s",item.first,item.second,found,text.c_str());
            }
            return true;
        }
        if(name=="inspect-name"){
            for(const auto& item:std::array<std::pair<const char*,const char*>,2>{{{"menu_EnterName","characterName"},{"menu_EnterName.buttons.btn_character_name.text","text"}}}){
                ui::SwfAsValue target,value;bool found=false;std::string text;
                if(!graph.find_target(root,item.first,target,error)||!graph.get_member(target,item.second,value,found,error)||!graph.to_text(value,text,error))return false;
                __android_log_print(ANDROID_LOG_INFO,tag,"Original name state inspected | path %s | member %s | found %d | text %s",item.first,item.second,found,text.c_str());
            }
            return true;
        }
        if(name.rfind("pop-above-",0)==0){
            if(!graph.global_value(receiver,error))return false;
            std::vector<ui::SwfAsValue> args;
            if(name=="pop-above-main")args={ui::SwfAsValue::text("menu_MainMenu")};
            else if(name=="pop-above-name")args={ui::SwfAsValue::text("menu_EnterName")};
            else if(name=="pop-above-absent")args={ui::SwfAsValue::text("menu_QuestLogSheet")};
            else if(name=="pop-above-invalid-number")args={ui::SwfAsValue::number(0)};
            else if(name=="pop-above-invalid-arity")args={ui::SwfAsValue::text("menu_MainMenu"),ui::SwfAsValue::number(0)};
            else {error="Unknown pop-above probe";return false;}
            if(!graph.invoke(root,receiver,"NativePopAllAbove",args,result,callable,error))return false;
        }else if(name=="authored-options"){
            if(!graph.find_target(root,"menu_MainMenu.btn_MENU_OPTIONS",receiver,error))return false;
            if(!graph.invoke(receiver,receiver,"onRelease",{},result,callable,error))return false;
        }else{
            if(!graph.global_value(receiver,error))return false;
            std::vector<ui::SwfAsValue> args;
            if(name=="invalid-number")args={ui::SwfAsValue::number(94)};
            else if(name=="invalid-arity")args={ui::SwfAsValue::text("MenuConfirm"),ui::SwfAsValue::number(0)};
            else args={ui::SwfAsValue::text(name)};
            if(!graph.invoke(root,receiver,"NativePlaySoundFX",args,result,callable,error))return false;
        }
        if(!callable){error="Menu sound probe source method missing";return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Original menu sound probe dispatched | probe %s",name.c_str());return true;
    },error);
}
bool OriginalUiSession::initialize(AAssetManager* manager,std::string& error) {
    try{impl_->manager=manager;impl_->assets.manager(manager);impl_->gpu.initialize(manager);impl_->report_frame=true;Impl::active_fscommand_owner=impl_.get();gameswf::register_fscommand_callback(&Impl::fscommand_handler);error.clear();return true;}
    catch(const std::exception& e){error=e.what();impl_->selected=false;return false;}
}
bool OriginalUiSession::load_front_screen(const std::string& directory,const std::string& screen,std::string& error) {
    if(screen!="main"&&screen!="loading"){error="Unknown authored front screen";return false;}
    if(!valid_private_directory(directory,error,"Required private UI files directory unavailable"))return false;
    // The development return from gameplay starts one fresh authored main
    // movie after a consumed gameplay handoff. An open authored Start clip
    // retains its timeline through GL recreation without restarting the UI.
    if((impl_->loaded&&impl_->front_screen!=screen)||(screen=="main"&&impl_->launch_delivered)){
        if(impl_->loaded&&impl_->front_screen=="loading"&&screen=="main"){
            // GSInit keeps the one Application settings owner across its
            // intro/splash -> main-menu transition; only SWF renderers reset.
            auto retained_settings=std::move(impl_->settings);
            auto retained_files=std::move(impl_->settings_files);
            if(!impl_->reset_failed(error))return false;
            impl_->settings=std::move(retained_settings);
            impl_->settings_files=std::move(retained_files);
        }else if(!impl_->reset_failed(error))return false;
    }
    impl_->front_screen=screen;impl_->directory=directory;impl_->live_player=false;
    impl_->front_name_input_active.store(screen=="main"&&!impl_->menu_stack.empty()&&
        impl_->menu_stack.back()=="menu_EnterName",std::memory_order_release);
    impl_->selected=true;impl_->report_frame=true;impl_->frame_time=std::chrono::steady_clock::now();error.clear();return true;
}
bool OriginalUiSession::load_health_panel(const std::string& directory,std::string& error) {
    if(!impl_->front_screen.empty()){if(!impl_->reset_failed(error))return false;impl_->front_screen.clear();}
    impl_->selected=false;
    impl_->live_player=false;
    if(!valid_private_directory(directory,error,"Required private UI files directory unavailable"))return false;
    impl_->directory=directory;
    const bool retained=impl_->loaded;
    if(!impl_->loaded&&!impl_->load(error)){
        const auto failure=error;std::string cleanup;impl_->reset_failed(cleanup);
        error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
    }
    impl_->selected=true;impl_->report_frame=true;
    __android_log_print(ANDROID_LOG_INFO,tag,"Original UI retained owner | session %p | movie %p | fonts %p | retained %d",static_cast<void*>(impl_.get()),static_cast<void*>(impl_->movie.get()),static_cast<void*>(impl_->fonts.get()),retained);
    error.clear();return true;
}
bool OriginalUiSession::render(int width,int height,std::string& error) {
    if(!active()){error="Original UI inspection owner inactive";return false;}
    if(!impl_->loaded){
        impl_->driver_width=width;impl_->driver_height=height;
        bool delivered=false;
        try{delivered=impl_->load(error);}catch(const std::exception& failure){error=failure.what();}
        if(!delivered){
            const auto failure=error;std::string cleanup;impl_->reset_failed(cleanup);
            error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
        }
    }
    // Refresh both retained renderer viewports before this frame's input and
    // timeline work. A surface resize must not use last frame's hit rectangle.
    if(!impl_->viewport(width,height,error))return false;
    if(impl_->front_screen=="main"){
        const auto rectangle=impl_->front_rectangle();if(!impl_->movie->input_rectangle(rectangle.data(),error))return false;
        if(impl_->shared_menu_movie&&!impl_->shared_menu_movie->input_rectangle(rectangle.data(),error))return false;
    }
    const bool front=!impl_->front_screen.empty();
    if(front){
        const auto now=std::chrono::steady_clock::now();
        const auto elapsed=now-impl_->frame_time;
        const float seconds=std::min(.1f,std::chrono::duration<float>(elapsed).count());impl_->frame_time=now;
        if(impl_->front_screen=="main"){
            const auto milliseconds=impl_->menu_clock.advance(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed));
            impl_->last_menu_dt=milliseconds;
            // MenuManager::Update (0x42ecc0..0x42ed28) visits every loaded
            // renderer in slot order, including an inactive shared slot 0.
            // This supplies its real frame timeline only; native state/input
            // stack ownership remains a separate required connection.
            auto* selected=impl_->active_menu_movie();
            auto* inactive=selected==impl_->movie.get()?impl_->shared_menu_movie.get():impl_->movie.get();
            auto& inactive_owner=inactive==impl_->shared_menu_movie.get()?impl_->shared_frame_owner:impl_->frame_owner;
            // Advance each exact root once; only the active renderer receives
            // cursor processing. Native callbacks retain their caller scope.
            if(inactive&&!inactive->advance_frames(milliseconds,inactive_owner->frames,error))return false;
            auto selected_owner=selected==impl_->shared_menu_movie.get()?impl_->shared_frame_owner:impl_->frame_owner;
            if(!selected_owner){error="Front menu frame owner unavailable";return false;}
            struct Dispatch {Impl& self;ui::SwfMovie* previous_movie;ui::SwfFrameConnection* previous_frames;
                ~Dispatch(){self.input_dispatch_movie=previous_movie;self.input_dispatch_frames=previous_frames;}}
                dispatch{*impl_,impl_->input_dispatch_movie,impl_->input_dispatch_frames};
            impl_->input_dispatch_movie=selected;
            impl_->input_dispatch_frames=&selected_owner->frames;
            if(!selected->input_advance(milliseconds,error))return false;
        }else if(!impl_->movie->advance(seconds,error))return false;
    }
    // A source native-state request was delivered inside the just-completed
    // movie scope. Stop at this safe boundary; root drains it after draw.
    if(!impl_->launch_requests.empty())return true;
    if(impl_->front_screen=="loading"){
        // GSInit::Draw 0x384ab4..0x384af4 supplies source rect 0,0,1280,752
        // separately from the animated loading overlay. Stretch the recovered
        // image across the complete display, matching the original presentation.
        ui::SwfTexture splash;
        const auto language=impl_->settings?impl_->settings->language():-1;
        const char* splash_name=language==4?"menus/splash_final_jp.tga":
            language==5?"menus/splash_final_kor.tga":"menus/splash_final.tga";
        if(!Impl::texture(impl_.get(),splash_name,0,0,splash,error))return false;
        if(splash.width<1280||splash.height<752){error="Original loading splash source rectangle exceeds atlas";return false;}
        const auto splash_viewport=ui::original_menu_viewport_v1::background_viewport(width,height);
        ui::SwfDraw background; background.kind=ui::SwfDraw::begin;
        background.bounds[1]=1280; background.bounds[3]=752;
        background.viewport[0]=splash_viewport[0]; background.viewport[1]=splash_viewport[1];
        background.viewport[2]=splash_viewport[2]; background.viewport[3]=splash_viewport[3];
        if(!impl_->gpu.draw(background,error))return false;
        background.kind=ui::SwfDraw::bitmap_quad;
        background.fill.kind=ui::SwfFill::bitmap; background.fill.texture=splash;
        background.rect[1]=1280; background.rect[3]=752;
        background.uv_rect[1]=1280.f/splash.width; background.uv_rect[3]=752.f/splash.height;
        if(!impl_->gpu.draw(background,error))return false;
        background.kind=ui::SwfDraw::end;
        if(!impl_->gpu.draw(background,error))return false;
        if(impl_->report_frame)__android_log_print(ANDROID_LOG_INFO,tag,"Original startup splash submitted | source rect 0 0 1280 752 | full surface %d %d",width,height);
    }
    const auto active_path=impl_->active_menu_path();
    const char* path=impl_->front_screen=="main"?active_path.c_str():impl_->front_screen=="loading"?"_root.anim_loading_splash":panel;
    if(impl_->front_screen=="main"){
        // Context recreation revokes scene GL names while retaining the SWF
        // graph. Rebuild the scene before drawing the retained menu again.
        const bool class_preview=!impl_->menu_stack.empty()&&impl_->menu_stack.back()=="menu_SelectClass";
        if(!model_renderer::active()||model_renderer::class_scene_active()!=class_preview){
            const auto scene=class_preview?model_renderer::load_class_scene(impl_->manager):model_renderer::load_menu_background(impl_->manager);
            if(scene.find("3D upload OK")!=0){error=scene;return false;}
        }
        if(class_preview&&!model_renderer::select_class_scene(impl_->class_index,impl_->last_menu_dt,error))return false;
        try{if(!class_preview)model_renderer::draw_menu_background(width,height);}
        catch(const std::exception& failure){error=failure.what();return false;}
        const auto rectangle=impl_->front_rectangle();
        if(!impl_->movie->display_clip("_root.menu_bg",rectangle[0],rectangle[1],
            rectangle[2],rectangle[3],error))return false;
    }
    auto* display_movie=impl_->front_screen=="main"?impl_->active_menu_movie():impl_->movie.get();
    const auto rectangle=front?impl_->front_rectangle():std::array<std::int32_t,4>{};
    if(!(front?display_movie->display_clip(path,rectangle[0],rectangle[1],
        rectangle[2],rectangle[3],error):impl_->movie->display_source_clip(path,error))){
        const auto failure=error;std::string cleanup;impl_->reset_failed(cleanup);
        error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
    }
    if(impl_->report_frame){
        if(impl_->front_screen=="main"&&!impl_->movie->action_script(impl_.get(),Impl::probe_main_background,error))return false;
        if(front){
            const auto rectangle=impl_->front_rectangle();
            const auto selected_assets=ui::original_menu_viewport_v1::
                source_front_menu_assets_v1(impl_->driver_width);
            const char* selected_movie=impl_->front_screen=="main"?
                selected_assets.main_movie:impl_->front_screen=="loading"?
                "data/menus/loadanims_droid.swf":"data/menus/dqhud_droid.swf";
            const auto stage_width=impl_->front_screen=="main"?
                selected_assets.width_pixels:480;
            const auto stage_height=impl_->front_screen=="main"?
                selected_assets.height_pixels:320;
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Original front screen submitted | screen %s | movie %s | authored stage %d x %d | display/input rect %d %d %d %d",
                impl_->front_screen.c_str(),selected_movie,stage_width,stage_height,
                rectangle[0],rectangle[1],rectangle[2],rectangle[3]);
        }else{
            __android_log_print(ANDROID_LOG_INFO,tag,"Original HUD screen submitted | source viewport %d %d",width,height);
        }
        __android_log_print(ANDROID_LOG_INFO,tag,"Original health panel submitted | viewport %d %d | strips %u | lines %u | masks %u | font uploads %u | bitmaps %u | strings %u | core diagnostics %u | authored HUD and gameplay menu input connected",width,height,impl_->strips,impl_->lines,impl_->masks,impl_->glyph_uploads,impl_->bitmap_uploads,impl_->string_calls,impl_->core_errors);
        impl_->report_frame=false;
    }
    return true;
}
bool OriginalUiSession::active() const{return impl_->selected&&(impl_->loaded||!impl_->front_screen.empty());}
bool OriginalUiSession::overlays_player() const{return impl_->selected&&impl_->live_player;}
bool OriginalUiSession::play_authored_animation_text(const char* style,std::uint32_t slot,
    float x_twips,float y_twips,const char* text,std::uint8_t r,std::uint8_t g,
    std::uint8_t b,std::uint8_t a,std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.live_player||!self.loaded||!self.movie){
        error="Authored combat text requires the retained live gameplay HUD movie";return false;
    }
    return self.movie->play_authored_animation_text(style,slot,x_twips,y_twips,
                                                     text,r,g,b,a,error);
}
bool OriginalUiSession::authored_animation_style_id(const char* style,std::int32_t& id,
                                                     std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.live_player||!self.loaded||!self.movie){
        error="Authored animation style lookup requires the retained live gameplay HUD movie";return false;
    }
    return self.movie->authored_animation_style_id(style,id,error);
}
bool OriginalUiSession::play_authored_animation_screen(const char* style,std::uint32_t slot,
    float screen_x_px,float screen_y_px,const char* text,std::uint8_t r,std::uint8_t g,
    std::uint8_t b,std::uint8_t a,std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.live_player||!self.loaded||!self.movie){
        error="Screen-space animation requires the retained live gameplay HUD movie";return false;
    }
    float logical[2]={screen_x_px,screen_y_px};
    if(!self.movie->screen_to_logical(logical,error))return false;
    const float x_twips=logical[0]*20.f,y_twips=logical[1]*20.f;
    if(!std::isfinite(x_twips)||!std::isfinite(y_twips)){
        error="Screen-space animation conversion exceeded the source SWF coordinate domain";return false;
    }
    return self.movie->play_authored_animation_text(style,slot,x_twips,y_twips,
                                                     text,r,g,b,a,error);
}
bool OriginalUiSession::source_combat_text_constant(const char* group,const char* name,
    std::int32_t& value,std::string& error){
    auto& self=*impl_;std::uint32_t raw{};
    if(!self.selected||!self.live_player||!self.loaded||!group||!name){
        error="Combat-text constants require the loaded live gameplay source owner";return false;
    }
    if(!Impl::constant(&self,group,name,raw,error))return false;
    std::memcpy(&value,&raw,sizeof(value));return true;
}
bool OriginalUiSession::gameplay_text_ready() const noexcept{
    const auto& self=*impl_;
    return self.selected&&self.live_player&&self.loaded;
}
bool OriginalUiSession::source_combat_text_string(std::int32_t string_id,
    std::string& value,std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.live_player||!self.loaded||string_id<0){
        error="Combat-text StringManager ID is invalid or gameplay is unloaded";return false;
    }
    return self.localization.string_id(static_cast<std::uint32_t>(string_id),
                                       self.text_services(),value,error);
}
bool OriginalUiSession::source_combat_text_format_int(std::int32_t string_id,
    std::int32_t argument,std::string& value,std::string& error){
    auto& self=*impl_;std::string pattern;
    if(!source_combat_text_string(string_id,pattern,error))return false;
    Impl::ParseContext parse{self,{}};
    const ui::HudTextServicesV1 services{&parse,Impl::parsed_text};
    const dh2::data::ItemTextArgumentV5 value_argument{
        static_cast<float>(argument),argument,nullptr};
    bool changed=false;
    return ui::item_text_varargs_v5(pattern.c_str(),&value_argument,1,services,
                                    value,changed,error);
}
bool OriginalUiSession::enqueue_source_status_item_pickup(
    const std::string& item_name,std::int32_t power_count,std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.movie||!self.gameplay_hud||!self.gameplay_hud_bound||
       item_name.find('\0')!=std::string::npos){
        error="StatusMsg item pickup requires the retained authored gameplay HUD";return false;
    }
    std::string formatted;
    if(!Impl::item_name_with_palette(self,item_name,power_count,formatted,error))return false;
    bool start_now=false;
    if(!self.source_status_messages.enqueue(std::move(formatted),start_now)){
        error="Retained StatusMsg queue rejected the pickup text";return false;
    }
    if(start_now&&!self.gameplay_hud->notify_status_message(error))return false;
    error.clear();return true;
}
bool OriginalUiSession::enqueue_source_status_auto_transmute(
    const std::string& item_name,std::int32_t power_count,std::int32_t payout,
    std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.movie||!self.gameplay_hud||!self.gameplay_hud_bound||
       item_name.find('\0')!=std::string::npos||power_count<0||payout<1){
        error="StatusMsg AutoTransmute producer requires a valid retained gameplay HUD and item";return false;
    }
    static constexpr const char* power_keys[]={"zero","one","two","three","four"};
    std::uint32_t palette_index=2;
    if(power_count<5){
        std::uint32_t raw=0;
        if(!Impl::constant(&self,"ItemPowerColor",power_keys[power_count],raw,error))return false;
        std::memcpy(&palette_index,&raw,sizeof(raw));
        if(static_cast<std::int32_t>(palette_index)<0){
            error="ItemPowerColor resolved to a negative FontPalette index";return false;
        }
    }
    std::uint32_t text_color=0;
    if(!ui::font_palette_text_color_at_index_v1(self.font_palette_bytes.data(),
            self.font_palette_bytes.size(),palette_index,text_color,error)||
       !self.ensure_localization_ready(error))return false;
    std::uint32_t raw_string_id=0;
    if(!Impl::constant(&self,"StrID","ITEMS_AUTO_TRANSMUTE",raw_string_id,error))return false;
    std::string pattern;
    if(!self.localization.string_id(raw_string_id,self.text_services(),pattern,error))return false;
    char color[16]{};
    std::snprintf(color,sizeof(color),"#%06X",unsigned(text_color&0x00ffffffu));
    const dh2::data::ItemTextArgumentV5 arguments[]={
        {0.f,0,color},{0.f,0,item_name.c_str()},{float(payout),payout,nullptr}};
    Impl::ParseContext parse{self,{}};
    const ui::HudTextServicesV1 services{&parse,Impl::parsed_text};
    std::string formatted;bool changed=false;
    if(!ui::item_text_varargs_v5(pattern.c_str(),arguments,3,services,
                                 formatted,changed,error))return false;
    bool start_now=false;
    if(!self.source_status_messages.enqueue(std::move(formatted),start_now)){
        error="Retained StatusMsg queue rejected AutoTransmute text";return false;
    }
    if(start_now&&!self.gameplay_hud->notify_status_message(error))return false;
    error.clear();return true;
}
bool OriginalUiSession::enqueue_source_status_constant(
    const char* group,const char* key,std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.movie||!self.gameplay_hud||!self.gameplay_hud_bound||!group||!key){
        error="StatusMsg localized producer requires the retained authored gameplay HUD";return false;
    }
    std::uint32_t string_id=0;
    if(!Impl::constant(&self,group,key,string_id,error)||
       !self.ensure_localization_ready(error))return false;
    std::string text;
    if(!self.localization.string_id(string_id,self.text_services(),text,error))return false;
    bool start_now=false;
    if(!self.source_status_messages.enqueue(std::move(text),start_now)){
        error="Retained StatusMsg queue rejected localized text";return false;
    }
    if(start_now&&!self.gameplay_hud->notify_status_message(error))return false;
    error.clear();return true;
}
bool OriginalUiSession::enqueue_source_dialog(std::int32_t actor_id,std::int32_t style_id,
    std::int32_t text_string_id,std::string& error){
    auto& self=*impl_;
    // The gameplay HUD SWF loads lazily on the first rendered player frame, but
    // level scripts (Script_StartDialog) can run in the first update, before it.
    if(self.selected&&!self.loaded&&self.live_player&&self.front_screen.empty()&&
       !self.load(error))return false;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||!self.movie){
        error="Source dialog enqueue requires the retained gameplay HUD session";return false;
    }
    // Script_StartDialog::Execute calls DebugSwitches::load/GetSwitch before
    // testing EnterLocationDialog or constructing the message.
    if(!self.debug_load(error)||!self.debug_query("isTracingScriptCmd",error))return false;
    if(!self.ensure_dialog_metadata_ready(error))return false;
    dh2_pycst_result enter_location{};
    if(dh2_pycst_get(&self.dialog_constants,"DialogStyles",sizeof("DialogStyles")-1,
           "EnterLocationDialog",sizeof("EnterLocationDialog")-1,&enter_location)!=0||
       !enter_location.found){error="DialogStyles.EnterLocationDialog constant is missing";return false;}
    if(style_id==enter_location.value&&self.source_dialog_messages.active()){
        // Source Execute flushes the existing queue and returns without
        // constructing/enqueuing the EnterLocationDialog message.
        self.source_dialog_messages.flush();error.clear();return true;
    }
    ui::SourceDialogMessageQueueV1::Message message;
    if(!self.source_dialog_string(0,message.title,error)||
       !self.source_dialog_string(text_string_id,message.text,error)||
       !self.source_dialog_style_name(style_id,message.style_name,error))return false;
    if(actor_id>=0){
        if(static_cast<std::size_t>(actor_id)>=self.dialog_actor_name_ids.size()){
            error="Source DialogMsg actor index exceeds DialogActors";return false;
        }
        if(!self.source_dialog_string(self.dialog_actor_name_ids[static_cast<std::size_t>(actor_id)],
                                      message.actor_name,error))return false;
    }
    message.style=style_id;
    bool start_now=false;
    if(!self.source_dialog_messages.enqueue(std::move(message),start_now)){
        error="Source DialogMsg queue rejected its message";return false;
    }
    if(start_now&&!self.movie->action_script(&self,Impl::invoke_source_dialog_start,error))
        return false;
    error.clear();return true;
}
bool OriginalUiSession::enqueue_source_dialog_id(std::int32_t dialog_id,
    std::string& error){
    auto& self=*impl_;
    if(self.selected&&!self.loaded&&self.live_player&&self.front_screen.empty()&&
       !self.load(error))return false;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.movie){
        error="Source dialog ID enqueue requires the retained gameplay HUD session";return false;
    }
    // Script_StartDialogID::Execute loads/query-checks the script trace switch,
    // then resolves a Dialogs row and queues all of its DialogSteps before
    // invoking the existing SWF StartDialog callback.
    if(!self.debug_load(error)||!self.debug_query("isTracingScriptCmd",error)||
       !self.ensure_dialog_metadata_ready(error))return false;
    if(dialog_id<0||static_cast<std::size_t>(dialog_id)>=self.source_dialog_table.size()){
        // GetDialogMsgListById returns false after its source assertion for an
        // invalid index. Execute then completes without queueing a message.
        error.clear();return true;
    }
    const auto& steps=self.source_dialog_table[static_cast<std::size_t>(dialog_id)];
    if(steps.empty()){error.clear();return true;}
    std::vector<ui::SourceDialogMessageQueueV1::Message> messages;
    messages.reserve(steps.size());
    for(const auto& step:steps){
        ui::SourceDialogMessageQueueV1::Message message;
        message.style=static_cast<std::int8_t>(static_cast<std::uint8_t>(step.style));
        if(step.title>=0&&!self.source_dialog_string(step.title,message.title,error))return false;
        if(step.text>=0&&!self.source_dialog_string(step.text,message.text,error))return false;
        if(step.style>=0&&!self.source_dialog_style_name(message.style,message.style_name,error))
            return false;
        if(step.actor>=0){
            if(static_cast<std::size_t>(step.actor)>=self.dialog_actor_name_ids.size()){
                error="Source DialogStep actor index exceeds DialogActors";return false;
            }
            if(!self.source_dialog_string(
                self.dialog_actor_name_ids[static_cast<std::size_t>(step.actor)],
                message.actor_name,error))return false;
        }
        messages.push_back(std::move(message));
    }
    bool start_now=false;
    for(auto& message:messages){
        if(!self.source_dialog_messages.enqueue(std::move(message),start_now)){
            error="Source DialogMsg queue rejected a DialogStep";return false;
        }
    }
    // Execute checks the final queue size after all false-start enqueues; only
    // a one-message queue causes the source StartDialog callback.
    if(self.source_dialog_messages.size()==1&&
       !self.movie->action_script(&self,Impl::invoke_source_dialog_start,error))
        return false;
    error.clear();return true;
}
bool OriginalUiSession::enqueue_source_new_quest_message(std::int32_t quest_name_string_id,
    std::string& error){
    auto& self=*impl_;
    if(self.selected&&!self.loaded&&self.live_player&&self.front_screen.empty()&&
       !self.load(error))return false;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.movie){
        error="Quest::SetState(Active) DialogMsg requires the retained gameplay HUD session";
        return false;
    }
    // Quest::SetState(Active) 0x480d00: DialogMsg(StrID.GLOBAL_QUEST_NEW, the
    // Quest's name string, DialogStyles.QuestMsgDialog, -1) is enqueued on the
    // same MenuMessageManager<DialogMsg, 1> the Script_StartDialog family uses.
    std::uint32_t title_id=0;
    if(!Impl::constant(&self,"StrID","GLOBAL_QUEST_NEW",title_id,error)||
       !self.ensure_dialog_metadata_ready(error))return false;
    dh2_pycst_result style_constant{};
    if(dh2_pycst_get(&self.dialog_constants,"DialogStyles",sizeof("DialogStyles")-1,
           "QuestMsgDialog",sizeof("QuestMsgDialog")-1,&style_constant)!=0||
       !style_constant.found){error="DialogStyles.QuestMsgDialog constant is missing";return false;}
    ui::SourceDialogMessageQueueV1::Message message;
    message.style=static_cast<std::int8_t>(static_cast<std::uint8_t>(style_constant.value));
    if(!self.source_dialog_string(static_cast<std::int32_t>(title_id),message.title,error)||
       !self.source_dialog_string(quest_name_string_id,message.text,error)||
       !self.source_dialog_style_name(message.style,message.style_name,error))return false;
    bool start_now=false;
    if(!self.source_dialog_messages.enqueue(std::move(message),start_now)){
        error="Source DialogMsg queue rejected the new-quest message";return false;
    }
    if(start_now&&!self.movie->action_script(&self,Impl::invoke_source_dialog_start,error))
        return false;
    error.clear();return true;
}
bool OriginalUiSession::source_quest_reward_text(std::int32_t kind,std::int32_t amount,
    std::string& line,std::string& error){
    auto& self=*impl_;
    // RewardList::CreateRewardString (0x4835a0): kind 0 Gold and 1 XP format
    // their StrID detail string with the reward amount through StringManager::
    // parse; kind 2 is StrID.MENU_AUTOTRANSMUTE_1, assigned verbatim when no
    // reward produced a line.
    const char* name=kind==0?"GAMEPLAYMENUS_REWARD_DETAIL_GOLD":
                     kind==1?"GAMEPLAYMENUS_REWARD_DETAIL_EXP":"MENU_AUTOTRANSMUTE_1";
    std::uint32_t id=0;
    std::string pattern;
    if(!Impl::constant(&self,"StrID",name,id,error)||!self.ensure_localization_ready(error)||
       !self.localization.string_id(id,self.text_services(),pattern,error))return false;
    line.clear();
    if(kind==2){line=std::move(pattern);error.clear();return true;}
    return Impl::format_integer_text(self,pattern.c_str(),amount,line,error);
}
bool OriginalUiSession::enqueue_source_quest_completed_message(std::int32_t quest_name_string_id,
    const std::string& reward_text,std::string& error){
    auto& self=*impl_;
    if(self.selected&&!self.loaded&&self.live_player&&self.front_screen.empty()&&
       !self.load(error))return false;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.movie){
        error="Quest::SetState(Closed) DialogMsg requires the retained gameplay HUD session";
        return false;
    }
    // Quest::SetState(Closed) 0x4810d8: DialogMsg(StringManager name of the
    // Quest, its RewardList text, DialogStyles.QuestCompletedMsgDialog, actor 1).
    if(!self.ensure_dialog_metadata_ready(error))return false;
    dh2_pycst_result style_constant{};
    if(dh2_pycst_get(&self.dialog_constants,"DialogStyles",sizeof("DialogStyles")-1,
           "QuestCompletedMsgDialog",sizeof("QuestCompletedMsgDialog")-1,&style_constant)!=0||
       !style_constant.found){error="DialogStyles.QuestCompletedMsgDialog constant is missing";return false;}
    ui::SourceDialogMessageQueueV1::Message message;
    message.style=static_cast<std::int8_t>(static_cast<std::uint8_t>(style_constant.value));
    message.text=reward_text;
    if(!self.source_dialog_string(quest_name_string_id,message.title,error)||
       !self.source_dialog_parse_player(message.text,error)||
       !self.source_dialog_style_name(message.style,message.style_name,error))return false;
    constexpr std::size_t actor_id=1;
    if(actor_id>=self.dialog_actor_name_ids.size()){
        error="Source DialogMsg actor index exceeds DialogActors";return false;
    }
    if(!self.source_dialog_string(self.dialog_actor_name_ids[actor_id],message.actor_name,error))
        return false;
    bool start_now=false;
    if(!self.source_dialog_messages.enqueue(std::move(message),start_now)){
        error="Source DialogMsg queue rejected the quest-completed message";return false;
    }
    if(start_now&&!self.movie->action_script(&self,Impl::invoke_source_dialog_start,error))
        return false;
    error.clear();return true;
}
bool OriginalUiSession::lock_source_tutorial(std::int32_t id,std::string& error){
    auto& self=*impl_;
    if(!self.settings){error="Script_LockTutorial requires the retained settings owner";return false;}
    if(!self.settings->lock_tutorial(id)){error.clear();return true;}
    return Impl::save_settings(impl_.get(),error);
}
bool OriginalUiSession::show_script_flash(const std::string& name,std::string& error){
    // Script_ShowFlash::Execute tests strstr(name,"HUD"). That branch runs
    // DisplayRightHud/FillActionIcon/DisplayOfCurrentHud (set_source_hud_shown);
    // the script host routes HUD-named flashes there, never to this menu path.
    if(name.find("HUD")!=std::string::npos){
        error="ShowFlash HUD-named branch is not reconstructed";return false;
    }
    if(name.empty()||name.find('\0')!=std::string::npos){
        error="ShowFlash requires a menu name";return false;
    }
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||!self.movie){
        error="ShowFlash requires the retained gameplay HUD movie";return false;
    }
    struct Flash {const std::string& name;} context{name};
    return self.movie->menu_action_script(&context,
        [](void* raw,ui::SwfAsGraph& graph,std::string& nested_error){
            const auto& flash=static_cast<Flash*>(raw)->name;
            ui::SwfAsValue root,menu,visibility,result;
            bool visible=false,accepted=false,callable=false,found=false;
            if(!graph.root_value(root,nested_error)||
               !graph.find_target(root,("_root."+flash).c_str(),menu,nested_error))return false;
            // MenuManager::GetMenuByName returning null is a source no-op.
            if(!menu.identity()){nested_error.clear();return true;}
            if(!graph.get_member(menu,"_visible",visibility,found,nested_error))return false;
            if(!found){nested_error="Authored flash visibility member absent";return false;}
            if(!graph.to_boolean(visibility,visible,nested_error))return false;
            // MenuBase::Show returns early when the menu is already visible.
            if(visible){nested_error.clear();return true;}
            if(!graph.set_member(menu,"_visible",ui::SwfAsValue::boolean(true),
                                 accepted,nested_error))return false;
            if(!accepted){nested_error="Authored flash visibility write rejected";return false;}
            // MenuBase::Show invokes onPush through RenderFX::InvokeASCallback.
            return graph.invoke(menu,menu,"onPush",{},result,callable,nested_error);
        },error);
}
bool OriginalUiSession::hide_script_flash(const std::string& name,std::string& error){
    // Script_HideFlash::Execute uses the same HUD-name split as ShowFlash.
    if(name.find("HUD")!=std::string::npos){
        error="HideFlash HUD-named branch is not reconstructed";return false;
    }
    if(name.empty()||name.find('\0')!=std::string::npos){
        error="HideFlash requires a menu name";return false;
    }
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||!self.movie){
        error="HideFlash requires the retained gameplay HUD movie";return false;
    }
    struct Flash {const std::string& name;} context{name};
    return self.movie->menu_action_script(&context,
        [](void* raw,ui::SwfAsGraph& graph,std::string& nested_error){
            const auto& flash=static_cast<Flash*>(raw)->name;
            ui::SwfAsValue root,menu,visibility,result;
            bool visible=false,accepted=false,callable=false,found=false;
            if(!graph.root_value(root,nested_error)||
               !graph.find_target(root,("_root."+flash).c_str(),menu,nested_error))return false;
            if(!menu.identity()){nested_error.clear();return true;}
            if(!graph.get_member(menu,"_visible",visibility,found,nested_error))return false;
            if(!found){nested_error="Authored flash visibility member absent";return false;}
            if(!graph.to_boolean(visibility,visible,nested_error))return false;
            // MenuBase::Hide returns early when the menu is already hidden.
            if(!visible){nested_error.clear();return true;}
            if(!graph.set_member(menu,"_visible",ui::SwfAsValue::boolean(false),
                                 accepted,nested_error))return false;
            if(!accepted){nested_error="Authored flash visibility write rejected";return false;}
            return graph.invoke(menu,menu,"onPop",{},result,callable,nested_error);
        },error);
}
bool OriginalUiSession::set_source_hud_shown(bool show,std::string& error){
    error.clear();
    if(!impl_){error="HUD flash requires the UI session";return false;}
    auto& self=*impl_;
    if(!self.loaded||!self.movie||!self.gameplay_hud||!self.gameplay_hud_bound)return true;
    if(show&&(!self.gameplay_hud->activate(error)||!self.gameplay_hud->refresh_action_icon(error)))
        return false;
    return self.gameplay_hud->display_of_current_hud(show,error);
}
bool OriginalUiSession::source_tutorial_active(std::int32_t id,bool& active,std::string& error)const{
    active=false;
    const auto& self=*impl_;
    if(!self.settings){error="Settings tutorial byte requires the retained settings owner";return false;}
    const auto& tutorials=self.settings->tutorials();
    if(id<0||std::size_t(id)>=tutorials.size()){
        error="Settings tutorial id is out of range";return false;
    }
    active=tutorials[std::size_t(id)]!=0;error.clear();return true;
}
void OriginalUiSession::queue_level_transition(const std::string& destination,double raw_entry_point){
    NativeStartGameIntentV1 request{};
    request.kind=NativeStartGameIntentV1::Kind::level_transition;
    request.destination_name=destination;
    request.raw_entry_point=raw_entry_point;
    impl_->launch_requests.push_back(std::move(request));
    __android_log_print(ANDROID_LOG_INFO,tag,"Script ChangeLevel queued | %s | raw entry %.9g",
        destination.c_str(),raw_entry_point);
}
bool OriginalUiSession::flush_source_messages(std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()){
        error="Source message flush requires the retained gameplay HUD session";return false;
    }
    self.source_dialog_messages.flush();
    self.source_status_messages.flush();
    error.clear();return true;
}
bool OriginalUiSession::source_dialog_active(bool& active,std::string& error)const{
    const auto& self=*impl_;
    if(self.selected&&!self.loaded&&self.live_player&&self.front_screen.empty()){
        // Nothing can have been queued before the HUD exists.
        active=false;error.clear();return true;
    }
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()){
        error="Source dialog activity requires the retained gameplay HUD session";return false;
    }
    active=self.source_dialog_messages.active();error.clear();return true;
}
bool OriginalUiSession::enqueue_source_char_menu_tutorial(std::int32_t string_id,
    const std::string& frame,const std::string& menu,std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||
       !self.movie||frame.find('\0')!=std::string::npos||
       menu.find('\0')!=std::string::npos){
        error="CharMenuTutorialMsg enqueue requires the retained gameplay CharacterMenu UI";return false;
    }
    self.source_char_menu_tutorial_messages.enqueue(string_id,frame,menu);
    error.clear();return true;
}
bool OriginalUiSession::skip_source_char_menu_tutorial(bool skip_all,std::string& error){
    auto& self=*impl_;
    if(!self.selected||!self.loaded||!self.live_player||!self.front_screen.empty()||!self.movie){
        error="CharMenuTutorialMsg skip requires the retained gameplay CharacterMenu UI";return false;
    }
    if(skip_all)self.source_char_menu_tutorial_messages.skip_all();
    else self.source_char_menu_tutorial_messages.skip_one();
    error.clear();return true;
}
void OriginalUiSession::deactivate(){impl_->front_name_input_active.store(false,std::memory_order_release);impl_->selected=false;}
bool OriginalUiSession::resume_front_after_start_failure(std::string& error){
    if(impl_->front_screen!="main"||!impl_->loaded){
        error="Failed NativeStartGame has no retained authored front-menu owner";return false;
    }
    impl_->selected=true;impl_->report_frame=true;
    impl_->frame_time=std::chrono::steady_clock::now();error.clear();
    __android_log_print(ANDROID_LOG_INFO,tag,
        "Authored menu resumed after failed NativeStartGame | current %s | slot assignment and source menu stack retained",
        impl_->active_menu_path().c_str());
    return true;
}

bool OriginalUiSession::attach_player(const std::string& directory,std::string& error){
    error.clear();
    if(!valid_private_directory(directory,error,"Required private HUD directory unavailable"))return false;
    if(!impl_->settings||!impl_->settings->has_option("HUDStyle")){
        error="Gameplay HUD requires the retained Application settings owner and HUDStyle";return false;
    }
    const auto hud_style=impl_->settings->option("HUDStyle");
    if(hud_style<0||hud_style>3){error="Saved HUDStyle is outside the four authored Android layouts";return false;}
    const bool retain_loaded_gameplay_ui=impl_->loaded&&impl_->front_screen.empty()&&impl_->live_player;
    if(retain_loaded_gameplay_ui&&(!impl_->gameplay_hud||!impl_->gameplay_hud_bound||!impl_->status)){
        error="Retained gameplay HUD movie has incomplete renderer bindings";return false;
    }
    // Bind before retiring the front movie so this ownership conflict cannot
    // turn a recoverable launch failure into a destroyed menu.
    if(!data::savegame_options_v1::bind_read_only_settings(impl_->settings.get())){
        error="Another Application settings owner is still bound to gameplay";return false;
    }
    if(!impl_->front_screen.empty()){
        // Application settings survive the menu→level renderer handoff in
        // source. Keep the existing owners while retiring the menu SWF; the
        // settings object still borrows Impl::option_table, which reset_failed
        // deliberately leaves intact. No second settings/option owner is made.
        auto retained_settings=std::move(impl_->settings);
        auto retained_files=std::move(impl_->settings_files);
        const bool reset=impl_->reset_failed(error);
        impl_->settings=std::move(retained_settings);
        impl_->settings_files=std::move(retained_files);
        if(!reset)return false;
        impl_->front_screen.clear();
    }
    impl_->hud_style=hud_style;
    impl_->directory=directory;impl_->live_player=true;impl_->selected=true;impl_->report_frame=true;
    if(!retain_loaded_gameplay_ui){
        impl_->gameplay_hud_bound=false;impl_->gameplay_hud_pointers.fill(-1);impl_->camera_fingers.reset();impl_->gameplay_enemy_hud_state={};impl_->gameplay_enemy_hud_player_identity=0;impl_->gameplay_hud_skills_refresh={};
    }else{
        __android_log_print(ANDROID_LOG_INFO,tag,
            "Gameplay HUD binding retained across surface recreation | style %d | movie %p | status %p",
            impl_->hud_style,static_cast<void*>(impl_->gameplay_hud.get()),static_cast<void*>(impl_->status.get()));
    }
    error.clear();return true;
}
bool OriginalUiSession::render_player(int width,int height,const std::int32_t* sheet,std::size_t count,
                                     std::uintptr_t character,std::string& error){
    if(!overlays_player()){error="Connected player HUD inactive";return false;}
    auto& self=*impl_;
    try{
        const auto require=[&](bool ok,const char* operation){
            if(!ok)throw std::runtime_error(error.empty()?operation:error+" | during: "+operation);
        };
        if(!self.loaded){self.driver_width=width;self.driver_height=height;require(self.load(error),"Gameplay UI load failed without a diagnostic");}
        require(self.viewport(width,height,error),"Gameplay UI viewport update failed without a diagnostic");
        if(self.character_menu_movie){
            const auto now=std::chrono::steady_clock::now();
            const auto elapsed=now-self.game_menu_frame_time;self.game_menu_frame_time=now;
            const auto milliseconds=self.game_menu_clock.advance(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed));
            self.last_menu_dt=milliseconds;
            const auto active_name=self.game_menu_stack.empty()?std::string():self.game_menu_stack.back();
            auto* active=active_name.empty()?self.movie.get():self.menu_movie(active_name);
            auto* inactive=active==self.character_menu_movie.get()?self.movie.get():self.character_menu_movie.get();
            auto& active_owner=active==self.character_menu_movie.get()?self.character_frame_owner:self.frame_owner;
            auto& inactive_owner=inactive==self.character_menu_movie.get()?self.character_frame_owner:self.frame_owner;
            require(inactive->advance_frames(milliseconds,inactive_owner->frames,error),"Inactive menu timeline update failed without a diagnostic");
            struct Dispatch {Impl& self;ui::SwfMovie* previous_movie;ui::SwfFrameConnection* previous_frames;
                ~Dispatch(){self.input_dispatch_movie=previous_movie;self.input_dispatch_frames=previous_frames;}}
                dispatch{self,self.input_dispatch_movie,self.input_dispatch_frames};
            self.input_dispatch_movie=active;self.input_dispatch_frames=&active_owner->frames;
            // Source MenuMerchant::Update clears its NativeMerchantSellItem
            // one-shot byte every frame before MenuBase::Update advances the
            // menu. Mirror that cadence so a sale can run again after the next
            // authored frame while still rejecting duplicate calls in one tick.
            if(active_name=="menu_Merchant")self.merchant_sell_one_shot_used=false;
            require(active->input_advance(milliseconds,error),"Active menu timeline update failed without a diagnostic");
            const auto hud_rectangle=self.viewport_rectangle();
            const auto menu_rectangle=self.front_rectangle();
            require(self.movie->input_rectangle(hud_rectangle.data(),error),"HUD hit rectangle update failed without a diagnostic");
            require(self.character_menu_movie->input_rectangle(menu_rectangle.data(),error),"Gameplay menu hit rectangle update failed without a diagnostic");
        }else{
            // The retained gameplay HUD still owns its SWF clock when the
            // optional character-menu movie is unavailable. Advance it once
            // through its canonical frame owner so authored combat-text clones
            // animate and retire; the menu-present branch above already does
            // this as active or inactive and must not be advanced twice.
            const auto now=std::chrono::steady_clock::now();
            if(self.game_menu_frame_time.time_since_epoch().count()==0){
                self.game_menu_clock.reset();self.game_menu_frame_time=now;
            }
            const auto elapsed=now-self.game_menu_frame_time;self.game_menu_frame_time=now;
            const auto milliseconds=self.game_menu_clock.advance(
                std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed));
            if(!self.movie||!self.frame_owner)
                throw std::runtime_error("Gameplay SWF frame owner is unavailable");
            struct Dispatch {Impl& self;ui::SwfMovie* previous_movie;ui::SwfFrameConnection* previous_frames;
                ~Dispatch(){self.input_dispatch_movie=previous_movie;self.input_dispatch_frames=previous_frames;}}
                dispatch{self,self.input_dispatch_movie,self.input_dispatch_frames};
            self.input_dispatch_movie=self.movie.get();
            self.input_dispatch_frames=&self.frame_owner->frames;
            require(self.movie->input_advance(milliseconds,error),
                    "Gameplay SWF timeline update failed without a diagnostic");
        }
        // The DialogBox hide animation ends in NativeStopMessage("dialog"), which
        // pops the DialogMsg; the next queued message starts now that the timeline
        // dispatch has unwound.
        require(Impl::flush_pending_dialog_start(self,error),
                "Queued DialogMsg StartDialog failed without a diagnostic");
        require(self.status->update(sheet,count,character,error),"Status panel update failed without a diagnostic");
        require(Impl::flush_deferred_faery_hud_skills(self,error),
                "Deferred active Faery HUD skills refresh failed without a diagnostic");
        if(self.gameplay_hud_active&&self.gameplay_hud_bound&&self.authored_joystick_ready){
            // render_player is called only for the attached local player in
            // the active level. Keep joystick Update on the source HUDControls
            // cadence and use the same Character owner as the status panel.
            std::uintptr_t identity=0;
            if(!model_renderer::ui_player_identity(identity,error))throw std::runtime_error(error);
            if(identity!=character){
                std::ostringstream out;out<<"Authored HUD player identity changed | status "<<character<<" | movement "<<identity;
                throw std::runtime_error(out.str());
            }
            require(ui::authored_joystick_update_v1(self.authored_joystick,self.authored_joystick_services(),error),
                    "Authored joystick update failed without a diagnostic");
        }
        if(self.gameplay_hud_active&&self.gameplay_hud_bound&&self.gameplay_hud&&
           self.hud_control_held(static_cast<int>(ui::AuthoredHudControlV1::attack))){
            // Source HUDControls::Update polls the pressed attack flag every
            // player frame. Keep using the shared model_renderer combat owner;
            // its return text is intentionally not logged at frame cadence.
            (void)model_renderer::player_attack(-1);
        }
        if(self.gameplay_hud_active&&self.gameplay_hud_bound&&self.gameplay_hud){
            std::uintptr_t identity=0;
            if(!model_renderer::ui_player_identity(identity,error))throw std::runtime_error(error);
            if(identity!=character){
                std::ostringstream out;out<<"Authored HUD state identity changed | status "<<character<<" | projection "<<identity;
                throw std::runtime_error(out.str());
            }
            const auto now=std::chrono::steady_clock::now();
            const bool refresh_usable=self.gameplay_hud_usable_refresh.due(now);
            auto projection=self.gameplay_hud_state;
            std::string projection_error;
            if(!model_renderer::ui_player_hud_projection(identity,refresh_usable,projection,projection_error)){
                if(!self.gameplay_hud_projection_warned){
                    __android_log_print(ANDROID_LOG_WARN,tag,
                        "Authored HUD live state projection unavailable | character %p | %s",
                        reinterpret_cast<void*>(identity),projection_error.c_str());
                    self.gameplay_hud_projection_warned=true;
                }
            }else{
                const bool dpad=self.settings&&self.settings->saved_option("DPad")!=0;
                require(self.gameplay_hud->update_action_icon(
                            projection.object_of_interest_type,error),
                        "Gameplay HUD action icon update failed without a diagnostic");
                require(self.gameplay_hud->update(projection.infos,projection.skill_slots,
                                                 projection.class_id,dpad,error),
                        "Gameplay HUD state update failed without a diagnostic");
                model_renderer::UiEnemyHudProjectionV1 enemy_projection{};
                require(model_renderer::ui_player_enemy_hud_projection(
                            identity,enemy_projection,error),
                        "Gameplay enemy HUD target projection failed without a diagnostic");
                ui::HudManagerActor enemy_ai{},enemy_character{},enemy_player{};
                const auto bind_enemy_actor=[](const model_renderer::UiEnemyHudActorV1& source,
                                                ui::HudManagerActor& target){
                    target.name_symbol=static_cast<std::uint32_t>(source.name_symbol);
                    target.debug_name=source.debug_name;target.network_id=source.network_id;
                    target.identity=source.identity;
                };
                if(enemy_projection.ai_target.present)
                    bind_enemy_actor(enemy_projection.ai_target,enemy_ai);
                if(enemy_projection.character_target.present)
                    bind_enemy_actor(enemy_projection.character_target,enemy_character);
                enemy_player.identity=enemy_projection.player_identity;
                enemy_player.target=enemy_projection.character_target.present?&enemy_character:nullptr;
                struct EnemyHudContext {
                    OriginalUiSession* owner;
                    ui::AuthoredGameplayHudV1* hud;
                    const model_renderer::UiEnemyHudProjectionV1* projection;
                    ui::HudManagerActor* player;
                    ui::HudManagerActor* ai;
                    ui::HudManagerActor* character;
                    std::string localized,error;
                    // Enemy kernel's displayed target (its hp_fraction query is the last).
                    const model_renderer::UiEnemyHudActorV1* shown{};
                } enemy_context{this,self.gameplay_hud.get(),&enemy_projection,
                                &enemy_player,&enemy_ai,&enemy_character,{},{}};
                const ui::HudManagerServices enemy_services{&enemy_context,
                    +[](void* opaque,ui::HudManagerState*,const ui::HudManagerRequest* request,
                        ui::HudManagerResponse* response)->int {
                        auto& context=*static_cast<EnemyHudContext*>(opaque);
                        if(!request||!response)return 0;
                        const auto actor_for=[&](std::uintptr_t subject)->const model_renderer::UiEnemyHudActorV1*{
                            if(subject==reinterpret_cast<std::uintptr_t>(context.ai)&&
                               context.projection->ai_target.present)return &context.projection->ai_target;
                            if(subject==reinterpret_cast<std::uintptr_t>(context.character)&&
                               context.projection->character_target.present)return &context.projection->character_target;
                            return nullptr;
                        };
                        const auto fail=[&](const char* message){context.error=message;return 0;};
                        switch(request->operation){
                        case ui::HudManagerOperation::cache_get:
                            if(request->index<19||request->index>22)return fail("Enemy HUD requested an unexpected cache slot");
                            response->identity=request->index+1;return 1;
                        case ui::HudManagerOperation::is_dead:
                            if(request->subject!=reinterpret_cast<std::uintptr_t>(context.player))return fail("Enemy HUD dead check escaped the local player");
                            response->value=context.projection->player_dead?1:0;return 1;
                        case ui::HudManagerOperation::target_character:
                            if(request->subject!=reinterpret_cast<std::uintptr_t>(context.player))return fail("Enemy HUD target query escaped the local player");
                            response->identity=context.projection->ai_target.present?reinterpret_cast<std::uintptr_t>(context.ai):0;return 1;
                        case ui::HudManagerOperation::is_character:
                        case ui::HudManagerOperation::is_monster:
                        case ui::HudManagerOperation::is_boss:
                        case ui::HudManagerOperation::level:
                        case ui::HudManagerOperation::hp_fraction:{
                            const auto* actor=actor_for(request->subject);if(!actor)return fail("Enemy HUD queried an unknown borrowed Character view");
                            if(request->operation==ui::HudManagerOperation::is_character)response->value=actor->is_character;
                            else if(request->operation==ui::HudManagerOperation::is_monster)response->value=actor->is_monster;
                            else if(request->operation==ui::HudManagerOperation::is_boss)response->value=actor->is_boss;
                            else if(request->operation==ui::HudManagerOperation::level)response->value=actor->level;
                            else response->fraction=actor->hp_fraction;
                            if(request->operation==ui::HudManagerOperation::hp_fraction)context.shown=actor;
                            return 1;
                        }
                        case ui::HudManagerOperation::string_symbol:
                            if(!actor_for(request->subject))return fail("Enemy HUD localized a non-target Character");
                            if(!context.owner->source_combat_text_string(request->value,context.localized,context.error))return 0;
                            response->text=context.localized.c_str();return 1;
                        case ui::HudManagerOperation::debug_load:return 1;
                        case ui::HudManagerOperation::debug_switch:response->value=0;return 1;
                        case ui::HudManagerOperation::visible:
                            // Run::visible routes the resolved cache identity in
                            // subject and leaves index at zero. Cache slot 19
                            // resolves to this enemy clip's token (20).
                            if(request->subject!=20)return fail("Enemy HUD visibility target differs from authored source");
                            return context.hud->enemy_visible(request->value!=0,context.error)?1:0;
                        case ui::HudManagerOperation::goto_label:
                            if(request->index!=19||!request->text)return fail("Enemy HUD label target differs from authored source");
                            return context.hud->enemy_label(request->text,context.error)?1:0;
                        case ui::HudManagerOperation::text:
                            if(!request->text)return fail("Enemy HUD text payload is missing");
                            return context.hud->enemy_text(request->index,request->text,context.error)?1:0;
                        case ui::HudManagerOperation::sprite_type:{
                            if(request->index!=22)return fail("Enemy HUD sprite target differs from authored source");
                            bool sprite=false;if(!context.hud->enemy_bar_is_sprite(sprite,context.error))return 0;
                            response->value=sprite?1:0;return 1;
                        }
                        case ui::HudManagerOperation::goto_frame:
                            if(request->index!=22)return fail("Enemy HUD frame target differs from authored source");
                            return context.hud->enemy_frame(request->value,false,context.error)?1:0;
                        case ui::HudManagerOperation::play_state:
                            if(request->index!=22||request->value!=1)return fail("Enemy HUD play-state request is unsupported");
                            return context.hud->enemy_play(context.error)?1:0;
                        default:return fail("Enemy HUD source kernel requested an unadapted operation");
                        }
                    }};
                if(self.gameplay_enemy_hud_player_identity!=identity){
                    self.gameplay_enemy_hud_state.cached_target_identity=0;
                    self.gameplay_enemy_hud_player_identity=identity;
                }
                self.gameplay_enemy_hud_state.render_fx=self.movie->player_identity();
                const auto enemy_result=dh2_ui_hud_enemy_v1(
                    &self.gameplay_enemy_hud_state,&enemy_player,&enemy_services);
                if(enemy_result){
                    error=enemy_context.error.empty()?"Authored enemy HUD source projection failed":enemy_context.error;
                    throw std::runtime_error(error);
                }
                self.gameplay_enemy_follow_identity=enemy_context.shown?enemy_context.shown->identity:0;
                self.gameplay_hud_state=projection;
                if(refresh_usable)self.gameplay_hud_usable_refresh.refreshed(now);
                self.gameplay_hud_projection_warned=false;
            }
        }
        if(self.gameplay_hud_active){
            if(!self.gameplay_hud||!self.gameplay_hud_bound)
                throw std::runtime_error("Gameplay HUD renderer owner is not bound");
            require(self.gameplay_hud->display(error),"Gameplay HUD clip display failed without a diagnostic");
            // DELIBERATE USER-APPROVED DEVIATION: after display() has laid the HUD
            // out, move HealthBars.enemy so its bounds centre sits on the displayed
            // target's head point. Runs post-display so parent world matrices are valid.
            const auto enemy_follow_identity=self.gameplay_enemy_follow_identity;
            self.gameplay_enemy_follow_identity=0;
            if(enemy_follow_identity){
                float head[3]{},view_projection[16]{};int viewport_width=0,viewport_height=0;bool valid=false;
                require(model_renderer::ui_enemy_target_head_projection(enemy_follow_identity,head,view_projection,
                            viewport_width,viewport_height,valid,error),"Enemy HUD follow target projection failed without a diagnostic");
                if(valid)require(self.gameplay_hud->enemy_follow(head,view_projection,viewport_width,viewport_height,error),
                            "Enemy HUD follow placement failed without a diagnostic");
            }
            require(self.movie->display_source_clip("_root.HurtCorners",error),
                    "HUD hurt-corner clip display failed without a diagnostic");
            require(self.movie->display_authored_animations(error),
                    "Authored gameplay animations failed to display without a diagnostic");
            // Root depth order: HurtCorners 3, menu_HUD_n 17, combat text 337..359,
            // DialogBox 365. The dialog was never drawn, so a script's StartDialog
            // (tutorials, quest updates, NPC talk) showed nothing while its
            // WaitDialog held every controller locked.
            require(self.movie->display_source_clip("_root.DialogBox",error),
                    "HUD dialog clip display failed without a diagnostic");
        }
        if(!self.game_menu_stack.empty()){
            const auto rectangle=self.front_rectangle();
            require(ui::gameplay_menu_draw_stack_v1(self.game_menu_stack,[&](const std::string& name){
                auto* renderer=self.menu_movie(name);
                if(!renderer){error="Gameplay menu renderer unavailable for draw";return false;}
                if(!renderer->display_clip(("_root."+name).c_str(),
                    rectangle[0],rectangle[1],rectangle[2],rectangle[3],error))return false;
                if(name=="menu_confirm2"&&!self.confirmation_state_probed){
                    ui::SwfClipInfo state;std::string probe_error;
                    if(renderer->clip("_root.menu_confirm2",state,probe_error)){
                        self.confirmation_state_probed=true;
                        __android_log_print(ANDROID_LOG_INFO,tag,
                            "Confirmation SWF state | visible %d | frame %d/%d | depth %d | local %.2f %.2f %.2f %.2f %.2f %.2f | world %.2f %.2f %.2f %.2f %.2f %.2f",
                            state.visible,state.frame,state.frames,state.depth,
                            state.local.value[0],state.local.value[1],state.local.value[2],state.local.value[3],state.local.value[4],state.local.value[5],
                            state.world.value[0],state.world.value[1],state.world.value[2],state.world.value[3],state.world.value[4],state.world.value[5]);
                    }
                }
                return true;
            }),"Gameplay menu stack draw failed without a diagnostic");
        }
        const auto frames=self.status->frames();
        if(self.report_frame||frames!=self.reported_frames){
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Connected player HUD submitted | active %d | menu depth %zu | viewport %d %d | character %p | HP %d %d | MP %d %d | XP %d %d | frames %d %d %d %d %d | dirty %zu | retained movie %p | original timeline and viewport",
                self.gameplay_hud_active,self.game_menu_stack.size(),
                width,height,reinterpret_cast<void*>(character),sheet[36],sheet[38],sheet[41],sheet[43],sheet[33],sheet[34],
                frames[0],frames[1],frames[2],frames[3],frames[4],self.status->dirty_nodes(),static_cast<void*>(self.movie.get()));
            self.reported_frames=frames;self.report_frame=false;
        }
        // The death fade timeline calls NativeReviveAllPlayers at its frame 88
        // during this advance; flush it once the SWF dispatch has unwound.
        if(self.gameplay_native_revive_pending&&
           !Impl::flush_deferred_native_revive(self,error))return false;
        if(!Impl::flush_deferred_back_to_hud(self,error))return false;
        return true;
    }catch(const std::exception& ex){
        const std::string failure=ex.what();std::string cleanup;self.reset_failed(cleanup);
        error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
    }
}
}
