#include "monster_external_script_session.hpp"
#include "ais_native_bindings.hpp"
#include "../adam-script-runtime/script_game_bindings.h"

#include "../adam-script-runtime/script_function_alias.h"
#include "../lua-numeric/numeric.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <exception>
#include <new>

namespace dh2::monster_external_script {
namespace {
constexpr char projectile_identity_transport[] = R"lua(
function __dh2_monster_projectile_delayed(callback_name, target, skill_script)
    return _G[callback_name]({_this = target}, skill_script)
end
)lua";
int discard_projectile_returns(void*,const dh2_script_first_return_v1*,
                               std::uint32_t,char*,std::size_t){return 0;}
struct BusyScope {
    bool& busy;
    explicit BusyScope(bool& value) : busy(value) { busy = true; }
    ~BusyScope() { busy = false; }
};

const char* callback_name(Event event) {
    switch (event) {
        case Event::enemy_spotted: return "OnEnemySpotted";
        case Event::target_out_of_range: return "OnTargetOutOfRange";
        case Event::init: return "OnInit";
        case Event::init_post: return "OnInitPost";
        case Event::init_final: return "OnInitFinal";
        case Event::update: return "OnUpdate";
        case Event::died: return "OnDied";
        case Event::target_in_melee_range: return "OnTargetInMeleeRange";
        case Event::target_hit: return "OnTargetHit";
        case Event::target_missed: return "OnTargetMissed";
        case Event::animation_end: return "OnEndOfAnim";
        case Event::friend_spotted: return "OnFriendSpotted";
        case Event::neutral_spotted: return "OnNeutralSpotted";
        case Event::master_out_of_sight: return "OnMasterOutOfSight";
        case Event::master_out_of_range: return "OnMasterOutOfRange";
        case Event::master_in_ranged_range: return "OnMasterInRangedRange";
        case Event::master_in_close_range: return "OnMasterInCloseRange";
        case Event::master_in_melee_range: return "OnMasterInMeleeRange";
        case Event::terminate: return "OnTerminate";
    }
    return nullptr;
}

bool valid_source(Source source) {
    return source.bytes != nullptr && source.size != 0 && source.size <= 131072 &&
           *static_cast<const unsigned char*>(source.bytes) != 0x1b;
}

// Authored identity transport. Source-value binding projects _this as type7,
// as it does for original object tables. Object tables share a method table;
// GetID is the original binding's pointer token (gameplay-object-callbacks
// row 1). Other GameObject methods are not yet bound and still raise.
constexpr char identity_transport[] = R"lua(
local __dh2_object_methods = {}
function __dh2_object_methods:GetID() return self._this end
local __dh2_object_mt = {__index = __dh2_object_methods}
local function __dh2_object(identity)
    return setmetatable({_this = identity}, __dh2_object_mt)
end
local native_GetTarget = GetTarget
function GetTarget()
    local identity = native_GetTarget()
    if identity == nil then return nil end
    return __dh2_object(identity)
end
function __dh2_monster_external_object(callback_name, identity)
    if identity == nil then return _G[callback_name](nil) end
    return _G[callback_name](__dh2_object(identity))
end
function __dh2_monster_external_combat(callback_name, attacker, defender)
    return _G[callback_name](__dh2_object(attacker), __dh2_object(defender))
end
)lua";

}  // namespace

struct Session::Impl {
    enum Operation { py_struct, property, from_fixed, py_constant, target_exists,
                     target_get, state_get, path_exists, target_set, face, move,
                     to_fixed, mul_fixed, div_fixed, alias_add, alias_push, alias_pop,
                     bit_not, bit_xor, bit_and, bit_or, py_oid, position,
                     host_level, host_difficulty, level_range, level_set, stop,
                     attack, register_summon, play_sound, stop_sound, master_exists,
                     master_set, clear_target, warp_behind, skill_combat_roll,
                     spawn_skill_projectile, set_projectile_target,
                     create_buff, remove_buff, apply_buff, set_skill_cooldown,
                     set_spell_cooldown, start_timer, stop_timer, self_id,
                     unsupported };
    struct Binding { Impl* session; Operation operation; const char* name; };
    static constexpr std::size_t max_bindings = 64;
    Services services;
    dh2_script_game_bindings timer_bindings{};
    dh2_script_vm* vm = nullptr;
    dh2_script_aliases* aliases = nullptr;
    Binding bindings[max_bindings]{};
    std::size_t binding_count = 0;
    ais_native_bindings::Result registration{};
    std::uintptr_t vm_wrapper_token = 0;
    std::uintptr_t binder_token = 0;
    std::uint32_t completed = 0;
    std::uint32_t failed = 0;
    std::uint32_t object_table_arguments = 0;
    bool faulted = false;
    Stage stage = Stage::empty;
    bool projectile_identity_transport_loaded = false;
    std::shared_ptr<void> service_lifetime;
    lua_script_load_once::State resolved_loads;

    explicit Impl(const Services& value) : services(value) {
        timer_bindings = {this, services.owner, timer_start_bridge,
                          timer_stop_bridge, 0};
    }
    static std::int32_t timer_start_bridge(void* raw,std::uintptr_t owner,
            std::uint32_t duration,std::int32_t repeat,std::int32_t event,
            std::uintptr_t user_ref) {
        if (!raw) return -1;
        auto& self=*static_cast<Impl*>(raw);
        return self.services.start_timer
            ? self.services.start_timer(self.services.context,owner,duration,
                repeat,event,user_ref) : -1;
    }
    static void timer_stop_bridge(void* raw,std::uintptr_t owner,
            std::uint32_t timer_id) {
        if (!raw) return;
        auto& self=*static_cast<Impl*>(raw);
        if (self.services.stop_timer)
            self.services.stop_timer(self.services.context,owner,timer_id);
    }
    ~Impl() {
        // VM closures borrow Binding and aliases; close them before the map.
        dh2_script_vm_destroy(vm);
        dh2_script_alias_destroy(aliases);
    }

    bool create_vm(std::size_t memory_limit) {
        vm = dh2_script_vm_create_deferred(memory_limit);
        aliases = dh2_script_alias_create();
        if (!vm || !aliases) return false;
        if (lua_script_load_once::reset(&resolved_loads, reinterpret_cast<std::uintptr_t>(vm)) !=
            lua_script_load_once::Status::complete) return false;
        stage = Stage::created;
        return true;
    }

