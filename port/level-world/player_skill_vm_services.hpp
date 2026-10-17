#pragma once
#include "character_player_skills_preparation_v3.hpp"
#include "ais_native_bindings.hpp"
#include "character_native_bindings.hpp"
#include "ais_external_init_vcb.hpp"
#include "debug_switches_runtime.hpp"
#include "lua_script_load_once.hpp"
#include "../adam-script-runtime/script_runtime.h"
#include <memory>

namespace dh2::player_skill_vm_services {
struct VmDeleter {void operator()(dh2_script_vm* p)const noexcept {dh2_script_vm_destroy(p);}};
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
    std::uintptr_t character;
};
struct Providers {
    void* context=nullptr;
    // Resolver owns actual source assets, starts after path resolution, and
    // returns immutable retained bytes plus the exact load-once path key.
    int (*resolve)(void*,const std::string&,Resource&,std::string&)=nullptr;
    // Reached native bodies must run real implementations. Nonzero/exception
    // becomes a required-service failure, even when Lua pcall catches it.
    int (*native)(void*,const NativeRequest&,const dh2_script_value*,std::uint32_t,
                  dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t)=nullptr;
    character_faery_selection::Services faery{};
    std::shared_ptr<void> lifetime;
};
struct Configuration {
    std::uintptr_t character=0;
    std::shared_ptr<const player_skill_tables_adapter::Tables> tables;
    ais_external_init_vcb::State* ais=nullptr;
    debug_switches::Globals* debug=nullptr;
    debug_switches::Services debug_services{};
    Providers providers;
    std::string initial_path;
};
struct Statistics {
    std::uint32_t resolutions=0,load_calls=0,cache_hits=0,declarations=0,init_vcb_calls=0;
    std::uint32_t native_calls=0,required_failures=0;
};
using LoadResult=lua_script_load_once::Result;
// Port composition, zero new original-body credit. Owns the transferred SAME
// VM, binding contexts, alias map, path and retained assets. Adopting opens the
// recovered four libraries/35 AIS globals and installs Character function
// entries; it does not implement object methods or execute/replay OnInit.
// Missing Include, objects, combat, buffs, timers and other native bodies fail.
class Session {
public:
    static std::unique_ptr<Session> adopt(Vm,Configuration,std::string& error);
    ~Session();
    Session(const Session&)=delete;Session& operator=(const Session&)=delete;
    Session(Session&&)=delete;Session& operator=(Session&&)=delete;
    character_player_skills_preparation_v3::Services preparation_services();
    // Load returns 0 for a normal source load result (including Lua error / a
    // false result); -1 for port/dependency failure. A caught required failure
    // can mark the source path loaded before this port reports failure.
    int load_resolved(const std::string& requested,LoadResult*,std::string& error);
    // Same indexed/first-return VM protocol and synchronous observer lifetime.
    int call(const char*,const dh2_script_value*,std::uint32_t,std::uint32_t index,
             dh2_script_return_observer_v1,void*,std::string& error);
    dh2_script_vm* vm()const noexcept;
    const std::string& script_path()const noexcept;
    std::size_t loaded_path_count()const noexcept;
    bool contains_path(const char*)const noexcept;
    const Statistics& statistics()const noexcept;
private:
    struct Impl;
    explicit Session(std::unique_ptr<Impl>);
    std::unique_ptr<Impl> impl_;
};
// One owning thread. Borrowed actor/AIS/Debug/Property/native owners and output
// storage remain live through calls and VM close. Close this Session BEFORE
// preparation Owner/native projections; finalizers can still query providers.
// Services borrow Session; never retain them past it. The lifetime token keeps
// provider contexts alive through VM close, not arbitrary borrowed projections.
// Callback destruction/rebinding/reentry on this Session/VM is forbidden;
// rejected busy entries leave outputs untouched. Independent sessions may nest.
// Prior Lua/cache/path/map/provider effects survive errors, without rollback or
// replay. Public borrowed vm() is for composition; operations/close cannot run
// during a callback, and external loads do not enter this adapter's path cache.
} // namespace dh2::player_skill_vm_services
