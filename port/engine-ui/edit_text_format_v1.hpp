#pragma once
#include "edit_text_field_v1.hpp"
#include <variant>
namespace dh2::ui::edit_text_v1 {
using FormatValue=std::variant<double,bool,std::string>;
struct FormatWriter {
    // The original getTextFormat invokes the actual TextFormat constructor on
    // the ORIGINAL fn_call, including its arguments. Writer rereads fn.result
    // and its virtual setter before each property; reentry can replace it.
    std::function<bool(std::string&)> construct;
    std::function<bool(const char*,const FormatValue&,std::string&)> write;
    std::function<bool(const std::string&,std::string&,std::string&)> intern;
};
bool get_format(State&,const FormatWriter&,std::string&);
}
