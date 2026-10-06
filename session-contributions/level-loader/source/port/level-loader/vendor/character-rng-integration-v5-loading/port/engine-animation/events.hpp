#pragma once
#include "../engine-resources/resources.hpp"
#include <string>
#include <vector>

namespace dh2::animation {
// New native ABI. Serialized pointer fields remain immutable 32-bit offsets.
struct EventGroup {std::uint32_t count;const char* const* names;};
struct EventView {
    std::uint32_t type,count;
    const std::uint8_t* times;
    const EventGroup* groups;
};
struct TriggeredEvent {std::int32_t lag_ms;const char* name;};
using EventCallback=void(*)(const TriggeredEvent*,void*);
struct EventCursor {std::int32_t last_entry=-1;};
class EventTrack {
    std::vector<std::uint8_t> bytes;
    std::vector<std::vector<const char*>> names;
    std::vector<EventGroup> groups;
    EventView track{};
public:
    EventTrack()=default;
    EventTrack(const EventTrack&)=delete;
    EventTrack& operator=(const EventTrack&)=delete;
    EventTrack(EventTrack&&) noexcept=default;
    EventTrack& operator=(EventTrack&&) noexcept=default;
    bool load(const resources::BresView&,std::string& error);
    const EventView& view()const{return track;}
};
}
extern "C" {
bool dh2_events_validate(const dh2::animation::EventView*);
std::int32_t dh2_events_find(const dh2::animation::EventView*,std::int32_t milliseconds);
std::int32_t dh2_events_time(const dh2::animation::EventView*,const char* name);
// Callback may observe the immutable track; mutation/reentrant dispatch is not
// supported by this port interface. No engine object ABI or refcount is reused.
bool dh2_events_update(const dh2::animation::EventView*,dh2::animation::EventCursor*,
    std::int32_t previous,std::int32_t current,std::int32_t start,std::int32_t end,
    dh2::animation::EventCallback,void* user);
bool dh2_events_update_interval(const dh2::animation::EventView*,std::int32_t previous,
    std::int32_t current,dh2::animation::EventCallback,void* user);
}
