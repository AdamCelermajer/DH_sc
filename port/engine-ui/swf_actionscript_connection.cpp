#include "swf_actionscript_connection.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_function.h"
#include <cstring>
#include <exception>
#include <limits>
#include <set>
namespace dh2::ui {
struct SwfAsGraph::State {
 SwfAsLease lease;
 SwfAsServices services;
 bool enabled{true};
 std::set<std::string> registered;
 bool allowed()const{return enabled&&lease.owner&&lease.player&&services.within_scope&&services.within_scope(services.context,lease.owner.get());}
 void fail(const std::string& e)const{if(services.failure)services.failure(services.context,e);}
 struct Native:gameswf::as_function {
  std::weak_ptr<State> graph;SwfAsServices provider;std::string name;
  Native(const std::shared_ptr<State>& s,const std::string& n):as_function(s->lease.player),graph(s),provider(s->services),name(n){}
  void operator()(const gameswf::fn_call& fn)override{
   const auto s=graph.lock();if(!s){if(provider.failure)provider.failure(provider.context,"Required native AS graph released: "+name);return;}
   // Strong local owner and provider copies protect callback-triggered release.
   const auto provider=s->services;
   if(!s->allowed()){s->fail("Native AS dispatch outside retained movie Scope: "+name);return;}
   std::string error;
   try{if(!provider.native_call||!provider.native_call(provider.context,name.c_str(),fn,error))
    s->fail(error.empty()?"Required native AS callback unavailable: "+name:error);
   }catch(const std::exception& e){s->fail(e.what());}
  }
 };
};
struct SwfAsValue::Pin {
 // Native value releases BEFORE the exact player/graph/provider lease.
 std::shared_ptr<SwfAsGraph::State> graph;
 gameswf::as_value value;
 Pin(const gameswf::as_value& v,std::shared_ptr<SwfAsGraph::State> s={}):graph(std::move(s)),value(v){}
};
SwfAsValue SwfAsValue::boolean(bool b){SwfAsValue v;v.pin_=std::make_shared<Pin>(gameswf::as_value(b));return v;}
SwfAsValue SwfAsValue::number(double n){SwfAsValue v;v.pin_=std::make_shared<Pin>(gameswf::as_value(n));return v;}
SwfAsValue SwfAsValue::text(const std::string& s){SwfAsValue v;v.pin_=std::make_shared<Pin>(gameswf::as_value(s.c_str()));return v;}
SwfAsValue SwfAsValue::null(){SwfAsValue v;v.pin_=std::make_shared<Pin>(gameswf::as_value(static_cast<gameswf::as_object*>(nullptr)));return v;}
SwfAsValue::Kind SwfAsValue::kind()const noexcept{
 if(!pin_||pin_->value.is_undefined())return Kind::undefined;
 const auto& v=pin_->value;
 if(v.is_bool())return Kind::boolean;
 if(v.is_string())return Kind::text;
 if(v.is_null())return Kind::null_value;
 if(v.is_object())return Kind::object;
 if(v.is_property())return Kind::property;
 return Kind::number;
}
std::uintptr_t SwfAsValue::identity()const noexcept{return pin_&&pin_->value.is_object()?reinterpret_cast<std::uintptr_t>(pin_->value.to_object()):0;}
SwfAsGraph::~SwfAsGraph(){release();}
bool SwfAsGraph::enter(std::shared_ptr<State>& s,std::string& e)const{
 s=state_;if(!s||!s->allowed()){e="Required retained AS graph Scope unavailable";return false;}e.clear();return true;
}
bool SwfAsGraph::value_ok(const std::shared_ptr<State>& s,const SwfAsValue& v,std::string& e)const{
 if(v.pin_&&(v.pin_->value.is_property()||(v.pin_->value.is_object()&&!v.pin_->value.is_null()))&&v.pin_->graph.get()!=s.get()){
  e="AS object belongs to a different retained movie";return false;
 }return true;
}
SwfAsValue SwfAsGraph::retain(const std::shared_ptr<State>& s,const void* p)const{
 SwfAsValue out;out.pin_=std::make_shared<SwfAsValue::Pin>(*static_cast<const gameswf::as_value*>(p),s);return out;
}
bool SwfAsGraph::bind(SwfAsLease lease,const SwfAsServices& services,std::string& e){
 if(!lease.owner||!lease.player||!services.within_scope||!services.owner||
    (!services.owner.owner_before(lease.owner)&&!lease.owner.owner_before(services.owner))||
    !services.within_scope(services.context,lease.owner.get())||
    (lease.root&&lease.root->m_player.get_ptr()!=lease.player)){
  e="Required owned AS graph/provider/Scope unavailable";return false;
 }
 auto next=std::make_shared<State>();next->lease=std::move(lease);next->services=services;
 release();state_=std::move(next);e.clear();return true;
}
bool SwfAsGraph::attach_root(gameswf::root* root,std::string& e){
 std::shared_ptr<State> s;if(!enter(s,e))return false;
 if(!root||root->m_player.get_ptr()!=s->lease.player||(s->lease.root&&s->lease.root!=root)){
  e="AS root belongs to a different retained player";return false;
 }s->lease.root=root;return true;
}
void SwfAsGraph::release()noexcept{if(state_)state_->enabled=false;state_.reset();}
bool SwfAsGraph::install_native(const std::vector<std::string>& names,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e))return false;
 std::set<std::string> batch;
 for(const auto& n:names)if(n.empty()||n.find('\0')!=std::string::npos||!batch.insert(n).second||s->registered.count(n)){
  e="Invalid or duplicate native AS function name";return false;
 }
 if(!names.empty()&&(!s->services.native_call||!s->services.failure)){e="Required native AS delivery/failure services unavailable";return false;}
 for(const auto& n:names){gameswf::gc_ptr<State::Native> fn=new State::Native(s,n);
  // Real global setter may run watchers and reject a read-only member.
  if(!s->lease.player->get_global()->set_member(n.c_str(),gameswf::as_value(fn.get_ptr()))){e="Native AS registration rejected: "+n;return false;}
  gameswf::as_value installed;
  if(!s->lease.player->get_global()->get_member(n.c_str(),&installed)||installed.to_object()!=fn.get_ptr()){
   e="Native AS registration did not retain requested function: "+n;return false;
  }
  s->registered.insert(n);
  if(!s->allowed()){e="AS graph released during native registration";return false;}
 }e.clear();return true;
}
bool SwfAsGraph::root_value(SwfAsValue& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e))return false;if(!s->lease.root){e="AS root not attached";return false;}
 const gameswf::as_value v(s->lease.root->get_root_movie());out=retain(s,&v);return true;
}
bool SwfAsGraph::global_value(SwfAsValue& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e))return false;const gameswf::as_value v(s->lease.player->get_global());out=retain(s,&v);return true;
}
bool SwfAsGraph::borrow_object(const SwfAsValue& v,gameswf::as_object*& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,v,e))return false;
 const auto pin=v.pin_;out=pin?pin->value.to_object():nullptr;return true;
}
bool SwfAsGraph::retain_object(gameswf::as_object* object,SwfAsValue& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e))return false;
 if(object&&object->get_player()!=s->lease.player){e="AS object belongs to a different retained player";return false;}
 const gameswf::as_value v(object);out=retain(s,&v);return true;
}
bool SwfAsGraph::find_target(const SwfAsValue& base,const char* path,SwfAsValue& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,base,e))return false;
 const auto receiver=base.pin_;auto* object=receiver?receiver->value.to_object():nullptr;
 if(!object||!path){e="Required AS target base/path unavailable";return false;}
 const gameswf::as_value v(object->find_target(path));out=retain(s,&v);return true;
}
bool SwfAsGraph::get_member(const SwfAsValue& base,const char* name,SwfAsValue& out,bool& found,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,base,e))return false;
 const auto receiver=base.pin_;auto* object=receiver?receiver->value.to_object():nullptr;
 if(!object||!name){e="Required AS member receiver/name unavailable";return false;}
 // Write into owned backing directly. Upstream as_value's copy constructor
 // invokes a bound property's getter; copying here would move that callback
 // ahead of the caller's actual source to_number/to_string operation.
 SwfAsValue next;next.pin_=std::make_shared<SwfAsValue::Pin>(gameswf::as_value(),s);
 found=object->get_member(name,&next.pin_->value);out=std::move(next);return true;
}
bool SwfAsGraph::set_member(const SwfAsValue& base,const char* name,const SwfAsValue& value,bool& accepted,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,base,e)||!value_ok(s,value,e))return false;
 const auto receiver=base.pin_;const auto incoming=value.pin_;auto* object=receiver?receiver->value.to_object():nullptr;
 if(!object||!name){e="Required AS setter receiver/name unavailable";return false;}
 const gameswf::as_value undefined;
 accepted=object->set_member(name,incoming?incoming->value:undefined);return true;
}
bool SwfAsGraph::to_number(const SwfAsValue& value,double& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,value,e))return false;
 const auto pin=value.pin_;const gameswf::as_value undefined;out=(pin?pin->value:undefined).to_number();return true;
}
bool SwfAsGraph::to_boolean(const SwfAsValue& value,bool& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,value,e))return false;
 const auto pin=value.pin_;const gameswf::as_value undefined;out=(pin?pin->value:undefined).to_bool();return true;
}
bool SwfAsGraph::to_text(const SwfAsValue& value,std::string& out,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,value,e))return false;
 const auto pin=value.pin_;const gameswf::as_value undefined;out=(pin?pin->value:undefined).to_string();return true;
}
bool SwfAsGraph::invoke(const SwfAsValue& environment_sprite,const SwfAsValue& receiver,const char* name,
 const std::vector<SwfAsValue>& arguments,SwfAsValue& out,bool& callable,std::string& e){
 std::shared_ptr<State>s;if(!enter(s,e)||!value_ok(s,environment_sprite,e)||!value_ok(s,receiver,e))return false;
 const auto environment_pin=environment_sprite.pin_;const auto receiver_pin=receiver.pin_;
 auto* source=environment_pin?environment_pin->value.to_object():nullptr;
 if(!source||!source->is(gameswf::sprite_instance::m_class_id)||!name||arguments.size()>std::size_t(std::numeric_limits<int>::max())){
  e="Required AS sprite environment/method unavailable";return false;
 }
 for(const auto& a:arguments)if(!value_ok(s,a,e))return false;
 auto* env=source->get_environment();if(!env){e="Required AS environment unavailable";return false;}
 struct Stack {gameswf::as_environment* env;int size;~Stack(){env->set_stack_size(size);}} stack{env,env->get_stack_size()};
 const gameswf::as_value undefined;
 for(auto i=arguments.rbegin();i!=arguments.rend();++i)env->push(i->pin_?i->pin_->value:undefined);
 const ::array<gameswf::with_stack_entry> with;
 const auto method=env->get_variable(name,with);callable=method.to_function()!=nullptr;
 const auto result=gameswf::call_method(method,env,receiver_pin?receiver_pin->value:undefined,
                                      int(arguments.size()),env->get_top_index());
 out=retain(s,&result);return true;
}
}
