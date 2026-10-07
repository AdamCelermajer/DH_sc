#pragma once
#include <memory>
#include <cstdint>
#include <string>
#include <functional>
#include <character_can_update.hpp>
namespace model_renderer {
bool source_campaign_character_can_update_v106(const std::shared_ptr<void>&,
 std::uintptr_t,bool&,std::string&);
//Scoped actual projection used by whole queued Character.Update prefix. It
//does not call CanUpdate itself and the callback cannot retain either loan.
bool source_campaign_character_with_eligibility_v108(const std::shared_ptr<void>&,
 std::uintptr_t,const std::function<bool(dh2::character::CanUpdateOwner40&,
 const dh2::character::CanUpdateServices24&,std::string&)>&,std::string&);
}
