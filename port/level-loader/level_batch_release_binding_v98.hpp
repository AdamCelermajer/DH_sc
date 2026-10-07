#pragma once
#include "level_destroy_source_v1.hpp"
#include "level_batching_source_v96.hpp"
namespace dh2::loader {
// One completion/resource receipt, held by EXISTING external LevelD1 services.
// The containing Level is weak; this binds no World/renderer/barrier pipeline.
class LevelBatchReleaseBindingV98 final {
 std::weak_ptr<CanonicalLevelContextV1> level_;
 std::shared_ptr<BatchNodeCompilerSourceV96> actual_;
 std::uintptr_t identity_{};
 bool completed_{},unpublished_{},retired_{},d1_busy_{},retire_busy_{},nested_failed_{};
 std::string nested_failure_;
 template<class A,class B>static bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
 bool scope(std::shared_ptr<CanonicalLevelContextV1>& out,std::string& e)const{out=level_.lock();if(!out){e="Required SAME live Level158 destructor scope";return false;}return true;}
 bool nested(std::string& e,const char* why){if(!nested_failed_){nested_failed_=true;nested_failure_=why;}e=nested_failure_;return false;}
 bool latch(std::string& e)const{if(nested_failed_){e=nested_failure_;return true;}return false;}
public:
 explicit LevelBatchReleaseBindingV98(std::weak_ptr<CanonicalLevelContextV1> level):level_(std::move(level)){}
 bool borrow(std::uintptr_t id,LevelReleaseReceiverV1& out,bool& handled,std::string& e){
  handled=false;std::shared_ptr<CanonicalLevelContextV1> level;if(!scope(level,e))return false;
  auto native=level->constructor_borrow_v3();if(!native.fields){e="Required SAME actual Level158 field";return false;}
  const auto pin=level->batch_compiler_owner_slot_v96();
  //Only matching positive158 routes here. Unknown IDs/15c stay incoming Main.
  if(!id||native.fields->field158!=id)return true;
  if(!pin)return true; //genuine other native158 receipt belongs to prior lender
  handled=true;
  if(pin->identity()!=id||retired_){e="Actual typed compiler does not match original positive Level158";return false;}
  if(actual_&&!same(actual_,pin)){e="Known Level158 compiler ownership was replaced";return false;}
  if(!actual_){actual_=pin;identity_=id;}
  out={actual_,identity_};e.clear();return true;
 }
 bool destroy(const LevelReleaseReceiverV1& receiver,bool& handled,std::string& e){
  handled=actual_&&receiver.identity==identity_&&same(receiver.owner,actual_);
  if(!handled)return true; //never cast an unknown receiver.owner
  if(latch(e))return false;if(d1_busy_)return nested(e,"Level158 native D1 reentered");
  std::shared_ptr<CanonicalLevelContextV1> level;if(!scope(level,e))return false;
  auto native=level->constructor_borrow_v3();const auto pin=level->batch_compiler_owner_slot_v96();
  if(!native.fields||native.fields->field158!=identity_||!same(pin,actual_)){e="Required SAME positive158/rawslot/typed owner before D1";return false;}
  if(completed_){e.clear();return true;} //actual successful D1 receipt; no replay
  d1_busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}} guard{d1_busy_};
  std::string reached;bool delivered{};
  try{delivered=actual_->destroy_source(reached);}catch(const std::exception& ex){reached=ex.what();}catch(...){reached="Actual Level158 compiler D1 threw";}
  //Record completed native D1 BEFORE validating callback-mutated metadata.
  if(delivered)completed_=true;
  if(latch(e))return false;
  if(!delivered){e=reached.empty()?"Required genuine BatchNodeCompiler158 D1":reached;return false;}
  if(native.fields->field158!=identity_||!same(level->batch_compiler_owner_slot_v96(),actual_)){e="Level158 owner/raw identity changed during genuine D1; completion retained";return false;}
  e.clear();return true; //original LevelDestroySource alone clears raw158 next
 }
 bool pre_retire(const std::shared_ptr<CanonicalLevelContextV1>& delivered,std::string& e){
  std::shared_ptr<CanonicalLevelContextV1> level;if(!scope(level,e)||!same(level,delivered))return false;
  auto native=level->constructor_borrow_v3();if(!native.fields||native.fields->field158){e="Original Level158 rawslot must clear before typed-pin retirement";return false;}
  const auto pin=level->batch_compiler_owner_slot_v96();
  if(pin&&(!actual_||!same(pin,actual_)||!completed_)){e="Typed Level158 pin lacks SAME genuine D1 completion receipt";return false;}
  if(actual_&&!completed_){e="Admitted Level158 D1 prefix remains incomplete";return false;}
  return true;
 }
 bool retire(const std::shared_ptr<CanonicalLevelContextV1>& level,
  const std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)>& prior,std::string& e){
  if(latch(e))return false;if(retire_busy_)return nested(e,"Level158 final unpublication reentered");
  if(retired_){e.clear();return true;}
  if(!pre_retire(level,e))return false;
  retire_busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}} guard{retire_busy_};
  if(!unpublished_){
   if(!prior){e="Required existing genuine final Level unpublication leaf";return false;}
   std::string reached;bool delivered{};
   try{delivered=prior(level,reached);}catch(const std::exception& ex){reached=ex.what();}catch(...){reached="Existing final unpublication threw";}
   if(delivered)unpublished_=true;
   if(latch(e))return false;
   if(!delivered){e=reached.empty()?"Existing final Level unpublication incomplete":reached;return false;}
  }
  //Prior may clear only the same empty typed alias. A new owner/raw158 is
  //never retired by this old receipt. Independent mesh/BRES pins stay native.
  if(!pre_retire(level,e))return false;
  auto& slot=level->batch_compiler_owner_slot_v96();if(slot)slot.reset();
  actual_.reset();retired_=true;e.clear();return true;
 }
};
inline bool bind_level_batch_release_source_v98(LevelDestroyServicesV1& services,
 std::weak_ptr<CanonicalLevelContextV1> level,std::string& e){
 if(!services.owner||level.expired()){e="Required existing independent LevelD1 owner/SAME weak Level";return false;}
 auto journal=std::make_shared<LevelBatchReleaseBindingV98>(std::move(level));
 auto get=std::move(services.batch_compiler);auto destroy=std::move(services.batch_d1);auto retire=std::move(services.retire_after_unpublication);
 services.batch_compiler=[journal,prior=std::move(get)](std::uintptr_t id,LevelReleaseReceiverV1& out,std::string& e){bool handled{};if(!journal->borrow(id,out,handled,e))return false;if(handled)return true;if(!prior){e="Required genuine Main borrower for other native batch ID/15c";return false;}return prior(id,out,e);};
 services.batch_d1=[journal,prior=std::move(destroy)](const LevelReleaseReceiverV1& receiver,std::string& e){bool handled{};if(!journal->destroy(receiver,handled,e))return false;if(handled)return true;if(!prior){e="Required genuine Main D1 for other native batch ID/15c";return false;}return prior(receiver,e);};
 services.retire_after_unpublication=[journal,prior=std::move(retire)](const auto& actual,std::string& e){return journal->retire(actual,prior,e);};
 e.clear();return true;
}
}
