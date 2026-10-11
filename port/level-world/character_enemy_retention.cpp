#include "character_enemy_retention.hpp"
#include <cstddef>
#include <cmath>
#include <cstring>

namespace dh2::character_enemy_retention { namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* p, std::size_t n, std::size_t a, Range& r) {
    const auto x = reinterpret_cast<std::uintptr_t>(p);
    if (!p || x % a || x > UINTPTR_MAX - n) return false;
    r = {x, x+n}; return true;
}
bool overlaps(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
template<class T> bool aligned(const T* p) { Range r; return range(p,sizeof(*p),alignof(T),r); }
template<class T> bool safe_view(const T* p, const Range* ranges, unsigned n) {
    Range r; if (!range(p,sizeof(*p),alignof(T),r)) return false;
    for (unsigned i=0;i<n;++i) if(overlaps(r,ranges[i])) return false;
    return true;
}
bool disjoint(const Range* ranges, unsigned n) {
    for(unsigned i=0;i<n;++i) for(unsigned j=0;j<i;++j)
        if(overlaps(ranges[i],ranges[j])) return false;
    return true;
}
bool queue_ranges(const List* l, const SearchServices* s, Range (&r)[4]) {
    if(!range(l,sizeof(*l),alignof(List),r[0]) ||
       !range(s,sizeof(*s),alignof(SearchServices),r[1])) return false;
    if(!l->capacity || l->capacity>65536 || l->count>l->capacity ||
       l->sort!=1 || l->character_flags || l->object_filter ||
       !range(l->heap,l->capacity*sizeof(Target),alignof(Target),r[2]) ||
       !range(l->owner,sizeof(Object),alignof(Object),r[3])) return false;
    return disjoint(r,4) && l->owner->identity &&
        (!l->reference_character || safe_view(l->reference_character,r,3));
}
Status query(const SearchServices& s, List* l, SearchOperation op,
             std::uintptr_t subject, std::uintptr_t other, SearchResponse& r,
             const Point* a=nullptr,const Point* b=nullptr) {
    if(!s.invoke) return Status::service_unavailable;
    r={}; const SearchRequest q{op,subject,other,a,b};
    try { if(s.invoke(s.context,l,&q,&r)) return Status::service_failed; }
    catch(...) { return Status::service_failed; }
    return Status::complete;
}
std::uintptr_t id(const Object* o) { return o ? o->identity : 0; }
bool lower(const Target& a,const Target& b) {
    if((a.flags&1)!=(b.flags&1)) return (b.flags&1)!=0;
    return a.distance>b.distance;
}
void push(List* l,Target t) {
    auto hole=l->count++;
    while(hole) { const auto p=(hole-1)/2; if(!lower(l->heap[p],t)) break;
        l->heap[hole]=l->heap[p]; hole=p; }
    l->heap[hole]=t;
}
Status pop(List* l) {
    Target t{};
    return target_search::dh2_target_pop(l,&t)==0 ? Status::complete : Status::invalid_source_fact;
}
float sub(float a,float b) { volatile float x=a-b; return x; }
float add(float a,float b) { volatile float x=a+b; return x; }
float mul(float a,float b) { volatile float x=a*b; return x; }
float value(std::uint32_t w) { float x; std::memcpy(&x,&w,4); return x; }
std::uint32_t bits(float x) { std::uint32_t w; std::memcpy(&w,&x,4); return w; }
Status valid_character(List* l,Object* c,const SearchServices& s,std::uint32_t& accepted,
                       const Range* ranges,unsigned n) {
    accepted=0;
    auto ref=l->reference_character;
    if(!ref) { accepted=1; return Status::complete; }
    if(!safe_view(ref,ranges,n)) return Status::invalid_source_fact;
    if(ref->character_word1314<c->character_word1310) return Status::complete;
    SearchResponse r;
    auto status=query(s,l,SearchOperation::is_dead,id(c),0,r);
    if(status!=Status::complete || r.identity) return status;
    ref=l->reference_character; // source reload AFTER IsDead
    if(!safe_view(ref,ranges,n)) return Status::invalid_source_fact;
    status=query(s,l,SearchOperation::is_enemy,id(ref),id(c),r);
    if(status!=Status::complete || !r.identity) return status;
    status=query(s,l,SearchOperation::is_player,id(c),0,r);
    if(status!=Status::complete) return status;
    if(!r.identity) { accepted=1; return Status::complete; }
    ref=l->reference_character; // source reload AFTER candidate IsPlayer
    if(!safe_view(ref,ranges,n)) return Status::invalid_source_fact;
    status=query(s,l,SearchOperation::is_player,id(ref),0,r);
    if(status==Status::complete) accepted=!r.identity;
    return status;
}
} // namespace

