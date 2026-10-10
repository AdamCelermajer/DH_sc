// Executes the current production Front frame body. Movie/GPU transports below
// are explicit fixtures; the actual frame selection and exception boundary are
// extracted from front_ui_session_v87.cpp by the companion runner.
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
 bool enabled=true,class_scene=false,main_camera=true,throw_draw=false;
 unsigned menu_loads{},class_loads{},main_draws{},main_camera_skips{},class_draws{},select_calls{};
 int selected=-1;
 bool avatar_drawn=false;
 std::vector<std::string> clips,order,stage_clips;
} evidence;
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
}
namespace model_renderer {
bool active(){return evidence.enabled;}
bool class_scene_active(){return evidence.enabled&&evidence.class_scene;}
std::string load_class_scene(void*){++evidence.class_loads;evidence.enabled=true;evidence.class_scene=true;return "3D upload OK";}
std::string load_menu_background(void*){++evidence.menu_loads;evidence.enabled=true;evidence.class_scene=false;return "3D upload OK";}
bool select_class_scene(int index,int,std::string& e){++evidence.select_calls;evidence.selected=index;e.clear();return true;}
void draw_menu_background(int,int,bool avatar){
 ++evidence.main_draws;
 if(!evidence.main_camera){++evidence.main_camera_skips;return;}
 if(evidence.throw_draw)throw std::runtime_error("Injected menu GPU submission failure");
 evidence.order.emplace_back("Main3D");
 evidence.avatar_drawn=avatar;
}
}
namespace dh2::android_ui {
struct MovieFixture {
 std::uintptr_t player_identity()const{return 1;}
 bool display_clip(const char* path,int x,int y,int w,int h,std::string& e){
  check(std::string(path)=="_root.menu_bg.BrownBG","unexpected manually submitted authored backdrop");
  check(x==0&&y==0&&w==2400&&h==1080,"BrownBG must use the full physical stage");
  evidence.stage_clips.emplace_back(path);evidence.order.emplace_back(path);e.clear();return true;
 }
 bool display_source_stage_clip_v5(const char* path,std::string& e){
  evidence.clips.emplace_back(path);
  evidence.order.emplace_back(path);
  if(std::string(path).find("menu_SelectClass")!=std::string::npos){
   if(!model_renderer::class_scene_active()){e="Class pane selected the wrong renderer";return false;}
   ++evidence.class_draws;
  }
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
  int class_index=2;
  void* manager{};
  bool viewport(int,int,std::string& e){e.clear();return true;}
 };
 std::shared_ptr<Impl> impl_=std::make_shared<Impl>();
 bool render_source_movies_v93(int,int,std::string&);
};
struct FrontStageV87 {int x{},y{},width{},height{};};
constexpr FrontStageV87 full_surface_stage_v87(int width,int height){return {0,0,width,height};}
#include "class_creation_render_under_test.inc"
}
int main(){try{
 using namespace dh2::android_ui;
 FrontUiSessionV87 front;std::string e;std::vector<FrontMovieDrawV93> list;
 auto main=[&](const char* path){return FrontMovieDrawV93{2,reinterpret_cast<std::uintptr_t>(front.impl_->movie.get()),path};};
 auto base=[&](const char* path){return FrontMovieDrawV93{0,reinterpret_cast<std::uintptr_t>(front.impl_->shared_menu_movie.get()),path};};
 front.impl_->source_draw_v93=[&](auto& out,auto& error){out=list;error.clear();return true;};
 list={main("_root.menu_MainMenu")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.main_draws==1&&evidence.avatar_drawn,"occupied main menu avatar admission changed");
 evidence={};list={main("_root.menu_MainMenu"),main("_root.menu_EnterName")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.main_draws==0&&evidence.menu_loads==0&&evidence.clips.size()==2&&
       evidence.stage_clips==std::vector<std::string>{"_root.menu_bg.BrownBG"}&&
       evidence.order==std::vector<std::string>{"_root.menu_bg.BrownBG","_root.menu_MainMenu","_root.menu_EnterName"},
       "name entry must submit only its authored BrownBG before the retained MainMenu/active Name clips, without swamp scene/avatar");
 evidence={};list={base("_root.menu_EnterName"),main("_root.menu_MainMenu")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.main_draws==0&&evidence.menu_loads==0&&evidence.clips==std::vector<std::string>{"_root.menu_EnterName","_root.menu_MainMenu"},
       "name-entry state in the shared slot must suppress the retained slot-2 MainMenu scene");
 evidence={};evidence.main_camera=false;list={main("_root.menu_MainMenu"),main("_root.menu_EnterName")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.main_draws==0&&evidence.main_camera_skips==0&&evidence.clips.size()==2,
  "retired Main camera must not replace the authored name-entry backdrop or abort the SWF continuation");
 evidence={};list={base("_root.menu_bg"),main("_root.menu_MainMenu")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.order==std::vector<std::string>{"Main3D","_root.menu_bg","_root.menu_MainMenu"},"3D repaint erased base movie's authored gradient");
 // SelectClass.Show has already destroyed Main's native camera. MainMenu
 // remains an underlying SWF stack entry, matching the Confirm crash.
 evidence={};evidence.main_camera=false;
 list={base("_root.menu_bg"),main("_root.menu_MainMenu"),main("_root.menu_EnterName"),main("_root.menu_SelectClass")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.main_draws==0&&evidence.menu_loads==0&&evidence.class_loads==1&&evidence.class_draws==1&&evidence.stage_clips.empty(),"Confirm attempted retired Main scene/camera or retained Name BrownBG");
 check(evidence.select_calls==1&&evidence.selected==2&&evidence.clips.size()==4,"creation pane lost authored scene/selection or underlying clips");
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.class_loads==1&&evidence.menu_loads==0&&evidence.main_draws==0&&evidence.class_draws==2,"subsequent frame flips between stacked scene owners");
 // Native SelectClass.Hide recreates Main's camera before returning.
 evidence={};evidence.main_camera=true;list={main("_root.menu_EnterName")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.menu_loads==0&&evidence.main_draws==0&&evidence.stage_clips==std::vector<std::string>{"_root.menu_bg.BrownBG"}&&
       evidence.order==std::vector<std::string>{"_root.menu_bg.BrownBG","_root.menu_EnterName"},
       "back to name must restore only its authored BrownBG before the Name clip, without menu scene/avatar");
 list={main("_root.menu_MainMenu")};
 check(front.render_source_movies_v93(2400,1080,e)&&evidence.avatar_drawn,"name overlay avatar suppression leaked after Back");
 evidence.throw_draw=true;
 check(!front.render_source_movies_v93(2400,1080,e)&&e=="Front menu rendering: Injected menu GPU submission failure","unrelated GPU errors must still reach the Front diagnostic boundary");
 evidence={};front.impl_->source_loading_render_v114=true;
 list={main("_root.menu_SelectClass"),base("_root.menu_Loading")};
 check(front.render_source_movies_v93(2400,1080,e),e.c_str());
 check(evidence.main_draws==0&&evidence.class_draws==0&&evidence.class_loads==0&&evidence.menu_loads==0&&evidence.stage_clips.empty()&&evidence.clips==std::vector<std::string>{"_root.menu_Loading"},"source loading admits retired front scene or Name BrownBG");
 std::cout<<"PASS production Front Confirm render: retired Main camera, persistent stacked clips, class pane and initial selection, EnterName/avatar/Back, required-error boundary, loading exclusion\n";
 return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
