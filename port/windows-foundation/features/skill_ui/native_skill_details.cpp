#include "native_skill_details.hpp"
namespace dh::foundation::skill_ui {
bool query_native_skill_details(dh2::ui::CharacterMenuQueriesOwnerV1& owner,int position,int player,NativeDetails& out,std::string& e){
 e.clear();if(position<0){e="Skill UI: NativeGetSkillDetails requires current class position";return false;}
 using V=dh2::ui::CharacterMenuValueV1;dh2::ui::CharacterMenuCallV1 call;NativeDetails projected;
 // Local semantic result object, scoped to this call; never an engine pointer.
 constexpr std::uintptr_t object=1;call.arguments={V::numeric(position),V::reference(object),V::numeric(player)};
 call.member=[&](std::uintptr_t target,const char* name,const V& value,std::string& error){if(target!=object||!name){error="Skill UI: native details write outside result object";return false;}projected[name]=value;return true;};
 if(!owner.dispatch("NativeGetSkillDetails",call,e))return false;
 for(const char* key:{"SkillName","SkillDescription","SkillCurrLevel","SkillNextLevel","SkillAssignable","SkillIcon","SkillLevel","SkillAssignedToSlot","SkillUnlocked"})if(!projected.count(key)){e="Skill UI: native skill details missing source member "+std::string(key);return false;}
 out=std::move(projected);return true;
}
bool native_skill_field_text(const NativeDetails& details,const std::string& path,const SymbolText& symbol,std::string& out,std::string& e){
 e.clear();std::string key,heading;
 if(path.find("/SKILL_NAME/")!=std::string::npos)key="SkillName";
 else if(path.find("/current_skill_description/")!=std::string::npos){key="SkillCurrLevel";heading="GAMEPLAYMENUS_current_level";}
 else if(path.find("/next_skill_description/")!=std::string::npos){key="SkillNextLevel";heading="GAMEPLAYMENUS_next_level";}
 else if(path.find("/skill_description/")!=std::string::npos)key="SkillDescription";
 else {e="Skill UI: source field is not native skill description";return false;}
 auto it=details.find(key);if(it==details.end()){e="Skill UI: missing NativeGetSkillDetails member "+key;return false;}
 if(it->second.kind!=4){e="Skill UI: native skill description member is not AS string";return false;}
 std::string value;if(!heading.empty()){if(!symbol){e="Skill UI: unsupported NativeGetStringFromSymbol heading provider";return false;}if(!symbol(heading,value,e))return false;value+="\n\n";}value+=it->second.text;out=std::move(value);return true;
}
}
