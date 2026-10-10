#include "effects_anchor_binding.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::effects {
bool EffectsAnchorBinding::bind(std::uintptr_t identity,EffectAnchorBorrow borrow,std::string& error) {
    error.clear();
    if(!identity||!borrow.lifetime||!borrow.read) { error="Required SAME source FX anchor identity/lifetime/fields";return false; }
    if(anchors_.count(identity)) { error="Source FX anchor replacement requires release first";return false; }
    anchors_.emplace(identity,std::move(borrow));return true;
}
bool EffectsAnchorBinding::invoke(dh2::fx::MeshFxRequestV1& request,bool& handled,std::string& error) const {
    using O=dh2::fx::MeshFxOperationV1;
    handled=request.operation==O::anchor_dead||request.operation==O::anchor_disabled||
        request.operation==O::anchor_stationary||request.operation==O::anchor_position||
        request.operation==O::anchor_rotation||request.operation==O::anchor_scale;
    if(!handled)return true;
    auto found=anchors_.find(request.identity);
    if(found==anchors_.end()) { error="Missing original FX actor/socket borrower";return false; }
    EffectAnchorSample sample;if(!found->second.read(sample,error))return false;
    if(request.operation==O::anchor_dead)request.result=sample.dead;
    else if(request.operation==O::anchor_disabled)request.result=sample.disabled;
    else if(request.operation==O::anchor_stationary)request.result=sample.stationary;
    else {
        const auto& values=request.operation==O::anchor_position?sample.source_position:
                           request.operation==O::anchor_rotation?sample.source_rotation:sample.source_scale;
        for(auto value:values)if(!std::isfinite(value)) { error="Invalid original FX anchor fields";return false; }
        std::copy(values.begin(),values.end(),request.point);
    }
    return true;
}
void EffectsAnchorBinding::release(std::uintptr_t identity) noexcept {
    if(detach_)detach_(identity);
    anchors_.erase(identity);
}
void EffectsAnchorBinding::clear() noexcept {
    if(detach_)for(const auto& entry:anchors_)detach_(entry.first);
    anchors_.clear();
}
}
