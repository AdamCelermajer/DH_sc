#pragma once
#include "lifecycle_v36.hpp"
#include <type_traits>
#include <utility>
namespace dh2::loader {
// Pins and aliases the SAME retained CanonicalLevelContext's completed C1
// scalar fields. Contains no scalar copies, second state or publication slot.
struct LifecycleBorrowV36 {
 std::shared_ptr<void> actual_level_owner;
 std::uintptr_t identity{};
 LifecycleFieldsV36 fields;
};
// Context is the actual selected CanonicalLevelContextV1. Template defers
// instantiation until the reviewed genuine134/138 scalar header is selected.
// A uintptr_t legacy field intentionally fails compilation; never reinterpret.
template<class Context>
bool borrow_lifecycle_fields_v36(const std::shared_ptr<Context>& receiver,
 LifecycleBorrowV36& out,std::string& error) {
 auto reject=[&](const char* reason){error=reason;return false;};
 if(!receiver)return reject("Required actual retained Level receiver");
 const auto* constructor=receiver->constructor_owner_v3();
 if(!constructor)return reject("Required actual completed Level C1 owner");
 using Phase=decltype(constructor->phase());
 if(constructor->phase()!=Phase::complete)return reject("Loading counter borrow requires completed actual Level C1");
 typename Context::LoadingFieldsV26 loading;
 if(!receiver->loading_fields_v26(loading,error))return false;
 auto constructor_borrow=receiver->constructor_borrow_v3();
 if(!constructor_borrow.fields)return reject("Required actual Level C1 fields");
 auto& fields=*constructor_borrow.fields;
 static_assert(std::is_same_v<decltype(fields.phase30),std::uint32_t>);
 static_assert(std::is_same_v<decltype(fields.field130),std::uint32_t>);
 static_assert(std::is_same_v<decltype(fields.field134),std::uint32_t>,"Require original-derived32-bit scalar field134");
 static_assert(std::is_same_v<decltype(fields.field138),std::uint32_t>,"Require original-derived32-bit scalar field138");
 auto same_owner=[&](const std::shared_ptr<void>& owner){return owner&&owner.get()==receiver.get()&&!owner.owner_before(receiver)&&!receiver.owner_before(owner);};
 if(!same_owner(loading.level_owner)||!same_owner(constructor_borrow.owner)||
    loading.identity!=receiver->identity()||constructor_borrow.identity!=receiver->identity())
  return reject("Loading and constructor borrows must pin the same actual Level identity/owner");
 if(loading.progress30!=&fields.phase30||loading.state130!=&fields.field130)
  return reject("Loading progress/state must alias the same actual C1 storage");
 LifecycleBorrowV36 candidate{std::move(loading.level_owner),loading.identity,
  {&fields.phase30,&fields.field130,&fields.field134,&fields.field138}};
 out=std::move(candidate);error.clear();return true;
}
}
