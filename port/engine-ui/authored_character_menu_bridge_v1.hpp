#pragma once
#include "character_menu_as_bridge_v1.hpp"
#include "character_menu_reload_action_v1.hpp"
#include "menu_stack_actions_v1.hpp"
#include "menu_rollover_input_v1.hpp"

namespace dh2::ui {
enum class AuthoredCharacterMenuRouteV1 { query, reload, navigation, rollover, application };
// Borrow these exact existing owners. This bridge creates neither a player nor
// a second menu stack. All borrows survive until the actual movie is released.
struct AuthoredCharacterMenuBindingsV1 {
 std::shared_ptr<void> owner;
 CharacterMenuQueriesOwnerV1* queries{};
 CharacterMenuReloadActionV1* reload{};
 MenuStackActionsV1* navigation{};
 MenuRolloverInputV1* rollover{};
 // Original settings, localization, tutorial, audio, quest/map, multiplayer and
 // platform callbacks belong to their real application owners. Reached absence
 // is an error, including when the callback does not write an AS result.
 CharacterMenuAsBridgeV1::Dispatch application;
};
class AuthoredCharacterMenuBridgeV1 final {
 AuthoredCharacterMenuBindingsV1 bindings_;
 CharacterMenuAsBridgeV1 bridge_;
 bool route(const char*,CharacterMenuCallV1&,std::string&);
public:
 explicit AuthoredCharacterMenuBridgeV1(AuthoredCharacterMenuBindingsV1);
 bool dispatch(const char*,const gameswf::fn_call&,std::string&)const;
 static AuthoredCharacterMenuRouteV1 route_for(const char*) noexcept;
 // Includes source startup callbacks plus shared-library startup settings and
 // string localization. Register BEFORE loading the original shared/root SWFs.
 static std::vector<std::string> native_actions();
};
}
