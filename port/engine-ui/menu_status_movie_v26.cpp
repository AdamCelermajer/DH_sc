#include "menu_status_movie_v26.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_function.h"
#include <cmath>
#include <cstring>
namespace dh2::ui {namespace {
struct Invocation {const char* method;std::int32_t context;};
bool invoke(void* p,SwfAsGraph& graph,std::string& error){
 const auto& call=*static_cast<Invocation*>(p);SwfAsValue root,result;bool callable{};
 if(!graph.root_value(root,error)||!graph.invoke(root,root,call.method,
    {SwfAsValue::number(call.context)},result,callable,error))return false;
 if(!callable){error="Required original status HUD ActionScript method";return false;}
 return true;
}
}
bool menu_status_invoke_movie_v26(SwfMovie& movie,const char* method,std::int32_t context,std::string& error){
 if(!method){error="Required actual status method symbol";return false;}
 Invocation call{method,context};return movie.menu_action_script(&call,invoke,error);
}
bool menu_status_native_v26(MenuStatusMessagesV26& owner,const char* name,const gameswf::fn_call& fn,
 bool& handled,std::string& error){
 handled=false;error.clear();if(!name)return true;
 if(std::strcmp(name,"NativeGetNextStatusMessage")==0){
  handled=true;
  // Original43fef4 accepted arity1/2, first exactNumber, second Bool or
  // Undefined. Native invalid arguments leave the original result untouched.
  if(fn.nargs<1||fn.nargs>2||!fn.arg(0).is_number())return true;
  const double value=fn.arg(0).to_number();if(std::isnan(value))return true;
  if(fn.nargs==2&&!fn.arg(1).is_bool()&&!fn.arg(1).is_undefined())return true;
  if(!std::isfinite(value)||value<0||value>=4){error="Status AS context outside actual source queue storage";return false;}
  const auto context=std::int32_t(value);std::string text;bool found{};
  if(!owner.next(context,text,found,error))return false;
  if(found&&fn.result)fn.result->set_string(text.c_str());
  return true;
 }
 if(std::strcmp(name,"NativeStopMessage")==0){
  if(fn.nargs<1||!fn.arg(0).is_string()){handled=true;return true;}
  if(std::strcmp(fn.arg(0).to_string(),"status")!=0)return true;
  handled=true;return owner.stop_status(error);
 }
 return true;
}
}
