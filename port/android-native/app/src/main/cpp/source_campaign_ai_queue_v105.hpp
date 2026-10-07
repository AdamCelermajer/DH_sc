#pragma once
#include <memory>
#include <string>
#include <cstdint>
namespace model_renderer {
bool source_campaign_ai_inc_queue_v105(const std::shared_ptr<void>&,std::string&);
bool source_campaign_ai_is_my_turn_v105(const std::shared_ptr<void>&,std::uintptr_t ai,bool&,std::string&);
}
