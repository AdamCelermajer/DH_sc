// Compile the actual extracted renderer helpers, not another implementation.
// Fake driver storage is byte metadata only: no GL/driver/large allocation.
#include "../resource_budget_v37.hpp"
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh2::resources;
using GLuint=std::uint32_t;using GLenum=std::uint32_t;
constexpr GLenum GL_ARRAY_BUFFER=1,GL_ELEMENT_ARRAY_BUFFER=2,GL_DYNAMIC_DRAW=3;
static ContextResourceBudgetV37 ledger;
namespace dh2::android_resources {
ContextResourceBudgetV37& budget_v38(){return ledger;}
void release_v38(ResourceTokenV37& token){if(!token)return;std::string e;if(!ledger.release(token,e))throw std::runtime_error(e);}
}
struct FakeCache {unsigned receipt{17};};
struct EffectGpuResourceV4 {
 GLuint vertices{},indices{};ResourceTokenV37 vertex_budget_v38,index_budget_v38;
 std::size_t vertex_capacity{},index_capacity{};std::uint64_t budget_generation_v38{},last_seen{};
 FakeCache geometry_cache;std::shared_ptr<const void> source_retention;
};
std::map<std::uintptr_t,EffectGpuResourceV4> effect_gpu_resources_v4;
std::unordered_map<std::uintptr_t,std::size_t> effect_source_indices_v34;
struct {std::uint64_t syncs{},evictions{};std::uint32_t cache_entries{};} effect_submission_counters_v34;
struct Driver {
 std::map<GLuint,std::uint64_t> storage;GLuint next{1},array{},elements{};
 unsigned creates{},deletes{},uploads{};bool fail_after_write{},error{},zero_name{};
 std::function<void()> after_write;
} driver;
static unsigned checks;
void ck(bool condition,const char* message){if(!condition)throw std::runtime_error(message);++checks;}
void glGenBuffers(int n,GLuint* out){
 const auto pending=ledger.snapshot().pending;
 ck(pending.objects[std::size_t(ResourceKindV37::vertex_buffer)]+pending.objects[std::size_t(ResourceKindV37::index_buffer)]>0,"GL name creation preceded admission");
 for(int i=0;i<n;++i){if(driver.zero_name){out[i]=0;driver.zero_name=false;continue;}out[i]=driver.next++;driver.storage.emplace(out[i],0);++driver.creates;}
}
void glDeleteBuffers(int n,const GLuint* names){for(int i=0;i<n;++i)if(names[i]){driver.storage.erase(names[i]);++driver.deletes;}}
void glBindBuffer(GLenum target,GLuint name){(target==GL_ARRAY_BUFFER?driver.array:driver.elements)=name;}
void glBufferData(GLenum target,std::size_t bytes,const void*,GLenum){
 const auto name=target==GL_ARRAY_BUFFER?driver.array:driver.elements;
 ck(driver.storage.count(name)==1,"Write attempted without actual driver name");
 const auto admission=ledger.snapshot().pending;
 ck(admission.gpu_bytes>=bytes,"Driver storage requested before full byte admission");
 driver.storage[name]=bytes;++driver.uploads;
 if(driver.after_write){auto callback=std::move(driver.after_write);driver.after_write={};callback();}
 if(driver.fail_after_write){driver.fail_after_write=false;driver.error=true;}
}
void check(const char*){if(driver.error){driver.error=false;throw std::runtime_error("Injected OOM/check failure after real fake-driver mutation");}}
#include "../reports/resource-budget-v37/fx-production-helper-v38.inc"
static void upload(EffectGpuResourceV4& r,std::size_t bytes){std::string e;glBindBuffer(GL_ARRAY_BUFFER,r.vertices);upload_effect_storage_v38(r,GL_ARRAY_BUFFER,bytes,nullptr,ResourceKindV37::vertex_buffer,r.vertex_budget_v38,e);r.vertex_capacity=bytes;}
static void fresh(){
 ck(ledger.snapshot().occupied_records==0,"Fixture leaked accounting from previous case");
 driver={};ledger.context_lost();std::string e;ck(ledger.begin_context(e),"Fixture context start failed");
}
int main(){try{
 std::string e;fresh();EffectGpuResourceV4 r;r.source_retention=std::make_shared<int>(42);auto source=r.source_retention;
 allocate_effect_buffer_names_v38(r,e);ck(driver.storage.size()==2,"Actual helper did not own two names");
 upload(r,200);auto name=r.vertices;auto token=r.vertex_budget_v38;
 ck(driver.storage[name]==200&&ledger.snapshot().live.gpu_bytes==200,"Successful storage/accounting mismatch");
 ck(ledger.snapshot().peak.objects[std::size_t(ResourceKindV37::vertex_buffer)]==1,"Same GLuint reallocation counted another GL name");
 const auto uploads=driver.uploads,deletes=driver.deletes;
 bool thrown=false;try{upload(r,64*mib_v37+1);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown,"Active FX quota denial absent");
 ck(driver.uploads==uploads&&driver.deletes==deletes&&r.vertices==name&&driver.storage[name]==200,"Admission denial touched valid old GL storage");
 ck(r.vertex_budget_v38.serial==token.serial&&ledger.snapshot().live.gpu_bytes==200,"Admission denial replaced old owner/accounting");
 upload(r,250);ck(driver.storage[name]==250&&ledger.snapshot().live.gpu_bytes==250,"Upload helper retry did not publish admitted storage");
 driver.fail_after_write=true;
 thrown=false;try{upload(r,300);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown,"Injected driver failure not observed");
 ck(!r.vertices&&!r.indices&&!r.vertex_budget_v38&&!r.index_budget_v38&&driver.storage.empty(),"Mutated storage failure did not clean both owned names");
 ck(ledger.snapshot().requested.gpu_bytes==0&&ledger.snapshot().occupied_records==0&&r.source_retention==source,"Failure changed source retention or leaked charges");
 allocate_effect_buffer_names_v38(r,e);upload(r,300);ck(r.vertices&&r.vertex_capacity==300,"Retry after OOM did not rebuild names");
 release_effect_buffers_v38(r,false);ck(driver.storage.empty()&&ledger.snapshot().occupied_records==0,"Live teardown failed");
 fresh();driver.zero_name=true;
 thrown=false;try{allocate_effect_buffer_names_v38(r,e);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown,"Zero name accepted");
 ck(driver.storage.empty()&&ledger.snapshot().pending.gpu_bytes==0&&ledger.snapshot().occupied_records==0,"Cold partial failure leaked candidate names/admission");
 // Actual context-loss commit error, reusing an old numeric name in a new
 // context for another admitted owner. Cleanup must discard stale names.
 fresh();allocate_effect_buffer_names_v38(r,e);upload(r,100);auto old_name=r.vertices;GLuint unrelated=0;ResourceTokenV37 unrelated_token;
 driver.after_write=[&](){
  ledger.context_lost();driver.storage.clear();driver.next=old_name;driver.array=driver.elements=0;
  ck(ledger.begin_context(e),"Replacement context start failed");
  ResourceReservationV37 pending;ck(ledger.reserve_create({ResourceKindV37::vertex_buffer,ResourceScopeV37::actor,13,0},pending,e),"Unrelated owner admission failed");
  glGenBuffers(1,&unrelated);driver.storage[unrelated]=13;ck(pending.commit(unrelated_token,e),"Unrelated owner commit failed");
 };
 thrown=false;try{upload(r,150);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown,"Stale-context commit accepted");
 ck(unrelated==old_name&&driver.storage.count(unrelated)==1&&driver.storage[unrelated]==13,"Old-context cleanup deleted an unrelated replacement-context name");
 ck(!r.vertices&&!r.indices&&ledger.snapshot().live.gpu_bytes==13,"Context-loss cleanup retained stale accounting");
 glDeleteBuffers(1,&unrelated);ck(ledger.release(unrelated_token,e),"Unrelated owner release failed");
 // Bind the ACTUAL production idle/active eviction function as well.
 fresh();effect_submission_counters_v34={};effect_submission_counters_v34.syncs=1000;
 for(std::uintptr_t id:{1u,2u}){auto& entry=effect_gpu_resources_v4[id];allocate_effect_buffer_names_v38(entry,e);upload(entry,100);entry.last_seen=id==1?0:990;}
 effect_source_indices_v34[1]=0;trim_effect_gpu_cache_v34();ck(effect_gpu_resources_v4.size()==2,"Active resource evicted by age");
 effect_source_indices_v34.clear();trim_effect_gpu_cache_v34();ck(effect_gpu_resources_v4.size()==1&&effect_gpu_resources_v4.count(2)&&ledger.snapshot().live.gpu_bytes==100,"Idle eviction failed exact release/accounting");
 release_effect_buffers_v38(effect_gpu_resources_v4.at(2),false);effect_gpu_resources_v4.clear();
 ck(driver.storage.empty()&&ledger.snapshot().occupied_records==0,"Final owner teardown leaked");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_production_helpers\":4,\"fake_GL_only\":true,\"live_gpu_bytes\":"<<ledger.snapshot().live.gpu_bytes<<"}\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
