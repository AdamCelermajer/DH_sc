#pragma once
#include "../level-world/canonical_character_candidate_v60.hpp"
#include "../game-data/world_map_profile_table_v59.hpp"
#include <cstring>

namespace dh::foundation {
// Pins immutable source inputs for mask1/2/4; holds no mutable Character or Save.
struct SourceProfileReaderTables {
 std::shared_ptr<dh2::character::CharacterGameDesign::Borrow> design;
 dh2::data::SkillTables::Borrow skills;
 std::shared_ptr<const dh2::data::WorldMapProfileTableV59> maps;
 std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> quests;
 std::vector<std::int32_t> level_defaults28;
};
inline bool bind_source_profile_reader_tables(
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& record,
 dh2::data::SkillTables::Borrow skills,
 std::shared_ptr<const dh2::data::WorldMapProfileTableV59> maps,
 std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> quests,
 dh2::character::CharacterMenuProfileLoadServicesV51& output,std::string& error){
 if(!record||!record->actor||!record->actor->object||!record->properties||!record->save||!record->services.design||
    !record->services.design->ready()||!skills||!maps||!maps->ready()||!quests||!quests->ready()||
    output.tables||output.level_defaults28||output.map_defaults8||output.character_skill_list){
  error="Required unbound same-Character profile reader and actual source tables";return false;
 }
 if(record->actor->object->properties!=record->properties||!record->services.skills||
    &record->services.skills.lists()!=&skills.lists()){
  error="Profile reader requires the same Character properties and SkillTables authority";return false;
 }
 auto tables=std::make_shared<SourceProfileReaderTables>();
 tables->design=std::make_shared<dh2::character::CharacterGameDesign::Borrow>(record->services.design->borrow());
 const auto* levels=tables->design->levels();
 if(tables->design->characters()!=record->design.characters()||levels!=record->design.levels()||
    !levels||levels->levels.size()!=levels->level_names.size()){
  error="Profile reader GameDesign differs from the same canonical Character";return false;
 }
 if(output.quests&&(output.quests->save()!=record->save||output.quests->tables()!=quests)){
  error="Profile Quest reader differs from the same Save/table authority";return false;
 }
 for(const auto& row:levels->levels){std::int32_t value{};const auto word=row.scalar.words[0x28/4];
  std::memcpy(&value,&word,sizeof(value));tables->level_defaults28.push_back(value);
 }
 tables->skills=std::move(skills);tables->maps=std::move(maps);tables->quests=std::move(quests);
 const auto weak_record=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(record);
 output.tables=tables;output.characters=tables->design->characters();output.skills=tables->skills;
 output.level_names=&levels->level_names;output.level_defaults28=&tables->level_defaults28;
 output.map_names=&tables->maps->names();output.map_defaults8=&tables->maps->defaults8();
 if(!output.quests)output.quests=std::make_shared<dh2::character::CharacterMenuQuestsV51>(record->save,tables->quests);
 output.character_skill_list=[weak_record,tables](std::uintptr_t identity,const std::vector<std::int32_t>*& rows,std::string& error){
  rows=nullptr;const auto same=weak_record.lock();
  if(!same||!same->actor||!same->actor->object||same->actor->object->identity!=identity||
     !same->properties||same->actor->object->properties!=same->properties||
     !same->save||same->save->character()!=identity||same->design.characters()!=tables->design->characters()||
     !same->services.skills||&same->services.skills.lists()!=&tables->skills.lists()){
   error="Expired or foreign canonical Character GetSkillsList receiver";return false;
  }
  const auto& lists=tables->skills.lists();auto index=same->properties->resolved[28];
  if(index<0||std::size_t(index)>=lists.size())index=3; // source GetSkillsList fallback
  if(index<0||std::size_t(index)>=lists.size()){
   error="Required actual SkillList fallback3 source row";return false;
  }
  rows=&lists[std::size_t(index)];error.clear();return true;
 };
 error.clear();return true;
}
}
