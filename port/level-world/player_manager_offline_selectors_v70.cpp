#include "player_manager_offline_selectors_v70.hpp"
namespace dh2::player {
std::shared_ptr<PlayerManagerOfflineSelectorsV70> process_player_manager_offline_selectors_v70(){
 static const auto actual=std::make_shared<PlayerManagerOfflineSelectorsV70>();return actual;
}
}
