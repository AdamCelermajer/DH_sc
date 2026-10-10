#pragma once
#include "authored_menu_localization_v1.hpp"
#include "authored_menu_weak_character_v59.hpp"
namespace dh2::ui {
// Borrows the registered movie state. The catalog is the SAME MenuFX catalog;
// this connector owns neither a second stack nor gameplay state.
struct AuthoredCharacterStateV1 {
 AuthoredMenuFieldsV1* fields{};
 AuthoredMenuWeakCharacterV59 weak_character_v59;
 // Resolve the SAME weak MenuBase context on each call. There is no persistent
 // strong AS handle which could mask the original weak-reference expiration.
 std::function<bool(SwfAsValue&,std::string&)> resolve_context;
};
struct AuthoredCharacterRegistrationServicesV1 {
 std::shared_ptr<void> owner;
 std::uintptr_t render{};
 std::function<bool(AuthoredMenuFieldsV1&,std::string&)> append_state;
 // Exact RenderFX::Find, including current-context/root fallback.
 std::function<bool(const char*,SwfAsValue&,bool&,std::string&)> find;
 std::function<bool(AuthoredMenuFieldsV1&,const SwfAsValue&,std::string&)> bind_weak_context;
 // Actual virtual Create. MenuBase implementation is empty; concrete overrides
 // must be supplied by their original owner, not replaced by a no-op here.
 std::function<bool(AuthoredMenuFieldsV1&,std::string&)> create;
 bool source_reassignment_v67{};
};
// MenuFX::RegisterState(7adf50) prefix, followed by RegisterMenu's valid write.
// A failed lookup retains the reached catalog mutation and leaves valid zero.
bool authored_character_register_state_v1(SwfAsGraph&,AuthoredCharacterStateV1&,
 const char* explicit_name,const AuthoredCharacterRegistrationServicesV1&,std::string&);
// Concrete per-movie callbacks for lifecycle services localize/set_visible/
// invoke_as. Other application/stack/debug operations remain required callers.
bool authored_character_movie_operation_v1(SwfAsGraph&,AuthoredCharacterStateV1&,
 const AuthoredMenuRequestV1&,const AuthoredMenuLocalizationServicesV1&,std::string&);
// MenuBase.Show425640..425684 refreshes option_Custom in the SAME weak menu
// context, then queries actual Application.IsLevelRunning before visibility.
bool authored_character_option_custom_level_running_v1(SwfAsGraph&,AuthoredCharacterStateV1&,
 const char*,const std::function<bool(bool&,std::string&)>& actual_level_running,std::string&);
}
