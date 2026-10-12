#pragma once
#include "stage_loader_v38_init_post.hpp"
#include <map>
#include "lifecycle_v36_counter_borrow.hpp"
namespace dh2::loader {
// One reached source state10 body. Its counter reads actual map1c ONCE before
// InitPost's resumable loop, including original reserved null0. Not count50.
template<class Manager> class Stage10BodyV38 {
 LifecycleBorrowV36 level_;Manager& manager_;
 CanonicalInitPostV38<Manager> dispatcher_;bool counter_produced_{},failed_{};
 std::string error_;
public:
 Stage10BodyV38(LifecycleBorrowV36 level,std::shared_ptr<void> actual_manager_pin,
  Manager& manager,CanonicalInitPostServicesV38<Manager> services):
  level_(std::move(level)),manager_(manager),dispatcher_(std::move(actual_manager_pin),manager,std::move(services)){}
 const std::string& error()const noexcept{return error_;}
 LifecycleStepV36 step(){
  if(failed_)return LifecycleStepV36::failed;
  if(!level_.actual_level_owner||!level_.fields.state130||*level_.fields.state130!=10||!level_.fields.current138){failed_=true;error_="Require actual retained Level state10 fields";return LifecycleStepV36::failed;}
  if(!counter_produced_){*level_.fields.current138=manager_.source_map_size1c_v38();counter_produced_=true;}
  const auto result=dispatcher_.step();if(result==LifecycleStepV36::failed){failed_=true;error_=dispatcher_.error();}return result;
 }
};
// Concrete retained Module.LoadModule provider. Preparation is the actual
// RetainedLevelModuleGraphV1, parameterized so legacy headers need not be
// modified/instantiated until the reviewed manager APIs are selected.
template<class Preparation,class ModuleXmlServices>
auto retained_stage10_module_load_v38(std::shared_ptr<Preparation> retained,
 ModuleXmlServices actual_xml_services){
 struct Journal {bool attempted{},complete{},failed{};std::string error;};
 return [retained=std::move(retained),actual_xml_services=std::move(actual_xml_services),
         journal=std::map<std::uintptr_t,Journal>{}](std::uintptr_t identity,std::string& error) mutable ->LifecycleStepV36 {
  if(!retained){error="Required actual retained module preparation";return LifecycleStepV36::dependency_missing;}
  auto& attempt=journal[identity];if(attempt.complete)return LifecycleStepV36::complete;
  if(attempt.failed||attempt.attempted){error=attempt.error.empty()?"Module source prefix already attempted":attempt.error;return LifecycleStepV36::failed;}
  auto reject=[&](const std::string& why){attempt.failed=true;attempt.error=why;error=why;return LifecycleStepV36::failed;};
  typename std::remove_reference_t<decltype(*retained->level())>::LoadingFieldsV26 fields;
  if(!retained->level()->loading_fields_v26(fields,error))return reject(error);
  auto& manager=retained->manager();
  if(!retained->status().root_complete||!fields.state130||*fields.state130!=10||manager.source_init_phase7c_v38()!=1)return reject("Module source loading requires actual state10/phase1 after root factories");
  // This API intentionally bypasses inspection facade's geometry-prepared
  // gate: source phase1 loads MGP/MVP before generic InitPost/floor/InitFinal.
  const auto& modules=retained->modules();
  for(std::size_t i=0;i<modules.size();++i){
   const auto& record=modules[i];if(!record||!record->receiver||record->receiver->base().identity()!=identity)continue;
   if(!retained->module_files()->begin_module(std::uint32_t(i),error))return reject(error);
   attempt.attempted=true;
   // Existing source module_load_v1 performs original selector/shared RNG,
   // Level18c/160 placement prefix, MGP then MVP, reset on successful tail.
   // It spins synchronously inside one real call; wrapper cannot preempt it.
   if(!record->receiver->load(actual_xml_services,retained->module_files()->load_borrow(),error))return reject(error);
   attempt.complete=true;return LifecycleStepV36::complete;
  }
  return reject("Actual Module list receiver absent from retained source records; dynamic module-record binding required");
 };
}
}
