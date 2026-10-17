#include "player_offline_registry_v1.hpp"
#include <stdexcept>
namespace dh2::player_offline_registry_v1 {
namespace p=player_info_record_v1;
namespace r=player_offline_registration_v1;
Owner::Owner(player_manager_host_level::PlayerRegistry& registry,p::Factory& factory,p::Record& fallback,input_manager_v1::Manager& input)
    :registry_(registry),factory_(factory),fallback_(fallback),input_(input){
    if(registry_.entry_count || !registry_.manager_plus_8 || !fallback.constructed)
        throw std::invalid_argument("Offline registry requires the existing empty manager and full fallback");
}
Owner::~Owner(){registry_.entries=nullptr;registry_.entry_count=0;}
Owner::Entry& Owner::retain(int key){retired_.push_back(std::make_unique<Entry>(key));return *retired_.back();}
p::Record* Owner::record(Projection* projection){
    if(projection==registry_.manager_plus_8)return &fallback_;
    for(auto& [key,entry]:entries_){(void)key;if(projection==&entry->projection)return &entry->record;}
    for(auto& entry:retired_)if(projection==&entry->projection)return &entry->record;
    return nullptr;
}
int Owner::temporary(Projection** output){
    if(temporary_)return 1;
    auto& entry=retain(-1);
    if(factory_.construct_record(entry.record)!=p::Status::complete)return 1;
    temporary_=&entry;*output=&entry.projection;return 0;
}
int Owner::subscript(int key,Projection** output){
    auto found=entries_.find(key);
    if(found!=entries_.end()){
        if(!found->second->record.constructed)return 1;
        *output=&found->second->projection;return 0;
    }
    // Source map[] at3785f0..378648: default C1, pair copy, node copy,
    // pair D1, default D1. A direct C1 in the final node loses those effects.
    auto& initial=retain(key);
    if(factory_.construct_record(initial.record)!=p::Status::complete)return 1;
    auto& pair=retain(key);
    if(p::copy_construct(pair.record,initial.record)!=p::Status::complete)return 1;
    auto& node=entries_.try_emplace(key,std::make_unique<Entry>(key)).first->second;
    if(p::copy_construct(node->record,pair.record)!=p::Status::complete)return 1;
    // Publish stable records only after node construction. The vector is an
    // index over this sole map, not a second store of PlayerInfo values.
    std::vector<Projection*> index;for(auto& [id,entry]:entries_){(void)id;index.push_back(&entry->projection);}
    published_.swap(index);
    registry_.entries=published_.data();registry_.entry_count=static_cast<unsigned>(published_.size());
    if(p::destroy(pair.record)!=p::Status::complete || p::destroy(initial.record)!=p::Status::complete)return 1;
    *output=&node->projection;return 0;
}
int Owner::retire(Projection* projection){
    if(!temporary_ || projection!=&temporary_->projection)return 1;
    if(p::destroy(temporary_->record)!=p::Status::complete)return 1;
    temporary_=nullptr;
    // Preserve source pointer-table aliases without dereferencing dead source
    // members: retain retired native storage for the session, with payloads
    // already destroyed. Serialization/network delivery remains unsupported.
    return 0;
}
r::Services Owner::services(const player_locality_v1::Services& queries){
    r::Services s{};s.context=this;s.queries=&queries;
    s.fields=[](void* raw,Projection* projection,r::SourceFields* output){
        auto* value=static_cast<Owner*>(raw)->record(projection);if(!value || !value->constructed)return 1;
        *output={projection,&value->save_slot_664,&value->controller_668,&value->local_66c,
                 &value->internal_id_670,&value->member_674,&value->number_678,&value->group_number_67c,
                 reinterpret_cast<std::uint8_t*>(&value->at(0x220)->header.value),
                 &value->at(0x158)->header.value,&value->at(0x180)->header.value};return 0;
    };
    s.construct_temporary=[](void* raw,Projection** output){return static_cast<Owner*>(raw)->temporary(output);};
    s.map_subscript=[](void* raw,r::Registry* registry,int key,Projection** output){auto& self=*static_cast<Owner*>(raw);return registry==&self.registry_?self.subscript(key,output):1;};
    s.assign=[](void* raw,Projection* destination,const Projection* source){auto& self=*static_cast<Owner*>(raw);auto* d=self.record(destination);auto* s=self.record(const_cast<Projection*>(source));return d&&s&&p::assign(*d,*s)==p::Status::complete?0:1;};
    s.destroy_temporary=[](void* raw,Projection* projection){return static_cast<Owner*>(raw)->retire(projection);};
    s.input_manager=[](void* raw,std::uintptr_t* output){*output=reinterpret_cast<std::uintptr_t>(&static_cast<Owner*>(raw)->input_);return 0;};
    s.gamepad_count=[](void* raw,std::uintptr_t identity,int* output){auto& input=static_cast<Owner*>(raw)->input_;if(identity!=reinterpret_cast<std::uintptr_t>(&input))return 1;*output=input_manager_v1::num_gamepads(input.base);return 0;};
    s.gamepad=[](void* raw,std::uintptr_t identity,unsigned index,r::InputDevice* output){auto& input=static_cast<Owner*>(raw)->input_;if(identity!=reinterpret_cast<std::uintptr_t>(&input))return 1;auto* pad=input_manager_v1::get_gamepad(input,static_cast<int>(index));if(!pad)return 1;
        // Source offsets1d8/1e0/1e4/1e8 belong to the same button14 channel's
        // previous values, rather than a manufactured Android joining flag.
        auto& channel=pad->buttons[14];*output={reinterpret_cast<std::uintptr_t>(pad),&pad->connected_758,&channel.previous_above_threshold,&channel.previous,&channel.previous_minimum,&channel.previous_maximum};return 0;};
    // Joining/HUD and online services stay mandatory when reached. This owner
    // installs no placeholder successes for those unfinished continuations.
    return s;
}
} // namespace dh2::player_offline_registry_v1
