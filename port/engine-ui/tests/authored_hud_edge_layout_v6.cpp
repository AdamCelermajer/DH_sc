#define main previous_swf_test_main
#include "swf_movie.cpp"
#undef main
#include "../authored_gameplay_hud_v1.hpp"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <stdexcept>
#include <array>
unsigned checks=0;int saved_style=0,width=480,height=320;std::array<int,3> slots{{0,-1,-1}};
void require(bool v,const std::string& e){++checks;if(!v)throw std::runtime_error(e);}
bool orientation(void*,std::int32_t& v,std::string&){v=0;return true;}
bool dimensions(void*,std::int32_t& w,std::int32_t& h,std::string&){w=width;h=height;return true;}
bool native(void*,const char* name,const gameswf::fn_call& fn,std::string& e){const std::string n=name;
 if(n=="NativeGetOptionParameters"){if(fn.nargs!=2||!fn.arg(1).to_object()){e="Real HUD option receiver missing";return false;}fn.arg(1).to_object()->set_member("CurrentOption",gameswf::as_value(saved_style));return true;}
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

#include "../authored_hud_edge_layout_v6.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include <cstring>
struct Record {gameswf::gc_ptr<gameswf::character> node;gameswf::matrix local,world;};
struct Snapshot {
 std::vector<Record> records;std::vector<Record> deadzones;
 static void visit(gameswf::character* c,Snapshot& s,unsigned depth=0){
  require(depth<64,"actual graph depth");s.records.push_back({c,c->get_matrix(),c->get_world_matrix()});
  if(std::string(c->get_name().c_str()).find("deadzone_")==0)s.deadzones.push_back(s.records.back());
  if(c->is(gameswf::sprite_instance::m_class_id)){auto* sp=static_cast<gameswf::sprite_instance*>(c);for(int i=0;i<sp->m_display_list.size();++i)if(auto* child=sp->m_display_list.get_character(i))visit(child,s,depth+1);}
 }
 static bool run(void* p,SwfAsGraph& g,std::string& e){SwfAsValue v;gameswf::as_object* o=nullptr;if(!g.root_value(v,e)||!g.borrow_object(v,o,e))return false;visit(static_cast<gameswf::character*>(o),*static_cast<Snapshot*>(p));return true;}
 void restored()const{for(auto& r:records)require(std::memcmp(r.local.m_,r.node->get_matrix().m_,sizeof(r.local.m_))==0,"source current matrix not restored");}
};
struct NodeQuery {
 std::string path;gameswf::character* node=nullptr;gameswf::point hit;bool found=false;
 static bool run(void* p,SwfAsGraph& g,std::string& e){auto& q=*static_cast<NodeQuery*>(p);SwfAsValue root,v;gameswf::as_object* o=nullptr;if(!g.root_value(root,e)||!g.find_target(root,q.path.c_str(),v,e)||!g.borrow_object(v,o,e))return false;q.node=static_cast<gameswf::character*>(o);
  gameswf::rect bounds;q.node->get_bound(&bounds);if(auto* parent=q.node->get_parent())parent->get_world_matrix().transform(&bounds);
  for(int y=1;y<20&&!q.found;++y)for(int x=1;x<20&&!q.found;++x){gameswf::point global(bounds.m_x_min+(bounds.m_x_max-bounds.m_x_min)*x/20,bounds.m_y_min+(bounds.m_y_max-bounds.m_y_min)*y/20),pp=global;if(auto* parent=q.node->get_parent())parent->get_world_matrix().transform_by_inverse(&pp,global);gameswf::character* top=nullptr;q.node->get_topmost_mouse_entity(top,pp.m_x,pp.m_y);if(top){q.hit=global;q.found=true;}}
  return true;
 }
};
static void near(float a,float b,const char* m){require(std::fabs(a-b)<.02f,m);}
int main(){try{
 for(saved_style=0;saved_style<4;++saved_style)for(auto size:{std::array<int,2>{480,320},{2400,1080},{1024,768}}){width=size[0];height=size[1];
  Test t;t.base="port/android-native/app/src/main/assets/original-cache/data/menus";SwfMovie movie;std::string e;auto s=t.services();s.native_owner=std::make_shared<int>(1);s.native_action=native;s.native_actions={"NativeGetOptionParameters","NativeSkillGetEquipedSkillsIDs","NativeGetSkillDetails","NativeHUDGetActiveFaery","NativeUseIpodPlayer"};slots={{0,-1,-1}};
  require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",s,e)&&movie.advance(0,e),e);
  ViewportState64 viewport{{0,9600,0,6400},{0,0,width,height},{0,0,width,height},1,0,0};require(movie.connect_viewport(viewport,{nullptr,orientation,dimensions},e),e);FlashCamera40 camera{};require(movie.update_viewport(camera,e),e);std::int32_t bounds[4]{0,0,width,height};require(movie.set_source_bounds(bounds,2,e),e);
  AuthoredGameplayHudV1 hud(movie);require(hud.bind(saved_style,e)&&hud.activate(e),e);require(hud.update_action_icon(-1,e),e);
  for(int repeat=0;repeat<3;++repeat){require(hud.refresh_skills(e),e);require(hud.joystick_stick_offset(120,-200,e),e);
   Snapshot before;require(movie.action_script(&before,Snapshot::run,e),e);
   float rect[4];std::int32_t pixels[4];require(movie.source_display_rectangle(rect,pixels,e),e);float left=rect[0],right=rect[1]-9600,top=rect[2],bottom=rect[3]-6400;
   const bool mirrored=saved_style==1||saved_style==3;
   auto check=[&](const std::string& path,float dx,float dy){NodeQuery q{path};require(movie.action_script(&q,NodeQuery::run,e),e);auto original=q.node->get_world_matrix();require(with_authored_hud_edge_layout_v6(movie,hud,[&](std::string& error){NodeQuery moved{path};if(!movie.action_script(&moved,NodeQuery::run,error))return false;require(moved.node==q.node,"canonical instance changed");const auto matrix=moved.node->get_world_matrix();near(matrix.m_[0][2],original.m_[0][2]+dx,"world x edge");near(matrix.m_[1][2],original.m_[1][2]+dy,"world y edge");for(int row=0;row<2;++row)for(int col=0;col<2;++col)require(matrix.m_[row][col]==original.m_[row][col],"uniform scale/skew changed");return true;},e),e);before.restored();};
   check(hud.elements_path()+".HealthBars.player",left,top);check(hud.control_path(AuthoredHudControlV1::character),left,top);check(hud.control_path(AuthoredHudControlV1::pause),left,top);check(hud.control_path(AuthoredHudControlV1::potion),right,top);check(hud.control_path(AuthoredHudControlV1::joystick),mirrored?right:left,bottom);check(hud.control_path(AuthoredHudControlV1::attack),mirrored?left:right,bottom);check(hud.control_path(AuthoredHudControlV1::faery),mirrored?left:right,bottom);
   if(saved_style<2)check(hud.elements_path()+".controls.controls.list",mirrored?left:right,bottom);else for(auto c:{AuthoredHudControlV1::skill1,AuthoredHudControlV1::skill2,AuthoredHudControlV1::skill3})check(hud.control_path(c),mirrored?left:right,bottom);
   check(hud.elements_path()+".itemname_text",left,0);check(hud.elements_path()+".itunes_controls",(left+right)/2,top);if(saved_style==3){check(hud.elements_path()+".controls.controls.btn_dpad",right,bottom);check(hud.elements_path()+".controls.controls.btn_dpad_limits",right,bottom);}
   require(with_authored_hud_edge_layout_v6(movie,hud,[&](std::string&){for(auto& d:before.deadzones){const std::string name=d.node->get_name().c_str();float dx=name=="deadzone_menus"?left:name=="deadzone_mainmenus"?right:mirrored?left:right;float dy=name=="deadzone_menus"||name=="deadzone_mainmenus"?top:bottom; // only selected HUD is translated
    if(std::string(d.node->get_parent()->get_name().c_str())=="HUDelements"){bool selected=false;for(auto* ancestor=d.node->get_parent();ancestor;ancestor=ancestor->get_parent())if(ancestor->get_name()==("menu_HUD_"+std::to_string(saved_style)).c_str())selected=true;if(selected){near(d.node->get_world_matrix().m_[0][2],d.world.m_[0][2]+dx,"ordered deadzone x");near(d.node->get_world_matrix().m_[1][2],d.world.m_[1][2]+dy,"ordered deadzone y");}}}return true;},e),e);before.restored();
   require(!with_authored_hud_edge_layout_v6(movie,hud,[](std::string& error){error="intentional downstream failure";return false;},e)&&e=="intentional downstream failure","failure prefix");before.restored();bool caught=false;try{with_authored_hud_edge_layout_v6(movie,hud,[](std::string&)->bool{throw std::runtime_error("intentional");},e);}catch(const std::runtime_error&){caught=true;}require(caught,"exception propagation");before.restored();
   auto screen=[&](gameswf::point p){return std::array<float,2>{pixels[0]+(p.m_x-rect[0])/(rect[1]-rect[0])*pixels[2],pixels[1]+(p.m_y-rect[2])/(rect[3]-rect[2])*pixels[3]};};
   for(auto c:{AuthoredHudControlV1::character,AuthoredHudControlV1::pause,AuthoredHudControlV1::potion}){NodeQuery q{hud.control_path(c)};require(movie.action_script(&q,NodeQuery::run,e)&&q.found,"actual cached control shape point "+q.path);float dx=c==AuthoredHudControlV1::potion?right:c==AuthoredHudControlV1::attack?(mirrored?left:right):left;float dy=c==AuthoredHudControlV1::attack?bottom:top;auto relocated=screen(gameswf::point(q.hit.m_x+dx,q.hit.m_y+dy));AuthoredHudGeometryV1 geometry;require(hud.geometry(c,relocated[0],relocated[1],geometry,e)&&geometry.hit,e+" relocated shape miss");require(geometry.character_id==q.node->get_id(),"same shape receiver identity");if(std::fabs(dx)>2000||std::fabs(dy)>2000){auto old=screen(q.hit);require(hud.geometry(c,old[0],old[1],geometry,e)&&!geometry.hit,"old separated control still hit");}before.restored();}
   NodeQuery stick{hud.control_path(AuthoredHudControlV1::joystick)+".stick"};require(movie.action_script(&stick,NodeQuery::run,e),e);const auto stick_local=stick.node->get_matrix();AuthoredHudGeometryV1 stick_geometry;require(hud.joystick_receiver_geometry(0,0,stick_geometry,e),e);for(int row=0;row<2;++row)for(int col=0;col<3;++col)require(stick_geometry.local_matrix[row*3+col]==stick_local.m_[row][col],"joystick live displacement changed");before.restored();
   auto prior=t.vertices;require(hud.display(e)&&t.vertices>prior,e+" actual shape draw missing");before.restored();
  }
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"cases\":12,\"scope\":\"actual cached HUD graph/actions/shape picking; explicit texture/font/native platform fixture\"}\n";
 }catch(const std::exception& ex){std::cerr<<"style="<<saved_style<<" size="<<width<<"x"<<height<<" "<<ex.what();return 1;}}
