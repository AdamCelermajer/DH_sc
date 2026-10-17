#include "player_local_selection_v1.hpp"
#include <cstddef>
#include <limits>

namespace dh2::player_local_selection_v1 {
namespace {
bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    if (!a || !b || !an || !bn) return false;
    const auto x=reinterpret_cast<std::uintptr_t>(a), y=reinterpret_cast<std::uintptr_t>(b);
    if (an>std::numeric_limits<std::uintptr_t>::max()-x ||
        bn>std::numeric_limits<std::uintptr_t>::max()-y) return true;
    return x<y+bn && y<x+an;
}
bool player_valid(const PlayerInfo* p) {return p && p->identity;}
bool registry_valid(const Registry* r) {
    if (!r || !r->manager_identity || !player_valid(r->manager_plus_8) ||
        r->entry_count>4096 || (r->entry_count && !r->entries)) return false;
    for (std::uint32_t i=0;i<r->entry_count;++i)
        if (!player_valid(r->entries[i]) ||
            (i && r->entries[i-1]->internal_id>=r->entries[i]->internal_id)) return false;
    return true;
}
bool aliases_player(const Result* out,const PlayerInfo* p) {
    return overlap(out,sizeof(*out),p,p?sizeof(*p):0) ||
        (p && overlap(out,sizeof(*out),p->character_level_member,
                      p->character_level_member?sizeof(*p->character_level_member):0));
}
bool aliases_services(const Result* out,const Services* s) {
    return overlap(out,sizeof(*out),s,s?sizeof(*s):0) ||
        (s && overlap(out,sizeof(*out),s->queries,s->queries?sizeof(*s->queries):0));
}
bool aliases_registry(const Result* out,const Registry* r) {
    if (overlap(out,sizeof(*out),r,sizeof(*r)) || aliases_player(out,r->manager_plus_8) ||
        overlap(out,sizeof(*out),r->entries,r->entry_count*sizeof(*r->entries))) return true;
    for (std::uint32_t i=0;i<r->entry_count;++i)
        if (aliases_player(out,r->entries[i])) return true;
    return false;
}
template<class F,class... A>
bool deliver(Result& result,Operation op,void* context,F fn,A... args) {
    result.last_operation=op;
    if (!fn) {result.status=Status::service_unavailable;return false;}
    ++result.service_calls;
    try {if (fn(context,args...)==0) return true;} catch (...) {}
    result.status=Status::service_failed;return false;
}
bool network_route(const Services* s,Result& r,bool& network) {
    const auto* q=s->queries;
    if (!q) {r.last_operation=Operation::online;r.status=Status::service_unavailable;return false;}
    std::uint8_t online=0;
    if (!deliver(r,Operation::online,q->context,q->online,&online)) return false;
    network=false;
    if (!online) return true;
    std::uint8_t game=0;
    if (!deliver(r,Operation::game_state_online,q->context,q->game_state_online,&game)) return false;
    if (!game) return true;
    player_locality_v1::Matching* matching=nullptr;std::int32_t room=0;
    if (!deliver(r,Operation::acquire_matching,q->context,q->acquire_matching,&matching)) return false;
    if (!matching || !matching->identity) {r.status=Status::missing_projection;return false;}
    if (!deliver(r,Operation::matching_in_room,q->context,q->matching_in_room,matching,&room)) return false;
    if (!room) return true;
    std::uintptr_t net=0;std::int32_t initialized=0;
    if (!deliver(r,Operation::acquire_net_manager,q->context,q->acquire_net_manager,&net)) return false;
    if (!net) {r.status=Status::missing_projection;return false;}
    if (!deliver(r,Operation::net_initialized,q->context,q->net_initialized,net,&initialized)) return false;
    network=initialized!=0;return true;
}
bool internal_player(const Registry* registry,const Services* s,std::int32_t id,
                     Result& r,PlayerInfo*& player) {
    const auto* q=s->queries;
    if (!q) {r.last_operation=Operation::internal_id_player;r.status=Status::service_unavailable;return false;}
    if (!deliver(r,Operation::internal_id_player,q->context,q->internal_id_player,
                 registry,id,0u,&player)) return false;
    if (!player_valid(player)) {r.status=Status::missing_projection;return false;}
    return true;
}
bool select_id(const Registry* registry,const Services* s,std::int32_t ordinal,
               std::uint32_t require_character,Result& r) {
    bool network=false;
    if (!network_route(s,r,network)) return false;
    const auto index=static_cast<std::uint32_t>(ordinal);
    if (!network) {
        r.id_route=IdRoute::local_tree;
        // Source compares against the tree size before scanning local entries.
        if (index>=registry->entry_count) return true;
        std::uint32_t local=0;
        for (std::uint32_t i=0;i<registry->entry_count;++i) {
            auto* p=registry->entries[i];std::uint8_t controlled=0;++r.entries_examined;
            if (!deliver(r,Operation::local_controller_66c,s->context,s->local_controller_66c,p,&controlled)) return false;
            if (!controlled) continue;
            if (require_character) {
                const auto* q=s->queries;std::uintptr_t character=0;
                if (!deliver(r,Operation::character_660,q->context,q->character_660,p,&character)) return false;
                if (!character) continue;
            }
            if (local==index)
                return deliver(r,Operation::internal_id_670,s->context,s->internal_id_670,p,&r.internal_id);
            ++local;
        }
        return true;
    }
    r.id_route=IdRoute::network_vector;
    const std::int32_t* ids=nullptr;std::uint32_t count=0;
    auto read_ids=[&]() {
        if (!deliver(r,Operation::local_ids_6b4,s->context,s->local_ids_6b4,registry,&ids,&count)) return false;
        if (count>4096 || (count && !ids)) {r.status=Status::missing_projection;return false;}
        return true;
    };
    if (!read_ids()) return false;
    if (index>=count) return true;
    if (!require_character) {r.internal_id=ids[index];return true;}
    std::uint32_t local=0;
    for (std::uint32_t i=0;i<count;) {
        PlayerInfo* player=nullptr;std::uintptr_t character=0;++r.entries_examined;
        if (!internal_player(registry,s,ids[i],r,player)) return false;
        const auto* q=s->queries;
        if (!deliver(r,Operation::character_660,q->context,q->character_660,player,&character)) return false;
        const auto captured=i++;
        if (character) {
            if (local==index) {
                // Original reloads vector start at the captured byte offset.
                if (!read_ids()) return false;
                if (captured>=count) {r.status=Status::missing_projection;return false;}
                r.internal_id=ids[captured];return true;
            }
            ++local;
        }
        if (!read_ids()) return false;
    }
    return true;
}
bool select_player(const Registry* registry,const Services* s,std::int32_t ordinal,
                   std::uint32_t require_character,Result& r) {
    if (!select_id(registry,s,ordinal,require_character,r)) return false;
    return internal_player(registry,s,r.internal_id,r,r.player);
}
} // namespace
Status get_internal_id_by_local_id(const Registry* registry,const Services* services,
                                   std::int32_t ordinal,std::uint32_t require_character,Result* output) {
    if (!services || !output || aliases_services(output,services)) return Status::invalid_argument;
    if (!registry_valid(registry)) return Status::invalid_registry;
    if (aliases_registry(output,registry)) return Status::invalid_argument;
    Result result;result.manager=registry;select_id(registry,services,ordinal,require_character,result);
    *output=result;return result.status;
}
Status get_local_player(const Registry* registry,const Services* services,std::int32_t ordinal,
                        std::uint32_t require_character,Result* output) {
    if (!services || !output || aliases_services(output,services)) return Status::invalid_argument;
    if (!registry_valid(registry)) return Status::invalid_registry;
    if (aliases_registry(output,registry)) return Status::invalid_argument;
    Result result;result.manager=registry;select_player(registry,services,ordinal,require_character,result);
    if (aliases_player(output,result.player)) return Status::invalid_argument;
    *output=result;return result.status;
}
Status assign_save_slot_to_player(const Services* services,std::int32_t slot,
                                  std::int32_t ordinal,Result* output) {
    if (!services || !output || aliases_services(output,services)) return Status::invalid_argument;
    Result r;
    if (!deliver(r,Operation::application,services->context,services->application,&r.application)) {*output=r;return r.status;}
    if (!r.application) {r.status=Status::missing_projection;*output=r;return r.status;}
    SavegameManager* save=nullptr;
    if (!deliver(r,Operation::savegame_manager_4c,services->context,services->savegame_manager_4c,r.application,&save)) {*output=r;return r.status;}
    if (!save || !save->identity || !save->last_slot_8) {r.status=Status::missing_projection;*output=r;return r.status;}
    if (overlap(output,sizeof(*output),save,sizeof(*save)) || overlap(output,sizeof(*output),save->last_slot_8,sizeof(*save->last_slot_8))) return Status::invalid_argument;
    r.savegame_manager=save->identity;
    *save->last_slot_8=slot;++r.last_slot_writes;
    if (!deliver(r,Operation::player_manager_40,services->context,services->player_manager_40,r.application,&r.manager)) {*output=r;return r.status;}
    if (!registry_valid(r.manager)) {r.status=Status::invalid_registry;*output=r;return r.status;}
    if (aliases_registry(output,r.manager)) return Status::invalid_argument;
    if (!select_player(r.manager,services,ordinal,0,r)) {*output=r;return r.status;}
    if (aliases_player(output,r.player)) return Status::invalid_argument;
    std::int32_t* selected_slot=nullptr;
    if (!deliver(r,Operation::save_slot_664,services->context,services->save_slot_664,r.player,&selected_slot)) {*output=r;return r.status;}
    if (!selected_slot) {r.status=Status::missing_projection;*output=r;return r.status;}
    if (overlap(output,sizeof(*output),selected_slot,sizeof(*selected_slot)) ||
        overlap(selected_slot,sizeof(*selected_slot),save,sizeof(*save)) ||
        selected_slot==save->last_slot_8) return Status::invalid_argument;
    *selected_slot=slot;++r.player_slot_writes;
    *output=r;return r.status;
}
} // namespace dh2::player_local_selection_v1
