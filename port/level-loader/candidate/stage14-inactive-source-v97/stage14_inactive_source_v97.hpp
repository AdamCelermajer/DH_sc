#pragma once
#include <application_services_owner_v5.hpp>
#include <functional>
#include <exception>
namespace dh2::loader {
struct Stage14MatchingBorrowV97 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 // Actual selected virtual3c LeaveRoom. bool means delivery, not its ignored
 // native result. Local full Reset/event/socket behavior is NOT identity Reset.
 std::function<bool(std::string&)> leave_room3c;
};
struct Stage14ConnectionBorrowV97 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::function<bool(std::string&)> disconnect10; // ignored native result
};
struct Stage14ConnectionManagerBorrowV97 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 // Actual current connection slot1c+4*i. Called anew AFTER previous delivery.
 std::function<bool(unsigned,Stage14ConnectionBorrowV97&,std::string&)> slot;
};
struct Stage14InactiveSourcesV97 {
 std::shared_ptr<void> owner; // independent actual process network/menu authority
 std::function<bool(Stage14MatchingBorrowV97&,std::string&)> get_matching;
 std::function<bool(Stage14ConnectionManagerBorrowV97&,std::string&)> get_connections;
 // Genuine whole Application.GoToMainMenu functional leaf, on SAME captured
 // App. Bool is delivery status; its original return is ignored by state14.
 std::function<bool(application::ApplicationServicesOwnerV5&,unsigned,std::string&)> go_to_main_menu;
};
// Narrow loader-owned original3f78f8..791c tail. No network/session/menu
// implementation, active-state substitution, App/World or connection registry.
class Stage14InactiveTailV97 final {
 Stage14InactiveSourcesV97 source_;std::weak_ptr<application::ApplicationServicesOwnerV5> expected_app_;
 Stage14MatchingBorrowV97 matching_;Stage14ConnectionManagerBorrowV97 connections_;
 Stage14ConnectionBorrowV97 connection_;
 bool attempted_{},busy_{},failed_{},matching_complete_{},menu_complete_{};
 unsigned next_slot_{};std::string failure_;
 bool fail(std::string& e,const char* leaf){failed_=true;if(failure_.empty())failure_=e.empty()?leaf:e;e=failure_;return false;}
public:
 Stage14InactiveTailV97(Stage14InactiveSourcesV97 source,
  const std::shared_ptr<application::ApplicationServicesOwnerV5>& captured):source_(std::move(source)),expected_app_(captured){}
 bool run(const std::shared_ptr<application::ApplicationServicesOwnerV5>& captured,std::string& e){
  if(failed_){e=failure_;return false;}if(busy_||attempted_)return fail(e,"Original inactive Stage14 tail cannot replay");attempted_=busy_=true;
  struct Guard{bool& busy;~Guard(){busy=false;}} guard{busy_};
  if(!source_.owner||!captured||expected_app_.lock()!=captured)return fail(e,"Required SAME captured native Application/independent tail authority");
  try{
   if(!source_.get_matching||!source_.get_matching(matching_,e)||!matching_.owner||!matching_.identity||!matching_.leave_room3c)return fail(e,"Required actual selected CMatching.Get/virtual3c LeaveRoom");
   if(failed_){e=failure_;return false;}
   if(!matching_.leave_room3c(e))return fail(e,"Required completed actual Matching virtual3c");matching_complete_=true;if(failed_){e=failure_;return false;}
   // Intentionally follows LeaveRoom even if its actual body disconnected.
   if(!source_.get_connections||!source_.get_connections(connections_,e)||!connections_.owner||!connections_.identity||!connections_.slot)return fail(e,"Required actual GetConnectionMgr receiver");
   if(failed_){e=failure_;return false;}
   while(next_slot_<32){connection_={};
    if(!connections_.slot(next_slot_,connection_,e))return fail(e,"Required SAME live connection slot1c+4*i");
    if(failed_){e=failure_;return false;}
    if(!connection_.identity){++next_slot_;continue;} // genuine native NULL slot branch
    if(!connection_.owner||!connection_.disconnect10||!connection_.disconnect10(e))return fail(e,"Required actual positive connection virtual10 Disconnect");
    ++next_slot_;if(failed_){e=failure_;return false;}
   }
   // DisconnectAll7fbe54 returns literal0; original stage ignores that value.
   if(!source_.go_to_main_menu||!source_.go_to_main_menu(*captured,3,e))return fail(e,"Required actual Application.GoToMainMenu(event3)");menu_complete_=true;if(failed_){e=failure_;return false;}
   // Native next branches3f6e90: dispatcher increments SAME Level130 even
   // after menu delivery. Do not re-query current World/online/local-active
   // here: GoToMainMenu is permitted to schedule/change current session.
  }catch(const std::exception& ex){e=ex.what();return fail(e,"Inactive Stage14 Main primitive threw");}catch(...){return fail(e,"Inactive Stage14 Main primitive threw");}
  e.clear();return true;
 }
 bool matching_completed()const noexcept{return matching_complete_;}
 unsigned completed_connection_slots()const noexcept{return next_slot_;}
 bool menu_completed()const noexcept{return menu_complete_;}
};
}
