#include "original_quest_adapter.hpp"
#include "source_quest_service_binding.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh2::data;
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"original cache file missing");return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char**argv){try{
    check(argc==2,"usage: original quest cache directory");const std::string root=argv[1];std::string e;
    auto array=read(root+"/v2quests_pyarray.bin"),names=read(root+"/v2quests_pyarraynames.bin");
    auto tables=std::make_shared<QuestTablesPersistenceV51>();
    check(tables->decode({array.data(),array.size()},{names.data(),names.size()},e),e.c_str());
    check(!tables->rows().empty(),"actual source quests must exist");
    auto save=std::make_shared<PlayerSavegameV1>();save->set_character(0x1234);
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> owner;
    check(construct_source_quest_owner(save,tables,owner,e),e.c_str());OriginalQuestAdapter adapter(owner);
    check(owner->save()==save,"source factory borrows existing SAME Save");
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> duplicate;
    check(!construct_source_quest_owner(save,tables,duplicate,e)&&!duplicate,"source factory never duplicates initialized save collection");
    std::vector<OriginalQuestView> views;std::vector<std::int32_t> delivered;
    QuestTextProvider text=[&](auto id,auto& out,auto& error){delivered.push_back(id);out="resolved:"+std::to_string(id);error.clear();return true;};
    for(unsigned which=0;which<2;++which)for(int difficulty=0;difficulty<3;++difficulty){
        check(adapter.list(which,difficulty,text,views,e),e.c_str());check(views.size()==tables->rows().size(),"all authored rows and difficulties");
        for(std::size_t i=0;i<views.size();++i){const auto& original=tables->rows()[i];const auto& v=views[i];
            check(v.source_name==original.name&&v.text_ids==original.text_fields&&v.id.row==int(i),"stable source IDs/text");
            check(v.state==original.state&&v.act==original.act&&v.priority==original.priority,"source state/act/priority");
            check(v.objectives.size()==original.objectives.size()&&v.authored_rewards.size()==original.rewards[difficulty].size(),"source objectives/rewards");
        }
    }
    auto* progress=adapter.progress(0);check(progress==&save->regular_quests_v45().progress(),"SAME source progress pointer");
    check(adapter.store_progress(0,0x44,3,0,e)&&adapter.store_progress(0,0x44,2,0,e)&&progress->current_act[0]==3,"monotonic original Act store");
    check(adapter.store_progress(0,0x2c,7,0,e)&&progress->current_quest[0]==7,"source currentquest store");
    check(!adapter.store_progress(0,0xdead,1,0,e),"unknown source field rejects");
    QuestPersistenceStateV51* q{};check(adapter.resolve({0,0,0},q,e),e.c_str());
    check(q&&q==owner->resolve_v70(save->regular_quests_v45().source_quests_v45()[0][0]),"SAME quest receiver");
    if(!q->objectives.empty()){q->objectives[0].quantity20=19;q->objectives[0].completed14=1;check(adapter.list(0,0,text,views,e),e.c_str());check(views[0].objectives[0].quantity==19&&views[0].objectives[0].completed==1,"live objective saved fields");}
    check(!adapter.resolve({2,0,0},q,e)&&!q,"invalid collection");check(!adapter.resolve({0,3,0},q,e),"invalid difficulty");
    const auto oldsize=views.size();check(!adapter.list(0,0,{},views,e)&&views.size()==oldsize,"missing localization rejects without menu partial publication");
    {
        auto payload=std::make_shared<int>(9);std::int32_t event_type=5,event_id=366,quantity=-1;std::uint8_t pending{},from_network{};std::uintptr_t actor=0x1234;
        dh2::loader::GameEventQuestBorrowV75 fields;fields.receiver=payload;fields.character8_cell=&actor;fields.id18_cell=&event_id;
        fields.pending_network10=&pending;fields.from_network11=&from_network;fields.quantity14=&quantity;
        dh2::loader::ScopedGameQuestEventV75 event(reinterpret_cast<std::uintptr_t>(payload.get()),&event_type,std::move(fields));
        dh2::loader::GameEventRuntimeServicesV75 projected;projected.provider=save;
        check(bind_source_quest_event_projection_v108(projected,e),e.c_str());
        dh2::loader::GameEventQuestBorrowV75 actual_event;
        check(projected.project_event(event.borrow(),actual_event,e),e.c_str());
        check(actual_event.receiver==payload&&actual_event.character8==actor&&actual_event.id18==366&&
              actual_event.pending_network10==&pending&&actual_event.quantity14==&quantity,
              "Exact scoped QE_TalkToNPC fields reach the shared native objective projector");
    }
    OriginalQuestAdapter missing({});check(!missing.resolve({0,0,0},q,e),"missing actual owner rejects");
    bool satisfied=true;check(!adapter.prerequisites({0,0,0},satisfied,e)&&!satisfied,"missing unlock provider never succeeds");
    check(!adapter.compile(0,false,{},e)&&!adapter.update(0,{},e),"missing runtime never progresses");
    dh2::world::NativeQuestFrameServicesV108 script_frame;
    {auto campaign=std::make_shared<OriginalCampaignRuntime>();bind_original_quest_scripts(script_frame,campaign);}
    std::int32_t script=-1;check(!script_frame.script_id("level.missing",script,e),"expired original campaign fails");
    check(script_frame.script_id("unqualified",script,e)&&script==-1,"source unqualified quest script omitted");
    SourceQuestServices binding_services;binding_services.world_owner=save;binding_services.campaign=std::make_shared<OriginalCampaignRuntime>();
    binding_services.local_quests=[owner](auto& out,auto& error){out=owner;error.clear();return true;};
    binding_services.character_quests=[owner](auto character,auto& out,auto& error){check(character==owner->save()->character(),"source existing character association");out=owner;error.clear();return true;};
    binding_services.difficulty=[](auto,auto& out,auto& error){out=0;error.clear();return true;};
    binding_services.online=[](auto& out,auto& error){out=false;error.clear();return true;};
    binding_services.constant=[](auto,auto,auto&,auto& error){error="fixture has no constant lookup";return false;};
    binding_services.application_time70=[](auto&,auto& error){error="fixture has no application clock";return false;};
    binding_services.constant=[](const char* group,const char* key,auto& out,auto& error){
        check(std::string(group)=="v2QuestPriority"&&std::string(key)=="Primary","Quest Log primary source constant");out=1;error.clear();return true;};
    auto binding=std::make_shared<SourceQuestServiceBinding>(binding_services);dh2::world::NativeQuestFrameServicesV108 bound_frame;
    check(binding->frame_services(bound_frame,e),e.c_str());bool permitted=true;
    check(bound_frame.frame_gate(permitted,e)&&!permitted,"SAME actual Save quest_sync14 zero gates transition");
    check(bound_frame.current_act(5,-1,e)&&progress->current_act[0]==5,"bound frame source Act store on same local Save");
    const QuestLogFunctorV108 missing_source_category{};
    const QuestLogTextV108 missing_source_text{};
    std::vector<QuestLogEntryV108> log;
    check(!binding->quest_log(QuestLogCategoryV108::active,missing_source_category,missing_source_text,log,e),
          "Quest Log refuses to infer source category or localization when native owners are unavailable");
    QuestLogDetailsV108 missing_detail;
    check(!binding->quest_log_details({0,0,0},missing_source_text,missing_detail,e),
          "Quest details refuse fallback text without the actual StringManager resolver");
    const auto saved_current_before_missing_functor=progress->current_quest[0];
    check(!binding->quest_log_activate({0,0,0},missing_source_category,e)&&
          progress->current_quest[0]==saved_current_before_missing_functor,
          "Quest activation refuses to mutate currentquest without the native category predicate");
    bool reward_given=true;check(!bound_frame.give_reward(0x1234,{},reward_given,e)&&!reward_given,"missing actual reward leaf fails on reach");
    dh2::world::NativeConditionStateV69 state;check(!binding->quest_state(0x1234,0,-1,state,e)&&!state.state0,"source unlock fails without published same native runtime");
    check(!binding->update(0x1234,false,e),"source SG_Update requires native sync producer");
    check(!binding->publish(owner,{},e),"shared Condition/Objective owners mandatory");
    check(!binding->quest_state(0x1234,-1,-1,state,e),"GetQuestByID requires actual assertion owner at invalid bound");
    binding_services.assertion_mode=[](auto& out,auto& error){out=0;error.clear();return true;};
    auto mode0binding=std::make_shared<SourceQuestServiceBinding>(binding_services);
    check(mode0binding->quest_state(0x1234,-1,-1,state,e)&&!state.receiver&&!state.state0,"native disabled-diagnostic invalid bound returns NULL before compile");
    binding_services.assertion_mode=[](auto& out,auto& error){out=2;error.clear();return true;};
    auto mode2binding=std::make_shared<SourceQuestServiceBinding>(binding_services);
    check(!mode2binding->quest_state(0x1234,-1,-1,state,e),"source assertion mode2 invalid bound fails");
    // Source CompileQuests: writes cache28 before callback failure; no fabricated completed condition.
    auto& collection=save->regular_quests_v45();dh2::world::QuestConditionCompileBorrowV70 fields{std::shared_ptr<void>(save,&collection),&collection,collection.source_character5c_v70(),collection.source_compiled28_v70()};
    dh2::world::QuestConditionCompileServicesV70 services;services.transport=save;
    services.character_difficulty3bb8e4=[](auto character,auto& out,auto& error){check(character==0x1234,"actual character difficulty");out=0;error.clear();return true;};
    check(!dh2::world::quest_condition_compile_v70(fields,false,services,e)&&*fields.compiled28[0]==1,"original interrupted compile cache prefix");
    std::vector<std::uintptr_t> compiled;services.quest_compile480178=[&](auto id,auto& error){compiled.push_back(id);error.clear();return true;};
    check(dh2::world::quest_condition_compile_v70(fields,true,services,e)&&compiled==collection.source_quests_v45()[0],"source ordered full forced compile");
    // Native RewardList consumes actual table rows in order and stops on the
    // original virtual false result, distinct from callback transport success.
    const std::vector<QuestRewardDefinitionV51>* rewards=nullptr;
    for(const auto& row:tables->rows())for(const auto& list:row.rewards)if(list.size()>1&&!rewards)rewards=&list;
    check(rewards!=nullptr,"original multi-reward fixture");std::size_t given=0;
    check(dh2::world::quest_reward_sequence_v108(0x1234,*rewards,[&](auto character,const auto& reward,bool& result,auto& error){
        check(character==0x1234&&reward.type==(*rewards)[given].type&&reward.parameter1==(*rewards)[given].parameter1&&reward.parameter2==(*rewards)[given].parameter2,"actual reward callback row");
        ++given;result=false;error.clear();return true;
    },e)&&given==1,"native reward stop-on-false");
    check(!dh2::world::quest_reward_sequence_v108(0x1234,*rewards,{},e),"missing native reward provider fails");
    dh2::world::NativeQuestRuntimeV76 missing_runtime(owner,{});
    dh2::world::NativeQuestFrameServicesV108 incomplete_frame;incomplete_frame.provider=save;
    check(!missing_runtime.bind_frame_v108(incomplete_frame,e),"missing native state frame leaves rejected");
    check(!missing_runtime.compile(collection.source_quests_v45()[0][0],e)&&missing_runtime.failed(),"missing objective/condition provider retained failure");
    std::cout<<"PASS "<<tables->rows().size()<<" original rows across 6 collections/difficulties; same-owner objective/progress and native compile prefixes\n";
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
