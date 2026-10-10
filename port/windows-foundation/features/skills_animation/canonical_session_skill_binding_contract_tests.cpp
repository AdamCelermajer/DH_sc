// Compile-time public API contract between the portable binding and the
// campaign's typed source loan. This deliberately does not manufacture or
// execute a canonical Character; live initialized-graph acceptance belongs to
// the native root fixture.
#include "canonical_session_skill_binding_record.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_character_fsm_v101.hpp"

#include <memory>
#include <string>

namespace {
using namespace dh::foundation;
using namespace dh::foundation::skills_animation;
using Loan=model_renderer::SourceCampaignCharacterSkillContextBorrowV1;

void compile_actual_typed_loan_consumer(CombatSession& session,
 features::SourceCharacterOwnerFactory& factory,
 const features::SourceCharacterOwnerAliases& aliases,const Loan& loan,
 SessionSkillServices& services,
 std::unique_ptr<NativeSkillLuaServicesV1>& npc,
 std::unique_ptr<NativeSkillLuaServices>& player,std::string& error){
 SkillActorBorrow actor{};
 auto map_actor=[](ActorId,std::uintptr_t&,std::string&){return false;};
 (void)validate_canonical_session_skill_loan(aliases,loan,error);
 (void)bind_canonical_native_skill_services(services,aliases,loan,npc,player,error);
 (void)make_canonical_session_skill_actor(session,1,aliases,loan,map_actor,actor,error);
 auto refresh=[](const auto&,Loan&,std::string&){return false;};
 services.borrow=make_canonical_session_skill_borrower<Loan>(session,1,factory,
  map_actor,refresh);
}
}

int main(){return 0;}
