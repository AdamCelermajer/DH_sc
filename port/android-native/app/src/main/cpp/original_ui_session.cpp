#include "original_ui_session.hpp"
#include "original_ui_assets.hpp"
#include "swf_gpu.hpp"
#include "swf_hud_freetype_provider.hpp"
#include "swf_text_font_platform_v1.hpp"
#include "gfnt_text_backend_v1.hpp"
#include "hud_freetype_font_v2.hpp"
#include "swf_font_resolver.hpp"
#include "localization.hpp"
#include "source_process_pydata_files_v101.hpp"
#include "source_process_arrays_v101.hpp"
#include "source_process_trophies_v100.hpp"
#include "hud_text_v1.hpp"
#include "progression_xp_text_v23.hpp"
#include "menu_status_movie_v26.hpp"
#include "menu_message_cache_v66.hpp"
#include "menu_dialog_messages_v97.hpp"
#include <quest_persistence_v51.hpp>
#include "source_script_ui_world_v97.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "source_settings_update_job_v102.hpp"
#include "source_campaign_items_v88.hpp"
#include "swf_anim_tooltip_v104.hpp"
#include "character_menu_font_palette_v1.hpp"
#include <source_assertion_process_v76.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <application_services_owner_v5.hpp>
#include "gameswf/gameswf_sprite.h"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include <level_destroy_source_v1.hpp>
#include "renderer_character_campaign_v62.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <player_xp_text_v1.hpp>
#include "source_campaign_script_execution_v96.hpp"
#include "character_design_services.hpp"
#include "script_constants.hpp"
#include "swf_texture.hpp"
#include "player_status_hud.hpp"
#include "combat_flash_swf_v1.hpp"
#include "combat_flash_inputs_v1.hpp"
#include "source_campaign_character_frame_v111.hpp"
#include "source_campaign_combat_v115.hpp"
#include <gameplay_camera_application_v23.hpp>
#include "flash_anim_manager_v92.hpp"
#include "authored_gameplay_hud_v1.hpp"
#include "authored_hurt_corners_layout_v7.hpp"
#include "authored_hurt_pulse_v7.hpp"
#include "authored_status_timeline_v27.hpp"
#include "authored_hud_portrait_v4.hpp"
#include "frame_perf_v35.hpp"
#include <optional>
#include "swf_menu_parsed_string_v1.hpp"
#include "authored_hud_options_bridge_v1.hpp"
#include "authored_joystick_v1.hpp"
#include "hud_manager_core.hpp"
#include "renderfx_text_connection.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_sound.h"
#include "authored_menu_character_projection_v4.hpp"
#include "math.hpp"
#include "character_menu_as_bridge_v1.hpp"
#include "character_panel_session_v1.hpp"
#include "gameplay_hud.hpp"
#include "textures.hpp"
#include <android/log.h>
#include <array>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <limits>
#include <map>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace dh2::android_ui {
namespace {
constexpr const char* tag="DH2Native";
constexpr const char* panel="_root.menu_HUD_0.HUDelements.HealthBars.player";
constexpr const char* hud_sha="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
struct AssetClose {void operator()(AAsset* value)const{if(value)AAsset_close(value);}};
struct ConstantsDelete {void operator()(dh2_script_constants* value)const{dh2_script_constants_destroy(value);}};
struct DebugDelete {void operator()(character::DebugSwitches* value)const{dh2_character_debug_destroy(value);}};
}
struct OriginalUiSession::Impl:std::enable_shared_from_this<Impl> {
    AAssetManager* manager{};
    OriginalUiAssets assets;
    SwfGpu gpu;
    std::string directory,font_failure,provider_failure;
    std::unique_ptr<dh2_script_constants,ConstantsDelete> constants{dh2_script_constants_create()};
    std::shared_ptr<character::DebugSwitches> debug{dh2_character_debug_create(),DebugDelete{}};
    character::DebugFileServices24 debug_files{this,debug_open,debug_close};
    ui::HudTextV1 localization;
    bool process_text_ready_v101{},process_text_attempted_v101{};
    std::string process_text_error_v101;
    std::uint32_t process_constants_stage20_v101{};
    bool process_constants_failed_v101{};
    std::string process_constants_error_v101;
    std::shared_ptr<SourceProcessArraysV101> process_arrays_v101{std::make_shared<SourceProcessArraysV101>()};
    std::shared_ptr<ui::MenuStatusMessagesV26> status_messages;
    ui::MenuMessageCachesV66 message_caches_v66;
#include "original_ui_item_tooltips_v104.inc"
#include "original_ui_messages_v97.inc"
    std::shared_ptr<ui::MenuStatusMessagesV26> message_owner(){
        if(status_messages)return status_messages;
        struct Provider {std::weak_ptr<Impl> ui;};auto held=std::make_shared<Provider>();held->ui=shared_from_this();
        ui::MenuStatusServicesV26 services;services.owner=held;
        services.hud_root=[held](std::uintptr_t& out,std::string& error){
            auto actual=held->ui.lock();if(!actual){error="Actual HUD status-message provider expired";return false;}
            out=0;if(!actual->loaded||!actual->movie){error.clear();return true;}
            return actual->movie->menu_action_script(&out,[](void* raw,ui::SwfAsGraph& graph,std::string& e){
                ui::SwfAsValue root;if(!graph.root_value(root,e))return false;
                *static_cast<std::uintptr_t*>(raw)=root.identity();return true;
            },error);
        };
        services.invoke=[held](std::uintptr_t receiver,const char* method,std::int32_t context,std::string& error){
            auto actual=held->ui.lock();if(!actual||!actual->loaded||!actual->movie){error="Required retained actual HUD status-message movie";return false;}
            struct Check {std::uintptr_t receiver;static bool run(void* raw,ui::SwfAsGraph& graph,std::string& e){
                ui::SwfAsValue root;if(!graph.root_value(root,e))return false;
                if(root.identity()!=static_cast<Check*>(raw)->receiver){e="Status message addressed a different actual HUD root";return false;}return true;
            }} check{receiver};
            if(!actual->movie->menu_action_script(&check,Check::run,error))return false;
            return actual->message_caches_v66.invoke(*actual->movie,ui::MessageFamilyV66::status,receiver,method,context,error);
        };
        status_messages=std::make_shared<ui::MenuStatusMessagesV26>(std::move(services));return status_messages;
    }
    std::map<std::uintptr_t,std::vector<std::uint8_t>> leases;
    std::uintptr_t next_lease=1;
    std::map<std::string,ui::SwfTexture> exports;
    bool selected=false,loaded=false,live_player=false,menu_string_flag=false,report_frame=true;
    int driver_width=480,driver_height=320;
    ui::FlashCamera40 camera{};
    std::shared_ptr<ui::MenuFlash2DCameraOwnerV93> menu_camera_v93;
    std::array<std::int32_t,5> reported_frames{{-1,-1,-1,-1,-1}};
    unsigned glyph_uploads=0,bitmap_uploads=0,string_calls=0,core_errors=0,packed_glyphs=0;
    unsigned strips=0,lines=0,masks=0;
    unsigned external_movies=0;
    int last_width=0,last_height=0;
    // Reverse destruction keeps every provider and owned texture alive until
    // the last movie and its reachable ActionScript graph have been released.
    std::unique_ptr<ui::SwfTextFontPlatformV1> fonts;
    std::shared_ptr<ui::SwfMovie> movie;
    bool hud_deleted_v94=false;
    std::shared_ptr<void> source_menu_owner_v94;
    std::unique_ptr<ui::PlayerStatusHud> status;
    ui::AuthoredHurtPulseV7 hurt_pulse;
    ui::AuthoredStatusTimelineV27 status_timeline;
    int hurt_reported_frame=-1,hurt_reported_outer=-1;
    std::unique_ptr<ui::EnemyStatusHudV1> enemy;
    std::shared_ptr<ui::CombatFlashSwfV1> combat_flash;
    std::weak_ptr<ui::FlashAnimManagerV92> flash_manager;
    std::unique_ptr<ui::AuthoredGameplayHudV1> gameplay;
    std::shared_ptr<std::int32_t> source_action_icon108_v62=std::make_shared<std::int32_t>(-1);
    std::unique_ptr<CharacterPanelSessionV1> gameplay_queries;
    std::unique_ptr<ui::CharacterMenuAsBridgeV1> gameplay_bridge;
    std::int32_t hud_style=0;
    bool gameplay_activated=false;
    std::array<std::int32_t,3> displayed_skill_ids{{-2,-2,-2}};
    std::map<int,ui::AuthoredHudControlV1> hud_pointers;
    // A gesture keeps its down-time owner even if it crosses a HUD control.
    // Coordinates remain raw scene viewport pixels throughout world dispatch.
    std::map<int,bool> world_pointers;
    ui::AuthoredJoystickStateV1 joystick;
    ui::HudAttackHeldFieldsV46 attack_controls_v46;
    ui::MenuInfoHudOwnerV62 source_info_hud_v62;
    std::unique_ptr<ui::HudManagerCore> source_hud_core_v62;
    ui::HudAdvanceOwner source_hud_advance_v62;
    std::weak_ptr<void> source_cached_target_lifetime_v107;
    std::uintptr_t source_cached_target_projection_v107{};
    bool source_cached_target_produced_v107{};
    std::shared_ptr<void> source_hud_transport_v62;
    std::weak_ptr<void> source_campaign_world_v104;
    bool source_campaign_detached_v104{};
    std::weak_ptr<void> source_script_world_v104;
    OriginalUiSession::HudWorldOperationV62 source_hud_world_v62;
    ui::HudControlsServicesV62 source_controls_v62;
    std::uintptr_t source_controls_root658_v62{}; //actual C1 store41b0c8
    std::uint8_t source_controls_cache8_v62{}; //actual C1 store41af38
    std::weak_ptr<events::EventManagerOwnerV12> source_controls_events_v62;
    bool source_controls_constructor_attempted_v62{},source_controls_constructed_v62{};
    ui::SwfAsGraph* source_hud_graph_v62{};std::string* source_hud_error_v62{};
#include "original_ui_postmovie_owner_v62.inc"
#include "original_ui_control_cache_v63.inc"
#include "original_ui_control_event_v68.inc"
    std::uint64_t held_dispatches_v46{};
    bool dispatch_attack_v46(std::string& error){
        if(!attack_controls_v46.held9)return true;
        const auto live=model_renderer::player_gameplay_binding();
        if(!gameplay_activated||!live.active||!live.life||live.life->dead){cancel_attack_v46();return true;}
        ui::HudAttackActorBorrowV46 actor;
        if(!model_renderer::player_hud_attack_actor_v46(0,false,actor,error))return false;
        if(!actor.character)return true;
        ui::HudAttackServicesV46 services;
        services.attack=[](void*,std::uintptr_t controller,std::uintptr_t target,std::string& e){return model_renderer::player_hud_attack_command_v46(controller,target,e);};
        if(!ui::hud_attack_dispatch_v46(attack_controls_v46,actor,services,error))return false;
        ++held_dispatches_v46;return true;
    }
    void cancel_attack_v46(){
        const bool was_held=attack_controls_v46.held9!=0;bool consumed{};std::string error;
        ui::hud_attack_event_v46(attack_controls_v46,7,consumed,error);
        if(was_held)__android_log_print(ANDROID_LOG_INFO,tag,"Source HUD held attack cancelled | command updates %llu",static_cast<unsigned long long>(held_dispatches_v46));
    }
    std::function<bool(std::int32_t&,std::string&)> platform_music;
    std::uint8_t source_korean_build=0; // Original BSS isKOREAN_BUILD9f640b.
    struct PendingText {
        std::string style,text;
        std::array<float,3> position{};
        std::int32_t number{},color{};bool numeric{};
    };
    std::vector<PendingText> pending_text;
    std::string borrowed_combat_string;
    unsigned reported_combat_active{};
    bool drawing_combat{},reported_combat_glyphs{};
    unsigned combat_quads{};
    std::array<float,2> first_combat_quad{};
    std::array<float,2> combat_quad_extent{};std::uint32_t combat_quad_rgba{};
    std::uintptr_t reported_enemy{};
    int reported_enemy_frame=-1;

