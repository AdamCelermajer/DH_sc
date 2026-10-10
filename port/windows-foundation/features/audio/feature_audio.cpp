#include "feature_audio.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <cmath>
#include <cstdio>
#include "../../../engine-audio/audio_probe_v1.hpp"
namespace dh::foundation::audio {
bool SourceAudioRouter::initialize(std::string& error){if(bank_.initialize_banks())return true;error=bank_.error();return false;}
bool SourceAudioRouter::submit(const SourceAudioRequest& r,std::uint64_t& token,std::string& error){
 token=0;error.clear();
 if(!r.generation||!r.producer||!r.occurrence){error="Required source generation/producer/occurrence identity";return false;}
 auto key=std::make_tuple(r.generation,r.producer,r.occurrence);
 auto old=delivered_.find(key);if(old!=delivered_.end()){token=old->second;return true;}
 if(r.catalog_uid<0){delivered_.emplace(key,0);return true;}
 if(!std::isfinite(r.command.left)||!std::isfinite(r.command.right)||!std::isfinite(r.command.pitch)||r.command.pitch<=0){error="Required finite source audio gains and positive pitch";return false;}
 bool ok=r.event?bank_.play_event(r.catalog_uid,r.random,r.command,token):bank_.play_uid(r.catalog_uid,r.command,token,r.assign_catalog_group);
 if(!ok){error=bank_.error();return false;}delivered_.emplace(key,token);return true;
}
bool SourceAudioRouter::stop(std::uint64_t token,std::uint64_t frame,std::uint32_t fade,std::string& error){if(!token)return true;dh2::audio::AudioCommandV34 c;c.kind=dh2::audio::AudioCommandKindV34::stop;c.token=token;c.start_frame=frame;c.fade_frames=fade;if(mixer_.post(c))return true;error="Audio stop queue full";return false;}
bool SourceAudioRouter::retire_generation(std::uint64_t generation,std::uint64_t frame,std::string& error){for(auto i=delivered_.begin();i!=delivered_.end();){if(std::get<0>(i->first)!=generation){++i;continue;}if(!stop(i->second,frame,0,error))return false;i=delivered_.erase(i);}return true;}
bool SourceAudioRouter::retire_producer_generation(std::uint64_t generation,std::uint64_t producer,std::uint64_t frame,std::string& error){for(auto i=delivered_.begin();i!=delivered_.end();){if(std::get<0>(i->first)!=generation||std::get<1>(i->first)!=producer){++i;continue;}if(!stop(i->second,frame,0,error))return false;i=delivered_.erase(i);}return true;}
bool AudioFilesystem::read(void* raw,const char* uri,std::shared_ptr<const std::vector<std::uint8_t>>& out,std::string& error){
 struct ProbeRead{double t0;const char* uri;std::shared_ptr<const std::vector<std::uint8_t>>& out;~ProbeRead(){const double d=dh2::audio::probe::ms_now()-t0;std::printf("PROBE read uri=%s bytes=%zu ms=%.1f at=%.0f\n",uri?uri:"",out?out->size():std::size_t(0),d,t0);}} probeRead{dh2::audio::probe::ms_now(),uri,out};
 out.reset();if(!raw||!uri){error="Required exact original audio URI reader";return false;}
 const std::filesystem::path relative(uri);if(relative.is_absolute()){error="Audio URI must be relative";return false;}for(const auto& p:relative)if(p==".."){error="Audio URI traversal rejected";return false;}
 std::ifstream stream(std::filesystem::path(static_cast<AudioFilesystem*>(raw)->root)/relative,std::ios::binary);if(!stream){error=std::string("Unavailable original audio asset: ")+uri;return false;}
 auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(stream),std::istreambuf_iterator<char>());if(stream.bad()){error="Original audio read failed";return false;}out=std::move(bytes);return true;
}
bool open_audio_output(const AudioOutputServices& s,std::string& error){if(!s.context||!s.open||!s.update||!s.close){error="Required actual platform audio output backend";return false;}return s.open(s.context,error);}
}
