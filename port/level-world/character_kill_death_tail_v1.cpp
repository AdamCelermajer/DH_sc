#include "character_kill_death_tail_v1.hpp"

#include <exception>
#include <stdexcept>

namespace dh2::character_kill_death_tail_v1 {
namespace {

bool failed(bool ok,std::string& error,const char* message) {
    if (!ok && error.empty()) error=message;
    return !ok;
}

} // namespace

Runtime::Runtime(Request request,Services services)
    :request_(request),services_(services) {
    if (!request.character || request.forced>1 || !services.preflight ||
        !services.drop_loot || !services.killer_credit ||
        !services.resolve_xp_killer ||
        !services.distribute_xp || !services.virtual_54 ||
        !services.read_character_14e4 || !services.objective_tail)
        throw std::invalid_argument("Invalid Character::Kill death-tail bindings");
}

Status Runtime::run(Result* output,std::string& error) {
    if (busy_) return Status::busy;
    if (consumed_) return Status::consumed;
    if (!output) return Status::invalid_argument;
    *output={};error.clear();

    // Source order is DropLoot, killer/aggro OnKill credit, killer ObjectHandle
    // conversion, optional DistributeXP, virtual+0x54, then Character+0x14e4.
    // Validate every future provider first because XP is irreversible.
    busy_=true;
    struct Reset { bool& value; ~Reset(){value=false;} } reset{busy_};
    try {
        if (failed(services_.preflight(services_.context,request_,error),error,
                   "Character::Kill death-tail provider preflight failed"))
            return Status::missing_owner;
        output->preflight_completed=1;

        // From this point onward a callback may have made an irreversible
        // source effect. Any failure consumes this episode; do not retry XP,
        // loot, the virtual callback, or the field read.
        consumed_=true;
        output->drop_loot_attempted=1;
        if (failed(services_.drop_loot(services_.context,request_.character,
                                      request_.killer_object,error),error,
                   "Character::DropLoot failed"))
            return Status::provider_failed;
        output->drop_loot_completed=1;
        if (request_.forced) {
            output->returned_after_drop=1;
            return Status::complete;
        }

        bool xp_credit=false;
        // Character::Kill enters the aggro/credit and ObjectHandle/XP branch
        // only for a non-null killer. A null killer still reaches the +0x54
        // and objective tail when force is false.
        if (request_.killer_object) {
            output->killer_credit_attempted=1;
            if (failed(services_.killer_credit(services_.context,request_.character,
                                               request_.killer_object,error),error,
                       "Character::Kill killer credit failed"))
                return Status::provider_failed;
            output->killer_credit_completed=1;

            output->killer_conversion_attempted=1;
            if (failed(services_.resolve_xp_killer(
                           services_.context,request_.character,
                           request_.killer_object,output->killer_character,
                           xp_credit,error),error,
                       "Character::Kill killer conversion failed"))
                return Status::provider_failed;
            if (xp_credit && !output->killer_character) {
                error="Character::Kill XP attribution resolved a null killer Character";
                return Status::provider_failed;
            }
            output->killer_conversion_completed=1;
            output->xp_credit=xp_credit?1u:0u;

            if (xp_credit) {
                output->xp_attempted=1;
                if (failed(services_.distribute_xp(
                               services_.context,output->killer_character,
                               request_.character,error),error,
                           "Character::DistributeXP failed"))
                    return Status::provider_failed;
                output->xp_completed=1;
            }
        }

        output->virtual_54_attempted=1;
        if (failed(services_.virtual_54(services_.context,request_.character,
                                       output->virtual_54_result,error),error,
                   "Character::Kill virtual +0x54 failed"))
            return Status::provider_failed;
        output->virtual_54_completed=1;
        // Source branches away from the quest tail when virtual+0x54 is
        // nonzero. The byte at +0x14e4 is read only on the zero-return path.
        if (output->virtual_54_result!=0) {
            output->objective_tail_skipped=1;
            return Status::complete;
        }

        output->character_14e4_attempted=1;
        if (failed(services_.read_character_14e4(
                       services_.context,request_.character,
                       output->character_14e4,error),error,
                   "Character::Kill Character+0x14e4 read failed"))
            return Status::provider_failed;
        output->character_14e4_completed=1;
        if (output->character_14e4!=0) {
            output->objective_tail_skipped=1;
            return Status::complete;
        }
        output->objective_tail_enabled=1;
        output->objective_tail_attempted=1;
        if (failed(services_.objective_tail(services_.context,request_.character,
                                            request_.killer_object,
                                            output->virtual_54_result,
                                            output->character_14e4,error),error,
                   "Character::Kill objective tail failed"))
            return Status::provider_failed;
        output->objective_tail_completed=1;
    } catch (const std::exception& ex) {
        error=ex.what();
        if (error.empty()) error="Character::Kill death-tail provider threw";
        return Status::provider_failed;
    } catch (...) {
        error="Character::Kill death-tail provider threw";
        return Status::provider_failed;
    }
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_kill_death_tail_v1
