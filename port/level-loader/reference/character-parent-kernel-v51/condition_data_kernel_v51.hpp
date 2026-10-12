#pragma once
#include "condition_data_services_v51.hpp"
namespace dh2::world {
namespace condition_kernel_detail_v51 {
template<class Fields>
bool borrow(Fields& base,std::uint32_t offset,const std::string*& name,std::uintptr_t*& compiled,std::string& error){
 if(offset!=0x8c&&offset!=0xb0){error="Required actual ConditionData subobject offset";return false;}
 name=base.string(offset+4);compiled=base.pointer(offset+0x1c);
 if(!name||!compiled||!base.byte(offset+0x20)){error="Required SAME constructor-backed ConditionData fields";return false;}return true;
}
}
template<class Fields>
bool condition_data_init_borrow_v51(Fields& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){
 error.clear();const std::string* name{};std::uintptr_t* compiled{};if(!condition_kernel_detail_v51::borrow(base,offset,name,compiled,error))return false;
 if(name->empty()||*name=="Invalid")return true; // exact source literal8da6a8
 if(!services.owner||!services.conditions){error="Required actual Arrays::Conditions snapshot";return false;}
 for(const auto& row:*services.conditions)if(row.name==*name){
  if(!services.construct_condition){error="Required actual ConditionList allocation/constructor";return false;}
  std::uintptr_t receiver{};if(!services.construct_condition(receiver,error))return false;
  if(!receiver){error="Actual ConditionList constructor receiver allocation failed";return false;}
  // Original stores the actual allocated receiver BEFORE initializing it.
  *compiled=receiver;
  if(!services.initialize_condition){error="Required actual ConditionList::AssignPyData478914";return false;}
  return services.initialize_condition(receiver,row.argument8,row.argument4,error);
 }
 return true; // genuine table name miss leaves existing compiled/tested fields
}
template<class Fields>
bool condition_data_clear_borrow_v51(Fields& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){
 error.clear();const std::string* name{};std::uintptr_t* compiled{};if(!condition_kernel_detail_v51::borrow(base,offset,name,compiled,error))return false;
 if(!*compiled)return true;
 if(!services.owner||!services.destroy_condition){error="Required actual ConditionList destructor/CustomFree";return false;}
 if(!services.destroy_condition(*compiled,error))return false;*compiled=0;return true;
}
}
