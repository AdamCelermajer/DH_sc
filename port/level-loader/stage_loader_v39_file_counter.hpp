#pragma once
#include "lifecycle_v36_counter_borrow.hpp"
namespace dh2::loader {
struct FileCounterBorrowV39 {
 std::shared_ptr<void> actual_level_owner;
 std::uintptr_t identity{};
 std::uint32_t* state130{};std::uint32_t* file13c{};
};
template<class Context> bool borrow_file_counter_v39(const std::shared_ptr<Context>& level,FileCounterBorrowV39& out,std::string& error){
 LifecycleBorrowV36 actual;if(!borrow_lifecycle_fields_v36(level,actual,error))return false;
 auto c1=level->constructor_borrow_v3();if(!c1.fields||!c1.owner){error="Required actual Level C1 file counter";return false;}
 static_assert(std::is_same_v<decltype(c1.fields->field13c),std::uint32_t>,"Require recovered actual32-bit scalar13c; never reinterpret legacy uintptr_t");
 if(c1.owner.get()!=actual.actual_level_owner.get()||c1.identity!=actual.identity||c1.owner.owner_before(actual.actual_level_owner)||actual.actual_level_owner.owner_before(c1.owner)){error="File13c borrow must use same actual completed C1 owner";return false;}
 FileCounterBorrowV39 next{std::move(actual.actual_level_owner),actual.identity,actual.fields.state130,&c1.fields->field13c};out=std::move(next);error.clear();return true;
}
inline std::function<bool(std::string&)> actual_file_counter_increment_v39(FileCounterBorrowV39 actual){
 return [actual=std::move(actual),done=false,failed=false](std::string& error)mutable{
  if(done){error.clear();return true;}if(failed){error="Actual file counter producer already failed";return false;}
  if(!actual.actual_level_owner||!actual.state130||!actual.file13c||*actual.state130!=8){failed=true;error="Require same actual Level state130=8 before original file13c ADD32";return false;}
  ++*actual.file13c;done=true;error.clear();return true; // defined uint32 wrap
 };
}
}
