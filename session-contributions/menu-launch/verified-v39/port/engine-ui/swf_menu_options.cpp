#include "swf_menu_options.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_object.h"
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
}
