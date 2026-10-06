#include "gameplay_camera_design_v5.hpp"
#include <cstring>
namespace dh2::camera {
namespace {
bool word(const data::DesignSettingsOwner::Borrow& b,unsigned offset,float& value,std::string& e){if(!b||b.rows().empty()||offset%4||offset/4>=44){e="Required actual DesignSettings first camera row";return false;}const auto raw=b.rows()[0].words[offset/4];std::memcpy(&value,&raw,4);return true;}
}
bool source_zoom_bounds_v5(const data::DesignSettingsOwner::Borrow& b,bool mode,float& lower,float& upper,std::string& e){float l,u;if(!word(b,mode?0x4c:0xac,l,e)||!word(b,mode?0x48:0xa8,u,e))return false;lower=l;upper=u;return true;}
bool source_autozoom_design_v5(const data::DesignSettingsOwner::Borrow& b,AutoZoomDesignV5& out,std::string& e){AutoZoomDesignV5 d;if(!word(b,0xc,d.reference,e)||!word(b,0x10,d.step,e)||!word(b,0x18,d.bottom,e)||!word(b,0x1c,d.sides,e)||!word(b,0x20,d.top,e))return false;out=d;return true;}
}
