#pragma once
#include "actor_definitions.hpp"
#include "original_campaign_runtime.hpp"
#include <optional>
namespace dh::foundation::persistence::death_flow {
// Caller borrows immutable source-initial admission and resolved source AI row.
// No HP, current visibility, current death or generic Character inference.
inline std::optional<bool> authenticated_ordinary_legacy_admission_v1(
 const ActorDefinition& origin, const std::string& saved_definition,
 std::uint32_t saved_state, bool saved_dead,
 std::optional<bool> source_initial_admission,
 std::optional<bool> resolved_source_ai_is_monster,
 const std::vector<OriginalCampaignScript>* complete_loaded_banks) {
 if(!origin.stableId||origin.sourceId.empty()||origin.sourcePath.empty()||origin.moduleName.empty()||origin.name.empty()||
    origin.sourceId!=saved_definition||!source_initial_admission||!*source_initial_admission||
    !resolved_source_ai_is_monster||!*resolved_source_ai_is_monster||!complete_loaded_banks||
    origin.gametype!="Character"||saved_state!=3||saved_dead)return std::nullopt;
 const auto value=[&](const char* key)->std::string {const auto p=origin.properties.find(key);return p==origin.properties.end()?std::string{}:p->second;};
 if(value("_templateName")!="MonsterCommonType1"||value("char_template_pydata")!="Charater_Templates"||
    value("char_template").empty()||value("char_group_role")!="Normal"||value("ai_state")!="Idle"||
    !value("char_group").empty()||value("spawn_prob")!="100")return std::nullopt;
 for(const char* key:{"activate_cond","deactivate_cond","min_difficulty","spawn_cond","respawn_cond"})
    if(!value(key).empty())return std::nullopt;
 for(const char* key:{"is_tutorial_only","is_demo_only","static"})
    if(!value(key).empty()&&value(key)!="0"&&value(key)!="false")return std::nullopt;
 for(const char* key:{"visible","auto_spawn"})
    if(!value(key).empty()&&value(key)!="1"&&value(key)!="true")return std::nullopt;
 // Conservatively exclude every authored command reference, not only Hide.
 // Module writers require a separate origin audit; unknown remains unknown.
 for(const auto& script:*complete_loaded_banks)for(const auto& command:script.commands) {
    if(command.class_name.find("Module")!=std::string::npos)return std::nullopt;
    for(const auto& field:command.strings)
      if(!origin.name.empty()&&field.second.find(origin.name)!=std::string::npos)return std::nullopt;
 }
 return *source_initial_admission;
}
}
