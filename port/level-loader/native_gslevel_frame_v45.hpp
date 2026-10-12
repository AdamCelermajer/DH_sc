#pragma once
#include <native_gslevel_runtime_v27.hpp>
#include "gslevel_update_v44_level_borrow.hpp"
namespace dh2::loader {
// Sibling connection: pins the original GS runtime, which does not own this
// frame driver. This avoids a GS -> frame -> GS ownership cycle.
class NativeGSLevelFrameV45 final {
 std::shared_ptr<NativeGSLevelRuntimeV27> runtime_;
 GSLevelFieldsV2<CanonicalLevelContextV1>* fields_{};
 std::unique_ptr<GSLevelUpdateV44<CanonicalLevelContextV1>> update_;
 std::string error_,required_;bool failed_{};
 NativeGSLevelFrameV45(std::shared_ptr<NativeGSLevelRuntimeV27>,GSLevelFieldsV2<CanonicalLevelContextV1>*,GSApplicationBorrowV44,GSLevelUpdateServicesV44<CanonicalLevelContextV1>);
public:
 static bool create(std::shared_ptr<NativeGSLevelRuntimeV27>,GSApplicationBorrowV44,GSLevelUpdateServicesV44<CanonicalLevelContextV1>,std::unique_ptr<NativeGSLevelFrameV45>&,std::string&);
 NativeGSLevelFrameV45(const NativeGSLevelFrameV45&)=delete;
 NativeGSLevelFrameV45& operator=(const NativeGSLevelFrameV45&)=delete;
 GSLevelUpdateResultV44 tick();
 const std::string& error()const noexcept{return error_;}
 const std::string& required_service()const noexcept{return required_;}
};
}
