#pragma once
#include <cstdint>
#include <string>

namespace dh2::ui {
struct FsContinueGameServicesV1 {
    void* context{};
    bool (*resolve_last_slot)(void*,std::int32_t&,std::string&){};
    bool (*queue_resume)(void*,std::int32_t,std::string&){};
    bool (*fallback_start_game)(void*,const char*,std::string&){};
};

// GameSWF strips "FSCommand:" before invoking its one global host callback.
// The slot is always read from the canonical SavegameManager owner; the SWF
// argument is only forwarded to the source StartGame fallback.
inline bool fscommand_continue_game_v1(const char* command,const char* argument,
    const FsContinueGameServicesV1& services,std::string& error) {
    error.clear();
    if(!command||std::string(command)!="ContinueGame")return true;
    if(!services.resolve_last_slot){error="ContinueGame last-slot owner unavailable";return false;}
    std::int32_t slot=-1;
    if(!services.resolve_last_slot(services.context,slot,error))return false;
    if(slot<0){
        if(!services.fallback_start_game){error="ContinueGame StartGame fallback unavailable";return false;}
        return services.fallback_start_game(services.context,argument?argument:"",error);
    }
    if(!services.queue_resume){error="ContinueGame resume queue unavailable";return false;}
    return services.queue_resume(services.context,slot,error);
}
}
