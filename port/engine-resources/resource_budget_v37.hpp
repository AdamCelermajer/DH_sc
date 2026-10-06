#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>

namespace dh2::resources {
inline constexpr std::uint64_t mib_v37=1024ull*1024ull;
enum class ResourceKindV37 : std::uint8_t {texture,vertex_buffer,index_buffer,program,framebuffer,renderbuffer,cpu_request,count};
enum class ResourceScopeV37 : std::uint8_t {world,actor,equipment,fx,loot,swf_front,swf_gameplay,shader,asset_archive,other,count};
enum class ReplacementModeV37 : std::uint8_t {same_object_storage,separate_candidate,same_object_coexisting_storage};
struct ResourceChargeV37 {
 ResourceKindV37 kind{ResourceKindV37::cpu_request};
 ResourceScopeV37 scope{ResourceScopeV37::other};
 std::uint64_t gpu_bytes{},cpu_bytes{};
 // Explicit archive/source stream policy, never a texture/FX exemption.
 bool bulk_archive{};
};
struct ResourceBudgetLimitsV37 {
 // Configurable engineering ceilings, not original-game constants or measured
 // driver residency. Census/profiling must validate them before acceptance.
 std::uint64_t gpu_bytes=256*mib_v37,texture_gpu_bytes=128*mib_v37;
 std::uint64_t fx_buffer_gpu_bytes=64*mib_v37,cpu_bytes=512*mib_v37;
 std::uint64_t individual_gpu_bytes=128*mib_v37,individual_cpu_bytes=64*mib_v37;
 std::uint64_t bulk_archive_cpu_bytes=512*mib_v37;
 std::array<std::uint32_t,std::size_t(ResourceKindV37::count)> objects{{4096,8192,8192,256,256,256,4096}};
 std::uint32_t record_slots=8192;
};
struct ResourceTokenV37 {
 std::uint64_t budget_identity{},serial{};
 std::uint32_t slot=UINT32_MAX;
 explicit operator bool()const noexcept{return budget_identity&&serial&&slot!=UINT32_MAX;}
};
struct ResourceUsageV37 {
 std::uint64_t gpu_bytes{},cpu_bytes{},texture_gpu_bytes{},fx_buffer_gpu_bytes{};
 std::array<std::uint64_t,std::size_t(ResourceKindV37::count)> objects{};
 std::array<std::uint64_t,std::size_t(ResourceKindV37::count)> gpu_bytes_by_kind{},cpu_bytes_by_kind{};
};
struct ResourceBudgetSnapshotV37 {
 ResourceUsageV37 live,pending,requested,peak;
 std::array<ResourceUsageV37,std::size_t(ResourceScopeV37::count)> live_by_scope{},pending_by_scope{};
 std::array<std::uint64_t,std::size_t(ResourceKindV37::count)> creates{},replacements{},releases{},context_discards{},rejections_by_kind{};
 std::uint64_t reservations{},commits{},aborts{},rejections{},context_generation{},context_losses{},bookkeeping_bytes{};
 std::uint32_t occupied_records{},pending_records{};
 bool context_ready{};
};
class ContextResourceBudgetV37;
class ResourceReservationV37 {
 friend class ContextResourceBudgetV37;
 ContextResourceBudgetV37* budget_{};ResourceTokenV37 pending_{};
 ResourceTokenV37 replaced_{};
 ResourceReservationV37(ContextResourceBudgetV37*,ResourceTokenV37,ResourceTokenV37);
public:
 ResourceReservationV37()=default;
 ~ResourceReservationV37();
 ResourceReservationV37(const ResourceReservationV37&)=delete;
 ResourceReservationV37& operator=(const ResourceReservationV37&)=delete;
 ResourceReservationV37(ResourceReservationV37&&)noexcept;
 ResourceReservationV37& operator=(ResourceReservationV37&&)noexcept;
 explicit operator bool()const noexcept{return budget_!=nullptr;}
 // Called only after allocation/GL success. Replacement retires old accounting;
 // caller must first delete the old separate GL candidate in its live context.
 // Failure preserves old token/accounting. Abort frees only pending admission.
 bool commit(ResourceTokenV37& destination,std::string& error);
 void abort()noexcept;
};
class ContextResourceBudgetV37 {
 friend class ResourceReservationV37;
 struct Record;
 ResourceBudgetLimitsV37 limits_;std::unique_ptr<Record[]> records_;
 mutable std::mutex mutex_;ResourceBudgetSnapshotV37 snapshot_{};
 std::uint64_t identity_{},serial_{};std::uint32_t cursor_{};
 bool reserve(const ResourceTokenV37*,const ResourceChargeV37&,ReplacementModeV37,ResourceReservationV37&,std::string&);
 bool commit(ResourceTokenV37 pending,ResourceTokenV37 replaced,ResourceTokenV37&,std::string&);
 void abort(ResourceTokenV37,ResourceTokenV37)noexcept;
 Record* find(ResourceTokenV37)noexcept;
 bool reject(ResourceKindV37,const char*,std::string&);
 void update_requested_and_peak()noexcept;
public:
 explicit ContextResourceBudgetV37(ResourceBudgetLimitsV37 limits={});
 ~ContextResourceBudgetV37();
 ContextResourceBudgetV37(const ContextResourceBudgetV37&)=delete;
 ContextResourceBudgetV37& operator=(const ContextResourceBudgetV37&)=delete;
 // One ledger shared by ALL owners on the actual context. It must outlive
 // reservations and stored tokens. Methods are synchronized; GL remains caller
 // owned and is never called while the ledger lock is held.
 bool begin_context(std::string& error);
 void context_lost()noexcept;
 bool reserve_create(const ResourceChargeV37&,ResourceReservationV37&,std::string&);
 bool reserve_replace(ResourceTokenV37,const ResourceChargeV37&,ReplacementModeV37,ResourceReservationV37&,std::string&);
 // Explicit pairing with successful owner cleanup. No automatic token destructor
 // silently decrements live GPU accounting while a GL object still exists.
 bool release(ResourceTokenV37&,std::string&);
 bool charge(ResourceTokenV37,ResourceChargeV37&,bool& has_live_gpu,std::string&)const;
 ResourceBudgetSnapshotV37 snapshot()const;
 const ResourceBudgetLimitsV37& limits()const noexcept{return limits_;}
};
bool checked_resource_bytes_v37(std::uint64_t count,std::uint64_t stride,std::uint64_t&,std::string&);
bool rgba_texture_bytes_v37(std::uint32_t width,std::uint32_t height,bool full_mip_chain,std::uint64_t&,std::string&);
bool swf_target_bytes_v37(std::uint32_t width,std::uint32_t height,std::uint64_t&,std::string&);
// Actual retained capacities of packet/scratch vertices, packet/scratch u16
// indices, and cached source u32 indices. Counts are capacities, not live size.
bool fx_geometry_cache_bytes_v37(std::uint64_t packet_vertices,std::uint64_t scratch_vertices,
 std::uint64_t packet_indices,std::uint64_t scratch_indices,std::uint64_t source_indices,
 std::uint64_t&,std::string&);
}
