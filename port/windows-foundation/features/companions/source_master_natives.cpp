#include "source_master_natives.hpp"
#include <algorithm>
#include <cstring>
namespace dh::foundation::companions {
namespace {
int call(void* raw,const dh2_script_value* args,std::uint32_t count,
    dh2_script_value* values,std::uint32_t capacity,std::uint32_t* returned,
    char* message,std::size_t message_capacity,unsigned operation) {
    std::string error;
    auto fail=[&] {if(message&&message_capacity){const auto n=std::min(error.size(),message_capacity-1);std::memcpy(message,error.data(),n);message[n]=0;}return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;};
    try {
        auto* owner=static_cast<SourceMasterNativeOwner*>(raw);
        if(!owner||!owner->lease||!owner->borrow||!returned){error="Required SAME original companion native lease/borrow";return fail();}
        SourceMasterBorrow borrow;if(!owner->borrow(borrow,error))return fail();
        *returned=0;
        if(operation==2){if(!source_set_master_values(borrow,args,count,error))return fail();return 0;}
        if(!values||!capacity){error="Missing original companion predicate result storage";return fail();}
        bool result{};
        if(!(operation==0?source_has_master(borrow,result,error):source_is_master_host(borrow,result,error)))return fail();
        values[0]={};values[0].type=DH2_SCRIPT_BOOLEAN;values[0].boolean=result;*returned=1;return 0;
    }catch(const std::exception& e){error=e.what();return fail();}
}
int has(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* r,std::uint32_t c,std::uint32_t* o,char* e,std::size_t z){return call(p,a,n,r,c,o,e,z,0);}
int host(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* r,std::uint32_t c,std::uint32_t* o,char* e,std::size_t z){return call(p,a,n,r,c,o,e,z,1);}
int set(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* r,std::uint32_t c,std::uint32_t* o,char* e,std::size_t z){return call(p,a,n,r,c,o,e,z,2);}
}
int select_source_master_native(void* owner,std::uint32_t source_callback,dh2_script_function* out,void** context) {
    dh2_script_function selected{};
    switch(source_callback){case 0x3b6f3c:selected=has;break;case 0x3b6fc4:selected=host;break;case 0x3b9084:selected=set;break;default:return 0;}
    if(!owner||!out||!context)return -1;
    auto& receiver=*static_cast<SourceMasterNativeOwner*>(owner);
    if(!receiver.lease||!receiver.borrow)return -1;
    *out=selected;*context=owner;return 1;
}
}
