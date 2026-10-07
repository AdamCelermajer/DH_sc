#pragma once
#include "character_world_ai_relationship_v1.hpp"
namespace dh2::character::skills {
// Whole mutable/const ObjectHandle.GetObject(false/true) (0x33fdc0/33ff8c),
// logical source map storage. Absent signed keys insert null records, preserving
// local cache/frame effects; shared object handle is not modified by GetObject.
extern "C" int dh2_world_handle_object_v1(std::uintptr_t*,target_providers::Handle16*,target_providers::Registry24*,std::uint32_t asserted,const std::int32_t* assert_mode,const WorldAiServicesV1*);
}
