#include "native_character_stop.hpp"

#include "character_physics_position.hpp"
#include "native_body.hpp"
#include "navigation_path.hpp"
#include "Box2D.h"

#include <cstddef>
#include <cstdint>

namespace dh2::native_character_stop {
namespace {

struct Range {
    std::uintptr_t begin;
    std::uintptr_t end;
};

bool range(const void* pointer, std::size_t size, std::size_t alignment,
           Range& result) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size) return false;
    result = {begin, begin + size};
    return true;
}

bool overlaps(Range left, Range right) {
    return left.begin < right.end && right.begin < left.end;
}

struct Context {
    const LiveView* view;
    bool logical_published = false;
};

void copy_back(const LiveView&, const game_object_stop::State&);

void read_back(const LiveView& view, game_object_stop::State& state) {
    for (unsigned i = 0; i < 3; ++i) {
        state.position[i] = view.runtime->subobjects.position[i];
        state.destination[i] = view.runtime->subobjects.destination[i];
        state.heading[i] = view.runtime->subobjects.heading[i];
    }
    state.moving = *view.moving;
    state.heading_active = *view.heading_active;
    state.physical_identity = view.physical_identity;
}

struct ReadAfterService {
    const LiveView& view;
    game_object_stop::State& state;
    // A SetXForm contact observer may reenter or throw after changing the
    // live owner. Preserve those changes on every return and failure path.
    ~ReadAfterService() { read_back(view, state); }
};

bool body_matches(const LiveView& view, std::uintptr_t identity) {
    return view.physical_identity && identity == view.physical_identity &&
           view.native_body &&
           identity == reinterpret_cast<std::uintptr_t>(view.native_body) &&
           view.native_body->body && view.native_body->pinned <= 1;
}

std::int32_t invoke(void* opaque, game_object_stop::State* state,
                    const game_object_stop::Request* request,
                    game_object_stop::Response* response) {
    if (!opaque || !state || !request || !response) return 1;
    auto& context = *static_cast<Context*>(opaque);
    const auto& view = *context.view;
    if (state->object_identity != view.actor_identity ||
        request->object_identity != view.actor_identity)
        return 1;

    // The kernel writes its logical projection after DropPath. Publish those
    // writes before the next provider can observe/reenter the live owner.
    if (request->operation != game_object_stop::Operation::drop_path &&
        !context.logical_published) {
        copy_back(view, *state);
        context.logical_published = true;
    }
    const ReadAfterService read_after{view, *state};

    switch (request->operation) {
    case game_object_stop::Operation::drop_path:
        if (request->subject_identity !=
            reinterpret_cast<std::uintptr_t>(&view.runtime->path))
            return 1;
        return dh2_nav_drop_path(&view.runtime->path);

    case game_object_stop::Operation::is_updating_position_from_physics: {
        if (request->subject_identity != view.actor_identity) return 1;
        const character_physics_position::CharacterView character{
            view.actor_identity, &view.character->state.flags};
        character_physics_position::Result result{};
        if (dh2_character_is_updating_position_from_physics(&character, &result) ||
            result.character_identity != view.actor_identity)
            return 1;
        response->word = result.raw;
        return 0;
    }

    case game_object_stop::Operation::set_linear_velocity:
        if (!body_matches(view, request->subject_identity) ||
            dh2_native_body_set_linear(view.native_body, request->values) ||
            dh2_native_body_refresh_view(&view.runtime->body, view.native_body))
            return 1;
        return 0;

    case game_object_stop::Operation::set_angular_velocity:
        if (!body_matches(view, request->subject_identity) ||
            dh2_native_body_set_angular(view.native_body, request->values) ||
            dh2_native_body_refresh_view(&view.runtime->body, view.native_body))
            return 1;
        return 0;

    case game_object_stop::Operation::set_position: {
        if (!body_matches(view, request->subject_identity)) return 1;
        // The original caller ignores PhysicalObject::setPosition's false
        // SetXForm result (e.g. frozen/out-of-world). The native backend uses
        // -1 for malformed input; 0 and 1 are both ordinary source returns.
        const int moved = dh2_native_body_set_position(view.native_body,
                                                       request->values);
        if (moved < 0 ||
            dh2_native_body_refresh_view(&view.runtime->body,
                                         view.native_body))
            return 1;
        return 0;
    }
    }
    return 1;
}

physical::BodyState* resolve_and_finish(void* opaque,
                                        std::uintptr_t identity) {
    if (!opaque) return nullptr;
    const auto& view = *static_cast<Context*>(opaque)->view;
    if (!body_matches(view, identity)) return nullptr;

    // GameObject::Stop's final inline writes are after the three physical
    // setter calls. In this pinned Box2D backend PutToSleep writes the same
    // sleep/velocity/force/torque fields. The caller then clears its separate
    // logical BodyState projection with dh2_physical_stop_finish.
    view.native_body->body->PutToSleep();
    if (dh2_native_body_refresh_view(&view.runtime->body, view.native_body))
        return nullptr;
    return &view.runtime->body;
}

bool valid_view(const LiveView& view) {
    if (!view.actor_identity || !view.character || !view.runtime ||
        !view.moving || !view.heading_active || *view.moving > 1 ||
        *view.heading_active > 1 || view.character->owner() != view.actor_identity)
        return false;
    if (!view.physical_identity)
        return view.native_body == nullptr;
    return body_matches(view, view.physical_identity) &&
           view.runtime->body.flags <= 0xffffu;
}

