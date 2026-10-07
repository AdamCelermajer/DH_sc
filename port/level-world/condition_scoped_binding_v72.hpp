#pragma once
#include <canonical_gameobject_graph_v68.hpp>
#include <condition_data_init_v3.hpp>
namespace dh2::world {
// Composition only: the input is the SAME genuine Main ConditionList services.
// No table, registry, list, evaluator or condition class is created here.
// The journal retains this binding until both embedded ConditionData objects
// have cleared. Its independent services owner must not own the containing
// World/Level/manager; actual_base must capture its receiver/scope weakly.
class ConditionDataBindingV72 final:public std::enable_shared_from_this<ConditionDataBindingV72> {
 std::uintptr_t identity_{};
 CanonicalBaseBorrowV68 actual_base_;
 ConditionDataInitServicesV3 conditions_;
 ConditionDataBindingV72(std::uintptr_t,CanonicalBaseBorrowV68,ConditionDataInitServicesV3);
 bool borrow(std::shared_ptr<void>&,CanonicalGameObjectBaseOwnerV1*&,std::string&)const;
public:
 static bool create(std::uintptr_t,CanonicalBaseBorrowV68,const ConditionDataInitServicesV3&,
  std::shared_ptr<ConditionDataBindingV72>&,std::string&);
 // Store this weak callable at the reached ObjectBase InitPost service. Keep
 // the binding separately in the existing native source-release journal.
 std::function<bool(std::uint32_t,std::string&)> initialization();
 bool initialize(std::uint32_t,std::string&);
 // Invoke at actual embedded ConditionData destruction, before receiver drop.
 // A failure retains binding/services and the exact remaining compiled slots.
 bool clear(std::string&);
};
}
