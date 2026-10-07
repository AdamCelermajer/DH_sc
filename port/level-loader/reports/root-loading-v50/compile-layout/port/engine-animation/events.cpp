#include "events.hpp"
#include <climits>
#include <cstring>

namespace {
using namespace dh2::animation;
constexpr float frame_ms=33.333332061767578125f; // original 0x42055555
std::int32_t signed_word(std::uint32_t raw){std::int32_t value;std::memcpy(&value,&raw,4);return value;}
std::int32_t add(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)+std::uint32_t(b));}
std::int32_t sub(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)-std::uint32_t(b));}
std::int32_t key(const EventView& track,unsigned i){
    const auto* p=track.times+i*(track.type==1?1:track.type==3?2:4);
    if(track.type==1)return p[0];
    if(track.type==3)return p[0]|(std::uint32_t(p[1])<<8);
    return signed_word(p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24));
}
std::int32_t float_to_int(float f){
    // ARM's __aeabi_f2iz saturates outside signed-int range. Avoid C++ UB.
    if(f>=2147483648.f)return INT32_MAX;
    if(f<=-2147483648.f)return INT32_MIN;
    return static_cast<std::int32_t>(f);
}
std::int32_t find(const EventView& track,std::int32_t ms){
    volatile float time=static_cast<float>(ms);
    if(track.type!=4)time=time/frame_ms;
    unsigned i=0;while(i<track.count&&!(time<static_cast<float>(key(track,i))))++i;
    return std::int32_t(i)-1;
}
void dispatch(const EventView& track,std::int32_t first,std::int32_t last,std::int32_t now,EventCallback callback,void* user){
    for(auto i=first;i<=last;++i){
        volatile float current=static_cast<float>(now);
        volatile float stamp=static_cast<float>(key(track,i));
        volatile float lag=0;
        if(track.type==4)lag=current-stamp;
        else{volatile float negative=stamp*(-frame_ms);lag=current+negative;}
        for(unsigned j=0;j<track.groups[i].count;++j){
            const TriggeredEvent event{float_to_int(lag),track.groups[i].names[j]};callback(&event,user);
        }
    }
}
}
extern "C" bool dh2_events_validate(const dh2::animation::EventView* track){
    if(!track||(track->type!=1&&track->type!=3&&track->type!=4)||track->count>100000)return false;
    if(track->count&&(!track->times||!track->groups))return false;
    std::int32_t prior=INT32_MIN;std::uint64_t total=0;
    for(unsigned i=0;i<track->count;++i){
        const auto stamp=key(*track,i);if(stamp<prior)return false;prior=stamp;
        const auto& group=track->groups[i];total+=group.count;
        if(total>1000000||(group.count&&!group.names))return false;
        for(unsigned j=0;j<group.count;++j)if(!group.names[j])return false;
    }
    return true;
}
extern "C" std::int32_t dh2_events_find(const dh2::animation::EventView* track,std::int32_t ms){
    return dh2_events_validate(track)?find(*track,ms):-1;
}
extern "C" std::int32_t dh2_events_time(const dh2::animation::EventView* track,const char* name){
    if(!name||!dh2_events_validate(track))return -1;
    std::int32_t result=-1;
    for(unsigned i=0;i<track->count;++i)for(unsigned j=0;j<track->groups[i].count;++j)
        if(std::strcmp(name,track->groups[i].names[j])==0){
            volatile float stamp=static_cast<float>(key(*track,i));
            if(track->type!=4)stamp=stamp*frame_ms;
            result=float_to_int(stamp); // original lookup keeps the final match
        }
    return result;
}
extern "C" bool dh2_events_update(const dh2::animation::EventView* track,dh2::animation::EventCursor* cursor,
    std::int32_t previous,std::int32_t current,std::int32_t start,std::int32_t end,dh2::animation::EventCallback callback,void* user){
    if(!cursor||!dh2_events_validate(track))return false;
    if(previous==current||!callback)return true;
    auto first=find(*track,sub(previous,1))+1;const auto last=find(*track,current);
    if(cursor->last_entry==first)++first;
    if(previous>current){
        dispatch(*track,first,find(*track,end),add(sub(end,start),current),callback,user);
        dispatch(*track,find(*track,sub(start,1))+1,last,current,callback,user);
    }else dispatch(*track,first,last,current,callback,user);
    cursor->last_entry=last;return true;
}
extern "C" bool dh2_events_update_interval(const dh2::animation::EventView* track,std::int32_t previous,
    std::int32_t current,dh2::animation::EventCallback callback,void* user){
    if(!dh2_events_validate(track))return false;
    if(callback)dispatch(*track,find(*track,previous)+1,find(*track,current),current,callback,user);
    return true;
}
