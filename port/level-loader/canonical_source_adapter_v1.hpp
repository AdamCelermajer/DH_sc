#pragma once
#include "object_entry_v1.hpp"
#include "canonical_object_factory_v1.hpp"
namespace dh2::loader {
// Supplied by the same canonical candidate owner after Module properties load.
// Source occurrence is diagnostic; ID/position are actual class-owned values.
struct CanonicalModuleContextV1 {
    // Pin the canonical context, which must not own this binding/attempt.
    // Candidate aggregation owns both separately to avoid a retention cycle.
    std::shared_ptr<void> candidate_owner;
    std::uint32_t occurrence{UINT32_MAX};
    std::int32_t runtime_module_id{-1};
    std::array<float,3> class_position{};
};
class CanonicalSourceBindingV1 {
    struct State;
    std::shared_ptr<const State> state_;
    friend bool prepare_canonical_source_binding_v1(XmlDocumentV1::Borrow,std::uint32_t,
        ObjectEntryRouteV1,const std::optional<std::string>&,CanonicalModuleContextV1,
        CanonicalSourceBindingV1&,std::string&);
public:
    explicit operator bool()const noexcept{return bool(state_);}
    const ObjectEntryV1& entry()const;
    world::CanonicalSourceObjectRequestV1 request()const;
};
// Takes retained original XML, never a derived map-inspection document.
// Missing and empty attributes stay distinct. Invalid input leaves out intact.
bool prepare_canonical_source_binding_v1(XmlDocumentV1::Borrow,std::uint32_t,
    ObjectEntryRouteV1,const std::optional<std::string>&,CanonicalModuleContextV1,
    CanonicalSourceBindingV1& out,std::string& error);
// Receiver adapter access to the entire original element, including children.
// Returns null for a request not created by this adapter.
const ObjectEntryV1* canonical_source_entry_v1(const world::CanonicalSourceObjectRequestV1&) noexcept;
enum class CanonicalBoundSourceStepV1 {empty,original_source_skip,source_complete,failed};
class CanonicalBoundSourceAttemptV1 {
    CanonicalSourceBindingV1 source_;
    std::unique_ptr<world::CanonicalObjectFactoryAttemptV1> factory_;
    CanonicalBoundSourceStepV1 step_{CanonicalBoundSourceStepV1::empty};
    bool attempted_{};
public:
    explicit CanonicalBoundSourceAttemptV1(CanonicalSourceBindingV1 source):source_(std::move(source)){}
    // Runs the canonical source construction prefix once. No runtime commit,
    // InitFinal, eligibility, rollback, or readiness is implied by source_complete.
    bool execute(world::CanonicalObjectManagerV1&,const world::CanonicalClassServicesV1&,std::string&);
    CanonicalBoundSourceStepV1 step()const noexcept{return step_;}
    const CanonicalSourceBindingV1& source()const noexcept{return source_;}
    // Preserve the main owner's exact handle/prefix on failure for its cleanup.
    // Null means no canonical factory operation was delivered (e.g. Player gate).
    const world::CanonicalObjectFactoryAttemptV1* factory_attempt()const noexcept{return factory_.get();}
};
}
