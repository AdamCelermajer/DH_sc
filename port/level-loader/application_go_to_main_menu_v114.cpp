#include "application_go_to_main_menu_v114.hpp"
#include <exception>
#include <utility>
namespace dh2::loader {
AreaTransitionStepV114 ApplicationGoToMainMenuV114::fail(const std::string& e,const char* why){
 if(first_failure_.empty())first_failure_=e.empty()?why:e;error_=first_failure_;failed_phase_=phase_;phase_=Phase::failed;return AreaTransitionStepV114::failed;
}
bool ApplicationGoToMainMenuV114::begin(std::int32_t event,ApplicationMainMenuServicesV114 services,std::string& e){
 if(busy_){if(reentry_.empty())reentry_="Application.GoToMainMenu reentered";if(first_failure_.empty())first_failure_=reentry_;e=first_failure_;return false;}
 if(phase_!=Phase::idle||!services.owner||!services.application||!services.current_application||!services.cleared_process_flags||!services.current_level){
  e="Require one original GoToMainMenu request and actual process/Level loans";return false;
 }
 event_=event;services_=std::move(services);phase_=Phase::app;e.clear();return true;
}
AreaTransitionStepV114 ApplicationGoToMainMenuV114::drain(std::string& e){
 using R=AreaTransitionStepV114;using P=Phase;
 if(busy_){e.clear();return R::pending;}
 if(phase_==P::failed){e=error_;return R::failed;}if(phase_==P::complete){e.clear();return R::complete;}
 if(phase_==P::idle){e="Require accepted original GoToMainMenu request";return R::failed;}
 busy_=true;reentry_.clear();struct Busy{bool& value;~Busy(){value=false;}} busy{busy_};
 auto reject=[&](const std::string& why,const char* fallback){fail(why,fallback);e=error_;return false;};
 auto valid=[&]{
  if(!reentry_.empty())return reject(reentry_,"GoToMainMenu nested failure");
  if(app_.identity){std::string local;
   if(!services_.current_application(app_,local))return reject(local,"GoToMainMenu process App changed");
   if(!reentry_.empty())return reject(reentry_,"GoToMainMenu nested failure");
   //SwitchState legitimately retires old World. Before it, captured saving
   //Level must remain the SAME native receiver, not just a surviving wrapper.
   if(level_.identity&&phase_<P::switch_state){MainMenuLevelBorrowV114 current;
    if(!services_.current_level(current,local)||current.identity!=level_.identity||!current.owner||
       current.owner.owner_before(level_.owner)||level_.owner.owner_before(current.owner))return reject(local,"Captured saving Level replaced/retired");
    if(!reentry_.empty())return reject(reentry_,"GoToMainMenu nested failure");
   }
  }return true;
 };
 auto loan=[&](const char* why,const auto& fn,auto&&... args){
  if(!valid())return false;if(!fn)return reject({},why);std::string local;
  if(!fn(std::forward<decltype(args)>(args)...,local))return reject(local,why);
  if(!reentry_.empty())return reject(reentry_,"GoToMainMenu nested loan failure");return true;
 };
 auto deliver=[&](const char* why,const auto& fn,P next,auto&&... args){
  if(!valid())return false;if(!fn)return reject({},why);std::string local;
  if(!fn(std::forward<decltype(args)>(args)...,local))return reject(local,why);
  //Commit genuine completed effects before callback/scope rejection.
  phase_=next;return valid();
 };
 auto branch=[&](const char* why,const auto& fn,auto& value,P no,P yes){
  if(!valid())return false;if(!fn)return reject({},why);std::string local;
  if(!fn(value,local))return reject(local,why);
  phase_=value?yes:no;return valid();
 };
 try{
  while(phase_!=P::complete){
   switch(phase_){
    case P::app:{
     if(!loan("Required SAME process Application loan",services_.application,app_))return R::failed;
     if(!app_.owner||!app_.identity||!app_.byte_ab||!app_.event_b0){reject({},"Required actual App ab/b0 cells");return R::failed;}
     if(!app_.owner.owner_before(services_.owner)&&!services_.owner.owner_before(app_.owner)){reject({},"GoToMainMenu services must independently own process leaves");return R::failed;}
     phase_=P::flags;break;
    }
    case P::flags:{std::uint8_t* first{};std::uint8_t* second{};std::shared_ptr<void> pin;
     if(!loan("Required actual two cleared process flag cells",services_.cleared_process_flags,first,second,pin))return R::failed;
     if(!pin||!first||!second){reject({},"Required SAME native process flag lease");return R::failed;}if(!valid())return R::failed;*first=0;*second=0;phase_=P::level;break;
    }
    case P::level:{
     if(!loan("Required actual Application.GetCurrentLevel",services_.current_level,level_))return R::failed;
     if(level_.identity){if(!level_.owner||!level_.state130||!level_.dungeon198||!level_.saving145){reject({},"Required SAME native Level source fields");return R::failed;}
     }
     if(!valid())return R::failed;phase_=P::manager;
     if(level_.identity&&*level_.state130==38)phase_=*level_.dungeon198?P::save_flag:P::map;break;
    }
    case P::map:if(!deliver("Required actual App54 map objectf4 virtual38(true)",services_.map_object_virtual38_true,P::save_flag))return R::failed;break;
    case P::save_flag:if(!valid())return R::failed;*level_.saving145=1;phase_=P::quick_save;break;
    case P::quick_save:if(!deliver("Required actual Level.QuickSave(false)",services_.quick_save,P::save_player,level_,false))return R::failed;break;
    case P::save_player:if(!deliver("Required actual Level.SG_SaveLocalPlayer(false)",services_.save_local_player,P::manager,level_,false))return R::failed;break;
    case P::manager:
     if(!loan("Required actual captured MenuManager",services_.menu_manager,manager_pin_,manager_))return R::failed;
     if(!manager_pin_||!manager_){reject({},"Required SAME native MenuManager pin");return R::failed;}if(!valid())return R::failed;phase_=P::reset;break;
    case P::reset:if(!deliver("Required actual MenuManager.Reset",services_.reset_menu_manager,P::debug_before_menu,manager_))return R::failed;break;
    case P::debug_before_menu:if(!deliver("Required original DEBUG_OUT32c25c",services_.debug,P::menu,0x32c25cu))return R::failed;break;
    case P::menu:if(!deliver("Required actual source GetMenuByName",services_.get_menu,P::state,manager_,"menu_MainMenu",menu_))return R::failed;break;
    case P::state:
     if(!loan("Required SAME GS menu-state c/29 loan",services_.menu_state,state_))return R::failed;
     if(!state_.owner||!state_.identity||!state_.menu_c||!state_.source29){reject({},"Required actual GS menu-state fields");return R::failed;}
     if(!valid())return R::failed;*state_.source29=1;*state_.menu_c=menu_;phase_=P::switch_state;break;
    case P::switch_state:{
     if(!valid())return R::failed;if(!services_.switch_state){reject({},"Required actual StateMachine.SwitchState(menu,false)");return R::failed;}
     std::string local;const auto r=services_.switch_state(app_,state_,false,local);
     if(r==R::failed){reject(local,"Actual menu-state transition failed");return R::failed;}
     if(r==R::complete)phase_=P::debug_score;
     if(!valid())return R::failed;if(r==R::pending){e.clear();return R::pending;}break;
    }
    case P::debug_score:if(!deliver("Required original DEBUG_OUT32c298",services_.debug,P::score,0x32c298u))return R::failed;break;
    case P::score:
     if(!services_.submit_online_scores){phase_=P::trophies;break;}
     if(!deliver("Required actual Application.SendGLHiScore",services_.send_gl_high_score,P::trophies,app_))return R::failed;break;
    case P::trophies:if(!deliver("Required actual TrophyManager.UnlockTrophiesGLLive",services_.unlock_trophies_gl_live,P::debug_players))return R::failed;break;
    case P::debug_players:if(!deliver("Required original DEBUG_OUT32c2bc",services_.debug,P::players,0x32c2bcu))return R::failed;break;
    case P::players:if(!deliver("Required actual PlayerManager.RemoveAllPlayers",services_.remove_all_players,P::online))return R::failed;break;
    case P::online:{std::uint8_t online{};
     if(!branch("Required actual GetOnline byte5",services_.online_byte5,online,P::debug_status,P::signin))return R::failed;break;
    }
    case P::signin:{std::uint8_t active{};
     if(!branch("Required actual CSignIn byte11",services_.signin_byte11,active,P::matching_query,P::signin_leave))return R::failed;break;
    }
    case P::signin_leave:if(!deliver("Required actual fresh CSignIn virtual14",services_.signin_virtual14,P::matching_query))return R::failed;break;
    case P::matching_query:{bool active{};
     if(!branch("Required actual CMatching virtual64",services_.matching_virtual64,active,P::matching_provider,P::matching_leave))return R::failed;break;
    }
    case P::matching_leave:if(!deliver("Required actual fresh CMatching virtual3c",services_.matching_virtual3c,P::matching_provider))return R::failed;break;
    case P::matching_provider:if(!deliver("Required actual CMatching.SetMatchingProvider(1)",services_.matching_set_provider,P::matching_destroy,std::int32_t{1}))return R::failed;break;
    case P::matching_destroy:if(!deliver("Required actual fresh CMatching.Destroy",services_.matching_destroy,P::online_clear))return R::failed;break;
    case P::online_clear:if(!deliver("Required actual COnline.SetIsOnlineGame(false)",services_.online_set_is_online,P::online_state_clear,false))return R::failed;break;
    case P::online_state_clear:{std::uint8_t* field{};std::shared_ptr<void> pin;
     if(!loan("Required actual OnlineGameState28 cell",services_.online_game_state28,field,pin))return R::failed;
     if(!field||!pin){reject({},"Required SAME OnlineGameState28 lease");return R::failed;}if(!valid())return R::failed;*field=0;phase_=P::center_clear;break;
    }
    case P::center_clear:{std::uint8_t* field{};std::shared_ptr<void> pin;
     if(!loan("Required actual GameCenter11 cell",services_.game_center11,field,pin))return R::failed;
     if(!field||!pin){reject({},"Required SAME GameCenter11 lease");return R::failed;}if(!valid())return R::failed;*field=0;phase_=P::debug_status;break;
    }
    case P::debug_status:if(!deliver("Required original DEBUG_OUT32c2e4",services_.debug,P::status,0x32c2e4u))return R::failed;break;
    case P::status:if(!deliver("Required actual Application.ShowStatubBar(false)",services_.show_status_bar,P::final_fields,app_,false))return R::failed;break;
    case P::final_fields:if(!valid())return R::failed;*app_.byte_ab=1;*app_.event_b0=event_;phase_=P::debug_end;break;
    case P::debug_end:if(!deliver("Required original DEBUG_OUT32c30c",services_.debug,P::complete,0x32c30cu))return R::failed;break;
    default:reject({},"Invalid retained GoToMainMenu source phase");return R::failed;
   }
  }e.clear();return R::complete;
 }catch(const std::exception& ex){reject(ex.what(),"GoToMainMenu native leaf threw");return R::failed;}
 catch(...){reject({},"GoToMainMenu native leaf threw; retained source prefix");return R::failed;}
}
bool ApplicationGoToMainMenuV114::resume(std::string& e){
 if(busy_||phase_!=Phase::failed||failed_phase_==Phase::idle||failed_phase_==Phase::failed){e=error_.empty()?"Require failed SAME GoToMainMenu native continuation":error_;return false;}
 //Main must explicitly resume any latched native V88/StateMachine provider
 //first. This clears only handler latch, never native error/readiness state.
 phase_=failed_phase_;error_.clear();reentry_.clear();e.clear();return true;
}
bool fs_go_to_main_menu_v114(ApplicationGoToMainMenuV114& handler,ApplicationMainMenuServicesV114 services,
 const char* method,const char* argument,void* userdata,std::int32_t& source_result,std::string& e){
 (void)method;(void)argument;(void)userdata;source_result=0;
 if(!handler.begin(0,std::move(services),e))return false;source_result=1;return true;
}
}
