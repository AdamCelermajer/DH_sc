#include "original_quest_adapter.hpp"
namespace dh::foundation {
namespace {
dh2::data::QuestSavegameV1* collection(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& owner,std::uint32_t which){
    if(!owner||!owner->save()||which>1)return nullptr;
    auto& save=*owner->save();return which?&save.volatile_quests_v45():&save.regular_quests_v45();
}
bool required(std::string& e,const char* name){e=std::string("Required original Quest provider: ")+name;return false;}
OriginalQuestObjectiveView objective(const dh2::data::QuestObjectivePersistenceV51& o){
    return {*o.definition,o.completed14,o.quantity20};
}
}
bool OriginalQuestAdapter::resolve(OriginalQuestId id,dh2::data::QuestPersistenceStateV51*& out,std::string& e)const{
    out=nullptr;auto* c=collection(owner_,id.collection);
    if(!c||!c->initialized()||id.difficulty<0||id.difficulty>=3||id.row<0)return required(e,"initialized SAME collection and stable ID");
    const auto& rows=c->source_quests_v45()[std::size_t(id.difficulty)];
    if(std::size_t(id.row)>=rows.size())return required(e,"authored table row");
    out=owner_->resolve_v70(rows[std::size_t(id.row)]);
    if(!out||!out->definition||out->difficulty!=id.difficulty||out->index!=id.row||out->character_owner!=owner_->save()->character()){
        out=nullptr;return required(e,"SAME quest identity and Character owner");
    }
    e.clear();return true;
}
bool OriginalQuestAdapter::list(std::uint32_t which,std::int32_t difficulty,const QuestTextProvider& text,
    std::vector<OriginalQuestView>& out,std::string& e)const{
    auto* c=collection(owner_,which);
    if(!c||!c->initialized()||difficulty<0||difficulty>=3)return required(e,"initialized menu quest collection");
    if(!text)return required(e,"Localization.StringID");
    std::vector<OriginalQuestView> staged;
    const auto count=c->source_quests_v45()[std::size_t(difficulty)].size();
    for(std::size_t i=0;i<count;++i){
        OriginalQuestId id{which,difficulty,std::int32_t(i)};dh2::data::QuestPersistenceStateV51* q;
        if(!resolve(id,q,e))return false;const auto& d=*q->definition;
        if(!q->accept.definition||!q->end.definition)return required(e,"authored accept/end objective");
        OriginalQuestView v;v.id=id;v.source_name=d.name;v.text_ids=d.text_fields;
        for(std::size_t t=0;t<v.text_ids.size();++t)if(!text(v.text_ids[t],v.localized_text[t],e))return false;
        v.state=q->state;v.priority=d.priority;v.act=d.act;v.target_level=d.target_level;v.repeatable=d.repeatable!=0;
        v.accept=objective(q->accept);v.end=objective(q->end);
        for(const auto& o:q->objectives){if(!o.definition)return required(e,"authored objective row");v.objectives.push_back(objective(o));}
        v.authored_rewards=d.rewards[std::size_t(difficulty)];staged.push_back(std::move(v));
    }
    out=std::move(staged);e.clear();return true;
}
const dh2::data::SavedQuestProgressV1* OriginalQuestAdapter::progress(std::uint32_t which)const{
    auto* c=collection(owner_,which);return c&&c->initialized()?&c->progress():nullptr;
}
bool OriginalQuestAdapter::store_progress(std::uint32_t which,std::uint32_t field,std::int32_t value,std::int32_t difficulty,std::string& e){
    auto* c=collection(owner_,which);if(!c||!c->initialized())return required(e,"SAME initialized progress cells");
    return c->source_store_progress_v108(field,value,difficulty,e);
}
}
