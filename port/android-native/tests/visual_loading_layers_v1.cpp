// Executes the production Front draw body with explicit movie/GPU endpoints.
// Checks menu gradient ordering and splash scope; no device or Android build.
#include "front_loading_render_policy_v1.hpp"
#include "front_scene_selection_v1.hpp"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
struct Evidence {
    std::vector<std::string> order;
    std::vector<std::string> stage_clips;
    std::vector<bool> splash_scopes;
    bool fail_clip{};
} evidence;
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
}
namespace model_renderer {
bool active(){return true;}
bool class_scene_active(){return false;}
std::string load_class_scene(void*){return "3D upload OK";}
std::string load_menu_background(void*){throw std::runtime_error("Unexpected scene reload");}
bool select_class_scene(int,int,std::string& e){e.clear();return true;}
void draw_menu_background(int,int,bool){evidence.order.emplace_back("Main3D");}
}
namespace dh2::android_ui {
struct MovieFixture {
    bool* splash{};
    std::uintptr_t player_identity()const{return 1;}
    bool display_clip(const char* path,int x,int y,int w,int h,std::string& e){
        check(std::string(path)=="_root.menu_bg.BrownBG","unexpected manually submitted authored backdrop");
        check(x==0&&y==0&&w==2400&&h==1080,"BrownBG must use the full physical stage");
        evidence.stage_clips.emplace_back(path);evidence.order.emplace_back(path);e.clear();return true;
    }
    bool display_source_stage_clip_v5(const char* path,std::string& e){
        evidence.order.emplace_back(path);
        evidence.splash_scopes.push_back(*splash);
        if(evidence.fail_clip){e="Required clip failed";return false;}
        e.clear();return true;
    }
};
struct FrontMovieDrawV93 {std::uint32_t slot{};std::uintptr_t expected_movie{};std::string actual_clip_path;};
struct FrontUiSessionV87 {
    struct Impl {
        std::shared_ptr<void> source_menu_owner_v93=std::make_shared<int>(1);
        std::function<bool(std::vector<FrontMovieDrawV93>&,std::string&)> source_draw_v93;
        bool source_transport_busy_v93{},source_loading_render_v114{},rendering_splash_clip_v1{};
        std::shared_ptr<MovieFixture> shared_menu_movie=std::make_shared<MovieFixture>(),movie=std::make_shared<MovieFixture>();
        int class_index{};
        void* manager{};
        bool viewport(int,int,std::string& e){e.clear();return true;}
        Impl(){shared_menu_movie->splash=movie->splash=&rendering_splash_clip_v1;}
    };
    std::shared_ptr<Impl> impl_=std::make_shared<Impl>();
    bool render_source_movies_v93(int,int,std::string&);
};
struct FrontStageV87 {int x{},y{},width{},height{};};
constexpr FrontStageV87 full_surface_stage_v87(int width,int height){return {0,0,width,height};}
#include "visual_loading_render_under_test.inc"
}

int main(){try{
    using namespace dh2::android_ui;
    FrontUiSessionV87 front;std::string error;std::vector<FrontMovieDrawV93> list;
    const auto main=[&](const char* path){return FrontMovieDrawV93{2,reinterpret_cast<std::uintptr_t>(front.impl_->movie.get()),path};};
    const auto base=[&](const char* path){return FrontMovieDrawV93{0,reinterpret_cast<std::uintptr_t>(front.impl_->shared_menu_movie.get()),path};};
    front.impl_->source_draw_v93=[&](auto& out,auto& e){out=list;e.clear();return true;};
    list={base("_root.menu_HelpButtons"),main("_root.menu_bg"),main("_root.menu_MainMenu")};
    check(front.render_source_movies_v93(2400,1080,error),error.c_str());
    check(evidence.order==std::vector<std::string>{"Main3D","_root.menu_HelpButtons","_root.menu_bg","_root.menu_MainMenu"},"3D repaint erased authored gradient");
    check(evidence.splash_scopes==std::vector<bool>{false,false,false},"Splash UV policy leaked into main menu");
    evidence={};list={main("_root.menu_MainMenu"),main("_root.menu_EnterName")};
    check(front.render_source_movies_v93(2400,1080,error),error.c_str());
    check(evidence.stage_clips==std::vector<std::string>{"_root.menu_bg.BrownBG"}&&
          evidence.order==std::vector<std::string>{"_root.menu_bg.BrownBG","_root.menu_MainMenu","_root.menu_EnterName"},
          "Name must render its isolated BrownBG sibling before active source clips");
    evidence={};list={main("_root.menu_SelectClass")};
    check(front.render_source_movies_v93(2400,1080,error),error.c_str());
    check(evidence.stage_clips.empty(),"SelectClass must not inherit the Name BrownBG backdrop");
    evidence={};list={main("_root.menu_bg"),main("_root.menu_splash")};
    check(front.render_source_movies_v93(2400,1080,error),error.c_str());
    check(evidence.stage_clips.empty()&&evidence.order==std::vector<std::string>{"Main3D","_root.menu_bg","_root.menu_splash"},"Splash clip escaped the retained draw order or inherited Name backdrop");
    check(evidence.splash_scopes==std::vector<bool>{false,true},"Background UV correction admitted the wrong clip");
    check(!front.impl_->rendering_splash_clip_v1&&!front.impl_->source_transport_busy_v93,"Scoped render admission leaked");
    evidence={};evidence.fail_clip=true;list={main("_root.menu_splash")};
    check(!front.render_source_movies_v93(2400,1080,error),"Required splash delivery failure was accepted");
    check(!front.impl_->rendering_splash_clip_v1&&!front.impl_->source_transport_busy_v93,"Failed splash leaked render admission");
    evidence={};front.impl_->source_loading_render_v114=true;
    list={main("_root.menu_MainMenu"),base("_root.menu_splash"),base("_root.menu_Loading")};
    check(front.render_source_movies_v93(2400,1080,error),error.c_str());
    check(evidence.stage_clips.empty()&&evidence.order==std::vector<std::string>{"_root.menu_Loading"},"Campaign loading retained menu scene, startup splash, or Name backdrop");
    check(evidence.splash_scopes==std::vector<bool>{false},"Campaign loading inherited splash-title UV correction");
    std::cout<<"PASS production Front: 3D before authored gradient and controls; splash UV scope and failure cleanup; campaign loading exclusion\n";
    return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
