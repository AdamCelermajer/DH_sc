#include "character_monster_retarget.hpp"
#include <cstddef>
#include <cstring>
#include <limits>

namespace dh2::character_monster_retarget {
namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* p, std::size_t size, std::size_t alignment, Range& out) {
    const auto at = reinterpret_cast<std::uintptr_t>(p);
    if (!p || at % alignment || at > UINTPTR_MAX-size) return false;
    out={at,at+size}; return true;
}
bool overlap(Range a,Range b) { return a.begin<b.end && b.begin<a.end; }
float number(std::uint32_t word) { float value; static_assert(sizeof(value)==4 && std::numeric_limits<float>::is_iec559); std::memcpy(&value,&word,4); return value; }
std::uint32_t bits(float value) { std::uint32_t word; std::memcpy(&word,&value,4); return word; }
}
Status update(State* state,std::uintptr_t entry_owner,const Services* services,Result* result) {
    Range ranges[3];
    if (!range(state,sizeof(*state),alignof(State),ranges[0]) ||
        !range(services,sizeof(*services),alignof(Services),ranges[1]) ||
        !range(result,sizeof(*result),alignof(Result),ranges[2]) || !entry_owner)
        return Status::invalid_argument;
    for (unsigned i=0;i<3;++i) for (unsigned j=0;j<i;++j)
        if (overlap(ranges[i],ranges[j])) return Status::invalid_argument;
    const auto ai=state->ai;
    if (!ai) return Status::invalid_argument;
    const Services bound=*services; *result={};
    auto invoke=[&](Operation op,Subject kind,std::uintptr_t subject,std::uintptr_t peer,Response& response) {
        if (kind!=Subject::global && !subject) return Status::invalid_source_fact;
        if (!bound.invoke) return Status::service_unavailable;
        ++result->service_calls;
        const Request request{op,kind,subject,peer,0}; response={};
        try { return bound.invoke(bound.context,state,&request,&response) ? Status::service_failed : Status::complete; }
        catch (...) { return Status::service_failed; }
    };
    auto done=[&](Decision value) { result->decision=value; return Status::complete; };
    Response response{};
    auto status=invoke(Operation::highest_aggro,Subject::owner_ai,entry_owner,0,response);
    if (status!=Status::complete) return status;
    const auto highest=response.identity; result->highest=highest;
    status=invoke(Operation::resolve_target_408,Subject::owner_ai,state->owner,0,response);
    if (status!=Status::complete) return status;
    const auto current=response.identity; result->current=current;
    if (highest && current && highest!=current) {
        status=invoke(Operation::get_aggro,Subject::owner_ai,state->owner,current,response);
        if (status!=Status::complete) return status;
        const auto current_threat=response.word; result->current_aggro_word=current_threat;
        status=invoke(Operation::get_aggro,Subject::owner_ai,state->owner,highest,response);
        if (status!=Status::complete) return status;
        const auto highest_threat=response.word; result->highest_aggro_word=highest_threat;
        status=invoke(Operation::design_factor_8,Subject::global,0,0,response);
        if (status!=Status::complete) return status;
        result->factor_word=response.word;
        volatile float threshold=number(current_threat)*number(response.word);
        result->threshold_word=bits(threshold);
        if (!(number(highest_threat)>threshold)) return done(Decision::keep_current);
        // Source debug GetSwitch's value is ignored, but its invocation and
        // any ownership effects precede the separately fresh SetTarget owner.
        status=invoke(Operation::diagnostic_switch,Subject::global,0,0,response);
        if (status!=Status::complete) return status;
        status=invoke(Operation::set_target,Subject::owner_ai,state->owner,highest,response);
        if (status!=Status::complete) return status;
        return done(Decision::switched_to_highest);
    }
    status=invoke(Operation::is_enemy,Subject::owner_ai,state->owner,current,response);
    if (status!=Status::complete) return status;
    if (response.word) {
        result->decision=Decision::enemy_retention_search_boundary;
        return Status::unsupported_branch;
    }
    // The clear path uses the ORIGINAL captured AI, not each current owner's
    // embedded AI. Each action still runs after preceding ownership mutations.
    status=invoke(Operation::clear_aggro,Subject::original_ai,ai,current,response);
    if (status!=Status::complete) return status;
    status=invoke(Operation::set_target,Subject::original_ai,ai,0,response);
    if (status!=Status::complete) return status;
    status=invoke(Operation::sync_last_target,Subject::original_ai,ai,0,response);
    if (status!=Status::complete) return status;
    return done(Decision::cleared_non_enemy);
}
}  // namespace dh2::character_monster_retarget
