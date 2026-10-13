#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include "lua_script_load_once.hpp"
#include "ais_state_callbacks.hpp"
#include "character_oid_cache_v1.hpp"
extern "C" {
#include "../adam-script-runtime/script_runtime.h"
}

namespace dh2::monster_external_script {

struct Source {
    const void* bytes;
    std::size_t size;
};

// Logical service boundary, not an overlay of Character or AISExternal.
// The table is copied. Its context, owner, targets and any native objects read
// or changed by callbacks remain externally retained until the session closes.
// Callbacks are synchronous, return 0 on success, and may change live facts.
// A failure stops the Lua callback after prior effects; no rollback is added.
// Exceptions are caught before returning to the C VM trampoline.
struct Services {
    void* context;
    std::uintptr_t owner;
    std::int32_t (*get_py_struct)(void*, const char* category, const char* member,
                                  std::int32_t* value);
    std::int32_t (*get_prop)(void*, std::uintptr_t owner, std::int32_t property,
                             float* raw_fixed_value);
    std::int32_t (*get_py_constant)(void*, const char* category, const char* member,
                                    std::int32_t* value);
    std::int32_t (*has_target)(void*, std::uintptr_t owner, std::uint32_t* value);
    std::int32_t (*get_target)(void*, std::uintptr_t owner, std::uintptr_t* target);
    std::int32_t (*get_state)(void*, std::uintptr_t owner, std::int32_t* state);
    std::int32_t (*has_path)(void*, std::uintptr_t owner, std::uint32_t* value);
    std::int32_t (*set_target)(void*, std::uintptr_t owner, std::uintptr_t target);
    std::int32_t (*head_to)(void*, std::uintptr_t owner, std::uintptr_t target);
    std::int32_t (*move_to)(void*, std::uintptr_t owner, std::uintptr_t target);
    // Optional providers for the unchanged monster_OnInit body. Missing used
    // providers raise a Lua error after prior effects. These callbacks must
    // execute the maintained source query/SetLevel kernels over live owners;
    // returning invented values does not satisfy native initialization.
    std::int32_t (*get_py_oid)(void*, const char* category, const char* member,
                              std::int32_t* value) = nullptr;
    std::int32_t (*get_position)(void*, std::uintptr_t owner, float xyz[3]) = nullptr;
    std::int32_t (*get_host_player_level)(void*, std::int32_t* value) = nullptr;
    std::int32_t (*get_host_player_difficulty)(void*, std::int32_t* value) = nullptr;
    // null numeric_argument is the source default/non-number path. count is
    // exact result arity (0 or 2), including sentinel -1/-1 and invalid modes.
    std::int32_t (*get_current_level_range)(void*, const float* numeric_argument,
                                          std::int32_t values[2], std::uint32_t* count) = nullptr;
    std::int32_t (*set_level)(void*, std::uintptr_t owner, float raw_fixed_level) = nullptr;
    // Optional adapters for the original Character::_Stop and Character::_Attack
    // Lua methods. Attack receives the opaque GameObject identity (which can be
    // null) only after the source userdata type check; its provider must enter
    // the native controller/FSM path and must not apply a hit directly. The
    // separate no-argument Attack path needs the source ReturnValues target and
    // fails closed until that provider is modeled.
    std::int32_t (*stop)(void*, std::uintptr_t owner) = nullptr;
    std::int32_t (*attack)(void*, std::uintptr_t owner, std::uintptr_t target) = nullptr;
    // Borrowed owner for the actual current Level's source CharacterTable-ID
    // cache. The Level integration must bind/clear its real identity/generation;
    // VM sessions never create or infer a Level/cache owner themselves.
    character_oid_cache_v1::Owner* current_level_oid_cache = nullptr;
    // The global AIS PlaySound/StopSound functions are forwarded with their
    // source argument order intact. The app-side provider owns catalog lookup
    // and audio delivery; absent providers remain explicit callback failures.
    std::int32_t (*play_sound)(void*, const char* label, std::size_t label_bytes,
                               bool looping, float fade_ms, bool stop_music) = nullptr;
    std::int32_t (*stop_sound)(void*, const char* label, std::size_t label_bytes,
                               float fade_ms) = nullptr;
    // Generic external-AIS methods used by the source follower script. These
    // providers resolve the same canonical Character/CharAI Record as owner.
    std::int32_t (*has_master)(void*, std::uintptr_t owner,
                               std::uint32_t* value) = nullptr;
    std::int32_t (*set_master)(void*, std::uintptr_t owner,
                               std::uintptr_t master) = nullptr;
    std::int32_t (*clear_target)(void*, std::uintptr_t owner) = nullptr;
    std::int32_t (*warp_behind)(void*, std::uintptr_t owner,
                                std::uintptr_t master) = nullptr;
    // Character::_SkillCombatRoll bridge for a skill script loaded into this
    // same AIS VM. The app provider must borrow the existing Character combat
    // and ApplyResult owners; this Session only forwards ReturnValues.
    std::int32_t (*skill_combat_roll)(void*, std::uintptr_t owner,
        const dh2_script_value* arguments, std::uint32_t count,
        dh2_script_value* output, std::uint32_t capacity,
        std::uint32_t* returned, char* error, std::size_t error_capacity) = nullptr;
    // Skill projectile calls are registered in this same AIS VM. Providers
    // must use the active Character/Level projectile owner and retain the
    // calling CharAISkillScript identity as Projectile user_data.
    std::int32_t (*spawn_skill_projectile)(void*, std::uintptr_t owner,
        const dh2_script_value* arguments, std::uint32_t count,
        dh2_script_value* output, std::uint32_t capacity,
        std::uint32_t* returned, char* error, std::size_t error_capacity) = nullptr;
    std::int32_t (*set_projectile_target)(void*, std::uintptr_t owner,
        const dh2_script_value* arguments, std::uint32_t count,
        dh2_script_value* output, std::uint32_t capacity,
        std::uint32_t* returned, char* error, std::size_t error_capacity) = nullptr;
    // Same-Character script methods used by shipped Monster and follower AIS
    // code. App providers must forward to the existing BuffOwner and retained
    // CharAISkillScript vectors; this Session does not create either owner.
    using CharacterScriptCallback = std::int32_t (*)(void*, std::uintptr_t owner,
        const dh2_script_value*, std::uint32_t, dh2_script_value*,
        std::uint32_t, std::uint32_t*, char*, std::size_t);
    CharacterScriptCallback create_buff = nullptr;
    CharacterScriptCallback remove_buff = nullptr;
    CharacterScriptCallback apply_buff = nullptr;
    CharacterScriptCallback set_skill_cooldown = nullptr;
    CharacterScriptCallback set_spell_cooldown = nullptr;
    // Source skill _commons timer functions. These route through the same
    // Character Coordinator timer vector and retain event0x35/user_ref0.
    std::int32_t (*start_timer)(void*, std::uintptr_t character,
        std::uint32_t duration_ms, std::int32_t repeat, std::int32_t event,
        std::uintptr_t user_ref) = nullptr;
    void (*stop_timer)(void*, std::uintptr_t character,
        std::uint32_t timer_id) = nullptr;
    // Character::_Flee (0x3b91a0). The Lua argument is only type-checked; the
    // provider reads the live CharAI target (+0x40) and issues the Point3D
    // v2Controller::Cmd_MoveTo toward owner+(owner-target). A null CharAI target
    // is the source no-op. Appended after the shipped fields so positional
    // aggregates that stop at stop_timer stay valid.
    std::int32_t (*flee)(void*, std::uintptr_t owner) = nullptr;
    // Character::_GetStateTime (0x3b6d6c): CharStateMachine+0x60, the whole
    // milliseconds spent in the current state (reset by _SetState on a change).
    std::int32_t (*get_state_time)(void*, std::uintptr_t owner,
                                   std::uint32_t* milliseconds) = nullptr;
    // --- Appended for the boss / special AIS scripts (positional aggregates that
    // stop at get_state_time stay valid). ---
    // LuaScript::_Include (0x37efe4) -> LuaManager::AddFile (0x37b23c): the
    // session composes the resolved "data/scripts/ai/<name>.luac" key (appending
    // ".luac" unless the name already carries ".lua[c]"), skips a path already in
    // this VM's loaded set, and asks the provider for the bytes. Returns zero
    // with `bytes`/`size` borrowed until the next call, nonzero when the file
    // does not exist (the original ignores a missing or failing Include).
    std::int32_t (*include_script)(void*, const char* resolved_path,
                                   const void** bytes, std::size_t* size) = nullptr;
    // The GameObject / Character natives Character::BindFunction installs in every
    // AIS VM (character_native_bindings rows) that have no dedicated field above.
    // `function` is the character_native_bindings::Function key and `name` the
    // source script name. `receiver` is the owner for a global call and the
    // `_this` identity of the object table for a `target:Method()` call
    // (`method`); `arguments` exclude that receiver. Return zero or a nonzero
    // status with `error` text; DH2_SCRIPT_REQUIRED_SERVICE_FAILURE marks an
    // explicitly missing implementation. Providers must report unimplemented
    // natives as failures, never as successful no-ops.
    std::int32_t (*character_native)(void*, std::uintptr_t owner,
        std::uint32_t function, const char* name, std::uintptr_t receiver,
        bool method, const dh2_script_value* arguments, std::uint32_t count,
        dh2_script_value* output, std::uint32_t capacity, std::uint32_t* returned,
        char* error, std::size_t error_capacity) = nullptr;
    // LuaScript::_PlayMusic (0x37e420): Sounds label + fade milliseconds.
    std::int32_t (*play_music)(void*, const char* label, std::size_t label_bytes,
                               float fade_ms) = nullptr;
    // LuaScript::_CallPyScript (0x37ef3c): start the named level script unless it
    // is already running.
    std::int32_t (*call_py_script)(void*, const char* name,
                                   std::size_t name_bytes) = nullptr;
    // Runs after every top-level Lua entry (dispatch, combat result, animation
    // event, call_all) has returned and the VM is idle again. Natives that
    // re-enter CharAI (DoSkill -> AI_BeginSkill -> OnSkillCheck...) cannot call
    // back into the busy VM, so the provider queues them and runs them here.
    void (*after_callback)(void*, std::uintptr_t owner) = nullptr;
};

// Optional GameObject providers for the identity-table methods (GetName, IsDead,
// LOCK, Kill, ...). Separate from Services so its positional aggregate
// initializers stay valid. Identities are the opaque native GameObject values
// carried in `_this`. Callbacks run synchronously, must not re-enter the VM, and
// read or change live facts without rollback. A missing callback raises a Lua
// error for that method only.
enum class ObjectQuery : std::uint32_t {
    is_player = 0,
    is_dead = 1,
    is_character = 2,
    state = 3,
    hp = 4,
    prop_hp_current = 5,
    prop_hp_max = 6,
    prop_hp_percent = 7,
    has_master = 8,
};

// `owner` is the calling monster (Services::owner); `identity` is the target.
enum class ObjectCommand : std::uint32_t {
    lock = 0,
    unlock = 1,
    kill = 2,
};

struct ObjectServices {
    void* context = nullptr;
    // Writes a NUL-terminated name, at most capacity-1 bytes before the NUL.
    std::int32_t (*object_name)(void*, std::uintptr_t identity, char* out,
                                std::uint32_t capacity) = nullptr;
    // Boolean queries write 0 or 1. Numeric queries write the integer value.
    std::int32_t (*object_query)(void*, std::uintptr_t identity, ObjectQuery query,
                                 std::int32_t* value) = nullptr;
    // Writes the master identity, or 0 when the object has no master.
    std::int32_t (*object_master)(void*, std::uintptr_t identity,
                                  std::uintptr_t* master) = nullptr;
    // Sets the target of `identity` to another GameObject identity.
    std::int32_t (*object_set_target)(void*, std::uintptr_t identity,
                                      std::uintptr_t target) = nullptr;
    std::int32_t (*object_command)(void*, std::uintptr_t owner, std::uintptr_t identity,
                                   ObjectCommand command) = nullptr;
};

enum class Event : std::uint32_t {
    enemy_spotted = 0,
    target_out_of_range = 1,
    init = 2,
    init_post = 3,
    init_final = 4,
    update = 5,
    died = 6,
    // Character event 17 enters AISExternal::OnTargetInMeleeRange; that Lua
    // callback is called with zero arguments.
    target_in_melee_range = 7,
    // AISDefault::OnCombatResults forwards the original attacker and defender
    // Character userdata to the cached source callback on this same AIS VM.
    target_hit = 8,
    target_missed = 9,
    // AISExternal::OnEndOfAnim invokes the named Lua callback without args.
    animation_end = 10,
    // Generic AISExternal source callbacks used by the authored follower.lua
    // path. These dispatch through this same AIS VM and alias map.
    friend_spotted = 11,
    master_out_of_sight = 12,
    master_out_of_range = 13,
    master_in_ranged_range = 14,
    master_in_close_range = 15,
    master_in_melee_range = 16,
    // AISExternal::OnTerminate is reached from CharAI::AI_ScriptCleanUp.
    terminate = 17,
    // AISExternal::OnNeutralSpotted forwards the candidate Character userdata.
    neutral_spotted = 18,
    // CharAI::_UpdateMaster events 18/21 reach AISExternal::OnMasterDied and
    // OnMasterInSight, which call the named Lua functions without arguments.
    master_died = 19,
    master_in_sight = 20,
    // CharAI::_UpdateTarget events 15/16 reach AISExternal::OnTargetInRangedRange
    // (0x3dcdcc, flag 0x8) and OnTargetInCloseRange (0x3dcdb0, flag 0x10).
    // Both call the named Lua function with zero arguments.
    target_in_ranged_range = 21,
    target_in_close_range = 22,
    // CharAI::RaiseAIEvent events 10/12/13 (CharAI virtuals +0x40/+0x48/+0x4c) reach
    // AISExternal::OnTargetDied / OnTargetOutOfSight / OnTargetInSight, which call
    // the named Lua function with zero arguments (always, no VCB flag).
    target_died = 23,
    target_out_of_sight = 24,
    target_in_sight = 25,
};

// Logical retained source AIS+0xb4 projection, not an ARM overlay. The current
// pointer is derived from the real constructor/state-registry producer. A
// nonnull entry retains the owned name fields corresponding to +0x14/+0x2c.
// Old/new entries and name backing remain live across synchronous callbacks.
using StateCallbacks = ais_state_callbacks::Table;
using CurrentState = ais_state_callbacks::State;
using LoadResult = lua_script_load_once::Result;

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    busy = 2,
    not_ready = 3,
    unsupported_callback = 4,
    script_error = 5,
    allocation_failed = 6,
    invalid_source_fact = 7,
};

