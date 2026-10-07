#pragma once
#include "../engine-resources/cpu_vector_capacity_v41.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
namespace dh2::ui {
struct SwfVertexCacheStatsV36 {
 std::uint64_t hits{},misses{},uploads{},uploaded_bytes{},evictions{},context_reuploads{};
 std::size_t resident_bytes{},resident_entries{};
};
// Modern renderer cache only. Every key compares the complete packed source
// vertex bytes, even after a hash match. Matrix/color/native/timeline state is
// never cached or omitted. Buffer handles belong to the caller's GL context.
class SwfVertexCacheV36 {
 struct Entry {std::uint64_t hash{};std::uint32_t mode{};std::uintptr_t buffer{};resources::CpuVectorCapacityV41 capacity_v43;std::vector<float> vertices;};
 std::shared_ptr<resources::ContextResourceBudgetV37> budget_v43_;
 resources::ResourceScopeV37 scope_v43_{resources::ResourceScopeV37::other};
 std::list<Entry> entries_;
 std::unordered_multimap<std::uint64_t,std::list<Entry>::iterator> index_;
 std::size_t maximum_bytes_,maximum_entries_;
 SwfVertexCacheStatsV36 stats_;
 void erase(std::list<Entry>::iterator,const std::function<void(std::uintptr_t)>&);
public:
 explicit SwfVertexCacheV36(std::size_t bytes=8u*1024u*1024u,std::size_t entries=1024):maximum_bytes_(bytes),maximum_entries_(entries){}
 bool bind_budget_v43(const std::shared_ptr<resources::ContextResourceBudgetV37>&,resources::ResourceScopeV37,std::string&);
 using Upload=std::function<bool(const float*,std::size_t,std::uintptr_t&,std::string&)>;
 bool acquire(const std::vector<float>&,std::uint32_t,const Upload&,
              const std::function<void(std::uintptr_t)>&,std::uintptr_t&,bool&,std::string&);
 void clear(const std::function<void(std::uintptr_t)>&);
 void abandon_context()noexcept;
 const SwfVertexCacheStatsV36& stats()const noexcept{return stats_;}
 static std::uint64_t fingerprint(const float*,std::size_t,std::uint32_t)noexcept;
};
// Ordered triangle-strip expansion used only for small adjacent-compatible
// batches. The shared-edge/winding sequence exactly matches GL_TRIANGLE_STRIP.
// Packed source input remains four floats per vertex: X,Y,U,V.
bool swf_append_triangles_v36(std::vector<float>&,const std::vector<float>&,
                             bool strip,std::string&);
}
