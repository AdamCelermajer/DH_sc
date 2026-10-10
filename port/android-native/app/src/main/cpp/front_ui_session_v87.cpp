#include <menu_end_loading_source_v114.hpp>
#include "swf_menu_device_v1.hpp"
#include "gameswf/gameswf_function.h"
#include "loading_menu_v1.hpp"
#include "swf_loading_menu_v1.hpp"
#include "front_ui_session_v87.hpp"
#include "character_debug_stdio_v136.hpp"
#include "front_loading_render_policy_v1.hpp"
#include "front_scene_selection_v1.hpp"
#include "front_inspection_avatar_services_v1.hpp"
#include "original_ui_assets.hpp"
#include "swf_gpu.hpp"
#include "splash_layout_v1.hpp"
#include "swf_hud_freetype_provider.hpp"
#include "swf_text_font_platform_v1.hpp"
#include "gfnt_text_backend_v1.hpp"
#include "hud_freetype_font_v2.hpp"
#include "swf_font_resolver.hpp"
#include "localization.hpp"
#include "hud_text_v1.hpp"
#include "application_services_owner_v5.hpp"
#include "source_online_loading_menu_v135.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "source_input_manager_v60.hpp"
#include "audio_application_manager_v42.hpp"
#include "player_manager_offline_selectors_v70.hpp"
#include "native_source_main_menu_v114.hpp"
#include "source_main_menu_process_v114.hpp"
#include "application_save_files_owner_v61.hpp"
#include "source_settings_update_job_v102.hpp"
#include "character_design_services.hpp"
#include "script_constants.hpp"
#include "swf_texture.hpp"
#include "swf_input_history.hpp"
#include "swf_frame_connection.hpp"
#include "swf_source_movie_v1.hpp"
#include "player_status_hud.hpp"
#include "textures.hpp"
#include "original_menu_sound_data.hpp"
#include "swf_menu_sound.hpp"
#include "swf_menu_navigation.hpp"
#include "swf_menu_options.hpp"
#include "settings_native_files_v1.hpp"
#include "settings_language_scene_v1.hpp"
#include "menu_native_event_v1.hpp"
#include "menu_frame_clock.hpp"
#include "swf_menu_save_slots.hpp"
#include "menu_save_slot_projection_v1.hpp"
#include "campaign_profile_files_v1.hpp"
#include "fresh_player_profile_v1.hpp"
#include "quest_persistence_v51.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include "localization_parse_ex_v1.hpp"
#include "swf_menu_parsed_string_v1.hpp"
#include "model_renderer.hpp"
#include <android/log.h>
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <limits>
#include <map>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <algorithm>
#include <deque>
#include <cmath>

