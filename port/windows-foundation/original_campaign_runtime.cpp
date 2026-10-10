#include "original_campaign_runtime.hpp"
#include "content_paths.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"
#include <cstring>
#include <stdexcept>
#include <utility>
namespace dh::foundation { namespace {
std::string attr(const TiXmlElement& n,const char* key){auto*p=n.Attribute(key);if(!p)throw std::runtime_error(std::string("Missing campaign field ")+key);return p;}
std::int64_t number(const std::string&s){std::size_t n=0;auto v=std::stoll(s,&n);if(n!=s.size())throw std::runtime_error("Invalid campaign integer");return v;}
std::uint32_t word(const OriginalCampaignCommand&c,unsigned o){auto i=c.scalars.find(o);if(i==c.scalars.end())throw std::runtime_error("Missing original command scalar "+std::to_string(o));return i->second;}
std::int32_t signed_bits(std::uint32_t b){std::int32_t v;std::memcpy(&v,&b,4);return v;}
int trigger_int(const OriginalCampaignTrigger&t,const char*k,int fallback){auto i=t.attributes.find(k);return i==t.attributes.end()?fallback:int(number(i->second));}
std::string trigger_text(const OriginalCampaignTrigger&t,const char*k){auto i=t.attributes.find(k);return i==t.attributes.end()?"":i->second;}
}
bool OriginalCampaignRuntime::fail(std::string message,std::string&error){failure_=message.empty()?"Original campaign callback failed":std::move(message);error=failure_;return false;}
bool OriginalCampaignRuntime::load(const AssetCatalog&assets,const std::string&path,std::string&error){
 try{
  for(const auto&c:contexts_)if(c.state!=2)throw std::runtime_error("Cannot reload active original campaign");
  auto raw=read_content(assets,path);if(raw.empty()||raw.size()>8*1024*1024)throw std::runtime_error("Campaign XML outside bounds");
  std::string text(raw.begin(),raw.end());if(text.find('\0')!=std::string::npos||text.find("<!")!=std::string::npos)throw std::runtime_error("Unsupported campaign XML declaration");
  TiXmlDocument doc;doc.Parse(text.c_str());if(doc.Error())throw std::runtime_error(doc.ErrorDesc());auto*r=doc.RootElement();
  if(!r||std::string(r->Value())!="originalCampaign"||attr(*r,"version")!="1")throw std::runtime_error("Expected originalCampaign version1");
  std::vector<OriginalCampaignScript> scripts;std::map<std::string,OriginalCampaignTrigger> triggers;std::size_t common=0;
  for(const char*scope:{"common","level"})for(auto*bank=r->FirstChildElement("scripts");bank;bank=bank->NextSiblingElement("scripts")){
   auto s=attr(*bank,"scope");if(s!="common"&&s!="level")throw std::runtime_error("Unknown campaign script scope");if(s!=scope)continue;
   std::size_t local=0;for(auto*sn=bank->FirstChildElement();sn;sn=sn->NextSiblingElement()){
    if(std::string(sn->Value())!="script"||number(attr(*sn,"id"))!=std::int64_t(local++))throw std::runtime_error("Campaign script IDs must retain source array order");
    OriginalCampaignScript script;script.id=int(scripts.size());script.scope=s;script.name=attr(*sn,"name");
    for(auto*cn=sn->FirstChildElement();cn;cn=cn->NextSiblingElement()){
     if(std::string(cn->Value())!="command"||number(attr(*cn,"index"))!=std::int64_t(script.commands.size()))throw std::runtime_error("Campaign command order differs");
     OriginalCampaignCommand c;c.kind=int(number(attr(*cn,"kind")));c.class_name=attr(*cn,"className");if(c.kind<0||c.kind>=80)throw std::runtime_error("Original command kind outside80 domain");
     for(auto*f=cn->FirstChildElement();f;f=f->NextSiblingElement()){
      auto o=number(attr(*f,"offset"));if(o<0||o>4096)throw std::runtime_error("Command struct offset outside bounds");std::string tag=f->Value();
      if(tag=="scalar"){auto v=number(attr(*f,"bits"));if(v<0||v>UINT32_MAX||!c.scalars.emplace(unsigned(o),std::uint32_t(v)).second)throw std::runtime_error("Invalid scalar command field");}
      else if(tag=="string"){if(!c.strings.emplace(unsigned(o),attr(*f,"value")).second)throw std::runtime_error("Duplicate string command field");}
      else if(tag=="array"){std::vector<std::uint32_t>values;for(auto*v=f->FirstChildElement();v;v=v->NextSiblingElement()){auto n=number(attr(*v,"bits"));if(std::string(v->Value())!="value"||n<0||n>UINT32_MAX||values.size()>=65536)throw std::runtime_error("Invalid command array");values.push_back(std::uint32_t(n));}if(!c.arrays.emplace(unsigned(o),std::move(values)).second)throw std::runtime_error("Duplicate command array");}
      else throw std::runtime_error("Unknown campaign command field");
     }script.commands.push_back(std::move(c));if(script.commands.size()>65536)throw std::runtime_error("Campaign command limit");
    }scripts.push_back(std::move(script));if(scripts.size()>8192)throw std::runtime_error("Campaign script limit");
   }if(s=="common")common=scripts.size();
  }
  if(auto*ts=r->FirstChildElement("sourceTriggers"))for(auto*t=ts->FirstChildElement();t;t=t->NextSiblingElement()){
   if(std::string(t->Value())!="trigger")throw std::runtime_error("Unknown trigger entry");OriginalCampaignTrigger value;
   for(auto*f=t->FirstChildElement();f;f=f->NextSiblingElement()){if(std::string(f->Value())!="attribute"||!value.attributes.emplace(attr(*f,"name"),attr(*f,"value")).second)throw std::runtime_error("Invalid trigger attribute");}
   auto name=value.attributes.find("name");if(name==value.attributes.end())throw std::runtime_error("Trigger missing source name");value.key=attr(*t,"source")+"::"+name->second;
   if(!triggers.emplace(value.key,std::move(value)).second)throw std::runtime_error("Duplicate source trigger key");
  }
  scripts_=std::move(scripts);triggers_=std::move(triggers);common_count_=common;contexts_.assign(scripts_.size(),{});trigger_state_.clear();failure_.clear();error.clear();return true;
 }catch(const std::exception&e){error=e.what();return false;}
}
int OriginalCampaignRuntime::script_id(const std::string&name,bool common)const{for(std::size_t i=common?0:common_count_;i<scripts_.size();++i)if(scripts_[i].name==name)return int(i);return -1;}
std::map<std::string,std::int32_t> OriginalCampaignRuntime::trigger_activations()const{
 std::map<std::string,std::int32_t> out;
 for(const auto& entry:trigger_state_) if(entry.second.activations) out[entry.first]=entry.second.activations;
 return out;
}
std::vector<std::string> OriginalCampaignRuntime::trigger_inside()const{
 std::vector<std::string> out;
 for(const auto& entry:trigger_state_) if(entry.second.inside) out.push_back(entry.first);
 return out;
}
void OriginalCampaignRuntime::restore_trigger_inside(const std::vector<std::string>& keys){
 for(const auto& key:keys) trigger_state_[key].inside=true;
}
void OriginalCampaignRuntime::restore_trigger_activations(const std::map<std::string,std::int32_t>& counts){
 for(auto& entry:trigger_state_) entry.second=TriggerState{};
 for(const auto& count:counts) trigger_state_[count.first].activations=count.second;
}
bool OriginalCampaignRuntime::running(int id)const{return id>=0&&std::size_t(id)<contexts_.size()&&contexts_[id].state!=2;}
bool OriginalCampaignRuntime::start(int id,int module,bool received,std::string&error){
 try {
 if(failed()){error=failure_;return false;}if(id<0||std::size_t(id)>=scripts_.size()){error.clear();return true;}
 if(!services_.admit_start)return fail("Original script start admission callback unavailable",error);bool admitted=false;
 if(!services_.admit_start(id,module,received,admitted,error))return fail(error,error);if(!admitted){error.clear();return true;}
 auto&c=contexts_[id];c={};c.state=0;c.module=module;error.clear();return true;
 } catch(const std::exception&e) { return fail(e.what(),error); }
}
bool OriginalCampaignRuntime::execute(std::size_t id,std::int32_t dt,std::string&error){
 auto&ctx=contexts_[id];auto&script=scripts_[id];std::size_t budget=0;
 while(ctx.state!=2){
  if(++budget>65536)return fail("Campaign per-script execution budget exhausted",error);
  if(ctx.command>=script.commands.size()){ctx.state=2;return true;}const auto&c=script.commands[ctx.command];
  if(ctx.state==0){
   if(c.kind==26){ctx.elapsed=0;ctx.duration=signed_bits(word(c,8));}
   else if(c.kind==0){auto chosen=word(c,12),count=word(c,16);if(count){auto a=c.arrays.find(20);if(a==c.arrays.end()||a->second.size()!=count||!services_.random)return fail("Source ExecScript random provider/array unavailable",error);std::uint32_t draw=0;if(!services_.random(count,draw,error)||draw>=count)return fail(error.empty()?"Source random draw outside bound":error,error);chosen=a->second[draw];}ctx.child=signed_bits(chosen);if(!word(c,8))ctx.child+=int(common_count_);if(!start(ctx.child,ctx.module,true,error))return false;}
   else{bool ignored=false;if(!services_.command||!services_.command(CampaignCommandPhase::execute,c,ctx.module,ignored,error))return fail(error.empty()?"Unsupported original command "+c.class_name:error,error);}
   ctx.state=1;
  }
  bool block=false;
  if(c.kind==26)block=ctx.elapsed<ctx.duration;
  else if(c.kind==0)block=word(c,24)!=0&&running(ctx.child);
  else if(!services_.command||!services_.command(CampaignCommandPhase::is_blocking,c,ctx.module,block,error))return fail(error.empty()?"Unsupported original blocking query "+c.class_name:error,error);
  if(block){if(c.kind==26)ctx.elapsed=signed_bits(std::uint32_t(ctx.elapsed)+std::uint32_t(dt));else if(c.kind!=0){bool ignored=false;if(!services_.command(CampaignCommandPhase::update,c,ctx.module,ignored,error))return fail(error,error);}return true;}
  ctx.state=0;++ctx.command;
 }return true;
}
bool OriginalCampaignRuntime::tick(std::int32_t dt,std::string&error){
 if(failed()){error=failure_;return false;}if(dt<0||ticking_){error="Invalid campaign dt or reentrant tick";return false;}
 ticking_=true;struct Guard{bool&b;~Guard(){b=false;}}guard{ticking_};
 try{for(auto&entry:trigger_state_)if(entry.second.delay>0)entry.second.delay=signed_bits(std::uint32_t(entry.second.delay)-std::uint32_t(dt));for(std::size_t i=0;i<contexts_.size();++i)if(!execute(i,dt,error))return false;error.clear();return true;}catch(const std::exception&e){return fail(e.what(),error);}
}
bool OriginalCampaignRuntime::register_trigger(const std::string&key,const std::map<std::string,std::string>&attributes,std::string&error){
 if(failed()){error=failure_;return false;}
 if(key.empty()||attributes.find("name")==attributes.end()){error="Trigger registration needs key and source name";return false;}
 if(triggers_.count(key)){error="Duplicate source trigger key";return false;}
 OriginalCampaignTrigger value;value.key=key;value.attributes=attributes;triggers_.emplace(key,std::move(value));error.clear();return true;
}
std::size_t OriginalCampaignRuntime::abandon_running_scripts(){
 std::size_t abandoned=0;
 for(auto&ctx:contexts_)if(ctx.state!=2){ctx={};++abandoned;}
 failure_.clear();ticking_=false;
 return abandoned;
}
bool OriginalCampaignRuntime::trigger_contact(const std::string&key,bool inside,bool qualified,int module,std::string&error){
 if(failed()){error=failure_;return false;}auto t=triggers_.find(key);if(t==triggers_.end()){error="Unknown source trigger key";return false;}if(!qualified){error.clear();return true;}
 const auto&source=t->second;if(trigger_text(source,"gametype")!="TriggerZone"){error="Source object is not TriggerZone";return false;}
 try{
  for(const char* field:{"script_all_player","script_all_player_move_out","effect_one_player","is_door_closed"})
   if(!trigger_text(source,field).empty())return fail(std::string("Original trigger feature requires additional source provider: ")+field,error);
  auto&s=trigger_state_[key];int count=trigger_int(source,"triggercount",1);if((count>=0&&s.activations>=count)||s.delay>0){error.clear();return true;}if(s.inside==inside){error.clear();return true;}
  auto name=trigger_text(source,inside?"script":"script_move_out");s.inside=inside;
  if(!name.empty()){int id=script_id(name,false);if(id<0)return fail("Source trigger script unresolved: "+name,error);if(!running(id)&&!start(id,module,false,error))return false;}
  if(inside){s.delay=trigger_int(source,"triggerdelay",0);if(trigger_text(source,"script_move_out").empty())++s.activations;}else if(!trigger_text(source,"script_move_out").empty())++s.activations;
  error.clear();return true;
 }catch(const std::exception&e){return fail(e.what(),error);}
}
}
