#include "original_campaign_world_adapter.hpp"
#include <cstring>
#include <exception>
#include <stdexcept>
namespace dh::foundation {namespace {
std::uint32_t scalar(const OriginalCampaignCommand&c,unsigned offset){auto i=c.scalars.find(offset);if(i==c.scalars.end())throw std::runtime_error("Missing source command scalar "+std::to_string(offset));return i->second;}
const std::string&text(const OriginalCampaignCommand&c,unsigned offset){auto i=c.strings.find(offset);if(i==c.strings.end())throw std::runtime_error("Missing source command string "+std::to_string(offset));return i->second;}
std::int32_t as_signed(std::uint32_t b){std::int32_t v;std::memcpy(&v,&b,4);return v;}
bool all_case_insensitive(const std::string&s){return s.size()==3&&(s[0]=='A'||s[0]=='a')&&(s[1]=='L'||s[1]=='l')&&(s[2]=='L'||s[2]=='l');}
template<class F,class...Args>bool effect(const F&fn,const char*name,std::string&e,Args&&...args){if(!fn){e=std::string("Unbound original campaign provider: ")+name;return false;}if(fn(std::forward<Args>(args)...,e))return true;if(e.empty())e=std::string("Original campaign provider failed: ")+name;return false;}
}
OriginalCampaignWorldAdapter::OriginalCampaignWorldAdapter(OriginalActorLifecycle&l,CampaignCameraAdapter*c):lifecycle_(&l),camera_(c){}
bool OriginalCampaignWorldAdapter::command(CampaignCommandPhase phase,const OriginalCampaignCommand&c,int module,bool skip,bool&blocking,std::string&error){
 blocking=false;error.clear();try{
  if(c.kind==4||c.kind==8){if(!camera_){if(c.kind==4)return true;error="Source camera command requires bound CampaignCameraAdapter";return false;}bool handled=false;if(!camera_->command(phase,c,skip,handled,blocking,error))return false;if(!handled){error="Source camera adapter did not handle command";return false;}return true;}
  if(c.kind==10||c.kind==12)return effect(providers_.dialog,"dialog",error,c,phase,blocking);
  if(c.kind==5)return effect(providers_.camera_clip,"camera clip",error,c,phase,blocking); // P16 CINE2
  if(c.kind==41||c.kind==42||c.kind==43||c.kind==45||c.kind==46||c.kind==29||c.kind==6||c.kind==19||c.kind==20||c.kind==21||c.kind==14||c.kind==51||c.kind==52){ // P16 OPENING
    if(!providers_.actor_verb){error="Unbound original campaign provider: actor verb";return false;}
    if(providers_.actor_verb(c,phase,module,blocking,error))return true;
    if(error.empty())error="Original campaign provider failed: actor verb";return false;}
  if(c.kind==22||c.kind==23){auto wait=scalar(c,20);if(wait>1){error="Invalid source Flash wait byte";return false;}return effect(providers_.flash,"flash",error,c.kind==22,text(c,16),scalar(c,8),wait!=0,phase,blocking);}
  switch(c.kind){case 1:case 2:case 3:case 24:case 25:case 30:case 31:case 32:case 39:case 69:case 70:case 77:case 78:case 79:case 27:case 28:case 16:case 17:break;default:error="Unsupported original campaign command kind "+std::to_string(c.kind)+" ("+c.class_name+")";return false;}
  // Actual source bodies for these kinds are nonblocking and inherit empty Update.
  if(phase!=CampaignCommandPhase::execute)return true;
  switch(c.kind){
  case 1:case 2:return effect(providers_.cutscene_mode,"cutscene mode",error,c.kind==1);
  case 24:case 25:{
   if(c.kind==24&&skip)return true;const auto&name=text(c,12);const bool global=c.kind==24?all_case_insensitive(name):name=="All";
   if(global)return effect(providers_.global_controller_blocked,"global controller block",error,c.kind==24);
   ActorId id=invalid_actor_id;bool found=false;if(!effect(providers_.named_character,"named character lookup",error,name,module,id,found))return false;
   if(!found)return true;if(id==invalid_actor_id){error="Original character lookup returned invalid identity";return false;}
   return effect(providers_.character_controller_blocked,"character controller block",error,id,c.kind==24);
  }
  case 30:{ActorId id=invalid_actor_id;bool found=false;if(!effect(providers_.named_character,"named character lookup",error,text(c,12),module,id,found))return false;if(!found)return true;if(id==invalid_actor_id){error="Original character lookup returned invalid identity";return false;}return lifecycle_->spawn(id,error);}
  case 31:case 32:case 39:{
   const auto&selector=text(c,c.kind==32?16:12);std::vector<ActorId>actors;if(!effect(providers_.character_selector,"source character selector",error,selector,module,actors))return false;
   if(actors.size()>65536){error="Source character selector exceeds bound";return false;}
   for(auto id:actors){if(id==invalid_actor_id){error="Source selector returned invalid actor";return false;}
    if(c.kind==32){auto scripted=scalar(c,8);if(scripted>1){error="Invalid source scripted byte";return false;}if(!effect(providers_.set_scripted,"mark character scripted",error,id,scripted!=0))return false;}
    else if(c.kind==39){if(!effect(providers_.stop_actor,"stop actor",error,id))return false;}
    else{bool allowed=false;if(!effect(providers_.idle_gate,"source Idle gate",error,id,allowed))return false;if(allowed&&!effect(providers_.set_idle,"source Idle state owner",error,id,!skip))return false;}
   }return true;
  }
  // P14 FAERY (T3): Script_SetFaeryState(slot @8, state @12) and Script_IncFaeryLevel(slot @8)
  // write the live CharacterState through the same-owner providers (unlock/level only).
  case 27:return effect(providers_.set_faery_state,"set faery state",error,scalar(c,8),scalar(c,12));
  case 28:return effect(providers_.inc_faery_level,"increment faery level",error,scalar(c,8));
  case 69:return effect(providers_.request_save,"request save",error);
  case 70:return effect(providers_.block_save,"block save",error);
  case 77:return effect(providers_.consume_tutorial,"lock tutorial",error,as_signed(scalar(c,8)));
  case 78:{
   const int tutorial=as_signed(scalar(c,16));if(tutorial<0){error="Source tutorial index outside native domain";return false;}OriginalTutorialGate gate;
   if(!effect(providers_.tutorial_gate,"source tutorial gate",error,tutorial,gate))return false;
   if(!gate.player_available||gate.difficulty!=0||!gate.enabled||gate.online)return true;
   if(!runtime_){error="Source tutorial continuation needs bound ScriptManager runtime";return false;}
   const int script=runtime_->script_id(text(c,12),true);
   if(script!=-1&&!runtime_->start(script,-1,true,error))return false;
   return effect(providers_.consume_tutorial,"consume source tutorial flag",error,tutorial)&&effect(providers_.save_tutorial_settings,"save source tutorial settings",error,gate.settings_menu_open);
  }
  case 79:return effect(providers_.flush_messages,"flush messages",error);
  // P17 SAFEZONE: Enter (16) / Leave (17) switch the level music to the safezone track and back.
  case 16:case 17:return effect(providers_.safe_zone,"safe zone",error,c.kind==16);
  case 3:return true; // OPENING2: Script_CONSOLE is a debug command; release builds do not show it (no-op, never blocks)
  default:error="Unsupported original campaign Execute";return false;
  }
 }catch(const std::exception&e){error=e.what();return false;}
}
}
