#include "application_player_manager_bootstrap_v59.hpp"
#include "application_services_owner_v5.hpp"
#include <exception>
namespace dh2::player {
ApplicationPlayerManagerBootstrapV59::ApplicationPlayerManagerBootstrapV59(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app)
 :lifetime_(std::make_shared<ProviderLifetime>()) {lifetime_->application=app;}
bool ApplicationPlayerManagerBootstrapV59::available(std::string& e)const{
 if(!lifetime_||lifetime_->application.expired()||!actual_runtime_||!actual_network_||!actual_runtime_->manager().source_initialized_v59()){
  e="Required actual retained Application/constructed SAME PlayerManager/CNet owner";return false;
 }return true;
}
bool ApplicationPlayerManagerBootstrapV59::service(void* raw,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& out,std::string& e){
 auto& t=*static_cast<ApplicationPlayerManagerBootstrapV59*>(raw);auto app=t.lifetime_->application.lock();
 if(!app){e="Application expired before source PlayerManager service";return false;}
 ++t.service_depth_;struct Scope{std::uint32_t& depth;~Scope(){--depth;}}scope{t.service_depth_};
 using O=PlayerManagerOperationV1;
 if(q.operation==O::online_enabled){out.value=app->get_online_loading_v55()->byte5();return true;}
 if(q.operation==O::construct_player_info){
  if(!q.player||!t.actual_network_||t.actual_network_->borrow(*q.player)||t.buffers_.count(q.player)){
   e="Required fresh SAME PlayerInfo constructor receiver; no constructor replay";return false;
  }
  // Existing typed CNet constructor/reset successor; packet/NetStruct
  // serialization is not invented. Source Reset80f27c precedes scalar Reset.
  if(!t.actual_network_->construct(*q.player,t.lifetime_,e))return false;
  // Derived PlayerInfo C1 publishes its actual selected vptr after CNet C2;
  // slot5c is36d48c, with the genuine offline literal-true source branch.
  if(!t.actual_network_->publish_player_info_dispatch_v67(*q.player,e))return false;
  *q.player=PlayerInfoFieldsV1{}; // source Reset373bdc scalar stores, including660=0/664=-1.
  if(!t.actual_network_->source_reset_player_class_v68(*q.player,e))return false;
  auto buffers=std::make_shared<PlayerInfoSkillBuffersV26>();
  t.buffers_.emplace(q.player,buffers); // retain reached prefix on failure
  return buffers->construct(*q.player,*t.actual_network_,e);
 }
 if(t.remaining_.invoke&&t.remaining_provider_)
  return t.remaining_.invoke(t.remaining_.context,q,out,e);
 e="Required original Application PlayerManager continuation "+std::to_string(std::uint32_t(q.operation));return false;
}
bool ApplicationPlayerManagerBootstrapV59::create_fresh(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,
 MatchingLocalSelectionOwnerV4& matching,std::shared_ptr<ApplicationPlayerManagerBootstrapV59>& out,std::string& e){
 if(!app||app->source_player_manager_v59()){e="Application already has a PlayerManager; borrow/adopt SAME owner";return false;}
 auto t=std::shared_ptr<ApplicationPlayerManagerBootstrapV59>(new ApplicationPlayerManagerBootstrapV59(app));
 t->network_=std::make_unique<PlayerNetworkLocalOwnerV4>(matching);
 t->runtime_=std::make_unique<PlayerManagerCombatRuntimeV2>(PlayerManagerCombatServicesV2{t.get(),service});
 t->actual_runtime_=t->runtime_.get();t->actual_network_=t->network_.get();
 out=t; // caller retains failed constructor prefix; never silently retries it
 if(!t->runtime_->initialize()){t->phase_=PlayerManagerBootstrapPhaseV59::constructor_failed;e=t->runtime_->error();t->error_=e;return false;}
 t->phase_=PlayerManagerBootstrapPhaseV59::constructed;e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::adopt_existing(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,
 std::unique_ptr<PlayerManagerCombatRuntimeV2>& runtime,std::unique_ptr<PlayerNetworkLocalOwnerV4>& network,
 PlayerManagerCombatServicesV2 expected,std::shared_ptr<void> prior,
 std::shared_ptr<ApplicationPlayerManagerBootstrapV59>& out,std::string& e){
 if(!app||app->source_player_manager_v59()||!runtime||!network||!prior||prior.get()!=expected.context||runtime->source_transport_active_v59()||
    !runtime->manager().source_initialized_v59()){
  e="Required unpublished App and idle actual previous PM/network/provider for adoption";return false;
 }
 PlayerInfoFieldsV1* dummy{};
 if(!runtime->manager().get_by_internal(-1,false,dummy,e)||!dummy||!network->borrow(*dummy)){
  if(e.empty())e="Previous manager lacks SAME produced dummy CNet constructor owner";return false;
 }
 auto t=std::shared_ptr<ApplicationPlayerManagerBootstrapV59>(new ApplicationPlayerManagerBootstrapV59(app));
 // Rebinding is guarded by the runtime's actual callback depth/manager guard
 // and expected old provider. No source C1/AddPlayer/Reset is executed here.
 if(!runtime->rebind_source_services_v59(expected,{t.get(),service},e))return false;
 t->predecessor_provider_=std::move(prior);t->runtime_=std::move(runtime);t->network_=std::move(network);
 t->actual_runtime_=t->runtime_.get();t->actual_network_=t->network_.get();
 t->phase_=PlayerManagerBootstrapPhaseV59::adopted;out=std::move(t);e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::network_enabled(bool& enabled,std::string& e){
 enabled=false;PlayerManagerResponseV1 out;
 for(auto operation:{PlayerManagerOperationV1::online_enabled,PlayerManagerOperationV1::network_enabled,
                    PlayerManagerOperationV1::session_ready,PlayerManagerOperationV1::session_active}){
  if(!service(this,{operation},out,e))return false;if(!out.value)return true;
 }enabled=true;return true;
}
bool ApplicationPlayerManagerBootstrapV59::source_first_local_add_prefix(const FirstLocalControllerServicesV59& s,std::string& e){
 if(!available(e)||busy_||service_depth_||actual_runtime_->source_transport_active_v59()||!s.provider||!s.count||!s.controller_zero){
  if(e.empty())e="Required idle App PM and actual first-controller source providers";return false;
 }
 busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}}scope{busy_};
 std::int32_t count{};if(!s.count(s.context,count,e))return false;
 const auto source_count=count<1?1:count;(void)source_count; // original378cbc..d4; first index remains0
 FirstLocalControllerBorrowV59 controller;
 if(!s.controller_zero(s.context,controller,e)||!controller.identity||!controller.receiver||!controller.connected758){
  if(e.empty())e="Required real InputManager GetGamepad(0)/byte758";return false;
 }
 observed_controller_connected_=*controller.connected758; // original378d38 read precedes forced first-local1
 bool online{};if(!network_enabled(online,e)){phase_=PlayerManagerBootstrapPhaseV59::first_local_failed;error_=e;return false;}
 if(online){e="Required whole online first-controller PlayerInfo178/1a0/670 producer";phase_=PlayerManagerBootstrapPhaseV59::first_local_failed;error_=e;return false;}
 // Original378d58->IsPlayerInLocalMap36d280; existing branch378cfc
 // borrows GetPlayerByInternalID(false) and reads slot664. Do not replay
 // even the duplicate _AddPlayer path on an existing receiver.
 if(actual_runtime_->manager().is_player_in_local_map_v59(0)){
  PlayerInfoFieldsV1* current{};
  if(!actual_runtime_->manager().get_by_internal(0,false,current,e)||!current||current->internal670!=0){
   if(e.empty())e="Required SAME existing local-map PlayerInfo0";
   phase_=PlayerManagerBootstrapPhaseV59::first_local_failed;error_=e;return false;
  }
  // IDA's _CheckLocalControllers first tests IsPlayerInLocalMap(0), then
  // skips _AddPlayer when that map key already exists. GetLocalPlayer(0,false)
  // independently enumerates only PlayerInfo records with local66c set. A
  // non-local record under internal key 0 therefore makes the source prefix
  // finish without producing a local player; surface that exact mismatch here
  // rather than reporting a successful first-local setup and failing later
  // when NativeAssignSaveSlotToPlayer resolves the PlayerManager sentinel.
  if(!current->local66c){
   auto& manager=actual_runtime_->manager();
   std::int32_t total=-1,local_any=-1,local_with_character=-1;
   const bool have_total=manager.num_players(total,e);
   std::string count_error=e;e.clear();
   const bool have_local_any=manager.num_local_players(false,local_any,e);
   if(!e.empty()){if(!count_error.empty())count_error+="; ";count_error+="local_count="+e;}e.clear();
   const bool have_local_with_character=manager.num_local_players(true,local_with_character,e);
   if(!e.empty()){if(!count_error.empty())count_error+="; ";count_error+="local_with_character="+e;}e.clear();
   PlayerInfoFieldsV1* resolved{};const bool resolved_ok=manager.get_local_player(0,false,resolved,e);
   if(!e.empty()){if(!count_error.empty())count_error+="; ";count_error+="get_local="+e;}e.clear();
   PlayerInfoFieldsV1* sentinel{};const bool sentinel_ok=manager.get_by_internal(-1,false,sentinel,e);
   if(!e.empty()){if(!count_error.empty())count_error+="; ";count_error+="sentinel="+e;}e.clear();
   e="First-local setup skipped AddPlayer because internal ID 0 is mapped to a non-local PlayerInfo"
     " {mapped internal670="+std::to_string(current->internal670)+
     ", local66c="+std::to_string(current->local66c)+
     ", save_slot664="+std::to_string(current->save_slot664)+
     "; total_players="+(have_total?std::to_string(total):"unavailable")+
     "; local_players_no_character="+(have_local_any?std::to_string(local_any):"unavailable")+
     "; local_players_with_character="+(have_local_with_character?std::to_string(local_with_character):"unavailable")+
     "; get_local_0_false="+(resolved_ok?"ok":"failed")+
     ", same_as_sentinel="+((resolved_ok&&sentinel_ok&&resolved==sentinel)?"true":"false")+
     ", internal670="+(resolved_ok&&resolved?std::to_string(resolved->internal670):"unavailable")+
     ", local66c="+(resolved_ok&&resolved?std::to_string(resolved->local66c):"unavailable")+
     ", save_slot664="+(resolved_ok&&resolved?std::to_string(resolved->save_slot664):"unavailable")+
     "; sentinel_lookup="+(sentinel_ok?"ok":"failed")+
     ", internal670="+(sentinel_ok&&sentinel?std::to_string(sentinel->internal670):"unavailable")+
     ", local66c="+(sentinel_ok&&sentinel?std::to_string(sentinel->local66c):"unavailable")+
     ", save_slot664="+(sentinel_ok&&sentinel?std::to_string(sentinel->save_slot664):"unavailable")+
     (count_error.empty()?"":"; query_errors="+count_error)+"}";
   phase_=PlayerManagerBootstrapPhaseV59::first_local_failed;error_=e;return false;
  }
  profile_input_pending_=current->save_slot664==-1;
  phase_=PlayerManagerBootstrapPhaseV59::first_local_retained;e.clear();return true;
 }
 // Original378d6c/70 ->378e3c/40/48/4c ->_AddPlayer378e50:
 // index0 internal0, controller-owner -1, local-index0, true. Not the
 // development launch tuple {0,0,0,true}, and not selected profile slot.
 if(!actual_runtime_->manager().add_player(0,-1,0,true,e)){phase_=PlayerManagerBootstrapPhaseV59::first_local_failed;error_=e;return false;}
 profile_input_pending_=false;phase_=PlayerManagerBootstrapPhaseV59::first_local_added;e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::publish_selected_save_slot_v67(std::int32_t index,std::int32_t slot,std::string& e){
 if(!available(e)||busy_||service_depth_||actual_runtime_->source_transport_active_v59()||index<0||slot<0||slot>=4){
  if(e.empty())e="Required idle actual PlayerManager and valid selected profile slot";return false;
 }
 bool online{};if(!network_enabled(online,e))return false;
 if(online){e="Selected local profile requires actual offline PlayerInfo";return false;}
 PlayerInfoFieldsV1* player{};
 if(!actual_runtime_->manager().get_local_player(index,false,player,e))return false;
 if(!player){e="Selected slot requires an actual local PlayerInfo receiver";return false;}
 if(!player->local66c||player->internal670<0){
  auto& manager=actual_runtime_->manager();std::int32_t total=-1,locals=-1;
  const bool have_total=manager.num_players(total,e);std::string query_errors=e;e.clear();
  const bool have_locals=manager.num_local_players(false,locals,e);
  if(!e.empty()){if(!query_errors.empty())query_errors+="; ";query_errors+="local_count="+e;}e.clear();
  PlayerInfoFieldsV1* internal_zero{};const bool have_zero=manager.get_by_internal(0,false,internal_zero,e);
  if(!e.empty()){if(!query_errors.empty())query_errors+="; ";query_errors+="internal0="+e;}e.clear();
  PlayerInfoFieldsV1* sentinel{};const bool have_sentinel=manager.get_by_internal(-1,false,sentinel,e);
  if(!e.empty()){if(!query_errors.empty())query_errors+="; ";query_errors+="sentinel="+e;}e.clear();
  e="Selected slot resolved to the PlayerManager sentinel/non-local PlayerInfo"
    " {local_index="+std::to_string(index)+", selected_slot="+std::to_string(slot)+
    ", total_players="+(have_total?std::to_string(total):"unavailable")+
    ", local_players="+(have_locals?std::to_string(locals):"unavailable")+
    ", resolved_internal670="+std::to_string(player->internal670)+
    ", resolved_local66c="+std::to_string(player->local66c)+
    ", resolved_save_slot664="+std::to_string(player->save_slot664)+
    ", same_as_sentinel="+((have_sentinel&&sentinel==player)?"true":"false")+
    ", internal0="+(have_zero&&internal_zero?std::to_string(internal_zero->internal670):"unavailable")+
    ", internal0_local="+(have_zero&&internal_zero?std::to_string(internal_zero->local66c):"unavailable")+
    ", internal0_slot="+(have_zero&&internal_zero?std::to_string(internal_zero->save_slot664):"unavailable")+
    (query_errors.empty()?"":"; query_errors="+query_errors)+"}";return false;
 }
 if(player->character660){
  e="Selected slot reached after Character660 publication; preserve the live Character owner";return false;
 }
 if(player->save_slot664!=-1&&player->save_slot664!=slot){
  e="A different profile already owns this PlayerInfo; require actual player removal";return false;
 }
 player->save_slot664=slot;profile_input_pending_=false;e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::assign_selected_save_slot_v70(std::int32_t index,std::int32_t slot,
 const FirstLocalControllerServicesV59& controllers,std::string& e){
 if(!source_first_local_add_prefix(controllers,e))return false;
 return publish_selected_save_slot_v67(index,slot,e);
}
bool ApplicationPlayerManagerBootstrapV59::publish_selected_profile_v68(std::int32_t index,std::int32_t slot,std::int32_t selected_class,std::string& e){
 if(selected_class<0){e="Required actual selected-profile PCLS row";return false;}
 if(!publish_selected_save_slot_v67(index,slot,e))return false;
 PlayerInfoFieldsV1* player{};if(!get_local_player(index,false,player,e)||!player||!actual_network_){if(e.empty())e="Required SAME local PlayerInfo after selected664 publication";return false;}
 return actual_network_->source_selected_profile_class_v68(*player,selected_class,e);
}
bool ApplicationPlayerManagerBootstrapV59::bind_remaining(std::shared_ptr<void> owner,PlayerManagerServicesV1 s,std::string& e){
 if(!available(e)||busy_||service_depth_||actual_runtime_->source_transport_active_v59()||!owner||!s.invoke){if(e.empty())e="Required idle actual PM remaining-provider lifetime";return false;}
 if(remaining_provider_&&(remaining_provider_.get()!=owner.get()||remaining_.context!=s.context||remaining_.invoke!=s.invoke||
    remaining_provider_.owner_before(owner)||owner.owner_before(remaining_provider_))){e="PlayerManager continuation belongs to a different source owner";return false;}
 remaining_provider_=std::move(owner);remaining_=s;e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::bind_campaign_remaining(const std::shared_ptr<void>& campaign,
 std::shared_ptr<void> provider,PlayerManagerServicesV1 s,
 std::function<bool(std::string&)> retired,std::string& e){
 if(!available(e)||busy_||service_depth_||actual_runtime_->source_transport_active_v59()||
    !campaign||!provider||s.context!=provider.get()||!s.invoke||!retired||
    (!provider.owner_before(campaign)&&!campaign.owner_before(provider))){
  if(e.empty())e="Required idle SAME PM and independent actual campaign continuation";return false;
 }
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(remaining_provider_&&same(remaining_provider_,provider)&&remaining_.context==s.context&&
    remaining_.invoke==s.invoke&&same(remaining_campaign_.lock(),campaign)){e.clear();return true;}
 if(remaining_provider_&&(!remaining_campaign_retired_||same(remaining_campaign_.lock(),campaign))){
  e="PlayerManager continuation requires a different campaign after authentic prior retirement";return false;
 }
 // Pin the old descriptor while checking its weak source-slot witness. Reentry
 // cannot replace it, and a failed validator leaves the whole binding intact.
 auto previous=remaining_provider_;
 busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}}scope{busy_};
 if(previous){
  try{if(!remaining_campaign_retired_(e)){if(e.empty())e="Prior PlayerManager campaign has not retired";return false;}}
  catch(const std::exception& failure){e=failure.what();return false;}
  catch(...){e="PlayerManager campaign retirement witness threw";return false;}
 }
 remaining_campaign_=campaign;remaining_campaign_retired_=std::move(retired);
 remaining_=s;remaining_provider_=std::move(provider);e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::bind_existing_buffers(std::shared_ptr<void> owner,
 std::function<bool(PlayerInfoFieldsV1&,std::shared_ptr<PlayerInfoSkillBuffersV26>&,std::string&)> get,std::string& e){
 if(!available(e)||busy_||service_depth_||actual_runtime_->source_transport_active_v59()||!owner||!get||predecessor_buffers_){if(e.empty())e="Required once-bound actual previous PlayerInfo buffer owner";return false;}
 predecessor_buffers_provider_=std::move(owner);predecessor_buffers_=std::move(get);e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::get_local_player(std::int32_t index,bool character,PlayerInfoFieldsV1*& out,std::string& e){
 if(!available(e))return false;return actual_runtime_->manager().get_local_player(index,character,out,e);
}
bool ApplicationPlayerManagerBootstrapV59::get_by_internal(std::int32_t id,bool character,PlayerInfoFieldsV1*& out,std::string& e){
 if(!available(e))return false;return actual_runtime_->manager().get_by_internal(id,character,out,e);
}
bool ApplicationPlayerManagerBootstrapV59::source_is_local_player_v61(std::uintptr_t character,bool& out,std::string& e){
 out=false;if(!character){e.clear();return true;} // original36f004->36f020, no manager/source queries
 if(!available(e))return false;PlayerInfoFieldsV1* actual{};
 if(!actual_runtime_->manager().get_by_character(character,false,actual,e))return false;
 if(!actual){e="Required actual source PlayerInfo/dummy virtual50 receiver";return false;}
 // Actual PlayerInfo and CNetPlayerInfo tables both select80f1ec. Existing
 // V4 owner reads SAME owner1a0 and invokes SAME Matching source bodies.
 return actual_network_->is_local(*actual,out,e);
}
bool ApplicationPlayerManagerBootstrapV59::skill_buffers(PlayerInfoFieldsV1& info,std::shared_ptr<PlayerInfoSkillBuffersV26>& out,std::string& e){
 if(!available(e))return false;auto found=buffers_.find(&info);
 if(found!=buffers_.end()){out=found->second;return true;}
 if(predecessor_buffers_&&predecessor_buffers_provider_){
  if(!predecessor_buffers_(info,out,e))return false;
  if(!out||out->receiver()!=&info){e="Previous skill buffers do not belong to SAME PlayerInfo";return false;}return true;
 }
 e="Required observed previous PlayerInfo skill buffers; adoption never replays Reset";return false;
}
bool ApplicationPlayerManagerBootstrapV59::lend_to_runtime(std::unique_ptr<PlayerManagerCombatRuntimeV2>& target,
 std::unique_ptr<PlayerNetworkLocalOwnerV4>& network,std::function<void()>& callback,std::string& e){
 if(!available(e)||busy_||service_depth_||loaned_||!runtime_||!network_||target||network||actual_runtime_->source_transport_active_v59()){
  if(e.empty())e="Required idle SAME App PM ownership and empty runtime fields before loan";return false;
 }
 auto self=shared_from_this();
 // Callback allocation precedes the noexcept ownership moves. Existing source
 // scalar fields/map/CNet receivers and all actual heap addresses stay equal.
 std::function<void()> returned=[self,&target,&network](){std::string error;if(!self->return_loan(target,network,error))self->error_=error;};
 target=std::move(runtime_);network=std::move(network_);loaned_=true;callback=std::move(returned);e.clear();return true;
}
bool ApplicationPlayerManagerBootstrapV59::return_loan(std::unique_ptr<PlayerManagerCombatRuntimeV2>& target,
 std::unique_ptr<PlayerNetworkLocalOwnerV4>& network,std::string& e){
 if(!loaned_||runtime_||network_||!actual_runtime_||!actual_network_||target.get()!=actual_runtime_||network.get()!=actual_network_||
    busy_||service_depth_||actual_runtime_->source_transport_active_v59()){
  // Runtime fields are about to disappear. Future App getters must fail
  // rather than dereference stale observed pointers after a changed owner.
  actual_runtime_=nullptr;actual_network_=nullptr;loaned_=false;
  e="PlayerManager loan owner changed/was active before return; publication invalidated";return false;
 }
 runtime_=std::move(target);network_=std::move(network);loaned_=false;e.clear();return true;
}
}