void copy_back(const LiveView& view, const game_object_stop::State& state) {
    auto& runtime = *view.runtime;
    for (unsigned i = 0; i < 3; ++i) {
        runtime.subobjects.destination[i] = state.destination[i];
        runtime.subobjects.heading[i] = state.heading[i];
    }
    *view.moving = state.moving;
    *view.heading_active = state.heading_active;
}

}  // namespace

Status stop(const LiveView* view, Result* result) {
    Range view_range{}, result_range{};
    if (!range(view, sizeof(*view), alignof(LiveView), view_range) ||
        !range(result, sizeof(*result), alignof(Result), result_range) ||
        overlaps(view_range, result_range))
        return Status::invalid_view;
    Range runtime_range{}, character_range{}, moving_range{}, heading_range{};
    if (!view->runtime || !view->character || !view->moving ||
        !view->heading_active ||
        !range(view->runtime, sizeof(*view->runtime), alignof(actor::RuntimeState),
               runtime_range) ||
        !range(view->character, sizeof(*view->character), alignof(character::Coordinator),
               character_range) ||
        !range(view->moving, sizeof(*view->moving), alignof(std::uint8_t),
               moving_range) ||
        !range(view->heading_active, sizeof(*view->heading_active),
               alignof(std::uint8_t), heading_range))
        return Status::invalid_view;
    if (overlaps(view_range, runtime_range) ||
        overlaps(view_range, character_range) ||
        overlaps(view_range, moving_range) ||
        overlaps(view_range, heading_range) ||
        overlaps(result_range, runtime_range) ||
        overlaps(result_range, character_range) ||
        overlaps(result_range, moving_range) ||
        overlaps(result_range, heading_range))
        return Status::invalid_view;
    Range native_range{}, body_range{};
    if (view->native_body) {
        if (!range(view->native_body, sizeof(*view->native_body),
                   alignof(physical::NativeBody), native_range))
            return Status::invalid_view;
        if (overlaps(view_range, native_range) ||
            overlaps(result_range, native_range))
            return Status::invalid_view;
        if (view->native_body->body) {
            if (!range(view->native_body->body, sizeof(b2Body), alignof(b2Body),
                       body_range))
                return Status::invalid_view;
            if (overlaps(view_range, body_range) ||
                overlaps(result_range, body_range))
                return Status::invalid_view;
        }
    }
    Range owned_ranges[10]{view_range, result_range, runtime_range,
                           character_range, moving_range, heading_range};
    unsigned owned_count = 6;
    if (view->native_body) {
        owned_ranges[owned_count++] = native_range;
        if (view->native_body->body) {
            owned_ranges[owned_count++] = body_range;
            Range world_range{};
            if (!range(view->native_body->body->GetWorld(), sizeof(b2World),
                       alignof(b2World), world_range))
                return Status::invalid_view;
            owned_ranges[owned_count++] = world_range;
        }
    }
    const auto& path = view->runtime->path;
    if (path.capacity) {
        Range route_range{};
        if (!range(path.segments,
                   std::size_t(path.capacity) * sizeof(navigation::PathSegment),
                   alignof(navigation::PathSegment), route_range))
            return Status::invalid_view;
        owned_ranges[owned_count++] = route_range;
    }
    for (unsigned i = 0; i < owned_count; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlaps(owned_ranges[i], owned_ranges[j]))
                return Status::invalid_view;
    if (!view->actor_identity || *view->moving > 1 || *view->heading_active > 1)
        return Status::invalid_view;
    if (view->character && view->character->owner() != view->actor_identity)
        return Status::owner_mismatch;
    if (!valid_view(*view)) return Status::body_mismatch;

    game_object_stop::State state{};
    state.object_identity = view->actor_identity;
    state.path_identity = reinterpret_cast<std::uintptr_t>(&view->runtime->path);
    state.physical_identity = view->physical_identity;
    for (unsigned i = 0; i < 3; ++i) {
        state.position[i] = view->runtime->subobjects.position[i];
        state.destination[i] = view->runtime->subobjects.destination[i];
        state.heading[i] = view->runtime->subobjects.heading[i];
    }
    state.moving = *view->moving;
    state.heading_active = *view->heading_active;

    Context context{view};
    const game_object_stop::Services services{&context, invoke,
                                               resolve_and_finish};
    game_object_stop::Result source{};
    const auto source_status = game_object_stop::execute(&state, &services,
                                                         &source);
    // The absent-body branch has no observing provider after the logical
    // writes. Other branches already published them before their providers;
    // writing a stale projection here would overwrite reentrant owner edits.
    if (!context.logical_published) copy_back(*view, state);
    if (result) {
        *result = {static_cast<std::int32_t>(source_status), source.phase,
                   source.service_calls, source.last_operation,
                   source.physical_setters};
    }
    return source_status == game_object_stop::Status::complete
        ? Status::complete : Status::source_failure;
}

}  // namespace dh2::native_character_stop

extern "C" int dh2_native_character_stop(
    const dh2::native_character_stop::LiveView* view,
    dh2::native_character_stop::Result* result) {
    return static_cast<int>(dh2::native_character_stop::stop(view, result));
}
