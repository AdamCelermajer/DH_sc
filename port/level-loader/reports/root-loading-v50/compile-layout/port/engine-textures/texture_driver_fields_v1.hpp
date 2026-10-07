#pragma once
#include <cstdint>
#include <functional>
#include <string>
namespace dh2::textures {
// Sole logical IVideoDriver+88 authority. Constructor 5aaff8/5aaffc;
// setOption whole body 5a90b0. GPU capabilities9c/extensions7ec are separate.
class TextureDriverOptionsOwnerV1 {
 std::uint32_t flags88_{0x10};
public:
 const std::uint32_t& flags88()const noexcept{return flags88_;}
 bool set_option(std::uint32_t mask,bool enabled,
  const std::function<bool(std::string&)>& virtual1fc,std::string& error){
  if(enabled){flags88_|=mask;return true;}
  flags88_&=~mask;
  if(!(mask&0x100))return true;
  if(!virtual1fc){error="Required actual IVideoDriver virtual1fc after option100 disable";return false;}
  return virtual1fc(error);
 }
};
struct ShaderConfigFileServicesV1 {
 // Actual retained file provider; found=false is the original retrying miss.
 std::function<bool(const char*,bool&,std::string&,std::string&)> read;
 std::function<bool(const char*,std::string&)> missing_diagnostic;
 const bool* trace_missing{};
};
class ShaderAdditionalConfigOwnerV1 {
 // Source ctor6e0ca4 stores config7c=NULL and length80=-1.
 std::int32_t length80_{-1};std::string text_;
public:
 bool initialize(const ShaderConfigFileServicesV1&,std::string&);
 const std::string* config()const noexcept{return length80_<0?nullptr:&text_;}
 std::int32_t length80()const noexcept{return length80_;}
};
}
