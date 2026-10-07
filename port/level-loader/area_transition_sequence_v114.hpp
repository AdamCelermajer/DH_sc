#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::loader {
struct AreaTransitionRequestV114;
enum class AreaTransitionStepV114:std::uint8_t {pending,complete,failed};
struct AreaTransitionServicesV114 {
 std::shared_ptr<void> owner; //Independent process provider; World/GS/Level weak.
 //Called only for an actual confirmed menu request, never ExitZone touch.
 //Delegates to existing V88 begin (closes the SAME source admission).
 std::function<bool(const std::shared_ptr<const AreaTransitionRequestV114>&,std::string&)> begin_retirement;
 std::function<AreaTransitionStepV114(std::string&)> drain_retirement;
 //Explicit acknowledgement/resume of the SAME failed V88 continuation.
 std::function<bool(std::string&)> retry_retirement;
 //Calls existing native campaign start/bootstrap once, with projected live
 //Save/GS arguments and SAME selected actor. False retains its actual prefix.
 std::function<bool(const std::shared_ptr<const AreaTransitionRequestV114>&,std::string&)> start_native;
 //Genuine native loading+selected actor restore completion, not callback
 //presence/readiness or a second restore pipeline. Does not drive a frame.
 std::function<AreaTransitionStepV114(const std::shared_ptr<const AreaTransitionRequestV114>&,std::string&)> native_completion;
};
//One accepted confirmed request. All methods execute on existing owning thread.
//Owns sequencing only; existing V88/V55 own actual cancellation/destruction/load.
class AreaTransitionSequenceV114 final {
public:
 enum class Phase:std::uint8_t {idle,begin_retirement,drain_retirement,start_native,await_native_completion,complete,failed};
private:
 std::shared_ptr<const AreaTransitionRequestV114> request_;
 AreaTransitionServicesV114 services_;
 Phase phase_{Phase::idle},failed_phase_{Phase::idle};
 bool busy_{},fresh_start_attempted_{};
 std::string error_,first_failure_;
 AreaTransitionStepV114 fail(const std::string&,const char*);
public:
 AreaTransitionSequenceV114()=default;
 AreaTransitionSequenceV114(const AreaTransitionSequenceV114&)=delete;
 AreaTransitionSequenceV114& operator=(const AreaTransitionSequenceV114&)=delete;
 bool accept(std::shared_ptr<const AreaTransitionRequestV114>,AreaTransitionServicesV114,std::string&);
 AreaTransitionStepV114 drain(std::string&);
 //Never automatic. Only failed old retirement may resume here; failed fresh
 //startup remains retained for genuine native retirement, never begin replay.
 bool resume_retirement(std::string&);
 Phase phase()const noexcept{return phase_;}
 const std::string& first_failure()const noexcept{return first_failure_;}
 const auto& request()const noexcept{return request_;}
};
}