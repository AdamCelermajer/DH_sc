#pragma once
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
namespace dh2::world {
struct DummyContinuationServicesV14 {
 std::shared_ptr<void> owner;
 // Whole actual GameObject base destruction on this SAME scene/PF graph.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_base;
};
// Factory3410a4 calls actual GameObjectC1(GO_ID20), then static84=1. Dummy
// has NO extra source fields and inherits generic declaration/Init methods.
class CanonicalDummyOwnerV14 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 services_;
 GameObjectInitializationOwnerV1 initialization_;
 DummyContinuationServicesV14 continuation_;
 bool destroyed_{};
public:
 CanonicalDummyOwnerV14(std::shared_ptr<void> actual_world_pin,
  actor::RuntimeState& same_runtime,GameObjectInitializationServicesV1,
  DummyContinuationServicesV14);
 CanonicalDummyOwnerV14(const CanonicalDummyOwnerV14&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept{return base_.properties();}
 bool init_post(bool& eligible,std::string& e){return initialization_.init_post(eligible,e);}
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static constexpr bool is_updatable()noexcept{return false;}
 static constexpr bool is_zonable()noexcept{return false;}
 static constexpr bool is_animated()noexcept{return false;}
 static constexpr bool is_interactive()noexcept{return false;}
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalDummyOwnerV14>,
                                                std::shared_ptr<const void> source_xml_lease);
};
}
