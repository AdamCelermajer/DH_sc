#include "skill_ui.hpp"
#include "original_skill_art.hpp"
#include <algorithm>
#include <limits>
namespace dh::foundation::skill_ui {
namespace {bool fail(std::string& e,const char* s){e=s;return false;}bool fail(std::string& e,const std::string& s){e=s;return false;}}
bool source_skill_tree_metadata(const dh2::data::CharacterTable& characters,int class_row,
 dh2::data::SkillTables::Borrow tables,int& authored_list,std::string& error){
 error.clear();if(!tables)return fail(error,"Skill UI: missing original SkillTables for source class selection");
 const auto field=std::find(characters.fields.begin(),characters.fields.end(),"SkillTree");
 if(field==characters.fields.end())return fail(error,"Skill UI: original CharacterTable lacks SkillTree metadata");
 if(class_row<0||static_cast<std::size_t>(class_row)>=characters.rows.size()||
    static_cast<std::size_t>(class_row)>=characters.names.size())return fail(error,"Skill UI: source saved class row is outside original CharacterTable");
 const auto column=static_cast<std::size_t>(field-characters.fields.begin());
 if(column>=characters.rows[class_row].size())return fail(error,"Skill UI: original CharacterTable.SkillTree column is outside row");
 const int authored=characters.rows[class_row][column];
 if(authored<0||static_cast<std::size_t>(authored)>=tables.lists().size())return fail(error,"Skill UI: source class row has no authored SkillTree list");
 const auto& list=tables.lists()[authored];const auto& skills=tables.skills();
 for(auto id:list)if(id<0||static_cast<std::size_t>(id)>=skills.size())return fail(error,"Skill UI: source CharacterTable SkillTree references an absent SkillTable row");
 authored_list=authored;return true;
}
bool original_class_frame_for_row(int source_class_row,unsigned& frame,std::string& error){
 error.clear();
 // Exact source rule from MenuBase::FS_GetPlayerClass2 (0x421980) and
 // playerClassAsStr (0x4218e4): saved class rows 263/325/290 select native
 // lower-case strings warrior/rogue/mage. Original sprite493 frames 0/1/2
 // carry those same family labels. The native function returns null for all
 // other rows; do not extend this switch to specializations or guess by name.
 unsigned resolved;
 switch(source_class_row){case 263:resolved=0;break;case 325:resolved=1;break;case 290:resolved=2;break;
  default:return fail(error,"Skill UI: original playerClassAsStr has no class frame for this saved class row");}
 frame=resolved;return true;
}
bool original_class_frame_for_row(const dh2::data::CharacterTable& characters,
                                 int source_class_row,unsigned& frame,std::string& error){
 error.clear();
 if(source_class_row<0||static_cast<std::size_t>(source_class_row)>=characters.rows.size())
  return fail(error,"Skill UI: source saved class row is outside original CharacterTable");
 const auto class_id_field=std::find(characters.fields.begin(),characters.fields.end(),"ClassID");
 if(class_id_field==characters.fields.end())return fail(error,"Skill UI: original CharacterTable lacks ClassID metadata");
 const auto column=static_cast<std::size_t>(class_id_field-characters.fields.begin());
 if(column>=characters.rows[static_cast<std::size_t>(source_class_row)].size())
  return fail(error,"Skill UI: original CharacterTable.ClassID column is outside row");
 const auto class_id=characters.rows[static_cast<std::size_t>(source_class_row)][column];
 std::optional<unsigned> resolved;
 // These are the source base rows whose family labels select SWF sprite493
 // frames 0/1/2. Authored specialization rows retain the corresponding
 // ClassID; do not use row-name prefixes or SkillTree position to guess art.
 for(const auto [base_row,base_frame]:{std::pair{263,0u},std::pair{325,1u},std::pair{290,2u}}){
  if(static_cast<std::size_t>(base_row)>=characters.rows.size()||
     column>=characters.rows[static_cast<std::size_t>(base_row)].size())
   return fail(error,"Skill UI: source base class family row is absent from CharacterTable");
  if(characters.rows[static_cast<std::size_t>(base_row)][column]!=class_id)continue;
  if(resolved&&*resolved!=base_frame)return fail(error,"Skill UI: source class ID maps to multiple authored skill families");
  resolved=base_frame;
 }
 if(!resolved)return fail(error,"Skill UI: source class ID has no authored skill family frame");
 frame=*resolved;return true;
}
bool Presenter::view(View& out,std::string& error)const {
 error.clear();if(!tables_)return fail(error,"Skill UI: missing original SkillTables");
 if(!services_.character)return fail(error,"Skill UI: unsupported Character::GetCharSkillListId / skill points service");
 if(!services_.progress)return fail(error,"Skill UI: unsupported Character skill progression service");
 if(!services_.slots)return fail(error,"Skill UI: unsupported saved skill slot service");
 View next;if(!services_.character(next.list_id,next.points,error))return false;
 // Source GetCharSkillListId falls back to authored list3 for invalid owner ID.
 if(next.list_id<0||static_cast<std::size_t>(next.list_id)>=tables_.lists().size())next.list_id=3;
 if(static_cast<std::size_t>(next.list_id)>=tables_.lists().size())return fail(error,"Skill UI: source fallback SkillList3 absent");
 const auto& list=tables_.lists()[next.list_id];
 for(std::size_t i=0;i<list.size();++i){int id=list[i];if(id<0||static_cast<std::size_t>(id)>=tables_.skills().size())return fail(error,"Skill UI: authored SkillList references absent SkillTable row");
  const auto& record=tables_.skills()[id];Row r;r.position=static_cast<int>(i);r.table_id=id;r.source_name=tables_.skill_names()[id];r.source_icon=record.icon;
  static_assert(sizeof(int)==4);std::uint32_t raw=record.scalar.words[8];r.required_level=raw<=std::uint32_t(std::numeric_limits<int>::max())?int(raw):int(std::int64_t(raw)-4294967296LL);
  if(!services_.progress(r.position,r.progress,error))return false;next.rows.push_back(std::move(r));
 }
 if(!services_.slots(next.slots,error))return false;
 for(int p:next.slots)if(p<-1||(p>=0&&static_cast<std::size_t>(p)>=next.rows.size()))return fail(error,"Skill UI: saved slot references absent class skill position");
 out=std::move(next);return true;
}
bool Presenter::select(int position,std::string& e){View v;if(!view(v,e))return false;if(position<0||static_cast<std::size_t>(position)>=v.rows.size())return fail(e,"Skill UI: selection outside authored class SkillList");selected_=position;selected_list_=v.list_id;return true;}
bool Presenter::probe_training(bool& accepted,std::string& e)const{View v;if(!view(v,e))return false;if(!selected_||selected_list_!=v.list_id||static_cast<std::size_t>(*selected_)>=v.rows.size())return fail(e,"Skill UI: no current class skill selected");
 if(!services_.probe_increment)return fail(e,"Skill UI: unsupported full NativeSkillsTrainSkill source probe");return services_.probe_increment(*selected_,accepted,e);
}
bool Presenter::train(bool source_flag,std::string& e){if(source_flag){bool accepted=false;if(!probe_training(accepted,e))return false;if(!accepted)return fail(e,"Skill UI: NativeSkillsTrainSkill source probe rejected training");return true;}
 View v;if(!view(v,e))return false;if(!selected_||selected_list_!=v.list_id||static_cast<std::size_t>(*selected_)>=v.rows.size())return fail(e,"Skill UI: no current class skill selected");
 const auto& p=v.rows[*selected_].progress;if(!p.available||!p.can_increment)return fail(e,"Skill UI: Character::CanIncrementSkill rejected training");
 if(!services_.increment)return fail(e,"Skill UI: unsupported Character::IncSkill service");return services_.increment(*selected_,source_flag,e);
}
bool Presenter::assign(int slot,std::string& e){View v;if(!view(v,e))return false;if(!selected_||selected_list_!=v.list_id||static_cast<std::size_t>(*selected_)>=v.rows.size())return fail(e,"Skill UI: no current class skill selected");
 if(slot<0||static_cast<std::size_t>(slot)>=v.slots.size())return fail(e,"Skill UI: slot outside saved skill slots");if(!v.rows[*selected_].progress.equippable)return fail(e,"Skill UI: Character::IsSkillEquippable rejected assignment");
 if(!services_.assign)return fail(e,"Skill UI: unsupported Character::SG_SetSkillInSlot service");return services_.assign(slot,*selected_,e);
}
bool Presenter::append(character_menu::Frame& frame,std::string& error)const{
 View v;if(!view(v,error))return false;if(!services_.text&&!services_.selected_texts)return fail(error,"Skill UI: unsupported NativeGetSkillDetails localization service");
 unsigned cf=0;if(!services_.icon){if(!services_.source_class_frame)return fail(error,"Skill UI: unsupported original source class frame service");if(!services_.source_class_frame(cf,error))return false;}
 auto next=frame;
 for(const auto& row:v.rows){if(services_.icon){if(!services_.icon(row.position,row.source_icon,next,error))return false;}else if(!append_original_skill_icon(cf,row.position,row.source_icon,next.art,error))return false;}
 if(!services_.icon)for(std::size_t slot=0;slot<v.slots.size();++slot){int p=v.slots[slot];if(!append_original_skill_slot_icon(cf,static_cast<int>(slot),p<0?"blank":v.rows[p].source_icon,next.art,error))return false;}
 bool show_add=false;if(selected_list_==v.list_id&&selected_&&static_cast<std::size_t>(*selected_)<v.rows.size()){
  if(!services_.probe_increment)return fail(error,"Skill UI: selected skill requires the original NativeSkillsTrainSkill probe");
  if(!services_.probe_increment(*selected_,show_add,error))return false;
 }
 if(!show_add)next.art.batches.erase(std::remove_if(next.art.batches.begin(),next.art.batches.end(),[](const auto& b){return b.role.find("menu_SkillTreeSheetNew/btn_add/")==0;}),next.art.batches.end());
 const int selected_position=selected_list_==v.list_id?selected_.value_or(-1):-1;std::vector<std::pair<std::string,std::string>> selected_texts;
 if(selected_position>=0&&services_.selected_texts&&!services_.selected_texts(selected_position,selected_texts,error))return false;
 for(const auto& f:character_menu::original_menu_art(character_menu::Tab::skills).text_fields){std::string value;bool owns=false;
  if(f.path.find("cp_Skill_Points/")!=std::string::npos){value=std::to_string(v.points);owns=true;}
  else {int position=selected_list_==v.list_id?selected_.value_or(-1):-1;auto at=f.path.find("/buttons/");if(at!=std::string::npos){auto begin=f.path.find("skill",at);if(begin!=std::string::npos){begin+=5;std::size_t end=begin;while(end<f.path.size()&&f.path[end]>='0'&&f.path[end]<='9')++end;if(end>begin)position=std::stoi(f.path.substr(begin,end-begin));}if(position<0||static_cast<std::size_t>(position)>=v.rows.size())continue;value=std::to_string(v.rows[position].progress.level);owns=true;}
   else {const bool skill_detail=f.path.find("/SKILL_NAME/")!=std::string::npos||f.path.find("/skill_description/")!=std::string::npos||f.path.find("/current_skill_description/")!=std::string::npos||f.path.find("/next_skill_description/")!=std::string::npos;
    if(!skill_detail)continue;
    owns=true;if(position<0||static_cast<std::size_t>(position)>=v.rows.size())value.clear();
    else if(services_.selected_texts){auto at=std::find_if(selected_texts.begin(),selected_texts.end(),[&](const auto& item){return item.first==f.path;});if(at==selected_texts.end())return fail(error,"Skill UI: selected NativeGetSkillDetails projection omitted source field "+f.path);value=at->second;}
    else if(!services_.text(position,f.path,value,error))return false;
   }
  }
  if(!owns)continue;
  next.text.erase(std::remove_if(next.text.begin(),next.text.end(),[&](const auto& t){return t.field.path==f.path;}),next.text.end());if(!value.empty())next.text.push_back({f,std::move(value)});
 }
 frame=std::move(next);return true;
}
bool Presenter::release(float x,float y,std::string& error){error.clear();if(!services_.source_class_frame)return fail(error,"Skill UI: unsupported original source class frame service");unsigned cf;if(!services_.source_class_frame(cf,error))return false;auto hit=original_skill_hit(cf,x,y);if(!hit)return true;
 switch(hit->kind){case HitKind::select:return select(hit->position,error);case HitKind::assign:return assign(hit->position,error);case HitKind::train:return train(false,error);}return true;
}
}


