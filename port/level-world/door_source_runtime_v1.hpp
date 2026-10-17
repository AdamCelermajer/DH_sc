#pragma once

#include "object_manager_runtime_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::door_source_runtime_v1 {

enum class Status : std::uint8_t {
    complete, no_change, invalid_state, provider_unavailable, provider_failed
};

struct Services {
    void* context{};
    bool (*play_animation)(void*,std::uintptr_t,const char*,std::string&){};
    bool (*play_idle)(void*,std::uintptr_t,const char*,bool,
                      std::string&){};
    bool (*set_physical_filter)(void*,std::uintptr_t,bool,std::string&){};
    bool (*flag_floor_dead_end)(void*,std::uintptr_t,bool,std::string&){};
    bool (*play_sound)(void*,std::uintptr_t,bool,std::string&){};
};

// Source Door::Open/Close/Opened/Closed portable transitions. Mutable state
// lives on the same canonical ObjectManager row; rendering, audio and physics
// borrow their existing source owners through Services.
Status open(object_manager_runtime_owner_v1::Owner&,std::uintptr_t identity,
            bool force,bool has_visual,bool interactive,
            bool property_750,bool property_752,const Services&,
            std::string& error);
Status close(object_manager_runtime_owner_v1::Owner&,std::uintptr_t identity,
             bool force,bool has_visual,bool interactive,
             bool property_750,bool property_752,const Services&,
             std::string& error);
Status opened(object_manager_runtime_owner_v1::Owner&,std::uintptr_t identity,
              bool event_call,const Services&,std::string& error);
Status closed(object_manager_runtime_owner_v1::Owner&,std::uintptr_t identity,
              bool event_call,const Services&,std::string& error);
// Source Door::Enabled/Disabled tail, called only after the ordered generic
// condition evaluator changed +0x8a on this same ObjectManager row.
Status set_enabled(object_manager_runtime_owner_v1::Owner&,
                   std::uintptr_t identity,bool enabled,const Services&,
                   std::string& error);

} // namespace dh2::door_source_runtime_v1
