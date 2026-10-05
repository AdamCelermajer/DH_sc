#include "character_menu_movie_v1.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_function.h"
#include <algorithm>
#include <cstring>
namespace dh2::ui {
namespace {
#include "reference/character-movie-startup-v1/root_menu_catalog_v1.inc"
#include "reference/character-movie-startup-v1/native_callback_names_v1.inc"
// Source static initialization uses "LINUX" for getVersion, while action_init
// publishes a separate literal "gameSWF" builtin $version. They are distinct.
void source_get_version(const gameswf::fn_call& fn){if(fn.result)fn.result->set_string("LINUX");}
}
struct CharacterMenuMovieV1::Provider {
 SwfServices source;
 static Provider& self(void* p){return *static_cast<Provider*>(p);}
 static bool read(void* p,const char* uri,std::vector<std::uint8_t>& bytes,std::string& error){auto& s=self(p).source;return s.read&&s.read(s.context,uri,bytes,error);}
 static bool texture(void* p,const char* name,int w,int h,SwfTexture& out,std::string& error){auto& s=self(p).source;return s.texture&&s.texture(s.context,name,w,h,out,error);}
 static bool image(void* p,int w,int h,unsigned channels,const std::uint8_t* pixels,int pitch,SwfTexture& out,std::string& error){auto& s=self(p).source;return s.image&&s.image(s.context,w,h,channels,pixels,pitch,out,error);}
 static bool draw(void* p,const SwfDraw& draw,std::string& error){auto& s=self(p).source;return s.draw&&s.draw(s.context,draw,error);}
 static bool native(void* p,const char* name,const std::vector<SwfValue>& args,SwfValue& out,std::string& error){auto& s=self(p).source;return s.native_call&&s.native_call(s.context,name,args,out,error);}
 static bool stencil(void* p,const float bounds[4],std::uint8_t pattern,bool& out,std::string& error){auto& s=self(p).source;return s.stencil&&s.stencil(s.context,bounds,pattern,out,error);}
 static void diagnostic(void* p,bool failure,const char* message){auto& s=self(p).source;if(s.diagnostic)s.diagnostic(s.context,failure,message);}
 static bool action(void* p,const char* name,const gameswf::fn_call& fn,std::string& error){auto& s=self(p).source;return s.native_action&&s.native_action(s.context,name,fn,error);}
 static bool start(void* p,const SwfAsLease& lease,std::string& error){
  auto& s=self(p).source;
  if(!lease.owner||!lease.player||lease.root){error="Character movie globals require the fresh owning player before root loading";return false;}
  auto* global=lease.player->get_global();
  if(!global){error="Actual character movie global object unavailable";return false;}
  global->builtin_member("$version",gameswf::as_value("gameSWF"));
  global->builtin_member("getVersion",gameswf::as_value(source_get_version));
  return !s.graph_start||s.graph_start(s.context,lease,error);
 }
 SwfServices wrapped(const std::shared_ptr<Provider>& lease){
  SwfServices s=source;s.context=this;s.native_owner=lease;
  // Every authored native name is routed to the retained game dispatcher.
  // An unsupported reached name must reject through that dispatcher, rather
  // than silently become an undefined AS function and continue initialization.
  for(const auto* name:source_native_callbacks)
   if(std::find(s.native_actions.begin(),s.native_actions.end(),name)==s.native_actions.end())s.native_actions.emplace_back(name);
  s.read=read;s.texture=texture;s.image=image;s.draw=draw;s.native_call=native;
  s.stencil=stencil;s.diagnostic=diagnostic;s.native_action=action;s.graph_start=start;
  return s;
 }
};
CharacterMenuMovieV1::CharacterMenuMovieV1()=default;
CharacterMenuMovieV1::~CharacterMenuMovieV1()=default;
bool CharacterMenuMovieV1::load(const SwfServices& source,std::string& error){
 error.clear();
 if(!source.native_owner||!source.native_action||!source.read||!source.draw||
    std::find(source.native_actions.begin(),source.native_actions.end(),"NativeReloadSkills")==source.native_actions.end()){
  error="Character movie requires retained game services and the original startup NativeReloadSkills binding";return false;
 }
 auto provider=std::make_shared<Provider>();provider->source=source;
 auto movie=std::make_unique<SwfMovie>();
 if(!movie->load({"dqshared_droid.swf"},"dqcharmenu_droid.swf",provider->wrapped(provider),error)||!movie->advance(0,error))return false;
 struct Bind {
  std::vector<CharacterMenuScreenV1> screens;
  static bool apply(void* p,SwfAsGraph& graph,std::string& error){
   auto& b=*static_cast<Bind*>(p);SwfAsValue root;
   if(!graph.root_value(root,error))return false;
   for(const auto& definition:source_screens){
    SwfAsValue receiver;
    const std::string path=std::string("_root.")+definition.name;
    if(!graph.find_target(root,path.c_str(),receiver,error)||receiver.kind()!=SwfAsValue::Kind::object){if(error.empty())error="Authored character menu receiver unavailable: "+path;return false;}
    b.screens.push_back({definition,std::move(receiver)});
   }
   return true;
  }
 } binding;
 if(!movie->action_script(&binding,Bind::apply,error))return false;
 for(const auto& screen:binding.screens){
  SwfClipInfo clip;const std::string path=std::string("_root.")+screen.definition.name;
  if(!movie->clip(path.c_str(),clip,error))return false;
  if(clip.id!=screen.definition.id||clip.depth!=screen.definition.depth||clip.frames!=screen.definition.frames){error="Character menu runtime layout disagrees with original source catalog: "+path;return false;}
 }
 // Publish only a fully loaded, source-identified actual graph. Failed reload
 // leaves the prior graph and its held records unchanged.
 screens_.clear();movie_=std::move(movie);screens_=std::move(binding.screens);loaded_=true;return true;
}
bool CharacterMenuMovieV1::advance(float dt,std::string& error){if(!loaded_){error="Character movie not loaded";return false;}return movie_->advance(dt,error);}
bool CharacterMenuMovieV1::display(int x,int y,int w,int h,std::string& error){if(!loaded_){error="Character movie not loaded";return false;}return movie_->display(x,y,w,h,error);}
bool CharacterMenuMovieV1::action_script(void* context,bool(*apply)(void*,SwfAsGraph&,std::string&),std::string& error){if(!loaded_){error="Character movie not loaded";return false;}return movie_->action_script(context,apply,error);}
bool CharacterMenuMovieV1::invoke(const char* screen,const char* method,const std::vector<SwfAsValue>& args,SwfAsValue& result,bool& callable,std::string& error){
 if(!loaded_||!screen||!method){error="Character movie invocation requires a loaded graph and named source receiver";return false;}
 const auto record=std::find_if(screens_.begin(),screens_.end(),[&](const auto& r){return !std::strcmp(r.definition.name,screen);});
 if(record==screens_.end()){error="Unknown source character menu receiver";return false;}
 struct Invocation {
  SwfAsValue receiver;const char* method;const std::vector<SwfAsValue>& args;SwfAsValue& result;bool& callable;
  static bool apply(void* p,SwfAsGraph& graph,std::string& error){auto& i=*static_cast<Invocation*>(p);return graph.invoke(i.receiver,i.receiver,i.method,i.args,i.result,i.callable,error);}
 } invocation{record->receiver,method,args,result,callable};
 return movie_->action_script(&invocation,Invocation::apply,error);
}
}
