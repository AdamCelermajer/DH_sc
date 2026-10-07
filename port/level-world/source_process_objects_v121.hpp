#pragma once
#include "application_services_owner_v5.hpp"
#include "canonical_object_manager_v1.hpp"
namespace dh2::world {
//One actual process ObjectManager, first produced before any preview/Level.
//Services change receivers at real scene transitions; C1 and storage do not.
class SourceProcessObjectsV121:public std::enable_shared_from_this<SourceProcessObjectsV121> {
 std::weak_ptr<application::ApplicationServicesOwnerV5> application_;
 std::shared_ptr<CanonicalObjectManagerV1> manager_;
 CanonicalObjectManagerServicesV1 services_{};
 std::shared_ptr<void> services_owner_;
 std::function<bool(std::string&)> services_current_;
 CanonicalObjectLifecycleV1 lifecycle_;
 bool native_class_domain_produced_{};
 explicit SourceProcessObjectsV121(const std::shared_ptr<application::ApplicationServicesOwnerV5>&);
 bool current(std::string&)const;
 bool service_current(std::string&)const;
 static bool local(void*,std::uintptr_t&,std::string&);
 static bool threat(void*,const char*,std::int32_t,bool,target_providers::Handle16&,std::string&);
 static bool missing(void*,std::string&);
 static bool duplicate(void*,CanonicalObjectBorrowV1&,std::string&);
 static bool network(void*,CanonicalObjectBorrowV1&,std::string&);
 static bool published(void*,std::int32_t,const CanonicalObjectBorrowV1&,std::uintptr_t,std::string&);
public:
 static bool acquire(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
  std::shared_ptr<SourceProcessObjectsV121>&,std::string&);
 bool belongs_to(const std::shared_ptr<application::ApplicationServicesOwnerV5>&)const noexcept;
 const std::shared_ptr<CanonicalObjectManagerV1>& manager()const noexcept{return manager_;}
 //Independent adapter pin + weak receiver guard. Never a strong containing
 //World or App cycle; native source callbacks continue to use SAME context.
 bool bind_services(CanonicalObjectManagerServicesV1,std::shared_ptr<void>,
  std::function<bool(std::string&)>,std::string&);
 bool bind_lifecycle(CanonicalObjectLifecycleV1,std::string&);
 //Real source Flush, using actual manager STL containers and exact positive
 //classD0 leaves. A genuine ongoing scene/draw/contact barrier must reject.
 bool flush(const std::function<bool(CanonicalObjectManagerV1&,std::string&)>& require_quiescent,std::string&);
};
}
