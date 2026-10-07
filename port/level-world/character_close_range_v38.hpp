#pragma once
#include <cstdint>
#include <string>
namespace dh2::character {
enum class CloseRangeOperationV38 { resolve_character,object_kind,interaction_type,interaction_range,range_parameters,position,debug_load,debug_query };
struct CloseRangeRequestV38 { CloseRangeOperationV38 operation{};std::uintptr_t subject{},other{};const char* name{}; };
struct CloseRangeResponseV38 {std::uintptr_t character{};std::int32_t word{},limits[3]{};float position[3]{};};
struct CloseRangeServicesV38 {void* context{};int(*invoke)(void*,const CloseRangeRequestV38&,CloseRangeResponseV38&,std::string&){};};
// Whole CharAI::AI_IsInCloseRange3d63d8. current_target is a borrowed snapshot
// of SAME AI+40 at entry; an explicit nonNULL target takes precedence.
int character_close_range_v38(std::int32_t&,std::uintptr_t owner,std::uintptr_t explicit_target,std::uintptr_t current_target,const CloseRangeServicesV38&,std::string&);
int character_close_distance_v38(const float owner[3],const float target[3],std::int32_t minimum);
}
