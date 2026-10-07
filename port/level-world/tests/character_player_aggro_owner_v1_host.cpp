#include "../character_player_aggro_owner_v1.hpp"
#include "../vox_music_state_owner_v1.hpp"
#include "../level_config_music_owner_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iterator>
namespace ch=dh2::character;
namespace {unsigned checks{};void check(bool value,const char*name){++checks;if(!value)throw std::runtime_error(name);}
struct Reader{std::vector<unsigned char> bytes;std::size_t at{};explicit Reader(const std::string&path){std::ifstream f(path,std::ios::binary);bytes.assign(std::istreambuf_iterator<char>(f),{});}std::uint32_t word(){if(bytes.size()-at<4)throw std::runtime_error("Truncated original oracle");std::uint32_t n;std::memcpy(&n,bytes.data()+at,4);at+=4;return n;}};
int open_missing(void*,const char*,std::uintptr_t*out){*out=0;return 0;}int close_missing(void*,std::uintptr_t){return -1;}
struct Fixture {
 std::int32_t in[13]{};std::uint8_t ambient{},level_music{},enabled{};std::uintptr_t scene=0x200000001;
 std::int32_t music=7;std::vector<unsigned> events;unsigned reject{};
 static int invoke(void*p,const ch::PlayerAggroRequestV1*q,ch::PlayerAggroResponseV1*out){auto&f=*static_cast<Fixture*>(p);if(q->service==f.reject)return -1;
 switch(q->service){
 case ch::player_aggro_online_v1:f.events.push_back(1);out->word=f.in[4];return 0;
 case ch::player_aggro_local_player_v1:f.events.push_back(2);out->word=f.in[5];return 0;
 case ch::player_aggro_current_level_v1:f.events.push_back(3);if(f.in[6])out->level={0x300000001,&f.scene,&f.enabled,&f.music};return 0;
 case ch::player_aggro_other_weight_v1:f.events.push_back(4);out->integer=f.in[3];return 0;
 case ch::player_aggro_sound_v1:out->sound={0x400000001,&f.ambient,&f.level_music};return 0;
 case ch::player_aggro_threshold_v1:out->integer=f.in[10];return 0;
 case ch::player_aggro_set_music_state_v1:f.events.push_back(!std::strcmp(q->name,"combat")?70:71);return 0;
 case ch::player_aggro_play_music_v1:check(q->music_id==f.music&&q->loop==1&&q->flag==0&&q->fade==2000,"Original PlayMusic arguments");f.events.push_back(8);return 0;
 default:return -1;
 }};
};
struct VoxFixture {std::int32_t music{},mute{},channel{},count{};std::vector<unsigned> events;
 static int invoke(void*p,const dh2::sound::VoxMusicRequestV1*q,dh2::sound::VoxMusicResponseV1*r){auto&f=*static_cast<VoxFixture*>(p);using namespace dh2::sound;
 switch(q->service){case vox_music_disabled_v1:r->integer=f.mute;return 0;case vox_music_event_index_v1:check(q->music==f.music,"Actual captured music ID");r->integer=2;return 0;case vox_music_channel_v1:check(q->sound_index==2,"Source event sound-index mapping");r->identity=f.channel;return 0;case vox_music_channel_info_v1:f.events.push_back(4);r->integer=f.count;r->identity=0x900000001;return 0;case vox_music_channel_state_v1:check(q->channel==0x8888&&!std::strcmp(q->state,"combat"),"Source ChannelState args");f.events.push_back(5);return 0;case vox_music_channel_info_destroy_v1:check(q->info==0x900000001,"Same actual ChannelInfo destructor");f.events.push_back(6);return 0;default:return -1;}}
};}
int main(int argc,char**argv){try{check(argc==2,"Root required");Reader oracle(std::string(argv[1])+"/port/level-world/reference/character-player-aggro-v1/source-fixture.bin");auto count=oracle.word();auto*debug=dh2_character_debug_create();check(debug,"Actual owned Debug constructor");ch::DebugFileServices24 files{nullptr,open_missing,close_missing};
for(unsigned n=0;n<count;++n){Fixture f;for(auto&v:f.in){const auto bits=oracle.word();std::memcpy(&v,&bits,4);}std::uint32_t expected[4];for(auto&v:expected)v=oracle.word();std::vector<unsigned> events;const auto event_count=oracle.word();for(unsigned i=0;i<event_count;++i)events.push_back(oracle.word());f.ambient=f.in[8];f.level_music=f.in[9];f.enabled=f.in[7];ch::PlayerAggroFieldsV1 fields;fields.count_d0=f.in[1];fields.weight_d4=f.in[2];fields.pending_c4.resize(f.in[12]);ch::CharacterPlayerAggroOwnerV1 owner(0x500000001,0x600000001,fields,*debug,files,{&f,Fixture::invoke});const auto status=f.in[0]?owner.on_deaggro(0x700000001):owner.on_aggro(0x700000001);check(status==0,owner.error().c_str());check(std::uint32_t(fields.count_d0)==expected[0],"Original count field");check(std::uint32_t(fields.weight_d4)==expected[1],"Original AI weight field");check(f.ambient==expected[2],"Original music flag write");check(fields.pending_c4.size()==expected[3],"Original vector end clear");check(f.events==events,"Original call order");}
Reader config(std::string(argv[1])+"/port/level-world/reference/character-player-aggro-v1/crypt01-level-config-music.bin");std::string config_error;auto level_config=dh2::world::LevelConfigMusicOwnerV1::load({config.bytes.data(),config.bytes.size()},config_error);check(bool(level_config),config_error.c_str());check(*level_config->combat_music_enabled()==1,"Actual Crypt config enables music");check(*level_config->attribute("music")=="CryptOneAmbientMusic","Actual Crypt music name");check(level_config->attribute("combat_music")->empty(),"Actual authored empty combat track");check(!dh2::world::LevelConfigMusicOwnerV1::load({config.bytes.data(),config.bytes.size()-1},config_error),"Truncated authored record rejected");
Reader ctor(std::string(argv[1])+"/port/level-world/reference/character-player-aggro-v1/vox-constructor-prefix.bin");dh2::sound::VoxMusicFieldsV1 ctor_fields;check(std::uint32_t(ctor_fields.current_music_24)==ctor.word(),"Original ctor current music");check(std::uint32_t(ctor_fields.field_28)==ctor.word(),"Original ctor field28");check(std::uint32_t(ctor_fields.field_2c)==ctor.word(),"Original ctor field2c");check((unsigned(ctor_fields.enabled_30)|(unsigned(ctor_fields.ambient_31)<<8)|(unsigned(ctor_fields.level_music_32)<<16)|(unsigned(ctor_fields.field_33)<<24))==ctor.word(),"Original ctor sound bytes");
Reader vox_oracle(std::string(argv[1])+"/port/level-world/reference/character-player-aggro-v1/vox-source-fixture.bin");auto vox_count=vox_oracle.word();
for(unsigned n=0;n<vox_count;++n){VoxFixture f;std::int32_t* slots[]={&f.music,&f.mute,&f.channel,&f.count};for(auto*v:slots){auto bits=vox_oracle.word();std::memcpy(v,&bits,4);}auto length=vox_oracle.word();std::vector<unsigned> expected;for(unsigned i=0;i<length;++i)expected.push_back(vox_oracle.word());dh2::sound::VoxMusicFieldsV1 fields;fields.current_music_24=f.music;dh2::sound::VoxMusicStateOwnerV1 owner(fields,{&f,VoxFixture::invoke});check(!owner.set_music_state("combat"),owner.error().c_str());check(f.events==expected,"Original complete Vox music calls");}
dh2::sound::VoxMusicFieldsV1 vox_fields;dh2::sound::VoxMusicStateOwnerV1 vox(vox_fields,{});check(!vox.set_music_state("combat"),"Genuine ctor no-current-music return");vox_fields.current_music_24=0;check(vox.set_music_state("combat")==-2,"Required audio delivery after actual music becomes active");
Fixture missing;missing.reject=ch::player_aggro_online_v1;ch::PlayerAggroFieldsV1 fields;ch::CharacterPlayerAggroOwnerV1 owner(1,2,fields,*debug,files,{&missing,Fixture::invoke});check(owner.on_aggro(3)==-2,"Reached online producer required");check(fields.count_d0==1&&fields.weight_d4==0,"Actual count prefix retained");check(owner.on_aggro(0)==-1,"Null other rejected");dh2_character_debug_destroy(debug);
std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"world_audio_fixtures\":true,\"production_music_provider\":false}"<<std::endl;return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<std::endl;return 1;}}
