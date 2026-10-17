#include "character_ai_queue.hpp"
#include <cstddef>
#include <cstring>

namespace dh2::character_ai_queue { namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* p, std::size_t n, std::size_t alignment, Range& r) {
    const auto a = reinterpret_cast<std::uintptr_t>(p);
    if (!p || a % alignment || a > UINTPTR_MAX - n) return false;
    r = {a, a+n}; return true;
}
bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
bool borrowed(const void* p, std::size_t n, std::size_t a, const Range* r) {
    Range b; if (!range(p,n,a,b)) return false;
    for (unsigned i=0; i<4; ++i) if (overlap(b,r[i])) return false;
    return true;
}
bool valid(Entry* e, const Range* r) {
    return borrowed(e,sizeof(*e),alignof(Entry),r) && e->ai &&
        borrowed(e->owner,sizeof(*e->owner),alignof(Owner),r) && e->owner->identity;
}
}
Status advance(State* s, const Services* services, Result* out) {
    Range r[4];
    if (!range(s,sizeof(*s),alignof(State),r[0]) ||
        !range(services,sizeof(*services),alignof(Services),r[1]) ||
        !range(out,sizeof(*out),alignof(Result),r[2]) || s->count>s->capacity ||
        !s->capacity || s->capacity>65536 ||
        !range(s->entries,s->capacity*sizeof(Entry*),alignof(Entry*),r[3])) return Status::invalid_argument;
    for (unsigned i=0; i<4; ++i) for (unsigned j=0; j<i; ++j)
        if (overlap(r[i],r[j])) return Status::invalid_argument;
    const auto bound=*services; *out={};
    const auto done=[&](Decision d) { out->decision=d; out->front=s->count?s->entries[0]->ai:0; return Status::complete; };
    const auto call=[&](Operation op, Entry* entry, std::uintptr_t subject, std::uint32_t& word) {
        if (!bound.invoke) return Status::service_unavailable;
        const Request request{op,subject}; word=0; ++out->calls;
        try { return bound.invoke(bound.context,s,entry,&request,&word) ? Status::service_failed : Status::complete; }
        catch (...) { return Status::service_failed; }
    };
    // Bounds/native projections checked before effects; no virtual predicates.
    for (unsigned i=0; i<s->count; ++i) if (!valid(s->entries[i],r)) return Status::invalid_source_fact;
    const auto captured_timer=s->timer;
    if (captured_timer>0) {
        std::uint32_t delta; auto status=call(Operation::frame_delta,nullptr,0,delta);
        if (status!=Status::complete) return status;
        const auto raw=static_cast<std::uint32_t>(captured_timer)-delta;
        std::memcpy(&s->timer,&raw,4);
        return done(Decision::timer_reduced);
    }
    s->timer=180;
    const auto captured_count=s->count;
    if (captured_count<=1) return done(Decision::reset_small_queue);
    for (auto remaining=captured_count; remaining; ) {
        auto* first=s->entries[0];
        for (unsigned i=1; i<s->count; ++i) s->entries[i-1]=s->entries[i];
        s->entries[s->count-1]=first; ++out->rotations;
        if (--remaining==0) return done(Decision::exhausted);
        auto* entry=s->entries[0];
        if (!valid(entry,r)) return Status::invalid_source_fact;
        auto* owner=entry->owner;
        if (!owner->forced && (s->global_blocked || owner->locked)) continue;
        std::uint32_t word;
        auto status=call(Operation::is_faerie,entry,owner->identity,word);
        if (status!=Status::complete) return status;
        if (word) continue;
        if (!valid(entry,r)) return Status::invalid_source_fact;
        status=call(Operation::is_follower,entry,entry->owner->identity,word);
        if (status!=Status::complete) return status;
        if (word) return done(Decision::selected);
        if (!valid(entry,r)) return Status::invalid_source_fact;
        owner=entry->owner; // retained through IsZonable and two field reads
        if (!owner->visible) continue;
        status=call(Operation::is_zonable,entry,owner->identity,word);
        if (status!=Status::complete) return status;
        if (!word || !owner->zoned || owner->in_zone) return done(Decision::selected);
        if (!valid(entry,r)) return Status::invalid_source_fact;
        if (!entry->owner->zoned) return done(Decision::selected);
    }
    return done(Decision::exhausted);
}
character_ai_turn::Globals turn_globals(const State& s) noexcept {
    return {s.count && s.entries && s.entries[0] ? s.entries[0]->ai : 0, s.count, s.timer};
}
}
