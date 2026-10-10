// Current NativeMenuPreview slot/setup/destroy bodies are extracted by the
// runner. Character allocation, scene/camera and visual methods are explicit
// host endpoints; source ChangeCharacter and quaternion operations are linked.
// Model/PCLS/GPU behavior is checked by the separate existing v138 regression.
#include "menu_avatar_preview_v1.hpp"
#include "native_menu_preview_physics_guard_v124.hpp"
#include "math.hpp"
#include <array>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace {
using model_renderer::ensure_menu_preview_physics_v124;
void check(bool ok,const std::string& text){if(!ok)throw std::runtime_error(text);}
constexpr int ANDROID_LOG_INFO=4;
constexpr const char* menu_preview_log_tag_v121="DH2Native";
template<class... Args> int __android_log_print(int,const char*,const char*,Args...){return 0;}
float source_float(std::uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
using Character=struct CharacterFixture {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{},visual_root{};
 std::function<bool(std::uint32_t,std::string&)> sg_load;
 std::function<bool(std::string&)> attach_visual;
 std::function<bool(const std::array<float,3>&,std::string&)> visual_position;
 std::function<bool(const std::array<float,4>&,std::string&)> visual_rotation;
 std::function<bool(bool,std::string&)> visual_visible;
};
struct CameraFixture {std::uintptr_t camera_borrow(){return 300;}};
struct SceneFixture {bool set_active_camera_v13(std::uintptr_t id,std::string& e){check(id==300,"Changed source camera");e.clear();return true;}};
struct PreviewFixture {
 dh2::ui::MenuAvatarPreviewStateV1 state;
 struct Services {
  dh2::ui::MenuAvatarPreviewStateV1* avatar_state{};
  std::shared_ptr<void> avatar_state_owner;
  std::shared_ptr<SceneFixture> scene;
  std::function<bool(std::string&)> flush_objects,flush_animation_sets;
  std::function<bool(bool&,std::string&)> physical_world_ready;
  std::function<bool(float,float,float,float,std::string&)> load_physics;
  std::function<bool(std::int32_t,bool&,std::string&)> save_exists;
  std::function<bool(std::int32_t,bool,Character&,std::string&)> create_player;
 } services_;
 struct Plane {std::uintptr_t identity=10;} plane_;
 std::shared_ptr<CameraFixture> camera_=std::make_shared<CameraFixture>();
 Character character_;
 bool busy_{};
 std::array<bool,4> occupied{true,false,true,true};
 int selected=-1;bool fresh{};unsigned allocations{},loads4{},flushes{},camera_calls{};
 std::string injected;
 std::vector<std::string> calls;
 PreviewFixture(){
  services_.avatar_state=&state;services_.avatar_state_owner=std::make_shared<int>(1);services_.scene=std::make_shared<SceneFixture>();
  services_.flush_objects=[this](auto& e){++flushes;selected=-1;return leaf("FlushObjects",e);};
  services_.flush_animation_sets=[this](auto& e){return leaf("FlushAnimationSets",e);};
  services_.physical_world_ready=[](auto& ready,auto& e){ready=true;e.clear();return true;};
  services_.load_physics=[](float,float,float,float,auto& e){e="Unexpected live PhysicalWorld reload";return false;};
  services_.save_exists=[this](auto slot,auto& exists,auto& e){check(slot==state.slot,"SG_Exists addressed a stale slot");exists=occupied.at(slot);return leaf("Exists:"+std::to_string(slot),e);};
  services_.create_player=[this](auto slot,bool new_profile,auto& out,auto& e){
   check(slot==state.slot&&new_profile==(state.fresh_slot_intent||!occupied.at(slot)),"CreatePlayer slot/fresh flags differ from explicit intent and selected save");
   check(!out.owner,"CreatePlayer retained previous Character without source Flush");
   ++allocations;selected=slot;fresh=new_profile;
   out.owner=std::make_shared<int>(slot);out.identity=100+allocations;out.visual_root=200+allocations;
   const auto identity=out.identity;
   out.sg_load=[this,identity](auto mask,auto& error){check(identity==character_.identity&&mask==4,"SG_Load4 addressed another Character");++loads4;return leaf("Load4:"+std::to_string(selected),error);};
   out.attach_visual=[this](auto& error){return leaf("Attach",error);};
   out.visual_position=[](const auto& position,auto& error){check(position==std::array<float,3>{0,-200,-20},"Preview position differs from original");error.clear();return true;};
   out.visual_rotation=[](const auto& rotation,auto& error){check(rotation[3]>0.9f&&rotation[2]<0,"Preview quaternion lost authored orientation");error.clear();return true;};
   out.visual_visible=[](bool visible,auto& error){check(visible,"Original preview visibility changed");error.clear();return true;};
   return leaf("Create:"+std::to_string(slot)+(new_profile?":fresh":":saved"),e);
  };
 }
 bool leaf(const std::string& name,std::string& e){calls.push_back(name);if(injected==name){e="injected "+name;return false;}e.clear();return true;}
 bool current(std::string& e){e.clear();return true;}
 bool effect(const std::function<bool(std::string&)>& f,const char*,std::string& e){return f&&f(e);}
 bool create_avatar_camera(std::string& e){++camera_calls;return leaf("CreateCamera",e);}
#include "menu_selection_destroy_live_v1.inc"
#include "menu_selection_setup_live_v1.inc"
#include "menu_selection_run_live_v1.inc"
#include "menu_selection_avatar_destroy_live_v1.inc"
#include "menu_selection_avatar_setup_live_v1.inc"
#include "menu_selection_avatar_camera_live_v1.inc"
 dh2::ui::MenuAvatarPreviewServicesV1 transport(){return {this,
  [](void* p,auto& e){return static_cast<PreviewFixture*>(p)->avatar_destroy(e);},
  [](void* p,auto slot,auto& e){return static_cast<PreviewFixture*>(p)->avatar_setup(slot,e);},
  [](void* p,auto& e){return static_cast<PreviewFixture*>(p)->avatar_camera(e);}};}
 bool select(int slot,bool force,std::string& e,bool fresh_slot_intent=false){return dh2::ui::change_menu_avatar_preview_v1(state,slot,force,transport(),e,fresh_slot_intent);}
};
}
int main(){try{
 PreviewFixture p;std::string e;
 check(p.select(0,false,e),e);check(p.selected==0&&!p.fresh&&p.allocations==1&&p.loads4==1,"Occupied Warrior setup not reached");
 const auto old=p.character_.identity;p.calls.clear();
 check(p.select(2,false,e),e);
 check(p.selected==2&&!p.fresh&&p.character_.identity!=old&&p.loads4==2,"GALCHOU slot2 retained slot0 Character");
 check(p.calls==std::vector<std::string>{"FlushObjects","FlushAnimationSets","Exists:2","Create:2:saved","Load4:2","Attach","CreateCamera"},"Saved selection source transport order changed");
 const auto stable=p.allocations;check(p.select(2,false,e)&&p.allocations==stable,"Same unforced slot recreated its Character");
 check(p.select(2,true,e)&&p.allocations==stable+1&&p.loads4==3,"Forced same-slot refresh failed to rebuild/load selected Character");
 // Creation writes dh2_002.savegame before refreshing the preview. SG_Exists
 // therefore returns true, while the one-shot intent still selects fresh Mage
 // setup and suppresses the existing-profile Load4 branch.
 const auto loads_before_fresh=p.loads4;
 p.calls.clear();check(p.select(2,true,e,true),e);
 check(p.fresh&&p.allocations==stable+2&&p.loads4==loads_before_fresh&&
  !p.state.fresh_slot_intent,"Fresh Mage intent was lost, replayed, or routed through Load4");
 check(p.calls==std::vector<std::string>{"FlushObjects","FlushAnimationSets","Exists:2","Create:2:fresh","Attach","CreateCamera"},
  "Fresh Mage with an already-written save did not use fresh CreatePlayer setup");
 p.calls.clear();const auto before_empty_allocations=p.allocations;
 check(p.select(1,false,e),e);check(p.state.slot==1&&p.selected==-1&&!p.character_.identity&&!p.character_.owner&&
  p.allocations==before_empty_allocations&&p.loads4==3,"Empty slot created or retained a preview Character");
 check(p.calls==std::vector<std::string>{"FlushObjects","FlushAnimationSets","Exists:1","CreateCamera"},
  "Empty slot should keep the scene camera but skip fresh CreatePlayer/Load4/Attach");
 check(p.select(-1,false,e),e);check(!p.character_.identity&&!p.character_.owner&&p.selected==-1,"Slot -1 retained previous avatar");
 check(p.select(3,false,e),e);check(p.selected==3&&!p.fresh&&p.loads4==4,"Rogue selection not routed to saved CreatePlayer/Load4");
 check(p.select(2,false,e)&&p.selected==2&&p.loads4==5,"Mage not restored after Empty/-1/Rogue");
 // Also exercise the production guard against a callback that changes the
 // selected slot before the normal DestroyCharacter prefix. Empty must clear
 // the retained preview Character rather than leaving the old model visible.
 PreviewFixture interrupted;check(interrupted.select(0,false,e),e);
 interrupted.calls.clear();interrupted.state.slot=1;
 check(interrupted.avatar_setup(1,e),e);
 check(!interrupted.character_.identity&&!interrupted.character_.owner&&interrupted.selected==-1,
  "Direct Empty-slot setup retained the previous Character");
 check(interrupted.calls==std::vector<std::string>{"Exists:1","FlushObjects","FlushAnimationSets"},
  "Direct Empty-slot cleanup changed the source Exists/Flush prefix");
 check(!p.avatar_setup(0,e)&&e=="Native avatar slot differs from SAME MainMenu producer","Stale adapter slot accepted");
 PreviewFixture before_show;before_show.plane_.identity=0;
 check(before_show.select(2,false,e)&&before_show.allocations==0,"Slot callback fabricated Character before SetupScene");
 PreviewFixture failed;failed.injected="Load4:2";
 check(!failed.select(2,false,e)&&e=="injected Load4:2"&&failed.state.slot==2&&!failed.busy_,"Required Load4 failure swallowed or slot publication rewound");
 std::cout<<"PASS current NativeSetSlot underlying ChangeCharacter: existing Mage -> fresh Mage with save already present -> Empty no actor, plus Warrior/Rogue transitions and lifecycle guards\n";
 return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
