#include "player_offline_registration_v1.hpp"
#include <array>
#include <cstddef>
#include <limits>

namespace dh2::player_offline_registration_v1 {
namespace {
bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    if (!a || !b || !an || !bn) return false;
    auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    if (an>UINTPTR_MAX-x || bn>UINTPTR_MAX-y) return true;
    return x<y+bn && y<x+an;
}
bool player_valid(const PlayerInfo* p) {return p && p->identity && p->character_level_member;}
bool registry_valid(const Registry* p) {
    if (!p || !p->manager_identity || !player_valid(p->manager_plus_8) ||
        p->entry_count>4096 || (p->entry_count && !p->entries)) return false;
    for (std::uint32_t i=0;i<p->entry_count;++i)
        if (!player_valid(p->entries[i]) || (i && p->entries[i-1]->internal_id>=p->entries[i]->internal_id)) return false;
    return true;
}
bool output_alias(const Result* out,const Registry& r,const Services* s=nullptr) {
    if (overlap(out,sizeof(*out),&r,sizeof(r)) ||
        overlap(out,sizeof(*out),r.entries,r.entry_count*sizeof(*r.entries)) ||
        overlap(out,sizeof(*out),r.manager_plus_8,sizeof(*r.manager_plus_8)) ||
        (s && (overlap(out,sizeof(*out),s,sizeof(*s)) ||
               overlap(out,sizeof(*out),s->queries,s->queries?sizeof(*s->queries):0) ||
               overlap(out,sizeof(*out),s->selection,s->selection?sizeof(*s->selection):0)))) return true;
    for (std::uint32_t i=0;i<r.entry_count;++i)
        if (overlap(out,sizeof(*out),r.entries[i],sizeof(*r.entries[i]))) return true;
    return false;
}
template<class T> bool aligned(const T* p) {return p && reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
template<class F,class... A>
bool send(Result& r,Operation op,void* context,F fn,A... args) {
    r.last_operation=op;
    if (!fn) {r.status=Status::service_unavailable;return false;}
    ++r.service_calls;
    try {if (fn(context,args...)==0) return true;} catch (...) {}
    r.status=Status::service_failed;return false;
}
bool missing(Result& r) {r.status=Status::missing_projection;return false;}
bool fields(const Services& s,PlayerInfo* p,SourceFields& f,Result& r,const Result* out) {
    if (!player_valid(p)) return missing(r);
    if (!send(r,Operation::fields,s.context,s.fields,p,&f)) return false;
    std::array<std::int32_t*,6> words{{f.save_slot_664,f.controller_668,f.internal_id_670,
                                    f.member_674,f.number_678,f.group_number_67c}};
    if (f.player!=p || !f.local_66c) return missing(r);
    for (unsigned i=0;i<words.size();++i) {
        if (!aligned(words[i])) return missing(r);
        if (overlap(out,sizeof(*out),words[i],4) || overlap(words[i],4,p,sizeof(*p)) ||
            overlap(words[i],4,f.local_66c,1)) {r.status=Status::invalid_argument;return false;}
        for (unsigned j=0;j<i;++j) if (overlap(words[i],4,words[j],4)) {r.status=Status::invalid_argument;return false;}
    }
    if (overlap(out,sizeof(*out),f.local_66c,1)) {r.status=Status::invalid_argument;return false;}
    return true;
}
bool find(const Registry& p,std::int32_t key,Result& r) {
    std::uint32_t low=0,high=p.entry_count;
    while (low<high) {auto mid=low+(high-low)/2;++r.entries_examined;
        if (p.entries[mid]->internal_id<key) low=mid+1;else high=mid;}
    return low<p.entry_count && p.entries[low]->internal_id==key;
}
bool after_online(const Services& s,Result& r,std::uint8_t online,bool& network) {
    network=false;const auto* q=s.queries;
    if (!online) return true;
    std::uint8_t game=0;
    if (!send(r,Operation::game_state_online,q->context,q->game_state_online,&game)) return false;
    if (!game) return true;
    player_locality_v1::Matching* matching=nullptr;std::int32_t room=0;
    if (!send(r,Operation::acquire_matching,q->context,q->acquire_matching,&matching)) return false;
    if (!matching || !matching->identity) return missing(r);
    if (!send(r,Operation::matching_in_room,q->context,q->matching_in_room,matching,&room)) return false;
    if (!room) return true;
    std::uintptr_t net=0;std::int32_t initialized=0;
    if (!send(r,Operation::acquire_net_manager,q->context,q->acquire_net_manager,&net)) return false;
    if (!net) return missing(r);
    if (!send(r,Operation::net_initialized,q->context,q->net_initialized,net,&initialized)) return false;
    network=initialized!=0;return true;
}
bool network_route(const Services& s,Result& r,bool& network) {
    if (!s.queries) {r.status=Status::service_unavailable;r.last_operation=Operation::online;return false;}
    std::uint8_t online=0;
    if (!send(r,Operation::online,s.queries->context,s.queries->online,&online)) return false;
    return after_online(s,r,online,network);
}
bool internal(const Registry& p,const Services& s,std::int32_t id,PlayerInfo*& player,Result& r) {
    const auto* q=s.queries;
    if (!q) {r.status=Status::service_unavailable;r.last_operation=Operation::internal_id_player;return false;}
    if (!send(r,Operation::internal_id_player,q->context,q->internal_id_player,&p,id,0u,&player)) return false;
    return player_valid(player)?true:missing(r);
}
bool renumber(Registry& p,const Services& s,Result& r,const Result* out) {
    ++r.renumber_calls;bool network=false;
    if (!network_route(s,r,network)) return false;
    std::int32_t number=0,local=0,remote=0;
    if (!network) {
        for (std::uint32_t i=0;i<p.entry_count;++i) {
            if (!registry_valid(&p)) {r.status=Status::invalid_registry;return false;}
            auto* selected=p.entries[i];SourceFields f;
            if (!fields(s,selected,f,r,out)) return false;
            const auto controlled=*f.local_66c;
            *f.number_678=number++;++r.source_writes;
            *f.group_number_67c=controlled?local++:remote++;++r.source_writes;
        }
        return true;
    }
    const auto* q=s.queries;const std::int32_t* ids=nullptr;std::uint32_t count=0;
    auto span=[&]() {
        if (!send(r,Operation::net_ids,q->context,q->net_ids,&p,&ids,&count)) return false;
        if (count>4096 || (count && !ids)) return missing(r);
        return true;
    };
    if (!span()) return false;
    for (std::uint32_t i=0;i<count;++i) {
        PlayerInfo* selected=nullptr;SourceFields f;
        if (!internal(p,s,ids[i],selected,r) || !fields(s,selected,f,r,out)) return false;
        if (*f.internal_id_670==-1) {*f.number_678=-1;++r.source_writes;}
        else {
            const auto controlled=*f.local_66c;
            *f.number_678=number++;++r.source_writes;
            *f.group_number_67c=controlled?local++:remote++;++r.source_writes;
        }
        if (!span()) return false;
    }
    return true;
}
bool add(Registry& p,const Services& s,std::int32_t id,std::int32_t member,
         std::int32_t controller,std::uint8_t local,Result& r,const Result* out) {
    bool network=false;if (!network_route(s,r,network)) return false;
    if (!network) {
        if (find(p,id,r)) {++r.duplicate_adds;return true;}
        PlayerInfo* temporary=nullptr;
        if (!send(r,Operation::construct_temporary,s.context,s.construct_temporary,&temporary)) return false;
        r.temporary=temporary;SourceFields f;
        if (!fields(s,temporary,f,r,out)) return false;
        // Exact source stack-object stores at378b58/5c/60/64.
        *f.internal_id_670=id;++r.source_writes;
        *f.local_66c=local;++r.source_writes;
        *f.controller_668=controller;++r.source_writes;
        *f.member_674=member;++r.source_writes;
        PlayerInfo* destination=nullptr;
        if (!send(r,Operation::map_subscript,s.context,s.map_subscript,&p,id,&destination)) return false;
        r.destination=destination;
        if (!player_valid(destination) || destination==temporary || !registry_valid(&p) ||
            !find(p,id,r) || destination->internal_id!=id) return missing(r);
        bool canonical=false;for (std::uint32_t i=0;i<p.entry_count;++i) if(p.entries[i]==destination)canonical=true;
        if (!canonical) return missing(r);
        if (!send(r,Operation::assign,s.context,s.assign,destination,static_cast<const PlayerInfo*>(temporary))) return false;
        ++r.added_players;
        if (!send(r,Operation::destroy_temporary,s.context,s.destroy_temporary,temporary)) return false;
    } else {
        PlayerInfo* selected=nullptr;std::int32_t active=0;
        if (!internal(p,s,id,selected,r) || !send(r,Operation::is_active,s.context,s.is_active,selected,&active)) return false;
        if (!active) return true;
        SourceFields f;if (!fields(s,selected,f,r,out)) return false;r.destination=selected;
        *f.local_66c=local;++r.source_writes;
        *f.controller_668=controller;++r.source_writes;
        *f.member_674=member;++r.source_writes;
        *f.internal_id_670=id;++r.source_writes;
        if (!send(r,Operation::set_enabled,s.context,s.set_enabled,selected,1u)) return false;
    }
    return renumber(p,s,r,out);
}
bool notify(const Services& s,PlayerInfo* player,const SourceFields& f,
            Result& r) {
    std::uintptr_t level=0,hud=0,value=0;
    if (!send(r,Operation::current_level,s.context,s.current_level,&level)) return false;
    if (!level) return true;
    if (!send(r,Operation::hud_root,s.context,s.hud_root,&hud)) return false;
    if (!hud) return missing(r);
    const auto number=*f.group_number_67c;
    if (!send(r,Operation::numeric_value,s.context,s.numeric_value,number,&value)) return false;
    if (!value) return missing(r);
    if (!send(r,Operation::invoke_as,s.context,s.invoke_as,hud,"menu_HUD_0","onNewPlayerLocal",value)) return false;
    if (!send(r,Operation::drop_as_value,s.context,s.drop_as_value,value)) return false;
    ++r.notifications;
    return send(r,Operation::set_state,s.context,s.set_state,player,std::uint8_t{2});
}
bool check(Registry& p,const Services& s,Result& r,const Result* out) {
    std::uintptr_t manager=0;std::int32_t count=0;
    if (!send(r,Operation::input_manager,s.context,s.input_manager,&manager)) return false;
    if (!manager) return missing(r);
    if (!send(r,Operation::gamepad_count,s.context,s.gamepad_count,manager,&count)) return false;
    if (count<1)count=1;
    if (count>4096) return missing(r);
    for (std::uint32_t index=0;index<static_cast<std::uint32_t>(count);++index) {
        InputDevice device;
        if (!send(r,Operation::gamepad,s.context,s.gamepad,manager,index,&device)) return false;
        if (!device.identity || !device.connected_758) return missing(r);
        const auto connected=*device.connected_758;
        ++r.controllers_examined;
        if (!s.queries) {r.status=Status::service_unavailable;r.last_operation=Operation::online;return false;}
        const auto* q=s.queries;std::uint8_t online=0;bool network=false;
        if (!send(r,Operation::online,q->context,q->online,&online)) return false;
        const bool present=index==0 || connected!=0;
        if (!after_online(s,r,online,network)) return false;
        std::int32_t id=static_cast<std::int32_t>(index),member=-1;bool changed=false;
        if (!network) changed=!find(p,id,r);
        else {
            std::uintptr_t net=0;PlayerInfo* selected=nullptr;std::int32_t active=0;
            if (!send(r,Operation::acquire_net_manager,q->context,q->acquire_net_manager,&net)) return false;
            if (!net) return missing(r);
            if (!send(r,Operation::net_local_player,s.context,s.net_local_player,net,index,&selected)) return false;
            if (!send(r,Operation::is_active,s.context,s.is_active,selected,&active)) return false;
            SourceFields f;if (!fields(s,selected,f,r,out)) return false;
            if (active) {
                if (!aligned(f.network_id_178) || !aligned(f.network_member_1a0))return missing(r);
                id=*f.network_id_178;member=*f.network_member_1a0;
            } else id=member=-1;
            const auto previous=*f.internal_id_670;changed=id!=previous;
            if (changed) {
                if (!s.selection) {r.status=Status::service_unavailable;r.last_operation=Operation::local_player;return false;}
                r.last_operation=Operation::local_player;++r.service_calls;
                player_local_selection_v1::Result local;
                if (player_local_selection_v1::get_local_player(&p,s.selection,0,0,&local)!=player_local_selection_v1::Status::complete) {r.status=Status::service_failed;return false;}
                SourceFields host;if (!fields(s,local.player,host,r,out))return false;
                if (previous==*host.internal_id_670 && *f.save_slot_664==-1) {
                    const auto* a=s.selection;std::uintptr_t application=0;
                    if (!send(r,Operation::application,a->context,a->application,&application))return false;
                    if (!application)return missing(r);
                    player_local_selection_v1::SavegameManager* save=nullptr;
                    if (!send(r,Operation::savegame_manager,a->context,a->savegame_manager_4c,application,&save))return false;
                    if (!save || !save->identity || !aligned(save->last_slot_8))return missing(r);
                    const auto slot=*save->last_slot_8;
                    if(slot!=-1){*f.save_slot_664=slot;++r.source_writes;}
                }
            }
            if (id==-1)continue;
        }
        if (!present) {
            if (!changed) {PlayerInfo* ignored=nullptr;if (!internal(p,s,id,ignored,r))return false;}
            continue;
        }
        if (changed) {if (!add(p,s,id,member,static_cast<std::int32_t>(index),1,r,out))return false;continue;}
        PlayerInfo* selected=nullptr;SourceFields f;
        if (!internal(p,s,id,selected,r) || !fields(s,selected,f,r,out))return false;
        if (*f.save_slot_664!=-1)continue;
        if (!f.joining_state_240)return missing(r);
        if (*f.joining_state_240==2) {
            if (!send(r,Operation::joining_controller,s.context,s.joining_controller,&p,index,id))return false;
            ++r.joining_calls;continue;
        }
        if (!aligned(device.minimum_1e0) || !aligned(device.maximum_1e4) ||
            !aligned(device.axis_1d8) || !device.pressed_1e8)return missing(r);
        volatile float sum=*device.minimum_1e0+*device.maximum_1e4;
        volatile float shifted=sum+1.0f;
        volatile float threshold=shifted*0.5f;
        const bool high=*device.axis_1d8>=threshold;
        const bool pressed=*device.pressed_1e8!=0;
        if (high!=pressed && !notify(s,selected,f,r))return false;
    }
    return true;
}
struct Active {bool& value;explicit Active(bool& p):value(p){value=true;}~Active(){value=false;}};
} // namespace
Status is_player_in_local_map(const Registry* registry,std::int32_t key,bool* found,Result* output) {
    if (!registry || !found || !output || overlap(found,sizeof(*found),output,sizeof(*output)))return Status::invalid_argument;
    if (!registry_valid(registry))return Status::invalid_registry;
    if(output_alias(output,*registry) || overlap(found,sizeof(*found),registry,sizeof(*registry)) ||
       overlap(found,sizeof(*found),registry->entries,registry->entry_count*sizeof(*registry->entries)))return Status::invalid_argument;
    Result result;const bool value=find(*registry,key,result);*found=value;*output=result;return result.status;
}
Status Runtime::add_player(std::int32_t id,std::int32_t member,std::int32_t controller,std::uint32_t local,Result* output) {
    if (!output || local>255 || overlap(output,sizeof(*output),this,sizeof(*this)))return Status::invalid_argument;
    if(!registry_valid(&registry_))return Status::invalid_registry;
    if(output_alias(output,registry_,&services_))return Status::invalid_argument;
    if(active_)return Status::reentrant;
    Active guard(active_);Result result;add(registry_,services_,id,member,controller,static_cast<std::uint8_t>(local),result,output);*output=result;return result.status;
}
Status Runtime::update_player_numbers(Result* output) {
    if (!output || overlap(output,sizeof(*output),this,sizeof(*this)))return Status::invalid_argument;
    if(!registry_valid(&registry_))return Status::invalid_registry;
    if(output_alias(output,registry_,&services_))return Status::invalid_argument;
    if(active_)return Status::reentrant;
    Active guard(active_);Result result;renumber(registry_,services_,result,output);*output=result;return result.status;
}
Status Runtime::check_local_controllers(Result* output) {
    if (!output || overlap(output,sizeof(*output),this,sizeof(*this)))return Status::invalid_argument;
    if(!registry_valid(&registry_))return Status::invalid_registry;
    if(output_alias(output,registry_,&services_))return Status::invalid_argument;
    if(active_)return Status::reentrant;
    Active guard(active_);Result result;check(registry_,services_,result,output);*output=result;return result.status;
}
} // namespace dh2::player_offline_registration_v1
