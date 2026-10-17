#pragma once
#include "../game-data/combat_result.hpp"
#include "debug_switches_runtime.hpp"
#include <map>
#include <memory>
#include <string>

namespace dh2::character_dot_attack {
struct Arguments { std::uintptr_t attacker, defender; std::uint32_t amount; std::int32_t element; };
struct Globals { std::uintptr_t debug_switches; };
enum class Operation : std::uint32_t { debug_load, string_construct, debug_query, string_destroy, calculate_result };
struct Request {
    Operation operation;
    std::uintptr_t subject, string;
    const char* text;
    data::CombatResult* output;
    Arguments arguments;
    std::uint32_t mask;
    std::int32_t weapon_category;
};
struct Reply { std::uintptr_t identity; std::uint32_t word; };
struct Services { void* context; std::int32_t (*invoke)(void*,const Request*,Reply*); };
struct Result { std::uint32_t calls,last_operation,loads,constructed,queries,destroyed,calculated,level_reads; };
enum class Status : std::int32_t { complete,invalid_argument,service_unavailable,service_failed,busy,debug_failed,actor_unavailable,calculation_failed };

// Original F_DotAttack412B caller for retained nonnull actor arguments. It
// captures the Debug singleton, performs load/construct/GetSwitch/destruction
// regardless of amount, then passes mask0x20080000/category-1/element/amount.
// Original invalid-null diagnostic/log/crash paths and stack-corruption traps
// are explicit excluded domains, not invented successful handling.
Status execute(const Arguments*,const Globals*,const Services*,data::CombatResult*,Result*);

// Source CF_SetCombatants+1c..33 projection. Not an ARM struct overlay. Source
// getters read cached Level19; differences wrap32. Identities/deltas/element
// and all four byte stores precede the second CalculateResult Debug phase.
struct CombatContext {
    std::uintptr_t attacker,defender;
    std::uint32_t level_delta,reverse_level_delta;
    std::int32_t element;
    std::uint8_t offhand,magic,blocked,critical;
};
struct Actor { std::uintptr_t identity; const std::int32_t* resolved; };
struct Storage {
    const Actor* actors;std::uint32_t actor_count;
    CombatContext* combat_context;
    data::CombatRandom* random;
    debug_switches::Globals* debug_globals;
    const debug_switches::Services* debug_services;
};

// Reusable real dependency adapter: retained std::string storage, original
// Debug Runtime.load/GetSwitch with caller-owned real IO/persistence services,
// and ONLY CalculateResult's direct0x20080000 continuation. It reuses existing
// combat arithmetic; the full other-mask CalculateResult body is not claimed.
class Runtime {
public:
    Runtime()=default;
    Runtime(const Runtime&)=delete;
    Runtime& operator=(const Runtime&)=delete;
    Status attack(const Storage*,const Arguments*,data::CombatResult*,Result*);
    std::size_t retained_strings()const{return strings_.size();}
private:
    struct Context;
    static std::int32_t operation(void*,const Request*,Reply*);
    std::map<std::uintptr_t,std::unique_ptr<std::string>> strings_;
    bool busy_=false;
};

// One owning thread retains actor/224-word sheet, context, random, Runtime,
// Debug Globals/owners/services, and retired selected backing through return.
// Source argument identities and bindings are captured; the second Debug owner
// is selected freshly after context writes. Debug callbacks may change live
// sheets, singleton selection or combat context (including null combatants).
// They cannot overwrite control/output storage, erase active backing or reenter
// the same Runtime/context/output. Independent owners/outputs may nest. Real
// Debug source recursion stays allowed. All controls are aligned/disjoint.
// Errors preserve preceding reset/context/debug/file/result effects with no
// rollback or added string cleanup. Strings surviving a failed query remain
// retained by Runtime until its destruction. Successful no-op Debug save/IO
// or result providers violate the execute dependency contract.
} // namespace dh2::character_dot_attack
