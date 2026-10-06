#pragma once
#include "localization_parse_ex_v1.hpp"
namespace gameswf {struct fn_call;}
namespace dh2::ui {
using ParsedMenuStringProviderV1=bool (*)(void*,const std::string&,
    const std::vector<LocalizationArgumentV1>&,std::string&,std::string&);
bool swf_menu_parsed_string_v1(const gameswf::fn_call&,void*,ParsedMenuStringProviderV1,std::string&);
}
