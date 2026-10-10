#include "player_manager_friendly_v1.hpp"
#include <cstddef>
#include <limits>
namespace dh2::player_manager_friendly_v1 {
namespace {
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){if(!a||!b||!n||!m)return false;auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);if(n>UINTPTR_MAX-x||m>UINTPTR_MAX-y)return true;return x<y+m&&y<x+n;}
bool valid(const Registry* r){if(!r||!r->manager_identity||!r->manager_plus_8||!r->manager_plus_8->identity||r->entry_count>4096||(r->entry_count&&!r->entries))return false;for(unsigned i=0;i<r->entry_count;++i)if(!r->entries[i]||!r->entries[i]->identity||(i&&r->entries[i-1]->internal_id>=r->entries[i]->internal_id))return false;return true;}
Status guard(const Registry* r,const Services* s,Result* out){
    if(!s||!out)return Status::invalid_argument;
    if(!valid(r))return Status::invalid_registry;
    if(overlap(out,sizeof(*out),r,sizeof(*r))||overlap(out,sizeof(*out),s,sizeof(*s))||overlap(out,sizeof(*out),s->queries,s->queries?sizeof(*s->queries):0)||overlap(out,sizeof(*out),r->entries,r->entry_count*sizeof(*r->entries))||overlap(out,sizeof(*out),r->manager_plus_8,sizeof(*r->manager_plus_8)))return Status::invalid_argument;
    for(unsigned i=0;i<r->entry_count;++i)if(overlap(out,sizeof(*out),r->entries[i],sizeof(*r->entries[i])))return Status::invalid_argument;
    return Status::complete;
}
template<class F,class... A> bool call(Result& r,Operation op,void* context,F fn,A... a){r.last_operation=op;if(!fn){r.status=Status::missing_provider;return false;}++r.calls;try{if(fn(context,a...)==0)return true;}catch(...){}r.status=Status::provider_failed;return false;}
bool missing(Result& r){r.status=Status::missing_projection;return false;}
bool route(const Services& s,Result& r,bool& network){
    network=false;auto* q=s.queries;if(!q){r.last_operation=Operation::online;r.status=Status::missing_provider;return false;}
    std::uint8_t online=0,game=0;int room=0,initialized=0;std::uintptr_t net=0;player_locality_v1::Matching* matching=nullptr;
    if(!call(r,Operation::online,q->context,q->online,&online))return false;
    if(!online)return true;
    if(!call(r,Operation::game_state_online,q->context,q->game_state_online,&game))return false;
    if(!game)return true;
    if(!call(r,Operation::matching,q->context,q->acquire_matching,&matching))return false;
    if(!matching||!matching->identity)return missing(r);
    if(!call(r,Operation::room,q->context,q->matching_in_room,matching,&room))return false;
    if(!room)return true;
    if(!call(r,Operation::net_manager,q->context,q->acquire_net_manager,&net))return false;
    if(!net)return missing(r);
    if(!call(r,Operation::net_initialized,q->context,q->net_initialized,net,&initialized))return false;
    network=initialized!=0;return true;
}
bool span(const Registry& registry,const Services& s,Result& r,const std::int32_t*& ids,unsigned& count){auto* q=s.queries;if(!q){r.status=Status::missing_provider;return false;}if(!call(r,Operation::net_ids,q->context,q->net_ids,&registry,&ids,&count))return false;if(count>4096||(count&&!ids))return missing(r);return true;}
bool count(const Registry& registry,const Services& s,Result& r){bool network=false;if(!route(s,r,network))return false;if(!network){r.value=static_cast<int>(registry.entry_count);return true;}const std::int32_t* ids=nullptr;unsigned size=0;if(!span(registry,s,r,ids,size))return false;r.value=static_cast<int>(size);return true;}
bool select_internal(const Registry& registry,const Services& s,int id,Result& r,PlayerInfo*& player){auto* q=s.queries;if(!q){r.status=Status::missing_provider;return false;}if(!call(r,Operation::internal_player,q->context,q->internal_id_player,&registry,id,0u,&player))return false;return player&&player->identity?true:missing(r);}
bool select(const Registry& registry,const Services& s,int ordinal,unsigned character,Result& r){
    bool network=false;if(!route(s,r,network))return false;
    const auto index=static_cast<unsigned>(ordinal);r.value=-1;
    if(!network){
        if(index>=registry.entry_count)return true;
        unsigned number=0;
        for(unsigned i=0;i<registry.entry_count;++i){auto* player=registry.entries[i];++r.examined;
            if(character){std::uintptr_t value=0;auto* q=s.queries;if(!call(r,Operation::character,q->context,q->character_660,player,&value))return false;if(!value)continue;}
            if(number==index)return call(r,Operation::internal_word,s.context,s.internal_id_670,player,&r.value);
            ++number;
        }
        return true;
    }
    // GetNumPlayers performs a fresh online/room/initialized read, even when
    // the ordinal helper already selected the network route.
    if(!count(registry,s,r))return false;
    const int players=r.value;r.value=-1;
    if(players<=ordinal)return true;
    const std::int32_t* ids=nullptr;unsigned size=0,number=0;
    if(!span(registry,s,r,ids,size))return false;
    for(unsigned i=0;i<size;){PlayerInfo* player=nullptr;++r.examined;
        if(!select_internal(registry,s,ids[i],r,player))return false;
        const unsigned captured=i++;bool include=true;
        if(character){std::uintptr_t value=0;auto* q=s.queries;if(!call(r,Operation::character,q->context,q->character_660,player,&value))return false;include=value!=0;}
        if(include){
            if(number==index){if(!span(registry,s,r,ids,size))return false;if(captured>=size)return missing(r);r.value=ids[captured];return true;}
            ++number;
        }
        if(!span(registry,s,r,ids,size))return false;
    }
    return true;
}
}
Status project_single_player_class(std::int32_t character_class_row,
                                  SinglePlayerClassProjection* output) {
    if (!output) return Status::invalid_argument;
    SinglePlayerClassProjection result{};
    // PlayerManager's Warrior/Rogue/Mage wrappers pass these CharacterTable
    // row indices to GetNumPlayerCharactersOfClass (IDA 0x36eab8..0x36eacc).
    // They are row indices, not each row's separate ClassID property.
    switch (character_class_row) {
    case 263:
        result.visual_class=SinglePlayerVisualClass::warrior;
        result.counts.warrior=1;
        break;
    case 325:
        result.visual_class=SinglePlayerVisualClass::rogue;
        result.counts.rogue=1;
        break;
    case 290:
        result.visual_class=SinglePlayerVisualClass::mage;
        result.counts.mage=1;
        break;
    default: return Status::missing_projection;
    }
    *output=result;
    return Status::complete;
}
Status project_single_player_class_counts(std::int32_t character_class_row,
                                           SinglePlayerClassCounts* output) {
    if (!output) return Status::invalid_argument;
    SinglePlayerClassProjection projection{};
    const auto status=project_single_player_class(character_class_row,&projection);
    if(status!=Status::complete)return status;
    *output=projection.counts;
    return Status::complete;
}
Status get_num_players(const Registry* registry,const Services* services,Result* output){auto status=guard(registry,services,output);if(status!=Status::complete)return status;Result result;count(*registry,*services,result);*output=result;return result.status;}
Status get_internal_id_by_friendly_id(const Registry* registry,const Services* services,int ordinal,unsigned require_character,Result* output){auto status=guard(registry,services,output);if(status!=Status::complete)return status;Result result;select(*registry,*services,ordinal,require_character,result);*output=result;return result.status;}
Status get_player(const Registry* registry,const Services* services,int ordinal,unsigned require_character,Result* output){auto status=guard(registry,services,output);if(status!=Status::complete)return status;Result result;if(select(*registry,*services,ordinal,require_character,result))select_internal(*registry,*services,result.value,result,result.player);*output=result;return result.status;}
} // namespace dh2::player_manager_friendly_v1
