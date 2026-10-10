// Current production Front render and SelectClass.Hide bodies are extracted by
// the runner. Scene/GPU/movie leaves are explicit host fixtures. No APK,
// authored ActionScript execution, device pixels or save creation is claimed.
#include "front_loading_render_policy_v1.hpp"
#include "front_scene_selection_v1.hpp"
#include "menu_avatar_preview_v1.hpp"
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
void check(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
struct PreviewFixture {
 dh2::ui::MenuAvatarPreviewStateV1 state;
 struct Services {
  dh2::ui::MenuAvatarPreviewStateV1* avatar_state{};
  std::shared_ptr<void> avatar_state_owner;
  std::function<bool(std::uintptr_t&,std::string&)> selected_scene_root;
  std::function<bool(std::uintptr_t,std::string&)> remove_selected_root;
  std::function<bool(std::string&)> remove_all_scene_nodes,flush_objects,flush_animation_sets;
 } services_;
 bool main_scene_root_live=true,class_selection_root_live=false;
 bool destroyed_=true,busy_{};
 std::string injected;
 std::vector<std::string> calls;
 PreviewFixture(){
  state.slot=-1;services_.avatar_state=&state;services_.avatar_state_owner=std::make_shared<int>(1);
  services_.selected_scene_root=[this](auto& out,auto& e){
   if(!class_selection_root_live){e="CLASS_SELECTION root loan absent";return false;}
   out=88;return leaf("Root",e);
  };
  services_.remove_selected_root=[this](auto id,auto& e){
   check(id==88&&class_selection_root_live,"Select.Hide changed the class-selection root loan");
   if(!leaf("RemoveRoot",e))return false;
   class_selection_root_live=false;return true;
  };
  services_.remove_all_scene_nodes=[this](auto& e){return leaf("RemoveAll",e);};
  services_.flush_objects=[this](auto& e){return leaf("FlushObjects",e);};
  services_.flush_animation_sets=[this](auto& e){return leaf("FlushAnimationSets",e);};
 }
 bool leaf(const char* name,std::string& e){calls.emplace_back(name);if(injected==name){e="injected "+injected;return false;}e.clear();return true;}
 bool current(std::string& e){e.clear();return true;}
 bool effect(const std::function<bool(std::string&)>& f,const char*,std::string& e){return f&&f(e);}
 bool destroy_scene(std::string& e){if(!leaf("DestroyMainScene",e))return false;main_scene_root_live=false;return true;}
 bool destroy_avatar_camera(std::string& e){return leaf("DestroyMainCamera",e);}
 bool publish_class_selection_root(std::string& e){
  if(main_scene_root_live||class_selection_root_live){e="Invalid class-scene root lifecycle";return false;}
  if(!leaf("PublishClassRoot",e))return false;
  class_selection_root_live=true;return true;
 }
 bool create_avatar_camera(std::string& e){if(!leaf("CreateCamera",e))return false;evidence.main_camera=true;return true;}
 bool setup_scene(std::string& e){return leaf("SetupScene",e);}
 bool setup_character(std::string& e){return leaf(("SetupCharacter:"+std::to_string(state.slot)).c_str(),e);}
#include "creation_preview_run_live_v1.inc"
#include "creation_preview_show_live_v1.inc"
#include "creation_preview_hide_live_v1.inc"
};
}
namespace model_renderer {
bool active(){return evidence.enabled;}
bool class_scene_active(){return evidence.enabled&&evidence.class_scene;}
std::string load_class_scene(void*){++evidence.class_loads;evidence.enabled=true;evidence.class_scene=true;return "3D upload OK";}
std::string load_menu_background(void*){++evidence.menu_loads;evidence.enabled=true;evidence.class_scene=false;return "3D upload OK";}
bool select_class_scene(int index,int,std::string& e){++evidence.select_calls;evidence.selected=index;e.clear();return true;}
void draw_menu_background(int,int,bool avatar){
 ++evidence.main_draws;evidence.order.emplace_back("Main3D");
 if(!evidence.main_camera){++evidence.main_camera_skips;return;}
 if(evidence.throw_draw)throw std::runtime_error("Injected menu GPU submission failure");
 evidence.avatar_drawn=avatar;
}
}
namespace dh2::ui {struct MenuMovieBorrowV58 {std::uintptr_t identity{};};}
namespace dh2::android_ui {
struct MovieFixture {
 std::function<bool(const char*,std::string&)> display;
 std::uintptr_t player_identity()const{return 1;}
 bool display_clip(const char* path,int x,int y,int w,int h,std::string& e){
  check(std::string(path)=="_root.menu_bg.BrownBG","unexpected manually submitted authored backdrop");
  check(x==0&&y==0&&w==2400&&h==1080,"BrownBG must use the full physical stage");
  evidence.stage_clips.emplace_back(path);evidence.order.emplace_back(path);e.clear();return true;
 }
 bool display_source_stage_clip_v5(const char* path,std::string& e){
  evidence.clips.emplace_back(path);evidence.order.emplace_back(path);
  if(std::string(path).find("menu_SelectClass")!=std::string::npos){
   if(!model_renderer::class_scene_active()){e="Class pane selected the wrong renderer";return false;}
   ++evidence.class_draws;
  }
  if(display)return display(path,e);
  e.clear();return true;
 }
};
struct FrontMovieDrawV93 {std::uint32_t slot{};std::uintptr_t expected_movie{};std::string actual_clip_path;};
struct FrontUiSessionV87 {
 struct Impl {
  std::shared_ptr<void> source_menu_owner_v93=std::make_shared<int>(1);
  std::function<bool(std::vector<FrontMovieDrawV93>&,std::string&)> source_draw_v93;
  bool source_transport_busy_v93{},source_loading_render_v114{},rendering_splash_clip_v1{};
  bool process_class_select_active_v87=true;
  std::shared_ptr<MovieFixture> shared_menu_movie=std::make_shared<MovieFixture>(),movie=std::make_shared<MovieFixture>();
  int class_index=2,class_left=101,class_right=102,class_applied_index=2;
  void* manager{};
  bool viewport(int,int,std::string& e){e.clear();return true;}
 };
 std::shared_ptr<Impl> impl_=std::make_shared<Impl>();
 bool movie_slot_v93(std::uint32_t slot,ui::MenuMovieBorrowV58& out,std::string& e){out.identity=slot==2?202:0;e.clear();return true;}
 bool process_class_select_hide_v87(std::uintptr_t,std::string&);
 bool render_source_movies_v93(int,int,std::string&);
};
struct FrontStageV87 {int x{},y{},width{},height{};};
constexpr FrontStageV87 full_surface_stage_v87(int width,int height){return {0,0,width,height};}
#include "creation_front_render_live_v1.inc"
#include "creation_front_hide_live_v1.inc"
}
int main(){try{
 using namespace dh2::android_ui;
 FrontUiSessionV87 front;std::string e;std::vector<FrontMovieDrawV93> list;
 auto main=[&](const char* path){return FrontMovieDrawV93{2,reinterpret_cast<std::uintptr_t>(front.impl_->movie.get()),path};};
 auto base=[&](const char* path){return FrontMovieDrawV93{0,reinterpret_cast<std::uintptr_t>(front.impl_->shared_menu_movie.get()),path};};
 front.impl_->source_draw_v93=[&](auto& out,auto& error){out=list;error.clear();return true;};
 list={base("_root.menu_bg"),main("_root.menu_MainMenu")};
 check(front.render_source_movies_v93(2400,1080,e),e);
 check(evidence.avatar_drawn&&evidence.stage_clips.empty()&&evidence.order==std::vector<std::string>{"Main3D","_root.menu_bg","_root.menu_MainMenu"},"Main gradient/avatar order changed or Name-only BrownBG leaked into Main");
 evidence={};list.push_back(main("_root.menu_EnterName"));
 check(front.render_source_movies_v93(2400,1080,e),e);
 check(evidence.main_draws==0&&evidence.menu_loads==0&&evidence.stage_clips==std::vector<std::string>{"_root.menu_bg.BrownBG"}&&
       evidence.order==std::vector<std::string>{"_root.menu_bg.BrownBG","_root.menu_bg","_root.menu_MainMenu","_root.menu_EnterName"},
       "Name entry must submit only its authored BrownBG before retained stack clips");
 evidence={};evidence.main_camera=false;list.push_back(main("_root.menu_SelectClass"));
 for(int frame=0;frame<3;++frame)check(front.render_source_movies_v93(2400,1080,e),e);
 check(evidence.main_draws==0&&evidence.menu_loads==0&&evidence.class_loads==1&&evidence.class_draws==3&&evidence.selected==2&&evidence.stage_clips.empty(),"Confirm/stacked-frame selection reached retired Main camera, retained Name BrownBG, or lost Mage selection");
 PreviewFixture preview;
 preview.state.slot=3;
 std::int32_t transferred_slot=-1;
 check(preview.select_show(transferred_slot,e),e);
 check(transferred_slot==3&&preview.state.slot==-1&&!preview.main_scene_root_live,
       "Select.Show did not tear down Main scene/camera and transfer slot");
 check(preview.publish_class_selection_root(e),e);
 check(preview.select_hide(3,0,e),e);
 check(preview.calls==std::vector<std::string>{"DestroyMainScene","DestroyMainCamera","PublishClassRoot","Root","RemoveRoot","RemoveAll","FlushObjects","FlushAnimationSets","CreateCamera","SetupScene","SetupCharacter:3"},"SelectClass.Show/Hide class-root and camera/flush/scene order differs from source");
 check(!preview.class_selection_root_live,"Select.Hide retained CLASS_SELECTION root after releasing it");
 check(evidence.main_camera&&!preview.destroyed_&&preview.state.slot==3,"Hide did not restore retained previous slot and camera");
 // Existing-profile selection enters the same class renderer, then the
 // confirmation continuation returns through Hide. Exercise a second root
 // loan lifetime so the fresh-profile Hide case above cannot mask stale ownership.
 PreviewFixture existing_confirm;existing_confirm.state.slot=2;std::int32_t existing_slot=-1;
 check(existing_confirm.select_show(existing_slot,e)&&existing_slot==2,e);
 check(existing_confirm.publish_class_selection_root(e),e);
 check(existing_confirm.select_hide(2,0,e),e);
 check(existing_confirm.calls==std::vector<std::string>{"DestroyMainScene","DestroyMainCamera","PublishClassRoot","Root","RemoveRoot","RemoveAll","FlushObjects","FlushAnimationSets","CreateCamera","SetupScene","SetupCharacter:2"},
       "Existing-profile Show/Confirm lost or released the wrong class root");
 check(!existing_confirm.class_selection_root_live,"Existing-profile confirmation retained CLASS_SELECTION root");
 check(!front.process_class_select_hide_v87(203,e)&&front.impl_->process_class_select_active_v87,"Foreign RenderFX mutated class selection state");
 check(front.process_class_select_hide_v87(202,e),e);
 check(!front.impl_->process_class_select_active_v87&&!front.impl_->class_left&&!front.impl_->class_right&&front.impl_->class_applied_index==-1,"Hide retained class arrow/selection state");
 list={base("_root.menu_bg"),main("_root.menu_MainMenu"),main("_root.menu_StartGame")};
 check(front.render_source_movies_v93(2400,1080,e),e);
 check(evidence.menu_loads==1&&evidence.avatar_drawn&&!evidence.class_scene&&evidence.class_draws==3,"Class confirmation return retained class pane or missing Main camera");
 evidence={};
 list={main("_root.menu_MainMenu"),main("_root.menu_EnterName")};
 check(front.render_source_movies_v93(2400,1080,e)&&!evidence.avatar_drawn,"Back-to-name retained saved avatar");
 check(evidence.main_draws==0&&evidence.menu_loads==0&&evidence.stage_clips==std::vector<std::string>{"_root.menu_bg.BrownBG"}&&
       evidence.order==std::vector<std::string>{"_root.menu_bg.BrownBG","_root.menu_MainMenu","_root.menu_EnterName"},
       "Back-to-name must restore the authored BrownBG before Name, without painting the retained MainMenu scene");
 evidence={};evidence.main_camera=false;list={main("_root.menu_MainMenu"),main("_root.menu_EnterName")};
 check(front.render_source_movies_v93(2400,1080,e),e);
 check(evidence.main_draws==0&&evidence.main_camera_skips==0&&evidence.stage_clips==std::vector<std::string>{"_root.menu_bg.BrownBG"}&&evidence.clips.size()==2,
  "failed Confirm with a retired Main camera must preserve the authored name backdrop/SWF continuation");
 list={main("_root.menu_MainMenu")};
 evidence.main_camera=true;
 check(front.render_source_movies_v93(2400,1080,e)&&evidence.avatar_drawn,"Name suppression persisted after Back");
 evidence.throw_draw=true;
 check(!front.render_source_movies_v93(2400,1080,e)&&e=="Front menu rendering: Injected menu GPU submission failure","unrelated GPU error escaped Front exception boundary");
 check(!front.impl_->source_transport_busy_v93,"Render exception left source transport busy");
 for(const char* leaf:{"Root","RemoveRoot","RemoveAll","FlushObjects","FlushAnimationSets","CreateCamera","SetupScene","SetupCharacter:3"}){
  PreviewFixture failed;failed.state.slot=3;std::int32_t saved=-1;
  check(failed.select_show(saved,e)&&saved==3,"Failure fixture could not enter class selection");
  check(failed.publish_class_selection_root(e),e);
  failed.injected=leaf;check(!failed.select_hide(3,0,e)&&e=="injected "+std::string(leaf),"Hide failure swallowed");
  check(!failed.busy_,"Hide failure left preview owner busy");
 }
 evidence={};front.impl_->source_loading_render_v114=true;
 list={main("_root.menu_SelectClass"),base("_root.menu_Loading")};
 check(front.render_source_movies_v93(2400,1080,e),e);
 check(evidence.main_draws==0&&evidence.class_draws==0&&evidence.class_loads==0&&evidence.menu_loads==0&&evidence.stage_clips.empty()&&evidence.clips==std::vector<std::string>{"_root.menu_Loading"},"Loading admitted retired creation scene or Name BrownBG");
 // Current splash state is scoped even on a movie failure. This keeps the
 // continuation fixture compatible with the current shared Front owner.
 front.impl_->source_loading_render_v114=false;list={base("_root.menu_splash")};
 front.impl_->shared_menu_movie->display=[&](const char*,auto& error){check(front.impl_->rendering_splash_clip_v1,"Splash scope absent");error="fixture splash failure";return false;};
 check(!front.render_source_movies_v93(2400,1080,e)&&e=="fixture splash failure"&&!front.impl_->rendering_splash_clip_v1&&!front.impl_->source_transport_busy_v93,"Failed splash clip leaked scoped state into subsequent creation frames");
 std::cout<<"PASS current production creation render/Hide continuation: authored name backdrop, retired Main camera, Mage pane, three frames, eight Hide failure leaves, native camera/slot restoration, Front state reset, return Main/StartGame, Back, exception/scoped-state cleanup, loading exclusion\n";
 return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
