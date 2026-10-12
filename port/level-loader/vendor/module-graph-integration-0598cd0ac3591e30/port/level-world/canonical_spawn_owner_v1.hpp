#pragma once
#include "canonical_class_receiver_bindings_v1.hpp"
namespace dh2::world {
struct CanonicalSpawnServicesV1 {
 void* context{};
 // Actual catalog constructor. Delivered NULL is the original allocation
 // branch, not a manufactured successful receiver. Preserve failed prefix.
 bool(*construct)(void*,const CanonicalFactoryEntryV1&,CanonicalClassReceiverV1&,std::string&){};
 bool(*unknown_type_debug)(void*,const char*,std::string&){};
 // SAME canonical manager's ObjectHandle::GetObject(bool) implementation.
 // The returned receiver must equal that key's actually registered object.
 bool(*resolve)(void*,target_providers::Handle16&,bool,const CanonicalObjectBorrowV1*&,std::string&){};
 bool(*test_enable_condition)(void*,const CanonicalObjectBorrowV1&,bool,std::string&){};
 bool(*virtual38)(void*,const CanonicalObjectBorrowV1&,bool&,std::string&){};
 // Appends the actual receiver to ObjectManager+34 pending list.
 bool(*append_pending)(void*,const CanonicalObjectBorrowV1&,std::string&){};
 // Borrow actual retained dispatch after duplicate Add returns OLD receiver.
 // May be absent when newly constructed object is actually published.
 bool(*receiver)(void*,const CanonicalObjectBorrowV1&,const CanonicalClassReceiverV1*&,std::string&){};
};
enum class CanonicalSpawnPhaseV1 {empty,constructor,registered,resolve_false,
 properties,defaults,name,archetype,init_post,condition,accepted,pending,complete};
class CanonicalSpawnAttemptV1 {
 CanonicalObjectManagerV1& manager_;CanonicalPropertyMapV1& properties_;
 CanonicalSpawnServicesV1 services_;CanonicalClassReceiverV1 receiver_;
 const CanonicalClassReceiverV1* actual_receiver_{};
 target_providers::Handle16 handle_{0,UINT32_MAX,0};
 CanonicalSpawnPhaseV1 phase_{CanonicalSpawnPhaseV1::empty};bool attempted_{};
 bool resolve(bool,const CanonicalObjectBorrowV1*&,std::string&);
public:
 CanonicalSpawnAttemptV1(CanonicalObjectManagerV1& m,CanonicalPropertyMapV1& p,CanonicalSpawnServicesV1 s):manager_(m),properties_(p),services_(s){}
 // Source Spawn34b724: deferred=false invokes InitPost then genuine
 // TestEnableCondition(true); both paths then call virtual38 and may enqueue.
 bool spawn(const char* type,const char* name,bool deferred,bool network,std::string&);
 CanonicalSpawnPhaseV1 phase()const noexcept{return phase_;}
 const target_providers::Handle16& handle()const noexcept{return handle_;}
 const CanonicalClassReceiverV1& constructed_receiver()const noexcept{return receiver_;}
};
}
