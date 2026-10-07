#pragma once
#include "character_death_owner_v56.hpp"
#include "npc_dead_state_v1.hpp"
#include "character_mesh_fx_owner_v4.hpp"
#include "character_buffs.hpp"
#include "character_menu_recalc_owner_v1.hpp"
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::character {
struct CanonicalCharacterDeathServicesV84 {
 std::shared_ptr<void> provider;
 AggroClearAllServicesV2 aggro;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::uintptr_t,std::string&)> group;
 std::function<bool(std::shared_ptr<void>&,fx::CharacterMeshFxOwnerV4*&,std::string&)> fx;
 std::function<bool(std::int32_t&,std::string&)> stance;
 //Existing CharProperties Buff owner only. Empty groups still execute the
 //original Recalc(true) tail; they do not require a positive buff allocation.
 std::function<bool(BuffOwner*&,std::string&)> buffs;
};
//One transport attached to an existing canonical actor. The actual life,
//FSM, private script, target, timers, skills and physical object remain theirs.
class CanonicalCharacterDeathV84 final {
 world::CanonicalCharacterCandidateRecordV60& record_;
 CanonicalCharacterDeathServicesV84 services_;
 AIDeathOwner24 actor_{};AIDeathState64 death_{};
 std::unique_ptr<NpcDeadStateV1> selector_;
 std::unique_ptr<CharacterDeathOwnerV56> owner_;
 std::unique_ptr<CharacterMenuRecalcOwnerV1> recalc_;
 std::array<std::uintptr_t,51> ais_keys_{};
 const std::uintptr_t* previous_ais_{};
 Services previous_body_{};
 const dh2_script_callback_scope* scope_{};
 std::string error_;
 ScriptLifecycleState64* lifecycle()noexcept;
 data::PropertyView* properties()noexcept;
 bool constant(const char*,const char*,std::int32_t&,std::string&);
 bool drop(std::uintptr_t&,std::string&);
 bool on_died(std::uintptr_t,const dh2_script_callback_scope*,std::string&);
 static int dead_constant(void*,const char*,const char*,int*);
 static int dead_stance(void*,std::uintptr_t,int*);
 static void body(void*,State*,const Request*);
 static int event(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*);
public:
 CanonicalCharacterDeathV84(world::CanonicalCharacterCandidateRecordV60&,CanonicalCharacterDeathServicesV84);
 ~CanonicalCharacterDeathV84();
 CanonicalCharacterDeathV84(const CanonicalCharacterDeathV84&)=delete;
 bool raise(std::uintptr_t,const dh2_script_callback_scope*,std::string&);
 bool set_dead(std::string&);
 bool source_set_dead_state_v115(bool,std::uintptr_t,bool,std::string&);
 //1 handled,0 another source body,-1 reached failure; source prefix retained.
 int source_body(State*,const Request&,std::string&);
 const std::string& error()const noexcept{return error_;}
};
}