    static int reject(char* error, std::size_t capacity, const char* reason) noexcept {
        if (error && capacity) std::snprintf(error, capacity, "%s", reason);
        return 1;
    }
    static void number(dh2_script_value& value, float numeric) noexcept {
        value = {};
        value.type = DH2_SCRIPT_NUMBER;
        value.number = numeric;
    }
    static bool identity(const dh2_script_value& value) noexcept {
        return value.type == DH2_SCRIPT_IDENTITY || value.type == 7;
    }
    static bool unsigned_integer(const dh2_script_value& value,
                                 character_oid_cache_v1::Argument* out) noexcept {
        if (!out || value.type != DH2_SCRIPT_NUMBER || !std::isfinite(value.number) ||
            value.number < 0.0f || std::floor(value.number) != value.number ||
            static_cast<double>(value.number) > 4294967295.0) return false;
        out->kind = character_oid_cache_v1::ValueKind::unsigned_integer;
        out->value = static_cast<std::uint32_t>(value.number);
        return true;
    }

    static int invoke(void* opaque, const dh2_script_value* arguments,
                      std::uint32_t count, dh2_script_value* output,
                      std::uint32_t capacity, std::uint32_t* returned,
                      char* error, std::size_t error_capacity) noexcept {
        if (!opaque || !returned || (count && !arguments))
            return reject(error, error_capacity, "invalid monster service invocation");
        *returned = 0;
        auto& binding = *static_cast<Binding*>(opaque);
        const auto& services = binding.session->services;
        try {
            if (binding.operation == unsupported) {
                char message[192];
                std::snprintf(message, sizeof(message), "unsupported source script function: %s",
                              binding.name ? binding.name : "unknown");
                return reject(error, error_capacity, message);
            }
            if (binding.operation == alias_add || binding.operation == alias_push ||
                binding.operation == alias_pop) {
                int status = 0;
                if (binding.operation == alias_add)
                    status = dh2_script_alias_add_values(binding.session->aliases, arguments, count);
                else if (binding.operation == alias_push)
                    status = dh2_script_alias_push(binding.session->aliases);
                else
                    status = dh2_script_alias_pop(binding.session->aliases);
                return status ? reject(error, error_capacity, "source VFTable update failed") : 0;
            }
            if (binding.operation == master_exists) {
                std::uint32_t value = 0;
                if (!output || capacity < 1 || !services.has_master ||
                    services.has_master(services.context, services.owner, &value) != 0 ||
                    value > 1)
                    return reject(error, error_capacity, "follower HasMaster failed or unsupported");
                output[0] = {};
                output[0].type = DH2_SCRIPT_BOOLEAN;
                output[0].boolean = value;
                *returned = 1;
                return 0;
            }
            if (binding.operation == skill_combat_roll) {
                if (!services.skill_combat_roll)
                    return reject(error, error_capacity,
                        "Character SkillCombatRoll provider missing");
                return services.skill_combat_roll(services.context, services.owner,
                    arguments, count, output, capacity, returned, error,
                    error_capacity);
            }
            if (binding.operation == spawn_skill_projectile ||
                binding.operation == set_projectile_target) {
                const auto callback = binding.operation == spawn_skill_projectile
                    ? services.spawn_skill_projectile : services.set_projectile_target;
                if (!callback)
                    return reject(error, error_capacity,
                        binding.operation == spawn_skill_projectile
                            ? "Character SpawnSkillProjectile provider missing"
                            : "Character SetProjectileTarget provider missing");
                return callback(services.context, services.owner, arguments,
                    count, output, capacity, returned, error, error_capacity);
            }
            if (binding.operation == create_buff || binding.operation == remove_buff ||
                binding.operation == apply_buff || binding.operation == set_skill_cooldown ||
                binding.operation == set_spell_cooldown) {
                const auto callback = binding.operation == create_buff ? services.create_buff :
                    binding.operation == remove_buff ? services.remove_buff :
                    binding.operation == apply_buff ? services.apply_buff :
                    binding.operation == set_skill_cooldown ? services.set_skill_cooldown :
                    services.set_spell_cooldown;
                if (!callback) {
                    const char* name = binding.operation == create_buff ? "CreateBuff" :
                        binding.operation == remove_buff ? "RemoveBuff" :
                        binding.operation == apply_buff ? "ApplyBuff" :
                        binding.operation == set_skill_cooldown ? "SetSkillCooldownTimerId__" :
                        "SetSpellCooldownTimerId__";
                    char message[128];
                    std::snprintf(message, sizeof(message),
                        "Character %s provider missing", name);
                    return reject(error, error_capacity, message);
                }
                return callback(services.context, services.owner, arguments, count,
                    output, capacity, returned, error, error_capacity);
            }
            if (binding.operation == start_timer) {
                if (!services.start_timer)
                    return reject(error,error_capacity,
                                  "Character StartTimer provider missing");
                return dh2_script_game_start_timer(&binding.session->timer_bindings,
                    arguments,count,output,capacity,returned,error,error_capacity);
            }
            if (binding.operation == stop_timer) {
                if (!services.stop_timer)
                    return reject(error,error_capacity,
                                  "Character StopTimer provider missing");
                return dh2_script_game_stop_timer(&binding.session->timer_bindings,
                    arguments,count,output,capacity,returned,error,error_capacity);
            }
            if (binding.operation == master_set || binding.operation == warp_behind) {
                // The original follower passes GameObject userdata for both
                // operations. Preserve the source non-object no-op overload.
                if (count == 0 || !identity(arguments[0])) return 0;
                if (!arguments[0].identity)
                    return reject(error, error_capacity, "follower master identity missing");
                if (arguments[0].type == 7) ++binding.session->object_table_arguments;
                const auto callback = binding.operation == master_set
                    ? services.set_master : services.warp_behind;
                if (!callback || callback(services.context, services.owner,
                        arguments[0].identity) != 0)
                    return reject(error, error_capacity, binding.operation == master_set
                        ? "follower SetMaster failed or unsupported"
                        : "follower WarpBehind failed or unsupported");
                return 0;
            }
            if (binding.operation == clear_target) {
                if (!services.clear_target || services.clear_target(
                        services.context, services.owner) != 0)
                    return reject(error, error_capacity,
                                  "follower ClearTarget failed or unsupported");
                return 0;
            }
            if (binding.operation == target_set || binding.operation == face ||
                binding.operation == move) {
                // Original object overloads ignore non-object values. This
                // includes MoveTo(nil) when the live GetTarget result is null.
                if (count == 0 || !identity(arguments[0])) return 0;
                if (arguments[0].type == 7) ++binding.session->object_table_arguments;
                const auto callback = binding.operation == target_set ? services.set_target :
                                      binding.operation == face ? services.head_to : services.move_to;
                if (!callback)
                    return reject(error, error_capacity, binding.operation == face ?
                        "monster HeadTo provider missing" : binding.operation == move ?
                        "monster MoveTo provider missing" : "monster SetTarget provider missing");
                const auto status=callback(services.context,services.owner,arguments[0].identity);
                if(status!=0){
                    char detail[160]{};
                    std::snprintf(detail,sizeof(detail),
                        "monster %s provider failed (status %d owner %llx target %llx)",
                        binding.operation==face?"HeadTo":binding.operation==move?"MoveTo":"SetTarget",
                        status,static_cast<unsigned long long>(services.owner),
                        static_cast<unsigned long long>(arguments[0].identity));
                    return reject(error,error_capacity,detail);
                }
                return 0;
            }
            if (binding.operation == stop) {
                // Character::_Stop ignores Lua arguments and dispatches the
                // controller command. Keep that call boundary explicit.
                if (!services.stop || services.stop(services.context, services.owner) != 0)
                    return reject(error, error_capacity, "monster Stop failed or unsupported");
                return 0;
            }
            if (binding.operation == play_sound) {
                if (count != 4 || arguments[0].type != DH2_SCRIPT_STRING ||
                    !arguments[0].text || arguments[0].text_bytes > 256 ||
                    arguments[1].type != DH2_SCRIPT_BOOLEAN ||
                    arguments[2].type != DH2_SCRIPT_NUMBER || !std::isfinite(arguments[2].number) ||
                    arguments[3].type != DH2_SCRIPT_BOOLEAN)
                    return reject(error, error_capacity, "unsupported monster PlaySound arguments");
                if (!services.play_sound || services.play_sound(services.context,
                        arguments[0].text, arguments[0].text_bytes,
                        arguments[1].boolean != 0, arguments[2].number,
                        arguments[3].boolean != 0) != 0)
                    return reject(error, error_capacity, "monster PlaySound failed or unsupported");
                return 0;
            }
            if (binding.operation == stop_sound) {
                if (count < 2 || arguments[0].type != DH2_SCRIPT_STRING ||
                    !arguments[0].text || arguments[0].text_bytes > 256 ||
                    arguments[1].type != DH2_SCRIPT_NUMBER ||
                    !std::isfinite(arguments[1].number))
                    return reject(error, error_capacity, "unsupported monster StopSound arguments");
                const float fade_ms = arguments[1].number;
                if (!services.stop_sound || services.stop_sound(services.context,
                        arguments[0].text, arguments[0].text_bytes, fade_ms) != 0)
                    return reject(error, error_capacity, "monster StopSound failed or unsupported");
                return 0;
            }
            if (binding.operation == register_summon) {
                // Lua 5.1 exposes numeric arguments as float32 here. Only
                // losslessly integral UInteger-compatible values are adapted;
                // bad/missing source arguments retain RegisterSummon's no-op.
                character_oid_cache_v1::Argument values[2]{};
                std::uint32_t used = 0;
                if (count == 0 || !unsigned_integer(arguments[0], &values[0])) return 0;
                used = 1;
                if (count > 1 && unsigned_integer(arguments[1], &values[1])) used = 2;
                auto* cache = services.current_level_oid_cache;
                if (!cache || !cache->level_identity())
                    return reject(error, error_capacity,
                        "RegisterSummon requires the current Level Character OID cache provider");
                character_oid_cache_v1::Result result{};
                const character_oid_cache_v1::Arguments input{values, used};
                const auto status = character_oid_cache_v1::register_summon(*cache, input, &result);
                if (status != character_oid_cache_v1::Status::complete)
                    return reject(error, error_capacity,
                        "RegisterSummon current Level Character OID cache update failed");
                return 0;
            }
            if (binding.operation == attack) {
                // Character::_Attack accepts only object userdata when an
                // explicit target argument is present; nonobjects are a no-op.
                // Its no-argument overload reads a hidden ReturnValues target
                // that this facade does not project, so never fake success.
                if (count == 0)
                    return reject(error, error_capacity, "monster Attack without explicit target unsupported");
                if (!identity(arguments[0])) return 0;
                if (!arguments[0].identity)
                    return reject(error, error_capacity, "monster Attack target identity missing");
                if (arguments[0].type == 7) ++binding.session->object_table_arguments;
                if (!services.attack || services.attack(services.context, services.owner,
                                                         arguments[0].identity) != 0)
                    return reject(error, error_capacity, "monster Attack failed or unsupported");
                return 0;
            }
            if (binding.operation == level_set) {
                // Source _SetLevel ignores an absent/non-number front value.
                if (count == 0 || arguments[0].type != DH2_SCRIPT_NUMBER) return 0;
                if (!services.set_level || services.set_level(services.context, services.owner,
                                                              arguments[0].number) != 0)
                    return reject(error, error_capacity, "monster SetLevel failed or unsupported");
                return 0;
            }
            if (binding.operation == position) {
                if (!output || capacity < 3)
                    return reject(error, error_capacity, "monster position output missing");
                float xyz[3]{};
                if (!services.get_position || services.get_position(services.context, services.owner, xyz) != 0)
                    return reject(error, error_capacity, "monster GetPosition failed or unsupported");
                for (unsigned i = 0; i < 3; ++i) number(output[i], xyz[i]);
                *returned = 3;
                return 0;
            }
            if (!output || capacity < 2)
                return reject(error, error_capacity, "monster query output missing");
            if (binding.operation == host_level || binding.operation == host_difficulty) {
                const auto callback = binding.operation == host_level ? services.get_host_player_level :
                                                                       services.get_host_player_difficulty;
                std::int32_t value = 0;
                if (!callback || callback(services.context, &value) != 0)
                    return reject(error, error_capacity, "monster host level/difficulty failed or unsupported");
                number(output[0], static_cast<float>(value));
                *returned = 1;
            } else if (binding.operation == level_range) {
                const float* argument = count && arguments[0].type == DH2_SCRIPT_NUMBER ? &arguments[0].number : nullptr;
                std::int32_t values[2]{}; std::uint32_t result_count = 0;
                if (!services.get_current_level_range || services.get_current_level_range(
                        services.context, argument, values, &result_count) != 0 ||
                        (result_count != 0 && result_count != 2))
                    return reject(error, error_capacity, "monster level range failed or unsupported");
                for (unsigned i = 0; i < result_count; ++i) number(output[i], static_cast<float>(values[i]));
                *returned = result_count;
            } else if (binding.operation == py_struct || binding.operation == py_constant || binding.operation == py_oid) {
                if (count < 2 || arguments[0].type != DH2_SCRIPT_STRING ||
                    arguments[1].type != DH2_SCRIPT_STRING)
                    return reject(error, error_capacity, "monster named query arguments unsupported");
                const auto callback = binding.operation == py_struct ? services.get_py_struct :
                    binding.operation == py_oid ? services.get_py_oid : services.get_py_constant;
                std::int32_t value = 0;
                if (!callback || callback(services.context, arguments[0].text,
                                          arguments[1].text, &value) != 0)
                    return reject(error, error_capacity, "monster named query failed or unsupported");
                number(output[0], static_cast<float>(value));
                *returned = 1;
            } else if (binding.operation == property) {
                if (count == 0 || arguments[0].type != DH2_SCRIPT_NUMBER ||
                    !std::isfinite(arguments[0].number) ||
                    arguments[0].number < -2147483648.0f ||
                    arguments[0].number >= 2147483648.0f)
                    return reject(error, error_capacity, "monster property key unsupported");
                float value = 0;
                if (!services.get_prop || services.get_prop(services.context, services.owner,
                    static_cast<std::int32_t>(arguments[0].number), &value) != 0 || !std::isfinite(value))
                    return reject(error, error_capacity, "monster property query failed or unsupported");
                number(output[0], value);
                *returned = 1;
            } else if (binding.operation == from_fixed) {
                if (count == 0 || arguments[0].type != DH2_SCRIPT_NUMBER)
                    return reject(error, error_capacity, "monster FromFixed argument unsupported");
                dh2_lua_numeric_result result{};
                if (dh2_lua_numeric(DH2_FROM_FIXED, &arguments[0].number, 1, &result) != 0)
                    return reject(error, error_capacity, "monster FromFixed failed");
                number(output[0], static_cast<float>(result.integer));
                number(output[1], result.number);
                *returned = result.count;
            } else if (binding.operation == to_fixed || binding.operation == mul_fixed ||
                       binding.operation == div_fixed || binding.operation == bit_not ||
                       binding.operation == bit_xor || binding.operation == bit_and ||
                       binding.operation == bit_or) {
                const auto needed = (binding.operation == to_fixed || binding.operation == bit_not) ?
                    1U : (binding.operation == bit_and || binding.operation == bit_or) ? count : 2U;
                if (count < needed) return reject(error, error_capacity, "numeric helper arguments missing");
                if (needed > 256) return reject(error, error_capacity, "numeric helper argument limit");
                float operands[256]{};
                for (std::uint32_t index = 0; index < needed; ++index) {
                    if (arguments[index].type != DH2_SCRIPT_NUMBER)
                        return reject(error, error_capacity, "numeric helper argument unsupported");
                    operands[index] = arguments[index].number;
                }
                dh2_lua_numeric_result result{};
                const auto operation = binding.operation == to_fixed ? DH2_TO_FIXED :
                    binding.operation == mul_fixed ? DH2_MUL_FIXED :
                    binding.operation == div_fixed ? DH2_DIV_FIXED :
                    binding.operation == bit_not ? DH2_BIT_NOT :
                    binding.operation == bit_xor ? DH2_BIT_XOR :
                    binding.operation == bit_and ? DH2_BIT_AND : DH2_BIT_OR;
                if (dh2_lua_numeric(operation, operands, needed, &result) != 0)
                    return reject(error, error_capacity, "numeric helper failed");
                // The shared source leaf reports zero results for unsupported
                // bitwise arities (for example BitAnd() or BitAnd(x)). Keep
                // that Lua result arity instead of fabricating integer zero.
                if (result.count != 0) {
                    number(output[0], static_cast<float>(result.integer));
                    *returned = result.count;
                }
            } else if (binding.operation == self_id) {
                // GameObject::GetID returns the object's pointer token; the
                // global form is bound to the scripted Character itself.
                output[0] = {};
                output[0].type = DH2_SCRIPT_IDENTITY;
                output[0].identity = services.owner;
                *returned = 1;
            } else if (binding.operation == target_get) {
                std::uintptr_t target = 0;
                if (!services.get_target || services.get_target(services.context, services.owner, &target) != 0)
                    return reject(error, error_capacity, "monster GetTarget failed or unsupported");
                output[0] = {};
                if (target) {
                    output[0].type = DH2_SCRIPT_IDENTITY;
                    output[0].identity = target;
                }
                *returned = 1;
            } else if (binding.operation == state_get) {
                std::int32_t state = 0;
                if (!services.get_state || services.get_state(services.context, services.owner, &state) != 0)
                    return reject(error, error_capacity, "monster GetState failed or unsupported");
                number(output[0], static_cast<float>(state));
                *returned = 1;
            } else {
                const auto callback = binding.operation == target_exists ? services.has_target : services.has_path;
                std::uint32_t value = 0;
                if (!callback || callback(services.context, services.owner, &value) != 0 || value > 1)
                    return reject(error, error_capacity, "monster boolean query failed or unsupported");
                output[0] = {};
                output[0].type = DH2_SCRIPT_BOOLEAN;
                output[0].boolean = value;
                *returned = 1;
            }
            return 0;
        } catch (const std::exception& exception) {
            return reject(error, error_capacity, exception.what());
        } catch (...) {
            return reject(error, error_capacity, "monster native service exception");
        }
    }

