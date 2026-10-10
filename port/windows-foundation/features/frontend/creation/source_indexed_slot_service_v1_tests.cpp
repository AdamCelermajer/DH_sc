#include "source_indexed_slot_service_v1.hpp"
#include "../../../../level-world/application_services_owner_v5.hpp"
#include "../../../../level-world/character_design_services.hpp"
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace {
unsigned checks{};
void check(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
struct ControllerFixture {
    std::uint8_t connected{};
    std::shared_ptr<int> provider=std::make_shared<int>(1);
    static bool count(void*,std::int32_t& out,std::string&){out=1;return true;}
    static bool zero(void* raw,dh2::player::FirstLocalControllerBorrowV59& out,std::string&){
        auto& self=*static_cast<ControllerFixture*>(raw);
        out={reinterpret_cast<std::uintptr_t>(&self.connected),&self.connected,self.provider};return true;
    }
    dh2::player::FirstLocalControllerServicesV59 services(){return {provider,this,count,zero};}
};
}
// Link-only unrelated Application FX diagnostics are deliberately fail-closed;
// this focused test never invokes them.
extern "C" int dh2_character_debug_load(dh2::character::DebugSwitches*,const dh2::character::DebugFileServices24*){return -1;}
extern "C" int dh2_character_debug_get(std::uint32_t*,dh2::character::DebugSwitches*,const char*,const dh2::character::DebugFileServices24*){return -1;}
int main(){
 try{
  using namespace dh::foundation::frontend;
  using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
  static_assert(std::is_same_v<decltype(creation::SourceIndexedSlotResultV1::selected_profile),FrontendProfileLoanV1>);
  // Aliasing owners exercise identity without constructing or dereferencing a
  // fabricated Character candidate.
  auto token=std::make_shared<int>(1);
  auto* identity=reinterpret_cast<Record*>(token.get());
  std::shared_ptr<Record> first(token,identity);
  FrontendProfileLoanV1 loan{first};
  FrontendProfileLoanV1 same{first};
  check(loan.valid()&&loan.same_as(same),"same canonical record and control block retained");
  auto foreign_token=std::make_shared<int>(2);
  std::shared_ptr<Record> second(foreign_token,identity);
  FrontendProfileLoanV1 foreign_owner{second};
  check(!loan.same_as(foreign_owner),"same raw pointer with foreign control block rejected");
  FrontendProfileLoanV1 empty;
  check(!empty.valid()&&!loan.same_as(empty),"empty candidate cannot be reported as selected profile");
  check(creation::SourceNativeStartEvidenceV1::same_candidate_slot(first,first,3,3),"same canonical candidate/slot start evidence accepted");
  check(!creation::SourceNativeStartEvidenceV1::same_candidate_slot(first,second,3,3),"wrong canonical record owner rejected by start evidence");
  check(!creation::SourceNativeStartEvidenceV1::same_candidate_slot(first,first,3,2),"wrong start receipt slot rejected");
  using Players=dh2::player::ApplicationPlayerManagerBootstrapV59;
  auto pm_token=std::make_shared<int>(3);
  auto* pm_identity=reinterpret_cast<Players*>(pm_token.get());
  std::shared_ptr<Players> pm(pm_token,pm_identity);
  auto* player=reinterpret_cast<dh2::player::PlayerInfoFieldsV1*>(token.get());
  using Assignment=creation::SourceIndexedSlotAssignmentReceiptV1;
  check(Assignment::matches_snapshot(pm,player,0,3,pm,player,0,3),"authentic assignment snapshot matches same PM, PlayerInfo+0x664 slot and local index");
  check(!Assignment::matches_snapshot(pm,player,0,3,pm,player,0,2),"wrong assigned slot rejected");
  check(!Assignment::matches_snapshot(pm,player,0,3,std::shared_ptr<Players>(foreign_token,pm_identity),player,0,3),"same PM address with stale/foreign owner rejected");
  check(!Assignment::matches_snapshot(pm,player,0,3,pm,reinterpret_cast<dh2::player::PlayerInfoFieldsV1*>(foreign_token.get()),0,3),"wrong PlayerInfo receiver rejected");
  check(!Assignment::matches_snapshot(pm,player,0,3,pm,player,1,3),"wrong local PlayerInfo index rejected");
  // Run the genuine same-Application PM assignment operation, then construct
  // a receipt observation from the actual local PlayerInfo reread.
  auto app=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();
  dh2::player::MatchingLocalSelectionOwnerV4 matching;
  std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> actual_pm;
  std::string error;
  check(dh2::player::ApplicationPlayerManagerBootstrapV59::create_fresh(app,matching,actual_pm,error),"real Application PlayerManager C1");
  check(app->publish_source_player_manager_v59(actual_pm,error),"same Application publishes actual PlayerManager");
  ControllerFixture controller;
  std::shared_ptr<Assignment> authentic_assignment;
  if(!Assignment::assign_source_slot(actual_pm,0,3,controller.services(),authentic_assignment,error))
      throw std::runtime_error("actual NativeAssignSaveSlotToPlayer PM source prefix and receipt: "+error);
  ++checks;
  dh2::player::PlayerInfoFieldsV1* actual_info{};
  check(actual_pm->get_local_player(0,false,actual_info,error)&&actual_info&&actual_info->save_slot664==3,"same PM PlayerInfo+0x664 readback after assignment");
  check(Assignment::matches_snapshot(actual_pm,actual_info,0,3,actual_pm,actual_info,0,3),"authentic assignment operation receipt snapshot accepted");
  check(authentic_assignment&&authentic_assignment->valid_current(actual_pm,3,error),"authentic assignment receipt rechecks live owner and slot");
  auto other_app=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();
  dh2::player::MatchingLocalSelectionOwnerV4 other_matching;
  std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> other_pm;
  check(dh2::player::ApplicationPlayerManagerBootstrapV59::create_fresh(other_app,other_matching,other_pm,error),"second real PlayerManager C1");
  check(!Assignment::matches_snapshot(actual_pm,actual_info,0,3,other_pm,actual_info,0,3),"foreign PlayerManager rejected after authentic assignment");
  check(!authentic_assignment->valid_current(other_pm,3,error),"authentic receipt rejects a different live PlayerManager");
  check(!Assignment::matches_snapshot(actual_pm,actual_info,0,3,actual_pm,actual_info,0,2),"stale PlayerInfo slot rejected after authentic assignment");
  actual_info->save_slot664=2;
  check(!authentic_assignment->valid_current(actual_pm,3,error),"stale same PlayerInfo+0x664 value invalidates assignment receipt");
  actual_info->save_slot664=3;
  FrontendRuntimeResultV1 result;result.outcome=FrontendRuntimeOutcomeV1::gameplay_handoff_ready;
  result.selected_slot=3;result.selected_profile=loan;
  check(!result.gameplay_ready(),"missing actual native Start receipt cannot produce gameplay success");
  std::cout<<"PASS indexed slot public loan contract; no fake native creation success; checks="<<checks<<'\n';
  return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}
}
