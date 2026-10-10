#include "level_script_paths_v51.hpp"
#include "level_lightset_stage9_v53.hpp"
#include "filename_root_source_v65.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace dh2::loader;
namespace {
unsigned checks;
void check(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
auto pin(){return std::make_shared<int>(0);}

void script_paths(){
 std::string ordinary="data\\PyData\\scripts\\001_SWAMP.pyscript";
 auto p=level_script_paths_v51(ordinary);
 check(p.reached&&ordinary=="data/PyData/scripts/001_SWAMP.pyscript","Reached path normalizes the actual CString");
 check(p.command_file=="data/PyData/scripts/001_SWAMP_pyscripts.bin"&&p.name_file=="data/PyData/scripts/001_SWAMP_pyscriptnames.bin","Ordinary authored paths unchanged");
 std::string tail="foo.pyscript1234567890";p=level_script_paths_v51(tail);
 check(p.command_file=="foo_pyscripts.bin"&&p.name_file=="foo_pyscriptnames.bin67890","Names replacement consumes only post-command suffix length14");
 std::string repeated="foo.pyscriptbar.pyscript1234567890";p=level_script_paths_v51(repeated);
 check(p.command_file=="foo.pyscriptbar_pyscripts.bin"&&p.name_file=="foo.pyscriptbar_pyscriptnames.bin67890","Reverse search chooses the last exact marker");
 std::string unreached="data\\Foo.PYSCRIPT";p=level_script_paths_v51(unreached);
 check(!p.reached&&unreached=="data\\Foo.PYSCRIPT","Unreached case-sensitive marker leaves actual backslashes intact");
 std::string marker=".pyscript";p=level_script_paths_v51(marker);
 check(p.reached&&p.name_file=="_pyscriptnames.bin","std::string clamps replacement count at original end");
}

struct Stage9Fixture {
 std::shared_ptr<int> owner=pin();std::uintptr_t config=reinterpret_cast<std::uintptr_t>(owner.get());
 std::string fixed="fixed.lightset_xml",regular="regular.lightset_xml";
 unsigned traces{},devices{},loads{};bool fixed_pipeline{};unsigned pending{};bool load_failure{};
 EarlyLoadingDebugV46 debug(){return {owner,[this](const char* key,bool& value,std::string& e){
  ++traces;check(std::string(key)=="isTracingLevel_Loading","Exact Stage9 original debug switch");value=false;e.clear();return true;}};}
 Stage9ConfigBorrowV53 fields(){return {owner,owner,&config,config,&fixed,&regular};}
 Stage9ServicesV53 services(){
  Stage9ServicesV53 s;s.actual_device_owner=owner;s.actual_file_loader_owner=owner;
  s.validate_current=[](std::string& e){e.clear();return true;};
  s.is_fixed_pipeline=[this](bool& result,std::string& e){++devices;check(traces==1,"Trace precedes Device selection");result=fixed_pipeline;e.clear();return true;};
  s.load_module_file_step=[this](const std::string& name,const char* tag,std::string& e){
   ++loads;check(name==(fixed_pipeline?fixed:regular)&&std::string(tag)=="Module","Selected source CString and exact Module tag");
   if(load_failure){e="nested factory leaf";return LifecycleStepV36::failed;}
   if(pending){--pending;e.clear();return LifecycleStepV36::pending;}e.clear();return LifecycleStepV36::complete;
  };return s;
 }
};
void stage9_trace(){
 std::string e;
 {Stage9Fixture f;f.regular.clear();Stage9TracePrefixV53 trace(f.debug());Stage9BodyV53 body(f.fields(),f.services());
  check(trace.step(e)&&body.step(e)==LifecycleStepV36::complete,"Empty light filename still executes trace and device");
  check(trace.step(e)&&body.step(e)==LifecycleStepV36::complete&&f.traces==1&&f.devices==1&&f.loads==0,"Completed occurrence does not replay trace/device/file");}
 {Stage9Fixture f;f.pending=2;Stage9TracePrefixV53 trace(f.debug());Stage9BodyV53 body(f.fields(),f.services());
  for(unsigned i=0;i<3;++i){check(trace.step(e),"Pending poll retains completed trace");check(body.step(e)==(i<2?LifecycleStepV36::pending:LifecycleStepV36::complete),"Only unfinished source file poll repeats");}
  check(f.traces==1&&f.devices==1&&f.loads==3,"Trace/Device prefixes run exactly once across pending polls");}
 {Stage9Fixture f;f.fixed_pipeline=true;Stage9TracePrefixV53 trace(f.debug());Stage9BodyV53 body(f.fields(),f.services());
  check(trace.step(e)&&body.step(e)==LifecycleStepV36::complete&&body.selected_filename()==f.fixed,"Fixed-pipeline source branch selects2d0");}
 {unsigned calls{};Stage9TracePrefixV53 trace({pin(),[&](const char*,bool&,std::string& e){++calls;e="debug leaf";return false;}});
  check(!trace.step(e)&&e=="debug leaf","Debug failure preserves original leaf");check(!trace.step(e)&&e=="debug leaf"&&calls==1,"Failed debug prefix cannot replay");}
 {unsigned calls{};Stage9TracePrefixV53 trace({pin(),[&](const char*,bool&,std::string&)->bool{++calls;throw std::runtime_error("debug exception");}});
  check(!trace.step(e)&&e=="debug exception","Debug exception is sticky");check(!trace.step(e)&&calls==1,"Throwing debug prefix cannot replay");}
 {Stage9TracePrefixV53* active{};Stage9TracePrefixV53 trace({pin(),[&](const char*,bool&,std::string& e){check(!active->step(e),"Recursive prefix rejected");return true;}});active=&trace;
  check(!trace.step(e)&&e=="Stage9 debug trace reentered","Outer callback cannot erase recursive failure");}
 {Stage9Fixture f;f.load_failure=true;Stage9TracePrefixV53 trace(f.debug());Stage9BodyV53 body(f.fields(),f.services());
  check(trace.step(e)&&body.step(e)==LifecycleStepV36::failed&&e=="nested factory leaf","Stage9 preserves actual factory error");
  check(body.step(e)==LifecycleStepV36::failed&&e=="nested factory leaf"&&f.loads==1,"Failed file prefix cannot replay");}
}

void filename_outcomes(){
 std::string e,uri;LevelRootFilenameTraceV52 trace;unsigned modes{},opens{};
 LevelRootFilenameServicesV52 s;s.actual_filesystem_owner=pin();
 s.is_using_uncompiled_data=[&](const std::string&,bool& value,std::string& e){++modes;value=true;e.clear();return true;};
 s.open_resource=[&](const std::string&,bool& found,std::string& uri,std::string& e){++opens;found=false;uri.clear();e.clear();return true;};
 LevelRootFilenameResolverV52 resolver(s);
 check(resolver.resolve_source("missing.lightset_xml",uri,&trace,e)==LevelFilenameResolutionV52::absent&&e.empty(),"All successful misses are typed absence");
 check(modes==4&&opens==8&&trace.open_resource_queries.size()==8,"All source raw/compiled attempts run before absence");
 check(!resolver.resolve("missing.lightset_xml",uri,nullptr,e)&&e=="Original Level.LoadFile filename attempts absent from actual filesystem","Legacy bool caller continues requiring a resource");
 s.open_resource=[&](const std::string&,bool& found,std::string& uri,std::string& e){found=false;uri.clear();e="open I/O failure";return false;};
 LevelRootFilenameResolverV52 io(s);check(io.resolve_source("missing.lightset_xml",uri,nullptr,e)==LevelFilenameResolutionV52::failed&&e=="open I/O failure","I/O failure is never absence");
 s.is_using_uncompiled_data=[](const std::string&,bool&,std::string& e){e="mode leaf";return false;};
 LevelRootFilenameResolverV52 mode(s);check(mode.resolve_source("missing.lightset_xml",uri,nullptr,e)==LevelFilenameResolutionV52::failed&&e=="mode leaf","Mode failure is never absence");
}

struct NativeFileFixture {
 std::shared_ptr<int> owner=pin();std::shared_ptr<CanonicalLevelContextV1> level;
 std::string payload="<Module/>";bool missing{},read_failure{},open_failure{},parser_failure{},factory_failure{};
 unsigned opens{},sizes{},reads{},closes{},parses{},consumes{};
 NativeFileFixture(){std::string e;check(CanonicalLevelContextV1::create({"TEST","test.mlx"},owner,level,e),"Actual native Level fixture creation");}
 FilenameSourceLeavesV65 leaves(){
  FilenameSourceLeavesV65 s;s.owner=owner;s.copy.owner=owner;
  s.is_using_uncompiled_data=[](const std::string&,bool& value,std::string& e){value=false;e.clear();return true;};
  s.open_resource=[this](const std::string& name,bool& found,std::string& uri,SourceIStreamBorrowV65& source,std::string& e){
   ++opens;found=false;uri.clear();source={};if(open_failure){e="open I/O failure";return false;}if(missing){e.clear();return true;}
   found=true;uri=name;source={owner,reinterpret_cast<std::uintptr_t>(owner.get())};e.clear();return true;};
  s.copy.size=[this](const SourceIStreamBorrowV65&,std::uint64_t& size,std::string& e){++sizes;size=payload.size();e.clear();return true;};
  s.copy.read=[this](const SourceIStreamBorrowV65&,void* out,std::uint32_t count,std::string& e){++reads;if(read_failure){e="read I/O failure";return false;}check(count==payload.size(),"Actual copied stream size");std::memcpy(out,payload.data(),count);e.clear();return true;};
  s.close_source=[this](SourceIStreamBorrowV65& source,std::string& e){++closes;source={};e.clear();return LifecycleStepV36::complete;};return s;
 }
 AssignedRootServicesV64 assigned(){AssignedReadLeavesV65 s;s.owner=owner;s.parse_result=[this](bool ok,std::string& e){++parses;if(!ok||parser_failure){e="parser leaf";return false;}e.clear();return true;};return assigned_root_kernel_services_v65(s);}
 auto consume(){return [this](const auto& document,const auto&,const CanonicalFileSourceServicesV1& source,std::string& e){
  ++consumes;if(!source.parse_result(document.diagnostic().code==0,e))return LevelFileWalkStepV1::failed;
  if(factory_failure){e="factory leaf";return LevelFileWalkStepV1::failed;}
  return source.release_load_state(e)?LevelFileWalkStepV1::complete:LevelFileWalkStepV1::failed;};}
};
void native_absence(){
 std::string e;
 {NativeFileFixture f;f.missing=true;FilenameRootRouteV65 route(f.leaves(),true);auto a=f.assigned();
  check(route.step(f.level,"missing.lightset_xml",a,f.consume(),e)==LifecycleStepV36::complete&&e.empty(),"Stage9 absence completes original LoadFile");
  check(f.opens==4&&f.sizes==0&&f.reads==0&&f.closes==0&&f.parses==0&&f.consumes==0,"Absent completion constructs no stream/parser/factory prefix");
  check(f.level->constructor_fields_v3().field140==0&&!f.level->assigned_source_owner_slot_v65(),"Absent completion never publishes assigned140");}
 {NativeFileFixture f;f.missing=true;FilenameRootRouteV65 route(f.leaves());auto a=f.assigned();
  check(route.step(f.level,"missing.lightset_xml",a,f.consume(),e)==LifecycleStepV36::failed,"Default source caller retains required-resource behavior");}
 {NativeFileFixture f;FilenameRootRouteV65 route(f.leaves(),true);auto a=f.assigned();
  check(route.step(f.level,"light.lightset_xml",a,f.consume(),e)==LifecycleStepV36::pending,"Found source keeps original copy/close pending prefix");
  check(route.step(f.level,"light.lightset_xml",a,f.consume(),e)==LifecycleStepV36::complete&&e.empty(),"Found source parses and releases the actual assigned receiver");
  check(f.opens==1&&f.sizes==1&&f.reads==1&&f.closes==1&&f.parses==1&&f.consumes==1,"Found source performs each real prefix once");
  check(f.level->constructor_fields_v3().field140==0&&!f.level->assigned_source_owner_slot_v65(),"Found completion clears the actual assigned140");}
 {NativeFileFixture f;f.open_failure=true;FilenameRootRouteV65 route(f.leaves(),true);auto a=f.assigned();
  check(route.step(f.level,"light.lightset_xml",a,f.consume(),e)==LifecycleStepV36::failed&&e=="open I/O failure","Stage9 opt-in does not mask open failure");}
 {NativeFileFixture f;f.read_failure=true;FilenameRootRouteV65 route(f.leaves(),true);auto a=f.assigned();
  check(route.step(f.level,"light.lightset_xml",a,f.consume(),e)==LifecycleStepV36::failed&&e=="read I/O failure","Stage9 opt-in does not mask read failure");}
 for(bool factory:{false,true}){NativeFileFixture f;f.factory_failure=factory;if(!factory)f.payload="<Module><GameObject></Module>";
  FilenameRootRouteV65 route(f.leaves(),true);auto a=f.assigned();
  check(route.step(f.level,"light.lightset_xml",a,f.consume(),e)==LifecycleStepV36::pending,"Actual found source copy/close prefix yields before parse");
  check(route.step(f.level,"light.lightset_xml",a,f.consume(),e)==LifecycleStepV36::failed&&e==(factory?"factory leaf":"parser leaf"),"Stage9 opt-in preserves reached parser/factory errors");
  const auto opens=f.opens,reads=f.reads,consumes=f.consumes;
  check(route.step(f.level,"light.lightset_xml",a,f.consume(),e)==LifecycleStepV36::failed&&f.opens==opens&&f.reads==reads&&f.consumes==consumes,"Failed native delivery cannot replay I/O or parse/factory prefix");}
}
}
int main(){try{script_paths();stage9_trace();filename_outcomes();native_absence();std::cout<<"PASS Stage8/9 source regressions: "<<checks<<" checks\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
