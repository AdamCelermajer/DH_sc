#include "hud_initialization_owned_v1.hpp"
#include "../level-world/character_level.hpp"
#include <limits>
#include <stdexcept>
namespace dh2::ui {
namespace {std::int32_t signed_word(std::uint32_t u){return u<=0x7fffffffu?static_cast<std::int32_t>(u):-1-static_cast<std::int32_t>(~u);}}
int hud_initialization_settings_v1_query(const OwnedHudSettingsV1&settings,const HudInitRequest64&q,HudInitResponse32&r){
 using Op=HudInitOperation;if((q.operation==Op::option_current||q.operation==Op::option_maximum||q.operation==Op::option_string_id)&&!q.text)return 0;if(q.operation==Op::option_current){r.value=settings.option(q.text);return 1;}
 if(q.operation!=Op::option_maximum&&q.operation!=Op::option_string_id)return -1;
 auto*row=settings.descriptor(q.text);if(!row){r.value=-1;return 1;}
 r.value=q.operation==Op::option_maximum?(row->type==2?signed_word(static_cast<std::uint32_t>(row->maximum)-1u):row->maximum):signed_word(static_cast<std::uint32_t>(row->value_string)+static_cast<std::uint32_t>(settings.option(q.text)));return 1;
}
HudInitSkillCatalogV1::HudInitSkillCatalogV1(data::SkillTables::Borrow tables):tables_(std::move(tables)){
 if(!tables_)throw std::invalid_argument("Missing actual SkillTables borrow");
 const auto& rows=tables_.skills();records_.reserve(rows.size());
 for(const auto& row:rows){const auto*w=row.scalar.words;records_.push_back({row.display_props.data(),static_cast<std::uint32_t>(row.display_props.size()),0,signed_word(w[8]),signed_word(w[12]),signed_word(w[13]),signed_word(w[16]),signed_word(w[17]),static_cast<std::uint8_t>(w[6]),static_cast<std::uint8_t>(w[11]),0,row.icon.c_str(),reinterpret_cast<std::uintptr_t>(&row),{0,0,0,0,0}});}
}
const HudInitSkill96*HudInitSkillCatalogV1::character_skill(std::int32_t list,std::uint32_t row)const noexcept{
 const auto&lists=tables_.lists();if(list<0||static_cast<std::size_t>(list)>=lists.size())list=3;
 if(static_cast<std::size_t>(list)>=lists.size()||row>=lists[list].size())return nullptr;
 auto id=lists[list][row];return id<0||static_cast<std::size_t>(id)>=records_.size()?nullptr:&records_[id];
}
int hud_initialization_owned_v1_query(const HudInitPlayerProjectionV1&p,const HudInitRequest64&q,HudInitResponse32&r,std::string&e){
 using Op=HudInitOperation;switch(q.operation){case Op::skill_slot:case Op::skill_id:case Op::character_skill:case Op::character_level:case Op::skill_level:case Op::unlocked_difficulty:case Op::can_increment:case Op::current_faery:case Op::faery_level:break;default:return -1;}
 if(!p.identity||q.subject!=p.identity){e="HUD source actor projection mismatch";return 0;}
 auto valid_saved_row=[&](){return !p.saved||q.index<p.saved->skills().size();};
 auto skill=[&]()->const HudInitSkill96*{if(!p.skills||!p.properties||dh2_property_validate(p.properties))return nullptr;return p.skills->character_skill(p.properties->resolved[28],q.index);};
 switch(q.operation){
  case Op::skill_slot:if(q.other==0){r.value=p.saved?p.saved->skill_in_slot(static_cast<std::int32_t>(q.index)):-1;return 1;}if(!valid_saved_row())break;r.value=p.saved?p.saved->skill_slot(q.index):-1;return 1;
  case Op::skill_id:if(!valid_saved_row())break;r.value=p.saved?p.saved->skill_id(q.index):-1;return 1;
  case Op::skill_level:if(!valid_saved_row())break;r.value=p.saved?p.saved->skill_level(q.index):-1;return 1;
  case Op::unlocked_difficulty:r.value=p.saved?p.saved->unlocked_difficulty():-1;return 1;
  case Op::character_level:if(!p.properties||dh2_character_get_level(&r.value,p.properties))break;return 1;
  case Op::character_skill:{auto*s=skill();if(!s)break;r.identity=reinterpret_cast<std::uintptr_t>(s);return 1;}
  case Op::can_increment:{if(!p.saved||p.saved->skills().empty()){r.value=0;return 1;}if(!valid_saved_row())break;std::int32_t level;if(!p.properties||dh2_character_get_level(&level,p.properties))break;auto*s=skill();if(!s)break;auto delta=signed_word(static_cast<std::uint32_t>(level)-static_cast<std::uint32_t>(s->required_level));r.value=p.saved->skill_level(q.index)<=delta;return 1;}
  case Op::current_faery:{if(!p.saved){r.value=0;return 1;}auto difficulty=q.value==-1?p.current_difficulty?*p.current_difficulty:-1:q.value;if(difficulty<0||difficulty>2)break;r.value=p.saved->current_faery(static_cast<std::uint32_t>(difficulty));return 1;}
  case Op::faery_level:{if(!p.saved){r.value=-1;return 1;}auto difficulty=q.value==-1?p.current_difficulty?*p.current_difficulty:-1:q.value;if(difficulty<0||difficulty>2)break;r.value=p.saved->faery_level(q.index,static_cast<std::uint32_t>(difficulty));return 1;}
  default:break;
 }
 e="Required HUD saved/property/SkillTable projection unavailable or malformed";return 0;
}
}
