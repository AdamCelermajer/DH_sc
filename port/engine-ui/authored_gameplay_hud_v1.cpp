#include "authored_gameplay_hud_v1.hpp"
#include "authored_hud_edge_layout_v6.hpp"
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wmismatched-tags"
#pragma clang diagnostic ignored "-Wnew-returns-null"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#endif
#include "gameswf/gameswf_character.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <algorithm>
namespace dh2::ui {
namespace {
struct Invocation {const std::string* path;const char* method;const std::vector<SwfAsValue>* args;};
bool invoke_graph(void* p,SwfAsGraph& graph,std::string& error){
 auto& q=*static_cast<Invocation*>(p);SwfAsValue root,receiver,result;bool callable{};
 if(!graph.root_value(root,error)||!graph.find_target(root,q.path->c_str(),receiver,error))return false;
 if(!graph.invoke(receiver,receiver,q.method,*q.args,result,callable,error))return false;
 if(!callable){error="Required authored HUD method unavailable: "+*q.path+"."+q.method;return false;}return true;
}
struct Geometry {const std::string* path;float x,y;AuthoredHudGeometryV1* out;};
bool geometry_graph(void* p,SwfAsGraph& graph,std::string& error){
 auto& q=*static_cast<Geometry*>(p);SwfAsValue root,value;gameswf::as_object* object{};
 if(!graph.root_value(root,error)||!graph.find_target(root,q.path->c_str(),value,error)||!graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::character::m_class_id)){error="Required authored HUD character: "+*q.path;return false;}
 auto* c=static_cast<gameswf::character*>(object);auto& out=*q.out;
 gameswf::rect bounds;c->get_bound(&bounds);auto* parent=c->get_parent();
 if(parent)parent->get_world_matrix().transform(&bounds);
 out.bounds[0]=bounds.m_x_min;out.bounds[1]=bounds.m_x_max;out.bounds[2]=bounds.m_y_min;out.bounds[3]=bounds.m_y_max;
 gameswf::point local;c->get_world_matrix().transform_by_inverse(&local,gameswf::point(q.x,q.y));
 out.local[0]=local.m_x;out.local[1]=local.m_y;out.character_id=c->get_id();
 const auto& matrix=c->get_matrix();for(int row=0;row<2;++row)for(int col=0;col<3;++col)out.local_matrix[row*3+col]=matrix.m_[row][col];
 gameswf::point pp(q.x,q.y);if(parent)parent->get_world_matrix().transform_by_inverse(&pp,gameswf::point(q.x,q.y));
 gameswf::character* top{};out.hit=c->get_topmost_mouse_entity(top,pp.m_x,pp.m_y);
 for(auto* a=c;a;a=a->get_parent())if(!a->get_visible())out.hit=false;
 return true;
}
}
bool AuthoredGameplayHudV1::invoke(const std::string& path,const char* method,const std::vector<SwfAsValue>& args,std::string& error){Invocation q{&path,method,&args};return movie_.action_script(&q,invoke_graph,error);}
bool AuthoredGameplayHudV1::frame(const std::string& path,std::int32_t frame,std::string& error){return invoke(path,"gotoAndStop",{SwfAsValue::number(static_cast<double>(frame)+1)},error);}
bool AuthoredGameplayHudV1::bind(std::int32_t style,std::string& error){
 if(style<0||style>3){error="Actual HUDStyle outside authored Android layouts";return false;}
 style_=style;menu_="_root.menu_HUD_"+std::to_string(style);elements_=menu_+".HUDelements";
 const auto c=elements_+".controls.controls.";
 controls_={elements_+".btn_mainmenu",elements_+".btn_charactermenu",elements_+".HealthBars.btn_potion",c+"btn_spell",
 c+(style>=2?"btn_skill1":"list.btn_0"),c+(style>=2?"btn_skill2":"list.btn_pre0"),c+(style>=2?"btn_skill3":"list.btn_post0"),c+"btn_interact",c+"Joystick"};
 SwfClipInfo info;for(const auto& path:controls_)if(!movie_.clip(path.c_str(),info,error)){style_=-1;return false;}
 if(!movie_.clip((controls_[8]+".stick").c_str(),info,error)){style_=-1;return false;}
 stick_rest_[0]=info.local.value[2];stick_rest_[1]=info.local.value[5];
 for(int n=0;n<4;++n)if(!movie_.set_visible(("_root.menu_HUD_"+std::to_string(n)).c_str(),n==style,error))return false;
 return true;
}
bool AuthoredGameplayHudV1::activate(std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 if(!invoke("_root","DisplayRightHud",{},error))return false;
 // Original producer chooses the SAME saved option through its native call.
 struct Selection{const std::string* path;};Selection q{&menu_};
 return movie_.action_script(&q,[](void* p,SwfAsGraph& g,std::string& e){auto& q=*static_cast<Selection*>(p);SwfAsValue root,current,selected;bool found{};
  if(!g.root_value(root,e)||!g.get_member(root,"CurrentHud",current,found,e)||!found||!g.find_target(root,q.path->c_str(),selected,e))return false;
  if(!current.identity()||current.identity()!=selected.identity()){e="Required authored DisplayRightHud selection matches same saved HUDStyle";return false;}return true;},error);
}
bool AuthoredGameplayHudV1::refresh_skills(std::string& error){
 // NativeSkillGetEquipedSkillsIDs appends three rows. The actual onPush
 // lifecycle recreates CurUsedSkillsIDs before calling setSkillsButtons and
 // resets the real Grey/button/faery timelines. Calling the tail alone reuses
 // stale first-three rows and grows beyond the authored three button owners.
 return invoke(menu_,"onPush",{},error);
}
std::int32_t AuthoredGameplayHudV1::action_icon(std::int8_t type) noexcept{
 // Original unsigned CMP rejects negative signed bytes too; table 8c9f28.
 static constexpr std::int32_t icons[]{0,1,2,3,5,5,6,7,5,5,4};
 const auto index=static_cast<std::uint32_t>(static_cast<std::int32_t>(type));
 return index<=10?icons[index]:5;
}
bool AuthoredGameplayHudV1::update_action_icon(std::int8_t type,std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 const auto icon=action_icon(type);if(icon==*action_icon_)return true;
 // 42eaf0 publishes before InvokeASCallback; failures retain that prefix.
 *action_icon_=icon;return refresh_action_icon(error);
}
bool AuthoredGameplayHudV1::refresh_action_icon(std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 return invoke("_root","FillActionIcon",{SwfAsValue::number(*action_icon_)},error);
}
bool AuthoredGameplayHudV1::display(std::string& error){if(style_<0){error="Authored HUD is not bound";return false;}return with_authored_hud_edge_layout_v6(movie_,*this,[this](std::string& e){return movie_.display_source_clip(elements_.c_str(),e);},error);}
bool AuthoredGameplayHudV1::update(const std::array<std::int32_t,17>& infos,std::int32_t cls,bool dpad,std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 struct Text{const std::string path;const std::string value;};Text text{controls_[2]+".cnt.value",std::to_string(infos[14])};
 if(!movie_.action_script(&text,[](void* p,SwfAsGraph& g,std::string& e){auto& q=*static_cast<Text*>(p);SwfAsValue root,value;bool accepted{};
  if(!g.root_value(root,e)||!g.find_target(root,q.path.c_str(),value,e)||!g.set_member(value,"text",SwfAsValue::text(q.value),accepted,e))return false;
  if(!accepted){e="Required authored potion text setter";return false;}return true;},error))return false;
 // Original HudManager portrait, availability, cooldown and usable expressions.
 const auto portrait=(cls>=290&&cls<=292)?2:(cls>=325&&cls<=327)?1:0;
 if(!frame(controls_[1]+".btimg.HudChar",portrait,error)||!movie_.set_visible((controls_[1]+".anim_levelup").c_str(),infos[15]!=0,error)||!movie_.set_visible(controls_[8].c_str(),dpad,error))return false;
 if(!frame(controls_[3]+".CoolDown",std::max(infos[6]-1,0),error)||!movie_.set_visible((controls_[3]+".Grey").c_str(),!infos[7],error))return false;
 for(unsigned n=0;n<3;++n){unsigned slot=n;
  if(style_<2){ // Rolling list buttons carry their actual authored SlotId.
   struct Slot{const std::string* path;unsigned* slot;};Slot q{&controls_[4+n],&slot};
   if(!movie_.action_script(&q,[](void* p,SwfAsGraph& g,std::string& e){auto& q=*static_cast<Slot*>(p);SwfAsValue root,v,id;bool found{};double number{};
    if(!g.root_value(root,e)||!g.find_target(root,q.path->c_str(),v,e)||!g.get_member(v,"SlotId",id,found,e)||!found||!g.to_number(id,number,e)){if(e.empty())e="Required rolling HUD SlotId";return false;}
    *q.slot=number>=0&&number<=2?static_cast<unsigned>(number):0;return true;},error))return false;
  }
  if(!frame(controls_[4+n]+".CoolDown",std::max(infos[8+slot*2]-1,0),error)||!movie_.set_visible((controls_[4+n]+".Grey").c_str(),!infos[9+slot*2],error))return false;
 }
 return true;
}
bool AuthoredGameplayHudV1::geometry(AuthoredHudControlV1 control,float x,float y,AuthoredHudGeometryV1& out,std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}float point[2]{x,y};if(!movie_.screen_to_logical(point,error))return false;
 // Original screen_to_logical returns SWF pixels; cursor hit traversal
 // converts both coordinates to twips (same source swf_cursor_input path).
 out={};Geometry q{&control_path(control),point[0]*20.f,point[1]*20.f,&out};return with_authored_hud_edge_layout_v6(movie_,*this,[&](std::string& e){return movie_.action_script(&q,geometry_graph,e);},error);
}
bool AuthoredGameplayHudV1::release(AuthoredHudControlV1 control,std::string& error){
 if(control==AuthoredHudControlV1::attack||control==AuthoredHudControlV1::joystick){error="Required original native attack/joystick input transport; authored clip has no attack command handler";return false;}
 return invoke(control_path(control),"onRelease",{},error);
}
bool AuthoredGameplayHudV1::joystick_stick_offset(float x,float y,std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 // GameSWF _x/_y accept pixels; retained matrices and pointer geometry are twips.
 return movie_.set_number((controls_[8]+".stick._x").c_str(),(stick_rest_[0]+x)/20.f,error)
  &&movie_.set_number((controls_[8]+".stick._y").c_str(),(stick_rest_[1]+y)/20.f,error);
}
bool AuthoredGameplayHudV1::joystick_stick_reset(std::string& error){return joystick_stick_offset(0,0,error);}
bool AuthoredGameplayHudV1::joystick_stick_position(std::int32_t x,std::int32_t y,std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 return movie_.set_number((controls_[8]+".stick._x").c_str(),x,error)&&movie_.set_number((controls_[8]+".stick._y").c_str(),y,error);
}
bool AuthoredGameplayHudV1::joystick_background_width(float& width,std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 struct Width{std::string path;float* out;};Width q{controls_[8]+".bg",&width};
 return movie_.action_script(&q,[](void* p,SwfAsGraph& g,std::string& e){auto& q=*static_cast<Width*>(p);SwfAsValue root,value;gameswf::as_object* object{};
  if(!g.root_value(root,e)||!g.find_target(root,q.path.c_str(),value,e)||!g.borrow_object(value,object,e))return false;
  if(!object||!object->is(gameswf::character::m_class_id)){e="Required actual Joystick.bg character";return false;}
  *q.out=static_cast<gameswf::character*>(object)->get_width();return true;},error);
}
bool AuthoredGameplayHudV1::joystick_receiver_geometry(float x,float y,AuthoredHudGeometryV1& out,std::string& error){
 if(style_<0){error="Authored HUD is not bound";return false;}
 float point[2]{x,y};if(!movie_.screen_to_logical(point,error))return false;const auto path=controls_[8]+".stick";
 out={};Geometry q{&path,point[0]*20.f,point[1]*20.f,&out};return with_authored_hud_edge_layout_v6(movie_,*this,[&](std::string& e){return movie_.action_script(&q,geometry_graph,e);},error);
}
}
