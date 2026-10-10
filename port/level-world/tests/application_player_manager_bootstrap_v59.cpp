#include "../application_player_manager_bootstrap_v59.hpp"
#include "../application_services_owner_v5.hpp"
#include "../source_input_manager_v60.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh2::player;
using App=dh2::application::ApplicationServicesOwnerV5;
using Boot=ApplicationPlayerManagerBootstrapV59;
namespace {
unsigned checks;
void check(bool ok){++checks;if(!ok)throw std::runtime_error("V59 check "+std::to_string(checks));}
// Fixture-only InputManager transport, never a production controller owner.
struct Input {
 std::int32_t gamepads{};std::uint8_t connected{};bool fail_count{},fail_zero{},missing_cell{};
 std::vector<int> order;std::shared_ptr<int> lease=std::make_shared<int>(1);
 static bool count(void* p,std::int32_t& out,std::string& e){auto& s=*static_cast<Input*>(p);s.order.push_back(1);if(s.fail_count){e="fixture count failure";return false;}out=s.gamepads;return true;}
 static bool zero(void* p,FirstLocalControllerBorrowV59& out,std::string& e){auto& s=*static_cast<Input*>(p);s.order.push_back(2);if(s.fail_zero){e="fixture zero failure";return false;}out={reinterpret_cast<std::uintptr_t>(s.lease.get()),s.missing_cell?nullptr:&s.connected,s.lease};return true;}
 FirstLocalControllerServicesV59 services(){return {lease,this,count,zero};}
};
// Actual existing CNet/PlayerInfo scalar/skill-buffer bodies, with fixture
// continuations only for tests of active transport and online failure order.
struct Previous {
 MatchingLocalSelectionOwnerV4 matching;
 std::unique_ptr<PlayerNetworkLocalOwnerV4> network=std::make_unique<PlayerNetworkLocalOwnerV4>(matching);
 std::unique_ptr<PlayerManagerCombatRuntimeV2> runtime;
 std::shared_ptr<int> lease=std::make_shared<int>(2);
 std::map<PlayerInfoFieldsV1*,std::shared_ptr<PlayerInfoSkillBuffersV26>> buffers;
 unsigned constructors{},online_queries{};bool active_rebind_refused{},active_query_rebind_refused{};
 static bool service(void* p,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& out,std::string& e){
  auto& s=*static_cast<Previous*>(p);
  if(q.operation==PlayerManagerOperationV1::online_enabled){++s.online_queries;out.value=0;
   std::string guard;s.active_query_rebind_refused=!s.runtime->rebind_source_services_v59({&s,service},{&s,service},guard);return true;}
  if(q.operation!=PlayerManagerOperationV1::construct_player_info){e="unbound previous continuation";return false;}
  ++s.constructors;if(!q.player||!s.network->construct(*q.player,s.lease,e))return false;
  *q.player=PlayerInfoFieldsV1{};auto b=std::make_shared<PlayerInfoSkillBuffersV26>();
  s.buffers.emplace(q.player,b);if(!b->construct(*q.player,*s.network,e))return false;
  std::string guard;
  s.active_rebind_refused=!s.runtime->rebind_source_services_v59({&s,service},{&s,service},guard);
  return true;
 }
 void construct(){runtime=std::make_unique<PlayerManagerCombatRuntimeV2>(PlayerManagerCombatServicesV2{this,service});check(runtime->initialize());check(active_rebind_refused);}
};
struct Online {
 std::vector<PlayerManagerOperationV1> order;PlayerManagerOperationV1 fail_at=PlayerManagerOperationV1::network_enabled;
 bool values{true};
 static bool service(void* p,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& out,std::string& e){auto& s=*static_cast<Online*>(p);s.order.push_back(q.operation);if(q.operation==s.fail_at){e="fixture online continuation unavailable";return false;}out.value=s.values;return true;}
};
void check_buffers(Boot& boot,PlayerInfoFieldsV1& record){
 std::string e;std::shared_ptr<PlayerInfoSkillBuffersV26> b;check(boot.skill_buffers(record,b,e));check(b&&b->receiver()==&record);
 std::array<std::int8_t,3> slots{};std::array<std::int8_t,30> levels{};std::int32_t copied{};
 check(b->get_slots(record,slots.data(),slots.size(),copied,e));check(copied==3);
 check(std::all_of(slots.begin(),slots.end(),[](auto v){return v==-1;}));
 check(b->get_levels(record,levels.data(),levels.size(),copied,e));check(copied==30);
 check(std::all_of(levels.begin(),levels.end(),[](auto v){return v==-1;}));
}
void fresh(){
 auto app=std::make_shared<App>();MatchingLocalSelectionOwnerV4 matching;std::shared_ptr<Boot> boot;std::string e;
 check(Boot::create_fresh(app,matching,boot,e));check(boot->phase()==PlayerManagerBootstrapPhaseV59::constructed);
 check(boot->belongs_to_application(app));check(!boot->belongs_to_application(std::make_shared<App>()));
 check(*boot->count_field()==0);check(!app->source_player_manager_v59());check(app->publish_source_player_manager_v59(boot,e));
 PlayerInfoFieldsV1* dummy{};check(boot->get_by_internal(-1,false,dummy,e));check(dummy&&dummy->save_slot664==-1&&dummy->character660==0);
 check_buffers(*boot,*dummy);const auto dummy_net=boot->network()->borrow(*dummy);check(dummy_net&&dummy_net->owner1a0==-1);
 std::int32_t n{};check(boot->manager()->num_players(n,e)&&n==0);
 // A no-entry manager resolves GetLocalPlayer(0,false) to the constructor
 // sentinel. Slot publication must reject it without changing sentinel state.
 check(!boot->publish_selected_save_slot_v67(0,2,e));
 check(e.find("sentinel/non-local PlayerInfo")!=std::string::npos&&dummy->save_slot664==-1);
 Input input;input.gamepads=-7; // source clamps; still requires GetGamepad(0)
 input.fail_count=true;check(!boot->source_first_local_add_prefix(input.services(),e));check(input.order==std::vector<int>{1});
 input.fail_count=false;input.fail_zero=true;input.order.clear();check(!boot->source_first_local_add_prefix(input.services(),e));check(input.order==std::vector<int>({1,2}));
 input.fail_zero=false;input.missing_cell=true;input.order.clear();check(!boot->source_first_local_add_prefix(input.services(),e));check(input.order==std::vector<int>({1,2}));
 check(boot->manager()->num_players(n,e)&&n==0);input.missing_cell=false;input.order.clear();
 // Positive production composition uses root's genuine source constructor
 // owner, not this test's injected count/connected fixtures.
 auto actual_input=std::make_shared<dh2::input::SourceInputManagerV60>();
 check(actual_input->gamepad_count()==4);std::shared_ptr<const dh2::input::GamepadC1V60> actual_pad;
 check(actual_input->get_gamepad(0,actual_pad,e));check(actual_pad&&actual_pad->connected758==0);
 check(boot->source_first_local_add_prefix(actual_input->first_local_services(),e));
 check(actual_pad->connected758==0&&boot->observed_controller_connected()==0);
 check(boot->phase()==PlayerManagerBootstrapPhaseV59::first_local_added&&!boot->profile_input_pending());
 PlayerInfoFieldsV1* local{};check(boot->get_local_player(0,false,local,e));check(local&&local!=dummy);
 check(local->internal670==0&&local->controller_local674==-1&&local->controller668==0&&local->local66c==1);
 check(local->friendly678==0&&local->local_remote67c==0&&local->character660==0&&local->save_slot664==-1);
 local->character660=0x12345678;
 check(!boot->publish_selected_save_slot_v67(0,2,e));
 check(e.find("after Character660 publication")!=std::string::npos&&local->character660==0x12345678&&local->save_slot664==-1);
 local->character660=0;
 check(boot->publish_selected_save_slot_v67(0,2,e));check(local->save_slot664==2&&!boot->profile_input_pending());
 check(boot->publish_selected_save_slot_v67(0,2,e));check(local->save_slot664==2);
 check(!boot->publish_selected_save_slot_v67(0,1,e));check(local->save_slot664==2&&!e.empty());
 local->save_slot664=-1;
 check(!boot->publish_selected_save_slot_v67(-1,2,e));check(local->save_slot664==-1&&!e.empty());
 check(!boot->publish_selected_save_slot_v67(0,4,e));check(local->save_slot664==-1&&!e.empty());
 check(*boot->count_field()==0);check(boot->manager()->num_players(n,e)&&n==1);check(boot->manager()->num_local_players(true,n,e)&&n==0);
 PlayerInfoFieldsV1* needs_character{};check(boot->get_local_player(0,true,needs_character,e));check(needs_character==dummy);
 check_buffers(*boot,*local);const auto local_net=boot->network()->borrow(*local);check(local_net&&local_net->receiver==local);
 bool is_local{};check(boot->network()->is_local(*local,is_local,e)&&is_local);
 check(boot->source_first_local_add_prefix(input.services(),e));check(boot->phase()==PlayerManagerBootstrapPhaseV59::first_local_retained&&boot->profile_input_pending());
 const auto address=local;local->save_slot664=7; // fixture of a future actual slot setter, not helper policy
 input.connected=1;check(boot->source_first_local_add_prefix(input.services(),e));check(boot->observed_controller_connected()==1);
 check(boot->get_by_internal(0,false,local,e)&&local==address&&local->save_slot664==7);
 check(boot->phase()==PlayerManagerBootstrapPhaseV59::first_local_retained&&!boot->profile_input_pending());
 check(boot->network()->borrow(*local)==local_net&&boot->network()->borrow(*dummy)==dummy_net);check(*boot->count_field()==0);
 std::shared_ptr<Boot> duplicate;check(!Boot::create_fresh(app,matching,duplicate,e));check(!duplicate);
 check(!boot->manager()->add_character(0,e));check(local->character660==0&&*boot->count_field()==0);
 // No App cycle: held helper/records cannot keep Application alive.
 std::weak_ptr<App> weak=app;app.reset();check(weak.expired());check(!boot->get_by_internal(0,false,local,e));
}
void adoption(){
 auto app=std::make_shared<App>();auto old=std::make_shared<Previous>();old->construct();std::string e;
 check(old->runtime->manager().add_player(0,0,0,true,e));check(old->active_query_rebind_refused);PlayerInfoFieldsV1* record{};
 check(old->runtime->manager().get_by_internal(0,false,record,e));record->character660=0x13579;record->save_slot664=4;
 const auto runtime=old->runtime.get();const auto network=old->network.get();const auto net=network->borrow(*record);
 const auto constructors=old->constructors;const auto queries=old->online_queries;std::shared_ptr<Boot> boot;
 check(!Boot::adopt_existing(app,old->runtime,old->network,{nullptr,Previous::service},old,boot,e));
 check(!Boot::adopt_existing(app,old->runtime,old->network,{old.get(),Previous::service},std::make_shared<int>(9),boot,e));
 check(old->runtime.get()==runtime&&old->network.get()==network&&old->constructors==constructors);
 check(Boot::adopt_existing(app,old->runtime,old->network,{old.get(),Previous::service},old,boot,e));
 check(!old->runtime&&!old->network);check(boot->runtime()==runtime&&boot->network()==network);
 check(old->constructors==constructors&&old->online_queries==queries);check(app->publish_source_player_manager_v59(boot,e));
 check(record->character660==0x13579&&record->save_slot664==4&&record->controller_local674==0);check(*boot->count_field()==0);
 PlayerInfoFieldsV1* current{};check(boot->get_local_player(0,true,current,e)&&current==record);check(boot->network()->borrow(*current)==net);
 std::shared_ptr<PlayerInfoSkillBuffersV26> b;check(!boot->skill_buffers(*record,b,e)); // never Reset an adopted record
 check(boot->bind_existing_buffers(old,[old](PlayerInfoFieldsV1& p,std::shared_ptr<PlayerInfoSkillBuffersV26>& out,std::string& error){auto it=old->buffers.find(&p);if(it==old->buffers.end()){error="actual previous buffer missing";return false;}out=it->second;return true;},e));
 check_buffers(*boot,*record);Input input;input.gamepads=4;check(boot->source_first_local_add_prefix(input.services(),e));
 // Original existing-map branch does not call AddPlayer or turn the old
 // development tuple into source provenance; preserve existing Character.
 check(boot->phase()==PlayerManagerBootstrapPhaseV59::first_local_retained&&!boot->profile_input_pending());
 check(record->character660==0x13579&&record->save_slot664==4&&record->controller_local674==0);check(old->constructors==constructors);
 std::unique_ptr<PlayerManagerCombatRuntimeV2> loan;std::unique_ptr<PlayerNetworkLocalOwnerV4> loan_network;std::function<void()> returned;
 check(boot->lend_to_runtime(loan,loan_network,returned,e));check(loan.get()==runtime&&loan_network.get()==network);
 check(boot->get_local_player(0,true,current,e)&&current==record);check(!boot->lend_to_runtime(loan,loan_network,returned,e));
 check(record->character660==0x13579&&boot->network()->borrow(*record)==net);returned();returned={};
 check(!loan&&!loan_network&&boot->runtime()==runtime&&boot->network()==network);
 check(boot->get_local_player(0,true,current,e)&&current==record);check(record->save_slot664==4&&*boot->count_field()==0);
 // Actual RemoveCharacter is required. Failure retains same Character/map.
 check(!boot->manager()->remove_player(0,e));check(boot->get_by_internal(0,false,current,e)&&current==record&&current->character660==0x13579);
}
void changed_loan_owner(){
 auto app=std::make_shared<App>();MatchingLocalSelectionOwnerV4 matching;std::shared_ptr<Boot> boot;std::string e;
 check(Boot::create_fresh(app,matching,boot,e));check(app->publish_source_player_manager_v59(boot,e));
 std::unique_ptr<PlayerManagerCombatRuntimeV2> loan;std::unique_ptr<PlayerNetworkLocalOwnerV4> network;std::function<void()> returned;
 check(boot->lend_to_runtime(loan,network,returned,e));auto displaced=std::move(loan);
 returned();returned={};check(!boot->runtime()&&!boot->network());
 PlayerInfoFieldsV1* record{};check(!boot->get_by_internal(-1,false,record,e));
 check(displaced->manager().source_initialized_v59()); // observed publication invalidated, no owner reset
}
void internal_zero_nonlocal(){
 auto app=std::make_shared<App>();MatchingLocalSelectionOwnerV4 matching;std::shared_ptr<Boot> boot;std::string e;
 check(Boot::create_fresh(app,matching,boot,e));check(app->publish_source_player_manager_v59(boot,e));
 check(boot->manager()->add_player(0,-1,0,false,e));
 PlayerInfoFieldsV1* mapped{};check(boot->get_by_internal(0,false,mapped,e));
 check(mapped&&mapped->internal670==0&&!mapped->local66c);
 Input input;input.gamepads=1;
 // IDA IsPlayerInLocalMap(0) is key membership, so the source first-local
 // prefix skips AddPlayer here. Its local-player enumeration then resolves
 // to the sentinel; report the mismatch instead of fabricating/promoting a row.
 check(!boot->source_first_local_add_prefix(input.services(),e));
 check(e.find("internal ID 0 is mapped to a non-local PlayerInfo")!=std::string::npos);
 check(e.find("mapped internal670=0, local66c=0, save_slot664=-1")!=std::string::npos);
 check(e.find("total_players=1; local_players_no_character=0; local_players_with_character=0")!=std::string::npos);
 check(e.find("get_local_0_false=ok, same_as_sentinel=true, internal670=-1, local66c=1, save_slot664=-1")!=std::string::npos);
 check(e.find("sentinel_lookup=ok, internal670=-1, local66c=1, save_slot664=-1")!=std::string::npos);
 check(boot->phase()==PlayerManagerBootstrapPhaseV59::first_local_failed);
 PlayerInfoFieldsV1* sentinel{};check(boot->get_local_player(0,false,sentinel,e));
 check(sentinel&&sentinel!=mapped&&sentinel->internal670==-1&&sentinel->local66c==1);
 check(!boot->publish_selected_save_slot_v67(0,2,e));
 check(e.find("sentinel/non-local PlayerInfo")!=std::string::npos&&mapped->save_slot664==-1&&sentinel->save_slot664==-1);
}
void selected_slot_authored_callback_route(){
 auto app=std::make_shared<App>();MatchingLocalSelectionOwnerV4 matching;std::shared_ptr<Boot> boot;std::string e;
 check(Boot::create_fresh(app,matching,boot,e));check(app->publish_source_player_manager_v59(boot,e));
 auto input=std::make_shared<dh2::input::SourceInputManagerV60>();
 // This is the production synchronous prefix behind the SWF's literal
 // NativeAssignSaveSlotToPlayer(0, SlotID) after NativeCreateSaveSlot.
 check(boot->assign_selected_save_slot_v70(0,3,input->first_local_services(),e));
 PlayerInfoFieldsV1 *selected{},*sentinel{};
 check(boot->get_local_player(0,false,selected,e));check(boot->get_by_internal(-1,false,sentinel,e));
 check(selected&&selected!=sentinel&&selected->internal670==0&&selected->local66c==1);
 check(selected->save_slot664==3&&selected->character660==0);
 std::int32_t locals{};check(boot->manager()->num_local_players(false,locals,e)&&locals==1);
 // Replay of the exact callback is idempotent for the same chosen slot.
 check(boot->assign_selected_save_slot_v70(0,3,input->first_local_services(),e));
 check(boot->get_local_player(0,false,selected,e)&&selected->save_slot664==3);
 check(!boot->assign_selected_save_slot_v70(0,2,input->first_local_services(),e));
 check(selected->save_slot664==3&&e.find("different profile")!=std::string::npos);
}
void online(){
 auto app=std::make_shared<App>();MatchingLocalSelectionOwnerV4 matching;std::shared_ptr<Boot> boot;std::string e;
 check(Boot::create_fresh(app,matching,boot,e));check(app->publish_source_player_manager_v59(boot,e));
 auto continuation=std::make_shared<Online>();check(boot->bind_remaining(continuation,{continuation.get(),Online::service},e));
 auto foreign=std::make_shared<Online>();check(!boot->bind_remaining(foreign,{foreign.get(),Online::service},e));
 app->get_online_loading_v55()->set_is_online_game(1);Input input;input.gamepads=4;
 check(!boot->source_first_local_add_prefix(input.services(),e));check(continuation->order==std::vector<PlayerManagerOperationV1>{PlayerManagerOperationV1::network_enabled});
 check(boot->phase()==PlayerManagerBootstrapPhaseV59::first_local_failed&&*boot->count_field()==0);
 continuation->fail_at=PlayerManagerOperationV1::session_ready;continuation->order.clear();
 check(!boot->source_first_local_add_prefix(input.services(),e));check(continuation->order==std::vector<PlayerManagerOperationV1>({PlayerManagerOperationV1::network_enabled,PlayerManagerOperationV1::session_ready}));
 continuation->fail_at=PlayerManagerOperationV1::network_get_info;continuation->order.clear();
 check(!boot->source_first_local_add_prefix(input.services(),e));check(continuation->order==std::vector<PlayerManagerOperationV1>({PlayerManagerOperationV1::network_enabled,PlayerManagerOperationV1::session_ready,PlayerManagerOperationV1::session_active}));
 check(e.find("PlayerInfo178/1a0/670")!=std::string::npos);check(*boot->count_field()==0);
 app->get_online_loading_v55()->set_is_online_game(0);std::int32_t count{};check(boot->manager()->num_players(count,e)&&count==0);
}
}
int main(){try{fresh();internal_zero_nonlocal();selected_slot_authored_callback_route();adoption();changed_loan_owner();online();std::cout<<"{\"status\":\"PASS\",\"checks\":"<<checks<<",\"whole_AddCharacter\":false,\"whole_InputManager\":false}\n";}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}

