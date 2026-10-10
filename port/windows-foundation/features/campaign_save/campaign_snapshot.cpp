#include "campaign_snapshot.hpp"
#include <algorithm>
#include <set>
#include <tuple>

namespace dh::foundation::campaign_save {
namespace {
constexpr std::size_t section_limit=16u*1024u*1024u,total_limit=64u*1024u*1024u;
bool text(const std::string& s) {
    if(s.empty()||s.size()>character_text_limit)return false;
    for(unsigned char c:s)if(c<32||c==127)return false;
    return true;
}
using Key=std::pair<std::string,std::uint32_t>;
Key key(const SourceRequirement& r) { return {r.owner_key,static_cast<std::uint32_t>(r.codec)}; }
bool requirement(const SourceRequirement& r,std::string& error) {
    if(!text(r.owner_key)||!text(r.definition_revision)){error="Missing stable source owner/revision";return false;}
    if(r.codec!=SourceCodec::quest_collection_v45 && r.codec!=SourceCodec::object_base_v3) {
        error="Complete source snapshot/restore producer unavailable for required owner: "+r.owner_key;return false;
    }
    return true;
}
bool same(const SourceRequirement& a,const SourceRequirement& b) {
    return a.owner_key==b.owner_key&&a.codec==b.codec&&a.definition_revision==b.definition_revision;
}
}

bool validate_campaign_snapshot(const CampaignSnapshot& snapshot,const std::vector<SourceRequirement>& expected,
                                 const SourceCaptureServices& services,std::string& error) {
    error.clear();
    if(snapshot.extension_version!=2){error="Unsupported campaign extension version";return false;}
    if(!validate_game_save(snapshot.canonical,error))return false;
    if(!services.validate){error="Source payload validation producer is unavailable";return false;}
    if(expected.size()>character_collection_limit||snapshot.sections.size()!=expected.size()){
        error="Campaign source coverage is incomplete or exceeds limits";return false;
    }
    std::map<Key,SourceRequirement> manifest;
    for(const auto& r:expected) {
        if(!requirement(r,error))return false;
        if(!manifest.emplace(key(r),r).second){error="Duplicate source coverage requirement";return false;}
    }
    std::set<Key> found;std::size_t total=0;
    for(const auto& section:snapshot.sections) {
        if(!requirement(section.requirement,error))return false;
        const auto id=key(section.requirement);const auto r=manifest.find(id);
        if(r==manifest.end()||!same(r->second,section.requirement)||!found.insert(id).second){
            error="Campaign source owner/revision/codec coverage differs";return false;
        }
        if(section.bytes.empty()||section.bytes.size()>section_limit||section.bytes.size()>total_limit-total){
            error="Campaign source payload exceeds bounded format";return false;
        }
        total+=section.bytes.size();
        if(section.requirement.codec==SourceCodec::object_base_v3&&section.bytes.size()!=2){
            error="ObjectBase source wire must contain exactly visible80/enabled8a";return false;
        }
        if(!services.validate(section,error))return false;
    }
    return true;
}

bool capture_campaign_snapshot(const GameSave& canonical,const SourceCaptureServices& services,
                                CampaignSnapshot& output,std::string& error) {
    error.clear();
    if(!services.enumerate||!services.capture||!services.validate){error="Complete source capture producers unavailable";return false;}
    std::vector<SourceRequirement> requirements;
    if(!services.enumerate(requirements,error))return false;
    if(requirements.size()>character_collection_limit){error="Campaign requirement count exceeds limit";return false;}
    CampaignSnapshot candidate;candidate.canonical=canonical;
    for(const auto& r:requirements) {
        if(!requirement(r,error))return false;
        SourceSection section;
        if(!services.capture(r,section,error))return false;
        if(!same(r,section.requirement)){error="Source capture changed requested identity/revision";return false;}
        candidate.sections.push_back(std::move(section));
    }
    if(!validate_campaign_snapshot(candidate,requirements,services,error))return false;
    std::sort(candidate.sections.begin(),candidate.sections.end(),[](const auto& a,const auto& b){return key(a.requirement)<key(b.requirement);});
    output=std::move(candidate);return true;
}

bool prepare_campaign_restore(const CampaignSnapshot& snapshot,const SourceRestoreServices& services,
                               std::unique_ptr<PreparedCampaignRestore>& output,std::string& error) {
    error.clear();
    if(!services.source.enumerate||!services.prepare){error="Transactional source restore factory unavailable";return false;}
    std::vector<SourceRequirement> requirements;
    if(!services.source.enumerate(requirements,error)
       ||!validate_campaign_snapshot(snapshot,requirements,services.source,error))return false;
    std::unique_ptr<PreparedCampaignRestore> candidate;
    if(!services.prepare(snapshot,candidate,error))return false;
    if(!candidate){error="Restore factory returned no detached complete world stage";return false;}
    output=std::move(candidate);return true;
}

bool capture_quest_collection(SourceRequirement r,const dh2::level::QuestSaveCollectionBorrowV45& writer,
                               SourceSection& output,std::string& error) {
    error.clear();
    if(r.codec!=SourceCodec::quest_collection_v45){error="QEST capture received a different source codec";return false;}
    if(!requirement(r,error))return false;
    if(writer.quests)for(const auto& list:*writer.quests)if(list.size()>character_collection_limit){error="QEST source count exceeds checkpoint limit";return false;}
    dh2::level::SavegameStreamV2 stream;
    if(!dh2::level::quest_save_collection_v45(writer,stream,error))return false;
    if(stream.bytes().size()>section_limit){error="QEST source wire exceeds section limit";return false;}
    SourceSection candidate{std::move(r),stream.bytes()};output=std::move(candidate);return true;
}

bool stage_quest_collection(const SourceSection& section,std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> tables,
                             std::uintptr_t character,std::shared_ptr<StagedQuestCollection>& output,std::string& error) {
    error.clear();
    if(section.requirement.codec!=SourceCodec::quest_collection_v45){error="QEST restore received a different source codec";return false;}
    if(!requirement(section.requirement,error))return false;
    if(!tables||!tables->ready()||tables->rows().size()>character_collection_limit||section.bytes.empty()||section.bytes.size()>section_limit){error="Actual quest table/payload unavailable or exceeds limit";return false;}
    auto candidate=std::make_shared<StagedQuestCollection>();
    if(!candidate->owner.construct(tables,character,candidate->collection,error))return false;
    std::size_t used=0;std::array<bool,3> mismatch{};
    if(!candidate->collection.load({section.bytes.data(),section.bytes.size()},candidate->owner.load_services(),used,mismatch,error))return false;
    if(std::any_of(mismatch.begin(),mismatch.end(),[](bool value){return value;})||used!=section.bytes.size()){
        error="QEST definition/count mismatch or trailing data";return false;
    }
    output=std::move(candidate);return true;
}

bool capture_object_base(SourceRequirement r,const dh2::world::ObjectSaveRestoreBorrowV3& borrow,
                          SourceSection& output,std::string& error) {
    error.clear();
    if(r.codec!=SourceCodec::object_base_v3){error="ObjectBase capture received a different source codec";return false;}
    if(!requirement(r,error))return false;
    dh2::level::SavegameStreamV2 stream;
    if(!dh2::world::object_base_serialize_v3(stream,borrow,error))return false;
    output={std::move(r),stream.bytes()};return true;
}

} // namespace dh::foundation::campaign_save
