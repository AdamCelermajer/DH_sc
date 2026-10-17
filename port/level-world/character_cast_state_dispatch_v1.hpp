#pragma once

#include "character_state.hpp"

namespace dh2::character_cast_state_dispatch_v1 {

enum class Status : std::int32_t { complete, invalid_argument };

struct Result {
    std::int32_t next = -1;
    std::uint32_t registered = 0;
};

// Source registration subset for the cast-state edge only. CSIdle/CSMove/
// CSAttack register event C356 (50006) to state7; CSCast registers event34
// to state3 and C358 (50008) to state12. This reports a registered decision
// without mutating or owning the Coordinator state. Other events are normal
// no-ops here; caller dispatches separate state/callback lifecycle kernels.
// IDA ARMv7: CSIdle::OnInit 0x3c7e60, CSMove::OnInit 0x3c80ac,
// CSAttack::OnInit 0x3c8284, CSCast::OnInit 0x3c85b8.
Status event(const character::State*, std::uint32_t event_id, Result*);

} // namespace dh2::character_cast_state_dispatch_v1
