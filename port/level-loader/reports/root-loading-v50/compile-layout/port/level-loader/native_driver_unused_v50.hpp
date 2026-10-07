#pragma once
#include <atomic>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

namespace dh2::loader {
// Native backend adaptation of IVideoDriver.removeUnused5aa290. The original
// engine uses intrusive collections; this port uses explicit consumer leases
// over EXISTING native allocations. shared_ptr.use_count is never a reference
// authority. Bind/register at allocation, acquire at raw Draw/UI/FX ownership,
// unregister at the actual GL/accounting release. No graphics reset occurs.
enum class UnusedDomainV50 {batch_baker,material_instance,material_renderer,texture};
struct UnusedKeyV50 {
 UnusedDomainV50 domain{};std::uint64_t generation{},identity{};
 bool operator<(const UnusedKeyV50& b)const noexcept{
  if(domain!=b.domain)return domain<b.domain;
  if(generation!=b.generation)return generation<b.generation;
  return identity<b.identity;
 }
 bool operator==(const UnusedKeyV50& b)const noexcept{return domain==b.domain&&generation==b.generation&&identity==b.identity;}
};
class NativeDriverUnusedOwnerV50 final {
 struct Entry {
  UnusedKeyV50 key;
  std::atomic<std::uint32_t> consumers{};
  std::weak_ptr<void> backend_owner;
  std::function<bool(bool,std::string&)> release;
  bool detached{},attempted{};
 };
 std::map<UnusedKeyV50,std::shared_ptr<Entry>> entries_;
 std::map<UnusedDomainV50,std::weak_ptr<void>> bound_;
 std::uint64_t generation_{};
 bool ready_{},busy_{},failed_{};
 const Entry* releasing_{};
 std::string failure_;
 std::function<bool(std::string&)> free_2d_textures_;
 std::function<bool(std::string&)> clear_texture_placeholders_;
 std::weak_ptr<void> driver_owner_;
 std::uint64_t releases_{},preserved_{},inspected_{};
 bool reject(const char* why,std::string& e){if(e.empty())e=why;if(!failed_)failure_=e;failed_=true;e=failure_;return false;}
 bool remove_unused(UnusedDomainV50 domain,std::string& e){
  auto binding=bound_.find(domain);
  if(binding==bound_.end()||binding->second.expired())return reject("Required actual registered backend collection owner",e);
  for(auto i=entries_.begin();i!=entries_.end();){
   auto current=i++;
   auto entry=current->second;
   if(entry->key.domain!=domain)continue;
   ++inspected_;
   if(entry->consumers.load(std::memory_order_acquire)){++preserved_;continue;}
   auto backend=entry->backend_owner.lock();
   if(!backend||!entry->release)return reject("Required actual allocation release owner/body",e);
   if(entry->attempted)return reject("Native cleanup release prefix already failed; refusing GL replay",e);
   entry->attempted=true;releasing_=entry.get();
   bool ok=false;
   try{ok=entry->release(false,e);}catch(const std::exception& x){e=x.what();}catch(...){e="Native cleanup backend release exception";}
   releasing_=nullptr;
   if(!ok||failed_)return reject("Native unused backend release failed",e);
   entry->detached=true;entries_.erase(current);++releases_;
  }
  return true;
 }
public:
 class UseLease final {
  friend class NativeDriverUnusedOwnerV50;
  std::shared_ptr<Entry> entry_;
  explicit UseLease(std::shared_ptr<Entry> entry):entry_(std::move(entry)){retain();}
  void retain(){
   if(!entry_)return;
   auto n=entry_->consumers.load(std::memory_order_relaxed);
   do{if(n==std::numeric_limits<std::uint32_t>::max())throw std::overflow_error("Native resource consumer count overflow");}
   while(!entry_->consumers.compare_exchange_weak(n,n+1,std::memory_order_release,std::memory_order_relaxed));
  }
 public:
  UseLease()=default;
  UseLease(const UseLease& b):entry_(b.entry_){retain();}
  UseLease(UseLease&& b)noexcept:entry_(std::move(b.entry_)){}
  UseLease& operator=(const UseLease& b){if(this!=&b){UseLease next(b);swap(next);}return *this;}
  UseLease& operator=(UseLease&& b)noexcept{if(this!=&b){reset();entry_=std::move(b.entry_);}return *this;}
  ~UseLease(){reset();}
  void swap(UseLease& b)noexcept{entry_.swap(b.entry_);}
  void reset()noexcept{if(entry_)entry_->consumers.fetch_sub(1,std::memory_order_acq_rel);entry_.reset();}
  explicit operator bool()const noexcept{return bool(entry_);}
 };
 struct Snapshot {std::uint64_t generation{},entries{},consumer_references{},releases{},preserved{},inspected{};bool ready{},failed{};};
 // Driver construction/binding must precede real allocations. Binding an
 // actual collection is mandatory even when its genuine current size is0.
 bool bind_driver(std::shared_ptr<void> owner,std::uint64_t generation,
   std::function<bool(std::string&)> actual_free_2d_textures,
   std::function<bool(std::string&)> actual_clear_placeholders,std::string& e){
  if(ready_||busy_||!owner||!generation||!actual_free_2d_textures||!actual_clear_placeholders){e="Require fresh actual native driver/freeTextures/placeholder binding";return false;}
  driver_owner_=owner;generation_=generation;free_2d_textures_=std::move(actual_free_2d_textures);clear_texture_placeholders_=std::move(actual_clear_placeholders);ready_=true;failed_=false;failure_.clear();e.clear();return true;
 }
 bool bind_collection(UnusedDomainV50 domain,std::shared_ptr<void> actual_owner,std::string& e){
  if(busy_)return reject("Native cleanup collection binding reentered",e);
  if(!ready_||busy_||!actual_owner||bound_.count(domain)){e="Require one actual native allocator collection binding";return false;}
  bound_.emplace(domain,actual_owner);e.clear();return true;
 }
 bool register_allocation(UnusedKeyV50 key,std::shared_ptr<void> backend_owner,
   std::function<bool(bool,std::string&)> actual_release,std::string& e){
  if(busy_)return reject("Native cleanup allocation publication reentered",e);
  if(!ready_||busy_||failed_||driver_owner_.expired()||key.generation!=generation_||!key.identity||!backend_owner||!actual_release||!bound_.count(key.domain)){
   e="Require live same-generation actual registered allocator/release";return false;
  }
  if(bound_.at(key.domain).expired()){e="Actual registered backend collection owner expired";return false;}
  if(entries_.count(key)){e="Native allocation identity already registered";return false;}
  auto entry=std::make_shared<Entry>();entry->key=key;entry->backend_owner=backend_owner;entry->release=std::move(actual_release);
  entries_.emplace(key,std::move(entry));e.clear();return true;
 }
 bool acquire(UnusedKeyV50 key,UseLease& out,std::string& e){
  if(busy_)return reject("Native cleanup raw consumer publication reentered",e);
  auto found=entries_.find(key);
  if(!ready_||busy_||failed_||found==entries_.end()||found->second->detached||found->second->attempted){e="Require registered live native allocation before raw consumer publication";return false;}
  UseLease candidate(found->second);out=std::move(candidate);e.clear();return true;
 }
 // Called from actual release_model_texture/release_program hooks. During
 // cleanup's own callback acknowledge only that exact current key; the outer
 // collector erases after backend success, avoiding iterator invalidation.
 bool unregister_allocation(UnusedKeyV50 key,std::string& e){
  auto found=entries_.find(key);
  if(found==entries_.end()){e="Actual native allocation release lacks cleanup registration";return false;}
  auto entry=found->second;
  if(entry->consumers.load(std::memory_order_acquire)){e="Actual resource release still has Draw/UI/FX consumers";return false;}
  if(busy_){
   if(failed_){e=failure_;return false;}
   if(releasing_==entry.get()){e.clear();return true;}
   return reject("Cleanup backend attempted unrelated registry mutation",e);
  }
  entry->detached=true;entries_.erase(found);e.clear();return true;
 }
 // Backend lost-release may run after abandon_context detached the records,
 // or after a new generation reused the same numeric name. Never acknowledge
 // a fabricated loss for an allocation in the currently live generation.
 bool unregister_lost_allocation(UnusedKeyV50 key,std::string& e){
  if(busy_ || (ready_&&key.generation==generation_)){
   e="Cannot abandon an allocation from the live native driver generation";return false;
  }
  auto found=entries_.find(key);
  if(found!=entries_.end()){
   found->second->detached=true;entries_.erase(found);
  }
  e.clear();return true;
 }
 bool clean_glitch(std::string& e){
  if(failed_){e=failure_;return false;}
  if(busy_)return reject("Application.CleanGlitch native cleanup reentered",e);
  if(!ready_||driver_owner_.expired()||!free_2d_textures_)return reject("Required live Application device/video-driver cleanup binding",e);
  busy_=true;struct Guard{bool& value;~Guard(){value=false;}}guard{busy_};
  // Original freeTextures → removeAllBatchBaker → clearUnusedInstances →
  // removeAll(false) → clearPlaceHolders → removeAll(false). Placeholder
  // and default2D consumer slots are released by the real caller-owned hooks;
  // their underlying texture allocations remain until the final pass.
  try{
   if(!free_2d_textures_(e)||failed_)return reject("Actual C2D/freeTextures failed",e);
   if(!remove_unused(UnusedDomainV50::batch_baker,e))return false;
   if(!remove_unused(UnusedDomainV50::material_instance,e))return false;
   if(!remove_unused(UnusedDomainV50::material_renderer,e))return false;
   if(!clear_texture_placeholders_(e)||failed_)return reject("Actual texture placeholder cleanup failed",e);
   if(!remove_unused(UnusedDomainV50::texture,e))return false;
   e.clear();return true;
  }catch(const std::exception& x){e=x.what();return reject("Native cleanup exception",e);}
  catch(...){return reject("Native cleanup unknown exception",e);}
 }
 // Context-loss callback cannot issue GL deletes. Detach old bookkeeping;
 // real backend context-lost release has its own generation-safe accounting.
 bool abandon_context(std::uint64_t actual_generation,std::string& e){
  if(busy_||!ready_||actual_generation!=generation_){e="Require actual old driver generation at context loss";return false;}
  for(auto& entry:entries_)entry.second->detached=true;
  entries_.clear();bound_.clear();free_2d_textures_={};clear_texture_placeholders_={};driver_owner_.reset();ready_=false;e.clear();return true;
 }
 Snapshot snapshot()const noexcept{
  Snapshot s{generation_,entries_.size(),0,releases_,preserved_,inspected_,ready_,failed_};
  for(const auto& row:entries_)s.consumer_references+=row.second->consumers.load(std::memory_order_acquire);
  return s;
 }
};
}
