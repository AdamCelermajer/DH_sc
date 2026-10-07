#pragma once
#include "level_source_loading_v43.hpp"
#include "lifecycle_v36.hpp"
#include <captured_menu_lease_v101.hpp>
#include <climits>
#include <exception>
#include <functional>
#include <utility>
namespace dh2::loader {
struct TerminalPlayerBorrowV97 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 const std::uintptr_t* character660{};
 std::function<bool(std::uint8_t&,std::string&)> ready545;
 std::function<bool(bool&,std::string&)> is_host;
};
struct TerminalLoadingServicesV97 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uint32_t,std::string&)> current;
 std::function<bool(std::string&)> trace;
 std::function<bool(bool&,std::string&)> online;
 std::function<bool(std::int32_t,bool,TerminalPlayerBorrowV97&,std::string&)> local_player;
 std::function<bool(bool&,std::string&)> local_hosting,all_loading_done,all_clients_ready;
 std::function<bool(std::uint8_t&,std::string&)> network_initialized1ac;
 std::function<bool(std::string&)> network_init;
 std::function<bool(float,std::string&)> update_objects;
 std::function<bool(const TerminalPlayerBorrowV97&,std::string&)> try_quest_sync;
 std::function<bool(const char*,ui::CapturedMenuLeaseV101&,std::string&)> capture_menu;
 std::function<bool(const char*,std::uintptr_t&,std::string&)> lookup_menu;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(const char*,std::string&)> menu_debug_set_text;
 std::function<bool(bool,std::int32_t&,std::string&)> num_local_players;
 std::function<bool(std::uintptr_t,std::string&)> unblock_controller;
 std::function<bool(std::string&)> place_faery_followers_null;
};
//Whole supplied ELF ShowMemoryStats31041c: BX LR. No fabricated memory stats.
inline void source_show_memory_stats_v97(const char*)noexcept{}
class TerminalLoadingV97 final {
 TerminalLoadingServicesV97 services_;bool busy_{},failed_{};std::string error_;
 // Services use optional std::function; captured receiver adapters are mandatory lambdas.
 template<class R,class...P>static bool available(const std::function<R(P...)>& fn)noexcept{return static_cast<bool>(fn);}
 template<class F>static bool available(const F&)noexcept{return true;}
 template<class F,class...A>bool call(const char* leaf,const F& fn,A&&... args){
  if(failed_)return false;
  // A native callback may reenter this owner. Keep its first latched failure
  // separate from the outer callback's mutable error receipt.
  std::string receipt;bool accepted=false;
  try{if(available(fn))accepted=fn(std::forward<A>(args)...,receipt);}
  catch(const std::exception& ex){receipt=ex.what();}
  catch(...){receipt=std::string("Terminal loading callback threw: ")+leaf;}
  if(failed_)return false;
  if(!accepted){error_=receipt.empty()?std::string("Required terminal loading source leaf: ")+leaf:std::move(receipt);failed_=true;return false;}
  return true;
 }
 LifecycleStepV36 fail(std::string& e){failed_=true;if(error_.empty())error_="Actual terminal source body failed";e=error_;return LifecycleStepV36::failed;}
 bool network_prefix(){std::uint8_t initialized{};return call("actualmanager1ac",services_.network_initialized1ac,initialized)&&(initialized||call("ObjectManager.NetworkInitLevel340be0",services_.network_init));}
public:
 explicit TerminalLoadingV97(TerminalLoadingServicesV97 services):services_(std::move(services)){}
 static bool bind(TerminalLoadingServicesV97 services,SourceLoadingInputsV43& inputs,std::string& e){
  if(inputs.external.stage_body[36]&&inputs.external.stage_body[37]){e.clear();return true;}
  if(!services.owner||!services.current){e="Required independent terminal loading source owner";return false;}
  auto actual=std::make_shared<TerminalLoadingV97>(std::move(services));
  if(!inputs.external.stage_body[36])inputs.external.stage_body[36]=[actual](auto& e){return actual->step(36,e);};
  if(!inputs.external.stage_body[37])inputs.external.stage_body[37]=[actual](auto& e){return actual->step(37,e);};
  inputs.resource_pins.push_back(actual);e.clear();return true;
 }
 LifecycleStepV36 step(std::uint32_t stage,std::string& e){
  if(failed_)return fail(e);if(busy_){error_="Terminal loading source reentered";return fail(e);}
  struct Busy{bool& b;Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} busy(busy_);
  if(!call("SAME actual source stage",services_.current,stage)||!call("isTracingLevel_Loading",services_.trace))return fail(e);
  if(stage==36){bool online{};if(!call("GetOnline.byte5",services_.online,online))return fail(e);
   if(!online){e.clear();return LifecycleStepV36::pending;}
   TerminalPlayerBorrowV97 local;std::uint8_t ready{};
   if(!call("GetLocalPlayer0(false)",services_.local_player,0,false,local)||!local.receiver||!local.identity||!call("actual PlayerInfo545",local.ready545,ready))return fail(e);
   if(ready){bool hosting{},loaded{},clients_ready{};
    if(!call("IsLocalPlayerHosting",services_.local_hosting,hosting))return fail(e);if(!hosting){e.clear();return LifecycleStepV36::pending;}
    if(!call("AllLoadingDone",services_.all_loading_done,loaded))return fail(e);if(!loaded){e.clear();return LifecycleStepV36::pending;}
    if(!call("AllClientsReadyToRoll",services_.all_clients_ready,clients_ready))return fail(e);if(clients_ready){e.clear();return LifecycleStepV36::pending;}
   }
   if(!network_prefix()||!call("ObjectManager.Update(1.f)",services_.update_objects,1.f))return fail(e);
   //Original re-fetches local0 and IsHost AFTER actual object update.
   local={};bool host{};if(!call("fresh GetLocalPlayer0(false)",services_.local_player,0,false,local)||!local.receiver||!local.identity||!call("CNetPlayerInfo.IsHost80f23c",local.is_host,host))return fail(e);
   if(host){if(!local.character660){error_="Unproduced SAME PlayerInfo660";return fail(e);}if(*local.character660&&!call("actual Save14e8.SG_TryQuestSync",services_.try_quest_sync,local))return fail(e);}
   e.clear();return LifecycleStepV36::pending;
  }
  if(stage!=37){error_="Terminal body requires actual36 or37";return fail(e);}
  ui::CapturedMenuLeaseV101 loading;bool visible{};
  if(!call("GetMenuByName(menu_Loading)",services_.capture_menu,"menu_Loading",loading)||!loading.identity()){if(error_.empty())error_="Original Loading.IsVisible NULL dereference rejected";return fail(e);}
  if(!call("captured Loading.IsVisible",[&loading](bool& value,std::string& error){return loading.visible(value,error);},visible))return fail(e);
  if(visible&&!call("captured Loading.Pop",[&loading](std::string& error){return loading.pop(error);}))return fail(e);
  std::uintptr_t ignored{};if(!call("GetMenuByName(menu_HUD_0)",services_.lookup_menu,"menu_HUD_0",ignored))return fail(e);
  source_show_memory_stats_v97("After Level Loading");
  bool display{};if(!call("IsDisplayLoadingStepName",services_.debug_switch,"IsDisplayLoadingStepName",display))return fail(e);
  if(display&&!call("MenuDebug.SetText(NULL)",services_.menu_debug_set_text,static_cast<const char*>(nullptr)))return fail(e);
  std::int32_t index{},count{};
  while(true){if(!call("GetNumLocalPlayers(false)",services_.num_local_players,false,count))return fail(e);if(index>=count)break;
   TerminalPlayerBorrowV97 player;if(!call("GetLocalPlayer(index,false)",services_.local_player,index,false,player)||!player.receiver||!player.character660)return fail(e);
   if(*player.character660&&!call("SAME controller378.byte8=0",services_.unblock_controller,*player.character660))return fail(e);
   if(index==INT32_MAX){error_="Terminal local-player source index overflow";return fail(e);}++index;
  }
  bool online{};if(!call("fresh GetOnline.byte5",services_.online,online))return fail(e);if(online&&!network_prefix())return fail(e);
  if(!call("Level.PlaceFaeryAndFollowers(NULL)",services_.place_faery_followers_null)||!call("SAME actual37 before dispatcher tail",services_.current,37))return fail(e);
  e.clear();return LifecycleStepV36::complete;
 }
};
}
