#include "authored_menu_movie_stack_v4.hpp"
#include "authored_menu_character_projection_v4.hpp"
#include <utility>
namespace dh2::ui {
AuthoredMenuMovieStackV4::AuthoredMenuMovieStackV4(AuthoredMenuMovieStackServicesV4 s):services_(std::move(s)){}
bool AuthoredMenuMovieStackV4::movie(MenuStackRequestV1& request,AuthoredMenuFieldsV1& fields,std::string& error){
 if(!request.render||!services_.scoped){error="Required actual HUD/base RenderFX movie Scope";return false;}
 struct Call {MenuStackRequestV1& request;AuthoredMenuFieldsV1& fields;
  static bool run(void* raw,SwfAsGraph& graph,std::string& error){auto& c=*static_cast<Call*>(raw);SwfAsValue root,receiver,result;
   if(!graph.root_value(root,error)||!graph.find_target(root,("_root."+c.fields.name).c_str(),receiver,error)||receiver.kind()!=SwfAsValue::Kind::object){if(error.empty())error="Required retained HUD/base MenuBase movie character";return false;}
   if(c.request.operation==MenuStackOperationV1::menu_set_visible){bool accepted=false;
    if(!graph.set_member(receiver,"_visible",SwfAsValue::boolean(c.request.value!=0),accepted,error))return false;
    if(!accepted){error="Actual MenuBase visibility write rejected";return false;}c.fields.visible74=static_cast<std::uint8_t>(c.request.value!=0);return true;}
   if(!c.request.text){error="Required source MenuBase movie method/label";return false;}
   if(c.request.operation==MenuStackOperationV1::play_animation){bool accepted=false;
    if(!authored_menu_play_animation_v4(graph,receiver,c.request.text,accepted,error))return false;
    c.request.result=accepted;return true;
   }
   bool callable=false;if(!graph.invoke(receiver,receiver,c.request.text,{},result,callable,error))return false;
   return true;
  }} call{request,fields};
 return services_.scoped(request.render->identity,&call,Call::run,error);
}
bool AuthoredMenuMovieStackV4::invoke(MenuStackV1& stack,MenuStackRequestV1& request,std::string& error){
 if(!services_.owner){error="Required same native HUD/base movie owner";return false;}
 auto* fields=request.menu&&services_.fields?services_.fields(request.menu->identity):nullptr;
 if(fields){switch(request.operation){
  // Only registered source MenuBase receivers use literal empty virtuals.
  case MenuStackOperationV1::menu_focus:case MenuStackOperationV1::menu_blur:return true;
  case MenuStackOperationV1::menu_valid:request.result=fields->valid7c;return true;
  case MenuStackOperationV1::menu_show:return authored_menu_show_v1(*fields,services_.lifecycle,error);
  case MenuStackOperationV1::menu_hide:return authored_menu_hide_v1(*fields,services_.lifecycle,error);
  case MenuStackOperationV1::invoke_as:case MenuStackOperationV1::menu_set_visible:case MenuStackOperationV1::play_animation:return movie(request,*fields,error);
  default:break;
 }}
 if(services_.required)return services_.required(stack,request,error);
 error="Required source external MenuManager continuation";return false;
}
}
