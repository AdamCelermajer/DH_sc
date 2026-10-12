#pragma once
#include "lifecycle_v36_counter_borrow.hpp"
#include <exception>
namespace dh2::loader {
// Exact original3f830c..3f8324 selector; raw words outside the supported native
// loading domain still enter original _LoadProcess unless they equal38.
inline bool original_level_update_loading_branch_v46(std::uint32_t state,bool force) noexcept {return state!=38&&!force;}
enum class LevelLoadingUpdateResultV46 { advanced,main_gameplay_required,failed };
struct LevelLoadProcessProviderV46 {
 std::shared_ptr<void> actual_level_owner;
 std::uintptr_t identity{};
 // Genuine complete _LoadProcess provider, or explicitly bounded native
 // composition with original per-call prefix/stage/tail services. Not gameplay.
 std::function<bool(std::string&)> load_process;
};
// Owns a checked SAME C1 borrow, not a replacement Level or a second phase.
// Main dispatches its full gameplay3f8350 body on main_gameplay_required.
// No source readiness, phase38 write, GS/global publication or save is added.
template<class Context> class LevelLoadingUpdateV46 final {
 std::shared_ptr<Context> level_;LifecycleBorrowV36 actual_;LevelLoadProcessProviderV46 provider_;
 bool busy_{},failed_{};std::string error_,required_;std::uint64_t calls_{};
 bool fail(const std::string& service,const std::string& reason){failed_=true;required_=service;error_=reason.empty()?"Required original service: "+service:reason;return false;}
 explicit LevelLoadingUpdateV46(std::shared_ptr<Context> level,LifecycleBorrowV36 actual,LevelLoadProcessProviderV46 provider):level_(std::move(level)),actual_(std::move(actual)),provider_(std::move(provider)){}
public:
 static bool create(std::shared_ptr<Context> level,LevelLoadProcessProviderV46 provider,std::unique_ptr<LevelLoadingUpdateV46>& out,std::string& error){
  LifecycleBorrowV36 actual;if(!borrow_lifecycle_fields_v36(level,actual,error))return false;
  if(!provider.actual_level_owner||provider.identity!=actual.identity||provider.actual_level_owner.get()!=actual.actual_level_owner.get()||provider.actual_level_owner.owner_before(actual.actual_level_owner)||actual.actual_level_owner.owner_before(provider.actual_level_owner)){error="Level.Update provider requires SAME completed C1 receiver/owner";return false;}
  auto candidate=std::unique_ptr<LevelLoadingUpdateV46>(new LevelLoadingUpdateV46(std::move(level),std::move(actual),std::move(provider)));out=std::move(candidate);error.clear();return true;
 }
 LevelLoadingUpdateResultV46 update(const std::shared_ptr<Context>& receiver,bool force){
  if(failed_)return LevelLoadingUpdateResultV46::failed;
  if(busy_){fail("Level.Update/nonreentrancy","Level.Update reentered; source prefix retained");return LevelLoadingUpdateResultV46::failed;}
  if(!receiver||receiver.get()!=level_.get()||receiver.owner_before(level_)||level_.owner_before(receiver)){fail("same-Level-receiver","Level.Update called with foreign Level");return LevelLoadingUpdateResultV46::failed;}
  LifecycleBorrowV36 fresh;std::string e;
  if(!borrow_lifecycle_fields_v36(receiver,fresh,e)||fresh.identity!=actual_.identity||fresh.fields.state130!=actual_.fields.state130){fail("completed-actual-C1-borrow",e);return LevelLoadingUpdateResultV46::failed;}
  // The original selector runs BEFORE loading-domain validation. Main owns
  // force=true even when the actual C1 is still at loadingstate0.
  if(!original_level_update_loading_branch_v46(*actual_.fields.state130,force))return LevelLoadingUpdateResultV46::main_gameplay_required;
  if(*actual_.fields.state130>37){fail("actual-loading-domain","Bounded native _LoadProcess requires original loadingstate0..37");return LevelLoadingUpdateResultV46::failed;}
  if(!provider_.load_process){fail("Level._LoadProcess",{});return LevelLoadingUpdateResultV46::failed;}
  struct Busy {bool& value;explicit Busy(bool& b):value(b){value=true;}~Busy(){value=false;}} guard(busy_);
  bool ok=false;try{++calls_;ok=provider_.load_process(e);}catch(const std::exception& ex){e=ex.what();}catch(...){e="Level._LoadProcess provider threw";}
  if(failed_)return LevelLoadingUpdateResultV46::failed;
  if(!ok){fail("Level._LoadProcess",e);return LevelLoadingUpdateResultV46::failed;}
  // Original Update returns after the load call, INCLUDING37->38. It never
  // executes gameplay immediately in that same invocation.
  return LevelLoadingUpdateResultV46::advanced;
 }
 const std::string& error()const noexcept{return error_;}
 const std::string& required_service()const noexcept{return required_;}
 std::uint64_t load_process_calls()const noexcept{return calls_;}
 bool failure_latched()const noexcept{return failed_;}
};
}
