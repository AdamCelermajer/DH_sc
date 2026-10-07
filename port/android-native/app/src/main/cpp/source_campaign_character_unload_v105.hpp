#pragma once
#include <memory>
#include <cstdint>
#include <string>
namespace model_renderer {
bool source_campaign_character_unload_script_v105(const std::shared_ptr<void>&,
 std::uintptr_t,bool final,std::string&);
//AI body only, after the concurrent-AI queue kernel has already erased its
//captured node. Never replay UnLoadScriptProcess's map prefix from this loan.
bool source_campaign_character_unload_ai_v108(const std::shared_ptr<void>&,
 std::uintptr_t,bool final,std::string&);
}
