#pragma once
#include "canonical_level_config_module_v1.hpp"
namespace dh2::world {
struct CanonicalModuleRecordV2 {
 // Sole runtime owned by the canonical receiver allocation, not loader state.
 actor::RuntimeState runtime;
 std::unique_ptr<CanonicalModuleV1> receiver;
 std::shared_ptr<const void> declaration;
};
struct CanonicalLevelModuleConstructionV2 {
 std::shared_ptr<void> candidate;
 LevelConfigServicesV1 level_config;
 ModuleRuntimeGlobalsV1* module_globals{}; // one retained source static owner
 // Called once before ctor; callbacks borrow record->receiver->base() AFTER
 // ctor. record is stable and services must capture weak record to avoid cycles.
 std::function<bool(const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalModuleRecordV2>&,
  GameObjectInitializationServicesV1&,ModuleInitServicesV1&,std::string&)> module_services;
};
// Additive class dispatcher consuming CURRENT loader CanonicalClassServicesV1.
// Other classes delegate to the SAME prior dispatcher; no duplicate manager.
class CanonicalLevelModuleBindingsV2 {
 CanonicalPropertyMapV1& properties_;
 CanonicalClassServicesV1 predecessor_;
 CanonicalLevelModuleConstructionV2 construction_;
 std::map<std::uintptr_t,CanonicalClassReceiverV1> receivers_;
 std::map<std::uintptr_t,std::shared_ptr<CanonicalLevelConfigV1>> configs_;
 std::map<std::uintptr_t,std::shared_ptr<CanonicalModuleRecordV2>> modules_;
 static bool construct(void*,const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,CanonicalObjectBorrowV1&,std::string&);
 static bool init_properties(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool set_template(void*,const CanonicalObjectBorrowV1&,const char*,std::string&);
 static bool defaults(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool overrides(void*,const CanonicalObjectBorrowV1&,const CanonicalSourceObjectRequestV1&,std::string&);
 static bool init_post(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool is_game_object(void*,const CanonicalObjectBorrowV1&,bool&,std::string&);
 static bool position(void*,const CanonicalObjectBorrowV1&,std::array<float,3>&,std::string&);
 static bool set_position(void*,const CanonicalObjectBorrowV1&,const std::array<float,3>&,bool,std::string&);
 static bool unknown(void*,const char*,std::string&);
 CanonicalClassReceiverV1* find(std::uintptr_t)noexcept;
public:
 CanonicalLevelModuleBindingsV2(CanonicalPropertyMapV1& p,CanonicalClassServicesV1 previous,CanonicalLevelModuleConstructionV2 c):properties_(p),predecessor_(previous),construction_(std::move(c)){}
 CanonicalClassServicesV1 services()noexcept;
 // Actual constructor result for loader's catalog-wide receiver transport.
 // Supports only this owner's proven LevelConfig/Module/Block catalog entries.
 bool construct_receiver(const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,
  CanonicalClassReceiverV1&,std::string&);
 const CanonicalLevelConfigV1* level_config(std::uintptr_t)const noexcept;
 CanonicalModuleRecordV2* module(std::uintptr_t)noexcept;
 // Actual manager removal/unpublication and graph release FIRST. This only
 // drops factory retention; it is not a no-op replacement for whole cleanup.
 void erased(std::uintptr_t id){receivers_.erase(id);configs_.erase(id);modules_.erase(id);}
 std::size_t retained_count()const noexcept{return receivers_.size();}
};
}
