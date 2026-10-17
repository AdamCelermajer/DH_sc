#include "player_locality_v1.hpp"
#include <cstddef>
#include <limits>

namespace dh2::player_locality_v1 {
namespace {
bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    if (!a || !b || !an || !bn) return false;
    auto x = reinterpret_cast<std::uintptr_t>(a), y = reinterpret_cast<std::uintptr_t>(b);
    if (an > std::numeric_limits<std::uintptr_t>::max()-x ||
        bn > std::numeric_limits<std::uintptr_t>::max()-y) return true;
    return x < y+bn && y < x+an;
}
bool player_valid(const PlayerInfo* p) {return p && p->identity;}
bool matching_valid(const Matching* m) {return m && m->identity;}
bool registry_valid(const Registry* r) {
    if (!r || !r->manager_identity || !player_valid(r->manager_plus_8) ||
        r->entry_count > 4096 || (r->entry_count && !r->entries)) return false;
    for (std::uint32_t i=0; i<r->entry_count; ++i)
        if (!player_valid(r->entries[i]) ||
            (i && r->entries[i-1]->internal_id >= r->entries[i]->internal_id)) return false;
    return true;
}
bool aliases(const Result* out, const Registry* r, const Services* s) {
    if (overlap(out,sizeof(*out),r,sizeof(*r)) ||
        overlap(out,sizeof(*out),s,sizeof(*s)) ||
        overlap(out,sizeof(*out),r->manager_plus_8,sizeof(*r->manager_plus_8)) ||
        overlap(out,sizeof(*out),r->manager_plus_8->character_level_member,
                r->manager_plus_8->character_level_member?sizeof(*r->manager_plus_8->character_level_member):0) ||
        overlap(out,sizeof(*out),r->entries,r->entry_count*sizeof(*r->entries))) return true;
    for (std::uint32_t i=0;i<r->entry_count;++i)
        if (overlap(out,sizeof(*out),r->entries[i],sizeof(*r->entries[i])) ||
            overlap(out,sizeof(*out),r->entries[i]->character_level_member,
                    r->entries[i]->character_level_member?sizeof(*r->entries[i]->character_level_member):0)) return true;
    return false;
}
template<class F, class... A>
bool deliver(const Services* s, Result& r, Operation op, F fn, A... args) {
    r.last_operation=op;
    if (!fn) {r.status=Status::service_unavailable;return false;}
    ++r.service_calls;
    try {if (fn(s->context,args...)==0) return true;} catch (...) {}
    r.status=Status::service_failed;
    return false;
}
bool acquire(const Services* s,Result& r,Matching*& matching) {
    if (!deliver(s,r,Operation::acquire_matching,s->acquire_matching,&matching)) return false;
    if (!matching_valid(matching)) {r.status=Status::missing_projection;return false;}
    return true;
}
void fallback(const Registry* registry,Result& result) {
    result.player=registry->manager_plus_8;
    result.route=Route::manager_plus_8;
    result.registered=false;
}
bool lookup(const Registry* registry,const Services* s,std::uintptr_t character,
            std::uint32_t flag,Result& r) {
    std::uint8_t online=0;
    if (!deliver(s,r,Operation::online,s->online,&online)) return false;
    bool network=false;
    if (online) {
        std::uint8_t game_online=0;
        if (!deliver(s,r,Operation::game_state_online,s->game_state_online,&game_online)) return false;
        if (game_online) {
            Matching* matching=nullptr;std::int32_t host=0;
            if (!acquire(s,r,matching) ||
                !deliver(s,r,Operation::matching_in_room,s->matching_in_room,matching,&host)) return false;
            if (host) {
                std::uintptr_t net=0;std::int32_t initialized=0;
                if (!deliver(s,r,Operation::acquire_net_manager,s->acquire_net_manager,&net)) return false;
                if (!net) {r.status=Status::missing_projection;return false;}
                if (!deliver(s,r,Operation::net_initialized,s->net_initialized,net,&initialized)) return false;
                network=initialized!=0;
            }
        }
    }
    if (!network) {
        for (std::uint32_t i=0;i<registry->entry_count;++i) {
            std::uintptr_t attached=0;++r.entries_examined;
            if (!deliver(s,r,Operation::character_660,s->character_660,registry->entries[i],&attached)) return false;
            if (attached==character) {
                r.player=registry->entries[i];r.route=Route::registered_player;r.registered=true;
                return true;
            }
        }
        fallback(registry,r);return true;
    }
    const std::int32_t* ids=nullptr;std::uint32_t count=0;
    auto read_ids=[&]() {
        if (!deliver(s,r,Operation::net_ids,s->net_ids,registry,&ids,&count)) return false;
        if (count>4096 || (count && !ids)) {r.status=Status::missing_projection;return false;}
        return true;
    };
    if (!read_ids()) return false;
    for (std::uint32_t i=0;i<count;) {
        PlayerInfo* player=nullptr;std::uintptr_t attached=0;++r.entries_examined;
        if (!deliver(s,r,Operation::internal_id_player,s->internal_id_player,registry,ids[i],flag,&player)) return false;
        if (!player_valid(player)) {r.status=Status::missing_projection;return false;}
        if (!deliver(s,r,Operation::character_660,s->character_660,player,&attached)) return false;
        if (attached==character) {
            // Source reloads vector start and the matched index before tailcalling
            // _GetNetPlayerInfo, not the preceding GetPlayerByInternalID probe.
            if (!read_ids()) return false;
            if (i>=count) {r.status=Status::missing_projection;return false;}
            player=nullptr;
            if (!deliver(s,r,Operation::net_player_info,s->net_player_info,registry,ids[i],flag,&player)) return false;
            if (!player_valid(player)) {r.status=Status::missing_projection;return false;}
            r.player=player;r.route=Route::network_player;r.registered=false;return true;
        }
        ++i;
        if (!read_ids()) return false;
    }
    fallback(registry,r);return true;
}
bool is_server(Matching* matching,const Services* s,Result& r,std::int32_t& value) {
    if (!matching_valid(matching) || !matching->active_c) {r.status=Status::missing_projection;return false;}
    if (!*matching->active_c) {value=0;return true;}
    std::int32_t first=0,second=0,server=0;
    if (!deliver(s,r,Operation::matching_member_id,s->matching_member_id,matching,&first)) return false;
    if (first<0) {value=0;return true;}
    if (!deliver(s,r,Operation::matching_member_id,s->matching_member_id,matching,&second) ||
        !deliver(s,r,Operation::matching_server_member_id,s->matching_server_member_id,matching,&server)) return false;
    value=second==server?1:0;return true;
}
} // namespace
void construct_matching_local_fields(MatchingLocalFields& fields) noexcept {
    fields.active_c=0;fields.member_3638=-1;fields.server_member_363c=-2;
}
void reset_matching_local_ids(MatchingLocalFields& fields) noexcept {
    fields.server_member_363c=-2;fields.member_3638=-1;
}
std::int32_t local_member_id(const MatchingLocalFields& f) noexcept {return f.member_3638;}
std::int32_t local_server_member_id(const MatchingLocalFields& f) noexcept {return f.server_member_363c;}
Status get_player_by_character(const Registry* registry,const Services* services,
                              std::uintptr_t character,std::uint32_t flag,Result* output) {
    if (!services || !output) return Status::invalid_argument;
    if (!registry_valid(registry)) return Status::invalid_registry;
    if (aliases(output,registry,services)) return Status::invalid_argument;
    Result result;lookup(registry,services,character,flag,result);*output=result;return result.status;
}
Status is_local_player(const Registry* registry,const Services* services,
                       std::uintptr_t character,Result* output) {
    if (!output) return Status::invalid_argument;
    if (!character) {
        if ((registry && overlap(output,sizeof(*output),registry,sizeof(*registry))) ||
            (services && overlap(output,sizeof(*output),services,sizeof(*services))) ||
            (registry && registry_valid(registry) && aliases(output,registry,services))) return Status::invalid_argument;
        *output=Result{};return Status::complete;
    }
    if (!services) return Status::invalid_argument;
    if (!registry_valid(registry)) return Status::invalid_registry;
    if (aliases(output,registry,services)) return Status::invalid_argument;
    Result result;
    if (lookup(registry,services,character,0,result))
        deliver(services,result,Operation::player_virtual_is_local,
                services->player_virtual_is_local,result.player,&result.value);
    *output=result;return result.status;
}
Status matching_is_server(Matching* matching,const Services* services,Result* output) {
    if (!services || !output || !matching_valid(matching)) return Status::invalid_argument;
    if (overlap(output,sizeof(*output),matching,sizeof(*matching)) ||
        overlap(output,sizeof(*output),services,sizeof(*services)) ||
        overlap(output,sizeof(*output),matching->active_c,matching->active_c?1:0)) return Status::invalid_argument;
    Result result;is_server(matching,services,result,result.value);*output=result;return result.status;
}
Status cnet_player_is_local(PlayerInfo* player,const Services* services,Result* output) {
    if (!services || !output || !player_valid(player)) return Status::invalid_argument;
    if (overlap(output,sizeof(*output),player,sizeof(*player)) ||
        overlap(output,sizeof(*output),player->character_level_member,
                player->character_level_member?sizeof(*player->character_level_member):0) ||
        overlap(output,sizeof(*output),services,sizeof(*services))) return Status::invalid_argument;
    Result result;result.player=player;Matching* matching=nullptr;std::int32_t server=0,member=0;
    if (acquire(services,result,matching) && is_server(matching,services,result,server) &&
        deliver(services,result,Operation::member_1a0,services->member_1a0,player,&member)) {
        if (server && member<0) result.value=1;
        else if (acquire(services,result,matching)) {
            std::int32_t current=0;
            if (deliver(services,result,Operation::matching_member_id,
                        services->matching_member_id,matching,&current)) result.value=member==current?1:0;
        }
    }
    *output=result;return result.status;
}
Status get_matching(const MatchingState* state,const MatchingFactory* factory,MatchingResult* output) {
    if (!state || !output || !state->singleton ||
        overlap(output,sizeof(*output),state,sizeof(*state)) ||
        overlap(output,sizeof(*output),state->singleton,sizeof(*state->singleton)) ||
        overlap(output,sizeof(*output),state->provider,sizeof(*state->provider)) ||
        (factory && overlap(output,sizeof(*output),factory,sizeof(*factory))) ||
        (*state->singleton && overlap(output,sizeof(*output),*state->singleton,sizeof(**state->singleton)))) return Status::invalid_argument;
    MatchingResult result;result.matching=*state->singleton;
    if (result.matching) {*output=result;return Status::complete;}
    if (!state->provider) return Status::invalid_argument;
    auto construct=[&](MatchingKind kind,std::uint32_t bytes,std::uint32_t mode) {
        if (!factory || !factory->construct) {result.status=Status::service_unavailable;return false;}
        Matching* matching=nullptr;++result.constructor_calls;
        try {if (factory->construct(factory->context,kind,bytes,mode,&matching)!=0) {
            result.status=Status::service_failed;return false;
        }} catch (...) {result.status=Status::service_failed;return false;}
        if (!matching_valid(matching)) {result.status=Status::missing_projection;return false;}
        *state->singleton=matching;result.matching=matching;return true;
    };
    const auto provider=*state->provider;
    if (provider==0) *state->provider=1;
    if (provider==0 || provider==1) {
        if (!construct(MatchingKind::local,0xa9e8,0)) {*output=result;return result.status;}
    }
    if (*state->provider==2) {
        if (!construct(MatchingKind::bluetooth,0xabb8,0)) {*output=result;return result.status;}
    }
    if (*state->provider==3) {
        if (!construct(MatchingKind::gl_live,0x6c08,0)) {*output=result;return result.status;}
    }
    if (*state->provider==4) {
        if (!construct(MatchingKind::gl_live,0x6c08,1)) {*output=result;return result.status;}
    }
    result.matching=*state->singleton;*output=result;return Status::complete;
}
} // namespace dh2::player_locality_v1
