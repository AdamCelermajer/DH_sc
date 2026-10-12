#pragma once
namespace dh2::world {
// Original GameObject::MeetCondition38ab60 is MOV r0,#1; BX LR.
// Container.InitPost39f9b0 and InitFinal39fce8 call this QUALIFIED body.
// Authored condition evaluation is the separate TestEnableCondition service.
constexpr bool qualified_gameobject_meet_condition_v47() noexcept {return true;}
}