Status init(List* l,Target* heap,std::uint32_t capacity,Object* owner,const SearchServices* services) {
    Range r[4];
    if(!capacity || capacity>65536 ||
       !range(l,sizeof(*l),alignof(List),r[0]) ||
       !range(services,sizeof(*services),alignof(SearchServices),r[1]) ||
       !range(heap,capacity*sizeof(Target),alignof(Target),r[2]) ||
       !range(owner,sizeof(*owner),alignof(Object),r[3]) || !disjoint(r,4) || !owner->identity)
        return Status::invalid_argument;
    const auto s=*services;
    // This provider uses its own source-backed Character predicate below;
    // keep the generic TargetList filter fields explicitly disabled.
    *l={heap,0,capacity,owner,nullptr,1,0,0};
    SearchResponse out; const auto status=query(s,l,SearchOperation::is_character,id(owner),0,out);
    if(status==Status::complete && out.identity) {
        // SetRefObject reloads its reference object after the virtual callback.
        if(!safe_view(l->owner,r,3)) return Status::invalid_source_fact;
        l->reference_character=l->owner;
    }
    return status;
}
Status character_valid(List* l,Object* c,const SearchServices* services,std::uint32_t* out) {
    Range r[4], extra[2];
    if(!queue_ranges(l,services,r) ||
       !range(c,sizeof(*c),alignof(Object),extra[0]) ||
       !range(out,sizeof(*out),alignof(std::uint32_t),extra[1])) return Status::invalid_argument;
    for(const auto& x:r) if(overlaps(x,extra[1])) return Status::invalid_argument;
    if(overlaps(extra[0],extra[1]) || !safe_view(c,r,3)) return Status::invalid_argument;
    *out=0; const auto s=*services;
    const Range protected_views[]{r[0],r[1],r[2],extra[1]};
    return valid_character(l,c,s,*out,protected_views,4);
}
namespace {
Status search_impl(List* l,const Registry* registry,float radius,const SearchServices* services,
                   const Range* outer,unsigned outer_count) {
    Range queue[4],r[7];
    if(outer_count>4 || !queue_ranges(l,services,queue)) return Status::invalid_argument;
    for(unsigned i=0;i<3;++i) r[i]=queue[i];
    for(unsigned i=0;i<outer_count;++i) r[3+i]=outer[i];
    const unsigned count=3+outer_count;
    if(!safe_view(registry,r,count) || !safe_view(registry->rooms,r,count)) return Status::invalid_argument;
    const auto s=*services;
    SearchResponse out;
    auto status=query(s,l,SearchOperation::look_vector,id(l->owner),0,out);
    if(status!=Status::complete) return status;
    const Point look=out.point;
    if(!safe_view(l->owner,r,count)) return Status::invalid_source_fact;
    status=query(s,l,SearchOperation::target_position,id(l->owner),0,out);
    if(status!=Status::complete) return status;
    const auto* origin=static_cast<const Point*>(out.view);
    if(!safe_view(origin,r,count)) return Status::invalid_source_fact;
    status=query(s,l,SearchOperation::diagnostic_switch,0,0,out);
    if(status!=Status::complete) return status; // genuine ignored normal return
    while(l->count) { status=pop(l); if(status!=Status::complete) return status; }
    // Source asserts this AFTER diagnostic/queue clearing. Assertion bodies
    // are outside the supported nonnegative-radius caller domain (cone=2pi).
    if(!(radius>=0)) return Status::invalid_source_fact;
    float own_radius=0;
    if(l->reference_character) {
        if(!safe_view(l->reference_character,r,count)) return Status::invalid_source_fact;
        status=query(s,l,SearchOperation::melee_radius,id(l->reference_character),0,out);
        if(status!=Status::complete) return status;
        own_radius=out.number;
    }
    auto* end=registry->rooms;
    auto* room=end->next;
    target_search::Entry16* entry=nullptr;
    std::uint32_t visits=0;
    while(room!=end) {
        if(++visits>65536 || !safe_view(room,r,count) || !safe_view(room->objects,r,count))
            return Status::invalid_source_fact;
        if(!entry) entry=room->objects->next;
        if(!safe_view(entry,r,count)) return Status::invalid_source_fact;
        if(entry==room->objects) { room=room->next; entry=nullptr; continue; }
        auto* object=entry->object;
        if(object && !safe_view(object,r,count)) return Status::invalid_source_fact;
        status=query(s,l,SearchOperation::resolve_character,id(object),0,out);
        if(status!=Status::complete) return status;
        auto* character=const_cast<Object*>(static_cast<const Object*>(out.view));
        if(character && !safe_view(character,r,count)) return Status::invalid_source_fact;
        // GetChar executes even for null/self/disabled entries.
        if(object && object!=l->owner && object->visible) {
            status=query(s,l,SearchOperation::is_zonable,id(object),0,out);
            if(status!=Status::complete) return status;
            if(!(out.identity && object->zoned && !object->in_zone)) {
                if(!safe_view(l->owner,r,count)) return Status::invalid_source_fact;
                status=query(s,l,SearchOperation::is_interactive,id(object),id(l->owner),out);
                if(status!=Status::complete) return status;
                if(out.identity && character) { // filter2 excludes noncharacters without virtual+90
                    std::uint32_t accepted=0;
                    status=valid_character(l,character,s,accepted,r,count);
                    if(status!=Status::complete) return status;
                    if(accepted) {
                        status=query(s,l,SearchOperation::interaction_radius,id(object),0,out);
                        if(status!=Status::complete) return status;
                        const auto target_radius=out.number;
                        status=query(s,l,SearchOperation::target_position,id(object),0,out);
                        if(status!=Status::complete) return status;
                        const auto* point=static_cast<const Point*>(out.view);
                        if(!safe_view(point,r,count)) return Status::invalid_source_fact;
                        const Point delta{{sub(point->coordinates[0],origin->coordinates[0]),
                            sub(point->coordinates[1],origin->coordinates[1]),
                            sub(point->coordinates[2],origin->coordinates[2])}};
                        const auto* d=delta.coordinates;
                        const float distance=sub(sub(std::sqrt(add(add(mul(d[0],d[0]),mul(d[1],d[1])),mul(d[2],d[2]))),target_radius),own_radius);
                        if(!(distance>radius)) {
                            // Source still calls angle at full cone; preserve provider effects.
                            status=query(s,l,SearchOperation::angle,0,0,out,&delta,&look);
                            if(status!=Status::complete) return status;
                            const auto angle=value(bits(out.number)&0x7fffffff);
                            if(l->count==l->capacity) return Status::capacity_exhausted;
                            push(l,{object->identity,distance,angle,1,0});
                        }
                    }
                }
            }
        }
        entry=entry->next; // live intrusive Next AFTER callbacks
    }
    return Status::complete;
}
} // namespace
Status search(List* l,const Registry* registry,float radius,const SearchServices* services) {
    return search_impl(l,registry,radius,services,nullptr,0);
}

