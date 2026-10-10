#include "source_owner_capture.hpp"
#include <algorithm>

namespace dh::foundation::campaign_save {
namespace {
bool text(const std::string& value) {
    return !value.empty() && value.size()<=character_text_limit &&
        std::none_of(value.begin(),value.end(),[](unsigned char c){return c<32||c==127;});
}
bool same(const SourceRequirement& a,const SourceRequirement& b) {
    return a.owner_key==b.owner_key&&a.codec==b.codec&&a.definition_revision==b.definition_revision;
}
}
bool SourceOwnerCapture::add_quest_collections(
    const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& owner,
    std::shared_ptr<void> character_lease,const std::array<std::string,2>& keys,
    const std::string& revision,std::string& error) {
    error.clear();
    if(!owner||!owner->save()||!owner->save()->character()||!character_lease||
       !owner->tables()||!owner->tables()->ready()||!text(revision)||
       !text(keys[0])||!text(keys[1])||keys[0]==keys[1]) {
        error="Actual Character/Save/quest tables, lease and distinct source keys required";return false;
    }
    if(std::find(quest_owners_.begin(),quest_owners_.end(),owner.get())!=quest_owners_.end()) {
        error="SAME quest owner already registered";return false;
    }
    if(bindings_.size()+2>character_collection_limit) {error="Source inventory exceeds limit";return false;}
    auto next=bindings_;
    for(unsigned selector=0;selector<2;++selector) {
        SourceRequirement requirement{keys[selector],revision,SourceCodec::quest_collection_v45};
        if(std::any_of(next.begin(),next.end(),[&](const Binding& b){return b.requirement.owner_key==requirement.owner_key&&b.requirement.codec==requirement.codec;})) {
            error="Duplicate source requirement key";return false;
        }
        Binding binding;binding.requirement=requirement;binding.lease=character_lease;
        binding.capture=[owner,selector,requirement](SourceSection& out,std::string& e){
            return capture_quest_collection(requirement,owner->writer(selector),out,e);
        };
        binding.validate=[owner,requirement](const SourceSection& section,std::string& e){
            if(!same(section.requirement,requirement)){e="Quest source identity/revision differs";return false;}
            std::shared_ptr<StagedQuestCollection> detached;
            // Exact native counts/full consumption validated on detached cells.
            // This does NOT register native objective/condition callbacks or
            // publish a reconstructed CharacterMenuQuests owner.
            return stage_quest_collection(section,owner->tables(),owner->save()->character(),detached,e);
        };
        next.push_back(std::move(binding));
    }
    auto owners=quest_owners_;owners.push_back(owner.get());
    bindings_.swap(next);quest_owners_.swap(owners);return true;
}
bool SourceOwnerCapture::require_unsupported(SourceRequirement requirement,std::string& error) {
    error.clear();
    if(!text(requirement.owner_key)||!text(requirement.definition_revision)||
       requirement.codec==SourceCodec::quest_collection_v45||requirement.codec==SourceCodec::object_base_v3) {
        error="Expected an explicitly unsupported required source fragment";return false;
    }
    if(bindings_.size()>=character_collection_limit){error="Source inventory exceeds limit";return false;}
    for(const auto& b:bindings_)if(b.requirement.owner_key==requirement.owner_key&&b.requirement.codec==requirement.codec){error="Duplicate source requirement key";return false;}
    Binding binding;binding.requirement=std::move(requirement);bindings_.push_back(std::move(binding));return true;
}
SourceCaptureServices SourceOwnerCapture::services() {
    std::weak_ptr<SourceOwnerCapture> weak=weak_from_this();SourceCaptureServices result;
    result.enumerate=[weak](std::vector<SourceRequirement>& out,std::string& error){
        const auto self=weak.lock();if(!self){error="Source capture inventory expired";return false;}
        std::vector<SourceRequirement> next;for(const auto& b:self->bindings_)next.push_back(b.requirement);
        out=std::move(next);return true;
    };
    result.capture=[weak](const SourceRequirement& r,SourceSection& out,std::string& error){
        const auto self=weak.lock();if(!self){error="Source capture inventory expired";return false;}
        for(const auto& b:self->bindings_)if(same(b.requirement,r)) {
            if(!b.capture){error="Complete source codec unavailable: "+r.owner_key;return false;}
            return b.capture(out,error);
        }
        error="Source capture requirement is unregistered";return false;
    };
    result.validate=[weak](const SourceSection& section,std::string& error){
        const auto self=weak.lock();if(!self){error="Source capture inventory expired";return false;}
        for(const auto& b:self->bindings_)if(same(b.requirement,section.requirement)) {
            if(!b.validate){error="Complete source codec unavailable: "+section.requirement.owner_key;return false;}
            return b.validate(section,error);
        }
        error="Source validation requirement is unregistered";return false;
    };
    return result;
}
} // namespace dh::foundation::campaign_save
