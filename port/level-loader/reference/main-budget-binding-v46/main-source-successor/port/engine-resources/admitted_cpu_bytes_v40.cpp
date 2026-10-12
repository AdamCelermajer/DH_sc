#include "admitted_cpu_bytes_v40.hpp"
#include <exception>
#include <utility>
namespace dh2::resources {
CpuAdmissionV40::~CpuAdmissionV40(){reset();}
CpuAdmissionV40::CpuAdmissionV40(CpuAdmissionV40&& other)noexcept
 :budget_(std::move(other.budget_)),token_(std::exchange(other.token_,{})),pending_(std::move(other.pending_)){}
CpuAdmissionV40& CpuAdmissionV40::operator=(CpuAdmissionV40&& other)noexcept{
 if(this!=&other){reset();budget_=std::move(other.budget_);token_=std::exchange(other.token_,{});pending_=std::move(other.pending_);}return *this;
}
void CpuAdmissionV40::reset()noexcept{
 pending_.abort();
 if(token_){std::string error;if(!budget_||!budget_->release(token_,error))std::terminate();}
 budget_.reset();
}
bool CpuAdmissionV40::reserve(std::shared_ptr<ContextResourceBudgetV37> budget,ResourceScopeV37 scope,std::uint64_t bytes,std::string& error){
 if(!budget||budget_){error="V40 CPU admission requires fresh owner and retained ledger lease";return false;}
 if(!budget->reserve_create({ResourceKindV37::cpu_request,scope,0,bytes},pending_,error))return false;
 budget_=std::move(budget);return true;
}
bool CpuAdmissionV40::commit(std::string& error){return pending_.commit(token_,error);}
}