namespace dh2::android_ui {
namespace {
constexpr const char* tag="DH2Native";
constexpr const char* panel="_root.menu_HUD_0.HUDelements.HealthBars.player";
constexpr const char* status_panel="_root.menu_HUD_0.HUDelements.HealthBars";
constexpr const char* hud_sha="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
struct FrontStageV87 {int x{},y{},width{},height{};};
constexpr FrontStageV87 full_surface_stage_v87(int width,int height){return {0,0,width,height};}
constexpr bool front_stage_is_full_surface_v87(FrontStageV87 stage,int width,int height){
    return stage.x==0&&stage.y==0&&stage.width==width&&stage.height==height;
}
// MenuFlash2DCamera::Update projects the authored 480x320 root frame through
// the complete native surface. These compile-time fixtures guard wide,
// landscape, and portrait sizes against reintroducing a centered 3:2 page.
static_assert(front_stage_is_full_surface_v87(full_surface_stage_v87(2400,1080),2400,1080));
static_assert(front_stage_is_full_surface_v87(full_surface_stage_v87(1920,1080),1920,1080));
static_assert(front_stage_is_full_surface_v87(full_surface_stage_v87(1080,1920),1080,1920));
struct AssetClose {void operator()(AAsset* value)const{if(value)AAsset_close(value);}};
struct ConstantsDelete {void operator()(dh2_script_constants* value)const{dh2_script_constants_destroy(value);}};
struct DebugDelete {void operator()(character::DebugSwitches* value)const{dh2_character_debug_destroy(value);}};
}
struct FrontUiSessionV87::Impl : std::enable_shared_from_this<Impl> {
    ui::GSFlashMenuFieldsV114 main_menu_state_v114;
    ui::GSFlashMenuServicesV114 main_menu_services_v114;
    ui::GSFlashMenuEntryV114 main_menu_entry_v114;
    bool main_menu_state_active_v114_{}; // adapter receipt, no source byte claim
    bool main_menu_request_busy_v114{};std::string main_menu_request_failure_v114;
    bool source_metadata_ready_v114{};std::string main_source_load_failure_v114;
    AAssetManager* manager{};
    OriginalUiAssets assets;
    SwfGpu gpu;
    std::string directory,font_failure,provider_failure;
    std::unique_ptr<dh2_script_constants,ConstantsDelete> constants{dh2_script_constants_create()};
    std::unique_ptr<character::DebugSwitches,DebugDelete> debug{dh2_character_debug_create()};
    character::DebugFileServices24 debug_files{this,debug_open,debug_close};
    ui::Localization localization;
    //Diagnostic-only cache when inspecting this session independently.
    //Native startup binds OriginalUiSession's sole process StringManager.
    ui::HudTextV1* process_text_v101{};
    std::shared_ptr<void> process_text_owner_v101;
    ui::GameOptionTableV1 option_table;
    std::shared_ptr<ui::OwnedHudSettingsV1> settings;
    std::weak_ptr<application::ApplicationServicesOwnerV5> process_application_v114;
    bool reread_profile_v114(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
        std::int32_t,std::int32_t,std::shared_ptr<const FrontSelectedProfileV50>&,std::string&);
    bool project_profile_v114(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
        std::int32_t,std::int32_t,data::CampaignProfileFileV1,
        std::shared_ptr<const FrontSelectedProfileV50>&,std::string&);
    std::unique_ptr<ui::SettingsNativeFilesV1> settings_files;
    bool process_settings_v105{},process_option_table_v105{};
    ui::SwfTexture process_splash_v119;
    std::string process_splash_uri_v119;
    // Survives GSInit's field release: the menu exports borrow the SAME
    // selected language/device image through the session texture cache.
    std::string splash_source_uri_v1;
    std::uintptr_t splash_texture_identity_v1{};
    bool rendering_splash_clip_v1{};
    std::vector<std::string> process_property_names_v119;
    std::weak_ptr<void> campaign_language_world_v109;
    std::function<bool(std::string&)> campaign_language_scene_v109;
    std::function<bool(std::uint32_t&,std::string&)> process_platform_language_v105;
    std::int32_t language_selection=-1;
    dh2::data::CharacterTable menu_characters;
    dh2::data::LevelTables menu_levels;
    std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> quest_tables_v59;
    // Front-only selected difficulty; gameplay ownership is a separate handoff.
    std::int32_t menu_selected_difficulty=0;
    FrontCampaignQuestServicesV87 campaign_quests;
    ui::MenuAvatarPreviewStateV1 menu_avatar;
    ui::LoadingHintTableV1 loading_hints;
    ui::LoadingMenuStateServicesV1 loading_state_services;
    ui::LoadingMenuMultiplayerServicesV1 loading_multiplayer_services;
    ui::LoadingHintRandomV1 loading_hint_random; // isolated front RNG; canonical Game RNG binding pending
    ui::MenuAvatarPreviewServicesV1 menu_avatar_services;
    std::shared_ptr<void> menu_avatar_services_owner_v121;
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
    // Initial owned navigation slice. Other registered native menu types,
    // shared-renderer settings and transition animations remain pending.
    std::vector<std::string> menu_stack;
    std::string active_menu_path()const{return "_root."+(menu_stack.empty()?std::string("menu_MainMenu"):menu_stack.back());}
    static bool shared_state(const std::string& name){return name=="menu_HelpButtons"||name=="menu_Help"||name=="menu_About"||name=="menu_Options"||name=="menu_Loading";}
    ui::SwfMovie* menu_movie(const std::string& name)const{return shared_state(name)?shared_menu_movie.get():movie.get();}
    ui::SwfMovie* active_menu_movie()const{return menu_stack.empty()?movie.get():menu_movie(menu_stack.back());}
    ui::SwfMovie* input_dispatch_movie{};
    std::int32_t last_menu_dt{};
    int class_index=0;
    int class_applied_index=-1;
    bool process_class_select_active_v87=false;
    std::uintptr_t class_left=0,class_right=0;
    std::chrono::steady_clock::time_point frame_time{};
    MenuFrameClock menu_clock;
    int driver_width=480,driver_height=320;
    ui::SourceMenuVariantV132 process_variant_v132{};
    bool source_movies_v132{};
    std::shared_ptr<ui::MenuFlash2DCameraOwnerV93> camera_owner_v93,shared_camera_owner_v93;
    bool main_deleted_v93=false,base_deleted_v93=false;
    std::shared_ptr<void> source_menu_owner_v93;
    std::function<bool(std::vector<FrontMovieDrawV93>&,std::string&)> source_draw_v93;
    std::function<bool(const char*,const gameswf::fn_call&,std::string&)> source_navigation_v93;
    bool source_transport_busy_v93=false;
    bool source_loading_render_v114=false;
    bool camera_update_v93(ui::SwfMovie& target,const std::shared_ptr<ui::MenuFlash2DCameraOwnerV93>& owner,std::string& error){
        if(!owner||owner->movie!=reinterpret_cast<std::uintptr_t>(&target)){error="Required same actual paired front camera";return false;}
        ui::FlashCamera40* state{};std::shared_ptr<void> pin;
        return owner->borrow_state(state,pin,error)&&target.update_viewport(*state,error);
    }
    std::array<std::int32_t,5> reported_frames{{-1,-1,-1,-1,-1}};
    unsigned glyph_uploads=0,bitmap_uploads=0,string_calls=0,core_errors=0,packed_glyphs=0;
    unsigned strips=0,lines=0,masks=0;
    bool loading_bitmap_reported=false;
    std::deque<std::string> menu_sounds;
    std::deque<std::string> menu_audio;
    std::deque<std::string> menu_browser;
    std::deque<int> menu_catalog;
    bool exit_confirmation_open=false;
    bool exit_requested=false;
    int last_width=0,last_height=0;
    // Reverse destruction keeps every provider and owned texture alive until
    // the last movie and its reachable ActionScript graph have been released.
    struct FrameOwner {
        std::shared_ptr<ui::SwfInputHistory> history;
        ui::SwfFrameConnection* frames{};
        ui::MenuNativeEventV1 main_events;
        std::uint32_t input_selection=0;
    };
    std::shared_ptr<FrameOwner> frame_owner;
    std::shared_ptr<FrameOwner> shared_frame_owner;
    std::unique_ptr<ui::SwfTextFontPlatformV1> fonts,shared_fonts;
    std::function<bool(std::string&)> process_font_cache_reset_v119;
    std::shared_ptr<ui::SwfMovie> movie;
    std::shared_ptr<ui::SwfMovie> shared_menu_movie;
    std::unique_ptr<ui::PlayerStatusHud> status;
    std::array<std::int32_t,4> front_rectangle()const{
        // MenuFlash2DCamera::Update sets the SWF viewport to the full surface.
        // Keep hit testing on that same surface; the scene's separate 3:2 fit
        // must not clip the authored, full-viewport menu controls.
        return {0,0,driver_width,driver_height};
    }
    static bool input_accepts(void*,ui::SwfEvent48&,bool& accepted,std::string&){
        // MenuBase::CanHandleEvent, 0x41f3fc, returns true.
        accepted=true;return true;
    }
    static bool input_advance(void* context,gameswf::root* root,float seconds,bool flag,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return self.frame_owner->frames->advance(root,seconds,flag,error);
    }
    static bool shared_input_advance(void* context,gameswf::root* root,float seconds,bool flag,std::string& error){
        return static_cast<Impl*>(context)->shared_frame_owner->frames->advance(root,seconds,flag,error);
    }
    static bool raw_event_position(void* context,int& x,int& y,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.input_dispatch_movie){error="Native menu event outside retained input delivery";return false;}
        return self.input_dispatch_movie->input_raw_position(x,y,error);
    }
    static bool menu_browser_request(void* context,const char* url,std::string& error){
        if(!url){error="Missing nativeOpenBrowser URL";return false;}
        auto& self=*static_cast<Impl*>(context);
        // Original nativeOpenBrowser hands the URL to Android's ACTION_VIEW.
        // Keep platform Activity creation outside retained SWF dispatch.
        self.menu_browser.emplace_back(url);
        __android_log_print(ANDROID_LOG_INFO,tag,"Original menu browser queued | %s",url);
        return true;
    }
    static bool menu_platform_language(void* context,int& language,std::string&){
        auto& self=*static_cast<Impl*>(context);language=self.settings?self.settings->language():0;return true;
    }
    static bool menu_online_request(void* context,bool live,int language,std::string& error){
        if(live){error="Required Gameloft Live account Activity owner unavailable";return false;}
        auto& self=*static_cast<Impl*>(context);self.menu_catalog.push_back(language<0?0:language);
        __android_log_print(ANDROID_LOG_INFO,tag,"Original More Games queued | language %d",language);return true;
    }
    static bool input_native_event(void* context,ui::SwfEvent48& event,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        ui::MenuNativeEventServicesV1 services;services.context=context;services.raw_position=raw_event_position;services.browser=menu_browser_request;services.language=menu_platform_language;services.online=menu_online_request;
        const bool class_select_input=self.process_class_select_active_v87||
            (!self.menu_stack.empty()&&self.menu_stack.back()=="menu_SelectClass");
        if(class_select_input&&event.kind==2){
            const int before=self.class_index;
            // MenuCharacterSelect::OnEvent 0x4282c8..0x42839c: compare
            // actual cached characters, clamp the native index to 0..2.
            // Source OnEvent gates selector actions with field fc; Update clears
            // it during a camera transition and restores it after IsAnimOver.
            if(model_renderer::class_scene_input_enabled()){
                if(event.character==self.class_right&&self.class_index<2)++self.class_index;
                else if(event.character==self.class_left&&self.class_index>0)--self.class_index;
            }
            if(before!=self.class_index){
                // Start immediately in this input batch, so a second queued tap
                // cannot skip from class 0 to 2 before the next frame update.
                if(!model_renderer::select_class_scene(self.class_index,0,error)||
                   !self.movie->menu_action_script(&self,update_class,error))return false;
            }
        }
        const bool shared=self.input_dispatch_movie==self.shared_menu_movie.get();
        auto actual=shared?self.shared_frame_owner:self.frame_owner;
        if(!actual){error="Required same live front native event owner";return false;}
        const bool delivered=!shared&&(self.menu_stack.empty()||self.menu_stack.back()=="menu_MainMenu")
            ?actual->main_events.main(event,services,error)
            :actual->main_events.base(event,services,error);
        __android_log_print(delivered?ANDROID_LOG_INFO:ANDROID_LOG_WARN,tag,
            "Original main native event stage | kind %u | name %s | delivered %d | consumed %u",event.kind,event.name?event.name:"<null>",delivered,event.consumed);
        return delivered;
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
        if(movie&&!main_deleted_v93&&!camera_update_v93(*movie,camera_owner_v93,error))return false;
        if(shared_menu_movie&&!base_deleted_v93&&!camera_update_v93(*shared_menu_movie,shared_camera_owner_v93,error))return false;
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
    static int debug_open(void* context,const char* name,std::uintptr_t* handle) {
        auto& self=*static_cast<Impl*>(context);
        if(!name||std::strcmp(name,"DebugSwitches.savegame")||!handle||self.directory.empty())return 1;
        *handle=0;errno=0;auto* file=std::fopen((self.directory+"/"+name).c_str(),"rb");
        if(!file)return errno==ENOENT?0:1;
        *handle=reinterpret_cast<std::uintptr_t>(file);return 0;
    }
    static int debug_close(void*,std::uintptr_t handle) {
        return !handle||std::fclose(reinterpret_cast<std::FILE*>(handle))?1:0;
    }
    bool debug_load(std::string& error) {
        const int status=character::load_debug_stdio_v136(debug.get(),&debug_files,directory.c_str());
        if(status<0){error="Required UI DebugSwitches load failed: "+std::to_string(status);return false;}return true;
    }
    bool debug_query(const char* key,std::string& error) {
        std::uint32_t ignored=0;const int status=dh2_character_debug_get(&ignored,debug.get(),key,&debug_files);
        if(status<0){error="Required UI DebugSwitches query failed: "+std::to_string(status);return false;}return true;
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
        auto& self=*static_cast<Impl*>(context);std::int32_t word=0;
        if(dh2_script_constants_get(self.constants.get(),group,key,&word)!=0){error=std::string("Required UI constant missing: ")+group+"."+key;return false;}
        value=static_cast<std::uint32_t>(word);return true;
    }
    static bool no_player(void*,std::uintptr_t& out,std::string&) {
        // Standalone authored-panel inspection has no selected world/player.
        // This executes the source null-character branch, never a made-up name.
        out=0;return true;
    }
    static bool unavailable_player_name(void*,std::uintptr_t,std::string&,std::string& error) {
        error="Player profile owner unavailable in standalone UI inspection";return false;
    }
    ui::LocalizationServices text_services() {
        ui::LocalizationServices services{this,text_open,text_close,localization_debug,constant,no_player,unavailable_player_name};
        if(front_screen=="main"){
            services.application_language=application_language;
            services.application_version=application_version;
        }
        return services;
    }
    bool text_native_v101(const std::string& symbol,ui::LocalizationResult& out,std::string& error){
        return process_text_v101?process_text_v101->native_string(symbol,text_services(),out,error):localization.native_string(symbol,text_services(),out,error);
    }
    bool text_id_v101(std::uint32_t id,std::string& out,std::string& error){
        if(!process_text_v101)return localization.string_id(id,text_services(),out,error);
        bool is_null{};return process_text_v101->integer_string(static_cast<std::int32_t>(id),text_services(),out,is_null,error);
    }
    bool text_parsed_v101(const std::string& symbol,const std::vector<ui::LocalizationArgumentV1>& args,std::string& out,std::string& error){
        if(!process_text_v101)return localization.parsed_string(symbol,args,text_services(),out,error);
        ui::HudTextEnvironmentV1 environment;environment.localization=text_services();
        return process_text_v101->parsed_string_v4(symbol,args,environment,out,error);
    }
    static bool application_language(void* context,std::int32_t& value,std::string&){
        auto& self=*static_cast<Impl*>(context);value=self.settings?self.settings->language():(self.process_text_v101?self.process_text_v101->pack():self.localization.pack());return true;
    }
    static bool application_version(void*,std::string& value,std::string&){
        // Application::GetVersionString 0x31f6e0..0x31f708, ordinary Android
        // operator package (not special package 4), include-version=true.
        value="1.0.2";return true;
    }
    static bool refresh_settings_scene(void* context,ui::OwnedHudSettingsV1&,std::int32_t,std::string& error){
        auto& self=*static_cast<Impl*>(context);
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
    ui::SettingsLanguageServicesV1 settings_language(){
        ui::SettingsLanguageServicesV1 out{this,refresh_settings_scene,nullptr,&localization};
        if(!campaign_language_world_v109.expired()||campaign_language_scene_v109)out.refresh_scene=[](void* raw,ui::OwnedHudSettingsV1&,std::int32_t,std::string& e){auto& self=*static_cast<Impl*>(raw);
            if(self.campaign_language_world_v109.expired()){e="Retired actual campaign localization scene";return false;}
            if(!self.campaign_language_scene_v109){e="Required actual campaign localization scene traversal";return false;}
            return self.campaign_language_scene_v109(e);
        };
        if(process_platform_language_v105)out.platform_language=[](void* context,std::uint32_t& value,std::string& error){
            return static_cast<Impl*>(context)->process_platform_language_v105(value,error);
        };
        if(process_text_v101){out.text=nullptr;out.text_context=this;
            out.switch_text_pack_v4=[](void* context,std::int32_t pack,bool unload,std::string& error){
                auto& self=*static_cast<Impl*>(context);return self.process_text_v101->switch_pack(pack,unload,error);
            };}
        return out;
    }
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
    static bool save_settings(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        auto app=self.process_application_v114.lock();
        if(!app||!self.settings||self.directory.empty()||app->source_settings4c_v67()!=self.settings){
            error="Front settings save requires the SAME process Application/SavegameManager";return false;
        }
        // IDA NativeSaveSettings (43b278) -> SavegameManager::saveSettings
        // (46cb34) uses the Savegame+4/byte37 gate and Application FileManager.
        // Its getLanguage/setLanguage tail (46d514/46d104) runs after save,
        // including when saveSettings returns early because the gate is false.
        if(!model_renderer::save_process_settings_v102(app,error))return false;
        // NativeSaveSettings calls saveSettings, then getLanguage/setLanguage
        // on the SAME SavegameManager. The owner already supplies the exact
        // language write, object traversal and TextManager switch prefix.
        auto language=self.settings_language();
        if(!self.settings->set_language(self.settings->language(),language,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Front settings save completed | music %d | fx %d | language %d",self.settings->option("VolumeMusic"),self.settings->option("VolumeFX"),self.settings->language());return true;
    }
    static bool option_string(void* context,std::int32_t id,std::string& text,std::string& error){auto& self=*static_cast<Impl*>(context);return self.text_id_v101(std::uint32_t(id),text,error);}
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
        if(!std::strcmp(name,"VolumeMusic")||!std::strcmp(name,"VolumeFX")){
            // NativeSetOptions immediately forwards the saved integer to the
            // same process VoxSoundManager. An absent global is the original
            // no-op; a live manager must receive the real group setter.
            dh2::audio::AudioApplicationBorrowV42 audio;
            if(!model_renderer::borrow_actual_application_audio_v42(audio,error))return false;
            if(audio.manager){
                const auto selector=!std::strcmp(name,"VolumeFX")?1:2;
                if(!audio.manager->set_source_volume_v68(selector,
                    static_cast<float>(self.settings->saved_option(name)),error))return false;
            }
            self.queue_volumes();
        }
        __android_log_print(ANDROID_LOG_INFO,tag,"Front option changed | name %s | value %d",name,self.settings->option(name));return true;
    }
    static bool reset_option_fonts(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.process_font_cache_reset_v119){error="Required SAME MultiMenuManager renderer directory for ResetFonts";return false;}
        return self.process_font_cache_reset_v119(error);
    }
    static bool enter_options(void* context,std::string&){static_cast<Impl*>(context)->menu_audio.emplace_back("resume");return true;}
    static bool refresh_front_hud(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);if(self.live_player){error="Settings HUD refresh requires attached player owner";return false;}return true;
    }
    static bool input_behavior(void* context,std::int32_t slot,bool rollover,std::string& error){
        auto& self=*static_cast<Impl*>(context);auto* target=slot==0?self.shared_menu_movie.get():slot==1?self.movie.get():nullptr;
        if(!target){error="Menu input renderer slot unconnected";return false;}return target->menu_input_behavior(rollover?0x84:4,error);
    }
    ui::SwfMenuOptionServicesV1 option_services(){
        ui::SwfMenuOptionServicesV1 s;s.settings=settings.get();s.context=this;s.string_by_id=option_string;s.apply_option=apply_option;s.reset_fonts=reset_option_fonts;
        s.load=load_settings;s.save=save_settings;s.enter=enter_options;s.refresh_hud=refresh_front_hud;s.input_behavior=input_behavior;return s;
    }
    static bool legacy_volume_option(void* context,const char* name,const gameswf::fn_call& fn,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.settings||fn.nargs<1||!fn.env){error="Malformed legacy volume option call";return false;}
        // NativeOptionFX/Music 0x43b390/0x43b424 convert the slider number to
        // float, call the live VoxSoundManager first, then store int(float) on
        // the SAME SavegameManager. With no sound manager the source is a no-op.
        const float volume=static_cast<float>(fn.arg(0).to_number());
        if(!std::isfinite(volume)||volume<float(std::numeric_limits<std::int32_t>::min())||
           volume>=2147483648.0f){error="Legacy volume option outside source integer range";return false;}
        dh2::audio::AudioApplicationBorrowV42 audio;
        if(!model_renderer::borrow_actual_application_audio_v42(audio,error))return false;
        if(audio.manager){
            const auto selector=!std::strcmp(name,"NativeOptionFX")?1:2;
            if(!audio.manager->set_source_volume_v68(selector,volume,error))return false;
            const auto value=static_cast<std::int32_t>(volume);
            self.settings->set_option(selector==1?"VolumeFX":"VolumeMusic",value);
            self.queue_volumes();
        }
        error.clear();return true;
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
#include "original_ui_gfnt_backend_v1.inc"
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
        if(path=="data/menus/loadanims_droid.swf") {
            if(!self.raw_asset("front-compat/loadanims_droid.swf",out,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Loading ring atlas compatibility movie connected | original timeline retained");
        }
        return true;
    }
    static bool texture(void* context,const char* name,int,int,ui::SwfTexture& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);std::string uri;
        if(!scene::swf_texture_filename("",std::string("data/")+name,uri,error))return false;
        const bool splash=ui::is_splash_source_uri_v1(uri);
        if(splash&&!self.splash_source_uri_v1.empty())uri=self.splash_source_uri_v1;
        const auto found=self.exports.find(uri);if(found!=self.exports.end()){
            out=found->second;if(splash)self.splash_texture_identity_v1=out.identity;return true;
        }
        std::vector<std::uint8_t> encoded;if(!self.assets.read(uri,encoded,error))return false;
        textures::View view{};auto status=dh2_texture_open(encoded.data(),encoded.size(),&view);
        if(status!=textures::Error::ok){error=dh2_texture_error(status);return false;}
        resources::RetainedBytesV39 rgba;std::uint64_t rgba_bytes;
        if(!resources::swf_bitmap_bytes_v39(view.width,view.height,4,rgba_bytes,error)||
           !rgba.allocate(self.gpu.resource_budget_lease_v39(),resources::ResourceScopeV37::swf_front,static_cast<std::size_t>(rgba_bytes),error))return false;
        status=dh2_texture_decode(&view,rgba.data(),rgba.size());
        if(status!=textures::Error::ok){error=dh2_texture_error(status);return false;}
        if(!self.gpu.image(view.width,view.height,4,rgba.data(),std::size_t(view.width)*4,out,error))return false;
        if(splash)self.splash_texture_identity_v1=out.identity;
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
        float splash_uv[6];
        if(self.rendering_splash_clip_v1&&command.fill.kind==ui::SwfFill::bitmap&&
           command.fill.texture.identity==self.splash_texture_identity_v1&&
           ui::splash_android_background_uv_v1(command.fill.uv.value,splash_uv)){
            auto corrected=command;
            std::copy_n(splash_uv,6,corrected.fill.uv.value);
            return self.gpu.draw(corrected,error);
        }
        return self.gpu.draw(command,error);
    }
    static bool stencil(void* context,const float bounds[4],std::uint8_t pattern,bool& out,std::string& error) {
        return static_cast<Impl*>(context)->gpu.stencil(bounds,pattern,out,error);
    }
    static bool native(void* context,const char* name,const std::vector<ui::SwfValue>& args,ui::SwfValue& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(std::strcmp(name,"NativeGetStringFromSymbol")||args.size()!=1||args[0].kind!=ui::SwfValue::text){error="Required native UI function/arguments unsupported";return false;}
        ui::LocalizationResult result;
        if(!self.text_native_v101(args[0].string,result,error))return false;
        if(result.sets_menu_string_flag)self.menu_string_flag=true;
        out.kind=ui::SwfValue::text;out.string=result.text;++self.string_calls;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original UI string delivered | symbol %s | found %d | text %s",args[0].string.c_str(),result.found,result.text.c_str());return true;
    }
    static void diagnostic(void* context,bool error,const char* text) {
        auto& self=*static_cast<Impl*>(context);if(error)++self.core_errors;
        __android_log_print(error?ANDROID_LOG_WARN:ANDROID_LOG_INFO,tag,"Original SWF core diagnostic: %s",text?text:"");
    }
    ui::MenuDeviceFactsV1 menu_device;
    bool menu_device_written_v4{};
    bool menu_persona_mode=false;
    // This front has no canonical campaign-to-gameplay loader. Capture the
    // authored launch attempt without assigning a Player or saving a campaign.
    bool start_feedback_pending=false;
    bool launch_pending=false;
    std::shared_ptr<const FrontSelectedProfileV50> selected_launch_profile_v50;
    std::int32_t start_requested_difficulty=0;
    static bool show_start_pending(void* context,ui::SwfAsGraph& graph,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        ui::SwfAsValue root,popup,field,result,value;bool accepted=false,callable=false,found=false;
        if(!graph.root_value(root,error))return false;
        for(const char* key:{"SlotID","ThePlayerSlot"}){
            if(!graph.get_member(root,key,value,found,error))return false;
            double number=0;if(found&&!graph.to_number(value,number,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Pending front launch | key %s | value %.0f | difficulty %d | canonical gameplay loader unavailable",key,number,self.start_requested_difficulty);
        }
        if(!graph.find_target(root,"menu_StartGame.Confirmation2",popup,error)||!popup.identity()||
           !graph.find_target(root,"menu_StartGame.Confirmation2.ConfirmationBox.text",field,error)||!field.identity()){
            error="Authored Start Game confirmation absent";return false;
        }
        if(!graph.set_member(field,"text",ui::SwfAsValue::text("Gameplay loading is still being connected."),accepted,error))return false;
        if(!graph.find_target(root,"menu_StartGame.Confirmation2.ConfirmationBox.btn_Ok.text",field,error)||!field.identity()){
            error="Authored Start Game confirmation button absent";return false;
        }
        ui::LocalizationResult localized;
        if(!self.text_native_v101("GLOBAL_OK",localized,error)||
           !graph.set_member(field,"text",ui::SwfAsValue::text(localized.text.c_str()),accepted,error)||
           !graph.invoke(popup,popup,"gotoAndPlay",{ui::SwfAsValue::text("Show")},result,callable,error))return false;
        if(!callable){error="Authored Start Game confirmation timeline absent";return false;}
        return true;
    }
    static bool persona_destroy(void* context,std::string& error){auto& self=*static_cast<Impl*>(context);return model_renderer::select_menu_persona(-1,self.manager,error);}
    static bool persona_camera(void*,std::string& error){error.clear();return true;}
    static bool persona_setup(void* context,std::int32_t slot,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(slot<0){
            __android_log_print(ANDROID_LOG_INFO,tag,"Menu profile selection | requested_slot %d | occupied 0 | profile_class <none> | legacy_persona -1",slot);
            return model_renderer::select_menu_persona(-1,self.manager,error);
        }
        bool occupied=false;
        if(!menu_slot_exists(context,std::uint32_t(slot),occupied,error))return false;
        if(!occupied){
            __android_log_print(ANDROID_LOG_INFO,tag,"Menu profile selection | requested_slot %d | occupied 0 | profile_class <none> | legacy_persona -1",slot);
            return model_renderer::select_menu_persona(-1,self.manager,error);
        }
        if(!self.menu_persona_mode){error="Occupied avatar requires canonical owner or explicit menu persona mode";return false;}
        dh2::data::CampaignProfileFileV1 file;
        if(!dh2::data::read_campaign_profile_v1(self.directory,slot,file,error))return false;
        dh2::data::MenuProfileMetadataV1 metadata;
        if(!dh2::data::load_menu_profile_metadata_v1({file.bytes.data(),file.bytes.size()},self.menu_characters,slot,self.menu_selected_difficulty,{&self,menu_store_difficulty,menu_load_quest_acts_v59},metadata,error))return false;
        const char* names[]={"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"};
        for(int i=0;i<3;++i){const auto found=std::find(self.menu_characters.names.begin(),self.menu_characters.names.end(),names[i]);if(found!=self.menu_characters.names.end()&&std::int32_t(found-self.menu_characters.names.begin())==metadata.character_row){
            __android_log_print(ANDROID_LOG_INFO,tag,"Menu profile selection | requested_slot %d | occupied 1 | player %s | PCLS %d:%s | legacy_persona %d",slot,metadata.name.c_str(),metadata.character_row,names[i],i);
            return model_renderer::select_menu_persona(i,self.manager,error);
        }}
        error="Menu persona requires a supported base class";return false;
    }
    void enable_menu_persona(){
        if(!install_front_inspection_avatar_services_v1(menu_avatar_services,menu_avatar_services_owner_v121,
            {this,persona_destroy,persona_setup,persona_camera}))return;
        menu_persona_mode=true;
        campaign_quests={nullptr,[](void*,bool& value,std::string& error){value=false;error.clear();return true;}};
    }
    bool create_menu_persona(const char* player_name,const char* character,std::uint32_t& slot,std::string& error){
        slot=4;
        for(std::uint32_t i=0;i<4;++i){bool used=false;if(!menu_slot_exists(this,i,used,error))return false;if(!used){slot=i;break;}}
        if(slot==4){error="No free campaign slot for menu persona";return false;}
        dh2::data::FreshPlayerProfileV1 profile;
        const auto timer=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        if(!dh2::data::fresh_player_profile_v1(menu_characters,character,player_name,std::uint32_t(timer),std::uint32_t(std::time(nullptr)),profile,error))return false;
        char name[32];std::snprintf(name,sizeof(name),"/dh2_%03u.savegame",slot);
        const auto path=directory+name;
        const int fd=::open(path.c_str(),O_WRONLY|O_CREAT|O_EXCL|O_NOFOLLOW,0600);
        if(fd<0){error="Cannot exclusively create persona save";return false;}
        std::size_t offset=0;bool ok=true;
        while(offset<profile.bytes.size()){const auto n=::write(fd,profile.bytes.data()+offset,profile.bytes.size()-offset);if(n<=0){ok=false;break;}offset+=std::size_t(n);}
        if(::fsync(fd))ok=false;::close(fd);
        if(!ok){::unlink(path.c_str());error="Persona save delivery failed";return false;}
        const auto marker=directory+"/menu-persona-preview.enabled";
        const int marker_fd=::open(marker.c_str(),O_WRONLY|O_CREAT|O_EXCL|O_NOFOLLOW,0600);if(marker_fd>=0)::close(marker_fd);
        enable_menu_persona();
        __android_log_print(ANDROID_LOG_INFO,tag,"Menu-only persona created | name %s | class %s | slot %u | bytes %zu | gameplay initialization pending",player_name,character,slot,profile.bytes.size());
        return ui::change_menu_avatar_preview_v1(menu_avatar,std::int32_t(slot),true,menu_avatar_services,error,true);
    }
    static bool menu_slot_exists(void* context,std::uint32_t slot,bool& occupied,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return dh2::data::campaign_profile_exists_v1(self.directory,slot,occupied,error);
    }
    static bool menu_flush_front_jobs(void* context,std::string& error){
        const auto& self=*static_cast<Impl*>(context);
        // This retained front owns only synchronous campaign file operations.
        // Once gameplay is attached its canonical job owner must replace this.
        if(self.front_screen!="main"||self.live_player){error="Campaign erase requires canonical gameplay save-job flush";return false;}
        error.clear();return true;
    }
    static bool menu_erase_slot_files(void* context,std::uint32_t slot,std::string& error){
        auto& self=*static_cast<Impl*>(context);std::uint32_t deleted{};
        const bool ok=dh2::data::erase_campaign_slot_files_v1(self.directory,slot,deleted,error);
        __android_log_print(ok?ANDROID_LOG_INFO:ANDROID_LOG_WARN,tag,"Original campaign erase | slot %u | deleted %u | success %d",slot,deleted,ok);
        return ok;
    }
    static bool menu_store_difficulty(void* context,std::int32_t value,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.settings){error="Required actual SavegameManager before menu difficulty delivery";return false;}
        self.settings->source_set_current_difficulty_v67(value);
        self.menu_selected_difficulty=value;error.clear();return true;
    }
    static bool menu_load_quest_acts_v59(void* context,dh2::data::Bytes bytes,
        std::array<std::int32_t,3>& regular,std::array<std::int32_t,3>& volatile_acts,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.quest_tables_v59){
            std::vector<std::uint8_t> array,names;bool found{};
            if(!self.original("data/pydata/v2quests_pyarray.bin",found,array,error))return false;
            if(!found){error="Original Quest PyArray missing for selected QEST profile";return false;}
            if(!self.original("data/pydata/v2quests_pyarraynames.bin",found,names,error))return false;
            if(!found){error="Original Quest names missing for selected QEST profile";return false;}
            auto tables=std::make_shared<dh2::data::QuestTablesPersistenceV51>();
            if(!tables->decode({array.data(),array.size()},{names.data(),names.size()},error))return false;
            self.quest_tables_v59=std::move(tables);
        }
        return dh2::data::load_quest_metadata_acts_v51(self.quest_tables_v59,bytes,regular,volatile_acts,error);
    }
    static bool menu_use_volatile_quest_acts(void* context,bool& value,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        const auto app=self.process_application_v114.lock();
        if(!app){error="Required actual process Application for Game::IsOnlineGame";return false;}
        std::shared_ptr<dh2::application::SourceOnlineLoadingOwnerV55> online;
        return dh2::application::source_menu_online_v114(*app,online,value,error);
    }
    static bool menu_slot_string(void* context,std::uint32_t id,std::string& text,std::string& error){
        auto& self=*static_cast<Impl*>(context);return self.text_id_v101(id,text,error);
    }
    static bool menu_slot_date(void*,std::uint32_t raw,std::tm& calendar,std::string& error){return ui::menu_save_slot_local_date_v1(raw,calendar,error);}
    static bool menu_slot_constant(void* context,const char* group,const char* key,std::int32_t& value,std::string& error){
        std::uint32_t raw{};if(!constant(context,group,key,raw,error))return false;
        std::memcpy(&value,&raw,sizeof(value));return true;
    }
    static bool menu_slot_details(void* context,std::uint32_t slot,bool occupied,std::int32_t difficulty,
        ui::SwfFrontSaveSlotDetailsV1& details,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!occupied){details={};error.clear();return true;}
        dh2::data::CampaignProfileFileV1 file;
        if(!dh2::data::read_campaign_profile_v1(self.directory,slot,file,error))return false;
        dh2::data::MenuProfileMetadataV1 metadata;
        // Temporary source metadata receiver; gameplay Save/Quest identity is
        // published separately at the actual campaign/player boundary.
        dh2::data::MenuProfileMetadataServicesV1 load_services{&self,menu_store_difficulty,menu_load_quest_acts_v59};
        if(!dh2::data::load_menu_profile_metadata_v1({file.bytes.data(),file.bytes.size()},self.menu_characters,
            static_cast<std::int32_t>(slot),self.menu_selected_difficulty,load_services,metadata,error))return false;
        ui::MenuSaveSlotPresentationServicesV1 display_services{&self,menu_slot_constant,menu_slot_string,menu_slot_date};
        bool volatile_acts=false;
        if(!self.campaign_quests.use_volatile_quest_acts){error="Canonical Game/Online quest-selection service unavailable";return false;}
        if(!self.campaign_quests.use_volatile_quest_acts(self.campaign_quests.context,volatile_acts,error))return false;
        const auto language=self.settings?self.settings->language():0;
        if(!ui::project_menu_save_slot_v1(metadata,self.menu_characters,self.menu_levels,difficulty,volatile_acts,
            static_cast<std::uint32_t>(language),display_services,details,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original occupied save-slot presentation | slot %u | backup %d | class %s | level %d | location %s",slot,file.origin==dh2::data::CampaignProfileOriginV1::backup,details.player_class.c_str(),details.player_level,details.player_location.c_str());
        return true;
    }
    static bool menu_parsed_string(void* context,const std::string& symbol,const std::vector<ui::LocalizationArgumentV1>& args,std::string& text,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.text_parsed_v101(symbol,args,text,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original parsed menu string | symbol %s | args %zu | text %s",symbol.c_str(),args.size(),text.c_str());return true;
    }
    static bool native_action(void* context,const char* name,const gameswf::fn_call& fn,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        // Process draw/input and authored navigation must address the SAME
        // MenuManager. The legacy front vector does not drive source renders.
        if(self.source_menu_owner_v93&&(!std::strcmp(name,"NativePushMenu")||
           !std::strcmp(name,"NativePopMenu")||!std::strcmp(name,"NativePopAllAbove")||
           !std::strcmp(name,"NativePopAllMenus")))
            return self.source_navigation_v93&&self.source_navigation_v93(name,fn,error);
        if(!std::strcmp(name,"NativeHasPushNotification")){
            // 440c84: ignore args; gate on Device.IsHighPerformance, multiplayer,
            // SHARP or HTC, then OR SAME Application+10e/+ee and GameCenter+15.
            if(!fn.result||!self.menu_device_written_v4){error="Push-notification query requires actual result/device facts";return false;}
            const ui::HudDevicePipeline16 pipeline{{self.menu_device.sharp,self.menu_device.htc,self.menu_device.multiplayer_mode},self.menu_device.driver_type};
            const int performance=dh2_hud_device_pipeline(&pipeline);
            if(performance<0){error="Push-notification graphics capability query failed";return false;}
            bool pending=false;
            if(performance||self.menu_device.multiplayer_mode||self.menu_device.sharp||self.menu_device.htc){
                const auto application=self.process_application_v114.lock();
                const auto game_center=dh2::player::process_player_manager_offline_selectors_v70();
                if(!application||!game_center){error="Push-notification query requires same process Application/GameCenter";return false;}
                pending=application->source_invitation10e()!=0||application->source_invitation_ee()!=0||game_center->game_center_notification15()!=0;
            }
            fn.result->set_bool(pending);error.clear();return true;
        }
        if(!std::strcmp(name,"NativeOnlineSanityCheck")){
            // 43a0fc ignores args/result and clears SAME OnlineGameState+26.
            // Reuse the existing process singleton projection, including its
            // byte28 queried by PlayerManager; COnline is a separate owner.
            const auto online=dh2::player::process_player_manager_offline_selectors_v70();
            if(!online){error="Required process OnlineGameState sanity receiver";return false;}
            online->source_online_sanity_check();
            __android_log_print(ANDROID_LOG_INFO,tag,"Original online sanity callback | same process receiver %zx | byte26 0",reinterpret_cast<std::uintptr_t>(online.get()));
            return true;
        }
        if(!std::strcmp(name,"NativeGoToMainMenu")){
            // 43ae9c ignores args/result and calls Application.GoToMainMenu(0).
            // The native frame drains this accepted command after AS unwinds.
            return model_renderer::request_source_main_menu_v114(0,error);
        }
        if(!std::strcmp(name,"NativePauseMusic")||!std::strcmp(name,"NativeStopMusic")){
            // Original wrappers ignore args, preserve AS result, and no-op
            // without a manager. The retained front has a real audio owner.
            self.menu_audio.emplace_back(!std::strcmp(name,"NativePauseMusic")?"music-pause":"music-stop,500");
            __android_log_print(ANDROID_LOG_INFO,tag,"Original menu music control | action %s",name);return true;
        }
        if(!std::strcmp(name,"NativePlayMusic")){
            // 43ad84: one string, real Sounds lookup, then
            // PlayMusic(id,true,false,2000). Invalid/absent names are no-ops.
            std::string requested;if(!ui::swf_menu_sound_argument(fn,requested))return true;
            for(std::size_t id=0;id<std::size(original_sounds);++id){
                if(requested!=original_sounds[id].name)continue;
                if(requested!="TitleMusic"){error="Required non-title music owner unavailable: "+requested;return false;}
                self.menu_audio.emplace_back("music-title,2000");
                __android_log_print(ANDROID_LOG_INFO,tag,"Original menu music requested | name %s | id %zu | loop 1 | force 0 | fade ms 2000",requested.c_str(),id);return true;
            }
            return true;
        }
        if(!std::strcmp(name,"NativeIsMultiplayerEnabled"))return ui::swf_menu_multiplayer_enabled_v1(fn,self.menu_device,error);
        if(!std::strcmp(name,"NativeIsMultiplayerLoadCompleted"))return ui::swf_loading_multiplayer_completed_v1(fn,self.loading_multiplayer_services,error);
        if(!std::strcmp(name,"NativeIsMultiplayerHost"))return ui::swf_loading_multiplayer_host_v1(fn,self.loading_multiplayer_services,error);
        if(!std::strcmp(name,"NativeMustWaitForHost"))return ui::swf_loading_wait_for_host_v1(fn,self.loading_multiplayer_services,error);
        if(!std::strcmp(name,"NativeGetLoadingProgress"))return ui::swf_loading_progress_v1(fn,self.loading_state_services,error);
        if(!std::strcmp(name,"NativeEndLoading")){
            bool advanced=false;if(!ui::swf_loading_end_v1(fn,self.loading_state_services,advanced,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original loading completion callback | actual Level advanced %d",advanced);return true;
        }
        if(!std::strcmp(name,"NativeGetLoadingTipStrID")){
            std::uint32_t id{};if(!ui::swf_loading_hint_v1(fn,self.loading_hints,self.loading_hint_random,id,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original loading tip selected | string id %u | seed %u | calls %u",id,self.loading_hint_random.seed,self.loading_hint_random.debug_calls);return true;
        }
        if(!std::strcmp(name,"NativeGetStringFromID")){
            return ui::swf_loading_string_id_v1(fn,&self,menu_slot_string,error);
        }
        // Original 439c64 NativeLaunchIGP is an intentional empty callback.
        // MenuMainMenu::OnEvent performs the platform action on release.
        if(!std::strcmp(name,"NativeLaunchIGP"))return true;
        if(!std::strcmp(name,"NativeLaunchTwitter")){
            // Original 43a6a4 only queries SavegameManager::getLanguage.
            // MenuBase::OnEvent owns the actual browser action on release.
            const auto language=self.settings?self.settings->language():0;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original Twitter AS callback | language %d | browser owned by native event",language);
            return true;
        }
        if(!std::strcmp(name,"NativeHUDInteract")){
            // Original 43afb8: "quit" sets the dialog flag, "no" clears
            // it, all other strings enter appDestroy -> Application::Quit.
            // Retain the movie during authored dispatch; consume only after
            // the frame returns, when Android can close its Activity safely.
            std::string action;if(!ui::swf_menu_sound_argument(fn,action))return true;
            if(action=="quit")self.exit_confirmation_open=true;
            else if(action=="no")self.exit_confirmation_open=false;
            else self.exit_requested=true;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original front exit action | %s | confirmation %d | requested %d",action.c_str(),self.exit_confirmation_open,self.exit_requested);
            return true;
        }
        if(!std::strcmp(name,"NativeStartGame")){
            // Explicit pending-owner feedback, not a port of Application::LoadLevel.
            // The authored single-player button supplies one numeric difficulty.
            if(!ui::swf_front_pending_start_v1(fn,self.start_requested_difficulty,error))return false;
            if(self.menu_avatar.slot<0){error="Select a character before Play";return false;}
            std::shared_ptr<const FrontSelectedProfileV50> retained;
            if(!self.reread_profile_v114(self.process_application_v114.lock(),self.menu_avatar.slot,
                self.start_requested_difficulty,retained,error))return false;
            self.selected_launch_profile_v50=std::move(retained);
            self.launch_pending=true;return true;
        }
        if(!std::strcmp(name,"NativeAssignSaveSlotToPlayer")){
            // dqmenus calls this as NativeAssignSaveSlotToPlayer(0, SlotID)
            // immediately before StartGame. Publish only into the same
            // process Application's already-owned PlayerManager; this does
            // not create a PlayerInfo, touch a save, or synthesize success.
            // GameSWF fn_call::arg(0) is the topmost ActionScript stack item
            // (the last source argument); IDA NativeAssignSaveSlotToPlayer
            // reads that item as the assigned profile slot, then reads arg1
            // as local-player index. The SWF authors the call as (0, SlotID).
            std::int32_t local_index=0,slot=0;
            if(!dh2::ui::swf_front_assign_save_slot_args_v1(fn,local_index,slot,error))return false;
            const auto application=self.process_application_v114.lock();
            const auto players=application?application->source_player_manager_v59():nullptr;
            if(!application||!players||!players->belongs_to_application(application)){
                error="NativeAssignSaveSlotToPlayer requires the same process Application PlayerManager";return false;
            }
            // The authored menu assigns SlotID before NativeStartGame. The
            // process PlayerManager constructor is retained at MenuManager
            // Init, but its original first-local-controller prefix is reached
            // later by the campaign adapter. Run that same source prefix here
            // before publishing the slot, so both paths use the SAME App
            // PlayerInfo and the campaign path can safely observe it as
            // already initialized.
            std::shared_ptr<dh2::input::SourceInputManagerV60> input;
            if(!model_renderer::borrow_actual_input_manager_v60(input,error)||!input)return false;
            const bool assigned=players->assign_selected_save_slot_v70(local_index,slot,
                input->first_local_services(),error);
            __android_log_print(assigned?ANDROID_LOG_INFO:ANDROID_LOG_ERROR,tag,
                "NativeAssignSaveSlotToPlayer | local_index %d | selected_slot %d | PM %zu | phase %d | assigned %d | error %s",
                local_index,slot,reinterpret_cast<std::size_t>(players->manager()),
                static_cast<int>(players->phase()),assigned,error.c_str());
            return assigned;
        }
        if(!std::strcmp(name,"NativeStartFromGCInvite")){
            // Original 0x43de30 is a platform invite continuation: it may
            // assign the invited save slot, initialize OnlineGameState, and
            // route to verification. This front-only owner has no GameCenter/
            // Wi-Fi invite session, so preserve the original no-invite result
            // as a handled callback rather than failing the authored event
            // delivery prefix.
            return true;
        }
        if(!std::strcmp(name,"NativeCreateSaveSlot"))return ui::swf_front_create_save_slot_v1(fn,&self,
            [](void* context,const char* player_name,const char* character,std::uint32_t& slot,std::string& error){return static_cast<Impl*>(context)->create_menu_persona(player_name,character,slot,error);},error);
        if(!std::strcmp(name,"NativeEraseSaveSlot"))return ui::swf_front_erase_save_slot_v1(fn,{&self,menu_flush_front_jobs,menu_erase_slot_files},error);
        if(!std::strcmp(name,"NativeSetSaveSlotIDToMainMenu")){
            const auto previous=self.menu_avatar.slot;
            const double requested=fn.nargs>0&&fn.env?fn.arg(0).to_number():-999.0;
            const bool handled=ui::swf_menu_avatar_preview_v1(fn,self.menu_avatar,self.menu_avatar_services,error);
            __android_log_print(handled?ANDROID_LOG_INFO:ANDROID_LOG_WARN,tag,
                "Menu slot callback | previous_slot %d | requested_slot %.0f | resulting_slot %d | handled %d | error %s",
                previous,requested,self.menu_avatar.slot,handled,error.c_str());
            return handled;
        }
        if(!std::strcmp(name,"NativeGetParsedString"))return ui::swf_menu_parsed_string_v1(fn,&self,menu_parsed_string,error);
        if(!std::strcmp(name,"NativeGetSaveSlotDetails")){
            ui::SwfFrontSaveSlotServicesV1 services{&self,menu_slot_exists,menu_slot_details};
            return ui::swf_front_save_slot_details(fn,services,error);
        }
        for(const auto* action:{"NativeGetOptionParameters","NativeSetOptions","NativeLoadSettings","NativeSaveSettings","NativeEnterOptionMenu","NativeRefreshHudManager","NativeChangeRolloverInputBehavior","NativeIsJapaneseVersion","NativeIsKorean"})
            if(!std::strcmp(name,action))return ui::swf_menu_settings_action(name,fn,self.option_services(),error);
        if(!std::strcmp(name,"NativeOptionFX")||!std::strcmp(name,"NativeOptionMusic"))
            return legacy_volume_option(&self,name,fn,error);
        const ui::SwfMenuNavigationServicesV1 navigation{context,push_menu,pop_menu,pop_top_menu,pop_above_menu};
        if(!std::strcmp(name,"NativePushMenu")){
            std::string requested;
            const bool string_argument=fn.nargs>0&&fn.env&&fn.arg(0).is_string();
            if(string_argument)requested=fn.arg(0).to_string();
            const bool trace=string_argument&&(requested=="menu_Options"||requested=="menu_info");
            if(trace)__android_log_print(ANDROID_LOG_INFO,tag,"Menu tap trace | NativePushMenu entry | argument %s | nargs %d | stack depth %zu | current %s",requested.c_str(),fn.nargs,self.menu_stack.size(),self.menu_stack.empty()?"":self.menu_stack.back().c_str());
            const bool handled=ui::swf_menu_push(fn,navigation,error);
            if(trace)__android_log_print(handled?ANDROID_LOG_INFO:ANDROID_LOG_WARN,tag,"Menu tap trace | NativePushMenu return | argument %s | handled %d | error %s",requested.c_str(),handled,error.c_str());
            return handled;
        }
        if(!std::strcmp(name,"NativePopMenu"))return ui::swf_menu_pop(fn,navigation,error);
        if(!std::strcmp(name,"NativePopAllAbove"))return ui::swf_menu_pop_above(fn,navigation,error);
        if(!std::strcmp(name,"NativeGetCreditMovement")){
            // 0x43ccec: HTC_DEVICES ? 2 : unsigned(Application.GetDt)/25+1.
            // This isolated emulator profile is non-HTC; integer menu dt is
            // the same live clock passed to both retained renderer timelines.
            return ui::swf_menu_credit_movement(fn,std::uint32_t(self.last_menu_dt),false,error);
        }
        if(!std::strcmp(name,"NativeBackToHud")){
            // 0x4449ac..0x4449b4 exits when Application.GetCurrentLevel is
            // null. The front-only session has no attached gameplay level.
            if(self.front_screen=="main"&&!self.live_player)return true;
            error="BackToHud requires the attached gameplay level owner";return false;
        }
        if(std::strcmp(name,"NativePlaySoundFX")){error=std::string("Unknown owned menu native action: ")+name;return false;}
        // Original 0x43ae10: exactly one STRING/WIDE_STRING, lookup by name;
        // invalid arguments or absent ID are a genuine no-op. This core uses
        // its UTF-8 STRING representation (it has no separate wide-string tag).
        std::string requested;
        if(!ui::swf_menu_sound_argument(fn,requested))return true;
        for(std::size_t id=0;id<std::size(original_sounds);++id){
            const auto& record=original_sounds[id];
            if(requested!=record.name)continue;
            // Source LoadSound -> StreamCFile::Init leaves size zero on absent
            // file. Its null cursor produces an invalid DataHandle, and Play
            // returns at IsReady (36b908/36b90c), without failing the AS caller.
            const std::string sound_path=std::string("original-media/")+record.file;
            AAsset* sound=AAssetManager_open(self.manager,sound_path.c_str(),AASSET_MODE_STREAMING);
            if(!sound){
                __android_log_print(ANDROID_LOG_WARN,tag,"Original menu sound not ready | name %s | id %zu | missing file %s",requested.c_str(),id,record.file);
                return true;
            }
            AAsset_close(sound);
            self.menu_sounds.emplace_back(record.file);
            __android_log_print(ANDROID_LOG_INFO,tag,"Original menu sound requested | name %s | id %zu | file %s",requested.c_str(),id,record.file);
            return true;
        }
        return true;
    }
    struct MenuChange {Impl* self;std::string name;bool show,pushed;};
    static bool render_class_scene(void*,int width,int height,std::string& error){
        try{model_renderer::draw_class_scene(width,height);return true;}
        catch(const std::exception& e){error=e.what();return false;}
    }
    static bool class_pane(void* context,const ui::SwfDraw& pane,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return self.gpu.scene_pane(pane.rect,&self,render_class_scene,error);
    }
    static bool update_class(void* context,ui::SwfAsGraph& graph,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        static constexpr const char* titles[]={"MENU_CLASS_00","MENU_CLASS_01","MENU_CLASS_02"};
        static constexpr const char* descriptions[]={"MENU_KNIGHT_DESC","MENU_ROGUE_DESC","MENU_MAGE_DESC"};
        static constexpr const char* classes[]={"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"};
        ui::SwfAsValue root,menu,result;bool callable=false,accepted=false;
        if(!graph.root_value(root,error)||!graph.find_target(root,"menu_SelectClass",menu,error))return false;
        // Both title and description are sprites containing EditText children.
        // The title child is placed by PlaceObject3; writing htmlText on its
        // parent only adds a sprite property and leaves TITLE_14 on screen.
        // The supplied description sprite is class_description, not the
        // s_description name used by the native reference's cached field.
        const char* paths[]={"menu_SelectClass.class_title.text","menu_SelectClass.class_description.text"};
        const char* symbols[]={titles[self.class_index],descriptions[self.class_index]};
        for(unsigned i=0;i<2;++i){
            ui::SwfAsValue field;std::uint32_t id=0;std::string text;
            if(!graph.find_target(root,paths[i],field,error)){
                const auto lookup_error=error;
                error=std::string("Required original class text field absent at ")+paths[i];
                if(!lookup_error.empty())error+=" ("+lookup_error+")";
                return false;
            }
            if(!field.identity()){error=std::string("Required original class text field has no retained receiver at ")+paths[i];return false;}
            if(!constant(&self,"StrID",symbols[i],id,error)||!self.text_id_v101(id,text,error))return false;
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
        if(!graph.invoke(menu,menu,"CurrentClass",{ui::SwfAsValue::text(classes[self.class_index])},result,callable,error))return false;
        if(!callable){error="Required original CurrentClass callback absent";return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Original class selection updated | index %d | class %s | preview actors pending",self.class_index,classes[self.class_index]);
        self.class_applied_index=self.class_index;
        return true;
    }
    static bool change_menu(void* context,ui::SwfAsGraph& graph,std::string& error){
        auto& change=*static_cast<MenuChange*>(context);
        ui::SwfAsValue root,menu,result;bool callable=false,accepted=false;
        if(!graph.root_value(root,error)||!graph.find_target(root,change.name.c_str(),menu,error))return false;
        if(!menu.identity()){error="Required authored navigation clip absent";return false;}
        if(!change.show){
            if(!graph.invoke(menu,menu,"onHide",{},result,callable,error))return false;
            return graph.set_member(menu,"_visible",ui::SwfAsValue::boolean(false),accepted,error);
        }
        if(!graph.set_member(menu,"_visible",ui::SwfAsValue::boolean(true),accepted,error)||
           !change.self->menu_movie(change.name)->menu_input_context(("_root."+change.name).c_str(),error))return false;
        if(change.name=="menu_EnterName"||change.name=="menu_SelectClass"){
            // Original MultiMenuManager::PushMenu 0x438488 invokes onPush,
            // then 0x4385f0..0x43860c plays the authored "show" animation
            // for the ordinary (non-0x40) renderer before MenuBase::Show.
            // Its frame 15 action clears the actual name field. Merely
            // exposing the idle clip leaves its authoring HTML as the name.
            if(change.pushed&&!graph.invoke(menu,menu,"onPush",{},result,callable,error))return false;
            if(!graph.invoke(menu,menu,"gotoAndPlay",{ui::SwfAsValue::text("show")},result,callable,error))return false;
            if(!callable){error="Authored name menu show timeline unavailable";return false;}
            if(!graph.invoke(menu,menu,"onShow",{},result,callable,error))return false;
            if(change.name=="menu_SelectClass"){
                // Show resets the previous index (0x428fe8), retaining the
                // current selection. The singleton constructor starts at 0.
                if(!update_class(change.self,graph,error)||
                   !change.self->movie->menu_display_callback("_root.menu_SelectClass.class_select",change.self,class_pane,error))return false;
            }
            return true;
        }
        if(!graph.invoke(menu,menu,"onShow",{},result,callable,error))return false;
        if(change.pushed&&!graph.invoke(menu,menu,"onPush",{},result,callable,error))return false;
        return true;
    }
    bool transition_menu(const std::string& previous,const std::string& next,bool pushed,std::string& error){
        const bool trace=next=="menu_Options"||next=="menu_info"||previous=="menu_Options"||previous=="menu_info";
        if(trace)__android_log_print(ANDROID_LOG_INFO,tag,"Menu tap trace | transition begin | %s -> %s | pushed %d",previous.c_str(),next.c_str(),pushed);
        MenuChange hide{this,previous,false,false},show{this,next,true,pushed};
        if(!menu_movie(previous)->menu_action_script(&hide,change_menu,error)||
           !menu_movie(next)->menu_action_script(&show,change_menu,error)){
            if(trace)__android_log_print(ANDROID_LOG_WARN,tag,"Menu tap trace | transition failed | %s -> %s | error %s",previous.c_str(),next.c_str(),error.c_str());
            return false;
        }
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu renderer selected | name %s | renderer %s",next.c_str(),shared_state(next)?"shared":"main");
        if(trace)__android_log_print(ANDROID_LOG_INFO,tag,"Menu tap trace | transition complete | %s -> %s | renderer %s",previous.c_str(),next.c_str(),shared_state(next)?"shared":"main");
        return true;
    }
    static bool push_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        const bool trace=name&&(!std::strcmp(name,"menu_Options")||!std::strcmp(name,"menu_info"));
        if(trace)__android_log_print(ANDROID_LOG_INFO,tag,"Menu tap trace | push_menu entry | requested %s | stack depth %zu | current %s",name,self.menu_stack.size(),self.menu_stack.empty()?"":self.menu_stack.back().c_str());
        if(!name||self.menu_stack.empty()){
            error="Native menu stack unavailable";
            if(trace)__android_log_print(ANDROID_LOG_WARN,tag,"Menu tap trace | push_menu unavailable | requested %s",name?name:"<null>");
            return false;
        }
        // GetMenuByName returns null for unregistered states. Explicitly log
        // the partial registration rather than presenting those paths as done.
        if(std::strcmp(name,"menu_info")&&std::strcmp(name,"menu_MainMenu")&&std::strcmp(name,"menu_EnterName")&&std::strcmp(name,"menu_SelectClass")&&std::strcmp(name,"menu_StartGame")&&!shared_state(name)){
            __android_log_print(ANDROID_LOG_WARN,tag,"Menu navigation not connected | requested %s",name);return true;
        }
        if(trace)__android_log_print(ANDROID_LOG_INFO,tag,"Menu tap trace | push_menu recognized | requested %s | stack depth %zu",name,self.menu_stack.size());
        if(std::find(self.menu_stack.begin(),self.menu_stack.end(),name)!=self.menu_stack.end()){
            if(trace)__android_log_print(ANDROID_LOG_INFO,tag,"Menu tap trace | push_menu duplicate guard | requested %s",name);
            return true;
        }
        const auto previous=self.menu_stack.back();
        self.menu_stack.emplace_back(name);
        if(!self.transition_menu(previous,name,true,error)){
            self.menu_stack.pop_back();
            if(trace)__android_log_print(ANDROID_LOG_WARN,tag,"Menu tap trace | push_menu transition rolled back | requested %s | current %s",name,self.menu_stack.empty()?"":self.menu_stack.back().c_str());
            return false;
        }
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu navigation | push %s | depth %zu",name,self.menu_stack.size());
        return true;
    }
    static bool pop_top_menu(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(self.menu_stack.size()<2)return true;
        const auto previous=self.menu_stack.back();self.menu_stack.pop_back();
        if(!self.transition_menu(previous,self.menu_stack.back(),false,error)){self.menu_stack.push_back(previous);return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu navigation | pop %s | current %s | depth %zu",previous.c_str(),self.menu_stack.back().c_str(),self.menu_stack.size());
        return true;
    }
    static bool pop_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.menu_stack.empty()&&name&&self.menu_stack.back()==name)return pop_top_menu(context,error);
        return true;
    }
    static bool pop_above_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!name){error="Missing pop-above menu name";return false;}
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
        if(!graph.invoke(menu,menu,"onShow",{},result,callable,error))return false;
        if(!callable){error="Authored main menu onShow missing";return false;}
        return graph.invoke(menu,menu,"onPush",{},result,callable,error);
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
        if(!self.frame_owner->history||!self.frame_owner->frames){error="Required shared source frame/history borrow";return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Original source frame/history bound before shared/root construction");
        return true;
    }
    static bool shared_graph_start(void* context,const ui::SwfAsLease& lease,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.shared_frame_owner||!lease.player){error="Shared menu frame owner unavailable";return false;}
        if(!self.shared_frame_owner->history||!self.shared_frame_owner->frames){error="Required shared source frame/history borrow";return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Original shared renderer frame/history bound before root construction");
        return true;
    }
    bool load(std::string& error,bool base_only_v105=false,const char* source_uri_v114=nullptr,bool source_main_v114=false,
     const std::function<bool(std::int32_t&,std::int32_t&,std::string&)>& source_layout_v114={}) {
        if(!constants||!debug){error="Required native UI owner allocation failed";return false;}
        if(source_main_v114&&!source_metadata_ready_v114){error="Required actual Init0 process font/string metadata prefix";return false;}
        if(!source_main_v114){
        for(const auto* uri:{"data/pydata/common_text_pycst.bin","data/fonts_pycst.bin"}) {
            std::vector<std::uint8_t> bytes;
            if(std::strcmp(uri,"data/fonts_pycst.bin")==0){if(!raw_asset(uri,bytes,error))return false;}
            else if(!assets.read(uri,bytes,error))return false;
            dh2_script_constants_reload record{};
            if(dh2_script_constants_load(constants.get(),bytes.data(),static_cast<std::uint32_t>(bytes.size()),&record)!=0){error="Required UI constants load failed";return false;}
        }
        std::array<std::vector<std::uint8_t>,3> metadata;
        const char* uris[]={"data/pydata/common_text_pyarray.bin","data/pydata/common_text_pyarraynames.bin","data/pydata/common_text_pystructnames.bin"};
        for(unsigned i=0;i<3;++i)if(!assets.read(uris[i],metadata[i],error))return false;
        if(!process_text_v101&&!localization.load({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
        if(front_screen=="main"){
            const char* suffixes[]={"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("data/help_pages")+suffixes[i]).c_str(),metadata[i],error))return false;
            if(!loading_hints.load({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original loading hints connected | rows %zu",loading_hints.rows().size());
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("data/character_properties")+suffixes[i]).c_str(),metadata[i],error))return false;
            if(!dh2::data::load_characters({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},menu_characters,error))return false;
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("data/levels")+suffixes[i]).c_str(),metadata[i],error))return false;
            if(!dh2::data::load_levels({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},menu_levels,error))return false;
            const char* inputs[]={"design_pyarray.bin","design_pyarraynames.bin","design_pystructnames.bin"};
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("original-cache/data/pydata/")+inputs[i]).c_str(),metadata[i],error))return false;
            if(!settings){
                option_table=ui::GameOptionTableV1();
                if(!option_table.load_design_cache({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
                settings=std::make_shared<ui::OwnedHudSettingsV1>(option_table.borrow());
            }
            // App4c and campaign save callbacks retain this SAME manager.
            // A front movie reload may reload options; it must not replay C1
            // or replace the mutable difficulty/options authority.
            if(!process_settings_v105){settings_files=std::make_unique<ui::SettingsNativeFilesV1>(directory);
                if(!load_settings(this,error))return false;}
            if(install_front_inspection_avatar_services_v1(menu_avatar_services,menu_avatar_services_owner_v121,
                {this,persona_destroy,persona_setup,persona_camera})&&
                ::access((directory+"/menu-persona-preview.enabled").c_str(),F_OK)==0)enable_menu_persona();
        }
        source_metadata_ready_v114=true;
        }
        ui::SwfServices services;
        frame_owner=std::make_shared<FrameOwner>();
        services.native_owner=frame_owner;services.graph_start=graph_start;
        services.native_actions={"NativePlaySoundFX","NativePushMenu","NativePopMenu","NativePopAllAbove","NativeGetCreditMovement","NativeBackToHud","NativeGetParsedString"};services.native_action=native_action;
        if(front_screen=="main"){
            services.native_actions.emplace_back("NativeGetSaveSlotDetails");
            services.native_actions.emplace_back("NativeEraseSaveSlot");
            services.native_actions.emplace_back("NativeAssignSaveSlotToPlayer");
            services.native_actions.emplace_back("NativeSetSaveSlotIDToMainMenu");
            services.native_actions.emplace_back("NativeCreateSaveSlot");
            services.native_actions.emplace_back("NativeStartGame");
            services.native_actions.emplace_back("NativeHUDInteract");
            services.native_actions.emplace_back("NativeLaunchTwitter");
            services.native_actions.emplace_back("NativeLaunchIGP");
            services.native_actions.emplace_back("NativeGetLoadingProgress");
            services.native_actions.emplace_back("NativeIsMultiplayerLoadCompleted");
            services.native_actions.emplace_back("NativeIsMultiplayerHost");
            services.native_actions.emplace_back("NativeMustWaitForHost");
            services.native_actions.emplace_back("NativeEndLoading");
            services.native_actions.emplace_back("NativeGetLoadingTipStrID");
            services.native_actions.emplace_back("NativeGetStringFromID");
            services.native_actions.emplace_back("NativeHasPushNotification");
            services.native_actions.emplace_back("NativeOnlineSanityCheck");
            // Init25 adds the full shipping table later, but invite-return can
            // be reached by an early main-menu callback before that phase.
            services.native_actions.emplace_back("NativeStartFromGCInvite");
            services.native_actions.emplace_back("NativeIsMultiplayerEnabled");
            for(const char* action:{"NativePlayMusic","NativePauseMusic","NativeStopMusic"})services.native_actions.emplace_back(action);
        }
        if(front_screen=="main")for(const auto* action:{"NativeGetOptionParameters","NativeSetOptions","NativeLoadSettings","NativeSaveSettings","NativeEnterOptionMenu","NativeRefreshHudManager","NativeChangeRolloverInputBehavior","NativeIsJapaneseVersion","NativeIsKorean","NativeOptionFX","NativeOptionMusic"})services.native_actions.emplace_back(action);
        services.context=this;services.read=movie_read;services.texture=texture;services.image=image;
        services.release_image_v119=[this](const ui::SwfTexture& image,std::string& error){return gpu.source_remove_image_v119(image,error);};
        services.draw=draw;services.stencil=stencil;services.native_call=native;services.diagnostic=diagnostic;
        const auto bitmap_backends=initialize_gfnt_backend_v1();
        const auto make_font_platform=[&](const ui::SwfServices& source){
            auto platform=std::make_unique<ui::SwfTextFontPlatformV1>(ui::SwfFontServices{this,source_font_read_gfnt_v1,font_diagnostic},source,shared_from_this(),bitmap_backends,1.f);
            platform->policy().renderer_feature=[this](const ui::edit_text_display_v1::Command& command,std::string& error){
                if(command.kind==ui::edit_text_display_v1::Command::grid_fit){gpu.set_grid_fit(command.enabled);return true;}
                error="Required original menu text render-cache connection";return false;
            };
            return platform;
        };
        if(front_screen=="main"&&!shared_menu_movie&&!source_main_v114){
            // Original RenderFX::Load (0x7ab7c0..0x7ab7e0) constructs a
            // separate player per renderer. Importing common definitions into
            // the main player does not activate the shared menu renderer.
            shared_frame_owner=std::make_shared<FrameOwner>();
            shared_menu_movie=std::make_shared<ui::SwfMovie>();base_deleted_v93=false;
            auto shared_services=services;
            shared_services.native_owner=shared_frame_owner;
            shared_services.graph_start=shared_graph_start;
            ui::SwfServices shared_wrapped;ui::SwfSourceFrameBorrowV1 shared_frames;
            if(!ui::source_movie_services_v1(shared_services,shared_wrapped,error)||
               !ui::source_movie_frame_borrow_v1(shared_wrapped,shared_frames,error))return false;
            shared_frame_owner->history=shared_frames.history;shared_frame_owner->frames=shared_frames.frames;
            shared_fonts=make_font_platform(shared_wrapped);
            if(!shared_menu_movie->load({},source_movies_v132?process_variant_v132.uri[0]:"data/menus/dqshared_droid.swf",shared_fonts->services(),error))return false;
            shared_camera_owner_v93=std::make_shared<ui::MenuFlash2DCameraOwnerV93>(reinterpret_cast<std::uintptr_t>(shared_menu_movie.get()),driver_width,driver_height);
            const auto& shared_rect=process_variant_v132.movie_rect;
            const ui::ViewportState64 shared_seed{{shared_rect[0],shared_rect[1],shared_rect[2],shared_rect[3]},{0,0,480,320},{0,0,480,320},1.f,0,0};
            std::vector<std::string> shared_states;
            ui::SwfClipInfo options;
            if(!shared_menu_movie->connect_viewport(shared_seed,{this,orientation,dimensions},error)||
               !camera_update_v93(*shared_menu_movie,shared_camera_owner_v93,error)||
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
        //Original Init0 publishes only base0; primary2 is created at Init2.
        if(base_only_v105){error.clear();return true;}
        movie=std::make_shared<ui::SwfMovie>();main_deleted_v93=false;
        const char* movie_uri=source_uri_v114?source_uri_v114:front_screen=="main"?(source_movies_v132?process_variant_v132.uri[2]:"data/menus/dqmenus_droid.swf"):front_screen=="loading"?"data/menus/loadanims_droid.swf":"data/menus/dqhud_droid.swf";
        ui::SwfServices wrapped;ui::SwfSourceFrameBorrowV1 borrowed_frames;
        if(!ui::source_movie_services_v1(services,wrapped,error)||
           !ui::source_movie_frame_borrow_v1(wrapped,borrowed_frames,error))return false;
        frame_owner->history=borrowed_frames.history;frame_owner->frames=borrowed_frames.frames;
        fonts=make_font_platform(wrapped);
        if(source_main_v114){
            if(!movie->load_source_resource_v98(movie_uri,fonts->services(),error)||!movie->source_set_text_buffering_v98(true,error))return false;
            // Original text-buffer85 store precedes actual paired camera C1.
            if(!source_layout_v114||!source_layout_v114(driver_width,driver_height,error))return false;
        }else if(!movie->load({source_movies_v132?process_variant_v132.uri[0]:"data/menus/dqshared_droid.swf"},movie_uri,fonts->services(),error))return false;
        if(shared_menu_movie){
            const auto primary=movie->player_identity(),shared=shared_menu_movie->player_identity();
            if(!primary||!shared||primary==shared){error="Menu renderers did not retain independent players";return false;}
            __android_log_print(ANDROID_LOG_INFO,tag,"Original menu renderer player identities | main %zx | shared %zx | distinct 1",primary,shared);
        }
        const auto& movie_rect=process_variant_v132.movie_rect;
        const ui::ViewportState64 seed{{movie_rect[0],movie_rect[1],movie_rect[2],movie_rect[3]},{0,0,480,320},{0,0,480,320},1.f,0,0};
        camera_owner_v93=std::make_shared<ui::MenuFlash2DCameraOwnerV93>(reinterpret_cast<std::uintptr_t>(movie.get()),driver_width,driver_height);
        if(!movie->connect_viewport(seed,{this,orientation,dimensions},error)||
           !camera_update_v93(*movie,camera_owner_v93,error)||(!source_main_v114&&!movie->advance(0,error)))return false;
        if(source_main_v114){
            ui::SwfInputCoreServices input;input.owner=frame_owner;input.context=this;
            input.native_receiver=reinterpret_cast<std::uintptr_t>(&frame_owner->main_events);
            input.can_handle_event=input_accepts;input.native_event=input_native_event;input.advance=input_advance;
            // RenderFX C2 7a85f8 writes flags8=0; LoadMainMenu sets84 later.
            if(!movie->connect_input("_root",frame_owner->history,0,frame_owner->input_selection,{this,orientation,dimensions},input,error))return false;
            frame_owner->main_events.render_bound=true;loaded=true;return true;
        }
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
                const auto scene=model_renderer::load_menu_background(manager);
                if(scene.find("3D upload OK")!=0){error=scene;return false;}
            }
            loaded=true;menu_clock.reset();frame_time=std::chrono::steady_clock::now();return true;
        }
        ui::SwfClipInfo clip;
        if(!movie->clip(panel,clip,error)||clip.id!=157){error="Required original health/mana parent differs";return false;}
        if(!font_failure.empty()){error=font_failure;return false;}
        status=std::make_unique<ui::PlayerStatusHud>(*movie);
        if(!status->bind(hud_sha,error))return false;
        loaded=true;return true;
    }
    bool reset_failed(std::string& error) {
        gpu.abort();status.reset();shared_menu_movie.reset();movie.reset();shared_camera_owner_v93.reset();camera_owner_v93.reset();main_deleted_v93=base_deleted_v93=false;shared_frame_owner.reset();frame_owner.reset();shared_fonts.reset();fonts.reset();gfnt_text_backend_v1.reset();loaded=false;selected=false;
        last_width=last_height=0;loading_bitmap_reported=false;reported_frames={{-1,-1,-1,-1,-1}};
        leases.clear();exports.clear();process_splash_v119={};process_splash_uri_v119.clear();
        splash_source_uri_v1.clear();splash_texture_identity_v1=0;rendering_splash_clip_v1=false;
        font_failure.clear();provider_failure.clear();
        menu_sounds.clear();
        menu_audio.clear();menu_browser.clear();menu_catalog.clear();settings.reset();settings_files.reset();language_selection=-1;
        menu_stack.clear();start_feedback_pending=false;launch_pending=false;selected_launch_profile_v50.reset();
        exit_confirmation_open=exit_requested=false;
        input_dispatch_movie=nullptr;last_menu_dt=0;
        menu_clock.reset();
        glyph_uploads=bitmap_uploads=string_calls=core_errors=packed_glyphs=strips=lines=masks=0;
        return gpu.reset_images(error);
    }
};
FrontUiSessionV87::FrontUiSessionV87():impl_(std::make_shared<Impl>()){}
bool FrontUiSessionV87::base_movie_borrow_v4(ui::SwfMovie*& movie,std::shared_ptr<void>& owner,std::string& error){
 movie=nullptr;owner.reset();
 if(!impl_->shared_menu_movie){error="Required actual retained base menu movie";return false;}
 movie=impl_->shared_menu_movie.get();owner=std::make_shared<std::pair<std::shared_ptr<Impl>,std::shared_ptr<ui::SwfMovie>>>(impl_,impl_->shared_menu_movie);return true;
}
bool FrontUiSessionV87::movie_slot_v93(std::uint32_t slot,ui::MenuMovieBorrowV58& out,std::string& e){
 out={};if(slot!=0&&slot!=2){e="Front movie owner supplies only source slots0/2";return false;}
 auto actual=slot==0?impl_->shared_menu_movie:impl_->movie;
 if(actual){out.identity=reinterpret_cast<std::uintptr_t>(actual.get());
  out.actual_owner=std::make_shared<std::pair<std::shared_ptr<Impl>,std::shared_ptr<ui::SwfMovie>>>(impl_,actual);}
 e.clear();return true;
}
bool FrontUiSessionV87::source_main_movie_borrow_v114(ui::MenuMovieBorrowV58& out,std::string& e){
 out={};auto actual=impl_;if(!actual){e="Required SAME actual Front resource owner";return false;}
 if(actual->movie){out.actual_owner=actual->movie;out.identity=reinterpret_cast<std::uintptr_t>(actual->movie.get());}
 e.clear();return true;
}
bool FrontUiSessionV87::source_load_main_movie_v114(const char* uri,
 const std::function<bool(std::int32_t&,std::int32_t&,std::string&)>& layout,
 ui::MenuMovieBorrowV58& out,std::string& e){
 out={};auto actual=impl_;if(!actual||!uri||!*uri){e="Required authored LoadSWFFile CString and SAME Front owner";return false;}
 auto& self=*actual;
 // MultiMenu.LoadSWFFile437d90 produces SAME Game_Text BEFORE slot2 lookup,
 // including an already retained positive renderer. NativeGameString writes
 // this SAME existing field; no parallel process global is introduced.
 self.menu_string_flag=true;
 if(!self.main_source_load_failure_v114.empty()){e=self.main_source_load_failure_v114;return false;}
 if(self.movie){
  if(self.main_deleted_v93||!self.movie->player_identity()){e="Actual positive main slot has an incomplete native resource prefix";return false;}
  return source_main_movie_borrow_v114(out,e);
 }
 if(self.camera_owner_v93||self.front_screen!="main"||self.directory.empty()||self.input_dispatch_movie||self.source_transport_busy_v93){
  e="Required same quiescent source main2/C1 camera/private-file context";return false;
 }
 std::string local;bool ok=false;
 try{ok=self.load(local,false,uri,true,layout);}catch(const std::exception& x){local=x.what();}catch(...){local="Native main2 resource loader threw";}
 if(!ok){self.main_source_load_failure_v114=local.empty()?"Failed actual main2 constructor/Load/camera prefix":local;e=self.main_source_load_failure_v114;return false;}
 return source_main_movie_borrow_v114(out,e);
}
bool FrontUiSessionV87::select_process_movies_v132(const ui::SourceMenuVariantV132& variant,std::string& e){
 auto& self=*impl_;
 if(self.movie||self.shared_menu_movie||self.source_movies_v132||!variant.uri[0]||!variant.uri[2]||!variant.uri[3]){e="Source movie selection requires fresh actual process slots";return false;}
 self.process_variant_v132=variant;self.source_movies_v132=true;e.clear();return true;
}
bool FrontUiSessionV87::load_main_movie_slot_v93(std::string& e){
 auto& self=*impl_;
 if(self.front_screen!="main"||self.directory.empty()||self.input_dispatch_movie||self.source_transport_busy_v93){e="Main resource load requires same quiescent main-menu context/private directory";return false;}
 if(self.movie){if(self.main_deleted_v93||!self.movie->player_identity()){e="Main source slot still awaits ordered resource clear";return false;}e.clear();return true;}
 if(self.camera_owner_v93){e="Main source camera still awaits ordered paired clear";return false;}
 // Source creates the missing primary2 without replacing retained base0.
 // Failure leaves the target's reached source prefix for root cleanup.
 return self.load(e);
}
bool FrontUiSessionV87::load_base_movie_slot_v105(std::string& e){
 auto& self=*impl_;
 if(self.front_screen!="main"||self.directory.empty()||self.input_dispatch_movie||self.source_transport_busy_v93){e="Base source load requires quiescent actual process front context";return false;}
 if(self.shared_menu_movie){if(self.base_deleted_v93||!self.shared_menu_movie->player_identity()){e="Base slot awaits genuine resource retirement";return false;}e.clear();return true;}
 if(self.shared_camera_owner_v93){e="Base camera awaits genuine paired field clear";return false;}
 return self.load(e,true);
}
bool FrontUiSessionV87::camera_slot_v93(std::uint32_t slot,ui::MenuCameraBorrowV93& out,std::string& e){
 out={};if(slot!=0&&slot!=2){e="Front camera owner supplies only source slots0/2";return false;}
 auto actual=slot==0?impl_->shared_camera_owner_v93:impl_->camera_owner_v93;
 if(actual){out={actual,reinterpret_cast<std::uintptr_t>(actual.get())};}e.clear();return true;
}
bool FrontUiSessionV87::movie_virtual10_v93(std::uint32_t slot,std::uintptr_t expected,std::int32_t dt,bool flag,std::string& e){
 if(slot!=0&&slot!=2){e="Front Update owns only source slots0/2";return false;}
 auto movie=slot==0?impl_->shared_menu_movie:impl_->movie;
 const bool deleted=slot==0?impl_->base_deleted_v93:impl_->main_deleted_v93;
 auto frame=slot==0?impl_->shared_frame_owner:impl_->frame_owner;
 if(!movie||deleted||reinterpret_cast<std::uintptr_t>(movie.get())!=expected||!frame||!frame->history||!frame->frames){e="Required same live source front movie/frame/input owner";return false;}
 impl_->last_menu_dt=dt;
 struct Dispatch {Impl& self;ui::SwfMovie* previous;~Dispatch(){self.input_dispatch_movie=previous;}} dispatch{*impl_,impl_->input_dispatch_movie};
 impl_->input_dispatch_movie=movie.get();
 if(!movie->source_update_v93(dt,flag,e))return false;
 if(slot==2&&model_renderer::class_scene_active())return model_renderer::select_class_scene(impl_->class_index,dt,e);
 return true;
}
bool FrontUiSessionV87::deleting_movie_v93(std::uint32_t slot,std::uintptr_t expected,std::string& e){
 if(slot!=0&&slot!=2){e="Front destructor owns only source slots0/2";return false;}
 auto movie=slot==0?impl_->shared_menu_movie:impl_->movie;
 if(!movie||reinterpret_cast<std::uintptr_t>(movie.get())!=expected||movie->player_identity()||impl_->input_dispatch_movie){e="Front deleting destructor requires same unloaded quiescent facade";return false;}
 bool& deleted=slot==0?impl_->base_deleted_v93:impl_->main_deleted_v93;
 if(deleted){e="Front deleting destructor already delivered";return false;}
 // Preserve the raw slot word until the source's separate clear operation.
 if(slot==2){impl_->status.reset();impl_->loaded=false;}
 deleted=true;e.clear();return true;
}
bool FrontUiSessionV87::clear_movie_slot_v93(std::uint32_t slot,std::string& e){
 if(slot!=0&&slot!=2){e="Front clear owns only source slots0/2";return false;}
 auto& movie=slot==0?impl_->shared_menu_movie:impl_->movie;
 bool& deleted=slot==0?impl_->base_deleted_v93:impl_->main_deleted_v93;
 if(movie&&!deleted){e="Source primary clear requires delivered deleting destructor";return false;}
 movie.reset();deleted=false;
 if(slot==0){impl_->shared_frame_owner.reset();impl_->shared_fonts.reset();}
 else {impl_->frame_owner.reset();impl_->fonts.reset();}
 // Frame/font ownership is NOT the paired camera. Shared GPU/export/settings
 // and the other movie resource remain live.
 e.clear();return true;
}
bool FrontUiSessionV87::deleting_camera_v93(std::uint32_t slot,std::uintptr_t expected,std::string& e){
 if(slot!=0&&slot!=2){e="Front camera destructor owns only source slots0/2";return false;}
 auto camera=slot==0?impl_->shared_camera_owner_v93:impl_->camera_owner_v93;
 if(!camera||reinterpret_cast<std::uintptr_t>(camera.get())!=expected||camera->deleted){e="Required same undeleted paired MenuFlash2DCamera";return false;}
 // Original42cd50 only frees camera storage; it does not touch renderer/font.
 camera->deleted=true;e.clear();return true;
}
bool FrontUiSessionV87::clear_camera_slot_v93(std::uint32_t slot,std::string& e){
 if(slot!=0&&slot!=2){e="Front paired clear owns only source slots0/2";return false;}
 auto& camera=slot==0?impl_->shared_camera_owner_v93:impl_->camera_owner_v93;
 if(camera&&!camera->deleted){e="Paired field clear requires delivered camera deleting destructor";return false;}
 camera.reset();e.clear();return true;
}
bool FrontUiSessionV87::claim_menu_transport_v93(std::shared_ptr<void> owner,std::function<bool(std::vector<FrontMovieDrawV93>&,std::string&)> draw,
 std::function<bool(const char*,const gameswf::fn_call&,std::string&)> navigation,std::string& e){
 if(!owner||!draw||!navigation||impl_->source_transport_busy_v93){e="Required same actual MenuManager draw/update/navigation ownership";return false;}
 const auto& old=impl_->source_menu_owner_v93;
 if(old&&(old.get()!=owner.get()||old.owner_before(owner)||owner.owner_before(old))){e="Front transport belongs to another live MenuManager";return false;}
 impl_->source_menu_owner_v93=std::move(owner);impl_->source_draw_v93=std::move(draw);impl_->source_navigation_v93=std::move(navigation);e.clear();return true;
}
bool FrontUiSessionV87::release_menu_transport_v93(const std::shared_ptr<void>& owner,std::string& e){
 const auto& old=impl_->source_menu_owner_v93;
 if(!old||!owner||old.get()!=owner.get()||old.owner_before(owner)||owner.owner_before(old)||impl_->source_transport_busy_v93){e="Front release requires same quiescent MenuManager";return false;}
 impl_->source_draw_v93={};impl_->source_navigation_v93={};impl_->source_menu_owner_v93.reset();impl_->menu_clock.reset();impl_->frame_time=std::chrono::steady_clock::now();e.clear();return true;
}
bool FrontUiSessionV87::render_source_movies_v93(int width,int height,std::string& e){
 try{
 auto& self=*impl_;if(!self.source_menu_owner_v93||!self.source_draw_v93||self.source_transport_busy_v93){e="Required actual front draw-order MenuManager";return false;}
 self.source_transport_busy_v93=true;struct Finish {bool& value;~Finish(){value=false;}} finish{self.source_transport_busy_v93};
 if(!self.viewport(width,height,e))return false;
 std::vector<FrontMovieDrawV93> draws;if(!self.source_draw_v93(draws,e))return false;
 bool loading_panel_drawn=false;
 // The retained stack can contain MainMenu beneath EnterName/SelectClass.
 // SelectClass.Show (IDA 0x428f38) already destroyed the main scene/camera.
 // Decide the scene for the whole frame before the first underlying clip;
 // selecting per clip would draw MainMenu with that retired camera, then
 // replace the renderer again for SelectClass.
 bool class_preview=false,name_entry=false,main_movie_visible=false;
 if(!self.source_loading_render_v114){
  for(const auto& q:draws){
   if(q.slot==2)main_movie_visible=true;
   class_preview=class_preview||q.actual_clip_path.find("menu_SelectClass")!=std::string::npos;
   name_entry=name_entry||q.actual_clip_path.find("menu_EnterName")!=std::string::npos;
  }
  const auto scene=front_scene_selection_v1(main_movie_visible,class_preview,name_entry);
  if(scene!=FrontSceneSelectionV1::authored_swf&&(!model_renderer::active()||model_renderer::class_scene_active()!=(scene==FrontSceneSelectionV1::class_selection))){
   const auto result=scene==FrontSceneSelectionV1::class_selection?model_renderer::load_class_scene(self.manager):model_renderer::load_menu_background(self.manager);
   if(result.find("3D upload OK")!=0){e=result;return false;}
   if(scene==FrontSceneSelectionV1::class_selection&&!model_renderer::select_class_scene(self.class_index,0,e))return false;
  }
  // Base-slot menu_bg carries the authored gradient as well. Draw the 3D
  // environment before every SWF layer, rather than repainting that gradient
  // when primary2 appears later in the retained draw list.
  if(scene==FrontSceneSelectionV1::menu_swamp)model_renderer::draw_menu_background(width,height,true);
  // EnterName's authored onShow activates BrownBG and its onHide clears it.
  // The native stack has hidden the parent menu_bg; submit only this authored
  // sibling, preserving its visibility gate and excluding RenderedBG/3D.
  if(scene==FrontSceneSelectionV1::authored_swf&&name_entry){
   if(!self.movie){e="Name backdrop requires the retained main movie";return false;}
   const auto stage=full_surface_stage_v87(width,height);
   if(!self.movie->display_clip("_root.menu_bg.BrownBG",stage.x,stage.y,stage.width,stage.height,e))return false;
  }
 }
 for(const auto& q:draws){
  if(q.slot!=0&&q.slot!=2){e="Front draw producer supplied non-front resource slot";return false;}
  const auto phase=self.source_loading_render_v114?FrontLoadingRenderPhaseV1::source_loading:
   FrontLoadingRenderPhaseV1::front_menu;
  if(self.source_loading_render_v114){
   if(!front_loading_clip_visible_v1(phase,q.slot,q.actual_clip_path))continue;
   loading_panel_drawn=true;
  }else if(!front_movie_slot_visible_v1(phase,q.slot))continue;
  auto movie=q.slot==0?self.shared_menu_movie:self.movie;
  if(!movie||reinterpret_cast<std::uintptr_t>(movie.get())!=q.expected_movie||!movie->player_identity()||q.actual_clip_path.empty()){e="Required same loaded front movie/source clip";return false;}
  // Class preview panes still render at their authored display callback.
  const bool previous_splash=self.rendering_splash_clip_v1;
  self.rendering_splash_clip_v1=q.actual_clip_path=="_root.menu_splash";
  struct SplashScope {bool& value;bool previous;~SplashScope(){value=previous;}} splash_scope{self.rendering_splash_clip_v1,previous_splash};
  if(!movie->display_source_stage_clip_v5(q.actual_clip_path.c_str(),e))return false;
 }
 if(self.source_loading_render_v114&&!loading_panel_drawn){
  e="Actual MenuManager draw list has no visible authored menu_Loading clip";return false;
 }
 // Timeline work belongs solely to MenuManager.virtual10. Empty draw order is
 // the actual producer's source result, not an absent-provider success.
 e.clear();return true;
 }catch(const std::exception& failure){
  // Front GPU transports throw on rejected required providers. Keep that
  // diagnostic in the existing error flow instead of escaping NativeBridge
  // and aborting the Android GLThread.
  e=std::string("Front menu rendering: ")+failure.what();return false;
 }
}
bool FrontUiSessionV87::character_menu_sound_v4(const gameswf::fn_call& call,std::string& error){
 return Impl::native_action(impl_.get(),"NativePlaySoundFX",call,error);
}
bool FrontUiSessionV87::main_movie_borrow_v4(ui::SwfMovie*& movie,std::shared_ptr<void>& owner,std::string& error){
 movie=nullptr;owner.reset();if(!impl_->movie){error="Required actual retained main-menu movie";return false;}
 movie=impl_->movie.get();owner=std::make_shared<std::pair<std::shared_ptr<Impl>,std::shared_ptr<ui::SwfMovie>>>(impl_,impl_->movie);return true;
}
FrontUiSessionV87::~FrontUiSessionV87(){std::string ignored;impl_->reset_failed(ignored);}
bool FrontUiSessionV87::gameplay_settings_borrow_v67(std::shared_ptr<ui::OwnedHudSettingsV1>& out,std::string& e)const{
 out=impl_->settings;
 if(!out){e="Required actual Application settings loaded by front session";return false;}
 e.clear();return true;
}
void FrontUiSessionV87::demo_persona_mode(){impl_->enable_menu_persona();}
bool FrontUiSessionV87::consume_launch_request(LaunchRequest& out){
 if(!impl_->launch_pending)return false;
 if(!impl_->selected_launch_profile_v50)return false;
 out={impl_->selected_launch_profile_v50->metadata.slot,
      impl_->selected_launch_profile_v50->requested_difficulty,
      std::move(impl_->selected_launch_profile_v50)};
 impl_->launch_pending=false;return true;
}
bool FrontUiSessionV87::touch(float x,float y,int action,std::string& error){
    if(!impl_->selected||!impl_->loaded||impl_->front_screen!="main"){error.clear();return true;}
    if(action<0||action>3||!std::isfinite(x)||!std::isfinite(y)){error="Malformed main menu touch";return false;}
    const auto rectangle=impl_->front_rectangle();
    if(!impl_->movie->input_rectangle(rectangle.data(),error))return false;
    // Cancellation must clear a held touch without producing onRelease.
    // Android DOWN/MOVE retain the source cursor button; UP clears it.
    auto* selected=impl_->active_menu_movie();
    struct Dispatch {Impl& self;ui::SwfMovie* previous;~Dispatch(){self.input_dispatch_movie=previous;}} dispatch{*impl_,impl_->input_dispatch_movie};
    impl_->input_dispatch_movie=selected;
    if(action==3)return selected->input_cancel(x,y,error);
    if(!selected->input_cursor({x,y,0.f,(action==0||action==2)?1:0},error))return false;
    // Run after the authored release returns; avoid nested movie evaluation.
    if(impl_->start_feedback_pending){
        impl_->start_feedback_pending=false;
        if(!selected->menu_action_script(impl_.get(),Impl::show_start_pending,error))return false;
    }
    return true;
}
std::string FrontUiSessionV87::consume_menu_sound(){
    auto& queue=impl_->menu_sounds;
    if(queue.empty())return {};
    auto value=std::move(queue.front());queue.pop_front();return value;
}
std::string FrontUiSessionV87::consume_menu_audio(){
    auto& queue=impl_->menu_audio;if(queue.empty())return {};auto value=std::move(queue.front());queue.pop_front();return value;
}
std::string FrontUiSessionV87::consume_menu_browser(){
    auto& queue=impl_->menu_browser;if(queue.empty())return {};
    auto value=std::move(queue.front());queue.pop_front();return value;
}
int FrontUiSessionV87::consume_menu_catalog(){
    auto& queue=impl_->menu_catalog;if(queue.empty())return -1;
    const auto language=queue.front();queue.pop_front();return language;
}
bool FrontUiSessionV87::consume_menu_exit(std::string& error){
    if(!impl_->exit_requested){error.clear();return false;}
    // Called on the GL owner after all ActionScript and frame work finished.
    if(!model_renderer::select_menu_persona(-1,impl_->manager,error))return false;
    impl_->menu_avatar={};impl_->menu_persona_mode=false;
    if(!impl_->reset_failed(error))return false;
    impl_->front_screen.clear();impl_->live_player=false;
    __android_log_print(ANDROID_LOG_INFO,tag,"Original front exit consumed | movie and preview released");
    return true;
}
bool FrontUiSessionV87::debug_menu_sound(const std::string& probe,std::string& error){
    if(!impl_->loaded||impl_->front_screen!="main"){error="Menu sound probe requires the retained main movie";return false;}
    struct Probe {const std::string& name;Impl* self;} context{probe,impl_.get()};
    return impl_->movie->action_script(&context,[](void* raw,ui::SwfAsGraph& graph,std::string& error){
        const auto& name=static_cast<Probe*>(raw)->name;
        ui::SwfAsValue root,receiver,result;bool callable=false;
        if(!graph.root_value(root,error))return false;
        if(name=="loading-panel-inspect"){
            auto& self=*static_cast<Probe*>(raw)->self;
            // Initial authored layout only. No Level fixture, progress tick,
            // multiplayer readiness, or completion is supplied by this probe.
            if(!Impl::push_menu(&self,"menu_Loading",error))return false;
            if(!self.shared_menu_movie->menu_action_script(nullptr,[](void*,ui::SwfAsGraph& graph,std::string& error){
                ui::SwfAsValue root,bar,result;bool callable=false;
                if(!graph.root_value(root,error)||!graph.find_target(root,"menu_Loading.loading_anim",bar,error))return false;
                if(!bar.identity()){error="Original loading artwork timeline absent";return false;}
                // Hold its authored first frame for layout inspection. A real
                // loader selects this timeline through BarState/onProgress.
                if(!graph.invoke(bar,bar,"gotoAndStop",{ui::SwfAsValue::number(1)},result,callable,error))return false;
                if(!callable){error="Loading artwork timeline control absent";return false;}
                return true;
            },error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original loading panel initial layout inspected | canonical progress not invoked");
            self.report_frame=true;return true;
        }
        if(name=="loading-tip-inspect"){
            if(!graph.invoke(root,root,"NativeGetLoadingTipStrID",{},result,callable,error)||!callable){if(error.empty())error="Loading tip callback absent";return false;}
            ui::SwfAsValue text_result;
            if(!graph.invoke(root,root,"NativeGetStringFromID",{result},text_result,callable,error)||!callable){if(error.empty())error="String ID callback absent";return false;}
            std::string text;if(!graph.to_text(text_result,text,error))return false;
            __android_log_print(ANDROID_LOG_INFO,tag,"Original loading tip localized | %s",text.c_str());return true;
        }
        if(name=="music-inspect"){
            static_cast<Probe*>(raw)->self->menu_audio.emplace_back("music-inspect");return true;
        }
        if(name=="music-pause"||name=="music-stop"||name=="music-title"||name=="music-invalid"){
            const char* method=name=="music-pause"?"NativePauseMusic":name=="music-stop"?"NativeStopMusic":"NativePlayMusic";
            std::vector<ui::SwfAsValue> args;
            if(name=="music-title")args.push_back(ui::SwfAsValue::text("TitleMusic"));
            if(name=="music-invalid")args.push_back(ui::SwfAsValue::text("MissingOriginalMusic"));
            if(!graph.invoke(root,root,method,args,result,callable,error)||!callable){if(error.empty())error="Required music callback absent";return false;}
            return true;
        }
        if(name=="create-menu-persona"){
            auto& self=*static_cast<Probe*>(raw)->self;
            std::uint32_t slot{};
            if(!self.create_menu_persona("Adam","KnightPlayerBase",slot,error))return false;
            if(!graph.find_target(root,"menu_MainMenu",receiver,error))return false;
            if(!graph.invoke(receiver,receiver,"OnShow",{},result,callable,error))return false;
            if(!callable){error="Authored main OnShow unavailable";return false;}return true;
        }
        if(name=="save-fixture-offline-mode"){
            auto& self=*static_cast<Probe*>(raw)->self;
            self.campaign_quests={nullptr,[](void*,bool& value,std::string& error){value=false;error.clear();return true;}};
            __android_log_print(ANDROID_LOG_INFO,tag,"Explicit offline campaign fixture service bound | test only; canonical online owner remains unavailable");
            if(!graph.find_target(root,"menu_MainMenu",receiver,error))return false;
            if(!graph.invoke(receiver,receiver,"OnShow",{},result,callable,error))return false;
            if(!callable){error="Authored main OnShow unavailable";return false;}return true;
        }
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
void FrontUiSessionV87::bind_menu_device(const std::string& manufacturer,const std::string& model,std::uint32_t driver_type){
    // Original DungeonHunter2.Get_PhoneManufacturer/Get_PhoneModel use
    // case-sensitive String.equals on Android Build.MANUFACTURER/MODEL.
    // appInit maps manufacturer0=HTC,2=SHARP and model99=SHW-M130L.
    impl_->menu_device={std::uint8_t(manufacturer=="SHARP"),std::uint8_t(manufacturer=="HTC"),std::uint8_t(model=="SHW-M130L"),0,driver_type};
    impl_->menu_device_written_v4=true;
    __android_log_print(ANDROID_LOG_INFO,tag,"Original menu device facts | manufacturer %s | model %s | driver type %u | sharp %u | htc %u | no IGP %u",manufacturer.c_str(),model.c_str(),driver_type,impl_->menu_device.sharp,impl_->menu_device.htc,impl_->menu_device.no_igp);
}
bool FrontUiSessionV87::menu_device_borrow_v4(ui::MenuDeviceFactsV1& out,std::string& error) const {
    if(!impl_||!impl_->menu_device_written_v4){error="Required actual Android Build and active driver facts";return false;}
    out=impl_->menu_device;error.clear();return true;
}
bool FrontUiSessionV87::menu_device_cells_v98(std::shared_ptr<void>& owner,const std::uint8_t*& htc,const std::uint8_t*& no_igp,std::string& error) const {
    owner.reset();htc=nullptr;no_igp=nullptr;
    if(!impl_||!impl_->menu_device_written_v4){error="Required actual Android Build device cells";return false;}
    owner=impl_;htc=&impl_->menu_device.htc;no_igp=&impl_->menu_device.no_igp;error.clear();return true;
}
bool FrontUiSessionV87::initialize(AAssetManager* manager,std::string& error) {
    try{impl_->manager=manager;impl_->assets.manager(manager);impl_->gpu.initialize(manager,resources::ResourceScopeV37::swf_front);impl_->report_frame=true;error.clear();return true;}
    catch(const std::exception& e){error=e.what();impl_->selected=false;return false;}
}
bool FrontUiSessionV87::bind_process_text_v101(ui::HudTextV1* text,std::shared_ptr<void> owner,std::string& error){
    if(!text||!owner){error="Required retained actual process StringManager borrower";return false;}
    if(impl_->process_text_v101&&(impl_->process_text_v101!=text||
       impl_->process_text_owner_v101.owner_before(owner)||owner.owner_before(impl_->process_text_owner_v101))){
        error="Front already borrows a different process StringManager";return false;
    }
    impl_->process_text_v101=text;impl_->process_text_owner_v101=std::move(owner);error.clear();return true;
}
bool FrontUiSessionV87::bind_process_font_cache_reset_v119(std::function<bool(std::string&)> reset,std::string& error){
    if(!impl_||!reset){error="Required actual process MenuManager::ResetFonts service";return false;}
    impl_->process_font_cache_reset_v119=std::move(reset);error.clear();return true;
}
bool FrontUiSessionV87::prepare_process_settings_v105(
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,const std::string& directory,
 std::function<bool(std::uint32_t&,std::string&)> platform,std::string& error){
 auto& self=*impl_;
 if(!app||!self.manager||directory.empty()||directory.front()!='/'||!platform||!self.process_text_v101){
  error="Required actual App/private files/platform language/process StringManager before GSInit1";return false;
 }
 if(self.process_settings_v105){
  if(self.directory!=directory||app->source_settings4c_v67()!=self.settings||
     self.process_application_v114.lock()!=app){error="Process settings authority/directory changed";return false;}
  error.clear();return true;
 }
 if(self.settings||app->source_settings4c_v67()){error="Process startup must adopt an already-existing settings owner explicitly";return false;}
 self.settings=std::make_shared<ui::OwnedHudSettingsV1>(); //Actual cold C1, no Array-ready receipt.
 if(!app->publish_source_settings4c_v67(self.settings,error))return false;
 self.process_application_v114=app;
 self.campaign_quests={&self,Impl::menu_use_volatile_quest_acts};
 self.directory=directory;self.front_screen="main";self.live_player=false;
 self.settings_files=std::make_unique<ui::SettingsNativeFilesV1>(directory);
 self.process_platform_language_v105=std::move(platform);self.process_settings_v105=true;error.clear();return true;
}
bool FrontUiSessionV87::load_process_settings_v105(bool language_only,std::string& error){
 auto& self=*impl_;
 if(!self.process_settings_v105||!self.settings||!self.settings_files||!self.menu_device_written_v4){
  error="Required actual process settings/files/device owner at GSInit1/4";return false;
 }
 if(!language_only&&!self.process_option_table_v105){
  std::array<std::vector<std::uint8_t>,3> bytes;
  const char* files[]{"design_pyarray.bin","design_pyarraynames.bin","design_pystructnames.bin"};
  for(unsigned i=0;i<3;++i)if(!self.raw_asset((std::string("original-cache/data/pydata/")+files[i]).c_str(),bytes[i],error))return false;
  if(!self.option_table.load_design_cache({bytes[0].data(),bytes[0].size()},{bytes[1].data(),bytes[1].size()},{bytes[2].data(),bytes[2].size()},error)||
     !self.settings->admit_process_option_table_v105(self.option_table.borrow(),error))return false;
  self.process_option_table_v105=true;
 }
 ui::SettingsLoadReceiptV1 receipt;auto language=self.settings_language();auto io=self.settings_files->services();
 ui::SettingsDeviceFactsV1 device;device.sharp_devices=self.menu_device.sharp;device.htc_devices=self.menu_device.htc;
 device.no_igp=self.menu_device.no_igp;device.in_multiplayer_mode=self.menu_device.multiplayer_mode;
 return self.settings->load(language_only,io,language,device,receipt,error);
}
void FrontUiSessionV87::bind_campaign_language_scene_v109(std::weak_ptr<void> world,std::function<bool(std::string&)> scene){
 impl_->campaign_language_world_v109=std::move(world);impl_->campaign_language_scene_v109=std::move(scene);
}
bool FrontUiSessionV87::set_process_language_v109(std::int32_t language,std::string& error){
 if(!impl_->process_settings_v105||!impl_->settings){error="Required SAME process settings for native SetLanguage";return false;}
 auto services=impl_->settings_language();return impl_->settings->set_language(language,services,error);
}
bool FrontUiSessionV87::bind_profile_application_v114(
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,std::string& e){
 auto& self=*impl_;const auto files=app?app->source_save_files_v61():nullptr;
 if(!app||!self.settings||app->source_settings4c_v67()!=self.settings||!files||
    !files->belongs_to_application(app)||!files->matches_directory(self.directory)){
  e="Front profile launch requires its actual published settings/FileManager";return false;
 }
 if(auto previous=self.process_application_v114.lock();previous&&previous!=app){
  e="Front profile launch belongs to another process Application";return false;
 }
 //Process audio initialization belongs solely to GSInit9. Binding the actual
 //profile owner must not replay that phase on initial or returning Front.
 self.process_application_v114=app;
 self.campaign_quests={&self,Impl::menu_use_volatile_quest_acts};e.clear();return true;
}
bool FrontUiSessionV87::reread_selected_profile_v101(
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,std::int32_t slot,std::int32_t difficulty,
 std::shared_ptr<const FrontSelectedProfileV50>& out,std::string& error){
 return impl_->reread_profile_v114(app,slot,difficulty,out,error);
}
bool FrontUiSessionV87::Impl::reread_profile_v114(
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,std::int32_t slot,std::int32_t difficulty,
 std::shared_ptr<const FrontSelectedProfileV50>& out,std::string& error){
 out.reset();auto& self=*this;
 if(!app||slot<0||slot>3||self.directory.empty()||!self.settings||
    app->source_settings4c_v67()!=self.settings){error="Required SAME process settings and actual selected slot for Continue";return false;}
 const auto files=app->source_save_files_v61();
 if(!files||!files->belongs_to_application(app)||!files->matches_directory(self.directory)){
  error="Required SAME process FileManager/jobs/private directory for Continue";return false;
 }
 char filename[32]{};std::snprintf(filename,sizeof(filename),"dh2_%03d.savegame",slot);
 bool found{};data::CampaignProfileFileV1 profile;
 //Read the real file AFTER the caller's unload/save flush. read_save itself
 //also executes source openSavefile's matching-job flush on this SAME owner.
 if(!files->read_save(filename,found,profile.bytes,error))return false;
 const bool corrupt=profile.bytes.size()>3&&profile.bytes[0]==255&&profile.bytes[1]==255&&profile.bytes[2]==255&&profile.bytes[3]==255;
 if(!found||profile.bytes.size()<=3||corrupt){
  if(!files->read_save(std::string(filename)+".bak",found,profile.bytes,error))return false;
  profile.origin=data::CampaignProfileOriginV1::backup;
 }
 if(!found){error="Actual selected Continue profile and backup are absent";return false;}
 return project_profile_v114(app,slot,difficulty,std::move(profile),out,error);
}
bool FrontUiSessionV87::project_selected_profile_file_v114(
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,std::int32_t slot,std::int32_t difficulty,
 data::CampaignProfileFileV1 profile,std::shared_ptr<const FrontSelectedProfileV50>& out,std::string& error){
 return impl_->project_profile_v114(app,slot,difficulty,std::move(profile),out,error);
}
bool FrontUiSessionV87::Impl::project_profile_v114(
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,std::int32_t slot,std::int32_t difficulty,
 data::CampaignProfileFileV1 profile,std::shared_ptr<const FrontSelectedProfileV50>& out,std::string& error){
 out.reset();auto& self=*this;
 const auto files=app?app->source_save_files_v61():nullptr;
 if(!app||slot<0||slot>3||self.directory.empty()||!self.settings||
    app->source_settings4c_v67()!=self.settings||!files||
    !files->belongs_to_application(app)||!files->matches_directory(self.directory)){
  error="Required SAME process settings/files for serialized profile projection";return false;
 }
 // The caller supplies real serialized bytes. Metadata uses the existing
 // class/quest/difficulty readers while the old campaign is still available;
 // this receipt itself retains only independent file bytes and metadata.
 data::MenuProfileMetadataV1 metadata;
 if(!data::load_menu_profile_metadata_v1({profile.bytes.data(),profile.bytes.size()},self.menu_characters,
    static_cast<std::uint32_t>(slot),difficulty,{&self,Impl::menu_store_difficulty,Impl::menu_load_quest_acts_v59},metadata,error))return false;
 if(metadata.character_row<0||std::size_t(metadata.character_row)>=self.menu_characters.names.size()){
  error="Actual refreshed Continue profile has no valid original class";return false;
 }
 return retain_front_selected_profile_v50(std::move(profile),std::move(metadata),self.directory,difficulty,out,error);
}
bool FrontUiSessionV87::show_game_loading(std::string& error){
    if(!impl_->loaded||impl_->front_screen!="main"){
        error="Original gameplay loading requires the retained front movies";return false;
    }
    if(!impl_->loading_state_services.current_level){
        error="Original gameplay loading requires its retained Application/Level provider";return false;
    }
    if(!Impl::push_menu(impl_.get(),"menu_Loading",error))return false;
    // GSLevel::Ctor 0x38624c..0x386280 pushes the authored menu, then
    // invokes onProgress. Level::_LoadProcess 0x3f6ef8..0x3f6f38 repeats
    // that invocation after updating the actual Level progress word.
    return refresh_game_loading(error);
}
bool FrontUiSessionV87::render_game_loading(int width,int height,std::string& error){
    // `loaded` tracks primary slot 2 as well as the front session, and the
    // actual MenuManager may unload that menu while source loading continues.
    // The authored loading clip lives in retained base slot 0, which is the
    // only movie admitted by render_source_movies_v93 during this phase.
    if(impl_->front_screen!="main"||!impl_->shared_menu_movie||impl_->base_deleted_v93||
       !impl_->shared_menu_movie->player_identity()||!impl_->source_menu_owner_v93||
       !impl_->source_draw_v93||impl_->source_transport_busy_v93){
        error="Source loading render requires the retained base movie and SAME MenuManager";return false;
    }
    const auto phase=front_loading_render_phase_v1(
        model_renderer::source_campaign_active_v55(),model_renderer::source_campaign_scene_active_v67());
    if(phase!=FrontLoadingRenderPhaseV1::source_loading){
        error="Source loading render requested outside the actual campaign loading interval";return false;
    }
    // Re-enter only for the live loading interval. The actual MenuManager
    // draw list supplies menu_Loading and its current onProgress state.
    // render_source_movies_v93 admits only base slot 0 here, so the retired
    // menu preview scene can never be submitted behind the loading panel.
    impl_->selected=true;
    impl_->source_loading_render_v114=true;
    struct Restore {bool& value;~Restore(){value=false;}} restore{impl_->source_loading_render_v114};
    return render(width,height,error);
}
bool FrontUiSessionV87::refresh_game_loading(std::string& error){
    if(!impl_->loaded||impl_->menu_stack.empty()||impl_->menu_stack.back()!="menu_Loading"){
        error="Original gameplay loading panel is not active";return false;
    }
    return impl_->shared_menu_movie->menu_action_script(nullptr,[](void*,ui::SwfAsGraph& graph,std::string& error){
        ui::SwfAsValue root,menu,result;bool callable=false;
        if(!graph.root_value(root,error)||!graph.find_target(root,"menu_Loading",menu,error))return false;
        if(!menu.identity()){error="Original shared loading clip absent";return false;}
        if(!graph.invoke(menu,menu,"onProgress",{},result,callable,error))return false;
        if(!callable){error="Original shared loading onProgress absent";return false;}
        return true;
    },error);
}
void FrontUiSessionV87::bind_loading_multiplayer_services(const ui::LoadingMenuMultiplayerServicesV1& services){impl_->loading_multiplayer_services=services;}
bool FrontUiSessionV87::finish_loading_fs_v114(const char* command,const char* args,bool& handled,std::string& error){
 handled=false;auto owner=impl_;if(!owner){error="Required SAME retained front LoadingMenu services";return false;}
 ui::MenuEndLoadingResultV114 result;
 if(!ui::menu_fs_end_loading_v114(command,args,nullptr,owner->loading_state_services,result,error))return false;
 handled=result.native_return!=0;return true;
}
bool FrontUiSessionV87::bind_loading_state_services(const ui::LoadingMenuStateServicesV1& services,
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,std::string& e){
 auto actual=impl_;auto registered=actual?actual->process_application_v114.lock():nullptr;
 if(!actual||!app||(registered&&registered!=app)||!services.context||!services.context_owner||services.context_owner.get()!=services.context||
    !services.current_level||!services.level_progress||!services.level_state||!services.advance_level_state){
  e="Loading callbacks require SAME process App and independently retained GS-global bridge";return false;
 }
 const auto online=app->get_online_loading_v55();
 if(!online){e="Loading queries require actual process COnline";return false;}
 actual->loading_multiplayer_services=application::source_online_loading_menu_v135(online);
 actual->loading_state_services=services;actual->main_menu_state_active_v114_=false;e.clear();return true;
}
void FrontUiSessionV87::bind_campaign_quest_services(const FrontCampaignQuestServicesV87& services){impl_->campaign_quests=services;}
void FrontUiSessionV87::bind_menu_avatar_services(const ui::MenuAvatarPreviewServicesV1& services){impl_->menu_avatar_services=services;}
bool FrontUiSessionV87::borrow_menu_avatar_state_v121(ui::MenuAvatarPreviewStateV1*& state,std::shared_ptr<void>& owner,std::string& error){
 state=nullptr;owner.reset();
 if(!impl_||!impl_->manager){error="Required actual retained Front avatar state";return false;}
 state=&impl_->menu_avatar;owner=impl_;error.clear();return true;
}
bool FrontUiSessionV87::menu_save_exists_v121(std::int32_t slot,bool& exists,std::string& error){
 exists=false;if(!impl_||slot<0||slot>=4){error="Invalid source preview save slot";return false;}
 return Impl::menu_slot_exists(impl_.get(),static_cast<std::uint32_t>(slot),exists,error);
}
bool FrontUiSessionV87::bind_menu_avatar_services_v121(ui::MenuAvatarPreviewServicesV1 services,std::shared_ptr<void> owner,std::string& error){
 if(!impl_||!owner||!services.context||!services.destroy_character||!services.setup_character||!services.create_avatar_camera){
  error="Required complete retained source menu avatar service transport";return false;
 }
 impl_->menu_avatar_services=services;impl_->menu_avatar_services_owner_v121=std::move(owner);error.clear();return true;
}
bool FrontUiSessionV87::process_class_select_show_v87(std::uintptr_t render,std::string& error){
 auto self=impl_;ui::MenuMovieBorrowV58 actual;
 if(!self||!render||self->front_screen!="main"||!self->movie||
    !movie_slot_v93(2,actual,error)||actual.identity!=render){
  if(error.empty())error="MenuCharacterSelect.Show requires the same live primary2 RenderFX";
  return false;
 }
 self->class_index=0;
 self->class_applied_index=-1;
 self->class_left=self->class_right=0;
 self->process_class_select_active_v87=false;
 struct Show {Impl& self;};Show call{*self};
 if(!self->movie->menu_action_script(&call,[](void* raw,ui::SwfAsGraph& graph,std::string& e){
  auto& q=*static_cast<Show*>(raw);
  // Native MenuCharacterSelect::Show has already delivered MenuBase::Show.
  // Preserve the retained Front implementation's actual class text, arrows,
  // CurrentClass callback and display-pane registration on this movie.
  if(!Impl::update_class(&q.self,graph,e)||
     !q.self.movie->menu_display_callback("_root.menu_SelectClass.class_select",&q.self,Impl::class_pane,e))return false;
  q.self.process_class_select_active_v87=true;
  return true;
 },error))return false;
 error.clear();return true;
}
bool FrontUiSessionV87::process_class_select_update_v87(std::uintptr_t render,std::string& error){
 auto self=impl_;ui::MenuMovieBorrowV58 actual;
 if(!self||!render||self->front_screen!="main"||!self->movie||
    !movie_slot_v93(2,actual,error)||actual.identity!=render){
  if(error.empty())error="MenuCharacterSelect.Update requires the same live primary2 RenderFX";
  return false;
 }
 if(!self->process_class_select_active_v87||self->class_applied_index==self->class_index){error.clear();return true;}
 // Native Update only formats class text/description, publishes CurrentClass,
 // toggles cached arrows and begins the transition when its two indices differ.
 if(!model_renderer::select_class_scene(self->class_index,0,error))return false;
 return self->movie->menu_action_script(self.get(),Impl::update_class,error);
}
bool FrontUiSessionV87::process_class_select_hide_v87(std::uintptr_t render,std::string& error){
 auto self=impl_;ui::MenuMovieBorrowV58 actual;
 if(!self||!render||!movie_slot_v93(2,actual,error)||actual.identity!=render){
  if(error.empty())error="MenuCharacterSelect.Hide requires the same live primary2 RenderFX";
  return false;
 }
 self->process_class_select_active_v87=false;self->class_left=self->class_right=0;
 self->class_applied_index=-1;error.clear();return true;
}
bool FrontUiSessionV87::bind_main_menu_state_services_v114(ui::GSFlashMenuServicesV114 services,std::string& e){
 auto actual=impl_;if(!actual||!services.owner||!services.current){e="Required independent SAME Front state services";return false;}
 if(!services.owner.owner_before(actual)&&!actual.owner_before(services.owner)){e="Front services cannot own containing Front Impl";return false;}
 if(actual->main_menu_services_v114.owner){e="Retained process GSFlashMenu services already enrolled";return false;}
 actual->main_menu_services_v114=std::move(services);e.clear();return true;
}
bool FrontUiSessionV87::borrow_main_menu_state_v114(loader::MainMenuStateBorrowV114& out,std::string& e){
 out={};auto actual=impl_;if(!actual){e="Required actual retained Front Impl";return false;}
 auto& fields=actual->main_menu_state_v114;
 out.owner=actual;out.identity=reinterpret_cast<std::uintptr_t>(&fields);
 out.menu_c=&fields.request_c;out.source29=&fields.source29;e.clear();return true;
}
bool FrontUiSessionV87::enter_main_menu_state_v114(const loader::MainMenuStateBorrowV114& expected,std::string& e){
 auto actual=impl_;if(!actual||expected.owner.get()!=actual.get()||expected.owner.owner_before(actual)||actual.owner_before(expected.owner)||
  expected.identity!=reinterpret_cast<std::uintptr_t>(&actual->main_menu_state_v114)||expected.menu_c!=&actual->main_menu_state_v114.request_c||expected.source29!=&actual->main_menu_state_v114.source29){
  e="Replaced actual GSFlashMenu state/cell owner";return false;}
 // No reset_failed, movie recreation, menu_Loading-ready synthesis or World
 // capture. Ctor runs on the SAME process Front cells and native providers.
 if(!actual->main_menu_entry_v114.enter(actual->main_menu_state_v114,actual->main_menu_services_v114,e))return false;
 actual->main_menu_state_active_v114_=true;
 actual->selected=true;actual->live_player=false;actual->report_frame=true;e.clear();return true;
}
bool FrontUiSessionV87::arm_main_menu_state_resume_v114(std::string& e){
 auto actual=impl_;if(!actual){e="No retained Front entry to resume";return false;}
 return actual->main_menu_entry_v114.arm_resume(e);
}
bool FrontUiSessionV87::resume_main_menu_state_v114(std::string& e){
 bool completed{};return resume_main_menu_state_v114(completed,e);
}
bool FrontUiSessionV87::resume_main_menu_state_v114(bool& completed_entry,std::string& e){
 completed_entry=false;
 auto actual=impl_;if(!actual){e="No retained actual Front state";return false;}
 if(actual->main_menu_entry_v114.failure().empty()){e.clear();return true;}
 if(!actual->main_menu_entry_v114.resume(actual->main_menu_state_v114,actual->main_menu_services_v114,e))return false;
 actual->main_menu_state_active_v114_=true;
 actual->selected=true;actual->live_player=false;actual->report_frame=true;
 completed_entry=true;e.clear();return true;
}
bool FrontUiSessionV87::main_menu_state_active_v114()const{return impl_&&impl_->main_menu_state_active_v114_;}
bool FrontUiSessionV87::update_main_menu_request_v114(std::string& e){
 auto actual=impl_;if(!actual){e="No retained actual Front state";return false;}
 if(actual->main_menu_request_busy_v114){if(actual->main_menu_request_failure_v114.empty())actual->main_menu_request_failure_v114="Recursive GSFlashMenu request delivery";e=actual->main_menu_request_failure_v114;return false;}
 if(!actual->main_menu_request_failure_v114.empty()){e=actual->main_menu_request_failure_v114;return false;}
 actual->main_menu_request_busy_v114=true;struct Busy{bool& b;~Busy(){b=false;}} busy{actual->main_menu_request_busy_v114};
 auto services=actual->main_menu_services_v114;const auto current=services.current;
 services.current=[actual,current](auto& x){if(!actual->main_menu_request_failure_v114.empty()){x=actual->main_menu_request_failure_v114;return false;}return current&&current(x);};
 std::string local;const bool ok=ui::gs_flash_menu_request_v114(actual->main_menu_state_v114,services,local);
 if(!actual->main_menu_request_failure_v114.empty()){e=actual->main_menu_request_failure_v114;return false;}
 if(!ok){actual->main_menu_request_failure_v114=local.empty()?"Actual GSFlashMenu request failed":local;e=actual->main_menu_request_failure_v114;return false;}e.clear();return true;
}
bool FrontUiSessionV87::load_front_screen(const std::string& directory,const std::string& screen,std::string& error) {
    if(screen!="main"&&screen!="loading"){error="Unknown authored front screen";return false;}
    if(directory.empty()||directory.front()!='/'){error="Required private UI files directory unavailable";return false;}
    if(impl_->loaded&&impl_->front_screen!=screen){if(!impl_->reset_failed(error))return false;}
    impl_->front_screen=screen;impl_->directory=directory;impl_->live_player=false;
    impl_->selected=true;impl_->report_frame=true;impl_->frame_time=std::chrono::steady_clock::now();error.clear();return true;
}
bool FrontUiSessionV87::prepare_front_resources_v114(int width,int height,std::string& error) {
    if(impl_->front_screen.empty()||!impl_->selected||width<=1||height<=1){
        error="Front resource loading requires its selected screen and resized surface";return false;
    }
    if(impl_->loaded){error.clear();return true;}
    impl_->driver_width=width;impl_->driver_height=height;
    return impl_->load(error);
}
bool FrontUiSessionV87::prepare_process_front_v119(const std::string& directory,int width,int height,std::string& e){
 if(width<=1||height<=1||!impl_->process_settings_v105||!impl_->settings||impl_->directory!=directory){e="Required actual resized process Front/settings context";return false;}
 // The retained main SWF resolves its authored splash bitmap while it is
 // imported. Select the same GSInit language/device image before that import
 // so both the startup quad and menu_splash share one cached texture identity.
 impl_->driver_width=width;impl_->driver_height=height;
 impl_->splash_source_uri_v1=ui::splash_source_uri_v1(width,impl_->settings->language());
 if(!load_front_screen(directory,"main",e))return false;
 e.clear();return true;
}
bool FrontUiSessionV87::load_process_splash_v119(std::int32_t language,std::string& e){
 //GSInit7 selects the exact shipping texture before assigning its intrusive
 //texture field. The retained texture cache owns the actual upload/lifetime.
 const auto selected=ui::splash_source_uri_v1(impl_->driver_width,language);
 if(!impl_->splash_source_uri_v1.empty()&&impl_->splash_source_uri_v1!=selected){
  e="GSInit splash selection changed after the retained main SWF imported its authored bitmap";return false;
 }
 impl_->splash_source_uri_v1=selected;
 const char* uri=impl_->splash_source_uri_v1.c_str()+5; // texture receives paths relative to data/.
 ui::SwfTexture texture;if(!Impl::texture(impl_.get(),uri,0,0,texture,e))return false;
 impl_->process_splash_v119=texture;impl_->process_splash_uri_v119=uri;e.clear();return true;
}
bool FrontUiSessionV87::render_process_splash_v119(int width,int height,std::string& e){
 if(!impl_->process_splash_uri_v119.empty()&&!Impl::texture(impl_.get(),impl_->process_splash_uri_v119.c_str(),0,0,impl_->process_splash_v119,e))return false;
 const auto& texture=impl_->process_splash_v119;
 if(!texture.width||!texture.height){e.clear();return true;} //Actual NULL GSInit10 texture before stage7.
 if(width<=1||height<=1||texture.width<1280||texture.height<752){e="Invalid original GSInit splash rectangle/surface";return false;}
 const auto viewport=ui::splash_full_viewport_v1(width,height);
 ui::SwfDraw draw;draw.kind=ui::SwfDraw::begin;draw.bounds[1]=1280;draw.bounds[3]=752;
 draw.viewport[0]=viewport.x;draw.viewport[1]=viewport.y;draw.viewport[2]=viewport.width;draw.viewport[3]=viewport.height;
 if(!impl_->gpu.draw(draw,e))return false;
 draw.kind=ui::SwfDraw::bitmap_quad;draw.fill.kind=ui::SwfFill::bitmap;draw.fill.texture=texture;
 draw.rect[1]=1280;draw.rect[3]=752;draw.uv_rect[1]=1280.f/texture.width;draw.uv_rect[3]=752.f/texture.height;
 if(!impl_->gpu.draw(draw,e)){impl_->gpu.abort();return false;}
 draw.kind=ui::SwfDraw::end;return impl_->gpu.draw(draw,e);
}
bool FrontUiSessionV87::load_process_property_names_v119(std::string& e){
 if(!impl_->debug_load(e))return false;std::uint32_t ignored{};
 if(dh2_character_debug_get(&ignored,impl_->debug.get(),"isTracingCharProperties",&impl_->debug_files)<0){e="CharProperties.LoadPropNames Debug failed";return false;}
 std::vector<std::uint8_t> bytes;if(!impl_->raw_asset("data/character_properties_pystructnames.bin",bytes,e))return false;
 std::size_t at{};auto word=[&](std::uint32_t& out){if(at>bytes.size()||bytes.size()-at<4)return false;out=0;for(unsigned i=0;i<4;++i)out|=std::uint32_t(bytes[at++])<<(8*i);return true;};
 std::uint32_t count{};if(!word(count)||count>bytes.size()/4){e="Malformed actual property-name count";return false;}
 //The source appends each CString as it is read; failure preserves that prefix.
 for(std::uint32_t i=0;i<count;++i){std::uint32_t n{};if(!word(n)||n>bytes.size()-at){e="Truncated actual property-name CString";return false;}
  impl_->process_property_names_v119.emplace_back(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;
 }e.clear();return true;
}
void FrontUiSessionV87::unload_process_property_names_v119(){std::vector<std::string>().swap(impl_->process_property_names_v119);}
bool FrontUiSessionV87::clear_process_splash_v119(std::string& e){
 // The original GSInit field releases its texture reference here. The menu
 // atlas can retain that SAME selected language/device texture. Our export cache owns
 // GPU images for the session, and Bitmap retains their identities until the
 // movie is unloaded. Retire the cache/GPU owner only in reset_images, after
 // those movie references have been released.
 impl_->process_splash_v119={};impl_->process_splash_uri_v119.clear();e.clear();return true;
}
bool FrontUiSessionV87::save_process_settings_v119(std::string& e){return Impl::save_settings(impl_.get(),e);}
void FrontUiSessionV87::resume_process_music_v119(){impl_->menu_audio.emplace_back("resume");}
bool FrontUiSessionV87::load_health_panel(const std::string& directory,std::string& error) {
    if(!impl_->front_screen.empty()){if(!impl_->reset_failed(error))return false;impl_->front_screen.clear();}
    impl_->selected=false;
    impl_->live_player=false;
    if(directory.empty()||directory.front()!='/'){error="Required private UI files directory unavailable";return false;}
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
bool FrontUiSessionV87::render(int width,int height,std::string& error) {
    if(impl_->source_menu_owner_v93)return render_source_movies_v93(width,height,error);
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
            if(inactive&&!inactive->advance_frames(milliseconds,*inactive_owner->frames,error))return false;
            struct Dispatch {Impl& self;ui::SwfMovie* previous;~Dispatch(){self.input_dispatch_movie=previous;}} dispatch{*impl_,impl_->input_dispatch_movie};
            impl_->input_dispatch_movie=selected;
            if(!selected->input_advance(milliseconds,error))return false;
        }else if(!impl_->movie->advance(seconds,error))return false;
    }
    if(impl_->front_screen=="loading"){
        // GSInit::Draw 0x384ab4..0x384af4 supplies source rect 0,0,1280,752
        // separately from the animated loading overlay. Map that full authored
        // rectangle to the complete display, with no letterbox bars or crop.
        ui::SwfTexture splash;
        if(!Impl::texture(impl_.get(),"menus/splash_final.tga",0,0,splash,error))return false;
        if(splash.width<1280||splash.height<752){error="Original loading splash source rectangle exceeds atlas";return false;}
        const auto viewport=ui::splash_full_viewport_v1(width,height);
        ui::SwfDraw background; background.kind=ui::SwfDraw::begin;
        background.bounds[1]=1280; background.bounds[3]=752;
        background.viewport[0]=viewport.x; background.viewport[1]=viewport.y;
        background.viewport[2]=viewport.width; background.viewport[3]=viewport.height;
        if(!impl_->gpu.draw(background,error))return false;
        background.kind=ui::SwfDraw::bitmap_quad;
        background.fill.kind=ui::SwfFill::bitmap; background.fill.texture=splash;
        background.rect[1]=1280; background.rect[3]=752;
        background.uv_rect[1]=1280.f/splash.width; background.uv_rect[3]=752.f/splash.height;
        if(!impl_->gpu.draw(background,error))return false;
        background.kind=ui::SwfDraw::end;
        if(!impl_->gpu.draw(background,error))return false;
        if(impl_->report_frame)__android_log_print(ANDROID_LOG_INFO,tag,"Original startup splash submitted | source rect 0 0 1280 752 | full display %d %d",viewport.width,viewport.height);
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
        const auto stage=full_surface_stage_v87(width,height);
        if(!impl_->movie->display_clip("_root.menu_bg",stage.x,stage.y,stage.width,stage.height,error))return false;
    }
    auto* display_movie=impl_->front_screen=="main"?impl_->active_menu_movie():impl_->movie.get();
    // Keep the authored Touch-to-Continue coordinates attached to the same
    // complete display transform as their startup image.
    const auto stage=full_surface_stage_v87(width,height);
    const bool previous_splash=impl_->rendering_splash_clip_v1;
    impl_->rendering_splash_clip_v1=std::strcmp(path,"_root.menu_splash")==0;
    struct SplashScope {bool& value;bool previous;~SplashScope(){value=previous;}} splash_scope{impl_->rendering_splash_clip_v1,previous_splash};
    if(!(front?display_movie->display_clip(path,
        stage.x,stage.y,
        stage.width,stage.height,error):impl_->movie->display_source_clip(path,error))){
        const auto failure=error;std::string cleanup;impl_->reset_failed(cleanup);
        error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
    }
    if(impl_->report_frame){
        if(impl_->front_screen=="main"&&!impl_->movie->action_script(impl_.get(),Impl::probe_main_background,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original front/HUD screen submitted | screen %s",impl_->front_screen.empty()?"health":impl_->front_screen.c_str());
        __android_log_print(ANDROID_LOG_INFO,tag,"Original health panel submitted | viewport %d %d | strips %u | lines %u | masks %u | font uploads %u | bitmaps %u | strings %u | core diagnostics %u | authored initial state | game updates/input unconnected",width,height,impl_->strips,impl_->lines,impl_->masks,impl_->glyph_uploads,impl_->bitmap_uploads,impl_->string_calls,impl_->core_errors);
        impl_->report_frame=false;
    }
    return true;
}
bool FrontUiSessionV87::active() const{return impl_->selected&&(impl_->loaded||!impl_->front_screen.empty());}
bool FrontUiSessionV87::overlays_player() const{return impl_->selected&&impl_->live_player;}
void FrontUiSessionV87::deactivate(){impl_->selected=false;}

bool FrontUiSessionV87::attach_player(const std::string& directory,std::string& error){
    if(!impl_->front_screen.empty()){if(!impl_->reset_failed(error))return false;impl_->front_screen.clear();}
    if(directory.empty()||directory.front()!='/'){error="Required private HUD directory unavailable";return false;}
    impl_->directory=directory;impl_->live_player=true;impl_->selected=true;impl_->report_frame=true;
    error.clear();return true;
}
bool FrontUiSessionV87::render_player(int width,int height,const std::int32_t* sheet,std::size_t count,
                                     std::uintptr_t character,std::string& error){
    if(!overlays_player()){error="Connected player HUD inactive";return false;}
    auto& self=*impl_;
    try{
        if(!self.loaded){self.driver_width=width;self.driver_height=height;if(!self.load(error))throw std::runtime_error(error);}
        if(!self.viewport(width,height,error)||!self.status->update(sheet,count,character,error)||
           !self.movie->display_source_clip(status_panel,error)||
           !self.movie->display_source_clip("_root.HurtCorners",error))throw std::runtime_error(error);
        const auto frames=self.status->frames();
        if(self.report_frame||frames!=self.reported_frames){
            __android_log_print(ANDROID_LOG_INFO,tag,
                "Connected player HUD submitted | viewport %d %d | character %p | HP %d %d | MP %d %d | XP %d %d | frames %d %d %d %d %d | dirty %zu | retained movie %p | original timeline and viewport",
                width,height,reinterpret_cast<void*>(character),sheet[36],sheet[38],sheet[41],sheet[43],sheet[33],sheet[34],
                frames[0],frames[1],frames[2],frames[3],frames[4],self.status->dirty_nodes(),static_cast<void*>(self.movie.get()));
            self.reported_frames=frames;self.report_frame=false;
        }
        return true;
    }catch(const std::exception& ex){
        const std::string failure=ex.what();std::string cleanup;self.reset_failed(cleanup);
        error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
    }
}
}
