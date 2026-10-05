#pragma once
#include "character_menu_queries_owner_v1.hpp"
#include <functional>
#include <memory>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
// Typed boundary for the authored menu's real ActionScript callbacks. The
// caller installs it through SwfServices.native_action, inside that movie's
// existing core Scope. The retained provider must not own the movie graph.
class CharacterMenuAsBridgeV1 final {
public:
 using Dispatch=std::function<bool(const char*,CharacterMenuCallV1&,std::string&)>;
 explicit CharacterMenuAsBridgeV1(std::shared_ptr<void> game_owner,Dispatch);
 CharacterMenuAsBridgeV1(const CharacterMenuAsBridgeV1&)=delete;
 CharacterMenuAsBridgeV1& operator=(const CharacterMenuAsBridgeV1&)=delete;
 bool dispatch(const char*,const gameswf::fn_call&,std::string&)const;
private:
 std::shared_ptr<void> game_owner_;
 Dispatch dispatch_;
};
}
