#pragma once
#include "status_effects.hpp"

namespace dh::foundation::status {
// A service provider over retained source owners, never an actor or clock.
// Host creates this before BuffOwner and keeps it stable through teardown.
struct DotTickBorrow {
 dh2::character::TimerEffectState32* state{};
 dh2::character::DotActor32* actor{};
 dh2::character::DotCombatContext32* combat{};
 const dh2::character::DotServices16* dot_services{};
 // Real remote-update/current-state/dead/debug queries. Calculate/apply are
 // supplied here by the recovered kernels, not delegated to an acceptance stub.
 const dh2::character::TimerEffectServices16* queries{};
};
class DotTickBinding {
public:
 explicit DotTickBinding(DotTickBorrow);
 DotTickBinding(const DotTickBinding&)=delete;
 DotTickBinding& operator=(const DotTickBinding&)=delete;
 bool valid(std::string&) const;
 const dh2::character::TimerEffectServices16& services() const{return services_;}
 const std::string& error() const{return error_;}
 int source_status() const{return source_status_;}
private:
 static int invoke(void*,dh2::character::TimerEffectState32*,
  const dh2::character::TimerEffectRequest40*,std::int32_t*,dh2::data::CombatResult*);
 DotTickBorrow b_;dh2::character::TimerEffectServices16 services_;
 std::string error_;int source_status_{};bool busy_{};
};
// Attach these dispatchers within the host's existing TimerServices32 and
// SkillApplyServicesV6 callbacks. Unknown operations remain unhandled. Timer
// callback must surface negative status to its host; its native void signature
// must not erase errors. No second Update, timer schedule, health or FSM exists.
int dispatch_status_timer(StatusEffects&,std::uintptr_t expected_character,
 std::uintptr_t delivered_character,std::int32_t event,
 const dh2::character::Timer32*,dh2::character::BuffResult24&,
 dh2::character::TimerEffectResult24&,std::string&);
// SAME owner's TimerServices32 callback adapter. Construct before BuffOwner,
// publish bind() after StatusEffects construction, keep it stable until source
// timer delivery ends. Native void expiry errors are latched until host checks
// failure(); no success is manufactured and no timer is advanced here.
class StatusTimerDelivery {
public:
 StatusTimerDelivery(dh2::character::TimerStore32&,
  const dh2::character::TimerServices32* other_source_events=nullptr);
 StatusTimerDelivery(const StatusTimerDelivery&)=delete;
 StatusTimerDelivery& operator=(const StatusTimerDelivery&)=delete;
 bool bind(StatusEffects&,std::string&);
 const dh2::character::TimerServices32& services()const{return services_;}
 int failure()const{return failure_;}
 const std::string& error()const{return error_;}
private:
 static void expired(void*,std::uintptr_t,std::int32_t,dh2::character::Timer32*);
 static int grow(void*,dh2::character::TimerStore32*,std::uint32_t);
 dh2::character::TimerStore32& timers_;const dh2::character::TimerServices32* other_;
 StatusEffects* status_{};dh2::character::TimerServices32 services_;
 int failure_{};std::string error_;
};
struct SkillStatusBorrow {
 StatusEffects* status{};
 const dh2::character::skills::SkillApplyServicesV6* continuation{};
};
int dispatch_skill_status(const SkillStatusBorrow&,
 const dh2::character::skills::SkillApplyRequestV6*,
 dh2::character::skills::SkillApplyResponseV6*,dh2::data::CombatResult*,std::string&);
}
