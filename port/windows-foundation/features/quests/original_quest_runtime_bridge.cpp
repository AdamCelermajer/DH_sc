#include "original_quest_adapter.hpp"
#include <cstring>
namespace dh::foundation {
namespace {
bool failure(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& owner,std::string& e,const char* reason){
    if(e.empty())e=reason;if(owner)owner->retain_runtime_failure_v76(e);return false;
}
dh2::data::QuestSavegameV1* selected(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& owner,std::uint32_t which){
    if(!owner||!owner->save()||which>1)return nullptr;
    return which?&owner->save()->volatile_quests_v45():&owner->save()->regular_quests_v45();
}
}
bool OriginalQuestAdapter::compile(std::uint32_t which,bool force,const dh2::world::QuestConditionCompileServicesV70& source,std::string& e){
    auto* c=selected(owner_,which);auto runtime=owner_?owner_->runtime_v76():nullptr;
    if(!c||!c->initialized()||!runtime||!source.transport||!source.character_difficulty3bb8e4){e="Required SAME initialized Quest native runtime and difficulty provider";return false;}
    if(!owner_->runtime_failure_v76().empty()){e=owner_->runtime_failure_v76();return false;}
    if(runtime->failed()){e=runtime->failure();return false;}
    auto services=source;services.quest_compile480178=[owner=owner_](auto id,auto& error){return owner->compile_quest_v76(id,error);};
    dh2::world::QuestConditionCompileBorrowV70 fields{std::shared_ptr<void>(owner_->save(),c),c,c->source_character5c_v70(),c->source_compiled28_v70()};
    if(!dh2::world::quest_condition_compile_v70(fields,force,services,e))return failure(owner_,e,"Original quest compile interrupted");
    e.clear();return true;
}
bool OriginalQuestAdapter::update(std::uint32_t which,const dh2::world::QuestConditionCompileServicesV70& source,std::string& e){
    if(!compile(which,false,source,e))return false;auto* c=selected(owner_,which);auto runtime=owner_->runtime_v76();
    auto difficulty=[&](std::int32_t& out){
        if(!source.character_difficulty3bb8e4(*c->source_character5c_v70(),out,e))return false;
        if(out<0||out>=3){e="Original quest update difficulty outside three source cells";return false;}return true;
    };
    std::int32_t d;if(!difficulty(d))return failure(owner_,e,"Quest difficulty provider");
    const auto count=c->source_quests_v45()[std::size_t(d)].size();
    for(std::size_t i=0;i<count;++i){
        if(!difficulty(d))return failure(owner_,e,"Quest difficulty provider");
        const auto& rows=c->source_quests_v45()[std::size_t(d)];
        if(i>=rows.size()||!rows[i]){e="Original Quest.Update reached unavailable live slot";return failure(owner_,e,"Quest update identity");}
        if(!runtime->update_quest_v108(rows[i],e))return failure(owner_,e,"Original quest transition interrupted");
    }
    e.clear();return true;
}
bool OriginalQuestAdapter::prerequisites(OriginalQuestId id,bool& satisfied,std::string& e){
    satisfied=false;dh2::data::QuestPersistenceStateV51* q;if(!resolve(id,q,e))return false;
    auto runtime=owner_->runtime_v76();if(!runtime){e="Required native ConditionRuntime for quest unlock";return false;}
    return runtime->prerequisites(reinterpret_cast<std::uintptr_t>(q),satisfied,e);
}
void bind_original_quest_scripts(dh2::world::NativeQuestFrameServicesV108& frame,const std::shared_ptr<OriginalCampaignRuntime>& campaign){
    std::weak_ptr<OriginalCampaignRuntime> weak=campaign;
    frame.script_id=[weak](const char* name,std::int32_t& out,std::string& e){auto c=weak.lock();
        out=-1;if(!name){e.clear();return true;}const auto* dot=std::strchr(name,'.');
        if(!dot){e.clear();return true;} //source Quest looks up suffix, level array only
        if(!c){e="Required retained original campaign script owner";return false;}out=c->script_id(dot+1,false);e.clear();return true;};
    frame.script_running=[weak](std::int32_t id,bool& out,std::string& e){auto c=weak.lock();
        if(!c){e="Required retained original campaign script owner";return false;}
        if(id<0||std::size_t(id)>=c->scripts().size()){e="Original quest script ID outside campaign owner";return false;}
        out=c->running(id);e.clear();return true;};
    frame.start_script=[weak](auto id,auto module,bool received,auto& e){auto c=weak.lock();
        if(!c){e="Required retained original campaign script owner";return false;}return c->start(id,module,received,e);};
}
}
