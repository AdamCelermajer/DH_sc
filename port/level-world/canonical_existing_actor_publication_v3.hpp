#pragma once
#include "canonical_object_manager_v1.hpp"
#include <map>
namespace dh2::world {
// Development adoption of already initialized canonical receivers. This is
// deliberately not ObjectFactory construction or InitProperties delivery.
struct CanonicalExistingActorV3 {
 CanonicalObjectBorrowV1 actor;
 const std::string* source_name{};
 const std::string* source_archetype{};
};
class CanonicalExistingActorPublicationV3 {
 CanonicalObjectManagerV1& manager_;
 struct Attempt { target_providers::Handle16 handle{}; bool complete{}; };
 std::map<std::uintptr_t,Attempt> attempts_;
public:
 explicit CanonicalExistingActorPublicationV3(CanonicalObjectManagerV1& manager):manager_(manager){}
 bool publish(const CanonicalExistingActorV3&,const char* name,const char* archetype,
              std::int32_t room,bool actual_network_policy,
              target_providers::Handle16&,std::string&);
 bool attempted(std::uintptr_t identity)const noexcept{return attempts_.count(identity)!=0;}
 bool completed_v4(std::uintptr_t identity)const noexcept{auto i=attempts_.find(identity);return i!=attempts_.end()&&i->second.complete;}
};
}
