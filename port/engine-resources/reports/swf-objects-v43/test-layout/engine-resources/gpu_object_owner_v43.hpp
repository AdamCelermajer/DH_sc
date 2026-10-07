#pragma once
#include "resource_budget_v37.hpp"
#include <exception>
#include <stdexcept>
#include <utility>
namespace dh2::resources {
// Owns a requested GL object, not driver residency. Deletion is injected so the
// same lifetime protocol is tested without a graphics driver. Names from an old
// context are never deleted in the new context, even when the integer is reused.
class GpuObjectOwnerV43 {
 std::shared_ptr<ContextResourceBudgetV37> budget_;
 ResourceTokenV37 token_{};
 std::uint64_t generation_{};
 std::uint32_t name_{};
 ResourceKindV37 kind_{ResourceKindV37::vertex_buffer};
 ResourceScopeV37 scope_{ResourceScopeV37::other};
 using Delete=void(*)(std::uint32_t);
 Delete delete_{};
public:
 GpuObjectOwnerV43()=default;
 ~GpuObjectOwnerV43(){reset();}
 GpuObjectOwnerV43(const GpuObjectOwnerV43&)=delete;
 GpuObjectOwnerV43& operator=(const GpuObjectOwnerV43&)=delete;
 GpuObjectOwnerV43(GpuObjectOwnerV43&& other)noexcept{*this=std::move(other);}
 GpuObjectOwnerV43& operator=(GpuObjectOwnerV43&& other)noexcept{
  if(this!=&other){reset();budget_=std::move(other.budget_);token_=std::exchange(other.token_,{});
   generation_=other.generation_;name_=std::exchange(other.name_,0);kind_=other.kind_;scope_=other.scope_;delete_=other.delete_;}
  return *this;
 }
 std::uint32_t name()const noexcept{return name_;}
 std::uint64_t generation()const noexcept{return generation_;}
 bool live()const{if(!name_||!budget_)return false;const auto c=budget_->context_state_v41();return c.ready&&c.generation==generation_;}
 void reset()noexcept{
  if(name_&&live()&&delete_)delete_(name_);
  name_=0;
  if(token_){std::string error;if(!budget_||!budget_->release(token_,error))std::terminate();}
  budget_.reset();
 }
 template<class Create> bool create(const std::shared_ptr<ContextResourceBudgetV37>& budget,
   ResourceKindV37 kind,ResourceScopeV37 scope,std::uint64_t bytes,Create allocate,Delete destroy,std::string& error){
  if(name_||token_||!budget||!destroy){error="V43 empty GPU owner, ledger and deleter required";return false;}
  ResourceReservationV37 admission;if(!budget->reserve_create({kind,scope,bytes,0},admission,error))return false;
  const auto generation=budget->context_state_v41().generation;
  std::uint32_t candidate=0;
  try{allocate(candidate);if(!candidate)throw std::runtime_error("V43 GL returned zero object name");
   if(!admission.commit(token_,error))throw std::runtime_error(error);
  }catch(...){const auto context=budget->context_state_v41();if(candidate&&context.ready&&context.generation==generation)destroy(candidate);throw;}
  budget_=budget;generation_=generation;name_=candidate;kind_=kind;scope_=scope;delete_=destroy;return true;
 }
 template<class Upload> bool replace_storage(std::uint64_t bytes,Upload upload,std::string& error){
  if(!live()){error="V43 GPU storage owner belongs to unavailable context";return false;}
  ResourceReservationV37 admission;
  if(!budget_->reserve_replace(token_,{kind_,scope_,bytes,0},ReplacementModeV37::same_object_coexisting_storage,admission,error))return false;
  try{upload(name_);if(!admission.commit(token_,error))throw std::runtime_error(error);}
  catch(...){admission.abort();reset();throw;}
  return true;
 }
};
}
