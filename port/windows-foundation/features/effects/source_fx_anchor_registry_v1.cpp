#include "source_fx_anchor_registry_v1.hpp"

#include <algorithm>
#include <limits>

namespace dh::foundation::effects {
namespace {
using Op = dh2::fx::MeshFxOperationV1;

bool anchor_operation(Op operation) {
    switch (operation) {
    case Op::anchor_dead:
    case Op::anchor_disabled:
    case Op::anchor_stationary:
    case Op::anchor_position:
    case Op::anchor_rotation:
    case Op::anchor_scale:
        return true;
    default:
        return false;
    }
}
}

bool SourceFxAnchorRegistryV1::is_level_object_identity(std::uintptr_t identity) noexcept {
    return identity != 0 && (identity & kLevelObjectTag) != 0;
}

bool SourceFxAnchorRegistryV1::register_actor(const std::string& source_name,
        std::int32_t module, ActorId actor, std::string& error) {
    error.clear();
    if (source_name.empty()) { error = "Source FX actor name is empty"; return false; }
    if (generation_exhausted_) { error = "Source FX world generation exhausted"; return false; }
    if (actor == invalid_actor_id || actor == 0 ||
        static_cast<std::uintptr_t>(actor) != actor ||
        is_level_object_identity(static_cast<std::uintptr_t>(actor))) {
        error = "Source FX actor requires a nonzero, untagged session ActorId";
        return false;
    }
    const NameKey name{module, source_name};
    if (names_.find(name) != names_.end()) {
        error = "Duplicate source FX anchor name/module: " + source_name;
        return false;
    }
    SourceFxAnchorKeyV1 key;
    key.kind = SourceFxAnchorKindV1::actor;
    key.generation = generation_;
    key.manager_identity = static_cast<std::uintptr_t>(actor);
    key.module = module;
    key.source_name = source_name;
    names_.emplace(name, std::move(key));
    return true;
}

bool SourceFxAnchorRegistryV1::register_object(const std::string& source_name,
        std::int32_t module, std::uint64_t source_object_identity,
        SourceFxAnchorProviderV1 provider,
        SourceFxAnchorKeyV1& key, std::string& error) {
    key = {};
    error.clear();
    if (source_name.empty()) { error = "Source FX object name is empty"; return false; }
    if (!source_object_identity) { error = "Source FX object requires its active source-world identity"; return false; }
    if (!provider) { error = "Source FX object requires a live/authored transform provider"; return false; }
    if (generation_exhausted_) { error = "Source FX world generation exhausted"; return false; }
    const NameKey name{module, source_name};
    if (names_.find(name) != names_.end()) {
        error = "Duplicate source FX anchor name/module: " + source_name;
        return false;
    }
    const auto max_token = kLevelObjectTag - 1;
    if (next_object_token_ == 0 || next_object_token_ > max_token) {
        error = "Source FX object identity space exhausted";
        return false;
    }
    const std::uintptr_t identity = kLevelObjectTag | next_object_token_++;
    key.kind = SourceFxAnchorKindV1::level_object;
    key.generation = generation_;
    key.source_object_identity = source_object_identity;
    key.manager_identity = identity;
    key.module = module;
    key.source_name = source_name;
    Entry entry{key, std::move(provider)};
    objects_.emplace(identity, std::move(entry));
    names_.emplace(name, key);
    return true;
}

bool SourceFxAnchorRegistryV1::resolve(const std::string& source_name,
        std::int32_t module, SourceFxAnchorKeyV1& key, bool& found,
        std::string& error) const {
    error.clear();
    found = false;
    key = {};
    const auto it = names_.find(NameKey{module, source_name});
    if (it == names_.end()) return true;
    if (it->second.generation != generation_) {
        error = "Stale source FX anchor registration: " + source_name;
        return false;
    }
    key = it->second;
    found = true;
    return true;
}

bool SourceFxAnchorRegistryV1::current(const SourceFxAnchorKeyV1& key) const noexcept {
    if (key.generation != generation_ || !key.manager_identity) return false;
    if (key.kind == SourceFxAnchorKindV1::actor) {
        const auto found = names_.find(NameKey{key.module, key.source_name});
        return found != names_.end() && found->second.kind == SourceFxAnchorKindV1::actor &&
               found->second.manager_identity == key.manager_identity;
    }
    const auto found = objects_.find(key.manager_identity);
    return found != objects_.end() && found->second.key.generation == key.generation &&
           found->second.key.source_object_identity == key.source_object_identity &&
           found->second.key.source_name == key.source_name &&
           found->second.key.module == key.module;
}

bool SourceFxAnchorRegistryV1::query(dh2::fx::MeshFxRequestV1& request,
        bool& handled, std::string& error) const {
    handled = is_level_object_identity(request.identity);
    error.clear();
    if (!handled) return true;
    const auto it = objects_.find(request.identity);
    if (it == objects_.end() || it->second.key.generation != generation_) {
        error = "Expired or unregistered source level-object FX anchor identity";
        return false;
    }
    if (!anchor_operation(request.operation)) {
        error = "Unsupported operation for source level-object FX anchor";
        return false;
    }
    SourceFxAnchorPoseV1 pose;
    if (!it->second.provider(pose, error)) {
        if (error.empty()) error = "Source level-object FX anchor provider failed";
        return false;
    }
    switch (request.operation) {
    case Op::anchor_dead: request.result = pose.alive ? 0u : 1u; break;
    case Op::anchor_disabled: request.result = pose.disabled ? 1u : 0u; break;
    case Op::anchor_stationary: request.result = pose.stationary ? 1u : 0u; break;
    case Op::anchor_position:
        std::copy(pose.position.begin(), pose.position.end(), request.point); break;
    case Op::anchor_rotation:
        std::copy(pose.rotation.begin(), pose.rotation.end(), request.point); break;
    case Op::anchor_scale:
        std::copy(pose.scale.begin(), pose.scale.end(), request.point); break;
    default: break;
    }
    error.clear();
    return true;
}

void SourceFxAnchorRegistryV1::clear_for_new_world() noexcept {
    names_.clear();
    objects_.clear();
    if (generation_ == std::numeric_limits<std::uint64_t>::max())
        generation_exhausted_ = true;
    else
        ++generation_;
}

} // namespace dh::foundation::effects
