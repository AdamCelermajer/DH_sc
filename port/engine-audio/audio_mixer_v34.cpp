#include "audio_mixer_v34.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace dh2::audio {
bool AudioMixerV34::configure_banks(const AudioBankV34*b,unsigned n)noexcept{if(n>banks_.size()||(!b&&n)||observed_voices_.load())return false;for(unsigned i=0;i<n;++i){if(b[i].max_playbacks<1||b[i].max_playbacks>64||b[i].behavior<0||b[i].behavior>3)return false;for(unsigned j=0;j<i;++j)if(b[i].id==b[j].id)return false;}std::copy_n(b,n,banks_.begin());bank_count_=n;return true;}
bool AudioMixerV34::set_rate(unsigned r)noexcept{if(r<8000||r>192000)return false;rate_=r;return true;}
bool AudioMixerV34::finish(Voice&v,AudioReceiptKindV34 kind)noexcept{
 v.active=false;v.retiring=true;v.retired_kind=kind;
 if(!receipts_.push({v.command.token,frame_,kind}))return false;
 v.retiring=false;v.command={};return true;
}
bool AudioMixerV34::bind_element(Voice&v,bool next)noexcept{
 const auto*s=v.command.sample;if(!s)return false;
 if(!s->native)return !next&&v.cursor.bind(s,0);
 if(v.command.native_state<0||unsigned(v.command.native_state)>=s->states.size())return false;
 const auto playlist=s->states[v.command.native_state].playlist;
 int ordinal=0;if(next){if(v.element<0)return false;ordinal=s->elements[v.element].source[1]+1;}
 int element=-1;for(unsigned i=0;i<s->elements.size();++i){const auto&row=s->elements[i].source;if(row[0]==int(playlist)&&row[1]==ordinal){if(element>=0)return false;element=int(i);}}
 if(element<0)return false;const auto&row=s->elements[element].source;
 // Every actual17 native file uses one sequential group per playlist,
 // implicit cues, unit element weight and once/infinite segment repeats.
 auto group=std::find_if(s->groups.begin(),s->groups.end(),[&](const auto&g){return g[0]==int(playlist)&&g[1]==row[2];});
 if(group==s->groups.end()||(*group)[2]!=0||(*group)[3]!=0||(*group)[4]!=1||(*group)[5]!=-1||row[4]||row[5]||(row[6]!=1&&row[6]!=-1)||row[7]!=1||row[3]<0)return false;
 if(!v.cursor.bind(s,unsigned(row[3])))return false;v.element=element;v.repeats=0;return true;
}
bool AudioMixerV34::apply(const AudioCommandV34&c)noexcept{
 if(c.kind==AudioCommandKindV34::configure_bank){
  const auto&bank=c.source_bank;
  if(bank.max_playbacks<1||bank.max_playbacks>64||bank.behavior<0||bank.behavior>3)return true;
  for(unsigned i=0;i<bank_count_;++i)if(banks_[i].id==bank.id){banks_[i]=bank;return true;}
  if(bank_count_<banks_.size())banks_[bank_count_++]=bank;return true;
 }
 if(c.kind==AudioCommandKindV34::play){
  if(!c.token||!c.sample||!std::isfinite(c.left)||!std::isfinite(c.right)||!std::isfinite(c.pitch)||c.left<0||c.right<0||c.pitch<=0||c.pitch>8||c.volume_group<0||c.volume_group>3){return receipts_.push({c.token,frame_,AudioReceiptKindV34::malformed});}
  Voice*slot=nullptr,*victim=nullptr;unsigned count=0;
  for(auto&v:voices_){if(!v.active&&!v.retiring&&!slot)slot=&v;if(v.active&&v.command.token==c.token)return receipts_.push({c.token,frame_,AudioReceiptKindV34::rejected});if(v.active&&v.command.bank==c.bank)++count;}
  const auto bank=std::find_if(banks_.begin(),banks_.begin()+bank_count_,[&](const auto&b){return b.id==c.bank;});
  if(bank==banks_.begin()+bank_count_||c.priority<bank->minimum_priority)return receipts_.push({c.token,frame_,AudioReceiptKindV34::rejected});
  if(count>=unsigned(bank->max_playbacks)){
   if(bank->behavior==3)return receipts_.push({c.token,frame_,AudioReceiptKindV34::rejected});
   for(auto&v:voices_)if(v.active&&v.command.bank==c.bank){
    if(bank->behavior==0){if(!victim||v.command.start_frame<victim->command.start_frame)victim=&v;}
    else if(c.priority>v.command.priority||(bank->behavior==2&&c.priority==v.command.priority)){
     if(!victim||v.command.priority<victim->command.priority||(v.command.priority==victim->command.priority&&v.command.start_frame<victim->command.start_frame))victim=&v;
    }
   }
   if(!victim)return receipts_.push({c.token,frame_,AudioReceiptKindV34::rejected});
   if(!finish(*victim,AudioReceiptKindV34::stolen))return false;slot=victim;
  }
  if(!slot)return receipts_.push({c.token,frame_,AudioReceiptKindV34::rejected});
  slot->command=c;slot->element=-1;slot->repeats=0;slot->position=0;slot->paused=false;slot->stop_after_fade=false;slot->started=false;slot->native_transition=false;slot->current_envelope={};slot->old_envelope={};slot->gain=c.fade_frames?0.f:1.f;slot->fade_target=1;slot->fade_remaining=c.fade_frames;slot->fade_step=c.fade_frames?1.f/float(c.fade_frames):0.f;
  if(!bind_element(*slot,false)){slot->command={};return receipts_.push({c.token,frame_,AudioReceiptKindV34::malformed});}
  slot->active=true;return true;
 }
 if(c.kind==AudioCommandKindV34::pause_all){paused_=true;return true;}
 if(c.kind==AudioCommandKindV34::resume_all){paused_=false;return true;}
 if(c.kind==AudioCommandKindV34::volume){if(c.volume_group<0||c.volume_group>3||!std::isfinite(c.left)||c.left<0||c.left>1)return true;
  const auto group=unsigned(c.volume_group);if(group_target_[group]==c.left)return true;
  if(!std::isfinite(c.source_fade_seconds)||c.source_fade_seconds<0)return true;
  const auto requested_frames=double(c.source_fade_seconds)*double(rate_);
  if(requested_frames>double(UINT32_MAX))return true;
  const auto frames=c.source_fade_seconds>0?std::uint32_t(requested_frames):c.fade_frames;
  group_target_[group]=c.left;group_remaining_[group]=frames;
  if(frames)group_step_[group]=(c.left-group_volume_[group])/float(frames);
  else group_volume_[group]=c.left;return true;}
 for(auto&v:voices_)if(v.active&&(c.kind==AudioCommandKindV34::stop_all||v.command.token==c.token)){
  if(c.kind==AudioCommandKindV34::stop||c.kind==AudioCommandKindV34::stop_all){
   // EmitterObj.Stop86e888 only creates a fade for current playing state1.
   // Nonplaying/paused channels retire immediately. Requested stopping state3
   // compares the new duration with the remaining envelope: shorten, never
   // extend. Stop0 still retires immediately during close/join/drain.
   if(!std::isfinite(c.source_fade_seconds)||c.source_fade_seconds<0)return true;
   const auto requested=double(c.source_fade_seconds)*double(rate_);if(requested>double(UINT32_MAX))return true;
   const auto frames=c.source_fade_seconds>0?std::uint32_t(requested):c.fade_frames;
   if(v.paused||!v.started){if(!finish(v,AudioReceiptKindV34::stopped))return false;continue;}
   if(v.stop_after_fade&&frames>=v.fade_remaining)continue;
   if(!frames){if(!finish(v,AudioReceiptKindV34::stopped))return false;}
   else{v.fade_remaining=frames;v.fade_target=0;v.fade_step=-v.gain/float(frames);v.stop_after_fade=true;}
  }
  else if(c.kind==AudioCommandKindV34::pause_voice)v.paused=true;
  else if(c.kind==AudioCommandKindV34::resume_voice&&v.paused&&!v.stop_after_fade){
   v.paused=false;
   // Source EmitterObj.Resume reaches its envelope only from paused state2.
   if(c.fade_frames){v.fade_target=1;v.fade_remaining=c.fade_frames;v.fade_step=(1.f-v.gain)/float(c.fade_frames);}
   else{v.gain=1;v.fade_remaining=0;}
  }
  else if(c.kind==AudioCommandKindV34::gains&&std::isfinite(c.left)&&std::isfinite(c.right)&&std::isfinite(c.pitch)&&c.left>=0&&c.right>=0&&c.pitch>0&&c.pitch<=8){v.command.left=c.left;v.command.right=c.right;v.command.pitch=c.pitch;}
  else if(c.kind==AudioCommandKindV34::native_state){
   const auto*s=v.command.sample;if(!s||!s->native||c.native_state<0||unsigned(c.native_state)>=s->states.size())return receipts_.push({c.token,frame_,AudioReceiptKindV34::control_required});
   if(c.native_state==v.command.native_state)return true;
   // Reached two-state source domain: phase-transposed playlists and exact
   // symmetric native Q30 fades. The extra dying-segment branch stays required.
   if(v.native_transition)return receipts_.push({c.token,frame_,AudioReceiptKindV34::control_required});
   const auto transition=std::find_if(s->transitions.begin(),s->transitions.end(),[&](const auto&t){return t.from==v.command.native_state&&t.to==c.native_state;});
   if(transition==s->transitions.end()||transition->rule<0||unsigned(transition->rule)>=s->rules.size())return receipts_.push({c.token,frame_,AudioReceiptKindV34::control_required});
   const auto&rule=s->rules[transition->rule];float fade_in,fade_out,total;std::memcpy(&fade_in,&rule[4],4);std::memcpy(&total,&rule[5],4);std::memcpy(&fade_out,&rule[6],4);
   if(rule[0]!=1||rule[1]||rule[2]||rule[3]||rule[7]||rule[8]!=0xffffffffu||transition->cue||fade_in!=total||fade_out!=total||!std::isfinite(total)||total<=0||total>60)return receipts_.push({c.token,frame_,AudioReceiptKindV34::control_required});
   const int length=int(total*float(s->rate));if(length<=0)return receipts_.push({c.token,frame_,AudioReceiptKindV34::control_required});const auto old_cursor=v.cursor;const auto old_position=v.position;const auto old_state=v.command.native_state;const auto old_element=v.element;const auto old_repeats=v.repeats;
   v.command.native_state=c.native_state;if(!bind_element(v,false)||v.cursor.frames()!=old_cursor.frames()){v.command.native_state=old_state;v.cursor=old_cursor;v.element=old_element;v.repeats=old_repeats;return receipts_.push({c.token,frame_,AudioReceiptKindV34::control_required});}
   v.old_cursor=old_cursor;v.old_position=old_position;v.position=old_position;v.current_envelope={0,length,1073741824/length,0,false};v.old_envelope={0,length,-(1073741824/length),1073741824,false};v.native_transition=true;
  }
 }
 return true;
}
void AudioMixerV34::render(float*out,unsigned frames)noexcept{
 if(!out)return;std::fill_n(out,std::size_t(frames)*2,0.f);
 for(auto&v:voices_)if(v.retiring)finish(v,v.retired_kind);
 for(unsigned i=0;i<frames;++i,++frame_){
  for(unsigned n=0;n<512;++n){const auto*c=commands_.front();if(!c)break;if(!apply(*c))break;commands_.pop();}
  if(paused_)continue;
  for(unsigned group=0;group<group_volume_.size();++group)if(group_remaining_[group]){
   group_volume_[group]+=group_step_[group];if(!--group_remaining_[group])group_volume_[group]=group_target_[group];
  }
  for(auto&v:voices_)if(v.active&&!v.paused&&v.command.start_frame<=frame_){
   if(!v.started){if(!receipts_.push({v.command.token,frame_,AudioReceiptKindV34::started}))continue;v.started=true;}
   const auto*s=v.command.sample;
   while(v.position>=v.cursor.frames()&&v.active){
    const auto length=v.cursor.frames();if(!length){finish(v,AudioReceiptKindV34::malformed);break;}v.position-=length;
    if(s->native){const auto repeats=s->elements[v.element].source[6];if(repeats==-1)continue;if(++v.repeats<repeats)continue;if(!bind_element(v,true))finish(v,AudioReceiptKindV34::completed);}
    else if(!v.command.loop)finish(v,AudioReceiptKindV34::completed);
   }if(!v.active)continue;
   float l0,r0,l1,r1;const auto position=std::uint64_t(v.position);if(!v.cursor.frame(position,l0,r0)){finish(v,AudioReceiptKindV34::malformed);continue;}
   if(!v.cursor.frame(position+1,l1,r1)){l1=l0;r1=r0;}
   const float fraction=float(v.position-double(position)),gain=v.gain*group_volume_[v.command.volume_group];float left=l0+(l1-l0)*fraction,right=r0+(r1-r0)*fraction;
   if(v.native_transition){
    float old_left,old_right;while(v.old_position>=v.old_cursor.frames())v.old_position-=v.old_cursor.frames();if(!v.old_cursor.frame(std::uint64_t(v.old_position),old_left,old_right)){finish(v,AudioReceiptKindV34::malformed);continue;}
    const std::int16_t current[2]{std::int16_t(l0*32768.f),std::int16_t(r0*32768.f)},previous[2]{std::int16_t(old_left*32768.f),std::int16_t(old_right*32768.f)};std::int32_t blended[2]{};
    auto current_envelope=v.current_envelope,old_envelope=v.old_envelope;audio_native_envelope_mix_v34(current,1,2,blended,current_envelope);audio_native_envelope_mix_v34(previous,1,2,blended,old_envelope);left=float(std::clamp(blended[0],-32768,32767))/32768.f;right=float(std::clamp(blended[1],-32768,32767))/32768.f;
    const double advance=double(s->rate)*double(v.command.pitch)/double(rate_);const unsigned source_frames=unsigned(std::floor(v.position+advance)-std::floor(v.position));for(unsigned n=0;n<source_frames;++n){std::int16_t silence[2]{};std::int32_t discarded[2]{};audio_native_envelope_mix_v34(silence,1,2,discarded,v.current_envelope);audio_native_envelope_mix_v34(silence,1,2,discarded,v.old_envelope);}v.old_position+=advance;if(v.old_envelope.stopped)v.native_transition=false;
   }
   out[2*i]+=left*v.command.left*gain;out[2*i+1]+=right*v.command.right*gain;
   v.position+=double(s->rate)*double(v.command.pitch)/double(rate_);
   if(v.fade_remaining){v.gain+=v.fade_step;if(!--v.fade_remaining){v.gain=v.fade_target;if(v.stop_after_fade)finish(v,AudioReceiptKindV34::stopped);}}
  }
  out[2*i]=std::clamp(out[2*i],-1.f,1.f);out[2*i+1]=std::clamp(out[2*i+1],-1.f,1.f);
 }
 unsigned active=0;for(const auto&v:voices_)active+=v.active;observed_voices_.store(active,std::memory_order_release);observed_frame_.store(frame_,std::memory_order_release);
}
}

