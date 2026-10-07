#pragma once
#include "../level-world/character_target_events.hpp"
// Versioned full coordinator over SAME retained target/owner/request types.
// Original event0xd OnTargetInSight3d22e4 Play3D3d23b8:
// r3=false, stack integer1, floats-1/-1. V38 corrects both source fields.
// Table/count capture precedes AI-ID query; manager capture precedes position;
// callback changes/reentry and captured active AIS endpoint retain source order.
// Other event0xa..0x11 handlers are copied from the retained coordinator.
// 0complete,1malformed atomic,2provider/lifetime failure after reached prefix.
extern "C" int dh2_audio_target_event_v38(dh2::character::TargetEventState32*,
 std::uint32_t,const dh2::character::TargetEventServices40*);
