#include "events.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::animation {
bool EventTrack::load(const resources::BresView& input,std::string& error){
    *this=EventTrack{};error.clear();
    try{
        if(!input.bytes||input.size>32*1024*1024)throw std::runtime_error("Event image exceeds limit");
        bytes.assign(input.bytes,input.bytes+input.size);resources::BresView image{};
        if(dh2_bres_open(&image,bytes.data(),bytes.size())!=resources::BresError::ok)throw std::runtime_error("Event BRES rejected");
        auto range=[&](std::uint32_t at,std::uint64_t size){if(size&&(at==0||at>image.size||size>image.size-at))throw std::runtime_error("Event field outside image");};
        auto word=[&](std::uint32_t at){range(at,4);const auto* p=image.bytes+at;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);};
        const auto record=word(image.root_offset+0x2c);
        track.type=1;if(!record)return true;
        range(record,24);track.type=word(record);track.count=word(record+8);
        if((track.type!=1&&track.type!=3&&track.type!=4)||word(record+4)!=1||track.count>100000||word(record+16)!=track.count)throw std::runtime_error("Unsupported event track layout");
        const auto times=word(record+12),group_records=word(record+20);
        range(times,std::uint64_t(track.count)*(track.type==1?1:track.type==3?2:4));range(group_records,std::uint64_t(track.count)*8);
        track.times=track.count?image.bytes+times:nullptr;names.resize(track.count);groups.resize(track.count);
        std::uint64_t total=0;
        for(unsigned i=0;i<track.count;++i){
            const auto count=word(group_records+8*i),list=word(group_records+8*i+4);total+=count;
            if(total>1000000)throw std::runtime_error("Event names exceed limit");
            range(list,std::uint64_t(count)*4);
            for(unsigned j=0;j<count;++j){
                const auto at=word(list+4*j);range(at,1);
                const auto* end=std::memchr(image.bytes+at,0,image.size-at);
                if(!end||static_cast<const std::uint8_t*>(end)-(image.bytes+at)>4096)throw std::runtime_error("Event name rejected");
                names[i].push_back(reinterpret_cast<const char*>(image.bytes+at));
            }
            groups[i]={count,count?names[i].data():nullptr};
        }
        track.groups=groups.empty()?nullptr:groups.data();
        if(!dh2_events_validate(&track))throw std::runtime_error("Event times out of order");
        return true;
    }catch(const std::exception& e){*this=EventTrack{};error=e.what();return false;}
}
}
