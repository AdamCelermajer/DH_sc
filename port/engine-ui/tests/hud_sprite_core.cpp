#include "swf_movie.hpp" // audit-only copy adds one scoped visit method
#include "hud_sprite_core.hpp"
#include "hud_player_values.hpp"
#include "hud_advance_owner.hpp"
#include "gameswf/gameswf_sprite.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
using namespace dh2::ui;
namespace {
constexpr const char* digest="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
struct Test {
 std::string base;unsigned notify_calls=0,sound_queries=0,tag_cases=0,geometry=0,guards=0,strips=0,vertices=0,status_cases=0,status_services=0;int desired=0,id=0;float expected[6]{};bool has_matrix=false;HudAdvanceOwner advance;gameswf::character* status_root=nullptr;std::string* status_error=nullptr;
 static bool read(void* p,const char* uri,std::vector<std::uint8_t>& out,std::string& error){auto& t=*static_cast<Test*>(p);std::string s=uri;s=s.substr(s.find_last_of('/')+1);std::ifstream f(t.base+"/"+s,std::ios::binary);if(!f){error="Actual SWF missing";return false;}out.assign(std::istreambuf_iterator<char>(f),{});return true;}
 static bool texture(void*,const char*,int w,int h,SwfTexture& out,std::string&){out={123,w?w:1024,h?h:1024};return true;}
 static bool image(void*,int w,int h,unsigned,const std::uint8_t*,int,SwfTexture& out,std::string&){out={456,w,h};return true;}
 static bool draw(void* p,const SwfDraw& d,std::string&){auto& t=*static_cast<Test*>(p);if(d.kind==SwfDraw::triangle_strip){++t.strips;t.vertices+=unsigned(d.xy.size()/2);}return true;}
 static bool native(void*,const char* name,const std::vector<SwfValue>& args,SwfValue& out,std::string& error){if(std::string(name)!="NativeGetStringFromSymbol"){error="Unavailable native callback";return false;}out.kind=SwfValue::text;out.string=args.empty()?"":args[0].string;return true;}
 static bool stencil(void*,const float*,std::uint8_t,bool& out,std::string&){out=false;return true;}
 static bool notify(void* p,gameswf::sprite_instance* s,std::string& error){auto& t=*static_cast<Test*>(p);++t.notify_calls;return t.advance.notify(s,error);}
 static bool sound(void* p,std::uintptr_t& out,std::string&){++static_cast<Test*>(p)->sound_queries;out=reinterpret_cast<std::uintptr_t>(gameswf::get_sound_handler());return true;}
 static int visit(void* p,gameswf::character* c,std::string& e){auto& t=*static_cast<Test*>(p);HudSpriteCoreBindingV1 binding;check(bind_hud_sprite_v1(c,digest,binding,e),e);check(binding.id==t.id,"Unexpected retained HUD identity");HudSpriteCoreServices services{p,notify,sound,nullptr};check(hud_core_goto_v1(binding,t.desired,services,e)>=0,e);check(hud_core_play_v1(binding,1,services,e)==0,e);auto* s=binding.sprite;check(s->m_current_frame==t.desired&&s->m_play_state==gameswf::character::STOP,"Source frame/STOP mismatch");check(t.advance.needs_advance(s),"Required retained advance notification missing");
  if(t.has_matrix){gameswf::character* child=nullptr;std::string listing;for(int i=0;i<s->m_display_list.size();++i){auto* v=s->m_display_list.get_character(i);listing+=std::to_string(v->get_id())+":"+std::to_string(v->get_depth())+",";if(v->get_depth()==3)child=v;}check(child!=nullptr,"Authored depth3 child missing: "+listing);const auto& m=child->get_matrix();float actual[6]={m.m_[0][0],m.m_[0][1],m.m_[0][2],m.m_[1][0],m.m_[1][1],m.m_[1][2]};check(std::memcmp(actual,t.expected,24)==0,"Authored placement matrix mismatch");++t.geometry;}++t.tag_cases;return 0;
 }
 static int failures(void* p,gameswf::character* c,std::string& e){auto& t=*static_cast<Test*>(p);HudSpriteCoreBindingV1 b;check(!bind_hud_sprite_v1(c,"bad",b,e),"Unverified movie accepted");++t.guards;check(bind_hud_sprite_v1(c,digest,b,e),e);auto* s=b.sprite;const auto frame=s->m_current_frame;s->m_action_list.push_back(nullptr);check(hud_core_goto_v1(b,0,{},e)==-2&&s->m_current_frame==frame&&s->m_action_list.size()==1,"Pending history silently discarded");s->m_action_list.clear();++t.guards;s->m_goto_frame_action_list.push_back(nullptr);check(hud_core_goto_v1(b,0,{},e)==-2&&s->m_goto_frame_action_list.size()==1,"Prior batch silently discarded");s->m_goto_frame_action_list.clear();++t.guards;
  HudSpriteCoreServices absent{};check(hud_core_play_v1(b,0,absent,e)==-2,"Sound lookup silently absent");++t.guards;HudSpriteCoreServices no_notify{p,nullptr,sound,nullptr};check(hud_core_play_v1(b,1,no_notify,e)==-2&&s->m_play_state==gameswf::character::STOP,"Required notify prefix missing");++t.guards;
  const int old=s->m_def->m_ss_id;s->m_def->m_ss_id=7;check(hud_core_goto_v1(b,0,{p,notify,sound,nullptr},e)==-2,"Unproved frame audio silently accepted");s->m_def->m_ss_id=old;++t.guards;b.version=2;check(hud_core_goto_v1(b,0,{p,notify,sound,nullptr},e)==-1,"Unknown adapter version accepted");++t.guards;return 0;
 }
 gameswf::character* resolve(unsigned index){std::string path=hud_value_clip_path(HudValueClip(index));if(index!=4)path="_root.menu_HUD_0."+path;auto* obj=status_root->find_target(gameswf::as_value(path.c_str()));check(obj&&obj->is(gameswf::character::m_class_id),"Source cached HUD path missing");return static_cast<gameswf::character*>(obj);}
 static int status(void* p,HudValuesState24*,const HudValueRequest32* r,HudValueResponse16* out){auto& t=*static_cast<Test*>(p);++t.status_services;auto& error=*t.status_error;
  if(r->operation==HudValueOperation::resolve_clip){out->clip=reinterpret_cast<std::uintptr_t>(t.resolve(unsigned(r->index)));return 1;}
  auto* c=reinterpret_cast<gameswf::character*>(r->clip);if(r->operation==HudValueOperation::is_sprite){out->value=c->is(gameswf::sprite_instance::m_class_id);return 1;}
  HudSpriteCoreBindingV1 binding;if(!bind_hud_sprite_v1(c,digest,binding,error))return 0;HudSpriteCoreServices services{p,notify,sound,nullptr};if(r->operation==HudValueOperation::goto_frame)return hud_core_goto_v1(binding,r->value,services,error)>=0?1:0;if(r->operation==HudValueOperation::set_play_state)return hud_core_play_v1(binding,r->value,services,error)==0?1:0;return 0;
 }
 static int status_visit(void* p,gameswf::character* c,std::string& e){auto& t=*static_cast<Test*>(p);t.status_root=c;t.status_error=&e;std::int32_t sheet[44]{};sheet[38]=sheet[43]=sheet[34]=100;HudValuesState24 state{sheet,44,0,1};HudValueServices16 services{p,status};const int values[8]={100,75,50,25,1,0,-1,101},hp[8]={99,74,49,24,0,0,0,99},mp[8]={99,74,49,24,0,0,0,99},xp[8]={99,75,50,25,1,0,0,99};
  for(unsigned n=0;n<8;++n){sheet[36]=sheet[41]=sheet[33]=values[n];check(dh2_ui_hud_player_values(&state,&services)==0,e);const int expected[5]={hp[n],mp[n],xp[n],hp[n],hp[n]};for(unsigned i=0;i<5;++i){auto* s=static_cast<gameswf::sprite_instance*>(t.resolve(i));check(s->m_current_frame==expected[i]&&s->m_play_state==gameswf::character::STOP,"Frozen status producer did not reach actual five clip frames");}++t.status_cases;}
  t.status_root=nullptr;t.status_error=nullptr;return 0;
 }
 SwfServices services(){SwfServices s;s.context=this;s.read=read;s.texture=texture;s.image=image;s.draw=draw;s.native_call=native;s.stencil=stencil;return s;}
};
unsigned u32(const unsigned char* p){unsigned v;std::memcpy(&v,p,4);return v;}
}
int main(int argc,char** argv){try{if(argc!=3)return 2;Test t;t.base=argv[1];std::ifstream f(argv[2],std::ios::binary);std::vector<unsigned char> data((std::istreambuf_iterator<char>(f)),{});check(data.size()>=8&&u32(data.data())==0x31505348,"Malformed authored matrix fixture");const auto count=u32(data.data()+4);check(data.size()==8+count*32,"Malformed matrix fixture count");SwfMovie movie;std::string error;check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",t.services(),error),error);check(movie.advance(0,error),error);const char* path0="_root.menu_HUD_0.HUDelements.HealthBars.player.bar_hp";
 for(unsigned i=0;i<count;++i){const auto* p=data.data()+8+i*32;t.id=int(u32(p));t.desired=int(u32(p+4));std::memcpy(t.expected,p+8,24);t.has_matrix=true;const char* suffix=t.id==90?"bar_hp":t.id==147?"bar_mp":"bar_xp";std::string path="_root.menu_HUD_0.HUDelements.HealthBars.player.";path+=suffix;check(movie.audit_visit_clip(path.c_str(),&t,Test::visit,error),error);if(i%25==0)check(movie.display_clip(path.c_str(),0,0,480,320,error),error);}
 for(const auto& entry:std::vector<std::pair<int,const char*>>{{113,"_root.menu_HUD_0.HUDelements.HealthBars.btn_potion.DistressGlow"},{31,"_root.HurtCorners"}}){t.id=entry.first;t.has_matrix=false;for(int frame=0;frame<103;++frame){t.desired=frame;check(movie.audit_visit_clip(entry.second,&t,Test::visit,error),error);}for(int frame=102;frame>=0;--frame){t.desired=frame;check(movie.audit_visit_clip(entry.second,&t,Test::visit,error),error);}}
 check(movie.audit_visit_clip(path0,&t,Test::failures,error),error);check(movie.audit_visit_clip("_root",&t,Test::status_visit,error),error);check(t.strips&&t.vertices&&t.geometry==count,"Actual geometry draw missing");const auto live=t.advance.live_nodes();check(live>0&&t.advance.dirty_nodes()==live,"Real advance records missing");t.advance.reset_after_advance();check(t.advance.dirty_nodes()==0,"Explicit native advance consume failed");movie=SwfMovie();check(t.advance.live_nodes()==0,"Advance owner retained dead movie objects");std::cout<<"{\"validation\":\"PASS\",\"retained_clip_cases\":"<<t.tag_cases<<",\"authored_matrix_comparisons\":"<<t.geometry<<",\"frozen_status_real_core_cases\":"<<t.status_cases<<",\"frozen_status_services\":"<<t.status_services<<",\"required_advance_deliveries\":"<<t.notify_calls<<",\"genuine_upstream_sound_queries\":"<<t.sound_queries<<",\"source_owned_weak_nodes\":"<<live<<",\"advance_ownership_guards\":3,\"failure_guards\":"<<t.guards<<",\"triangle_strips\":"<<t.strips<<",\"vertices\":"<<t.vertices<<"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
