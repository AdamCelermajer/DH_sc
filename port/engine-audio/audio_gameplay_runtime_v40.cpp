#include "audio_gameplay_runtime_v40.hpp"
#include <cstring>
namespace dh2::audio {
AudioGameplayRuntimeV40::AudioGameplayRuntimeV40(std::uintptr_t manager,AudioGameplaySourcesV40 sources):manager_(manager),sources_(sources),bank_(catalog_,mixer_,sources.exact_assets){}
bool AudioGameplayRuntimeV40::initialize_exact_source(std::string&error){
 if(initialized_){error.clear();return true;}
 if(!manager_||!sources_.exact_assets.read){error="Required actual source Vox identity and exact asset service";return false;}
 std::shared_ptr<const std::vector<std::uint8_t>>records,names,xml;
 for(const auto&entry:{std::pair<const char*,decltype(records)*>{AudioSourceBindingsV38::records_uri,&records},{AudioSourceBindingsV38::names_uri,&names},{"data/sounds/sounds.xml",&xml}}){if(!sources_.exact_assets.read(sources_.exact_assets.context,entry.first,*entry.second,error)||!*entry.second){if(error.empty())error="Required original audio source member "+std::string(entry.first);return false;}}
 AudioCatalogV34 catalog;AudioSourceBindingsV38 bindings;if(!catalog.load_xml(*xml,error)||!bindings.load(records->data(),records->size(),names->data(),names->size(),error))return false;
 for(const auto&row:bindings.rows()){if(row.uid<0)continue;if(row.event?std::size_t(row.uid)>=catalog.events().size():!catalog.sound(row.uid)){error="Required generated binding numeric target in selected source XML";return false;}}
 catalog_=std::move(catalog);bindings_=std::move(bindings);if(!bank_.initialize_banks()){error=bank_.error();return false;}initialized_=true;quiescing_=false;error.clear();return true;
}
int AudioGameplayRuntimeV40::emit(const dh2::character::CombatSoundPlayV1&play,int uid,bool event){
 if(!initialized_){error_="Required exact source soundpack initialization";return -1;}
 if(event){if(!catalog_.select_event(uid,sources_.random,uid,error_))return -1;if(uid<0)return 0;}
 const auto*sound=catalog_.sound(uid);if(!sound){error_="Required original selected XML sound UID";return -1;}const auto*group=catalog_.group(sound->group);if(!group){error_="Required original sound group";return -1;}
 // Keep source event RNG/history and exact load prefix ahead of output gate.
 if(!bank_.load(uid)){error_=bank_.error();return -1;}
 if(!sources_.output_ready||!sources_.output_ready(sources_.context,error_)){if(error_.empty())error_="Required focused output for same current source epoch";return -1;}
 std::uint64_t frame;if(!clock_.frame_at(event_ns_,mixer_.output_frame(),frame)){error_="Required real focused output timestamp for source event";return -1;}
 if(voices_.size()>=1024){error_="Audio voice observation capacity; drain producer receipts";return -1;}
 AudioCommandV34 command;if(!sources_.source_command||!sources_.source_command(sources_.context,play,*sound,*group,command,error_)){if(error_.empty())error_="Required same actual source listener/emitter/DSP/music fields";return -1;}command.start_frame=frame;
 std::uint64_t token;if(!bank_.play_uid(uid,command,token)){error_=bank_.error();return -1;}voices_.emplace(token,AudioVoiceStateV40{uid,false});last_token_=token;last_uid_=uid;return 0;
}
int AudioGameplayRuntimeV40::invoke(void*raw,const dh2::sound::VoxPlay3DRequestV2&q,dh2::sound::VoxPlay3DResponseV2&r){
 auto&self=*static_cast<AudioGameplayRuntimeV40*>(raw);using O=dh2::sound::VoxPlay3DOperationV2;
 if(!q.play||q.play->manager!=self.manager_){self.error_="Required same live source Vox manager";return -1;}
 if(q.operation==O::sound_row){const auto*row=self.bindings_.row(q.play->sound_id);if(!row){self.error_="Required exact source ordinal in generated638 table";return -1;}r.value=row->uid;r.type=row->event;return 0;}
 if(q.operation==O::bank_info){const auto*s=self.catalog_.sound(q.selected_sound);if(!s){self.error_="Required original source bank-info UID";return -1;}r.bank_fields[0]=s->uid;r.bank_fields[1]=s->format;r.bank_fields[2]=s->bank;r.bank_fields[3]=s->group;r.bank_fields[4]=s->loading_flags;return 0;}
 if(q.operation==O::emit)return self.emit(*q.play,q.selected_sound,false);
 if(q.operation==O::native_play)return self.emit(*q.play,q.selected_sound,true);
 if(!self.sources_.gates.invoke){self.error_="Required actual World/GS/Vox gate "+std::to_string(unsigned(q.operation));return -1;}
 return self.sources_.gates.invoke(self.sources_.gates.context,q,r);
}
bool AudioGameplayRuntimeV40::submit_actual_play(const dh2::character::CombatSoundPlayV1&play,std::int64_t event,std::string&error){
 if(in_submit_){error="Source audio submit reentry requires separate caller event scope";return false;}
 last_token_=0;last_uid_=-1;last_phase_=0;last_status_=-1;error_.clear();
 if(quiescing_){error="Source World audio teardown is active";return false;}
 if(event<=0){error="Required actual authored source event monotonic time";return false;}
 in_submit_=true;event_ns_=event;struct Exit{bool&active;std::int64_t&event;~Exit(){active=false;event=0;}}exit{in_submit_,event_ns_};dh2::sound::VoxPlay3DOwnerV2 owner({this,invoke});
 try{last_status_=owner.play(play);last_phase_=owner.phase();}catch(const std::exception&e){last_phase_=owner.phase();last_status_=-2;error=std::string("Source audio provider exception: ")+e.what();return false;}
 if(last_status_){error=error_.empty()?owner.error():error_+"; "+owner.error();return false;}error.clear();return true;
}
void AudioGameplayRuntimeV40::pump_receipts(){if(observations_.size()>=1024)return;bank_.pump_receipts();AudioReceiptV34 receipt;while(observations_.size()<1024&&bank_.take_receipt(receipt)){observations_.push_back(receipt);auto found=voices_.find(receipt.token);if(found==voices_.end())continue;if(receipt.kind==AudioReceiptKindV34::started)found->second.started=true;else if(receipt.kind==AudioReceiptKindV34::control_required)found->second.control_required=true;else voices_.erase(found);}}
bool AudioGameplayRuntimeV40::take_receipt(AudioReceiptV34&out){if(observations_.empty())return false;out=observations_.front();observations_.erase(observations_.begin());return true;}
bool AudioGameplayRuntimeV40::load_actual_xml_uid(int uid,std::string&error){if(!initialized_){error="Required original selected soundpack before LoadSound";return false;}if(!bank_.load(uid)){error=bank_.error();return false;}error.clear();return true;}
const AudioVoiceStateV40* AudioGameplayRuntimeV40::voice(std::uint64_t token)const noexcept{const auto found=voices_.find(token);return found==voices_.end()?nullptr:&found->second;}
bool AudioGameplayRuntimeV40::stop_world(std::string&error){AudioCommandV34 command;command.kind=AudioCommandKindV34::stop_all;if(!mixer_.post(command)){error="World stop command queue full; keep source runtime alive";return false;}quiescing_=true;error.clear();return true;}
bool AudioGameplayRuntimeV40::finalize_after_output_closed(const AudioControlClosedProofV40&proof,std::string&error){
 if(proof.mixer!=&mixer_||!proof.close_completed||!proof.control_joined||!proof.close_completed->load(std::memory_order_acquire)||!proof.control_joined->load(std::memory_order_acquire)){error="Required same output close success and joined control thread; keep runtime alive";return false;}
 quiescing_=true;float scratch[2]{};
 // At most511 commands+64 retiring voices; bounded receipt backpressure.
 for(unsigned attempt=0;attempt<1024;++attempt){observations_.clear();pump_receipts();AudioCommandV34 stop;stop.kind=AudioCommandKindV34::stop_all;const bool posted=mixer_.post(stop);mixer_.render(scratch,1);pump_receipts();if(posted&&!mixer_.active_voices()&&!bank_.pinned_samples()){voices_.clear();error.clear();return true;}}
 error="Required final audio receipt/stop completion; keep runtime alive";return false;
}
bool AudioGameplayRuntimeV40::native_state(std::uint64_t token,const char*name,std::string&error){
 const auto*state=voice(token);if(!state||!state->started||!name){error="Required actual active music voice and source state name";return false;}auto sample=bank_.load(state->xml_uid);if(!sample||!sample->native){error="Required same native music sample";return false;}int index=-1;for(unsigned i=0;i<sample->states.size();++i)if(sample->states[i].name==name){if(index>=0){error="Ambiguous original music state";return false;}index=int(i);}if(index<0){error="Required exact authored native state name";return false;}AudioCommandV34 command;command.kind=AudioCommandKindV34::native_state;command.token=token;command.native_state=index;if(!mixer_.post(command)){error="Source music control queue full";return false;}error.clear();return true;
}
}
