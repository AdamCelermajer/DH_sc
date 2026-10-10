#include "../authored_gameplay_hud_v1.hpp"
#include "../gameplay_hud_usable_refresh_v1.hpp"
#include "../../android-native/app/src/main/cpp/gameplay_main_menu_request_v1.hpp"
#include "../swf_frame_connection.hpp"
#include "swf_movie_test_fixture.hpp"
#include <stdexcept>
#include <memory>
#include <vector>
#include "gameswf/gameswf_character.h"
unsigned checks{};
unsigned status_pulls{};
unsigned main_menu_requests{};
std::vector<std::string> activation_calls;
std::string status_pull_shape;
void require(bool v,const std::string& e){++checks;if(!v)throw std::runtime_error(e);}
bool orientation(void*,std::int32_t& v,std::string&){v=0;return true;}
bool dimensions(void*,std::int32_t& w,std::int32_t& h,std::string&){w=480;h=320;return true;}
struct FrameOwner {std::shared_ptr<SwfInputHistory> history{std::make_shared<SwfInputHistory>()};SwfFrameConnection frames;};
bool graph_start(void* context,const SwfAsLease& lease,std::string& error){
 auto& owner=*static_cast<FrameOwner*>(static_cast<Test*>(context)->graph_context);
 return owner.history->bind(lease.player,error)&&owner.frames.bind(lease.player,owner.history,error);
}
bool activation_native(void*,const char* name,const gameswf::fn_call& fn,std::string& error){
 const std::string n=name;
 activation_calls.push_back(n);
 if(n=="NativeGetOptionParameters"){
  if(fn.nargs!=2||std::string(fn.arg(0).to_string())!="HUDStyle"||!fn.arg(1).to_object()){error="Actual activation argument order";return false;}
  fn.arg(1).to_object()->set_member("CurrentOption",gameswf::as_value(2));return true;
 }
 if(n=="NativeGoToMainMenu"){
  if(fn.nargs!=0){error="Source main-menu action takes no arguments";return false;}
  ++main_menu_requests;return true;
 }
 if(n=="NativeSkillGetEquipedSkillsIDs"){
  auto* array=fn.arg(0).to_object();if(!array){error="Required actual skill receiver";return false;}
  for(const char* key:{"0","1","2"})array->set_member(key,gameswf::as_value(-1));return true;
 }
 if(n=="NativeHUDGetActiveFaery"){fn.result->set_int(-1);return true;}
 if(n=="NativeGetNextStatusMessage"){
  status_pull_shape="nargs="+std::to_string(fn.nargs);
  for(int i=0;i<fn.nargs;++i)status_pull_shape+=" arg"+std::to_string(i)+"="+fn.arg(i).to_string();
  if(fn.nargs!=2){error="StatusMsg SWF pull shape "+status_pull_shape;return false;}
  ++status_pulls;fn.result->set_string("");return true;
 }
 if(n=="NativeUseIpodPlayer"){fn.result->set_bool(false);return true;}
 // Explicit host observers for source onPush platform/input side effects.
 if(n=="NativeChangeRolloverInputBehavior"||n=="NativeUpdateOrientation"||n=="NativePauseAllSounds")return true;
 error="Unprovided activation native "+n;return false;
}
int main(){try{Test t;t.base="port/android-native/app/src/main/assets/original-cache/data/menus";FrameOwner frame_owner;SwfMovie movie;std::string e;
 // The native HUD callback only queues teardown; one post-dispatch consumer
 // owns the transition and duplicate release events cannot unload twice.
 {
  dh2::native::GameplayMainMenuRequestV1 request;
  require(!request.consume(),"main-menu handoff began pending");
  request.request();require(request.consume(),"source confirmation did not queue menu handoff");
  require(!request.consume(),"menu handoff was consumed more than once");
  request.request();request.request();require(request.consume()&&!request.consume(),"duplicate request did not remain a single transition");
 }
 // A skill tap can start the cooldown between HUD refresh polls. It must
 // force the next projection to re-run source OnSkillCheck immediately.
 {
  dh2::ui::GameplayHudUsableRefreshV1 gate;
  const auto now=dh2::ui::GameplayHudUsableRefreshV1::Clock::now();
  require(gate.due(now),"initial skill usability refresh was not due");
  gate.refreshed(now);
  require(!gate.due(now+std::chrono::milliseconds(499)),"ordinary 500 ms throttle was lost");
  require(gate.due(now+std::chrono::milliseconds(500)),"ordinary usability refresh did not expire");
  gate.refreshed(now+std::chrono::milliseconds(500));
  require(!dh2::ui::run_hud_action_and_refresh_usable_v1(gate,[]{return false;})&&
          !gate.due(now+std::chrono::milliseconds(501)),
          "failed cast changed the cached HUD usability result");
  require(dh2::ui::run_hud_action_and_refresh_usable_v1(gate,[]{return true;})&&
          gate.due(now+std::chrono::milliseconds(501)),
          "successful skill or spell cast did not invalidate usability cache");
  gate.refreshed(now+std::chrono::milliseconds(600));
  dh2::ui::refresh_usable_after_skill_train_v1(gate,true,1);
  require(!gate.due(now+std::chrono::milliseconds(601)),
          "test-only training invalidated the source HUD cache");
  dh2::ui::refresh_usable_after_skill_train_v1(gate,false,0);
  require(!gate.due(now+std::chrono::milliseconds(601)),
          "rejected training invalidated the source HUD cache");
  dh2::ui::refresh_usable_after_skill_train_v1(gate,false,1);
  require(gate.due(now+std::chrono::milliseconds(601)),
          "successful training did not refresh skill usability");
 }
 auto services=t.services();services.native_owner=std::make_shared<int>(1);services.native_action=activation_native;services.native_actions={"NativeGetOptionParameters","NativeGoToMainMenu","NativeSkillGetEquipedSkillsIDs","NativeHUDGetActiveFaery","NativeGetNextStatusMessage","NativeUseIpodPlayer","NativeChangeRolloverInputBehavior","NativeUpdateOrientation","NativePauseAllSounds"};
 t.graph_context=&frame_owner;services.graph_start=graph_start;
 require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,e),e);require(movie.advance(0,e),e);
 ViewportState64 viewport{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1,0,0};require(movie.connect_viewport(viewport,{nullptr,orientation,dimensions},e),e);
 AuthoredGameplayHudV1 hud(movie);require(!hud.bind(4,e),"invalid style accepted");
 for(int style=0;style<4;++style){require(hud.bind(style,e),e);
  for(unsigned n=0;n<9;++n){SwfClipInfo info;require(movie.clip(hud.control_path(static_cast<AuthoredHudControlV1>(n)).c_str(),info,e),e);require(info.id>0,"authored character missing");}
 }
 require(hud.bind(2,e),e);std::array<std::int32_t,17> values{1,3,80,20,60,30,35,1,15,1,60,0,100,1,4,1,0};
 require(hud.cached_action_icon()==-1,"source cache constructor");
 const auto activation_errors=t.errors;require(hud.activate(e),e); // genuine authored CurrentHud producer, no injection
 require(t.errors==activation_errors,"whole activation introduced unresolved authored/native call");
 const auto before_menu_click=activation_calls.size();
 require(hud.release(AuthoredHudControlV1::pause,e),e);
 unsigned pause_sounds=0,rollover=0;
 for(std::size_t i=before_menu_click;i<activation_calls.size();++i){
  pause_sounds+=activation_calls[i]=="NativePauseAllSounds";
  rollover+=activation_calls[i]=="NativeChangeRolloverInputBehavior";
 }
 require(pause_sounds==1&&rollover==1&&main_menu_requests==0,
         "HUD main-menu click must open the authored pause flow without skipping its confirmation");
 const bool status_rendered=hud.notify_status_message(e);
 require(status_pulls==1,"actual onStatusMessage handler did not reach the status queue: "+status_pull_shape);
 require(status_pull_shape=="nargs=2 arg0=0 arg1=false","source SWF status pull arguments changed: "+status_pull_shape);
 if(!status_rendered)require(e=="Required original text glyph provider unavailable","unexpected status-message dispatch failure: "+e);
 require(hud.update_action_icon(-1,e),e);
 SwfClipInfo empty_interest_icon;
 require(movie.clip((hud.control_path(AuthoredHudControlV1::attack)+".btimg").c_str(),empty_interest_icon,e),e);
 require(empty_interest_icon.frame==5,"source empty object-of-interest type must select native fallback icon frame 5");
 const int icons[]{0,1,2,3,5,5,6,7,5,5,4};
 for(int type=-128;type<128;++type){const auto expected=type>=0&&type<=10?icons[type]:5;require(AuthoredGameplayHudV1::action_icon(static_cast<std::int8_t>(type))==expected,"source full signed-byte mapping");}
 for(int type=0;type<=11;++type){require(hud.update_action_icon(static_cast<std::int8_t>(type),e),e);SwfClipInfo icon;require(movie.clip((hud.control_path(AuthoredHudControlV1::attack)+".btimg").c_str(),icon,e),e);require(icon.frame==AuthoredGameplayHudV1::action_icon(static_cast<std::int8_t>(type)),"actual FillActionIcon authored frame");}
 require(hud.refresh_action_icon(e),e);
 float width{};require(hud.joystick_background_width(width,e)&&width>0,e);
 AuthoredHudGeometryV1 receiver{};require(hud.joystick_receiver_geometry(70,235,receiver,e),e);require(receiver.character_id==412,"actual joystick receiver");
 // The visible list rows in the real droid SWF have no SlotId: styles 0/1
 // use their ordinal and retain the regular cooldown projection regression.
 for(int style=0;style<2;++style){require(hud.bind(style,e),e);require(hud.update(values,290,true,e),e);
  SwfClipInfo portrait;require(movie.clip((hud.control_path(AuthoredHudControlV1::character)+".btimg.HudChar").c_str(),portrait,e),e);require(portrait.frame==2,"source portrait frame for HUD style");
  for(unsigned n=0;n<3;++n){
   SwfClipInfo cooldown;
   require(movie.clip((hud.control_path(static_cast<AuthoredHudControlV1>(4+n))+".CoolDown").c_str(),cooldown,e),e);
   require(cooldown.frame==values[8+n*2]-1,"rolling HUD row selects its ordinal cooldown");
  }
 }
 // Empty slots are represented by source -1 values. In fixed layouts the
 // SWF omits SlotId from blank buttons, which must not abort the HUD update.
 auto empty_skills=values;for(unsigned n=6;n<14;++n)empty_skills[n]=0;
 for(int style=2;style<4;++style){require(hud.bind(style,e),e);
  std::array<bool,3> source_visibility{};
  for(unsigned n=0;n<3;++n){SwfClipInfo before_update;require(movie.clip(hud.control_path(static_cast<AuthoredHudControlV1>(4+n)).c_str(),before_update,e),e);source_visibility[n]=before_update.visible;}
  if(style==2)require(!source_visibility[0]&&!source_visibility[1]&&!source_visibility[2],
                      "authored onPush must hide all three empty Save-slot buttons");
  require(hud.update(empty_skills,290,true,e),e);
  for(unsigned n=0;n<3;++n){const auto path=hud.control_path(static_cast<AuthoredHudControlV1>(4+n));
   SwfClipInfo slot,icon,cooldown,grey;
   require(movie.clip(path.c_str(),slot,e),e);require(movie.clip((path+".btimg").c_str(),icon,e),e);
   require(movie.clip((path+".CoolDown").c_str(),cooldown,e),e);require(movie.clip((path+".Grey").c_str(),grey,e),e);
   require(slot.visible==source_visibility[n],
           "FastUpdate must preserve setSkillsButtons/onButtonSet source visibility | "+path);
   require(cooldown.frame==0,"empty fixed-layout skill row must render zero cooldown");
   require(grey.visible,"empty fixed-layout skill row must remain visibly disabled");
  }
 }
 require(hud.bind(2,e),e);SwfClipInfo info;
 SwfClipInfo rest;require(movie.clip((hud.control_path(AuthoredHudControlV1::joystick)+".stick").c_str(),rest,e),e);
 require(hud.joystick_stick_offset(120,-200,e),e);require(movie.clip((hud.control_path(AuthoredHudControlV1::joystick)+".stick").c_str(),info,e),e);
 require(std::abs(info.local.value[2]-rest.local.value[2]-120)<.001f&&std::abs(info.local.value[5]-rest.local.value[5]+200)<.001f,"actual stick matrix displacement");
 require(hud.joystick_stick_reset(e),e);require(movie.clip((hud.control_path(AuthoredHudControlV1::joystick)+".stick").c_str(),info,e),e);require(std::abs(info.local.value[2]-rest.local.value[2])<.001f&&std::abs(info.local.value[5]-rest.local.value[5])<.001f,"actual stick rest reset");
 AuthoredHudGeometryV1 geometry{};require(hud.geometry(AuthoredHudControlV1::attack,425,260,geometry,e),e);require(geometry.character_id==374,"actual attack artwork ID");require(geometry.bounds[1]>geometry.bounds[0],"actual bounds");require(geometry.hit,"actual positive attack shape hit");
 require(hud.geometry(AuthoredHudControlV1::attack,-100,-100,geometry,e),e);require(!geometry.hit,"offscreen shape hit");
 require(!hud.release(AuthoredHudControlV1::attack,e),"fabricated attack AS handler");
 const auto before=t.vertices;require(hud.display(e),e);require(t.vertices>before,"authored HUD not submitted");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"vertices\":"<<t.vertices-before<<",\"status_callback\":\"_root.onStatusMessage(0) reached NativeGetNextStatusMessage(0,false) exactly once\",\"status_text\":\"host render stops at missing original text glyph provider\",\"scope\":\"actual Android SWF layouts, portrait/cooldown frames, viewport geometry and whole authored display; texture/localization providers are host fixtures, production actions external\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what();return 1;}}
