#pragma once
#include "authored_menu_lifecycle_v1.hpp"
#include "hud_text_v1.hpp"
#include "swf_actionscript_connection.hpp"
namespace dh2::ui {
struct AuthoredMenuSearchEntryV1 {std::string name,path;SwfAsValue character;};
// Original RenderFX SearchIndex: actual named live characters, depth-first
// display order and source substring matching, not generic find_target.
class AuthoredMenuSearchIndexV1 {
 std::vector<AuthoredMenuSearchEntryV1> entries_;
public:
 bool initialize(SwfAsGraph&,const SwfAsValue& context,std::string&);
 bool find(const std::string&,SwfAsValue&,bool& found,std::string&)const;
 const std::vector<AuthoredMenuSearchEntryV1>& entries()const noexcept{return entries_;}
};
struct AuthoredMenuLocalizationServicesV1 {
 HudTextV1* text{};LocalizationServices strings;
 // Complete actual DebugSwitches.load+GetSwitch, source ignores result.
 std::function<bool(const char*,std::string&)> debug;
 // Publish THIS RenderFX's actual context before initializing SearchIndex.
 std::function<bool(const SwfAsValue&,std::string&)> set_context;
};
// Called inside the exact movie Scope. Null context is the original early
// return and does not publish localized75. All character/font/layout writes
// occur on the actual graph, using retained source edit-text implementation.
bool authored_menu_process_localization_v1(SwfAsGraph&,AuthoredMenuFieldsV1&,
 const SwfAsValue& context,const AuthoredMenuLocalizationServicesV1&,std::string&);
}
