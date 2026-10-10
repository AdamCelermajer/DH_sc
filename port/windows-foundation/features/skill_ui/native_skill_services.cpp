#include "native_skill_services.hpp"
#include "../../../engine-ui/character_menu_actions_owner_v1.hpp"
#include <limits>
#include <cmath>
#include <cstring>

namespace dh::foundation::skill_ui {
namespace {
bool fail(std::string& e,const std::string& message){e=message;return false;}
std::int32_t signed_word(std::uint32_t value){std::int32_t out;std::memcpy(&out,&value,sizeof(out));return out;}
bool details_for(dh2::ui::CharacterMenuQueriesOwnerV1& queries,int position,int player,NativeDetails& out,std::string& e){
 return query_native_skill_details(queries,position,player,out,e);
}
bool same_tables(const dh2::data::SkillTables::Borrow& a,const dh2::data::SkillTables::Borrow& b){
 if(!a||!b||a.lists()!=b.lists()||a.list_names()!=b.list_names()||a.skill_names()!=b.skill_names()||a.skills().size()!=b.skills().size())return false;
 for(std::size_t i=0;i<a.skills().size();++i){const auto& x=a.skills()[i];const auto& y=b.skills()[i];
  if(std::memcmp(x.scalar.words,y.scalar.words,sizeof(x.scalar.words))||x.display_props!=y.display_props||x.script!=y.script||x.icon!=y.icon)return false;
 }
 return true;
}
}

bool bind_live_player_services(dh2::ui::CharacterMenuActionsOwnerV1& actions,
 dh2::ui::CharacterMenuQueriesOwnerV1& queries,dh2::data::SkillTables::Borrow tables,
 const LiveServicesInputV1& input,Services& out,std::string& error){
 error.clear();
 if(!input.symbol_text)return fail(error,"Skill UI: source StringManager symbol resolver required for native skill headings");
 const auto& graph=actions.bindings();if(!graph.owner||!graph.equipment||!graph.skills||!graph.save||!graph.skill_tables||!graph.skill_list_index)
  return fail(error,"Skill UI: existing same-player CharacterMenuActionsOwnerV1 graph required");
 if(!same_tables(tables,graph.skill_tables))return fail(error,"Skill UI: presenter and live character-menu actions do not share the same original SkillTables");
 if(!actions.validate_graph(true,error))return false;
 Services next;
 next.character=[&actions](int& list,int& points,std::string& e){
  if(!actions.validate_graph(true,e))return false;const auto& g=actions.bindings();
  if(!g.skill_list_index||!g.save)return fail(e,"Skill UI: live class skill-list or saved-level authority unavailable");
  list=*g.skill_list_index;return actions.skill_points(points,e);
 };
 next.progress=[&actions](int position,Progress& result,std::string& e){
  if(position<0)return fail(e,"Skill UI: negative class skill position");
  if(!actions.validate_graph(true,e))return false;const auto& g=actions.bindings();
  if(static_cast<std::size_t>(position)>=g.save->skills().size())return fail(e,"Skill UI: live save has no class-position skill row");
  const int saved_level=static_cast<int>(g.save->skill_level(static_cast<std::uint32_t>(position)));
  std::string ignored;const auto* skill=actions.skill_record(position,ignored);if(!skill)return fail(e,ignored);
  const int required_level=signed_word(skill->scalar.words[8]);const bool available=(g.equipment->properties()->resolved[19]>>8)>=required_level;
  const bool assignable=skill->scalar.words[11]!=0;bool can_increment;
  if(!actions.can_increment(static_cast<std::uint32_t>(position),can_increment,e))return false;
  result={saved_level,available,can_increment,assignable&&saved_level>0};return true;
 };
 next.slots=[&actions](std::vector<int>& slots,std::string& e){
  if(!actions.validate_graph(true,e))return false;std::vector<int> fresh;
  if(!actions.append_equipped_skills([&](int slot,std::string&){fresh.push_back(slot);return true;},e))return false;
  if(fresh.size()!=3)return fail(e,"Skill UI: source player did not expose all three saved skill slots");slots=std::move(fresh);return true;
 };
 next.increment=[&actions](int position,bool source_flag,std::string& e){
  if(source_flag)return fail(e,"Skill UI: source IncSkill flag has no bound native caller contract");
  if(position<0)return fail(e,"Skill UI: negative class skill position");int points_left=0;
  return actions.train_skill(position,points_left,e);
 };
 next.probe_increment=[&actions](int position,bool& accepted,std::string& e){
  if(position<0)return fail(e,"Skill UI: negative class skill position");
  return actions.probe_train_skill(position,accepted,e);
 };
 next.assign=[&actions](int slot,int position,std::string& e){
  if(slot<0||position<0)return fail(e,"Skill UI: negative source slot/skill position");
  return actions.equip_skill(slot,position,e);
 };
 next.selected_texts=[&queries,symbol=input.symbol_text,player=input.player_index](int position,std::vector<std::pair<std::string,std::string>>& result,std::string& e){
  if(position<0)return fail(e,"Skill UI: select a source class skill before requesting its details");NativeDetails details;
  if(!details_for(queries,position,player,details,e))return false;std::vector<std::pair<std::string,std::string>> fresh;
  for(const auto& field:character_menu::original_menu_art(character_menu::Tab::skills).text_fields){
   if(field.path.find("/SKILL_NAME/")==std::string::npos&&field.path.find("/skill_description/")==std::string::npos&&
      field.path.find("/current_skill_description/")==std::string::npos&&field.path.find("/next_skill_description/")==std::string::npos)continue;
   std::string text;if(!native_skill_field_text(details,field.path,symbol,text,e))return false;fresh.emplace_back(field.path,std::move(text));
  }
  result=std::move(fresh);return true;
 };
 next.source_class_frame=[source=input.source_class_frame,&actions](unsigned& frame,std::string& e){
  // The exact native frame branch starts from SG_GetPlayerClass on this same
  // selected Character. CharacterTable.SkillTree stays diagnostic only:
  // GetCharSkillListId reads the current Character property and may reflect a
  // live override independent of the saved class row.
  if(!actions.validate_graph(true,e))return false;const auto& g=actions.bindings();
  if(!g.save)return fail(e,"Skill UI: live saved class identity unavailable");unsigned resolved=0;
  if(!original_class_frame_for_row(g.save->class_id(),resolved,e))return false;
  if(source){unsigned native_frame=0;if(!source(native_frame,e))return false;
   if(native_frame>2||native_frame!=resolved)return fail(e,"Skill UI: native class-frame result differs from original class-row branch");}
  frame=resolved;return true;
 };
 next.icon=input.icon;out=std::move(next);return true;
}
}
