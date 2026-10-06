#pragma once
#include "native_level_application_v25.hpp"
#include "gslevel_lifecycle_v2.hpp"
namespace dh2::loader {
// Production composition of the exact outer GSLevel body and full Level C1.
// Caller lends the ONE actual s_level slot and genuine animation/menu/network/
// unload providers. No ready flag, null menu or cleanup provider is fabricated.
class NativeGSLevelConnectionV26 {
 LevelSourceRequestV1 request_;
 LevelConstructorApplicationV4 application_;
 std::shared_ptr<NativeLevelConnectionV25> connection_;
 GSLevelFieldsV2<CanonicalLevelContextV1>& fields_;
 std::shared_ptr<CanonicalLevelContextV1>& current_;
 std::unique_ptr<GSLevelLifecycleV2<CanonicalLevelContextV1>> lifecycle_;
public:
 NativeGSLevelConnectionV26(LevelSourceRequestV1 request,LevelConstructorApplicationV4 application,
  GSLevelFieldsV2<CanonicalLevelContextV1>& fields,std::shared_ptr<CanonicalLevelContextV1>& current,
  GSLevelServicesV2<CanonicalLevelContextV1> services):request_(std::move(request)),application_(std::move(application)),
  connection_(std::make_shared<NativeLevelConnectionV25>()),fields_(fields),current_(current){
  services.allocate_level=[this](auto& allocation,std::string& e){
   if(!connection_->allocate(request_,application_.owner,e))return false;
   allocation=connection_->candidate();return true;
  };
  services.construct_level=[this](const auto& allocation,const auto& a,auto& level,std::string& e){
   const auto& candidate=connection_->candidate();
   if(!candidate||allocation.get()!=candidate.get()||allocation.owner_before(candidate)||candidate.owner_before(allocation)){
    e="GSLevel constructor requires its SAME allocated native Level";return false;
   }
   const LevelConstructorArgumentsV3 input{a.name18.c_str(),a.level1c,a.word20,a.word24,a.word28,a.byte2c,a.byte2d,a.word30,a.word40};
   if(!connection_->construct_allocated(input,application_,e))return false;
   level=candidate;return true;
  };
  lifecycle_=std::make_unique<GSLevelLifecycleV2<CanonicalLevelContextV1>>(fields_,current_,std::move(services));
 }
 NativeGSLevelConnectionV26(const NativeGSLevelConnectionV26&)=delete;
 NativeGSLevelConnectionV26& operator=(const NativeGSLevelConnectionV26&)=delete;
 bool construct(){return lifecycle_->construct();}
 bool destroy(){return lifecycle_->destroy();}
 const std::shared_ptr<NativeLevelConnectionV25>& level_connection()const noexcept{return connection_;}
 const GSLevelLifecycleV2<CanonicalLevelContextV1>& lifecycle()const noexcept{return *lifecycle_;}
};
}
