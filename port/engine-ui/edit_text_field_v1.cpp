#include "edit_text_field_v1.hpp"

namespace dh2::ui::edit_text_v1 {
namespace {
bool required(bool present,const char* name,std::string& error){
    if(present)return true;
    error=std::string("source edit text requires ")+name;return false;
}
bool valid(State& s,std::string& e){return required(bool(s.definition),"retained definition",e);}
}
bool format(State& s,bool html,const Services& v,std::string& e){
    if(!valid(s,e))return false;
    // The original rereads the shared definition's multiline byte in format.
    s.layout.multiline=s.definition->multiline;
    s.layout.rect_min=s.definition->rectangle[0];
    s.layout.rect_max=s.definition->rectangle[1];
    return text_v1::format_text(s.layout,html,v.layout,e);
}
bool set_text(State& s,const std::string& input,bool html,const Services& v,std::string& e){
    if(!valid(s,e))return false;
    if(s.layout.text==input){e.clear();return true;}
    // Freeze only the supplied value. The definition is read after assignment,
    // in the same order as set_text78f1ec.
    s.layout.text=input;
    const auto maximum=s.definition->maximum;
    if(maximum>0 && s.layout.text.size()>std::size_t(maximum))
        s.layout.text.resize(std::size_t(maximum));
    return format(s,html,v,e);
}
bool set_text_value(State& s,const std::string& input,bool html,const Services& v,std::string& e){
    // A caller can supply s.layout.text itself; source tu_string retains the
    // original argument while the field assignment/resize/format executes.
    const auto supplied=input;
    if(!set_text(s,supplied,html,v,e))return false;
    if(!s.definition->variable.empty()){
        const auto variable=s.definition->variable;
        if(!required(bool(v.write_bound),"current bound-variable setter",e))return false;
        if(!v.write_bound(variable,supplied,e))return false;
    }
    e.clear();return true;
}
bool refresh_bound(State& s,const Services& v,std::string& e){
    if(!valid(s,e))return false;
    if(!s.definition->variable.empty()){
        const auto variable=s.definition->variable;BoundRead value;
        if(!required(bool(v.read_bound),"current bound-variable getter",e))return false;
        if(!v.read_bound(variable,value,e))return false;
        if(value.found && !value.self){
            if(!required(bool(value.to_text),"retained bound-value conversion",e))return false;
            std::string compared;if(!value.to_text(compared,e))return false;
            if(compared!=s.layout.text){
                std::string assigned;if(!value.to_text(assigned,e))return false;
                return set_text(s,assigned,false,v,e);
            }
        }
    }
    e.clear();return true;
}
bool initialize(State& s,const Services& v,std::string& e){
    if(!valid(s,e))return false;
    auto& q=s.layout;
    q.word_glyph=-1;q.word_record=q.cursor=q.first_line=0;
    s.focus=false;q.xcursor=q.ycursor=q.x=q.y=0;
    q.rgba=s.definition->rgba;q.text_height=s.definition->text_height;
    q.font=s.definition->font;q.alignment=s.definition->alignment;
    q.left=s.definition->left;q.right=s.definition->right;
    q.indent=s.definition->indent;q.leading=s.definition->leading;
    q.letter_spacing=0;s.background=0xffffffff;
    const auto initial=s.definition->default_text;
    if(!set_text(s,initial,false,v,e))return false;
    // Virtual to_string reads the optional binding and can reenter setters.
    if(!refresh_bound(s,v,e))return false;
    const auto refreshed=s.layout.text;
    if(!set_text_value(s,refreshed,false,v,e))return false;
    if(!required(bool(v.initialize_fill),"actual dummy fill storage",e))return false;
    if(!v.initialize_fill(e))return false;
    s.needs_update=!s.definition->variable.empty();
    e.clear();return true;
}
}
