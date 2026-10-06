#include "../../audio_source_bindings_v38.hpp"
#include "../../audio_listener_rows_v38.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <cstring>
#include <limits>
using Bytes = std::vector<std::uint8_t>;
extern "C" unsigned dh2_vox_source_fields_v38(const unsigned char*,unsigned char*);
static Bytes read(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error(path);
    return Bytes(std::istreambuf_iterator<char>(stream), {});
}
int main(int argc, char** argv) {
    if (argc != 2) return 2;
    const std::string directory=argv[1];
    auto binary=read(directory+"/sdd_dungeon_hunter_2_iphone_pyarray.bin");
    auto names=read(directory+"/sdd_dungeon_hunter_2_iphone_pyarraynames.bin");
    auto gold=read(directory+"/original-runtime-pairs.bin");
    dh2::audio::AudioSourceBindingsV38 bindings;
    std::string error;
    unsigned checks{};
    auto check=[&](bool result) { ++checks; if (!result) throw std::runtime_error(error.empty()?"binding assertion":error); };
    auto load=[&](const Bytes& b,const Bytes& n) { return bindings.load(b.data(),b.size(),n.data(),n.size(),error); };
    auto i32=[](const Bytes& data,std::size_t offset) {
        const auto word=std::uint32_t(data[offset])|(std::uint32_t(data[offset+1])<<8)|(std::uint32_t(data[offset+2])<<16)|(std::uint32_t(data[offset+3])<<24);
        std::int32_t result; std::memcpy(&result,&word,4); return result;
    };
    check(load(binary,names)); check(bindings.rows().size()==638); check(gold.size()==638*8);
    for (std::int32_t id=0;id<638;++id) {
        const auto* row=bindings.row(id);
        check(row&&row->uid==i32(gold,std::size_t(id)*8)&&row->event==i32(gold,std::size_t(id)*8+4));
        check(bindings.source_id(bindings.names()[std::size_t(id)])==id);
    }
    check(!bindings.row(-1)); check(!bindings.row(638)); check(bindings.source_id("RequiredMissingName")==-1);
    check(bindings.source_id("DropGold")==62); check(bindings.row(62)->uid==151);
    check(bindings.source_id("sfx_mage_staff_elemental_earth")==478); check(bindings.row(478)->uid==31);
    for (std::size_t n : {std::size_t(0),std::size_t(3),binary.size()-1}) {
        Bytes bad(binary.begin(),binary.begin()+static_cast<std::ptrdiff_t>(n)); check(!load(bad,names)); check(bindings.rows().size()==638);
    }
    auto bad=binary; bad[8]=2; check(!load(bad,names)); check(bindings.row(62)->uid==151);
    bad=binary; bad.push_back(0); check(!load(bad,names));
    bad=names; bad[0]=0; check(!load(binary,bad));
    bad=names; bad[8]=0; check(!load(binary,bad));
    bad=names; bad.pop_back(); check(!load(binary,bad));
    bad=names; bad.push_back(0); check(!load(binary,bad));
    check(load(binary,names));
    const auto fields=read(directory+"/../authorities-v38/original-properties-gold.bin");
    check(fields.size()==8+102*188); check(i32(fields,4)==102);
    for (std::size_t i=0;i<102;++i) {
        unsigned char actual[52]{};
        const auto* input=fields.data()+8+i*188;
        check(dh2_vox_source_fields_v38(input,actual)==52);
        check(std::memcmp(actual,input+136,52)==0);
    }
    const auto legacy=read(directory+"/../../../android-native/app/src/main/assets/data/sounds_pyarray.bin");
    std::vector<dh2::audio::AudioListenerRowV38> listeners;
    check(dh2::audio::audio_listener_rows_v38(legacy.data(),legacy.size(),listeners,error));
    check(listeners.size()==5);
    const char* listener_names[]{"AAA_DONT_DELETE_Listener","curListener","PlayerListener","BAPListener","CameraListener"};
    for (std::size_t i=0;i<5;++i) {
        const auto listener_gold=read(directory+"/../authorities-v38/"+listener_names[i]+".bin");
        check(listener_gold.size()==24);
        check(std::memcmp(&listeners[i],listener_gold.data(),24)==0);
        dh2::audio::VoxListenerAuthorityV38 authority{};
        authority.listener.velocity[0]=7; authority.listener.velocity[1]=-8; authority.listener.velocity[2]=9;
        authority.reference_distance=-123; authority.maximum_distance=456; authority.rolloff=2;
        const float position[]{float(i)+10,20,-30},front[]{0,0,-1},up[]{0,1,0};
        check(dh2::audio::audio_listener_update_v38(listeners[i],position,front,up,authority,error));
        check(std::memcmp(authority.listener.position,position,12)==0);
        check(std::memcmp(authority.listener.front,front,12)==0);
        check(std::memcmp(authority.listener.up,up,12)==0);
        check(authority.listener.velocity[0]==7&&authority.listener.velocity[1]==-8&&authority.listener.velocity[2]==9);
        check(authority.reference_distance==listeners[i].reference_distance&&authority.maximum_distance==listeners[i].maximum_distance&&authority.rolloff==listeners[i].rolloff);
        const auto before=authority;
        const float invalid[]{0,std::numeric_limits<float>::quiet_NaN(),0};
        check(!dh2::audio::audio_listener_update_v38(listeners[i],invalid,front,up,authority,error));
        check(std::memcmp(&before,&authority,sizeof authority)==0);
        check(!dh2::audio::audio_listener_update_v38(listeners[i],position,nullptr,up,authority,error));
        check(std::memcmp(&before,&authority,sizeof authority)==0);
    }
    // Find the verified Listener prefix extent in the actual multi-table stream;
    // later legacy tables are intentionally outside this parser's contract.
    std::size_t prefix=4;
    const auto characters=i32(legacy,0);
    for (std::int32_t i=0;i<characters;++i) {
        for (unsigned j=0;j<4;++j) { const auto count=i32(legacy,prefix); prefix+=4+4*std::size_t(count); }
        prefix+=2;
    }
    const auto listener_count_offset=prefix;
    prefix+=4+24*listeners.size();
    const auto listener_snapshot=listeners;
    for (std::size_t length : {std::size_t(0),std::size_t(3),prefix-1}) {
        check(!dh2::audio::audio_listener_rows_v38(legacy.data(),length,listeners,error));
        check(listeners.size()==5&&std::memcmp(listeners.data(),listener_snapshot.data(),120)==0);
    }
    auto invalid_legacy=legacy;
    for (unsigned j=0;j<4;++j) invalid_legacy[listener_count_offset+j]=255;
    check(!dh2::audio::audio_listener_rows_v38(invalid_legacy.data(),invalid_legacy.size(),listeners,error));
    check(listeners.size()==5&&std::memcmp(listeners.data(),listener_snapshot.data(),120)==0);
    std::cout << "PASS " << checks << " checks; source binding pairs and names plus transactional failure cases\n";
}
