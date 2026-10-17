#pragma once
#include "character_player_skills_preparation_v3.hpp"
#include "ais_native_bindings.hpp"
#include "character_native_bindings.hpp"
#include "ais_player_init_vcb.hpp"
#include "debug_switches_runtime.hpp"
#include "lua_script_load_once.hpp"
#include "ais_combat_result_dispatch_v1.hpp"
#include "../adam-script-runtime/script_runtime.h"
#include <memory>

namespace dh2::player_skill_session_v1 {
struct VmDeleter {void operator()(dh2_script_vm*)const noexcept;};
using Vm=std::unique_ptr<dh2_script_vm,VmDeleter>;
struct Resource {
    std::string resolved_path;
    std::shared_ptr<const std::vector<std::uint8_t>> bytes;
};
enum class Domain {ais,character};
struct NativeRequest {
    Domain domain;
    const char* name;
    ais_native_bindings::Function ais_function{};
    character_native_bindings::Function character_function{};
    std::uintptr_t character=0,userdata=0;
};
struct Providers {
    void* context=nullptr;
    // Exact resolved source path and immutable retained bytes; zero success.
    int (*resolve)(void*,const std::string&,Resource&,std::string&)=nullptr;
    // Current VM callback ABI: zero success, positive ordinary Lua error,
    // -1001 required failure. Other negative statuses/throws are required
    // failures. Missing reached providers are never successful no-ops.
    int (*native)(void*,const NativeRequest&,const dh2_script_value*,std::uint32_t,
                  dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t)=nullptr;
    character_faery_selection::Services faery{};
    std::shared_ptr<void> lifetime;
};
struct Configuration {
    std::uintptr_t character=0;
    ais_player_init_vcb::State* ais=nullptr;
    std::shared_ptr<const player_skill_tables_adapter::Tables> tables;
    debug_switches::Globals* debug=nullptr;
    debug_switches::Services debug_services{};
    Providers providers;
    std::string initial_path="data/scripts/ai/";
};
enum class Stage {created,ais_bound,character_bound,faulted};
struct Statistics {
    std::uint32_t ais_bindings=0,character_function_bindings=0;
    std::uint32_t resolutions=0,load_calls=0,cache_hits=0,declarations=0;
    std::uint32_t init_vcb_calls=0,native_calls=0,required_failures=0;
    std::uint32_t combat_callback_calls=0,combat_callback_failures=0;
};
using LoadResult=lua_script_load_once::Result;

// Port glue adapted from Adam c3ae797 ScriptOwnerV2/ScriptSessionV3 and our
// paused draft. ONE transferred VM; no inventory/property/timer/frame owner.
// Original Player lifecycle/select/publication remain caller-owned. Bind the
// AIS globals, then Character globals after genuine SetCharacter association.
// Method/object Binder registration is external; these stages install only the
// source function entries. This is not a complete Binder or Player AI body.
class Session {
public:
    static std::unique_ptr<Session> adopt(Vm,Configuration,std::string& error);
    ~Session();
    Session(const Session&)=delete;Session& operator=(const Session&)=delete;
    Session(Session&&)=delete;Session& operator=(Session&&)=delete;
    int bind_ais_functions(std::string& error);
    int bind_character_functions(std::string& error);
    character_player_skills_preparation_v3::Services preparation_services();
    // Zero is a normal source result, including positive Lua load failure.
    // Inspect Result.vm_status/source_success. Negative port/unsupported status
    // stops the adapter; -5 records required failures even caught by Lua.
    // A caught failure can leave the path cached when source loading succeeded.
    int load_resolved(const std::string&,LoadResult*,std::string& error);
    // Exact indexed source return observer on this VM; no replay/discarded
    // extra call. Source alias is captured before dispatch. VM statuses retained.
    int call(const char*,const dh2_script_value*,std::uint32_t,std::uint32_t index,
             dh2_script_return_observer_v1,void*,std::string& error);
    // Same source alias and one Lua call; observes all source ReturnValues.
    int call_all(const char*,const dh2_script_value*,std::uint32_t,
                 dh2_script_returns_observer_v1,void*,std::string& error);
    // Source _ProjectileHit/_Check on this same Player VM. The caller supplies
    // Args at script+0x0c; dispatch uses target UserData at Projectile+0x3cc
    // and then the Projectile Pointer. Every delayed
    // ReturnValues entry is delivered once, in source order, to the observer.
    int call_projectile_skill(const char*,const dh2_script_value*,std::uint32_t,
        std::uintptr_t target_identity,std::uintptr_t projectile_identity,
        dh2_script_returns_observer_v1,void*,std::string& error);
    int initialize_vcb(ais_player_init_vcb::Result*,std::string& error);
    // Source AISDefault::OnCombatResults call through this Session's retained
    // Player AIS VM. The caller applies the attack first and checks the source
    // VCB membership bit; this method verifies the AIS and Character owner,
    // wraps both Character identities as source UserData tables, resolves the
    // current alias and invokes exactly one callback without replaying damage.
    int dispatch_combat_result(std::uintptr_t ais,
        ais_combat_result_dispatch_v1::Callback,
        std::uintptr_t attacker,std::uintptr_t defender,std::string& error);
    dh2_script_vm* vm()const noexcept;
    // Exact presence of this owner's source LuaScript/VM pointer; independent
    // of alias/load readiness and never a substitute for the owner checks.
    bool has_source_lua_script()const noexcept;
    std::uintptr_t character_identity()const noexcept;
    std::uintptr_t ais_identity()const noexcept;
    Stage stage()const noexcept;
    const std::string& script_path()const noexcept;
    const std::string& last_error()const noexcept;
    const Statistics& statistics()const noexcept;
    std::size_t loaded_path_count()const noexcept;
    bool contains_path(const char*)const noexcept;
    bool contains_alias(const char*)const noexcept;
private:
    struct Impl;
    explicit Session(std::unique_ptr<Impl>);
    std::unique_ptr<Impl> impl_;
};
// One owning thread. Borrowed actor/AIS/Debug/property/faery/callback/output
// storage stays live through synchronous calls AND VM close. Close Session
// before preparation Owner/leases/native projections. Provider lifetime token
// retires AFTER VM close; it does not own arbitrary borrowed controls.
// No destruction/rebinding/direct VM operation from callbacks. Same Session
// reentry rejects before output writes; independent sessions may nest. Error
// output cannot be owned solely by a retiring callback context. Prior native,
// Lua/cache/path/flag effects survive failure without rollback or OnInit replay.
} // namespace dh2::player_skill_session_v1
