#include "source_root_scopes.hpp"
#include "source_world_objects.hpp"
#include "asset_catalog.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
int main(int argc,char**argv){try{
 check(argc==2,"Supply repository root");AssetCatalog assets(std::filesystem::path(argv[1])/"port/windows-foundation/assets");
 const std::string uri="original-cache/data/scene/001_swamp.mlx";std::vector<ActorDefinition>definitions;std::string error;
 check(load_actor_definitions(assets,uri,definitions,error),error);
 SourceRootScopes unbound;check(!unbound.debug_load(error),"Unbound standalone Debug load accepted");
 SourceWorldObjects objects;check(objects.load(definitions,error),error);SourceRootScopes root;
 check(root.build(assets,uri,definitions,objects,error),error);
 check(root.status().step==SourceLevelConstructionStep::complete&&root.status().attempts==10&&root.count()==9&&root.module_records().size()==9,"Actual root constructors differ");
 check(root.context().moduleId==-1&&root.context().moduleOffset==std::array<float,3>{0,0,0}&&root.context().onlineByte5==0,"In-house root/offline context changed");
 bool tracingBefore=false,tracingAfter=false;
 check(root.debug_switch("isTracingPlayersCollision",tracingBefore,error),error);
 check(root.debug_load(error)&&root.debug_load(error),error);
 check(root.debug_switch("isTracingPlayersCollision",tracingAfter,error)&&tracingBefore==tracingAfter&&root.count()==9,"Standalone load changed source query or scope identity");
 unsigned customQueries=0;SourceRootScopeOptions custom;custom.debugOwner=std::make_shared<int>(1);
 custom.debugSwitch=[&](const char*,bool& value,std::string& e){++customQueries;value=false;e.clear();return true;};
 SourceRootScopes customRoot;check(customRoot.load(assets,uri,definitions,error,custom),error);
 const auto priorQueries=customQueries;
 check(!customRoot.debug_load(error)&&customQueries==priorQueries,"Query-only custom Debug fabricated standalone load");
 ActorId id=0;bool found=false;
 const auto room=root.module_records().at(1)->receiver->module_id();
 check(objects.named_object("_prim_TriggerZone_LizManIntro",room,id,found,error)&&found,"Actual module room could not resolve source intro trigger");
 check(objects.load(definitions,error),error);
 check(!objects.named_object("_prim_TriggerZone_LizManIntro",room,id,found,error),"Reload unexpectedly retained scopes");
 check(root.bind(objects,error),error);
 check(objects.named_object("_prim_TriggerZone_LizManIntro",room,id,found,error)&&found,"Rebinding did not restore constructor-derived room");
 check(root.count()==9&&!root.load(assets,uri,definitions,error),"Rebind replayed constructors or construction replay accepted");
 SourceRootScopeOptions release;release.fileOwner=std::make_shared<int>(1);
 release.file.release_load_state=[](std::string&e){e="explicit host load-state release failure";return false;};
 SourceRootScopes failed;check(!failed.load(assets,uri,definitions,error,release)&&error.find("explicit host load-state")!=std::string::npos,"Reached release callback failure swallowed");
 check(failed.count()==9&&failed.status().step==SourceLevelConstructionStep::failed,"Failed actual source prefix not retained");
 check(!failed.bind(objects,error),"Failed constructor traversal published scopes");
 const auto first=root.modules().entries().front();auto ambiguous=definitions;
 bool copied=false;for(const auto&d:definitions)if(d.moduleName==first.occurrence.hostOccurrence){auto duplicate=d;duplicate.stableId^=0x100000000ull;duplicate.moduleName=first.occurrence.hostOccurrence+"_explicit_other";ambiguous.push_back(std::move(duplicate));copied=true;break;}
 check(copied,"Source module occurrence fixture missing");
 SourceRootScopes blocked;check(!blocked.load(assets,uri,ambiguous,error)&&error.find("explicit unique")!=std::string::npos,"Ambiguous declaration occurrence guessed an ID");
 SourceRootScopeOptions explicitOccurrence;explicitOccurrence.hostOccurrenceByElement[first.occurrence.element]=first.occurrence.hostOccurrence;
 SourceRootScopes explicitRoot;check(explicitRoot.load(assets,uri,ambiguous,error,explicitOccurrence),error);
 check(explicitRoot.count()==9,"Explicit opaque occurrence changed actual module IDs");
 std::cout<<"PASS actual root config+9 constructors, owned offline context, source room lookup, reload rebind, honest failed prefix, explicit ambiguous occurrence\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
