#include "swf_menu_options.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_object.h"
#include <cstring>
#include <cmath>
#include <limits>
namespace dh2::ui {
bool swf_menu_option_parameters(const gameswf::fn_call& fn,const SwfMenuOptionServicesV1& services,std::string& error){
    // The source reads two arguments without an arity guard. Reject a
    // malformed host call explicitly instead of reading outside its stack.
    if(fn.nargs<2||!fn.result||!fn.env){error="Malformed option-parameters AS call";return false;}
    if(!services.settings){error="Required option settings owner unavailable";return false;}
    const std::string name=fn.arg(0).to_string();
    // Original accepts only the OBJECT tag. to_object alone would also
    // evaluate a PROPERTY getter in this core, which the source never does.
    gameswf::gc_ptr<gameswf::as_object> object=fn.arg(1).is_object()?fn.arg(1).to_object():nullptr;
    const auto current=services.settings->option(name.c_str());
    auto maximum=services.settings->option_max(name.c_str());
    const auto string_id=services.settings->option_string(name.c_str());
    std::string text;
    if(std::uint32_t(string_id)-std::uint32_t(current)!=0xffffffffu){
        if(!services.string_by_id){error="Required option StringManager backend unavailable";return false;}
        if(!services.string_by_id(services.context,string_id,text,error))return false;
    }
    const bool language=name=="Language";
    if(object){
        if(services.sharp_device&&language)maximum=5;
        object->set_member("NumOptions",gameswf::as_value(maximum));
        object->set_member("CurrentOption",gameswf::as_value(current));
        object->set_member("OptionString",gameswf::as_value(text.c_str()));
        fn.result->set_as_object(object.get_ptr());
    }
    if(services.sharp_device&&language){
        if(!services.observe_sharp_language){error="Required Sharp language observation owner unavailable";return false;}
        if(!services.observe_sharp_language(services.context,current,error))return false;
    }
    error.clear();return true;
}
bool swf_menu_settings_action(const char* action,const gameswf::fn_call& fn,const SwfMenuOptionServicesV1& s,std::string& error){
    error.clear();if(!action||!fn.result||!s.settings){error="Settings AS owner unavailable";return false;}
    if(!std::strcmp(action,"NativeGetOptionParameters"))return swf_menu_option_parameters(fn,s,error);
    if(!std::strcmp(action,"NativeSetOptions")){
        if(fn.nargs<3||!s.apply_option){error="Settings mutation arguments/backend missing";return false;}
        const std::string name=fn.arg(0).to_string();
        gameswf::gc_ptr<gameswf::as_object> object=fn.arg(1).is_object()?fn.arg(1).to_object():nullptr;
        const double n=fn.arg(2).to_number();
        // ARM __aeabi_d2iz saturation/NaN conversion, rather than undefined
        // C++ conversion of out-of-range doubles.
        const auto value=std::isnan(n)?0:n>=2147483647.0?INT32_MAX:n<=-2147483648.0?INT32_MIN:static_cast<std::int32_t>(n);
        if(!s.apply_option(s.context,name.c_str(),value,error))return false;
        const auto current=s.settings->option(name.c_str()),id=s.settings->option_string(name.c_str());std::string text;
        if(std::uint32_t(id)-std::uint32_t(current)!=0xffffffffu){
            if(!s.string_by_id){error="Required option StringManager backend unavailable";return false;}
            if(!s.string_by_id(s.context,id,text,error))return false;
        }
        if(object){object->set_member("CurrentOption",gameswf::as_value(current));object->set_member("OptionString",gameswf::as_value(text.c_str()));fn.result->set_as_object(object.get_ptr());}
        return true;
    }
    if(!std::strcmp(action,"NativeIsJapaneseVersion")){fn.result->set_bool(s.japanese_build);return true;}
    if(!std::strcmp(action,"NativeIsKorean")){fn.result->set_bool(s.settings->language()==5);return true;}
    if(!std::strcmp(action,"NativeChangeRolloverInputBehavior")){
        if(fn.nargs<2||!s.input_behavior){error="Required menu input behavior owner unavailable";return false;}
        const double n=fn.arg(0).to_number();const std::int32_t slot=std::isfinite(n)&&n>=0&&n<=3?static_cast<std::int32_t>(n):-1;
        if(slot<0){error="Menu renderer slot outside connected owner range";return false;}
        return s.input_behavior(s.context,slot,fn.arg(1).to_bool(),error);
    }
    bool (*service)(void*,std::string&)=!std::strcmp(action,"NativeLoadSettings")?s.load:!std::strcmp(action,"NativeSaveSettings")?s.save:!std::strcmp(action,"NativeEnterOptionMenu")?s.enter:!std::strcmp(action,"NativeRefreshHudManager")?s.refresh_hud:nullptr;
    if(!service){error="Required menu settings action unavailable";return false;}
    if(!service(s.context,error))return false;
    return true;
}
}
