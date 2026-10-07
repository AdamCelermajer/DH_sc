"""Capture and stage active producer edits only; never mutate live sources."""
from pathlib import Path
import datetime,difflib,hashlib,json,subprocess
O=Path(__file__).resolve().parent;R=O.parents[3];C='port/android-native/app/src/main/cpp/'
old={};new={};raw={}
def source(p):
 if p not in old:raw[p]=(R/p).read_bytes();old[p]=raw[p].decode().replace('\r\n','\n');new[p]=old[p]
 return new[p]
def replace(p,a,b):
 t=source(p);assert t.count(a)==1,(p,a[:70],t.count(a));new[p]=t.replace(a,b)
def span(p,a,b,s):
 t=source(p);i=t.index(a);j=t.index(b,i);new[p]=t[:i]+s+t[j:]
model=C+'model_renderer.cpp';anim=C+'renderer_animation_sound_v4.inc';combat=C+'renderer_combat_sound_v2.inc';target=C+'renderer_player_target_frame_v2.inc'
replace(model,'#include "audio_named_animation_sound_v38.hpp"','#include "audio_named_animation_sound_v38.hpp"\n#include "integration-v42/events/audio_application_transport_v42.hpp"')
replace(model,'namespace model_renderer {','namespace model_renderer {\n// Focus/native owner provides this actual nullable process-global borrow.\nbool borrow_actual_application_audio_v42(dh2::audio::AudioApplicationBorrowV42&,std::string&);')
replace(model,'  dh2::audio::AudioSourceBindingsV38 audio_source_bindings_v38;','  dh2::audio::AudioSourceBindingsV38 audio_source_bindings_v38;\n  dh2::audio::AudioActualEventClockV42 audio_source_clock_v42; // REQUIRED actual authored event scope, never now()')
replace(model,'#include "renderer_combat_sound_v2.inc"','#include "integration-v42/events/renderer_application_audio_v42.inc"\n#include "renderer_combat_sound_v2.inc"')
span(anim,'static int source_animation_request_v38(','\nstatic int source_animation_sound_v4(','')
replace(anim,' if(!t.vox_music||!t.world||!t.world->player_object){t.error="Required same animation Vox/Character owner";return -1;}',' if(!t.world||!t.world->player_object){t.error="Required actual animation Character owner";return -1;}\n CapturedProducerAudioV42 captured{t,{}};if(!borrow_actual_application_audio_v42(captured.application,t.error))return -1;\n const auto captured_identity=captured.application.identity(); // BEFORE actual target position')
replace(anim,'source_animation_request_v38(t,dh2::audio::audio_world_request_v38(t.vox_music->identity(),0,id,','source_animation_request_v42(captured,dh2::audio::audio_world_request_v38(captured_identity,0,id,')
replace(anim,'dh2::audio::AudioNamedAnimationSoundServicesV38 services{};services.context=&t;','CapturedProducerAudioV42 captured{t,{}};\n dh2::audio::AudioNamedAnimationSoundServicesV38 services{};services.context=&captured;')
replace(anim,'services.manager=[](void* p,std::uintptr_t& out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);if(!r.vox_music)return -1;out=r.vox_music->identity();return 0;};','services.manager=[](void* p,std::uintptr_t& out){auto& c=*static_cast<CapturedProducerAudioV42*>(p);\n  if(!borrow_actual_application_audio_v42(c.application,c.t.error))return -1;out=c.application.identity();return 0;};')
replace(anim,'auto& r=*static_cast<PlayerSkillsRuntime*>(p);const float* position=nullptr;','auto& r=static_cast<CapturedProducerAudioV42*>(p)->t;const float* position=nullptr;')
replace(anim,'return source_animation_request_v38(*static_cast<PlayerSkillsRuntime*>(p),request);','return source_animation_request_v42(*static_cast<CapturedProducerAudioV42*>(p),request);')
# Combat selected row/minimal/dead/RNG prefixes stay in the original consumer.
replace(combat,' CombatSoundServicesV1 s{};s.context=&t;',' CapturedProducerAudioV42 captured{t,{}};\n CombatSoundServicesV1 s{};s.context=&captured;')
new[combat]=source(combat).replace('auto& r=*static_cast<PlayerSkillsRuntime*>(p);','auto& r=static_cast<CapturedProducerAudioV42*>(p)->t;')
span(combat,' s.manager=[]','\n s.random=[]',''' s.manager=[](void* p,std::uintptr_t* out){auto& c=*static_cast<CapturedProducerAudioV42*>(p);
  if(!out||!borrow_actual_application_audio_v42(c.application,c.t.error))return -1;*out=c.application.identity();return 0;};''')