    static Operation operation_for(ais_native_bindings::Function function) {
        using Function = ais_native_bindings::Function;
        switch (function) {
            case Function::add_to_vf_table: return alias_add;
            case Function::push_vf_table: return alias_push;
            case Function::pop_vf_table: return alias_pop;
            case Function::to_fixed: return to_fixed;
            case Function::from_fixed: return from_fixed;
            case Function::mul_fixed: return mul_fixed;
            case Function::div_fixed: return div_fixed;
            case Function::bit_not: return bit_not;
            case Function::bit_xor: return bit_xor;
            case Function::bit_and: return bit_and;
            case Function::bit_or: return bit_or;
            case Function::get_py_cst: return py_constant;
            case Function::get_py_struct: return py_struct;
            case Function::get_py_oid: return py_oid;
            case Function::get_host_player_level: return host_level;
            case Function::get_host_player_difficulty: return host_difficulty;
            case Function::get_current_level_range: return level_range;
            case Function::play_sound: return play_sound;
            case Function::stop_sound: return stop_sound;
            default: return unsupported;
        }
    }

    static std::int32_t open_source_library(void* raw, std::uintptr_t wrapper,
                                            ais_native_bindings::Library library) {
        auto& session = *static_cast<Impl*>(raw);
        if (!session.vm || wrapper != reinterpret_cast<std::uintptr_t>(&session.vm_wrapper_token)) return 1;
        return dh2_script_vm_open_library(session.vm,
            static_cast<dh2_script_library>(library)) == 0 ? 0 : 1;
    }

