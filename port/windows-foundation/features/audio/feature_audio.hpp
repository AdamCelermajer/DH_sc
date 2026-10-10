#pragma once
#include "../../../engine-audio/audio_bank_v34.hpp"
#include <map>
#include <tuple>
namespace dh::foundation::audio {
// Caller resolves original SoundAutoGen ordinals and Play versus Play3D/event
// semantics through the recovered owner BEFORE submitting this transport leaf.
struct SourceAudioRequest {
 std::uint64_t generation{}, producer{}, occurrence{};
 int catalog_uid{-1}; bool event{};
 bool assign_catalog_group{true};
 dh2::audio::AudioCommandV34 command;
 dh2::audio::AudioRandomV34 random;
};
class SourceAudioRouter {
 dh2::audio::AudioMixerV34& mixer_;
 dh2::audio::AudioSampleBankV34 bank_;
 std::map<std::tuple<std::uint64_t,std::uint64_t,std::uint64_t>,std::uint64_t> delivered_;
public:
 SourceAudioRouter(dh2::audio::AudioCatalogV34& c,dh2::audio::AudioMixerV34& m,dh2::audio::AudioAssetServicesV34 assets):mixer_(m),bank_(c,m,assets){}
 bool initialize(std::string&);
 bool submit(const SourceAudioRequest&,std::uint64_t& token,std::string&);
 bool stop(std::uint64_t token,std::uint64_t at_frame,std::uint32_t fade_frames,std::string&);
 bool retire_generation(std::uint64_t generation,std::uint64_t at_frame,std::string&);
 bool retire_producer_generation(std::uint64_t generation,std::uint64_t producer,std::uint64_t at_frame,std::string&);
 void pump(){bank_.pump_receipts();}
 bool receipt(dh2::audio::AudioReceiptV34& r){return bank_.take_receipt(r);}
};
// Exact URI reader: root/data/sounds/filename; no basename/suffix fallback.
struct AudioFilesystem {std::string root;static bool read(void*,const char*,std::shared_ptr<const std::vector<std::uint8_t>>&,std::string&);};
// Common platform delivery contract. WinMM and existing AndroidAudioOutputV34
// consume the SAME mixer; only one consumer may be attached at a time.
struct AudioOutputServices {void* context{};bool(*open)(void*,std::string&){};bool(*update)(void*,std::string&){};void(*close)(void*){};};
bool open_audio_output(const AudioOutputServices&,std::string&);
}
