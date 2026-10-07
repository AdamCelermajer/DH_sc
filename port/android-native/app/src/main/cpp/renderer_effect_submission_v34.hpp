#pragma once
#include <cstdint>
#include <unordered_map>
namespace model_renderer {
// Modern renderer work counters, not source simulation/animation clocks.
struct EffectSubmissionCountersV34 {
 std::uint64_t syncs{},sources{},packet_updates{},topology_rebuilds{},warm_hits{},cold_resources{},evictions{};
 std::uint64_t vertex_uploads{},vertex_skips{},vertex_bytes{},index_uploads{},index_skips{},index_bytes{},storage_allocations{};
 std::uint64_t state_snapshots{},draws{};
 std::uint32_t cache_entries{},visible_entries{};
};
}
