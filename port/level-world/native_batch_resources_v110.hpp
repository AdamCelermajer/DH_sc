#pragma once
#include <level_batching_source_v96.hpp>
#include "gameobject_scene_root_registry_v1.hpp"
#include <array>
#include <functional>
#include <memory>
#include <vector>
namespace dh2::world {
class NativeBatchCompiledV111;
// Native successors of the actual batch resource allocations. Host leases
// protect synchronous loans; source reference counts alone drive native D1.
struct NativeBatchLifetimeV110 {
 std::shared_ptr<void> owner;
 std::function<bool(std::string&)> require_release;
};
struct NativeBatchSegmentV110 {
 std::uintptr_t game_object2c{};
};
class NativeBatchMeshV110 : public std::enable_shared_from_this<NativeBatchMeshV110> {
 NativeBatchLifetimeV110 lifetime_;
 std::uint32_t references4_{}; //578af0: C1 stores ZERO, not one.
 std::uint32_t attributes70_{};
 std::array<float,6> bounds38_{-1,-1,-1,1,1,1},bounds50_{-1,-1,-1,1,1,1};
 std::int32_t source_word68_{},source_word6c_{-1};
 std::uint8_t source_byte74_{1};std::uint32_t source_word78_{};
 std::vector<NativeBatchSegmentV110> segments_;
 std::shared_ptr<NativeBatchCompiledV111> compiled_v111_;
 bool destroyed_{},destroying_{};
public:
 NativeBatchMeshV110(std::uint32_t attributes,NativeBatchLifetimeV110 lifetime):lifetime_(std::move(lifetime)),attributes70_(attributes){}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool grab(std::string&);bool drop(std::string&);
 bool borrow(loader::BatchMeshBorrowV96&,std::string&);
 bool segment(std::size_t,loader::BatchSegmentBorrowV96&,std::string&);
 std::uint32_t references()const noexcept{return references4_;}
 bool live()const noexcept{return !destroyed_&&!destroying_;}
 std::uint32_t attributes()const noexcept{return attributes70_;}
 //Only the real Scene compiler publishes segments. Empty C1 is never
 //advertised as a completed compiled mesh or as uploaded GPU storage.
 bool source_update_bounds_v111(const std::array<float,6>&,std::string&);
 bool publish_compiled_v111(std::shared_ptr<NativeBatchCompiledV111>,std::string&);
 const auto& compiled_v111()const noexcept{return compiled_v111_;}
 std::vector<NativeBatchSegmentV110>& compiler_segments()noexcept{return segments_;}
};
class NativeBatchNodeV110 : public std::enable_shared_from_this<NativeBatchNodeV110> {
 NativeBatchLifetimeV110 lifetime_;
 std::shared_ptr<NativeBatchMeshV110> mesh130_;
 std::uint32_t references164_{1},flags11c_{0x60f},culling118_{},word134_{2},word138_{2};
 std::int32_t id10c_{-1},word144_{-1},word15c_{-1};
 std::uint32_t word13c_{},word140_{},word14c_{},word154_{},word158_{};
 std::uint8_t byte148_{1},byte150_{1},local120_{1},parent121_{1};
 std::uintptr_t parentec_{},manager110_{};
 std::string name_;
 bool mesh_grabbed_{},destroyed_{},destroying_{};
 std::function<void()> notify_visibility_;
public:
 explicit NativeBatchNodeV110(std::int32_t id,NativeBatchLifetimeV110 lifetime):lifetime_(std::move(lifetime)),id10c_(id){}
 bool initialize_mesh(std::shared_ptr<NativeBatchMeshV110>,std::string&);
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool grab(std::string&);bool drop(std::string&);
 bool set_visible(bool,std::string&);bool notify_visibility(bool,std::string&);
 bool borrow(loader::BatchRootBorrowV96&,std::string&);
 bool scene_loan(const std::shared_ptr<GameObjectSceneRootRegistryV1>&,GameObjectSceneRootBorrowV1&,std::string&);
 bool source_static_frame_v111(std::uint32_t,std::string&);
 bool source_compile_setup_v112(std::uint32_t,std::string&);
 bool source_visible_v111()const noexcept{return (flags11c_&1u)!=0;}
 bool live()const noexcept{return !destroyed_&&!destroying_;}
 std::uint32_t references()const noexcept{return references164_;}
 const std::shared_ptr<NativeBatchMeshV110>& mesh()const noexcept{return mesh130_;}
};
}
