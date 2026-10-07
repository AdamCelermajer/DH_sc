#pragma once
#include "area_transition_sequence_v114.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::loader {
struct MainMenuApplicationBorrowV114 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::uint8_t* byte_ab{};std::int32_t* event_b0{};
};
struct MainMenuLevelBorrowV114 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::uint32_t* state130{};const std::uint8_t* dungeon198{};std::uint8_t* saving145{};
};
struct MainMenuStateBorrowV114 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::uintptr_t* menu_c{};std::uint8_t* source29{};
};
struct ApplicationMainMenuServicesV114 {
 // Modern offline delivery policy. Default preserves the recovered routine;
 // a native offline build explicitly defers legacy online score submission.
 bool submit_online_scores{true};
 std::shared_ptr<void> owner; //Independent services, actual App/World weak.
 std::function<bool(MainMenuApplicationBorrowV114&,std::string&)> application;
 //Validates SAME process App only; authentic SwitchState retires old World.
 std::function<bool(const MainMenuApplicationBorrowV114&,std::string&)> current_application;
 //Outputs actual Is_In_Multiplayer_IGM then Is_In_Multiplayer_Mode, in source write order.
 std::function<bool(std::uint8_t*&,std::uint8_t*&,std::shared_ptr<void>&,std::string&)> cleared_process_flags;
 std::function<bool(MainMenuLevelBorrowV114&,std::string&)> current_level; //True+identity0 is real NULL.
 std::function<bool(std::string&)> map_object_virtual38_true; //actual App54->f4->virtual38(true).
 std::function<bool(const MainMenuLevelBorrowV114&,bool,std::string&)> quick_save,save_local_player;
 std::function<bool(std::shared_ptr<void>&,std::uintptr_t&,std::string&)> menu_manager;
 std::function<bool(std::uintptr_t,std::string&)> reset_menu_manager;
 std::function<bool(unsigned source_debug_call,std::string&)> debug;
 std::function<bool(std::uintptr_t,const char*,std::uintptr_t&,std::string&)> get_menu;
 std::function<bool(MainMenuStateBorrowV114&,std::string&)> menu_state;
 //Actual StateMachine.SwitchState(state,false). Main composes existing V88
 //quiescence/GS retirement/front state entry. Pending resumes that SAME native
 //continuation; this handler never creates another destructor/menu controller.
 std::function<AreaTransitionStepV114(const MainMenuApplicationBorrowV114&,const MainMenuStateBorrowV114&,bool,std::string&)> switch_state;
 std::function<bool(const MainMenuApplicationBorrowV114&,std::string&)> send_gl_high_score;
 std::function<bool(std::string&)> unlock_trophies_gl_live,remove_all_players;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5,signin_byte11;
 std::function<bool(std::string&)> signin_virtual14,matching_virtual3c,matching_destroy;
 std::function<bool(bool&,std::string&)> matching_virtual64;
 std::function<bool(std::int32_t,std::string&)> matching_set_provider;
 std::function<bool(bool,std::string&)> online_set_is_online;
 std::function<bool(std::uint8_t*&,std::shared_ptr<void>&,std::string&)> online_game_state28,game_center11;
 std::function<bool(const MainMenuApplicationBorrowV114&,bool,std::string&)> show_status_bar;
};
//Portable original Application.GoToMainMenu32c1f4. Retains actual source loans
//through native callbacks and failed prefixes. No new menu, Save, GS or player.
class ApplicationGoToMainMenuV114 final {
 enum class Phase:std::uint8_t {idle,app,flags,level,map,save_flag,quick_save,save_player,manager,reset,debug_before_menu,menu,state,switch_state,debug_score,score,trophies,debug_players,players,online,signin,signin_leave,matching_query,matching_leave,matching_provider,matching_destroy,online_clear,online_state_clear,center_clear,debug_status,status,final_fields,debug_end,complete,failed};
 Phase phase_{Phase::idle},failed_phase_{Phase::idle};bool busy_{};std::int32_t event_{};
 ApplicationMainMenuServicesV114 services_;MainMenuApplicationBorrowV114 app_;MainMenuLevelBorrowV114 level_;
 MainMenuStateBorrowV114 state_;std::shared_ptr<void> manager_pin_;std::uintptr_t manager_{},menu_{};
 std::string error_,first_failure_,reentry_;
 AreaTransitionStepV114 fail(const std::string&,const char*);
public:
 bool begin(std::int32_t original_event,ApplicationMainMenuServicesV114,std::string&);
 AreaTransitionStepV114 drain(std::string&);
 //Explicit native provider retry; preserves completed source effects.
 bool resume(std::string&);
 const std::string& first_failure()const noexcept{return first_failure_;}
};
//FS420c3c ignores both strings/userdata, invokes App event0, and source returns1.
//Parent routes this acceptance after authentic FS delivery; no automatic exit.
bool fs_go_to_main_menu_v114(ApplicationGoToMainMenuV114&,ApplicationMainMenuServicesV114,
 const char*,const char*,void*,std::int32_t& source_result,std::string&);
}
