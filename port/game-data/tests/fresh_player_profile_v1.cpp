#include "../fresh_player_profile_v1.hpp"
#include "../player_profile_index_v1.hpp"
#include "../player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
using Buffer=std::vector<std::uint8_t>;
Buffer read(const std::string& file){std::ifstream f(file,std::ios::binary);if(!f)throw std::runtime_error(file);return {std::istreambuf_iterator<char>(f),{}};}
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Reader{
    Buffer b;std::size_t at{};
    std::uint32_t word(){check(at+4<=b.size(),"fixture word");std::uint32_t v=0;for(unsigned i=0;i<4;++i)v|=std::uint32_t(b[at++])<<(8*i);return v;}
    Buffer bytes(std::size_t n){check(at+n<=b.size(),"fixture bytes");Buffer v(b.begin()+at,b.begin()+at+n);at+=n;return v;}
    std::string text(){auto v=bytes(word());return {v.begin(),v.end()};}
};
int main(int argc,char** argv){try{
    check(argc==3,"usage cache-dir fixture");std::string folder=argv[1],error;
    auto data=read(folder+"/character_properties_pyarray.bin"),names=read(folder+"/character_properties_pyarraynames.bin"),schema=read(folder+"/character_properties_pystructnames.bin");
    CharacterTable characters;check(load_characters({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},characters,error),error.c_str());
    Reader input{read(argv[2])};auto signature=input.bytes(4);
    const bool dated=signature==Buffer({'F','P','P','3'});
    const bool whole_files=dated||signature==Buffer({'F','P','P','2'});
    check(whole_files||signature==Buffer({'F','P','P','1'}),"fixture signature");
    auto count=input.word();unsigned comparisons=0,file_comparisons=0;
    FreshPlayerProfileV1 result;
    for(unsigned i=0;i<count;++i){
        auto character=input.text(),name=input.text();auto timer=input.word();
        auto date=dated?input.word():0;
        check(fresh_player_profile_v1(characters,character.c_str(),name.c_str(),timer,date,result,error),error.c_str());
        check(result.saved_date==date,"save timestamp owner");
        PlayerProfileIndexV1 index;check(index.load({result.bytes.data(),result.bytes.size()},error),error.c_str());auto view=index.borrow();
        auto sections=input.word();check(sections==7&&view.source_sections().size()==7,"original seven sections");
        for(unsigned j=0;j<sections;++j){auto tag=input.text();auto expected=input.bytes(input.word());auto actual=view.payload(tag.c_str());
            check(actual.size==expected.size()&&(!actual.size||!std::memcmp(actual.data,expected.data(),actual.size)),("original serializer mismatch: "+tag).c_str());++comparisons;
        }
        if(whole_files){auto expected=input.bytes(input.word());check(result.bytes==expected,"original Savegame::saveAll complete buffer mismatch");++file_comparisons;}
        PlayerSavegameV1 saved;std::size_t used=0;
        check(saved.load_name(view.payload("PNAM"),used,error)&&saved.name()==name,"name reader round trip");
        check(saved.load_level(view.payload("PLVL"),used,error)&&saved.level()==1,"level reader round trip");
        check(saved.load_class(view.payload("PCLS"),characters.names,used,error)&&saved.class_id()==result.character_row,"CharacterTable class identity round trip");
        check(saved.load_location(view.payload("LNAM"),used,error)&&used==40,"location reader round trip");
        check(saved.location().save_date==date,"location save date round trip");
        check(saved.load_entry_points(view.payload("LEPT"),used,error)&&used==12,"entry-point reader round trip");
        check(saved.load_spawn_points(view.payload("LUSP"),used,error)&&used==3,"spawn-point reader round trip");
        for(unsigned difficulty=0;difficulty<3;++difficulty){
            check(saved.location().entry_points[difficulty]==0,"original starting entry point");
            check(saved.location().use_spawn_point[difficulty]==1,"original starting spawn flag");
            check(saved.location().levels[difficulty]==41,"starting level location");
            check(std::uint32_t(saved.location().seeds[difficulty])==result.seeds[difficulty],"per-difficulty seed round trip");
            check(saved.location().current_acts[difficulty]==1&&saved.location().volatile_acts[difficulty]==1,"both quest-owner current acts");
        }
    }
    check(input.at==input.b.size(),"fixture trailing bytes");auto before=result.bytes;
    check(!fresh_player_profile_v1(characters,"absent","A",0,0,result,error)&&result.bytes==before,"non-playable atomic failure");
    check(!fresh_player_profile_v1(characters,nullptr,"A",0,0,result,error)&&result.bytes==before,"null atomic failure");
    CharacterTable missing;
    check(!fresh_player_profile_v1(missing,"KnightPlayerBase","A",0,0,result,error)&&result.bytes==before,"missing authored owner atomic failure");
    std::cout<<"PASS original creation metadata: "<<count<<" cases, "<<comparisons<<" original serialized sections, "<<file_comparisons<<" original complete file buffers; native index/name/level/class readers and atomic rejection\n";
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
