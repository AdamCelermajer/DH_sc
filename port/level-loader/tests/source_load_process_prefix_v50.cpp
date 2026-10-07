#include "../source_load_process_prefix_v50.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::loader;
static unsigned checks;
static void ck(bool value,const char* why){++checks;if(!value)throw std::runtime_error(why);}
struct Fixture {
 std::shared_ptr<int> owner=std::make_shared<int>(0);
 std::uint32_t phase=7,other_phase=12;
 std::uint8_t cheat=0,map=9;
 std::uintptr_t character=77;
 bool current=true,display=false;
 unsigned failed_at=999,call_index=0;
 std::vector<std::string> trace;
 LoadPrefixServicesV50 service;
 bool step(std::string name,std::string& error){trace.push_back(std::move(name));if(call_index++==failed_at){error="injected source body failure";return false;}return true;}
 Fixture(){
  service.actual_application_owner=owner;service.cheat_global_owner=owner;service.debug_owner=owner;service.handle_cheats_inGame=&cheat;
  service.current_level=[this](auto& out,std::string& e){if(!step("current",e))return false;out=current?LoadPrefixLevelV50{owner,2,&other_phase}:LoadPrefixLevelV50{};return true;};
  service.local_player=[this](std::int32_t id,bool flag,auto& out,std::string& e){ck(id==0&&flag,"Source local player args");if(!step("player",e))return false;out={owner,3,&character};return true;};
  service.unlock_fast_travels=[this](auto id,std::string& e){ck(id==77,"Source character identity");return step("unlock",e);};
  service.world_map=[this](auto& out,std::string& e){if(!step("map",e))return false;out={owner,4,&map};return true;};
  service.debug_load=[this](std::string& e){return step("load",e);};
  service.debug_get_switch=[this](const char* key,bool& out,std::string& e){ck(std::string(key)=="IsDisplayLoadingStepName","Exact display key");if(!step("display",e))return false;out=display;return true;};
  service.source_step_name=[this](auto state,const char*& out,std::string& e){ck(state==phase,"Step name freshly reads source phase");if(!step("name",e))return false;out="original-pointer";return true;};
  service.menu_debug_set_text=[this](const char* text,std::string& e){ck(text&&std::string(text)=="original-pointer","Source text pointer");return step("text",e);};
  service.debug_out_loading_step=[this](auto state,std::string& e){ck(state==phase,"Logger freshly reads source phase");return step("log",e);};
  service.debug_instance_get_switch=[this](const char* key,bool& out,std::string& e){ck(std::string(key)=="isTracingLevel_Loading","Exact trace key");out=true;return step("instance_trace",e);};
 }
 LoadPrefixLevelV50 level(){return {owner,1,&phase};}
};
int main(){try{
 {Fixture f;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;
  ck(p.before_dispatch(99,e),"Flag0 source prefix");ck(f.trace==std::vector<std::string>({"load","display","log"}),"Flag0 must not touch current/PM/Map");ck(f.map==9&&f.phase==7,"No scalar suffix mutation");
  ck(p.before_dispatch(123,e),"Original per-call repeat");ck(f.trace.size()==6,"Prefix actually executes each call");}
 {Fixture f;f.cheat=255;f.display=true;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;
  ck(p.before_dispatch(99,e),"Noncanonical source flag is nonzero");ck(f.trace==std::vector<std::string>({"current","player","unlock","map","load","display","name","text","log"}),"Whole source order");ck(f.map==1,"Map byte source store");}
 {Fixture f;f.cheat=1;f.current=false;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;
  ck(p.before_dispatch(99,e),"Genuine empty global");ck(f.trace==std::vector<std::string>({"current","load","display","log"}),"Empty global skips PM");ck(f.map==9,"Empty global retains Map suffix");}
 {Fixture f;f.cheat=1;f.character=0;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;
  ck(p.before_dispatch(99,e),"Genuine null Character");ck(f.trace==std::vector<std::string>({"current","player","load","display","log"}),"Null Character skips Save/Map");}
 {Fixture f;f.display=true;f.service.debug_get_switch=[&](const char*,bool& out,std::string& e){f.phase=17;out=true;return f.step("display",e);};
  f.service.menu_debug_set_text=[&](const char* text,std::string& e){ck(text!=nullptr,"Step pointer passed through");f.phase=29;return f.step("text",e);};
  SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;ck(p.before_dispatch(99,e),"Fresh phase after Debug and SetText");ck(f.phase==29,"Original prefix must not replace source phase");}
 for(unsigned stage:{3u,16u,19u}){Fixture f;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;ck(p.debug_only_stage(stage,e),"Original tracing stage");ck(f.trace==std::vector<std::string>({"instance_trace"}),"Trace does not duplicate load");}
 for(unsigned fail=0;fail<9;++fail){Fixture f;f.cheat=1;f.display=true;f.failed_at=fail;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;
  ck(!p.before_dispatch(99,e),"Injected source error must stop");ck(f.trace.size()==fail+1,"Only completed source prefix reached");ck(f.map==(fail>=4?1:9),"Map store respects failure prefix");
  const auto receipt=e;ck(!p.before_dispatch(99,e)&&e==receipt&&f.trace.size()==fail+1,"Failure must not replay irreversible prefix");}
 {Fixture f;f.service.handle_cheats_inGame=nullptr;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;ck(!p.before_dispatch(99,e)&&f.trace.empty(),"Unavailable global is not false");}
 {Fixture f;f.cheat=1;f.service.local_player=[](auto,bool,auto& out,std::string&){out={};return true;};SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;
  ck(!p.before_dispatch(99,e)&&f.trace==std::vector<std::string>({"current"}),"Unavailable PlayerInfo is not null Character");}
 {Fixture f;f.cheat=1;f.service.world_map=[&](auto& out,std::string& e){out={f.owner,4,nullptr};return f.step("map",e);};SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;
  ck(!p.before_dispatch(99,e)&&f.map==9&&f.trace.size()==4,"Missing Map field must stop before store/log");}
 {Fixture f;f.service.debug_out_loading_step={};SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;ck(!p.before_dispatch(99,e)&&f.trace.size()==2,"Logger cannot be a success stub");}
 {Fixture f;SourceLoadProcessPrefixV50* live{};f.service.debug_load=[&](std::string& e){std::string inner;ck(!live->before_dispatch(99,inner),"Nested source call rejected");e.clear();return true;};
  SourceLoadProcessPrefixV50 p(f.level(),f.service);live=&p;std::string e;ck(!p.before_dispatch(99,e)&&f.trace.empty(),"Swallowed nested failure must latch");}
 {Fixture f;f.service.debug_load=[](std::string&)->bool{throw std::runtime_error("body exception");};SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;ck(!p.before_dispatch(99,e)&&e=="body exception","Source exceptions contained");}
 {Fixture f;SourceLoadProcessPrefixV50 p(f.level(),f.service);std::string e;ck(!p.debug_only_stage(4,e)&&f.trace.empty(),"Unrelated stage never enters debug-only body");}
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"scope\":\"source prefix envelope with explicit provider fixtures; not whole original native providers\"}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
