#include "door_commands_v1.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::doors;
namespace {
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
}
int main(int argc,char** argv){try{
 check(argc==2,"asset root required");AssetCatalog assets(argv[1]);
 containers::ContainerTablesV1 tables;std::string error;std::vector<std::string> notices;
 check(tables.load(assets,error),error);
 ActorDefinition definition;definition.stableId=101;definition.name="source-red-door-fixture";
 definition.gametype="Door";definition.properties["data"]="Red_Door";
 definition.properties["door_collision"]="0";definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
 DoorRuntimeV1 runtime;check(runtime.load(tables,{definition},notices,error),error);
 check(runtime.size()==1,"Red_Door source table row");check(runtime.adopt(assets,notices,error),error);
 auto views=runtime.views();check(views[0].visual,"red door visual");auto& visual=*views[0].visual;
 check(std::string(visual.active_clip_name())=="idleclosed","initial closed clip");
 DoorCommandsV1 commands(runtime);OriginalCampaignCommand open{},close{},missing{};
 open.kind=54;open.strings[12]=definition.name;open.scalars[20]=1;
 close.kind=55;close.strings[12]=definition.name;close.scalars[20]=1;
 missing=open;missing.strings[12]="absent-door";
 bool handled=false,blocking=false;
 auto execute=[&](OriginalCampaignCommand& command,bool skip=false){check(commands.command(CampaignCommandPhase::execute,command,-1,skip,handled,blocking,error),error);check(handled,"door router admission");};
 execute(missing);check(!runtime.busy(0)&&runtime.state(0)==DoorStateV1::closed,"missing name mutates door");
 execute(open);check(runtime.busy(0)&&runtime.state(0)==DoorStateV1::opening,"normal open admission");
 check(runtime.update(.5,error),error);
 check(runtime.state(0)==DoorStateV1::opened&&runtime.busy(0)&&!runtime.collision_closed(0),"opened marker applies filter before clip end");
 const auto clock=visual.animation_elapsed_seconds();execute(open);execute(close);
 check(visual.animation_elapsed_seconds()==clock&&std::string(visual.active_clip_name())=="open","busy duplicate/reverse command restarts clip");
 check(commands.command(CampaignCommandPhase::is_blocking,open,-1,false,handled,blocking,error)&&blocking,"wait command unblocks at marker");
 check(runtime.update(1,error),error);
 check(!runtime.busy(0)&&runtime.state(0)==DoorStateV1::opened,"open callback clears busy");
 check(std::string(visual.active_clip_name())=="open","marker-ended Open must hold its terminal clip, not IdleOpened");
 execute(close);check(runtime.state(0)==DoorStateV1::closing&&runtime.busy(0),"normal close admission");
 check(runtime.update(.8,error),error);
 check(runtime.state(0)==DoorStateV1::closed&&runtime.busy(0)&&runtime.collision_closed(0),"closed marker/filter");
 const auto closeClock=visual.animation_elapsed_seconds();execute(open);execute(close);
 check(visual.animation_elapsed_seconds()==closeClock&&std::string(visual.active_clip_name())=="close","busy reverse/duplicate close restarts clip");
 check(runtime.update(.7,error),error);check(!runtime.busy(0)&&runtime.state(0)==DoorStateV1::closed,"close callback clears busy");
 check(std::string(visual.active_clip_name())=="close","marker-ended Close must hold its terminal clip, not IdleClosed");
 check(runtime.restore(0,DoorStateV1::closed,error),error);check(std::string(visual.active_clip_name())=="idleclosed","restore selects closed idle");
 execute(open,true);check(!runtime.busy(0)&&std::string(visual.active_clip_name())=="idleopened","immediate Open selects idle");
 execute(close,true);check(!runtime.busy(0)&&std::string(visual.active_clip_name())=="idleclosed","immediate Close selects idle");
 std::cout<<"PASS source door marker/callback clip lifecycle and duplicate gates\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
