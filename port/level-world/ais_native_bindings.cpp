#include "ais_native_bindings.hpp"

namespace dh2::ais_native_bindings {
namespace {
constexpr Binding base[] = {
    {"Include",Function::include}, {"Trace",Function::trace},
    {"SetInt",Function::set_int}, {"GetInt",Function::get_int},
    {"AddToVFTable",Function::add_to_vf_table},
    {"PushVFTable",Function::push_vf_table}, {"PopVFTable",Function::pop_vf_table},
    {"ToFixed",Function::to_fixed}, {"FromFixed",Function::from_fixed},
    {"MulFixed",Function::mul_fixed}, {"DivFixed",Function::div_fixed},
    {"Rand",Function::rand}, {"RandF",Function::rand_f},
    {"BitNot",Function::bit_not}, {"BitAnd",Function::bit_and},
    {"BitOr",Function::bit_or}, {"BitXOr",Function::bit_xor},
    {"GetPyCst",Function::get_py_cst}, {"GetPyStruct",Function::get_py_struct},
    {"GetPyOID",Function::get_py_oid}, {"CallPyScript",Function::call_py_script},
    {"GetNumPlayers",Function::get_num_players},
    {"GetHostPlayer",Function::get_host_player},
    {"GetHostPlayerLevel",Function::get_host_player_level},
    {"GetHostPlayerDifficulty",Function::get_host_player_difficulty},
    {"GetCurrentLevelRange",Function::get_current_level_range},
    {"GetGameObjectsByType",Function::get_game_objects_by_type},
    {"SetGameType",Function::set_game_type}, {"GetGameScript",Function::get_game_script},
    {"OnTargetDied",Function::is_player_character},
    {"PlayMusic",Function::play_music}, {"PlaySound",Function::play_sound},
    {"StopSound",Function::stop_sound}
};
constexpr Binding character[] = {
    {"RegisterAIState",Function::register_ai_state},
    {"ChangeAIState",Function::change_ai_state}
};
constexpr Library libraries[] = {Library::base,Library::math,Library::table,Library::string};
bool span(const void* p, std::size_t n, std::size_t alignment) {
    const auto at = reinterpret_cast<std::uintptr_t>(p);
    return p && at % alignment == 0 && at <= UINTPTR_MAX - n;
}
bool overlaps(const void* p, std::size_t n, const void* q, std::size_t m) {
    const auto a = reinterpret_cast<std::uintptr_t>(p), b = reinterpret_cast<std::uintptr_t>(q);
    return a <= b ? b - a < n : a - b < m;
}
Status register_set(const State& s, const Services& c, Result& out,
                    const Binding* bindings, std::size_t count) {
    for (std::size_t i=0;i<count;++i) {
        if (!c.bind_function) return Status::service_unavailable;
        ++out.calls;
        try {
            if (c.bind_function(c.context,s.binder,&bindings[i],s.script))
                return Status::service_failed;
        } catch (...) {return Status::service_failed;}
        ++out.functions_bound;
    }
    return Status::complete;
}
Status run(const State* s, const Services* c, Result* out, bool use_base, bool use_character) {
    if (!span(s,sizeof(*s),alignof(State)) || !span(c,sizeof(*c),alignof(Services)) ||
        !span(out,sizeof(*out),alignof(Result)) || !s->script || !s->binder ||
        (use_base && !s->vm_wrapper) ||
        overlaps(s,sizeof(*s),c,sizeof(*c)) || overlaps(s,sizeof(*s),out,sizeof(*out)) ||
        overlaps(c,sizeof(*c),out,sizeof(*out))) return Status::invalid_argument;
    const State state=*s;
    const Services services=*c;
    *out={};
    if (use_base) {
        for (const auto library:libraries) {
            if (!services.open_library) return Status::service_unavailable;
            ++out->calls;
            try {
                if (services.open_library(services.context,state.vm_wrapper,library))
                    return Status::service_failed;
            } catch (...) {return Status::service_failed;}
            ++out->libraries_opened;
        }
        const auto status=register_set(state,services,*out,base,sizeof(base)/sizeof(base[0]));
        if (status!=Status::complete) return status;
    }
    return use_character ? register_set(state,services,*out,character,2) : Status::complete;
}
}
const Binding* base_bindings(std::size_t* count) {
    if (count) *count=sizeof(base)/sizeof(base[0]);
    return base;
}
const Binding* character_bindings(std::size_t* count) {
    if (count) *count=sizeof(character)/sizeof(character[0]);
    return character;
}
Status bind_base(const State* s,const Services* c,Result* out) {return run(s,c,out,true,false);}
Status bind_character(const State* s,const Services* c,Result* out) {return run(s,c,out,false,true);}
Status bind_all(const State* s,const Services* c,Result* out) {return run(s,c,out,true,true);}
} // namespace dh2::ais_native_bindings
