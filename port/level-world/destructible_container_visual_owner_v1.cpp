#include "destructible_container_visual_owner_v1.hpp"

#include "../engine-resources/resources.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

namespace dh2::destructible_container_visual_owner_v1 {
namespace {

std::uint32_t word(const std::uint8_t* p) noexcept {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
        (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}

bool clip_catalog(const std::uint8_t* bdae, std::size_t size,
                  std::vector<Clip>& out, std::string& error) {
    resources::BresView view{};
    if (dh2_bres_open(&view, bdae, size) != resources::BresError::ok) {
        error = "DestructibleContainer BDAE clip catalog is malformed";
        return false;
    }
    const auto count = dh2_bres_library_count(
        &view, resources::Library::animation_clip);
    if (!count || count > 4096) {
        error = "DestructibleContainer BDAE has no bounded animation clip catalog";
        return false;
    }
    std::vector<Clip> clips;
    clips.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto* row = dh2_bres_library_item(
            &view, resources::Library::animation_clip,
            static_cast<std::int32_t>(i));
        if (!row || size < 12) {
            error = "DestructibleContainer animation clip row is absent";
            return false;
        }
        const auto name_offset = word(row);
        if (name_offset >= size) {
            error = "DestructibleContainer animation clip name is outside the BDAE";
            return false;
        }
        const auto* name_end = static_cast<const std::uint8_t*>(
            std::memchr(bdae + name_offset, 0, size - name_offset));
        if (!name_end || name_end == bdae + name_offset ||
            name_end - (bdae + name_offset) > 255) {
            error = "DestructibleContainer animation clip name is invalid";
            return false;
        }
        Clip clip{};
        clip.name.assign(reinterpret_cast<const char*>(bdae + name_offset),
                         static_cast<std::size_t>(name_end - (bdae + name_offset)));
        clip.start_ms = static_cast<std::int32_t>(word(row + 4));
        clip.end_ms = static_cast<std::int32_t>(word(row + 8));
        if (clip.end_ms <= clip.start_ms ||
            std::any_of(clips.begin(), clips.end(), [&](const Clip& prior) {
                return prior.name == clip.name;
            })) {
            error = "DestructibleContainer animation clip interval/name is invalid";
            return false;
        }
        clips.push_back(std::move(clip));
    }
    out = std::move(clips);
    error.clear();
    return true;
}

} // namespace

Owner::Owner() noexcept {
    timeline_.scale = 1.f;
    timeline_.library_present = 1;
    timeline_.clip_index = -1;
}

const Clip* Owner::find_clip(const std::string& name) const noexcept {
    const auto found = std::find_if(clips_.begin(), clips_.end(),
        [&](const Clip& clip) { return clip.name == name; });
    return found == clips_.end() ? nullptr : &*found;
}

bool Owner::load(const std::uint8_t* bdae, std::size_t size,
                 std::string& error) noexcept {
    error.clear();
    if (!bdae || !size || size > 32u * 1024u * 1024u) {
        error = "DestructibleContainer BDAE visual bytes are invalid";
        return false;
    }
    try {
        std::vector<Clip> next_clips;
        if (!clip_catalog(bdae, size, next_clips, error)) return false;
        objects::Resource next_resource;
        if (!objects::load_resource(bdae, size, bdae, size,
                                    next_resource, error)) {
            if (error.empty()) error = "DestructibleContainer BDAE scene/animator load failed";
            return false;
        }
        resource_ = std::move(next_resource);
        clips_ = std::move(next_clips);
        timeline_ = {};
        timeline_.scale = 1.f;
        timeline_.library_present = 1;
        timeline_.clip_index = -1;
        event_cursor_ = {};
        callback_context_ = nullptr;
        event_callback_ = nullptr;
        completion_callback_ = nullptr;
        active_clip_.clear();
        looping_ = false;
        completion_sent_ = false;
        dispatch_ok_ = true;
        dispatch_error_.clear();
        // SceneManager::LoadScene calls constructAnimator once on the root;
        // the loaded scene resource is the retained result of that path.
        controller_present_ = !resource_.scene.graph.empty() &&
                              !resource_.primitives.empty();
        loaded_ = controller_present_;
        if (!loaded_) {
            error = "DestructibleContainer BDAE did not produce a retained root Animator";
            return false;
        }
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    } catch (...) {
        error = "DestructibleContainer BDAE visual owner failed";
        return false;
    }
}

bool Owner::start_initial_state(std::string& error) noexcept {
    return start_initial_state(nullptr, nullptr, nullptr, error);
}

bool Owner::start_initial_state(EventCallback event,
                                CompletionCallback completion, void* context,
                                std::string& error) noexcept {
    if (!loaded_) {
        error = "DestructibleContainer root Animator is not loaded";
        return false;
    }
    const Clip* clip = find_clip("prespawn");
    if (!clip) clip = find_clip("spawn");
    if (!clip) clip = find_clip("idle");
    if (!clip) {
        error = "DestructibleContainer BDAE lacks source prespawn, spawn, or idle clip";
        return false;
    }
    if (!play_named(clip->name.c_str(), clip->name != "prespawn" &&
            clip->name != "spawn", event, completion, context, error))
        return false;
    const std::uint8_t state = clip->name == "prespawn" ? 0u :
        clip->name == "spawn" ? 1u : 2u;
    return set_container_state(state, error);
}

bool Owner::set_container_state(std::uint8_t state,
                               std::string& error) noexcept {
    error.clear();
    if (!loaded_ || resource_.scene.graph.empty() || state > 4) {
        error = "DestructibleContainer SetState has no retained root node or valid state";
        return false;
    }
    auto& flags = resource_.scene.graph.front().source_flags_284;
    if (state == 3) flags &= ~std::uint32_t{0x400};
    else flags |= std::uint32_t{0x400};
    return true;
}

bool Owner::set_current_frame(std::int32_t frame, std::string& error) noexcept {
    error.clear();
    if (!loaded_ || active_clip_.empty() || frame < 0) {
        error = "DestructibleContainer SetCurrentFrame has no active visual or a negative frame";
        return false;
    }
    // The source event/timeline format uses 30 Hz frame timestamps; preserve
    // its recovered float constant and truncating int conversion.
    volatile float milliseconds = static_cast<float>(frame) *
                                  33.333332061767578125f;
    if (milliseconds >= 2147483648.f) {
        error = "DestructibleContainer SetCurrentFrame exceeds source time range";
        return false;
    }
    const auto at = static_cast<std::int32_t>(milliseconds);
    if (dh2_timeline_jump(&timeline_, at) != 0 ||
        !objects::sample(resource_, at, error)) {
        if (error.empty()) error = "DestructibleContainer SetCurrentFrame rejected";
        return false;
    }
    event_cursor_ = {};
    completion_sent_ = false;
    error.clear();
    return true;
}

bool Owner::play_named(const char* name, bool loop, EventCallback event,
                       CompletionCallback completion, void* context,
                       std::string& error) noexcept {
    error.clear();
    if (!loaded_ || !name || !*name) {
        error = "DestructibleContainer animation request is incomplete";
        return false;
    }
    const Clip* clip = find_clip(name);
    if (!clip) {
        error = std::string("DestructibleContainer BDAE animation is missing: ") + name;
        return false;
    }
    if (dh2_timeline_clip(&timeline_, static_cast<std::int32_t>(clip - clips_.data()),
                         clip->start_ms, clip->end_ms) != 0 ||
        dh2_timeline_loop(&timeline_, loop ? 1u : 0u) != 0) {
        error = "DestructibleContainer timeline rejected the selected BDAE clip";
        return false;
    }
    active_clip_ = clip->name;
    event_cursor_ = {};
    callback_context_ = context;
    event_callback_ = event;
    completion_callback_ = completion;
    looping_ = loop;
    completion_sent_ = false;
    dispatch_ok_ = true;
    dispatch_error_.clear();
    if (!objects::sample(resource_, clip->start_ms, error)) {
        if (error.empty()) error = "DestructibleContainer selected BDAE pose could not be sampled";
        return false;
    }
    error.clear();
    return true;
}

bool Owner::play_activate(EventCallback event, CompletionCallback completion,
                          void* context, std::string& error) noexcept {
    return play_named("activate", false, event, completion, context, error);
}

void Owner::timeline_ended(void* raw, timeline::State*) {
    auto& self = *static_cast<Owner*>(raw);
    if (self.completion_sent_) return;
    self.completion_sent_ = true;
    if (!self.completion_callback_) return;
    self.dispatch_ok_ = self.completion_callback_(self.callback_context_,
                                                  self.dispatch_error_);
}

void Owner::dispatch_event(const animation::TriggeredEvent* event, void* raw) {
    auto& self = *static_cast<Owner*>(raw);
    if (!event || !event->name || !self.event_callback_ || !self.dispatch_ok_) return;
    self.dispatch_ok_ = self.event_callback_(self.callback_context_, event->name,
                                             event->lag_ms, self.dispatch_error_);
}

bool Owner::update(std::uint32_t absolute_ms, std::string& error) noexcept {
    error.clear();
    if (!loaded_ || active_clip_.empty()) {
        error = "DestructibleContainer animation update has no live clip";
        return false;
    }
    const auto previous = timeline_.current_ms;
    const auto event_start = timeline_.start_ms;
    const auto event_end = timeline_.end_ms;
    std::int32_t source_time = 0;
    std::memcpy(&source_time, &absolute_ms, sizeof(source_time));
    dispatch_ok_ = true;
    dispatch_error_.clear();
    // Keep the selected clip interval stable through event dispatch. The
    // source Animator's named event callback (for example `opened`) belongs
    // to the clip update; its separate timeline-end callback may synchronously
    // change state and select idleactive.
    const timeline::Services no_end_callback{nullptr, nullptr};
    if (dh2_timeline_update(&timeline_, source_time, &no_end_callback) != 0) {
        error = "DestructibleContainer timeline update rejected";
        return false;
    }
    if (!dispatch_ok_) {
        error = dispatch_error_.empty() ?
            "DestructibleContainer source completion callback failed" : dispatch_error_;
        return false;
    }
    if (event_callback_ && !active_clip_.empty() &&
        !dh2_events_update(&resource_.animation.events.view(), &event_cursor_,
            previous, timeline_.current_ms, event_start, event_end,
            dispatch_event, this)) {
        error = "DestructibleContainer BDAE event track update rejected";
        return false;
    }
    if (timeline_.ended && !completion_sent_) timeline_ended(this, &timeline_);
    if (!dispatch_ok_) {
        error = dispatch_error_.empty() ?
            "DestructibleContainer source completion callback failed" : dispatch_error_;
        return false;
    }
    if (!objects::sample(resource_, timeline_.current_ms, error)) {
        if (error.empty()) error = "DestructibleContainer BDAE pose sampling failed";
        return false;
    }
    error.clear();
    return true;
}

bool Owner::detach() noexcept {
    loaded_ = false;
    controller_present_ = false;
    clips_.clear();
    resource_ = {};
    timeline_ = {};
    event_cursor_ = {};
    callback_context_ = nullptr;
    event_callback_ = nullptr;
    completion_callback_ = nullptr;
    active_clip_.clear();
    looping_ = false;
    completion_sent_ = false;
    dispatch_ok_ = true;
    dispatch_error_.clear();
    return true;
}

} // namespace dh2::destructible_container_visual_owner_v1
