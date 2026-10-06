#define main previous_swf_test_main
#include "swf_movie.cpp"
#undef main
#include "../authored_gameplay_hud_v1.hpp"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <stdexcept>
#include <array>
unsigned checks=0;std::array<int,3> slots{{0,-1,-1}};
void require(bool v,const std::string& e){++checks;if(!v)throw std::runtime_error(e);}
bool orientation(void*,std::int32_t& v,std::string&){v=0;return true;}
bool dimensions(void*,std::int32_t& w,std::int32_t& h,std::string&){w=480;h=320;return true;}
bool native(void*,const char* name,const gameswf::fn_call& fn,std::string& e){const std::string n=name;
 if(n=="NativeGetOptionParameters"){if(fn.nargs!=2||!fn.arg(1).to_object()){e="Real HUD option receiver missing";return false;}fn.arg(1).to_object()->set_member("CurrentOption",gameswf::as_value(2));return true;}
 if(n=="NativeSkillGetEquipedSkillsIDs"){
  auto* object=fn.arg(0).to_object();if(!object||!object->is(gameswf::as_array::m_class_id)){e="Real skill Array missing";return false;}
  auto* array=static_cast<gameswf::as_array*>(object);for(int row:slots)array->push(gameswf::as_value(row));fn.result->set_bool(true);return true;
 }
 if(n=="NativeGetSkillDetails"){
  auto* object=fn.arg(1).to_object();if(!object||fn.arg(0).to_int()!=0){e="Actual fixture skill row/receiver missing";return false;}
  object->set_member("SkillIcon",gameswf::as_value("bash_down")); // actual cached sprite245 frame10
  int slot=-1;for(int i=0;i<3;++i)if(slots[i]==0)slot=i;object->set_member("SkillAssignedToSlot",gameswf::as_value(slot));return true;
 }
 if(n=="NativeHUDGetActiveFaery"){fn.result->set_int(-1);return true;}
 if(n=="NativeUseIpodPlayer"){fn.result->set_bool(false);return true;}
 e="Required native fixture provider "+n;return false;
}
struct Query{int length{};bool tail=false;
 static bool run(void* p,SwfAsGraph& g,std::string& e){auto& q=*static_cast<Query*>(p);SwfAsValue root,hud,array,value,result;bool found=false,callable=false;
  if(!g.root_value(root,e)||!g.find_target(root,"_root.menu_HUD_2",hud,e))return false;
  if(q.tail)return g.invoke(hud,hud,"setSkillsButtons",{},result,callable,e)&&callable;
  if(!g.get_member(hud,"CurUsedSkillsIDs",array,found,e)||!found||!g.get_member(array,"length",value,found,e)||!found)return false;
  double n=0;if(!g.to_number(value,n,e))return false;q.length=int(n);return true;
 }};
int main(){try{Test t;t.base="port/android-native/app/src/main/assets/original-cache/data/menus";SwfMovie movie;std::string e;auto s=t.services();s.native_owner=std::make_shared<int>(1);s.native_action=native;s.native_actions={"NativeGetOptionParameters","NativeSkillGetEquipedSkillsIDs","NativeGetSkillDetails","NativeHUDGetActiveFaery","NativeUseIpodPlayer"};
 require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",s,e),e);require(movie.advance(0,e),e);
 ViewportState64 viewport{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1,0,0};require(movie.connect_viewport(viewport,{nullptr,orientation,dimensions},e),e);
 AuthoredGameplayHudV1 hud(movie);require(hud.bind(2,e)&&hud.activate(e),e);Query query;
 require(movie.action_script(&query,Query::run,e)&&query.length==3,"actual onPush must produce3 rows");
 slots={{-1,-1,0}};query.tail=true;require(movie.action_script(&query,Query::run,e),e);query.tail=false;require(movie.action_script(&query,Query::run,e)&&query.length==6,"broken tail must retain old3 +appendnew3");
 SwfClipInfo old;require(movie.clip(hud.control_path(AuthoredHudControlV1::skill1).c_str(),old,e)&&old.visible,"stale first button reproduction");
 for(int selected: {2,2,1,0,2}){
  slots={{-1,-1,-1}};slots[selected]=0;require(hud.refresh_skills(e),e);require(movie.action_script(&query,Query::run,e)&&query.length==3,"whole onPush must replace array every refresh");
  for(unsigned i=0;i<3;++i){SwfClipInfo button,icon;auto path=hud.control_path(static_cast<AuthoredHudControlV1>(4+i));require(movie.clip(path.c_str(),button,e),e);require(button.visible==(int(i)==selected),"authored button visibility differs from actual slot");
   if(int(i)==selected)require(movie.clip((path+".btimg").c_str(),icon,e)&&icon.frame==10,"actual bash_down sprite frame not selected");}
 }
 slots={{-1,-1,-1}};require(hud.refresh_skills(e),e);require(movie.action_script(&query,Query::run,e)&&query.length==3,e);
 for(unsigned i=0;i<3;++i){SwfClipInfo button;require(movie.clip(hud.control_path(static_cast<AuthoredHudControlV1>(4+i)).c_str(),button,e)&&!button.visible,"empty slot must use authored hiding branch");}
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"scope\":\"actual cached HUD AS, true append semantics, stale-tail reproduction and whole onPush refresh; player/details/texture/font platform fixture providers\"}\n";
 }catch(const std::exception& ex){std::cerr<<ex.what();return 1;}}
