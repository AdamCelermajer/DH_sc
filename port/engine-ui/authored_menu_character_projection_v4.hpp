#pragma once
#include "authored_menu_deadzones_v3.hpp"
#include "swf_actionscript_connection.hpp"
#include "menu_stack_v1.hpp"
#include <memory>
namespace dh2::ui {
// flags8 reaches Gameloft's additional focus-enabled field. Stock GameSWF
// cannot supply it; the actual source getter is mandatory only on that branch.
bool authored_menu_stack_character_v4(gameswf::as_object*,std::uint32_t render_flags,
 const std::function<bool(gameswf::as_object*,bool&,std::string&)>& source_focus,
 MenuStackCharacterV1&,std::string&);
// Whole RenderFX::PlayAnim7aba04/GotoFrame7ab924: sprite check, actual
// goto_labeled_frame result, then PLAY only on successful label resolution.
bool authored_menu_play_animation_v4(SwfAsGraph&,const SwfAsValue&,const char*,bool&,std::string&);
// Same root movie definition's authored frame rectangle, in source twips.
bool authored_menu_movie_rect_v4(SwfAsGraph&,float out[4],std::string&);
// Scope-local projection for RegisterDeadZones' exact flags=0 domain. The
// legacy sprite focus flag is not read by that path and is not reconstructed.
class AuthoredMenuCharacterProjectionV4 {
 std::vector<std::unique_ptr<AuthoredMenuCharacterBorrowV3>> nodes_;
 bool append(gameswf::as_object*,AuthoredMenuCharacterBorrowV3*&,std::string&);
public:
 bool root(SwfAsGraph&,const std::string& actual_menu,AuthoredMenuCharacterBorrowV3*&,std::string&);
 bool absolute_bounds(AuthoredMenuCharacterBorrowV3&,AuthoredMenuDeadZoneV3&,std::string&);
};
}
