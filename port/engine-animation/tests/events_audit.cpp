#include "events.hpp"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
using namespace dh2::animation;
void observe(const TriggeredEvent* event,void* context){assert(event&&event->name);++*static_cast<unsigned*>(context);}
unsigned word(const std::vector<std::uint8_t>& raw,unsigned at){return raw[at]|(unsigned(raw[at+1])<<8)|(unsigned(raw[at+2])<<16)|(unsigned(raw[at+3])<<24);}
void put(std::vector<std::uint8_t>& raw,unsigned at,unsigned value){for(unsigned i=0;i<4;++i)raw[at+i]=value>>(8*i);}
int main(int argc,char** argv){
    assert(argc==2);unsigned files=0,tracks=0,groups=0,events=0,mutations=0;std::vector<std::uint8_t> fixture;
    for(const auto& entry:std::filesystem::directory_iterator(argv[1])){
        if(entry.path().extension()!=".bdae")continue;
        std::ifstream input(entry.path(),std::ios::binary);std::vector<std::uint8_t> raw{std::istreambuf_iterator<char>(input),{}};dh2::resources::BresView image{};
        assert(dh2_bres_open(&image,raw.data(),raw.size())==dh2::resources::BresError::ok);EventTrack source;std::string error;assert(source.load(image,error));++files;
        const auto count=source.view().count;if(!count)continue;++tracks;groups+=count;
        EventTrack moved=std::move(source);raw.clear();raw.shrink_to_fit();EventCursor cursor;unsigned called=0;
        assert(dh2_events_update(&moved.view(),&cursor,-100000,100000,-100000,100000,observe,&called));
        unsigned expected=0;for(unsigned i=0;i<count;++i){const auto& g=moved.view().groups[i];expected+=g.count;for(unsigned j=0;j<g.count;++j)assert(dh2_events_time(&moved.view(),g.names[j])>=-100000);}
        assert(called==expected);events+=called;
        assert(dh2_events_update(&moved.view(),&cursor,100000,100001,-100000,100000,observe,&called));assert(called==expected);
        dh2::resources::BresView absent{};assert(!source.load(absent,error));
        if(fixture.empty()){std::ifstream copy(entry.path(),std::ios::binary);fixture.assign(std::istreambuf_iterator<char>(copy),{});}
    }
    assert(tracks>=3&&events>=3&&!fixture.empty());std::mt19937 random(221026);
    for(unsigned i=0;i<5000;++i){
        auto raw=fixture;for(unsigned j=0;j<1+i%5;++j){const auto at=random()%raw.size();raw[at]^=1u<<(random()%8);}
        dh2::resources::BresView image{};if(dh2_bres_open(&image,raw.data(),raw.size())!=dh2::resources::BresError::ok){++mutations;continue;}
        EventTrack parsed;std::string error;if(parsed.load(image,error)){assert(dh2_events_validate(&parsed.view()));EventCursor cursor;unsigned called=0;assert(dh2_events_update(&parsed.view(),&cursor,-100000,100000,-100000,100000,observe,&called));}else assert(parsed.view().count==0);++mutations;
    }
    for(unsigned field:{0u,4u,8u,12u,16u,20u}){
        auto raw=fixture;const auto record=word(raw,word(raw,32)+0x2c);put(raw,record+field,0xffffffff);dh2::resources::BresView image{};
        if(dh2_bres_open(&image,raw.data(),raw.size())==dh2::resources::BresError::ok){EventTrack parsed;std::string error;assert(!parsed.load(image,error)&&!error.empty()&&parsed.view().count==0);}
    }
    EventView invalid{4,1,nullptr,nullptr};EventCursor cursor;unsigned called=0;
    assert(!dh2_events_update(&invalid,&cursor,0,1,0,100,observe,&called)&&cursor.last_entry==-1&&called==0);
    std::cout<<"{\"original_bdae_files\":"<<files<<",\"authored_tracks\":"<<tracks<<",\"authored_groups\":"<<groups<<",\"callback_events\":"<<events<<",\"mutations\":"<<mutations<<",\"move_ownership_verified\":true,\"invalid_input_rejected\":true}\n";
}
