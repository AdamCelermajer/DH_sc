#include "swf_event_dispatch.hpp"
#include "swf_event_core.hpp"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_function.h"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
using namespace dh2::ui;
namespace {
using Bytes=std::vector<unsigned char>;
const char* names[]={"button","btnDelete","btn_GAMEPLAYMENUS_ACCEPT","btn_GAMEPLAYMENUS_REFUSE","btn_Legend","btn_deadzone",u8"\u00e9button"};
const char* methods[]={"on_focus_in","on_focus_out","on_clicked","onPress","onRelease","onReleaseOutside","onRollOver","onRollOut","onDragOver","onDragOut"};
std::uint32_t word(const unsigned char*p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
void append(Bytes& b,std::uint32_t v){auto n=b.size();b.resize(n+4);std::memcpy(b.data()+n,&v,4);}
Bytes snapshot(const SwfEvent48&e){Bytes out;append(out,e.character!=0);unsigned n=0;while(n<7&&std::strcmp(e.name,names[n]))++n;if(n==7)throw std::runtime_error("Unexpected fixture name");append(out,n);auto*p=reinterpret_cast<const unsigned char*>(&e.kind);out.insert(out.end(),p,p+32);return out;}
struct Context {
 SwfEvent48* event{};std::uint32_t* selection{};unsigned na{},aa{},depth{};bool fail_native{},fail_as{};
 std::vector<Bytes> calls;
 void mutate(bool as=false){auto&e=*event;if(as){if(aa){e.name=names[std::array<unsigned,5>{0,4,2,3,5}[aa]];e.kind=0;e.consumed=1;}return;}
  switch(na){case 1:e.consumed=1;break;case 2:e.kind=6;e.name=names[1];break;case 3:e.kind=11;e.character=0;break;case 4:e.value1=-1;e.flag=254;break;default:break;}}
 static int native(void*p,SwfEvent48*e){auto&c=*static_cast<Context*>(p);c.event=e;Bytes b;append(b,1);auto s=snapshot(*e);b.insert(b.end(),s.begin(),s.end());c.calls.push_back(b);c.mutate();
  if(c.na==5&&!c.depth){c.depth=1;e->kind=6;SwfEventServices24 svc{&c,native,method};if(dh2_ui_swf_send_event(e,c.selection,&svc))return 0;c.depth=0;}
  return !c.fail_native;}
 static int method(void*p,std::uintptr_t character,const char*name){auto&c=*static_cast<Context*>(p);unsigned m=0;while(m<10&&std::strcmp(name,methods[m]))++m;if(m==10)throw std::runtime_error("Unexpected method");Bytes b;append(b,2);append(b,character!=0);append(b,m);c.calls.push_back(b);c.mutate(true);return !c.fail_as;}
};
struct Core {
 gameswf::gc_ptr<gameswf::player> player;gameswf::gc_ptr<gameswf::movie_def_impl> definition;gameswf::gc_ptr<gameswf::root> root;unsigned* destroyed;
 explicit Core(unsigned*d):destroyed(d){player=new gameswf::player;definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);auto&r=definition->m_frame_size;r.m_x_min=0;r.m_x_max=9600;r.m_y_min=0;r.m_y_max=6400;definition->set_frame_count(1);definition->m_playlist.resize(1);root=definition->create_root();}
 ~Core(){root->get_root_movie()->m_members.clear();root->get_root_movie()->m_proto=nullptr;player->get_global()->m_members.clear();player->get_global()->m_proto=nullptr;player->clear_heap();definition->m_instance=nullptr;player->m_current_root=nullptr;root=nullptr;definition=nullptr;player=nullptr;++*destroyed;}
};
struct Actual {SwfViewportLease*lease{};gameswf::character* expected{};unsigned calls{};unsigned* destroyed{};bool release{},reenter{};gameswf::character* environment{};};
Actual* active=nullptr;
void called(const gameswf::fn_call& f){auto&a=*active;if(f.nargs||f.this_ptr!=a.expected||(a.environment&&f.env!=a.environment->get_environment()))throw std::runtime_error("Actual AS receiver/environment changed");++a.calls;f.result->set_string("source discarded text result");
 if(a.reenter){a.reenter=false;bool invoked=false;std::string error;if(!swf_event_method(*a.lease,a.expected,"onPress",invoked,error)||!invoked)throw std::runtime_error("Actual AS method reentry failed");}
 if(a.release){a.release=false;a.lease->root=nullptr;a.lease->owner.reset();if(*a.destroyed)throw std::runtime_error("Graph closed inside AS method");}}
}
int main(int argc,char**argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);Bytes bytes((std::istreambuf_iterator<char>(f)),{});if(bytes.size()<8||word(bytes.data())!=0x31455653)return 2;
 std::size_t at=8;unsigned comparisons=word(bytes.data()+4),calls=0,guards=0,reentry=0;
 for(unsigned i=0;i<comparisons;++i){if(at+12>bytes.size())return 2;auto il=word(bytes.data()+at),ol=word(bytes.data()+at+4),ec=word(bytes.data()+at+8);at+=12;if(il!=56||ol!=44||at+il+ol>bytes.size())return 2;auto*raw=bytes.data()+at;at+=il;Bytes expected(bytes.begin()+at,bytes.begin()+at+ol);at+=ol;std::vector<Bytes> expected_calls;
  for(unsigned k=0;k<ec;++k){if(at+4>bytes.size())return 2;auto n=word(bytes.data()+at);at+=4;if(at+n>bytes.size())return 2;if(word(bytes.data()+at)!=3)expected_calls.emplace_back(bytes.begin()+at,bytes.begin()+at+n);at+=n;}
  SwfEvent48 event{};event.character=word(raw);event.name=names[word(raw+4)];std::memcpy(&event.kind,raw+8,32);std::uint32_t selection=word(raw+40);Context context{&event,&selection,word(raw+44),word(raw+48)};SwfEventServices24 svc{&context,Context::native,Context::method};
  if(dh2_ui_swf_send_event(&event,&selection,&svc))return 1;auto actual=snapshot(event);append(actual,selection);if(actual!=expected||context.calls!=expected_calls)return 1;calls+=unsigned(context.calls.size());if(context.na==5)++reentry;
 }
 if(at!=bytes.size())return 2;
 SwfEvent48 event{1,"button",4,0,0,0,0,0,0,0,0,0};std::uint32_t selection=42;Context context{&event,&selection};SwfEventServices24 svc{&context,Context::native,Context::method};const auto before=snapshot(event);
 if(dh2_ui_swf_send_event(nullptr,&selection,&svc)!=-1||selection!=42)return 1;++guards;
 if(dh2_ui_swf_send_event(&event,nullptr,&svc)!=-1||snapshot(event)!=before)return 1;++guards;
 if(dh2_ui_swf_send_event(&event,&selection,nullptr)!=-1||snapshot(event)!=before)return 1;++guards;
 auto bad=event;bad.name=nullptr;if(dh2_ui_swf_send_event(&bad,&selection,&svc)!=-1)return 1;++guards;
 auto absent=svc;absent.native_event=nullptr;if(dh2_ui_swf_send_event(&event,&selection,&absent)!=-2||snapshot(event)!=before)return 1;++guards;
 context.fail_native=true;context.na=2;if(dh2_ui_swf_send_event(&event,&selection,&svc)!=-2||event.kind!=6||std::strcmp(event.name,"btnDelete"))return 1;++guards;
 context.fail_native=false;context.na=0;context.fail_as=true;if(dh2_ui_swf_send_event(&event,&selection,&svc)!=-2||selection!=42)return 1;++guards;
 absent=svc;absent.as_method=nullptr;event.consumed=1;if(dh2_ui_swf_send_event(&event,&selection,&absent)!=0)return 1;++guards;
 unsigned destroyed=0;auto core=std::make_shared<Core>(&destroyed);SwfViewportLease lease{core,core->root.get_ptr()};auto* sprite=core->root->get_root_movie();sprite->set_member("onPress",gameswf::as_value(called));bool invoked=false;std::string error;Actual actual{&lease,sprite,0,&destroyed};active=&actual;
 unsigned gates=0,gate_callbacks=0;
 for(unsigned flags=0;flags<32;++flags){bool present=flags&16,is_sprite=flags&8,parent_present=flags&4,parent_sprite=flags&2,alive=flags&1;
  gameswf::gc_ptr<gameswf::character> input=is_sprite?static_cast<gameswf::character*>(new gameswf::sprite_instance(core->player.get_ptr(),core->definition.get_ptr(),core->root.get_ptr(),nullptr,201)):new gameswf::character(core->player.get_ptr(),nullptr,201);
  gameswf::gc_ptr<gameswf::character> parent=parent_sprite?static_cast<gameswf::character*>(new gameswf::sprite_instance(core->player.get_ptr(),core->definition.get_ptr(),core->root.get_ptr(),nullptr,202)):new gameswf::character(core->player.get_ptr(),nullptr,202);
  input->set_member("onPress",gameswf::as_value(called));parent->set_member("onPress",gameswf::as_value(called));input->set_parent(parent_present?parent.get_ptr():nullptr);
  auto detach=[&](gameswf::character*c){c->m_members.clear();c->m_proto=nullptr;if(auto*env=c->get_environment())env->m_target=nullptr;core->player->m_heap.erase(c);c->set_parent(nullptr);};
  actual.expected=input.get_ptr();actual.environment=is_sprite?input.get_ptr():parent.get_ptr();if(!alive){detach(parent.get_ptr());parent=nullptr;}
  bool expected=present&&(is_sprite||(parent_present&&parent_sprite&&alive));auto old=actual.calls;if(!swf_event_method(lease,present?input.get_ptr():nullptr,"onPress",invoked,error)||invoked!=expected||actual.calls-old!=unsigned(expected)){std::cerr<<"Method gate "<<flags<<" invoked "<<invoked<<" expected "<<expected<<" calls "<<actual.calls-old<<" error "<<error<<"\n";return 1;}
  gate_callbacks+=actual.calls-old;++gates;detach(input.get_ptr());input=nullptr;if(parent){detach(parent.get_ptr());parent=nullptr;}
 }
 actual.calls=0;actual.expected=sprite;actual.environment=sprite;
 if(!swf_event_method(lease,nullptr,"onPress",invoked,error)||invoked)return 1;++guards;
 if(!swf_event_method(lease,sprite,"missingSourceMethod",invoked,error)||!invoked||actual.calls)return 1;++guards;
 if(!swf_event_method(lease,sprite,"onPress",invoked,error)||!invoked||actual.calls!=1)return 1;++guards;
 auto child=gameswf::gc_ptr<gameswf::character>(new gameswf::character(core->player.get_ptr(),sprite,101));actual.expected=child.get_ptr();if(!swf_event_method(lease,child.get_ptr(),"onPress",invoked,error)||!invoked||actual.calls!=2)return 1;++guards;
 child->set_parent(nullptr);if(!swf_event_method(lease,child.get_ptr(),"onPress",invoked,error)||invoked)return 1;++guards;child=nullptr;
 actual.expected=sprite;actual.reenter=true;if(!swf_event_method(lease,sprite,"onPress",invoked,error)||actual.calls!=4)return 1;++guards;
 auto weak=std::weak_ptr<Core>(core);core.reset();actual.release=true;if(!swf_event_method(lease,sprite,"onPress",invoked,error)||!invoked||actual.calls!=5||destroyed!=1||!weak.expired())return 1;++guards;
 if(swf_event_method(lease,nullptr,"onPress",invoked,error))return 1;++guards;active=nullptr;
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_comparisons\":"<<comparisons<<",\"ordered_native_and_AS_requests\":"<<calls<<",\"original_bound_reentry_cases\":"<<reentry<<",\"native_failure_and_actual_core_guards\":"<<guards<<",\"original_bound_actual_core_method_gates\":"<<gates<<",\"actual_AS_method_callbacks\":"<<gate_callbacks+5<<",\"actual_UI_library_executed\":true,\"whole_input_or_frame_parity\":false,\"mismatches\":0}\n";
}
