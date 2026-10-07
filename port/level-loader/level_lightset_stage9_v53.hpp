#pragma once
#include "lifecycle_v36.hpp"
#include <canonical_level_context_v1.hpp>
#include <canonical_level_config_module_v1.hpp>
namespace dh2::loader {
struct Stage9ConfigBorrowV53 {
 std::shared_ptr<void> level_owner,config_owner;
 std::uintptr_t* actual_config38{};std::uintptr_t config_identity{};
 const std::string* fixed2d0{};const std::string* regular2b8{};
};
bool borrow_stage9_config_v53(const std::shared_ptr<CanonicalLevelContextV1>&,
 const std::shared_ptr<world::CanonicalLevelConfigV1>&,Stage9ConfigBorrowV53&,std::string&);
struct Stage9ServicesV53 {
 std::shared_ptr<void> actual_device_owner;
 std::function<bool(bool&,std::string&)> is_fixed_pipeline;
 std::shared_ptr<void> actual_file_loader_owner;
 std::function<LifecycleStepV36(const std::string&,const char*,std::string&)> load_module_file_step;
 // Check actual current Level/config/device owner on each synchronous poll.
 std::function<bool(std::string&)> validate_current;
};
// Source _LoadLightSet3f459c: one of fixed/regular files, tag Module.
// Nonblocking poll adaptation; original loops synchronously until LoadFile!=0.
// This stores no alternate Level, manager, light-set Names or gameplay state.
class Stage9BodyV53 final {
 Stage9ConfigBorrowV53 fields_;Stage9ServicesV53 services_;
 bool selected_{},fixed_{},complete_{},failed_{},busy_{};std::string filename_,error_;
 LifecycleStepV36 fail(const std::string&,std::string&);
public:
 Stage9BodyV53(Stage9ConfigBorrowV53 f,Stage9ServicesV53 s):fields_(std::move(f)),services_(std::move(s)){}
 LifecycleStepV36 step(std::string&);
 const std::string& selected_filename()const noexcept{return filename_;}
};
}
