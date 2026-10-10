#include "source_quest_service_binding.hpp"
#include <algorithm>
namespace dh::foundation {
namespace {
bool required(std::string& e,const char* leaf){if(e.empty())e=std::string("Required SAME source Quest service: ")+leaf;return false;}
bool actual(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& q,std::uintptr_t character,std::string& e){
    return q&&q->save()&&q->save()->character()==character&&character?true:required(e,"Character/Save/Quest association");
}
}
bool bind_source_quest_event_projection_v108(dh2::loader::GameEventRuntimeServicesV75& services,std::string& e){
    if(!services.provider)return required(e,"shared actual GameEventRuntime callback provider");
    auto previous=std::move(services.project_event);
    services.project_event=[previous=std::move(previous)](const dh2::events::EventBorrowV12& event,
        dh2::loader::GameEventQuestBorrowV75& out,std::string& error){
        if(dh2::loader::project_scoped_game_quest_event_v75(event,out,error))return true;
        if(!error.empty())return false;
        if(previous)return previous(event,out,error);
        error="Required actual typed QuestEvent family for this listener";return false;
    };
    e.clear();return true;
}
bool construct_source_quest_owner(const std::shared_ptr<dh2::data::PlayerSavegameV1>& save,
    const std::shared_ptr<const dh2::data::QuestTablesPersistenceV51>& tables,
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& out,std::string& e){
    if(out||!save||!save->character()||!tables||!tables->ready())return required(e,"existing Character-bound Save and actual Quest table");
    if(save->regular_quests_v45().initialized()||save->volatile_quests_v45().initialized())return required(e,"fresh original Save collection initialization");
    auto staged=std::make_shared<dh2::character::CharacterMenuQuestsV51>(save,tables);
    if(!staged->initialize(0,e)||!staged->initialize(1,e))return false;
    out=std::move(staged);e.clear();return true;
}
dh2::world::QuestConditionCompileServicesV70 SourceQuestServiceBinding::compile_services(){
    auto self=shared_from_this();dh2::world::QuestConditionCompileServicesV70 s;s.transport=self;
    s.character_difficulty3bb8e4=[self](auto id,auto& out,auto& e){return self->source_.world_owner&&self->source_.difficulty?self->source_.difficulty(id,out,e):required(e,"live Character difficulty");};return s;
}
bool construct_source_condition_runtime(const std::shared_ptr<const dh2::world::NativeConditionTableV69>& tables,
    dh2::world::NativeConditionServicesV69 services,std::shared_ptr<dh2::world::NativeConditionRuntimeV69>& out,std::string& e){
    if(out||!tables||!tables->ready()||!services.transport||!services.local_player||!services.current_level||
       !services.quest_state||!services.event_state||!services.assertion_mode)return required(e,"once-only decoded condition owner and actual PM/Level/Quest/Event/assertion services");
    return dh2::world::NativeConditionRuntimeV69::create(tables,std::move(services),out,e);
}
bool construct_source_objective_runtime(const std::shared_ptr<dh2::loader::GameEventManagerV50>& manager,
    dh2::loader::GameEventRuntimeServicesV75 services,std::shared_ptr<dh2::loader::GameEventRuntimeV75>& out,std::string& e){
    if(out||!manager||!manager->diagnostics().storage_load_complete||!services.provider||
       !services.current_level||!services.constant||!services.common_script_count||!services.start_script||
       !services.project_event)return required(e,"once-only SAME loaded GameEvents194 and source Level/script/constants/event transport");
    auto runtime=std::make_shared<dh2::loader::GameEventRuntimeV75>(manager,std::move(services));
    out=std::move(runtime);e.clear();return true;
}
bool SourceQuestServiceBinding::selected(std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q,dh2::data::QuestSavegameV1*& collection,std::int32_t& difficulty,std::string& e){
    collection=nullptr;if(!q||!q->save())return required(e,"Quest/Save owner");bool online;
    if(!source_.online||!source_.online(online,e)||!source_.difficulty||!source_.difficulty(q->save()->character(),difficulty,e))return required(e,"online and live difficulty selectors");
    if(difficulty<0||difficulty>=3)return required(e,"three source difficulty cells");
    collection=online?&q->save()->volatile_quests_v45():&q->save()->regular_quests_v45();
    if(!collection->initialized()||*collection->source_character5c_v70()!=q->save()->character())return required(e,"initialized SAME collection5c");e.clear();return true;
}
bool SourceQuestServiceBinding::progress(std::uint32_t field,std::int32_t value,std::int32_t requested,std::string& e){
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
    if(!source_.local_quests||!source_.local_quests(q,e))return required(e,"PM local0 Character.SG wrapper");
    if(!q){e.clear();return true;} //actual nullable local character branch
    if(!q->save())return required(e,"local Character Save14e8");
    auto difficulty=requested;if(difficulty==-1&&(!source_.difficulty||!source_.difficulty(q->save()->character(),difficulty,e)))return required(e,"SG live difficulty");
    bool online;if(!source_.online||!source_.online(online,e))return required(e,"SG online collection selector");
    auto* c=online?&q->save()->volatile_quests_v45():&q->save()->regular_quests_v45();
    if(!c->initialized()||*c->source_character5c_v70()!=q->save()->character())return required(e,"SAME initialized SG collection5c");
    return c->source_store_progress_v108(field,value,difficulty,e);
}
bool SourceQuestServiceBinding::frame_services(dh2::world::NativeQuestFrameServicesV108& out,std::string& e){
    if(!source_.world_owner||!source_.campaign||!source_.local_quests||!source_.character_quests||!source_.difficulty||!source_.online||!source_.constant||!source_.application_time70)return required(e,"live App/PM/Character/campaign/time/constants owner");
    auto self=shared_from_this();dh2::world::NativeQuestFrameServicesV108 s;s.provider=self;
    s.constant=source_.constant;s.application_time70=source_.application_time70;
    s.frame_gate=[self](bool& permitted,std::string& error){permitted=false;std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
        if(!self->source_.local_quests(q,error))return false;
        if(!q){permitted=true;error.clear();return true;}
        if(!q->save())return required(error,"actual local Save14e8");
        if(!q->save()->source_quest_sync_ready14_v3()){error.clear();return true;}
        bool online;if(!self->source_.online(online,error))return false;
        if(!online){permitted=true;error.clear();return true;}
        bool hosting;if(!self->source_.local_hosting||!self->source_.local_hosting(hosting,error))return required(error,"PM.IsLocalHosting");
        if(hosting){permitted=true;error.clear();return true;}
        return self->source_.online_script_admission?self->source_.online_script_admission(permitted,error):required(error,"ScriptManager online quest admission");
    };
    bind_original_quest_scripts(s,source_.campaign);
    s.current_quest=[self](auto value,auto diff,auto& error){return self->progress(0x2c,value,diff,error);};
    s.current_primary=[self](auto value,auto diff,auto& error){return self->progress(0x38,value,diff,error);};
    s.current_act=[self](auto value,auto diff,auto& error){return self->progress(0x44,value,diff,error);};
    s.transition_save=[self](auto& error){return self->source_.transition_save?self->source_.transition_save(error):required(error,"whole Level/Character transition save");};
    s.new_dialog=[self](const auto& q,auto& error){return self->source_.new_dialog?self->source_.new_dialog(q,error):required(error,"source new quest dialog");};
    s.completed_dialog=[self](const auto& q,const auto& rewards,auto& error){return self->source_.completed_dialog?self->source_.completed_dialog(q,rewards,error):required(error,"source completed quest dialog");};
    s.give_reward=[self](auto character,const auto& reward,bool& given,auto& error){given=false;return self->source_.give_reward?self->source_.give_reward(character,reward,given,error):required(error,"actual reward Character/inventory/property producer");};
    s.online=source_.online;
    s.online_activation=[self](auto& error){return self->source_.online_activation?self->source_.online_activation(error):required(error,"online activation message");};
    s.online_act=[self](auto& error){return self->source_.online_act?self->source_.online_act(error):required(error,"online current act message");};
    s.is_local=[self](auto character,auto& local,auto& error){return self->source_.is_local?self->source_.is_local(character,local,error):required(error,"PM source IsLocalPlayer");};
    s.all_quests_trophy=[self](auto character,auto& error){return self->all_quests(character,error);};
    out=std::move(s);e.clear();return true;
}
bool SourceQuestServiceBinding::publish(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& q,dh2::world::NativeQuestRuntimeServicesV76 services,std::string& e){
    if(!q||!q->save()||!services.provider||!services.conditions||!services.objectives)return required(e,"existing shared native Condition/Objective services");
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> actual_q;
    if(!source_.character_quests||!source_.character_quests(q->save()->character(),actual_q,e)||actual_q!=q)return required(e,"existing SAME character quest publication");
    if(q->runtime_v76())return required(e,"once-only native runtime publication");
    if(!q->save()->regular_quests_v45().initialized()||!q->save()->volatile_quests_v45().initialized())return required(e,"both original initialized collections");
    dh2::world::NativeQuestFrameServicesV108 frame;if(!frame_services(frame,e))return false;
    auto runtime=std::make_shared<dh2::world::NativeQuestRuntimeV76>(q,std::move(services));
    return runtime->bind_frame_v108(std::move(frame),e)&&q->bind_runtime_v76(std::move(runtime),e);
}
bool SourceQuestServiceBinding::quest_state(std::uintptr_t character,std::int32_t row,std::int32_t requested,dh2::world::NativeConditionStateV69& out,std::string& e){
    out={};std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
    if(!source_.world_owner||!source_.character_quests||!source_.character_quests(character,q,e)||!actual(q,character,e))return required(e,"SG_GetQuestByID Character");
    auto d=requested;if(d==-1&&(!source_.difficulty||!source_.difficulty(character,d,e)))return required(e,"SG_GetQuestByID source difficulty");
    if(d<0||d>=3)return required(e,"SG_GetQuestByID requested difficulty");
    bool online;if(!source_.online||!source_.online(online,e))return required(e,"SG_GetQuestByID online selector");
    auto* c=online?&q->save()->volatile_quests_v45():&q->save()->regular_quests_v45();
    const auto valid=[&](){return row>=0&&std::size_t(row)<c->source_quests_v45()[std::size_t(d)].size();};
    if(!valid()){
        std::int32_t mode;if(!source_.assertion_mode||!source_.assertion_mode(mode,e))return required(e,"QuestSavegame.GetQuestByID assertion mode");
        if(mode==2){e="Original QuestSavegame.GetQuestByID assertion mode2 NULL store";return false;}
        if(mode==1&&(!source_.assertion||!source_.assertion("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\SaveGames\\QuestSavegame.cpp",0x126,"id >= 0 && id < (int)m_quests[diff].size()",e)))return false;
        if(!valid()){e.clear();return true;}
    }
    OriginalQuestAdapter adapter(q);const auto which=online?1u:0u;
    if(!adapter.compile(which,false,compile_services(),e))return false;
    const auto& rows=c->source_quests_v45()[std::size_t(d)];
    if(!valid())return required(e,"SG_GetQuestByID callback-mutated live slot");
    auto* state=q->resolve_v70(rows[std::size_t(row)]);if(!state)return required(e,"actual Quest state0");
    out={std::shared_ptr<void>(q,state),&state->state};e.clear();return true;
}
bool SourceQuestServiceBinding::update(std::uintptr_t character,bool force,std::string& e){
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
    if(!source_.world_owner||!source_.character_quests||!source_.character_quests(character,q,e)||!actual(q,character,e))return required(e,"Character.SG_Update owner");
    if(!source_.synchronize_quests||!source_.synchronize_quests(character,e))return required(e,"actual PlayerSaveQuestSyncOwnerV3::try_sync");
    dh2::data::QuestSavegameV1* c;std::int32_t d;if(!selected(q,c,d,e))return false;
    const auto which=c==&q->save()->volatile_quests_v45()?1u:0u;OriginalQuestAdapter adapter(q);
    auto services=compile_services();return (!force||adapter.compile(which,true,services,e))&&adapter.update(which,services,e);
}
bool SourceQuestServiceBinding::all_quests(std::uintptr_t character,std::string& e){
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
    if(!source_.character_quests||!source_.character_quests(character,q,e)||!actual(q,character,e))return false;
    dh2::data::QuestSavegameV1* c;std::int32_t d;if(!selected(q,c,d,e))return false;
    const auto count=c->source_quests_v45()[std::size_t(d)].size();
    for(std::size_t i=0;i<count;++i){dh2::world::NativeConditionStateV69 state;
        if(!quest_state(character,std::int32_t(i),-1,state,e)||!state.state0)return required(e,"all-quests live ordered state");
        if(*state.state0<=12){e.clear();return true;}
    }
    return source_.unlock_trophy?source_.unlock_trophy("quest_allcomplete",e):required(e,"process TrophyManager quest_allcomplete");
}
bool SourceQuestServiceBinding::quest_log(QuestLogCategoryV108 category,const QuestLogFunctorV108& functor,
    const QuestLogTextV108& text,std::vector<QuestLogEntryV108>& out,std::string& e){
    if(!functor||!text)return required(e,"native GetQuestFunctor and Localization.StringID");
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
    if(!source_.local_quests||!source_.local_quests(q,e)||!q||!q->save())return required(e,"live local Character/Save for Quest Log");
    dh2::data::QuestSavegameV1* c{};std::int32_t difficulty{};
    if(!selected(q,c,difficulty,e))return false;
    const auto which=c==&q->save()->volatile_quests_v45()?1u:0u;
    const auto current=c->progress().current_quest[std::size_t(difficulty)];
    OriginalQuestAdapter adapter(q);std::vector<OriginalQuestView> rows;
    if(!adapter.list(which,difficulty,text,rows,e))return false;
    std::vector<QuestLogEntryV108> staged;staged.reserve(rows.size());
    for(const auto& row:rows){
        dh2::data::QuestPersistenceStateV51* state{};
        if(!adapter.resolve(row.id,state,e))return false;
        bool include{};
        if(!functor(*state,category,include,e))return required(e,"native Quest.GetQuestFunctor category predicate");
        if(include)staged.push_back({row.id,row.localized_text[0],row.id.row==current});
    }
    std::stable_sort(staged.begin(),staged.end(),[](const auto& a,const auto& b){return a.title<b.title;});
    out=std::move(staged);e.clear();return true;
}
bool SourceQuestServiceBinding::quest_log_details(OriginalQuestId id,const QuestLogTextV108& text,
    QuestLogDetailsV108& out,std::string& e){
    if(!text)return required(e,"Localization.StringID for selected Quest details");
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
    if(!source_.local_quests||!source_.local_quests(q,e)||!q||!q->save())return required(e,"live local Character/Save for Quest details");
    dh2::data::QuestSavegameV1* c{};std::int32_t difficulty{};
    if(!selected(q,c,difficulty,e))return false;
    const auto which=c==&q->save()->volatile_quests_v45()?1u:0u;
    if(id.collection!=which||id.difficulty!=difficulty)return required(e,"selected Quest identity in current collection/difficulty");
    OriginalQuestAdapter adapter(q);dh2::data::QuestPersistenceStateV51* state{};
    if(!adapter.resolve(id,state,e))return false;
    QuestLogDetailsV108 staged;staged.id=id;staged.source_name=state->definition->name;staged.text_ids=state->definition->text_fields;
    for(std::size_t i=0;i<staged.text_ids.size();++i)if(!text(staged.text_ids[i],staged.text[i],e))return false;
    std::int32_t primary{};
    if(!source_.constant||!source_.constant("v2QuestPriority","Primary",primary,e))return required(e,"actual v2QuestPriority.Primary constant");
    staged.primary=state->definition->priority==primary;out=std::move(staged);e.clear();return true;
}
bool SourceQuestServiceBinding::quest_log_activate(OriginalQuestId id,const QuestLogFunctorV108& functor,std::string& e){
    if(!functor)return required(e,"native GetQuestFunctor Active category predicate");
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> q;
    if(!source_.local_quests||!source_.local_quests(q,e)||!q||!q->save())return required(e,"live local Character/Save for Quest activation");
    dh2::data::QuestSavegameV1* c{};std::int32_t difficulty{};
    if(!selected(q,c,difficulty,e))return false;
    const auto which=c==&q->save()->volatile_quests_v45()?1u:0u;
    if(id.collection!=which||id.difficulty!=difficulty)return required(e,"selected Quest identity in current collection/difficulty");
    OriginalQuestAdapter adapter(q);dh2::data::QuestPersistenceStateV51* state{};
    if(!adapter.resolve(id,state,e))return false;
    bool active{};if(!functor(*state,QuestLogCategoryV108::active,active,e))return required(e,"native Quest.GetQuestFunctor Active category predicate");
    if(!active)return required(e,"source Assigned Quest row eligible for activation");
    // This is the same Character.SG_SetCurrentQuest source cell used by the
    // original native frame bridge; completed/closed rows cannot reach it.
    return c->source_store_progress_v108(0x2c,id.row,difficulty,e);
}
}
