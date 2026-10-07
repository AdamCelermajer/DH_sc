#pragma once
#include "localization.hpp"
namespace dh2::ui {
// Original VarArgs keeps both a float32 numeric projection and nullable text.
struct LocalizationArgumentV1 {float number{};bool has_text=false;std::string text;};
struct LocalizationNumberStyleV1 {std::string decimal,thousands;std::int32_t group_at{};};
struct LocalizationParseExServicesV1 {
    void* context{};
    bool (*title)(void*,std::string&,std::string&){};
    bool (*version)(void*,std::string&,std::string&){};
};
bool localization_parse_ex_v1(const std::string&,const std::vector<LocalizationArgumentV1>&,
    const LocalizationNumberStyleV1&,bool add_space,const LocalizationParseExServicesV1&,
    std::string&,bool& changed,std::string&);
}
