#pragma once
#include "../../../engine-ui/character_menu_queries_owner_v1.hpp"
#include <map>
namespace dh::foundation::skill_ui {
using NativeDetails=std::map<std::string,dh2::ui::CharacterMenuValueV1>;
// Uses the existing NativeGetSkillDetails owner/temporary binding and actual
// localized scripts. This owns only one invocation's AS result projection.
bool query_native_skill_details(dh2::ui::CharacterMenuQueriesOwnerV1&,int class_position,int player_index,NativeDetails&,std::string&);
// Source20f3c..20fc7 combines localized heading with native current/next text.
// Symbol callback must use the actual StringManager, not English literals.
using SymbolText=std::function<bool(const std::string&,std::string&,std::string&)>;
bool native_skill_field_text(const NativeDetails&,const std::string& source_path,const SymbolText&,std::string&,std::string&);
}
