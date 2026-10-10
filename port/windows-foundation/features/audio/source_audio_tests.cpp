#include "source_audio_runtime.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <filesystem>
#include <iostream>
using namespace dh::foundation;using namespace dh::foundation::audio;
static std::vector<std::uint8_t> read(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){assert(argc==2);const std::filesystem::path root(argv[1]);std::string error;dh2::audio::AudioCatalogV34 catalog;assert(catalog.load_xml(read(root/"data/sounds/sounds.xml"),error));
 dh2::audio::AudioSourceBindingsV38 bindings;auto records=read(root/"data/pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin"),names=read(root/"data/pydata/sdd_dungeon_hunter_2_iphone_pyarraynames.bin");assert(bindings.load(records.data(),records.size(),names.data(),names.size(),error));assert(bindings.row(363)->uid==459);
 auto mixer=std::make_unique<dh2::audio::AudioMixerV34>();AudioFilesystem files{root.string()};SourceAudioRouter router(catalog,*mixer,{&files,AudioFilesystem::read,{}});assert(router.initialize(error));
 int position_queries{},spatial_queries{},plain_queries{},trace_queries{};std::int64_t requested_wall=-1;
 SourceAudioRuntimeServices services;services.actual_manager=11;services.actual_actor=12;services.output_rate=48000;
 services.target_position=[&](std::array<float,3>& p,std::string&){++position_queries;p={3,4,5};return true;};
 services.prepare_3d=[&](const dh2::character::CombatSoundPlayV1& p,const auto& s,const auto&,auto& cmd,bool& admitted,std::string&){++spatial_queries;assert(p.manager==11&&p.target==12&&p.sound_id==232&&p.position[0]==3&&p.position[2]==5&&!p.source_bool&&p.source_integer==1&&p.source_float0==-1&&p.source_float1==-1);assert(s.uid==22);cmd.source_emitter_position=p.position;admitted=true;return true;};
 services.prepare_plain=[&](int id,bool flag,int arg,int group,bool positional,const auto& s,const auto&,auto&,bool& admitted,std::string&){++plain_queries;assert(id==363&&!flag&&!arg&&!group&&!positional&&s.uid==459);admitted=true;return true;};
 services.source_frame=[&](std::int64_t wall,std::uint64_t& frame,std::string&){requested_wall=wall;frame=mixer->output_frame();return true;};services.trace_script=[&](std::string&){++trace_queries;return true;};
 SourceAudioRuntime runtime(router,catalog,bindings,services);RetainedAnimationEvent event{"sfx_WeaponSwoosh1","source-test-clip",0,1250,9,40};assert(runtime.named_sound(event,12,1,error));assert(requested_wall==1210&&position_queries==1&&spatial_queries==1);
 std::vector<float> output(1024);mixer->render(output.data(),512);assert(mixer->active_voices()==1);assert(runtime.named_sound(event,12,1,error));mixer->render(output.data(),512);assert(mixer->active_voices()==1);
 event.wall_timestamp_ms=1350;assert(runtime.named_sound(event,12,1,error));mixer->render(output.data(),512);assert(mixer->active_voices()==2); // later loop/batch reuses ordinal
 bool handled{},blocking{};OriginalCampaignCommand command;command.kind=13;command.scalars={{8,0},{12,0},{13,0},{16,363}};assert(runtime.campaign(CampaignCommandPhase::execute,command,true,20,13,1,1300,handled,blocking,error));assert(handled&&!blocking&&plain_queries==0);
 assert(runtime.campaign(CampaignCommandPhase::execute,command,false,20,13,1,1300,handled,blocking,error));assert(plain_queries==1&&trace_queries==1);mixer->render(output.data(),512);
 command.kind=14;command.scalars={{8,0},{12,0},{16,363}};assert(runtime.campaign(CampaignCommandPhase::execute,command,false,20,13,2,1400,handled,blocking,error));mixer->render(output.data(),512);
 assert(runtime.retire(9,1500,error));mixer->render(output.data(),512);assert(mixer->active_voices()==0);
 router.pump();dh2::audio::AudioReceiptV34 receipt;while(router.receipt(receipt))runtime.observe_receipt(receipt);
 std::cout<<"actual source ordinal/cue asset mapping, named args/lag, duplicate, command13 received gate/plain, command14 stop, generation retirement passed\n";
}