enum class Stage : std::uint32_t {
    empty = 0,
    created = 1,
    functions_bound = 2,
    common_loaded = 3,
    external_loaded = 4,
    faulted = 5,
    ais_functions_bound = 6,
};

struct Statistics {
    std::uint32_t completed_callbacks;
    std::uint32_t failed_callbacks;
    std::uint32_t projected_object_table_arguments;
    std::uint32_t source_libraries_opened;
    std::uint32_t source_functions_bound;
    std::size_t lua_memory_used;
    bool faulted;
};

// Owns one source-built float32 Lua5.1.4 VM and its source VFTable alias map.
// Load order is unchanged ai/_commons.luac, then ai/monster.luac. The six events
// above are dispatchable through explicit source providers. OnUpdate receives
// ZERO explicit Lua arguments, matching original LuaScript::Call(char const*),
// even though the common Lua declaration names a timestamp argument. Post/final
// dispatch uses the current alias map and retained VM. The source caller owns
// skills-before-post/final ordering; adding dispatch support does not perform
// SetSkillsAndSpells or complete InitScriptProcess. Combat,
// timers/candidate search remain outside this bounded session. Dispatching
// OnInit does not by itself promote an active AIS.
//
// Source bytes are borrowed only for the initialize/stage-load call that reads
// them. Actor identities use Lua
// tables with a light-userdata _this field, never float pointer conversions.
// These neutral tables expose identity transport plus the methods backed by
// ObjectServices; other entity methods remain unsupported.
// One owning thread; initialize/dispatch/reset reject synchronous reentry.
// The caller must not destroy this session or its borrowed native owners from
// a service callback. Output arguments (including error storage) remain alive
// until the synchronous call returns. A retiring callback context cannot be
// the sole owner of that storage; retain it separately if it holds an output.
// The VM has an allocation cap; no instruction cap or
// untrusted-script sandbox is provided by the reused source VM.
class Session {
public:
    Session();
    ~Session();
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;
    Session(Session&&) = delete;
    Session& operator=(Session&&) = delete;

