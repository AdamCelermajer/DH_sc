#include "resource_budget_v37.hpp"
#include <algorithm>
#include <atomic>
#include <limits>
#include <stdexcept>
#include <utility>
namespace dh2::resources {
namespace {
std::atomic<std::uint64_t> next_budget{1};
constexpr std::size_t kinds=std::size_t(ResourceKindV37::count),scopes=std::size_t(ResourceScopeV37::count);
bool gpu_kind(ResourceKindV37 k){return k!=ResourceKindV37::cpu_request;}
ResourceUsageV37 footprint(const ResourceChargeV37& c,bool gpu){
 ResourceUsageV37 u;u.cpu_bytes=c.cpu_bytes;u.gpu_bytes=gpu?c.gpu_bytes:0;
 u.objects[std::size_t(c.kind)]=gpu||!gpu_kind(c.kind)?1:0;
 u.gpu_bytes_by_kind[std::size_t(c.kind)]=u.gpu_bytes;u.cpu_bytes_by_kind[std::size_t(c.kind)]=u.cpu_bytes;
 if(c.kind==ResourceKindV37::texture)u.texture_gpu_bytes=u.gpu_bytes;
 if(c.scope==ResourceScopeV37::fx&&(c.kind==ResourceKindV37::vertex_buffer||c.kind==ResourceKindV37::index_buffer))u.fx_buffer_gpu_bytes=u.gpu_bytes;
 return u;
}
void add(ResourceUsageV37& a,const ResourceUsageV37& b){a.gpu_bytes+=b.gpu_bytes;a.cpu_bytes+=b.cpu_bytes;a.texture_gpu_bytes+=b.texture_gpu_bytes;a.fx_buffer_gpu_bytes+=b.fx_buffer_gpu_bytes;for(std::size_t i=0;i<kinds;++i){a.objects[i]+=b.objects[i];a.gpu_bytes_by_kind[i]+=b.gpu_bytes_by_kind[i];a.cpu_bytes_by_kind[i]+=b.cpu_bytes_by_kind[i];}}
void sub(ResourceUsageV37& a,const ResourceUsageV37& b){a.gpu_bytes-=b.gpu_bytes;a.cpu_bytes-=b.cpu_bytes;a.texture_gpu_bytes-=b.texture_gpu_bytes;a.fx_buffer_gpu_bytes-=b.fx_buffer_gpu_bytes;for(std::size_t i=0;i<kinds;++i){a.objects[i]-=b.objects[i];a.gpu_bytes_by_kind[i]-=b.gpu_bytes_by_kind[i];a.cpu_bytes_by_kind[i]-=b.cpu_bytes_by_kind[i];}}
ResourceUsageV37 delta(const ResourceUsageV37& next,const ResourceUsageV37& old){
 ResourceUsageV37 u;auto positive=[](auto a,auto b){return a>b?a-b:0;};
 u.gpu_bytes=positive(next.gpu_bytes,old.gpu_bytes);u.cpu_bytes=positive(next.cpu_bytes,old.cpu_bytes);
 u.texture_gpu_bytes=positive(next.texture_gpu_bytes,old.texture_gpu_bytes);u.fx_buffer_gpu_bytes=positive(next.fx_buffer_gpu_bytes,old.fx_buffer_gpu_bytes);
 for(std::size_t i=0;i<kinds;++i){u.objects[i]=positive(next.objects[i],old.objects[i]);u.gpu_bytes_by_kind[i]=positive(next.gpu_bytes_by_kind[i],old.gpu_bytes_by_kind[i]);u.cpu_bytes_by_kind[i]=positive(next.cpu_bytes_by_kind[i],old.cpu_bytes_by_kind[i]);}return u;
}
bool fits(std::uint64_t current,std::uint64_t extra,std::uint64_t limit){return current<=limit&&extra<=limit-current;}
}
struct ContextResourceBudgetV37::Record {
 ResourceChargeV37 charge{};ResourceUsageV37 held{};
 std::uint64_t serial{},context_generation{},replacement_serial{};
 bool occupied{},pending{},gpu_live{};
};
ContextResourceBudgetV37::ContextResourceBudgetV37(ResourceBudgetLimitsV37 limits):limits_(limits){
 if(!limits.record_slots||limits.record_slots>16384||!limits.gpu_bytes||!limits.cpu_bytes||!limits.texture_gpu_bytes||!limits.fx_buffer_gpu_bytes||!limits.individual_gpu_bytes||!limits.individual_cpu_bytes||!limits.bulk_archive_cpu_bytes)
  throw std::invalid_argument("V37 nonzero bounded engineering budgets required");
 for(auto n:limits.objects)if(!n)throw std::invalid_argument("V37 nonzero kind counts required");
 identity_=next_budget.fetch_add(1,std::memory_order_relaxed);if(!identity_)throw std::overflow_error("V37 budget identity exhausted");
 records_=std::make_unique<Record[]>(limits.record_slots);snapshot_.bookkeeping_bytes=std::uint64_t(limits.record_slots)*sizeof(Record);
}
ContextResourceBudgetV37::~ContextResourceBudgetV37()=default;
ContextResourceBudgetV37::Record* ContextResourceBudgetV37::find(ResourceTokenV37 token)noexcept{
 if(token.budget_identity!=identity_||token.slot>=limits_.record_slots)return nullptr;
 auto& r=records_[token.slot];return r.occupied&&r.serial==token.serial?&r:nullptr;
}
bool ContextResourceBudgetV37::reject(ResourceKindV37 kind,const char* message,std::string& error){
 ++snapshot_.rejections;if(std::size_t(kind)<kinds)++snapshot_.rejections_by_kind[std::size_t(kind)];error=message;return false;
}
void ContextResourceBudgetV37::update_requested_and_peak()noexcept{
 auto u=snapshot_.live;add(u,snapshot_.pending);snapshot_.requested=u;
 auto& p=snapshot_.peak;p.gpu_bytes=std::max(p.gpu_bytes,u.gpu_bytes);p.cpu_bytes=std::max(p.cpu_bytes,u.cpu_bytes);
 p.texture_gpu_bytes=std::max(p.texture_gpu_bytes,u.texture_gpu_bytes);p.fx_buffer_gpu_bytes=std::max(p.fx_buffer_gpu_bytes,u.fx_buffer_gpu_bytes);
 for(std::size_t i=0;i<kinds;++i){p.objects[i]=std::max(p.objects[i],u.objects[i]);p.gpu_bytes_by_kind[i]=std::max(p.gpu_bytes_by_kind[i],u.gpu_bytes_by_kind[i]);p.cpu_bytes_by_kind[i]=std::max(p.cpu_bytes_by_kind[i],u.cpu_bytes_by_kind[i]);}
}
bool ContextResourceBudgetV37::begin_context(std::string& error){
 std::lock_guard<std::mutex> lock(mutex_);error.clear();
 if(snapshot_.context_ready)return reject(ResourceKindV37::framebuffer,"V37 same-context initialization replay requires live teardown",error);
 if(snapshot_.context_generation==UINT64_MAX)return reject(ResourceKindV37::framebuffer,"V37 context generation exhausted",error);
 ++snapshot_.context_generation;snapshot_.context_ready=true;return true;
}
void ContextResourceBudgetV37::context_lost()noexcept{
 std::lock_guard<std::mutex> lock(mutex_);if(!snapshot_.context_ready)return;
 for(std::uint32_t i=0;i<limits_.record_slots;++i){auto& r=records_[i];if(!r.occupied)continue;
  auto& total=r.pending?snapshot_.pending:snapshot_.live;auto& owner=r.pending?snapshot_.pending_by_scope[std::size_t(r.charge.scope)]:snapshot_.live_by_scope[std::size_t(r.charge.scope)];
  if(r.pending){auto lost=r.held;lost.cpu_bytes=0;lost.cpu_bytes_by_kind.fill(0);lost.objects[std::size_t(ResourceKindV37::cpu_request)]=0;sub(total,lost);sub(owner,lost);sub(r.held,lost);}
  else if(r.gpu_live){auto lost=footprint(r.charge,true);lost.cpu_bytes=0;lost.cpu_bytes_by_kind.fill(0);sub(total,lost);sub(owner,lost);r.charge.gpu_bytes=0;r.gpu_live=false;++snapshot_.context_discards[std::size_t(r.charge.kind)];}
 }
 snapshot_.context_ready=false;++snapshot_.context_losses;update_requested_and_peak();
}
bool ContextResourceBudgetV37::reserve_create(const ResourceChargeV37& c,ResourceReservationV37& out,std::string& e){return reserve(nullptr,c,ReplacementModeV37::separate_candidate,out,e);}
bool ContextResourceBudgetV37::reserve_replace(ResourceTokenV37 old,const ResourceChargeV37& c,ReplacementModeV37 mode,ResourceReservationV37& out,std::string& e){return reserve(&old,c,mode,out,e);}
bool ContextResourceBudgetV37::reserve(const ResourceTokenV37* old,const ResourceChargeV37& c,ReplacementModeV37 mode,ResourceReservationV37& out,std::string& error){
 std::lock_guard<std::mutex> lock(mutex_);error.clear();
 if(out)return reject(c.kind,"V37 destination reservation already owns pending admission",error);
 if(std::size_t(c.kind)>=kinds||std::size_t(c.scope)>=scopes)return reject(c.kind,"V37 resource kind/scope outside domain",error);
 if(mode!=ReplacementModeV37::same_object_storage&&mode!=ReplacementModeV37::separate_candidate&&mode!=ReplacementModeV37::same_object_coexisting_storage)return reject(c.kind,"V37 replacement mode outside domain",error);
 if(c.bulk_archive&&(c.kind!=ResourceKindV37::cpu_request||c.scope!=ResourceScopeV37::asset_archive))return reject(c.kind,"V37 archive policy cannot exempt GPU/FX/image requests",error);
 if(c.kind==ResourceKindV37::cpu_request&&(!c.cpu_bytes||c.gpu_bytes))return reject(c.kind,"V37 CPU request requires CPU bytes and no GPU charge",error);
 if(gpu_kind(c.kind)&&!snapshot_.context_ready)return reject(c.kind,"V37 actual GL context is unavailable",error);
 if(c.gpu_bytes>limits_.individual_gpu_bytes||c.cpu_bytes>(c.bulk_archive?limits_.bulk_archive_cpu_bytes:limits_.individual_cpu_bytes))return reject(c.kind,"V37 individual resource request exceeds engineering byte budget",error);
 Record* prior=nullptr;if(old){prior=find(*old);if(!prior||prior->pending||prior->replacement_serial)return reject(c.kind,"V37 replacement requires unlocked live owner token",error);}
 if(prior&&mode!=ReplacementModeV37::separate_candidate&&(prior->charge.kind!=c.kind||prior->charge.scope!=c.scope))return reject(c.kind,"V37 same-object storage cannot change kind/scope",error);
 const auto target=footprint(c,gpu_kind(c.kind));
 auto held=prior&&mode==ReplacementModeV37::same_object_storage?delta(target,footprint(prior->charge,prior->gpu_live)):target;
 if(prior&&mode==ReplacementModeV37::same_object_coexisting_storage){const auto old_usage=footprint(prior->charge,prior->gpu_live);for(std::size_t i=0;i<kinds;++i)held.objects[i]=target.objects[i]>old_usage.objects[i]?target.objects[i]-old_usage.objects[i]:0;}
 const auto& current=snapshot_.requested;
 if(!fits(current.gpu_bytes,held.gpu_bytes,limits_.gpu_bytes))return reject(c.kind,"V37 aggregate requested GPU byte budget exhausted",error);
 if(!fits(current.cpu_bytes,held.cpu_bytes,limits_.cpu_bytes))return reject(c.kind,"V37 aggregate requested CPU byte budget exhausted",error);
 if(!fits(current.texture_gpu_bytes,held.texture_gpu_bytes,limits_.texture_gpu_bytes))return reject(c.kind,"V37 aggregate texture/mip byte budget exhausted",error);
 if(!fits(current.fx_buffer_gpu_bytes,held.fx_buffer_gpu_bytes,limits_.fx_buffer_gpu_bytes))return reject(c.kind,"V37 aggregate FX VBO/EBO byte budget exhausted (active included)",error);
 for(std::size_t i=0;i<kinds;++i)if(!fits(current.objects[i],held.objects[i],limits_.objects[i]))return reject(c.kind,"V37 resource-kind count budget exhausted",error);
 std::uint32_t slot=UINT32_MAX;for(std::uint32_t i=0;i<limits_.record_slots;++i){auto at=(cursor_+i)%limits_.record_slots;if(!records_[at].occupied){slot=at;break;}}
 if(slot==UINT32_MAX)return reject(c.kind,"V37 bounded ownership record capacity exhausted",error);
 if(serial_==UINT64_MAX)return reject(c.kind,"V37 resource token serial exhausted",error);
 const auto serial=++serial_;auto& r=records_[slot];r={c,held,serial,snapshot_.context_generation,0,true,true,false};
 if(prior)prior->replacement_serial=serial;
 add(snapshot_.pending,held);add(snapshot_.pending_by_scope[std::size_t(c.scope)],held);
 ++snapshot_.occupied_records;++snapshot_.pending_records;++snapshot_.reservations;cursor_=(slot+1)%limits_.record_slots;update_requested_and_peak();
 out=ResourceReservationV37(this,{identity_,serial,slot},old?*old:ResourceTokenV37{});return true;
}
bool ContextResourceBudgetV37::commit(ResourceTokenV37 pending,ResourceTokenV37 old,ResourceTokenV37& destination,std::string& error){
 std::lock_guard<std::mutex> lock(mutex_);error.clear();auto* next=find(pending);auto* prior=old?find(old):nullptr;
 if(!next||!next->pending||(old&&(!prior||prior->pending||prior->replacement_serial!=pending.serial)))return reject(ResourceKindV37::cpu_request,"V37 stale pending/replacement token cannot commit",error);
 if(destination&&(!old||destination.budget_identity!=old.budget_identity||destination.serial!=old.serial||destination.slot!=old.slot))return reject(next->charge.kind,"V37 commit destination would overwrite an unrelated owned token",error);
 if(gpu_kind(next->charge.kind)&&(!snapshot_.context_ready||next->context_generation!=snapshot_.context_generation))return reject(next->charge.kind,"V37 context changed before GPU admission commit",error);
 sub(snapshot_.pending,next->held);sub(snapshot_.pending_by_scope[std::size_t(next->charge.scope)],next->held);
 if(prior){auto was=footprint(prior->charge,prior->gpu_live);sub(snapshot_.live,was);sub(snapshot_.live_by_scope[std::size_t(prior->charge.scope)],was);prior->occupied=false;--snapshot_.occupied_records;++snapshot_.replacements[std::size_t(next->charge.kind)];}
 else ++snapshot_.creates[std::size_t(next->charge.kind)];
 next->pending=false;next->gpu_live=gpu_kind(next->charge.kind);const auto now=footprint(next->charge,next->gpu_live);
 add(snapshot_.live,now);add(snapshot_.live_by_scope[std::size_t(next->charge.scope)],now);--snapshot_.pending_records;++snapshot_.commits;
 destination=pending;update_requested_and_peak();return true;
}
void ContextResourceBudgetV37::abort(ResourceTokenV37 token,ResourceTokenV37 old)noexcept{
 std::lock_guard<std::mutex> lock(mutex_);auto* r=find(token);if(!r||!r->pending)return;
 sub(snapshot_.pending,r->held);sub(snapshot_.pending_by_scope[std::size_t(r->charge.scope)],r->held);
 if(auto* prior=find(old);prior&&prior->replacement_serial==token.serial)prior->replacement_serial=0;
 r->occupied=false;--snapshot_.occupied_records;--snapshot_.pending_records;++snapshot_.aborts;update_requested_and_peak();
}
bool ContextResourceBudgetV37::release(ResourceTokenV37& token,std::string& error){
 std::lock_guard<std::mutex> lock(mutex_);error.clear();auto* r=find(token);
 if(!r||r->pending||r->replacement_serial)return reject(ResourceKindV37::cpu_request,"V37 release requires an unlocked live owner token",error);
 auto was=footprint(r->charge,r->gpu_live);sub(snapshot_.live,was);sub(snapshot_.live_by_scope[std::size_t(r->charge.scope)],was);
 ++snapshot_.releases[std::size_t(r->charge.kind)];r->occupied=false;--snapshot_.occupied_records;token={};update_requested_and_peak();return true;
}
bool ContextResourceBudgetV37::charge(ResourceTokenV37 token,ResourceChargeV37& out,bool& gpu,std::string& error)const{
 std::lock_guard<std::mutex> lock(mutex_);auto* r=const_cast<ContextResourceBudgetV37*>(this)->find(token);error.clear();
 if(!r||r->pending){error="V37 charge query requires live owner token";return false;}out=r->charge;gpu=r->gpu_live;return true;
}
ResourceBudgetSnapshotV37 ContextResourceBudgetV37::snapshot()const{std::lock_guard<std::mutex> lock(mutex_);return snapshot_;}
ResourceReservationV37::ResourceReservationV37(ContextResourceBudgetV37* b,ResourceTokenV37 p,ResourceTokenV37 old):budget_(b),pending_(p),replaced_(old){}
ResourceReservationV37::~ResourceReservationV37(){abort();}
ResourceReservationV37::ResourceReservationV37(ResourceReservationV37&& other)noexcept:budget_(std::exchange(other.budget_,nullptr)),pending_(other.pending_),replaced_(other.replaced_){}
ResourceReservationV37& ResourceReservationV37::operator=(ResourceReservationV37&& other)noexcept{if(this!=&other){abort();budget_=std::exchange(other.budget_,nullptr);pending_=other.pending_;replaced_=other.replaced_;}return *this;}
bool ResourceReservationV37::commit(ResourceTokenV37& out,std::string& error){if(!budget_){error="V37 reservation is absent";return false;}if(!budget_->commit(pending_,replaced_,out,error))return false;budget_=nullptr;return true;}
void ResourceReservationV37::abort()noexcept{if(budget_){auto* owner=std::exchange(budget_,nullptr);owner->abort(pending_,replaced_);}}
bool checked_resource_bytes_v37(std::uint64_t count,std::uint64_t stride,std::uint64_t& out,std::string& error){error.clear();if(stride&&count>UINT64_MAX/stride){error="V37 resource byte multiplication overflow";return false;}out=count*stride;return true;}
bool rgba_texture_bytes_v37(std::uint32_t w,std::uint32_t h,bool mip,std::uint64_t& out,std::string& error){
 error.clear();if(!w||!h){error="V37 texture dimensions must be nonzero";return false;}std::uint64_t total=0;
 do{std::uint64_t pixels,bytes;if(!checked_resource_bytes_v37(w,h,pixels,error)||!checked_resource_bytes_v37(pixels,4,bytes,error))return false;if(bytes>UINT64_MAX-total){error="V37 mip-chain byte addition overflow";return false;}total+=bytes;if(!mip||(w==1&&h==1))break;w=std::max(1u,w/2);h=std::max(1u,h/2);}while(true);
 out=total;return true;
}
bool swf_target_bytes_v37(std::uint32_t w,std::uint32_t h,std::uint64_t& out,std::string& error){if(!w||!h){error="V37 target dimensions must be nonzero";return false;}std::uint64_t pixels;if(!checked_resource_bytes_v37(w,h,pixels,error))return false;return checked_resource_bytes_v37(pixels,12,out,error);}
bool fx_geometry_cache_bytes_v37(std::uint64_t pv,std::uint64_t sv,std::uint64_t pi,std::uint64_t si,std::uint64_t source,std::uint64_t& out,std::string& error){
 std::uint64_t total=0;const std::uint64_t counts[]{pv,sv,pi,si,source},strides[]{36,36,2,2,4};
 for(unsigned i=0;i<5;++i){std::uint64_t bytes;if(!checked_resource_bytes_v37(counts[i],strides[i],bytes,error))return false;if(bytes>UINT64_MAX-total){error="V37 FX CPU capacity addition overflow";return false;}total+=bytes;}
 out=total;return true;
}
}
