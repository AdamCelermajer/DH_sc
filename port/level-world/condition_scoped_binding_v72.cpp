#include "condition_scoped_binding_v72.hpp"
#include <utility>
namespace dh2::world {
ConditionDataBindingV72::ConditionDataBindingV72(std::uintptr_t id,CanonicalBaseBorrowV68 b,ConditionDataInitServicesV3 s)
 :identity_(id),actual_base_(std::move(b)),conditions_(std::move(s)){}
bool ConditionDataBindingV72::borrow(std::shared_ptr<void>& pin,CanonicalGameObjectBaseOwnerV1*& base,std::string& e)const{
 pin.reset();base=nullptr;
 if(!actual_base_||!actual_base_(pin,base,e))return false;
 if(!pin||!base||base->identity()!=identity_){e="Required SAME live ConditionData constructor/base receiver";return false;}
 return true;
}
bool ConditionDataBindingV72::create(std::uintptr_t id,CanonicalBaseBorrowV68 b,const ConditionDataInitServicesV3& s,
 std::shared_ptr<ConditionDataBindingV72>& out,std::string& e){
 if(!id||!b||!s.owner||!s.conditions||!s.construct_condition||!s.initialize_condition||!s.destroy_condition){
  e="Required genuine complete Main ConditionList services and scoped receiver borrower";return false;
 }
 auto next=std::shared_ptr<ConditionDataBindingV72>(new ConditionDataBindingV72(id,std::move(b),s));
 std::shared_ptr<void> pin;CanonicalGameObjectBaseOwnerV1* base{};
 if(!next->borrow(pin,base,e))return false;
 // Reject a direct receiver-owned service alias. Deeper capture ownership is
 // the existing provider contract: service callbacks must borrow scope weakly.
 if(!s.owner.owner_before(pin)&&!pin.owner_before(s.owner)){
  e="Required independent ConditionList service owner, separate from receiver";return false;
 }
 out=std::move(next);e.clear();return true;
}
std::function<bool(std::uint32_t,std::string&)> ConditionDataBindingV72::initialization(){
 const std::weak_ptr<ConditionDataBindingV72> weak=shared_from_this();
 return [weak](std::uint32_t offset,std::string& e){const auto same=weak.lock();
  if(!same){e="Required retained ConditionData source-release binding";return false;}
  return same->initialize(offset,e);
 };
}
bool ConditionDataBindingV72::initialize(std::uint32_t offset,std::string& e){
 std::shared_ptr<void> pin;CanonicalGameObjectBaseOwnerV1* base{};
 if(!borrow(pin,base,e))return false;
 return condition_data_init_v3(*base,offset,conditions_,e);
}
bool ConditionDataBindingV72::clear(std::string& e){
 // One scoped pin spans both ordered native clears, including provider calls.
 std::shared_ptr<void> pin;CanonicalGameObjectBaseOwnerV1* base{};
 if(!borrow(pin,base,e))return false;
 return condition_data_clear_v3(*base,0x8c,conditions_,e)&&condition_data_clear_v3(*base,0xb0,conditions_,e);
}
}
