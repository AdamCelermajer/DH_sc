#pragma once
#include <string>
namespace dh2::loader {
struct PropertyScratchV66;struct ProceduralXmlServicesV66;
// Actual CustomAlloc310454 -> libc malloc, fixed256 for the reached source
// formatter. Allocations retain process lifetime as the original helpers do.
// No zero fill, terminator insertion, scratch reuse or level-owned cleanup.
bool allocate_process_property_scratch256_v69(PropertyScratchV66&,std::string&);
// Bind/prewarm during the existing application's service setup. A previously
// supplied genuine Main memory-domain binding is preserved. No runtime/World,
// PropertyMap, descriptor or formatter implementation is replaced.
bool bind_process_property_scratch_v69(ProceduralXmlServicesV66&,std::string&);
}
