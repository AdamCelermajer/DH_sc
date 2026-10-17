#pragma once
#include <cstddef>
#include <cstdint>

namespace dh2::ais_native_bindings {
enum class Library : std::uint8_t {base, math, table, string};
// Native provider keys. These are never original ELF function addresses.
enum class Function : std::uint8_t {
    include, trace, set_int, get_int, add_to_vf_table, push_vf_table, pop_vf_table,
    to_fixed, from_fixed, mul_fixed, div_fixed, rand, rand_f, bit_not, bit_and,
    bit_or, bit_xor, get_py_cst, get_py_struct, get_py_oid, call_py_script,
    get_num_players, get_host_player, get_host_player_level,
    get_host_player_difficulty, get_current_level_range, get_game_objects_by_type,
    set_game_type, get_game_script, is_player_character, play_music, play_sound,
    stop_sound, register_ai_state, change_ai_state
};
struct Binding {const char* name; Function function;};
struct State {
    std::uintptr_t script;
    std::uintptr_t vm_wrapper;
    std::uintptr_t binder;
};
struct Services {
    void* context;
    std::int32_t (*open_library)(void*, std::uintptr_t vm_wrapper, Library);
    std::int32_t (*bind_function)(void*, std::uintptr_t binder,
                                 const Binding*, std::uintptr_t userdata);
};
struct Result {std::uint32_t calls, libraries_opened, functions_bound;};
enum class Status : std::int32_t {
    complete, invalid_argument, service_unavailable, service_failed
};

const Binding* base_bindings(std::size_t* count);
const Binding* character_bindings(std::size_t* count);

// Original LuaScript::BindFunction1252B: base/math/table/string library opens,
// then33 ordered registrations. CharAIScriptBindFunction100B: two ordered AI
// registrations. CharAIScript::BindFunction24B composes both. All caller-owned
// identities are captured at entry; libraries resolve their live VM through the
// retained wrapper. The provider owns the actual functions, Lua libraries and
// Binder implementation. Unsupported functions must fail closed, not no-op.
// Source registers "OnTargetDied" to _IsPlayerCharacter; preserve that observed
// pairing. RegisterAIState/ChangeAIState are the original global names.
// One owning thread; callbacks may alter external VM/global registrations but
// must retain identities/backing and not overwrite services/result or reenter
// on this same output. Errors retain earlier side effects and stop at boundary.
Status bind_base(const State*, const Services*, Result*);
Status bind_character(const State*, const Services*, Result*);
Status bind_all(const State*, const Services*, Result*);
} // namespace dh2::ais_native_bindings