    static std::int32_t bind_source_function(void* raw, std::uintptr_t binder,
        const ais_native_bindings::Binding* source, std::uintptr_t userdata) {
        auto& session = *static_cast<Impl*>(raw);
        if (!session.vm || binder != reinterpret_cast<std::uintptr_t>(&session.binder_token) ||
            userdata != reinterpret_cast<std::uintptr_t>(&session) || !source || !source->name ||
            session.binding_count >= max_bindings) return 1;
        auto& binding = session.bindings[session.binding_count];
        binding = {&session, operation_for(source->function), source->name};
        if (dh2_script_vm_bind_source_values(session.vm, source->name, invoke, &binding) != 0) return 1;
        ++session.binding_count;
        return 0;
    }

    bool bind_extra(const char* name, Operation operation) {
        if (binding_count >= max_bindings) return false;
        auto& binding = bindings[binding_count];
        binding = {this, operation, name};
        if (dh2_script_vm_bind_source_values(vm, name, invoke, &binding) != 0) return false;
        ++binding_count;
        return true;
    }

    bool bind_ais_functions() {
        const ais_native_bindings::State state{reinterpret_cast<std::uintptr_t>(this),
            reinterpret_cast<std::uintptr_t>(&vm_wrapper_token),
            reinterpret_cast<std::uintptr_t>(&binder_token)};
        const ais_native_bindings::Services services{this, open_source_library, bind_source_function};
        if (ais_native_bindings::bind_all(&state, &services, &registration) !=
                ais_native_bindings::Status::complete || registration.libraries_opened != 4 ||
            registration.functions_bound != 35) return false;
        return true;
    }

