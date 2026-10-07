#pragma once
#include <cstdint>
#include <set>
#include <string>
#include <vector>
namespace dh2::resources {
struct NativeUnusedSweepV50 {std::uint64_t inspected{},preserved{},released{};};
// Native backend adaptation: current allocation registries remain authoritative.
// Owning collections and raw bindings both retain resources. Preflight every
// allocation before any release; a stale generation cannot cause partial work.
template<class Registry,class Validate,class Release>
bool sweep_native_unused_v50(const Registry& allocations,
 const std::set<std::uint32_t>& retained,Validate validate,Release release,
 NativeUnusedSweepV50& out,std::string& error){
 NativeUnusedSweepV50 next;std::vector<std::uint32_t> unused;
 for(const auto& allocation:allocations){
  if(!validate(allocation.first,allocation.second,error))return false;
  ++next.inspected;
  if(retained.count(allocation.first))++next.preserved;
  else unused.push_back(allocation.first);
 }
 // No GL calls occur until the complete retained snapshot/preflight succeeds.
 for(auto name:unused){
  if(!release(name,error)){out=next;return false;}
  ++next.released;
 }
 out=next;error.clear();return true;
}
}