    // Failed initialization destroys only its candidate VM/map and preserves
    // an earlier session. Native effects from load-time providers are retained.
    Status initialize(Source commons, Source monster, const Services& services,
                      std::string& error, std::size_t memory_limit = 2 * 1024 * 1024);
    // Staged source lifecycle for AISExternal. Each call advances the same VM;
    // a failed stage faults it and keeps prior script/provider effects. The
    // caller must discard a faulted staged session rather than replay a stage.
    // `initialize` remains the atomic convenience wrapper used by older callers.
    Status create(const Services& services, std::string& error,
                  std::size_t memory_limit = 2 * 1024 * 1024,
                  std::shared_ptr<void> service_lifetime = {});
    // Port ownership bridge, not a new original game function. After the AIS
    // constructor creates its VM and SetScript publishes the real pending
    // pointer, install the prepared same-owner callbacks before any binding or
    // load. The returned callback lifetime is required; other borrowed native
    // owners still outlive this Session. No VM/map/cache replacement or source
    // stage advance occurs. Later stages and synchronous reentry are rejected.
    // New services/lifetime are committed before the retired lifetime is
    // released, and that release runs while the Session is busy.
    Status install_created_services(const Services&, std::string& error,
                                    std::shared_ptr<void> callback_lifetime);
    Status bind_functions(std::string& error);
    // Separate real registration stages for native pending AIS initialization.
    // The first installs the 35 AIS entries/libraries; the second installs the
    // supported live Character adapters. bind_functions retains both stages
    // as one convenience call for existing users.
    Status bind_ais_functions(std::string& error);
    Status bind_character_functions(std::string& error);
    // Stores the GameObject providers read by the object-method globals bound
    // in bind_character_functions. Callbacks are read at each call, so this may
    // run after create. Busy during a synchronous callback.
    Status bind_object_services(const ObjectServices&, std::string& error);
    Status load_common(Source commons, std::string& error);
    Status load_external(Source external, std::string& error);
    // AISExternal obtains its script path from the selected AITable row.
    // This overload loads those exact bytes and path into the existing VM;
    // it does not construct another AIS, VM, or alias table.
    Status load_external_named(Source external, const char* source_path,
                               std::string& error);
    // AISExternal::OnDied passes one Lua argument: a GameObject userdata table
    // for a nonnull killer, or nil for null. `enemy` carries that identity for
    // Event::died. Event 2's FSM payload remains a separate later dispatch.
    Status dispatch(Event event, std::uintptr_t enemy, std::string& error);
    // AISDefault::OnCombatResults passes two Character userdata arguments in
    // attacker, defender order. This dispatch reuses this session's VM and
    // alias map; the caller must have checked the active AIS VCB membership.
    Status dispatch_combat_result(Event event, std::uintptr_t attacker,
                                  std::uintptr_t defender, std::string& error);
    // AISDefault::OnAnimEvent forwards the event name (after CharAI strips the
    // `ev_` prefix) and current CharAnimator step index to the same Lua VM.
    Status dispatch_animation_event(const char* event_name,
                                    std::uint32_t step_index,
                                    std::string& error);
    // Call a source-registered Lua alias on this same AIS VM and observe every
    // ReturnValues entry from that one call. Used by CharAISkillScript's
    // SetSkill/OnSkillUpdate path after the source skill loader has registered
    // its callbacks; this creates no VM and performs no alias replay.
    int call_all(const char* name, const dh2_script_value* arguments,
                 std::uint32_t count, dh2_script_returns_observer_v1 observer,
                 void* context, std::string& error);
    // Source _ProjectileHit/_Check uses Args at script+0x0c, target UserData
    // at Projectile+0x3cc, then the Projectile Pointer. SetSkill ReturnValues
    // are discarded; the observer receives all delayed callback ReturnValues.
    // Source only maps an explicit first boolean false to callback result 1;
    // callers may derive that disposition from the full observer snapshot.
    int call_projectile_skill(const char* callback_name,
                 const dh2_script_value* set_skill_arguments,
                 std::uint32_t count, std::uintptr_t target_identity,
                 std::uintptr_t projectile_identity,
                 dh2_script_returns_observer_v1 observer, void* context,
                 std::string& error);
    // Reuse the independent complete20B source CallState wrappers. Each call reads the
    // current pointer once, skips only an actual null projection, otherwise
    // reads its selected name and calls the current source alias with no args.
    // A missing name/function/provider is an explicit failure. These do not
    // register states or invent a null-state fact for a live AIS.
    Status call_state_update(const CurrentState*, std::string& error);
    Status call_state_conditions(const CurrentState*, std::string& error);
    // Starts AFTER original LuaManager path resolution. The exact supplied key
    // is cached per this same VM; a hit skips bytes/loading, a miss executes the
    // real source loader and records only success. Failed misses preserve Lua
    // effects/error text and are retryable, without faulting an otherwise ready
    // session. Existing staged common/external loads retain their previous
    // behavior and are not silently assigned an unproved resolved cache key.
    Status load_resolved(const char* resolved_path, Source, LoadResult*, std::string& error);
    Status reset(std::string& error);
    bool ready() const noexcept;
    // Whether the canonical LuaScript pointer owned by this Session is
    // present. This does not imply that its source stages are ready.
    bool has_source_lua_script() const noexcept;
    Stage stage() const noexcept;
    // Opaque identity of the actual owned VM, for ownership/provenance checks.
    // Never dereference it; it is valid only while this VM remains owned.
    std::uintptr_t vm_identity() const noexcept;
    // Exact service-table match lets an actor wrapper prove that this is the
    // pending AIS VM prepared with its stable per-actor callback context.
    bool uses_services(const Services& services) const noexcept;
    Statistics statistics() const noexcept;
    // Borrowed owned-map text, valid until a later alias mutation, successful
    // initialize, or reset.
    // Unknown event/no session returns nullptr; no recursive alias lookup.
    const char* source_alias(Event event) const noexcept;
    bool contains_source_alias(const char* name, bool& present) const noexcept;

private:
    Status call_state(const CurrentState*, bool conditions, std::string& error);
    struct Impl;
    std::unique_ptr<Impl> impl_;
    bool busy_ = false;
};

}  // namespace dh2::monster_external_script
