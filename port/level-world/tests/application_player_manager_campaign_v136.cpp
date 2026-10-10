#include "../application_player_manager_bootstrap_v59.hpp"
#include "../application_services_owner_v5.hpp"
#include "../source_input_manager_v60.hpp"
#include "../player_manager_update_owner_v70.hpp"
#include "../../level-loader/stage_loader_v50_game_events.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::player;
using namespace dh2::loader;
namespace {
unsigned checks{};
void check(bool value){if(!value)throw std::runtime_error("campaign continuation check "+std::to_string(checks));++checks;}
// Fixture native World/Character endpoints. Real App/PM/PlayerInfo, PM Update,
// Stage6, event storage and RemoveAllCharacters kernels run below. This does
// not claim a renderer, complete Character construction or whole GS retirement.
struct Campaign {
 std::shared_ptr<int> loading=std::make_shared<int>(1),gs=std::make_shared<int>(2),level=std::make_shared<int>(3);
 std::uint32_t state130=6;std::uintptr_t field194{};
 std::shared_ptr<GameEventManagerV50> events;
};
struct Continuation {
 std::weak_ptr<ApplicationPlayerManagerBootstrapV59> pm;
 std::shared_ptr<Campaign> campaign; // Deliberate completed Spawn-prefix pin.
 std::uintptr_t character{};unsigned adds{},removes{},updates{};
 std::vector<PlayerManagerUpdateStageV70> stages;
 std::unique_ptr<PlayerManagerUpdateOwnerV70> update;
 std::shared_ptr<Campaign> attempted_campaign;
 std::shared_ptr<Continuation> attempted_provider;
 bool active_rebind_refused{};
 static bool invoke(void* raw,const PlayerManagerRequestV1& q,PlayerManagerResponseV1&,std::string& error){
  auto& self=*static_cast<Continuation*>(raw);auto same=self.pm.lock();
  if(!same||!q.player||q.character_count6c4!=same->manager()->character_count_field())return false;
  if(self.attempted_provider){
   self.active_rebind_refused=!self.bind(same,self.attempted_campaign,self.attempted_provider,error);
   if(!self.active_rebind_refused)return false;error.clear();
  }
  if(q.operation==PlayerManagerOperationV1::character_initialization){
   if(q.player->character660||*q.character_count6c4)return false;
   q.player->character660=self.character;++*q.character_count6c4;++self.adds;return true;
  }
  if(q.operation==PlayerManagerOperationV1::remove_character){
   if(q.player->character660!=self.character||*q.character_count6c4!=1)return false;
   q.player->character660=0;--*q.character_count6c4;++self.removes;return true;
  }
  error="Unexpected fixture continuation operation";return false;
 }
 static bool bind(const std::shared_ptr<ApplicationPlayerManagerBootstrapV59>& pm,
  const std::shared_ptr<Campaign>& campaign,const std::shared_ptr<Continuation>& provider,std::string& error){
  std::weak_ptr<Campaign> weak=campaign;
  return pm->bind_campaign_remaining(campaign,provider,{provider.get(),invoke},[weak](std::string& e){
   auto old=weak.lock();if(old&&(old->loading||old->gs||old->level)){e="Fixture prior native wrappers still owned";return false;}
   e.clear();return true;
  },error);
 }
 static std::shared_ptr<Continuation> create(const std::shared_ptr<ApplicationPlayerManagerBootstrapV59>& pm,
  const std::shared_ptr<Campaign>& campaign,std::uintptr_t identity){
  auto out=std::make_shared<Continuation>();out->pm=pm;out->campaign=campaign;out->character=identity;
  PlayerManagerUpdateServicesV70 s;s.provider=std::make_shared<std::weak_ptr<Continuation>>(out);
  std::weak_ptr<Continuation> weak=out;
  s.stage=[weak](auto stage,auto& manager,std::string& e){auto self=weak.lock();
   if(!self||!self->pm.lock()||self->pm.lock()->manager()!=&manager)return false;
   self->stages.push_back(stage);
   if(stage==PlayerManagerUpdateStageV70::manage_characters)return manager.add_character(0,e);
   return true;
  };
  out->update=std::make_unique<PlayerManagerUpdateOwnerV70>(*pm->manager(),std::move(s));return out;
 }
};
void stage6(const std::shared_ptr<ApplicationPlayerManagerBootstrapV59>& pm,
 const std::shared_ptr<Campaign>& scope,const std::shared_ptr<Continuation>& continuation){
 std::string e;
 GameEventTablesV50 tables;
 // One actual typed event row, no Objective callback fixtures are needed.
 const std::uint8_t rows[]{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 const std::uint8_t names[]{1,0,0,0,1,0,0,0,'E'};
 check(tables.initialize(rows,sizeof rows,names,sizeof names,e));
 Stage6ServicesV50 s;s.actual_events=tables.borrow();
 s.debug={scope,[](const char* key,bool& value,std::string&){value=false;return std::string(key)=="isTracingLevel_Loading";}};
 auto* manager=pm->manager();auto* fields=manager->source_frame_fields_v68();
 s.player={std::shared_ptr<void>(pm,manager),reinterpret_cast<std::uintptr_t>(manager),&fields->byte6c9};
 s.player_update=[continuation,manager](const Stage6PlayerBorrowV50& b,std::string& e){
  if(b.identity!=reinterpret_cast<std::uintptr_t>(manager)||b.actual_owner.get()!=manager||*b.byte6c9!=1)return false;
  ++continuation->updates;return continuation->update->update(e);
 };
 auto body=stage6_game_events_v50({scope,reinterpret_cast<std::uintptr_t>(scope.get()),&scope->state130,&scope->field194,&scope->events},std::move(s));
 LifecycleStepV36 result=LifecycleStepV36::pending;
 for(unsigned i=0;i<20&&result==LifecycleStepV36::pending;++i)result=body(e);
 check(result==LifecycleStepV36::complete&&scope->state130==6&&scope->field194==reinterpret_cast<std::uintptr_t>(scope->events.get()));
 check(scope->events->diagnostics().storage_load_complete&&continuation->updates==1&&continuation->adds==1);
 const std::vector<PlayerManagerUpdateStageV70> expected={PlayerManagerUpdateStageV70::check_online_transition,
  PlayerManagerUpdateStageV70::check_local_controllers,PlayerManagerUpdateStageV70::check_remote_controllers,
  PlayerManagerUpdateStageV70::manage_characters,PlayerManagerUpdateStageV70::check_local_deaths,
  PlayerManagerUpdateStageV70::check_global_deaths,PlayerManagerUpdateStageV70::statistics_update};
 check(continuation->stages==expected);
}
void run(){
 auto app=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();
 MatchingLocalSelectionOwnerV4 matching;std::shared_ptr<ApplicationPlayerManagerBootstrapV59> pm;std::string e;
 check(ApplicationPlayerManagerBootstrapV59::create_fresh(app,matching,pm,e));check(app->publish_source_player_manager_v59(pm,e));
 auto input=std::make_shared<dh2::input::SourceInputManagerV60>();check(pm->source_first_local_add_prefix(input->first_local_services(),e));
 PlayerInfoFieldsV1* player{};check(pm->get_local_player(0,false,player,e));check(pm->publish_selected_save_slot_v67(0,1,e));
 const auto* manager=pm->manager();const auto* network=pm->network();const auto* record=player;
 auto first=std::make_shared<Campaign>();auto old=Continuation::create(pm,first,0x1001);
 check(Continuation::bind(pm,first,old,e));
 auto second=std::make_shared<Campaign>();auto next=Continuation::create(pm,second,0x2002);
 // A new owner cannot take over a live World, even when no PM call runs.
 check(!Continuation::bind(pm,second,next,e));check(!Continuation::bind(pm,first,next,e));
 check(!pm->bind_remaining(next,{next.get(),Continuation::invoke},e));
 old->attempted_campaign=second;old->attempted_provider=next;
 stage6(pm,first,old);check(old->active_rebind_refused);
 check(pm->get_local_player(0,false,player,e)&&player==record&&player->character660==0x1001&&*pm->count_field()==1);
 old->attempted_provider.reset();old->attempted_campaign.reset();
 check(pm->manager()->remove_all_characters_v114(e));check(old->removes==1&&player->character660==0&&*pm->count_field()==0);
 check(first->events->destroy_native_storage_v1({},e));first->events.reset();first->field194=0;
 first->loading.reset();check(!Continuation::bind(pm,second,next,e));
 first->gs.reset();check(!Continuation::bind(pm,second,next,e));
 first->level.reset();
 // The old Spawn bag intentionally pins World: replacing it must use actual
 // retired-wrapper proof, rather than require weak World expiration.
 std::weak_ptr<Campaign> retired_world=first;std::weak_ptr<Continuation> retired_provider=old;
 first.reset();old.reset();check(!retired_world.expired()&&!retired_provider.expired());
 check(Continuation::bind(pm,second,next,e));check(retired_world.expired()&&retired_provider.expired());
 check(pm->manager()==manager&&pm->network()==network&&app->source_player_manager_v59()==pm);
 check(pm->get_local_player(0,false,player,e)&&player==record&&player->save_slot664==1&&player->character660==0);
 stage6(pm,second,next);
 check(pm->get_local_player(0,false,player,e)&&player==record&&player->character660==0x2002&&*pm->count_field()==1);
 check(pm->manager()->remove_all_characters_v114(e));check(next->removes==1&&player->character660==0);
 check(second->events->destroy_native_storage_v1({},e));
}
}
int main(){try{run();std::cout<<"PASS campaign launch/Stage6/retire/relaunch: "<<checks<<" checks; SAME App/PM/PlayerInfo, retired provider dropped, active takeover rejected; native World/Character endpoints are fixtures\n";}
 catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