span(combat,' s.play=[]','\n CombatSoundOutputV1 out{};',''' s.play=[](void* p,const CombatSoundPlayV1* play){if(!play)return -1;
  return source_animation_request_v42(*static_cast<CapturedProducerAudioV42*>(p),*play);};''')
# Generic coordinator captures global once before all source position callbacks.
replace(target,' bool committed{};',' bool committed{};dh2::audio::AudioApplicationBorrowV42 application;')
t=source(target);i=t.index('  dh2::sound::VoxPlay3DOwnerV2 owner(',t.index('case target_event_play_sound:'));j=t.index('\n }\n case target_event_active_dispatch:',i)
new[target]=t[:i]+'  return dh2::audio::audio_submit_application_v42(c.application,t.audio_source_clock_v42,play,t.error)?0:-1;'+t[j:]
replace(target,' PlayerTargetEventContextV2 context{*this,&state,{}};',' PlayerTargetEventContextV2 context{*this,&state,{},false,{}};\n if(!borrow_actual_application_audio_v42(context.application,error))return -1;')
replace(target,'unsigned(target_ai_sounds.size()),0,vox_music?vox_music->identity():0','unsigned(target_ai_sounds.size()),0,context.application.identity()')
# Add only the source-proved sfx_ authored attack/cast prefix.
replace(model,'static int source_named_animation_sound_v38(PlayerSkillsRuntime&,std::uintptr_t,const char*);','static int source_named_animation_sound_v38(PlayerSkillsRuntime&,std::uintptr_t,const char*);\nstatic int source_player_named_prefix_v42(PlayerSkillsRuntime&,const char*,bool&);')
replace(model,'#include "renderer_animation_sound_v4.inc"','#include "renderer_animation_sound_v4.inc"\n#include "integration-v42/events/renderer_player_named_prefix_v42.inc"')
replace(model,'void player_authored_event(const dh2::animation::TriggeredEvent& event,int clip){','void player_authored_event(const dh2::animation::TriggeredEvent& event,int clip){\n if(event.name&&!std::strncmp(event.name,"sfx_",4)){\n  if(!player_skills_runtime)throw std::runtime_error("Required actual player named-event owner");\n  bool handled=false;const int status=source_player_named_prefix_v42(*player_skills_runtime,event.name,handled);\n  if(!handled||status)throw std::runtime_error("Required source attack named sound: "+player_skills_runtime->error);return;\n }')
replace(model,'   if(prince_state.current==7&&player_skills_runtime&&player_skills_runtime->cast){\n    if(player_skills_runtime->cast->animation_event','   if(prince_state.current==7&&player_skills_runtime&&player_skills_runtime->cast){\n    bool handled=false;const int named_status=source_player_named_prefix_v42(*player_skills_runtime,static_cast<const char*>(event.handoff.payload),handled);\n    if(handled){if(named_status)throw std::runtime_error("Required source cast named sound: "+player_skills_runtime->error);return;}\n    if(player_skills_runtime->cast->animation_event')
entries=[];patch=[]
for p in sorted(old):
 for name,data in [('source-snapshot',raw[p]),('prospective',new[p].encode())]:
  dst=O/name/p;dst.parent.mkdir(parents=True,exist_ok=True);dst.write_bytes(data)
 patch.extend(difflib.unified_diff(old[p].splitlines(True),new[p].splitlines(True),fromfile='a/'+p,tofile='b/'+p))
 entries.append(dict(path=p,base_sha256=hashlib.sha256(raw[p]).hexdigest(),base_canonical_sha256=hashlib.sha256(old[p].encode()).hexdigest(),staged_sha256=hashlib.sha256(new[p].encode()).hexdigest()))
(O/'active-application-producers-v42.patch').write_text(''.join(patch),encoding='utf-8')
head=subprocess.run(['git','rev-parse','HEAD'],cwd=R,capture_output=True,text=True,check=True).stdout.strip()
(O/'staged-contract.json').write_text(json.dumps(dict(captured_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),git_head=head,scope='NEW staged active sources only; no shared edits or device/build execution',files=entries,changed_during_capture=[p for p in old if(R/p).read_bytes()!=raw[p]],required='actual event clock scope; focus/native borrow implementation; actual V42 gates/listener/source command providers'),indent=2)+'\n')
print('staged',len(entries),'active sources',head)
