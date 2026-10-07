#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::scene {
// Source RenderEntry construction 354e8c and transparent comparator 3538ac.
// Identities and leases refer to retained native nodes/material receivers.
struct TransparentEntryV1 {
 std::uintptr_t node{};std::uint32_t part{};std::uintptr_t material{};
 std::int32_t priority{};float distance{};
 std::shared_ptr<const void> node_owner,material_owner;
};
struct TransparentQueueServicesV1 {
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> node_priority_d8;
 std::function<bool(std::uintptr_t,float&,std::string&)> node_distance_bias_d0;
 std::function<bool(std::uintptr_t,std::array<float,3>&,std::string&)> node_position_38;
 std::function<bool(std::uintptr_t,std::uintptr_t,bool&,std::string&)> material_equal_3537b0;
 std::function<bool(std::uintptr_t,std::uintptr_t,bool&,std::string&)> material_less_3537e4;
 std::function<bool(std::uintptr_t,std::uint32_t,std::int32_t&,std::string&)> node_suborder_20;
};
bool transparent_entry_v1(TransparentEntryV1&,const std::array<float,3>& actual_camera,
 const std::array<float,3>* nullable_position,std::int32_t priority_argument,
 const TransparentQueueServicesV1&,std::string&);
bool transparent_less_v1(const TransparentEntryV1&,const TransparentEntryV1&,
 const TransparentQueueServicesV1&,bool& result,std::string&);
// A retained native queue adapter. Original vector allocator/sort implementation
// is outside these two recovered kernels; no global render-order receipt here.
class TransparentQueueV1 {
 std::vector<TransparentEntryV1> entries_;
public:
 bool append(TransparentEntryV1,const std::array<float,3>&,
  const std::array<float,3>*,std::int32_t,const TransparentQueueServicesV1&,std::string&);
 const std::vector<TransparentEntryV1>& entries()const noexcept{return entries_;}
 void clear()noexcept{entries_.clear();}
};
}
