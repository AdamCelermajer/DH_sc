#include "character_quest_page_v1.hpp"
#include <algorithm>

namespace dh::foundation {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_id(const CharacterQuestIdV1& a,const CharacterQuestIdV1& b){
    return a.collection==b.collection&&a.difficulty==b.difficulty&&a.row==b.row;
}
}

SourceCharacterQuestPageV1::SourceCharacterQuestPageV1(
    CharacterState& character, CharacterQuestProgressV1& progress,
    std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> tables,
    CharacterQuestTextV1 text)
    : character_(&character),progress_(&progress),tables_(std::move(tables)),
      text_(std::move(text)) {}

bool SourceCharacterQuestPageV1::refresh(
    std::uint32_t collection,std::int32_t difficulty,
    CharacterQuestCategoryV1 category,const CharacterQuestLogPolicyV1& policy,
    CharacterQuestPageSnapshotV1& out,std::string& error) const {
    if(!character_||!progress_||!tables_||!progress_->belongs_to(*character_))
        return fail(error,"Quest Page requires the same bound CharacterState/progress owner and original Quest table");
    CharacterQuestPageSnapshotV1 staged;
    staged.collection=collection;staged.difficulty=difficulty;staged.category=category;
    if(!progress_->filter(*character_,*tables_,collection,difficulty,category,policy,
                          text_,staged.rows,staged.title_sorted,error))return false;
    out=std::move(staged);error.clear();return true;
}

bool SourceCharacterQuestPageV1::select(
    const CharacterQuestPageSnapshotV1& snapshot,const CharacterQuestIdV1& id,
    CharacterQuestPageSelectionV1& out,std::string& error) const {
    if(!character_||!progress_||!progress_->belongs_to(*character_))
        return fail(error,"Quest details require the same bound CharacterState/progress owner");
    if(id.collection!=snapshot.collection||id.difficulty!=snapshot.difficulty)
        return fail(error,"Selected Quest identity does not belong to the visible source page");
    const auto item=std::find_if(snapshot.rows.begin(),snapshot.rows.end(),
        [&](const auto& row){return same_id(row.id,id);});
    if(item==snapshot.rows.end())return fail(error,"Selected Quest row is not in the authored Active/Closed list");
    if(!tables_||id.row<0||std::size_t(id.row)>=tables_->rows().size())
        return fail(error,"Selected Quest row is outside the retained authored definition table");
    CharacterQuestPageSelectionV1 staged;
    staged.row=*item;
    if(!resolve_source_quest_page_text_v1(tables_->rows()[std::size_t(id.row)],
                                          *character_,text_,staged.details,error))return false;
    staged.activation_visible=snapshot.category==CharacterQuestCategoryV1::assigned;
    out=std::move(staged);error.clear();return true;
}

bool SourceCharacterQuestPageV1::activate(
    const CharacterQuestPageSnapshotV1& snapshot,const CharacterQuestIdV1& id,
    const CharacterQuestLogPolicyV1& policy,std::string& error) const {
    if(snapshot.category!=CharacterQuestCategoryV1::assigned)
        return fail(error,"Completed Quest rows do not expose the authored Make Active action");
    if(id.collection!=snapshot.collection||id.difficulty!=snapshot.difficulty||
       std::none_of(snapshot.rows.begin(),snapshot.rows.end(),
                    [&](const auto& row){return same_id(row.id,id);}))
        return fail(error,"Make Active target is not in the current authored Assigned list");
    if(!character_||!progress_||!tables_)
        return fail(error,"Make Active requires the same CharacterState/progress/table owners");
    return progress_->activate(*character_,*tables_,id,policy,error);
}

} // namespace dh::foundation
