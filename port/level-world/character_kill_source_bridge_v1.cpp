#include "character_kill_source_bridge_v1.hpp"

#include <stdexcept>

namespace dh2::character_kill_source_bridge_v1 {

Runtime::Runtime(std::uintptr_t character,
                 player_kill_continuation_v1::CtrlCaller& caller)
    :character_(character),caller_(&caller) {
    if(!character_||caller.character()!=character_)
        throw std::invalid_argument("Character kill bridge and CtrlCaller identities differ");
}

Status Runtime::dispatch(const Request& request,Result* output,
                         std::string& error) {
    if(busy_)return Status::busy;
    if(!output||request.character!=character_||request.forced>1||
       (request.source!=Source::player_melee&&
        request.source!=Source::player_skill&&
        request.source!=Source::npc_attack))
        return Status::invalid_argument;
    busy_=true;
    struct Reset {bool& busy;~Reset(){busy=false;}} reset{busy_};
    Result result{};result.source=request.source;
    const auto status=caller_->kill(request.killer,request.forced,
                                    &result.ctrl,error);
    *output=result;
    return status;
}

} // namespace dh2::character_kill_source_bridge_v1
