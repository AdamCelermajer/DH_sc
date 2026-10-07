#pragma once
#include "../asset-payloads/payloads.hpp"
#include <string>
namespace dh2::fx {
//Same immutable authored DB segment domain as Animation::Player. Clip ranges
//remain separate from segments; the actual controller selects the clip.
inline bool source_fx_segment_v87(const resources::BresView& image,std::int32_t ms,std::uint32_t& out,std::string& e){
 const auto count=dh2_animation_segments(&image);
 if(!count||count>256){e="Invalid authored FX segment domain";return false;}
 out=0;
 for(std::uint32_t i=0;i+1<count;++i){assets::Animation range{};
  if(dh2_animation_open(&range,&image,0,i)!=assets::Error::ok){e="Invalid authored FX segment range";return false;}
  if(ms<range.segment_end)break;out=i+1;
 }
 return true;
}
}
