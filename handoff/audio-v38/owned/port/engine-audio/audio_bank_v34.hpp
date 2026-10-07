#pragma once
#include "audio_catalog_v34.hpp"
#include <functional>
#include <memory>
#include <map>
namespace dh2::audio {
struct AudioAssetServicesV34 {void*context{};bool(*read)(void*,const char* exact_source_uri,std::shared_ptr<const std::vector<std::uint8_t>>&,std::string&){};};
// Producer-thread cache. Voice tokens pin bytes until the audio consumer's
// terminal receipt is drained; LRU eviction never touches an active sample.
class AudioSampleBankV34 {
 struct Entry{std::shared_ptr<const AudioSampleV34> sample;std::uint64_t used{};};
 AudioCatalogV34&catalog_;AudioMixerV34&mixer_;AudioAssetServicesV34 assets_;
 std::map<std::string,Entry>cache_;std::map<std::uint64_t,std::shared_ptr<const AudioSampleV34>>pins_;
 std::uint64_t next_token_{1},use_clock_{},cache_limit_{64*1024*1024},cache_bytes_{};
 std::vector<AudioReceiptV34>observed_;std::string error_;
 void trim();
public:
 AudioSampleBankV34(AudioCatalogV34&c,AudioMixerV34&m,AudioAssetServicesV34 a):catalog_(c),mixer_(m),assets_(a){}
 bool initialize_banks();
 std::shared_ptr<const AudioSampleV34> load(int source_uid);
 // Command gains/pitch/timestamp come from the source request/3D producer.
 // No automatic bank readiness, level phase, or source event is fabricated.
 bool play_uid(int uid,AudioCommandV34 source,std::uint64_t&token);
 bool play_event(int event,AudioRandomV34,AudioCommandV34 source,std::uint64_t&token);
 void pump_receipts();
 bool take_receipt(AudioReceiptV34&);
 std::size_t pinned_samples()const noexcept{return pins_.size();}
 std::uint64_t cache_bytes()const noexcept{return cache_bytes_;}
 const std::string&error()const noexcept{return error_;}
};
}
