#pragma once
#include "application_go_to_main_menu_v114.hpp"
#include <memory>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace model_renderer {
//SAME current native GS Level loan; true+empty is authentic NULL currentLevel.
bool source_main_menu_level_v114(dh2::loader::MainMenuLevelBorrowV114&,std::string&);
bool source_main_menu_quick_save_v114(const dh2::loader::MainMenuLevelBorrowV114&,bool,std::string&);
bool source_main_menu_save_local_v114(const dh2::loader::MainMenuLevelBorrowV114&,bool,std::string&);
//Process PM survives old World retirement; no lookup of released source World.
bool source_main_menu_remove_players_v114(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
//App54->f4 virtual38(true) is parent's existing process shared_stack_v27
//pop_current(true, actual roster stack_services), not another World/map route.
}