    bool bind_character_functions() {
        return bind_extra("GetProp", property) && bind_extra("HasTarget", target_exists) &&
            bind_extra("GetTarget", target_get) && bind_extra("GetState", state_get) &&
            bind_extra("HasPath", path_exists) && bind_extra("SetTarget", target_set) &&
            bind_extra("HeadTo", face) && bind_extra("MoveTo", move) &&
            bind_extra("GetPosition", position) && bind_extra("SetLevel", level_set) &&
            bind_extra("Stop", stop) && bind_extra("Attack", attack) &&
            bind_extra("RegisterSummon", register_summon) &&
            bind_extra("HasMaster", master_exists) &&
            bind_extra("SetMaster", master_set) &&
            bind_extra("ClearTarget", clear_target) &&
            bind_extra("WarpBehind", warp_behind) &&
            bind_extra("SkillCombatRoll__", skill_combat_roll) &&
            bind_extra("SpawnSkillProjectile__", spawn_skill_projectile) &&
            bind_extra("SetProjectileTarget", set_projectile_target) &&
            bind_extra("CreateBuff", create_buff) &&
            bind_extra("RemoveBuff", remove_buff) &&
            bind_extra("ApplyBuff", apply_buff) &&
            bind_extra("SetSkillCooldownTimerId__", set_skill_cooldown) &&
            bind_extra("SetSpellCooldownTimerId__", set_spell_cooldown) &&
            bind_extra("StartTimer", start_timer) &&
            bind_extra("StopTimer", stop_timer) &&
            bind_extra("GetID", self_id);
    }

    bool bind_functions() {
        return bind_ais_functions() && bind_character_functions();
    }

    bool load_common(Source source) {
        if (dh2_script_vm_load(vm, source.bytes, source.size,
                               "data/scripts/ai/_commons.luac") != 0) return false;
        stage = Stage::common_loaded;
        return true;
    }

    static std::int32_t load_same_vm(void* raw, std::uintptr_t identity,
        const void* bytes, std::size_t size, const char* name) {
        auto& session = *static_cast<Impl*>(raw);
        if (!session.vm || identity != reinterpret_cast<std::uintptr_t>(session.vm)) return -1;
        return dh2_script_vm_load(session.vm, bytes, size, name);
    }

    bool load_external(Source source, const char* source_path,
                       bool require_monster_callbacks) {
        if (!source_path || !*source_path || std::strlen(source_path) > 256)
            return false;
        if (dh2_script_vm_load(vm, source.bytes, source.size,
                               source_path) != 0 ||
            dh2_script_vm_load(vm, identity_transport, sizeof(identity_transport) - 1,
                               "monster-neutral-identity-transport") != 0) return false;
        if (require_monster_callbacks &&
            (std::strcmp(dh2_script_alias_resolve(aliases, "OnEnemySpotted"), "monster_OnEnemySpotted") ||
            std::strcmp(dh2_script_alias_resolve(aliases, "OnTargetOutOfRange"), "monster_OnTargetOutOfRange"))
           )
            return false;
        stage = Stage::external_loaded;
        return true;
    }
};

Session::Session() = default;
Session::~Session() {
    // Close the VM and retained callback owner with the same no-reentry rule
    // as explicit reset. The member pointer is cleared before release.
    busy_ = true;
    impl_.reset();
}

Status Session::initialize(Source commons, Source monster, const Services& services,
                           std::string& error, std::size_t memory_limit) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!services.owner || !valid_source(commons) || !valid_source(monster)) {
        error = "invalid monster session input";
        return Status::invalid_argument;
    }
    BusyScope scope(busy_);
    auto candidate = std::unique_ptr<Impl>(new (std::nothrow) Impl(services));
    if (!candidate) { error = "monster session allocation failed"; return Status::allocation_failed; }
    if (!candidate->create_vm(memory_limit)) {
        error = "monster Lua state or alias allocation failed";
        return Status::allocation_failed;
    }
    if (!candidate->bind_functions()) {
        candidate->faulted = true;
        candidate->stage = Stage::faulted;
        error = dh2_script_vm_error(candidate->vm);
        return Status::script_error;
    }
    candidate->stage = Stage::functions_bound;
    if (!candidate->load_common(commons)) {
        candidate->faulted = true;
        candidate->stage = Stage::faulted;
        error = dh2_script_vm_error(candidate->vm);
        return Status::script_error;
    }
    if (!candidate->load_external(monster, "data/scripts/ai/monster.luac", true)) {
        candidate->faulted = true;
        candidate->stage = Stage::faulted;
        error = dh2_script_vm_error(candidate->vm);
        if (error.empty()) error = "original monster callback registration absent";
        return Status::script_error;
    }
    impl_ = std::move(candidate);
    error.clear();
    return Status::complete;
}

Status Session::create(const Services& services, std::string& error,
                       std::size_t memory_limit, std::shared_ptr<void> service_lifetime) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (impl_) { error = "staged session already exists; reset before create"; return Status::busy; }
    if (!services.owner) { error = "invalid monster session service owner"; return Status::invalid_argument; }
    BusyScope scope(busy_);
    auto candidate = std::unique_ptr<Impl>(new (std::nothrow) Impl(services));
    if (!candidate) { error = "monster session allocation failed"; return Status::allocation_failed; }
    if (!candidate->create_vm(memory_limit)) {
        error = "monster Lua state or alias allocation failed";
        return Status::allocation_failed;
    }
    candidate->service_lifetime = std::move(service_lifetime);
    impl_ = std::move(candidate);
    error.clear();
    return Status::complete;
}

