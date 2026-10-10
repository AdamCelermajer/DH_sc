#pragma once
#include "encounter_services.hpp"
#include "../../../level-world/canonical_class_receiver_bindings_v1.hpp"
namespace dh::foundation::encounters {
struct TriggerRecord {
 dh2::actor::RuntimeState runtime;
 dh2::world::CanonicalSourceObjectRequestV1 declaration;
 std::shared_ptr<dh2::world::CanonicalTriggerZoneV22> receiver;
 dh2::world::CanonicalClassReceiverV1 transport;
};
struct TriggerConstructorServices {
 std::shared_ptr<void> world_owner,previous_owner;
 std::shared_ptr<dh2::world::CanonicalPropertyMapV1> properties;
 dh2::world::CanonicalClassServicesV1 previous;
 // Runs before actual ctor; callbacks must weakly borrow stable record.
 std::function<bool(const std::shared_ptr<TriggerRecord>&,
   dh2::world::TriggerZoneServicesV22&,std::string&)> source_services;
};
// Actual canonical Trigger constructor/property transport for original MGP/MVP
// declarations. Other classes delegate to supplied real source constructors.
class TriggerConstructors {
public:
 explicit TriggerConstructors(TriggerConstructorServices);
 dh2::world::CanonicalClassServicesV1 services()const;
 std::shared_ptr<void> lease()const;
 bool record(std::uintptr_t,std::shared_ptr<TriggerRecord>&,std::string&)const;
private:
 struct Impl;std::shared_ptr<Impl> impl_;
};
}
