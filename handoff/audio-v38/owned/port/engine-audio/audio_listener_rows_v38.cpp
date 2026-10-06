#include "audio_listener_rows_v38.hpp"
#include <cstring>
#include <cmath>
#include <stdexcept>
namespace dh2::audio {
bool audio_listener_rows_v38(const std::uint8_t*bytes,std::size_t size,std::vector<AudioListenerRowV38>&out,std::string&error){try{
 std::size_t at=0;auto word=[&](){if(!bytes||at>size||size-at<4)throw std::runtime_error("Original Listener word extent");std::uint32_t v;std::memcpy(&v,bytes+at,4);at+=4;return v;};const auto chars=word();if(chars>65536)throw std::runtime_error("Original CharSounds count");for(unsigned i=0;i<chars;++i){for(unsigned j=0;j<4;++j){const auto n=word();if(n>(size-at)/4)throw std::runtime_error("Original CharSounds list extent");at+=4*std::size_t(n);}if(size-at<2)throw std::runtime_error("Original CharSounds flags extent");at+=2;}
 const auto count=word();if(!count||count>65536||count>(size-at)/24)throw std::runtime_error("Original Listener rows extent");std::vector<AudioListenerRowV38>rows(count);static_assert(sizeof(AudioListenerRowV38)==24);for(auto&row:rows){std::memcpy(&row,bytes+at,24);at+=24;if(!std::isfinite(row.rolloff))throw std::runtime_error("Original Listener rolloff");}out=std::move(rows);error.clear();return true;
}catch(const std::exception&e){error=e.what();return false;}}
bool audio_listener_update_v38(const AudioListenerRowV38&row,const float*position,const float*front,const float*up,VoxListenerAuthorityV38&authority,std::string&error){
 if(!position||!front||!up||!std::isfinite(row.rolloff)){error="Required same Level listener authorities";return false;}for(unsigned i=0;i<3;++i)if(!std::isfinite(position[i])||!std::isfinite(front[i])||!std::isfinite(up[i])){error="Required finite actual listener vectors";return false;}
 for(unsigned i=0;i<3;++i){authority.listener.position[i]=position[i];authority.listener.front[i]=front[i];authority.listener.up[i]=up[i];}authority.reference_distance=row.reference_distance;authority.maximum_distance=row.maximum_distance;authority.rolloff=row.rolloff;error.clear();return true;
}
}
