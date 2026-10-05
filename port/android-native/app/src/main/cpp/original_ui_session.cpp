#include "original_ui_session.hpp"
#include "original_ui_assets.hpp"
#include "swf_gpu.hpp"
#include "swf_hud_freetype_provider.hpp"
#include "swf_text_font_platform_v1.hpp"
#include "gfnt_text_backend_v1.hpp"
#include "hud_freetype_font_v2.hpp"
#include "swf_font_resolver.hpp"
#include "localization.hpp"
#include "hud_text_v1.hpp"
#include "character_design_services.hpp"
#include "script_constants.hpp"
#include "swf_texture.hpp"
#include "player_status_hud.hpp"
#include "combat_flash_swf_v1.hpp"
#include "textures.hpp"
#include <android/log.h>
#include <array>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <limits>
#include <map>
#include <stdexcept>
#include <utility>

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
struct OriginalUiSession::Impl:std::enable_shared_from_this<Impl> {
    AAssetManager* manager{};
    OriginalUiAssets assets;
    SwfGpu gpu;
    std::string directory,font_failure,provider_failure;
    std::unique_ptr<dh2_script_constants,ConstantsDelete> constants{dh2_script_constants_create()};
    std::unique_ptr<character::DebugSwitches,DebugDelete> debug{dh2_character_debug_create()};
    character::DebugFileServices24 debug_files{this,debug_open,debug_close};
    ui::HudTextV1 localization;
    std::map<std::uintptr_t,std::vector<std::uint8_t>> leases;
    std::uintptr_t next_lease=1;
    std::map<std::string,ui::SwfTexture> exports;
    bool selected=false,loaded=false,live_player=false,menu_string_flag=false,report_frame=true;
    int driver_width=480,driver_height=320;
    ui::FlashCamera40 camera{};
    std::array<std::int32_t,5> reported_frames{{-1,-1,-1,-1,-1}};
    unsigned glyph_uploads=0,bitmap_uploads=0,string_calls=0,core_errors=0,packed_glyphs=0;
    unsigned strips=0,lines=0,masks=0;
    int last_width=0,last_height=0;
    // Reverse destruction keeps every provider and owned texture alive until
    // the last movie and its reachable ActionScript graph have been released.
    std::unique_ptr<ui::SwfTextFontPlatformV1> fonts;
    std::unique_ptr<ui::SwfMovie> movie;
    std::unique_ptr<ui::PlayerStatusHud> status;
    std::unique_ptr<ui::EnemyStatusHudV1> enemy;
    std::unique_ptr<ui::CombatFlashSwfV1> combat_flash;
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
    bool load(std::string& error) {
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
        if(!localization.load({metadata[0].data(),metadata[0].size()},{metadata[1].data(),metadata[1].size()},{metadata[2].data(),metadata[2].size()},error))return false;
        movie=std::make_unique<ui::SwfMovie>();ui::SwfServices services;
        services.context=this;services.read=movie_read;services.texture=texture;services.image=image;
        services.draw=draw;services.stencil=stencil;services.native_call=native;services.diagnostic=diagnostic;
        fonts=std::make_unique<ui::SwfTextFontPlatformV1>(ui::SwfFontServices{this,source_font_read_gfnt_v1,font_diagnostic},services,shared_from_this(),initialize_gfnt_backend_v1(),1.f);
        fonts->policy().renderer_feature=[this](const ui::edit_text_display_v1::Command& command,std::string& error){
            if(command.kind==ui::edit_text_display_v1::Command::grid_fit){gpu.set_grid_fit(command.enabled);return true;}
            error="Required original text render-cache connection";return false;
        };
        if(!movie->load({"data/menus/dqshared_droid.swf"},"data/menus/dqhud_droid.swf",fonts->services(),error))return false;
        const ui::ViewportState64 seed{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1.f,0,0};
        if(!movie->connect_viewport(seed,{this,orientation,dimensions},error)||
           !movie->update_viewport(camera,error)||!movie->advance(0,error))return false;
        ui::SwfClipInfo clip;
        if(!movie->clip(panel,clip,error)||clip.id!=157){error="Required original health/mana parent differs";return false;}
        if(!font_failure.empty()){error=font_failure;return false;}
        status=std::make_unique<ui::PlayerStatusHud>(*movie);
        if(!status->bind(hud_sha,error))return false;
        enemy=std::make_unique<ui::EnemyStatusHudV1>(*movie,ui::EnemyHudTextServicesV1{this,integer_string});
        combat_flash=std::make_unique<ui::CombatFlashSwfV1>(*movie,ui::CombatFlashProjectionV1{this,combat_project,combat_rectangle});
        if(!combat_flash->scan(error))return false;
        __android_log_print(ANDROID_LOG_INFO,tag,"Source combat flash connected | styles %zu | same retained HUD/font/viewport | Level load-process producer pending",combat_flash->queue().styles().size());
        loaded=true;return true;
    }
    bool reset_failed(std::string& error) {
        gpu.abort();combat_flash.reset();enemy.reset();status.reset();movie.reset();fonts.reset();gfnt_text_backend_v1.reset();loaded=false;selected=false;
        pending_text.clear();borrowed_combat_string.clear();reported_combat_active=0;
        reported_enemy=0;reported_enemy_frame=-1;
        last_width=last_height=0;reported_frames={{-1,-1,-1,-1,-1}};
        leases.clear();exports.clear();font_failure.clear();provider_failure.clear();
        glyph_uploads=bitmap_uploads=string_calls=core_errors=packed_glyphs=strips=lines=masks=0;
        return gpu.reset_images(error);
    }
};
OriginalUiSession::OriginalUiSession():impl_(std::make_shared<Impl>()){}
OriginalUiSession::~OriginalUiSession(){
    // The platform pins this real resource owner. Release the owned graph and
    // platform before the session to break that deliberate lifetime cycle.
    impl_->combat_flash.reset();impl_->enemy.reset();impl_->status.reset();impl_->movie.reset();impl_->fonts.reset();impl_->gfnt_text_backend_v1.reset();
}
bool OriginalUiSession::initialize(AAssetManager* manager,std::string& error) {
    try{impl_->manager=manager;impl_->assets.manager(manager);impl_->gpu.initialize(manager);impl_->report_frame=true;error.clear();return true;}
    catch(const std::exception& e){error=e.what();impl_->selected=false;return false;}
}
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
void OriginalUiSession::deactivate(){impl_->selected=false;}

