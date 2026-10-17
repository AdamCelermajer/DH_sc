#pragma once
#include "character_regeneration.hpp"

namespace dh2::character_regen_tick_v1 {
using State=character_regeneration::State;
using Globals=character_regeneration::Globals;
enum class Operation {debug_load,string_construct,debug_query,string_destroy,read_property,regen_hp,regen_mp};
struct Request {
    Operation operation;
    std::uintptr_t subject,sheet;
    std::uint32_t property,amount;
    const char* text;
};
using Reply=character_regeneration::Reply;
struct Services {void* context;int (*invoke)(void*,const Request*,Reply*);};
struct Result {
    std::uint32_t calls=0,last_operation=0,in_combat=0,hp_property=0,mp_property=0,
                  hp_amount=0,mp_amount=0,hp_completed=0,mp_completed=0;
};
enum class Status {complete,invalid_argument,service_unavailable,service_failed};
// Original Character::RegenTick(bool),356B0x3bdd90. Always Debug load, real
// string construct/query/destroy (query word discarded), then cached HP rate
// 39/40 -> RegenHP, fresh cached MP rate44/45 -> RegenMP. Captures actor,
// properties and resolved sheet; no elapsed factor, class/property recalc or
// extra vitals initialization. All reached services required. Failure retains
// source prefix and leaves surviving string backing with its actual provider.
Status tick(const State*,const Globals*,std::uint32_t in_combat,const Services*,Result*);
}
