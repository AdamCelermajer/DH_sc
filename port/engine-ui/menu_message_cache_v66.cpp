#include "menu_message_cache_v66.hpp"
#include "swf_movie.hpp"
#include "gameswf/gameswf_character.h"
#include "base/weak_ptr.h"
namespace dh2::ui {namespace {
bool same_owner(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){
 return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);
}
bool character(SwfAsGraph& graph,const SwfAsValue& value,gameswf::character*& out,std::string& e){
 gameswf::as_object* object{};if(!graph.borrow_object(value,object,e))return false;
 if(object&&!object->is(gameswf::character::m_class_id)){e="Cached source receiver is not an actual SWF character";return false;}
 out=static_cast<gameswf::character*>(object);return true;
}
}
struct DebugCachedCharacterV66::State {
 //41aeec: byte0=0/count4=0/path8empty/render20NULL/parent24NULL/weak28NULL.
 std::uint8_t refresh0{};std::uint32_t changed4{};std::string path8;
 std::uintptr_t render20{};weak_ptr<gameswf::character> parent24,cached28;
 std::weak_ptr<void> generation;
};
DebugCachedCharacterV66::DebugCachedCharacterV66():state_(std::make_unique<State>()){}
DebugCachedCharacterV66::~DebugCachedCharacterV66()=default;
void DebugCachedCharacterV66::refresh_null_character()noexcept{}
const std::string& DebugCachedCharacterV66::path()const noexcept{return state_->path8;}
std::uint32_t DebugCachedCharacterV66::changed_count()const noexcept{return state_->changed4;}
bool DebugCachedCharacterV66::refresh_character(SwfAsGraph& graph,const SwfAsValue& value,
 std::uintptr_t render,const SwfAsValue* parent,std::string& e){
 gameswf::character* actual{};if(!character(graph,value,actual,e))return false;
 if(!actual){e.clear();return true;}
 gameswf::character* actual_parent{};if(parent&&!character(graph,*parent,actual_parent,e))return false;
 std::shared_ptr<void> generation;if(!graph.scope_identity_v59(generation,e))return false;
 // Preserve source assignment order: weak character, source name, parent, render.
 state_->cached28=actual;state_->generation=generation;
 state_->path8=actual->get_name().c_str();state_->parent24=actual_parent;state_->render20=render;
 e.clear();return true;
}
bool DebugCachedCharacterV66::refresh_name(SwfAsGraph& graph,const char* path,
 std::uintptr_t render,const SwfAsValue* parent,std::string& e){
 if(!path){e="Required source cached-character path";return false;}
 SwfAsValue base,value;gameswf::character* actual_parent{};
 if(parent&&!character(graph,*parent,actual_parent,e))return false;
 if(actual_parent)base=*parent;else if(!graph.root_value(base,e))return false;
 if(!graph.find_target(base,path,value,e))return false;
 gameswf::character* actual{};
 if(!character(graph,value,actual,e))return false;
 std::shared_ptr<void> generation;if(!graph.scope_identity_v59(generation,e))return false;
 state_->cached28=actual;state_->generation=generation;state_->path8=path;
 state_->render20=render;state_->parent24=actual_parent;
 // weak_ptr validates the upstream proxy (the final427ce0 branch).
 (void)state_->cached28.get_ptr();e.clear();return true;
}
bool DebugCachedCharacterV66::alive_in(SwfAsGraph& graph,bool& out,std::string& e){
 std::shared_ptr<void> generation;if(!graph.scope_identity_v59(generation,e))return false;
 // Native backend generation replaces original renderer graph destruction;
 // no strong AS value is retained here to keep an unloaded graph alive.
 out=same_owner(state_->generation.lock(),generation)&&state_->cached28.get_ptr();
 e.clear();return true;
}
bool DebugCachedCharacterV66::get_character(SwfAsGraph& graph,SwfAsValue& out,std::string& e){
 bool alive{};if(!alive_in(graph,alive,e))return false;
 auto* before=alive?state_->cached28.get_ptr():nullptr;
 if(state_->refresh0){
  SwfAsValue parent;const SwfAsValue* parent_arg{};
  if(auto* p=state_->parent24.get_ptr()){
   if(!graph.retain_object(p,parent,e))return false;parent_arg=&parent;
  }
  const auto path=state_->path8;
  if(!refresh_name(graph,path.c_str(),state_->render20,parent_arg,e))return false;
 }
 auto* after=state_->refresh0?state_->cached28.get_ptr():before;
 if(after!=before){++state_->changed4;
  // Original walks cached character's weak parent chain for diagnostics.
  for(auto* p=state_->cached28.get_ptr();p&&p->get_parent();p=p->get_parent()){}
 }
 // Source assertion mode99f914 is BSS0. No diagnostic-mode/trap invented.
 return graph.retain_object(after,out,e);
}
void MenuMessageCachesV66::level_stage26_refresh_null()noexcept{
 cache(MessageFamilyV66::dialog).refresh_null_character();
 cache(MessageFamilyV66::achievement).refresh_null_character();
 cache(MessageFamilyV66::dialog).refresh_null_character();
 cache(MessageFamilyV66::status).refresh_null_character();
 cache(MessageFamilyV66::online).refresh_null_character();
}
const char* MenuMessageCachesV66::start_method(MessageFamilyV66 family)noexcept{
 switch(family){case MessageFamilyV66::status:return "onStatusMessage";
 case MessageFamilyV66::online:return "onOnlineStatusMessage";
 case MessageFamilyV66::dialog:return "StartDialog";
 case MessageFamilyV66::achievement:return "onAchievementMessage";
 case MessageFamilyV66::tutorial:return "onTutorialMessage";}return nullptr;
}
bool MenuMessageCachesV66::invoke(SwfMovie& movie,MessageFamilyV66 family,
 std::uintptr_t expected_root,const char* method,std::int32_t context,std::string& e){
 if(!method){e="Required original message method";return false;}
 struct Call {DebugCachedCharacterV66& cache;std::uintptr_t root,render;const char* method;std::int32_t context;const char* path;
  static bool run(void* p,SwfAsGraph& graph,std::string& e){
   auto& c=*static_cast<Call*>(p);SwfAsValue root,receiver,result;
   if(!graph.root_value(root,e))return false;
   if(root.identity()!=c.root){e="Message invocation addressed another actual HUD root";return false;}
   bool alive{};if(!c.cache.alive_in(graph,alive,e))return false;
   if(!alive&&!c.cache.refresh_name(graph,c.path,c.render,nullptr,e))return false;
   if(!c.cache.get_character(graph,receiver,e))return false;
   if(!receiver.identity()){e="Required original message cached character";return false;}
   bool callable{};
   if(!graph.invoke(receiver,receiver,c.method,{SwfAsValue::number(c.context)},result,callable,e))return false;
   if(!callable){e="Required original message HUD ActionScript method";return false;}return true;
  }
 } call{cache(family),expected_root,reinterpret_cast<std::uintptr_t>(&movie),method,context,
  family==MessageFamilyV66::tutorial?"_root.tutorials_dialog":"_root"};
 return movie.menu_action_script(&call,Call::run,e);
}
}
