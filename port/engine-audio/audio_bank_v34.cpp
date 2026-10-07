#include "audio_bank_v34.hpp"
#include <algorithm>
namespace dh2::audio {
void AudioSampleBankV34::trim(){
 while(cache_bytes_>cache_limit_){auto oldest=cache_.end();for(auto it=cache_.begin();it!=cache_.end();++it)if(it->second.sample.use_count()==1&&(oldest==cache_.end()||it->second.used<oldest->second.used))oldest=it;if(oldest==cache_.end())return;cache_bytes_-=oldest->second.sample->bytes->size();cache_.erase(oldest);}
}
bool AudioSampleBankV34::initialize_banks(){const auto&banks=catalog_.banks();if(!mixer_.configure_banks(banks.data(),unsigned(banks.size()))){error_="Required stopped source bank configuration";return false;}return true;}
std::shared_ptr<const AudioSampleV34> AudioSampleBankV34::load(int uid){
 error_.clear();const auto*definition=catalog_.sound(uid);if(!definition){error_="Required original soundpack UID "+std::to_string(uid);return {};}
 const auto slot=source_slots_v101_.find(uid);if(slot!=source_slots_v101_.end()){error_=slot->second.unavailable;return slot->second.sample;}
 auto existing=cache_.find(definition->filename);if(existing!=cache_.end()){existing->second.used=++use_clock_;source_slots_v101_.emplace(uid,SourceSlotV101{uid,existing->second.sample,{}});return existing->second.sample;}
 if(!assets_.read&&!assets_.read_optional){error_="Required original audio asset reader";return {};}
 const std::string uri="data/sounds/"+definition->filename;std::shared_ptr<const std::vector<std::uint8_t>>bytes;
 if(assets_.read_optional){bool found{};
  if(!assets_.read_optional(assets_.context,uri.c_str(),found,bytes,error_))return {};
  if(!found){
   error_="Unavailable original audio datasource: "+uri;
   //LoadSound3699fc always stores its allocated DataHandle receiver;
   //LoadDataSource86b144 constructs id=-1 when file open fails. This slot
   //has identity, but no ready sample/bytes/voice and is never retried.
   source_slots_v101_.emplace(uid,SourceSlotV101{uid,{},error_});return {};
  }
 }else if(!assets_.read(assets_.context,uri.c_str(),bytes,error_)){if(error_.empty())error_="Required original audio asset "+uri;return {};}
 auto sample=std::make_shared<AudioSampleV34>();if(!audio_sample_open_v34(std::move(bytes),*sample,error_))return {};
 // The original filename's real stream is authoritative for decoding.
 // XML format vxn must select the native segmented family, never a flat WAV.
 if((definition->format==2)!=sample->native){error_="Required original XML/source stream family identity";return {};}
 cache_bytes_+=sample->bytes->size();cache_.emplace(definition->filename,Entry{sample,++use_clock_});source_slots_v101_.emplace(uid,SourceSlotV101{uid,sample,{}});trim();return sample;
}
bool AudioSampleBankV34::play_uid(int uid,AudioCommandV34 source,std::uint64_t&token,bool assign_catalog_group){
 token=0;auto sample=load(uid);if(!sample)return false;const auto*definition=catalog_.sound(uid);const auto*group=catalog_.group(definition->group);if(!group){error_="Required same soundpack group";return false;}
 source.kind=AudioCommandKindV34::play;source.sample=sample.get();source.bank=definition->bank;source.priority=definition->priority;
 if(assign_catalog_group)source.volume_group=group->volume_group;
 source.loop=definition->loop;
 if(sample->native&&source.native_state<0){error_="Required source initial native music state";return false;}
 if(!next_token_){error_="Source audio token exhaustion";return false;}source.token=next_token_++;
 pins_.emplace(source.token,sample);if(!mixer_.post(source)){pins_.erase(source.token);error_="Audio source command queue full";return false;}token=source.token;return true;
}
bool AudioSampleBankV34::play_event(int event,AudioRandomV34 random,AudioCommandV34 source,std::uint64_t&token){int uid;token=0;if(!catalog_.select_event(event,random,uid,error_))return false;if(uid<0)return true;return play_uid(uid,source,token);}
void AudioSampleBankV34::pump_receipts(){AudioReceiptV34 receipt;while(observed_.size()<1024&&mixer_.receipt(receipt)){observed_.push_back(receipt);if(receipt.kind!=AudioReceiptKindV34::started&&receipt.kind!=AudioReceiptKindV34::control_required)pins_.erase(receipt.token);}trim();}
bool AudioSampleBankV34::take_receipt(AudioReceiptV34&out){if(observed_.empty())return false;out=observed_.front();observed_.erase(observed_.begin());return true;}
}