Status Session::install_created_services(const Services& services, std::string& error,
                                         std::shared_ptr<void> callback_lifetime) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_ || impl_->stage != Stage::created || impl_->faulted) {
        error = "monster service installation requires a created, unbound VM";
        return Status::not_ready;
    }
    if (!callback_lifetime || !services.owner || services.owner != impl_->services.owner) {
        error = "monster service installation requires the same owner and retained callbacks";
        return Status::invalid_argument;
    }
    // The input table can itself belong to the retired callback context.
    // Capture it before releasing that context; the VM and its deferred
    // libraries/bindings/alias/cache storage remain untouched.
    const Services bound = services;
    BusyScope scope(busy_);
    auto retired = std::move(impl_->service_lifetime);
    impl_->services = bound;
    impl_->service_lifetime = std::move(callback_lifetime);
    retired.reset();
    error.clear();
    return Status::complete;
}

Status Session::bind_functions(std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_) { error = "monster session not created"; return Status::not_ready; }
    if (impl_->stage != Stage::created || impl_->faulted) {
        error = "monster function-binding stage is out of order";
        return Status::not_ready;
    }
    BusyScope scope(busy_);
    if (!impl_->bind_functions()) {
        impl_->faulted = true;
        impl_->stage = Stage::faulted;
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "monster function binding failed";
        return Status::script_error;
    }
    impl_->stage = Stage::functions_bound;
    error.clear();
    return Status::complete;
}

Status Session::bind_ais_functions(std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_ || impl_->stage != Stage::created || impl_->faulted) {
        error = "AIS registration stage is out of order"; return Status::not_ready;
    }
    BusyScope scope(busy_);
    if (!impl_->bind_ais_functions()) {
        impl_->faulted = true; impl_->stage = Stage::faulted;
        error = "AIS registration failed"; return Status::script_error;
    }
    impl_->stage = Stage::ais_functions_bound; error.clear(); return Status::complete;
}

Status Session::bind_character_functions(std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_ || impl_->stage != Stage::ais_functions_bound || impl_->faulted) {
        error = "Character registration stage is out of order"; return Status::not_ready;
    }
    BusyScope scope(busy_);
    if (!impl_->bind_character_functions()) {
        impl_->faulted = true; impl_->stage = Stage::faulted;
        error = "Character registration failed"; return Status::script_error;
    }
    impl_->stage = Stage::functions_bound; error.clear(); return Status::complete;
}

Status Session::load_common(Source commons, std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_) { error = "monster session not created"; return Status::not_ready; }
    if (impl_->stage != Stage::functions_bound || impl_->faulted || !valid_source(commons)) {
        error = "monster common-script stage is out of order or invalid";
        return Status::not_ready;
    }
    BusyScope scope(busy_);
    if (!impl_->load_common(commons)) {
        impl_->faulted = true;
        impl_->stage = Stage::faulted;
        error = dh2_script_vm_error(impl_->vm);
        return Status::script_error;
    }
    error.clear();
    return Status::complete;
}

Status Session::load_external(Source external, std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_) { error = "monster session not created"; return Status::not_ready; }
    if (impl_->stage != Stage::common_loaded || impl_->faulted || !valid_source(external)) {
        error = "monster external-script stage is out of order or invalid";
        return Status::not_ready;
    }
    BusyScope scope(busy_);
    if (!impl_->load_external(external, "data/scripts/ai/monster.luac", true)) {
        impl_->faulted = true;
        impl_->stage = Stage::faulted;
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "original monster callback registration absent";
        return Status::script_error;
    }
    error.clear();
    return Status::complete;
}

Status Session::load_external_named(Source external, const char* source_path,
                                    std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_) { error = "monster session not created"; return Status::not_ready; }
    if (impl_->stage != Stage::common_loaded || impl_->faulted ||
        !valid_source(external) || !source_path || !*source_path ||
        std::strlen(source_path) > 256) {
        error = "named external-script stage, source, or path is invalid";
        return Status::not_ready;
    }
    BusyScope scope(busy_);
    if (!impl_->load_external(external, source_path, false)) {
        impl_->faulted = true;
        impl_->stage = Stage::faulted;
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "named AISExternal source load failed";
        return Status::script_error;
    }
    error.clear();
    return Status::complete;
}

Status Session::dispatch(Event event, std::uintptr_t enemy, std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    const char* requested = callback_name(event);
    if (!requested) { error = "monster callback unsupported"; return Status::unsupported_callback; }
    if (event == Event::target_hit || event == Event::target_missed) {
        error = "combat callback requires attacker and defender identities";
        return Status::invalid_argument;
    }
    if ((event == Event::enemy_spotted || event == Event::friend_spotted ||
         event == Event::neutral_spotted) && enemy == 0) {
        error = "spotted Character identity missing";
        return Status::invalid_argument;
    }
    if (!ready()) { error = "monster session not ready"; return Status::not_ready; }
    BusyScope scope(busy_);
    int status = 0;
    if (event == Event::enemy_spotted || event == Event::friend_spotted ||
        event == Event::neutral_spotted || event == Event::died) {
        // These source AIS callbacks receive one GameObject userdata.
        // OnDied's argument is the killer passed to AISExternal::OnDied.
        const char* resolved = dh2_script_alias_resolve(impl_->aliases, requested);
        dh2_script_value arguments[2]{};
        arguments[0].type = DH2_SCRIPT_STRING;
        arguments[0].text = resolved;
        arguments[0].text_bytes = std::strlen(resolved);
        if (event == Event::died && enemy == 0) {
            // Value::_pushOnStack maps null tag-7 UserData to one Lua nil.
            arguments[1].type = DH2_SCRIPT_NIL;
        } else {
            arguments[1].type = DH2_SCRIPT_IDENTITY;
            arguments[1].identity = enemy;
        }
        status = dh2_script_vm_call_discard_source(impl_->vm, "__dh2_monster_external_object", arguments, 2);
    } else {
        status = dh2_script_alias_call_discard_source(impl_->vm, impl_->aliases, requested, nullptr, 0);
    }
    if (status != 0) {
        ++impl_->failed;
        // LuaScript::Call reports a runtime error and the VM stays usable.
        error = dh2_script_vm_error(impl_->vm);
        return Status::script_error;
    }
    ++impl_->completed;
    error.clear();
    return Status::complete;
}

