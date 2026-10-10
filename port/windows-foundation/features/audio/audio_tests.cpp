#include "feature_audio.hpp"
#include "winmm_output.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
#include <filesystem>
using namespace dh::foundation::audio;
static std::vector<std::uint8_t> bytes(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
static bool asset(void* p,const char* uri,std::shared_ptr<const std::vector<std::uint8_t>>& out,std::string&){const std::string name(uri);assert(name.rfind("data/sounds/",0)==0);out=std::make_shared<const std::vector<std::uint8_t>>(bytes(std::filesystem::path(static_cast<const char*>(p))/name.substr(12)));return true;}
int main(int argc,char** argv){assert(argc==2);std::string error;dh2::audio::AudioCatalogV34 catalog;assert(catalog.load_xml(bytes(std::filesystem::path(argv[1])/"sounds.xml"),error));
 assert(catalog.sound_uid("FootstepWalkStone1")==72);assert(catalog.sound(72)->filename=="sfx_fs_walk_stone_1_22.wav");assert(catalog.sound_uid("WeaponSwoosh1")==22);assert(catalog.sound_uid("made_up_sound")==-1);
 dh2::audio::AudioMixerV34 mixer;SourceAudioRouter router(catalog,mixer,{argv[1],asset,{}});assert(router.initialize(error));
 SourceAudioRequest request;request.generation=1;request.producer=2;request.occurrence=3;request.catalog_uid=22;request.command.start_frame=1024;
 std::uint64_t token{},again{};assert(router.submit(request,token,error)&&token);assert(router.submit(request,again,error)&&token==again);
 std::vector<float> output(2048);mixer.render(output.data(),512);router.pump();dh2::audio::AudioReceiptV34 receipt;assert(!router.receipt(receipt));mixer.render(output.data(),512);router.pump();assert(!router.receipt(receipt));mixer.render(output.data(),512);router.pump();assert(router.receipt(receipt)&&receipt.kind==dh2::audio::AudioReceiptKindV34::started&&receipt.token==token);
 assert(router.stop(token,mixer.output_frame(),0,error));mixer.render(output.data(),512);router.pump();assert(router.receipt(receipt)&&receipt.kind==dh2::audio::AudioReceiptKindV34::stopped);
 request.occurrence=4;request.catalog_uid=-1;assert(router.submit(request,token,error)&&token==0);assert(router.retire_generation(1,mixer.output_frame(),error));
 assert(!open_audio_output({},error));
 request.generation=2;request.occurrence=5;request.catalog_uid=177;request.command.start_frame=mixer.output_frame();assert(router.submit(request,token,error)&&token);
 for(unsigned i=0;i<1200;++i)mixer.render(output.data(),512);router.pump();assert(mixer.active_voices()==1);assert(router.retire_generation(2,mixer.output_frame(),error));mixer.render(output.data(),512);router.pump();assert(mixer.active_voices()==0);
 std::cout<<"original catalog/cue decoder; authored delayed start; duplicate occurrence; stop; generation; absent backend passed\n";
}
