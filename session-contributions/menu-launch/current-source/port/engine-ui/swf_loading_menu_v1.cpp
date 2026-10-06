#include "swf_loading_menu_v1.hpp"
#include "gameswf/gameswf_function.h"
#include <cstring>
namespace dh2::ui {
bool swf_loading_hint_v1(const gameswf::fn_call& fn,const LoadingHintTableV1& table,LoadingHintRandomV1& rng,std::uint32_t& id,std::string& error){
 if(!fn.result){error="Loading hint requires its actual AS result";return false;}
 if(!table.next(rng,id,error))return false;
 std::int32_t signed_id;std::memcpy(&signed_id,&id,4);fn.result->set_double(double(signed_id));return true;
}
bool swf_loading_string_id_v1(const gameswf::fn_call& fn,void* context,LoadingStringIdV1 localize,std::string& error){
 if(fn.nargs!=1||!fn.result||!localize){error="String ID callback requires one argument/result/provider";return false;}
 const auto id=static_cast<std::uint32_t>(fn.arg(0).to_int());std::string text;
 if(!localize(context,id,text,error))return false;
 fn.result->set_string(text.c_str());return true;
}
bool swf_loading_progress_v1(const gameswf::fn_call& fn,const LoadingMenuStateServicesV1& services,std::string& error){
 if(!fn.result){error="Loading progress requires its actual AS result";return false;}
 std::int32_t progress{};if(!loading_menu_read_progress_v1(services,progress,error))return false;
 fn.result->set_double(double(progress));return true;
}
bool swf_loading_end_v1(const gameswf::fn_call&,const LoadingMenuStateServicesV1& services,bool& advanced,std::string& error){
 // Original 43ab00 ignores fn_call and does not set an AS return value.
 return loading_menu_finish_v1(services,advanced,error);
}

namespace {
using LoadingBoolQuery=bool(*)(const LoadingMenuMultiplayerServicesV1&,bool&,std::string&);
bool loading_bool_result(const gameswf::fn_call& fn,const LoadingMenuMultiplayerServicesV1& s,LoadingBoolQuery query,std::string& error){
 if(!fn.result){error="Loading multiplayer callback requires its actual AS result";return false;}
 bool value{};if(!query(s,value,error))return false;
 fn.result->set_bool(value);return true;
}
}
bool swf_loading_multiplayer_completed_v1(const gameswf::fn_call& fn,const LoadingMenuMultiplayerServicesV1& s,std::string& error){return loading_bool_result(fn,s,loading_menu_multiplayer_completed_v1,error);}
bool swf_loading_multiplayer_host_v1(const gameswf::fn_call& fn,const LoadingMenuMultiplayerServicesV1& s,std::string& error){return loading_bool_result(fn,s,loading_menu_multiplayer_host_v1,error);}
bool swf_loading_wait_for_host_v1(const gameswf::fn_call& fn,const LoadingMenuMultiplayerServicesV1& s,std::string& error){return loading_bool_result(fn,s,loading_menu_wait_for_host_v1,error);}

bool swf_loading_back_to_hud_v1(const gameswf::fn_call&,const LoadingMenuHudServicesV1& s,std::string& error){
 // Original444990 ignores fn_call and leaves the AS result unchanged.
 return loading_menu_back_to_hud_v1(s,error);
}
bool swf_loading_refresh_hud_v1(const gameswf::fn_call&,const LoadingMenuHudServicesV1& s,std::string& error){
 // Original43a810 ignores fn_call and leaves the AS result unchanged.
 return loading_menu_refresh_hud_v1(s,error);
}
}


