#include "../menu_profile_metadata_v1.hpp"

#include <array>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
unsigned checks=0;
void require(bool value,const char* message){
    if(!value)throw std::runtime_error(message);
    ++checks;
}
void put_u32(std::vector<std::uint8_t>& bytes,std::uint32_t value){
    for(unsigned i=0;i<4;++i)bytes.push_back(std::uint8_t(value>>(8*i)));
}
struct CallbackContext {
    const dh2::data::PlayerSavegameV1* save{};
    std::array<std::uint8_t,8> expected_payload{};
    std::size_t payload_size{};
    bool called{};
};
bool load_quest_acts(void* raw,
    const std::shared_ptr<dh2::data::PlayerSavegameV1>& save,
    const dh2::data::PlayerProfileIndexV1::Borrow& profile,
    std::array<std::int32_t,3>& regular,
    std::array<std::int32_t,3>& volatile_acts,std::string& error){
    auto& context=*static_cast<CallbackContext*>(raw);
    const auto payload=profile.payload("QEST");
    if(!save||!profile||save->slot()!=3||payload.size!=context.payload_size||
       !payload.data||!std::equal(payload.data,payload.data+payload.size,
                                  context.expected_payload.begin())||
       profile.bytes().data()==nullptr){
        error="QEST callback did not borrow the metadata reader's exact Save/profile";
        return false;
    }
    context.save=save.get();context.called=true;
    regular={{2,4,6}};volatile_acts={{3,5,7}};error.clear();return true;
}
}

int main(){
    try{
        std::vector<std::uint8_t> profile;
        put_u32(profile,1);put_u32(profile,8);profile.insert(profile.end(),{'Q','E','S','T'});
        const std::uint8_t payload[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
        profile.insert(profile.end(),std::begin(payload),std::end(payload));
        dh2::data::CharacterTable characters;CallbackContext context;
        std::copy(std::begin(payload),std::end(payload),context.expected_payload.begin());
        context.payload_size=sizeof(payload);
        dh2::data::MenuProfileMetadataServicesV1 services{&context,nullptr,load_quest_acts};
        dh2::data::MenuProfileMetadataV1 output;std::string error;
        const bool loaded=dh2::data::load_menu_profile_metadata_v1(
            {profile.data(),profile.size()},characters,3,0,services,output,error);
        if(!loaded)throw std::runtime_error(error.empty()?"metadata QEST projection failed":error);
        require(context.called&&context.save,"canonical QEST callback not reached");
        require(output.slot==3&&output.location.current_acts==std::array<std::int32_t,3>{{2,4,6}}&&
                output.location.volatile_acts==std::array<std::int32_t,3>{{3,5,7}},
                "QEST act projection did not use canonical callback result");
        std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks
                 <<",\"same_metadata_save_and_profile_borrow\":true,\"qest_act_projection\":true}\n";
        return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
