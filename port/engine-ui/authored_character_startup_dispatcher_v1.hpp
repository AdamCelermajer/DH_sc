#pragma once
#include "authored_character_application_v1.hpp"
#include "authored_character_menu_bridge_v1.hpp"
namespace dh2::ui {
struct AuthoredCharacterStartupBindingsV1 {
 std::shared_ptr<void> owner;
 // Fresh same-player borrow, such as CharacterPanelSession::dispatch. This
 // runs during source movie load as well as after publication; never JSON.
 CharacterMenuAsBridgeV1::Dispatch queries;
 CharacterMenuReloadActionV1* reload{};
 MenuStackActionsV1* navigation{};
 MenuRolloverInputV1* rollover{};
 AuthoredCharacterApplicationV1* application{};
 CharacterMenuAsBridgeV1::Dispatch required_application;
};
class AuthoredCharacterStartupDispatcherV1 {
 AuthoredCharacterStartupBindingsV1 bindings_;
 CharacterMenuAsBridgeV1 bridge_;
 std::vector<std::string> reached_;
 bool route(const char*,CharacterMenuCallV1&,std::string&);
public:
 explicit AuthoredCharacterStartupDispatcherV1(AuthoredCharacterStartupBindingsV1);
 bool dispatch(const char*,const gameswf::fn_call&,std::string&)const;
 const std::vector<std::string>& reached_callbacks()const noexcept{return reached_;}
};
}
