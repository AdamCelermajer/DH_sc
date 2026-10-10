#include "../original_campaign_runtime.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool x,const std::string&e){if(!x)throw std::runtime_error(e);}
int main(int argc,char**argv){try{
 check(argc==2,"Supply original-campaign asset directory");AssetCatalog assets(argv[1]);OriginalCampaignRuntime runtime;std::string error;
 check(runtime.load(assets,"original-campaign.xml",error),error);check(runtime.scripts().size()==70,"Original common/level source count");
 std::vector<std::string>order,spawn;std::vector<int>modules;unsigned starts=0;OriginalCampaignServices services;
 services.admit_start=[&](int,int,bool,bool&allowed,std::string&){++starts;allowed=true;return true;};
 services.command=[&](CampaignCommandPhase phase,const OriginalCampaignCommand&c,int module,bool&block,std::string&e){
  if(phase==CampaignCommandPhase::update){e="Fixture must not receive a nonblocking update";return false;}
  block=false;if(phase==CampaignCommandPhase::is_blocking)return true;
  // Explicit fake service boundary for the real first encounter command kinds.
  switch(c.kind){case 1:case 2:case 8:case 10:case 12:case 22:case 23:case 24:case 25:case 31:case 32:case 39:case 69:case 70:case 78:case 79:case 30:break;default:e="Unsupported fixture command "+c.class_name;return false;}
  order.push_back(c.class_name);modules.push_back(module);if(c.kind==30)spawn.push_back(c.strings.at(12));return true;
 };runtime.bind(services);
 std::string key;for(auto&t:runtime.triggers()){auto i=t.second.attributes.find("script");if(i!=t.second.attributes.end()&&i->second=="LizardMan_Intro")key=t.first;}
 check(!key.empty(),"Original encounter trigger absent");auto id=runtime.script_id("LizardMan_Intro",false);
 check(runtime.trigger_contact(key,true,false,7,error)&&!runtime.running(id),"Unqualified trigger started");
 check(runtime.trigger_contact(key,true,true,7,error)&&runtime.running(id),error);
 check(runtime.tick(0,error),error);check(order.empty(),"Lower common script must run next pass");
 check(runtime.tick(0,error),error);check(order.size()>=8&&order.front()=="Script_BlockSaveGame"&&spawn.empty(),"Begin scripted cutscene callback order");
 check(runtime.tick(499,error)&&spawn.empty(),error);check(runtime.tick(1,error)&&spawn.empty(),"Wait completed during Update instead of next call");
 check(runtime.tick(0,error),error);check(spawn.size()==1&&spawn[0]=="_prim_Monster_LizManIntro1","First timed original spawn");
 check(runtime.tick(1500,error)&&spawn.size()==1,"Second spawn fired in same threshold update");
 check(runtime.tick(0,error),error);check(spawn.size()==2&&spawn[1]=="_prim_Monster_LizManIntro2","Second timed original spawn");
 check(runtime.tick(2000,error)&&runtime.running(id),error);check(runtime.tick(0,error)&&runtime.running(id),error);
 check(runtime.tick(0,error)&&!runtime.running(id),error);check(order.back()=="Script_DoTutorial","Original tutorial continuation missing");
 check(std::find(order.begin(),order.end(),"Script_UnlockCharacter")!=order.end(),"Original unlock callback missing");
 for(int module:modules)check(module==7,"Nested scripts lost module context");auto count=starts;
 check(runtime.trigger_contact(key,false,true,7,error)&&runtime.trigger_contact(key,true,true,7,error)&&starts==count,"Single-fire trigger restarted");
 check(!runtime.tick(-1,error)&&!runtime.failed(),"Bad dt should reject without poisoning source state");
 OriginalCampaignRuntime unsupported;check(unsupported.load(assets,"original-campaign.xml",error),error);OriginalCampaignServices bad;bad.admit_start=services.admit_start;
 bad.command=[](CampaignCommandPhase,const OriginalCampaignCommand&,int,bool&,std::string&e){e="Deliberate unbound original callback";return false;};unsupported.bind(bad);
 check(unsupported.start(1,7,true,error),error);check(!unsupported.tick(0,error)&&unsupported.failed()&&error=="Deliberate unbound original callback","Unsupported command swallowed");
 std::cout<<"Original campaign runtime tests passed scripts=70 sourceSpawns=2 timerThresholdNextCall=true callbacks="<<order.size()<<"\n";
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
