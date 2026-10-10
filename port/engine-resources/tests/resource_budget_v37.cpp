#include "../resource_budget_v37.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace dh2::resources;
namespace {unsigned checks;
void ck(bool value,const std::string& message){if(!value)throw std::runtime_error("V37 check "+std::to_string(checks)+": "+message);++checks;}
ResourceBudgetLimitsV37 tiny(){ResourceBudgetLimitsV37 l;l.gpu_bytes=1000;l.texture_gpu_bytes=600;l.fx_buffer_gpu_bytes=500;l.cpu_bytes=1000;l.individual_gpu_bytes=1000;l.individual_cpu_bytes=800;l.bulk_archive_cpu_bytes=1000;l.objects.fill(8);l.objects[std::size_t(ResourceKindV37::program)]=2;l.record_slots=32;return l;}
ResourceTokenV37 allocate(ContextResourceBudgetV37& b,ResourceChargeV37 c){ResourceReservationV37 p;ResourceTokenV37 token;std::string e;ck(b.reserve_create(c,p,e),e);ck(p.commit(token,e),e);return token;}
}
int main(){try{
 std::string e;ContextResourceBudgetV37 b(tiny());ResourceReservationV37 p;ResourceTokenV37 token;
 auto texture_limits=tiny();ContextResourceBudgetV37 texture_diagnostic(texture_limits);
 ck(texture_diagnostic.begin_context(e),e);
 auto ui_texture=allocate(texture_diagnostic,{ResourceKindV37::texture,ResourceScopeV37::swf_front,450,0});
 ResourceReservationV37 excess_texture;
 ck(!texture_diagnostic.reserve_create({ResourceKindV37::texture,ResourceScopeV37::world,200,0},excess_texture,e),
    "Aggregate texture byte overflow admitted");
 ck(e.find("current=450 held=200 limit=600 incoming_kind=texture incoming_scope=world")!=std::string::npos&&
    e.find("live_scopes={swf_front=450}")!=std::string::npos,"Texture rejection omits byte operands or scope census: "+e);
 ck(texture_diagnostic.release(ui_texture,e),e);
 const ResourceChargeV37 tex{ResourceKindV37::texture,ResourceScopeV37::swf_front,300,100};
 ck(!b.reserve_create(tex,p,e),"GPU admitted without actual context");ck(b.begin_context(e),e);ck(!b.begin_context(e),"Same-context replay accepted");
 ck(b.reserve_create(tex,p,e),e);ck(b.snapshot().pending.gpu_bytes==300&&b.snapshot().live.gpu_bytes==0,"Reservation missing from requested peak");
 ck(p.commit(token,e),e);ck(b.snapshot().live.gpu_bytes==300&&b.snapshot().pending.gpu_bytes==0,"Commit accounting differs");
 auto old=token;
 {
  ResourceReservationV37 abandoned;ck(b.reserve_replace(token,{tex.kind,tex.scope,500,120},ReplacementModeV37::same_object_storage,abandoned,e),e);
  ck(b.snapshot().requested.gpu_bytes==500&&b.snapshot().requested.cpu_bytes==120,"Grow delta differs");
  auto alias=token;ck(!b.release(alias,e),"Locked old owner released");
 }
 ck(b.snapshot().live.gpu_bytes==300&&b.snapshot().pending.gpu_bytes==0,"RAII rollback altered old owner");
 ck(b.reserve_replace(token,{tex.kind,tex.scope,500,120},ReplacementModeV37::same_object_storage,p,e),e);
 ck(p.commit(token,e),e);ck(token.serial!=old.serial&&b.snapshot().live.gpu_bytes==500,"Replacement owner not refreshed");
 ck(!b.release(old,e),"Stale token released newer resource");
 ck(!b.reserve_replace(token,{tex.kind,tex.scope,200,50},ReplacementModeV37::separate_candidate,p,e),"Candidate coexistence texture peak bypassed");
 ck(b.snapshot().live.gpu_bytes==500&&!p,"Rejected replacement changed owner");
 ck(b.reserve_replace(token,{tex.kind,tex.scope,200,50},ReplacementModeV37::same_object_storage,p,e),e);
 ck(b.snapshot().requested.gpu_bytes==500,"Shrink freed old storage before success");ck(p.commit(token,e),e);
 ck(b.snapshot().live.gpu_bytes==200&&b.snapshot().live.cpu_bytes==50,"Shrink commit failed to release difference");
 auto fx=allocate(b,{ResourceKindV37::vertex_buffer,ResourceScopeV37::fx,400,20});
 ck(!b.reserve_create({ResourceKindV37::index_buffer,ResourceScopeV37::fx,101,0},p,e),"Active FX aggregate byte exemption exists");
 auto program1=allocate(b,{ResourceKindV37::program,ResourceScopeV37::shader,0,30});
 auto program2=allocate(b,{ResourceKindV37::program,ResourceScopeV37::shader,0,30});
 ck(!b.reserve_create({ResourceKindV37::program,ResourceScopeV37::shader,0,30},p,e),"Program count overflow accepted");
 ck(!b.reserve_create({ResourceKindV37::cpu_request,ResourceScopeV37::fx,0,801},p,e),"Individual CPU image/stream cap bypassed");
 ck(!b.reserve_create({ResourceKindV37::texture,ResourceScopeV37::fx,1,801,true},p,e),"Bulk flag bypassed FX/image budget");
 auto archive=allocate(b,{ResourceKindV37::cpu_request,ResourceScopeV37::asset_archive,0,850,true});
 ck(!b.reserve_create({ResourceKindV37::cpu_request,ResourceScopeV37::other,0,100},p,e),"Global CPU aggregate cap bypassed");
 ck(b.release(archive,e),e);
 // Pending GPU allocation through context loss cannot publish stale names.
 ck(b.reserve_replace(token,{tex.kind,tex.scope,250,70},ReplacementModeV37::same_object_storage,p,e),e);
 b.context_lost();auto lost=b.snapshot();
 ck(!lost.context_ready&&lost.live.gpu_bytes==0&&lost.pending.gpu_bytes==0,"Lost context retained GPU accounting");
 ck(lost.live.cpu_bytes==130&&lost.pending.cpu_bytes==20,"Lost context discarded retained CPU backing");
 ck(lost.live.cpu_bytes_by_kind[std::size_t(ResourceKindV37::texture)]==50&&lost.pending.cpu_bytes_by_kind[std::size_t(ResourceKindV37::texture)]==20,"Lost context corrupted CPU category accounting");
 ck(!p.commit(token,e),"Stale pending GPU commit accepted");ck(b.begin_context(e),e);ck(!p.commit(token,e),"Old generation admitted into new context");p.abort();
 ResourceChargeV37 prior;bool gpu=true;ck(b.charge(token,prior,gpu,e)&&!gpu&&prior.gpu_bytes==0&&prior.cpu_bytes==50,"Lost GPU owner token invalidated CPU authority");
 ck(b.reserve_replace(token,{tex.kind,tex.scope,300,50},ReplacementModeV37::same_object_storage,p,e),e);
 ck(p.commit(token,e),e);ck(b.snapshot().live.cpu_bytes==130&&b.snapshot().live.gpu_bytes==300,"Reupload double-charged CPU");
 ck(b.release(token,e)&&b.release(fx,e)&&b.release(program1,e)&&b.release(program2,e),e);
 ck(b.snapshot().requested.gpu_bytes==0&&b.snapshot().requested.cpu_bytes==0&&b.snapshot().occupied_records==0,"Teardown retained ownership charges");
 // Separate names charge full candidate coexisting with live old storage.
 token=allocate(b,{ResourceKindV37::vertex_buffer,ResourceScopeV37::actor,400,100});
 ck(b.reserve_replace(token,{ResourceKindV37::vertex_buffer,ResourceScopeV37::actor,500,120},ReplacementModeV37::separate_candidate,p,e),e);
 ck(b.snapshot().requested.gpu_bytes==900&&b.snapshot().requested.cpu_bytes==220,"Candidate peak excludes live old storage");
 ResourceTokenV37 fresh;ck(p.commit(fresh,e),e);ck(!b.release(token,e),"Replaced old token still alive");token=fresh;
 ck(b.release(token,e),e);
 // Driver storage may coexist behind one unchanged GL name. Reserve the full
 // byte peak, while preserving exact one-name kind quotas and high water.
 auto one_name=tiny();one_name.objects[std::size_t(ResourceKindV37::vertex_buffer)]=1;
 ContextResourceBudgetV37 storage(one_name);ck(storage.begin_context(e),e);
 auto same_name=allocate(storage,{ResourceKindV37::vertex_buffer,ResourceScopeV37::fx,200,0});
 ResourceReservationV37 over_count;ck(!storage.reserve_create({ResourceKindV37::vertex_buffer,ResourceScopeV37::fx,1,0},over_count,e),"Vertex buffer count overflow admitted");
 ck(e.find("kind=vertex_buffer current=1 held=1 limit=1")!=std::string::npos,"Count rejection omits the exact exceeded kind/current/held/limit: "+e);
 ck(storage.snapshot().requested.objects[std::size_t(ResourceKindV37::vertex_buffer)]==1,"Rejected count changed requested ownership");
 ck(storage.reserve_replace(same_name,{ResourceKindV37::vertex_buffer,ResourceScopeV37::fx,250,0},ReplacementModeV37::same_object_coexisting_storage,p,e),e);
 ck(storage.snapshot().requested.gpu_bytes==450&&storage.snapshot().requested.objects[std::size_t(ResourceKindV37::vertex_buffer)]==1,"Same-name coexistence peak/count differs");
 ck(p.commit(same_name,e),e);ck(storage.snapshot().live.gpu_bytes==250,"Same-name replacement commit differs");
 ck(storage.release(same_name,e),e);
 // A scene batch retains several independently admitted CPU vectors per part,
 // while UI and GPU owners share the same record pool. Keep the all-kind pool
 // separate from the existing per-kind CPU ceiling.
 ResourceBudgetLimitsV37 cpu_many;ContextResourceBudgetV37 cpu_owners(cpu_many);
 ck(cpu_many.objects[std::size_t(ResourceKindV37::cpu_request)]==8192&&cpu_many.record_slots==16384,"CPU kind and shared record ceilings differ from the bounded policy");
 ck(cpu_owners.begin_context(e),e);std::vector<ResourceTokenV37> cpu_tokens,texture_tokens;cpu_tokens.reserve(8192);texture_tokens.reserve(4096);
 for(unsigned i=0;i<8192;++i)cpu_tokens.push_back(allocate(cpu_owners,{ResourceKindV37::cpu_request,ResourceScopeV37::world,0,1}));
 for(unsigned i=0;i<4096;++i)texture_tokens.push_back(allocate(cpu_owners,{ResourceKindV37::texture,ResourceScopeV37::swf_front,0,0}));
 ck(cpu_owners.snapshot().occupied_records==12288&&cpu_owners.snapshot().live.objects[std::size_t(ResourceKindV37::cpu_request)]==8192,"Shared pool rejected legitimate mixed owners above the old 8192-slot capacity");
 ck(!cpu_owners.reserve_create({ResourceKindV37::cpu_request,ResourceScopeV37::world,0,1},p,e)&&e.find("kind=cpu_request current=8192 held=1 limit=8192")!=std::string::npos,"Larger shared pool bypassed the independent CPU kind ceiling");
 for(auto& token:cpu_tokens)ck(cpu_owners.release(token,e),e);
 for(auto& token:texture_tokens)ck(cpu_owners.release(token,e),e);
 ck(cpu_owners.snapshot().live.cpu_bytes==0&&cpu_owners.snapshot().occupied_records==0,"CPU/GPU owner stress fixture leaked accounting");
 // Foreign context tokens never release objects with recycled numeric slots.
 ContextResourceBudgetV37 other(tiny());ck(other.begin_context(e),e);auto foreign=allocate(other,tex);auto borrow=foreign;
 ck(!b.release(borrow,e),"Cross-budget owner token accepted");ck(other.release(foreign,e),e);
 // CPU reservations stay meaningful across context loss, unlike GPU names.
 ck(b.reserve_create({ResourceKindV37::cpu_request,ResourceScopeV37::other,0,10},p,e),e);b.context_lost();
 ck(p.commit(token,e),e);ck(b.release(token,e),e);
 // Record capacity is bounded independently of per-kind byte/count ceilings.
 auto small=tiny();small.record_slots=2;ContextResourceBudgetV37 capacity(small);ck(capacity.begin_context(e),e);
 auto a=allocate(capacity,{ResourceKindV37::framebuffer,ResourceScopeV37::swf_front,0,0});
 auto c=allocate(capacity,{ResourceKindV37::framebuffer,ResourceScopeV37::swf_gameplay,0,0});
 ck(!capacity.reserve_create({ResourceKindV37::cpu_request,ResourceScopeV37::other,0,1},p,e),"Unbounded resource records accepted");
 ck(e.find("incoming_kind=cpu_request")!=std::string::npos&&e.find("incoming_scope=other")!=std::string::npos&&
    e.find("occupied=2")!=std::string::npos&&e.find("pending_records=0")!=std::string::npos&&e.find("record_slots=2")!=std::string::npos&&
    e.find("swf_front:{framebuffer=1}")!=std::string::npos&&e.find("swf_gameplay:{framebuffer=1}")!=std::string::npos,
    "Record exhaustion diagnostic must census kind, scope and live/pending owners");
 ck(capacity.release(a,e)&&capacity.release(c,e),e);
 std::uint64_t bytes=123;
 ck(checked_resource_bytes_v37(65535,36,bytes,e)&&bytes==2359260,"Vertex estimate differs");
 ck(!checked_resource_bytes_v37(UINT64_MAX,2,bytes,e)&&bytes==2359260,"Overflow mutates estimate");
 ck(rgba_texture_bytes_v37(4,2,true,bytes,e)&&bytes==44,"Mip chain must include all levels");
 ck(rgba_texture_bytes_v37(3,5,true,bytes,e)&&bytes==72,"NPOT mip chain differs");
 ck(!rgba_texture_bytes_v37(UINT32_MAX,UINT32_MAX,true,bytes,e),"Huge image byte overflow accepted");
 ck(!rgba_texture_bytes_v37(0,1,false,bytes,e),"Zero image dimensions accepted");
 ck(swf_target_bytes_v37(2400,1080,bytes,e)&&bytes==31104000,"Two colors and packed depth estimate differs");
 ck(fx_geometry_cache_bytes_v37(100,200,300,400,300,bytes,e)&&bytes==13400,"FX CPU capacity envelope differs");
 ck(!fx_geometry_cache_bytes_v37(UINT64_MAX,0,0,0,0,bytes,e)&&bytes==13400,"FX CPU estimate overflow modifies previous output");
 auto final=b.snapshot();ck(final.peak.gpu_bytes==900&&final.rejections>0&&final.context_losses==2,"Counters/high water differ");
 ck(final.peak.gpu_bytes_by_kind[std::size_t(ResourceKindV37::vertex_buffer)]==900&&final.live.gpu_bytes_by_kind[std::size_t(ResourceKindV37::vertex_buffer)]==0,"VBO category current/peak differs");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"requested_gpu_peak\":"<<final.peak.gpu_bytes<<",\"rejections\":"<<final.rejections<<",\"context_losses\":"<<final.context_losses<<",\"live_gpu_bytes\":"<<final.live.gpu_bytes<<",\"live_cpu_bytes\":"<<final.live.cpu_bytes<<"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
