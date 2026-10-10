#include "animation_markers.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace dh::foundation {
namespace {
constexpr float frame_ms=33.333332061767578125f;
std::int32_t raw_key(const dh2::animation::EventView& v,unsigned i){
    const auto*p=v.times+i*(v.type==1?1:v.type==3?2:4);
    std::uint32_t n=p[0];if(v.type!=1)n|=std::uint32_t(p[1])<<8;
    if(v.type==4){n|=std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;std::int32_t s;std::memcpy(&s,&n,4);return s;}return static_cast<std::int32_t>(n);
}
std::int32_t threshold(std::int32_t key,unsigned type){
    // Match original float query comparison, not the truncated lookup time.
    std::int64_t low=std::numeric_limits<std::int32_t>::min(),high=std::numeric_limits<std::int32_t>::max();
    while(low<high){const auto mid=low+(high-low)/2;volatile float query=static_cast<float>(mid);if(type!=4)query=query/frame_ms;if(query>=static_cast<float>(key))high=mid;else low=mid+1;}return static_cast<std::int32_t>(low);
}
std::int32_t authored(std::int32_t key,unsigned type){volatile float stamp=static_cast<float>(key);if(type!=4)stamp=stamp*frame_ms;if(stamp>=2147483648.f)return INT32_MAX;if(stamp<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(stamp);}
}
bool AnimationMarkers::load(const dh2::animation::EventView& view,std::int32_t start,std::int32_t end,std::string& error){
    error.clear();if(end<=start||!dh2_events_validate(&view)){error="Invalid animation marker track or range";return false;}
    std::vector<AnimationMarker> next;
    for(unsigned i=0;i<view.count;++i){const auto key=raw_key(view,i);const auto stamp=threshold(key,view.type);for(unsigned j=0;j<view.groups[i].count;++j){const auto*name=view.groups[i].names[j];const auto length=std::strlen(name);if(length>4096){error="Animation marker name exceeds limit";return false;}next.push_back({std::string(name,length),stamp,authored(key,view.type),i,j,static_cast<unsigned>(next.size())});}}
    markers_=std::move(next);start_=start;end_=end;loaded_=true;return true;
}
bool AnimationMarkers::load(const std::uint8_t* bytes,std::size_t size,std::int32_t start,std::int32_t end,std::string& error){
    dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes,size)!=dh2::resources::BresError::ok){error="Animation marker BRES rejected";return false;}dh2::animation::EventTrack track;if(!track.load(view,error))return false;return load(track.view(),start,end,error);
}
bool AnimationMarkers::restart(MarkerCursor& cursor,std::string& error){if(cursor.generation==UINT64_MAX){error="Animation marker generation overflow";return false;}cursor={0,cursor.generation+1,false};error.clear();return true;}
bool AnimationMarkers::advance(MarkerCursor& cursor,std::uint64_t delta,bool loop,std::vector<MarkerOccurrence>& output,std::string& error)const{
    error.clear();if(!loaded_){error="Animation markers not loaded";return false;}
    const auto duration=static_cast<std::uint64_t>(std::int64_t(end_)-start_);
    if(cursor.elapsed_ms>UINT64_MAX-delta){error="Animation marker elapsed overflow";return false;}
    const auto now=loop?cursor.elapsed_ms+delta:std::min(duration,cursor.elapsed_ms+delta);
    if(!loop&&cursor.elapsed_ms>duration){error="One-shot marker cursor outside clip";return false;}
    std::vector<MarkerOccurrence> next;
    for(const auto&marker:markers_){if(marker.time_ms<start_||marker.time_ms>end_)continue;const auto offset=static_cast<std::uint64_t>(std::int64_t(marker.time_ms)-start_);
        std::uint64_t cycle=0;if(loop&&cursor.elapsed_ms>offset){cycle=(cursor.elapsed_ms-offset)/duration;if(cursor.elapsed_ms-offset!=cycle*duration||cursor.started)++cycle;}
        else if(cursor.started&&cursor.elapsed_ms==offset){if(!loop)continue;cycle=1;}
        if(!loop&&offset<cursor.elapsed_ms)continue;
        for(;;){if(cycle>(UINT64_MAX-offset)/duration)break;const auto at=cycle*duration+offset;if(at>now)break;
            if(next.size()==1000000){error="Animation marker emission limit exceeded";return false;}
            next.push_back({marker,cursor.generation,cycle,at,now-at});if(!loop||cycle==UINT64_MAX)break;++cycle;
        }
    }
    std::stable_sort(next.begin(),next.end(),[](const auto&a,const auto&b){if(a.elapsed_ms!=b.elapsed_ms)return a.elapsed_ms<b.elapsed_ms;if(a.cycle!=b.cycle)return a.cycle<b.cycle;return a.marker.index<b.marker.index;});
    cursor.elapsed_ms=now;cursor.started=true;output=std::move(next);return true;
}
}
