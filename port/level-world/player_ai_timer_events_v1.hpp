#pragma once
#include "character_ai_events.hpp"
#include "character_ai_initialization.hpp"
#include "character_coordinator.hpp"
#include "character_regen_tick_v1.hpp"
#include "character_dot_tick.hpp"
#include "character_dot_attack.hpp"
#include "object_update_culling.hpp"
#include "../game-data/properties.hpp"
#include <map>
#include <memory>
#include <string>

namespace dh2::player_ai_timer_events_v1 {
struct ApplyProvider {
    void* context=nullptr;
    // Actual F_ApplyResult for this self-attacker/self-defender Player. The
    // full reached player path is mandatory; a partial monster application
    // or successful no-op is not a substitute. Only reached for positive DoT.
    int (*invoke)(void*,const character_dot_tick::ApplyRequest*,std::string&)=nullptr;
};
struct Bindings {
    character_ai_initialization::State* ai=nullptr;
    character::Coordinator* coordinator=nullptr;
    const object_update_culling::Object* object=nullptr;
    std::uintptr_t controller=0,properties=0;
    data::PropertyView* view=nullptr;
    const std::uint32_t* dead=nullptr;
    debug_switches::Globals* debug_globals=nullptr;
    const debug_switches::Services* debug_services=nullptr;
    character_dot_attack::Runtime* dot_attack=nullptr;
    const character_dot_attack::Storage* dot_storage=nullptr;
    ApplyProvider apply{};
    // Some canonical factory Records expose Character::IsDead's source byte
    // directly. Existing uint32-backed owners continue to use `dead`.
    const std::uint8_t* dead_byte=nullptr;
};
enum class Status {complete,invalid_argument,busy,failed};
struct Result {
    character::AIEventResult16 dispatch{};
    character_regen_tick_v1::Result regen{};
    character_regeneration::Result hp{},mp{};
    character_dot_tick::Result dots{};
    character_dot_attack::Result attack{};
    std::uint32_t event=0,remote_word=0,in_combat=0,regen_skipped=0;
};
// Borrowed source composition for timer events33/34. Uses actual RaiseAIEvent,
// _UpdateRegen/GetInCombat/Regentick and HandleDots, no frame/AIS/Lua update.
// Owns only temporary source string backing; sole existing Debug, properties,
// CharAI, Coordinator, VM, buff groups and combat providers retain ownership.
class Runtime {
public:
    explicit Runtime(Bindings);
    Status deliver(std::int32_t event,const character::Timer32*,Result*,std::string& error);
    std::size_t retained_strings()const{return strings_.size();}
private:
    struct Call;
    Bindings bindings_;
    std::map<std::uintptr_t,std::unique_ptr<std::string>> strings_;
    bool busy_=false;
};
// One owning thread. Borrowed backing/identities remain live through dispatch;
// owner identity and PropertyView sheet addresses cannot rebind in providers.
// Values, Debug singleton selection and later cached properties may change.
// Missing/throwing/nonzero reached providers preserve completed effects and
// fail; surviving strings remain retained. No pause, extra timer traversal,
// extra state-machine event, property recalculation or rollback is introduced.
// Coordinator's typed route must return delivered after complete, failed after
// failure, and machine for events outside this source handler's domain.
}
