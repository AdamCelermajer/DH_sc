// Real AVM1 players and callback dispatch; tiny SWF/provider fixtures isolate
// renderer ownership. This does not stand in for the application menu stack.
#include "swf_movie.hpp"
#include "swf_input_history.hpp"
#include "swf_frame_connection.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_render.h"
#include "gameswf/gameswf_function.h"
#include <fstream>
#include <cstdio>
#include <stdexcept>
using namespace dh2::ui;
void require(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
struct Glyph:gameswf::glyph_provider {
 gameswf::bitmap_info*get_char_image(gameswf::character_def*,Uint16,const tu_string&,bool,bool,int,gameswf::rect*,float*)override{return nullptr;}
};
struct Owner {
 std::shared_ptr<SwfInputHistory> history=std::make_shared<SwfInputHistory>();
 SwfFrameConnection frames;
};
struct Fixture {
 SwfMovie movie;Glyph glyph;std::string path,label;unsigned probes=0;
 Fixture* peer{};
 std::shared_ptr<Owner> owner=std::make_shared<Owner>();
 static bool read(void* c,const char*,std::vector<std::uint8_t>& out,std::string& error){
  std::ifstream file(static_cast<Fixture*>(c)->path,std::ios::binary);if(!file){error="fixture unavailable";return false;}
  out.assign(std::istreambuf_iterator<char>(file),{});return true;
 }
 static bool draw(void*,const SwfDraw&,std::string&){return true;}
 static bool start(void* c,const SwfAsLease& lease,std::string& e){auto& f=*static_cast<Fixture*>(c);return f.owner->history->bind(lease.player,e)&&f.owner->frames.bind(lease.player,f.owner->history,e);}
 static bool native(void* c,const char* name,const gameswf::fn_call& fn,std::string& e){
  auto& f=*static_cast<Fixture*>(c);
  require(reinterpret_cast<std::uintptr_t>(fn.get_player())==f.movie.player_identity(),"native player ownership changed");
  require(gameswf::get_glyph_provider()==&f.glyph,"native glyph provider ownership changed");
  if(std::string(name)=="EnterPeer"){
   require(f.peer,"native peer absent");auto* rh=gameswf::get_render_handler();
   require(f.peer->movie.menu_action_script(f.peer,probe,e),e);
   require(gameswf::get_render_handler()==rh&&gameswf::get_glyph_provider()==&f.glyph,"native caller providers not restored");
   require(reinterpret_cast<std::uintptr_t>(fn.get_player())==f.movie.player_identity(),"native caller environment changed");
   fn.result->set_string(f.peer->label.c_str());return true;
  }
  if(std::string(name)=="Reject"){e="fixture native rejection";return false;}
  require(std::string(name)=="Probe","unexpected native callback");++f.probes;fn.result->set_string(f.label.c_str());return true;
 }
 void load(){
  SwfServices s;s.context=this;s.read=read;s.draw=draw;s.glyphs=&glyph;s.native_owner=owner;
  s.graph_start=start;s.native_actions={"Probe","Reject","EnterPeer"};s.native_action=native;
  std::string e;require(movie.load({},"fixture.swf",s,e),e);require(movie.advance(0,e),e);
 }
 static bool probe(void* c,SwfAsGraph& graph,std::string& e){
  auto& f=*static_cast<Fixture*>(c);SwfAsValue root,value,result;bool found=false,callable=false;
  require(graph.root_value(root,e),e);
  require(graph.get_member(root,"Owner",value,found,e)&&found,"authored root Owner absent");
  std::string text;require(graph.to_text(value,text,e)&&text==f.label,"wrong renderer root");
  require(graph.invoke(root,root,"Probe",{},result,callable,e)&&callable,e);
  require(graph.to_text(result,text,e)&&text==f.label,"wrong renderer native callback");return true;
 }
};
struct Chain {
 Fixture* a;Fixture* b;unsigned guards=0;
 static bool throwing(void*,SwfAsGraph&,std::string&){throw std::runtime_error("fixture nested exception");}
 static bool inner(void* c,SwfAsGraph& graph,std::string& e){
  auto& t=*static_cast<Chain*>(c);require(Fixture::probe(t.b,graph,e),e);
  auto* rh=gameswf::get_render_handler();
  require(t.a->movie.menu_action_script(t.a,Fixture::probe,e),e);
  require(gameswf::get_render_handler()==rh&&gameswf::get_glyph_provider()==&t.b->glyph,"B providers not restored after B->A");
  return Fixture::probe(t.b,graph,e);
 }
 static bool outer(void* c,SwfAsGraph& graph,std::string& e){
  auto& t=*static_cast<Chain*>(c);require(Fixture::probe(t.a,graph,e),e);
  auto* rh=gameswf::get_render_handler();
  std::string rejected;
  require(!t.b->movie.action_script(t.b,Fixture::probe,rejected)&&rejected=="SWF core busy","ordinary cross-renderer recursion unexpectedly allowed");++t.guards;
  require(t.b->movie.menu_action_script(&t,inner,e),e);
  require(gameswf::get_render_handler()==rh&&gameswf::get_glyph_provider()==&t.a->glyph,"A providers not restored after A->B");
  SwfAsValue root,result;bool callable=false;std::string text;
  require(graph.root_value(root,e)&&graph.invoke(root,root,"EnterPeer",{},result,callable,e)&&callable,e);
  require(graph.to_text(result,text,e)&&text=="B","synchronous native renderer dispatch result lost");
  require(!t.b->movie.menu_action_script(nullptr,throwing,rejected)&&rejected=="fixture nested exception","nested exception lost");
  require(gameswf::get_render_handler()==rh&&gameswf::get_glyph_provider()==&t.a->glyph,"A providers not restored after exception");
  return Fixture::probe(t.a,graph,e);
 }
 static bool failure(void* c,SwfAsGraph& graph,std::string& e){
  auto& t=*static_cast<Chain*>(c);SwfAsValue root,result;bool callable=false;
  require(graph.root_value(root,e)&&graph.invoke(root,root,"Reject",{},result,callable,e)&&callable,e);
  std::string nested;
  require(!t.a->movie.menu_action_script(t.a,Fixture::probe,nested)&&nested=="fixture native rejection","same-renderer nesting cleared prior failure");
  require(t.b->movie.menu_action_script(t.b,Fixture::probe,e),e);
  return true;
 }
};
int main(int argc,char** argv){try{
 require(argc==2,"usage fixture-directory");Fixture a,b;a.path=std::string(argv[1])+"/a.swf";a.label="A";b.path=std::string(argv[1])+"/b.swf";b.label="B";
 Glyph sentinel;gameswf::set_glyph_provider(&sentinel);a.load();b.load();
 require(a.movie.player_identity()!=b.movie.player_identity(),"fixture players are shared");
 a.peer=&b;Chain chain{&a,&b};std::string e;
 require(a.movie.action_script(&chain,Chain::outer,e),e);
 require(gameswf::get_glyph_provider()==&sentinel&&gameswf::get_render_handler()==nullptr,"top-level providers not restored");
 require(!a.movie.action_script(&chain,Chain::failure,e)&&e=="fixture native rejection","outer native failure lost");
 require(a.movie.action_script(&a,Fixture::probe,e),"next top-level frame retained stale error");
 require(gameswf::get_glyph_provider()==&sentinel&&gameswf::get_render_handler()==nullptr,"failure cleanup changed providers");
 gameswf::set_glyph_provider(nullptr);
 std::printf("PASS | independent AVM1 roots/native dispatch | A->B->A restoration | synchronous native cross-renderer call | exception restoration | ordinary recursion rejected | same-renderer failure retained | next-frame recovery | probes %u %u\n",a.probes,b.probes);return 0;
}catch(const std::exception& e){std::fprintf(stderr,"FAIL | %s\n",e.what());return 1;}}
