#pragma once
#include "canonical_constructor_prefix_v89.hpp"
#include "canonical_light_point_v53.hpp"
#include <canonical_class_receiver_bindings_v1.hpp>
namespace dh2::loader {
struct CanonicalLightPointRecordV53 {
 CanonicalConstructorStateV89 constructor_state{CanonicalConstructorStateV89::prepared};
 std::unique_ptr<world::CanonicalLightPointV53> owner;
 std::shared_ptr<const void> declaration;
};
struct CanonicalLightPointFactoryInputsV53 {
 std::shared_ptr<void> world;
 std::function<bool(const world::CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalLightPointRecordV53>&,world::LightPointInitServicesV53&,std::string&)> services;
};
// Bind this to the existing remaining canonical factory dispatch before the
// unknown-class failure. SAME catalog factory34115c, no extra registry.
class CanonicalLightPointFactoryV53 final {
 CanonicalLightPointFactoryInputsV53 input_;
 std::vector<std::shared_ptr<CanonicalLightPointRecordV53>> records_;
public:
 explicit CanonicalLightPointFactoryV53(CanonicalLightPointFactoryInputsV53 i):input_(std::move(i)){}
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&);
 const auto& records()const noexcept{return records_;}
 // Call AFTER actual manager unpublication and transport.erased(identity).
 void erased(std::uintptr_t);
};
}
