#include "edit_text_format_v1.hpp"
#include <cstring>
namespace dh2::ui::edit_text_v1 {
bool get_format(State& s,const FormatWriter& w,std::string& e){
    if(!s.definition||!w.construct||!w.write||!w.intern){e="source getTextFormat requires receiver, constructor, setter and string pool";return false;}
    if(!w.construct(e))return false;
    auto number=[&](const char* name,float v){return w.write(name,double(v/20.0f),e);};
    if(!number("leftMargin",s.layout.left)||!number("indent",s.layout.indent)||
       !number("rightMargin",s.layout.right)||!number("leading",s.layout.leading)||
       !number("letterSpacing",s.layout.letter_spacing))return false;
    const auto c=s.layout.rgba;
    const auto argb=((c&255)<<16)|(c&0xff00)|((c>>16)&255)|(c&0xff000000);
    // Source 792100..792120 preserves alpha then signed i2d, so ordinary
    // opaque colors become negative AS numbers; do not strip alpha as stock.
    std::int32_t signed_color;std::memcpy(&signed_color,&argb,4);
    if(!w.write("color",double(signed_color),e)||!number("size",s.layout.text_height))return false;
    const char* align=nullptr;
    switch(s.layout.alignment){case 0:align="left";break;case 1:align="right";break;case 2:align="center";break;case 3:align="justify";break;}
    if(align){std::string value;if(!w.intern(align,value,e)||!w.write("align",value,e))return false;}
    if(!s.layout.font){e="source getTextFormat reached null font";return false;}
    // Original font strings are copied through the current player's pool.
    std::string name;if(!w.intern(s.layout.font->name,name,e)||!w.write("font",name,e))return false;
    if(!s.layout.font){e="source getTextFormat font removed during setter";return false;}
    if(!w.write("bold",s.layout.font->bold,e))return false;
    if(!s.layout.font){e="source getTextFormat font removed during setter";return false;}
    if(!w.write("italic",s.layout.font->italic,e))return false;
    e.clear();return true;
}
}
