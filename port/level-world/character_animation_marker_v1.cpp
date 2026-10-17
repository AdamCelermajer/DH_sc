#include "character_animation_marker_v1.hpp"

namespace dh2::character_animation_marker_v1 {
namespace {
bool invoke_named_set(ProviderResult (*callback)(void*, std::uintptr_t,
        std::string_view, std::string&), const Services& services,
        const Request& request, std::string_view name, Status& status,
        std::string& error) {
    if (!callback) {
        error = "Animation marker has no bound source-table lookup provider";
        status = Status::missing_service;
        return false;
    }
    const auto outcome = callback(services.context, request.character, name, error);
    if (outcome == ProviderResult::failed) {
        if (error.empty()) error = "Mapped animation marker backend failed";
        status = Status::dispatch_failed;
        return false;
    }
    // `source_no_match` is the source's return-1 no-op for an absent table name.
    status = Status::complete;
    return true;
}

bool invoke_object_animation(ProviderResult (*callback)(void*, std::uintptr_t,
        std::string_view, std::string_view, std::string&),
        const Services& services, const Request& request,
        std::string_view target, std::string_view animation, Status& status,
        std::string& error) {
    if (!callback) {
        error = "CharAI an_ marker has no source camera/ObjectManager provider";
        status = Status::missing_service;
        return false;
    }
    if (callback(services.context, request.character, target, animation, error) ==
        ProviderResult::failed) {
        if (error.empty()) error = "CharAI object animation provider failed";
        status = Status::dispatch_failed;
        return false;
    }
    status = Status::complete;
    return true;
}
}

Result dispatch(const Request& request, const Services& services,
                std::string& error) {
    error.clear();
    Result result{};
    if (!request.character || request.marker.empty() || request.source_step < 0) {
        error = "Animation marker request has no Character, marker, or source step";
        result.status = Status::invalid_request;
        return result;
    }
    const auto marker = request.marker;
    const auto starts = [&](std::string_view prefix) {
        return marker.size() > prefix.size() && marker.substr(0, prefix.size()) == prefix;
    };
    if (starts("ev_")) {
        result.kind = Kind::script;
        const auto name = marker.substr(3);
        if (!services.script_event) {
            error = "CharAI ev_ marker has no same-Record AIS callback provider";
            result.status = Status::missing_service;
            return result;
        }
        if (!services.script_event(services.context, request.character, name,
                                   request.source_step, error)) {
            if (error.empty()) error = "CharAI AISDefault callback rejected the marker";
            result.status = Status::dispatch_failed;
            return result;
        }
        if (name == "step_left" || name == "step_right") {
            result.kind = Kind::footstep;
            if (!services.footstep) {
                error = "AISDefault footstep marker has no source footprint provider";
                result.status = Status::missing_service;
                return result;
            }
            // Source AISDefault's branch is intentionally crossed: step_right
            // requests GetLeftFootPosition, and step_left requests right.
            const auto source_foot = name == "step_right" ? Foot::left : Foot::right;
            if (!services.footstep(services.context, request.character,
                                   source_foot, error)) {
                if (error.empty()) error = "AISDefault footstep provider rejected the marker";
                result.status = Status::dispatch_failed;
                return result;
            }
        }
        result.status = Status::complete;
        return result;
    }
    if (starts("an_")) {
        result.kind = Kind::object_animation;
        const auto value = marker.substr(3);
        const auto slash = value.find('/');
        if (slash == std::string_view::npos || slash == 0 || slash + 1 >= value.size()) {
            // _ParseObjectAnimEvent returns without side effects when there is
            // no slash; CharAI::_OnAnimEvent still returns source-success.
            result.kind = Kind::ignored;
            result.status = Status::complete;
            return result;
        }
        (void)invoke_object_animation(services.object_animation, services,
            request, value.substr(0, slash), value.substr(slash + 1),
            result.status, error);
        return result;
    }
    if (starts("fx_")) {
        result.kind = Kind::effect_set;
        (void)invoke_named_set(services.effect_set, services, request,
                               marker.substr(3), result.status, error);
        return result;
    }
    if (starts("sfx_")) {
        result.kind = Kind::sound;
        (void)invoke_named_set(services.spatial_sound, services, request,
                               marker.substr(4), result.status, error);
        return result;
    }
    // CharAI::_OnAnimEvent returns 1 for unmatched names. State-dependent
    // combat markers are outside this CharacterVisual event service, so they
    // remain successful no-ops here rather than being routed elsewhere.
    result.kind = Kind::ignored;
    result.status = Status::complete;
    return result;
}

} // namespace dh2::character_animation_marker_v1
