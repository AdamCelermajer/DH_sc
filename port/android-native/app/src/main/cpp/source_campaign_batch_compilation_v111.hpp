#pragma once
#include <native_batch_compiler_v111.hpp>
#include <android/asset_manager.h>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
struct NativeBatchResourceLoansV111 {
 std::shared_ptr<void> owner;
 AAssetManager* assets{};
 std::function<bool(std::uintptr_t,std::shared_ptr<dh2::world::NativeBatchMeshV110>&,std::string&)> borrow_mesh;
 std::function<bool(std::uintptr_t,std::shared_ptr<dh2::world::NativeBatchNodeV110>&,std::string&)> borrow_node;
 //SceneManager creates CBatchDriver using SAME underlying driver.d4
 //capability9c/a0 values, not authored Config limits or a hardware guess.
 std::function<bool(std::uint32_t&,std::uint32_t&,std::string&)> initial_driver_limits;
};
bool bind_native_batch_compilation_v110(const SourceCampaignCandidateBorrowV55&,
 NativeBatchResourceLoansV111,dh2::loader::BatchNativeServicesV96&,std::string&);
bool upload_source_batch_mesh_v111(const std::shared_ptr<void>& actual_world,
 const std::shared_ptr<dh2::world::NativeBatchMeshV110>&,AAssetManager*,std::string&);
bool release_source_batch_mesh_v111(const std::shared_ptr<void>& actual_world,std::uintptr_t,std::string&);
bool invalidate_source_batch_mesh_v111(const std::shared_ptr<void>& actual_world,std::uintptr_t,std::string&);
}
