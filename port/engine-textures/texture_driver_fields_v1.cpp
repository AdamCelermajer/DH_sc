#include "texture_driver_fields_v1.hpp"
#include <limits>
namespace dh2::textures {
bool ShaderAdditionalConfigOwnerV1::initialize(const ShaderConfigFileServicesV1& s,std::string& e){
 if(length80_!=-1)return true;
 if(!s.read){e="Required actual shader file provider for glsl.config";return false;}
 bool found=false;std::string bytes;
 if(!s.read("glsl.config",found,bytes,e))return false;
 if(!found){
  if(!s.trace_missing){e="Required actual shader missing-file diagnostic flag";return false;}
  if(*s.trace_missing){if(!s.missing_diagnostic){e="Required source shader missing-file diagnostic";return false;}if(!s.missing_diagnostic("glsl.config",e))return false;}
  return true;
 }
 if(bytes.size()>std::size_t(std::numeric_limits<std::int32_t>::max())||bytes.find('\0')!=std::string::npos){e="Unsupported shader config length/embedded NUL";return false;}
 // Source stores length before allocation/read; this callback returns the
 // complete actual file read. C++ allocation failure remains an exception.
 length80_=std::int32_t(bytes.size());text_=std::move(bytes);
 for(char& c:text_)if(c=='^')c='\n';
 return true;
}
}
