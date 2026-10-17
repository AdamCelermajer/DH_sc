#include "character_dot_tick.hpp"
#include <cstddef>
#include <cstring>

namespace dh2::character_dot_tick { namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* p, std::size_t bytes, std::size_t alignment, Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-bytes)return false;
    out={at,at+bytes};return true;
}
bool overlap(Range a, Range b) { return a.begin<b.end && b.begin<a.end; }
std::int32_t signed_word(std::uint32_t value) {
    std::int32_t result;std::memcpy(&result,&value,sizeof(result));return result;
}
template<class Callback> Status invoke(Result* out, Operation operation, Callback callback) {
    ++out->calls;out->last_operation=std::uint32_t(operation);
    try { if(callback())return Status::service_failed; }
    catch(...) { return Status::service_failed; }
    return Status::complete;
}
}
Status tick(const State* state, const Services* services, Result* out) {
    Range controls[4];
    if(!range(state,sizeof(*state),alignof(State),controls[0]) ||
       !range(services,sizeof(*services),alignof(Services),controls[1]) ||
       !range(out,sizeof(*out),alignof(Result),controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    const auto captured=*state;const auto bound=*services;
    if(!captured.properties || !captured.resolved_sheet ||
       !range(captured.owner,sizeof(*captured.owner),alignof(Owner),controls[3]))return Status::invalid_argument;
    for(unsigned j=0;j<3;++j)if(overlap(controls[3],controls[j]))return Status::invalid_argument;
    *out={};
    // Only a successful genuine DotAttack provider can make this projection
    // observable. Default native values are not a reconstructed source ctor.
    dh2::data::CombatResult attack_result;
    for(std::uint32_t property=126;property<132;++property) {
        out->last_property=property;
        if(!bound.read_property)return Status::service_unavailable;
        std::uint32_t amount=0;
        const ReadRequest read{captured.properties,captured.resolved_sheet,property};
        auto status=invoke(out,Operation::read_property,[&]{return bound.read_property(bound.context,&read,&amount);});
        if(status!=Status::complete)return status;
        ++out->property_reads;out->captured_amount=amount;
        if(signed_word(amount)>0) {
            ++out->positive_properties;
            auto character=captured.owner->character;
            if(!character)return Status::invalid_argument;
            if(!bound.is_dead)return Status::service_unavailable;
            std::uint32_t dead=0;
            status=invoke(out,Operation::is_dead,[&]{return bound.is_dead(bound.context,character,&dead);});
            if(status!=Status::complete)return status;
            if(dead)++out->dead_skips;
            else {
                character=captured.owner->character;
                if(!character)return Status::invalid_argument;
                if(!bound.dot_attack)return Status::service_unavailable;
                const AttackRequest attack{&attack_result,character,character,amount,std::int32_t(property)-127};
                status=invoke(out,Operation::dot_attack,[&]{return bound.dot_attack(bound.context,&attack);});
                if(status!=Status::complete)return status;
                ++out->attacks;
                character=captured.owner->character;
                if(!character)return Status::invalid_argument;
                if(!bound.apply_result)return Status::service_unavailable;
                const ApplyRequest apply{&attack_result,character,character,dead};
                status=invoke(out,Operation::apply_result,[&]{return bound.apply_result(bound.context,&apply);});
                if(status!=Status::complete)return status;
                ++out->applications;
            }
        }
        ++out->completed_properties;
    }
    return Status::complete;
}
} // namespace dh2::character_dot_tick
