#pragma once
#include "native_gslevel_connection_v26.hpp"
#include "canonical_level_loading_v26.hpp"
namespace dh2::loader {
// The actual native global storage. Retain once for the Application process;
// GSLevel writes this cell, and every current-Level/Loading/Kill getter lends
// this same cell. It owns no parallel readiness or development-Level flag.
struct NativeGSLevelGlobalsV27 {
 std::shared_ptr<CanonicalLevelContextV1> s_level;
 CanonicalGSLevelGlobalSlotV1 borrow(const std::shared_ptr<NativeGSLevelGlobalsV27>& self){
  return {self,&s_level};
 }
};
class NativeGSLevelRuntimeV27 {
 std::shared_ptr<NativeGSLevelGlobalsV27> globals_;
 GSLevelFieldsV2<CanonicalLevelContextV1> fields_;
 std::unique_ptr<NativeGSLevelConnectionV26> gs_;
 std::unique_ptr<CanonicalLevelLoadingV26> loading_;
 bool attempted_{};
public:
 explicit NativeGSLevelRuntimeV27(std::shared_ptr<NativeGSLevelGlobalsV27> globals):globals_(std::move(globals)){}
 // Register Loading native callbacks before the GS constructor reaches its
 // synchronous onProgress call. Absent-Level Online access stays required
 // only on that source branch, rather than blocking a nonnull Level path.
 bool prepare_loading(std::function<bool(std::uint32_t&,std::string&)>,std::string&);
 bool construct(LevelSourceRequestV1,GSLevelArgumentsV2,LevelConstructorApplicationV4,
  GSLevelServicesV2<CanonicalLevelContextV1>,std::function<bool(std::uint32_t&,std::string&)>,std::string&);
 bool destroy(std::string&);
 bool current(CanonicalCurrentLevelBorrowV1&,std::string&)const;
 ui::LoadingMenuStateServicesV1 loading_services()noexcept{return loading_?loading_->services():ui::LoadingMenuStateServicesV1{};}
  bool frame_fields_v45(GSLevelFieldsV2<CanonicalLevelContextV1>*&,std::string&);
bool owns_globals_v50(const std::shared_ptr<NativeGSLevelGlobalsV27>& actual)const noexcept{
  return actual&&globals_.get()==actual.get()&&!globals_.owner_before(actual)&&!actual.owner_before(globals_);
 }
 const GSLevelFieldsV2<CanonicalLevelContextV1>& fields()const noexcept{return fields_;}
 const std::unique_ptr<NativeGSLevelConnectionV26>& connection()const noexcept{return gs_;}
};
}
