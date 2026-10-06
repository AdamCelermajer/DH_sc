#include "swf_menu_parsed_string_v1.hpp"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <cmath>
#include <utility>
namespace dh2::ui {
bool swf_menu_parsed_string_v1(const gameswf::fn_call& fn,void* context,ParsedMenuStringProviderV1 provider,std::string& error){
    if(fn.nargs!=2||(!fn.arg(0).is_string()&&!fn.arg(0).is_object())||!fn.arg(1).is_object())return true;
    if(!fn.result||!provider){error="Parsed menu string provider/result unavailable";return false;}
    auto* array=gameswf::cast_to<gameswf::as_array>(fn.arg(1).to_object());
    if(!array){error="Parsed menu string requires actual AS Array";return false;}
    if(array->size()>65536){error="Parsed menu array outside bounds";return false;}
    std::vector<LocalizationArgumentV1> args;
    for(int i=0;i<array->size();++i){
        const auto& value=array->m_array[i];LocalizationArgumentV1 arg;
        if(value.is_string()||value.is_object()){arg.has_text=true;arg.text=value.to_xstring();}
        else if(value.is_number()){const double number=value.to_number();if(!std::isnan(number))arg.number=static_cast<float>(number);}
        args.push_back(std::move(arg));
    }
    std::string text;
    if(!provider(context,fn.arg(0).to_xstring(),args,text,error))return false;
    fn.result->set_string(text.c_str());return true;
}
}
