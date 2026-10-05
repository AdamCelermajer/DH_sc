#include "original_ui_session.hpp"
#include "original_ui_assets.hpp"
#include "swf_gpu.hpp"
#include "swf_hud_freetype_provider.hpp"
#include "swf_font_resolver.hpp"
#include "localization.hpp"
#include "character_design_services.hpp"
#include "script_constants.hpp"
#include "swf_texture.hpp"
#include "swf_input_history.hpp"
#include "swf_frame_connection.hpp"
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
#include "localization_parse_ex_v1.hpp"
#include "gameswf/gameswf_as_classes/as_array.h"
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
struct AssetClose {void operator()(AAsset* value)const{if(value)AAsset_close(value);}};
struct ConstantsDelete {void operator()(dh2_script_constants* value)const{dh2_script_constants_destroy(value);}};
struct DebugDelete {void operator()(character::DebugSwitches* value)const{dh2_character_debug_destroy(value);}};
}
struct OriginalUiSession::Impl {
    AAssetManager* manager{};
    OriginalUiAssets assets;
    SwfGpu gpu;
    std::string directory,font_failure,provider_failure;
    std::unique_ptr<dh2_script_constants,ConstantsDelete> constants{dh2_script_constants_create()};
    std::unique_ptr<character::DebugSwitches,DebugDelete> debug{dh2_character_debug_create()};
    character::DebugFileServices24 debug_files{this,debug_open,debug_close};
    ui::Localization localization;
    ui::GameOptionTableV1 option_table;
    std::unique_ptr<ui::OwnedHudSettingsV1> settings;
    std::unique_ptr<ui::SettingsNativeFilesV1> settings_files;
    std::int32_t language_selection=-1;
    dh2::data::CharacterTable menu_characters;
    dh2::data::LevelTables menu_levels;
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
    // Initial owned navigation slice. Other registered native menu types,
    // shared-renderer settings and transition animations remain pending.
    std::vector<std::string> menu_stack;
    std::string active_menu_path()const{return "_root."+(menu_stack.empty()?std::string("menu_MainMenu"):menu_stack.back());}
    static bool shared_state(const std::string& name){return name=="menu_HelpButtons"||name=="menu_Help"||name=="menu_About"||name=="menu_Options";}
    ui::SwfMovie* menu_movie(const std::string& name)const{return shared_state(name)?shared_menu_movie.get():movie.get();}
    ui::SwfMovie* active_menu_movie()const{return menu_stack.empty()?movie.get():menu_movie(menu_stack.back());}
    ui::SwfMovie* input_dispatch_movie{};
    std::int32_t last_menu_dt{};
    int class_index=0;
    std::uintptr_t class_left=0,class_right=0;
    std::chrono::steady_clock::time_point frame_time{};
    MenuFrameClock menu_clock;
    int driver_width=480,driver_height=320;
    ui::FlashCamera40 camera{};
    ui::FlashCamera40 shared_camera{};
    std::array<std::int32_t,5> reported_frames{{-1,-1,-1,-1,-1}};
    unsigned glyph_uploads=0,bitmap_uploads=0,string_calls=0,core_errors=0,packed_glyphs=0;
    unsigned strips=0,lines=0,masks=0;
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
    std::unique_ptr<ui::SwfHudFreetypeProvider> fonts;
    std::unique_ptr<ui::SwfMovie> movie;
    std::unique_ptr<ui::SwfMovie> shared_menu_movie;
    std::unique_ptr<ui::PlayerStatusHud> status;
    std::array<std::int32_t,4> front_rectangle()const{
        const int w=std::min(driver_width,driver_height*3/2),h=std::min(driver_height,driver_width*2/3);
        return {(driver_width-w)/2,(driver_height-h)/2,w,h};
    }
    static bool input_accepts(void*,ui::SwfEvent48&,bool& accepted,std::string&){
        // MenuBase::CanHandleEvent, 0x41f3fc, returns true.
        accepted=true;return true;
    }
    static bool input_advance(void* context,gameswf::root* root,float seconds,bool flag,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        return self.frame_owner->frames.advance(root,seconds,flag,error);
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
        const bool delivered=self.menu_stack.empty()||self.menu_stack.back()=="menu_MainMenu"
            ?self.frame_owner->main_events.main(event,services,error)
            :self.frame_owner->main_events.base(event,services,error);
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
        if(!movie->update_viewport(camera,error))return false;
        if(shared_menu_movie&&!shared_menu_movie->update_viewport(shared_camera,error))return false;
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
        ui::LocalizationServices services{this,text_open,text_close,localization_debug,constant,no_player,unavailable_player_name};
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
    static bool save_settings(void* context,std::string& error){
        auto& self=*static_cast<Impl*>(context);if(!self.settings||self.directory.empty()){error="Front settings save owner unavailable";return false;}
        const auto bytes=self.settings->serialized();const std::string path=self.directory+"/dh2_settings.savegame",temporary=path+".front.tmp";
        auto* file=std::fopen(temporary.c_str(),"wb");if(!file){error="Unable to open private settings output";return false;}
        const bool written=std::fwrite(bytes.data(),1,bytes.size(),file)==bytes.size();const bool closed=std::fclose(file)==0;
        if(!written||!closed){std::remove(temporary.c_str());error="Unable to write private settings output";return false;}
        if(std::rename(temporary.c_str(),path.c_str())){std::remove(temporary.c_str());error="Unable to publish private settings output";return false;}
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
        auto& self=*static_cast<Impl*>(context);if(self.live_player){error="Settings HUD refresh requires attached player owner";return false;}return true;
    }
    static bool input_behavior(void* context,std::int32_t slot,bool rollover,std::string& error){
        auto& self=*static_cast<Impl*>(context);auto* target=slot==0?self.shared_menu_movie.get():slot==1?self.movie.get():nullptr;
        if(!target){error="Menu input renderer slot unconnected";return false;}return target->menu_input_behavior(rollover?0x84:4,error);
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
    static bool menu_slot_details(void* context,std::uint32_t slot,bool occupied,std::int32_t difficulty,
        ui::SwfFrontSaveSlotDetailsV1& details,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!occupied){details={};error.clear();return true;}
        dh2::data::CampaignProfileFileV1 file;
        if(!dh2::data::read_campaign_profile_v1(self.directory,slot,file,error))return false;
        dh2::data::MenuProfileMetadataV1 metadata;
        // Canonical QEST provider is pending in the gameplay owner. A present
        // QEST fails explicitly; fresh source profiles contain seven sections.
        dh2::data::MenuProfileMetadataServicesV1 load_services{&self,menu_store_difficulty,nullptr};
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
    static bool native_action(void* context,const char* name,const gameswf::fn_call& fn,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!std::strcmp(name,"NativeGetParsedString")){
            if(fn.nargs!=2||(!fn.arg(0).is_string()&&!fn.arg(0).is_object())||!fn.arg(1).is_object())return true;
            if(!fn.result){error="Parsed string result unavailable";return false;}
            auto* array=gameswf::cast_to<gameswf::as_array>(fn.arg(1).to_object());
            if(!array){error="Parsed string requires actual AS Array";return false;}
            std::vector<ui::LocalizationArgumentV1> args;
            for(int i=0;i<array->size();++i){
                const auto& value=array->m_array[i];ui::LocalizationArgumentV1 arg;
                if(value.is_string()||value.is_object()){arg.has_text=true;arg.text=value.to_xstring();}
                else if(value.is_number()){const double number=value.to_number();if(!std::isnan(number))arg.number=static_cast<float>(number);}
                args.push_back(std::move(arg));
            }
            std::string text;
            if(!self.localization.parsed_string(fn.arg(0).to_xstring(),args,self.text_services(),text,error))return false;
            fn.result->set_string(text.c_str());
            __android_log_print(ANDROID_LOG_INFO,tag,"Original parsed menu string | symbol %s | args %zu | text %s",fn.arg(0).to_xstring(),args.size(),text.c_str());return true;
        }
        if(!std::strcmp(name,"NativeGetSaveSlotDetails")){
            ui::SwfFrontSaveSlotServicesV1 services{&self,menu_slot_exists,menu_slot_details};
            return ui::swf_front_save_slot_details(fn,services,error);
        }
        for(const auto* action:{"NativeGetOptionParameters","NativeSetOptions","NativeLoadSettings","NativeSaveSettings","NativeEnterOptionMenu","NativeRefreshHudManager","NativeChangeRolloverInputBehavior","NativeIsJapaneseVersion","NativeIsKorean"})
            if(!std::strcmp(name,action))return ui::swf_menu_settings_action(name,fn,self.option_services(),error);
        const ui::SwfMenuNavigationServicesV1 navigation{context,push_menu,pop_menu,pop_top_menu,pop_above_menu};
        if(!std::strcmp(name,"NativePushMenu"))return ui::swf_menu_push(fn,navigation,error);
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
        if(std::strcmp(name,"NativePlaySoundFX")){error="Unknown owned menu native action";return false;}
        // Original 0x43ae10: exactly one STRING/WIDE_STRING, lookup by name;
        // invalid arguments or absent ID are a genuine no-op. This core uses
        // its UTF-8 STRING representation (it has no separate wide-string tag).
        std::string requested;
        if(!ui::swf_menu_sound_argument(fn,requested))return true;
        for(std::size_t id=0;id<std::size(original_sounds);++id){
            const auto& record=original_sounds[id];
            if(requested!=record.name)continue;
            if(!record.menu_backend){error=std::string("Required sound backend unavailable: ")+requested;return false;}
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
        const char* paths[]={"menu_SelectClass.class_title.text","menu_SelectClass.class_description.text"};
        const char* symbols[]={titles[self.class_index],descriptions[self.class_index]};
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
        if(!graph.invoke(menu,menu,"CurrentClass",{ui::SwfAsValue::text(classes[self.class_index])},result,callable,error))return false;
        if(!callable){error="Required original CurrentClass callback absent";return false;}
        __android_log_print(ANDROID_LOG_INFO,tag,"Original class selection updated | index %d | class %s | preview actors pending",self.class_index,classes[self.class_index]);
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
        MenuChange hide{this,previous,false,false},show{this,next,true,pushed};
        if(!menu_movie(previous)->menu_action_script(&hide,change_menu,error)||
           !menu_movie(next)->menu_action_script(&show,change_menu,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Owned menu renderer selected | name %s | renderer %s",next.c_str(),shared_state(next)?"shared":"main");
        return true;
    }
    static bool push_menu(void* context,const char* name,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!name||self.menu_stack.empty()){error="Native menu stack unavailable";return false;}
        // GetMenuByName returns null for unregistered states. Explicitly log
        // the partial registration rather than presenting those paths as done.
        if(std::strcmp(name,"menu_info")&&std::strcmp(name,"menu_MainMenu")&&std::strcmp(name,"menu_EnterName")&&std::strcmp(name,"menu_SelectClass")&&!shared_state(name)){
            __android_log_print(ANDROID_LOG_WARN,tag,"Menu navigation not connected | requested %s",name);return true;
        }
        if(std::find(self.menu_stack.begin(),self.menu_stack.end(),name)!=self.menu_stack.end())return true;
        const auto previous=self.menu_stack.back();
        self.menu_stack.emplace_back(name);
        if(!self.transition_menu(previous,name,true,error)){self.menu_stack.pop_back();return false;}
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
    static bool shared_graph_start(void* context,const ui::SwfAsLease& lease,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
        if(!self.shared_frame_owner||!lease.player){error="Shared menu frame owner unavailable";return false;}
        if(!self.shared_frame_owner->history->bind(lease.player,error)||
           !self.shared_frame_owner->frames.bind(lease.player,self.shared_frame_owner->history,error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Original shared renderer frame/history bound before root construction");
        return true;
    }
    bool load(std::string& error) {
        if(!constants||!debug){error="Required native UI owner allocation failed";return false;}
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
        if(!localization.load({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
        if(front_screen=="main"){
            const char* suffixes[]={"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("data/character_properties")+suffixes[i]).c_str(),metadata[i],error))return false;
            if(!dh2::data::load_characters({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},menu_characters,error))return false;
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("data/levels")+suffixes[i]).c_str(),metadata[i],error))return false;
            if(!dh2::data::load_levels({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},menu_levels,error))return false;
            const char* inputs[]={"design_pyarray.bin","design_pyarraynames.bin","design_pystructnames.bin"};
            for(unsigned i=0;i<3;++i)if(!raw_asset((std::string("original-cache/data/pydata/")+inputs[i]).c_str(),metadata[i],error))return false;
            settings.reset();option_table=ui::GameOptionTableV1();
            if(!option_table.load_design_cache({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
            settings=std::make_unique<ui::OwnedHudSettingsV1>(option_table.borrow());settings_files=std::make_unique<ui::SettingsNativeFilesV1>(directory);
            if(!load_settings(this,error))return false;
        }
        fonts=std::make_unique<ui::SwfHudFreetypeProvider>(ui::SwfFontServices{this,font_read,font_diagnostic},1.f,bitmap_probe);
        movie=std::make_unique<ui::SwfMovie>();ui::SwfServices services;
        frame_owner=std::make_shared<FrameOwner>();
        services.native_owner=frame_owner;services.graph_start=graph_start;
        services.native_actions={"NativePlaySoundFX","NativePushMenu","NativePopMenu","NativePopAllAbove","NativeGetCreditMovement","NativeBackToHud","NativeGetParsedString"};services.native_action=native_action;
        if(front_screen=="main")services.native_actions.emplace_back("NativeGetSaveSlotDetails");
        if(front_screen=="main")for(const auto* action:{"NativeGetOptionParameters","NativeSetOptions","NativeLoadSettings","NativeSaveSettings","NativeEnterOptionMenu","NativeRefreshHudManager","NativeChangeRolloverInputBehavior","NativeIsJapaneseVersion","NativeIsKorean"})services.native_actions.emplace_back(action);
        services.context=this;services.read=movie_read;services.texture=texture;services.image=image;
        services.draw=draw;services.stencil=stencil;services.native_call=native;services.diagnostic=diagnostic;
        services.glyphs=fonts->borrowed_provider();
        if(front_screen=="main"){
            // Original RenderFX::Load (0x7ab7c0..0x7ab7e0) constructs a
            // separate player per renderer. Importing common definitions into
            // the main player does not activate the shared menu renderer.
            shared_frame_owner=std::make_shared<FrameOwner>();
            shared_menu_movie=std::make_unique<ui::SwfMovie>();
            auto shared_services=services;
            shared_services.native_owner=shared_frame_owner;
            shared_services.graph_start=shared_graph_start;
            if(!shared_menu_movie->load({},"data/menus/dqshared_droid.swf",shared_services,error))return false;
            const ui::ViewportState64 shared_seed{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1.f,0,0};
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
        const char* movie_uri=front_screen=="main"?"data/menus/dqmenus_droid.swf":front_screen=="loading"?"data/menus/loadanims_droid.swf":"data/menus/dqhud_droid.swf";
        if(!movie->load({"data/menus/dqshared_droid.swf"},movie_uri,services,error))return false;
        if(shared_menu_movie){
            const auto primary=movie->player_identity(),shared=shared_menu_movie->player_identity();
            if(!primary||!shared||primary==shared){error="Menu renderers did not retain independent players";return false;}
            __android_log_print(ANDROID_LOG_INFO,tag,"Original menu renderer player identities | main %zx | shared %zx | distinct 1",primary,shared);
        }
        const ui::ViewportState64 seed{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1.f,0,0};
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
        gpu.abort();status.reset();shared_menu_movie.reset();movie.reset();shared_frame_owner.reset();frame_owner.reset();fonts.reset();loaded=false;selected=false;
        last_width=last_height=0;loading_bitmap_reported=false;reported_frames={{-1,-1,-1,-1,-1}};
        leases.clear();exports.clear();font_failure.clear();provider_failure.clear();
        menu_sounds.clear();
        menu_audio.clear();settings.reset();settings_files.reset();language_selection=-1;
        menu_stack.clear();
        input_dispatch_movie=nullptr;last_menu_dt=0;
        menu_clock.reset();
        glyph_uploads=bitmap_uploads=string_calls=core_errors=packed_glyphs=strips=lines=masks=0;
        return gpu.reset_images(error);
    }
};
OriginalUiSession::OriginalUiSession():impl_(std::make_unique<Impl>()){}
OriginalUiSession::~OriginalUiSession()=default;
bool OriginalUiSession::touch(float x,float y,int action,std::string& error){
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
    return selected->input_cursor({x,y,0.f,(action==0||action==2)?1:0},error);
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
    try{impl_->manager=manager;impl_->assets.manager(manager);impl_->gpu.initialize(manager);impl_->report_frame=true;error.clear();return true;}
    catch(const std::exception& e){error=e.what();impl_->selected=false;return false;}
}
bool OriginalUiSession::load_front_screen(const std::string& directory,const std::string& screen,std::string& error) {
    if(screen!="main"&&screen!="loading"){error="Unknown authored front screen";return false;}
    if(directory.empty()||directory.front()!='/'){error="Required private UI files directory unavailable";return false;}
    if(impl_->loaded&&impl_->front_screen!=screen){if(!impl_->reset_failed(error))return false;}
    impl_->front_screen=screen;impl_->directory=directory;impl_->live_player=false;
    impl_->selected=true;impl_->report_frame=true;impl_->frame_time=std::chrono::steady_clock::now();error.clear();return true;
}
bool OriginalUiSession::load_health_panel(const std::string& directory,std::string& error) {
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
            struct Dispatch {Impl& self;ui::SwfMovie* previous;~Dispatch(){self.input_dispatch_movie=previous;}} dispatch{*impl_,impl_->input_dispatch_movie};
            impl_->input_dispatch_movie=selected;
            if(!selected->input_advance(milliseconds,error))return false;
        }else if(!impl_->movie->advance(seconds,error))return false;
    }
    if(impl_->front_screen=="loading"){
        // GSInit::Draw 0x384ab4..0x384af4 supplies source rect 0,0,1280,752
        // separately from the animated loading overlay. Fit those original
        // image pixels to the modern surface without stretching the artwork.
        ui::SwfTexture splash;
        if(!Impl::texture(impl_.get(),"menus/splash_final.tga",0,0,splash,error))return false;
        if(splash.width<1280||splash.height<752){error="Original loading splash source rectangle exceeds atlas";return false;}
        const int w=std::min(width,height*1280/752),h=std::min(height,width*752/1280);
        ui::SwfDraw background; background.kind=ui::SwfDraw::begin;
        background.bounds[1]=1280; background.bounds[3]=752;
        background.viewport[0]=(width-w)/2; background.viewport[1]=(height-h)/2;
        background.viewport[2]=w; background.viewport[3]=h;
        if(!impl_->gpu.draw(background,error))return false;
        background.kind=ui::SwfDraw::bitmap_quad;
        background.fill.kind=ui::SwfFill::bitmap; background.fill.texture=splash;
        background.rect[1]=1280; background.rect[3]=752;
        background.uv_rect[1]=1280.f/splash.width; background.uv_rect[3]=752.f/splash.height;
        if(!impl_->gpu.draw(background,error))return false;
        background.kind=ui::SwfDraw::end;
        if(!impl_->gpu.draw(background,error))return false;
        if(impl_->report_frame)__android_log_print(ANDROID_LOG_INFO,tag,"Original startup splash submitted | source rect 0 0 1280 752 | aspect fit %d %d",w,h);
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
        if(!impl_->movie->display_clip("_root.menu_bg",
            (width-std::min(width,height*3/2))/2,(height-std::min(height,width*2/3))/2,
            std::min(width,height*3/2),std::min(height,width*2/3),error))return false;
    }
    auto* display_movie=impl_->front_screen=="main"?impl_->active_menu_movie():impl_->movie.get();
    if(!(front?display_movie->display_clip(path,
        (width-std::min(width,height*3/2))/2,(height-std::min(height,width*2/3))/2,
        std::min(width,height*3/2),std::min(height,width*2/3),error):impl_->movie->display_source_clip(path,error))){
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
bool OriginalUiSession::active() const{return impl_->selected&&(impl_->loaded||!impl_->front_screen.empty());}
bool OriginalUiSession::overlays_player() const{return impl_->selected&&impl_->live_player;}
void OriginalUiSession::deactivate(){impl_->selected=false;}

bool OriginalUiSession::attach_player(const std::string& directory,std::string& error){
    if(!impl_->front_screen.empty()){if(!impl_->reset_failed(error))return false;impl_->front_screen.clear();}
    if(directory.empty()||directory.front()!='/'){error="Required private HUD directory unavailable";return false;}
    impl_->directory=directory;impl_->live_player=true;impl_->selected=true;impl_->report_frame=true;
    error.clear();return true;
}
bool OriginalUiSession::render_player(int width,int height,const std::int32_t* sheet,std::size_t count,
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
