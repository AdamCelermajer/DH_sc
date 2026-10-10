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
 std::shared_ptr<CanonicalLevelLoadingV26> loading_;
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
 bool abort_unpublished_constructor_v114(std::string&);
 bool bind_release_services_v88(
  std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> unload,
  std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> destroy,
  std::string& error){
  if(!gs_){error="Required SAME actual GS C1 before release enrollment";return false;}
  return gs_->bind_release_services_v88(std::move(unload),std::move(destroy),error);
 }
 bool bind_failed_constructor_release_v115(
  std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> unload,
  std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> destroy,std::string& e){
  if(!gs_||!globals_||!fields_.level34||globals_->s_level!=fields_.level34){e="Require actual failed GS published Level";return false;}
  return gs_->bind_failed_constructor_release_v115(std::move(unload),std::move(destroy),e);
 }
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