Status Session::dispatch_combat_result(Event event, std::uintptr_t attacker,
                                       std::uintptr_t defender, std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (event != Event::target_hit && event != Event::target_missed) {
        error = "event is not an AIS combat-result callback";
        return Status::unsupported_callback;
    }
    if (!attacker || !defender) {
        error = "combat attacker and defender identities are required";
        return Status::invalid_argument;
    }
    if (!ready()) { error = "monster session not ready"; return Status::not_ready; }
    const char* requested = callback_name(event);
    const char* resolved = dh2_script_alias_resolve(impl_->aliases, requested);
    if (!resolved) { error = "original combat callback alias is absent"; return Status::unsupported_callback; }
    BusyScope scope(busy_);
    dh2_script_value arguments[3]{};
    arguments[0].type = DH2_SCRIPT_STRING;
    arguments[0].text = resolved;
    arguments[0].text_bytes = std::strlen(resolved);
    arguments[1].type = DH2_SCRIPT_IDENTITY;
    arguments[1].identity = attacker;
    arguments[2].type = DH2_SCRIPT_IDENTITY;
    arguments[2].identity = defender;
    const int status = dh2_script_vm_call_discard_source(
        impl_->vm, "__dh2_monster_external_combat", arguments, 3);
    impl_->object_table_arguments += 2;
    if (status != 0) {
        ++impl_->failed;
        // LuaScript::Call reports a runtime error and the VM stays usable.
        error = dh2_script_vm_error(impl_->vm);
        return Status::script_error;
    }
    ++impl_->completed;
    error.clear();
    return Status::complete;
}

Status Session::dispatch_animation_event(const char* event_name,
                                        std::uint32_t step_index,
                                        std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!event_name || !*event_name || std::strlen(event_name) > 255) {
        error = "invalid AISDefault animation event name";
        return Status::invalid_argument;
    }
    if (!ready()) { error = "monster session not ready"; return Status::not_ready; }
    dh2_script_value arguments[2]{};
    arguments[0].type = DH2_SCRIPT_STRING;
    arguments[0].text = event_name;
    arguments[0].text_bytes = std::strlen(event_name);
    arguments[1].type = DH2_SCRIPT_NUMBER;
    arguments[1].number = static_cast<float>(step_index);
    BusyScope scope(busy_);
    const int status = dh2_script_alias_call_discard_source(
        impl_->vm, impl_->aliases, "OnAnimEvent", arguments, 2);
    if (status != 0) {
        ++impl_->failed;
        // LuaScript::Call reports a runtime error and the VM stays usable.
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "AISDefault OnAnimEvent Lua callback failed";
        return Status::script_error;
    }
    ++impl_->completed;
    error.clear();
    return Status::complete;
}

int Session::call_all(const char* name, const dh2_script_value* arguments,
                      std::uint32_t count,
                      dh2_script_returns_observer_v1 observer, void* context,
                      std::string& error) {
    if (busy_) { error = "monster session busy"; return -1; }
    if (!name || !*name || (count && !arguments) || !observer) {
        error = "invalid source callback call";
        return -1;
    }
    if (!ready()) { error = "monster session not ready"; return -1; }
    const char* alias = dh2_script_alias_resolve(impl_->aliases, name);
    if (!alias) { error = "source callback alias is absent"; return -1; }
    std::string captured;
    try { captured.assign(alias); }
    catch (...) { error = "source callback alias capture failed"; return -1; }
    BusyScope scope(busy_);
    const int status = dh2_script_vm_call_all_source_v1(
        impl_->vm, captured.c_str(), arguments, count, observer, context);
    error = dh2_script_vm_error(impl_->vm);
    if (status == -5 && error.empty())
        error = "required monster native service failure caught by Lua";
    return status;
}

int Session::call_projectile_skill(const char* callback_name,
        const dh2_script_value* set_skill_arguments,std::uint32_t count,
        std::uintptr_t target_identity,std::uintptr_t projectile_identity,
        dh2_script_returns_observer_v1 observer,void* context,
        std::string& error) {
    if(busy_) {error="monster session busy";return -1;}
    if(!ready() || !set_skill_arguments || count!=2 || !target_identity ||
       !projectile_identity || !callback_name || !observer ||
       (std::strcmp(callback_name,"OnDelayedSkill")!=0 &&
        std::strcmp(callback_name,"OnDelayedSkillCheck")!=0) ||
       set_skill_arguments[0].type!=DH2_SCRIPT_STRING ||
       !set_skill_arguments[0].text ||
       set_skill_arguments[1].type!=DH2_SCRIPT_NUMBER) {
        error="invalid same-VM Monster projectile skill callback request";return -1;
    }
    const char* set_skill_alias=dh2_script_alias_resolve(impl_->aliases,"SetSkill");
    if(!set_skill_alias) {
        error="Monster projectile SetSkill alias is absent";return -1;
    }
    BusyScope scope(busy_);
    try {
        const std::string set_skill_name(set_skill_alias);
        const auto discard=dh2_script_vm_call_all_source_v1(
            impl_->vm,set_skill_name.c_str(),set_skill_arguments,count,
            discard_projectile_returns,nullptr);
        error=dh2_script_vm_error(impl_->vm);
        // SetSkill ReturnValues are erased before the delayed call; only its
        // error flag suppresses that later call in the original callback.
        if(discard!=0) {
            if(discard==-5 && error.empty())
                error="required Monster SetSkill service failure caught by Lua";
            return discard;
        }
        // The source calls LuaScript::Call twice, resolving each alias as it
        // reaches that call; preserve SetSkill's alias-table side effects.
        const char* delayed_alias=dh2_script_alias_resolve(impl_->aliases,callback_name);
        if(!delayed_alias) {
            error="Monster delayed projectile callback alias is absent";return -1;
        }
        const std::string delayed_name(delayed_alias);
        if(!impl_->projectile_identity_transport_loaded) {
            const int loaded=dh2_script_vm_load(impl_->vm,projectile_identity_transport,
                sizeof(projectile_identity_transport)-1,
                "monster-projectile-identity-transport");
            if(loaded) {
                error=dh2_script_vm_error(impl_->vm);
                if(error.empty())error="Monster projectile identity transport load failed";
                return loaded;
            }
            impl_->projectile_identity_transport_loaded=true;
        }
        dh2_script_value arguments[3]{};
        arguments[0].type=DH2_SCRIPT_STRING;
        arguments[0].text=delayed_name.c_str();
        arguments[0].text_bytes=delayed_name.size();
        arguments[1].type=DH2_SCRIPT_IDENTITY;arguments[1].identity=target_identity;
        arguments[2].type=DH2_SCRIPT_IDENTITY;arguments[2].identity=projectile_identity;
        const auto delayed=dh2_script_vm_call_all_source_v1(impl_->vm,
            "__dh2_monster_projectile_delayed",arguments,3,
            observer,context);
        error=dh2_script_vm_error(impl_->vm);
        if(delayed==-5 && error.empty())
            error="required Monster delayed projectile service failure caught by Lua";
        return delayed;
    } catch(...) {
        error="Monster projectile callback dependency exception";return -1;
    }
}