namespace {
Status invoke(const Services& s,State* state,Result* out,Operation op,Subject kind,
              std::uintptr_t subject,std::uintptr_t peer,Response& response,
              std::uint32_t word=0,std::uint32_t extra=0) {
    if(!s.invoke) return Status::service_unavailable;
    response={}; ++out->service_calls; const Request q{op,kind,subject,peer,word,extra};
    try { if(s.invoke(s.context,state,&q,&response)) return Status::service_failed; }
    catch(...) { return Status::service_failed; }
    return Status::complete;
}
bool table_valid(const data::AggroTable* table,const Range* r,unsigned n) {
    if(!safe_view(table,r,n) || table->count>table->capacity || table->capacity>65536) return false;
    if(!table->count) return true;
    Range entries;
    if(!range(table->entries,table->capacity*sizeof(data::AggroEntry),alignof(data::AggroEntry),entries)) return false;
    for(unsigned i=0;i<n;++i) if(overlaps(entries,r[i])) return false;
    Range table_range; range(table,sizeof(*table),alignof(data::AggroTable),table_range);
    if(overlaps(entries,table_range)) return false;
    for(unsigned i=0;i<table->count;++i)
        if(table->entries[i].reserved || (i && table->entries[i-1].character>=table->entries[i].character)) return false;
    return true;
}
bool contains(const data::AggroTable& table,std::uintptr_t player) {
    std::uint32_t low=0,high=table.count;
    while(low<high) { const auto mid=low+(high-low)/2;
        if(table.entries[mid].character<player) low=mid+1; else high=mid; }
    return low<table.count && table.entries[low].character==player;
}
}
Status update(State* state,std::uintptr_t current,Target* scratch,std::uint32_t capacity,
              const Services* services,Result* out) {
    Range ranges[5];
    if(!capacity || capacity>65536 ||
       !range(state,sizeof(*state),alignof(State),ranges[0]) ||
       !range(services,sizeof(*services),alignof(Services),ranges[1]) ||
       !range(out,sizeof(*out),alignof(Result),ranges[2]) ||
       !range(scratch,capacity*sizeof(Target),alignof(Target),ranges[3]) ||
       !range(state->owner,sizeof(Owner),alignof(Owner),ranges[4]) ||
       !disjoint(ranges,5) || !state->ai || !state->owner->object.identity) return Status::invalid_argument;
    const auto s=*services;
    *out={}; List list{};
    auto status=init(&list,scratch,capacity,&state->owner->object,&s.search);
    if(status!=Status::complete) return status;
    if(!safe_view(list.owner,ranges,4) ||
       (list.reference_character && !safe_view(list.reference_character,ranges,4)))
        return Status::invalid_source_fact;
    Response response;
    status=invoke(s,state,out,Operation::application,Subject::global,0,0,response);
    if(status!=Status::complete) return status;
    const auto* app=static_cast<const Application*>(response.view);
    if(!safe_view(app,ranges,4) || !safe_view(app->level_38,ranges,4)) return Status::invalid_source_fact;
    const auto* rooms=app->level_38->rooms_70; // captured before global table/GetCharAIId
    if(!safe_view(rooms,ranges,4)) return Status::invalid_source_fact;
    const Registry captured_rooms{rooms->rooms}; // source fixed sentinel, Reset rereads its next
    if(!safe_view(state->owner,ranges,4)) return Status::invalid_source_fact;
    const auto query_owner=state->owner->object.identity; // source owner load precedes table load
    status=invoke(s,state,out,Operation::ai_table,Subject::global,0,0,response);
    if(status!=Status::complete) return status;
    const auto* table=static_cast<const AiTable*>(response.view);
    if(!safe_view(table,ranges,4) || !table->count || table->count>65536) return Status::invalid_source_fact;
    Range rows;
    const auto* captured_rows=table->rows;
    const auto captured_count=table->count;
    if(!range(captured_rows,captured_count*sizeof(AiRow),alignof(AiRow),rows)) return Status::invalid_source_fact;
    for(unsigned i=0;i<4;++i) if(overlaps(rows,ranges[i])) return Status::invalid_source_fact;
    status=invoke(s,state,out,Operation::char_ai_id,Subject::owner_ai,query_owner,0,response);
    if(status!=Status::complete) return status;
    if(response.word>=captured_count) return Status::invalid_source_fact;
    out->radius_word=captured_rows[response.word].words[16]; // source row+40 read AFTER callback
    // Scratch is already protected by search_impl's own queue heap range.
    status=search_impl(&list,&captured_rooms,value(out->radius_word),&s.search,ranges,3);
    if(status!=Status::complete) return status;
    out->search_count=list.count;
    if(!list.count) {
        status=invoke(s,state,out,Operation::clear_aggro,Subject::original_ai,state->ai,current,response);
        if(status!=Status::complete) return status;
        status=invoke(s,state,out,Operation::set_target,Subject::original_ai,state->ai,0,response,0);
        if(status!=Status::complete) return status;
        status=invoke(s,state,out,Operation::sync_last_target,Subject::original_ai,state->ai,0,response);
        if(status==Status::complete) out->decision=Decision::cleared_empty_search;
        return status;
    }
    const auto manager=app->player_manager_40; // fresh field AFTER search on captured Application
    if(!manager) return Status::invalid_source_fact;
    status=invoke(s,state,out,Operation::get_local_player,Subject::player_manager,manager,0,response,0,1);
    if(status!=Status::complete) return status;
    auto* owner=state->owner; // capture owner before PlayerInfo field read
    const auto* player_info=static_cast<const PlayerInfo*>(response.view);
    if(!safe_view(owner,ranges,4) || !safe_view(player_info,ranges,4)) return Status::invalid_source_fact;
    out->player=player_info->character_660;
    if(!table_valid(owner->outgoing,ranges,4)) return Status::invalid_source_fact;
    out->known_player=contains(*owner->outgoing,out->player); // captured BEFORE queue pop callbacks
    while(list.count && list.heap[0].identity!=out->player) {
        status=pop(&list); if(status!=Status::complete) return status;
        ++out->unmatched_pops;
    }
    out->found_player=list.count!=0;
    if(out->known_player) { out->decision=Decision::retained_known_player; return Status::complete; }
    if(!out->found_player) { out->decision=Decision::player_not_found; return Status::complete; }
    if(!safe_view(state->owner,ranges,4)) return Status::invalid_source_fact;
    status=invoke(s,state,out,Operation::is_enemy,Subject::owner_ai,state->owner->object.identity,out->player,response);
    if(status!=Status::complete) return status;
    if(!response.identity) { out->decision=Decision::player_not_enemy; return Status::complete; }
    // Caller captures the owner BEFORE reading global Design+30.
    if(!safe_view(state->owner,ranges,4)) return Status::invalid_source_fact;
    const auto add_owner=state->owner->object.identity;
    status=invoke(s,state,out,Operation::design_30,Subject::global,0,0,response);
    if(status!=Status::complete) return status;
    out->added_word=response.word;
    status=invoke(s,state,out,Operation::add_aggro,Subject::owner_ai,add_owner,out->player,response,out->added_word);
    if(status==Status::complete) out->decision=Decision::added_player_aggro;
    return status;
}
} // namespace dh2::character_enemy_retention
