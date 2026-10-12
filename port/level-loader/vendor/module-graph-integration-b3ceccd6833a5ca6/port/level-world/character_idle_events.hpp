#pragma once
#include "character_animation_instance.hpp"
#include "character_state_owner_behavior.hpp"
#include "character_script_session.hpp"
#include "character_ai_events.hpp"
#include "character_animation_ai.hpp"
#include "character_design_services.hpp"
#include <deque>

namespace dh2::character {
// Dispatch keys captured from the original CharAI/AISExternal static tables;
// these are provenance identities, never host function pointers.
const std::uintptr_t* character_idle_ai_keys() noexcept;
const std::uintptr_t* character_idle_external_keys() noexcept;

// Compatibility operations7/8 in the frozen StateOwner are actually the
// Debug.load/string-destructor boundaries, not profilers. This additive owner
// restores source ctor/query behavior between them, with the exact authored
// "isTracingCharSM" literal. Borrow the existing world DebugSwitches/file owner;
// no independent switch values or successful existing-file decode is invented.
class StateOwnerDebugDiagnostics final {
 DebugSwitches& debug_;const DebugFileServices24& files_;
 std::deque<std::string> temporary_;
 std::uint64_t loads_=0,queries_=0,destructions_=0;
 int begin(const char*);
 int end();
public:
 StateOwnerDebugDiagnostics(DebugSwitches& debug,const DebugFileServices24& files):debug_(debug),files_(files){}
 StateOwnerDebugDiagnostics(const StateOwnerDebugDiagnostics&)=delete;
 StateOwnerDebugDiagnostics& operator=(const StateOwnerDebugDiagnostics&)=delete;
 int invoke(const StateOwnerRequest48&);
 // Original Idle Focus/Blur use isTracingCharState before any state effects.
 int idle_behavior(std::uint32_t source_function);
 std::uint64_t loads()const noexcept{return loads_;}
 std::uint64_t queries()const noexcept{return queries_;}
 std::uint64_t destructions()const noexcept{return destructions_;}
 std::size_t retained_strings()const noexcept{return temporary_.size();}
};

struct IdleEventProviders {
 void* context=nullptr;
 // Only unresolved/non-Idle services reach these optional providers. Missing
 // callbacks are failure, including an unknown nonempty selected AIS method.
 int(*ai)(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*)=nullptr;
 int(*state)(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*)=nullptr;
 int(*body)(void*,State*,const Request*)=nullptr;
 StateOwnerDebugDiagnostics* diagnostics=nullptr;
};
struct IdleEventCounts {
 std::uint64_t character_events=0,animation_events=0,AI_services=0,
  FSM_events=0,state_getters=0,state_changed=0,end_relays=0,external_end_calls=0,
  animation_helpers=0,animation_selections=0;
};
// Borrowed single-authority startup and animation-event composition. All
// referenced records, providers, session and immutable tables outlive this
// nonmoving adapter and synchronous callbacks. RNG is the caller's shared
// original random producer; no independent per-character generator is created.
// This owns neither a second FSM nor a frame clock, body, path or GL object.
class CharacterIdleEvents final {
 CharacterStateOwner& states_;
 CharacterAnimationInstance& animation_;
 const data::AnimationTables& tables_;
 data::AnimationRandom& random_;
 const Facts& facts_;
 AIEventState64& ai_;
 AnimationAIState96& animation_ai_;
 IdleEventProviders providers_;
 CharacterScriptSession* session_=nullptr;
 bool input_bound_=false;
 const dh2_script_callback_scope* scope_=nullptr;
 float global_speed_;
 Services bodies_{};
 StateOwnerBehaviorPredicate8 predicates_{};
 StateOwnerBehaviorContext40 behavior_{};
 StateOwnerServices16 state_services_{};
 AIEventServices24 ai_services_{};
 AnimationAIServices16 animation_services_{};
 std::string error_;
 IdleEventCounts counts_{};
 actor::BlendedEventObserver previous_observer_{};
 static void body(void*,State*,const Request*);
 static int state_service(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*);
 static int remaining(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*);
 static int ai_service(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*);
 static void animation_service(void*,AnimationAIState96*,const AnimationAIRequest32*,AnimationAIResponse16*);
 static void observe(void*,actor::BlendedPlayback&,const actor::BlendedPlaybackEvent&);
 int fail(const char*);
 int fallback(AIEventState64*,const AIEventRequest40*,std::uint32_t*);
 bool coherent() const;
public:
 CharacterIdleEvents(CharacterStateOwner&,CharacterAnimationInstance&,
  const data::AnimationTables&,data::AnimationRandom&,const Facts&,
  AIEventState64&,AnimationAIState96&,float global_speed,
  const IdleEventProviders& providers={});
 ~CharacterIdleEvents();
 CharacterIdleEvents(const CharacterIdleEvents&)=delete;
 CharacterIdleEvents& operator=(const CharacterIdleEvents&)=delete;
 CharacterIdleEvents(CharacterIdleEvents&&)=delete;
 CharacterIdleEvents& operator=(CharacterIdleEvents&&)=delete;
 // Call before CharacterScriptSession::create/start. Installs exactly this
 // owner's NativeFsm in the persistent session input; preserves all other data.
 bool bind_input(CharacterScriptSessionInput&);
 // Attach before start; publish only after genuine Init/source active publication.
 // publish_external selects the captured External table and private AIS identity.
 bool attach(CharacterScriptSession&);
 bool publish_external();
 bool initialize_idle(); // exact LevelLoadStates preset3, after successful Init
 bool raise(std::uint32_t,std::uintptr_t payload=0,
            const dh2_script_callback_scope* scope=nullptr);
 bool scene_phase(std::uint32_t source_timestamp);
 bool animator_phase();
 const std::string& error() const noexcept{return error_;}
 const IdleEventCounts& counts() const noexcept{return counts_;}
 const StateOwnerServices16& state_services() const noexcept{return state_services_;}
 // Observer/body ABIs return void. A missing provider is latched and exposed
 // after the enclosing call; already delivered source effects are never undone.
 // Caller must stop driving this adapter after failure; no silent recovery.
};
}
