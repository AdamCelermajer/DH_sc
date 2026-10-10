#include "source_quest_service_binding.hpp"
#include "../interactions/npc_source_services.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh2;
namespace {
void check(bool value,const std::string& why){if(!value)throw std::runtime_error(why);}
std::vector<std::uint8_t> read(const std::string& path){
    std::ifstream file(path,std::ios::binary);check(bool(file),"missing actual quest table input: "+path);
    return {std::istreambuf_iterator<char>(file),{}};
}
}
int main(int argc,char** argv){try{
    check(argc==2,"usage: npc_talk_objective_integration_test quest-cache");
    const std::string root=argv[1];std::string error;
    auto records=read(root+"/v2quests_pyarray.bin"),names=read(root+"/v2quests_pyarraynames.bin");
    auto tables=std::make_shared<data::QuestTablesPersistenceV51>();
    check(tables->decode({records.data(),records.size()},{names.data(),names.size()},error),error);
    check(tables->rows().size()==64,"fixture must decode all original 64 Quest rows");
    const auto authored=std::find_if(tables->rows().begin(),tables->rows().end(),[](const auto& row){return row.name=="Abbey_Rescue";});
    check(authored!=tables->rows().end()&&authored->objectives.size()==1&&authored->objectives[0].type==5&&
          authored->objectives[0].oid1==443&&authored->objectives[0].oid2==1&&authored->objectives[0].value==1&&
          authored->objectives[0].on_complete==-1,"original Abbey_Rescue TalkToNPC objective tuple");
    const auto row_index=std::int32_t(authored-tables->rows().begin());

    constexpr std::uintptr_t character=0x1234,level_id=0x194;
    auto save=std::make_shared<data::PlayerSavegameV1>();save->set_character(character);
    std::shared_ptr<character::CharacterMenuQuestsV51> quests;
    check(construct_source_quest_owner(save,tables,quests,error),error);
    OriginalQuestAdapter adapter(quests);data::QuestPersistenceStateV51* quest{};
    check(adapter.resolve({0,0,row_index},quest,error),error);
    check(quest->character_owner==character&&quest->definition==&*authored&&quest->objectives.size()==1,
          "Quest receiver, table row and Character all belong to the same Save owner");
    auto& saved=quest->objectives[0];saved.source_type4=5;saved.runtime_ctor_produced_v76=true;

    // NativeQuestRuntime projects this exact retained Quest row into the
    // GameEvent Objective payload. The borrowed fields below alias its live
    // completed14/quantity20 and derived constructor cells.
    loader::GameEventObjectiveRowV50 objective_row{authored->objectives[0].type,
        authored->objectives[0].description,authored->objectives[0].on_complete,
        authored->objectives[0].oid1,authored->objectives[0].oid2,authored->objectives[0].value,
        authored->objectives[0].str1,authored->objectives[0].str2};
    loader::ObjectiveBorrowV75 objective{
        std::shared_ptr<void>(quests,&saved),reinterpret_cast<std::uintptr_t>(&saved),saved.source_type4,
        saved.compiled8,&objective_row,saved.character_owner,saved.completed14,saved.receiver1c,
        {{loader::ObjectiveWordBorrowV75(&saved.quantity20),
          loader::ObjectiveWordBorrowV75(&saved.words24_2c[0]),
          loader::ObjectiveWordBorrowV75(&saved.words24_2c[1]),
          loader::ObjectiveWordBorrowV75(&saved.words24_2c[2])}}};

    auto world=std::make_shared<int>(1),level=std::make_shared<int>(2);
    events::EventManagerOwnerV12 same_level_dispatcher(level_id);std::int32_t level_row=1;
    std::vector<std::string> order;std::uint8_t pending_seen_during_network{};
    loader::GameEventRuntimeServicesV75 event_services;event_services.provider=world;
    event_services.current_level=[&](std::shared_ptr<void>& lease,events::EventManagerOwnerV12*& dispatcher,
        const std::int32_t*& row,std::string& e){lease=level;dispatcher=&same_level_dispatcher;row=&level_row;e.clear();return true;};
    event_services.talk_flag=[&](std::int32_t npc,std::uint8_t value,std::string& e){
        check(npc==authored->objectives[0].oid1,"actual TalkToNPC marker identity");
        order.push_back(value?"npc_flag_add":"npc_flag_remove");e.clear();return true;};
    event_services.common_script_count=[](std::int32_t& count,std::string& e){count=0;e.clear();return true;};
    event_services.start_script=[](std::int32_t,std::int32_t,bool,std::string& e){
        e="Abbey_Rescue TalkToNPC source row unexpectedly requires an on-complete script";return false;};
    event_services.send_network=[&](const loader::GameEventQuestBorrowV75& event,std::string& e){
        check(event.pending_network10&&event.quantity14,
              "source Raise listener retains pending/quantity fields");
        check(*event.pending_network10==1&&*event.quantity14==1,
              "source Raise listener pending="+std::to_string(*event.pending_network10)+" quantity="+
              std::to_string(*event.quantity14)+" before network check");
        check(event.receiver&&event.character8==character&&event.id18==authored->objectives[0].oid1,
              "same stack-scoped TalkToNPC actor and authored NPC key survive through network check");
        pending_seen_during_network=*event.pending_network10;order.push_back("network_check");e.clear();return true;};
    check(bind_source_quest_event_projection_v108(event_services,error),error);
    auto objectives=std::make_shared<loader::GameEventRuntimeV75>(std::move(event_services));
    check(objectives->compile_objective(objective,error),error);
    check(objectives->register_objective(objective,true,error),error);
    check(saved.quantity20==0&&!saved.completed14&&saved.compiled8&&
          std::find(order.begin(),order.end(),"npc_flag_add")!=order.end(),
          "actual source Objective compiled and registered against same Level dispatcher");

    interactions::NpcInteractServices npc_services;
    check(interactions::bind_npc_quest_raise(npc_services,[&](std::uintptr_t captured,
        interactions::NpcLevelEventBorrow& out,std::string& e){
        check(captured==level_id,"NPC RaiseAsync retains the captured source Level identity");
        out={level,level_id,&same_level_dispatcher};e.clear();return true;},error),error);
    interactions::NpcTalkEvent talk;talk.objective_type=authored->objectives[0].type;
    talk.actor=character;talk.room=level_row;talk.data_id=authored->objectives[0].oid1;
    talk.source_index=-1; // exact QE_TalkToNPC constructor default
    check(npc_services.raise_async(level_id,talk,error),error);
    check(saved.completed14==1&&saved.quantity20==1&&pending_seen_during_network==1,
          "one authored TalkToNPC event changes the same Quest Objective completed14 and event pending byte");
    check(order==std::vector<std::string>{"npc_flag_add","npc_flag_remove","network_check"},
          "native Objective unregister and network guard preserve source callback order");
    check(same_level_dispatcher.delayed_count()==1,
          "source Objective schedules the native observer detach on the same dispatcher");
    check(same_level_dispatcher.update(0.0,error),error);
    check(same_level_dispatcher.delayed_count()==0&&same_level_dispatcher.receiver_count(5)==0,
          "Level dispatcher applies its native deferred detach");
    std::cout<<"PASS original Abbey_Rescue TalkToNPC objective: 64 Quest rows, same Save/Objective, scoped QE_TalkToNPC, Level dispatcher order\n";
    return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