bool OriginalUiSession::attach_player(const std::string& directory,std::string& error){
    if(directory.empty()||directory.front()!='/'){error="Required private HUD directory unavailable";return false;}
    impl_->directory=directory;impl_->live_player=true;impl_->selected=true;impl_->report_frame=true;
    impl_->pending_text.clear();if(impl_->combat_flash)impl_->combat_flash->stop_all();impl_->reported_combat_active=0;
    error.clear();return true;
}
bool OriginalUiSession::prepare_player_frame(int width,int height,std::string& error){
    if(!overlays_player()){error="Combat text requires active player HUD";return false;}
    auto& self=*impl_;
    if(!self.loaded){self.driver_width=width;self.driver_height=height;if(!self.load(error)){
        const auto failure=error;std::string cleanup;self.reset_failed(cleanup);error=failure;return false;
    }}
    return self.viewport(width,height,error);
}
model_renderer::CombatTextSinkV1 OriginalUiSession::combat_text_sink(){return {impl_.get(),Impl::combat_localized,Impl::combat_enqueue};}
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
    if(frame.tick&&!self.combat_flash->update(frame.application_dt,frame.level_load_phase,error))return false;
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
        if(!self.loaded){self.driver_width=width;self.driver_height=height;if(!self.load(error))throw std::runtime_error(error);}
        if(!self.viewport(width,height,error))throw std::runtime_error(error);
        if(enemy&&!update_enemy(*enemy,error))throw std::runtime_error(error);
        if(!self.status->update(sheet,count,character,error)||
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
bool OriginalUiSession::update_enemy(const ui::EnemyHudWorldBorrowV1& borrow,std::string& error){
    auto& self=*impl_;
    if(!overlays_player()||!self.loaded||!self.enemy){error="Connected enemy HUD movie unavailable";return false;}
    if(!self.enemy->update(borrow,error))return false;
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
