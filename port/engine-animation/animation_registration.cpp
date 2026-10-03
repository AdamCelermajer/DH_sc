#include "animation_registration.hpp"
#include <algorithm>
#include <exception>

namespace dh2::animation {
bool RegistrationSet::append(std::int32_t id,std::uint64_t identity,const Player* player,std::string& error){
    error.clear();
    if(id<0||!identity||!player||occurrences_.size()>=1024){error="Invalid animation registration";return false;}
    try{
        auto occurrences=occurrences_;auto entries=entries_;
        const auto index=static_cast<std::int32_t>(occurrences.size());
        occurrences.push_back({index,id,identity,player});
        const auto found=std::lower_bound(entries.begin(),entries.end(),id,
            [](const RegistrationEntry& entry,std::int32_t key){return entry.dictionary_id<key;});
        if(found==entries.end()||found->dictionary_id!=id)entries.insert(found,{id,index,identity});
        occurrences_.swap(occurrences);entries_.swap(entries);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool RegistrationSet::set_default(std::uint64_t identity,const Player* player,std::string& error){
    error.clear();if(!identity||!player){error="Invalid animation default resource";return false;}
    default_identity_=identity;default_player_=player;return true;
}
void RegistrationSet::refresh_indices(){
    for(auto& entry:entries_){
        const auto found=std::find_if(occurrences_.begin(),occurrences_.end(),
            [&](const RegistrationOccurrence& occurrence){return occurrence.resource_identity==entry.resource_identity;});
        entry.engine_index=found==occurrences_.end()?-1:found->engine_index;
    }
}
std::int32_t RegistrationSet::lookup(std::int32_t id)const{
    const auto found=std::lower_bound(entries_.begin(),entries_.end(),id,
        [](const RegistrationEntry& entry,std::int32_t key){return entry.dictionary_id<key;});
    return found!=entries_.end()&&found->dictionary_id==id?found->engine_index:-1;
}
std::vector<TransformClipInput> RegistrationSet::compiled_inputs()const{
    std::vector<TransformClipInput> result;result.reserve(occurrences_.size());
    for(const auto& occurrence:occurrences_)result.push_back({occurrence.engine_index,occurrence.player});
    return result;
}
}