Status Session::call_state(const CurrentState* projection, bool conditions, std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!projection || reinterpret_cast<std::uintptr_t>(projection) % alignof(CurrentState) || !projection->ais) {
        error = "invalid retained current-state projection"; return Status::invalid_argument;
    }
    if (!ready()) { error = "monster session not ready"; return Status::not_ready; }
    if (projection->active && reinterpret_cast<std::uintptr_t>(projection->active) % alignof(StateCallbacks)) {
        error = "invalid retained source state entry"; return Status::invalid_argument;
    }
    BusyScope scope(busy_);
    struct Call { Impl* session; Status status; } context{impl_.get(), Status::complete};
    const ais_state_callbacks::Services services{&context, [](void* raw, ais_state_callbacks::State*,
            const ais_state_callbacks::Request* request) -> std::int32_t {
        auto& call = *static_cast<Call*>(raw);
        if (!request->name) { call.status = Status::invalid_source_fact; return 1; }
        if (dh2_script_alias_call_discard_source(call.session->vm, call.session->aliases,
                request->name, nullptr, 0) != 0) { call.status = Status::script_error; return 1; }
        return 0;
    }};
    ais_state_callbacks::Result result{};
    // The reused kernel reads this projection but does not write it. Its native
    // provider signature permits mutations by separately retained owner services;
    // this bridge itself only forwards the captured name to this same VM.
    const auto status = ais_state_callbacks::invoke(const_cast<CurrentState*>(projection),
        conditions ? ais_state_callbacks::Callback::conditions : ais_state_callbacks::Callback::update,
        &services, &result);
    if (context.status == Status::invalid_source_fact) {
        error = "nonnull source state has a null callback name"; return Status::invalid_source_fact;
    }
    if (status != ais_state_callbacks::Status::complete) {
        ++impl_->failed; // runtime error; VM stays usable error = dh2_script_vm_error(impl_->vm);
        return Status::script_error;
    }
    if (result.called) ++impl_->completed;
    error.clear(); return Status::complete;
}
Status Session::call_state_update(const CurrentState* projection, std::string& error) {
    return call_state(projection, false, error);
}
Status Session::call_state_conditions(const CurrentState* projection, std::string& error) {
    return call_state(projection, true, error);
}

Status Session::load_resolved(const char* path, Source bytes, LoadResult* result, std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    if (!impl_ || impl_->faulted || (impl_->stage != Stage::functions_bound &&
        impl_->stage != Stage::common_loaded && impl_->stage != Stage::external_loaded)) {
        error = "resolved load requires a bound live VM"; return Status::not_ready;
    }
    BusyScope scope(busy_);
    const lua_script_load_once::Source source{reinterpret_cast<std::uintptr_t>(impl_->vm), path, bytes.bytes, bytes.size};
    const lua_script_load_once::Services services{impl_.get(), Impl::load_same_vm};
    const auto status = lua_script_load_once::load_once(&impl_->resolved_loads, &source, &services, result);
    using CacheStatus = lua_script_load_once::Status;
    if (status == CacheStatus::complete) { error.clear(); return Status::complete; }
    if (status == CacheStatus::allocation_failed) { error = "resolved load cache allocation failed"; return Status::allocation_failed; }
    if (status == CacheStatus::load_failed) { error = dh2_script_vm_error(impl_->vm); return Status::script_error; }
    error = "resolved load input or native loader failed";
    return status == CacheStatus::service_failed ? Status::script_error : Status::invalid_argument;
}

Status Session::reset(std::string& error) {
    if (busy_) { error = "monster session busy"; return Status::busy; }
    BusyScope scope(busy_);
    impl_.reset();
    error.clear();
    return Status::complete;
}

bool Session::ready() const noexcept { return impl_ && !impl_->faulted && impl_->stage == Stage::external_loaded; }
bool Session::has_source_lua_script() const noexcept { return impl_ && impl_->vm != nullptr; }
Stage Session::stage() const noexcept { return impl_ ? impl_->stage : Stage::empty; }
std::uintptr_t Session::vm_identity() const noexcept {
    return impl_ ? reinterpret_cast<std::uintptr_t>(impl_->vm) : 0;
}
bool Session::uses_services(const Services& services) const noexcept {
    if (!impl_) return false;
    const auto& own = impl_->services;
    return own.context == services.context && own.owner == services.owner &&
        own.get_py_struct == services.get_py_struct && own.get_prop == services.get_prop &&
        own.get_py_constant == services.get_py_constant && own.has_target == services.has_target &&
        own.get_target == services.get_target && own.get_state == services.get_state &&
        own.has_path == services.has_path && own.set_target == services.set_target &&
        own.head_to == services.head_to && own.move_to == services.move_to &&
        own.get_py_oid == services.get_py_oid && own.get_position == services.get_position &&
        own.get_host_player_level == services.get_host_player_level &&
        own.get_host_player_difficulty == services.get_host_player_difficulty &&
        own.get_current_level_range == services.get_current_level_range && own.set_level == services.set_level &&
        own.stop == services.stop && own.attack == services.attack &&
        own.current_level_oid_cache == services.current_level_oid_cache &&
        own.play_sound == services.play_sound && own.stop_sound == services.stop_sound &&
        own.has_master == services.has_master && own.set_master == services.set_master &&
        own.clear_target == services.clear_target && own.warp_behind == services.warp_behind &&
        own.skill_combat_roll == services.skill_combat_roll &&
        own.spawn_skill_projectile == services.spawn_skill_projectile &&
        own.set_projectile_target == services.set_projectile_target &&
        own.create_buff == services.create_buff && own.remove_buff == services.remove_buff &&
        own.apply_buff == services.apply_buff &&
        own.set_skill_cooldown == services.set_skill_cooldown &&
        own.set_spell_cooldown == services.set_spell_cooldown &&
        own.start_timer == services.start_timer && own.stop_timer == services.stop_timer;
}
const char* Session::source_alias(Event event) const noexcept {
    const char* requested = callback_name(event);
    return ready() && requested ? dh2_script_alias_resolve(impl_->aliases, requested) : nullptr;
}
bool Session::contains_source_alias(const char* name, bool& present) const noexcept {
    if (!ready() || !name) return false;
    present = dh2_script_alias_contains(impl_->aliases, name) != 0;
    return true;
}
Statistics Session::statistics() const noexcept {
    return impl_ ? Statistics{impl_->completed, impl_->failed, impl_->object_table_arguments,
                             impl_->registration.libraries_opened,
                             impl_->registration.functions_bound,
                             dh2_script_vm_memory(impl_->vm), impl_->faulted}
                 : Statistics{};
}

}  // namespace dh2::monster_external_script
