#include "script_command_receivers_v58.hpp"
#include <stdexcept>
namespace dh2::loader {
#include "command_c1_descriptors_v58.inc"
namespace {
std::int32_t signed_word(std::uint32_t v){std::int32_t s;std::memcpy(&s,&v,4);return s;}
bool operand_domain(const CommandC1DescriptorV58& d,std::uint32_t o,std::uint8_t n){return o>=16&&(n==1||n==4)&&o+n<=d.original_size&&(n==1||o%4==0);}
}
CanonicalScriptCommandV58::CanonicalScriptCommandV58(const CommandC1DescriptorV58& d,ScriptCommandBehaviorV58 b):descriptor_(d),behaviors_(std::move(b)){
 for(const auto& s:d.stores){if(s.offset==4){if(s.width!=1)throw std::runtime_error("Original skip4 store width mismatch");skip4_=std::uint8_t(s.value);}else if(s.offset==8){kind8_=signed_word(s.value);}else if(s.offset==12){if(s.value)throw std::runtime_error("Original data_c constructor must be NULL");data_c_=0;}else if(operand_domain(d,s.offset,std::uint8_t(s.width)))operands_[s.offset]=OperandScalarV58{std::uint8_t(s.width),s.value};else throw std::runtime_error("Unrecovered original C1 operand store");}
}
bool CanonicalScriptCommandV58::fail(const std::string& e,std::string& error){failed_=true;failure_=e.empty()?"Command required actual body failed":e;error=failure_;return false;}
bool CanonicalScriptCommandV58::checked_data_borrow(CheckedCommandBorrowV58& out,std::string& error){
 if(released_){error="Released actual command receiver";return false;}
 if(receiver_kind()!=descriptor_.kind||kind8_!=descriptor_.kind||!data_||!data_->complete()||data_->source_kind()!=descriptor_.kind||data_c_!=data_->identity()){error="Required SAME actual command/header/generated-data assignment";return false;}
 CheckedCommandBorrowV58 next{shared_from_this(),data_,identity(),&descriptor_,&skip4_,&kind8_,&data_c_};out=std::move(next);error.clear();return true;
}
ScriptCommandBorrowV52 CanonicalScriptCommandV58::constructor_borrow(){auto self=shared_from_this();return {self,identity(),descriptor_.factory,&skip4_,&kind8_,&data_c_,&data_,[self](std::string& e){return self->init(e);},[self](std::string& e){return self->release_base_storage(e);}};}
bool CanonicalScriptCommandV58::init(std::string& error){
 if(busy_)return fail("Actual command Init reentered; prefix retained",error);if(failed_){error=failure_;return false;}if(initialized_)return fail("Actual command Init already completed; refusing replay",error);
 CheckedCommandBorrowV58 actual;if(!checked_data_borrow(actual,error))return fail(error,error);
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy_);
 if(descriptor_.Init==0x455628){initialized_=true;error.clear();return true;} // Original literal BX LR.
 if(!behaviors_.actual_owner||!behaviors_.init)return fail("Required original "+std::string(descriptor_.class_name)+" Init "+std::to_string(descriptor_.Init),error);
 try{if(!behaviors_.init(actual,error))return fail(error,error);if(failed_){error=failure_;return false;}CheckedCommandBorrowV58 after;if(!checked_data_borrow(after,error)||after.actual_data.get()!=actual.actual_data.get())return fail("Main Init changed SAME receiver/data assignment",error);initialized_=true;error.clear();return true;}catch(const std::exception& e){return fail(e.what(),error);}catch(...){return fail("Main original Init threw",error);}
}
bool CanonicalScriptCommandV58::execute(bool skip,std::int32_t context,std::string& error){
 if(released_||!initialized_){error="Actual command Execute requires live initialized receiver";return false;}
 if(busy_){error="Actual command Execute reentry unavailable";return false;}
 CheckedCommandBorrowV58 actual;if(!checked_data_borrow(actual,error))return false;if(!behaviors_.actual_owner||!behaviors_.execute){error="Required Main original "+std::string(descriptor_.class_name)+" Execute(bool,int)";return false;}
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy_);
 return behaviors_.execute(actual,skip,context,error);
}
bool CanonicalScriptCommandV58::release_base_storage(std::string& error){
 if(busy_){error="Actual command base teardown during body unavailable";return false;}if(released_){error="Actual command base storage already released";return false;}
 // Manager executes source BASE data-free/dtor/c-pointer clear FIRST. Do not
 // silently substitute destructor release while a data assignment remains.
 if(data_||data_c_){error="Require original BASE data teardown prefix before command CustomFree";return false;}
 operands_.clear();released_=true;error.clear();return true;
}
bool CanonicalScriptCommandV58::read_operand_word(std::uint32_t o,std::uint8_t n,std::uint32_t& out,std::string& error)const {
 if(released_||!operand_domain(descriptor_,o,n)){error="Invalid actual receiver operand word";return false;}
 for(const auto& p:operands_)if(auto scalar=std::get_if<OperandScalarV58>(&p.second);scalar&&p.first<=o&&o+n<=p.first+scalar->width){out=scalar->bits>>(8*(o-p.first));if(n==1)out&=255;error.clear();return true;}
 error="Original operand unproduced or currently a native pointer";return false;
}
bool CanonicalScriptCommandV58::write_operand_word(std::uint32_t o,std::uint8_t n,std::uint32_t value,std::string& error){
 if(released_||!operand_domain(descriptor_,o,n)){error="Invalid actual receiver operand word producer";return false;}
 for(auto& p:operands_)if(auto scalar=std::get_if<OperandScalarV58>(&p.second);scalar&&p.first<=o&&o+n<=p.first+scalar->width){const auto shift=8*(o-p.first);const auto mask=n==4?0xffffffffu:255u<<shift;scalar->bits=(scalar->bits&~mask)|((value<<shift)&mask);error.clear();return true;}
 operands_[o]=OperandScalarV58{n,n==1?value&255:value};error.clear();return true;
}
bool CanonicalScriptCommandV58::read_operand_pointer(std::uint32_t o,std::uintptr_t& out,std::string& error)const{
 if(released_||!operand_domain(descriptor_,o,4)){error="Invalid actual native pointer operand";return false;}auto p=operands_.find(o);if(p==operands_.end()){error="Native pointer operand not produced";return false;}
 if(auto v=std::get_if<OperandPointerV58>(&p->second)){out=v->value;error.clear();return true;}
 error="Original opaque constructor word has no proven native pointer producer";return false;
}
bool CanonicalScriptCommandV58::write_operand_pointer(std::uint32_t o,std::uintptr_t value,std::shared_ptr<void> owner,std::string& error){
 if(released_||!operand_domain(descriptor_,o,4)||(value&&!owner)){error="Required actual pointer producer/lease for command operand";return false;}
 operands_[o]=OperandPointerV58{value,std::move(owner)};error.clear();return true;
}
bool CanonicalScriptCommandV58::constructor_word(std::uint32_t o,std::uint8_t n,std::uint32_t& out,std::string& error)const{
 if(o==4&&n==1){out=skip4_;error.clear();return true;}if(o==8&&n==4){out=std::uint32_t(kind8_);error.clear();return true;}if(o==12&&n==4&&data_c_==0){out=0;error.clear();return true;}return read_operand_word(o,n,out,error);
}
bool create_script_command_receiver_v58(std::int32_t kind,ScriptCommandBehaviorV58 services,std::shared_ptr<CanonicalScriptCommandV58>& out,std::string& error){
 const auto found=command_c1_descriptors_v58().find(kind);if(found==command_c1_descriptors_v58().end()){error="Required original C1 receiver for unretained command class "+std::to_string(kind);return false;}
 try{std::shared_ptr<CanonicalScriptCommandV58> candidate;switch(kind){
case 0:candidate=std::make_shared<ScriptCommandReceiverV58<0>>(found->second,services);break;
case 1:candidate=std::make_shared<ScriptCommandReceiverV58<1>>(found->second,services);break;
case 2:candidate=std::make_shared<ScriptCommandReceiverV58<2>>(found->second,services);break;
case 3:candidate=std::make_shared<ScriptCommandReceiverV58<3>>(found->second,services);break;
case 5:candidate=std::make_shared<ScriptCommandReceiverV58<5>>(found->second,services);break;
case 6:candidate=std::make_shared<ScriptCommandReceiverV58<6>>(found->second,services);break;
case 8:candidate=std::make_shared<ScriptCommandReceiverV58<8>>(found->second,services);break;
case 10:candidate=std::make_shared<ScriptCommandReceiverV58<10>>(found->second,services);break;
case 12:candidate=std::make_shared<ScriptCommandReceiverV58<12>>(found->second,services);break;
case 13:candidate=std::make_shared<ScriptCommandReceiverV58<13>>(found->second,services);break;
case 14:candidate=std::make_shared<ScriptCommandReceiverV58<14>>(found->second,services);break;
case 15:candidate=std::make_shared<ScriptCommandReceiverV58<15>>(found->second,services);break;
case 16:candidate=std::make_shared<ScriptCommandReceiverV58<16>>(found->second,services);break;
case 17:candidate=std::make_shared<ScriptCommandReceiverV58<17>>(found->second,services);break;
case 19:candidate=std::make_shared<ScriptCommandReceiverV58<19>>(found->second,services);break;
case 20:candidate=std::make_shared<ScriptCommandReceiverV58<20>>(found->second,services);break;
case 21:candidate=std::make_shared<ScriptCommandReceiverV58<21>>(found->second,services);break;
case 22:candidate=std::make_shared<ScriptCommandReceiverV58<22>>(found->second,services);break;
case 23:candidate=std::make_shared<ScriptCommandReceiverV58<23>>(found->second,services);break;
case 24:candidate=std::make_shared<ScriptCommandReceiverV58<24>>(found->second,services);break;
case 25:candidate=std::make_shared<ScriptCommandReceiverV58<25>>(found->second,services);break;
case 26:candidate=std::make_shared<ScriptCommandReceiverV58<26>>(found->second,services);break;
case 27:candidate=std::make_shared<ScriptCommandReceiverV58<27>>(found->second,services);break;
case 29:candidate=std::make_shared<ScriptCommandReceiverV58<29>>(found->second,services);break;
case 30:candidate=std::make_shared<ScriptCommandReceiverV58<30>>(found->second,services);break;
case 31:candidate=std::make_shared<ScriptCommandReceiverV58<31>>(found->second,services);break;
case 32:candidate=std::make_shared<ScriptCommandReceiverV58<32>>(found->second,services);break;
case 39:candidate=std::make_shared<ScriptCommandReceiverV58<39>>(found->second,services);break;
case 40:candidate=std::make_shared<ScriptCommandReceiverV58<40>>(found->second,services);break;
case 41:candidate=std::make_shared<ScriptCommandReceiverV58<41>>(found->second,services);break;
case 42:candidate=std::make_shared<ScriptCommandReceiverV58<42>>(found->second,services);break;
case 43:candidate=std::make_shared<ScriptCommandReceiverV58<43>>(found->second,services);break;
case 44:candidate=std::make_shared<ScriptCommandReceiverV58<44>>(found->second,services);break;
case 45:candidate=std::make_shared<ScriptCommandReceiverV58<45>>(found->second,services);break;
case 46:candidate=std::make_shared<ScriptCommandReceiverV58<46>>(found->second,services);break;
case 51:candidate=std::make_shared<ScriptCommandReceiverV58<51>>(found->second,services);break;
case 52:candidate=std::make_shared<ScriptCommandReceiverV58<52>>(found->second,services);break;
case 54:candidate=std::make_shared<ScriptCommandReceiverV58<54>>(found->second,services);break;
case 55:candidate=std::make_shared<ScriptCommandReceiverV58<55>>(found->second,services);break;
case 63:candidate=std::make_shared<ScriptCommandReceiverV58<63>>(found->second,services);break;
case 68:candidate=std::make_shared<ScriptCommandReceiverV58<68>>(found->second,services);break;
case 69:candidate=std::make_shared<ScriptCommandReceiverV58<69>>(found->second,services);break;
case 70:candidate=std::make_shared<ScriptCommandReceiverV58<70>>(found->second,services);break;
case 77:candidate=std::make_shared<ScriptCommandReceiverV58<77>>(found->second,services);break;
case 78:candidate=std::make_shared<ScriptCommandReceiverV58<78>>(found->second,services);break;
case 79:candidate=std::make_shared<ScriptCommandReceiverV58<79>>(found->second,services);break;
 default:error="Unretained original class C1";return false;}out=std::move(candidate);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
ScriptManagerServicesV52 bind_script_command_factories_v58(ScriptManagerServicesV52 input,ScriptCommandBehaviorV58 services){
 if(input.command_factory)throw std::invalid_argument("Existing real command factory must not be replaced");
 input.command_factory=[services=std::move(services)](std::int32_t kind,ScriptCommandBorrowV52& out,std::string& error){std::shared_ptr<CanonicalScriptCommandV58> actual;if(!create_script_command_receiver_v58(kind,services,actual,error))return false;auto borrow=actual->constructor_borrow();out=std::move(borrow);error.clear();return true;};return input;
}
}
