#pragma once
#include "openable_container_owner_v1.hpp"
namespace dh2::world {
struct OpenableContainerQuestEventV2 {
    std::int32_t objective_type{},room{},source_index=-1,data_id=-1;
    std::uintptr_t actor{};
    bool byte18=false,byte19=false;
};
struct OpenableContainerInteractionServicesV2 {
    std::shared_ptr<void> owner;
    std::function<bool(std::uintptr_t&,std::string&)> current_level;
    std::function<bool(std::string&)> assert_missing_level;
    std::function<bool(bool&,std::string&)> local_player_hosting;
    std::function<bool(std::int32_t&,std::string&)> room64;
    std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
    std::function<bool(std::uintptr_t,const OpenableContainerQuestEventV2&,std::string&)> raise_async;
    std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> handle_as_character;
    std::function<bool(std::uintptr_t,bool&,std::string&)> is_player,is_local_player;
    std::function<bool(std::uintptr_t,std::int32_t,std::int32_t,std::string&)> props_add_int;
    std::function<bool(std::uintptr_t,std::int32_t,bool,std::int32_t&,std::string&)> props_get_int;
    std::function<bool(const char*,std::int32_t&,std::string&)> trophy_name_index;
    std::function<bool(std::int32_t,std::string&)> unlock_trophy;
};
// Whole source derived Interact3a1904. Actual base fields/owners remain borrowed.
bool openable_container_interact_v2(OpenableContainerOwnerV1&,
    OpenableContainerFieldsV1&,const OpenableContainerInteractionServicesV2&,
    std::uintptr_t actor,std::string&);
}
