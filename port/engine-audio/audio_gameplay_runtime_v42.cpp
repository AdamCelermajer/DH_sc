#include "audio_gameplay_runtime_v42.hpp"
#include <cstring>
#include <cmath>
namespace dh2::audio {
AudioGameplayRuntimeV42::AudioGameplayRuntimeV42(std::uintptr_t manager,AudioGameplaySourcesV40 sources):manager_(manager),sources_(sources),bank_(catalog_,mixer_,sources.exact_assets){}
bool AudioGameplayRuntimeV42::initialize_exact_source(std::string&error){
 if(initialized_){error.clear();return true;}
 if(!manager_||!sources_.exact_assets.read){error="Required actual source Vox identity and exact asset service";return false;}
 std::shared_ptr<const std::vector<std::uint8_t>>records,names,xml;
 for(const auto&entry:{std::pair<const char*,decltype(records)*>{AudioSourceBindingsV38::records_uri,&records},{AudioSourceBindingsV38::names_uri,&names},{"data/sounds/sounds.xml",&xml}}){if(!sources_.exact_assets.read(sources_.exact_assets.context,entry.first,*entry.second,error)||!*entry.second){if(error.empty())error="Required original audio source member "+std::string(entry.first);return false;}}
 AudioCatalogV34 catalog;AudioSourceBindingsV38 bindings;if(!catalog.load_xml(*xml,error)||!bindings.load(records->data(),records->size(),names->data(),names->size(),error))return false;
 for(const auto&row:bindings.rows()){if(row.uid<0)continue;if(row.event?std::size_t(row.uid)>=catalog.events().size():!catalog.sound(row.uid)){error="Required generated binding numeric target in selected source XML";return false;}}
 catalog_=std::move(catalog);bindings_=std::move(bindings);if(!bank_.initialize_banks()){error=bank_.error();return false;}initialized_=true;quiescing_=false;error.clear();return true;
}
int AudioGameplayRuntimeV42::emit(const dh2::character::CombatSoundPlayV1&play,int uid,bool event){
 if(!initialized_){error_="Required exact source soundpack initialization";return -1;}
 if(event){if(!catalog_.select_event(uid,sources_.random,uid,error_))return -1;if(uid<0)return 0;}
 const auto*sound=catalog_.sound(uid);if(!sound){error_="Required original selected XML sound UID";return -1;}const auto*group=catalog_.group(sound->group);if(!group){error_="Required original sound group";return -1;}
 // Keep source event RNG/history and exact load prefix ahead of output gate.
 if(!bank_.load(uid)){error_=bank_.error();return -1;}
 if(!sources_.output_ready||!sources_.output_ready(sources_.context,error_)){if(error_.empty())error_="Required focused output for same current source epoch";return -1;}
 std::uint64_t frame;if(!clock_.frame_at(event_ns_,mixer_.output_frame(),frame)){error_="Required real focused output timestamp for source event";return -1;}
 if(voices_.size()>=1024){error_="Audio voice observation capacity; drain producer receipts";return -1;}
 AudioCommandV34 command;if(!sources_.source_command||!sources_.source_command(sources_.context,play,*sound,*group,command,error_)){if(error_.empty())error_="Required same actual source listener/emitter/DSP/music fields";return -1;}command.start_frame=frame;
 std::uint64_t token;if(!bank_.play_uid(uid,command,token)){error_=bank_.error();return -1;}voices_.emplace(token,VoiceV42{uid,command.source_emitter_position});last_token_=token;last_uid_=uid;return 0;
}
int AudioGameplayRuntimeV42::invoke(void*raw,const dh2::sound::VoxPlay3DRequestV2&q,dh2::sound::VoxPlay3DResponseV2&r){
 auto&self=*static_cast<AudioGameplayRuntimeV42*>(raw);using O=dh2::sound::VoxPlay3DOperationV2;
 if(!q.play||q.play->manager!=self.manager_){self.error_="Required same live source Vox manager";return -1;}
 if(q.operation==O::sound_row){const auto*row=self.bindings_.row(q.play->sound_id);if(!row){self.error_="Required exact source ordinal in generated638 table";return -1;}r.value=row->uid;r.type=row->event;return 0;}
 if(q.operation==O::bank_info){const auto*s=self.catalog_.sound(q.selected_sound);if(!s){self.error_="Required original source bank-info UID";return -1;}r.bank_fields[0]=s->uid;r.bank_fields[1]=s->format;r.bank_fields[2]=s->bank;r.bank_fields[3]=s->group;r.bank_fields[4]=s->loading_flags;return 0;}
 if(q.operation==O::emit)return self.emit(*q.play,q.selected_sound,false);
 if(q.operation==O::native_play)return self.emit(*q.play,q.selected_sound,true);
 if(!self.sources_.gates.invoke){self.error_="Required actual World/GS/Vox gate "+std::to_string(unsigned(q.operation));return -1;}
 return self.sources_.gates.invoke(self.sources_.gates.context,q,r);
}
bool AudioGameplayRuntimeV42::submit_actual_play(const dh2::character::CombatSoundPlayV1&play,std::int64_t event,std::string&error){
 if(in_submit_){error="Source audio submit reentry requires separate caller event scope";return false;}
 last_token_=0;last_uid_=-1;last_phase_=0;last_status_=-1;error_.clear();
 if(quiescing_){error="Source World audio teardown is active";return false;}
 if(event<=0){error="Required actual authored source event monotonic time";return false;}
 in_submit_=true;event_ns_=event;struct Exit{bool&active;std::int64_t&event;~Exit(){active=false;event=0;}}exit{in_submit_,event_ns_};dh2::sound::VoxPlay3DOwnerV2 owner({this,invoke});
 try{last_status_=owner.play(play);last_phase_=owner.phase();}catch(const std::exception&e){last_phase_=owner.phase();last_status_=-2;error=std::string("Source audio provider exception: ")+e.what();return false;}
 if(last_status_){error=error_.empty()?owner.error():error_+"; "+owner.error();return false;}error.clear();return true;
}
void AudioGameplayRuntimeV42::pump_receipts(){if(observations_.size()>=1024)return;bank_.pump_receipts();AudioReceiptV34 receipt;while(observations_.size()<1024&&bank_.take_receipt(receipt)){observations_.push_back(receipt);auto found=voices_.find(receipt.token);if(found==voices_.end())continue;if(receipt.kind==AudioReceiptKindV34::started)found->second.started=true;else if(receipt.kind==AudioReceiptKindV34::control_required)found->second.control_required=true;else voices_.erase(found);}}
bool AudioGameplayRuntimeV42::submit_plain_source(int ordinal,std::int64_t event,const PlainCommand&make,std::string&error){
 if(!initialized_||quiescing_||in_submit_){error="Required active non-reentrant SAME Play receiver";return false;}
 const auto*row=bindings_.row(ordinal);const auto*sound=row?catalog_.sound(row->uid):nullptr;
 const auto*group=sound?catalog_.group(sound->group):nullptr;
 if(!sound||!group){error="Required generated Play row.uid and selected soundpack properties";return false;}
 in_submit_=true;struct Exit{bool&v;~Exit(){v=false;}}exit{in_submit_};
 last_token_=0;last_uid_=-1;
 if(!bank_.load(sound->uid)){error=bank_.error();return false;}
 if(!sources_.output_ready||!sources_.output_ready(sources_.context,error)){if(error.empty())error="Required focused SAME plain Play output";return false;}
 std::uint64_t frame{};
 if(event<=0||!clock_.frame_at(event,mixer_.output_frame(),frame)){error="Required actual authored Play time and focused device clock";return false;}
 if(voices_.size()>=1024){error="Audio voice observation capacity; drain producer receipts";return false;}
 AudioCommandV34 command;
 if(!make||!make(*sound,*group,command,error)){if(error.empty())error="Required whole original fresh Play emitter/properties body";return false;}
 command.kind=AudioCommandKindV34::play;command.start_frame=frame;
 std::uint64_t token{};if(!bank_.play_uid(sound->uid,command,token,false)){error=bank_.error();return false;}
 voices_.emplace(token,VoiceV42{sound->uid,command.source_emitter_position});last_token_=token;last_uid_=sound->uid;
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::source_ordinal_playing(int ordinal,bool&playing,std::string&error){
 const auto*row=bindings_.row(ordinal);if(!initialized_||!row){error="Required actual source music row";return false;}
 pump_receipts();playing=false;
 for(const auto&entry:voices_)if(entry.second.xml_uid==row->uid){playing=true;break;}
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::stop_source_ordinal(int ordinal,std::uint32_t fade,std::string&error){
 const auto*row=bindings_.row(ordinal);if(!initialized_||quiescing_||!row){error="Required active actual Stop sound row";return false;}
 pump_receipts();
 for(const auto&entry:voices_)if(entry.second.xml_uid==row->uid){AudioCommandV34 command;command.kind=AudioCommandKindV34::stop;command.token=entry.first;command.fade_frames=fade;
  if(!mixer_.post(command)){error="Actual Stop queue full; preserve reached source prefix";return false;}}
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::stop_source_sound_v106(int ordinal,int milliseconds,std::string&error){
 return stop_source_3d_v112(ordinal,milliseconds,nullptr,0.f,error);
}
bool AudioGameplayRuntimeV42::stop_source_3d_v112(int ordinal,int milliseconds,const float* center,float radius,std::string&error){
 if(ordinal<0){error.clear();return true;}
 if(!initialized_||quiescing_||in_submit_){error="Required SAME live source Stop receiver";return false;}
 const auto*row=bindings_.row(ordinal);
 if(!row||row->uid<0){error="Required actual generated Stop sound row.uid";return false;}
 // Stop never loads a resource or selects an event. Only an existing ready
 // source datasource reaches original GetEmitterHandles(data, handles,10).
 const auto sample=bank_.source_slot_sample_v101(row->uid);
 if(!sample){error.clear();return true;}
 if(!sample->bytes||sample->bytes->empty()){error.clear();return true;}
 if(observations_.size()>=1024){error="Required receipt drain before source Stop handle enumeration";return false;}
 pump_receipts();
 std::array<std::uint64_t,10>handles{};unsigned count{};
 for(const auto&entry:voices_)if(entry.second.xml_uid==row->uid){handles[count++]=entry.first;if(count==handles.size())break;}
 // Source i2f(fade)/1000.0f; Stop's duration is independent of an authored
 // play timestamp. The actual callback converts it using its current rate.
 const auto duration=float(milliseconds)/1000.f;
 for(unsigned i=0;i<count;++i){AudioCommandV34 command;command.kind=AudioCommandKindV34::stop;
  if(center){const auto& position=voices_.at(handles[i]).position;
   volatile float x=position[0]-center[0],y=position[1]-center[1],z=position[2]-center[2];
   volatile float xx=x*x,yy=y*y,zz=z*z,sum=xx+yy,total=sum+zz;
   const float distance=static_cast<float>(std::sqrt(static_cast<double>(total)));
   //36a3e8 source fcmplt(radius,distance): equality retains the emitter.
   if(!(radius<distance))continue;
  }
  command.token=handles[i];command.source_fade_seconds=duration>0?duration:0;
  if(!mixer_.post(command)){error="Per-source Stop command queue full after reached emitter prefix";return false;}}
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::resume_source_ordinal(int ordinal,std::uint32_t fade,std::string&error){
 const auto*row=bindings_.row(ordinal);if(!initialized_||quiescing_||!row){error="Required actual Resume sound row";return false;}
 for(const auto&entry:voices_)if(entry.second.xml_uid==row->uid){AudioCommandV34 command;command.kind=AudioCommandKindV34::resume_voice;command.token=entry.first;command.fade_frames=fade;
  if(!mixer_.post(command)){error="Actual Resume queue full";return false;}}
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::fresh_native_state_v68(int uid,int&state,std::string&error){
 const auto sample=load_sample_actual_xml_uid(uid,error);if(!sample)return false;
 if(!sample->native||sample->states.empty()){error="Required original native states on SAME decoded sample";return false;}
 // Source NativeSubDecoder C1 zeroes initial state shorts8/a at885da8/dac.
 // Verify that the selected actual VXN contains that constructor-selected row.
 if(sample->states[0].playlist>=sample->playlists.size()){error="Required source initial state's actual playlist";return false;}
 state=0;error.clear();return true;
}
bool AudioGameplayRuntimeV42::initialize_priority_banks_v100(std::string&error){
 if(!initialized_||quiescing_||in_submit_){error="Required constructed SAME source engine for Initialize banks";return false;}
 const auto&banks=catalog_.banks();
 // Source XML vectors include unused UID0; Initialize delivers UIDs1..last.
 // The native catalog retains only actual bank rows, in that same UID order.
 int expected=1;for(const auto&bank:banks)if(bank.id!=expected++){error="Required original contiguous priority bank UID1..last";return false;}
 // Constructor installed these same banks while output was closed. Original
 // Initialize setters now travel through the single consumer, avoiding a
 // producer write to callback storage if focused output is already running.
 for(const auto&bank:banks){AudioCommandV34 command;command.kind=AudioCommandKindV34::configure_bank;command.source_bank=bank;
  if(!mixer_.post(command)){error="Original priority bank setter command queue full";return false;}}
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::select_source_event_v100(int uid,AudioRandomV34 random,int&sound,std::string&error){
 if(!initialized_||quiescing_||in_submit_){error="Required SAME live XML event owner during Initialize";return false;}
 return catalog_.select_event(uid,random,sound,error);
}
bool AudioGameplayRuntimeV42::source_static_bus_routing_v100(const char*path,std::string&error){
 if(!initialized_||quiescing_||!path||std::strcmp(path,"/app_home/data/routing/template_dhpsn.vrt")){
  error="Required reached original static bus routing request";return false;
 }
 // Original callback driver's inherited DriverInterface.SetStaticBusRouting
 // 88ed4c is bx lr. It reads no file and creates no invented DSP routing graph.
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::take_receipt(AudioReceiptV34&out){if(observations_.empty())return false;out=observations_.front();observations_.erase(observations_.begin());return true;}
bool AudioGameplayRuntimeV42::load_actual_xml_uid(int uid,std::string&error){if(!initialized_){error="Required original selected soundpack before LoadSound";return false;}if(!bank_.load(uid)){error=bank_.error();return false;}error.clear();return true;}
std::shared_ptr<const AudioSampleV34> AudioGameplayRuntimeV42::load_sample_actual_xml_uid(int uid,std::string&error){if(!initialized_||quiescing_){error="Required active selected pack before precache";return {};}auto sample=bank_.load(uid);if(!sample){error=bank_.error();return {};}error.clear();return sample;}
const AudioVoiceStateV40* AudioGameplayRuntimeV42::voice(std::uint64_t token)const noexcept{const auto found=voices_.find(token);return found==voices_.end()?nullptr:&found->second;}
bool AudioGameplayRuntimeV42::stop_world(std::string&error){AudioCommandV34 command;command.kind=AudioCommandKindV34::stop_all;if(!mixer_.post(command)){error="World stop command queue full; keep source runtime alive";return false;}quiescing_=true;error.clear();return true;}
bool AudioGameplayRuntimeV42::finalize_after_output_closed(const AudioControlClosedProofV40&proof,std::string&error){
 if(proof.mixer!=&mixer_||!proof.close_completed||!proof.control_joined||!proof.close_completed->load(std::memory_order_acquire)||!proof.control_joined->load(std::memory_order_acquire)){error="Required same output close success and joined control thread; keep runtime alive";return false;}
 quiescing_=true;float scratch[2]{};
 // At most511 commands+64 retiring voices; bounded receipt backpressure.
 for(unsigned attempt=0;attempt<1024;++attempt){observations_.clear();pump_receipts();AudioCommandV34 stop;stop.kind=AudioCommandKindV34::stop_all;const bool posted=mixer_.post(stop);mixer_.render(scratch,1);pump_receipts();if(posted&&!mixer_.active_voices()&&!bank_.pinned_samples()){voices_.clear();error.clear();return true;}}
 error="Required final audio receipt/stop completion; keep runtime alive";return false;
}
bool AudioGameplayRuntimeV42::native_state(std::uint64_t token,const char*name,std::string&error){
 const auto*state=voice(token);if(!state||!state->started){error="Required actual started native music voice";return false;}
 return native_state_owned_v101(token,name,error);
}
bool AudioGameplayRuntimeV42::first_source_channel_v101(int uid,std::uint64_t&token,std::string&error){
 if(!initialized_||quiescing_){error="Required active source channel engine";return false;}
 if(observations_.size()>=1024){error="Required producer receipt drain before source channel query";return false;}
 pump_receipts();token=0;
 for(const auto&entry:voices_)if(entry.second.xml_uid==uid){token=entry.first;break;}
 error.clear();return true;
}
bool AudioGameplayRuntimeV42::native_state_owned_v101(std::uint64_t token,const char*name,std::string&error){
 const auto*state=voice(token);const auto sample=state?bank_.source_slot_sample_v101(state->xml_uid):nullptr;
 if(!initialized_||quiescing_||!state||!name||!sample){error="Required actual owned music channel/sample/state";return false;}
 if(!sample->native){error.clear();return true;} // Original PCM channel has no native cursor.
 int index=-1;for(unsigned i=0;i<sample->states.size();++i)if(sample->states[i].name==name){if(index>=0){error="Ambiguous original music state";return false;}index=int(i);}
 if(index<0){error="Required exact authored native state name";return false;}
 AudioCommandV34 command;command.kind=AudioCommandKindV34::native_state;command.token=token;command.native_state=index;
 if(!mixer_.post(command)){error="Source music control queue full";return false;}
 error.clear();return true;
}
int AudioGameplayRuntimeV42::source_operation_v101(const dh2::sound::VoxPlay3DRequestV2&q,
 dh2::sound::VoxPlay3DResponseV2&out,std::int64_t event,std::string&error){
 using O=dh2::sound::VoxPlay3DOperationV2;
 if(!initialized_||quiescing_||in_submit_||!q.play||q.play->manager!=manager_){error="Required SAME nonreentrant source Vox operation receiver";return -1;}
 const bool emits=q.operation==O::emit||q.operation==O::native_play;
 if(emits&&event<=0){error="Required authored source time on reached Item/audio emission";return -1;}
 in_submit_=true;event_ns_=emits?event:0;error_.clear();
 struct Exit{bool&busy;std::int64_t&time;~Exit(){busy=false;time=0;}}exit{in_submit_,event_ns_};
 const auto status=invoke(this,q,out);
 error=status?(error_.empty()?"Required actual source Vox operation":error_):std::string{};return status;
}
}

namespace dh2::audio {
bool AudioGameplayRuntimeV42::source_dynamic_bus_routing_v94(const char* path,std::string& error){
 if(!initialized_||quiescing_||!path){error="Required SAME live process engine dynamic routing call";return false;}
 // Original Android/Callback driver virtual30 -> DriverInterface88ed50 BXLR.
 error.clear();return true;
}
}
