#include "../menu_profile_metadata_v1.hpp"
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
using Buffer=std::vector<std::uint8_t>;
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
Buffer read(const std::string& path){std::ifstream file(path,std::ios::binary);check(bool(file),"input missing");return {std::istreambuf_iterator<char>(file),{}};}
struct Reader{Buffer bytes;std::size_t at=0;
    std::uint32_t word(){check(at+4<=bytes.size(),"fixture truncated");std::uint32_t value=0;for(unsigned i=0;i<4;++i)value|=std::uint32_t(bytes[at++])<<(8*i);return value;}
    std::int32_t integer(){auto raw=word();std::int32_t value;std::memcpy(&value,&raw,4);return value;}
    Buffer blob(){auto count=word();check(at+count<=bytes.size(),"fixture blob truncated");Buffer out(bytes.begin()+at,bytes.begin()+at+count);at+=count;return out;}
    std::string text(){auto value=blob();return {value.begin(),value.end()};}
};
struct Context{std::int32_t selected=0;unsigned stores=0,quests=0;bool reject=false;
    static bool store(void* raw,std::int32_t value,std::string&){auto& c=*static_cast<Context*>(raw);c.selected=value;++c.stores;return true;}
    static bool quest(void* raw,Bytes bytes,std::array<std::int32_t,3>& regular,std::array<std::int32_t,3>& volatile_acts,std::string& error){
        auto& c=*static_cast<Context*>(raw);++c.quests;check(bytes.size==4,"quest provider payload");
        if(c.reject){error="TEST-quest-rejected";return false;}regular={7,8,9};volatile_acts={4,5,6};return true;
    }
};
void append(Buffer& bytes,unsigned value){for(unsigned i=0;i<4;++i)bytes.push_back(value>>(8*i));}
Buffer one_section(const char* tag,const Buffer& payload){Buffer out;append(out,1);append(out,payload.size());out.insert(out.end(),tag,tag+4);out.insert(out.end(),payload.begin(),payload.end());return out;}
int main(int argc,char** argv){try{
    check(argc==3,"usage character-cache-dir fixture");std::string directory=argv[1],error;
    auto data=read(directory+"/character_properties_pyarray.bin"),names=read(directory+"/character_properties_pyarraynames.bin"),schema=read(directory+"/character_properties_pystructnames.bin");
    CharacterTable characters;check(load_characters({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},characters,error),"actual characters unavailable");
    Reader input{read(argv[2])};check(input.word()==0x31504d4d,"fixture signature");auto count=input.word();Context context;
    MenuProfileMetadataServicesV1 services{&context,Context::store,nullptr};
    for(unsigned i=0;i<count;++i){
        auto slot=input.integer(),initial=input.integer();auto profile=input.blob();context={};context.selected=initial;
        MenuProfileMetadataV1 actual;
        check(load_menu_profile_metadata_v1({profile.data(),profile.size()},characters,slot,initial,services,actual,error),error.c_str());
        check(actual.slot==input.integer()&&actual.level==input.integer()&&actual.character_row==input.integer(),"original identity/level/class");
        check(actual.selected_difficulty==input.integer()&&actual.unlocked_difficulty==input.integer(),"original difficulty");
        check(context.selected==actual.selected_difficulty,"global selected difficulty delivery");
        const auto expected_date=input.word();const auto expected_name=input.text();
        if(actual.location.save_date!=expected_date||actual.name!=expected_name){
            std::fprintf(stderr,"Row %u date %u/%u name bytes %zu/%zu\n",i,actual.location.save_date,expected_date,actual.name.size(),expected_name.size());
            throw std::runtime_error("original date/name");
        }
        for(const auto* values:{&actual.location.levels,&actual.location.seeds,&actual.location.current_acts,&actual.location.volatile_acts,&actual.location.entry_points})
            for(auto value:*values)check(value==input.integer(),"original location/act/entry array");
        for(auto flag:actual.location.use_spawn_point)check(flag==input.word(),"original raw spawn byte");
    }
    check(input.at==input.bytes.size(),"fixture trailing bytes");
    MenuProfileMetadataV1 result;result.name="retain";
    auto qest=one_section("QEST",{1,2,3,4});
    check(!load_menu_profile_metadata_v1({qest.data(),qest.size()},characters,2,0,services,result,error)&&result.name=="retain"&&error=="Menu profile QEST requires canonical quest loader","QEST silently skipped");
    services.load_quest_acts=Context::quest;
    check(load_menu_profile_metadata_v1({qest.data(),qest.size()},characters,2,0,services,result,error)&&context.quests==1&&result.location.current_acts==std::array<std::int32_t,3>{7,8,9}&&result.location.volatile_acts==std::array<std::int32_t,3>{4,5,6},"canonical QEST delivery");
    context.reject=true;result.name="retain";
    check(!load_menu_profile_metadata_v1({qest.data(),qest.size()},characters,2,0,services,result,error)&&error=="TEST-quest-rejected"&&result.name=="retain","quest provider refusal");
    auto malformed=one_section("PLVL",{1,2,3});
    check(!load_menu_profile_metadata_v1({malformed.data(),malformed.size()},characters,2,0,services,result,error)&&result.name=="retain","malformed present section treated missing");
    auto pdfl=one_section("PDFL",{2,0,0,0,1,0,0,0});services.store_selected_difficulty=nullptr;
    check(!load_menu_profile_metadata_v1({pdfl.data(),pdfl.size()},characters,2,0,services,result,error)&&result.name=="retain","missing CurrentDifficulty owner accepted");
    std::printf("PASS %u original sparse/complete menu metadata profiles; QEST provider gating, both acts, malformed present section, difficulty ownership and atomic output\n",count);
}catch(const std::exception& e){std::fprintf(stderr,"FAIL %s\n",e.what());return 1;}}
