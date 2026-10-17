#pragma once

#include <cstdint>
#include <string>

namespace dh2::character_kill_death_tail_v1 {

struct Request {
    std::uintptr_t character=0;
    std::uintptr_t killer_object=0;
    std::uint32_t forced=0;
};

struct Services {
    void* context=nullptr;
    // Pure readiness pass. Bind the actual DropLoot, ObjectHandle-to-Character,
    // DistributeXP, Character vtable +0x54 and byte +0x14e4 owners here before
    // entering any mutating source tail callback.
    bool (*preflight)(void*,const Request&,std::string& error)=nullptr;
    bool (*drop_loot)(void*,std::uintptr_t character,
                      std::uintptr_t killer_object,std::string& error)=nullptr;
    // Source CharAI::RaiseAIEvent(4) / active AIS OnKill and kill properties;
    // called after DropLoot and before ObjectHandle-to-Character conversion.
    bool (*killer_credit)(void*,std::uintptr_t character,
                          std::uintptr_t killer_object,std::string& error)=nullptr;
    // Mirrors the source ObjectHandle conversion. xp_credit is the source
    // Character::Kill attribution predicate, not an inferred player test.
    bool (*resolve_xp_killer)(void*,std::uintptr_t character,
                              std::uintptr_t killer_object,
                              std::uintptr_t& killer_character,
                              bool& xp_credit,std::string& error)=nullptr;
    bool (*distribute_xp)(void*,std::uintptr_t killer_character,
                          std::uintptr_t killed_character,
                          std::string& error)=nullptr;
    bool (*virtual_54)(void*,std::uintptr_t character,
                       std::int32_t& source_result,
                       std::string& error)=nullptr;
    bool (*read_character_14e4)(void*,std::uintptr_t character,
                                std::uint8_t& value,std::string& error)=nullptr;
    // Calls the existing Kill objective-tail owner only when virtual+0x54 and
    // Character+0x14e4 both read zero. CtrlCaller raises outer event 2 later.
    bool (*objective_tail)(void*,std::uintptr_t character,
                           std::uintptr_t killer,std::int32_t virtual_result,
                           std::uint8_t character_14e4,
                           std::string& error)=nullptr;
};

struct Result {
    std::uint32_t preflight_completed=0;
    std::uint32_t drop_loot_attempted=0,drop_loot_completed=0;
    std::uint32_t killer_credit_attempted=0,killer_credit_completed=0;
    std::uint32_t killer_conversion_attempted=0,killer_conversion_completed=0;
    std::uint32_t xp_attempted=0,xp_completed=0;
    std::uint32_t virtual_54_attempted=0,virtual_54_completed=0;
    std::uint32_t character_14e4_attempted=0,character_14e4_completed=0;
    std::uint32_t objective_tail_attempted=0,objective_tail_completed=0;
    std::uint32_t returned_after_drop=0,objective_tail_skipped=0,
                  objective_tail_enabled=0;
    std::int32_t virtual_54_result=0;
    std::uintptr_t killer_character=0;
    std::uint32_t xp_credit=0;
    std::uint8_t character_14e4=0;
};

enum class Status : std::uint32_t {
    complete,invalid_argument,busy,missing_owner,consumed,provider_failed
};

// One episode per victim. Any reached mutating callback consumes it even if
// that callback fails: earlier gameplay side effects must never be replayed.
class Runtime {
    Request request_{};
    Services services_{};
    bool busy_=false,consumed_=false;
public:
    Runtime(Request,Services);
    Runtime(const Runtime&)=delete;
    Runtime& operator=(const Runtime&)=delete;
    Status run(Result*,std::string& error);
};

} // namespace dh2::character_kill_death_tail_v1
