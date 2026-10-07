#include "gameplay_camera_device_v9.hpp"
#include <cstring>
namespace dh2::camera {
bool source_android_manufacturer_v9(const char* actual,std::int32_t& code,std::string& e){
 if(!actual){e="Required actual JNI Android Build.MANUFACTURER";return false;}
 // DEX String.equals is case-sensitive; unmatched source default is1.
 code=!std::strcmp(actual,"HTC")?0:!std::strcmp(actual,"SHARP")?2:!std::strcmp(actual,"motorola")?3:!std::strcmp(actual,"Sony Ericsson")?4:!std::strcmp(actual,"LGE")?5:1;return true;
}
bool source_lg_devices_v9(const char* actual,std::uint8_t& lg,std::string& e){std::int32_t code;if(!source_android_manufacturer_v9(actual,code,e))return false;lg=static_cast<std::uint8_t>(code==5);return true;}
}
