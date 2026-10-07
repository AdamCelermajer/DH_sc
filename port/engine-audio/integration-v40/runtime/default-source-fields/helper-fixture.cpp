#include "default_source_fields_v40.hpp"
#include <fstream>
#include <iterator>
#include <vector>
#include <stdexcept>
#include <iostream>
int main(int argc,char**argv) {
    if(argc!=2)return 2;
    std::ifstream file(argv[1],std::ios::binary);
    std::vector<unsigned char> gold(std::istreambuf_iterator<char>(file),{});
    unsigned checks{};auto check=[&](bool value){++checks;if(!value)throw std::runtime_error("original fresh-emitter default mismatch");};
    check(gold.size()==8+24*60);
    auto general=dh2::audio::original_vox_boot_general_v40();
    check(general.distance_model==2);check(general.doppler_factor==1.f);
    std::uint32_t speed;std::memcpy(&speed,&general.speed_over_doppler,4);check(speed==0x43aba666);
    dh2::audio::original_vox_manager_general_init_v40(general);check(general.distance_model==4);
    for(unsigned i=0;i<24;++i) {
        auto source=dh2::audio::original_fresh_emitter_fields_v40(general);
        const auto* original=gold.data()+8+i*60;
        check(std::memcmp(source.position,original,12)==0);
        check(std::memcmp(source.velocity,original+12,12)==0);
        check(!source.relative);
        check(std::memcmp(&source.maximum_distance,original+40,4)==0);
        check(std::memcmp(&source.reference_distance,original+44,4)==0);
        check(std::memcmp(&source.rolloff,original+48,4)==0);
        const float gain=dh2::audio::original_fresh_emitter_gain_v40(),pitch=dh2::audio::original_fresh_emitter_pitch_v40();
        check(std::memcmp(&gain,original+52,4)==0);check(std::memcmp(&pitch,original+56,4)==0);
        check(source.distance_model==4&&source.doppler_factor==1.f);
    }
    std::cout<<"PASS "<<checks<<" default-field checks against original ARM emitter records\n";
}
