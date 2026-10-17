#pragma once
#include "character_state.hpp"
#include "character_player_buffs_v1.hpp"
#include "debug_switches_runtime.hpp"
#include "character_player_skills_preparation_v3.hpp"
#include "player_skill_use_session_v1.hpp"

namespace dh2::character_dead_focus_services_v1 {
enum class Status : std::int32_t {complete=1,invalid_argument=-1,provider_failed=-2,busy=-3};
enum class Phase : std::uint32_t {none,debug_load,debug_query,is_player,delete_sneak_buff,
    mark_byte415,read_sneak,list,skill_row,script,active,pre,drop_fx,remove_buffs,complete};
struct Result {
    Phase phase=Phase::none;std::uint32_t calls=0,player=0,active=0,pre_delivered=0;
    std::int32_t sneak=0,selected_slot=-1;
    character_player_buffs_v1::Result buff{};
};
struct Services {
    void* context=nullptr;
    // Zero means actual synchronous delivery; IsPlayer writes its raw word.
    int (*is_player)(void*,std::uintptr_t,std::uint32_t*,std::string&)=nullptr;
    // Required only for a reached nonnull handle. The genuine FX manager
    // receives the canonical handle by reference; failure retains its prefix.
    int (*drop_fx)(void*,std::uintptr_t&,std::string&)=nullptr;
};
struct Bindings {
    std::uintptr_t character=0;
    data::PropertyView* properties=nullptr;
    character_player_buffs_v1::Owner* buffs=nullptr;
    // Canonical Character+415 is embedded CharAI+4d (constructor value1),
    // never a new flag owned by this adapter. FX fields are Character ctor0.
    std::uint8_t* byte415=nullptr;
    std::uintptr_t* self_fx=nullptr;  // +1484
    std::uintptr_t* state_fx=nullptr; // +148c
    std::uintptr_t* highlight=nullptr; // +14a0
    debug_switches::Globals* debug_globals=nullptr;
    const debug_switches::Services* debug_services=nullptr;
    const data::SkillTables* skills=nullptr;
    character_player_skills_preparation_v3::Owner* preparation=nullptr;
    player_skill_use_session_v1::Runtime* skill_calls=nullptr;
    Services services{};
};
// Narrow borrowed receiver for the selected source dead12 focus services.
// No VM, property/buff store, timer, Scene, Character fields or frame ownership.
class Adapter {
    Bindings bindings_;bool busy_=false;
public:
    explicit Adapter(Bindings bindings):bindings_(bindings){}
    static bool handles(character::Service) noexcept;
    Status focus_prelude(Result*,std::string&);
    Status deliver(character::Service,Result*,std::string&);
private:
    bool valid()const noexcept;
    bool output_aliases(const Result*,const std::string&)const noexcept;
    Status cancel_sneaking(Result&,std::string&);
    Status drop(std::uintptr_t*,bool highlight,Result&,std::string&);
};
// Borrowed owners/view/arrays/fields/providers/output survive synchronous calls.
// Same adapter reentry is rejected. Effects remain on failures; no rollback,
// blanket field reset, compensating buff removal or successful provider stub.
// Root invokes the prelude through dead_focus_prelude before dead flags/LookAt.
// Positive Sneak borrows the existing preparation and same VM use runtime;
// callbacks must not reprepare or retire those owners while a call is active.
}
