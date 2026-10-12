#include "condition_data_init_v3.hpp"
namespace dh2::world {
namespace {
bool borrow(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,std::string*& name,std::uintptr_t*& compiled,std::string& error){
 if(offset!=0x8c&&offset!=0xb0){error="Required actual ConditionData subobject offset";return false;}
 name=base.string(offset+4);compiled=base.pointer(offset+0x1c);
 if(!name||!compiled||!base.byte(offset+0x20)){error="Required SAME constructor-backed ConditionData fields";return false;}return true;
}
}
bool condition_data_init_v3(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){
 error.clear();std::string* name{};std::uintptr_t* compiled{};if(!borrow(base,offset,name,compiled,error))return false;
 if(name->empty()||*name=="Invalid")return true; // exact source literal8da6a8
 if(!services.owner||!services.conditions){error="Required actual Arrays::Conditions snapshot";return false;}
 for(const auto& row:*services.conditions)if(row.name==*name){
  if(!services.construct_condition){error="Required actual CCondition allocation/constructor";return false;}
  std::uintptr_t receiver{};if(!services.construct_condition(receiver,error))return false;
  if(!receiver){error="Actual CCondition constructor receiver allocation failed";return false;}
  // Original stores the actual allocated receiver BEFORE initializing it.
  *compiled=receiver;
  if(!services.initialize_condition){error="Required actual CCondition::Init478914";return false;}
  return services.initialize_condition(receiver,row.argument8,row.argument4,error);
 }
 return true; // genuine table name miss leaves existing compiled/tested fields
}
bool condition_data_clear_v3(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){
 error.clear();std::string* name{};std::uintptr_t* compiled{};if(!borrow(base,offset,name,compiled,error))return false;
 if(!*compiled)return true;
 if(!services.owner||!services.destroy_condition){error="Required actual CCondition destructor/deallocation";return false;}
 if(!services.destroy_condition(*compiled,error))return false;*compiled=0;return true;
}
}
