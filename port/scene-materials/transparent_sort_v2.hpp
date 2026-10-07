#pragma once
#include "transparent_queue_v1.hpp"
namespace dh2::scene {
// Whole source core::heapsort<STransparentNodeEntry> 356f08 and heapsink
// 356d80. Native shared leases replace ARM intrusive reference operations.
// This does not register nodes or certify a whole-scene render pass.
bool transparent_sort_v2(std::vector<TransparentEntryV1>&,
 const TransparentQueueServicesV1&,std::string&);
}
