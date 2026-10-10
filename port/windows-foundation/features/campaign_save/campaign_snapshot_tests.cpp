#include "campaign_snapshot.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::campaign_save;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
void word(std::vector<std::uint8_t>& b,std::int32_t value){for(unsigned i=0;i<4;++i)b.push_back(std::uint8_t(std::uint32_t(value)>>(i*8)));}
void objective(std::vector<std::uint8_t>& b,int type){word(b,type);for(int i=0;i<7;++i)word(b,0);}
std::shared_ptr<dh2::data::QuestTablesPersistenceV51> table(){
    // Fixture rows use the genuine v2Quest read field order, not a new quest FSM.
    std::vector<std::uint8_t> rows,names;word(rows,1);for(int i=0;i<4;++i)word(rows,0);
    word(rows,0);word(rows,1);objective(rows,0);for(int i=0;i<3;++i)word(rows,0);
    objective(rows,4);objective(rows,6);word(rows,1);rows.push_back(0);word(rows,2);
    for(int i=0;i<14;++i)word(rows,0);word(rows,0);word(rows,1);
    word(names,1);std::string name="fixture.quest";word(names,std::int32_t(name.size()+1));
    names.insert(names.end(),name.begin(),name.end());names.push_back(0);
    auto result=std::make_shared<dh2::data::QuestTablesPersistenceV51>();std::string error;
    check(result->decode({rows.data(),rows.size()},{names.data(),names.size()},error),error);return result;
}
GameSave canonical(){
    GameSave save;save.level_uri="fixture.level";save.controlled_actor_id=1;
    save.character=make_default_character("player","Player","warrior");
    PersistedPlayableActor p;p.actor.id=1;p.actor.definition_id="fixture.actor";p.actor.faction_id=0;
    p.actor.health=p.actor.max_health=p.actor.resource=p.actor.max_resource=100;
    p.actor.persistent_character_id="player";
    for(unsigned index:{36u,38u,41u,43u})p.combat.sheets.resolved[index]=25600;
    p.traits.is_player=true;save.actors.push_back(p);return save;
}
struct Prepared final:PreparedCampaignRestore{
    int* live;int staged;Prepared(int& value,int next):live(&value),staged(next){}
    void commit()noexcept override{*live=staged;}
};
}
int main(){try{
    std::string error;auto tables=table();
    dh2::data::QuestSavegameV1 collection;dh2::data::QuestPersistenceOwnerV51 quests;
    check(quests.construct(tables,0x1234abcdfedc9999ull,collection,error),error);
    auto* original=quests.resolve(collection.source_quests_v45()[1][0]);
    original->state=5;original->objectives[0].quantity20=7;original->objectives[0].completed14=1;
    dh2::level::QuestSaveCollectionBorrowV45 writer;
    writer.receiver=tables;writer.quests=&collection.source_quests_v45();writer.progress=&collection.progress();
    writer.save_quest=[&](std::uintptr_t identity,dh2::level::SavegameStreamV2& stream,std::string& e){
        std::vector<std::uint8_t> bytes;if(!quests.save_quest(identity,bytes,e))return false;return stream.write({bytes.data(),bytes.size()},e);
    };
    SourceRequirement q{"player/regular-quests","fixture-native-table-revision",SourceCodec::quest_collection_v45};
    SourceSection quest;check(capture_quest_collection(q,writer,quest,error),error);
    std::shared_ptr<StagedQuestCollection> stage;
    check(stage_quest_collection(quest,tables,0xfedc1234abcd8888ull,stage,error),error);
    auto* restored=stage->owner.resolve(stage->collection.source_quests_v45()[1][0]);
    check(restored->state==5&&restored->objectives[0].quantity20==7&&restored->objectives[0].completed14==1,
          "Actual source quest cells did not roundtrip");
    check(restored->character_owner==0xfedc1234abcd8888ull,"Native identity was serialized instead of reconstructed");
    const auto preserved=stage;auto malformed=quest;malformed.bytes.pop_back();
    check(!stage_quest_collection(malformed,tables,0,stage,error)&&stage==preserved,"Truncated QEST changed detached output");
    malformed=quest;malformed.bytes.push_back(0);
    check(!stage_quest_collection(malformed,tables,0,stage,error)&&stage==preserved,"Trailing QEST accepted");
    malformed=quest;malformed.bytes[0]=0;
    check(!stage_quest_collection(malformed,tables,0,stage,error),"Native count-mismatch success was mistaken for complete restore");
    std::uint8_t visible=1,enabled=0,tested1=1,tested2=1;bool produced=true;
    dh2::world::ObjectSaveRestoreBorrowV3 base{1,&visible,&enabled,&tested1,&tested2,nullptr,nullptr,&produced};
    SourceRequirement b{"level/module/object/base","fixture-object-revision",SourceCodec::object_base_v3};
    SourceSection object;check(capture_object_base(b,base,object,error),error);
    check(object.bytes==std::vector<std::uint8_t>({1,0}),"Source ObjectBase wire contains non-stable cells");
    produced=false;check(!capture_object_base(b,base,object,error),"Unproduced visibility accepted");produced=true;
    SourceCaptureServices services;
    services.enumerate=[&](auto& out,std::string&){out={q,b};return true;};
    services.capture=[&](const auto& req,SourceSection& out,std::string&){out=req.codec==q.codec?quest:object;return true;};
    services.validate=[&](const SourceSection& section,std::string& e){
        if(section.requirement.codec==SourceCodec::object_base_v3)return section.bytes.size()==2;
        std::shared_ptr<StagedQuestCollection> detached;return stage_quest_collection(section,tables,0,detached,e);
    };
    CampaignSnapshot snapshot;check(capture_campaign_snapshot(canonical(),services,snapshot,error),error);
    check(snapshot.sections.size()==2&&snapshot.canonical.character.inventory.empty(),"Canonical inventory duplicated or source coverage lost");
    const auto previous=snapshot;auto incomplete=services;
    incomplete.enumerate=[&](auto& out,std::string&){out={q,b,{"registered-actor/source-lifecycle","revision",SourceCodec::actor_lifecycle_v3}};return true;};
    check(!capture_campaign_snapshot(canonical(),incomplete,snapshot,error)&&snapshot.sections.size()==previous.sections.size(),
          "Incomplete lifecycle was silently saved or output overwritten");
    int live=9;SourceRestoreServices restore;restore.source=services;
    restore.prepare=[&](const CampaignSnapshot&,std::unique_ptr<PreparedCampaignRestore>& out,std::string&){
        out=std::make_unique<Prepared>(live,17);return true;
    };
    std::unique_ptr<PreparedCampaignRestore> prepared;
    check(prepare_campaign_restore(snapshot,restore,prepared,error)&&live==9,"Prepare mutated live state");
    auto wrong=snapshot;wrong.sections[0].requirement.definition_revision="changed";
    auto* previous_stage=prepared.get();
    check(!prepare_campaign_restore(wrong,restore,prepared,error)&&prepared.get()==previous_stage&&live==9,
          "Wrong source revision changed prepared/live state");
    prepared->commit();check(live==17,"Prepared publication failed");
    std::cout<<"campaign snapshot tests passed: native QEST/base wire, detached stages, exact coverage, unsupported lifecycle rejection, transactional factory contract\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
