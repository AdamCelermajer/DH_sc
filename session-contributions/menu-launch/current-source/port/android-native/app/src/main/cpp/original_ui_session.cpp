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
#include "menu_native_event_v1.hpp"
#include "menu_frame_clock.hpp"
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
    std::map<std::uintptr_t,std::vector<std::uint8_t>> leases;
    std::uintptr_t next_lease=1;
    std::map<std::string,ui::SwfTexture> exports;
    bool selected=false,loaded=false,live_player=false,menu_string_flag=false,report_frame=true;
    std::string front_screen;
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
    static bool raw_event_position(void* context,int& x,int& y,std::string& error){
        return static_cast<Impl*>(context)->movie->input_raw_position(x,y,error);
    }
    static bool input_native_event(void* context,ui::SwfEvent48& event,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        ui::MenuNativeEventServicesV1 services;services.context=context;services.raw_position=raw_event_position;
        const bool delivered=self.frame_owner->main_events.main(event,services,error);
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
        return {this,text_open,text_close,localization_debug,constant,no_player,unavailable_player_name};
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
    static bool native_action(void* context,const char* name,const gameswf::fn_call& fn,std::string& error) {
        auto& self=*static_cast<Impl*>(context);
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
        fonts=std::make_unique<ui::SwfHudFreetypeProvider>(ui::SwfFontServices{this,font_read,font_diagnostic},1.f,bitmap_probe);
        movie=std::make_unique<ui::SwfMovie>();ui::SwfServices services;
        frame_owner=std::make_shared<FrameOwner>();
        services.native_owner=frame_owner;services.graph_start=graph_start;
        services.native_actions={"NativePlaySoundFX"};services.native_action=native_action;
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
        }
        // The base loading movie is authored for the splash atlas supplied by
        // this cache. The Droid variant's fill points into unrelated artwork.
        const char* movie_uri=front_screen=="main"?"data/menus/dqmenus_droid.swf":front_screen=="loading"?"data/menus/loadanims.swf":"data/menus/dqhud_droid.swf";
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
    if(action==3)return impl_->movie->input_cancel(x,y,error);
    // Android DOWN/MOVE retain the source cursor button; UP clears it.
    return impl_->movie->input_cursor({x,y,0.f,(action==0||action==2)?1:0},error);
}
std::string OriginalUiSession::consume_menu_sound(){
    auto& queue=impl_->menu_sounds;
    if(queue.empty())return {};
    auto value=std::move(queue.front());queue.pop_front();return value;
}
bool OriginalUiSession::debug_menu_sound(const std::string& probe,std::string& error){
    if(!impl_->loaded||impl_->front_screen!="main"){error="Menu sound probe requires the retained main movie";return false;}
    struct Probe {const std::string& name;} context{probe};
    return impl_->movie->action_script(&context,[](void* raw,ui::SwfAsGraph& graph,std::string& error){
        const auto& name=static_cast<Probe*>(raw)->name;
        ui::SwfAsValue root,receiver,result;bool callable=false;
        if(!graph.root_value(root,error))return false;
        if(name=="authored-options"){
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
    }
    const bool front=!impl_->front_screen.empty();
    if(front){
        const auto now=std::chrono::steady_clock::now();
        const auto elapsed=now-impl_->frame_time;
        const float seconds=std::min(.1f,std::chrono::duration<float>(elapsed).count());impl_->frame_time=now;
        if(impl_->front_screen=="main"){
            const auto milliseconds=impl_->menu_clock.advance(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed));
            // MenuManager::Update (0x42ecc0..0x42ed28) visits every loaded
            // renderer in slot order, including an inactive shared slot 0.
            // This supplies its real frame timeline only; native state/input
            // stack ownership remains a separate required connection.
            if(impl_->shared_menu_movie&&!impl_->shared_menu_movie->advance(float(milliseconds)*.001f,error))return false;
            if(!impl_->movie->input_advance(milliseconds,error))return false;
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
    const char* path=impl_->front_screen=="main"?"_root.menu_MainMenu":impl_->front_screen=="loading"?"_root.anim_loading_splash":panel;
    if(impl_->front_screen=="main"){
        // Context recreation revokes scene GL names while retaining the SWF
        // graph. Rebuild the scene before drawing the retained menu again.
        if(!model_renderer::active()){
            const auto scene=model_renderer::load_menu_background(impl_->manager);
            if(scene.find("3D upload OK")!=0){error=scene;return false;}
        }
        try{model_renderer::draw_menu_background(width,height);}
        catch(const std::exception& failure){error=failure.what();return false;}
        if(!impl_->movie->display_clip("_root.menu_bg",
            (width-std::min(width,height*3/2))/2,(height-std::min(height,width*2/3))/2,
            std::min(width,height*3/2),std::min(height,width*2/3),error))return false;
    }
    if(!(front?impl_->movie->display_clip(path,
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
