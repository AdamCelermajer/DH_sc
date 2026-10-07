#pragma once
#include "resource_budget_v37.hpp"
#include <exception>
#include <type_traits>
#include <utility>
#include <vector>
namespace dh2::resources {
// Pair with a vector's actual capacity. Declare BEFORE the vector so storage
// dies before charges. Growth admits old+new coexisting storage, not just delta.
class CpuVectorCapacityV41 {
 std::shared_ptr<ContextResourceBudgetV37> budget_;
 ResourceTokenV37 token_{};
 std::uint64_t bytes_{};
 ResourceScopeV37 scope_{ResourceScopeV37::other};
public:
 CpuVectorCapacityV41()=default;
 ~CpuVectorCapacityV41(){reset();}
 CpuVectorCapacityV41(const CpuVectorCapacityV41&)=delete;
 CpuVectorCapacityV41& operator=(const CpuVectorCapacityV41&)=delete;
 CpuVectorCapacityV41(CpuVectorCapacityV41&& other)noexcept
  :budget_(std::move(other.budget_)),token_(std::exchange(other.token_,{})),bytes_(std::exchange(other.bytes_,0)),scope_(other.scope_){}
 CpuVectorCapacityV41& operator=(CpuVectorCapacityV41&& other)noexcept{
  if(this!=&other){reset();budget_=std::move(other.budget_);token_=std::exchange(other.token_,{});bytes_=std::exchange(other.bytes_,0);scope_=other.scope_;}return *this;
 }
 void reset()noexcept{
  if(token_){std::string error;if(!budget_||!budget_->release(token_,error))std::terminate();}
  bytes_=0;budget_.reset();
 }
 bool owns()const noexcept{return bool(token_);}
 bool same_owner(const std::shared_ptr<ContextResourceBudgetV37>& budget,ResourceScopeV37 scope)const noexcept{return !token_||(budget_==budget&&scope_==scope);}
 std::uint64_t bytes()const noexcept{return bytes_;}
 bool reserve(const std::shared_ptr<ContextResourceBudgetV37>& budget,ResourceScopeV37 scope,std::uint64_t bytes,ResourceReservationV37& out,std::string& error){
  if(!budget||!bytes||(token_&&(budget_!=budget||scope_!=scope))){error="V41 vector capacity requires same admitted ledger/scope";return false;}
  const ResourceChargeV37 charge{ResourceKindV37::cpu_request,scope,0,bytes};
  return token_?budget->reserve_replace(token_,charge,ReplacementModeV37::same_object_coexisting_storage,out,error):budget->reserve_create(charge,out,error);
 }
 bool commit(const std::shared_ptr<ContextResourceBudgetV37>& budget,ResourceScopeV37 scope,std::uint64_t bytes,ResourceReservationV37& reservation,std::string& error){
  if(!reservation.commit(token_,error))return false;
  budget_=budget;scope_=scope;bytes_=bytes;return true;
 }
};
template<class T> bool reserve_cpu_vector_v41(std::vector<T>& vector,CpuVectorCapacityV41& owner,
 const std::shared_ptr<ContextResourceBudgetV37>& budget,ResourceScopeV37 scope,std::size_t wanted,std::string& error){
 static_assert(std::is_trivially_copyable_v<T>&&std::is_trivially_destructible_v<T>,"V41 numeric renderer buffers only");
 std::uint64_t actual;
 if(!checked_resource_bytes_v37(vector.capacity(),sizeof(T),actual,error))return false;
 if(actual!=owner.bytes()||bool(actual)!=owner.owns()){error="V41 vector capacity differs from its admitted owner; adoption after allocation prohibited";return false;}
 if(!owner.same_owner(budget,scope)){error="V41 vector requires same admitted ledger/scope even without growth";return false;}
 if(wanted<=vector.capacity())return true;
 std::uint64_t bytes;if(!checked_resource_bytes_v37(wanted,sizeof(T),bytes,error))return false;
 ResourceReservationV37 pending;if(!owner.reserve(budget,scope,bytes,pending,error))return false;
 // reserve requests exactly wanted elements on the supported C++17 libraries.
 // Existing accepted vector survives allocator failure; no copy can throw here.
 vector.reserve(wanted);
 if(vector.capacity()!=wanted){
  std::vector<T>().swap(vector);pending.abort();owner.reset();
  error="V41 unsupported vector reserve capacity policy";return false;
 }
 return owner.commit(budget,scope,bytes,pending,error);
}
template<class T> struct CpuGeometryStorageV41 {
 CpuVectorCapacityV41 capacity;
 std::vector<T> vertices;
 CpuGeometryStorageV41()=default;
 CpuGeometryStorageV41(const CpuGeometryStorageV41&)=delete;
 CpuGeometryStorageV41& operator=(const CpuGeometryStorageV41&)=delete;
 CpuGeometryStorageV41(CpuGeometryStorageV41&&)=default;
 CpuGeometryStorageV41& operator=(CpuGeometryStorageV41&& other)noexcept{
  if(this!=&other){vertices=std::move(other.vertices);capacity=std::move(other.capacity);}return *this;
 }
};
}
