#pragma once
#include <memory>
#include <cstdint>
#include <string>
namespace model_renderer {
//Whole pre-controller queued prefix, then genuine controller and source bot
//prefix. accepted=false is original CanUpdate rejection, not a frame failure.
bool source_campaign_character_start_v108(const std::shared_ptr<void>&,
 std::uintptr_t,bool& accepted,std::string&);
}
