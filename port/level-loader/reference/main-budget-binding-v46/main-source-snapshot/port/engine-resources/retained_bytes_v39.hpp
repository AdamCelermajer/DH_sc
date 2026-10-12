#pragma once
#include "resource_budget_v37.hpp"
#include <memory>
namespace dh2::resources {
// Exact-size retained CPU storage. The ledger must outlive this owner. Unlike
// vector::resize, the admitted byte count also bounds the storage capacity.
class RetainedBytesV39 {
 std::shared_ptr<ContextResourceBudgetV37> budget_;
 ResourceTokenV37 token_{};
 std::unique_ptr<std::uint8_t[]> bytes_;
 std::size_t size_{};
public:
 RetainedBytesV39()=default;
 ~RetainedBytesV39();
 RetainedBytesV39(const RetainedBytesV39&)=delete;
 RetainedBytesV39& operator=(const RetainedBytesV39&)=delete;
 RetainedBytesV39(RetainedBytesV39&&)noexcept;
 RetainedBytesV39& operator=(RetainedBytesV39&&)noexcept;
 bool allocate(std::shared_ptr<ContextResourceBudgetV37>,ResourceScopeV37,std::size_t,std::string&);
 void reset()noexcept;
 std::uint8_t* data()noexcept{return bytes_.get();}
 const std::uint8_t* data()const noexcept{return bytes_.get();}
 std::uint8_t& operator[](std::size_t i)noexcept{return bytes_[i];}
 const std::uint8_t& operator[](std::size_t i)const noexcept{return bytes_[i];}
 std::size_t size()const noexcept{return size_;}
};
// Checked actual GL_ALPHA/GL_RGB/GL_RGBA bitmap bytes, rather than treating all
// HUD font atlases as RGBA. Framebuffer color/depth storage is separately RGBA.
bool swf_bitmap_bytes_v39(std::int32_t,std::int32_t,unsigned,std::uint64_t&,std::string&);
}