    static bool combat_project(void*,const float p[3],std::int32_t* x,std::int32_t* y,std::string& error){
        return model_renderer::combat_text_project(p,x,y,error);
    }
    static bool combat_rectangle(void* context,float r[4],std::int32_t v[4],std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!self.movie){error="Combat text requires retained HUD movie";return false;}
        return self.movie->source_display_rectangle(r,v,error);
    }
    static int combat_localized(void* context,std::int32_t id,const char** out){
        auto& self=*static_cast<Impl*>(context);bool is_null{};std::string error;
        if(!out||!self.loaded||!integer_string(context,id,self.borrowed_combat_string,is_null,error)||is_null){
            __android_log_print(ANDROID_LOG_ERROR,tag,"Combat text localization failed | id %d | %s",id,error.c_str());return 1;
        }
        *out=self.borrowed_combat_string.c_str();return 0;
    }
    static bool combat_format_integer(void* context,const char* format,std::int32_t amount,std::string& out,std::string& error){
        auto& self=*static_cast<Impl*>(context);const auto live=model_renderer::player_gameplay_binding();
        if(!format||!self.loaded||!live.active||!live.world_owner){error="XP formatting requires the actual HUD/World StringManager";return false;}
        auto environment=live.text_environment;environment.localization=self.text_services();
        return ui::progression_xp_format_v23(self.localization,environment,format,amount,out,error);
    }
    static int combat_enqueue(void* context,const character::skills::CombatTextRequestV1* request){
        auto& self=*static_cast<Impl*>(context);
        if(!request||!request->style||(!request->numeric&&!request->text)||!self.loaded||!self.combat_flash)return 1;
        PendingText copy;copy.style=request->style;if(request->text)copy.text=request->text;
        std::copy_n(request->position,3,copy.position.begin());
        copy.number=request->number;copy.color=request->color;copy.numeric=request->numeric;
        self.pending_text.push_back(std::move(copy));return 0;
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
        const std::int32_t bounds[]{0,0,width,height};
        // Modern HUD policy uses the recovered aspect-fit bounds path. Drawing,
        // global Viewport publication and pointer conversion share these bounds.
        if(!movie->set_source_bounds(bounds,2,error))return false;
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
        const int status=dh2_character_debug_load(debug.get(),&debug_files);
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
        return {this,text_open,text_close,localization_debug,constant,source_player_character_v97,source_player_name_v97};
    }
    static bool integer_string(void* context,std::int32_t id,std::string& value,bool& is_null,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return self.localization.integer_string(id,self.text_services(),value,is_null,error);
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
            request->value=0;return 0;
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
#include "original_ui_movie_services_v1.inc"
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
        return self.assets.read(path,out,error);
    }
    static bool texture(void* context,const char* name,int,int,ui::SwfTexture& out,std::string& error) {
        auto& self=*static_cast<Impl*>(context);std::string uri;
        if(!scene::swf_texture_filename("",std::string("data/")+name,uri,error))return false;
        const auto found=self.exports.find(uri);if(found!=self.exports.end()){out=found->second;return true;}
        std::vector<std::uint8_t> encoded;if(!self.assets.read(uri,encoded,error))return false;
        textures::View view{};auto status=dh2_texture_open(encoded.data(),encoded.size(),&view);
        if(status!=textures::Error::ok){error=dh2_texture_error(status);return false;}
        resources::RetainedBytesV39 rgba;std::uint64_t rgba_bytes;
        if(!resources::swf_bitmap_bytes_v39(view.width,view.height,4,rgba_bytes,error)||
           !rgba.allocate(self.gpu.resource_budget_lease_v39(),resources::ResourceScopeV37::swf_gameplay,static_cast<std::size_t>(rgba_bytes),error))return false;
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
        if(command.kind==ui::SwfDraw::line_strip)++self.lines;
        if(command.kind==ui::SwfDraw::mask_begin)++self.masks;
        if(self.drawing_combat&&command.kind==ui::SwfDraw::bitmap_quad){
            if(!self.combat_quads){const auto& m=command.matrix.value;self.first_combat_quad={m[0]*command.rect[0]+m[1]*command.rect[2]+m[2],m[3]*command.rect[0]+m[4]*command.rect[2]+m[5]};
                self.combat_quad_extent={m[0]*(command.rect[1]-command.rect[0])+m[1]*(command.rect[3]-command.rect[2]),m[3]*(command.rect[1]-command.rect[0])+m[4]*(command.rect[3]-command.rect[2])};
                self.combat_quad_rgba=(std::uint32_t(command.fill.rgba[3])<<24)|(std::uint32_t(command.fill.rgba[0])<<16)|(std::uint32_t(command.fill.rgba[1])<<8)|command.fill.rgba[2];}
            ++self.combat_quads;
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
        if(!self.localization.native_string(args[0].string,self.text_services(),result,error))return false;
        if(result.sets_menu_string_flag)self.menu_string_flag=true;
        out.kind=ui::SwfValue::text;out.string=result.text;++self.string_calls;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original UI string delivered | symbol %s | found %d | text %s",args[0].string.c_str(),result.found,result.text.c_str());return true;
    }
    static void diagnostic(void* context,bool error,const char* text) {
        auto& self=*static_cast<Impl*>(context);if(error)++self.core_errors;
        __android_log_print(error?ANDROID_LOG_WARN:ANDROID_LOG_INFO,tag,"Original SWF core diagnostic: %s",text?text:"");
    }
#include "original_ui_gameplay_bridge_v1.inc"
    bool initialize_process_text_v101(std::string& error) {
        if(process_text_ready_v101){error.clear();return true;}
        if(process_text_attempted_v101){error=process_text_error_v101;return false;}
        if(!manager||!constants||!debug){error="Required actual initialized process text resources";return false;}
        process_text_attempted_v101=true;
        const auto initialize=[&]()->bool{
        if(!constants||!debug){error="Required native UI owner allocation failed";return false;}
        std::size_t cache_files=0;
        if(!assets.mount_cache(cache_files,error)||!assets.verify_cache(error))return false;
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
        const auto selected_pack=localization.pack();
        if(!localization.load({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error)||
           !localization.switch_pack(selected_pack,false,error))return false;
        return true;
        };
        if(!initialize()){process_text_error_v101=error;return false;}
        process_text_ready_v101=true;error.clear();return true;
    }
    bool load(std::string& error,bool source_loadmenu3=false) {
        //HUD generations borrow the process cache. Reloading its metadata
        //here would erase the pack selected by GSInit9/10 and loaded sheets.
        if(!initialize_process_text_v101(error))return false;
        movie=std::make_shared<ui::SwfMovie>();hud_deleted_v94=false;ui::SwfServices services;
        services.context=this;services.read=movie_read;services.texture=texture;services.image=image;
        services.draw=draw;services.stencil=stencil;services.native_call=native;services.diagnostic=diagnostic;
        gameplay_queries=std::make_unique<CharacterPanelSessionV1>(manager);
        gameplay_bridge=std::make_unique<ui::CharacterMenuAsBridgeV1>(shared_from_this(),[this](const char* name,ui::CharacterMenuCallV1& call,std::string& e){return gameplay_call(name,call,e);});
        services.native_actions={"NativeSkillGetEquipedSkillsIDs","NativeGetSkillDetails","NativeHUDGetActiveFaery",
             "NativeHUDSkill","NativeHUDSpell","NativeUsePotion","NativeSwapEquipment","NativeGetOptionParameters","NativeUseIpodPlayer",
             "NativeGetNextStatusMessage","NativeGetNextDialogMessage","NativeGetNextAchievementMessage",
             "NativeSkipAchievementMessage","NativeStopMessage"};
        services.native_action=gameplay_native;services.native_owner=shared_from_this();
        fonts=std::make_unique<ui::SwfTextFontPlatformV1>(ui::SwfFontServices{this,source_font_read_gfnt_v1,font_diagnostic},services,shared_from_this(),initialize_gfnt_backend_v1(),1.f);
        fonts->policy().renderer_feature=[this](const ui::edit_text_display_v1::Command& command,std::string& error){
            if(command.kind==ui::edit_text_display_v1::Command::grid_fit){gpu.set_grid_fit(command.enabled);return true;}
            error="Required original text render-cache connection";return false;
        };
        if(!movie->load({"data/menus/dqshared_droid.swf"},"data/menus/dqhud_droid.swf",fonts->services(),error))return false;
        // Paired slot3 aliases the SAME camera payload already used by this
        // retained HUD viewport. The envelope weakly borrows this generation.
        menu_camera_v93=std::make_shared<ui::MenuFlash2DCameraOwnerV93>(
          reinterpret_cast<std::uintptr_t>(movie.get()),driver_width,driver_height,shared_from_this(),camera);
        const ui::ViewportState64 seed{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1.f,0,0};
        const std::int32_t hud_bounds[]{0,0,driver_width,driver_height};
        if(!movie->connect_viewport(seed,{this,orientation,dimensions},error)||
           !movie->update_viewport(camera,error)||!movie->set_source_bounds(hud_bounds,2,error)||!movie->advance(0,error))return false;
        ui::SwfClipInfo clip;
        if(!movie->clip(panel,clip,error)||clip.id!=157){error="Required original health/mana parent differs";return false;}
        if(!font_failure.empty()){error=font_failure;return false;}
        status=std::make_unique<ui::PlayerStatusHud>(*movie);
        if(!status->bind(hud_sha,error))return false;
        enemy=std::make_unique<ui::EnemyStatusHudV1>(*movie,ui::EnemyHudTextServicesV1{this,integer_string});
        auto manager=flash_manager.lock();if(!manager){error="Required process FlashAnimManager host";return false;}
        if(!manager->bind_hud(movie,shared_from_this(),ui::CombatFlashProjectionV1{this,combat_project,combat_rectangle},error))return false;
        combat_flash=manager->backend();
        gameplay=std::make_unique<ui::AuthoredGameplayHudV1>(*movie,source_action_icon108_v62);gameplay_activated=false;
        if(!source_loadmenu3&&!manager->scan_hud(error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Source combat flash connected | styles %zu | same retained HUD/font/viewport | Level load-process producer pending",combat_flash->queue().styles().size());
        loaded=true;return true;
    }
    bool reset_failed(std::string& error) {
        if(movie){auto manager=flash_manager.lock();if(!manager){error="Required process Flash manager for HUD cleanup";return false;}
            const auto id=reinterpret_cast<std::uintptr_t>(movie.get());
            if(manager->scanned_fx()==id){if(!manager->reset_scan_for_anims(id,error))return false;}
            else if(manager->backend()->bound_movie_v92()==id){if(!manager->discard_unscanned_hud(id,error))return false;}
        }
        cancel_attack_v46();
        hud_pointers.clear();world_pointers.clear();model_renderer::world_touch_cancel();joystick={};model_renderer::authored_hud_heading(0,0,false);
        hurt_pulse.release();hurt_reported_frame=hurt_reported_outer=-1;
        status_timeline.release();
        gpu.abort();gameplay.reset();combat_flash.reset();enemy.reset();status.reset();movie.reset();fonts.reset();gfnt_text_backend_v1.reset();gameplay_bridge.reset();gameplay_queries.reset();loaded=false;selected=false;gameplay_activated=false;
        if(menu_camera_v93)menu_camera_v93->deleted=true;
        menu_camera_v93.reset();
        pending_text.clear();borrowed_combat_string.clear();reported_combat_active=0;
        reported_enemy=0;reported_enemy_frame=-1;
        last_width=last_height=0;reported_frames={{-1,-1,-1,-1,-1}};
        // Additional authored movies hold their own font platform and real
        // resource leases. A HUD failure must not invalidate those textures.
        if(!external_movies){leases.clear();exports.clear();}
        font_failure.clear();provider_failure.clear();
        glyph_uploads=bitmap_uploads=string_calls=core_errors=packed_glyphs=strips=lines=masks=0;
        if(external_movies){error.clear();return true;}
        return gpu.reset_images(error);
    }
};
OriginalUiSession::OriginalUiSession():impl_(std::make_shared<Impl>()),flash_manager_(std::make_shared<ui::FlashAnimManagerV92>()){impl_->flash_manager=flash_manager_;}
#include "original_ui_postmovie_methods_v62.inc"
bool OriginalUiSession::hud_movie_borrow_v4(ui::SwfMovie*& movie,std::shared_ptr<void>& owner,std::string& error){
 movie=nullptr;owner.reset();
 if(!impl_->movie||!impl_->gameplay){error="Required actual loaded gameplay HUD movie";return false;}
 movie=impl_->movie.get();owner=impl_->movie;return true;
}
bool OriginalUiSession::authored_action_cache_v62(ui::AuthoredGameplayHudV1*& facade,std::shared_ptr<void>& owner,std::string& error){
 facade=nullptr;owner.reset();
 if(!impl_->movie||!impl_->gameplay){error="Required actual existing HUD/manager108 cache owner";return false;}
 facade=impl_->gameplay.get();owner=impl_;error.clear();return true;
}
bool OriginalUiSession::renderer_set_wire_frame_v62(bool value,std::string& error){
 if(!impl_->manager){error="Required actual initialized SWF renderer before virtual40";return false;}
 impl_->gpu.source_set_wire_frame_v62(value);error.clear();return true;
}
bool OriginalUiSession::localization_borrow_v4(ui::HudTextV1*& text,std::shared_ptr<void>& owner,std::string& error){
 return process_string_manager_borrow_v101(text,owner,error);
}
bool OriginalUiSession::process_string_manager_borrow_v101(ui::HudTextV1*& text,std::shared_ptr<void>& owner,std::string& error){
 text=nullptr;owner.reset();
 if(!impl_->manager||!impl_->constants){error="Required initialized process StringManager resource owner";return false;}
 text=&impl_->localization;owner=impl_;error.clear();return true;
}
bool OriginalUiSession::load_process_constants_stage_v101(
 const std::function<bool(const char*,std::string&)>& actual_debug,bool& complete,std::string& error){
 complete=false;auto& self=*impl_;
 if(self.process_constants_failed_v101){error=self.process_constants_error_v101;return false;}
 if(!self.manager||!self.constants||!actual_debug){error="Required actual process PyDataConstants resource/Debug owners";return false;}
 //4c3730 switch covers0..27; case27 returns1 without increment.
 if(self.process_constants_stage20_v101==27){complete=true;error.clear();return true;}
 if(self.process_constants_stage20_v101>27){error="Original PyDataConstants source phase domain";return false;}
 const auto& file=process_pydata_constants_v101[self.process_constants_stage20_v101];
 std::vector<std::uint8_t> bytes;
 const auto fail=[&](){self.process_constants_failed_v101=true;self.process_constants_error_v101=error;return false;};
 if(!self.assets.read(std::string("data/")+file.uri,bytes,error))return fail();
 if(!actual_debug("isTracingPyDataConstants",error))return fail();
 dh2_script_constants_reload receipt{};
 //Source reloadData merges into THIS map; source name-stop is successful
 //delivery with prefix assignments preserved, not malformed native input.
 const int status=dh2_script_constants_load(self.constants.get(),bytes.data(),static_cast<std::uint32_t>(bytes.size()),&receipt);
 if(status<0){error=std::string("Process PyDataConstants.reloadData failed: ")+file.registration;return fail();}
 ++self.process_constants_stage20_v101;error.clear();return true;
}
bool OriginalUiSession::load_process_arrays_stage_v101(bool& complete,std::string& error){
 complete=false;auto& self=*impl_;
 if(!self.manager||!self.process_arrays_v101){error="Required actual initialized process PyDataArrays owner";return false;}
 const auto read=[&self](const std::string& uri,std::vector<std::uint8_t>& bytes,std::string& error){
  //Compiled member schemas are retained native APK metadata, not original
  //FileManager runtime paths. Record/name streams use the original URI.
  if(uri.size()>=18&&uri.compare(uri.size()-18,18,"_pystructnames.bin")==0){
   const auto slash=uri.rfind('/');return self.raw_asset((std::string("data/")+uri.substr(slash+1)).c_str(),bytes,error);
  }
  return self.assets.read(uri,bytes,error);
 };
 if(!self.process_arrays_v101->load_stage(read,complete,error))return false;
 if(complete&&!self.initialize_process_text_v101(error)){complete=false;return false;}
 return true;
}
bool OriginalUiSession::process_arrays_borrow_v101(std::shared_ptr<SourceProcessArraysV101>& out,std::string& error){
 out.reset();if(!impl_->manager||!impl_->process_arrays_v101){error="Required actual initialized process PyDataArrays";return false;}
 out=impl_->process_arrays_v101;error.clear();return true;
}
#include "original_ui_process_trophies_v119.inc"
bool OriginalUiSession::status_messages_v26(std::shared_ptr<ui::MenuStatusMessagesV26>& out,std::string& error){
    out=impl_->message_owner();error.clear();return true;
}
bool OriginalUiSession::source_script_ui_leaves_v97(const model_renderer::SourceCampaignCandidateBorrowV55& candidate,
 SourceScriptUiLeavesV96& out,std::string& e){
 if(!impl_->manager){e="Required initialized actual UI resource/text owner for script dialogue";return false;}
 std::shared_ptr<SourceScriptUiWorldV97> world;
 if(!SourceScriptUiWorldV97::create(candidate,world,e))return false;
 //Provider rebinding enrolls the new actual campaign; it does not flush old
 //queues, change a movie generation or fabricate a player/menu readiness.
 impl_->script_ui_world_v97=world;impl_->source_script_world_v104=candidate.actual_world;
 struct Provider {std::weak_ptr<Impl> ui;std::shared_ptr<SourceScriptUiWorldV97> world;};
 auto provider=std::make_shared<Provider>();provider->ui=impl_;provider->world=world;
 out={};out.owner=provider;
 out.publish_current_name=[provider](auto name,auto& error){return provider->world->publish_current_name(name,error);};
 out.send_script_message=[provider](auto skip,auto script,auto module,auto& error){return provider->world->send_script_message(skip,script,module,error);};
 out.store_controller_global=[provider](auto value,auto& error){return provider->world->store_controller_global(value,error);};
 out.dialog_active=[provider](auto& active,auto& error){auto ui=provider->ui.lock();
  if(!ui){error="Actual dialog queue owner expired";return false;}active=ui->dialog_owner_v97()->active();error.clear();return true;};
 out.enqueue_dialog=[provider](auto first,auto text,auto style,auto speaker,auto context,auto start,auto& error){
  auto ui=provider->ui.lock();if(!ui){error="Actual DialogMsg owner expired";return false;}
  auto queue=ui->dialog_owner_v97();dh2::ui::DialogMsgV97 message;
  if(!queue->construct(first,text,style,speaker,message,error))return false;
  return queue->enqueue(message,context,start,error);
 };
 out.flush_dialogs=[provider](auto& error){auto ui=provider->ui.lock();if(!ui){error="Actual dialog queue expired";return false;}
  ui->dialog_owner_v97()->flush();error.clear();return true;};
 out.flush_status=[provider](auto& error){auto ui=provider->ui.lock();if(!ui){error="Actual status queue expired";return false;}
  return ui->message_owner()->flush_source_context_v97(0,error);};
 out.flush_achievements=[provider](auto& error){auto ui=provider->ui.lock();if(!ui){error="Actual achievement queue expired";return false;}
  ui->auxiliary_messages_v97.flush_achievements();error.clear();return true;};
 out.flush_online=[provider](auto& error){auto ui=provider->ui.lock();if(!ui){error="Actual online-status queue expired";return false;}
  ui->auxiliary_messages_v97.flush_online();error.clear();return true;};
 e.clear();return true;
}
bool OriginalUiSession::quest_completed_dialog_v108(std::int32_t title_id,std::int32_t style,const std::vector<data::QuestRewardDefinitionV51>& rewards,std::string& e){
 if(!impl_->constants||!impl_->manager){e="Required SAME quest StringManager/constants";return false;}
 std::string title,text;bool null{};ui::HudTextEnvironmentV1 environment;environment.localization=impl_->text_services();
 if(!impl_->localization.integer_string(title_id,environment.localization,title,null,e))return false;if(null)title.clear();
 bool first=true;
 for(const auto& reward:rewards){
  std::string fragment;
  if(reward.type==0||reward.type==1){
   const char* key=reward.type==0?"GAMEPLAYMENUS_REWARD_DETAIL_GOLD":"GAMEPLAYMENUS_REWARD_DETAIL_EXP";
   std::int32_t id;if(dh2_script_constants_get(impl_->constants.get(),"StrID",key,&id)){e="Required actual quest reward text constant";return false;}
   std::string format;if(!impl_->localization.integer_string(id,environment.localization,format,null,e))return false;
   if(null){e="Original reward format is NULL";return false;}
   if(!ui::progression_xp_format_v23(impl_->localization,environment,format.c_str(),reward.parameter1,fragment,e))return false;
  }
  //Source483680 joins EVERY enabled reward, even an empty non-text kind.
  if(!first)text+='\n';text+=fragment;first=false;
 }
 if(first){std::int32_t id;if(dh2_script_constants_get(impl_->constants.get(),"StrID","MENU_AUTOTRANSMUTE_1",&id)||!impl_->localization.integer_string(id,environment.localization,text,null,e))return false;if(null)text.clear();}
 auto queue=impl_->dialog_owner_v97();ui::DialogMsgV97 message;
 return queue->construct_strings_v108(title,text,style,1,message,e)&&queue->enqueue(message,0,true,e);
}
bool OriginalUiSession::gameplay_text_environment_v67(ui::HudTextEnvironmentV1& out,std::shared_ptr<void>& owner,std::string& e){
 out={};owner.reset();
 if(!impl_->manager||!impl_->constants||impl_->directory.empty()){
  e="Required actual initialized gameplay StringManager/resource providers";return false;
 }
 out.localization=impl_->text_services();owner=impl_;e.clear();return true;
}
bool OriginalUiSession::bind_combat_presentation_v115(const model_renderer::SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<model_renderer::SourceWorldBorrowV61> world;auto movie=impl_->movie;auto manager=impl_->flash_manager.lock();
 if(!model_renderer::borrow_source_campaign_condition_world_v70(candidate,world,e)||!impl_->loaded||!impl_->constants||
    !movie||!manager||manager->backend()!=impl_->combat_flash||manager->scanned_fx()!=reinterpret_cast<std::uintptr_t>(movie.get())){
  if(e.empty())e="Required SAME source combat HUD/Flash/StringManager generation";return false;
 }
 struct Presentation {
  std::weak_ptr<model_renderer::SourceWorldBorrowV61> world;
  std::shared_ptr<Impl> ui;
  std::weak_ptr<dh2::ui::SwfMovie> movie;
  std::uintptr_t player{};
  std::string localized;
  bool current(std::shared_ptr<model_renderer::SourceWorldBorrowV61>& source,std::string& e){
   source=world.lock();auto m=movie.lock();auto manager=ui->flash_manager.lock();
   if(!source||!m||m!=ui->movie||m->player_identity()!=player||!ui->loaded||!ui->combat_flash||!manager||
      manager->backend()!=ui->combat_flash||manager->scanned_fx()!=reinterpret_cast<std::uintptr_t>(m.get())||
      ui->combat_flash->bound_movie_v92()!=manager->scanned_fx()){
    e="Expired actual combat presentation World/HUD generation";return false;
   }
   model_renderer::SourceCampaignCandidateBorrowV55 c;
   if(!model_renderer::borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=source->owner){if(e.empty())e="Combat presentation belongs to an unpublished World";return false;}
   return true;
  }
 };
 auto presentation=std::make_shared<Presentation>();presentation->world=world;presentation->ui=impl_;
 presentation->movie=movie;presentation->player=movie->player_identity();
 model_renderer::SourceCombatPresentationV115 services;services.owner=presentation;
 services.sound_index=[presentation](const char* name,std::int32_t& index,std::string& e){
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;if(!presentation->current(source,e))return false;
  return model_renderer::campaign_sound_index_v115(source->owner,name,index,e);
 };
 services.play_sound=[presentation](std::int32_t index,bool enabled,std::int32_t fade,std::int32_t group,bool bypass_online,std::string& e){
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;if(!presentation->current(source,e))return false;
  return model_renderer::play_campaign_plain_sound_v115(source->owner,index,enabled,fade,group,bypass_online,e);
 };
 services.settings_job_start=[presentation,application=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(candidate.application)](std::string& e){
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;if(!presentation->current(source,e))return false;
  auto app=application.lock();if(!app){e="Released SAME combat-tail settings Application";return false;}
  return model_renderer::start_process_settings_job_v102(app,e);
 };
 services.localized=[presentation](std::int32_t id,const char*& out,std::string& e){
  out=nullptr;std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;if(!presentation->current(source,e))return false;
  bool null{};if(!presentation->ui->localization.integer_string(id,presentation->ui->text_services(),presentation->localized,null,e))return false;
  if(!null)out=presentation->localized.c_str();return true; //Preserve genuine source NULL string.
 };
 services.enqueue=[presentation](const character::skills::CombatTextRequestV1& text,std::string& e){
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;if(!presentation->current(source,e))return false;
  if(!text.style||(!text.numeric&&!text.text)){e="Required actual combat Flash style/text";return false;}
  //Same scanned source style map, current camera projection and source pool.
  //Queue copies borrowed string before synchronous provider returns; no widget,
  //award, audio replay or independently advanced timer is introduced.
  return presentation->ui->combat_flash->play(text.style,text.position,text.text,text.number,text.color,text.numeric,e);
 };
 services.critical_camera=[presentation](std::uintptr_t id,std::string& e){
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;if(!presentation->current(source,e))return false;
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!model_renderer::borrow_current_native_level_v27(current,e))return false;
  if(!current){e.clear();return true;} //F_ApplyResult actual CurrentLevel NULL guard.
  model_renderer::SourceCampaignCandidateBorrowV55 c;
  if(!model_renderer::borrow_source_campaign_candidate_v55(c,e)||current.level()!=c.level){if(e.empty())e="Critical camera requires SAME current campaign Level";return false;}
  const auto camera128=current.level()->constructor_fields_v3().field128;
  if(!camera128){e.clear();return true;} //Genuine source Camera128 NULL no-op.
  auto app=c.camera_application;auto session=app?app->world():nullptr;
  auto camera=session&&session->camera?session->camera->level():nullptr;
  if(!camera||reinterpret_cast<std::uintptr_t>(camera.get())!=camera128||!camera->level()){
   e="Required SAME actual CameraLevel128/animator for critical effect";return false;
  }
  model_renderer::SourceCampaignCharacterBorrowV62 actor;
  if(!model_renderer::borrow_source_campaign_character_v62(source->owner,id,actor,e))return false;
  bool player{};if(!actor.character->is_player(player,e))return false;
  //Whole CameraLevel.CanPlayShakeAnim40f980 positive-Character branch.
  if(player){auto pm=c.application->source_player_manager_v59();std::int32_t count{};bool local{};
   if(!pm||!pm->manager()||!pm->manager()->num_local_players(false,count,e))return false;
   if(count!=1){e.clear();return true;}
   if(!pm->source_is_local_player_v61(id,local,e))return false;
   if(!local){e.clear();return true;}
  }
  dh2::camera::PointV2 look;if(!model_renderer::source_campaign_character_look_at_v68(source->owner,id,look,e))return false;
  std::shared_ptr<const void> tables_owner;const data::AnimationTables* tables{};
  if(!model_renderer::borrow_source_campaign_animation_tables_v67(source->owner,tables_owner,tables,e))return false;
  const auto row=camera->source_animation_row80_v115();
  if(!tables_owner||!tables||row<0||static_cast<std::size_t>(row)>=tables->cameras.size()){
   e="Required original CamAnimSetTable[CameraLevel80] critical row";return false;
  }
  return camera->level()->play_animation(tables->cameras[static_cast<std::size_t>(row)].crit,0,true,e);
 };
 return model_renderer::bind_source_campaign_combat_presentation_v115(candidate.actual_world,std::move(services),e);
}
bool OriginalUiSession::bind_character_reward_text_v114(const model_renderer::SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;
 auto manager=impl_->flash_manager.lock();auto movie=impl_->movie;
 if(!model_renderer::borrow_source_campaign_condition_world_v70(candidate,source,e)||!source->design||
    !impl_->loaded||!movie||!impl_->constants||!manager||!impl_->combat_flash||
    manager->backend()!=impl_->combat_flash||manager->scanned_fx()!=reinterpret_cast<std::uintptr_t>(movie.get())||
    impl_->combat_flash->bound_movie_v92()!=manager->scanned_fx()){
  if(e.empty())e="Required actual scanned HUD/Flash/localization before Character reward enrollment";return false;
 }
 struct RewardTransport {
  std::weak_ptr<model_renderer::SourceWorldBorrowV61> world;
  std::shared_ptr<Impl> ui;
  std::weak_ptr<dh2::ui::SwfMovie> movie;
  std::uintptr_t player{};
 };
 auto transport=std::make_shared<RewardTransport>();transport->world=source;transport->ui=impl_;
 transport->movie=movie;transport->player=movie->player_identity();
 return model_renderer::bind_source_campaign_character_reward_text_v114(candidate.actual_world,transport,
  [transport](std::uintptr_t id,model_renderer::SourceCharacterRewardTextV114 kind,std::int32_t amount,std::string& e){
   auto world=transport->world.lock();auto movie=transport->movie.lock();auto& presentation=*transport->ui;
   auto manager=presentation.flash_manager.lock();
   if(!world||!world->design||!movie||movie!=presentation.movie||movie->player_identity()!=transport->player||
      !presentation.loaded||!presentation.combat_flash||!manager||manager->backend()!=presentation.combat_flash||
      manager->scanned_fx()!=reinterpret_cast<std::uintptr_t>(movie.get())||
      presentation.combat_flash->bound_movie_v92()!=manager->scanned_fx()){
    e="Released or replaced actual buffered reward HUD generation";return false;
   }
   model_renderer::SourceCampaignCharacterBorrowV62 actor;
   if(!model_renderer::borrow_source_campaign_character_v62(world->owner,id,actor,e)||
      !actor.character||!actor.character->actor||!actor.character->actor->source_bounds_ready){
    if(e.empty())e="Required same Character reward target/bounding-box producer";return false;
   }
   std::array<float,3> position;
   if(!model_renderer::source_campaign_character_target_position_v114(world->owner,id,position,e))return false;
   const auto* bounds=actor.character->actor->runtime.subobjects.local_bounds;
   //Same original floating-point height subtraction and Z addition; target
   //cache184 is already selected by original GetTargetPosition180/80 guard.
   volatile float height=bounds[5]-bounds[2];volatile float z=position[2]+height;position[2]=z;
   const bool xp=kind==model_renderer::SourceCharacterRewardTextV114::xp;
   const auto design=world->design->borrow();const auto* constants=design.design();
   std::int32_t color{},string_id{};
   if(!constants||!constants->lookup||
      constants->lookup(constants->context,0,"ScrollingCombatText",xp?"QR_XP":"QR_Gold",&color)||
      constants->lookup(constants->context,0,"StrID",xp?"GAMEPLAYMENUS_REWARD_XP":"ITEMS_GOLD",&string_id)){
    e="Required actual Gold/XP reward StrID/color constants";return false;
   }
   std::string format,text;bool null{};ui::HudTextEnvironmentV1 environment;
   environment.localization=presentation.text_services();
   if(!presentation.localization.integer_string(string_id,environment.localization,format,null,e))return false;
   if(null){e="Original Character reward localized format is NULL";return false;}
   //508ef4 typed integer varargs uses the SAME StringManager and selected pack.
   //XP arrives after the original ASR8 conversion; do not rescale it again.
   if(!ui::progression_xp_format_v23(presentation.localization,environment,format.c_str(),amount,text,e))return false;
   //Play owns the source twelve-context/eight-instance admission, immediate
   //camera projection, authored clip timeline and original queue-full drop.
   //The source Character suffix resets1500/1504 only after this returns.
   return presentation.combat_flash->play(xp?"anim_reward_xp":"anim_reward_gold",position.data(),text.c_str(),0,color,false,e);
  },e);
}
bool OriginalUiSession::bind_death_rewards_text_v84(const model_renderer::SourceCampaignCandidateBorrowV55& candidate,
 model_renderer::SourceDeathRewardsLeavesV84& leaves,std::string& e){
 std::shared_ptr<model_renderer::SourceWorldBorrowV61> world;
 if(!model_renderer::borrow_source_campaign_condition_world_v70(candidate,world,e)||!leaves.owner||
  !impl_->manager||!impl_->constants){if(e.empty())e="Required actual campaign death StringManager/native provider";return false;}
 struct XpTransport {
  std::weak_ptr<model_renderer::SourceWorldBorrowV61> world;
  std::shared_ptr<Impl> ui;
  std::shared_ptr<void> prior;
  std::string error;
  bool actor(std::uintptr_t id,model_renderer::SourceCampaignCharacterBorrowV62& out){
   auto w=world.lock();if(!w||!model_renderer::borrow_source_campaign_character_v62(w->owner,id,out,error)||!out.character||!out.character->actor){if(error.empty())error="Released actual XP Character";return false;}return true;
  }
 };
 auto transport=std::make_shared<XpTransport>();transport->world=world;transport->ui=impl_;transport->prior=leaves.owner;
 leaves.owner=transport;
 leaves.xp_text=[transport](character::ProgressionActorV1& victim,character::ProgressionActorV1& player,std::int32_t value,std::string& e){
  auto& t=*transport;auto world=t.world.lock();auto& presentation=*t.ui;
  if(!world||!world->design||!presentation.loaded||!presentation.movie||!presentation.combat_flash){e="Required SAME actual HUD3/Flash manager for XP animation";return false;}
  model_renderer::SourceCampaignCharacterBorrowV62 actual_player;if(!t.actor(player.identity,actual_player)||!actual_player.character->properties||
   player.properties->resolved!=actual_player.character->properties->resolved.data()){e=t.error.empty()?"XP recipient property owner changed":t.error;return false;}
  //Original GetStyleIdFromName414678 reads the same process Flash name map;
  //the later Play uses that actual scanned source queue, not another widget.
  (void)presentation.combat_flash->queue().style_id("anim_sct_xp");
  auto design=world->design->borrow();const auto* constants=design.design();std::int32_t color{};
  if(!constants||!constants->lookup||constants->lookup(constants->context,0,"ScrollingCombatText","XPColor",&color)){e="Required source ScrollingCombatText.XPColor";return false;}
  character::skills::PlayerXPTextServicesV1 services;services.common.context=&t;
  services.common.position=[](void* raw,std::uintptr_t id,float out[3]){auto& t=*static_cast<XpTransport*>(raw);model_renderer::SourceCampaignCharacterBorrowV62 actor;
   if(!out||!t.actor(id,actor))return -1;
   std::copy_n(actor.character->actor->runtime.subobjects.position,3,out);return 0;};
  services.common.height=[](void* raw,std::uintptr_t id,float* out){auto& t=*static_cast<XpTransport*>(raw);model_renderer::SourceCampaignCharacterBorrowV62 actor;
   if(!out||!t.actor(id,actor)||!actor.character->actor->source_bounds_ready){t.error="Required actual source XP victim bounding box";return -1;}
   const auto* box=actor.character->actor->runtime.subobjects.local_bounds;*out=box[5]-box[2];return 0;};
  services.common.constant=[](void* raw,const char* group,const char* key,std::int32_t* out){auto& t=*static_cast<XpTransport*>(raw);auto world=t.world.lock();
   if(!world||!world->design||!out)return -1;auto design=world->design->borrow();auto* constants=design.design();return constants&&constants->lookup?constants->lookup(constants->context,0,group,key,out):-1;};
  services.common.localized=[](void* raw,std::int32_t id,const char** out){auto& t=*static_cast<XpTransport*>(raw);return Impl::combat_localized(t.ui.get(),id,out);};
  services.format=[](void* raw,const char* format,std::int32_t number,std::string* out){auto& t=*static_cast<XpTransport*>(raw);
   if(!format||!out||!t.ui->loaded)return -1;ui::HudTextEnvironmentV1 environment;environment.localization=t.ui->text_services();
   return ui::progression_xp_format_v23(t.ui->localization,environment,format,number,*out,t.error)?0:-1;};
  services.common.enqueue=[](void* raw,const character::skills::CombatTextRequestV1* q){auto& t=*static_cast<XpTransport*>(raw);
   if(!q||!q->style||!q->text||!t.ui->loaded||!t.ui->combat_flash)return -1;
   return t.ui->combat_flash->play(q->style,q->position,q->text,q->number,q->color,q->numeric,t.error)?0:-1;};
 t.error.clear();const auto status=character::skills::player_xp_text_v1(victim.identity,value,color,services);
  if(status!=1){e=t.error.empty()?"Required source localized XP/anim_sct_xp delivery":t.error;return false;}e.clear();return true;
 };
 leaves.level_presentation=[transport](character::ProgressionActorV1& player,std::int32_t level,std::string& e){
  auto world=transport->world.lock();if(!world||!world->design){e="Released actual LevelUp campaign";return false;}
  model_renderer::SourceCampaignCharacterBorrowV62 actor;if(!transport->actor(player.identity,actor)||!actor.character->properties||
   !player.properties||player.properties->resolved!=actor.character->properties->resolved.data()){e="LevelUp presentation must use SAME canonical player/property cells";return false;}
  auto design=world->design->borrow();std::int32_t text_id{};const auto* constants=design.design();
  if(!constants||!constants->lookup||constants->lookup(constants->context,0,"StrID","MENU_LEVEL_UP",&text_id)){e="Required actual StrID.MENU_LEVEL_UP";return false;}
  std::string text;bool is_null{};auto& ui=*transport->ui;
  if(!ui.localization.integer_string(text_id,ui.text_services(),text,is_null,e))return false;
  if(is_null)text.clear();auto messages=ui.message_owner();if(!messages||!messages->enqueue_level_up(text,e))return false;
  return model_renderer::source_campaign_level_up_presentation_v88(world->owner,player,level,e);
 };
 e.clear();return true;
}
bool OriginalUiSession::bind_item_presentation_v88(const model_renderer::SourceCampaignCandidateBorrowV55& candidate,
 model_renderer::SourceCampaignItemLeavesV88& leaves,std::string& error){
 std::shared_ptr<model_renderer::SourceWorldBorrowV61> world;
 if(!model_renderer::borrow_source_campaign_condition_world_v70(candidate,world,error)||!leaves.owner)return false;
 std::shared_ptr<ui::MenuStatusMessagesV26> messages;if(!status_messages_v26(messages,error)||!messages)return false;
 struct ItemPresentation {std::shared_ptr<void> prior;std::shared_ptr<ui::MenuStatusMessagesV26> messages;};
 auto owner=std::make_shared<ItemPresentation>();owner->prior=leaves.owner;owner->messages=messages;leaves.owner=owner;
 leaves.enqueue_status=[owner](const char* text,std::uint32_t metadata,std::string& e){
  if(!text){e="Required actual pickup StatusMsg CString";return false;}
  return owner->messages->enqueue_status(0,text,metadata,e);
 };
 const std::weak_ptr<model_renderer::SourceWorldBorrowV61> weak=world;auto presentation=impl_;auto prior_outer=leaves.item_outer;auto prior_pickup=leaves.pickup;
 auto borrow_item=[weak](std::uintptr_t id,std::shared_ptr<character::RetainedWorldItemObjectV1>& item,std::shared_ptr<model_renderer::SourceWorldBorrowV61>& w,std::string& e){
  w=weak.lock();model_renderer::SourceCampaignCandidateBorrowV55 current;
  if(!w||!model_renderer::borrow_source_campaign_candidate_v55(current,e)||current.actual_world!=w->owner||!w->prepared_items_v88||!w->prepared_items_v88->items()){if(e.empty())e="Released SAME native Item UI owner";return false;}
  item=w->prepared_items_v88->items()->factory().find(id);if(!item){e="Foreign source Item tooltip receiver";return false;}return true;
 };
 auto destroy=[presentation,borrow_item](std::uintptr_t id,std::string& e){std::shared_ptr<character::RetainedWorldItemObjectV1> item;std::shared_ptr<model_renderer::SourceWorldBorrowV61> w;
  if(!borrow_item(id,item,w,e))return false;const auto address=item->fields().tooltip3c8;if(!address)return true;
  auto found=presentation->item_tooltips_v104.find(id);if(found==presentation->item_tooltips_v104.end()||!found->second->source||reinterpret_cast<std::uintptr_t>(found->second->source.get())!=address){e="Item tooltip pointer differs from its source C1 owner";return false;}
  if(!found->second->source->destroy(e))return false;presentation->item_tooltips_v104.erase(found);return true;
 };
 leaves.release_item_ui=destroy;
 leaves.pickup=[prior_pickup,destroy](const character::LootInteractRequestV8& q,character::LootInteractResponseV8& out,std::string& e){
  if(q.operation==character::LootInteractOperationV8::tooltip_destroy)return destroy(q.object,e);
  if(prior_pickup)return prior_pickup(q,out,e);e="Required different positive Item pickup UI/network receiver";return false;
 };
 leaves.item_outer=[presentation,borrow_item,prior_outer](const character::WorldItemRequestV1& q,std::int32_t& value,std::string& e){
  using O=character::WorldItemOperationV1;
  if(q.operation!=O::show_tooltip&&q.operation!=O::hide_tooltip&&q.operation!=O::tooltip_visible&&q.operation!=O::tooltip_position&&q.operation!=O::tooltip_update){
   if(prior_outer)return prior_outer(q,value,e);e="Required different original Item native continuation";return false;}
  std::shared_ptr<character::RetainedWorldItemObjectV1> item;std::shared_ptr<model_renderer::SourceWorldBorrowV61> w;
  if(!borrow_item(q.object,item,w,e))return false;auto& fields=item->fields();auto found=presentation->item_tooltips_v104.find(q.object);
  if(q.operation==O::show_tooltip){
   auto* instance=item->inventory().peek();if(!instance){e="Original ShowTooltip GetItem(0) NULL";return false;}
   if(!fields.tooltip3c8){
    if(found!=presentation->item_tooltips_v104.end()){e="Tooltip retains failed source construction prefix";return false;}
    auto record=std::make_unique<Impl::ItemTooltipV104>();record->source=std::make_unique<ui::SwfAnimTooltipV104>(presentation->item_tooltip_services_v104(w->files_owner));
    auto* source=record->source.get();presentation->item_tooltips_v104.emplace(q.object,std::move(record));
    if(!source->construct(e))return false;fields.tooltip3c8=reinterpret_cast<std::uintptr_t>(source);found=presentation->item_tooltips_v104.find(q.object);
   }
  }
  if(!fields.tooltip3c8){value=0;e.clear();return true;}
  if(found==presentation->item_tooltips_v104.end()||!found->second->source||reinterpret_cast<std::uintptr_t>(found->second->source.get())!=fields.tooltip3c8){e="Required SAME original Item SWFAnimToolTip receiver";return false;}
  auto& tooltip=*found->second->source;bool visible{};
  if(q.operation==O::tooltip_visible){if(!tooltip.is_visible(visible,e))return false;value=visible;return true;}
  if(q.operation==O::hide_tooltip){if(!tooltip.is_visible(visible,e))return false;return !visible||tooltip.fade_out(100,e);}
  if(q.operation==O::tooltip_update)return tooltip.update(e);
  if(q.operation==O::tooltip_position)return presentation->item_tooltip_position_v104(tooltip.anim(),item->base().vector3(0x160),e);
  if(!tooltip.is_visible(visible,e))return false;if(visible)return true;
  if(!tooltip.fade_in(e))return false;
  auto* instance=item->inventory().peek();character::ItemColorLookupServicesV2 colors;colors.design=w->design->borrow();
  colors.font_text_color=[](void*,std::int32_t row,std::uint32_t& out,std::string& e){const dh2::ui::CharacterMenuFontPaletteV1* palette{};return model_renderer::borrow_actual_font_palette_v4(palette,e)&&palette&&palette->text_color(row,out,e);};
  std::uint32_t color{};if(!instance||!character::item_color_lookup_v2(*instance,colors,color,e))return false;
  if(!presentation->item_anim_scope_v104([&](auto& graph,auto& e){Impl::ItemAnimV104* anim{};gameswf::character* clip{};if(!presentation->item_anim_character_v104(graph,tooltip.anim(),anim,clip,e))return false;
   ui::SwfAsValue receiver,field;if(!graph.retain_object(clip,receiver,e)||!graph.find_target(receiver,"_text_itemname",field,e))return false;
   if(field.identity()){bool accepted{};if(!graph.set_member(field,"text",ui::SwfAsValue::text(instance->name),accepted,e))return false;
    gameswf::as_object* text{};if(!graph.borrow_object(field,text,e)||!text||!text->is(gameswf::character::m_class_id)){e="Required actual SWFAnim bound text character";return false;}
    gameswf::cxform transform;for(int n=0;n<3;++n)transform.m_[n][0]=0;transform.m_[3][0]=1;
    transform.m_[0][1]=float((color>>16)&255);transform.m_[1][1]=float((color>>8)&255);transform.m_[2][1]=float(color&255);transform.m_[3][1]=float((color>>24)&255);
    static_cast<gameswf::character*>(text)->set_cxform(transform);
   }return true; //Original absent BindText target skips text/color writes.
  },e)||!presentation->item_tooltip_position_v104(tooltip.anim(),item->base().vector3(0x160),e))return false;
  if(!w->player_manager||!w->player_manager->manager()){e="Required SAME ShowTooltip PlayerManager";return false;}
  dh2::player::PlayerInfoFieldsV1* info{};if(!w->player_manager->manager()->get_by_character(fields.owner3bc,false,info,e)||!info)return false;
  const auto friendly=info->friendly678;
  return presentation->item_anim_scope_v104([&](auto& graph,auto& e){Impl::ItemAnimV104* anim{};gameswf::character* clip{};if(!presentation->item_anim_character_v104(graph,tooltip.anim(),anim,clip,e))return false;
   ui::SwfAsValue receiver,ignored;bool callable{};return graph.retain_object(clip,receiver,e)&&graph.invoke(receiver,receiver,"linkToPlayer",{ui::SwfAsValue::number(friendly)},ignored,callable,e);
  },e);
 };
 //The original +3ff7a8 tail dereferences the inventory's Character+4 before
 //IsPlayer. NULL-character temporary inventories genuinely have no receiver;
 //preserve the inserted item and report this reached source fault safely.
 if(!leaves.full_notifications)leaves.full_notifications=[](data::LootTemporaryInventoryV8& inventory,data::ItemInstanceV1& item,std::string& e){
  bool found=false;for(const auto& slot:inventory.items())if(slot&&slot->item.get()==&item){found=true;break;}
  if(!found){e="Full notification must follow actual SAME inserted Item";return false;}
  e="Original Inventory full-notification tail reached NULL Character+4 dereference (3ff7a8)";return false;
 };
 error.clear();return true;
}
bool OriginalUiSession::refresh_message_caches_stage26_v66(std::string& error){
    // The actual source calls need the real statics, but all supplied characters
    // are NULL. They neither resolve a root nor overwrite the retained context.
    impl_->message_caches_v66.level_stage26_refresh_null();error.clear();return true;
}
bool OriginalUiSession::complete_refresh_stage26_v66(std::string& error){
    if(!impl_->loaded||!impl_->movie){error="Required actual new HUD for source completeRefresh";return false;}
    return impl_->movie->menu_action_script(nullptr,[](void*,ui::SwfAsGraph& graph,std::string& e){
        ui::SwfAsValue root,hud,result;bool callable{};
        if(!graph.root_value(root,e)||!graph.find_target(root,"menu_HUD_0",hud,e))return false;
        if(!hud.identity()){e="Required authored source menu_HUD_0 character";return false;}
        if(!graph.invoke(hud,hud,"completeRefresh",{},result,callable,e))return false;
        if(!callable){e="Required source HUD completeRefresh ActionScript";return false;}return true;
    },error);
}
bool OriginalUiSession::display_fast_travel_v83(bool visible,const char* localized,const char* level,std::int32_t entry,std::string& error){
 if(!impl_->loaded||!impl_->movie){error="Required SAME source HUD primary3/root140 for DisplayFastTravel";return false;}
 if(!localized||!level){error="Required actual FastTravel localized/level CString arguments";return false;}
 struct Arguments {bool visible;std::string localized,level;std::int32_t entry;} arguments{visible,localized,level,entry};
 return impl_->movie->menu_action_script(&arguments,[](void* raw,ui::SwfAsGraph& graph,std::string& e){
  const auto& actual=*static_cast<Arguments*>(raw);
  const std::vector<ui::SwfAsValue> values{ui::SwfAsValue::boolean(actual.visible),ui::SwfAsValue::text(actual.localized),ui::SwfAsValue::text(actual.level),ui::SwfAsValue::number(actual.entry)};
  return graph.invoke_renderfx("_root","DisplayFastTravel",values,e);
 },error);
}
bool OriginalUiSession::process_cache_read_v119(const std::string& uri,bool& found,
 std::vector<std::uint8_t>& bytes,std::uint32_t maximum,std::string& error){
 found=false;bytes.clear();
 if(!impl_->manager){error="Process asset delivery requires the initialized UI/cache owner";return false;}
 if(!maximum){error="Process asset delivery requires a finite payload budget";return false;}
 return impl_->assets.read_cache_admitted_v119(uri,found,bytes,[maximum](std::uint32_t size,std::string& e){
  if(size>maximum){e="Authored process asset exceeds payload budget";return false;}
  e.clear();return true;
 },error);
}
bool OriginalUiSession::enqueue_tutorial_v118(const std::shared_ptr<void>& world,std::int32_t text,std::int32_t duration,std::string& error){
 if(!world||impl_->source_script_world_v104.lock()!=world||!impl_->script_ui_world_v97){
  error="Tutorial queue requires same current campaign UI binding";return false;
 }
 impl_->auxiliary_messages_v97.enqueue_tutorial({text,duration});
 if(impl_->auxiliary_messages_v97.tutorial_count()==1)
  return impl_->invoke_message_v97(ui::MessageFamilyV66::tutorial,"onTutorialMessage",0,error);
 error.clear();return true;
}
bool OriginalUiSession::skip_tutorial_v118(const std::shared_ptr<void>& world,bool all,std::string& error){
 if(!world||impl_->source_script_world_v104.lock()!=world||!impl_->script_ui_world_v97){
  error="Tutorial skip requires same current campaign UI binding";return false;
 }
 if(all)impl_->auxiliary_messages_v97.flush_tutorial(); //46033c clears before Skip.
 return impl_->skip_tutorial_message_v118(error);
}
bool OriginalUiSession::source_integer_string_v83(std::int32_t id,std::string& text,std::string& error){
 bool is_null{};if(!impl_->localization.integer_string(id,impl_->text_services(),text,is_null,error))return false;
 if(is_null)text.clear();return true;
}
bool OriginalUiSession::character_inventory_preview_v4(ui::SwfMovie& movie,std::string& error){
 return movie.menu_display_callback("_root.menu_InventorySheetMain.avatarpane",impl_.get(),[](void* raw,const ui::SwfDraw& pane,std::string& e){
  auto& self=*static_cast<Impl*>(raw);
  return self.gpu.scene_pane(pane.rect,raw,[](void*,int width,int height,std::string& failure){return model_renderer::draw_character_inventory_preview_v4(width,height,failure);},e);
 },error);
}
bool OriginalUiSession::character_parsed_string_v4(const gameswf::fn_call& call,std::string& error){
 return ui::swf_menu_parsed_string_v1(call,impl_.get(),[](void* raw,const std::string& symbol,const std::vector<ui::LocalizationArgumentV1>& args,std::string& out,std::string& e){
  auto& self=*static_cast<Impl*>(raw);const auto live=model_renderer::player_gameplay_binding();
  if(!live.active||!live.world_owner){e="Parsed character string requires the current World";return false;}
  auto env=live.text_environment;env.localization=self.text_services();
  return self.localization.parsed_string_v4(symbol,args,env,out,e);
 },error);
}
bool OriginalUiSession::character_swap_hud_v4(const char* callback,std::string& error){
 if(!callback||!impl_->gameplay||!impl_->gameplay_activated){error="Required same active gameplay HUD for equipment swap";return false;}
 if(!std::strcmp(callback,"DisplayRightHud"))return impl_->gameplay->activate(error);
 if(!std::strcmp(callback,"FillActionIcon"))return impl_->gameplay->refresh_action_icon(error);
 error="Unsupported source equipment-swap HUD callback";return false;
}
OriginalUiSession::~OriginalUiSession(){
    // Process-manager backend outlives the HUD unique resource generation.
    // Clear its actual binding/pins before disposing this slot's facade.
    if(impl_->movie){
        const auto id=reinterpret_cast<std::uintptr_t>(impl_->movie.get());std::string failure;
        bool cleared=true;
        if(flash_manager_->scanned_fx()==id)cleared=flash_manager_->reset_scan_for_anims(id,failure);
        else if(flash_manager_->backend()->bound_movie_v92()==id)cleared=flash_manager_->discard_unscanned_hud(id,failure);
        if(!cleared)__android_log_print(ANDROID_LOG_ERROR,tag,"Required Flash process shutdown cleanup rejected | %s",failure.c_str());
    }

    // The platform pins this real resource owner. Release the owned graph and
    // platform before the session to break that deliberate lifetime cycle.
    impl_->hurt_pulse.release();impl_->hurt_reported_frame=impl_->hurt_reported_outer=-1;
    impl_->status_timeline.release();
    impl_->gameplay.reset();impl_->combat_flash.reset();impl_->enemy.reset();impl_->status.reset();impl_->movie.reset();impl_->fonts.reset();impl_->gfnt_text_backend_v1.reset();impl_->gameplay_bridge.reset();impl_->gameplay_queries.reset();
    if(impl_->menu_camera_v93)impl_->menu_camera_v93->deleted=true;
    impl_->menu_camera_v93.reset();
}
bool OriginalUiSession::initialize(AAssetManager* manager,std::string& error) {
    try{impl_->manager=manager;impl_->assets.manager(manager);impl_->gpu.initialize(manager,resources::ResourceScopeV37::swf_gameplay);impl_->report_frame=true;error.clear();return true;}
    catch(const std::exception& e){error=e.what();impl_->selected=false;return false;}
}
void OriginalUiSession::bind_platform_music(std::function<bool(std::int32_t&,std::string&)> callback){impl_->platform_music=std::move(callback);}
bool OriginalUiSession::load_health_panel(const std::string& directory,std::string& error) {
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
bool OriginalUiSession::render(int width,int height,std::string& error) {
    if(!active()){error="Original UI inspection owner inactive";return false;}
    if(!impl_->viewport(width,height,error)||!impl_->movie->display_source_clip(panel,error)){
        const auto failure=error;std::string cleanup;impl_->reset_failed(cleanup);
        error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
    }
    if(impl_->report_frame){
        __android_log_print(ANDROID_LOG_INFO,tag,"Original health panel submitted | viewport %d %d | strips %u | lines %u | masks %u | font uploads %u | bitmaps %u | strings %u | core diagnostics %u | authored initial state | game updates/input unconnected",width,height,impl_->strips,impl_->lines,impl_->masks,impl_->glyph_uploads,impl_->bitmap_uploads,impl_->string_calls,impl_->core_errors);
        impl_->report_frame=false;
    }
    return true;
}
bool OriginalUiSession::active() const{return impl_->selected&&impl_->loaded;}
bool OriginalUiSession::overlays_player() const{return impl_->selected&&impl_->live_player;}
void OriginalUiSession::deactivate(){impl_->cancel_attack_v46();impl_->hud_pointers.clear();impl_->world_pointers.clear();model_renderer::world_touch_cancel();impl_->joystick={};model_renderer::authored_hud_heading(0,0,false);impl_->selected=false;}

bool OriginalUiSession::attach_player(const std::string& directory,std::string& error){
    if(directory.empty()||directory.front()!='/'){error="Required private HUD directory unavailable";return false;}
    impl_->cancel_attack_v46();
    impl_->directory=directory;impl_->live_player=true;impl_->selected=true;impl_->report_frame=true;
    impl_->gameplay_activated=false;impl_->displayed_skill_ids={{-2,-2,-2}};
    impl_->hud_pointers.clear();impl_->world_pointers.clear();model_renderer::world_touch_cancel();impl_->joystick={};model_renderer::authored_hud_heading(0,0,false);
    const auto player=model_renderer::player_gameplay_binding();
    if(!player.settings){error="Authored gameplay HUD requires the retained Application settings";return false;}
    if(!player.settings->has_option("HUDStyle"))player.settings->initialize_defaults();
    // Explicit development session selection of the original fixed-buttons,
    // left-joystick layout. This is _initSettings + setOption, not file loading.
    if(!player.settings->set_option("HUDStyle",2)){error="Original HUDStyle option absent from actual descriptors";return false;}
    if(!player.settings->set_option("DPad",1)){error="Original DPad option absent from actual descriptors";return false;}
    impl_->hud_style=player.settings->option("HUDStyle");
    impl_->pending_text.clear();if(impl_->combat_flash)impl_->combat_flash->stop_all();impl_->reported_combat_active=0;
    error.clear();return true;
}
bool OriginalUiSession::prepare_player_frame(int width,int height,std::string& error){
    if(!overlays_player()){error="Combat text requires active player HUD";return false;}
    auto& self=*impl_;
    if(!self.loaded){self.driver_width=width;self.driver_height=height;if(!self.load(error)){
        const auto failure=error;std::string cleanup;self.reset_failed(cleanup);error=failure;return false;
    }}
    if(!self.viewport(width,height,error))return false;
    const auto live=model_renderer::player_gameplay_binding();
    if(!self.source_menu_owner_v94&&self.gameplay_activated&&live.active&&live.life&&!live.life->dead){
        // Exact source held dispatch, within the current authored UI adapter.
        // Whole HUDControls cache8/Level198 initialization is not fabricated.
        if(!self.dispatch_attack_v46(error))return false;
        if(!ui::authored_joystick_update_v1(self.joystick,self.joystick_services(),error))return false;
    }else if(!self.source_menu_owner_v94)self.cancel_attack_v46();
    return true;
}
bool OriginalUiSession::activate_source_player_v67(const model_renderer::PlayerGameplayBinding& player,
 const std::shared_ptr<void>& manager,std::string& error){
 auto& self=*impl_;const auto id=reinterpret_cast<std::uintptr_t>(self.movie.get());
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!player.active||!player.world_owner||!player.character||!player.save||!player.settings||!player.gear||
    !player.gear->ready()||!self.loaded||!self.movie||!self.movie->player_identity()||
    !same(self.source_menu_owner_v94,manager)||!self.source_info_hud_v62.fields().initialized||
    self.source_info_hud_v62.fields().render_fx!=id||self.source_controls_root658_v62!=id||
    !self.source_controls_constructed_v62||!self.source_controls_cache8_v62||
    !self.source_hud_transport_v62||!self.source_hud_world_v62||!self.gameplay){
  error="Source HUD activation requires same loaded player/profile/Gear, MenuManager and completed InfoHUD/controls caches";return false;
 }
 if(!player.settings->has_option("HUDStyle")||!player.settings->has_option("DPad")){
  error="Source HUD needs actual loaded settings descriptors";return false;
 }
 self.hud_style=player.settings->saved_option("HUDStyle");
 // Native AS refresh/initialization already ran in genuine LoadMenu3. This
 // binds the same shape-selected touch facade without replaying activation,
 // resetting settings, reloading the movie or advancing any timeline.
 if(!self.gameplay->bind(self.hud_style,error))return false;
 ui::AuthoredHudPortraitAlignmentV5 alignment;
 if(!ui::authored_hud_portrait_align_v5(*self.movie,*self.gameplay,alignment,error))return false;
 self.selected=self.live_player=self.gameplay_activated=true;self.report_frame=true;
 error.clear();return true;
}
bool OriginalUiSession::render_source_player_v67(int width,int height,std::string& error){
 auto& self=*impl_;
 if(!overlays_player()||!self.loaded||!self.movie||!self.source_menu_owner_v94||!self.gameplay_activated){
  error="Required current source gameplay HUD presentation";return false;
 }
 if(!self.viewport(width,height,error)||!self.movie->display_source_stage_clip_v5("_root.menu_HUD_0",error))return false;
 // InfoHUD/HUDControls and movie timers belong to the GS MenuManager update.
 // Display does not run the development sheet/target snapshot update again.
 if(!ui::display_authored_hurt_corners_v7(*self.movie,nullptr,error))return false;
 model_renderer::SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;
 if(!model_renderer::borrow_source_campaign_candidate_v55(candidate,error)||
    !model_renderer::borrow_source_campaign_condition_world_v70(candidate,source,error)||
    !candidate.level||!candidate.application||!source->debug||!source->debug_files){
  if(error.empty())error="Required same source Flash frame Application/Level/Debug";return false;
 }
 bool disabled{};if(ui::combat_flash_debug_gate_v1(&disabled,source->debug.get(),source->debug_files)!=1){
  error="Required source IsDisablingFlashAnimation Debug query";return false;
 }
 auto manager=self.flash_manager.lock();if(!manager||!manager->menu_update_owned()){
  error="Source Flash timeline must remain owned by actual MenuManager.Update";return false;
 }
 model_renderer::CombatTextFrameV1 frame;
 frame.application_dt=candidate.application->source_loading_v55().dt8c;
 frame.level_load_phase=candidate.level->constructor_fields_v3().field130;
 frame.debug_disabled=disabled;frame.tick=false; //Display never advances source timers.
 return render_combat_text(frame,error);
}
bool OriginalUiSession::movie_services(const ui::SwfServices& application,ui::SwfServices& out,std::string& error,bool load_hud){
    auto& self=*impl_;
    if(!application.native_owner||!application.native_action){error="Authored movie requires retained real application callback owner";return false;}
    if(load_hud&&!self.loaded&&!self.load(error)){
        const auto failure=error;std::string cleanup;self.reset_failed(cleanup);
        error=failure+(cleanup.empty()?"":"; cleanup: "+cleanup);return false;
    }
    try {
        auto lease=std::make_shared<Impl::MoviePlatformLease>(impl_,application);
        auto services=lease->services();
        ui::SwfTextFontPlatformV1 platform({impl_.get(),Impl::source_font_read_gfnt_v1,Impl::font_diagnostic},
            services,lease,lease->bitmap->backends(),1.f);
        platform.policy().renderer_feature=[lease](const ui::edit_text_display_v1::Command& command,std::string& e){
            if(command.kind==ui::edit_text_display_v1::Command::grid_fit){lease->resources->gpu.set_grid_fit(command.enabled);return true;}
            e="Required original text render-cache connection";return false;
        };
        out=platform.services();error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool OriginalUiSession::character_panel_services(const ui::SwfServices& application,ui::AuthoredCharacterPanelServicesV2& out,std::string& error,bool load_hud){
    ui::SwfServices platform;if(!movie_services(application,platform,error,load_hud))return false;
    out.movie=std::move(platform);out.localization.text=&impl_->localization;
    out.localization.strings=impl_->text_services();
    out.localization.strings.player_character=[](void*,std::uintptr_t& character,std::string&){
        const auto live=model_renderer::player_gameplay_binding();character=live.active?live.character:0;return true;
    };
    out.localization.strings.player_name=[](void*,std::uintptr_t character,std::string& name,std::string& e){
        const auto live=model_renderer::player_gameplay_binding();
        if(!live.active||!live.world_owner||!live.save||character!=live.character||live.save->character()!=character){e="Original character localization lost same selected Save";return false;}
        name=live.save->name();return true;
    };
    const auto retained=impl_;
    out.localization.debug=[retained](const char* key,std::string& e){return retained->debug_load(e)&&retained->debug_query(key,e);};
    // set_context stays the caller's actual RenderFX publication callback.
    // The movie/platform lease and debug closure both pin these resources.
    error.clear();return true;
}
bool OriginalUiSession::hud_pointer(int action,int pointer,float x,float y,std::string& command,std::string& error){
    auto& self=*impl_;command.clear();error.clear();
    if(!overlays_player()||!self.gameplay_activated||!self.gameplay)return true;
    using Control=ui::AuthoredHudControlV1;
    if(action==3){
        const bool owned_joystick=std::any_of(self.hud_pointers.begin(),self.hud_pointers.end(),[](const auto& entry){return entry.second==Control::joystick;});
        self.cancel_attack_v46();
        for(const auto& entry:self.hud_pointers){if(entry.second==Control::faery)model_renderer::player_gameplay_action(3,0);}
        self.hud_pointers.clear();self.world_pointers.clear();model_renderer::world_touch_cancel();const auto live=model_renderer::player_gameplay_binding();
        return !owned_joystick||ui::authored_joystick_release_v1(self.joystick,live.active&&live.controller.controller,self.joystick_services(),error);
    }
    if(action==0){
        if(self.hud_pointers.count(pointer)||self.world_pointers.count(pointer))return true;
        for(unsigned i=0;i<9;++i){
            ui::AuthoredHudGeometryV1 geometry;const auto control=Control(i);
            if(!self.gameplay->geometry(control,x,y,geometry,error))return false;
            if(!geometry.hit)continue;
            for(const auto& entry:self.hud_pointers)if(entry.second==control)return true;
            self.hud_pointers.emplace(pointer,control);
            __android_log_print(ANDROID_LOG_INFO,tag,"Authored HUD pointer | down %d | control %u | source character %d | screen %.1f %.1f",pointer,i,geometry.character_id,x,y);
            if(control==Control::attack){
                bool consumed{};self.held_dispatches_v46=0;
                if(!ui::hud_attack_event_v46(self.attack_controls_v46,4,consumed,error))return false;
                // Android may drain a short down/up pair before the next draw.
                // Dispatch while the genuine down is held; never after release.
                if(!self.dispatch_attack_v46(error))return false;
            }
            if(control==Control::faery){const auto report=model_renderer::player_gameplay_action(1,0);__android_log_print(ANDROID_LOG_INFO,tag,"Authored faery press | %s",report.c_str());}
            if(control==Control::joystick){
                ui::AuthoredHudGeometryV1 receiver;
                if(!self.gameplay->joystick_receiver_geometry(x,y,receiver,error)||!ui::authored_joystick_press_v1(self.joystick,receiver.local[0],receiver.local[1],error))return false;
            }
            return true;
        }
        self.world_pointers.emplace(pointer,true);
        return model_renderer::world_touch(x,y,false,error);
    }
    const auto world=self.world_pointers.find(pointer);
    if(world!=self.world_pointers.end()){
        if(action==1)return true; // World press remains owned during drag.
        if(action!=2){error="Unknown world pointer phase";return false;}
        self.world_pointers.erase(world);
        return model_renderer::world_touch(x,y,true,error);
    }
    const auto found=self.hud_pointers.find(pointer);if(found==self.hud_pointers.end())return true;
    const auto control=found->second;
    if(action==1){
        if(control==Control::joystick){
            ui::AuthoredHudGeometryV1 receiver;
            if(!self.gameplay->joystick_receiver_geometry(x,y,receiver,error))return false;
            if(!ui::authored_joystick_drag_v1(self.joystick,receiver.local[0],receiver.local[1],receiver.local_matrix[2],receiver.local_matrix[5],self.joystick_services(),error))return false;
        }
        return true;
    }
    if(action!=2){error="Unknown authored HUD pointer phase";return false;}
    self.hud_pointers.erase(found);
    if(control==Control::attack){
        ui::AuthoredHudGeometryV1 geometry;if(!self.gameplay->geometry(control,x,y,geometry,error))return false;
        bool consumed{};if(!ui::hud_attack_event_v46(self.attack_controls_v46,geometry.hit?6:7,consumed,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Source HUD held attack released | pointer %d | inside %u | command updates %llu",pointer,unsigned(geometry.hit),static_cast<unsigned long long>(self.held_dispatches_v46));return true;
    }
    if(control==Control::joystick){const auto live=model_renderer::player_gameplay_binding();return ui::authored_joystick_release_v1(self.joystick,live.active&&live.controller.controller,self.joystick_services(),error);}
    if(control==Control::faery){const auto report=model_renderer::player_gameplay_action(3,0);__android_log_print(ANDROID_LOG_INFO,tag,"Authored faery release | %s",report.c_str());return true;}
    ui::AuthoredHudGeometryV1 geometry;if(!self.gameplay->geometry(control,x,y,geometry,error))return false;
    if(!geometry.hit)return true;
    if(control==Control::character){command="character";return true;}
    if(control==Control::pause){command="pause";return true;}
    return self.gameplay->release(control,error);
}
std::shared_ptr<ui::FlashAnimManagerV92> OriginalUiSession::flash_anim_manager_v92()const noexcept{return flash_manager_;}
bool OriginalUiSession::hud_movie_slot_v93(ui::MenuMovieBorrowV58& out,std::string& e){
 out={};if(impl_->movie)out={impl_->movie,reinterpret_cast<std::uintptr_t>(impl_->movie.get())};e.clear();return true;
}
bool OriginalUiSession::hud_movie_virtual10_v93(std::uintptr_t expected,std::int32_t dt,bool flag,std::string& e){
 auto movie=impl_->movie;if(!movie||impl_->hud_deleted_v94||reinterpret_cast<std::uintptr_t>(movie.get())!=expected){e="Required same live HUD3 source update receiver";return false;}
 return movie->source_update_v93(dt,flag,e);
}
bool OriginalUiSession::deleting_hud_movie_v93(std::uintptr_t expected,std::string& e){
 auto& self=*impl_;if(!self.movie||self.hud_deleted_v94||reinterpret_cast<std::uintptr_t>(self.movie.get())!=expected||self.movie->player_identity()||flash_manager_->backend()->bound_movie_v92()==expected){e="HUD3 D0 requires same resource-unloaded/Flash-reset facade";return false;}
 self.hurt_pulse.release();self.status_timeline.release();self.gameplay.reset();self.enemy.reset();self.status.reset();self.combat_flash.reset();
 self.loaded=self.selected=self.gameplay_activated=false;self.hud_deleted_v94=true;
 self.pending_text.clear();self.borrowed_combat_string.clear();self.hud_pointers.clear();self.world_pointers.clear();e.clear();return true;
}
bool OriginalUiSession::clear_hud_movie_slot_v93(std::string& e){
 if(impl_->movie&&!impl_->hud_deleted_v94){e="HUD3 field clear precedes actual D0";return false;}
 impl_->movie.reset();impl_->hud_deleted_v94=false;e.clear();return true;
}
bool OriginalUiSession::claim_menu_transport_v93(std::shared_ptr<void> owner,std::string& e){
 auto& current=impl_->source_menu_owner_v94;
 if(!owner||(current&&(current.get()!=owner.get()||current.owner_before(owner)||owner.owner_before(current)))){e="HUD source transport requires same MenuManager owner";return false;}
 current=std::move(owner);e.clear();return true;
}
bool OriginalUiSession::release_menu_transport_v93(const std::shared_ptr<void>& owner,std::string& e){
 auto& current=impl_->source_menu_owner_v94;
 if(!owner||!current||current.get()!=owner.get()||current.owner_before(owner)||owner.owner_before(current)){e="HUD source release requires same MenuManager owner";return false;}
 if(impl_->source_hud_graph_v62){e="HUD source teardown during active InfoHUD scope";return false;}
 current.reset();impl_->hurt_pulse.release();impl_->status_timeline.release();
 impl_->source_hud_world_v62={};impl_->source_controls_v62={};impl_->source_hud_transport_v62.reset();e.clear();return true;
}
bool OriginalUiSession::hud_camera_slot_v93(ui::MenuCameraBorrowV93& out,std::string& e){
 out={};if(impl_->menu_camera_v93){out.actual_owner=impl_->menu_camera_v93;
  out.identity=reinterpret_cast<std::uintptr_t>(impl_->menu_camera_v93.get());}
 e.clear();return true;
}
bool OriginalUiSession::delete_hud_camera_v93(std::uintptr_t expected,std::string& e){
 const auto& camera=impl_->menu_camera_v93;
 if(impl_->movie||!camera||camera->deleted||reinterpret_cast<std::uintptr_t>(camera.get())!=expected){
  e="HUD paired-camera deleting callback requires SAME camera after primary deletion";return false;
 }
 camera->deleted=true;e.clear();return true;
}
bool OriginalUiSession::clear_hud_camera_slot_v93(std::string& e){
 if(impl_->menu_camera_v93&&!impl_->menu_camera_v93->deleted){e="HUD camera slot clear precedes camera D0";return false;}
 impl_->menu_camera_v93.reset();e.clear();return true;
}
bool OriginalUiSession::reset_flash_for_movie_v92(std::uintptr_t fx,std::string& e){return flash_manager_->reset_scan_for_anims(fx,e);}
bool OriginalUiSession::destroy_hud_movie_slot_v92(std::uintptr_t expected,std::string& e){
 return deleting_hud_movie_v93(expected,e)&&clear_hud_movie_slot_v93(e);
}
bool OriginalUiSession::destroy_hud_auxiliary_v92(std::string& e){
 if(impl_->movie){e="HUD auxiliary disposal requires deleted source movie slot";return false;}
 impl_->fonts.reset();impl_->gameplay_bridge.reset();impl_->gameplay_queries.reset();
 // Shared GPU/images/exports/gfnt backend and other movies stay retained.
 e.clear();return true;
}
model_renderer::CombatTextSinkV1 OriginalUiSession::combat_text_sink(){return {impl_.get(),Impl::combat_localized,Impl::combat_enqueue,Impl::combat_format_integer};}
bool OriginalUiSession::render_combat_text(const model_renderer::CombatTextFrameV1& frame,std::string& error){
    auto& self=*impl_;if(!self.loaded||!self.combat_flash){error="Combat flash retained owner unavailable";return false;}
    // Event values are copied before draw; projection uses this frame's camera.
    // The source queue itself retains its original twelve-context drop policy.
    while(!self.pending_text.empty()){
        const auto& p=self.pending_text.front();
        if(!self.combat_flash->play(p.style.c_str(),p.position.data(),p.numeric?nullptr:p.text.c_str(),p.number,p.color,p.numeric,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Source combat text queued | style %s | number %d | text %s | color %08x | position %.4f %.4f %.4f",p.style.c_str(),p.number,p.text.c_str(),unsigned(p.color),p.position[0],p.position[1],p.position[2]);
        self.pending_text.erase(self.pending_text.begin());
    }
    auto process_manager=self.flash_manager.lock();if(!process_manager){error="Required process Flash update ownership";return false;}
    // MenuManager.Update owns the timer only after actual root claim succeeds.
    if(frame.tick&&!process_manager->menu_update_owned()&&!self.combat_flash->update(frame.application_dt,frame.level_load_phase,error))return false;
    self.combat_quads=0;self.drawing_combat=true;
    const bool drawn=self.combat_flash->draw(frame.debug_disabled,error);self.drawing_combat=false;
    if(!drawn)return false;
    unsigned active=0;for(const auto& c:self.combat_flash->queue().contexts())if(c.flags&1)++active;
    if(active!=self.reported_combat_active||(self.combat_quads&&!self.reported_combat_glyphs)){
        __android_log_print(ANDROID_LOG_INFO,tag,"Source combat text submitted | active %u | native glyph quads %u | application dt %u | load phase %d | debug disabled %d | first glyph twips %.4f %.4f | extent %.4f %.4f | ARGB %08x",active,self.combat_quads,frame.application_dt,frame.level_load_phase,frame.debug_disabled,self.first_combat_quad[0],self.first_combat_quad[1],self.combat_quad_extent[0],self.combat_quad_extent[1],self.combat_quad_rgba);
        self.reported_combat_active=active;
    }
    if(self.combat_quads)self.reported_combat_glyphs=true;if(!active)self.reported_combat_glyphs=false;
    return true;
}
bool OriginalUiSession::render_player(int width,int height,const std::int32_t* sheet,std::size_t count,
                                     std::uintptr_t character,std::string& error,const ui::EnemyHudWorldBorrowV1* enemy){
    if(!overlays_player()){error="Connected player HUD inactive";return false;}
    auto& self=*impl_;
    try{
        std::optional<dh2::perf::Scope> update_scope;update_scope.emplace(dh2::perf::Phase::ui_update);
        if(!self.loaded){self.driver_width=width;self.driver_height=height;if(!self.load(error))throw std::runtime_error(error);}
        if(!self.viewport(width,height,error))throw std::runtime_error(error);
        const auto live=model_renderer::player_gameplay_binding();
        if(!live.active||!live.save||!live.settings||!live.attack_fields||live.attack_fields->owner!=live.character||live.attack_fields->object_of_interest_type<-128||live.attack_fields->object_of_interest_type>127)throw std::runtime_error("Authored HUD lost the same live player/settings/OOI owner");
        if(!self.gameplay_activated){
            if(!self.gameplay->bind(self.hud_style,error)||!self.status->bind(hud_sha,self.gameplay->menu_path().c_str(),error)||!self.gameplay->activate(error))throw std::runtime_error(error);
            float width{};
            if(!self.gameplay->joystick_background_width(width,error)||!ui::authored_joystick_initialize_v1(self.joystick,width,error))throw std::runtime_error(error);
            self.gameplay_activated=true;
        }
        if(enemy&&!update_enemy(*enemy,error))throw std::runtime_error(error);
        std::array<std::int32_t,3> skill_ids{};
        for(unsigned i=0;i<3;++i){const auto row=live.save->skill_in_slot(i);skill_ids[i]=row<0?-1:live.save->skill_id(unsigned(row));}
        if(skill_ids!=self.displayed_skill_ids){
            if(!self.gameplay->refresh_skills(error))throw std::runtime_error(error);
            __android_log_print(ANDROID_LOG_INFO,tag,"Original HUD same Save skill bindings | rows %d %d %d | ids %d %d %d | controls %s | %s | %s",live.save->skill_in_slot(0),live.save->skill_in_slot(1),live.save->skill_in_slot(2),skill_ids[0],skill_ids[1],skill_ids[2],self.gameplay->control_path(ui::AuthoredHudControlV1::skill1).c_str(),self.gameplay->control_path(ui::AuthoredHudControlV1::skill2).c_str(),self.gameplay->control_path(ui::AuthoredHudControlV1::skill3).c_str());
            self.displayed_skill_ids=skill_ids;
        }
        const auto values=model_renderer::gameplay_hud_snapshot(live);
        if(values.size()!=27||values[25])throw std::runtime_error("Required authored HudInfos service failed at operation "+std::to_string(values.size()==27?values[26]:-1));
        std::array<std::int32_t,17> infos{};std::copy_n(values.begin(),17,infos.begin());
        if(!self.gameplay->update(infos,live.save->class_id(),live.settings->saved_option("DPad")!=0,error))throw std::runtime_error(error);
        ui::AuthoredHudPortraitAlignmentV5 alignment;std::string alignment_error;
        if(ui::authored_hud_portrait_align_v5(*self.movie,*self.gameplay,alignment,alignment_error)){
            if(self.report_frame)__android_log_print(ANDROID_LOG_INFO,tag,"Modern HUD portrait paint alignment | source shape %d | before %.3f %.3f | ring aperture %.3f %.3f | after %.3f %.3f | local delta %.3f %.3f",alignment.shape,alignment.before_world[0],alignment.before_world[1],alignment.target_world[0],alignment.target_world[1],alignment.after_world[0],alignment.after_world[1],alignment.delta_local[0],alignment.delta_local[1]);
        }else if(self.report_frame)__android_log_print(ANDROID_LOG_WARN,tag,"Modern HUD portrait alignment unavailable | %s",alignment_error.c_str());
        const auto action_icon=self.gameplay->cached_action_icon();
        if(!self.gameplay->update_action_icon(static_cast<std::int8_t>(live.attack_fields->object_of_interest_type),error))throw std::runtime_error(error);
        if(action_icon!=self.gameplay->cached_action_icon())__android_log_print(ANDROID_LOG_INFO,tag,"Original HUD action icon | same character %p | raw OOI type %d | icon %d",reinterpret_cast<void*>(live.character),live.attack_fields->object_of_interest_type,self.gameplay->cached_action_icon());
        if(!self.status->update(sheet,count,character,error))throw std::runtime_error(error);
        model_renderer::CombatTextFrameV1 clock;
        if(!model_renderer::combat_text_frame(clock,error))throw std::runtime_error(error);
        ui::AuthoredHurtPulseDiagnosticV7 pulse;
        ui::AuthoredStatusDiagnosticV27 status_animation;
        if(!self.hurt_pulse.update(*self.movie,clock.application_dt,clock.tick&&!self.source_menu_owner_v94,pulse,error)||
           !self.status_timeline.update(*self.movie,*self.gameplay,clock.application_dt,clock.tick&&!self.source_menu_owner_v94,status_animation,error))throw std::runtime_error(error);
        update_scope.reset();
        if(!self.gameplay->display(error))throw std::runtime_error(error);
        ui::HurtCornersLayoutV7 hurt_layout;
        if(!ui::display_authored_hurt_corners_v7(*self.movie,&hurt_layout,error))throw std::runtime_error(error);
        if(pulse.outer_frame!=self.hurt_reported_outer||
           (pulse.health_alpha>0&&pulse.advanced&&(pulse.pulse_frame==0||pulse.pulse_frame==18)&&pulse.pulse_frame!=self.hurt_reported_frame)){
            __android_log_print(ANDROID_LOG_INFO,tag,"Original critical health pulse | HP %d %d | health frame %d alpha %.6f | pulse frame %d of %d alpha %.6f | tick %d dt %u | full display %.3f %.3f %.3f %.3f | paint %.3f %.3f %.3f %.3f",sheet[36],sheet[38],pulse.outer_frame,pulse.health_alpha,pulse.pulse_frame,pulse.pulse_frames,pulse.pulse_alpha,clock.tick,clock.application_dt,hurt_layout.display[0],hurt_layout.display[1],hurt_layout.display[2],hurt_layout.display[3],hurt_layout.paint_bounds[0],hurt_layout.paint_bounds[1],hurt_layout.paint_bounds[2],hurt_layout.paint_bounds[3]);
            self.hurt_reported_outer=pulse.outer_frame;self.hurt_reported_frame=pulse.pulse_frame;
        }
        const auto frames=self.status->frames();
        if(self.report_frame||frames!=self.reported_frames){
            if(self.report_frame){
                ui::AuthoredHudPortraitDiagnosticV4 portrait;std::string measurement_error;
                if(ui::authored_hud_portrait_v4(*self.movie,*self.gameplay,portrait,measurement_error)){
                    const auto& b=portrait.button.world.value;const auto& i=portrait.bitmap_container.local.value;const auto& p=portrait.portrait.local.value;const auto& s=portrait.shape_world_bounds;const auto& d=portrait.display_rectangle;const auto& v=portrait.viewport;
                    __android_log_print(ANDROID_LOG_INFO,tag,"Original HUD portrait source geometry | button %d matrix %.6f %.6f %.3f %.6f %.6f %.3f | btimg %d local %.6f %.6f %.3f %.6f %.6f %.3f | HudChar %d frame %d local %.6f %.6f %.3f %.6f %.6f %.3f | shape %d bounds %.3f %.3f %.3f %.3f | display %.3f %.3f %.3f %.3f | viewport %d %d %d %d",portrait.button.id,b[0],b[1],b[2],b[3],b[4],b[5],portrait.bitmap_container.id,i[0],i[1],i[2],i[3],i[4],i[5],portrait.portrait.id,portrait.portrait.frame,p[0],p[1],p[2],p[3],p[4],p[5],portrait.actual_shape_id,s[0],s[1],s[2],s[3],d[0],d[1],d[2],d[3],v[0],v[1],v[2],v[3]);
                }else __android_log_print(ANDROID_LOG_WARN,tag,"Original HUD portrait measurement unavailable | %s",measurement_error.c_str());
            }
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
bool OriginalUiSession::update_enemy(const ui::EnemyHudWorldBorrowV1& borrow,std::string& error){
    auto& self=*impl_;
    if(!overlays_player()||!self.loaded||!self.enemy){error="Connected enemy HUD movie unavailable";return false;}
    if(!self.enemy->update(borrow,error,self.hud_style))return false;
    if(self.enemy->target()!=self.reported_enemy||self.enemy->hp_frame()!=self.reported_enemy_frame){
        __android_log_print(ANDROID_LOG_INFO,tag,
            "Connected enemy HUD | world %p | target %p | visible %d | name %s | level %s | HP frame %d | retained original SWF",
            reinterpret_cast<void*>(borrow.world),reinterpret_cast<void*>(self.enemy->target()),self.enemy->visible(),
            self.enemy->name().c_str(),self.enemy->level().c_str(),self.enemy->hp_frame());
        self.reported_enemy=self.enemy->target();self.reported_enemy_frame=self.enemy->hp_frame();
    }
    return true;
}
}
