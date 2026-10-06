#define main previous_swf_test_main
#include "swf_movie.cpp"
#undef main
#include "../authored_gameplay_hud_v1.hpp"
#include <stdexcept>
#include "gameswf/gameswf_character.h"
unsigned checks{};
void require(bool v,const std::string& e){++checks;if(!v)throw std::runtime_error(e);}
bool orientation(void*,std::int32_t& v,std::string&){v=0;return true;}
bool dimensions(void*,std::int32_t& w,std::int32_t& h,std::string&){w=480;h=320;return true;}
bool activation_native(void*,const char* name,const gameswf::fn_call& fn,std::string& error){
 const std::string n=name;
 if(n=="NativeGetOptionParameters"){
  if(fn.nargs!=2||std::string(fn.arg(0).to_string())!="HUDStyle"||!fn.arg(1).to_object()){error="Actual activation argument order";return false;}
  fn.arg(1).to_object()->set_member("CurrentOption",gameswf::as_value(2));return true;
 }
 if(n=="NativeSkillGetEquipedSkillsIDs"){
  auto* array=fn.arg(0).to_object();if(!array){error="Required actual skill receiver";return false;}
  for(const char* key:{"0","1","2"})array->set_member(key,gameswf::as_value(-1));return true;
 }
 if(n=="NativeHUDGetActiveFaery"){fn.result->set_int(-1);return true;}
 if(n=="NativeUseIpodPlayer"){fn.result->set_bool(false);return true;}
 // Explicit host observers for source onPush platform/input side effects.
 if(n=="NativeChangeRolloverInputBehavior"||n=="NativeUpdateOrientation"||n=="NativePauseAllSounds")return true;
 error="Unprovided activation native "+n;return false;
}
int main(){try{Test t;t.base="port/android-native/app/src/main/assets/original-cache/data/menus";SwfMovie movie;std::string e;
 auto services=t.services();services.native_owner=std::make_shared<int>(1);services.native_action=activation_native;services.native_actions={"NativeGetOptionParameters","NativeSkillGetEquipedSkillsIDs","NativeHUDGetActiveFaery","NativeUseIpodPlayer","NativeChangeRolloverInputBehavior","NativeUpdateOrientation","NativePauseAllSounds"};
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
 const int icons[]{0,1,2,3,5,5,6,7,5,5,4};
 for(int type=-128;type<128;++type){const auto expected=type>=0&&type<=10?icons[type]:5;require(AuthoredGameplayHudV1::action_icon(static_cast<std::int8_t>(type))==expected,"source full signed-byte mapping");}
 for(int type=0;type<=11;++type){require(hud.update_action_icon(static_cast<std::int8_t>(type),e),e);SwfClipInfo icon;require(movie.clip((hud.control_path(AuthoredHudControlV1::attack)+".btimg").c_str(),icon,e),e);require(icon.frame==AuthoredGameplayHudV1::action_icon(static_cast<std::int8_t>(type)),"actual FillActionIcon authored frame");}
 require(hud.refresh_action_icon(e),e);
 float width{};require(hud.joystick_background_width(width,e)&&width>0,e);
 AuthoredHudGeometryV1 receiver{};require(hud.joystick_receiver_geometry(70,235,receiver,e),e);require(receiver.character_id==412,"actual joystick receiver");
 require(hud.update(values,290,true,e),e);SwfClipInfo info;
 SwfClipInfo rest;require(movie.clip((hud.control_path(AuthoredHudControlV1::joystick)+".stick").c_str(),rest,e),e);
 require(hud.joystick_stick_offset(120,-200,e),e);require(movie.clip((hud.control_path(AuthoredHudControlV1::joystick)+".stick").c_str(),info,e),e);
 require(std::abs(info.local.value[2]-rest.local.value[2]-120)<.001f&&std::abs(info.local.value[5]-rest.local.value[5]+200)<.001f,"actual stick matrix displacement");
 require(hud.joystick_stick_reset(e),e);require(movie.clip((hud.control_path(AuthoredHudControlV1::joystick)+".stick").c_str(),info,e),e);require(std::abs(info.local.value[2]-rest.local.value[2])<.001f&&std::abs(info.local.value[5]-rest.local.value[5])<.001f,"actual stick rest reset");
 require(movie.clip((hud.control_path(AuthoredHudControlV1::character)+".btimg.HudChar").c_str(),info,e),e);require(info.frame==2,"source portrait frame");
 for(unsigned n=0;n<3;++n){require(movie.clip((hud.control_path(static_cast<AuthoredHudControlV1>(4+n))+".CoolDown").c_str(),info,e),e);require(info.frame==values[8+n*2]-1,"actual cooldown frame");}
 AuthoredHudGeometryV1 geometry{};require(hud.geometry(AuthoredHudControlV1::attack,425,260,geometry,e),e);require(geometry.character_id==374,"actual attack artwork ID");require(geometry.bounds[1]>geometry.bounds[0],"actual bounds");require(geometry.hit,"actual positive attack shape hit");
 require(hud.geometry(AuthoredHudControlV1::attack,-100,-100,geometry,e),e);require(!geometry.hit,"offscreen shape hit");
 require(!hud.release(AuthoredHudControlV1::attack,e),"fabricated attack AS handler");
 const auto before=t.vertices;require(hud.display(e),e);require(t.vertices>before,"authored HUD not submitted");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"vertices\":"<<t.vertices-before<<",\"scope\":\"actual Android SWF layouts, portrait/cooldown frames, viewport geometry and whole authored display; texture/localization providers are host fixtures, production actions external\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what();return 1;}}
