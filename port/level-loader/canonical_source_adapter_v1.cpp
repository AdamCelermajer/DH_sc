#include "canonical_source_adapter_v1.hpp"
#include <cmath>
#include <stdexcept>
namespace dh2::loader {
namespace {
struct SourceState {
    ObjectEntryV1 entry;
    std::shared_ptr<void> candidate_owner;
    std::optional<std::string> filter;
    world::CanonicalSourceObjectRequestV1 request;
};
const char* attribute(void* context,std::uint32_t element,const char* name) {
    const auto* state=static_cast<const SourceState*>(context);
    if(!state||!name||element!=state->entry.element)return nullptr;
    const auto* value=state->entry.source().attribute(name);
    return value?value->c_str():nullptr;
}
}
struct CanonicalSourceBindingV1::State {
    std::shared_ptr<const SourceState> source;
};
const ObjectEntryV1& CanonicalSourceBindingV1::entry()const {
    if(!state_)throw std::logic_error("Canonical source binding unavailable");
    return state_->source->entry;
}
world::CanonicalSourceObjectRequestV1 CanonicalSourceBindingV1::request()const {
    if(!state_)throw std::logic_error("Canonical source binding unavailable");
    auto request=state_->source->request;request.source_lease=state_->source;return request;
}
bool prepare_canonical_source_binding_v1(XmlDocumentV1::Borrow document,std::uint32_t element,
    ObjectEntryRouteV1 route,const std::optional<std::string>& filter,CanonicalModuleContextV1 module,
    CanonicalSourceBindingV1& out,std::string& error) {
    error.clear();
    if(!module.candidate_owner){error="Canonical candidate owner lease required";return false;}
    for(float v:module.class_position)if(!std::isfinite(v)){error="Canonical module position must be finite";return false;}
    if(module.occurrence==UINT32_MAX&&(module.runtime_module_id!=-1||module.class_position!=std::array<float,3>{})) {
        error="Level source requires reset module context (offset zero, ID -1)";return false;
    }
    auto next=std::make_shared<SourceState>();
    if(!prepare_object_entry_v1(std::move(document),element,route,filter,next->entry,error))return false;
    next->candidate_owner=std::move(module.candidate_owner);next->filter=filter;
    // This request is returned separately below: its lease owns SourceState,
    // while State owns both. SourceState must not own a lease to itself.
    next->request.source_context=next.get();next->request.attribute=&attribute;
    next->request.element=element;next->request.module_occurrence=module.occurrence;
    next->request.runtime_module_id=module.runtime_module_id;next->request.module_offset=module.class_position;
    next->request.type_filter=next->filter?next->filter->c_str():nullptr;
    auto holder=std::make_shared<CanonicalSourceBindingV1::State>();holder->source=next;
    out.state_=std::move(holder);return true;
}
const ObjectEntryV1* canonical_source_entry_v1(const world::CanonicalSourceObjectRequestV1& request) noexcept {
    if(request.attribute!=&attribute||!request.source_lease||request.source_lease.get()!=request.source_context)return nullptr;
    const auto* state=static_cast<const SourceState*>(request.source_context);
    return request.element==state->entry.element?&state->entry:nullptr;
}
bool CanonicalBoundSourceAttemptV1::execute(world::CanonicalObjectManagerV1& manager,
    const world::CanonicalClassServicesV1& services,std::string& error) {
    error.clear();if(attempted_){error="Canonical bound source already attempted";return false;}attempted_=true;
    if(!source_){step_=CanonicalBoundSourceStepV1::failed;error="Canonical source binding unavailable";return false;}
    const auto disposition=source_.entry().disposition;
    if(disposition==ObjectEntryDispositionV1::unsafe_level_missing_type){step_=CanonicalBoundSourceStepV1::failed;error="Original Level null gametype branch outside safe adapter domain";return false;}
    if(disposition==ObjectEntryDispositionV1::original_player_exclusion){step_=CanonicalBoundSourceStepV1::original_source_skip;return true;}
    auto request=source_.request();
    factory_=std::make_unique<world::CanonicalObjectFactoryAttemptV1>(std::move(request));
    if(!factory_->execute(manager,services,error)){step_=CanonicalBoundSourceStepV1::failed;return false;}
    step_=factory_->stage()==world::CanonicalFactoryStageV1::source_skip?CanonicalBoundSourceStepV1::original_source_skip:CanonicalBoundSourceStepV1::source_complete;
    return true;
}
}
