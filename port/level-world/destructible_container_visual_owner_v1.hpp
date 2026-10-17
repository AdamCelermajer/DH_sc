#pragma once

#include "objects.hpp"
#include "../engine-animation/events.hpp"
#include "visual_timeline.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::destructible_container_visual_owner_v1 {

struct Clip {
    std::string name;
    std::int32_t start_ms{};
    std::int32_t end_ms{};
};

using EventCallback = bool (*)(void*, const char* name, std::int32_t lag_ms,
                               std::string& error);
using CompletionCallback = bool (*)(void*, std::string& error);

// Retained BDAE VisualObject state for one source Container. The renderer owns
// GPU draw buffers separately and borrows resource() while this Owner lives.
// This Owner has one root scene Animator; clip rows are registrations on that
// animator, not separate animator instances.
class Owner {
    objects::Resource resource_;
    std::vector<Clip> clips_;
    timeline::State timeline_{};
    animation::EventCursor event_cursor_{};
    void* callback_context_{};
    EventCallback event_callback_{};
    CompletionCallback completion_callback_{};
    std::string active_clip_;
    bool loaded_{};
    bool controller_present_{};
    bool looping_{};
    bool completion_sent_{};
    bool dispatch_ok_{true};
    std::string dispatch_error_;

    static void timeline_ended(void*, timeline::State*);
    static void dispatch_event(const animation::TriggeredEvent*, void*);
    const Clip* find_clip(const std::string&) const noexcept;

public:
    Owner() noexcept;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) noexcept = default;
    Owner& operator=(Owner&&) noexcept = default;

    bool load(const std::uint8_t* bdae, std::size_t size,
              std::string& error) noexcept;
    bool controller_present() const noexcept { return controller_present_; }
    bool has_clip(const char* name) const noexcept { return name && find_clip(name); }
    bool clip_active() const noexcept { return loaded_ && !active_clip_.empty(); }
    std::uint32_t root_animator_count() const noexcept {
        return controller_present_ ? 1u : 0u;
    }
    const objects::Resource& resource() const noexcept { return resource_; }
    objects::Resource& resource() noexcept { return resource_; }
    const std::vector<Clip>& clips() const noexcept { return clips_; }
    const std::string& active_clip() const noexcept { return active_clip_; }
    std::int32_t current_ms() const noexcept { return timeline_.current_ms; }
    std::int32_t start_ms() const noexcept { return timeline_.start_ms; }
    std::int32_t end_ms() const noexcept { return timeline_.end_ms; }

    bool start_initial_state(std::string& error) noexcept;
    bool start_initial_state(EventCallback, CompletionCallback, void* context,
                             std::string& error) noexcept;
    // Container::SetState mutates root ISceneNode +0x284 before its caller
    // selects any follow-up animation.
    bool set_container_state(std::uint8_t state,
                             std::string& error) noexcept;
    bool set_current_frame(std::int32_t frame, std::string& error) noexcept;
    bool play_named(const char* name, bool loop, EventCallback,
                    CompletionCallback, void* context,
                    std::string& error) noexcept;
    bool play_activate(EventCallback, CompletionCallback, void* context,
                       std::string& error) noexcept;
    // Uses the existing absolute scene timestamp (milliseconds), matching the
    // native frame phase and CTimelineController's clock semantics.
    bool update(std::uint32_t absolute_ms, std::string& error) noexcept;
    bool detach() noexcept;
};

} // namespace dh2::destructible_container_visual_owner_v1
