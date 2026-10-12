#pragma once
#include "condition_list_owner_v55.hpp"
#include <unordered_map>
namespace dh2::world {
struct CanonicalConditionProfileV55 {std::uint32_t factory,vtable,allocation_bytes,eval,deleting_dtor,deleting_vtable;};
struct CanonicalConditionFieldsV55 {
 std::shared_ptr<const void> receiver_alias;
 std::shared_ptr<const void> authority_lease; // temporary live borrow, never stored in receiver
 std::uintptr_t identity{};
 const CanonicalConditionProfileV55* profile{};
 std::uint32_t* source_dispatch0{};
 std::uintptr_t* stub4{};
 std::int32_t* comparator8{}; // ONLY original12-byte quest discriminator classes
};
struct ConditionEvaluationServicesV55 {
 std::shared_ptr<void> owner;
 std::function<bool(const CanonicalConditionFieldsV55&,bool&,std::string&)> invoke_original;
};
class CanonicalConditionReceiverV55 {
 friend class CanonicalConditionFactoryV55;
 std::weak_ptr<const void> world_;
 const CanonicalConditionProfileV55* profile_;
 std::uint32_t dispatch0_;
 std::uintptr_t stub4_{};
 bool retired_{}; // native lifetime guard, not an extra source Condition field
protected:
 CanonicalConditionReceiverV55(std::shared_ptr<const void>,const CanonicalConditionProfileV55&);
 virtual std::int32_t* comparator_cell8()noexcept{return nullptr;}
public:
 virtual ~CanonicalConditionReceiverV55()=default;
};
class CanonicalConditionFactoryV55:public std::enable_shared_from_this<CanonicalConditionFactoryV55> {
 std::weak_ptr<const void> world_;
 std::weak_ptr<void> eval_owner_;
 ConditionEvaluationServicesV55 eval_;
 std::unordered_map<std::uintptr_t,std::weak_ptr<CanonicalConditionReceiverV55>> receivers_;
 bool construct(unsigned,ActualConditionBorrowV55&,std::string&);
 bool deleting_destructor(const ActualConditionBorrowV55&,std::string&);
public:
 explicit CanonicalConditionFactoryV55(std::shared_ptr<const void>,ConditionEvaluationServicesV55={});
 ActualConditionFactoriesV55 factories();
 bool borrow(std::uintptr_t,CanonicalConditionFieldsV55&,std::string&)const;
 bool evaluate(std::uintptr_t,bool&,std::string&)const;
 static const CanonicalConditionProfileV55* source_profile(unsigned)noexcept;
};
}
