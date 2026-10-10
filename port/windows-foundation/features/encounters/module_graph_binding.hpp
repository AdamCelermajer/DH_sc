#pragma once
#include "../../source_module_construction.hpp"
#include "../../../level-world/canonical_module_graph_v3.hpp"
namespace dh::foundation::encounters {
// A construction-time bridge into the existing SAME Module graph. The host
// retains graph externally; its callbacks weakly resolve it. No new PF world,
// registry, module receiver, phase scheduling or admission is created here.
inline bool bind_module_graph_construction(
 SourceModuleConstructionProviders& actual_construction,
 const std::shared_ptr<dh2::world::CanonicalModuleGraphV3>& graph,
 const std::shared_ptr<dh2::world::NativeConditionRuntimeV69>& same_conditions,
 std::string& error){
 if(!graph||!same_conditions||actual_construction.initialization){
  error="Module graph bridge requires actual graph/condition arena and unbound construction initialization";return false;
 }
 if(actual_construction.conditions&&actual_construction.conditions!=same_conditions){
  error="Module graph bridge cannot replace SAME constructor condition arena";return false;
 }
 if(!graph->bind_condition_services_v75(same_conditions->condition_data_services(),error))return false;
 actual_construction.conditions=same_conditions;
 std::weak_ptr<dh2::world::CanonicalModuleGraphV3> weak=graph;
 actual_construction.initialization=[weak](const auto&,const auto& record,auto& init,auto& module,std::string& e){
  auto retained=weak.lock();if(!retained){e="Actual Module graph expired before constructor initialization binding";return false;}
  return retained->bind(record,init,module,e);
 };
 error.clear();return true;
}
}
