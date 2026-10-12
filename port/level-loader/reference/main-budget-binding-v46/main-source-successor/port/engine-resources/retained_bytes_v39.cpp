#include "retained_bytes_v39.hpp"
#include <exception>
#include <limits>
#include <utility>
namespace dh2::resources {
RetainedBytesV39::~RetainedBytesV39(){reset();}
RetainedBytesV39::RetainedBytesV39(RetainedBytesV39&& other)noexcept
 :budget_(std::move(other.budget_)),token_(std::exchange(other.token_,{})),
 bytes_(std::move(other.bytes_)),size_(std::exchange(other.size_,0)){}
RetainedBytesV39& RetainedBytesV39::operator=(RetainedBytesV39&& other)noexcept{
 if(this!=&other){reset();budget_=std::move(other.budget_);token_=std::exchange(other.token_,{});bytes_=std::move(other.bytes_);size_=std::exchange(other.size_,0);}return *this;
}
void RetainedBytesV39::reset()noexcept{
 // Deallocate first: accounting cannot advertise free capacity while the actual
 // old buffer is still retained. A valid owned token always releases successfully.
 bytes_.reset();size_=0;
 if(token_){std::string error;if(!budget_||!budget_->release(token_,error))std::terminate();}
 budget_.reset();
}
bool RetainedBytesV39::allocate(std::shared_ptr<ContextResourceBudgetV37> budget,ResourceScopeV37 scope,std::size_t size,std::string& error){
 if(!size||!budget||budget_){error="V39 retained storage requires nonempty unpublished owner and ledger lease";return false;}
 ResourceReservationV37 reservation;
 if(!budget->reserve_create({ResourceKindV37::cpu_request,scope,0,size},reservation,error))return false;
 // Pending admission is already visible BEFORE allocation. Exceptions destroy
 // reservation and candidate, preserving the empty destination and old owners.
 auto candidate=std::make_unique<std::uint8_t[]>(size);
 ResourceTokenV37 token;
 if(!reservation.commit(token,error))return false;
 bytes_=std::move(candidate);size_=size;token_=token;budget_=std::move(budget);return true;
}
bool swf_bitmap_bytes_v39(std::int32_t width,std::int32_t height,unsigned channels,std::uint64_t& out,std::string& error){
 if(width<=0||height<=0||(channels!=1&&channels!=3&&channels!=4)){error="V39 bitmap dimensions/channels outside supported domain";return false;}
 std::uint64_t pixels,next;
 if(!checked_resource_bytes_v37(std::uint32_t(width),std::uint32_t(height),pixels,error)||!checked_resource_bytes_v37(pixels,channels,next,error))return false;
 if(next>std::numeric_limits<std::size_t>::max()){error="V39 bitmap byte range exceeds host capacity";return false;}
 out=next;return true;
}
}
