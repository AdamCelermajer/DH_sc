// Actual cached movie/navigation/shape test. Application, profile, GPU and
// localization platform hooks below are explicitly fixtures, never production.
#define main historical_swf_fixture_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "swf_movie.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../authored_character_panel_v2.hpp"
#include "../swf_text_font_platform_v1.hpp"
#include "../swf_font_resolver.hpp"
#include "../../script-runtime/script_constants.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_movie_def.h"
#include <set>
#include <stdexcept>
namespace {
void require(bool yes,const std::string& e){if(!yes)throw std::runtime_error(e);}
struct Host:Test {
 std::shared_ptr<int> pin=std::make_shared<int>(1);HudTextV1 text;
 std::unique_ptr<SwfTextFontPlatformV1> fonts;
 dh2_script_constants* constants=dh2_script_constants_create();
 ~Host(){fonts.reset();dh2_script_constants_destroy(constants);}
 MenuStackGlobalsV1 globals{};MenuStackCharacterV1 hud{1,1,1,1,1},basechar{2,1,1,1,1};
 std::string cache;std::set<std::string> natives;unsigned lifecycle_calls{},creates{},queries{},stack_calls{};bool reject=false;
 static bool native_action(void* raw,const char* name,const gameswf::fn_call& fn,std::string& e){auto& h=*static_cast<Host*>(raw);h.natives.insert(name);
  if(h.reject&&std::string(name)=="NativePlaySoundFX"){e="Required fixture sound delivery rejected";return false;}
  if(std::string(name)=="NativeIsMultiplayerEnabled")fn.result->set_bool(false);
  // Declared external native fixture; onRelease/stack and every actual AS
  // instruction execute unchanged. This does not prove profile mutations.
  return true;
 }
 static bool open(void* raw,const char* uri,bool& found,std::vector<std::uint8_t>& out,std::uintptr_t& lease,std::string&){auto& h=*static_cast<Host*>(raw);std::ifstream f(h.cache+"/data/"+uri,std::ios::binary);found=bool(f);if(found){out.assign(std::istreambuf_iterator<char>(f),{});lease=1;}return true;}
 static int resolver(void* raw,FontResolveRequest40* q){auto& h=*static_cast<Host*>(raw);switch(q->kind){
  case FontResolveService::debug_load:case FontResolveService::debug_get_switch:return 0;
  case FontResolveService::language:q->value=0;return 0; // explicit English platform fixture
  case FontResolveService::rewrite_path:{std::string path=q->text;if(path.rfind("cache/data/",0)!=0)return -1;path=h.cache+"/data/"+path.substr(11);if(path.size()>=q->capacity)return -1;std::memcpy(q->buffer,path.c_str(),path.size()+1);return 0;}
  case FontResolveService::open_read:q->value=reinterpret_cast<std::uintptr_t>(std::fopen(q->text,"rb"));return 0;
  case FontResolveService::close_read:return std::fclose(reinterpret_cast<FILE*>(q->value));
 }return -1;}
 static bool font_read(void* raw,const char* name,bool bold,bool italic,std::vector<std::uint8_t>& bytes,std::string& e){char path[4096];FontResolveOutput32 output{path,sizeof path,0,0,0};FontResolveInput24 input{name,"cache/",std::uint32_t(bold),std::uint32_t(italic)};FontResolveServices16 services{raw,resolver};if(dh2_swf_font_resolve(&output,&input,&services)||!output.found){e="Actual authored font unavailable";return false;}std::ifstream f(path,std::ios::binary);if(!f){e="Resolved authored font vanished";return false;}bytes.assign(std::istreambuf_iterator<char>(f),{});return true;}
 AuthoredCharacterPanelServicesV2 services_for_panel(std::string& e){
  std::vector<std::uint8_t> bytes[3];const char* names[]{"common_text_pyarray.bin","common_text_pyarraynames.bin","common_text_pystructnames.bin"};
  for(unsigned i=0;i<3;++i){std::ifstream f(cache+"/data/pydata/"+names[i],std::ios::binary);require(bool(f),"Actual common_text fixture absent");bytes[i].assign(std::istreambuf_iterator<char>(f),{});}
  require(text.load({bytes[0].data(),bytes[0].size()},{bytes[1].data(),bytes[1].size()},{bytes[2].data(),bytes[2].size()},e)&&text.switch_pack(0,false,e),e);
  {std::ifstream f(cache+"/data/pydata/common_text_pycst.bin",std::ios::binary);require(bool(f),"Actual text constants absent");std::vector<std::uint8_t> b{std::istreambuf_iterator<char>(f),{}};dh2_script_constants_reload output;require(dh2_script_constants_load(constants,b.data(),b.size(),&output)==0,"Actual text constants load failed");}
  AuthoredCharacterPanelServicesV2 s;s.movie=services();s.movie.native_owner=pin;s.movie.native_action=native_action;s.movie.native_actions={"NativeReloadSkills"};s.maximum_occurrences=64;s.globals=&globals;s.hud={1,0x40,&hud,nullptr};s.base={2,0x40,&basechar,nullptr};
  s.queries=[this](const char* name,auto&,auto&){++queries;natives.insert(name);return true;};
  s.character=[](gameswf::as_object* object,auto& out,auto&){out={reinterpret_cast<std::uintptr_t>(object),1,1,1,1};return true;};
  s.panel_render_flags=[](auto& flags,auto&){flags=0x40;return true;}; // explicit no-animation fixture domain
  s.create=[this](auto&,auto&){++creates;return true;};
  s.lifecycle.owner=pin;s.lifecycle.invoke=[this](auto&,const auto&,auto& result,auto&){++lifecycle_calls;result=0;return true;};
  s.remaining_stack={this,[](void* p,MenuStackV1*,MenuStackRequestV1* q){++static_cast<Host*>(p)->stack_calls;q->result=0;return 0;}};
  s.localization.text=&text;s.localization.strings.context=this;s.localization.strings.open=open;
  s.localization.strings.close=[](void*,auto,auto&){return true;};s.localization.strings.debug=[](void*,const char*,auto&){return true;};
  s.localization.strings.constant=[](void* raw,const char* group,const char* key,std::uint32_t& out,std::string& e){int value;auto rc=dh2_script_constants_get(static_cast<Host*>(raw)->constants,group,key,&value);if(rc){e="Actual localization constant unavailable";return false;}std::memcpy(&out,&value,4);return true;};
  s.localization.strings.player_character=[](void*,auto& id,auto&){id=0;return true;}; // declared no-profile localization fixture
  s.localization.debug=[](const char*,auto&){return true;};s.localization.set_context=[](const auto&,auto&){return true;};
  TextFontBackendsV2 backends;backends.bitmap_face=[](const auto&,auto& face,auto&){face={};return true;};
  fonts=std::make_unique<SwfTextFontPlatformV1>(SwfFontServices{this,font_read,nullptr},s.movie,pin,backends,1);
  fonts->policy().renderer_feature=[](const auto& command,auto& e){if(command.kind==edit_text_display_v1::Command::grid_fit)return true;e="Fixture render cache unavailable";return false;};
  s.movie=fonts->services();return s;
 }
};
bool member_text(CharacterMenuMovieV1& movie,const char* name,std::string& value,std::string& e){struct Read {const char* name;std::string& value;static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& r=*static_cast<Read*>(raw);SwfAsValue root,v;bool found=false;return graph.root_value(root,e)&&graph.get_member(root,r.name,v,found,e)&&found&&graph.to_text(v,r.value,e);}} r{name,value};return movie.action_script(&r,Read::run,e);}
}
int main(int argc,char** argv){try{
 require(argc==2,"Actual original-cache directory required");Host host;host.cache=argv[1];host.base=host.cache+"/data/menus";std::string e;AuthoredCharacterPanelV2 panel;
 auto services=host.services_for_panel(e);services.create={};
 require(panel.initialize(services,e),e);require(host.creates==0&&panel.movie()->screens().size()==20,"Whole source base catalog registration missing");
 require(panel.open(e),e);require(panel.stack()->menu("menu_CharacterSheetStats")->status!=0,"Actual default stats handler did not push stats");
 std::string tab;require(member_text(*panel.movie(),"LastSelectedTab",tab,e)&&tab.empty(),"Actual initial tab state");
 require(panel.tab(2,e),e);require(member_text(*panel.movie(),"LastSelectedTab",tab,e)&&tab=="Skill","Actual skills onRelease not executed");
 require(panel.stack()->menu("menu_SkillTreeSheetNew")->status!=0,"Actual skill tree push absent");
 require(panel.tab(1,e),e);require(member_text(*panel.movie(),"LastSelectedTab",tab,e)&&tab=="Inventory","Actual inventory onRelease not executed");
 require(panel.stack()->menu("menu_InventorySheetMain")->status!=0,"Actual inventory push absent");
 ViewportState64 viewport{};
 require(panel.movie()->action_script(&viewport,[](void* raw,SwfAsGraph& graph,std::string& e){SwfAsValue value;gameswf::as_object* object=nullptr;if(!graph.root_value(value,e)||!graph.borrow_object(value,object,e)||!object||!object->is(gameswf::sprite_instance::m_class_id))return false;auto* def=dynamic_cast<gameswf::movie_def_impl*>(static_cast<gameswf::sprite_instance*>(object)->get_movie_definition());if(!def){e="Actual cached root movie definition absent";return false;}auto& out=*static_cast<ViewportState64*>(raw);auto& r=def->m_frame_size;out.movie_rect[0]=r.m_x_min;out.movie_rect[1]=r.m_x_max;out.movie_rect[2]=r.m_y_min;out.movie_rect[3]=r.m_y_max;return true;},e),e);
 viewport.viewport[2]=viewport.bounds[2]=854;viewport.viewport[3]=viewport.bounds[3]=480;viewport.pixel_scale=1;viewport.player_receiver=1;
 SwfViewportDriver driver{nullptr,[](void*,auto& orientation,auto&){orientation=0;return true;},[](void*,auto& w,auto& h,auto&){w=854;h=480;return true;}};
 require(panel.movie()->movie()->connect_viewport(viewport,driver,e),e);
 require(panel.display(0,0,854,480,e),e);
 AuthoredHudGeometryV1 g;const char* path="_root.menu_CharacterMenu.CharacterMenuTabs.btnCharacterSheet";
 require(panel.geometry(path,0,0,g,e),e);require(g.character_id>0&&g.bounds[1]>g.bounds[0],"Authored shape/matrix domain absent");
 // Convert the actual world-twips shape centre through retained movie viewport.
 float point[2]{(g.bounds[0]+g.bounds[1])/40.f,(g.bounds[2]+g.bounds[3])/40.f};
 float origin[2]{0,0},extent[2]{854,480};require(panel.movie()->movie()->screen_to_logical(origin,e)&&panel.movie()->movie()->screen_to_logical(extent,e),e);
 point[0]=(point[0]-origin[0])*854.f/(extent[0]-origin[0]);point[1]=(point[1]-origin[1])*480.f/(extent[1]-origin[1]);
 require(panel.geometry(path,point[0],point[1],g,e)&&g.hit,"Actual tab shape centre not hit");
 std::cerr<<"shape bounds "<<g.bounds[0]<<","<<g.bounds[1]<<","<<g.bounds[2]<<","<<g.bounds[3]<<" screen "<<point[0]<<","<<point[1]<<"\n";
 require(panel.pointer(0,7,point[0],point[1],e),e);
 require(panel.movie()->action_script(nullptr,[](void*,SwfAsGraph& graph,std::string& e){SwfAsValue value;gameswf::as_object* o{};if(!graph.root_value(value,e)||!graph.borrow_object(value,o,e))return false;auto* sprite=static_cast<gameswf::sprite_instance*>(o);auto* r=sprite->get_root();auto* active=r->m_mouse_button_state.m_active_entity.get_ptr();auto* top=r->m_mouse_button_state.m_topmost_entity.get_ptr();gameswf::character* direct{};auto hit=sprite->get_topmost_mouse_entity(direct,r->m_mouse_x*20.f,r->m_mouse_y*20.f);std::cerr<<"mouse "<<r->m_mouse_x<<","<<r->m_mouse_y<<" rootMatch="<<(r->get_root_movie()==sprite)<<" direct="<<(direct?direct->get_name().c_str():"NULL")<<" hit="<<hit<<" visible="<<sprite->get_visible()<<" matrix="<<sprite->get_matrix().m_[0][0]<<","<<sprite->get_matrix().m_[0][2]<<","<<sprite->get_matrix().m_[1][2]<<" active="<<(active?active->get_name().c_str():"NULL")<<" top="<<(top?top->get_name().c_str():"NULL")<<"\n";return true;},e),e);
 require(panel.pointer(2,8,point[0],point[1],e),e); // secondary pointer cannot steal source mouse
 require(panel.pointer(1,7,point[0],point[1],e),e);
 require(member_text(*panel.movie(),"LastSelectedTab",tab,e)&&tab.empty(),"Whole native mouse shape-selected release failed");
 require(panel.pointer(0,7,point[0],point[1],e)&&panel.pointer(3,7,point[0],point[1],e),e);
 require(panel.release(path,e),e);require(member_text(*panel.movie(),"LastSelectedTab",tab,e)&&tab.empty(),"Scoped named release failed");
 require(panel.back(e),e);require(panel.stack()->view()->count==0,"Actual back did not pop stack");
 require(panel.open(e),e);host.reject=true;require(!panel.tab(1,e)&&e.find("sound delivery rejected")!=std::string::npos,"Required nested native failure swallowed");host.reject=false;
 require(panel.advance(0,e),e);
 std::cout<<"{\"validation\":\"PASS\",\"authored_screens\":20,\"source_stats_skills_inventory_back\":true,\"actual_shape_hit_and_scoped_release\":true,\"nested_required_failure_propagated\":true,\"fixture_query_calls\":"<<host.queries<<",\"fixture_lifecycle_calls\":"<<host.lifecycle_calls<<",\"limits\":{\"profile_queries_and_application_are_fixtures\":true,\"GL_upload_and_draw_are_fixtures\":true,\"live_Android_menu_acceptance\":false}}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
