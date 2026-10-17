#pragma once
#include "ais_external_init_vcb.hpp"

namespace dh2::ais_player_init_vcb {
using State=ais_external_init_vcb::State;
using Services=ais_external_init_vcb::Services;
using Result=ais_external_init_vcb::Result;
using Status=ais_external_init_vcb::Status;

// Adam c3ae797 AISPlayer60B caller adapted to the maintained Default92B.
// Default resets/stores b8; capture its result, query fresh OnKill membership,
// then store captured flags | 0x400 on membership. Lua globals are not VFTable.
// Same borrowed-state/services/lifetime/reentry contract as initialize_default.
// Services captured once; provider failures keep previous effects and stores.
// AISPlayerIPhone inherits this caller. No VM/actor/timer/property ownership,
// callback execution, player selection or native frame activation is added.
Status initialize(State*,const Services*,Result*);
}
