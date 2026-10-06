#include "authored_character_menu_session_v1.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
namespace dh2::ui {
namespace {
bool character(SwfAsGraph& graph,const SwfAsValue& value,gameswf::character*& out,std::string& error){
 gameswf::as_object* object=nullptr;if(!graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::character::m_class_id)){error="Required actual registered MenuBase character unavailable";return false;}
 out=static_cast<gameswf::character*>(object);return true;
}
}
bool authored_character_register_state_v1(SwfAsGraph& graph,AuthoredCharacterStateV1& state,const char* name,const AuthoredCharacterRegistrationServicesV1& services,std::string& error){
 if(!state.fields||!services.owner||!services.render||!services.append_state||!services.find||!services.bind_weak_context||!services.create){error="Required original MenuFX registration provider unavailable";return false;}
 auto& fields=*state.fields;fields.render=services.render;
 if(!services.append_state(fields,error))return false;
 SwfAsValue value;bool found=false;if(!services.find(name?name:fields.name.c_str(),value,found,error))return false;
 if(!found){error="Original MenuFX::RegisterState required character not found: "+fields.name;return false;}
 if(!services.bind_weak_context(fields,value,error))return false;
 state.context=value;gameswf::character* live=nullptr;if(!character(graph,value,live,error))return false;
 live->set_visible(false);if(!services.create(fields,error))return false;
 fields.valid7c=1;return true;
}
bool authored_character_movie_operation_v1(SwfAsGraph& graph,AuthoredCharacterStateV1& state,const AuthoredMenuRequestV1& request,const AuthoredMenuLocalizationServicesV1& localization,std::string& error){
 if(!state.fields){error="Required retained MenuBase fields unavailable";return false;}
 if(!state.resolve_context){error="Required original MenuBase weak-context resolver unavailable";return false;}
 if(!state.resolve_context(state.context,error))return false;
 if(request.operation==AuthoredMenuOperationV1::localize)return authored_menu_process_localization_v1(graph,*state.fields,state.context,localization,error);
 gameswf::character* live=nullptr;if(!character(graph,state.context,live,error))return false;
 if(request.operation==AuthoredMenuOperationV1::set_visible){live->set_visible(request.value!=0);return true;}
 if(request.operation==AuthoredMenuOperationV1::invoke_as){
  auto* environment=live->is(gameswf::sprite_instance::m_class_id)?live:live->get_parent();
  if(!environment){error="Original RenderFX invoke requires actual sprite environment";return false;}
  SwfAsValue env,result;if(!graph.retain_object(environment,env,error))return false;
  bool callable=false;return graph.invoke(env,state.context,request.text,{},result,callable,error);
 }
 error="Operation requires its original application/stack service";return false;
}
}
