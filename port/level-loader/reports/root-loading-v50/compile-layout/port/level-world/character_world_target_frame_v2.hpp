#pragma once
#include "character_world_attack_geometry_v1.hpp"
#include "character_target_update.hpp"
namespace dh2::character::skills {
struct WorldTargetFrameServicesV2 {
 void* context{};
 // Whole SM_GetState's live pointer selection, including UINT_MAX when its
 // current state pointer is NULL. Never use renderer animation labels.
 int(*machine_state)(void*,std::uintptr_t,std::uint32_t*){};
 // Character.RaiseEvent -> actual CharAI handler -> selected AIS endpoint.
 // Do NOT call Lua OnTargetDied directly, or publish this from a death store.
 int(*raise_event)(void*,std::uintptr_t,std::uint32_t,std::uintptr_t){};
 // Source AI_IsInCloseRange's whole inventory/interaction family. Reached
 // only for actual ranged owners; no generic radius substitute is supplied.
 int(*close_range)(void*,std::uintptr_t,std::uintptr_t,std::uint32_t*){};
};
// Borrows the exact AI+40/+44/+48/+49 state. No target, life, timer or FSM
// copies. Composes the already oracle-verified complete _UpdateTarget body.
class CharacterWorldTargetFrameV2 {
 CharacterWorldRuntimeV1& world_;
 CharacterWorldAttackGeometryV1& geometry_;
 const data::AiTables& ai_;
 WorldTargetFrameServicesV2 services_;
 std::string error_;
 static int query(void*,TargetState48*,const TargetUpdateRequest24*,std::uint32_t*);
 bool position(std::uintptr_t,const float*&);
public:
 CharacterWorldTargetFrameV2(CharacterWorldRuntimeV1& w,CharacterWorldAttackGeometryV1& g,
  const data::AiTables& a,WorldTargetFrameServicesV2 s):world_(w),geometry_(g),ai_(a),services_(s){}
 int update(TargetState48&);
 const std::string& error()const noexcept{return error_;}
};
}
