#pragma once
#include "loading_menu_v1.hpp"
namespace gameswf {struct fn_call;}
namespace dh2::ui {
bool swf_loading_hint_v1(const gameswf::fn_call&,const LoadingHintTableV1&,LoadingHintRandomV1&,std::uint32_t& id,std::string&);
using LoadingStringIdV1=bool(*)(void*,std::uint32_t,std::string&,std::string&);
bool swf_loading_string_id_v1(const gameswf::fn_call&,void*,LoadingStringIdV1,std::string&);
bool swf_loading_progress_v1(const gameswf::fn_call&,const LoadingMenuStateServicesV1&,std::string&);
bool swf_loading_end_v1(const gameswf::fn_call&,const LoadingMenuStateServicesV1&,bool& advanced,std::string&);

bool swf_loading_multiplayer_completed_v1(const gameswf::fn_call&,const LoadingMenuMultiplayerServicesV1&,std::string&);
bool swf_loading_multiplayer_host_v1(const gameswf::fn_call&,const LoadingMenuMultiplayerServicesV1&,std::string&);
bool swf_loading_wait_for_host_v1(const gameswf::fn_call&,const LoadingMenuMultiplayerServicesV1&,std::string&);
}

