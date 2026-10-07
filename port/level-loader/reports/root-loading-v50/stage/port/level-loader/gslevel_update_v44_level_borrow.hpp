#pragma once
#include "gslevel_update_v44.hpp"
#include "lifecycle_v36_counter_borrow.hpp"
namespace dh2::loader {
// Production body providers over completed actual C1. The generic test fixture
// does not stand in for this validated pointer/ownership binding.
template<class Context> bool borrow_gslevel_read_v44(const std::shared_ptr<Context>& level,GSLevelReadBorrowV44<Context>& out,std::string& error){
 LifecycleBorrowV36 loading;if(!borrow_lifecycle_fields_v36(level,loading,error))return false;
 const auto c1=level->constructor_borrow_v3();if(!c1.fields||!c1.owner){error="Require actual Level C1 byte145";return false;}
 static_assert(std::is_same_v<decltype(c1.fields->byte145),std::uint8_t>,"Require actual C1 byte145 storage");
 if(c1.owner.get()!=loading.actual_level_owner.get()||c1.identity!=loading.identity||c1.owner.owner_before(loading.actual_level_owner)||loading.actual_level_owner.owner_before(c1.owner)){error="GS read borrow must use SAME C1 receiver";return false;}
 out={level,loading.identity,loading.fields.state130,&c1.fields->byte145};error.clear();return true;
}
template<class Context> bool actual_level_load_v44(const std::shared_ptr<Context>& level,std::string& error){
 LifecycleBorrowV36 actual;if(!borrow_lifecycle_fields_v36(level,actual,error))return false;
 *actual.fields.state130=0;error.clear();return true; // Exact3ef21c; no Init/ready flag.
}
}
