#include "area_transition_sequence_v114.hpp"
#include <exception>
#include <utility>
namespace dh2::loader {
AreaTransitionStepV114 AreaTransitionSequenceV114::fail(const std::string& e,const char* why){
 if(first_failure_.empty())first_failure_=e.empty()?why:e;
 error_=first_failure_;failed_phase_=phase_;phase_=Phase::failed;return AreaTransitionStepV114::failed;
}
bool AreaTransitionSequenceV114::accept(std::shared_ptr<const AreaTransitionRequestV114> request,AreaTransitionServicesV114 services,std::string& e){
 if(busy_||phase_!=Phase::idle||!request||!services.owner||!services.begin_retirement||!services.drain_retirement||
    !services.retry_retirement||!services.start_native||!services.native_completion){
  e="Require one confirmed transition and complete existing native lifecycle providers";return false;
 }
 //Retain before any actual admission/native effect. Acceptance closes the old
 //source admission immediately, so the normal frame cannot run between ticks.
 request_=std::move(request);services_=std::move(services);phase_=Phase::begin_retirement;busy_=true;
 struct Scope{bool& busy;~Scope(){busy=false;}} scope{busy_};
 try{std::string local;if(!services_.begin_retirement(request_,local)){fail(local,"Existing native retirement admission failed");e=error_;return false;}
  phase_=Phase::drain_retirement;e.clear();return true;
 }catch(const std::exception& ex){fail(ex.what(),"Retirement admission threw");e=error_;return false;}
 catch(...){fail({},"Retirement admission threw; retained request prefix");e=error_;return false;}
}
AreaTransitionStepV114 AreaTransitionSequenceV114::drain(std::string& e){
 using R=AreaTransitionStepV114;
 if(busy_){e.clear();return R::pending;} //Existing synchronous owning-thread delivery.
 if(phase_==Phase::failed){e=error_;return R::failed;}
 if(phase_==Phase::complete){e.clear();return R::complete;}
 if(phase_==Phase::idle||phase_==Phase::begin_retirement){e="Require accepted SAME native transition request";return R::failed;}
 busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}} scope{busy_};
 try{
  if(phase_==Phase::drain_retirement){std::string local;const auto r=services_.drain_retirement(local);
   if(r==R::failed){fail(local,"Existing native retirement failed");e=error_;return R::failed;}
   if(r==R::pending){e.clear();return R::pending;}
   //V88 complete means admission drained, source cancellation, original GS/
   //Level D1 and genuine final unpublication all completed in existing owners.
   phase_=Phase::start_native;
  }
  if(phase_==Phase::start_native){
   if(fresh_start_attempted_){fail({},"Native fresh bootstrap already attempted");e=error_;return R::failed;}
   //Latch BEFORE invoking genuine existing V55 startup; false/exception keeps
   //its native constructor prefix and cannot produce another GS on retry.
   fresh_start_attempted_=true;std::string local;
   if(!services_.start_native(request_,local)){fail(local,"Fresh native campaign startup failed; prefix retained");e=error_;return R::failed;}
   phase_=Phase::await_native_completion;
  }
  if(phase_==Phase::await_native_completion){std::string local;const auto r=services_.native_completion(request_,local);
   if(r==R::failed){fail(local,"Actual native load/selected actor restoration failed");e=error_;return R::failed;}
   if(r==R::pending){e.clear();return R::pending;}
   phase_=Phase::complete;e.clear();return R::complete;
  }
  fail({},"Invalid retained area transition phase");e=error_;return R::failed;
 }catch(const std::exception& ex){fail(ex.what(),"Actual transition provider threw");e=error_;return R::failed;}
 catch(...){fail({},"Actual transition provider threw; retained reached native prefix");e=error_;return R::failed;}
}
bool AreaTransitionSequenceV114::resume_retirement(std::string& e){
 if(busy_||phase_!=Phase::failed||failed_phase_!=Phase::drain_retirement||fresh_start_attempted_||!services_.retry_retirement){
  e=error_.empty()?"Only the SAME failed old retirement continuation can resume":error_;return false;
 }
 busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}} scope{busy_};
 try{std::string local;if(!services_.retry_retirement(local)){e=error_;return false;}
  //Keep first_failure_ for diagnostics; clear only active failure latch.
  phase_=Phase::drain_retirement;error_.clear();e.clear();return true;
 }catch(...){e=error_;return false;}
}
}